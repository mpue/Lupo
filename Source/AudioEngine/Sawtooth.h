/*
  ==============================================================================
    Sawtooth.h
    Created: 3 Jun 2016 9:02:16pm
    Author:  Matthias Pueski
  ==============================================================================
*/
#pragma once

#include "Oszillator.h"
#include "Modulator.h"
#include "../BlitSaw.h"
#include "../JuceLibraryCode/JuceHeader.h"

const float scales[] = {
    1.0f,
    sqrt(1.0 / 2.0),
    sqrt(1.0 / 3.0),
    sqrt(1.0 / 4.0),
    sqrt(1.0 / 5.0),
    sqrt(1.0 / 6.0),
    sqrt(1.0 / 7.0),
    sqrt(1.0 / 8.0),
};

class Sawtooth : public Oszillator, public Modulator {

public:
    void setSpread(float spread);
    virtual float getOutput() override;
    virtual void setFine(float fine) override;
    virtual float getFine() const override;

    stk::BlitSaw* blitsaw[8];

    Sawtooth(float sampleRate, int buffersize);
    ~Sawtooth();

    float fine;

    virtual void reset() override;
    float process() override;
    void setFrequency(double frequency) override;

private:
    float saw;
    float lastValue;
    int bufferSize;
    float spread;
    float value;
    float totalWeight;
};


