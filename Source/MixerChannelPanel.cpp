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
//[/Headers]

#include "MixerChannelPanel.h"
#include "MessageBus/MessageBus.h"
#include "MessageBus/Topic.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MixerChannelPanel::MixerChannelPanel ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    channelGroup.reset (new GroupComponent ("channelGroup",
                                            TRANS("CH1")));
    addAndMakeVisible (channelGroup.get());

    channelGroup->setBounds (0, 0, 112, 128);

    volSlider.reset (new Slider ("volSlider"));
    addAndMakeVisible (volSlider.get());
    volSlider->setRange (0, 1, 0.01);
    volSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    volSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    volSlider->addListener (this);

    volSlider->setBounds (16, 16, 56, 48);

    panSlider.reset (new Slider ("panSlider"));
    addAndMakeVisible (panSlider.get());
    panSlider->setRange (-1, 1, 0.01);
    panSlider->setSliderStyle (Slider::RotaryHorizontalDrag);
    panSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    panSlider->addListener (this);

    panSlider->setBounds (16, 64, 56, 48);

    volumeLabel.reset (new Label ("volumeLabel",
                                  TRANS("Vol\n")));
    addAndMakeVisible (volumeLabel.get());
    volumeLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    volumeLabel->setJustificationType (Justification::centredLeft);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    volumeLabel->setBounds (64, 32, 31, 24);

    panLabel.reset (new Label ("panLabel",
                               TRANS("Pan\n")));
    addAndMakeVisible (panLabel.get());
    panLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    panLabel->setJustificationType (Justification::centredLeft);
    panLabel->setEditable (false, false, false);
    panLabel->setColour (TextEditor::textColourId, Colours::black);
    panLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    panLabel->setBounds (64, 80, 40, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MixerChannelPanel::~MixerChannelPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    channelGroup = nullptr;
    volSlider = nullptr;
    panSlider = nullptr;
    volumeLabel = nullptr;
    panLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MixerChannelPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MixerChannelPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MixerChannelPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == volSlider.get())
    {
        //[UserSliderCode_volSlider] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic(getName() + ".volume", sliderThatWasMoved->getValue());
        //[/UserSliderCode_volSlider]
    }
    else if (sliderThatWasMoved == panSlider.get())
    {
        //[UserSliderCode_panSlider] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic(getName() + ".pan", sliderThatWasMoved->getValue());
        //[/UserSliderCode_panSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MixerChannelPanel" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="channelGroup" id="9f266fdaeb7fb76b" memberName="channelGroup"
                  virtualName="" explicitFocusOrder="0" pos="0 0 112 128" title="CH1"/>
  <SLIDER name="volSlider" id="b16b5072d98a689d" memberName="volSlider"
          virtualName="" explicitFocusOrder="0" pos="16 16 56 48" min="0.0"
          max="1.0" int="0.01" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="panSlider" id="5c349ac21e02e861" memberName="panSlider"
          virtualName="" explicitFocusOrder="0" pos="16 64 56 48" min="-1.0"
          max="1.0" int="0.01" style="RotaryHorizontalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="volumeLabel" id="7c2ca28cdcb89db2" memberName="volumeLabel"
         virtualName="" explicitFocusOrder="0" pos="64 32 31 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Vol&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="panLabel" id="27664fc0db1a0720" memberName="panLabel" virtualName=""
         explicitFocusOrder="0" pos="64 80 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pan&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

