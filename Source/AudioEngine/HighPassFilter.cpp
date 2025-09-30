//
//  HighPassFilter.cpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//  Enhanced to prevent artifacts when adjusting cutoff
//

#include "HighPassFilter.h"
#include "ADSR.h"

using juce::IIRFilter;
using juce::IIRCoefficients;

HighPassFilter::HighPassFilter() {
    this->filter1 = new IIRFilter();
    this->filter2 = new IIRFilter();
    this->frequency = 1000.0f;
    this->resonance = 0.7f;
    this->sampleRate = 44100.0f;
    this->currentModulatedValue = 1.0f;
    this->updateCounter = 0;
    this->updateInterval = 4;  // More frequent updates for better response
    this->lastFrequency = -1.0f;
    this->freqEpsilon = 0.1f;  // Smaller epsilon for more immediate response
}

HighPassFilter::~HighPassFilter() {
    this->filter1 = nullptr;
    this->filter2 = nullptr;
}

void HighPassFilter::coefficients(float sampleRate, float frequency, float resonance) {
    this->sampleRate = sampleRate;
    this->frequency = frequency;
    this->resonance = resonance;
    
    if (frequency >= sampleRate / 2) {
        frequency = sampleRate / 2 - 1;
    }
    if (frequency <= 0) {
        frequency = 1.0f;
    }
    
    IIRCoefficients ic1 = IIRCoefficients::makeHighPass(sampleRate, frequency, resonance);
    filter1->setCoefficients(ic1);
    filter2->setCoefficients(ic1);
    
    lastFrequency = frequency;
}

void HighPassFilter::setFrequency(float newFrequency) {
    this->frequency = newFrequency;
    // Don't update coefficients immediately - let process() handle it with throttling
}

void HighPassFilter::setResonance(float newResonance) {
    this->resonance = newResonance;
    // Update coefficients immediately for resonance as it's less likely to cause artifacts
    coefficients(sampleRate, frequency, resonance);
}

void HighPassFilter::process(float *in, float *out, int numSamples) {
    
    for (int i = 0; i < numSamples; ++i) {
        // Update coefficients at a controlled rate to prevent artifacts
        if (++updateCounter >= updateInterval) {
            float targetFreq = this->frequency * currentModulatedValue;
            
            if (targetFreq <= 1.0f) {
                targetFreq = 1.0f;
            }
            if (targetFreq >= sampleRate / 2) {
                targetFreq = sampleRate / 2 - 1;
            }
            
            // Only update if there's a meaningful change
            if (std::abs(targetFreq - lastFrequency) > freqEpsilon) {
                IIRCoefficients ic1 = IIRCoefficients::makeHighPass(sampleRate, targetFreq, this->resonance);
                filter1->setCoefficients(ic1);
                filter2->setCoefficients(ic1);
                lastFrequency = targetFreq;
            }
            
            updateCounter = 0;
        }
        
        // Process the sample
        float sample = in[i];
        sample = filter1->processSingleSampleRaw(sample);
        sample = filter2->processSingleSampleRaw(sample);
        
        if (out != in) {
            out[i] = sample;
        } else {
            in[i] = sample;
        }
    }
}

void HighPassFilter::processModulation()
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


