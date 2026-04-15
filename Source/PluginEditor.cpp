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
	setLookAndFeel(&edlf);

	// Native design resolution
	constexpr int kNativeW = 1550;
	constexpr int kNativeH = 780;

	sizeConstrainer.setFixedAspectRatio((double)kNativeW / (double)kNativeH);
	sizeConstrainer.setMinimumSize(kNativeW / 2, kNativeH / 2);
	sizeConstrainer.setMaximumSize(kNativeW * 2, kNativeH * 2);
	setConstrainer(&sizeConstrainer);
	setResizable(true, true);
	setSize(kNativeW, kNativeH);

	mainUI = std::make_unique<MainUI>(&p, p.getFactory());
	mainUI->setBounds(0, 0, kNativeW, kNativeH);
	addAndMakeVisible(mainUI.get());
	mainUI->updatePresetList();
	loadSettings();
	resized();
}

LupoAudioProcessorEditor::~LupoAudioProcessorEditor()
{
	saveSettings();
	setLookAndFeel(nullptr);
	if (JUCEApplication::isStandaloneApp()) {
		mainUI = nullptr;
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
	if (mainUI != nullptr)
	{
		constexpr float kNativeW = 1550.0f;
		constexpr float kNativeH = 780.0f;
		float scale = (float)getWidth() / kNativeW;
		mainUI->setBounds(0, 0, (int)kNativeW, (int)kNativeH);
		mainUI->setTransform(AffineTransform::scale(scale));
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

	ValueTree v("Settings");
	auto xml = v.createXml();
	
	if (xml != nullptr)
		xml->writeToFile(configFile, "");
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
		
		if (xml != nullptr) {
			ValueTree v = ValueTree::fromXml(*xml);
			// TODO: apply settings from ValueTree
		}
	}
}
