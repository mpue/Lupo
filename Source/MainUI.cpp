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
#include "AudioEngine/LupoSynth.h"
//[/Headers]

#include "MainUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainUI::MainUI (LupoSynth* lupo)
{
    //[Constructor_pre] You can add your own custom stuff here..
	this->lupo = lupo;
    //[/Constructor_pre]

    groupComponent7.reset (new GroupComponent ("new group",
                                               TRANS("Lupo")));
    addAndMakeVisible (groupComponent7.get());

    groupComponent7->setBounds (0, 8, 944, 320);

    groupComponent3.reset (new GroupComponent ("new group",
                                               TRANS("Oscilators")));
    addAndMakeVisible (groupComponent3.get());

    groupComponent3->setBounds (8, 24, 304, 296);

    groupComponent5.reset (new GroupComponent ("new group",
                                               TRANS("Osc 2")));
    addAndMakeVisible (groupComponent5.get());

    groupComponent5->setBounds (16, 176, 288, 136);

    groupComponent4.reset (new GroupComponent ("new group",
                                               TRANS("Osc 1")));
    addAndMakeVisible (groupComponent4.get());

    groupComponent4->setBounds (16, 40, 288, 136);

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS("Amp Envelope")));
    addAndMakeVisible (groupComponent.get());

    groupComponent->setBounds (320, 24, 408, 144);

    groupComponent2.reset (new GroupComponent ("new group",
                                               TRANS("Filter Envelope")));
    addAndMakeVisible (groupComponent2.get());

    groupComponent2->setBounds (320, 176, 528, 144);

    osc1Pitch.reset (new Slider ("osc1Pitch"));
    addAndMakeVisible (osc1Pitch.get());
    osc1Pitch->setRange (-36, 36, 1);
    osc1Pitch->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1Pitch->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    osc1Pitch->addListener (this);

    osc1Pitch->setBounds (32, 64, 80, 80);

    osc2Pitch.reset (new Slider ("osc2Pitch"));
    addAndMakeVisible (osc2Pitch.get());
    osc2Pitch->setRange (-36, 36, 1);
    osc2Pitch->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2Pitch->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    osc2Pitch->addListener (this);

    osc2Pitch->setBounds (32, 200, 80, 80);

    pitchlabel1.reset (new Label ("pitchlabel1",
                                  TRANS("Pitch")));
    addAndMakeVisible (pitchlabel1.get());
    pitchlabel1->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    pitchlabel1->setJustificationType (Justification::centredLeft);
    pitchlabel1->setEditable (false, false, false);
    pitchlabel1->setColour (TextEditor::textColourId, Colours::black);
    pitchlabel1->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    pitchlabel1->setBounds (32, 144, 39, 24);

    pitchlabel2.reset (new Label ("pitchlabel1",
                                  TRANS("Pitch")));
    addAndMakeVisible (pitchlabel2.get());
    pitchlabel2->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    pitchlabel2->setJustificationType (Justification::centredLeft);
    pitchlabel2->setEditable (false, false, false);
    pitchlabel2->setColour (TextEditor::textColourId, Colours::black);
    pitchlabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    pitchlabel2->setBounds (32, 280, 39, 24);

    amp_attack.reset (new Slider ("amp_attack"));
    addAndMakeVisible (amp_attack.get());
    amp_attack->setRange (0, 10, 0.01);
    amp_attack->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_attack->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_attack->addListener (this);

    amp_attack->setBounds (336, 40, 80, 80);

    amp_decay.reset (new Slider ("amp_decay"));
    addAndMakeVisible (amp_decay.get());
    amp_decay->setRange (0, 5, 0.01);
    amp_decay->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_decay->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_decay->addListener (this);

    amp_decay->setBounds (432, 40, 80, 80);

    amp_sustain.reset (new Slider ("amp_sustain"));
    addAndMakeVisible (amp_sustain.get());
    amp_sustain->setRange (0, 1, 0.01);
    amp_sustain->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_sustain->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_sustain->addListener (this);

    amp_sustain->setBounds (528, 40, 80, 80);

    amp_release.reset (new Slider ("amp_release"));
    addAndMakeVisible (amp_release.get());
    amp_release->setRange (0, 10, 0.01);
    amp_release->setSliderStyle (Slider::RotaryVerticalDrag);
    amp_release->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    amp_release->addListener (this);

    amp_release->setBounds (624, 40, 80, 80);

    attack.reset (new Label ("attack",
                             TRANS("Attack")));
    addAndMakeVisible (attack.get());
    attack->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    attack->setJustificationType (Justification::centredLeft);
    attack->setEditable (false, false, false);
    attack->setColour (TextEditor::textColourId, Colours::black);
    attack->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    attack->setBounds (352, 128, 56, 24);

    decay.reset (new Label ("decay",
                            TRANS("Decay\n")));
    addAndMakeVisible (decay.get());
    decay->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    decay->setJustificationType (Justification::centredLeft);
    decay->setEditable (false, false, false);
    decay->setColour (TextEditor::textColourId, Colours::black);
    decay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    decay->setBounds (448, 128, 56, 24);

    sustain.reset (new Label ("sustain",
                              TRANS("Sustain")));
    addAndMakeVisible (sustain.get());
    sustain->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sustain->setJustificationType (Justification::centredLeft);
    sustain->setEditable (false, false, false);
    sustain->setColour (TextEditor::textColourId, Colours::black);
    sustain->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sustain->setBounds (544, 128, 56, 24);

    Release.reset (new Label ("Release",
                              TRANS("Release")));
    addAndMakeVisible (Release.get());
    Release->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Release->setJustificationType (Justification::centredLeft);
    Release->setEditable (false, false, false);
    Release->setColour (TextEditor::textColourId, Colours::black);
    Release->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Release->setBounds (632, 128, 56, 24);

    osc1Fine.reset (new Slider ("osc1Fine"));
    addAndMakeVisible (osc1Fine.get());
    osc1Fine->setRange (-1, 1, 0.01);
    osc1Fine->setSliderStyle (Slider::RotaryVerticalDrag);
    osc1Fine->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    osc1Fine->addListener (this);

    osc1Fine->setBounds (120, 80, 64, 64);

    osc2Fine.reset (new Slider ("osc2Fine"));
    addAndMakeVisible (osc2Fine.get());
    osc2Fine->setRange (-1, 1, 0.01);
    osc2Fine->setSliderStyle (Slider::RotaryVerticalDrag);
    osc2Fine->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    osc2Fine->addListener (this);

    osc2Fine->setBounds (120, 216, 64, 64);

    fineLabel.reset (new Label ("fineLabel",
                                TRANS("Fine\n")));
    addAndMakeVisible (fineLabel.get());
    fineLabel->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    fineLabel->setJustificationType (Justification::centredLeft);
    fineLabel->setEditable (false, false, false);
    fineLabel->setColour (TextEditor::textColourId, Colours::black);
    fineLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    fineLabel->setBounds (120, 144, 39, 24);

    fineLabel2.reset (new Label ("fineLabel",
                                 TRANS("Fine\n")));
    addAndMakeVisible (fineLabel2.get());
    fineLabel2->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    fineLabel2->setJustificationType (Justification::centredLeft);
    fineLabel2->setEditable (false, false, false);
    fineLabel2->setColour (TextEditor::textColourId, Colours::black);
    fineLabel2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    fineLabel2->setBounds (120, 280, 39, 24);

    flt_attack.reset (new Slider ("flt_attack"));
    addAndMakeVisible (flt_attack.get());
    flt_attack->setRange (0, 10, 0.01);
    flt_attack->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_attack->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_attack->addListener (this);

    flt_attack->setBounds (335, 202, 80, 80);

    flt_decay.reset (new Slider ("flt_decay"));
    addAndMakeVisible (flt_decay.get());
    flt_decay->setRange (0, 5, 0.01);
    flt_decay->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_decay->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_decay->addListener (this);

    flt_decay->setBounds (431, 202, 80, 80);

    flt_sustain.reset (new Slider ("flt_sustain"));
    addAndMakeVisible (flt_sustain.get());
    flt_sustain->setRange (0, 1, 0.01);
    flt_sustain->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_sustain->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_sustain->addListener (this);

    flt_sustain->setBounds (527, 202, 80, 80);

    flt_release.reset (new Slider ("flt_release"));
    addAndMakeVisible (flt_release.get());
    flt_release->setRange (0, 10, 0.01);
    flt_release->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_release->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_release->addListener (this);

    flt_release->setBounds (623, 202, 80, 80);

    attack2.reset (new Label ("attack",
                              TRANS("Attack")));
    addAndMakeVisible (attack2.get());
    attack2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    attack2->setJustificationType (Justification::centredLeft);
    attack2->setEditable (false, false, false);
    attack2->setColour (TextEditor::textColourId, Colours::black);
    attack2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    attack2->setBounds (351, 290, 56, 24);

    decay2.reset (new Label ("decay",
                             TRANS("Decay\n")));
    addAndMakeVisible (decay2.get());
    decay2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    decay2->setJustificationType (Justification::centredLeft);
    decay2->setEditable (false, false, false);
    decay2->setColour (TextEditor::textColourId, Colours::black);
    decay2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    decay2->setBounds (447, 290, 56, 24);

    sustain2.reset (new Label ("sustain",
                               TRANS("Sustain")));
    addAndMakeVisible (sustain2.get());
    sustain2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sustain2->setJustificationType (Justification::centredLeft);
    sustain2->setEditable (false, false, false);
    sustain2->setColour (TextEditor::textColourId, Colours::black);
    sustain2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sustain2->setBounds (543, 290, 56, 24);

    Release2.reset (new Label ("Release",
                               TRANS("Release")));
    addAndMakeVisible (Release2.get());
    Release2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Release2->setJustificationType (Justification::centredLeft);
    Release2->setEditable (false, false, false);
    Release2->setColour (TextEditor::textColourId, Colours::black);
    Release2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Release2->setBounds (631, 290, 56, 24);

    flt_release2.reset (new Slider ("flt_release"));
    addAndMakeVisible (flt_release2.get());
    flt_release2->setRange (0, 1, 0.01);
    flt_release2->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_release2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_release2->addListener (this);

    flt_release2->setBounds (736, 200, 80, 80);

    EnvAmt.reset (new Label ("EnvAmt",
                             TRANS("Amount")));
    addAndMakeVisible (EnvAmt.get());
    EnvAmt->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    EnvAmt->setJustificationType (Justification::centredLeft);
    EnvAmt->setEditable (false, false, false);
    EnvAmt->setColour (TextEditor::textColourId, Colours::black);
    EnvAmt->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    EnvAmt->setBounds (744, 288, 56, 24);

    groupComponent6.reset (new GroupComponent ("new group",
                                               TRANS("Filter")));
    addAndMakeVisible (groupComponent6.get());

    groupComponent6->setBounds (736, 24, 200, 144);

    flt_cutoff.reset (new Slider ("flt_cutoff"));
    addAndMakeVisible (flt_cutoff.get());
    flt_cutoff->setRange (0, 18000, 10);
    flt_cutoff->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_cutoff->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_cutoff->addListener (this);

    flt_cutoff->setBounds (752, 40, 80, 80);

    flt_res.reset (new Slider ("flt_res"));
    addAndMakeVisible (flt_res.get());
    flt_res->setRange (0, 1, 0.01);
    flt_res->setSliderStyle (Slider::RotaryVerticalDrag);
    flt_res->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    flt_res->addListener (this);

    flt_res->setBounds (840, 40, 80, 80);

    Cutoff.reset (new Label ("Cutoff",
                             TRANS("Cutoff\n")));
    addAndMakeVisible (Cutoff.get());
    Cutoff->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Cutoff->setJustificationType (Justification::centredLeft);
    Cutoff->setEditable (false, false, false);
    Cutoff->setColour (TextEditor::textColourId, Colours::black);
    Cutoff->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Cutoff->setBounds (768, 128, 56, 24);

    Res.reset (new Label ("Res",
                          TRANS("Q")));
    addAndMakeVisible (Res.get());
    Res->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Res->setJustificationType (Justification::centredLeft);
    Res->setEditable (false, false, false);
    Res->setColour (TextEditor::textColourId, Colours::black);
    Res->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Res->setBounds (872, 128, 24, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1000, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MainUI::~MainUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupComponent7 = nullptr;
    groupComponent3 = nullptr;
    groupComponent5 = nullptr;
    groupComponent4 = nullptr;
    groupComponent = nullptr;
    groupComponent2 = nullptr;
    osc1Pitch = nullptr;
    osc2Pitch = nullptr;
    pitchlabel1 = nullptr;
    pitchlabel2 = nullptr;
    amp_attack = nullptr;
    amp_decay = nullptr;
    amp_sustain = nullptr;
    amp_release = nullptr;
    attack = nullptr;
    decay = nullptr;
    sustain = nullptr;
    Release = nullptr;
    osc1Fine = nullptr;
    osc2Fine = nullptr;
    fineLabel = nullptr;
    fineLabel2 = nullptr;
    flt_attack = nullptr;
    flt_decay = nullptr;
    flt_sustain = nullptr;
    flt_release = nullptr;
    attack2 = nullptr;
    decay2 = nullptr;
    sustain2 = nullptr;
    Release2 = nullptr;
    flt_release2 = nullptr;
    EnvAmt = nullptr;
    groupComponent6 = nullptr;
    flt_cutoff = nullptr;
    flt_res = nullptr;
    Cutoff = nullptr;
    Res = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainUI::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == osc1Pitch.get())
    {
        //[UserSliderCode_osc1Pitch] -- add your slider handling code here..
		lupo->setPitch(1, sliderThatWasMoved->getValue());
        //[/UserSliderCode_osc1Pitch]
    }
    else if (sliderThatWasMoved == osc2Pitch.get())
    {
        //[UserSliderCode_osc2Pitch] -- add your slider handling code here..
		lupo->setPitch(2, sliderThatWasMoved->getValue());
        //[/UserSliderCode_osc2Pitch]
    }
    else if (sliderThatWasMoved == amp_attack.get())
    {
        //[UserSliderCode_amp_attack] -- add your slider handling code here..
		lupo->setAmpAttack(sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_attack]
    }
    else if (sliderThatWasMoved == amp_decay.get())
    {
        //[UserSliderCode_amp_decay] -- add your slider handling code here..
		lupo->setAmpDecay(sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_decay]
    }
    else if (sliderThatWasMoved == amp_sustain.get())
    {
        //[UserSliderCode_amp_sustain] -- add your slider handling code here..
		lupo->setAmpSustain(sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_sustain]
    }
    else if (sliderThatWasMoved == amp_release.get())
    {
        //[UserSliderCode_amp_release] -- add your slider handling code here..
		lupo->setAmpRelease(sliderThatWasMoved->getValue());
        //[/UserSliderCode_amp_release]
    }
    else if (sliderThatWasMoved == osc1Fine.get())
    {
        //[UserSliderCode_osc1Fine] -- add your slider handling code here..
        //[/UserSliderCode_osc1Fine]
    }
    else if (sliderThatWasMoved == osc2Fine.get())
    {
        //[UserSliderCode_osc2Fine] -- add your slider handling code here..
        //[/UserSliderCode_osc2Fine]
    }
    else if (sliderThatWasMoved == flt_attack.get())
    {
        //[UserSliderCode_flt_attack] -- add your slider handling code here..
        //[/UserSliderCode_flt_attack]
    }
    else if (sliderThatWasMoved == flt_decay.get())
    {
        //[UserSliderCode_flt_decay] -- add your slider handling code here..
        //[/UserSliderCode_flt_decay]
    }
    else if (sliderThatWasMoved == flt_sustain.get())
    {
        //[UserSliderCode_flt_sustain] -- add your slider handling code here..
        //[/UserSliderCode_flt_sustain]
    }
    else if (sliderThatWasMoved == flt_release.get())
    {
        //[UserSliderCode_flt_release] -- add your slider handling code here..
        //[/UserSliderCode_flt_release]
    }
    else if (sliderThatWasMoved == flt_release2.get())
    {
        //[UserSliderCode_flt_release2] -- add your slider handling code here..
        //[/UserSliderCode_flt_release2]
    }
    else if (sliderThatWasMoved == flt_cutoff.get())
    {
        //[UserSliderCode_flt_cutoff] -- add your slider handling code here..
		lupo->setCutoff(sliderThatWasMoved->getValue());
        //[/UserSliderCode_flt_cutoff]
    }
    else if (sliderThatWasMoved == flt_res.get())
    {
        //[UserSliderCode_flt_res] -- add your slider handling code here..
		lupo->setResonance(sliderThatWasMoved->getValue());
        //[/UserSliderCode_flt_res]
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

<JUCER_COMPONENT documentType="Component" className="MainUI" componentName=""
                 parentClasses="public Component" constructorParams="LupoSynth* lupo"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="1000" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="34c663503f4553a7" memberName="groupComponent7"
                  virtualName="" explicitFocusOrder="0" pos="0 8 944 320" title="Lupo"/>
  <GROUPCOMPONENT name="new group" id="be488b129ef124bc" memberName="groupComponent3"
                  virtualName="" explicitFocusOrder="0" pos="8 24 304 296" title="Oscilators"/>
  <GROUPCOMPONENT name="new group" id="525fa9fde4c3cef4" memberName="groupComponent5"
                  virtualName="" explicitFocusOrder="0" pos="16 176 288 136" title="Osc 2"/>
  <GROUPCOMPONENT name="new group" id="92909cf09d44f991" memberName="groupComponent4"
                  virtualName="" explicitFocusOrder="0" pos="16 40 288 136" title="Osc 1"/>
  <GROUPCOMPONENT name="new group" id="a42d8319e5f14b10" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="320 24 408 144" title="Amp Envelope"/>
  <GROUPCOMPONENT name="new group" id="54ad5f2997b90d8a" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="320 176 528 144" title="Filter Envelope"/>
  <SLIDER name="osc1Pitch" id="57e273c20e9f4d5e" memberName="osc1Pitch"
          virtualName="" explicitFocusOrder="0" pos="32 64 80 80" min="-36.0"
          max="36.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="osc2Pitch" id="68bf7610708b8fbe" memberName="osc2Pitch"
          virtualName="" explicitFocusOrder="0" pos="32 200 80 80" min="-36.0"
          max="36.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="pitchlabel1" id="abdc5c483d1b1ed5" memberName="pitchlabel1"
         virtualName="" explicitFocusOrder="0" pos="32 144 39 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pitch" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="pitchlabel1" id="366db22c600bf086" memberName="pitchlabel2"
         virtualName="" explicitFocusOrder="0" pos="32 280 39 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pitch" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="amp_attack" id="dd56ce6b5eccf295" memberName="amp_attack"
          virtualName="" explicitFocusOrder="0" pos="336 40 80 80" min="0.0"
          max="10.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="amp_decay" id="336632529c646979" memberName="amp_decay"
          virtualName="" explicitFocusOrder="0" pos="432 40 80 80" min="0.0"
          max="5.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="amp_sustain" id="cefe10e4212549e2" memberName="amp_sustain"
          virtualName="" explicitFocusOrder="0" pos="528 40 80 80" min="0.0"
          max="1.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="amp_release" id="9ced331028b46d97" memberName="amp_release"
          virtualName="" explicitFocusOrder="0" pos="624 40 80 80" min="0.0"
          max="10.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="attack" id="4938c10e2b6678d9" memberName="attack" virtualName=""
         explicitFocusOrder="0" pos="352 128 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Attack" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="decay" id="b3fe82ab215c0350" memberName="decay" virtualName=""
         explicitFocusOrder="0" pos="448 128 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Decay&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="sustain" id="3a9c1db62849c1a4" memberName="sustain" virtualName=""
         explicitFocusOrder="0" pos="544 128 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sustain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Release" id="a532e9811921ca43" memberName="Release" virtualName=""
         explicitFocusOrder="0" pos="632 128 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="osc1Fine" id="533f1365e15f5919" memberName="osc1Fine" virtualName=""
          explicitFocusOrder="0" pos="120 80 64 64" min="-1.0" max="1.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="osc2Fine" id="a50768aa6278c2d2" memberName="osc2Fine" virtualName=""
          explicitFocusOrder="0" pos="120 216 64 64" min="-1.0" max="1.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="fineLabel" id="bd9e25fff1c8157f" memberName="fineLabel"
         virtualName="" explicitFocusOrder="0" pos="120 144 39 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Fine&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="fineLabel" id="2b67e6b3d8fe0757" memberName="fineLabel2"
         virtualName="" explicitFocusOrder="0" pos="120 280 39 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Fine&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="flt_attack" id="116c6460652f9cf5" memberName="flt_attack"
          virtualName="" explicitFocusOrder="0" pos="335 202 80 80" min="0.0"
          max="10.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="flt_decay" id="e6431f3b00d22953" memberName="flt_decay"
          virtualName="" explicitFocusOrder="0" pos="431 202 80 80" min="0.0"
          max="5.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="flt_sustain" id="d7ce0685292d435d" memberName="flt_sustain"
          virtualName="" explicitFocusOrder="0" pos="527 202 80 80" min="0.0"
          max="1.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="flt_release" id="f81fb664aa7ec2dc" memberName="flt_release"
          virtualName="" explicitFocusOrder="0" pos="623 202 80 80" min="0.0"
          max="10.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="attack" id="bb7257520ca3063c" memberName="attack2" virtualName=""
         explicitFocusOrder="0" pos="351 290 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Attack" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="decay" id="a0b7de16cff5411" memberName="decay2" virtualName=""
         explicitFocusOrder="0" pos="447 290 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Decay&#10;" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="sustain" id="7bfc71c3310f6673" memberName="sustain2" virtualName=""
         explicitFocusOrder="0" pos="543 290 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sustain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Release" id="3d31d573f2e0f3c6" memberName="Release2" virtualName=""
         explicitFocusOrder="0" pos="631 290 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="flt_release" id="1ea7f17dc2554b7c" memberName="flt_release2"
          virtualName="" explicitFocusOrder="0" pos="736 200 80 80" min="0.0"
          max="1.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="EnvAmt" id="fabbb7a7cb7f72a5" memberName="EnvAmt" virtualName=""
         explicitFocusOrder="0" pos="744 288 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Amount" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="new group" id="d21f32d49ecf1836" memberName="groupComponent6"
                  virtualName="" explicitFocusOrder="0" pos="736 24 200 144" title="Filter"/>
  <SLIDER name="flt_cutoff" id="820aba5f43f549e0" memberName="flt_cutoff"
          virtualName="" explicitFocusOrder="0" pos="752 40 80 80" min="0.0"
          max="18000.0" int="10.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="flt_res" id="e9128200d848646a" memberName="flt_res" virtualName=""
          explicitFocusOrder="0" pos="840 40 80 80" min="0.0" max="1.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Cutoff" id="3a68e51a612b8d7" memberName="Cutoff" virtualName=""
         explicitFocusOrder="0" pos="768 128 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Res" id="25ed1f5df93f7ca0" memberName="Res" virtualName=""
         explicitFocusOrder="0" pos="872 128 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Q" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

