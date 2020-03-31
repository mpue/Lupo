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
#include "../ModMatrix.h"
#include "Modulation.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846 
#endif


LupoSynth::LupoSynth(Model* model, ModMatrix* modMatrix) {
	this->model = model;
	this->matrix = modMatrix;
	modEnvelopes = new vector<SynthLab::ADSR*>();

	for (int i = 0; i < 3; i++) {
		SynthLab::ADSR* env = new SynthLab::ADSR();
		modEnvelopes->push_back(env);
	}

	this->filter = new MultimodeFilter();
	filter->setModulator(modEnvelopes->at(0));
	delay = new StereoDelay();
	reverb = new StereoReverb();	
	distortion = new Distortion();
	arp = new Arpeggiator();
	
	modMatrix->registerSource("LFO1", 1);
	modMatrix->registerSource("LFO2", 2);
	modMatrix->registerSource("LFO2", 3);
	modMatrix->registerSource("AUX", 4);

	modMatrix->registerTarget("Osc 1 pitch", 1);
	modMatrix->registerTarget("Osc 2 pitch", 2);
	modMatrix->registerTarget("Osc 3 pitch", 3);
	modMatrix->registerTarget("Osc 4 pitch", 4);

	modMatrix->registerTarget("Filter cutoff", 5);

	oscGroup1 = new OscGroup();
	oscGroup2 = new OscGroup();
	oscGroup3 = new OscGroup();
	oscGroup4 = new OscGroup();

	for (int i = 0; i < 6; i++) {
		modMatrix->addModulation(new Modulation(nullptr,nullptr));
	}



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
	delete distortion;
	delete lfo1;
	delete lfo2;
	delete arp;
	delete oscGroup1;
	delete oscGroup2;
	delete oscGroup3;
	delete oscGroup4;
}

MultimodeOscillator* LupoSynth::createOscillator(Oszillator::OscMode mode) {

	MultimodeOscillator* osc = nullptr;

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

		MultimodeOscillator* osc1 = createOscillator(mode1);
		MultimodeOscillator* osc2 = createOscillator(mode2);
		MultimodeOscillator* osc3 = createOscillator(mode3);
		MultimodeOscillator* osc4 = createOscillator(mode4);

		oscGroup1->addTarget(osc1);
		oscGroup2->addTarget(osc2);
		oscGroup3->addTarget(osc3);
		oscGroup4->addTarget(osc4);

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

	lfo1 = new MultimodeOscillator(sampleRate, bufferSize);
	lfo1->setMode(2.0f);
	lfo2 = new MultimodeOscillator(sampleRate, bufferSize);
	lfo2->setMode(2.0f);
	lfo3 = new MultimodeOscillator(sampleRate, bufferSize);
	lfo3->setMode(2.0f);


	// filter->setModulator(lfo2);
	lfo1->setModAmount(0);
	lfo2->setModAmount(0);
	lfo3->setModAmount(0);

	lfo1->setFrequency(0);
	lfo2->setFrequency(0);
	lfo3->setFrequency(0);
	
	for (int i = 0; i < modEnvelopes->size(); i++) {
		modEnvelopes->at(i)->setAttackRate(0 * sampleRate);  // 1 second
		modEnvelopes->at(i)->setDecayRate(1 * sampleRate);
		modEnvelopes->at(i)->setReleaseRate(1 * sampleRate);
		modEnvelopes->at(i)->setSustainLevel(.8);
		modEnvelopes->at(i)->setModAmount(0);
	}

	arp->prepareToPlay(sampleRate, bufferSize);

	matrix->addModulator(lfo1);
	matrix->addModulator(lfo2);
	matrix->addModulator(lfo3);
	matrix->addModulator(modEnvelopes->at(0));

	matrix->addModTarget(oscGroup1);
	matrix->addModTarget(oscGroup2);
	matrix->addModTarget(oscGroup3);
	matrix->addModTarget(oscGroup4);
	matrix->addModTarget(filter);

	// updateState();

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

	this->arp->processBlock(buffer, midiMessages);
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
		
		buffer.addSample(0, sample, distortion->processSample(valueL));
		buffer.addSample(1, sample, distortion->processSample(valueR));

		matrix->process();
	}

	leftOut = buffer.getWritePointer(0);
	rightOut = buffer.getWritePointer(1);

	filter->processStereo(leftOut, rightOut, buffer.getNumSamples());
	chorus->processStereo(leftOut, rightOut, buffer.getNumSamples());
	delay->processStereo(leftOut, rightOut,buffer.getNumSamples());
	reverb->processStereo(leftOut, rightOut, buffer.getNumSamples());
}

void LupoSynth::updateState() {
}

Arpeggiator * LupoSynth::getArpeggiator()
{
	return arp;
}


void LupoSynth::changeListenerCallback(ChangeBroadcaster* source) {
	updateState();
}

