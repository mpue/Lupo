//
//  Pulse.hpp
//  Trio
//
//  Created by Matthias Pueski on 23.11.16.
//
//

#ifndef Pulse_h
#define Pulse_h

#include <stdio.h>
#include "Oszillator.h"
#include "Modulator.h"
#include "..\BlitSquare.h"
#include "ModTarget.h"

class Pulse : public Oszillator , public Modulator, public ModTarget {
    
    
public:
    Pulse(float sampleRate, int buffersize);
	~Pulse() {
		delete blitSquare;			
	}
    virtual float process() override;
    virtual void setFine(float fine) override;
    virtual float getFine() const override;
    virtual void setFrequency(double frequency) override;
    virtual float getOutput() override;
    virtual void reset() override;
	virtual void processModulation() override;
    void addPwmModulator(std::shared_ptr<Modulator> mod) override
    {
        pwmModulators.push_back(mod);
    }

    void removePwmModulator(std::shared_ptr<Modulator> mod) override {
        pwmModulators.erase(std::remove(pwmModulators.begin(), pwmModulators.end(), mod), pwmModulators.end());
    }
    //! Set the pulse width directly (0.0 to 1.0, where 0.5 = 50% duty cycle)
    void setPulseWidth(float width);
    
    //! Get the current pulse width
    float getPulseWidth() const;
    
    //! Set the sample rate
    virtual void setSampleRate(double sampleRate) override;
    
private:
    float fine;
    double step;
    double phase;
    double value;
    bool on = false;
	stk::BlitSquare* blitSquare;    
    float currentModulatedFrequency = 0.0f;  // Track current modulated frequency
};

#endif /* Pulse_hpp */
