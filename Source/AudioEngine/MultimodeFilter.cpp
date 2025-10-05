//
//  MultimodePassFilter.cpp
//  Trio
//
//  Created by Matthias Pueski on 16.11.16.
//
//


#include "MultimodeFilter.h"
#include "ADSR.h"

MultimodeFilter::MultimodeFilter() {

    this->lowPassLeftStage1 = std::make_unique<LowPassFilter>();
    this->lowPassRightStage1 = std::make_unique<LowPassFilter>();

	this->lowPassLeftStage2 = std::make_unique<LowPassFilter>();
	this->lowPassRightStage2 = std::make_unique<LowPassFilter>();

    this->highPassLeft = std::make_unique<HighPassFilter>();
    this->highPassRight = std::make_unique<HighPassFilter>();
    
    this->mode = LOWPASS;
	this->keyTrack = 0;
}

MultimodeFilter::~MultimodeFilter() {
    this->lowPassLeftStage1 = nullptr;
    this->lowPassRightStage1 = nullptr;
    
	this->lowPassLeftStage2 = nullptr;
	this->lowPassRightStage2 = nullptr;

	this->highPassLeft = nullptr;
    this->highPassRight = nullptr;
}

void MultimodeFilter::setMode(Mode mode) {
    this->mode = mode;
}

void MultimodeFilter::coefficients(float sampleRate, float frequency, float resonance) {	

	if (out == nullptr) {
		out = new float[1024];
	}

	if (frequency <= 0) {
		frequency = 0.1;
	}
	if (resonance <= 0) {
		resonance = 0.01f;
	}
    this->lowPassLeftStage1->coefficients(sampleRate, frequency , resonance);
    this->lowPassRightStage1->coefficients(sampleRate, frequency, resonance);
	
	this->lowPassLeftStage2->coefficients(sampleRate, frequency + keyTrack * 10, resonance);
	this->lowPassRightStage2->coefficients(sampleRate, frequency + keyTrack * 10, resonance);
	
    this->highPassLeft->coefficients(sampleRate, frequency, resonance);
    this->highPassRight->coefficients(sampleRate, frequency, resonance);
}

void MultimodeFilter::processStereo(float *const left, float *const right, const int numSamples) {
    if (this->enabled) {
        if (this->mode == Mode::LOWPASS) {
            this->lowPassLeftStage1->process(left, numSamples);
            this->lowPassRightStage1->process(right, numSamples);
			this->lowPassLeftStage2->process(left, numSamples);
			this->lowPassRightStage2->process(right, numSamples);
			
		}
        else {
            this->highPassLeft->process(left, numSamples);
            this->highPassRight->process(right, numSamples);
        }   
    }
}

void MultimodeFilter::processMono(int channel, float * const samples, const int numSamples)
{
	if (this->enabled) {
		if (this->mode == Mode::LOWPASS) {
			this->lowPassLeftStage1->process(samples, numSamples);
			this->lowPassLeftStage2->process(samples, numSamples);


		}
		else {
			this->highPassLeft->process(samples, numSamples);
		}

	}

}

void MultimodeFilter::addModulator(std::shared_ptr<Modulator> mod)
{
	lowPassLeftStage1->addModulator(mod);
	lowPassRightStage1->addModulator(mod);
	lowPassLeftStage2->addModulator(mod);
	lowPassRightStage2->addModulator(mod);
	highPassLeft->addModulator(mod);
	highPassRight->addModulator(mod);

}

void MultimodeFilter::removeModulator(std::shared_ptr<Modulator> mod) {
	lowPassLeftStage1->removeModulator(mod);
	lowPassRightStage1->removeModulator(mod);
	lowPassLeftStage2->removeModulator(mod);
	lowPassRightStage2->removeModulator(mod);
	highPassLeft->removeModulator(mod);
	highPassRight->removeModulator(mod);
}

void MultimodeFilter::setKeyTrack(int track) {
	this->keyTrack = track;
}

void MultimodeFilter::processModulation()
{
	lowPassLeftStage1->processModulation();
	lowPassRightStage1->processModulation();
	lowPassLeftStage2->processModulation();
	lowPassRightStage2->processModulation();
	highPassLeft->processModulation();
	highPassRight->processModulation();
}

void MultimodeFilter::setFrequency(float frequency) {
	this->frequency = frequency;		
	// Let the individual filters handle smoothing - don't set immediately
	this->lowPassLeftStage1->setFrequency(frequency);
	this->lowPassRightStage1->setFrequency(frequency);
	this->lowPassLeftStage2->setFrequency(frequency);
	this->lowPassRightStage2->setFrequency(frequency);
	this->highPassLeft->setFrequency(frequency);
	this->highPassRight->setFrequency(frequency);
}

void MultimodeFilter::setFrequencyImmediate(float frequency) {
	this->frequency = frequency;
	// For real-time control, use immediate updates
	this->lowPassLeftStage1->setFrequencyImmediate(frequency);
	this->lowPassRightStage1->setFrequencyImmediate(frequency);
	this->lowPassLeftStage2->setFrequencyImmediate(frequency);
	this->lowPassRightStage2->setFrequencyImmediate(frequency);
	// HighPass filters don't have immediate method yet, use regular
	this->highPassLeft->setFrequencyImmediate(frequency);
	this->highPassRight->setFrequencyImmediate(frequency);
}

void MultimodeFilter::setResonance(float resonance) {
	this->resonance = resonance;
	lowPassLeftStage1->setResonance(resonance);
	lowPassRightStage1->setResonance(resonance);
	lowPassLeftStage2->setResonance(resonance);
	lowPassRightStage2->setResonance(resonance);
	highPassLeft->setResonance(resonance);
	highPassRight->setResonance(resonance);
}




