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
    this->modulator = 0;
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
    
    if (this->modulator != 0) {
        
        
        if (SynthLab::ADSR* env = dynamic_cast<SynthLab::ADSR*>(this->modulator)) {
            
            if(env->getState() != SynthLab::ADSR::env_idle) {
                f =  this->frequency + (modulator->getOutput() * modulator->getModAmount() * (22000 - this->frequency));
            }
            else {
                env->reset();
            }
            
        }
        else {
            f =  this->frequency + (modulator->getOutput() * modulator->getModAmount() * 1000);
            // modulator->process();
        }
        
        if (f <= 0) {
            f = 0.1;
        }
        if (f > 22000) {
            f = 22000;
        }
        
        IIRCoefficients ic1  = IIRCoefficients::makeHighPass (44100, f, this->resonance);
        
        filter1->setCoefficients(ic1);
        filter2->setCoefficients(ic1);
    }
    
    this->filter1->processSamples(in,numSamples);
    // in -= numSamples;
    // this->filter2->processSamples(in,numSamples);
}

void HighPassFilter::setModulator(Modulator* mod) {
    this->modulator = mod;
}

