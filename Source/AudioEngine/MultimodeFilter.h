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

    virtual void addPwmModulator(std::shared_ptr<Modulator> mod) override {};
    virtual void removePwmModulator(std::shared_ptr<Modulator> mod) override {};
    
    virtual void coefficients(float sampleRate,float frequency, float resonance) override;
    virtual void processStereo(float *const left, float *const right, const int numSamples) override;
	virtual void processMono(int channel, float *const samples, const int numSamples);
    virtual void addModulator(std::shared_ptr<Modulator> mod) override;
    virtual void removeModulator(std::shared_ptr<Modulator> mod) override;

    void setFrequency(float frequency);	
    void setFrequencyImmediate(float frequency);
    void setResonance(float resonance);
    void setMode(Mode mode);
	void setKeyTrack(int track);

	virtual void processModulation() override;

private:
    
    std::unique_ptr<LowPassFilter> lowPassLeftStage1;
    std::unique_ptr<LowPassFilter> lowPassRightStage1;

    std::unique_ptr<LowPassFilter> lowPassLeftStage2;
    std::unique_ptr<LowPassFilter> lowPassRightStage2;

    std::unique_ptr<HighPassFilter> highPassLeft;
    std::unique_ptr<HighPassFilter> highPassRight;
        
    float* out;
    
    Mode mode;

	int keyTrack;

    float frequency;
    float resonance;

    JUCE_LEAK_DETECTOR(MultimodeFilter);
    
};

#endif 