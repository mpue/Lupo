//
//  MultimodeOscillator.cpp
//  Trio
//
//  Created by Matthias Pueski on 20.11.16.
//
//

#include "MultimodeOscillator.h"



MultimodeOscillator::MultimodeOscillator(float sampleRate,int buffersize) : Oszillator(sampleRate) {
    this->volume = 1.0f;
    this->frequency = 0.0f;
    this->fine = 0.0f;
	this->modAmount = 0.0f;
    this->sine = new Sine(sampleRate, buffersize);
    this->saw = new Sawtooth(sampleRate, buffersize);
    this->pulse = new Pulse(sampleRate, buffersize);
    this->noise = new WhiteNoise(sampleRate, buffersize);
    this->mode = OscMode::SAW;
    this->modulator = nullptr;
	this->pan = 0.0f;
}

MultimodeOscillator::~MultimodeOscillator() {
    delete sine;
    delete saw;
    delete pulse;
    delete noise;
}

void MultimodeOscillator::setFrequency(double frequency) {
    this->saw->setFrequency(frequency);
    this->sine->setFrequency(frequency);
    this->pulse->setFrequency(frequency);
}

void MultimodeOscillator::setVolume(float volume) {
    this->saw->setVolume(volume);
    this->sine->setVolume(volume);
    this->pulse->setVolume(volume);
    this->noise->setVolume(volume);
}

void MultimodeOscillator::setModulator(Modulator* mod) {
    this->modulator = mod;
}

void MultimodeOscillator::setModAmount(float amount) {
    this->modAmount = amount;
}

float MultimodeOscillator::getOutput() {
    if (this->mode == SAW) {
        return this->saw->getOutput();
    }
    else if (this->mode == SINE) {
        return this->sine->getOutput();
    }
    else if (this->mode == PULSE) {
        return this->pulse->getOutput();
    }
    else if(this->mode == NOISE) {
        return this->noise->getOutput();
    }
    else {
        return 0;
    }
}

float MultimodeOscillator::process() {
    
    
    if (this->mode == SAW) {
		if (this->modulator != nullptr) {
			saw->setPitchMod(modulator->getOutput() * modAmount);	
			saw->setFine(fine);
		}
        return this->saw->process();
    }
    else if (this->mode == SINE) {
        return this->sine->process();
    }
    else if (this->mode == PULSE) {
        return this->pulse->process();
    }
    else if(this->mode == NOISE) {
        return this->noise->process();
    }
    else {
        return 0;
    }
    
}

void MultimodeOscillator::reset() {
    this->saw->reset();
    this->pulse->reset();
    this->sine->reset();
}

void MultimodeOscillator::setFine(float fine) {
    this->fine = fine;
    saw->setFine(fine);
    sine->setFine(fine);
    pulse->setFine(fine);
}

float MultimodeOscillator::getFine() const {
    return this->fine;
}

void MultimodeOscillator::setSlave(Oszillator *slave) {
    this->saw->setSlave(slave);
}

void MultimodeOscillator::setSync(bool sync) {
    this->saw->setSync(sync);
}
