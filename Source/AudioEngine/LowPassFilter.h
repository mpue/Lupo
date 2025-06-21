//
//  LowPassFilter.hpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//
//

#ifndef LowPassFilter_hpp
#define LowPassFilter_hpp

#include <stdio.h>
#include "Filter.h"
#include "Modulator.h"
#include "ModTarget.h"

#include "../JuceLibraryCode/JuceHeader.h"

class LowPassFilter : public Filter, public ModTarget {
    
public:
    virtual void coefficients(float sampleRate, float frequency, float resonance) override;
    virtual void process(float *in, float *out,int numSamples);
    virtual void setModulator(Modulator* mod) override;
    
    LowPassFilter();
    virtual ~LowPassFilter();
    virtual void applyModulation(float value) override {
		this->currentModulatedValue = value;
	}
private:
    juce::IIRFilter* filter1;
    juce::IIRFilter* filter2;
    float frequency;
    float resonance;
    float currentModulatedValue = 0.0f;

    float modulationDepth = 5000.0f;
    float lastFrequency = 0.0f;

    JUCE_LEAK_DETECTOR(LowPassFilter);

};


#endif /* LowPassFilter_hpp */

