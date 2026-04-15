#include "StepSequencer.h"

static const double kDivFactors[4] = { 1.0, 0.5, 0.25, 0.125 };

// ── Preset serialisation ─────────────────────────────────────────────────────

juce::String StepSequencer::getStateAsString() const
{
    // Format: "active,note,velocity,length" per step, separated by ";"
    juce::String s;
    for (int i = 0; i < MAX_STEPS; ++i)
    {
        if (i > 0) s += ";";
        s += juce::String((int)steps[i].active.load())   + ","
           + juce::String(steps[i].note.load())           + ","
           + juce::String(steps[i].velocity.load())       + ","
           + juce::String(steps[i].length.load(), 3);
    }
    return s;
}

void StepSequencer::loadStateFromString(const juce::String& s)
{
    if (s.trim().isEmpty()) return;

    juce::StringArray stepTokens;
    stepTokens.addTokens(s.trim(), ";", "");

    for (int i = 0; i < juce::jmin(MAX_STEPS, stepTokens.size()); ++i)
    {
        juce::StringArray vals;
        vals.addTokens(stepTokens[i], ",", "");
        if (vals.size() < 4) continue;

        steps[i].active.store   (vals[0].getIntValue() != 0);
        steps[i].note.store     (juce::jlimit(0, 127, vals[1].getIntValue()));
        steps[i].velocity.store (juce::jlimit(1, 127, vals[2].getIntValue()));
        steps[i].length.store   (juce::jlimit(0.05f, 1.0f, vals[3].getFloatValue()));
    }
}

StepSequencer::StepSequencer()
{
    // Default pattern: C major scale ascending/descending
    const int defaultNotes[16] = { 60, 62, 64, 65, 67, 69, 71, 72,
                                    72, 71, 69, 67, 65, 64, 62, 60 };
    for (int i = 0; i < MAX_STEPS; ++i)
    {
        steps[i].note.store     (defaultNotes[i % 16]);
        steps[i].velocity.store (100);
        steps[i].active.store   (i < 16);
        steps[i].length.store   (0.5f);
    }
}

void StepSequencer::prepareToPlay(double sr, int /*blockSize*/)
{
    sampleRate       = sr;
    samplesUntilNext = stepLengthSamples();
    noteOffLeft      = -1;
    lastNote         = -1;
    currentStep      = 0;
    pingPongDir      = 1;
    isEvenStep       = true;
    heldNoteCount    = 0;
    transposition    = 0;
    hostBpm          = 0.0;
}

int StepSequencer::stepLengthSamples() const
{
    double bpm = (hostBpm > 0.0) ? hostBpm : (double)tempo.load();
    double div = kDivFactors[juce::jlimit(0, 3, division.load())];
    return juce::jmax(1, (int)(60.0 / bpm * div * sampleRate));
}

void StepSequencer::advanceStep()
{
    int n = juce::jmax(1, numSteps.load());

    switch ((Direction)direction.load())
    {
        case Direction::Forward:
            currentStep = (currentStep + 1) % n;
            break;
        case Direction::Backward:
            currentStep = (currentStep - 1 + n) % n;
            break;
        case Direction::PingPong:
        {
            int next = currentStep + pingPongDir;
            if (next >= n)     { pingPongDir = -1; next = juce::jmax(0, n - 2); }
            else if (next < 0) { pingPongDir =  1; next = juce::jmin(1, n - 1); }
            currentStep = next;
            break;
        }
        case Direction::Random:
            currentStep = juce::Random::getSystemRandom().nextInt(n);
            break;
    }
    isEvenStep = !isEvenStep;
}

void StepSequencer::setEnabled(bool b)
{
    enabled.store(b);
    if (!b) reset();
}

void StepSequencer::reset()
{
    lastNote         = -1;
    noteOffLeft      = -1;
    currentStep      = 0;
    pingPongDir      = 1;
    isEvenStep       = true;
    heldNoteCount    = 0;
    transposition    = 0;
    samplesUntilNext = stepLengthSamples();
    loopPosition.store(0.0f);
    running.store(false);
}

