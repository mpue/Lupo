/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainUI.h"
#include "CyberpunkLookAndFeel.h"
#include "AttachmentFactory.h"

//==============================================================================
LupoAudioProcessorEditor::LupoAudioProcessorEditor (LupoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setLookAndFeel(&rlf);
    setSize (1570, 580);	
	setResizable(true,true);
	mainUI = new MainUI(&p, p.getFactory());
	addAndMakeVisible(mainUI);
	mainUI->updatePresetList();
	loadSettings();
	resized();
}

LupoAudioProcessorEditor::~LupoAudioProcessorEditor()
{
	saveSettings();
	setLookAndFeel(nullptr);
	if (JUCEApplication::isStandaloneApp()) {
		delete mainUI;
	}
}

//==============================================================================
void LupoAudioProcessorEditor::paint (Graphics& g)
{
    // Fill with cyberpunk black background
    g.fillAll(CyberpunkLookAndFeel::CYBER_BLACK);
    
    // Add some cyberpunk atmosphere effects
    cyberpunkLaf.drawCyberGrid(g, getLocalBounds(), 25.0f);
    cyberpunkLaf.drawHexagonalPattern(g, getLocalBounds(), 12.0f);
    
    // Add subtle corner glows
    Rectangle<float> topLeft(0, 0, 100, 100);
    Rectangle<float> topRight(getWidth() - 100, 0, 100, 100);
    Rectangle<float> bottomLeft(0, getHeight() - 100, 100, 100);
    Rectangle<float> bottomRight(getWidth() - 100, getHeight() - 100, 100, 100);
    
    cyberpunkLaf.drawGlowEffect(g, topLeft, CyberpunkLookAndFeel::CYBER_GLOW, 0.2f);
    cyberpunkLaf.drawGlowEffect(g, topRight, CyberpunkLookAndFeel::CYBER_GLOW, 0.2f);
    cyberpunkLaf.drawGlowEffect(g, bottomLeft, CyberpunkLookAndFeel::CYBER_GLOW, 0.2f);
    cyberpunkLaf.drawGlowEffect(g, bottomRight, CyberpunkLookAndFeel::CYBER_GLOW, 0.2f);
    
    // Add neon border around the entire interface
    cyberpunkLaf.drawNeonBorder(g, getLocalBounds().toFloat().reduced(2), 
                                CyberpunkLookAndFeel::CYBER_YELLOW, 1.5f);
}

void LupoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	if (mainUI != nullptr) {
		mainUI->setBounds(0, 0, getWidth(), getHeight());
	}
}


void LupoAudioProcessorEditor::saveSettings()
{
	String userHome = File::getSpecialLocation(File::userHomeDirectory).getFullPathName();
	File appDir = File(userHome + "/.Lupo");

	if (!appDir.exists()) {
		appDir.createDirectory();
	}

	File configFile = File(userHome + "/.Lupo/settings.xml");

	if (!configFile.exists()) {
		configFile.create();
	}
	else {
		configFile.deleteFile();
		configFile = File(userHome + "/.Lupo/settings.xml");
		configFile.create();
	}

	ValueTree* v = new ValueTree("Settings");

	

	std::unique_ptr<XmlElement> xml = v->createXml();
	xml->writeToFile(configFile, "");

	xml = nullptr;
	delete v;
}

void LupoAudioProcessorEditor::loadSettings()
{
	String userHome = File::getSpecialLocation(File::userHomeDirectory).getFullPathName();

	File appDir = File(userHome + "/.Lupo");

	if (!appDir.exists()) {
		appDir.createDirectory();
	}

	File configFile = File(userHome + "/.Lupo/settings.xml");

	if (configFile.exists()) {
		std::unique_ptr<XmlElement> xml = XmlDocument(configFile).getDocumentElement();
		ValueTree v = ValueTree::fromXml(*xml.get());
		
		xml = nullptr;
	}
}