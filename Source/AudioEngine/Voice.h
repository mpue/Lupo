/*
  ==============================================================================

    Voice.h
    Created: 4 Jun 2016 6:53:13pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#ifndef VOICE_H_INCLUDED
#define VOICE_H_INCLUDED

#include <vector>

#include "MultimodeOscillator.h"
#include "MultimodeFilter.h"
#include "Sawtooth.h"
#include "Modulator.h"
#include "Note.h"
#include "ADSR.h"
#include "ModTarget.h"
#include "Sampler.h"
#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;

class Voice : public ModTarget {
    
public:
        
    Voice(float sampleRate);
    ~Voice() {
        oscillators.clear();
    };
    void applyModulation(float value) override;

    void addOszillator(std::unique_ptr<MultimodeOscillator> o, int index);
    Oszillator* getOscillator(int num);
    
    float process(int channel);
    
    void setNoteNumber(int number);
    int getNoteNumber() const;
    void setOctave(int number);
    int getOctave() const;
    
    void setOffset(int number);
    
    void setPitch(int number);
    int getPitch() const;
    void updateOscillator(int index);
    
    void setPlaying(bool playing);
    bool isPlaying() const;

    virtual void setModulator(Modulator* mod) override;
    
	SynthLab::ADSR* getAmpEnvelope();
    SynthLab::ADSR* getFilterEnvelope();

    void setSampleRate(double rate);
    float getSampleRate();
    
    void setPitchBend(float bend);
    
    void setDuration(float duration);
    float getDuration();
    void setTime(float time);
    float getTime();
    
	void setVelocity(int velocity);
	int getVelocity() const;
	void setNoteAndVelocity(int note, int velocity);

	void setOscVolume(int osc, float volume);
	void setOscPan(int osc, float pan);
	void setOscSpread(int osc, float spread);

    MultimodeFilter* getFilter1() {
        return filter1.get();
    }
    MultimodeFilter* getFilter2() {
        return filter2.get();       
    }

    
private:
    std::unique_ptr<MultimodeFilter> filter1;
    std::unique_ptr<MultimodeFilter> filter2;
    std::vector<std::unique_ptr<MultimodeOscillator>> oscillators;
    float sampleRate = 44100;
    int noteNumber = 0;
    int pitch = 0;
    int velocity = 0;
    double midiNote[128] = { 0 };
    void calculateFrequencyTable();
    bool playing = false;
    std::unique_ptr <SynthLab::ADSR> ampEnvelope;
    std::unique_ptr <SynthLab::ADSR> filterEnvelope;
	// Modulation
	Modulator* modulator = nullptr;
    float pitchBend;
    float duration;
    float time;
    int octave = 0;
    int offset = 0;
	float value = 0;
    float modAmount = 0;
	
};



#endif  // VOICE_H_INCLUDED
