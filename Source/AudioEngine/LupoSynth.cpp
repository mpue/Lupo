/*
  ==============================================================================

	LupoSynth.cpp
	Created: 14 Mar 2020 12:33:42pm
	Author:  mpue

	This file is part of Lupo, a software synthesizer.

	It is the main synthesizer class that manages voices, oscillators, filters,
	envelopes, LFOs, effects, and the modulation matrix.

  ==============================================================================
*/

#include "LupoSynth.h"
#include "LowPassFilter.h"
#include "MultimodeOscillator.h"
#include "Sampler.h"
#include "../ModMatrix.h"
#include "Modulation.h"


#ifndef M_PI
#define M_PI       3.14159265358979323846 
#endif

LupoSynth::LupoSynth(Model* model, ModMatrix* modMatrix) {
	this->model = model;
	this->matrix = modMatrix;

	for (int i = 0; i < 3; i++) {
		modEnvelopes.emplace_back(std::make_unique<SynthLab::ADSR>());
	}

	delay = std::make_unique <StereoDelay>();
	reverb = std::make_unique <StereoReverb>();
	distortion = std::make_unique <Distortion>();

	// Initialize distortion with model values
	distortion->controls.mode = static_cast<int>(model->distMode);
	distortion->controls.drive = model->distDrive;
	distortion->controls.mix = model->distMix;

	arp = std::make_unique<Arpeggiator>();
	chorus = std::make_unique<StereoChorus>(sampleRate, bufferSize);

	modMatrix->registerSource("LFO1", 1);
	modMatrix->registerSource("LFO2", 2);
	modMatrix->registerSource("LFO3", 3);
	modMatrix->registerSource("ENV 1", 4);
	modMatrix->registerSource("ENV 2", 5);

	modMatrix->registerTarget("Osc 1 pitch", 1);
	modMatrix->registerTarget("Osc 2 pitch", 2);
	modMatrix->registerTarget("Osc 3 pitch", 3);
	modMatrix->registerTarget("Osc 4 pitch", 4);
	modMatrix->registerTarget("Filter 1 cutoff", 5);
	modMatrix->registerTarget("Filter 2 cutoff", 6);
	modMatrix->registerTarget("Osc 1 PWM", 7);

	oscGroup1 = std::make_unique<OscGroup>();
	oscGroup2 = std::make_unique<OscGroup>();
	oscGroup3 = std::make_unique<OscGroup>();
	oscGroup4 = std::make_unique<OscGroup>();

	configureOscillators(Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW);
	configureModulation();
}

LupoSynth::~LupoSynth()
{
	if (oscGroup1) oscGroup1->clearModulators();
	if (oscGroup1) oscGroup1->getTargets().clear();
	if (oscGroup2) oscGroup2->clearModulators();
	if (oscGroup2) oscGroup2->getTargets().clear();
	if (oscGroup3) oscGroup3->clearModulators();
	if (oscGroup3) oscGroup3->getTargets().clear();
	if (oscGroup4) oscGroup4->clearModulators();
	if (oscGroup4) oscGroup4->getTargets().clear();
	if (filterTargetGroup1) filterTargetGroup1->clearTargets();
	if (filterTargetGroup2) filterTargetGroup2->clearTargets();
	if (oscPwmTarget) oscPwmTarget->clearTargets();
}

std::shared_ptr<MultimodeOscillator> LupoSynth::createOscillator(Oszillator::OscMode mode) {

	std::shared_ptr<MultimodeOscillator> osc = nullptr;

	switch (mode) {
	case Oszillator::OscMode::SAW: {
		osc = std::make_shared<MultimodeOscillator>(sampleRate, bufferSize);
		osc->setMode(Oszillator::OscMode::SAW);
		break;
	}
	case Oszillator::OscMode::SINE: {
		osc = std::make_shared<MultimodeOscillator>(sampleRate, bufferSize);
		osc->setMode(Oszillator::OscMode::SINE);
		break;
	}
	default:
		break;
	}

	if (osc != nullptr) {
		osc->setPitch(0);
	}

	return osc;
}

