//
//  Pulse.cpp
//  Trio
//
//  Created by Matthias Pueski on 23.11.16.
//
//

#include "Pulse.h"
#include "..\BlitSquare.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"

Pulse::Pulse(float sampleRate, int buffersize) : Oszillator(sampleRate) {
    this->volume = 1.0f;
    this->frequency = 440.0f;
    this->fine = 0.0f;
    this->step = (frequency * 2 * M_PI) / sampleRate;
    this->phase = 0;
    this->currentModulatedFrequency = this->frequency + this->fine;
	blitSquare = new stk::BlitSquare();
}

void Pulse::reset() {
    this->phase = 0;
	blitSquare->reset();
}

void Pulse::processModulation()
{
    // Process regular modulators (for frequency modulation)
    float frequencyModulation = 0.0f;
    for (auto mod : modulators) {
        frequencyModulation += mod->getOutput() * mod->getModAmount();
    }
    
    // Calculate the new modulated frequency
    float newModulatedFrequency = this->frequency + this->fine + frequencyModulation;
    
    // Only update frequency if it has changed (optimization)
    if (newModulatedFrequency != currentModulatedFrequency) {
        currentModulatedFrequency = newModulatedFrequency;
        blitSquare->setFrequency(currentModulatedFrequency);
    }
    
    // Process PWM modulators (for pulse width modulation)
    float modulatedValue = 0.5f; // Default to 50% duty cycle
    
    if (!pwmModulators.empty()) {
        modulatedValue = 0.0f;
        for (auto mod : pwmModulators) {
            modulatedValue += mod->getOutput() * mod->getModAmount();
        }
        modulatedValue = modulatedValue / (float)pwmModulators.size(); // average
        modulatedValue = (modulatedValue + 1.0f) / 2.0f; // map from -1..1 to 0..1
        modulatedValue = jlimit(0.01f, 0.99f, modulatedValue); // limit to avoid extremes
    }
	
	// Set pulse width on blitSquare
	blitSquare->setPulseWidth(modulatedValue);
}


float Pulse::getOutput() {
    return blitSquare->lastOut();
}

float Pulse::process() {
    processModulation(); // Apply any PWM modulation
	return blitSquare->tick() * volume;
}

void Pulse::setFrequency(double frequency) {
    this->frequency = frequency;
    this->currentModulatedFrequency = frequency + this->fine;
	blitSquare->setFrequency(this->currentModulatedFrequency);
    //this->step = ((frequency + (double)this->fine) * 2 * M_PI / sampleRate);
}

void Pulse::setFine(float fine) {
    this->fine = fine;
    this->currentModulatedFrequency = this->frequency + fine;
	setFrequency(this->frequency);
    // this->step = ((frequency + (double)this->fine) * 2 * M_PI / sampleRate);
}

float Pulse::getFine() const {
    return this->fine;
}

void Pulse::setPulseWidth(float width) {
    blitSquare->setPulseWidth(width);
}

float Pulse::getPulseWidth() const {
    return blitSquare->getPulseWidth();
}

void Pulse::setSampleRate(double sampleRate) {
    this->sampleRate = sampleRate;
    // The BlitSquare doesn't have a setSampleRate method, it uses the global Stk::sampleRate()
    // We would need to call Stk::setSampleRate() here if needed
}
