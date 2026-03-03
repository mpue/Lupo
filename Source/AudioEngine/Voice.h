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

    enum class FilterRouting {
        Serial,   // Filter1 -> Filter2
        Parallel  // Filter1 + Filter2 mixed
    };
        
    Voice(float sampleRate);
    ~Voice() {
        oscillators.clear();
    };

    void addOszillator(std::shared_ptr<MultimodeOscillator> o, int index);
    Oszillator* getOscillator(int num);
    
 
    void processBlock(AudioBuffer<float>& buffer);  // Neue optimierte Block-Verarbeitung
    
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

	virtual void processModulation() override;
    
	SynthLab::ADSR* getAmpEnvelope();
    std::shared_ptr<SynthLab::ADSR> getFilterEnvelope();

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

    void setFilterRouting(FilterRouting routing) { filterRouting = routing; }
    FilterRouting getFilterRouting() const { return filterRouting; }

    std::shared_ptr<MultimodeFilter> getFilter1() {
        return filter1;
    }

    std::shared_ptr<MultimodeFilter> getFilter2() {
        return filter2;       
    }

    void addModulator(std::shared_ptr<Modulator> mod) override;
	void removeModulator(std::shared_ptr<Modulator> mod) override;
	void addPwmModulator(std::shared_ptr<Modulator> mod) override;
	void removePwmModulator(std::shared_ptr<Modulator> mod) override;

private:
    std::shared_ptr<MultimodeFilter> filter1;
    std::shared_ptr<MultimodeFilter> filter2;
    std::vector<std::shared_ptr<MultimodeOscillator>> oscillators;
    double sampleRate = 44100;
    int noteNumber = 0;
    int pitch = 0;
    int velocity = 0;
    double midiNote[128] = { 0 };
    void calculateFrequencyTable();
    bool playing = false;
    std::unique_ptr <SynthLab::ADSR> ampEnvelope;
    std::shared_ptr <SynthLab::ADSR> filterEnvelope;
	// Modulation
	Modulator* modulator = nullptr;
    float pitchBend;
    float duration;
    float time;
    int octave = 0;
    int offset = 0;
	float value = 0;
    float modAmount = 0;
	FilterRouting filterRouting = FilterRouting::Serial;
};



#endif  // VOICE_H_INCLUDED
