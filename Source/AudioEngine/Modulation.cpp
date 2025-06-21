/*
  ==============================================================================

    Modulation.cpp
    Created: 10 Dec 2016 9:55:40am
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "Modulation.h"

Modulation::Modulation() {
}

Modulation::~Modulation() {

    /*
    for (int i = 0; i < targets.size();i++) {
        targets.at(i)->setModulator(NULL);
    }
    
    
    for(std::vector<ModTarget*>::iterator it = targets.begin(); it != targets.end(); ++it) {
        delete *it;
    }
    */
    
}

Modulation::Modulation(Modulator* modulator, ModTarget* target) {
	this->modulator = modulator;
	
	if (target != nullptr) {
		target->setModulator(modulator);
	}
}

float Modulation::process() {
    if (this->modulator != nullptr) {
        return this->modulator->process();
    }
    return 0;
}

Modulator* Modulation::getModulator() {
    return this->modulator;
}

void Modulation::setModulator(Modulator *modulator) {
    this->modulator = modulator;
}

void Modulation::setTarget(ModTarget* target) {		
	this->target = target;
}

ModTarget* Modulation::getTarget()
{
    return target;
}

void Modulation::clearTarget()
{	
	if (this->target != nullptr) {
		this->target->setModulator(nullptr);
	}

}


void Modulation::setEnabled(bool enabled) {
    this->enabled = enabled;
}

bool Modulation::isEnabled() {
    return this->enabled;
}