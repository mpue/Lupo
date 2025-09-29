#include "LowPassFilter.h"

// ─────────────────────────────────────────────────────────────
// Konstruktor
// ─────────────────────────────────────────────────────────────
LowPassFilter::LowPassFilter()
{
    smoothedCutoff.reset(spec.sampleRate, 0.02);            // 20 ms Glide
    svf1.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    svf1.prepare(spec);
    svf2.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    svf2.prepare(spec);

}

// Wird von außen einmal aufgerufen (z.B. prepareToPlay)
void LowPassFilter::coefficients(float newSampleRate,
    float newFrequency,
    float newResonance)
{
    spec.sampleRate = newSampleRate;
    svf1.prepare(spec);
    svf2.prepare(spec);
    frequency = newFrequency;
    resonance = juce::jlimit(0.05f, 10.0f, newResonance);
    svf1.setResonance(resonance);
    svf2.setResonance(resonance);    
}

void LowPassFilter::setFrequency(float newFrequency)
{
    this->frequency = newFrequency;  // Fix: was assigning frequency to itself
    // Apply the new frequency immediately
    float targetCutoff = frequency * currentModulatedValue;
    targetCutoff = juce::jlimit(20.0f, 20000.0f, targetCutoff);
    svf1.setCutoffFrequency(targetCutoff);
    svf2.setCutoffFrequency(targetCutoff);
}

void LowPassFilter::setResonance(float newResonance)
{
    this->resonance = newResonance;  // Fix: was assigning resonance to itself
    float clampedResonance = juce::jlimit(0.05f, 5.0f, newResonance);
    svf1.setResonance(clampedResonance);
    svf2.setResonance(clampedResonance);
}

// ─────────────────────────────────────────────────────────────
// Echtzeit-Verarbeitung (in-place, Mono)
// ─────────────────────────────────────────────────────────────
void LowPassFilter::process(float* samples, int numSamples)
{
    // 1) neues Ziel berechnen
    float targetCutoff = frequency * currentModulatedValue;
    targetCutoff = juce::jlimit(20.0f, 20000.0f, targetCutoff);
    smoothedCutoff.setTargetValue(targetCutoff);

    for (int i = 0; i < numSamples; ++i)
    {
        // 2) nur alle updateInterval-Samples Filter neu parametrieren
        if (++updateCounter >= updateInterval)
        {
            float smooth = smoothedCutoff.getNextValue();   // geglättet

            if (std::abs(smooth - lastCutoff) > cutoffEpsilon)
            {
                svf1.setCutoffFrequency(smooth);
                svf2.setCutoffFrequency(smooth);    
                lastCutoff = smooth;
            }
            updateCounter = 0;
        }

        // 3) Sample durch den Filter schicken
        samples[i] = svf1.processSample(0, samples[i]);
        samples[i] = svf2.processSample(0, samples[i]);
    }
}

void LowPassFilter::processModulation()
{
	// now iterate through all modulators and accumulate their values,
	// then apply to cutoff frequency

	float modulatedValue = 1.0f;
    for (auto mod : modulators) {
        // Scale the envelope output by its modulation amount
        float envelopeValue = mod->getOutput();
        float modAmount = mod->getModAmount();
        
        // Apply modulation with proper scaling for musical filter sweep:
        // - The envelope output ranges from 0 to 1
        // - The modAmount is the user-controlled envelope amount (typically 0-100 or similar)
        // - Scale to provide musical filter sweeps (multiply by base frequency works well)
        modulatedValue += (envelopeValue * modAmount * 10.0f); // 10.0f provides good musical scaling
	}

	currentModulatedValue = juce::jmax(0.01f, modulatedValue); // Prevent cutoff from going to zero

	// Logger::getCurrentLogger()->writeToLog("LowPassFilter Modulated Value: " + String(currentModulatedValue) + " (Mod Depth: " + String(modulationDepth) + ")");
}

