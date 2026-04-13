#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine/ParametricEQ.h"

class AttachmentFactory;
class Model;

//==============================================================================
/** Draws the frequency response curve and handles drag-to-edit band nodes. */
class FrequencyResponseDisplay : public Component
{
public:
    FrequencyResponseDisplay (ParametricEQ* eq);

    void setFreqSliders (std::array<Slider*, 8> sliders);
    void setGainSliders (std::array<Slider*, 8> sliders);

    void paint  (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp   (const MouseEvent& e) override;

private:
    ParametricEQ*               eq;
    std::array<Slider*, 8>      freqSliders {};
    std::array<Slider*, 8>      gainSliders {};

    int   dragBand      = -1;
    float dragStartFreq = 0.0f;
    float dragStartGain = 0.0f;
    Point<int> dragStart;

    float freqToX (float freq)  const;
    float xToFreq (float x)     const;
    float gainToY (float gainDb) const;
    float yToGain (float y)      const;
    int   findNearestBand (Point<int> pos) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyResponseDisplay)
};

//==============================================================================
class EQPanel : public Component,
                public Slider::Listener
{
public:
    EQPanel (Model* model, AttachmentFactory* factory, ParametricEQ* eq);
    ~EQPanel() override;

    void paint   (Graphics& g) override;
    void resized () override;
    void sliderValueChanged (Slider* slider) override;

    void initAttachments();

private:
    Model*              model;
    AttachmentFactory*  factory;
    ParametricEQ*       eq;

    std::unique_ptr<FrequencyResponseDisplay> responseDisplay;
    std::unique_ptr<ToggleButton>             bypassButton;

    std::array<std::unique_ptr<Slider>, 8>  gainKnobs;
    std::array<std::unique_ptr<Slider>, 8>  freqKnobs;
    std::array<std::unique_ptr<Slider>, 8>  qKnobs;

    std::array<std::unique_ptr<Label>, 8>   bandLabels;
    std::array<std::unique_ptr<Label>, 8>   gainLabels;
    std::array<std::unique_ptr<Label>, 8>   freqLabels;
    std::array<std::unique_ptr<Label>, 8>   qLabels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQPanel)
};
