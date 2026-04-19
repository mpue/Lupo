/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "AttachmentFactory.h"
#include "AudioEngine/Arpeggiator.h"
//[/Headers]

#include "ArpPanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ArpPanel::ArpPanel (AttachmentFactory* factory, Arpeggiator* arp )
{
    //[Constructor_pre] You can add your own custom stuff here..
	this->factory = factory;
	this->arp = arp;
    //[/Constructor_pre]

    enabledButton.reset (new ToggleButton ("enabledButton"));
    addAndMakeVisible (enabledButton.get());
    enabledButton->setButtonText (TRANS("On"));
    enabledButton->addListener (this);
    enabledButton->setBounds (16, 20, 100, 32);

    latchButton.reset (new ToggleButton ("latchButton"));
    addAndMakeVisible (latchButton.get());
    latchButton->setButtonText (TRANS("Latch"));
    latchButton->addListener (this);
    latchButton->setBounds (130, 20, 100, 32);

    speedSlider.reset (new Slider ("speedSlider"));
    addAndMakeVisible (speedSlider.get());
    speedSlider->setRange (0, 3, 1);
    speedSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    speedSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    speedSlider->addListener (this);
    speedSlider->setBounds (32, 80, 80, 80);

    label.reset (new Label ("new label",
                            TRANS("Speed")));
    addAndMakeVisible (label.get());
    label->setFont (Font (13.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    label->setBounds (32, 164, 80, 20);

    tempoSlider.reset (new Slider ("tempoSlider"));
    addAndMakeVisible (tempoSlider.get());
    tempoSlider->setRange (60, 200, 1);
    tempoSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    tempoSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 60, 20);
    tempoSlider->addListener (this);
    tempoSlider->setBounds (160, 80, 80, 80);

    tempoLabel.reset (new Label ("tempoLabel",
                                TRANS("Tempo")));
    addAndMakeVisible (tempoLabel.get());
    tempoLabel->setFont (Font (13.00f, Font::plain).withTypefaceStyle ("Regular"));
    tempoLabel->setJustificationType (Justification::centred);
    tempoLabel->setEditable (false, false, false);
    tempoLabel->setColour (TextEditor::textColourId, Colours::black);
    tempoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    tempoLabel->setBounds (160, 184, 80, 20);

    octaveLabel.reset (new Label ("octaveLabel",
                                  TRANS("Octaves")));
    addAndMakeVisible (octaveLabel.get());
    octaveLabel->setFont (Font (13.00f, Font::plain).withTypefaceStyle ("Regular"));
    octaveLabel->setJustificationType (Justification::centredLeft);
    octaveLabel->setEditable (false, false, false);
    octaveLabel->setColour (TextEditor::textColourId, Colours::black);
    octaveLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    octaveLabel->setBounds (280, 80, 88, 22);

    octaveCombo.reset (new ComboBox ("octaveCombo"));
    addAndMakeVisible (octaveCombo.get());
    octaveCombo->setEditableText (false);
    octaveCombo->setJustificationType (Justification::centredLeft);
    octaveCombo->setTextWhenNothingSelected (String());
    octaveCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    octaveCombo->addItem (TRANS("1"), 1);
    octaveCombo->addItem (TRANS("2"), 2);
    octaveCombo->addItem (TRANS("3"), 3);
    octaveCombo->addItem (TRANS("4"), 4);
    octaveCombo->addListener (this);
    octaveCombo->setBounds (280, 104, 88, 28);

    modeLabel.reset (new Label ("modeLabel",
                                TRANS("Mode")));
    addAndMakeVisible (modeLabel.get());
    modeLabel->setFont (Font (13.00f, Font::plain).withTypefaceStyle ("Regular"));
    modeLabel->setJustificationType (Justification::centredLeft);
    modeLabel->setEditable (false, false, false);
    modeLabel->setColour (TextEditor::textColourId, Colours::black);
    modeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    modeLabel->setBounds (280, 152, 88, 22);

    modeCombo.reset (new ComboBox ("modeCombo"));
    addAndMakeVisible (modeCombo.get());
    modeCombo->setEditableText (false);
    modeCombo->setJustificationType (Justification::centredLeft);
    modeCombo->setTextWhenNothingSelected (String());
    modeCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    modeCombo->addItem (TRANS("Up"), 1);
    modeCombo->addItem (TRANS("Down"), 2);
    modeCombo->addItem (TRANS("Random"), 3);
    modeCombo->addItem (TRANS("Chord"), 4);
    modeCombo->addListener (this);
    modeCombo->setBounds (280, 176, 88, 28);

    clockModeLabel.reset (new Label ("clockModeLabel",
                                    TRANS("Clock")));
    addAndMakeVisible (clockModeLabel.get());
    clockModeLabel->setFont (Font (13.00f, Font::plain).withTypefaceStyle ("Regular"));
    clockModeLabel->setJustificationType (Justification::centredLeft);
    clockModeLabel->setEditable (false, false, false);
    clockModeLabel->setColour (TextEditor::textColourId, Colours::black);
    clockModeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    clockModeLabel->setBounds (384, 80, 80, 22);

    clockModeCombo.reset (new ComboBox ("clockModeCombo"));
    addAndMakeVisible (clockModeCombo.get());
    clockModeCombo->setEditableText (false);
    clockModeCombo->setJustificationType (Justification::centredLeft);
    clockModeCombo->setTextWhenNothingSelected (String());
    clockModeCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    clockModeCombo->addItem (TRANS("Internal"), 1);
    clockModeCombo->addItem (TRANS("MIDI"), 2);
    clockModeCombo->addListener (this);
    clockModeCombo->setBounds (384, 104, 80, 28);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	modeCombo.get()->setSelectedItemIndex(0);
	octaveCombo.get()->setSelectedItemIndex(0);
	clockModeCombo.get()->setSelectedItemIndex(0);  // Default: Internal
	speedSlider.get()->setValue(2, juce::NotificationType::dontSendNotification);  // Default: 1/16
	tempoSlider.get()->setValue(120.0f, juce::NotificationType::dontSendNotification);  // Default: 120 BPM
    //[/Constructor]
}

ArpPanel::~ArpPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    speedSlider = nullptr;
    tempoSlider = nullptr;
    clockModeCombo = nullptr;
    octaveCombo = nullptr;
    label = nullptr;
    tempoLabel = nullptr;
    clockModeLabel = nullptr;
    octaveLabel = nullptr;
    modeCombo = nullptr;
    modeLabel = nullptr;
    enabledButton = nullptr;
    latchButton = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ArpPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ArpPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ArpPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == speedSlider.get())
    {
        //[UserSliderCode_speedSlider] -- add your slider handling code here..
		arp->setDivisionIndex(static_cast<int>(speedSlider->getValue()));
        //[/UserSliderCode_speedSlider]
    }
    else if (sliderThatWasMoved == tempoSlider.get())
    {
        //[UserSliderCode_tempoSlider] -- add your slider handling code here..
        arp->setTempo(static_cast<float>(tempoSlider->getValue()));
        //[/UserSliderCode_tempoSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void ArpPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == octaveCombo.get())
    {
        //[UserComboBoxCode_octaveCombo] -- add your combo box handling code here..
		  arp->setOctaves(comboBoxThatHasChanged->getSelectedItemIndex() + 1);
        //[/UserComboBoxCode_octaveCombo]
    }
    else if (comboBoxThatHasChanged == modeCombo.get())
    {
        //[UserComboBoxCode_modeCombo] -- add your combo box handling code here..
        {
            int idx = comboBoxThatHasChanged->getSelectedItemIndex();
            if      (idx == 0) arp->setMode(Arpeggiator::Mode::Up);
            else if (idx == 1) arp->setMode(Arpeggiator::Mode::Down);
            else if (idx == 2) arp->setMode(Arpeggiator::Mode::Random);
            else if (idx == 3) arp->setMode(Arpeggiator::Mode::Chord);

            // Write to APVTS so the mode is saved with the preset
            if (factory != nullptr)
                if (auto* apvts = factory->getValueTreeState())
                    if (auto* param = apvts->getParameter("arpMode"))
                        param->setValueNotifyingHost(apvts->getParameterRange("arpMode").convertTo0to1((float)idx));
        }
        //[/UserComboBoxCode_modeCombo]
    }
    else if (comboBoxThatHasChanged == clockModeCombo.get())
    {
        //[UserComboBoxCode_clockModeCombo] -- add your combo box handling code here..
        if (comboBoxThatHasChanged->getSelectedItemIndex() == 0) {
            arp->setClockMode(Arpeggiator::ClockMode::Internal);
            // Enable tempo slider for internal clock
            tempoSlider->setEnabled(true);
            tempoLabel->setEnabled(true);
        }
        else if (comboBoxThatHasChanged->getSelectedItemIndex() == 1) {
            arp->setClockMode(Arpeggiator::ClockMode::Midi);
            // Disable tempo slider for MIDI clock (uses external tempo)
            tempoSlider->setEnabled(false);
            tempoLabel->setEnabled(false);
        }
        //[/UserComboBoxCode_clockModeCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void ArpPanel::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == enabledButton.get())
    {
        //[UserButtonCode_enabledButton] -- add your button handler code here..
        bool enabled = enabledButton->getToggleState();
        arp->setEnabled(enabled);
        // Also write to the APVTS parameter so the state is saved with presets
        if (factory)
        {
            auto* apvts = factory->getValueTreeState();
            if (apvts)
            {
                if (auto* param = apvts->getParameter("arpEnabled"))
                {
                    auto range = apvts->getParameterRange("arpEnabled");
                    param->setValueNotifyingHost(range.convertTo0to1(enabled ? 1.0f : 0.0f));
                }
            }
        }
        //[/UserButtonCode_enabledButton]
    }
    else if (buttonThatWasClicked == latchButton.get())
    {
        bool on = latchButton->getToggleState();
        arp->setLatch(on);
        if (factory)
        {
            auto* apvts = factory->getValueTreeState();
            if (apvts)
            {
                if (auto* param = apvts->getParameter("arpLatch"))
                {
                    auto range = apvts->getParameterRange("arpLatch");
                    param->setValueNotifyingHost(range.convertTo0to1(on ? 1.0f : 0.0f));
                }
            }
        }
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ArpPanel::syncArpEnabled(bool enabled)
{
    enabledButton->setToggleState(enabled, juce::dontSendNotification);
    arp->setEnabled(enabled);
}

void ArpPanel::syncModeCombo(int modeIndex)
{
    modeCombo->setSelectedItemIndex(modeIndex, juce::dontSendNotification);
}

void ArpPanel::syncLatch(bool on)
{
    latchButton->setToggleState(on, juce::dontSendNotification);
    arp->setLatch(on);
}

void ArpPanel::initAttachments()
{
	if (factory == nullptr) {
		return; // Safety check to prevent crash if factory is not initialized
	}
	
    // TODO :: plugin crashes, when trying to init attachments here, WTF?
    
	//factory->createSliderAttachment("arpSpeed", speedSlider.get());
	//factory->createSliderAttachment("arpTempo", tempoSlider.get());
	//factory->createComboAttachment("arpMode", modeCombo.get());
    //factory->createComboAttachment("arpClockMode", clockModeCombo.get());
	//factory->createComboAttachment("arpOctaves", octaveCombo.get());
	//factory->createButtonAttachment("arpEnabled", enabledButton.get());
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ArpPanel" componentName=""
                 parentClasses="public Component" constructorParams="AttachmentFactory* factory, Arpeggiator* arp "
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
  <SLIDER name="speedSlider" id="833adf5cce999dbe" memberName="speedSlider"
          virtualName="" explicitFocusOrder="0" pos="8 0 56 56" min="0.1"
          max="1.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="tempoSlider" id="bcdef01234567890" memberName="tempoSlider"
          virtualName="" explicitFocusOrder="0" pos="128 0 56 56" min="60"
          max="200.0" int="0.1" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="50" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="octaveCombo" id="cf2901baf3ad338a" memberName="octaveCombo"
            virtualName="" explicitFocusOrder="0" pos="72 16 48 24" editable="0"
            layout="33" items="1&#10;2&#10;3&#10;4" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="b746faf96526607c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 56 47 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Speed&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="tempoLabel" id="abc123def456ghi789" memberName="tempoLabel"
         virtualName="" explicitFocusOrder="0" pos="136 76 47 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Tempo&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="clockModeCombo" id="f123456789abcdef" memberName="clockModeCombo"
            virtualName="" explicitFocusOrder="0" pos="200 16 60 24" editable="0"
            layout="33" items="Internal&#10;MIDI" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="clockModeLabel" id="def456abc123ghi789" memberName="clockModeLabel"
         virtualName="" explicitFocusOrder="0" pos="200 48 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Clock" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="octaveLabel" id="45e1c7793f8f724c" memberName="octaveLabel"
         virtualName="" explicitFocusOrder="0" pos="72 48 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Octaves&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="modeCombo" id="74b36556f6ab2928" memberName="modeCombo"
            virtualName="" explicitFocusOrder="0" pos="72 80 48 24" editable="0"
            layout="33" items="Up&#10;Down" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="modeLabel" id="db810af5d1ad888" memberName="modeLabel"
         virtualName="" explicitFocusOrder="0" pos="72 104 56 25" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="enabledButton" id="bec584e638bdadc9" memberName="enabledButton"
                virtualName="" explicitFocusOrder="0" pos="16 80 48 24" buttonText="on"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

