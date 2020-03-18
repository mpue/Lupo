/*
  ==============================================================================

    OscGroup.cpp
    Created: 14 Mar 2020 11:53:07am
    Author:  mpue

  ==============================================================================
*/

#include "OscGroup.h"

OscGroup::OscGroup(double sampleRate, int bufferSize) {
	for (int i = 0; i < 128; i++) {
		saw[i] = new Sawtooth(sampleRate, bufferSize);
		saw[i]->setFrequency(200);
		saw[i]->setVolume(0.5f);
	}
}

OscGroup::~OscGroup() {
	for (int i = 0; i < 128; i++) {
		delete saw[i];
	}
}


float OscGroup::getOutput() {
	return 0;
}

void OscGroup::setPitch(int pitch) {
	for (auto j = 0; j < 128; j++) {
		saw[j]->setPitch(pitch);
	}
	this->pitch = pitch;
}

int OscGroup::getPitch() {
	return pitch;
}

void OscGroup::reset() {
	for (auto j = 0; j < 128; j++) {
		saw[j]->reset();
	}
}


float OscGroup::process(int voice) {
	return saw[voice]->process();
}

void OscGroup::setFrequency(int voice, double frequency) {
	saw[voice]->setFrequency(frequency);
}

void OscGroup::setFine(float fine) {
	for (auto j = 0; j < 128; j++) {
		saw[j]->setFine(fine);
	}
	this->fine = fine;
}

float OscGroup::getFine() const {
	return fine;
}