void LupoSynth::parameterChanged(const String & parameterID, float newValue)
{
	if (parameterID == "cutoff") {
		model->cutoff = newValue;
		filter->coefficients(sampleRate, model->cutoff, model->resonance);
	}
	else if (parameterID == "resonance") {
		model->resonance = newValue;
		filter->coefficients(sampleRate, model->cutoff, model->resonance);
	}
	else if (parameterID == "mainVolume") {
		mainVolume = newValue;
	}
	else if (parameterID == "envAmt") {
	
	}
	else if (parameterID == "ampAttack") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getAmpEnvelope()->setAttackRate(newValue * sampleRate * 2);
		}
	}
	else if (parameterID == "ampDecay") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getAmpEnvelope()->setDecayRate(newValue * sampleRate * 2);
		}
	}
	else if (parameterID == "ampSustain") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getAmpEnvelope()->setSustainLevel(newValue);
		}
	}
	else if (parameterID == "ampRelease") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getAmpEnvelope()->setReleaseRate(newValue * sampleRate * 2);
		}
	}
	else if (parameterID == "filAttack") {
		modEnvelopes->at(0)->setAttackRate(newValue * sampleRate);
	}
	else if (parameterID == "filDecay") {
		modEnvelopes->at(0)->setDecayRate(newValue * sampleRate);
	}
	else if (parameterID == "filSustain") {
		modEnvelopes->at(0)->setSustainLevel(newValue);
	}
	else if (parameterID == "filRelease") {
		modEnvelopes->at(0)->setReleaseRate(newValue * sampleRate);
	}
	else if (parameterID == "auxAttack") {
		modEnvelopes->at(1)->setAttackRate(newValue * sampleRate);
	}
	else if (parameterID == "auxDecay") {
		modEnvelopes->at(1)->setDecayRate(newValue * sampleRate);
	}
	else if (parameterID == "auxSustain") {
		modEnvelopes->at(1)->setSustainLevel(newValue);
	}
	else if (parameterID == "auxRelease") {
		modEnvelopes->at(1)->setReleaseRate(newValue * sampleRate);
	}
	else if (parameterID == "osc1Shape") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(0)->setMode(newValue);
		}
	}
	else if (parameterID == "osc2Shape") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(1)->setMode(newValue);
		}
	}
	else if (parameterID == "osc3Shape") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(2)->setMode(newValue);
		}
	}
	else if (parameterID == "osc4Shape") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(3)->setMode(newValue);
		}
	}

	else if (parameterID == "osc1Pitch") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(0)->setPitch(newValue);
			voices->at(i)->updateOscillator(0);
		}
	}
	else if (parameterID == "osc1Fine") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(0)->setFine(newValue);
			voices->at(i)->updateOscillator(0);
		}

	}
	else if (parameterID == "osc1Volume") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscVolume(0, newValue);
		}

	}
	else if (parameterID == "osc1Pan") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscPan(0, newValue);
		}

	}
	else if (parameterID == "osc2Pitch") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(1)->setPitch(newValue);
			voices->at(i)->updateOscillator(1);
		}
	}
	else if (parameterID == "osc2Fine") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(1)->setFine(newValue);
			voices->at(i)->updateOscillator(1);
		}
	}
	else if (parameterID == "osc2Volume") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscVolume(1, newValue);
		}
	}
	else if (parameterID == "osc2Pan") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscPan(1, newValue);
		}
	}
	else if (parameterID == "osc3Pitch") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(2)->setPitch(newValue);
			voices->at(i)->updateOscillator(2);
		}
	}
	else if (parameterID == "osc3Fine") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(2)->setFine(newValue);
			voices->at(i)->updateOscillator(2);
		}
	}
	else if (parameterID == "osc3Volume") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscVolume(2, newValue);
		}
	}
	else if (parameterID == "osc3Pan") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscPan(2, newValue);
		}
	}
	else if (parameterID == "osc4Pitch") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(3)->setPitch(newValue);
			voices->at(i)->updateOscillator(3);
		}
	}
	else if (parameterID == "osc4Fine") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->getOscillator(3)->setFine(newValue);
			voices->at(i)->updateOscillator(3);
		}
	}
	else if (parameterID == "osc4Volume") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscVolume(3, newValue);
		}
	}
	else if (parameterID == "osc4Pan") {
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setOscPan(3, newValue);
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
	else if (parameterID == "rvbDdamping") {
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
		chorus->delay = newValue;
	}
	else if (parameterID == "chrModulation") {
		chorus->leftMod = newValue;
		chorus->rightMod = newValue;
	}
	else if (parameterID == "chrFeedback") {
		chorus->feedback = newValue;
	}
	else if (parameterID == "chrMix") {
		chorus->mix = newValue;
	}
	else if (parameterID == "lfo1Speed") {
		lfo1->setFrequency(newValue);
	}
	else if (parameterID == "lfo1Amount") {
		/*
		for (int i = 0; i < voices->size(); i++) {
			voices->at(i)->setModAmount(newValue * 10);
		}
		*/
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
		
	}
	else if (parameterID == "lfo2Shape") {
		lfo2->setMode(newValue);
	}
	else if (parameterID == "distDrive") {
		distortion->controls.drive = newValue;
	}
	else if (parameterID == "distMix") {
		distortion->controls.mix = newValue;
	}
	else if (parameterID == "distMode") {
		distortion->controls.mode = newValue;
	}
	
}

void LupoSynth::parameterValueChanged(int parameterIndex, float newValue)
{
}

void LupoSynth::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}

ModMatrix * LupoSynth::getModMatrix()
{
	return matrix;
}
