//
//  MultimodeOscillator.hpp
//  Trio
//
//  Created by Matthias Pueski on 20.11.16.
//
//

#ifndef MultimodeOscillator_hpp
#define MultimodeOscillator_hpp

#include <stdio.h>

#include "Oszillator.h";
#include "ModTarget.h"
#include "Sine.h"
#include "Sawtooth.h"
#include "Pulse.h"
#include "WhiteNoise.h"
#include "SampleAndHold.h"



class MultimodeOscillator : public Oszillator, public Modulator, public ModTarget {
        
public:
    MultimodeOscillator(float sampleRate, int buffersize);
    ~MultimodeOscillator();
    virtual float process() override;
    virtual void setFine(float fine) override;
    virtual float getFine() const override;
    virtual void setFrequency(double frequency) override;
    virtual void setVolume(float volume) override;
    virtual float getOutput() override;
    virtual void setModulator(Modulator* mod) override;    
    virtual void reset() override;
    virtual void setSlave(Oszillator* slave) override;
    virtual void setSync(bool sync) override;
	void setSpread(float spread);

    virtual void applyModulation(float value) override {
        this->currentModulatedValue = value;
    }

    void setSampleRate(double sampleRate) override {
        this->sine->setSampleRate(sampleRate);
        this->saw->setSampleRate(sampleRate);
        this->pulse->setSampleRate(sampleRate);
        this->noise->setSampleRate(sampleRate);
    }
private:
    Sawtooth* saw = nullptr;
    Sine* sine = nullptr;
    Pulse* pulse = nullptr;
    WhiteNoise* noise = nullptr;
	SampleAndHold* sah = nullptr;
    float fine = 0;
	float frequency = 0;
    float currentModulatedValue = 0.0f;
};

#endif /* MultimodeOscillator_hpp */
