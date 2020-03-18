/*
  ==============================================================================

    OscGroup.h
    Created: 14 Mar 2020 11:53:07am
    Author:  mpue

  ==============================================================================
*/

#pragma once
#include "Sawtooth.h"

class OscGroup {

public:
	OscGroup(double sampleRate, int bufferSize);
	~OscGroup();

	float process(int voice) ;
	float getOutput() ;
	void setFine(float fine) ;
	float getFine() const ;

	void setPitch(int pitch);
	int getPitch();

	virtual void setFrequency(int voice,double frequency) ;
	virtual void reset() ;

private:
	float frequency;
	float fine;
	Sawtooth* saw[128];
	int pitch;
};