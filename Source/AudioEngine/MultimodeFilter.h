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
    virtual void setModulator(Modulator* mod) override;
    void setFrequency(float frequency) {
    	this->frequency = frequency;		
        this->lowPassLeftStage1->setFrequency(frequency);
        this->lowPassRightStage1->setFrequency(frequency);
	}
    void setResonance(float resooance) {
        this->resonance = resooance;
        lowPassLeftStage1->setResonance(resooance);
        lowPassRightStage1->setResonance(resooance);
    }
    void setMode(Mode mode);
	void setKeyTrack(int track);
    virtual void applyModulation(float value) override {
        lowPassRightStage1->applyModulation(value);
        lowPassLeftStage1->applyModulation(value);
        lowPassRightStage2->applyModulation(value);
        lowPassLeftStage2->applyModulation(value);
        highPassLeft->applyModulation(value);
        highPassRight->applyModulation(value);
    }
        

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