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

	this->filter1 = new MultimodeFilter();
	this->filter2 = new MultimodeFilter();
	filter1->setModulator(modEnvelopes->at(0));
	delay = new StereoDelay();
	reverb = new StereoReverb();	
	distortion = new Distortion();
	arp = new Arpeggiator();
	
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

	oscGroup1 = new OscGroup();
	oscGroup2 = new OscGroup();
	oscGroup3 = new OscGroup();
	oscGroup4 = new OscGroup();

	for (int i = 0; i < 6; i++) {
		modMatrix->addModulation(new Modulation(nullptr,nullptr),i);
	}

}

LupoSynth::~LupoSynth() {

	for (int i = 0; i < 3; i++) {
		delete (*modEnvelopes)[i];
	}

	delete modEnvelopes;

	for (int i = 0; i < 128;i++) {
		delete voices[i];
	}
	
	delete filter1;
	delete filter2;
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
	


	for (int i = 0; i < 128; i++) {

		Voice* v = new Voice(sampleRate);

		MultimodeOscillator* osc1 = createOscillator(mode1);
		MultimodeOscillator* osc2 = createOscillator(mode2);
		MultimodeOscillator* osc3 = createOscillator(mode3);
		MultimodeOscillator* osc4 = createOscillator(mode4);

		oscGroup1->addTarget(osc1);
		oscGroup2->addTarget(osc2);
		oscGroup3->addTarget(osc3);
		oscGroup4->addTarget(osc4);

		v->addOszillator(osc1,0);
		v->addOszillator(osc2,1);
		v->addOszillator(osc3,2);
		v->addOszillator(osc4,3);

		voices[i] = v;
	}
}