void StepSequencer::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    // Always flush a pending note-off even when disabled
    if (!enabled.load())
    {
        if (lastNote >= 0)
        {
            midi.addEvent(juce::MidiMessage::noteOff(1, lastNote, (juce::uint8)0), 0);
            lastNote    = -1;
            noteOffLeft = -1;
        }
        return;
    }

    const int numSamples = buffer.getNumSamples();

    // ── Update host BPM ──────────────────────────────────────────────────────
    if (playHead != nullptr)
    {
        juce::AudioPlayHead::CurrentPositionInfo info;
        if (playHead->getCurrentPosition(info) && info.bpm > 0.0)
            hostBpm = info.bpm;
    }

    // ── Scan incoming MIDI ───────────────────────────────────────────────────
    // Latch behaviour:
    //   • Note-on  → start from step 0 if not yet running, else just update
    //               transposition. Eat the note (replaced by sequence output).
    //   • Note-off → do NOT stop (latched). Just track held count.
    //   • Other    → pass through unchanged.
    juce::MidiBuffer filtered;

    for (const auto meta : midi)
    {
        auto m   = meta.getMessage();
        int  pos = meta.samplePosition;

        if (m.isNoteOn())
        {
            int note = m.getNoteNumber();

            if (!running.load())
            {
                // First trigger: send note-off for any lingering note, restart from step 0
                if (lastNote >= 0)
                {
                    filtered.addEvent(juce::MidiMessage::noteOff(1, lastNote, (juce::uint8)0), pos);
                    lastNote    = -1;
                    noteOffLeft = -1;
                }
                currentStep      = 0;
                samplesUntilNext = 0;   // fire step immediately at start of next sample loop
                isEvenStep       = true;
                pingPongDir      = 1;
                running.store(true);
            }

            // Update transposition so new notes shift the pattern root
            transposition = note - 60;
            heldNoteCount = juce::jmax(0, heldNoteCount) + 1;
            // Note-on consumed – sequencer generates its own pitches
        }
        else if (m.isNoteOff())
        {
            heldNoteCount = juce::jmax(0, heldNoteCount - 1);
            // LATCH: do not stop on note-off – sequence continues until disabled
            // Note-off consumed as well
        }
        else
        {
            filtered.addEvent(m, pos);  // CCs, pitch-bend, etc. pass through
        }
    }

    // ── Generate sequence output ─────────────────────────────────────────────
    if (running.load())
    {
        for (int i = 0; i < numSamples; ++i)
        {
            // Note-off countdown
            if (noteOffLeft > 0)
            {
                --noteOffLeft;
                if (noteOffLeft == 0 && lastNote >= 0)
                {
                    filtered.addEvent(juce::MidiMessage::noteOff(1, lastNote, (juce::uint8)0), i);
                    lastNote = -1;
                }
            }

            // Step trigger
            --samplesUntilNext;
            if (samplesUntilNext <= 0)
            {
                // Kill previous note if still sounding
                if (lastNote >= 0)
                {
                    filtered.addEvent(juce::MidiMessage::noteOff(1, lastNote, (juce::uint8)0), i);
                    lastNote    = -1;
                    noteOffLeft = -1;
                }

                playingStep.store(currentStep);
                loopPosition.store((float)currentStep / (float)juce::jmax(1, numSteps.load()));
                const auto& s = steps[currentStep];

                if (s.active.load())
                {
                    int note  = juce::jlimit(0, 127, s.note.load() + transposition);
                    int vel   = juce::jlimit(1, 127, s.velocity.load());
                    float len = juce::jlimit(0.05f, 1.0f, s.length.load());

                    filtered.addEvent(juce::MidiMessage::noteOn(1, note, (juce::uint8)vel), i);
                    lastNote    = note;
                    noteOffLeft = juce::jmax(1, (int)(stepLengthSamples() * len));
                }

                advanceStep();

                int stepLen = stepLengthSamples();
                if (swing.load() > 0.0f && !isEvenStep)
                    stepLen = (int)(stepLen * (1.0f + swing.load()));

                samplesUntilNext = juce::jmax(1, stepLen);
            }
        }

        // Update loopPosition continuously within the current step so that
        // automation consumers get a smooth 0..1 ramp rather than step-quantised jumps.
        {
            int n    = juce::jmax(1, numSteps.load());
            int slen = juce::jmax(1, stepLengthSamples());
            float frac = 1.0f - juce::jlimit(0.0f, 1.0f, (float)samplesUntilNext / (float)slen);
            loopPosition.store(((float)playingStep.load() + frac) / (float)n);
        }
    }

    midi.swapWith(filtered);
}
