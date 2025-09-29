/*
  ==============================================================================

    GridButton.cpp
    Created: 28 Sep 2025 10:59:04pm
    Author:  mpue

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GridButton.h"

//==============================================================================
GridButton::GridButton() : ToggleButton()
{
}

GridButton::GridButton(int col, int r) : ToggleButton(), column(col), row(r)
{
}

GridButton::~GridButton()
{
}

void GridButton::setGridPosition(int col, int r)
{
    this->column = col;
    this->row = r;
}

void GridButton::paint(juce::Graphics& g)
{
    // Draw the button background
    auto bounds = getLocalBounds();
    
    if (getToggleState())
    {
        // Active connection - bright color
        g.setColour(juce::Colours::orange);
        g.fillRect(bounds);
        
        g.setColour(juce::Colours::white);
        g.drawRect(bounds, 1);
    }
    else
    {
        // Inactive - dark with subtle border
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(bounds);
        
        g.setColour(juce::Colours::grey);
        g.drawRect(bounds, 1);
    }
}
