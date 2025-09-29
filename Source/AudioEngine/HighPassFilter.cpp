//
//  HighPassFilter.cpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//
//


#include "HighPassFilter.h"
#include "ADSR.h"

using juce::IIRFilter;
using juce::IIRCoefficients;

HighPassFilter::HighPassFilter() {
    this->filter1 = new IIRFilter();
    this->filter2 = new IIRFilter();
    
}

HighPassFilter::~HighPassFilter() {
    this->filter1 = nullptr;
    this->filter2 = nullptr;
}


void HighPassFilter::coefficients(float sampleRate, float frequency, float resonance) {
    
    this->frequency = frequency / 10;
    this->resonance = resonance;
    
    if (frequency >= sampleRate / 2) {
        frequency = sampleRate / 2;
    }
    
    IIRCoefficients ic1  = IIRCoefficients::makeHighPass (sampleRate, frequency / 10, resonance);
    filter1->setCoefficients(ic1);
    filter2->setCoefficients(ic1);
}

void HighPassFilter::process(float *in, float *out, int numSamples) {
    
    float f = frequency;
    
    f = this->frequency + (currentModulatedValue * 1000);
        
    if (f <= 0) {
        f = 0.1;
    }
    if (f > 22000) {
        f = 22000;
    }
        
    IIRCoefficients ic1  = IIRCoefficients::makeHighPass (44100, f, this->resonance);
        
    filter1->setCoefficients(ic1);
    filter2->setCoefficients(ic1);
    
    this->filter1->processSamples(in,numSamples);
    // in -= numSamples;
    // this->filter2->processSamples(in,numSamples);
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


