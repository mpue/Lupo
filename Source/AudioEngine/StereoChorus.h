/*
  ==============================================================================

	StereoChorus.h
	Created: 4 Dec 2016 11:19:25am
	Author:  Matthias Pueski

  ==============================================================================
*/

#ifndef STEREOCHORUS_H_INCLUDED
#define STEREOCHORUS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "StereoEffect.h"
#include "FractionalDelayBuffer.h"
#include "Sine.h"


class StereoChorus : public StereoEffect {

public:

	StereoChorus(float sampleRate, int bufferSize);
	~StereoChorus();

	void processStereo(float* const left, float* const right, const int numSamples);

	void prepareToPlay(double sampleRate, int bufferSize) {
		
		this->sampleRate = sampleRate;
		this->bufferSize = bufferSize;

		if (leftOsc == nullptr) {
			leftOsc = new Sine(sampleRate, bufferSize);
		}
		if (rightOsc == nullptr) {
			rightOsc = new Sine(sampleRate, bufferSize);
		}

		leftBuffer->setBufferSize((int)sampleRate / 10);
		rightBuffer->setBufferSize((int)sampleRate / 10);
		leftOsc->setFrequency(0.5);
		rightOsc->setFrequency(0.6);

	}

	float modulation = 0.0f;
	float delay = 0.00f;
	float feedback = 0.0f;
	float leftMod = 0.0f;
	float rightMod = 0.0f;
	float mix = 0.0f;

private:
	float sampleRate;
	int bufferSize;	
	int currentSample = 0;
	float leftDelayTime, rightDelayTime = 10;
	Sine* leftOsc = nullptr;
	Sine* rightOsc = nullptr;
	FractionalDelayBuffer* leftBuffer = nullptr;
	FractionalDelayBuffer* rightBuffer = nullptr;
	JUCE_LEAK_DETECTOR(StereoChorus);

};



#endif  // STEREOCHORUS_H_INCLUDED
