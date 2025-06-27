/*
  ==============================================================================

    Voice.cpp
    Created: 4 Jun 2016 6:53:08pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "Voice.h"
#include "MultimodeOscillator.h"
#include "../fasttrig.hpp"

#ifndef M_PI
#define M_PI       3.14159265358979323846 
#endif

Voice::Voice(float sampleRate) {

    this->sampleRate = sampleRate;
    this->calculateFrequencyTable();
    this->playing = false;
    this->ampEnvelope = std::make_unique<SynthLab::ADSR>();
    this->filterEnvelope = std::make_unique<SynthLab::ADSR>();
    this->modulator = 0;
    this->pitchBend = 1;
    
    this->filter1 = std::make_unique<MultimodeFilter>();
    this->filter2 = std::make_unique<MultimodeFilter>(); 

    ampEnvelope->setAttackRate(0 * sampleRate);  // 1 second
    ampEnvelope->setDecayRate(1 * sampleRate);
    ampEnvelope->setReleaseRate(1 * sampleRate);
    ampEnvelope->setSustainLevel(.8);

    filterEnvelope->setAttackRate(0 * sampleRate);  // 1 second
    filterEnvelope->setDecayRate(1 * sampleRate);
    filterEnvelope->setReleaseRate(1 * sampleRate);
    filterEnvelope->setSustainLevel(.8);
    filterEnvelope->setTargetRatioA(0.1f);
    filterEnvelope->setTargetRatioDR(0.01f);
  
}

void Voice::applyModulation(float value) 
{    		
	for (int i = 0; i < 4; i++) {
		oscillators[i]->applyModulation(value);
	}
    filter1->applyModulation(value);
    filter2->applyModulation(value);
}

void Voice::setNoteAndVelocity(int note, int velocity) {

	this->noteNumber = note;
	this->velocity = velocity;

    for (int i = 0; i < 4; i++) {
	    oscillators[i]->setFrequency((midiNote[noteNumber + oscillators[i]->getPitch()]) * pitchBend);
    }

}

void Voice::setPitchBend(float bend) {
    this->pitchBend = bend;
	for (int i = 0; i < 4;i++) {        
        oscillators[i]->setFrequency((midiNote[noteNumber + oscillators[i]->getPitch()]) * pitchBend);
    }
}

void Voice::addOszillator(std::unique_ptr<MultimodeOscillator> o, int index) {
    this->oscillators.push_back(std::move(o));
}

Oszillator* Voice::getOscillator(int num) {
	return oscillators.at(num).get();
}

float Voice::process(int channel)
{
    float outL = 0.0f;
    float outR = 0.0f;

    if (ampEnvelope->getState() != SynthLab::ADSR::env_idle) {

        float amplitude = (velocity / 127.0f) * ampEnvelope->process();

        for (int i = 0; i < 4; i++) {
            if (!oscillators[i]->enabled)
                continue;

            if (i == 1 && oscillators[i - 1]->enabled && oscillators[i - 1]->isSync()) {
                oscillators[i - 1]->reset();
            }

            float pan = oscillators[i]->getPan();
            float gain = (channel == 0)
                ? fast_trig::sin_fast(((float)M_PI * (pan + 1.0f) / 4.0f))
                : fast_trig::cos_fast(((float)M_PI * (pan + 1.0f) / 4.0f));

            float sample = oscillators[i]->process();

            if (modulator != nullptr) {
                oscillators[i]->setPitchMod(modulator->getOutput() * this->modAmount);
            }

            if (channel == 0)
                outL += sample * gain * amplitude;
            else
                outR += sample * gain * amplitude;
        }
        
        filter1->processStereo(&outL, &outR, 1);
        // filter2->processStereo(&outL, &outR, 1);
    }
    else {
        ampEnvelope->reset();
    }

    return (channel == 0) ? outL : outR;
}


void Voice::setNoteNumber(int number) {
    this->noteNumber = number;
}

int Voice::getNoteNumber() const {
    return this->noteNumber;
}

void Voice::setPitch(int number) {
    this->pitch = number;
}

int Voice::getPitch() const {
    return this->pitch;
}

void Voice::setOscVolume(int osc, float volume) {
	oscillators[osc]->setVolume(volume);
}

void Voice::setOscPan(int osc,float pan) {
	oscillators[osc]->setPan(pan);
}

void Voice::setOscSpread(int osc, float spread)
{
	MultimodeOscillator* mmo = oscillators.at(osc).get();
	mmo->setSpread(spread);
}

void Voice::setOctave(int number) {
    this->octave = number;
	for (int i = 0; i < 4; i++) {      
        oscillators[i]->setFrequency((midiNote[noteNumber + oscillators[i]->getPitch() + this->offset + this->octave * 12]) * pitchBend);
    }
}

void Voice::setOffset(int number) {
    this->offset = number;
	for (int i = 0; i < 4; i++) {     
		oscillators[i]->setFrequency((midiNote[noteNumber + oscillators[i]->getPitch() + this->offset + this->octave * 12]) * pitchBend);
    }
}

int Voice::getOctave() const {
    return this->octave;
}

void Voice::updateOscillator(int index) {
	if (this->noteNumber >= 0) {
	    oscillators[index]->setFrequency(midiNote[this->noteNumber + oscillators[index]->getPitch()]);
	}
}

void Voice::calculateFrequencyTable() {
    int a = 440; // a is 440 hz...
    for (int x = 0; x < 127; ++x)
    {
        midiNote[x] = a * pow(2.0,(x-69.0)/12.0);
    }
}

void Voice::setPlaying(bool playing) {
    this->playing = playing;
}

bool Voice::isPlaying() const {
    return this->playing;
}

void Voice::setSampleRate(double rate) {
    this->sampleRate = rate;
    ampEnvelope->setAttackRate(0 * sampleRate);  // 1 second
    ampEnvelope->setDecayRate(1 * sampleRate);
    ampEnvelope->setReleaseRate(1 * sampleRate);
    ampEnvelope->setSustainLevel(.8);

    filterEnvelope->setAttackRate(0 * sampleRate);  // 1 second
    filterEnvelope->setDecayRate(1 * sampleRate);
    filterEnvelope->setReleaseRate(1 * sampleRate);
    filterEnvelope->setSustainLevel(.8);

    for (int i = 0; i < 4; i++) {
        oscillators[i]->setSampleRate(rate);
    }
   
}

float Voice::getSampleRate() {
    return this->sampleRate;
}

SynthLab::ADSR* Voice::getAmpEnvelope() {
    return ampEnvelope.get();
}

SynthLab::ADSR* Voice::getFilterEnvelope() {
    return filterEnvelope.get();
}


void Voice::setModulator(Modulator* modulator) {
	
	if (modulator) {
		this->modulator = modulator;
		for (int i = 0; i < 4; i++) {
			MultimodeOscillator* mmo = oscillators.at(i).get();                
			if (mmo != nullptr) {
				mmo->setModulator(modulator);
			}
		}
	}
    this->modulator = modulator;
	
}

float Voice::getTime() {
    return this->time;
}

void Voice::setTime(float time) {
    this->time = time;
}

float Voice::getDuration() {
    return this->duration;
}

void Voice::setDuration(float duration) {
    this->duration = duration;
}

void Voice::setVelocity(int velocity) {
	this->velocity = velocity;
}

int Voice::getVelocity() const {
	return this->velocity;
}
