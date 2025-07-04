﻿#ifndef LowPassFilter_hpp
#define LowPassFilter_hpp

#include "Filter.h"
#include "Modulator.h"
#include "ModTarget.h"

#include <juce_dsp/juce_dsp.h>

class LowPassFilter : public Filter, public ModTarget
{
public:
    LowPassFilter();
    ~LowPassFilter() override = default;

    // wie bisher
    void coefficients(float sampleRate, float frequency, float resonance) override;
    void process(float* in, int numSamples);
    void setModulator(Modulator* mod) override;
    void applyModulation(float value) override { currentModulatedValue = value; }
    void setFrequency(float frequency) {
        this->frequency = frequency;
        svf1.setCutoffFrequency(juce::jlimit(0.1f,22000.0f,frequency * currentModulatedValue * modulationDepth));
        svf2.setCutoffFrequency(juce::jlimit(0.1f, 22000.0f, frequency * currentModulatedValue * modulationDepth));
    }

    void setResonance(float resonance) {
        this->resonance = resonance;
        svf1.setResonance(juce::jlimit(0.05f, 5.0f, resonance));
        svf2.setResonance(juce::jlimit(0.05f, 5.0f, resonance));
    }

private:
    //
    // ⇢ ersetzt beide IIR-Stufen
    //
   juce::dsp::StateVariableTPTFilter<float> svf1;
   juce::dsp::StateVariableTPTFilter<float> svf2;

   juce::dsp::ProcessSpec spec{ 44100.0, 512, 1 };

    float frequency = 1000.0f;
    float resonance = 0.7f;
    float currentModulatedValue = 1.0f;

    float modulationDepth = 1000.0f;    
    int    updateCounter = 0;
    int    updateInterval = 8;          // alle 8 Samples Coeffs erneuern
    float  lastCutoff = -1.0f;      // merken, ob sich etwas geändert hat
    float  cutoffEpsilon = 0.5f;

    LinearSmoothedValue<float> smoothedCutoff;

    JUCE_LEAK_DETECTOR(LowPassFilter)
};

#endif
