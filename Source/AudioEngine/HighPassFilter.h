//
//  HighPassFilter.hpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//  Enhanced to prevent artifacts when adjusting cutoff
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
    
    void setFrequency(float frequency);
    void setResonance(float resonance);

    HighPassFilter();
    virtual ~HighPassFilter();
    
private:
    juce::ScopedPointer<juce::IIRFilter> filter1;
    juce::ScopedPointer<juce::IIRFilter> filter2;
    float frequency;
    float resonance;
    float sampleRate = 44100.0f;
    float currentModulatedValue = 1.0f;
    
    // Additional members for artifact prevention
    int updateCounter = 0;
    int updateInterval = 4;      // More frequent updates for better response
    float lastFrequency = -1.0f; // Track last frequency to avoid redundant updates
    float freqEpsilon = 0.1f;    // Smaller epsilon for more immediate response
    
    JUCE_LEAK_DETECTOR(HighPassFilter);
};

#endif /* HighPassFilter_hpp */

