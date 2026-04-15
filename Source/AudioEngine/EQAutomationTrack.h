#pragma once
#include <JuceHeader.h>
#include "ParametricEQ.h"
#include <array>
#include <vector>
#include <atomic>
#include <cstdint>

/**
 * Records and plays back per-band EQ (freq + gain) automation.
 *
 * Usage:
 *   Press REC  → drag EQ handles for as long as you like → press REC again.
 *   The elapsed time becomes the loop length.
 *   Press PLAY → the recorded motion loops independently of the step sequencer.
 *
 * Thread safety:
 *   UI thread   : startRecording / stopRecording / recordEvent / clearAll / setPlaying
 *   Audio thread: prepareToPlay / advance()
 *   A SpinLock protects event lists; the audio thread uses a try-lock so it never blocks.
 */
class EQAutomationTrack
{
public:
    struct Event
    {
        float timeSec = 0.0f;   // seconds from the start of the recording
        float freq    = 1000.0f;
        float gainDb  = 0.0f;
    };

    // ── Audio thread ──────────────────────────────────────────────────────────

    /** Call once after the sample rate is known. */
    void prepareToPlay (double sampleRate);

    /** Advance playback / recording clocks and apply automation to eq. */
    void advance (int numSamples, ParametricEQ* eq);

    // ── UI thread ─────────────────────────────────────────────────────────────

    /** Clear all events, reset clocks, begin a fresh recording. */
    void startRecording();

    /** Finish recording; elapsed duration becomes the loop length. */
    void stopRecording();

    /** Record the current EQ state for one band at the current recording clock.
     *  Call from the UI thread during a mouse-drag on an EQ handle. */
    void recordEvent (int band, float freq, float gainDb);

    /** Erase everything and stop playback. */
    void clearAll();

    /** Start or stop looped playback; resets playback position when starting. */
    void setPlaying (bool b);

    bool  isRecording()        const { return recording.load(); }
    bool  isPlaying()          const { return playing.load(); }
    bool  hasData()            const;
    float getLoopDurationSec() const;
    float getPlaybackPosSec()  const { return playbackPosSec.load(); }

    // ── Preset persistence ────────────────────────────────────────────────────

    juce::String getStateAsString()            const;
    void         loadStateFromString (const juce::String& s);

private:
    std::array<std::vector<Event>, 8> events;
    mutable juce::SpinLock            lock;

    std::atomic<bool>    recording            { false };
    std::atomic<bool>    playing              { false };
    std::atomic<bool>    playbackResetPending { false };
    std::atomic<int64_t> recordSamplePos      { 0 };
    std::atomic<int64_t> loopDurationSamples  { 0 };
    std::atomic<float>   playbackPosSec       { 0.0f };

    double playbackSamplePos = 0.0;
    double sampleRate_       = 44100.0;

    bool interpolate (int band, float timeSec,
                      float& outFreq, float& outGain) const;
};
