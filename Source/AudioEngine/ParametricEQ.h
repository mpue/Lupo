#pragma once
#include <JuceHeader.h>

class ParametricEQ
{
public:
    static constexpr int NUM_BANDS = 8;

    enum class BandType { LowShelf, Peak, HighShelf };

    struct Band
    {
        float frequency = 1000.0f;
        float gainDb    = 0.0f;
        float q         = 1.0f;
        BandType type   = BandType::Peak;
    };

    ParametricEQ();

    void prepare (double sampleRate, int blockSize);
    void processStereo (float* left, float* right, int numSamples);

    void setBandFrequency (int band, float hz);
    void setBandGain      (int band, float dB);
    void setBandQ         (int band, float q);
    void setEnabled       (bool enabled);

    bool isEnabled() const { return mEnabled; }
    const Band& getBand (int index) const { return bands[index]; }

    float getMagnitudeForFrequency (float freq) const;

private:
    using CoeffsPtr = juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>>;

    std::array<Band, NUM_BANDS>                           bands;
    std::array<juce::dsp::IIR::Filter<float>, NUM_BANDS> filtersL;
    std::array<juce::dsp::IIR::Filter<float>, NUM_BANDS> filtersR;
    std::array<CoeffsPtr, NUM_BANDS>                      coefficients;

    double sampleRate = 44100.0;
    bool   mEnabled   = false;
    bool   prepared   = false;

    void updateCoefficients (int band);
};
