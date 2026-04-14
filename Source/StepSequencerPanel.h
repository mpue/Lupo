#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine/StepSequencer.h"

//==============================================================================
/** Single step cell: handles display + interaction for one sequencer step. */
class StepCell : public juce::Component
{
public:
    explicit StepCell(int index);

    void setStep(StepSequencer::Step* s);
    void setPlaying(bool p);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override;

private:
    static juce::String noteName(int midiNote);
    static juce::Colour cellColour(int index);

    int  stepIndex  = 0;
    bool isPlaying  = false;

    StepSequencer::Step* step = nullptr;

    int  dragStartY   = 0;
    int  dragStartVel = 100;
    bool isDragging   = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepCell)
};

//==============================================================================
class StepSequencerPanel : public juce::Component,
                           public juce::Timer
{
public:
    explicit StepSequencerPanel(StepSequencer* sequencer);
    ~StepSequencerPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    StepSequencer* seq;

    std::array<std::unique_ptr<StepCell>, StepSequencer::MAX_STEPS> cells;

    std::unique_ptr<juce::ToggleButton> enableButton;

    std::unique_ptr<juce::Slider> tempoSlider;
    std::unique_ptr<juce::Label>  tempoLabel;
    std::unique_ptr<juce::Slider> swingSlider;
    std::unique_ptr<juce::Label>  swingLabel;

    std::unique_ptr<juce::Label> stepsLabel;
    std::unique_ptr<juce::Label> divLabel;
    std::unique_ptr<juce::Label> dirLabel;

    std::array<std::unique_ptr<juce::TextButton>, 3> stepsButtons;
    std::array<std::unique_ptr<juce::TextButton>, 4> divButtons;
    std::array<std::unique_ptr<juce::TextButton>, 4> dirButtons;

    int lastPlayingStep = -1;

    void updateStepsButtons();
    void updateDivButtons();
    void updateDirButtons();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepSequencerPanel)
};
