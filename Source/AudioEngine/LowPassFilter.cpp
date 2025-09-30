#include "LowPassFilter.h"

// ─────────────────────────────────────────────────────────────
// Konstruktor
// ─────────────────────────────────────────────────────────────
LowPassFilter::LowPassFilter()
{
    smoothedCutoff.reset(spec.sampleRate, 0.002);            // Much faster smoothing: 2ms for immediate musical response
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
    
    // Update smoothing for new sample rate with fast response
    smoothedCutoff.reset(newSampleRate, 0.002);
    
    frequency = newFrequency;
    resonance = juce::jlimit(0.05f, 10.0f, newResonance);
    svf1.setResonance(resonance);
    svf2.setResonance(resonance);    
}

void LowPassFilter::setFrequency(float newFrequency)
{
    this->frequency = newFrequency;  
    // For musical response, we need immediate target updates without large thresholds
}

void LowPassFilter::setFrequencyImmediate(float newFrequency)
{
    this->frequency = newFrequency;
    // For real-time user control, bypass smoothing and update immediately
    float targetCutoff = frequency * currentModulatedValue;
    targetCutoff = juce::jlimit(20.0f, 20000.0f, targetCutoff);
    
    // Force immediate coefficient update for real-time responsiveness
    svf1.setCutoffFrequency(targetCutoff);
    svf2.setCutoffFrequency(targetCutoff);
    lastCutoff = targetCutoff;
    
    // Also update the smoother to prevent jumps
    smoothedCutoff.setCurrentAndTargetValue(targetCutoff);
}

void LowPassFilter::setResonance(float newResonance)
{
    this->resonance = newResonance;  
    float clampedResonance = juce::jlimit(0.05f, 5.0f, newResonance);
    svf1.setResonance(clampedResonance);
    svf2.setResonance(clampedResonance);
}

// ─────────────────────────────────────────────────────────────
// Echtzeit-Verarbeitung (in-place, Mono)
// ─────────────────────────────────────────────────────────────
void LowPassFilter::process(float* samples, int numSamples)
{
    // 1) Calculate new target considering modulation
    float targetCutoff = frequency * currentModulatedValue;
    targetCutoff = juce::jlimit(20.0f, 20000.0f, targetCutoff);
    
    // Always update target for immediate musical response - remove threshold
    smoothedCutoff.setTargetValue(targetCutoff);

    for (int i = 0; i < numSamples; ++i)
    {
        // 2) Update filter coefficients more frequently for responsiveness
        if (++updateCounter >= updateInterval)
        {
            float smooth = smoothedCutoff.getNextValue();   // geglättet

            // Reduce epsilon for more immediate updates
            if (std::abs(smooth - lastCutoff) > cutoffEpsilon)
            {
                svf1.setCutoffFrequency(smooth);
                svf2.setCutoffFrequency(smooth);    
                lastCutoff = smooth;
            }
            updateCounter = 0;
        }
        else {
            // Even when not updating coefficients, advance the smoother
            smoothedCutoff.skip(1);
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
}

