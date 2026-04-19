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

    this->charFilter = std::make_unique<CharacterFilter>();
    
    this->mode = LOWPASS;
	this->keyTrack = 0;
    this->out = nullptr;
}

MultimodeFilter::~MultimodeFilter() {
    delete[] out;

    this->lowPassLeftStage1 = nullptr;
    this->lowPassRightStage1 = nullptr;
    
	this->lowPassLeftStage2 = nullptr;
	this->lowPassRightStage2 = nullptr;

	this->highPassLeft = nullptr;
    this->highPassRight = nullptr;

    this->charFilter = nullptr;
}

void MultimodeFilter::setMode(Mode mode) {
    this->mode = mode;
    charFilter->setMode(mode == LOWPASS ? CharacterFilter::LOWPASS : CharacterFilter::HIGHPASS);
}

void MultimodeFilter::setCharacter(CharacterFilter::Character c) {
    this->character = c;
    charFilter->setCharacter(c);
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
    this->frequency = frequency;
    this->resonance = resonance;
    this->lowPassLeftStage1->coefficients(sampleRate, frequency , resonance);
    this->lowPassRightStage1->coefficients(sampleRate, frequency, resonance);
	
	this->lowPassLeftStage2->coefficients(sampleRate, frequency + keyTrack * 10, resonance);
	this->lowPassRightStage2->coefficients(sampleRate, frequency + keyTrack * 10, resonance);
	
    this->highPassLeft->coefficients(sampleRate, frequency, resonance);
    this->highPassRight->coefficients(sampleRate, frequency, resonance);

    charFilter->prepare(sampleRate);
    charFilter->setFrequencyImmediate(frequency);
    charFilter->setResonance(resonance);
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

void MultimodeFilter::setCutoffModulation(float value)
{
	lowPassLeftStage1->setModulatedValue(value);
	lowPassRightStage1->setModulatedValue(value);
	lowPassLeftStage2->setModulatedValue(value);
	lowPassRightStage2->setModulatedValue(value);
	highPassLeft->setModulatedValue(value);
	highPassRight->setModulatedValue(value);
	// Sync character filter with the combined modulation (list * envelope)
	charFilter->setModulatedValue(lowPassLeftStage1->getModulatedValue());
}

void MultimodeFilter::processSampleStereo(float& left, float& right)
{
	if (!enabled) return;

	if (character != CharacterFilter::STANDARD)
	{
		charFilter->processSample(left, right);
		return;
	}

	if (mode == LOWPASS) {
		left  = lowPassLeftStage1->processSample(left);
		left  = lowPassLeftStage2->processSample(left);
		right = lowPassRightStage1->processSample(right);
		right = lowPassRightStage2->processSample(right);
	} else {
		left  = highPassLeft->processSample(left);
		right = highPassRight->processSample(right);
	}
}

void MultimodeFilter::processModulation()
{
	lowPassLeftStage1->processModulation();
	lowPassRightStage1->processModulation();
	lowPassLeftStage2->processModulation();
	lowPassRightStage2->processModulation();
	highPassLeft->processModulation();
	highPassRight->processModulation();

	// Sync character filter with the modulated cutoff multiplier from stage 1
	if (character != CharacterFilter::STANDARD)
		charFilter->setModulatedValue(lowPassLeftStage1->getModulatedValue());
}

void MultimodeFilter::setFrequency(float frequency) {
	this->frequency = frequency;		
	this->lowPassLeftStage1->setFrequency(frequency);
	this->lowPassRightStage1->setFrequency(frequency);
	this->lowPassLeftStage2->setFrequency(frequency);
	this->lowPassRightStage2->setFrequency(frequency);
	this->highPassLeft->setFrequency(frequency);
	this->highPassRight->setFrequency(frequency);
	charFilter->setFrequency(frequency);
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
	charFilter->setFrequencyImmediate(frequency);
}

void MultimodeFilter::setResonance(float resonance) {
	this->resonance = resonance;
	lowPassLeftStage1->setResonance(resonance);
	lowPassRightStage1->setResonance(resonance);
	lowPassLeftStage2->setResonance(resonance);
	lowPassRightStage2->setResonance(resonance);
	highPassLeft->setResonance(resonance);
	highPassRight->setResonance(resonance);
	charFilter->setResonance(resonance);
}







