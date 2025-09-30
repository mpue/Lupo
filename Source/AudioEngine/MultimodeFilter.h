//
//  MultimodeFilter.hpp
//  Trio
//
//  Created by Matthias Pueski on 01.12.16.
//
//

#ifndef MultimodeFilter_hpp
#define MultimodeFilter_hpp

#include <stdio.h>
#include "Filter.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "StereoEffect.h"
#include "ModTarget.h"

#include "../JuceLibraryCode/JuceHeader.h"

class MultimodeFilter : public Filter, public StereoEffect, public ModTarget {

public:
    enum Mode {
        HIGHPASS,
        LOWPASS
    };
    
    MultimodeFilter();
    virtual ~MultimodeFilter();
    
    virtual void coefficients(float sampleRate,float frequency, float resonance) override;
    virtual void processStereo(float *const left, float *const right, const int numSamples) override;
	virtual void processMono(int channel, float *const samples, const int numSamples);
    virtual void addModulator(Modulator* mod) override;

    void setFrequency(float frequency) {
    	this->frequency = frequency;		
        // Let the individual filters handle smoothing - don't set immediately
        this->lowPassLeftStage1->setFrequency(frequency);
        this->lowPassRightStage1->setFrequency(frequency);
        this->lowPassLeftStage2->setFrequency(frequency);
        this->lowPassRightStage2->setFrequency(frequency);
        this->highPassLeft->setFrequency(frequency);
        this->highPassRight->setFrequency(frequency);
	}
	
	void setFrequencyImmediate(float frequency) {
    	this->frequency = frequency;		
        // For real-time control, use immediate updates
        this->lowPassLeftStage1->setFrequencyImmediate(frequency);
        this->lowPassRightStage1->setFrequencyImmediate(frequency);
        this->lowPassLeftStage2->setFrequencyImmediate(frequency);
        this->lowPassRightStage2->setFrequencyImmediate(frequency);
        // HighPass filters don't have immediate method yet, use regular
        this->highPassLeft->setFrequency(frequency);
        this->highPassRight->setFrequency(frequency);
	}
	
    void setResonance(float resonance) {
        this->resonance = resonance;
        lowPassLeftStage1->setResonance(resonance);
        lowPassRightStage1->setResonance(resonance);
        lowPassLeftStage2->setResonance(resonance);
        lowPassRightStage2->setResonance(resonance);
        highPassLeft->setResonance(resonance);
        highPassRight->setResonance(resonance);
    }
    void setMode(Mode mode);
	void setKeyTrack(int track);

	virtual void processModulation() override;

private:
    
    ScopedPointer<LowPassFilter> lowPassLeftStage1;
    ScopedPointer<LowPassFilter> lowPassRightStage1;

	ScopedPointer<LowPassFilter> lowPassLeftStage2;
	ScopedPointer<LowPassFilter> lowPassRightStage2;

    ScopedPointer<HighPassFilter> highPassLeft;
    ScopedPointer<HighPassFilter> highPassRight;
        
    float* out;
    
    Mode mode;

	int keyTrack;

    float frequency;
    float resonance;

    JUCE_LEAK_DETECTOR(MultimodeFilter);
    
};

#endif /* MultimodeFilter_hpp */