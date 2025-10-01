#ifndef LowPassFilter_hpp
#define LowPassFilter_hpp

#include "Filter.h"
#include "Modulator.h"
#include "ModTarget.h"

#include <juce_dsp/juce_dsp.h>

/**
*  This is a simple low pass filter using JUCE's StateVariableTPTFilter.
*  It supports modulation of the cutoff frequency via the ModTarget interface.
* 
*  Author: Matthias Pueski
*  Date: June 2024
* 
*/

class LowPassFilter : public Filter, public ModTarget
{
public:
	LowPassFilter();
	~LowPassFilter() override = default;

	void coefficients(float sampleRate, float frequency, float resonance) override;
	void process(float* in, int numSamples);
	void setFrequency(float frequency);
	void setResonance(float resonance);
	
	virtual void addPwmModulator(Modulator* mod) override {};
	virtual void removePwmModulator(Modulator* mod) override {};

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

	JUCE_LEAK_DETECTOR(LowPassFilter)
};

#endif
