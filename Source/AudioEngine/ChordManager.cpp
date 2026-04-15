#include "ChordManager.h"
#include <algorithm>

// ── Scale tables ──────────────────────────────────────────────────────────────
// Each row: 7 semitone steps, then 7 chord types (0=maj 1=min 2=dim)
struct ScaleInfo {
    int steps[7];
    int chords[7];   // 0=major  1=minor  2=diminished
};

static const ScaleInfo kScales[] = {
    // Major:       I    ii   iii  IV   V    vi   vii°
    {{ 0,2,4,5,7,9,11 }, { 0,1,1,0,0,1,2 }},
    // Natural Minor: i   ii°  III  iv   v    VI   VII
    {{ 0,2,3,5,7,8,10 }, { 1,2,0,1,1,0,0 }},
    // Dorian:      i    ii   III  IV   v    vi°  VII
    {{ 0,2,3,5,7,9,10 }, { 1,1,0,0,1,2,0 }},
    // Phrygian:    i    II   III  iv   v°   VI   vii
    {{ 0,1,3,5,7,8,10 }, { 1,0,0,1,2,0,1 }},
    // Lydian:      I    II   iii  iv°  V    vi   vii
    {{ 0,2,4,6,7,9,11 }, { 0,0,1,2,0,1,1 }},
    // Mixolydian:  I    ii   iii° IV   v    vi   VII
    {{ 0,2,4,5,7,9,10 }, { 0,1,2,0,1,1,0 }},
};

// ── Constructor ───────────────────────────────────────────────────────────────

ChordManager::ChordManager()
{
    for (auto& s : activeNotes) s.count = 0;
}

// ── UI thread ──────────────────────────────────────────────────────────────────

void ChordManager::startCapture()
{
    captureHeldNotes.clear();
    captureAllNotes.clear();
    capturing.store (true);
}

std::vector<int> ChordManager::getIntervals() const
{
    const juce::SpinLock::ScopedTryLockType sl (intervalsLock);
    if (sl.isLocked()) return intervals;
    return {};
}

bool ChordManager::isNoteInScale (int midiNote) const
{
    int k = key.load();
    int s = juce::jlimit (0, (int) Scale::NUM_SCALES - 1, scale.load());
    int noteInKey = ((midiNote - k) % 12 + 12) % 12;
    for (int i = 0; i < 7; ++i)
        if (kScales[s].steps[i] == noteInKey) return true;
    return false;
}

int ChordManager::nextScaleNote (int midiNote, int direction) const
{
    int next = midiNote + direction;
    for (int i = 0; i < 12 && next >= 0 && next <= 127; ++i, next += direction)
        if (isNoteInScale (next)) return next;
    return midiNote;  // fallback: no scale note found
}

// ── Audio thread ───────────────────────────────────────────────────────────────

void ChordManager::captureFromMidi (const juce::MidiBuffer& midi)
{
    if (!capturing.load()) return;

    for (const auto& meta : midi)
    {
        auto msg = meta.getMessage();

        if (msg.isNoteOn())
        {
            int n = msg.getNoteNumber();
            if (std::find (captureHeldNotes.begin(), captureHeldNotes.end(), n) == captureHeldNotes.end())
                captureHeldNotes.push_back (n);
            if (std::find (captureAllNotes.begin(), captureAllNotes.end(), n) == captureAllNotes.end())
                captureAllNotes.push_back (n);
        }
        else if (msg.isNoteOff())
        {
            int n = msg.getNoteNumber();
            captureHeldNotes.erase (std::remove (captureHeldNotes.begin(), captureHeldNotes.end(), n),
                                    captureHeldNotes.end());
            if (captureHeldNotes.empty() && !captureAllNotes.empty())
                finalizeCapture();
        }
    }
}

void ChordManager::finalizeCapture()
{
    if (captureAllNotes.empty()) { capturing.store (false); return; }

    int root = *std::min_element (captureAllNotes.begin(), captureAllNotes.end());
    std::vector<int> newIntervals;
    for (int n : captureAllNotes)
    {
        int iv = n - root;
        if (std::find (newIntervals.begin(), newIntervals.end(), iv) == newIntervals.end())
            newIntervals.push_back (iv);
    }
    std::sort (newIntervals.begin(), newIntervals.end());

    { const juce::SpinLock::ScopedLockType sl (intervalsLock); intervals = std::move (newIntervals); }

    captureAllNotes.clear();
    captureHeldNotes.clear();
    capturing.store (false);
}

std::vector<int> ChordManager::getDiatonicIntervals (int rootNote) const
{
    int k = key.load();
    int s = juce::jlimit (0, (int) Scale::NUM_SCALES - 1, scale.load());
    const auto& si = kScales[s];

    int noteInKey = ((rootNote - k) % 12 + 12) % 12;

    // Find exact scale degree
    int degree = -1;
    for (int i = 0; i < 7; ++i)
        if (noteInKey == si.steps[i]) { degree = i; break; }

    // Chromatic note: nearest lower scale degree
    if (degree < 0)
    {
        int bestDiff = 12;
        for (int i = 0; i < 7; ++i)
        {
            int diff = ((noteInKey - si.steps[i]) % 12 + 12) % 12;
            if (diff < bestDiff) { bestDiff = diff; degree = i; }
        }
    }

    switch (si.chords[degree])
    {
        case 2:  return { 0, 3, 6 };  // diminished
        case 1:  return { 0, 3, 7 };  // minor
        default: return { 0, 4, 7 };  // major
    }
}

void ChordManager::expandMidi (juce::MidiBuffer& midi)
{
    if (!enabled.load()) return;

    bool useAuto = autoChord.load();

    // For manual mode, lock and check intervals have content
    juce::SpinLock::ScopedTryLockType manualLock (intervalsLock);
    if (!useAuto)
    {
        if (!manualLock.isLocked()) return;
        if (intervals.size() <= 1) return;
    }

    juce::MidiBuffer extras;

    for (const auto& meta : midi)
    {
        auto msg = meta.getMessage();
        int  pos = meta.samplePosition;

        if (msg.isNoteOn())
        {
            int root = msg.getNoteNumber();
            auto& slot = activeNotes[(size_t) root];
            slot.count = 0;

            const std::vector<int>& ivs = useAuto ? getDiatonicIntervals (root) : intervals;

            for (int iv : ivs)
            {
                if (iv == 0) continue;
                int note = juce::jlimit (0, 127, root + iv);
                extras.addEvent (juce::MidiMessage::noteOn (msg.getChannel(), note, msg.getVelocity()), pos);
                if (slot.count < MAX_INTERVALS)
                    slot.notes[slot.count++] = note;
            }
        }
        else if (msg.isNoteOff())
        {
            int root = msg.getNoteNumber();
            auto& slot = activeNotes[(size_t) root];
            for (int i = 0; i < slot.count; ++i)
                extras.addEvent (juce::MidiMessage::noteOff (msg.getChannel(), slot.notes[i], (juce::uint8) 0), pos);
            slot.count = 0;
        }
    }

    for (const auto& meta : extras)
        midi.addEvent (meta.getMessage(), meta.samplePosition);
}
