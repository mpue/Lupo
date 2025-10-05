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
    sqrt(1.0f / 2.0f),
    sqrt(1.0f / 3.0f),
    sqrt(1.0f / 4.0f),
    sqrt(1.0f / 5.0f),
    sqrt(1.0f / 6.0f),
    sqrt(1.0f / 7.0f),
    sqrt(1.0f / 8.0f),
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


