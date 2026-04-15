#include "EQAutomationTrack.h"
#include <algorithm>
#include <cmath>

// ── Audio thread ───────────────────────────────────────────────────────────────

void EQAutomationTrack::prepareToPlay (double sr)
{
    sampleRate_ = sr;
}

void EQAutomationTrack::advance (int numSamples, ParametricEQ* eq)
{
    // Increment recording clock
    if (recording.load())
        recordSamplePos.fetch_add (numSamples);

    if (!playing.load()) return;

    int64_t loopLen = loopDurationSamples.load();
    if (loopLen <= 0) return;

    // Allow UI thread to reset playback position (e.g. on PLAY toggle)
    if (playbackResetPending.exchange (false))
        playbackSamplePos = 0.0;

    playbackSamplePos += numSamples;
    while (playbackSamplePos >= (double) loopLen)
        playbackSamplePos -= (double) loopLen;

    float posSec = (float) (playbackSamplePos / sampleRate_);
    playbackPosSec.store (posSec);

    const juce::SpinLock::ScopedTryLockType tryLock (lock);
    if (!tryLock.isLocked()) return;

    for (int i = 0; i < 8; ++i)
    {
        float freq = 0.0f, gain = 0.0f;
        if (interpolate (i, posSec, freq, gain))
        {
            eq->setBandFrequency (i, freq);
            eq->setBandGain      (i, gain);
        }
    }
}

// ── UI thread ──────────────────────────────────────────────────────────────────

void EQAutomationTrack::startRecording()
{
    {
        const juce::SpinLock::ScopedLockType sl (lock);
        for (auto& v : events) v.clear();
    }
    recordSamplePos.store (0);
    loopDurationSamples.store (0);
    recording.store (true);
}

void EQAutomationTrack::stopRecording()
{
    loopDurationSamples.store (recordSamplePos.load());
    recording.store (false);
}

void EQAutomationTrack::recordEvent (int band, float freq, float gainDb)
{
    if (!recording.load()) return;
    if (band < 0 || band >= 8) return;

    float timeSec = (float) ((double) recordSamplePos.load() / sampleRate_);

    // 20 ms dedup window — prevents accumulating identical points per UI frame
    static constexpr float kWindow = 0.020f;

    const juce::SpinLock::ScopedLockType sl (lock);
    auto& v = events[(size_t) band];

    v.erase (std::remove_if (v.begin(), v.end(),
                             [timeSec] (const Event& e) {
                                 return std::abs (e.timeSec - timeSec) < kWindow;
                             }),
             v.end());

    auto it = std::lower_bound (v.begin(), v.end(), timeSec,
                                [] (const Event& e, float t) { return e.timeSec < t; });
    v.insert (it, { timeSec, freq, gainDb });
}

void EQAutomationTrack::clearAll()
{
    {
        const juce::SpinLock::ScopedLockType sl (lock);
        for (auto& v : events) v.clear();
    }
    loopDurationSamples.store (0);
    recordSamplePos.store (0);
    playbackResetPending.store (true);
    playbackPosSec.store (0.0f);
}

void EQAutomationTrack::setPlaying (bool b)
{
    if (b) playbackResetPending.store (true);
    playing.store (b);
}

bool EQAutomationTrack::hasData() const
{
    const juce::SpinLock::ScopedLockType sl (lock);
    for (const auto& v : events)
        if (!v.empty()) return true;
    return false;
}

float EQAutomationTrack::getLoopDurationSec() const
{
    int64_t s = loopDurationSamples.load();
    if (s <= 0) return 0.0f;
    return (float) ((double) s / sampleRate_);
}

// ── Audio thread helper ────────────────────────────────────────────────────────

bool EQAutomationTrack::interpolate (int band, float timeSec,
                                      float& outFreq, float& outGain) const
{
    const auto& v = events[(size_t) band];
    if (v.empty()) return false;

    if (v.size() == 1) { outFreq = v[0].freq; outGain = v[0].gainDb; return true; }

    if (timeSec <= v.front().timeSec)
        { outFreq = v.front().freq; outGain = v.front().gainDb; return true; }
    if (timeSec >= v.back().timeSec)
        { outFreq = v.back().freq;  outGain = v.back().gainDb;  return true; }

    for (int i = 0; i + 1 < (int) v.size(); ++i)
    {
        if (timeSec >= v[i].timeSec && timeSec < v[(size_t)(i + 1)].timeSec)
        {
            float span = v[(size_t)(i + 1)].timeSec - v[i].timeSec;
            float t    = (timeSec - v[i].timeSec) / span;

            float logF0 = std::log10 (juce::jmax (20.0f, v[i].freq));
            float logF1 = std::log10 (juce::jmax (20.0f, v[(size_t)(i + 1)].freq));
            outFreq = std::pow (10.0f, logF0 + t * (logF1 - logF0));
            outGain = v[i].gainDb + t * (v[(size_t)(i + 1)].gainDb - v[i].gainDb);
            return true;
        }
    }
    return false;
}

// ── Preset persistence ─────────────────────────────────────────────────────────

juce::String EQAutomationTrack::getStateAsString() const
{
    const juce::SpinLock::ScopedLockType sl (lock);

    int64_t loopSamples = loopDurationSamples.load();
    float durationSec   = loopSamples > 0 ? (float) ((double) loopSamples / sampleRate_) : 0.0f;

    juce::String s = juce::String (durationSec, 4);

    for (int b = 0; b < 8; ++b)
    {
        s += ";";
        const auto& v = events[(size_t) b];
        for (int i = 0; i < (int) v.size(); ++i)
        {
            if (i > 0) s += "|";
            s += juce::String (v[(size_t)i].timeSec, 4) + ","
               + juce::String (v[(size_t)i].freq,    2) + ","
               + juce::String (v[(size_t)i].gainDb,  3);
        }
    }
    return s;
}

void EQAutomationTrack::loadStateFromString (const juce::String& s)
{
    if (s.trim().isEmpty()) return;

    juce::StringArray tokens;
    tokens.addTokens (s.trim(), ";", "");
    if (tokens.isEmpty()) return;

    const juce::SpinLock::ScopedLockType sl (lock);
    for (auto& v : events) v.clear();

    float durationSec = tokens[0].getFloatValue();
    loopDurationSamples.store ((int64_t) (durationSec * sampleRate_));

    for (int b = 0; b < juce::jmin (8, tokens.size() - 1); ++b)
    {
        if (tokens[b + 1].trim().isEmpty()) continue;

        juce::StringArray evs;
        evs.addTokens (tokens[b + 1].trim(), "|", "");

        for (const auto& ev : evs)
        {
            juce::StringArray vals;
            vals.addTokens (ev.trim(), ",", "");
            if (vals.size() < 3) continue;
            events[(size_t) b].push_back ({ vals[0].getFloatValue(),
                                            vals[1].getFloatValue(),
                                            vals[2].getFloatValue() });
        }
    }
}
