/*
  ==============================================================================

    LupoSynth.h
    Created: 14 Mar 2020 12:33:42pm
    Author:  mpue

  ==============================================================================
*/

#pragma once

#include "OscGroup.h"
#include "ParametricEQ.h"
#include "StepSequencer.h"
#include "EQAutomationTrack.h"
#include "ChordManager.h"
#include "ADSR.h"
#include "MultimodeFilter.h"
#include "Voice.h"
#include "Oszillator.h"
#include "StereoDelay.h"
#include "StereoReverb.h"
#include "StereoChorus.h"
#include "Distortion.h"
#include "Arpeggiator.h"
#include "OscGroup.h"
#include "../Model.h"
#include "../ModMatrix.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ModTargetGroup.h"
#include "PanModTarget.h"
#include "ScalarModulator.h"

class LupoSynth : public ChangeListener, 
			      public AudioProcessorValueTreeState::Listener, 
				  public AudioProcessorParameter::Listener,
				  public ChangeBroadcaster
{
	
public:

	LupoSynth(Model* model, ModMatrix* modMatrix);
	~LupoSynth();

	Arpeggiator* getArpeggiator();
	Voice* findFreeVoice(int noteNumber);
	ModMatrix* getModMatrix();

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);	
	void processMidi(MidiBuffer& midiMessages);
	void prepareToPlay(double sampleRate, int bufferSize);	
	void updateState(ValueTree state, juce::String modMatrixState);
	std::shared_ptr<MultimodeOscillator> createOscillator(Oszillator::OscMode mode);
	void configureOscillators(Oszillator::OscMode mode1, Oszillator::OscMode mode2, Oszillator::OscMode mode3, Oszillator::OscMode mode4);
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void parameterChanged(const String& parameterID, float newValue) override;
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
	bool cutoffLink = false;
	bool running = true;
	void updateMatrix();
	void configureModulation();

	/** Pass the host's AudioPlayHead so the arpeggiator can sync to host transport. */
	void setPlayHead(juce::AudioPlayHead* ph);
	ParametricEQ*       getEQ()    { return eq.get(); }
	StepSequencer*      getSeq()   { return seq.get(); }
	EQAutomationTrack*  getEqAuto()     { return eqAuto.get(); }
	ChordManager*       getChordManager(){ return chordManager.get(); }

	void setGlobalTranspose(int semitones) { globalTranspose = juce::jlimit(-24, 24, semitones); }
	int  getGlobalTranspose() const        { return globalTranspose; }

	/** Immediately silence all voices and reset arpeggiator note state. */
	void stopAllNotes();

	// Methods for real-time envelope visualization
	Voice* getActiveVoice();
	int getAmpEnvelopeState();
	int getFilterEnvelopeState();
	int getModEnvelopeState(int index);
	float getAmpEnvelopeValue();
	float getFilterEnvelopeValue();
	float getModEnvelopeValue(int index);

	float getLeftPeak();
	float getRighPeak();

private:

	Model* model;	
	using ADSRPtr = std::shared_ptr<SynthLab::ADSR>;
	vector<ADSRPtr> modEnvelopes;
	double sampleRate;
	int bufferSize;
	std::vector<std::shared_ptr<Voice>> voices;
	float filterMode = 1.0f;
	float cutoff = 15000.0f;
	float resonance = 1.0f;
	int currentSample = 0;

	std::shared_ptr<OscGroup> oscGroup1;
	std::shared_ptr<OscGroup> oscGroup2;
	std::shared_ptr<OscGroup> oscGroup3;
	std::shared_ptr<OscGroup> oscGroup4;

	std::shared_ptr<MultimodeOscillator> lfo1;
	std::shared_ptr<MultimodeOscillator> lfo2;
	std::shared_ptr<MultimodeOscillator> lfo3;

	std::shared_ptr<ScalarModulator> modWheelMod;
	std::shared_ptr<ScalarModulator> velocityMod;

	std::unique_ptr<StereoDelay> delay;
	std::unique_ptr<StereoReverb> reverb;
	std::unique_ptr<StereoChorus> chorus = nullptr;
	std::unique_ptr<Distortion>	distortion;
	std::unique_ptr<Arpeggiator> arp;
	
	std::unique_ptr<ParametricEQ>       eq;
	std::unique_ptr<StepSequencer>      seq;
	std::unique_ptr<EQAutomationTrack>  eqAuto;
	std::unique_ptr<ChordManager>       chordManager;

	std::shared_ptr<ModTargetGroup> filterTargetGroup1 = nullptr;
	std::shared_ptr<ModTargetGroup> filterTargetGroup2 = nullptr;

	std::shared_ptr<PanModTarget> panGroup1;
	std::shared_ptr<PanModTarget> panGroup2;
	std::shared_ptr<PanModTarget> panGroup3;
	std::shared_ptr<PanModTarget> panGroup4;


	ModMatrix* matrix;

	int highestNote = 0;
	int globalTranspose = 0;

	float mainVolume = 1.0f;
	float* leftOut;
	float* rightOut;

	float elapsed;
	float lastTime;
	float currentTime;
	float deltaTime;

	float volume[4];
	float pan[4];

	float leftPeak = 0.0f;
	float rightPeak = 0.0f;

	bool prepared = false;
    bool eqAutoNotesWereActive = false;
};
