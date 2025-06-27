#include "LowPassFilter.h"

// ─────────────────────────────────────────────────────────────
// Konstruktor
// ─────────────────────────────────────────────────────────────
LowPassFilter::LowPassFilter()
{
    smoothedCutoff.reset(spec.sampleRate, 0.02);            // 20 ms Glide
    svf.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    svf.prepare(spec);
}

// Wird von außen einmal aufgerufen (z.B. prepareToPlay)
void LowPassFilter::coefficients(float newSampleRate,
    float newFrequency,
    float newResonance)
{
    spec.sampleRate = newSampleRate;
    svf.prepare(spec);

    frequency = newFrequency;
    resonance = juce::jlimit(0.05f, 10.0f, newResonance);
    svf.setResonance(resonance);
}

// ─────────────────────────────────────────────────────────────
// Echtzeit-Verarbeitung (in-place, Mono)
// ─────────────────────────────────────────────────────────────
void LowPassFilter::process(float* samples, int numSamples)
{
    // 1) neues Ziel berechnen
    float targetCutoff = frequency * currentModulatedValue * modulationDepth;
    targetCutoff = juce::jlimit(20.0f, 20'000.0f, targetCutoff);
    smoothedCutoff.setTargetValue(targetCutoff);

    for (int i = 0; i < numSamples; ++i)
    {
        // 2) nur alle updateInterval-Samples Filter neu parametrieren
        if (++updateCounter >= updateInterval)
        {
            float smooth = smoothedCutoff.getNextValue();   // geglättet

            if (std::abs(smooth - lastCutoff) > cutoffEpsilon)
            {
                svf.setCutoffFrequency(smooth);
                lastCutoff = smooth;
            }
            updateCounter = 0;
        }

        // 3) Sample durch den Filter schicken
        samples[i] = svf.processSample(0, samples[i]);
    }
}

void LowPassFilter::setModulator(Modulator* /*mod*/) { /* optional */ }