#pragma once
#include <JuceHeader.h>

class Arpeggiator
{
public:
    Arpeggiator();
    ~Arpeggiator();

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

    enum class Mode { Up, Down, Random };
    enum class ClockMode { Internal, Midi };

    void setEnabled(bool shouldRun)                     noexcept { enabled = shouldRun; }
    void setOctaves(int num)                            noexcept { octaves = juce::jmax(1, num); }
    void setMode(Mode m)                             noexcept { mode = m; }
    void setClockMode(ClockMode m)                        noexcept { clockMode = m; }
    void setDivisionIndex(int idx)                            noexcept;
    void setDivisionTicks(int ticks)                          noexcept { ticksPerStep = juce::jmax(1, ticks); }

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

    juce::Array<int> notes;
    int currentNote = -1;
    int lastNote = -1;
    int direction = 1;
    int octave = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};
