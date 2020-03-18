/*
  ==============================================================================

    LupoSynth.h
    Created: 14 Mar 2020 12:33:42pm
    Author:  mpue

  ==============================================================================
*/

#pragma once

#include "OscGroup.h"
#include "ADSR.h"
#include "MultimodeFilter.h"
#include "Voice.h"
#include "Oszillator.h"
#include "../MessageBus/BusListener.h"
#include "../MessageBus/Topic.h"
#include "../MessageBus/MessageBus.h"

class LupoSynth : public BusListener {
	
public:

	LupoSynth(float sampleRate, int bufferSize);
	~LupoSynth();

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);	
	void processMidi(MidiBuffer& midiMessages);

	void prepareToPlay(double sampleRate, int bufferSize);
	void topicChanged(Topic* topic) override;
	
	Oszillator* createOscillator(Oszillator::OscMode mode);
	void configureOscillators(Oszillator::OscMode mode1, Oszillator::OscMode mode2, Oszillator::OscMode mode3);

private:
	vector<Voice*> voices;	
	vector<SynthLab::ADSR*> modEnvelopes;
	float sampleRate;
	int bufferSize;
	MultimodeFilter* filter;
	float cutoff;
	float resonance;
	int currentSample = 0;
	int numVoices = 0;

	float* leftOut;
	float* rightOut;

	float elapsed;
	float lastTime;
	float currentTime;
	float deltaTime;

};