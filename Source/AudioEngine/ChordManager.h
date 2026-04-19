#pragma once
#include <JuceHeader.h>
#include <array>
#include <vector>
#include <atomic>

/**
 * Two chord modes:
 *
 *  Manual  – capture a chord via startCapture() + keyboard; replay those
 *            exact intervals on every incoming note.
 *
 *  Auto    – diatonic harmonisation: for every incoming note, look up which
 *            triad fits harmonically in the current key/scale and play that.
 *            Scales: Major, Natural Minor, Dorian, Phrygian, Lydian, Mixolydian.
 *            Chromatic notes fall back to the nearest lower scale degree.
 *
 * Thread safety:
 *   UI thread   : setEnabled / setAutoChord / setKey / setScale /
 *                 startCapture / isCapturing / getIntervals
 *   Audio thread: captureFromMidi / expandMidi
 */
class ChordManager
{
public:
    static constexpr int MAX_INTERVALS = 12;

    enum class Scale { Major = 0, Minor, Dorian, Phrygian, Lydian, Mixolydian, NUM_SCALES };

    ChordManager();

    // ── UI thread ─────────────────────────────────────────────────────────────

    void setEnabled   (bool b)    { enabled.store (b); }
    bool isEnabled()        const { return enabled.load(); }

    /** Toggle auto (diatonic) chord mode.  When on, manual intervals are ignored. */
    void setAutoChord (bool b)    { autoChord.store (b); }
    bool isAutoChord()      const { return autoChord.load(); }

    /** Root key: 0 = C, 1 = C#/Db … 11 = B */
    void setKey   (int k)         { key.store (juce::jlimit (0, 11, k)); }
    int  getKey()           const { return key.load(); }

    void setScale (Scale s)       { scale.store ((int) s); }
    Scale getScale()        const { return (Scale) scale.load(); }

    /** Arms manual capture.  The next chord played becomes the template. */
    void startCapture();
    bool isCapturing()      const { return capturing.load(); }

    /** Returns a copy of the current manual interval list. */
    std::vector<int> getIntervals() const;

    /** Directly set the manual interval list (e.g. when loading a preset). */
    void setIntervals(const std::vector<int>& iv);

    /** Serialise the full manager state for preset save. */
    juce::String getStateAsString() const;

    /** Restore state from a string produced by getStateAsString(). */
    void loadStateFromString(const juce::String& s);

    /** True if midiNote is a diatonic member of the current key/scale. */
    bool isNoteInScale (int midiNote) const;

    /** Returns the nearest scale note in the given direction (+1 up / -1 down).
     *  Falls back to midiNote itself if no scale note found within an octave. */
    int nextScaleNote (int midiNote, int direction) const;

    // ── Audio thread ──────────────────────────────────────────────────────────

    /** Scan raw MIDI (before seq/arp) for manual chord capture.  Does NOT modify buffer. */
    void captureFromMidi (const juce::MidiBuffer& midi);

    /** Expand every note-on/off with chord intervals.  No-op when disabled. */
    void expandMidi (juce::MidiBuffer& midi);

private:
    std::atomic<bool> enabled   { false };
    std::atomic<bool> autoChord { false };
    std::atomic<bool> capturing { false };
    std::atomic<int>  key       { 0 };    // 0=C … 11=B
    std::atomic<int>  scale     { 0 };    // Scale enum

    mutable juce::SpinLock intervalsLock;
    std::vector<int>        intervals { 0 };  // manual intervals (root = 0, sorted)

    // Capture state – audio thread only
    std::vector<int> captureHeldNotes;
    std::vector<int> captureAllNotes;

    // Active chord note tracking – audio thread only (fixed-size, no heap allocs in hot path)
    struct NoteSlot { int notes[MAX_INTERVALS] = {}; int count = 0; };
    std::array<NoteSlot, 128> activeNotes {};

    void finalizeCapture();

    /** Compute diatonic triad intervals for rootNote in current key/scale.
     *  Returns e.g. {0,4,7} (major), {0,3,7} (minor), {0,3,6} (diminished). */
    std::vector<int> getDiatonicIntervals (int rootNote) const;
};
