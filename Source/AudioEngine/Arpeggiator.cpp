#include "Arpeggiator.h"

Arpeggiator::Arpeggiator() {}
Arpeggiator::~Arpeggiator() {}

void Arpeggiator::prepareToPlay(double sr, int)
{
    sampleRate = static_cast<float> (sr);
    timeSamples = 0;
    clockCounter = 0;
    isPlaying = false;
    currentNote = -1;
    lastNote = -1;
    octave = 0;
    notes.clear();
}

void Arpeggiator::setDivisionIndex(int idx) noexcept
{
    static const int table[]{ 24, 12, 6, 3 }; // 1/4, 1/8, 1/16, 1/32
    idx = juce::jlimit(0, 3, idx);
    ticksPerStep = table[idx];
}

static int getNextNoteIndex(int current, int size, Arpeggiator::Mode mode, int& dir)
{
    if (size == 0) return -1;

    switch (mode)
    {
    case Arpeggiator::Mode::Up:
        return (current + 1) % size;

    case Arpeggiator::Mode::Down:
        if (current <= 0) dir = 1;
        else if (current >= size - 1) dir = -1;
        return juce::jlimit(0, size - 1, current + dir);

    case Arpeggiator::Mode::Random:
        return juce::Random::getSystemRandom().nextInt(size);
    }

    return 0;
}

void Arpeggiator::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    if (!enabled) return;

    juce::MidiMessage msg;
    int pos;
    juce::MidiBuffer output;

    for (juce::MidiBuffer::Iterator it(midi); it.getNextEvent(msg, pos); )
    {
        if (msg.isMidiClock())               ++clockCounter;
        else if (msg.isMidiStart()) { clockCounter = 0; isPlaying = true; }
        else if (msg.isMidiContinue()) { isPlaying = true; }
        else if (msg.isMidiStop()) { isPlaying = false; }

        else if (msg.isNoteOn())             notes.addIfNotAlreadyThere(msg.getNoteNumber());
        else if (msg.isNoteOff())            notes.removeFirstMatchingValue(msg.getNoteNumber());
        else                                 output.addEvent(msg, pos);
    }

    bool stepDue = false;

    if (clockMode == ClockMode::Midi && isPlaying)
    {
        if (clockCounter >= ticksPerStep)
        {
            clockCounter = 0;
            stepDue = true;
        }
    }
    else if (clockMode == ClockMode::Internal)
    {
        int duration = static_cast<int> (sampleRate * 0.25f * (24.0f / ticksPerStep)); // skaliert zur Division
        timeSamples += buffer.getNumSamples();
        if (timeSamples >= duration)
        {
            timeSamples %= duration;
            stepDue = true;
        }
    }

    if (stepDue)
    {
        if (lastNote >= 0)
            output.addEvent(juce::MidiMessage::noteOff(1, lastNote + 12 * octave), 0);

        if (notes.size() > 0)
        {
            currentNote = getNextNoteIndex(currentNote, notes.size(), mode, direction);
            lastNote = notes[currentNote];
            output.addEvent(juce::MidiMessage::noteOn(1, lastNote + 12 * octave, (uint8)120), 0);
        }

        if (octaves > 0)
            octave = (octave + 1) % octaves;
    }

    midi.swapWith(output);
}
