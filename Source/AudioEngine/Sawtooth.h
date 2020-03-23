/*
  ==============================================================================

    Sawtooth.h
    Created: 3 Jun 2016 9:02:16pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#ifndef SAWTOOTH_H_INCLUDED
#define SAWTOOTH_H_INCLUDED


#include "Oszillator.h"
#include "Modulator.h"
#include "../BlitSaw.h"
#include "../JuceLibraryCode/JuceHeader.h"

class Sawtooth : public Oszillator, public Modulator {
    
public:
	~Sawtooth() {
		delete blitsaw;
	}

    virtual float process() override;
    virtual float getOutput() override;
    virtual void setFine(float fine) override;
    virtual float getFine() const override;
    
	stk::BlitSaw* blitsaw;

    Sawtooth(float sampleRate, int buffersize);
    float fine;
    virtual void setFrequency(double frequency) override;
    virtual void reset() override;
    
    float p;      //current position
    float dp;     //change in postion per sample
    float pmax;   //maximum position
    float x;      //position in sinc function
    float leak;   //leaky integrator
    float dc;     //dc offset
    float saw;    //output
    float lastValue;
};



#endif  // SAWTOOTH_H_INCLUDED
