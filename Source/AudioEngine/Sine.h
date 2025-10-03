//
//  Sine.hpp
//  Trio
//
//  Created by Matthias Pueski on 20.11.16.
//
//

#ifndef Sine_hpp
#define Sine_hpp

#include <stdio.h>
#include "Oszillator.h"
#include "Modulator.h"

class Sine : public Oszillator, public Modulator {

public:
    Sine(float sampleRate, int buffersize);
    virtual float process() override;
    virtual void setFine(float fine) override;
    virtual float getFine() const override;
    virtual void setFrequency(double frequency) override;
    virtual float getOutput() override;
    virtual void reset() override;
    virtual void setSampleRate(double sampleRate) override;

protected:  // Changed from private to protected
    float fine;
    double step;
    double phase;  // This is already defined in Oszillator
    float value;

private:
    static constexpr double TWO_PI = 6.283185307179586476925286766559;
    void updateStep();
};

#endif /* Sine_hpp */
