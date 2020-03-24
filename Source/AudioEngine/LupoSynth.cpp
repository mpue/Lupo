/*
  ==============================================================================

    LupoSynth.cpp
    Created: 14 Mar 2020 12:33:42pm
    Author:  mpue

  ==============================================================================
*/

#include "LupoSynth.h"
#include "LowPassFilter.h"
#include "MultimodeOscillator.h"
#include "Sampler.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846 
#endif


LupoSynth::LupoSynth(Model* model) {
	this->model = model;

	modEnvelopes = new vector<SynthLab::ADSR*>();

	for (int i = 0; i < 3; i++) {
		SynthLab::ADSR* env = new SynthLab::ADSR();
		modEnvelopes->push_back(env);
	}

	this->filter = new MultimodeFilter();
	filter->setModulator(modEnvelopes->at(0));
	delay = new StereoDelay();
	reverb = new StereoReverb();
	
}

LupoSynth::~LupoSynth() {
	for (int i = 0; i < 3; i++) {
		delete (*modEnvelopes)[i];
	}

	delete modEnvelopes;

	for (std::vector<Voice*>::iterator it = voices->begin(); it != voices->end(); ++it) {
		delete *it;
	}
	voices->clear();
	delete voices;
	delete filter;
	delete delay;
	delete reverb;
	delete chorus;
}

