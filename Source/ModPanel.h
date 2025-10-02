/*
  ==============================================================================

    ModPanel.h
    Matrix-based modulation panel with GridButton elements

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GridButton.h"
#include <memory>
#include <vector>

class Model;
class AttachmentFactory;
class ModMatrix;

//==============================================================================
/**
    Matrix-based modulation panel using GridButton elements.
    Sources are listed vertically on the left, targets horizontally on top.
    GridButtons in the matrix allow routing modulators to targets.
*/
class ModPanel : public juce::Component,
                 public juce::Button::Listener
{
public:
    //==============================================================================
    ModPanel(ModMatrix* matrix, Model* model, AttachmentFactory* factory);
    ~ModPanel() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

    void initAttachments();
    void updateMatrix();

    // Grid state serialization methods
    juce::String getGridStateAsString() const;
    void setGridStateFromString(const juce::String& gridState);

private:
    AttachmentFactory* factory;
    Model* model;
    ModMatrix* matrix;
    
    static constexpr int GRID_SIZE = 44;
    static constexpr int LABEL_HEIGHT = 20;
    static constexpr int LABEL_WIDTH = 120;
    static constexpr int MARGIN_LEFT = 10;
    static constexpr int MARGIN_TOP = 50;
    
    // Grid of modulation routing buttons
    std::vector<std::vector<std::unique_ptr<GridButton>>> gridButtons;
    
    // Labels for sources (left side)
    std::vector<std::unique_ptr<juce::Label>> sourceLabels;
    
    // Labels for targets (top side)  
    std::vector<std::unique_ptr<juce::Label>> targetLabels;
    
    int numSources = 0;
    int numTargets = 0;
    
    void createMatrix();
    void setupLabels();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModPanel)
};

