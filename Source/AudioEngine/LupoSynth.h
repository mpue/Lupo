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
#include "StereoDelay.h"
#include "StereoReverb.h"
#include "StereoChorus.h"
#include "../Model.h"
#include "../JuceLibraryCode/JuceHeader.h"

class LupoSynth : public ChangeListener, 
			      public AudioProcessorValueTreeState::Listener, 
				  public AudioProcessorParameter::Listener,
				  public ChangeBroadcaster
{
	
public:

	LupoSynth(Model* model);
	~LupoSynth();

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);	
	void processMidi(MidiBuffer& midiMessages);
	void prepareToPlay(double sampleRate, int bufferSize);	
	void updateState();

	Oszillator* createOscillator(Oszillator::OscMode mode);
	void configureOscillators(Oszillator::OscMode mode1, Oszillator::OscMode mode2, Oszillator::OscMode mode3, Oszillator::OscMode mode4);
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void parameterChanged(const String& parameterID, float newValue) override;
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

private:
	Model* model;
	vector<Voice*>* voices;	
	vector<SynthLab::ADSR*>* modEnvelopes;
	float sampleRate;
	int bufferSize;
	MultimodeFilter* filter;
	float cutoff = 15000.0f;
	float resonance = 1.0f;
	int currentSample = 0;
	int numVoices = 0;

	MultimodeOscillator* lfo1;
	MultimodeOscillator* lfo2;

	StereoDelay* delay;
	StereoReverb* reverb;
	StereoChorus* chorus;

	int highestNote = 0;

	float mainVolume = 1.0f;
	float* leftOut;
	float* rightOut;

	float elapsed;
	float lastTime;
	float currentTime;
	float deltaTime;

	float volume[4];
	float pan[4];
};