//
//  HighPassFilter.cpp
// 
//  Original use in Trio, adapted for SynthLab, then modified and used here.
//
//  Created by Matthias Pueski on 16.11.16.
//  Enhanced to prevent artifacts when adjusting cutoff,
//  since we are using a state variable filter in series now
// 
//

#include "HighPassFilter.h"
#include "ADSR.h"
#include <cmath>

HighPassFilter::HighPassFilter()
{
	smoothedCutoff.reset(spec.sampleRate, 0.002);
	// Much faster smoothing: 2ms for immediate musical response
	svf1.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	svf1.prepare(spec);
	svf2.setType(juce::dsp::StateVariableTPTFilterType::highpass);
	svf2.prepare(spec);
}

// called from other prepareToPlayMethods (z.B. prepareToPlay)
void HighPassFilter::coefficients(float newSampleRate,
	float newFrequency,
	float newResonance)
{
	spec.sampleRate = newSampleRate;
	svf1.prepare(spec);
	svf2.prepare(spec);

	// Update smoothing for new sample rate with fast response
	smoothedCutoff.reset(newSampleRate, 0.002);

	frequency = newFrequency;
	resonance = juce::jlimit(0.05f, 10.0f, newResonance);
	svf1.setResonance(resonance);
	svf2.setResonance(resonance);
}

void HighPassFilter::setFrequency(float newFrequency)
{
	this->frequency = newFrequency;
	// For musical response, we need immediate target updates without large thresholds
}

void HighPassFilter::setFrequencyImmediate(float newFrequency)
{
	this->frequency = newFrequency;
	// For real-time user control, bypass smoothing and update immediately
	float targetCutoff = frequency * currentModulatedValue;
	targetCutoff = juce::jlimit(20.0f, 20000.0f, targetCutoff);

	// Force immediate coefficient update for real-time responsiveness
	svf1.setCutoffFrequency(targetCutoff);
	svf2.setCutoffFrequency(targetCutoff);
	lastCutoff = targetCutoff;

	// Also update the smoother to prevent jumps
	smoothedCutoff.setCurrentAndTargetValue(targetCutoff);
}

void HighPassFilter::setResonance(float newResonance)
{
	this->resonance = newResonance;
	float clampedResonance = juce::jlimit(0.05f, 5.0f, newResonance);
	svf1.setResonance(clampedResonance);
	svf2.setResonance(clampedResonance);
}

// ─────────────────────────────────────────────────────────────
// Real-time processing (in-place, Mono)
// ─────────────────────────────────────────────────────────────
void HighPassFilter::process(float* samples, int numSamples)
{
	// 1) Calculate new target considering modulation
	float targetCutoff = frequency * currentModulatedValue;
	targetCutoff = juce::jlimit(20.0f, 20000.0f, targetCutoff);

	// Always update target for immediate musical response - remove threshold
	smoothedCutoff.setTargetValue(targetCutoff);

	for (int i = 0; i < numSamples; ++i)
	{
		// 2) Update filter coefficients more frequently for responsiveness
		if (++updateCounter >= updateInterval)
		{
			float smooth = smoothedCutoff.getNextValue();   // geglättet

			// Reduce epsilon for more immediate updates
			if (std::abs(smooth - lastCutoff) > cutoffEpsilon)
			{
				svf1.setCutoffFrequency(smooth);
				svf2.setCutoffFrequency(smooth);
				lastCutoff = smooth;
			}
			updateCounter = 0;
		}
		else {
			// Even when not updating coefficients, advance the smoother
			smoothedCutoff.skip(1);
		}

		// finally process the sample through both filters in series
		samples[i] = svf1.processSample(0, samples[i]);
		samples[i] = svf2.processSample(0, samples[i]);
	}
}

float HighPassFilter::processSample(float sample)
{
	float targetCutoff = juce::jlimit(20.0f, 20000.0f, frequency * currentModulatedValue);

	// Large jumps (e.g. note-on with instant attack) bypass smoothing for immediate response
	if (std::abs(targetCutoff - lastCutoff) > 500.0f)
	{
		smoothedCutoff.setCurrentAndTargetValue(targetCutoff);
		svf1.setCutoffFrequency(targetCutoff);
		svf2.setCutoffFrequency(targetCutoff);
		lastCutoff = targetCutoff;
	}
	else
	{
		smoothedCutoff.setTargetValue(targetCutoff);
	}

	if (++updateCounter >= updateInterval)
	{
		float smooth = smoothedCutoff.getNextValue();
		if (std::abs(smooth - lastCutoff) > cutoffEpsilon)
		{
			svf1.setCutoffFrequency(smooth);
			svf2.setCutoffFrequency(smooth);
			lastCutoff = smooth;
		}
		updateCounter = 0;
	}
	else
	{
		smoothedCutoff.skip(1);
	}

	sample = svf1.processSample(0, sample);
	sample = svf2.processSample(0, sample);
	return sample;
}

void HighPassFilter::processModulation()
{
    // Octave-based modulation: modAmount=1.0 sweeps 4 octaves upward.
    float totalOctaves = 0.0f;
    for (auto mod : modulators) {
        totalOctaves += mod->getOutput() * mod->getModAmount() * 4.0f;
    }
    currentModulatedValue = juce::jmax(0.01f, std::pow(2.0f, totalOctaves));
}

