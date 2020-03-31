//
//  LowPassFilter.cpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//
//


#include "LowPassFilter.h"
#include "ADSR.h"

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

void LowPassFilter::process(float *in, float *out, int numSamples) {
    
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
        }

        if (f <= 0) {
            f = 0.1;
        }
        if (f > 22000) {
            f = 22000;
        }
		if (resonance <= 0) {
			resonance = 0.001f;
		}

        IIRCoefficients ic1  = IIRCoefficients::makeLowPass (44100, f, this->resonance);

        filter1->setCoefficients(ic1);
        filter2->setCoefficients(ic1);
    }

    this->filter1->processSamples(in,numSamples);
    // in -= numSamples;
    // this->filter2->processSamples(in,numSamples);
}

void LowPassFilter::setModulator(Modulator* mod) {
    this->modulator = mod;
}

