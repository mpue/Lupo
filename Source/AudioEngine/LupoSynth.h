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

class LupoSynth {
	
public:

	LupoSynth(float sampleRate, int bufferSize);
	~LupoSynth();

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);	
	void processMidi(MidiBuffer& midiMessages);

	void prepareToPlay(double sampleRate, int bufferSize);
	void setPitch(int osc, int pitch);

	void setAmpAttack(float attack);
	void setAmpDecay(float decay);
	void setAmpSustain(float sustain);
	void setAmpRelease(float release);

	void setCutoff(float cutoff);
	void setResonance(float resonance);

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