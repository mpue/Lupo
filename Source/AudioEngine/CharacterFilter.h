//
//  CharacterFilter.h
//  Lupo
//
//  Stereo 2-stage biquad filter supporting multiple filter characters:
//  Standard (SVF-like), Butterworth, Chebyshev (1dB ripple), Bessel, Linkwitz-Riley
//
//  Header-only implementation.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

class CharacterFilter
{
public:
    enum Character { STANDARD = 0, BUTTERWORTH = 1, CHEBYSHEV = 2, BESSEL = 3, LINKWITZ_RILEY = 4 };
    enum Mode      { LOWPASS  = 0, HIGHPASS    = 1 };

    CharacterFilter() = default;
    ~CharacterFilter() = default;

    void prepare(double sr)           { sampleRate = sr; reset(); }
    void setCharacter(Character c)    { character = c; }
    void setMode(Mode m)              { mode = m; }
    void setFrequency(float hz)       { frequency = hz; }
    void setFrequencyImmediate(float hz) { frequency = hz; }
    void setResonance(float r)        { resonance = r; }
    void setModulatedValue(float v)   { modulatedValue = v; }

    void reset()
    {
        stateL1 = stateL2 = stateR1 = stateR2 = BiquadState{};
    }

    void processSample(float& left, float& right)
    {
        BiquadCoeffs c1 = computeCoeffs(0);
        BiquadCoeffs c2 = computeCoeffs(1);
        left  = processBiquad(left,  c1, stateL1);
        left  = processBiquad(left,  c2, stateL2);
        right = processBiquad(right, c1, stateR1);
        right = processBiquad(right, c2, stateR2);
    }

    void processBlock(float* left, float* right, int numSamples)
    {
        BiquadCoeffs c1 = computeCoeffs(0);
        BiquadCoeffs c2 = computeCoeffs(1);
        for (int i = 0; i < numSamples; ++i)
        {
            left[i]  = processBiquad(left[i],  c1, stateL1);
            left[i]  = processBiquad(left[i],  c2, stateL2);
            right[i] = processBiquad(right[i], c1, stateR1);
            right[i] = processBiquad(right[i], c2, stateR2);
        }
    }

private:
    struct BiquadCoeffs { float b0, b1, b2, a1, a2; };
    struct BiquadState  { float z1 = 0.f, z2 = 0.f; };

    BiquadCoeffs computeCoeffs(int stage) const
    {
        // Per-character Q values for the two biquad stages [character][stage]
        static const float kBaseQ[5][2] = {
            { 0.5000f, 0.5000f },   // STANDARD
            { 0.5412f, 1.3066f },   // BUTTERWORTH
            { 0.7966f, 3.6513f },   // CHEBYSHEV 1dB
            { 0.5219f, 0.8055f },   // BESSEL
            { 0.7071f, 0.7071f },   // LINKWITZ-RILEY
        };
        const float* qBase = kBaseQ[character];
        float extra = resonance - 1.0f;
        float q = (stage == 0) ? (qBase[0] + extra * 0.5f)
                               : (qBase[1] + extra);
        q = juce::jmax(0.01f, q);

        float fc = juce::jlimit(10.f, (float)(sampleRate * 0.49), frequency * modulatedValue);
        float w  = std::tan(juce::MathConstants<float>::pi * fc / (float)sampleRate);
        float w2 = w * w;

        BiquadCoeffs c;
        float norm = 1.f / (1.f + w / q + w2);
        if (mode == LOWPASS)
        {
            c.b0 =  w2 * norm;
            c.b1 =  2.f * c.b0;
            c.b2 =  c.b0;
        }
        else
        {
            c.b0 =  norm;
            c.b1 = -2.f * norm;
            c.b2 =  norm;
        }
        c.a1 = -2.f * (w2 - 1.f) * norm;
        c.a2 = -(1.f - w / q + w2) * norm;
        return c;
    }

    // Direct Form II Transposed
    float processBiquad(float x, const BiquadCoeffs& c, BiquadState& s) const
    {
        float y = c.b0 * x + s.z1;
        s.z1 = c.b1 * x + c.a1 * y + s.z2;
        s.z2 = c.b2 * x + c.a2 * y;
        return y;
    }

    double sampleRate    = 44100.0;
    Character character  = STANDARD;
    Mode mode            = LOWPASS;
    float frequency      = 1000.f;
    float resonance      = 1.f;
    float modulatedValue = 1.f;

    BiquadState stateL1, stateL2;
    BiquadState stateR1, stateR2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CharacterFilter)
};
