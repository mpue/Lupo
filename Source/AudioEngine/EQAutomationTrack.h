#pragma once
#include <JuceHeader.h>
#include "ParametricEQ.h"
#include <array>
#include <vector>
#include <atomic>

/**
 * Records and plays back per-band EQ (freq + gain) automation,
 * synchronised to the step-sequencer loop position (0.0 … 1.0).
 *
 * Thread safety:
 *  - UI thread  : startRecording / stopRecording / recordEvent / clearAll / setPlaying
 *  - Audio thread: apply()
 *  A SpinLock protects the event lists; the audio thread uses a try-lock
 *  so it never blocks the real-time thread.
 */
class EQAutomationTrack
{
public:
    struct Event
    {
        float loopPos = 0.0f;   // 0.0 … 1.0
        float freq    = 1000.0f;
        float gainDb  = 0.0f;
    };

    // ── UI thread ─────────────────────────────────────────────────────────────

    /** Clears all recorded events and starts recording mode. */
    void startRecording();

    /** Exits recording mode (events are kept). */
    void stopRecording();

    /** Record one EQ state snapshot for a band at the given loop position. */
    void recordEvent (int band, float loopPos, float freq, float gainDb);

    /** Erase all recorded events. */
    void clearAll();

    void setPlaying (bool b)   { playing.store (b); }

    bool isRecording() const   { return recording.load(); }
    bool isPlaying()   const   { return playing.load(); }
    bool hasData()     const;

    // ── Audio thread ──────────────────────────────────────────────────────────

    /** Apply interpolated automation values for the given loop position to eq.
     *  Only touches bands that have recorded events.  No-op if not playing. */
    void apply (float loopPos, ParametricEQ* eq);

    // ── Preset persistence ────────────────────────────────────────────────────

    juce::String getStateAsString() const;
    void loadStateFromString (const juce::String& s);

private:
    std::array<std::vector<Event>, 8> events;
    mutable juce::SpinLock            lock;
    std::atomic<bool>                 recording { false };
    std::atomic<bool>                 playing   { false };

    /** Linear interpolation between recorded events (lock must already be held). */
    bool interpolate (int band, float loopPos,
                      float& outFreq, float& outGain) const;
};
