//
//  CharacterFilter.cpp
//  Lupo
//

#include "CharacterFilter.h"
#include <cmath>

CharacterFilter::CharacterFilter()
{
}

void CharacterFilter::prepare(double sr)
{
    sampleRate = sr;
    reset();
}

void CharacterFilter::setCharacter(Character c)
{
    character = c;
}

void CharacterFilter::setMode(Mode m)
{
    mode = m;
}

void CharacterFilter::setFrequency(float hz)
{
    frequency = hz;
}

void CharacterFilter::setFrequencyImmediate(float hz)
{
    frequency = hz;
}

void CharacterFilter::setResonance(float r)
{
    resonance = r;
}

void CharacterFilter::setModulatedValue(float value)
{
    modulatedValue = value;
}

void CharacterFilter::reset()
{
    stateL1 = stateL2 = stateR1 = stateR2 = BiquadState{};
}

// Compute bilinear-transform biquad coefficients for a given stage.
// Uses per-character Q tables with resonance offset applied.
CharacterFilter::BiquadCoeffs CharacterFilter::computeCoeffs(int stage) const
{
    const float* qBase = kBaseQ[character];
    float extra = resonance - 1.0f;
    float q1 = qBase[0] + extra * 0.5f;
    float q2 = qBase[1] + extra;
    float q = (stage == 0) ? q1 : q2;
    q = juce::jmax(0.01f, q);

    float fc = juce::jlimit(10.f, (float)(sampleRate * 0.49), frequency * modulatedValue);
    float w = std::tan(juce::MathConstants<float>::pi * fc / (float)sampleRate);
    float w2 = w * w;

    BiquadCoeffs c;
    if (mode == LOWPASS)
    {
        float norm = 1.f / (1.f + w / q + w2);
        c.b0 =  w2 * norm;
        c.b1 =  2.f * c.b0;
        c.b2 =  c.b0;
        c.a1 = -2.f * (w2 - 1.f) * norm;
        c.a2 = -(1.f - w / q + w2) * norm;
    }
    else
    {
        float norm = 1.f / (1.f + w / q + w2);
        c.b0 =  norm;
        c.b1 = -2.f * norm;
        c.b2 =  norm;
        c.a1 = -2.f * (w2 - 1.f) * norm;
        c.a2 = -(1.f - w / q + w2) * norm;
    }
    return c;
}

// Direct Form II Transposed
float CharacterFilter::processBiquad(float x, BiquadCoeffs& c, BiquadState& s) const
{
    float y = c.b0 * x + s.z1;
    s.z1 = c.b1 * x + c.a1 * y + s.z2;
    s.z2 = c.b2 * x + c.a2 * y;
    return y;
}

void CharacterFilter::processSample(float& left, float& right)
{
    BiquadCoeffs c1 = computeCoeffs(0);
    BiquadCoeffs c2 = computeCoeffs(1);

    left  = processBiquad(left,  c1, stateL1);
    left  = processBiquad(left,  c2, stateL2);
    right = processBiquad(right, c1, stateR1);
    right = processBiquad(right, c2, stateR2);
}

void CharacterFilter::processBlock(float* left, float* right, int numSamples)
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
