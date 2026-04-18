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
    lastPpqPosition = -1.0;
    notes.clear();
}

void Arpeggiator::setDivisionIndex(int idx) noexcept
{
    static const int table[]{ 24, 12, 6, 3 }; // 1/4, 1/8, 1/16, 1/32
idx = juce::jlimit(0, 3, idx);
    ticksPerStep = table[idx];
}

double Arpeggiator::getStepLengthInQuarterNotes() const noexcept
{
    // ticksPerStep is in 24-PPQN ticks
// 24 ticks = 1 quarter note
    return static_cast<double>(ticksPerStep) / 24.0;
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

    const int numSamples = buffer.getNumSamples();

    // Collect note-on/off from incoming MIDI, pass through everything else
    juce::MidiMessage msg;
    int pos;
  juce::MidiBuffer output;

    for (juce::MidiBuffer::Iterator it(midi); it.getNextEvent(msg, pos); )
    {
        if (msg.isNoteOn())       notes.addIfNotAlreadyThere(msg.getNoteNumber());
        else if (msg.isNoteOff())        notes.removeFirstMatchingValue(msg.getNoteNumber());
        else   output.addEvent(msg, pos);
    }

    // --- Determine step triggers ---

    if (clockMode == ClockMode::Midi && playHead != nullptr)
    {
        // === HOST SYNC MODE ===
     // Use AudioPlayHead to get PPQ position from the DAW

        auto posInfo = playHead->getPosition();

        if (posInfo.hasValue())
        {
  auto hostIsPlaying = posInfo->getIsPlaying();
            auto ppqOpt = posInfo->getPpqPosition();
            auto bpmOpt = posInfo->getBpm();

            if (hostIsPlaying && ppqOpt.hasValue() && bpmOpt.hasValue())
            {
      double ppqPosition = *ppqOpt;
              double hostBpm = *bpmOpt;
         double stepLen = getStepLengthInQuarterNotes();

         // How many samples per quarter note at host BPM
           double samplesPerQuarterNote = (60.0 / hostBpm) * sampleRate;
     double samplesPerStep = stepLen * samplesPerQuarterNote;

        // Walk through each sample in this block and check for step boundaries
       for (int sample = 0; sample < numSamples; ++sample)
        {
  double currentPpq = ppqPosition + (static_cast<double>(sample) / samplesPerQuarterNote);

        // Quantize to step grid
            double currentStepIndex = std::floor(currentPpq / stepLen);
      double prevPpq = currentPpq - (1.0 / samplesPerQuarterNote);
 double prevStepIndex = std::floor(prevPpq / stepLen);

     if (currentStepIndex != prevStepIndex || lastPpqPosition < 0.0)
     {
           // Step boundary crossed at this sample
        if (mode == Mode::Chord) {
            for (int n : chordNotes)
                output.addEvent(juce::MidiMessage::noteOff(1, n + 12 * octave), sample);
            chordNotes.clear();
        } else {
            if (lastNote >= 0)
                output.addEvent(juce::MidiMessage::noteOff(1, lastNote + 12 * octave), sample);
        }

            if (notes.size() > 0)
         {
            if (mode == Mode::Chord) {
                for (int n : notes) {
                    output.addEvent(juce::MidiMessage::noteOn(1, n + 12 * octave, (uint8)120), sample);
                    chordNotes.add(n);
                }
                lastNote = notes[0];
            } else {
                currentNote = getNextNoteIndex(currentNote, notes.size(), mode, direction);
                lastNote = notes[currentNote];
                output.addEvent(juce::MidiMessage::noteOn(1, lastNote + 12 * octave, (uint8)120), sample);
            }
  }

        if (octaves > 0)
         octave = (octave + 1) % octaves;
            }
      }

lastPpqPosition = ppqPosition + (static_cast<double>(numSamples) / samplesPerQuarterNote);
  }
       else
  {
         // Host is stopped — release any held note(s)
            for (int n : chordNotes)
                output.addEvent(juce::MidiMessage::noteOff(1, n + 12 * octave), 0);
            chordNotes.clear();
         if (lastNote >= 0)
        {
          output.addEvent(juce::MidiMessage::noteOff(1, lastNote + 12 * octave), 0);
     lastNote = -1;
    }
                lastPpqPosition = -1.0;
            }
        }
    }
    else if (clockMode == ClockMode::Internal)
    {
   // === INTERNAL CLOCK MODE (unchanged) ===
        float beatsPerSecond = tempo / 60.0f;
     float samplesPerBeat = sampleRate / beatsPerSecond;
        float samplesPerTick = samplesPerBeat / 24.0f;  // 24 PPQN
        int duration = static_cast<int>(samplesPerTick * ticksPerStep);

        timeSamples += numSamples;
        if (timeSamples >= duration)
        {
            timeSamples %= duration;

        if (mode == Mode::Chord) {
                for (int n : chordNotes)
                    output.addEvent(juce::MidiMessage::noteOff(1, n + 12 * octave), 0);
                chordNotes.clear();
            } else {
                if (lastNote >= 0)
                    output.addEvent(juce::MidiMessage::noteOff(1, lastNote + 12 * octave), 0);
            }

            if (notes.size() > 0)
    {
                if (mode == Mode::Chord) {
                    for (int n : notes) {
                        output.addEvent(juce::MidiMessage::noteOn(1, n + 12 * octave, (uint8)120), 0);
                        chordNotes.add(n);
                    }
                    lastNote = notes[0];
                } else {
                    currentNote = getNextNoteIndex(currentNote, notes.size(), mode, direction);
                    lastNote = notes[currentNote];
                    output.addEvent(juce::MidiMessage::noteOn(1, lastNote + 12 * octave, (uint8)120), 0);
                }
     }

       if (octaves > 0)
    octave = (octave + 1) % octaves;
        }
    }

    midi.swapWith(output);
}
