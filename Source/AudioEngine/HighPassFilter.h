//
//  LowPassFilter.hpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//
//

#ifndef HighPassFilter_hpp
#define HighPassFilter_hpp

#include <stdio.h>
#include "Filter.h"
#include "Modulator.h"
#include "ModTarget.h"

#include "../JuceLibraryCode/JuceHeader.h"

class HighPassFilter : public Filter, public ModTarget {
    
public:
    virtual void coefficients(float sampleRate, float frequency, float resonance) override;
    virtual void process(float *in, float *out,int numSamples);
	virtual void processModulation() override;
    
    // Add missing methods for consistency with LowPassFilter
    void setFrequency(float frequency);
    void setResonance(float resonance);

    HighPassFilter();
    virtual ~HighPassFilter();
    
private:
    juce::ScopedPointer<juce::IIRFilter> filter1;
    juce::ScopedPointer<juce::IIRFilter> filter2;
    float frequency;
    float resonance;
    float sampleRate = 44100.0f;  // Add sample rate storage
    float currentModulatedValue = 1.0f;  // Fix initial value - was 0.0f 
    JUCE_LEAK_DETECTOR(HighPassFilter);
    
};


#endif /* HighPassFilter_hpp */

