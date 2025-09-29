/*
  ==============================================================================

    GridButton.h
    Created: 28 Sep 2025 10:59:04pm
    Author:  mpue

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    GridButton is a ToggleButton with additional column and row properties
    for use in modulation matrix grid layouts.
*/
class GridButton  : public juce::ToggleButton
{
public:
    GridButton();
    GridButton(int column, int row);
    ~GridButton() override;

    void setGridPosition(int column, int row);
    int getColumn() const { return column; }
    int getRow() const { return row; }

    void paint (juce::Graphics&) override;

private:
    int column = 0;
    int row = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GridButton)
};
