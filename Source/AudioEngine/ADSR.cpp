//
//  ADSR.cpp
//
//  Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//
//  1.01  2016-01-02  njr   added calcCoefs to SetTargetRatio functions that were in the ADSR widget but missing in this code
//

#include "ADSR.h"
#include <math.h>


SynthLab::ADSR::ADSR(void) {
    reset();
    setAttackRate(0.01);
    setDecayRate(1 * 48000);
    setReleaseRate(0);
    setSustainLevel(1.0);
    setTargetRatioA(0.01);
    setTargetRatioDR(0.01);
    setModAmount(1.0);	
}

SynthLab::ADSR::~ADSR(void) {
}

float SynthLab::ADSR::process() {
	switch (state) {
	case env_idle:
		break;
	case env_attack:
		// Handle instantaneous attack when attackCoef is 0
		if (attackCoef == 0.0f) {
			output = 1.0f;  // Jump instantly to full level
			state = env_decay;
		}
		else {
			output = attackBase + output * attackCoef;
			if (output >= 1.0) {
				output = 1.0;
				state = env_decay;
			}
		}
		break;
	case env_decay:
		output = decayBase + output * decayCoef;
		if (output <= sustainLevel) {
			output = sustainLevel;
			state = env_sustain;
		}
		break;
	case env_sustain:
		break;
	case env_release:
		output = releaseBase + output * releaseCoef;
		if (output <= 0.0) {
			output = 0.0;
			state = env_idle;
		}
	}

	return (output / 127.0f) * velocity; // Korrigiert: Division durch 127 statt 128
}

void SynthLab::ADSR::gate(int gate) {

	if (gate) {
		velocity = gate;
		// Explizit output zurücksetzen um Pops zu vermeiden
		if (state == env_idle || state == env_release) {
			output = 0.0f;
		}
		state = env_attack;
	}
	else if (state != env_idle)
		state = env_release;
}

int SynthLab::ADSR::getState() {
	return state;
}

void SynthLab::ADSR::reset() {
	state = env_idle;
	output = 0.0f;
	velocity = 0;
}


void SynthLab::ADSR::setAttackRate(float rate) {
    // Store the actual rate for later use
    float actualRate = rate;
    
    attackRate = rate * 1000;
    
    // Handle zero or very small attack rates for instantaneous attack
    if (actualRate <= 0.0001f) {
        attackCoef = 0.0f;  // Make attack instantaneous
        attackBase = 1.0f;  // Jump directly to full level
    } else {
        attackCoef = calcCoef(rate, targetRatioA);
        attackBase = (1.0 + targetRatioA) * (1.0 - attackCoef);
    }
}

float SynthLab::ADSR::getAttackRate() {
    return this->attackRate;
}

void SynthLab::ADSR::setDecayRate(float rate) {
    // rate /= 32;
    decayRate = rate;
    decayCoef = calcCoef(rate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
}

float SynthLab::ADSR::getDecayRate() {
    return this->decayRate;
}

void SynthLab::ADSR::setReleaseRate(float rate) {
    releaseRate = rate * 1000;
    releaseCoef = calcCoef(rate, targetRatioDR);
    releaseBase = -targetRatioDR * (1.0 - releaseCoef);
}

float SynthLab::ADSR::getReleaseRate() {
    return this->releaseRate;
}

float SynthLab::ADSR::calcCoef(float rate, float targetRatio) {
    return exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}

void SynthLab::ADSR::setSustainLevel(float level) {
    sustainLevel = level;
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
}

float SynthLab::ADSR::getSustainLevel() {
    return this->sustainLevel;
}

void SynthLab::ADSR::setTargetRatioA(float targetRatio) {
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    targetRatioA = targetRatio;
    
    // Recalculate coefficients only if attack rate is not zero
    if (attackRate > 0.0001f) {
        attackCoef = calcCoef(attackRate / 1000.0f, targetRatioA);
        attackBase = (1.0 + targetRatioA) * (1.0 - attackCoef);
    }
}

void SynthLab::ADSR::setTargetRatioDR(float targetRatio) {
    if (targetRatio < 0.000000001)
        targetRatio = 0.000000001;  // -180 dB
    targetRatioDR = targetRatio;
    decayCoef = calcCoef(decayRate, targetRatioDR);
    releaseCoef = calcCoef(releaseRate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0 - decayCoef);
    releaseBase = -targetRatioDR * (1.0 - releaseCoef);
}
