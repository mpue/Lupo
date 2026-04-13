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
#include <cmath>

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
    
    this->filter1 = std::make_shared<MultimodeFilter>();
    this->filter2 = std::make_shared<MultimodeFilter>();

    ampEnvelope->setAttackRate(0 * sampleRate);
    ampEnvelope->setDecayRate(1 * sampleRate);
    ampEnvelope->setReleaseRate(1 * sampleRate);
    ampEnvelope->setSustainLevel(0.8f);

    filterEnvelope->setAttackRate(0 * sampleRate);
    filterEnvelope->setDecayRate(1 * sampleRate);
    filterEnvelope->setReleaseRate(1 * sampleRate);
    filterEnvelope->setSustainLevel(0.0f);
    filterEnvelope->setTargetRatioA(0.1f);
    filterEnvelope->setTargetRatioDR(0.1f);
  
}

void Voice::setNoteAndVelocity(int note, int velocity) {

	this->noteNumber = note;
	this->velocity = velocity;

    for (int i = 0; i < 4; i++) {
    int index = noteNumber + oscillators[i]->getPitch();
        if (index >= 0 && index < 128)
	    oscillators[i]->setFrequency((midiNote[index]) * pitchBend);
    }

}

void Voice::setPitchBend(float bend) {
    this->pitchBend = bend;
	for (int i = 0; i < 4;i++) {
        int index = noteNumber + oscillators[i]->getPitch();
        if (index >= 0 && index < 128)
            oscillators[i]->setFrequency((midiNote[index]) * pitchBend);
    }
}

void Voice::addOszillator(std::shared_ptr<MultimodeOscillator> o, int index) {
    this->oscillators.push_back(std::move(o));
}

Oszillator* Voice::getOscillator(int num) {
	return oscillators.at(num).get();
}

void Voice::processBlock(AudioBuffer<float>& buffer) {
    const int numSamples = buffer.getNumSamples();
    auto* leftChannel  = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    if (ampEnvelope->getState() == SynthLab::ADSR::env_idle) {
        ampEnvelope->reset();
        if (filterEnvelope->getState() != SynthLab::ADSR::env_idle)
            filterEnvelope->reset();
        return;
    }

    // Filter2 uses a modEnvelope processed block-level in LupoSynth
    filter2->processModulation();

    for (int sample = 0; sample < numSamples; ++sample) {
        float amplitude    = (velocity / 127.0f) * ampEnvelope->process();
        float filterEnvVal = filterEnvelope->process();

        // Per-sample filter1 modulation: envelope opens filter upward by up to 4 octaves
        float modMult = std::exp2f(filterEnvVal * filterEnvelope->getModAmount() * 4.0f);
        filter1->setCutoffModulation(modMult);

        float outL = 0.0f;
        float outR = 0.0f;

        for (int i = 0; i < 4; i++) {
            if (!oscillators[i]->enabled)
                continue;

            if (i == 1 && oscillators[i - 1]->enabled && oscillators[i - 1]->isSync()) {
                oscillators[i - 1]->reset();
            }

            float oscSample = oscillators[i]->process();

            if (modulator != nullptr) {
                oscillators[i]->setPitchMod(modulator->getOutput() * this->modAmount);
            }

            float pan       = oscillators[i]->getPan();
            float leftGain  = fast_trig::sin_fast(((float)M_PI * (pan + 1.0f) / 4.0f));
            float rightGain = fast_trig::cos_fast(((float)M_PI * (pan + 1.0f) / 4.0f));

            outL += oscSample * leftGain  * amplitude;
            outR += oscSample * rightGain * amplitude;
        }

        // Apply filters per sample
        if (filterRouting == FilterRouting::Serial) {
            filter1->processSampleStereo(outL, outR);
            filter2->processSampleStereo(outL, outR);
        } else {
            float parL = outL, parR = outR;
            filter1->processSampleStereo(outL, outR);
            filter2->processSampleStereo(parL, parR);
            outL = (outL + parL) * 0.5f;
            outR = (outR + parR) * 0.5f;
        }

        leftChannel[sample]  = outL;
        rightChannel[sample] = outR;
    }
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

void Voice::addModulator(std::shared_ptr<Modulator> mod)
{
    for (int i = 0; i < 4; i++) {
        oscillators[i]->addModulator(mod);
    }
}

void Voice::removeModulator(std::shared_ptr<Modulator> mod)
{
    for (int i = 0; i < 4; i++) {
        oscillators[i]->removeModulator(mod);
    }
}

void Voice::addPwmModulator(std::shared_ptr<Modulator> mod)
{
    for (int i = 0; i < 4; i++) {
        oscillators[i]->addPwmModulator(mod);
	}
}

void Voice::removePwmModulator(std::shared_ptr<Modulator> mod)
{
    for (int i = 0; i < 4; i++) {
        oscillators[i]->removePwmModulator(mod);
	}
}


void Voice::setOctave(int number) {
    this->octave = number;
	for (int i = 0; i < 4; i++) {
        int index = noteNumber + oscillators[i]->getPitch() + this->offset + this->octave * 12;
        if (index >= 0 && index < 128)
            oscillators[i]->setFrequency((midiNote[index]) * pitchBend);
    }
}

void Voice::setOffset(int number) {
    this->offset = number;
	for (int i = 0; i < 4; i++) {
        int index = noteNumber + oscillators[i]->getPitch() + this->offset + this->octave * 12;
     if (index >= 0 && index < 128)
		    oscillators[i]->setFrequency((midiNote[index]) * pitchBend);
    }
}

int Voice::getOctave() const {
    return this->octave;
}

void Voice::updateOscillator(int index) {
	if (this->noteNumber >= 0) {
        int midiIndex = this->noteNumber + oscillators[index]->getPitch();
      if (midiIndex >= 0 && midiIndex < 128)
	        oscillators[index]->setFrequency(midiNote[midiIndex]);
	}
}

void Voice::calculateFrequencyTable() {
    int a = 440; // a is 440 hz...
    for (int x = 0; x < 128; ++x)
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

void Voice::processModulation()
{
    for (int i = 0; i < 4; i++) {
		// oscillators[i]->processModulation();
    }
}

void Voice::setSampleRate(double rate) {
    this->sampleRate = rate;

    // Rate-dependent envelope parameters (times in samples)
    // User-set values (attack/decay/release/sustain) are applied via parameterChanged
    // and will override these defaults. Sustain is NOT rate-dependent, so we don't set it here.
    ampEnvelope->setAttackRate(0 * sampleRate);
    ampEnvelope->setDecayRate(1 * sampleRate);
    ampEnvelope->setReleaseRate(1 * sampleRate);

    filterEnvelope->setAttackRate(0 * sampleRate);
    filterEnvelope->setDecayRate(1 * sampleRate);
    filterEnvelope->setReleaseRate(1 * sampleRate);

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

std::shared_ptr<SynthLab::ADSR> Voice::getFilterEnvelope() {
    return filterEnvelope;
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