void LupoSynth::configureOscillators(Oszillator::OscMode mode1, Oszillator::OscMode mode2, Oszillator::OscMode mode3, Oszillator::OscMode mode4) {

	const int maxVoices = model->maxVoices;

	voices.clear();

	for (int i = 0; i < maxVoices; i++) {
		auto v = std::make_shared<Voice>(sampleRate);

		std::shared_ptr<MultimodeOscillator> osc1 = createOscillator(mode1);
		std::shared_ptr<MultimodeOscillator> osc2 = createOscillator(mode2);
		std::shared_ptr<MultimodeOscillator> osc3 = createOscillator(mode3);
		std::shared_ptr<MultimodeOscillator> osc4 = createOscillator(mode4);

		oscGroup1->addTarget(osc1);
		oscGroup2->addTarget(osc2);
		oscGroup3->addTarget(osc3);
		oscGroup4->addTarget(osc4);

		v->addOszillator(osc1, 0);
		v->addOszillator(osc2, 1);
		v->addOszillator(osc3, 2);
		v->addOszillator(osc4, 3);

		voices.push_back(std::move(v));
	}
}

Voice* LupoSynth::findFreeVoice(int noteNumber) {
	for (auto& v : voices) {
		if (!v->isPlaying() && v->getAmpEnvelope()->getState() == SynthLab::ADSR::env_idle) {
			return v.get();
		}
	}
	return nullptr;
}

void LupoSynth::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	this->sampleRate = sampleRate;
	this->bufferSize = samplesPerBlock;

	chorus->prepareToPlay(sampleRate, samplesPerBlock);
	reverb->setSampleRate(sampleRate);
	arp->prepareToPlay(sampleRate, bufferSize);
	arp->setEnabled(false);
	arp->setClockMode(Arpeggiator::ClockMode::Internal);

	for (auto& v : voices) {
		// Initialize filters with proper sample rate and initial values
		v->getFilter1()->coefficients(sampleRate, model->cutoff1, model->resonance1);
		v->getFilter2()->coefficients(sampleRate, model->cutoff2, model->resonance2);
		v->setSampleRate(sampleRate);
		v->getFilter1()->addModulator(v->getFilterEnvelope());
		// Connect auxiliary envelope to filter 2 if envAmt2 > 0
		if (model->envAmt2 > 0.0f) {
			modEnvelopes.at(1)->setModAmount(model->envAmt2);
			v->getFilter2()->addModulator(modEnvelopes.at(1));
		}
		v->getFilterEnvelope()->setAttackRate(0);
	}

	lfo1->setSampleRate(sampleRate);
	lfo2->setSampleRate(sampleRate);
	lfo3->setSampleRate(sampleRate);

	// Sync oscillator enabled flags from the model so the synth produces sound
	// even when the editor has never been opened (UI panels normally do this via
	// sendChangeMessage -> changeListenerCallback, but without a UI that path is
	// never triggered).
	for (auto& v : voices) {
		v->getOscillator(0)->enabled = model->osc1Enabled;
		v->getOscillator(1)->enabled = model->osc2Enabled;
		v->getOscillator(2)->enabled = model->osc3Enabled;
		v->getOscillator(3)->enabled = model->osc4Enabled;
	}
}

