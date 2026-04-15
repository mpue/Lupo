#include "EQAutomationTrack.h"
#include <algorithm>

// ── UI thread ─────────────────────────────────────────────────────────────────

void EQAutomationTrack::startRecording()
{
    // Do NOT clear existing events: subsequent REC passes overdub over old data.
    // Use clearAll() to erase everything intentionally.
    recording.store (true);
}

void EQAutomationTrack::stopRecording()
{
    recording.store (false);
}

void EQAutomationTrack::recordEvent (int band, float loopPos, float freq, float gainDb)
{
    if (!recording.load()) return;
    if (band < 0 || band >= 8) return;

    // Replace window: ~1/8 of a step for 16-step sequences at typical resolutions.
    // Wide enough to prevent duplicate points accumulating; narrow enough to preserve detail.
    static constexpr float kWindow = 0.01f;

    const juce::SpinLock::ScopedLockType sl (lock);
    auto& v = events[(size_t)band];

    // Erase any existing events within ±kWindow of this position (overdub/replace)
    v.erase (std::remove_if (v.begin(), v.end(),
                             [loopPos](const Event& e) {
                                 return std::abs (e.loopPos - loopPos) < kWindow;
                             }),
             v.end());

    // Insert new event in sorted loopPos order
    auto it = std::lower_bound (v.begin(), v.end(), loopPos,
                                [](const Event& e, float pos) { return e.loopPos < pos; });
    v.insert (it, { loopPos, freq, gainDb });
}

void EQAutomationTrack::clearAll()
{
    const juce::SpinLock::ScopedLockType sl (lock);
    for (auto& v : events) v.clear();
}

bool EQAutomationTrack::hasData() const
{
    const juce::SpinLock::ScopedLockType sl (lock);
    for (const auto& v : events)
        if (!v.empty()) return true;
    return false;
}

// ── Audio thread ──────────────────────────────────────────────────────────────

bool EQAutomationTrack::interpolate (int band, float loopPos,
                                      float& outFreq, float& outGain) const
{
    // Called with lock held.
    const auto& v = events[(size_t)band];
    if (v.empty()) return false;

    if (v.size() == 1)
    {
        outFreq = v[0].freq;
        outGain = v[0].gainDb;
        return true;
    }

    // Hold first / last value at the edges
    if (loopPos <= v.front().loopPos)
    {
        outFreq = v.front().freq;
        outGain = v.front().gainDb;
        return true;
    }
    if (loopPos >= v.back().loopPos)
    {
        outFreq = v.back().freq;
        outGain = v.back().gainDb;
        return true;
    }

    // Linear search for surrounding events (event count is small, ~100s at most)
    for (int i = 0; i + 1 < (int)v.size(); ++i)
    {
        if (loopPos >= v[i].loopPos && loopPos < v[(size_t)(i + 1)].loopPos)
        {
            float t = (loopPos - v[i].loopPos) / (v[(size_t)(i + 1)].loopPos - v[i].loopPos);

            // Interpolate frequency in log space for perceptually linear motion
            float logF0 = std::log10 (juce::jmax (20.0f, v[i].freq));
            float logF1 = std::log10 (juce::jmax (20.0f, v[(size_t)(i + 1)].freq));
            outFreq = std::pow (10.0f, logF0 + t * (logF1 - logF0));

            // Interpolate gain linearly
            outGain = v[i].gainDb + t * (v[(size_t)(i + 1)].gainDb - v[i].gainDb);
            return true;
        }
    }
    return false;
}

void EQAutomationTrack::apply (float loopPos, ParametricEQ* eq)
{
    if (!playing.load()) return;

    const juce::SpinLock::ScopedTryLockType tryLock (lock);
    if (!tryLock.isLocked()) return;    // audio thread must never block

    for (int i = 0; i < 8; ++i)
    {
        float freq = 0.0f, gain = 0.0f;
        if (interpolate (i, loopPos, freq, gain))
        {
            eq->setBandFrequency (i, freq);
            eq->setBandGain      (i, gain);
        }
    }
}

// ── Preset persistence ────────────────────────────────────────────────────────

juce::String EQAutomationTrack::getStateAsString() const
{
    const juce::SpinLock::ScopedLockType sl (lock);
    juce::String s;
    for (int b = 0; b < 8; ++b)
    {
        if (b > 0) s += ";";
        const auto& v = events[(size_t)b];
        for (int i = 0; i < (int)v.size(); ++i)
        {
            if (i > 0) s += "|";
            s += juce::String (v[(size_t)i].loopPos, 4) + ","
               + juce::String (v[(size_t)i].freq,    2) + ","
               + juce::String (v[(size_t)i].gainDb,  3);
        }
    }
    return s;
}

void EQAutomationTrack::loadStateFromString (const juce::String& s)
{
    if (s.trim().isEmpty()) return;

    const juce::SpinLock::ScopedLockType sl (lock);
    for (auto& v : events) v.clear();

    juce::StringArray bands;
    bands.addTokens (s.trim(), ";", "");

    for (int b = 0; b < juce::jmin (8, bands.size()); ++b)
    {
        if (bands[b].trim().isEmpty()) continue;

        juce::StringArray evs;
        evs.addTokens (bands[b].trim(), "|", "");

        for (const auto& ev : evs)
        {
            juce::StringArray vals;
            vals.addTokens (ev.trim(), ",", "");
            if (vals.size() < 3) continue;

            events[(size_t)b].push_back ({
                vals[0].getFloatValue(),
                vals[1].getFloatValue(),
                vals[2].getFloatValue()
            });
        }
    }
}
