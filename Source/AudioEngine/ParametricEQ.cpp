#include "ParametricEQ.h"

ParametricEQ::ParametricEQ()
{
    // Default band setup
    bands[0] = { 60.0f,    0.0f, 0.7f, BandType::LowShelf  };
    bands[1] = { 200.0f,   0.0f, 1.0f, BandType::Peak      };
    bands[2] = { 500.0f,   0.0f, 1.0f, BandType::Peak      };
    bands[3] = { 1000.0f,  0.0f, 1.0f, BandType::Peak      };
    bands[4] = { 2000.0f,  0.0f, 1.0f, BandType::Peak      };
    bands[5] = { 4000.0f,  0.0f, 1.0f, BandType::Peak      };
    bands[6] = { 8000.0f,  0.0f, 1.0f, BandType::Peak      };
    bands[7] = { 14000.0f, 0.0f, 0.7f, BandType::HighShelf };
}

void ParametricEQ::prepare (double sr, int /*blockSize*/)
{
    sampleRate = sr;
    prepared   = true;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sr;
    spec.maximumBlockSize = 4096;
    spec.numChannels      = 1;

    for (int i = 0; i < NUM_BANDS; ++i)
    {
        updateCoefficients (i);
        filtersL[i].prepare (spec);
        filtersR[i].prepare (spec);
    }
}

void ParametricEQ::updateCoefficients (int i)
{
    const auto& b = bands[i];
    const float freq  = juce::jlimit (20.0f, 20000.0f, b.frequency);
    const float gain  = b.gainDb;
    const float q     = juce::jlimit (0.1f, 10.0f, b.q);
    const double sr   = sampleRate;

    CoeffsPtr c;
    switch (b.type)
    {
        case BandType::LowShelf:
            c = juce::dsp::IIR::Coefficients<float>::makeLowShelf  (sr, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
        case BandType::HighShelf:
            c = juce::dsp::IIR::Coefficients<float>::makeHighShelf (sr, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
        default:
            c = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sr, freq, q, juce::Decibels::decibelsToGain (gain));
            break;
    }

    coefficients[i] = c;

    if (prepared)
    {
        *filtersL[i].coefficients = *c;
        *filtersR[i].coefficients = *c;
    }
}

void ParametricEQ::processStereo (float* left, float* right, int numSamples)
{
    if (!mEnabled || !prepared)
        return;

    for (int i = 0; i < NUM_BANDS; ++i)
    {
        for (int n = 0; n < numSamples; ++n)
        {
            left[n]  = filtersL[i].processSample (left[n]);
            right[n] = filtersR[i].processSample (right[n]);
        }
    }
}

void ParametricEQ::setBandFrequency (int band, float hz)
{
    if (band < 0 || band >= NUM_BANDS) return;
    bands[band].frequency = hz;
    updateCoefficients (band);
}

void ParametricEQ::setBandGain (int band, float dB)
{
    if (band < 0 || band >= NUM_BANDS) return;
    bands[band].gainDb = dB;
    updateCoefficients (band);
}

void ParametricEQ::setBandQ (int band, float q)
{
    if (band < 0 || band >= NUM_BANDS) return;
    bands[band].q = q;
    updateCoefficients (band);
}

void ParametricEQ::setEnabled (bool enabled)
{
    if (!enabled && mEnabled)
    {
        // Reset filter states to avoid clicks on re-enable
        for (int i = 0; i < NUM_BANDS; ++i)
        {
            filtersL[i].reset();
            filtersR[i].reset();
        }
    }
    mEnabled = enabled;
}

float ParametricEQ::getMagnitudeForFrequency (float freq) const
{
    double mag = 1.0;
    for (int i = 0; i < NUM_BANDS; ++i)
    {
        if (coefficients[i] != nullptr)
            mag *= coefficients[i]->getMagnitudeForFrequency ((double) freq, sampleRate);
    }
    return (float) juce::Decibels::gainToDecibels (mag);
}
