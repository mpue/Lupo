#pragma once
#include <JuceHeader.h>
#include <array>
#include <atomic>

/**
 * Note-triggered 32-step MIDI sequencer.
 *
 * - Starts when a MIDI note is held; stops when all notes are released.
 * - Transposes the pattern relative to the held note (C4 = no transposition).
 * - Uses host BPM when a play-head is available; falls back to internal tempo.
 */
class StepSequencer
{
public:
    static constexpr int MAX_STEPS = 32;

    struct Step
    {
        std::atomic<bool>  active   { false };
        std::atomic<int>   note     { 60 };
        std::atomic<int>   velocity { 100 };
        std::atomic<float> length   { 0.5f };

        Step() = default;
        Step(const Step&) = delete;
        Step& operator=(const Step&) = delete;
    };

    enum class Direction { Forward = 0, Backward, PingPong, Random };

    StepSequencer();

    void prepareToPlay(double sampleRate, int blockSize);

    /** Must be called from the audio thread.
     *  Scans incoming MIDI for note-on/off to trigger the sequencer,
     *  eats the trigger notes and replaces them with the sequence output. */
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

    void setPlayHead(juce::AudioPlayHead* ph) { playHead = ph; }

    void setEnabled(bool b);
    void setTempo(float bpm)    { tempo.store(juce::jlimit(20.0f, 300.0f, bpm)); }
    void setNumSteps(int n)     { numSteps.store(juce::jlimit(1, MAX_STEPS, n)); }
    void setDivision(int d)     { division.store(juce::jlimit(0, 3, d)); }
    void setDirection(int d)    { direction.store(juce::jlimit(0, 3, d)); }
    void setSwing(float s)      { swing.store(juce::jlimit(0.0f, 0.75f, s)); }

    Step&       getStep(int i)       { return steps[i]; }
    const Step& getStep(int i) const { return steps[i]; }

    int   getPlayingStep()     const { return playingStep.load(); }
    int   getNumSteps()        const { return numSteps.load(); }
    bool  isEnabled()          const { return enabled.load(); }
    bool  isRunning()          const { return running.load(); }
    float getTempo()           const { return tempo.load(); }
    int   getDivision()        const { return division.load(); }
    int   getDirection()       const { return direction.load(); }
    float getSwing()           const { return swing.load(); }
    int   getTransposition()   const { return transposition; }   // semitones, audio-thread value

    /** Serialises all 32 step values to a compact string (for preset saving). */
    juce::String getStateAsString() const;

    /** Restores step data from a string previously created by getStateAsString(). */
    void loadStateFromString(const juce::String& s);

private:
    std::array<Step, MAX_STEPS> steps;

    std::atomic<bool>  enabled     { false };
    std::atomic<int>   numSteps    { 16 };
    std::atomic<int>   division    { 2 };    // 0=1/4  1=1/8  2=1/16  3=1/32
    std::atomic<int>   direction   { 0 };
    std::atomic<float> swing       { 0.0f };
    std::atomic<float> tempo       { 120.0f };
    std::atomic<int>   playingStep { 0 };
    std::atomic<bool>  running     { false };

    // Audio-thread-only state
    double sampleRate       = 44100.0;
    double hostBpm          = 0.0;   // >0 when host provides BPM
    int    samplesUntilNext = 0;
    int    noteOffLeft      = -1;
    int    lastNote         = -1;
    int    currentStep      = 0;
    int    pingPongDir      = 1;
    bool   isEvenStep       = true;
    int    heldNoteCount    = 0;
    int    transposition    = 0;     // semitones offset from C4

    juce::AudioPlayHead* playHead = nullptr;

    void reset();
    int  stepLengthSamples() const;
    void advanceStep();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepSequencer)
};
