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
#include "MessageBus/MessageBus.h"
//[/Headers]

#include "EnvelopePanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EnvelopePanel::EnvelopePanel ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS("Envelope")));
    addAndMakeVisible (groupComponent.get());

    groupComponent->setBounds (1, -3, 408, 144);

    amp_attack.reset (new Slider ("amp_attack"));
    addAndMakeVisible (amp_attack.get());
    amp_attack->setRange (0, 10, 0.01);
    amp_attack->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_attack->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_attack->addListener (this);

    amp_attack->setBounds (17, 13, 80, 80);

    amp_decay.reset (new Slider ("amp_decay"));
    addAndMakeVisible (amp_decay.get());
    amp_decay->setRange (0, 5, 0.01);
    amp_decay->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_decay->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_decay->addListener (this);

    amp_decay->setBounds (113, 13, 80, 80);

    amp_sustain.reset (new Slider ("amp_sustain"));
    addAndMakeVisible (amp_sustain.get());
    amp_sustain->setRange (0, 1, 0.01);
    amp_sustain->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_sustain->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_sustain->addListener (this);

    amp_sustain->setBounds (209, 13, 80, 80);

    amp_release.reset (new Slider ("amp_release"));
    addAndMakeVisible (amp_release.get());
    amp_release->setRange (0, 10, 0.01);
    amp_release->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_release->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_release->addListener (this);

    amp_release->setBounds (305, 13, 80, 80);

    attack.reset (new Label ("attack",
                             TRANS("Attack")));
    addAndMakeVisible (attack.get());
    attack->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    attack->setJustificationType (Justification::centredLeft);
    attack->setEditable (false, false, false);
    attack->setColour (TextEditor::textColourId, Colours::black);
    attack->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    attack->setBounds (33, 101, 56, 24);

    decay.reset (new Label ("decay",
                            TRANS("Decay\n")));
    addAndMakeVisible (decay.get());
    decay->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    decay->setJustificationType (Justification::centredLeft);
    decay->setEditable (false, false, false);
    decay->setColour (TextEditor::textColourId, Colours::black);
    decay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    decay->setBounds (129, 101, 56, 24);

    sustain.reset (new Label ("sustain",
                              TRANS("Sustain")));
    addAndMakeVisible (sustain.get());
    sustain->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sustain->setJustificationType (Justification::centredLeft);
    sustain->setEditable (false, false, false);
    sustain->setColour (TextEditor::textColourId, Colours::black);
    sustain->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sustain->setBounds (225, 101, 56, 24);

    Release.reset (new Label ("Release",
                              TRANS("Release")));
    addAndMakeVisible (Release.get());
    Release->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Release->setJustificationType (Justification::centredLeft);
    Release->setEditable (false, false, false);
    Release->setColour (TextEditor::textColourId, Colours::black);
    Release->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Release->setBounds (313, 101, 56, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

EnvelopePanel::~EnvelopePanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupComponent = nullptr;
    amp_attack = nullptr;
    amp_decay = nullptr;
    amp_sustain = nullptr;
    amp_release = nullptr;
    attack = nullptr;
    decay = nullptr;
    sustain = nullptr;
    Release = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EnvelopePanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EnvelopePanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EnvelopePanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == amp_attack.get())
    {
        //[UserSliderCode_amp_attack] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic(getName() + ".attack", sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_attack]
    }
    else if (sliderThatWasMoved == amp_decay.get())
    {
        //[UserSliderCode_amp_decay] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic(getName() + ".decay", sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_decay]
    }
    else if (sliderThatWasMoved == amp_sustain.get())
    {
        //[UserSliderCode_amp_sustain] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic(getName() + ".sustain", sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_sustain]
    }
    else if (sliderThatWasMoved == amp_release.get())
    {
        //[UserSliderCode_amp_release] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic(getName() + ".release", sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_release]
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

<JUCER_COMPONENT documentType="Component" className="EnvelopePanel" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="a42d8319e5f14b10" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="1 -3 408 144" title="Envelope"/>
  <SLIDER name="amp_attack" id="dd56ce6b5eccf295" memberName="amp_attack"
          virtualName="" explicitFocusOrder="0" pos="17 13 80 80" min="0.0"
          max="10.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="amp_decay" id="336632529c646979" memberName="amp_decay"
          virtualName="" explicitFocusOrder="0" pos="113 13 80 80" min="0.0"
          max="5.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="amp_sustain" id="cefe10e4212549e2" memberName="amp_sustain"
          virtualName="" explicitFocusOrder="0" pos="209 13 80 80" min="0.0"
          max="1.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="amp_release" id="9ced331028b46d97" memberName="amp_release"
          virtualName="" explicitFocusOrder="0" pos="305 13 80 80" min="0.0"
          max="10.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="attack" id="4938c10e2b6678d9" memberName="attack" virtualName=""
         explicitFocusOrder="0" pos="33 101 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Attack" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="decay" id="b3fe82ab215c0350" memberName="decay" virtualName=""
         explicitFocusOrder="0" pos="129 101 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Decay&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="sustain" id="3a9c1db62849c1a4" memberName="sustain" virtualName=""
         explicitFocusOrder="0" pos="225 101 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sustain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Release" id="a532e9811921ca43" memberName="Release" virtualName=""
         explicitFocusOrder="0" pos="313 101 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
