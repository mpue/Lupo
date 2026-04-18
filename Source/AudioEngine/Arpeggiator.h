#pragma once
#include <JuceHeader.h>

class Arpeggiator
{

public:

    Arpeggiator();
    ~Arpeggiator();

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

    enum class Mode { Up, Down, Random, Chord };
    enum class ClockMode { Internal, Midi };

    void setEnabled(bool shouldRun)  noexcept { enabled = shouldRun; }
    void setOctaves(int num)         noexcept { octaves = juce::jmax(1, num); }
    void setMode(Mode m)             noexcept { mode = m; }
    void setClockMode(ClockMode m)   noexcept { clockMode = m; }
    void setDivisionIndex(int idx)   noexcept;
    void setDivisionTicks(int ticks) noexcept { ticksPerStep = juce::jmax(1, ticks); }
    void setTempo(float bpm)         noexcept { tempo = juce::jlimit(60.0f, 200.0f, bpm); }

    /** Clear all held notes and reset playback state (call on preset change). */
    void panic() noexcept { notes.clearQuick(); chordNotes.clearQuick(); lastNote = -1; currentNote = -1; }
    float getTempo() const           noexcept { return tempo; }

    /** Call this before processBlock to provide host transport info. */
    void setPlayHead(juce::AudioPlayHead* ph) noexcept { playHead = ph; }

private:

    bool enabled = true;
    int  octaves = 1;

    Mode       mode = Mode::Up;
    ClockMode  clockMode = ClockMode::Internal;

    float sampleRate = 44100.f;
    int   timeSamples = 0;
    int   clockCounter = 0;
    int   ticksPerStep = 6;  // default: 1/16 bei 24 PPQN
    bool  isPlaying = false;
    float tempo = 120.0f;    // BPM for internal clock

    juce::Array<int> notes;
    juce::Array<int> chordNotes;  // notes currently sounding in Chord mode
    int currentNote = -1;
    int lastNote = -1;
    int direction = 1;
    int octave = 0;

    juce::AudioPlayHead* playHead = nullptr;

    // Host sync state
    double lastPpqPosition = -1.0;

    /** Returns the step length in quarter notes for the current division. */
    double getStepLengthInQuarterNotes() const noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};