void LupoSynth::prepareToPlay(double sampleRate, int samplesPerBlock)
{		

	this->sampleRate = sampleRate;
	this->bufferSize = samplesPerBlock;

	filter1->coefficients(sampleRate, 15000.0f, 1.0f);
	filter2->coefficients(sampleRate, 18000.0f, 0.01f);
	configureOscillators(Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW);
	
	chorus = new StereoChorus(sampleRate,bufferSize);
	chorus->setEnabled(true);

	lfo1 = new MultimodeOscillator(sampleRate, bufferSize);
	lfo1->setMode(2.0f);
	lfo2 = new MultimodeOscillator(sampleRate, bufferSize);
	lfo2->setMode(2.0f);
	lfo3 = new MultimodeOscillator(sampleRate, bufferSize);
	lfo3->setMode(2.0f);


	filter1->setModulator(modEnvelopes->at(0));
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
	matrix->addModulator(modEnvelopes->at(1));

	matrix->addModTarget(oscGroup1);
	matrix->addModTarget(oscGroup2);
	matrix->addModTarget(oscGroup3);
	matrix->addModTarget(oscGroup4);
	matrix->addModTarget(filter1);
	matrix->addModTarget(filter2);
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
				modEnvelopes->at(envIdx)->gate(true);
			}


			if (!voices[noteNumber]->isPlaying()) {
				voices[noteNumber]->getAmpEnvelope()->gate(true);
				voices[noteNumber]->setPlaying(true);				
				numVoices++;
				voices[noteNumber]->setNoteAndVelocity(noteNumber, m.getVelocity());
				voices[noteNumber]->setDuration(250);
				voices[noteNumber]->setTime(elapsed);
			}


		}
		if (m.isNoteOff())
		{
			int noteNumber = m.getNoteNumber();

			voices[noteNumber]->setPlaying(false);
			voices[noteNumber]->getAmpEnvelope()->gate(false);

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

			for (int i = 0; i < 128; i++) {
				if (voices[i]->isPlaying())
					voices[i]->setPitchBend(nPitch);
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

	filter1->setKeyTrack(highestNote);
	

}

void LupoSynth::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {

	this->arp->processBlock(buffer, midiMessages);
	this->processMidi(midiMessages);

	for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

		
		float valueL = 0;
		float valueR = 0;

		for (int i = 0; i < 128; i++) {
			if (voices[i]->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {				
				
				valueL += voices[i]->process(0) * mainVolume * 4;
				valueR += voices[i]->process(1) * mainVolume * 4;
			}
		}		
		
		buffer.addSample(0, sample, distortion->processSample(valueL));
		buffer.addSample(1, sample, distortion->processSample(valueR));

		matrix->process();
		modEnvelopes->at(0)->process();
	}

	leftOut = buffer.getWritePointer(0);
	rightOut = buffer.getWritePointer(1);

	// serial
	if (filterMode == 0.0f) {
		filter1->processStereo(leftOut, rightOut, buffer.getNumSamples());
		filter2->processStereo(leftOut, rightOut, buffer.getNumSamples());
	}
	// parallel
	else {
		filter1->processMono(0, leftOut, buffer.getNumSamples());
		filter2->processMono(1, rightOut, buffer.getNumSamples());
	}

	chorus->processStereo(leftOut, rightOut, buffer.getNumSamples());
	delay->processStereo(leftOut, rightOut,buffer.getNumSamples());
	reverb->processStereo(leftOut, rightOut, buffer.getNumSamples());
}

void LupoSynth::updateState() {
	if (model->osc2Sync) {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(0)->setSlave(voices[i]->getOscillator(1));
			voices[i]->getOscillator(0)->setSync(true);
		}
	}


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
	bool matrixChanged = false;

	Logger::getCurrentLogger()->writeToLog(parameterID + " : " + String(newValue));

	if (parameterID == "cutoff1") 
	{	
		model->cutoff1 = newValue;
		if (cutoffLink) {
			model->cutoff2 = newValue;
			filter2->coefficients(sampleRate, model->cutoff1, model->resonance2);
		}
		filter1->coefficients(sampleRate, model->cutoff1, model->resonance1);
	}
	else if (parameterID == "resonance1") {
		model->resonance1 = newValue;
		filter1->coefficients(sampleRate, model->cutoff1, model->resonance1);
	}
	else if (parameterID == "filterMode1") {
		if (newValue == 0.0f) {
			filter1->setMode(MultimodeFilter::LOWPASS);
		}
		else if (newValue == 1.0f) {
			filter1->setMode(MultimodeFilter::HIGHPASS);
		}
	}
	else if (parameterID == "envAmt1") {
		modEnvelopes->at(0)->setModAmount(newValue);
	}

	if (parameterID == "cutoff2") {
		model->cutoff2 = newValue;
		filter2->coefficients(sampleRate, model->cutoff2, model->resonance2);
	}
	else if (parameterID == "resonance2") {
		model->resonance2 = newValue;
		if (cutoffLink) {
			filter2->coefficients(sampleRate, model->cutoff1, model->resonance2);
		}
		filter2->coefficients(sampleRate, model->cutoff2, model->resonance2);
	}
	else if (parameterID == "filterMode2") {
		if (newValue == 0.0f) {
			filter2->setMode(MultimodeFilter::LOWPASS);
		}
		else if (newValue == 1.0f) {
			filter2->setMode(MultimodeFilter::HIGHPASS);
		}
	}
	else if (parameterID == "envAmt2") {
		modEnvelopes->at(1)->setModAmount(newValue);
	}


	else if (parameterID == "mainVolume") {
		mainVolume = newValue;
	}
	else if (parameterID == "ampAttack") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getAmpEnvelope()->setAttackRate(newValue * sampleRate * 4);
		}
	}
	else if (parameterID == "ampDecay") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getAmpEnvelope()->setDecayRate(newValue * sampleRate * 4);
		}
	}
	else if (parameterID == "ampSustain") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getAmpEnvelope()->setSustainLevel(newValue);
		}
	}
	else if (parameterID == "ampRelease") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getAmpEnvelope()->setReleaseRate(newValue * sampleRate * 4);
		}
	}
	else if (parameterID == "auxAttack1") {
		modEnvelopes->at(0)->setAttackRate(newValue * sampleRate * 4);
	}
	else if (parameterID == "auxDecay1") {
		modEnvelopes->at(0)->setDecayRate(newValue * sampleRate * 4);
	}
	else if (parameterID == "auxSustain1") {
		modEnvelopes->at(0)->setSustainLevel(newValue);
	}
	else if (parameterID == "auxRelease1") {
		modEnvelopes->at(0)->setReleaseRate(newValue * sampleRate * 4);
	}
	else if (parameterID == "auxAttack2") {
		modEnvelopes->at(1)->setAttackRate(newValue * sampleRate* 4);
	}
	else if (parameterID == "auxDecay2") {
		modEnvelopes->at(1)->setDecayRate(newValue * sampleRate * 4);
	}
	else if (parameterID == "auxSustain2") {
		modEnvelopes->at(1)->setSustainLevel(newValue);
	}
	else if (parameterID == "auxRelease2") {
		modEnvelopes->at(1)->setReleaseRate(newValue * sampleRate * 4);
	}

	else if (parameterID == "osc1Shape") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(0)->setMode(newValue);
		}
	}
	else if (parameterID == "osc2Shape") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(1)->setMode(newValue);
		}
	}
	else if (parameterID == "osc3Shape") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(2)->setMode(newValue);
		}
	}
	else if (parameterID == "osc4Shape") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(3)->setMode(newValue);
		}
	}

	else if (parameterID == "osc1Pitch") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(0)->setPitch(newValue);
			voices[i]->updateOscillator(0);
		}
	}
	else if (parameterID == "osc1Fine") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(0)->setFine(newValue);
			voices[i]->updateOscillator(0);
		}

	}
	else if (parameterID == "osc1Volume") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscVolume(0, newValue);
		}

	}
	else if (parameterID == "osc1Pan") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscPan(0, newValue);
		}

	}

	else if (parameterID == "osc1Spread") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscSpread(0, newValue);
		}
	}

	else if (parameterID == "osc2Pitch") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(1)->setPitch(newValue);
			voices[i]->updateOscillator(1);
		}
	}
	else if (parameterID == "osc2Fine") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(1)->setFine(newValue);
			voices[i]->updateOscillator(1);
		}
	}
	else if (parameterID == "osc2Volume") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscVolume(1, newValue);
		}
	}
	else if (parameterID == "osc2Pan") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscPan(1, newValue);
		}
	}
	else if (parameterID == "osc2Spread") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscSpread(1, newValue);
		}
	}

	else if (parameterID == "osc3Pitch") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(2)->setPitch(newValue);
			voices[i]->updateOscillator(2);
		}
	}
	else if (parameterID == "osc3Fine") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(2)->setFine(newValue);
			voices[i]->updateOscillator(2);
		}
	}
	else if (parameterID == "osc3Volume") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscVolume(2, newValue);
		}
	}
	else if (parameterID == "osc3Pan") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscPan(2, newValue);
		}
	}
	else if (parameterID == "osc3Spread") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscSpread(2, newValue);
		}
	}

	else if (parameterID == "osc4Pitch") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(3)->setPitch(newValue);
			voices[i]->updateOscillator(3);
		}
	}
	else if (parameterID == "osc4Fine") {
		for (int i = 0; i < 128; i++) {
			voices[i]->getOscillator(3)->setFine(newValue);
			voices[i]->updateOscillator(3);
		}
	}
	else if (parameterID == "osc4Volume") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscVolume(3, newValue);
		}
	}
	else if (parameterID == "osc4Pan") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscPan(3, newValue);
		}
	}
	else if (parameterID == "osc4Spread") {
		for (int i = 0; i < 128; i++) {
			voices[i]->setOscSpread(3, newValue);
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
		for (int i = 0; i < 128; i++) {
			voices[i]->setModAmount(newValue * 10);
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
	else if (parameterID == "filterMode") {
		filterMode = newValue;
	}
	else if (parameterID == "cutoffLink") {
		newValue > 0 ? cutoffLink = true : cutoffLink = false;
	}
	else if (parameterID.startsWith("Amount")) {
		const int index = parameterID.substring(parameterID.lastIndexOf("_") + 1).getIntValue();
		model->modAmount[index] = newValue;
		matrixChanged = true;
	}
	else if (parameterID.startsWith("Target")) {
		const int index = parameterID.substring(parameterID.lastIndexOf("_") + 1).getIntValue();
		model->modTarget[index] = (int)newValue;
		matrixChanged = true;
	}
	else if (parameterID.startsWith("Source")) {
		const int index = parameterID.substring(parameterID.lastIndexOf("_") + 1).getIntValue();
		model->modSource[index] = (int)newValue;		
		matrixChanged = true;
	}
	if (matrixChanged) {
		updateMatrix();
		matrixChanged = false;

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

void LupoSynth::updateMatrix() {
	for (int i = 0; i < 6; i++) {
		matrix->getModulations()[i]->setTarget(matrix->getModTargets().at(model->modTarget[i]));
		matrix->getModulations()[i]->setModulator(matrix->getModulators().at(model->modSource[i]));	
	}
	for (int i = 0; i <6 ; i++) {
		if (matrix->getModulations()[i]->getModulator() != nullptr) {
			matrix->getModulations()[i]->getModulator()->setModAmount(model->modAmount[i]);
		}
	}
}