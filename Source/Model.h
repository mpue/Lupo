/*
  ==============================================================================

    Model.h
    Created: 19 Mar 2020 7:08:09pm
    Author:  mpue

  ==============================================================================
*/

#pragma once

#include "AudioEngine/MultimodeOscillator.h"

class Model {

public:

	Model() {
		
		cutoff = 18000.0f;
		resonance = 0.5f;
		mainVolume = 1.0f;
		envAmt = 0.5f;
		
		osc1Pitch = 0;
		osc1Fine = 0;
		osc1Volume = 0.5f;
		osc1Pan = 0;
		osc1Shape = MultimodeOscillator::OscMode::SAW;
		osc2Pitch = 0;
		osc2Fine = 0;
		osc2Volume = 0.5f;
		osc2Pan = 0;
		osc2Shape = MultimodeOscillator::OscMode::SAW;
		osc3Pitch = 0;
		osc3Fine = 0;
		osc3Pan = 0;
		osc3Volume = 0.5f;
		osc3Shape = MultimodeOscillator::OscMode::SAW;
		osc4Pitch = 0;
		osc4Fine = 0;
		osc4Volume = 0.5f;
		osc4Pan = 0;
		osc4Shape = MultimodeOscillator::OscMode::SAW;

	}

	~Model() {}


	float cutoff;
	float resonance;
	float mainVolume;
	float envAmt;
	
	float ampAttack = 0;
	float ampDecay = 1;
	float ampSustain = 0.1f;
	float ampRelease = 0.1f;

	float fltAttack = 0;
	float fltDecay = 1;
	float fltSustain = 0.1f;
	float fltRelease = 0.1f;

	float auxAttack = 0;
	float auxDecay = 1;
	float auxSustain = 0.1f;
	float auxRelease = 0.1f;

	float osc1Pitch;
	float osc1Fine;
	float osc1Volume;
	float osc1Pan;

	MultimodeOscillator::OscMode osc1Shape;

	float osc2Pitch;
	float osc2Fine;
	float osc2Volume;
	float osc2Pan;
	MultimodeOscillator::OscMode osc2Shape;

	float osc3Pitch;
	float osc3Fine;
	float osc3Volume;
	float osc3Pan;
	MultimodeOscillator::OscMode osc3Shape;

	float osc4Pitch;
	float osc4Fine;
	float osc4Volume;
	float osc4Pan;
	MultimodeOscillator::OscMode osc4Shape;

	
};