void LupoSynth::processMidi(MidiBuffer& midiMessages) {

	MidiMessage m;
	int time;

	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
	{

		if (m.isNoteOn())
		{
			int noteNumber = m.getNoteNumber();

			// First, check if this note is already playing and steal that voice
			// (handles fast repeated notes where note-on arrives before note-off)
			for (auto& voice : voices) {
				if (voice->isPlaying() && voice->getNoteNumber() == noteNumber) {
					voice->getAmpEnvelope()->gate(false);
					voice->getFilterEnvelope()->gate(false);
					voice->setPlaying(false);
					break;
				}
			}

			Voice* voice = findFreeVoice(noteNumber);

			// Voice stealing: if no free voice, steal the oldest one in release phase
			if (voice == nullptr) {
				for (auto& v : voices) {
					if (!v->isPlaying() && v->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {
						voice = v.get();
						voice->getAmpEnvelope()->reset();
						voice->getFilterEnvelope()->reset();
						break;
					}
				}
			}

			// Last resort: steal any voice (oldest playing)
			if (voice == nullptr && !voices.empty()) {
				voice = voices[0].get();
				voice->getAmpEnvelope()->reset();
				voice->getFilterEnvelope()->reset();
				voice->setPlaying(false);
			}

			if (noteNumber > highestNote) {
				highestNote = noteNumber;
			}

			if (voice) {
				// Retrigger modulation envelopes on every note-on so the filter
				// envelope restarts regardless of how many voices are already playing.
				for (auto& env : modEnvelopes) {
					env->reset();
					env->gate(true);
				}

				voice->setPlaying(true);
				voice->getAmpEnvelope()->reset();
				voice->getFilterEnvelope()->reset();
				voice->getAmpEnvelope()->gate(true);
				voice->getFilterEnvelope()->gate(true);
				voice->setNoteAndVelocity(noteNumber, m.getVelocity());
				voice->setDuration(250);
				voice->setTime(elapsed);
				lfo1->reset();
				lfo2->reset();
				lfo3->reset();
			}

		}
		else if (m.isAllNotesOff() || m.isAllSoundOff())
		{
			// Release ALL voices
			for (auto& voice : voices) {
				if (voice->isPlaying()) {
					voice->getAmpEnvelope()->gate(false);
					voice->getFilterEnvelope()->gate(false);
					voice->setPlaying(false);
				}
			}
			for (auto& env : modEnvelopes) {
				env->gate(false);
			}
			highestNote = 0;
		}
		else if (m.isNoteOff())
		{
			int noteNumber = m.getNoteNumber();

			for (auto& voice : voices) {
				if (voice->isPlaying() && voice->getNoteNumber() == noteNumber) {
					voice->getAmpEnvelope()->gate(false);
					voice->getFilterEnvelope()->gate(false);
					voice->setPlaying(false);
					break;
				}
			}

			// Recalculate highest note and check if any voices are still playing
			highestNote = 0;
			bool anyPlaying = false;
			for (auto& voice : voices) {
				if (voice->isPlaying()) {
					anyPlaying = true;
					if (voice->getNoteNumber() > highestNote)
						highestNote = voice->getNoteNumber();
				}
			}

			if (!anyPlaying) {
				for (auto& env : modEnvelopes) {
					env->gate(false);
				}
			}
		}
	}
	if (m.isAftertouch())
	{
	}
	if (m.isPitchWheel())
	{
		int pitch = m.getPitchWheelValue();

		float nPitch = ((float)pitch - (float)0x3fff / 2.0) / 8192;
		float semitones = 3;

		nPitch = (nPitch * semitones) / 12;
		nPitch = pow(2, nPitch);

		for (auto& voice : voices)
			if (voice->isPlaying())
				voice->setPitchBend(nPitch);

	}
	// TODO : Implement aftertouch and modulation wheel handling
	if (m.isController()) {

		// Modulation wheel
		if (m.getControllerNumber() == 1) {

		}
	}

	for (auto& voice : voices) {
		voice->getFilter1()->setKeyTrack(highestNote);
		voice->getFilter2()->setKeyTrack(highestNote);
	}

}

void LupoSynth::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {

	for (auto i = 0; i < 2; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	this->arp->processBlock(buffer, midiMessages);
	this->processMidi(midiMessages);

	const int numSamples = buffer.getNumSamples();
	auto* leftChannel = buffer.getWritePointer(0);
	auto* rightChannel = buffer.getWritePointer(1);

	for (auto& env : modEnvelopes) {
		// Process each modulation envelope for the entire block
		for (int sample = 0; sample < numSamples; ++sample) {
			env->process();
		}
	}

	// Process LFOs for the entire block
	for (int sample = 0; sample < numSamples; ++sample) {
		lfo1->process();
		lfo2->process();
		lfo3->process();
	}

	// Temp-Buffer für Stereo-Voice-Verarbeitung
	AudioBuffer<float> voiceBuffer(2, numSamples);

	for (auto& voice : voices) {
		if (voice->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {
			voiceBuffer.clear();

			// Voice verarbeitet den ganzen Block in einem Durchgang
			voice->processBlock(voiceBuffer);

			// Stereo-Mix mit Skalierung
			const float gain = mainVolume;
			for (int sample = 0; sample < numSamples; ++sample) {
				leftChannel[sample] += voiceBuffer.getSample(0, sample) * gain;
				rightChannel[sample] += voiceBuffer.getSample(1, sample) * gain;
			}
		}
	}

	distortion->controls.mode = static_cast<int>(model->distMode);
	distortion->controls.drive = model->distDrive;
	distortion->controls.mix = static_cast<float>(model->distMix);

	// Distortion processing - only apply if enabled (mix > 0 or drive > 1)
	if (model->distMix > 0.0f || model->distDrive > 1.0f) {
		for (int sample = 0; sample < numSamples; ++sample) {
			leftChannel[sample] = distortion->processSample(leftChannel[sample]);
			rightChannel[sample] = distortion->processSample(rightChannel[sample]);
		}
	}

	leftOut = buffer.getWritePointer(0);
	rightOut = buffer.getWritePointer(1);

	if (chorus == nullptr) {
		chorus = std::make_unique<StereoChorus>(sampleRate, bufferSize);
		chorus->setEnabled(true);
	}
	chorus->processStereo(leftOut, rightOut, buffer.getNumSamples());
	delay->processStereo(leftOut, rightOut, buffer.getNumSamples());

	if (model->reverbEnabled) {
		reverb->processStereo(leftOut, rightOut, buffer.getNumSamples());
	}

	float masterGain = Decibels::decibelsToGain(model->outputGaindB);
	buffer.applyGain(masterGain);

	// for the peak meters in the GUI
	leftPeak = buffer.getMagnitude(0, 0, buffer.getNumSamples());
rightPeak = buffer.getMagnitude(1, 0, buffer.getNumSamples());

}

void LupoSynth::updateState(ValueTree state, juce::String modMatríxState) {

	model->gridState = modMatríxState;

	// Per-instance notification: store state in Model for the UI to pick up
	model->pendingModMatrixState = modMatríxState;
	model->modMatrixStateChanged.store(true);

	for (int j = 0; j < state.getNumChildren(); j++) {

		String parameterID = state.getChild(j).getProperty("id");
		float newValue = state.getChild(j).getProperty("value");

		Logger::getCurrentLogger()->writeToLog(parameterID + ": " + String(newValue));

		const int index = parameterID.substring(parameterID.lastIndexOf("_") + 1).getIntValue();

		if (parameterID.startsWith("Amount")) {
			model->modAmount[index] = newValue;
		}
		else if (parameterID.startsWith("Target")) {
			model->setModTarget(index, (int)newValue);
		}
		else if (parameterID.startsWith("Source")) {
			model->setModSource(index, (int)newValue);
		}
		else {
			parameterChanged(parameterID, newValue);
		}
	}

	if (model->osc1Sync) {
		for (auto& voice : voices) {
			voice->getOscillator(0)->setSlave(voice->getOscillator(1));
			voice->getOscillator(0)->setSync(true);
		}
	}

	updateMatrix();
}

Arpeggiator* LupoSynth::getArpeggiator()
{
	return arp.get();
}

void LupoSynth::changeListenerCallback(ChangeBroadcaster* source) {
	// updateState();
	for (auto& voice : voices) {
		voice->getOscillator(0)->enabled = model->osc1Enabled;
		voice->getOscillator(1)->enabled = model->osc2Enabled;
		voice->getOscillator(2)->enabled = model->osc3Enabled;
		voice->getOscillator(3)->enabled = model->osc4Enabled;
	}
}

void LupoSynth::parameterChanged(const String& parameterID, float newValue)
{
	bool matrixChanged = false;

	if (parameterID == "cutoff1")
	{
		model->cutoff1 = newValue;

		for (auto& voice : voices) {
			// Use immediate update for real-time cutoff control responsiveness
			voice->getFilter1()->setFrequencyImmediate(model->cutoff1);
		}

		// Handle cutoff linking
		if (cutoffLink) {
			model->cutoff2 = newValue;
			for (auto& voice : voices) {
				voice->getFilter2()->setFrequencyImmediate(model->cutoff1);
			}
		}
	}
	else if (parameterID == "resonance1") {
		model->resonance1 = newValue;

		for (auto& voice : voices) {
			voice->getFilter1()->setResonance(model->resonance1);
			// Remove redundant coefficients call
		}
	}
	else if (parameterID == "filterMode1") {
		if (newValue == 0.0f) {
			for (auto& voice : voices) {
				voice->getFilter1()->setMode(MultimodeFilter::LOWPASS);
			}
		}
		else if (newValue == 1.0f) {
			for (auto& voice : voices) {
				voice->getFilter1()->setMode(MultimodeFilter::HIGHPASS);
			}
		}
	}
	else if (parameterID == "envAmt1") {
		for (auto& voice : voices) {
			voice->getFilterEnvelope()->setModAmount(newValue);
		}
	}
	else if (parameterID == "cutoff2") {
		model->cutoff2 = newValue;

		for (auto& voice : voices) {
			voice->getFilter2()->setFrequencyImmediate(newValue);
		}

		if (cutoffLink) {
			model->cutoff1 = newValue;
			for (auto& voice : voices) {
				voice->getFilter1()->setFrequencyImmediate(newValue);
			}
		}
	}
	else if (parameterID == "resonance2") {
		model->resonance2 = newValue;

		for (auto& voice : voices) {
			voice->getFilter2()->setResonance(model->resonance2);
			// Remove redundant coefficients call
		}
	}
	else if (parameterID == "filterMode2") {
		if (newValue == 0.0f) {
			for (auto& voice : voices) {
				voice->getFilter2()->setMode(MultimodeFilter::LOWPASS);
			}
		}
		else if (newValue == 1.0f) {
			for (auto& voice : voices) {
				voice->getFilter2()->setMode(MultimodeFilter::HIGHPASS);
			}
		}
	}
	else if (parameterID == "envAmt2") {
		model->envAmt2 = newValue;
		for (auto& voice : voices) {
			voice->getFilter2()->clearModulators();
			if (newValue > 0.0f) {
				modEnvelopes.at(1)->setModAmount(newValue);
				voice->getFilter2()->addModulator(modEnvelopes.at(1));
			}
		}
	}
	else if (parameterID == "mainVolume") {
		mainVolume = newValue;
	}
	else if (parameterID == "ampAttack") {
		float attack = jmax(0.01f, newValue);
		for (auto& voice : voices) {
			voice->getAmpEnvelope()->setAttackRate(attack * sampleRate);
		}
	}
	else if (parameterID == "ampDecay") {
		for (auto& voice : voices) {
			voice->getAmpEnvelope()->setDecayRate(newValue * sampleRate);
		}
	}
	else if (parameterID == "ampSustain") {
		for (auto& voice : voices) {
			voice->getAmpEnvelope()->setSustainLevel(newValue);
		}
	}
	else if (parameterID == "ampRelease") {
		for (auto& voice : voices) {
			voice->getAmpEnvelope()->setReleaseRate(newValue * sampleRate);
		}
	}
	else if (parameterID == "auxAttack1") {
		float attack = jmax(0.01f, newValue);
		for (auto& voice : voices) {
			voice->getFilterEnvelope()->setAttackRate(attack * sampleRate);
		}
	}
	else if (parameterID == "auxDecay1") {
		for (auto& voice : voices) {
			voice->getFilterEnvelope()->setDecayRate(newValue * sampleRate);
		}
	}
	else if (parameterID == "auxSustain1") {
		for (auto& voice : voices) {
			voice->getFilterEnvelope()->setSustainLevel(newValue);
		}
	}
	else if (parameterID == "auxRelease1") {
		for (auto& voice : voices) {
			voice->getFilterEnvelope()->setReleaseRate(newValue * sampleRate);
		}
	}
	else if (parameterID == "auxAttack2") {
		modEnvelopes.at(1)->setAttackRate(jmax(0.01f, newValue) * sampleRate);
	}
	else if (parameterID == "auxDecay2") {
		modEnvelopes.at(1)->setDecayRate(newValue * sampleRate);
	}
	else if (parameterID == "auxSustain2") {
		modEnvelopes.at(1)->setSustainLevel(newValue);
	}
	else if (parameterID == "auxRelease2") {
		modEnvelopes.at(1)->setReleaseRate(newValue * sampleRate);
	}
	else if (parameterID == "osc1Shape") {
		for (auto& voice : voices) {
			voice->getOscillator(0)->setMode(newValue);
		}
	}
	else if (parameterID == "osc2Shape") {
		for (auto& voice : voices) {
			voice->getOscillator(1)->setMode(newValue);
		}
	}
	else if (parameterID == "osc3Shape") {
		for (auto& voice : voices) {
			voice->getOscillator(2)->setMode(newValue);
		}
	}
	else if (parameterID == "osc4Shape") {
		for (auto& voice : voices) {
			voice->getOscillator(3)->setMode(newValue);
		}
	}
	else if (parameterID == "osc1Pitch") {
		for (auto& voice : voices) {
			voice->getOscillator(0)->setPitch(newValue);
			voice->updateOscillator(0);
		}
	}
	else if (parameterID == "osc1Fine") {
		for (auto& voice : voices) {
			voice->getOscillator(0)->setFine(newValue);
			voice->updateOscillator(0);
		}
	}
	else if (parameterID == "osc1Volume") {
		for (auto& voice : voices) {
			voice->setOscVolume(0, newValue);
		}
	}
	else if (parameterID == "osc1Pan") {
		for (auto& voice : voices) {
			voice->setOscPan(0, newValue);
		}
	}
	else if (parameterID == "osc1Spread") {
		for (auto& voice : voices) {
			voice->setOscSpread(0, newValue);
		}
	}
	else if (parameterID == "osc2Pitch") {
		for (auto& voice : voices) {
			voice->getOscillator(1)->setPitch(newValue);
			voice->updateOscillator(1);
		}
	}
	else if (parameterID == "osc2Fine") {
		for (auto& voice : voices) {
			voice->getOscillator(1)->setFine(newValue);
			voice->updateOscillator(1);
		}
	}
	else if (parameterID == "osc2Volume") {
		for (auto& voice : voices) {
			voice->setOscVolume(1, newValue);
		}
	}
	else if (parameterID == "osc2Pan") {
		for (auto& voice : voices) {
			voice->setOscPan(1, newValue);
		}
	}
	else if (parameterID == "osc2Spread") {
		for (auto& voice : voices) {
			voice->setOscSpread(1, newValue);
		}
	}
	else if (parameterID == "osc3Pitch") {
		for (auto& voice : voices) {
			voice->getOscillator(2)->setPitch(newValue);
			voice->updateOscillator(2);
		}
	}
	else if (parameterID == "osc3Fine") {
		for (auto& voice : voices) {
			voice->getOscillator(2)->setFine(newValue);
			voice->updateOscillator(2);
		}
	}
	else if (parameterID == "osc3Volume") {
		for (auto& voice : voices) {
			voice->setOscVolume(2, newValue);
		}
	}
	else if (parameterID == "osc3Pan") {
		for (auto& voice : voices) {
			voice->setOscPan(2, newValue);
		}
	}
	else if (parameterID == "osc3Spread") {
		for (auto& voice : voices) {
			voice->setOscSpread(2, newValue);
		}
	}

	else if (parameterID == "osc4Pitch") {
		for (auto& voice : voices) {
			voice->getOscillator(3)->setPitch(newValue);
			voice->updateOscillator(3);
		}
	}
	else if (parameterID == "osc4Fine") {
		for (auto& voice : voices) {
			voice->getOscillator(3)->setFine(newValue);
			voice->updateOscillator(3);
		}
	}
	else if (parameterID == "osc4Volume") {
		for (auto& voice : voices) {
			voice->setOscVolume(3, newValue);
		}
	}
	else if (parameterID == "osc4Pan") {
		for (auto& voice : voices) {
			voice->setOscPan(3, newValue);
		}
	}
	else if (parameterID == "osc4Spread") {
		for (auto& voice : voices) {
			voice->setOscSpread(3, newValue);
		}
	}
	else if (parameterID == "dlyTimeLeft") {
		delay->setDelay(StereoDelay::Channel::LEFT, newValue);
	}
	else if (parameterID == "dlyTimeRight") {
		delay->setDelay(StereoDelay::Channel::RIGHT, newValue);
	}
	else if (parameterID == "dlyFeedback") {
		delay->setFeedback(StereoDelay::Channel::LEFT, newValue);
		delay->setFeedback(StereoDelay::Channel::RIGHT, newValue);
	}
	else if (parameterID == "dlyMix") {
		delay->setMix(StereoDelay::Channel::LEFT, newValue);
		delay->setMix(StereoDelay::Channel::RIGHT, newValue);
	}
	else if (parameterID == "rvbRoomSize") {
		juce::Reverb::Parameters params = reverb->getParameters();
		params.roomSize = newValue;
		reverb->setParameters(params);
	}
	else if (parameterID == "rvbDamping") {
		juce::Reverb::Parameters params = reverb->getParameters();
		params.damping = newValue;
		reverb->setParameters(params);
	}
	else if (parameterID == "rvbWetLevel") {
		juce::Reverb::Parameters params = reverb->getParameters();
		params.wetLevel = newValue;
		reverb->setParameters(params);
	}
	else if (parameterID == "rvbDryLevel") {
		juce::Reverb::Parameters params = reverb->getParameters();
		params.dryLevel = newValue;
		reverb->setParameters(params);
	}
	else if (parameterID == "rvbWidth") {
		juce::Reverb::Parameters params = reverb->getParameters();
		params.width = newValue;
		reverb->setParameters(params);
	}
	else if (parameterID == "rvbFreezeMode") {
		juce::Reverb::Parameters params = reverb->getParameters();
		params.freezeMode = newValue;
		reverb->setParameters(params);
	}
	else if (parameterID == "chrDelay") {
		if (chorus != nullptr)
			chorus->delay = newValue;
	}
	else if (parameterID == "chrModulation") {
		if (chorus != nullptr) {
			chorus->leftMod = newValue;
			chorus->rightMod = newValue;
		}
	}
	else if (parameterID == "chrFeedback") {
		if (chorus != nullptr)
			chorus->feedback = newValue;
	}
	else if (parameterID == "chrMix") {
		if (chorus != nullptr)
			chorus->mix = newValue;
	}
	else if (parameterID == "lfo1Speed") {
		lfo1->setFrequency(newValue);
	}
	else if (parameterID == "lfo1Amount") {
		lfo1->setModAmount(newValue);
	}
	else if (parameterID == "fmAmount") {
	}
	else if (parameterID == "lfo1Shape") {
		lfo1->setMode(newValue);
	}
	else if (parameterID == "lfo2Speed") {
		lfo2->setFrequency(newValue);
	}
	else if (parameterID == "lfo2Amount") {
		lfo2->setModAmount(newValue);
	}
	else if (parameterID == "lfo2Shape") {
		lfo2->setMode(newValue);
	}
	else if (parameterID == "lfo3Speed") {
		lfo3->setFrequency(newValue);
	}
	else if (parameterID == "lfo3Amount") {
		lfo3->setModAmount(newValue);
	}
	else if (parameterID == "lfo3Shape") {
		lfo3->setMode(newValue);
	}
	else if (parameterID == "distDrive") {
		model->distDrive = newValue;
		distortion->controls.drive = newValue;
	}
	else if (parameterID == "distMix") {
		model->distMix = newValue;
		distortion->controls.mix = newValue;
	}
	else if (parameterID == "distMode") {
		model->distMode = newValue;
		distortion->controls.mode = static_cast<int>(newValue);
	}
	else if (parameterID == "filterMode") {
		filterMode = newValue;
		// Apply serial/parallel routing to all voices
		for (auto& voice : voices) {
			if (newValue < 0.5f) {
				voice->setFilterRouting(Voice::FilterRouting::Serial);
			}
			else {
				voice->setFilterRouting(Voice::FilterRouting::Parallel);
			}
		}
	}
	else if (parameterID == "cutoffLink") {
		cutoffLink = newValue > 0;
		// If cutoff link is enabled, sync filter 2 to filter 1
		if (cutoffLink) {
			model->cutoff2 = model->cutoff1;
			for (auto& voice : voices) {
				voice->getFilter2()->setFrequency(model->cutoff1);
			}
		}
	}
	// Arpeggiator parameters
	else if (parameterID == "arpEnabled") {
		arp->setEnabled(newValue > 0.5f);
	}
	else if (parameterID == "arpSpeed") {
		arp->setDivisionIndex(static_cast<int>(newValue));
	}
	else if (parameterID == "arpTempo") {
		arp->setTempo(newValue);
	}
	else if (parameterID == "arpClockMode") {
		if (newValue < 0.5f) {
			arp->setClockMode(Arpeggiator::ClockMode::Internal);
		}
		else {
			arp->setClockMode(Arpeggiator::ClockMode::Midi);
		}
	}
	else if (parameterID == "arpOctaves") {
		arp->setOctaves(static_cast<int>(newValue) + 1);  // 0-3 -> 1-4 octaves
	}
	else if (parameterID == "arpMode") {
		if (newValue < 0.5f) {
			arp->setMode(Arpeggiator::Mode::Up);
		}
		else if (newValue < 1.5f) {
			arp->setMode(Arpeggiator::Mode::Down);
		}
		else {
			arp->setMode(Arpeggiator::Mode::Random);
		}
	}
}

void LupoSynth::parameterValueChanged(int parameterIndex, float newValue)
{

}

void LupoSynth::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}

ModMatrix* LupoSynth::getModMatrix()
{
	return matrix;
}

void LupoSynth::updateMatrix() {
}

void LupoSynth::configureModulation()
{
	lfo1 = std::make_unique<MultimodeOscillator>(sampleRate, bufferSize);
	lfo1->setMode(2.0f);
	lfo2 = std::make_unique<MultimodeOscillator>(sampleRate, bufferSize);
	lfo2->setMode(2.0f);
	lfo3 = std::make_unique<MultimodeOscillator>(sampleRate, bufferSize);
	lfo3->setMode(2.0f);

	lfo1->setModAmount(0);
	lfo2->setModAmount(0);
	lfo3->setModAmount(0);

	lfo1->setFrequency(0);
	lfo2->setFrequency(0);
	lfo3->setFrequency(0);

	for (int i = 0; i < modEnvelopes.size(); i++) {
	modEnvelopes.at(i)->setAttackRate(0 * sampleRate);  // 1 second
	modEnvelopes.at(i)->setDecayRate(1 * sampleRate);
	modEnvelopes.at(i)->setReleaseRate(1 * sampleRate);
	modEnvelopes.at(i)->setSustainLevel(.8);
	modEnvelopes.at(i)->setModAmount(0);
	}

	lfo1->setName("LFO1");
	lfo2->setName("LFO2");
	lfo3->setName("LFO3");

	modEnvelopes.at(0)->setName("ENV1");
	modEnvelopes.at(1)->setName("ENV2");

	matrix->addModulator(lfo1);
	matrix->addModulator(lfo2);
matrix->addModulator(lfo3);
	matrix->addModulator(modEnvelopes.at(0));
	matrix->addModulator(modEnvelopes.at(1));

	matrix->addModTarget(oscGroup1);
	matrix->addModTarget(oscGroup2);
	matrix->addModTarget(oscGroup3);
matrix->addModTarget(oscGroup4);

	filterTargetGroup1 = std::make_shared<ModTargetGroup>();
	filterTargetGroup2 = std::make_shared<ModTargetGroup>();
	for (auto& voice : voices) {
		filterTargetGroup1->addTarget(voice->getFilter1());
		filterTargetGroup2->addTarget(voice->getFilter2());
	}
	oscPwmTarget = std::make_unique<ModTargetGroup>();

	oscPwmTarget->addTarget(oscGroup1);
	oscPwmTarget->addTarget(oscGroup2);
	oscPwmTarget->addTarget(oscGroup3);
	oscPwmTarget->addTarget(oscGroup4);

	matrix->addModTarget(filterTargetGroup1);
	matrix->addModTarget(filterTargetGroup2);
matrix->addModTarget(oscPwmTarget);

	lfo1->enabled = true;
	lfo2->enabled = true;
	lfo3->enabled = true;
}

// Implementation of real-time envelope visualization methods
Voice* LupoSynth::getActiveVoice() {
	// Return the first active voice (playing a note)
	for (auto& voice : voices) {
		if (voice->isPlaying() && voice->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {
			return voice.get();
		}
	}
	return nullptr;
}

int LupoSynth::getAmpEnvelopeState() {
	Voice* activeVoice = getActiveVoice();
	if (activeVoice != nullptr) {
		return activeVoice->getAmpEnvelope()->getState();
	}
	return 0; // idle
}

int LupoSynth::getFilterEnvelopeState() {
	Voice* activeVoice = getActiveVoice();
	if (activeVoice != nullptr) {
		return activeVoice->getFilterEnvelope()->getState();
	}
	return 0; // idle
}

int LupoSynth::getModEnvelopeState(int index) {
	if (index >= 0 && index < modEnvelopes.size()) {
		return modEnvelopes[index]->getState();
	}
	return 0; // idle
}

float LupoSynth::getAmpEnvelopeValue() {
	Voice* activeVoice = getActiveVoice();
	if (activeVoice != nullptr) {
		return activeVoice->getAmpEnvelope()->getOutput();
	}
	return 0.0f;
}

float LupoSynth::getFilterEnvelopeValue() {
	Voice* activeVoice = getActiveVoice();
	if (activeVoice != nullptr) {
		return activeVoice->getFilterEnvelope()->getOutput();
	}
	return 0.0f;
}

float LupoSynth::getModEnvelopeValue(int index) {
	if (index >= 0 && index < modEnvelopes.size()) {
		return modEnvelopes[index]->getOutput();
	}
	return 0.0f;
}

float LupoSynth::getLeftPeak()
{
	return leftPeak;
}

float LupoSynth::getRighPeak()
{
	return rightPeak;
}

void LupoSynth::setPlayHead(juce::AudioPlayHead* ph)
{
	arp->setPlayHead(ph);
}
