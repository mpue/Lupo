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

#ifndef M_PI
#define M_PI       3.14159265358979323846 
#endif


LupoSynth::LupoSynth(float sampleRate, int bufferSize) {
	this->sampleRate = sampleRate;
	this->bufferSize = bufferSize;

	for (int i = 0; i < 3; i++) {
		SynthLab::ADSR* env = new SynthLab::ADSR();
		modEnvelopes.push_back(env);
	}

	MessageBus::getInstance()->addListener("ampEnvelope.attack", this);
	MessageBus::getInstance()->addListener("ampEnvelope.decay", this);
	MessageBus::getInstance()->addListener("ampEnvelope.sustain", this);
	MessageBus::getInstance()->addListener("ampEnvelope.release", this);

	MessageBus::getInstance()->addListener("filterEnvelope.attack", this);
	MessageBus::getInstance()->addListener("filterEnvelope.decay", this);
	MessageBus::getInstance()->addListener("filterEnvelope.sustain", this);
	MessageBus::getInstance()->addListener("filterEnvelope.release", this);

	MessageBus::getInstance()->addListener("filter.cutoff", this);
	MessageBus::getInstance()->addListener("filter.resonance", this);
	MessageBus::getInstance()->addListener("filter.envAmount", this);

	MessageBus::getInstance()->addListener("mainVolume", this);

	for (int i = 1; i < 5; i++) {
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.pitch", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.fine", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.pulsewidth", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.shape.pulse", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.shape.saw", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.shape.sine", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.shape.noise", this);
		MessageBus::getInstance()->addListener("osc" + String(i) + "Panel.sync", this);
		MessageBus::getInstance()->addListener("channel" + String(i) + ".volume", this);
		MessageBus::getInstance()->addListener("channel" + String(i) + ".pan", this);
	}
		
}

LupoSynth::~LupoSynth() {
	for (int i = 0; i < 3; i++) {
		delete modEnvelopes[i];
	}

	for (std::vector<Voice*>::iterator it = voices.begin(); it != voices.end(); ++it) {
		delete *it;
	}
	voices.clear();
	delete filter;

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

	default:
		break;
	}

	if (osc != nullptr) {
		osc->setPitch(0);
	}

	return osc;

}

void LupoSynth::configureOscillators(Oszillator::OscMode mode1, Oszillator::OscMode mode2, Oszillator::OscMode mode3, Oszillator::OscMode mode4) {
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

		voices.push_back(v);
	}
}

