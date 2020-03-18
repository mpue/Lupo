/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainUI.h"

//==============================================================================
LupoAudioProcessorEditor::LupoAudioProcessorEditor (LupoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1040, 550);	
	mainUI = new MainUI(p.getSynth());
	addAndMakeVisible(mainUI);
	
}

LupoAudioProcessorEditor::~LupoAudioProcessorEditor()
{
	delete mainUI;
}

//==============================================================================
void LupoAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);

}

void LupoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
