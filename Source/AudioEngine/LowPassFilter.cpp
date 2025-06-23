//
//  LowPassFilter.cpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//
//


#include "LowPassFilter.h"
#include "ADSR.h"

template <typename T>
T clamp(T value, T min, T max) {
    return std::max(min, std::min(value, max));
}

using juce::IIRFilter;
using juce::IIRCoefficients;

LowPassFilter::LowPassFilter() {
    this->filter1 = new IIRFilter();
    this->filter2 = new IIRFilter();
    this->modulator = 0;
}

LowPassFilter::~LowPassFilter() {
    delete this->filter1;
    delete this->filter2;
}


void LowPassFilter::coefficients(float sampleRate, float frequency, float resonance) {

    this->frequency = frequency;
    this->resonance = resonance;
    
    
    if (frequency > sampleRate / 2) {
        frequency = sampleRate / 2;
    }
    
    if (frequency <= 0) {
        frequency = 0.1;
    }
    
    if (resonance == 0) {
        resonance = 0.1;
    }
    
    IIRCoefficients ic1  = IIRCoefficients::makeLowPass (sampleRate, frequency, resonance);
    filter1->setCoefficients(ic1);
    filter2->setCoefficients(ic1);
}

void LowPassFilter::process(float* in, float* out, int numSamples) {

    float modValue = currentModulatedValue;

    float f = frequency + modValue * modulationDepth; // modulationDepth = z. B. 5000.0f

    // Clamp
    f = clamp(f, 20.0f, 20000.0f);

    if (std::abs(f - lastFrequency) > 0.1f) {
        IIRCoefficients ic = IIRCoefficients::makeLowPass(44100.0, f, std::max(resonance, 0.001f));
        filter1->setCoefficients(ic);
        filter2->setCoefficients(ic);
        lastFrequency = f;
    }

    filter1->processSamples(in, numSamples);
    // Optional zweite Stufe:
    // filter2->processSamples(in, numSamples);
}

void LowPassFilter::setModulator(Modulator* mod) {
    this->modulator = mod;
}

