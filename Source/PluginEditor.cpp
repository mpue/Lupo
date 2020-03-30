/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainUI.h"
#include "TrioLookAndFeel.h"
#include "AttachmentFactory.h"

//==============================================================================
LupoAudioProcessorEditor::LupoAudioProcessorEditor (LupoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setLookAndFeel(&tlf);
    setSize (1000, 900);	
	mainUI = new MainUI(&p, p.getFactory());
	addAndMakeVisible(mainUI);
	mainUI->updatePresetList();
	if (p.selectedProgram.length() > 0) {
		p.setSelectedProgram(p.selectedProgram);
	}	
}

LupoAudioProcessorEditor::~LupoAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
	if (JUCEApplication::isStandaloneApp()) {
		delete mainUI;
	}
}

//==============================================================================
void LupoAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	g.fillAll(Colours::lightgrey);
    g.setColour (Colours::white);

}

void LupoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
