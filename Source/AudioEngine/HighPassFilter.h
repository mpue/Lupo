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
	HighPassFilter();
	~HighPassFilter() override = default;

	virtual void addPwmModulator(Modulator* mod) override {};
	virtual void removePwmModulator(Modulator* mod) override {};

	void coefficients(float sampleRate, float frequency, float resonance) override;
	void process(float* in, int numSamples);
	void setFrequency(float frequency);
	void setResonance(float resonance);

	// Force immediate update for real-time control responsiveness
	void setFrequencyImmediate(float frequency);

	virtual void processModulation() override;

private:

	juce::dsp::StateVariableTPTFilter<float> svf1;
	juce::dsp::StateVariableTPTFilter<float> svf2;

	juce::dsp::ProcessSpec spec{ 44100.0, 512, 1 };

	float frequency = 1000.0f;
	float resonance = 0.7f;
	float currentModulatedValue = 1.0f;

	float modulationDepth = 1000.0f;
	int    updateCounter = 0;
	int    updateInterval = 4;          // More frequent updates: every 4 samples for better response
	float  lastCutoff = -1.0f;			// remember last cutoff, to react on changes
	float  cutoffEpsilon = 0.1f;        // Smaller epsilon for more immediate response

	LinearSmoothedValue<float> smoothedCutoff;

    JUCE_LEAK_DETECTOR(HighPassFilter);
};

#endif /* HighPassFilter_hpp */

