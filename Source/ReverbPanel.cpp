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

#include "ReverbPanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ReverbPanel::ReverbPanel ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    reverbGroup.reset (new GroupComponent ("reverbGroup",
                                           TRANS("Reverb")));
    addAndMakeVisible (reverbGroup.get());

    reverbGroup->setBounds (0, 0, 392, 112);

    roomSize.reset (new Slider ("roomSize"));
    addAndMakeVisible (roomSize.get());
    roomSize->setRange (0, 1, 0.01);
    roomSize->setSliderStyle (Slider::RotaryHorizontalDrag);
    roomSize->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    roomSize->addListener (this);

    roomSize->setBounds (8, 16, 56, 64);

    roomSIzeLbl.reset (new Label ("roomSIzeLbl",
                                  TRANS("Room size")));
    addAndMakeVisible (roomSIzeLbl.get());
    roomSIzeLbl->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    roomSIzeLbl->setJustificationType (Justification::centredLeft);
    roomSIzeLbl->setEditable (false, false, false);
    roomSIzeLbl->setColour (TextEditor::textColourId, Colours::black);
    roomSIzeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    roomSIzeLbl->setBounds (8, 80, 63, 24);

    damping.reset (new Slider ("damping"));
    addAndMakeVisible (damping.get());
    damping->setRange (0, 1, 0.01);
    damping->setSliderStyle (Slider::RotaryHorizontalDrag);
    damping->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    damping->addListener (this);

    damping->setBounds (72, 16, 56, 64);

    dampingLbl.reset (new Label ("dampingLbl",
                                 TRANS("Damping")));
    addAndMakeVisible (dampingLbl.get());
    dampingLbl->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    dampingLbl->setJustificationType (Justification::centredLeft);
    dampingLbl->setEditable (false, false, false);
    dampingLbl->setColour (TextEditor::textColourId, Colours::black);
    dampingLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    dampingLbl->setBounds (72, 80, 63, 24);

    damping2.reset (new Slider ("damping"));
    addAndMakeVisible (damping2.get());
    damping2->setRange (0, 1, 0.01);
    damping2->setSliderStyle (Slider::RotaryHorizontalDrag);
    damping2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    damping2->addListener (this);

    damping2->setBounds (137, 16, 56, 64);

    widthLbl.reset (new Label ("widthLbl",
                               TRANS("Width")));
    addAndMakeVisible (widthLbl.get());
    widthLbl->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    widthLbl->setJustificationType (Justification::centredLeft);
    widthLbl->setEditable (false, false, false);
    widthLbl->setColour (TextEditor::textColourId, Colours::black);
    widthLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    widthLbl->setBounds (144, 80, 63, 24);

    dryLevel.reset (new Slider ("dryLevel"));
    addAndMakeVisible (dryLevel.get());
    dryLevel->setRange (0, 1, 0.01);
    dryLevel->setSliderStyle (Slider::RotaryHorizontalDrag);
    dryLevel->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    dryLevel->addListener (this);

    dryLevel->setBounds (200, 16, 56, 64);

    dryLbl.reset (new Label ("dryLbl",
                             TRANS("Dry\n")));
    addAndMakeVisible (dryLbl.get());
    dryLbl->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    dryLbl->setJustificationType (Justification::centredLeft);
    dryLbl->setEditable (false, false, false);
    dryLbl->setColour (TextEditor::textColourId, Colours::black);
    dryLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    dryLbl->setBounds (208, 80, 34, 24);

    wetLevel.reset (new Slider ("wetLevel"));
    addAndMakeVisible (wetLevel.get());
    wetLevel->setRange (0, 1, 0.01);
    wetLevel->setSliderStyle (Slider::RotaryHorizontalDrag);
    wetLevel->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    wetLevel->addListener (this);

    wetLevel->setBounds (264, 16, 56, 64);

    wetLbl.reset (new Label ("wetLbl",
                             TRANS("Wet")));
    addAndMakeVisible (wetLbl.get());
    wetLbl->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    wetLbl->setJustificationType (Justification::centredLeft);
    wetLbl->setEditable (false, false, false);
    wetLbl->setColour (TextEditor::textColourId, Colours::black);
    wetLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    wetLbl->setBounds (272, 80, 34, 24);

    wetLevel2.reset (new Slider ("wetLevel"));
    addAndMakeVisible (wetLevel2.get());
    wetLevel2->setRange (0, 1, 0.01);
    wetLevel2->setSliderStyle (Slider::RotaryHorizontalDrag);
    wetLevel2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    wetLevel2->addListener (this);

    wetLevel2->setBounds (328, 16, 56, 64);

    freezeLbl.reset (new Label ("freezeLbl",
                                TRANS("Freeze\n")));
    addAndMakeVisible (freezeLbl.get());
    freezeLbl->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    freezeLbl->setJustificationType (Justification::centredLeft);
    freezeLbl->setEditable (false, false, false);
    freezeLbl->setColour (TextEditor::textColourId, Colours::black);
    freezeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    freezeLbl->setBounds (336, 80, 48, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ReverbPanel::~ReverbPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    reverbGroup = nullptr;
    roomSize = nullptr;
    roomSIzeLbl = nullptr;
    damping = nullptr;
    dampingLbl = nullptr;
    damping2 = nullptr;
    widthLbl = nullptr;
    dryLevel = nullptr;
    dryLbl = nullptr;
    wetLevel = nullptr;
    wetLbl = nullptr;
    wetLevel2 = nullptr;
    freezeLbl = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ReverbPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ReverbPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ReverbPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == roomSize.get())
    {
        //[UserSliderCode_roomSize] -- add your slider handling code here..
        //[/UserSliderCode_roomSize]
    }
    else if (sliderThatWasMoved == damping.get())
    {
        //[UserSliderCode_damping] -- add your slider handling code here..
        //[/UserSliderCode_damping]
    }
    else if (sliderThatWasMoved == damping2.get())
    {
        //[UserSliderCode_damping2] -- add your slider handling code here..
        //[/UserSliderCode_damping2]
    }
    else if (sliderThatWasMoved == dryLevel.get())
    {
        //[UserSliderCode_dryLevel] -- add your slider handling code here..
        //[/UserSliderCode_dryLevel]
    }
    else if (sliderThatWasMoved == wetLevel.get())
    {
        //[UserSliderCode_wetLevel] -- add your slider handling code here..
        //[/UserSliderCode_wetLevel]
    }
    else if (sliderThatWasMoved == wetLevel2.get())
    {
        //[UserSliderCode_wetLevel2] -- add your slider handling code here..
        //[/UserSliderCode_wetLevel2]
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

<JUCER_COMPONENT documentType="Component" className="ReverbPanel" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
  <GROUPCOMPONENT name="reverbGroup" id="827b920ebb279fef" memberName="reverbGroup"
                  virtualName="" explicitFocusOrder="0" pos="0 0 392 112" title="Reverb"/>
  <SLIDER name="roomSize" id="3affdff2a3157f75" memberName="roomSize" virtualName=""
          explicitFocusOrder="0" pos="8 16 56 64" min="0.0" max="1.0" int="0.01"
          style="RotaryHorizontalDrag" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="roomSIzeLbl" id="1abc27b61ccb8637" memberName="roomSIzeLbl"
         virtualName="" explicitFocusOrder="0" pos="8 80 63 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Room size" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="damping" id="b8bfc21a9c0ca3d0" memberName="damping" virtualName=""
          explicitFocusOrder="0" pos="72 16 56 64" min="0.0" max="1.0"
          int="0.01" style="RotaryHorizontalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="dampingLbl" id="fab930de840213e1" memberName="dampingLbl"
         virtualName="" explicitFocusOrder="0" pos="72 80 63 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Damping" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="damping" id="63317514a3284189" memberName="damping2" virtualName=""
          explicitFocusOrder="0" pos="137 16 56 64" min="0.0" max="1.0"
          int="0.01" style="RotaryHorizontalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="widthLbl" id="ec953c6e67180da9" memberName="widthLbl" virtualName=""
         explicitFocusOrder="0" pos="144 80 63 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Width" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="dryLevel" id="246c47f37893dfef" memberName="dryLevel" virtualName=""
          explicitFocusOrder="0" pos="200 16 56 64" min="0.0" max="1.0"
          int="0.01" style="RotaryHorizontalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="dryLbl" id="8133e7067b6f46f0" memberName="dryLbl" virtualName=""
         explicitFocusOrder="0" pos="208 80 34 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="wetLevel" id="6f75904fe54beef4" memberName="wetLevel" virtualName=""
          explicitFocusOrder="0" pos="264 16 56 64" min="0.0" max="1.0"
          int="0.01" style="RotaryHorizontalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="wetLbl" id="298de79cd0c6bb3d" memberName="wetLbl" virtualName=""
         explicitFocusOrder="0" pos="272 80 34 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="wetLevel" id="17f4021cbeb408a5" memberName="wetLevel2"
          virtualName="" explicitFocusOrder="0" pos="328 16 56 64" min="0.0"
          max="1.0" int="0.01" style="RotaryHorizontalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="freezeLbl" id="e04867dabc2ea1df" memberName="freezeLbl"
         virtualName="" explicitFocusOrder="0" pos="336 80 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Freeze&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
