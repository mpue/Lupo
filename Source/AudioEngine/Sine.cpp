//
//  Sine.cpp
//  Trio
//
//  Created by Matthias Pueski on 20.11.16.
//
//

#include "Sine.h"
#include "../fasttrig.hpp"
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

Sine::Sine(double sampleRate, int buffersize) : Oszillator(sampleRate) {
    this->volume = 1.0f;
    this->frequency = 440.0f;        
    this->fine = 0.0f;
    this->step = (frequency * 2 * M_PI) / sampleRate;
    this->phase = 0;
    this->value = 0;
}

float Sine::getOutput() {
    return this->value;
}

void Sine::reset() {
    this->phase = 0;
}

float Sine::process() {
    // Phase increment
    phase += step;

    // CRITICAL FIX: Wrap phase to prevent overflow
    while (phase >= TWO_PI) {
        phase -= TWO_PI;
    }

    // CRITICAL FIX: Apply volume to amplitude, not phase
    this->value = fast_trig::sin_fast(phase) * this->volume;

    return this->value;
}

void Sine::setFrequency(double frequency) {
    this->frequency = frequency;
    updateStep();
}

float Sine::getFine() const {
    return this->fine;
}

void Sine::setFine(float fine) {
    this->fine = fine;
    updateStep();
}

void Sine::setSampleRate(double sampleRate) {
    this->sampleRate = sampleRate;
    this->step = ((frequency + static_cast<double>(fine)) * 2.0 * M_PI) / sampleRate;
}

void Sine::updateStep() {
    this->step = ((frequency + static_cast<double>(fine)) * TWO_PI) / sampleRate;
}