void LupoSynth::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	Logger::getCurrentLogger()->writeToLog("SampleRate : " + String(sampleRate) + ", SamplesPerBlock : " + String(samplesPerBlock));

	this->sampleRate = sampleRate;
	this->bufferSize = samplesPerBlock;

	this->filter = new MultimodeFilter();
	filter->setModulator(modEnvelopes[0]);

	configureOscillators(Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW);

	for (int i = 0; i < modEnvelopes.size(); i++) {
		modEnvelopes.at(i)->setAttackRate(0 * sampleRate);  // 1 second
		modEnvelopes.at(i)->setDecayRate(0 * sampleRate);
		modEnvelopes.at(i)->setReleaseRate(0 * sampleRate);
		modEnvelopes.at(i)->setSustainLevel(.8);
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

			for (int envIdx = 0; envIdx < this->modEnvelopes.size(); envIdx++) {
				modEnvelopes.at(envIdx)->reset();
				modEnvelopes.at(envIdx)->gate(true);
			}

			if (!voices[noteNumber]->isPlaying()) {
				voices[noteNumber]->setNoteAndVelocity(noteNumber, m.getVelocity());
				voices[noteNumber]->setPlaying(true);
				voices[noteNumber]->getAmpEnvelope()->reset();
				voices[noteNumber]->getAmpEnvelope()->gate(true);
				voices[noteNumber]->setDuration(250);
				voices[noteNumber]->setTime(elapsed);

				numVoices++;
			}

		}
		if (m.isNoteOff())
		{
			int noteNumber = m.getNoteNumber();

			voices[noteNumber]->setPlaying(false);
			voices[noteNumber]->getAmpEnvelope()->gate(false);

			numVoices--;

			if (numVoices == 0) {
				for (int envIdx = 0; envIdx < this->modEnvelopes.size(); envIdx++) {
					modEnvelopes.at(envIdx)->gate(false);
				}
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

			for (int i = 0; i < voices.size(); i++) {
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
}

void LupoSynth::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {

	this->processMidi(midiMessages);
	
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

		float valueL = 0;
		float valueR = 0;

		for (int i = 0; i < voices.size(); i++) {
			if (voices[i]->isPlaying() || voices[i]->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {				
				valueL += voices[i]->process(0) * mainVolume;				
				valueR += voices[i]->process(1) * mainVolume;
			}
		}
		
		buffer.addSample(0, sample, valueL);
		buffer.addSample(1, sample, valueR);
		
		for (int i = 0; i < modEnvelopes.size(); i++) {
			modEnvelopes.at(i)->process();
		}
	}



	leftOut = buffer.getWritePointer(0);
	rightOut = buffer.getWritePointer(1);

	filter->processStereo(leftOut, rightOut, buffer.getNumSamples());
		
}

void LupoSynth::topicChanged(Topic* topic) {

	if (topic->getName() == "mainVolume") {
		mainVolume = topic->getValue() * 2;
	}
	
	if (topic->getName().startsWith("ampEnvelope")) {

		for (int i = 0; i < voices.size(); i++) {
			if (topic->getName() == "ampEnvelope.attack") {
				voices[i]->getAmpEnvelope()->setAttackRate(topic->getValue() * sampleRate);
			}
			if (topic->getName() == "ampEnvelope.decay") {
				voices[i]->getAmpEnvelope()->setDecayRate(topic->getValue() * sampleRate);
			}
			if (topic->getName() == "ampEnvelope.sustain") {
				voices[i]->getAmpEnvelope()->setSustainLevel(topic->getValue());
			}
			if (topic->getName() == "ampEnvelope.release") {
				voices[i]->getAmpEnvelope()->setReleaseRate(topic->getValue() * sampleRate);
			}
		
		}

	}
	else if (topic->getName().startsWith("filter")) {

		if (topic->getName().startsWith("filterEnvelope")) {
			if (topic->getName() == "filterEnvelope.attack") {
				modEnvelopes[0]->setAttackRate(topic->getValue() * sampleRate);
			}
			if (topic->getName() == "filterEnvelope.decay") {
				modEnvelopes[0]->setDecayRate(topic->getValue() * sampleRate);
			}
			if (topic->getName() == "filterEnvelope.sustain") {
				modEnvelopes[0]->setSustainLevel(topic->getValue());
			}
			if (topic->getName() == "filterEnvelope.release") {
				modEnvelopes[0]->setReleaseRate(topic->getValue() * sampleRate);
			}

		}
		else {
			if (topic->getName() == "filter.cutoff") {
				cutoff = topic->getValue();
				filter->coefficients(sampleRate, cutoff, resonance);
			}
			if (topic->getName() == "filter.resonance") {
				resonance = topic->getValue();
				filter->coefficients(sampleRate, cutoff, resonance);
			}
			if (topic->getName() == "filter.envAmount") {
				filter->setModAmount(topic->getValue());
			
			}
		}

	}
	else if (topic->getName().startsWith("osc")) {
		if (topic->getName().startsWith("osc1Panel")) {
			updateSynthState(topic, 0);
		}
		if (topic->getName().startsWith("osc2Panel")) {
			updateSynthState(topic, 1);
		}
		if (topic->getName().startsWith("osc3Panel")) {
			updateSynthState(topic, 2);
		}
		if (topic->getName().startsWith("osc4Panel")) {
			updateSynthState(topic, 3);
		}

	}
	else if (topic->getName().startsWith("channel")) {

		if (topic->getName().contains("1")) {
			updateChannel(topic, 0);
		}
		if (topic->getName().contains("2")) {
			updateChannel(topic, 1);
		}
		if (topic->getName().contains("3")) {
			updateChannel(topic, 2);
		}
		if (topic->getName().contains("4")) {
			updateChannel(topic, 3);
		}

	}
}

void LupoSynth::updateSynthState(Topic* topic, int oscillator) {
	if (topic->getName().endsWith("pitch")) {
		for (int i = 0; i < voices.size(); i++) {
			voices.at(i)->getOscillator(oscillator)->setPitch(topic->getValue());
		}
	}
	if (topic->getName().endsWith("fine")) {
		for (int i = 0; i < voices.size(); i++) {
			voices.at(i)->getOscillator(oscillator)->setFine(topic->getValue());
		}

	}
	if (topic->getName().endsWith("pulsewidth")) {
		// TODO : set pulsewidth
	}

	if (topic->getName().contains("shape")) {
		if (topic->getName().endsWith("saw")) {
			for (int i = 0; i < voices.size(); i++) {
				voices.at(i)->getOscillator(oscillator)->setMode(MultimodeOscillator::OscMode::SAW);
			}
		}
		if (topic->getName().endsWith("sine")) {
			for (int i = 0; i < voices.size(); i++) {
				voices.at(i)->getOscillator(oscillator)->setMode(MultimodeOscillator::OscMode::SINE);
			}
		}
		if (topic->getName().endsWith("pulse")) {
			for (int i = 0; i < voices.size(); i++) {
				voices.at(i)->getOscillator(oscillator)->setMode(MultimodeOscillator::OscMode::PULSE);
			}
		}
		if (topic->getName().endsWith("noise")) {
			for (int i = 0; i < voices.size(); i++) {
				voices.at(i)->getOscillator(oscillator)->setMode(MultimodeOscillator::OscMode::NOISE);
			}
		}

	}
	for (int i = 0; i < voices.size(); i++) {
		for (int j = 0; j < voices[i]->getOszillators().size(); j++) {
			voices[i]->updateOscillator(j);
		}

	}

	// TODO : Evaluate sync
}

void LupoSynth::updateChannel(Topic* topic, int channel) {
	if (topic->getName().endsWith("volume")) {
		volume[channel] = topic->getValue();
		for (int i = 0; i < voices.size(); i++) {
			voices[i]->setOscVolume(channel, volume[channel]);
		}
	}
	if (topic->getName().endsWith("pan")) {
		pan[channel] = topic->getValue();
		for (int i = 0; i < voices.size(); i++) {
			voices[i]->setOscPan(channel, pan[channel]);
		}
	}
}