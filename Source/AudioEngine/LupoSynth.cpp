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


LupoSynth::LupoSynth(float sampleRate, int bufferSize) {
	this->sampleRate = sampleRate;
	this->bufferSize = bufferSize;

	for (int i = 0; i < 3; i++) {
		SynthLab::ADSR* env = new SynthLab::ADSR();
		modEnvelopes.push_back(env);
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

void LupoSynth::setAmpAttack(float attack) {
}
void LupoSynth::setAmpDecay(float decay) {
}

void LupoSynth::setAmpSustain(float sustain) {
}

void LupoSynth::setAmpRelease(float release) {
}


void LupoSynth::setPitch(int osc, int pitch) {

}

void LupoSynth::setCutoff(float cutoff) {
	this->cutoff = cutoff;
	filter->coefficients(sampleRate, cutoff, resonance);
}

void LupoSynth::setResonance(float resonance) {
	this->resonance = resonance;
	filter->coefficients(sampleRate, cutoff, resonance);
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

void LupoSynth::configureOscillators(Oszillator::OscMode mode1, Oszillator::OscMode mode2, Oszillator::OscMode mode3) {
	for (int i = 0; i < 127; i++) {

		Voice* v = new Voice(sampleRate);

		Oszillator* osc1 = createOscillator(mode1);
		Oszillator* osc2 = createOscillator(mode2);
		Oszillator* osc3 = createOscillator(mode3);

		v->addOszillator(osc1);
		v->addOszillator(osc2);
		v->addOszillator(osc3);

		voices.push_back(v);
	}
}

void LupoSynth::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	Logger::getCurrentLogger()->writeToLog("SampleRate : " + String(sampleRate) + ", SamplesPerBlock : " + String(samplesPerBlock));

	this->sampleRate = sampleRate;
	this->bufferSize = samplesPerBlock;

	this->filter = new MultimodeFilter();


	configureOscillators(Oszillator::OscMode::SAW, Oszillator::OscMode::SAW, Oszillator::OscMode::SAW);

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


		float value = 0;

		for (int i = 0; i < voices.size(); i++) {
			if (voices[i]->isPlaying() || voices[i]->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle) {
				value += voices[i]->process();
			}
		}

		// *leftOut++ = value;
		// *rightOut++ = value;

		
		buffer.addSample(0, sample, value);
		buffer.addSample(1, sample, value);
		

	}

	leftOut = buffer.getWritePointer(0);
	rightOut = buffer.getWritePointer(1);

	filter->processStereo(leftOut, rightOut, buffer.getNumSamples());

	
}