Oszillator* LupoSynth::createOscillator(Oszillator::OscMode mode) {

	Oszillator* osc = nullptr;

	switch (mode) {
	case Oszillator::OscMode::SAW: {
		osc = new MultimodeOscillator(sampleRate, bufferSize);
		osc->setMode(Oszillator::OscMode::SAW);
		break;
	}
	case Oszillator::OscMode::SINE: {
		osc = new MultimodeOscillator(sampleRate, bufferSize);
		osc->setMode(Oszillator::OscMode::SINE);
		break;
	}

	case Oszillator::OscMode::SAMPLE: {
		osc = new Sampler(sampleRate, bufferSize);
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
	
	voices = new vector<Voice*>();

	for (int i = 0; i < 127; i++) {

		Voice* v = new Voice(sampleRate);

		Oszillator* osc1 = createOscillator(mode1);
		Oszillator* osc2 = createOscillator(mode2);
		Oszillator* osc3 = createOscillator(mode3);
		Oszillator* osc4 = createOscillator(mode4);

		v->addOszillator(osc1);
		v->addOszillator(osc2);
		v->addOszillator(osc3);
		v->addOszillator(osc4);

		voices->push_back(v);
	}
}

void LupoSynth::prepareToPlay(double sampleRate, int samplesPerBlock)
{		

	this->sampleRate = sampleRate;
	this->bufferSize = samplesPerBlock;

	filter->coefficients(sampleRate, 15000.0f, 1.0f);
	configureOscillators(Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW);
	
	chorus = new StereoChorus(sampleRate,bufferSize);
	chorus->setEnabled(true);

	for (int i = 0; i < modEnvelopes->size(); i++) {
		modEnvelopes->at(i)->setAttackRate(0 * sampleRate);  // 1 second
		modEnvelopes->at(i)->setDecayRate(1 * sampleRate);
		modEnvelopes->at(i)->setReleaseRate(1 * sampleRate);
		modEnvelopes->at(i)->setSustainLevel(.8);
	}

	updateState();

}

void LupoSynth::processMidi(MidiBuffer& midiMessages) {

	MidiMessage m;
	int time;

	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
	{

		if (m.isNoteOn())
		{

			int noteNumber = m.getNoteNumber();
			if (noteNumber > highestNote) {
				highestNote = noteNumber;
			}

			for (int envIdx = 0; envIdx < this->modEnvelopes->size(); envIdx++) {
				modEnvelopes->at(envIdx)->reset();
				modEnvelopes->at(envIdx)->gate(true);
			}

			if (!voices->at(noteNumber)->isPlaying()) {
				voices->at(noteNumber)->setNoteAndVelocity(noteNumber, m.getVelocity());
				voices->at(noteNumber)->setPlaying(true);
				voices->at(noteNumber)->getAmpEnvelope()->reset();
				voices->at(noteNumber)->getAmpEnvelope()->gate(true);
				voices->at(noteNumber)->setDuration(250);
				voices->at(noteNumber)->setTime(elapsed);

				numVoices++;
			}

		}
		if (m.isNoteOff())
		{
			int noteNumber = m.getNoteNumber();

			voices->at(noteNumber)->setPlaying(false);
			voices->at(noteNumber)->getAmpEnvelope()->gate(false);

			numVoices--;

			if (numVoices == 0) {
				for (int envIdx = 0; envIdx < this->modEnvelopes->size(); envIdx++) {
					modEnvelopes->at(envIdx)->gate(false);
				}
				highestNote = 0;
			}

			// under some circumstances we get negative voices :(
			if (numVoices < 0) {
				numVoices = 0;
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

			for (int i = 0; i < voices->size(); i++) {
				if (voices->at(i)->isPlaying())
					voices->at(i)->setPitchBend(nPitch);
			}

		}
		if (m.isController()) {

			// Modulation wheel
			if (m.getControllerNumber() == 1) {

			}
		}
		else {
			//( Logger::getCurrentLogger()->writeToLog("Other message : " + String(m.getTimeStamp()));
		}

	}

	filter->setKeyTrack(highestNote);

}

void LupoSynth::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {

	this->processMidi(midiMessages);
	
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

		float valueL = 0;
		float valueR = 0;

		for (int i = 0; i < voices->size(); i++) {
			if (voices->at(i)->isPlaying() || voices->at(i)->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {				
				valueL += voices->at(i)->process(0) * mainVolume * 4;				
				valueR += voices->at(i)->process(1) * mainVolume * 4;
			}
		}
		
		buffer.addSample(0, sample, valueL);
		buffer.addSample(1, sample, valueR);
		
		for (int i = 0; i < modEnvelopes->size(); i++) {
			modEnvelopes->at(i)->process();
		}
	}

	leftOut = buffer.getWritePointer(0);
	rightOut = buffer.getWritePointer(1);

	filter->processStereo(leftOut, rightOut, buffer.getNumSamples());
	chorus->processStereo(leftOut, rightOut, buffer.getNumSamples());
	delay->processStereo(leftOut, rightOut,buffer.getNumSamples());
	reverb->processStereo(leftOut, rightOut, buffer.getNumSamples());
}

void LupoSynth::updateState() {
		
	mainVolume = model->mainVolume;
	
	filter->coefficients(sampleRate, model->cutoff, model->resonance);
	filter->setModAmount(model->envAmt);

	delay->setDelay(StereoDelay::Channel::LEFT, model->dlyTimeLeft);
	delay->setDelay(StereoDelay::Channel::RIGHT, model->dlyTimeRight);
	delay->setFeedback(StereoDelay::Channel::LEFT, model->dlyFeedback);
	delay->setFeedback(StereoDelay::Channel::RIGHT, model->dlyFeedback);
	delay->setMix(StereoDelay::Channel::LEFT, model->dlyMix);
	delay->setMix(StereoDelay::Channel::RIGHT, model->dlyMix);

	chorus->delay = model->chrDelay;
	chorus->feedback = model->chrFeedback;
	chorus->leftMod = model->chrModulation;
	chorus->rightMod = model->chrModulation;
	chorus->mix = model->chrMix;

	modEnvelopes->at(0)->setAttackRate(model->fltAttack * sampleRate);
	modEnvelopes->at(0)->setDecayRate(model->fltDecay * sampleRate);
	modEnvelopes->at(0)->setSustainLevel(model->fltSustain);
	modEnvelopes->at(0)->setReleaseRate(model->fltRelease * sampleRate);

	modEnvelopes->at(1)->setAttackRate(model->auxAttack * sampleRate);
	modEnvelopes->at(1)->setDecayRate(model->auxDecay * sampleRate);
	modEnvelopes->at(1)->setSustainLevel(model->auxSustain);
	modEnvelopes->at(1)->setReleaseRate(model->auxRelease * sampleRate);

	juce::Reverb::Parameters params =  reverb->getParameters();
	params.damping = model->rvbDdamping;
	params.dryLevel = model->rvbDryLevel;
	params.freezeMode = model->rvbFreezeMode;
	params.roomSize = model->rvbRoomSize;
	params.wetLevel = model->rvbWetLevel;
	params.width = model->rvbWidth;
	reverb->setParameters(params);


	for (int i = 0; i < voices->size(); i++) {

		voices->at(i)->getAmpEnvelope()->setAttackRate(model->ampAttack * sampleRate);
		voices->at(i)->getAmpEnvelope()->setDecayRate(model->ampDecay * sampleRate);
		voices->at(i)->getAmpEnvelope()->setSustainLevel(model->ampSustain);
		voices->at(i)->getAmpEnvelope()->setReleaseRate(model->ampRelease * sampleRate);

		voices->at(i)->getOscillator(0)->setPitch(model->osc1Pitch);
		voices->at(i)->getOscillator(0)->setFine(model->osc1Fine);
		
		voices->at(i)->setOscVolume(0, model->osc1Volume);
		voices->at(i)->setOscPan(0, model->osc1Pan);

		voices->at(i)->getOscillator(1)->setPitch(model->osc2Pitch);
		voices->at(i)->getOscillator(1)->setFine(model->osc2Fine);
		
		voices->at(i)->setOscVolume(1, model->osc2Volume);
		voices->at(i)->setOscPan(1, model->osc2Pan);

		voices->at(i)->getOscillator(2)->setPitch(model->osc3Pitch);
		voices->at(i)->getOscillator(2)->setFine(model->osc3Fine);
		
		voices->at(i)->setOscVolume(2, model->osc3Volume);
		voices->at(i)->setOscPan(2, model->osc3Pan);

		voices->at(i)->getOscillator(3)->setPitch(model->osc4Pitch);
		voices->at(i)->getOscillator(3)->setFine(model->osc4Fine);
		
		voices->at(i)->setOscVolume(3, model->osc4Volume);
		voices->at(i)->setOscPan(3, model->osc4Pan);

		for (int j = 0; j < 4; j++) {
			voices->at(i)->updateOscillator(j);
		}

		voices->at(i)->getOscillator(0)->setMode(model->osc1Shape);
		voices->at(i)->getOscillator(1)->setMode(model->osc2Shape);
		voices->at(i)->getOscillator(2)->setMode(model->osc3Shape);
		voices->at(i)->getOscillator(3)->setMode(model->osc4Shape);

	}

}

void LupoSynth::changeListenerCallback(ChangeBroadcaster* source) {
	updateState();
}

void LupoSynth::parameterChanged(const String & parameterID, float newValue)
{
	Logger::getCurrentLogger()->writeToLog("Parameter " + parameterID + " changed to " + String(newValue));

	if (parameterID == "cutoff") {
		model->cutoff = newValue;
	}
	if (parameterID == "resonance") {
		model->resonance = newValue;
	}

	
}

void LupoSynth::parameterValueChanged(int parameterIndex, float newValue)
{
	Logger::getCurrentLogger()->writeToLog("Parameter " + String(parameterIndex) + " changed to " + String(newValue));
}

void LupoSynth::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}
