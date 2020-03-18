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
#include "OscillatorPanel.h"

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

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS("Amplifier")));
    addAndMakeVisible (groupComponent.get());

    groupComponent->setBounds (592, 0, 440, 232);

    groupComponent3.reset (new GroupComponent ("new group",
                                               TRANS("Oscilators")));
    addAndMakeVisible (groupComponent3.get());

    groupComponent3->setBounds (8, 0, 456, 536);

    groupComponent6.reset (new GroupComponent ("new group",
                                               TRANS("Filter")));
    addAndMakeVisible (groupComponent6.get());

    groupComponent6->setBounds (592, 232, 440, 304);

    fltCutoff.reset (new Slider ("fltCutoff"));
    addAndMakeVisible (fltCutoff.get());
    fltCutoff->setRange (0, 18000, 10);
    fltCutoff->setSliderStyle (Slider::RotaryVerticalDrag);
    fltCutoff->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    fltCutoff->addListener (this);

    fltCutoff->setBounds (608, 248, 80, 80);

    fltResonance.reset (new Slider ("fltResonance"));
    addAndMakeVisible (fltResonance.get());
    fltResonance->setRange (0, 5, 0.01);
    fltResonance->setSliderStyle (Slider::RotaryVerticalDrag);
    fltResonance->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    fltResonance->addListener (this);

    fltResonance->setBounds (696, 248, 80, 80);

    Cutoff.reset (new Label ("Cutoff",
                             TRANS("Cutoff\n")));
    addAndMakeVisible (Cutoff.get());
    Cutoff->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Cutoff->setJustificationType (Justification::centredLeft);
    Cutoff->setEditable (false, false, false);
    Cutoff->setColour (TextEditor::textColourId, Colours::black);
    Cutoff->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Cutoff->setBounds (624, 336, 56, 24);

    Res.reset (new Label ("Res",
                          TRANS("Q")));
    addAndMakeVisible (Res.get());
    Res->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Res->setJustificationType (Justification::centredLeft);
    Res->setEditable (false, false, false);
    Res->setColour (TextEditor::textColourId, Colours::black);
    Res->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Res->setBounds (728, 336, 24, 24);

    osc1Panel.reset (new OscillatorPanel());
    addAndMakeVisible (osc1Panel.get());
    osc1Panel->setName ("osc1Panel");

    osc1Panel->setBounds (16, 16, 216, 248);

    osc3Panel.reset (new OscillatorPanel());
    addAndMakeVisible (osc3Panel.get());
    osc3Panel->setName ("osc3Panel");

    osc3Panel->setBounds (16, 288, 216, 240);

    osc2Panel.reset (new OscillatorPanel());
    addAndMakeVisible (osc2Panel.get());
    osc2Panel->setName ("osc2Panel");

    osc2Panel->setBounds (240, 16, 216, 248);

    ampEnvelope.reset (new EnvelopePanel());
    addAndMakeVisible (ampEnvelope.get());
    ampEnvelope->setName ("ampEnvelope");

    ampEnvelope->setBounds (608, 16, 408, 144);

    filterEnvelope.reset (new EnvelopePanel());
    addAndMakeVisible (filterEnvelope.get());
    filterEnvelope->setName ("filterEnvelope");

    filterEnvelope->setBounds (608, 368, 408, 144);

    mainVolume.reset (new Slider ("mainVolume"));
    addAndMakeVisible (mainVolume.get());
    mainVolume->setRange (0, 1, 0.01);
    mainVolume->setSliderStyle (Slider::RotaryVerticalDrag);
    mainVolume->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    mainVolume->addListener (this);

    mainVolume->setBounds (952, 160, 64, 64);

    volumeLabel.reset (new Label ("volumeLabel",
                                  TRANS("Main volume")));
    addAndMakeVisible (volumeLabel.get());
    volumeLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    volumeLabel->setJustificationType (Justification::centredLeft);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    volumeLabel->setBounds (864, 176, 96, 24);

    envAmt.reset (new Slider ("envAmt"));
    addAndMakeVisible (envAmt.get());
    envAmt->setRange (0, 1, 0.01);
    envAmt->setSliderStyle (Slider::RotaryVerticalDrag);
    envAmt->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    envAmt->addListener (this);

    envAmt->setBounds (792, 248, 80, 80);

    Amt.reset (new Label ("Amt",
                          TRANS("Env. Amount")));
    addAndMakeVisible (Amt.get());
    Amt->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Amt->setJustificationType (Justification::centredLeft);
    Amt->setEditable (false, false, false);
    Amt->setColour (TextEditor::textColourId, Colours::black);
    Amt->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Amt->setBounds (792, 336, 88, 24);

    mixerGroiup.reset (new GroupComponent ("mixerGroiup",
                                           TRANS("Mixer")));
    addAndMakeVisible (mixerGroiup.get());

    mixerGroiup->setBounds (464, 0, 128, 536);

    ch1Panel.reset (new MixerChannelPanel());
    addAndMakeVisible (ch1Panel.get());
    ch1Panel->setName ("channel1");

    ch1Panel->setBounds (472, 16, 112, 128);

    ch2Panel.reset (new MixerChannelPanel());
    addAndMakeVisible (ch2Panel.get());
    ch2Panel->setName ("channel2");

    ch2Panel->setBounds (472, 144, 112, 128);

    ch3Panel.reset (new MixerChannelPanel());
    addAndMakeVisible (ch3Panel.get());
    ch3Panel->setName ("channel3");

    ch3Panel->setBounds (472, 272, 112, 128);

    ch4Panel.reset (new MixerChannelPanel());
    addAndMakeVisible (ch4Panel.get());
    ch4Panel->setName ("channel4");

    ch4Panel->setBounds (472, 400, 112, 128);

    osc4Panel.reset (new OscillatorPanel());
    addAndMakeVisible (osc4Panel.get());
    osc4Panel->setName ("osc4Panel");

    osc4Panel->setBounds (240, 288, 216, 240);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1100, 768);


    //[Constructor] You can add your own custom stuff here..



    //[/Constructor]
}

MainUI::~MainUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    groupComponent = nullptr;
    groupComponent3 = nullptr;
    groupComponent6 = nullptr;
    fltCutoff = nullptr;
    fltResonance = nullptr;
    Cutoff = nullptr;
    Res = nullptr;
    osc1Panel = nullptr;
    osc3Panel = nullptr;
    osc2Panel = nullptr;
    ampEnvelope = nullptr;
    filterEnvelope = nullptr;
    mainVolume = nullptr;
    volumeLabel = nullptr;
    envAmt = nullptr;
    Amt = nullptr;
    mixerGroiup = nullptr;
    ch1Panel = nullptr;
    ch2Panel = nullptr;
    ch3Panel = nullptr;
    ch4Panel = nullptr;
    osc4Panel = nullptr;


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

    if (sliderThatWasMoved == fltCutoff.get())
    {
        //[UserSliderCode_fltCutoff] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic("filter.cutoff", sliderThatWasMoved->getValue());
        //[/UserSliderCode_fltCutoff]
    }
    else if (sliderThatWasMoved == fltResonance.get())
    {
        //[UserSliderCode_fltResonance] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic("filter.resonance", sliderThatWasMoved->getValue());
        //[/UserSliderCode_fltResonance]
    }
    else if (sliderThatWasMoved == mainVolume.get())
    {
        //[UserSliderCode_mainVolume] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic("mainVolume", sliderThatWasMoved->getValue());
        //[/UserSliderCode_mainVolume]
    }
    else if (sliderThatWasMoved == envAmt.get())
    {
        //[UserSliderCode_envAmt] -- add your slider handling code here..
		MessageBus::getInstance()->updateTopic("filter.envAmount", sliderThatWasMoved->getValue());
        //[/UserSliderCode_envAmt]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MainUI::topicChanged(Topic* topic) {
	Logger::getCurrentLogger()->writeToLog("topic " + topic->getName() + String(topic->getValue()));
}



//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainUI" componentName=""
                 parentClasses="public Component, public BusListener" constructorParams="LupoSynth* lupo"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="1100" initialHeight="768">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GROUPCOMPONENT name="new group" id="fddf59086c1c83a4" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="592 0 440 232" title="Amplifier"/>
  <GROUPCOMPONENT name="new group" id="be488b129ef124bc" memberName="groupComponent3"
                  virtualName="" explicitFocusOrder="0" pos="8 0 456 536" title="Oscilators"/>
  <GROUPCOMPONENT name="new group" id="d21f32d49ecf1836" memberName="groupComponent6"
                  virtualName="" explicitFocusOrder="0" pos="592 232 440 304" title="Filter"/>
  <SLIDER name="fltCutoff" id="820aba5f43f549e0" memberName="fltCutoff"
          virtualName="" explicitFocusOrder="0" pos="608 248 80 80" min="0.0"
          max="18000.0" int="10.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="fltResonance" id="e9128200d848646a" memberName="fltResonance"
          virtualName="" explicitFocusOrder="0" pos="696 248 80 80" min="0.0"
          max="5.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Cutoff" id="3a68e51a612b8d7" memberName="Cutoff" virtualName=""
         explicitFocusOrder="0" pos="624 336 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Res" id="25ed1f5df93f7ca0" memberName="Res" virtualName=""
         explicitFocusOrder="0" pos="728 336 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Q" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="osc1Panel" id="de6e8c64c8286c8d" memberName="osc1Panel"
                    virtualName="" explicitFocusOrder="0" pos="16 16 216 248" class="OscillatorPanel"
                    params=""/>
  <GENERICCOMPONENT name="osc3Panel" id="6bf55931fc2de34d" memberName="osc3Panel"
                    virtualName="" explicitFocusOrder="0" pos="16 288 216 240" class="OscillatorPanel"
                    params=""/>
  <GENERICCOMPONENT name="osc2Panel" id="ffc4e13aca23b88e" memberName="osc2Panel"
                    virtualName="" explicitFocusOrder="0" pos="240 16 216 248" class="OscillatorPanel"
                    params=""/>
  <GENERICCOMPONENT name="ampEnvelope" id="2e9f4701bbd4595c" memberName="ampEnvelope"
                    virtualName="" explicitFocusOrder="0" pos="608 16 408 144" class="EnvelopePanel"
                    params=""/>
  <GENERICCOMPONENT name="filterEnvelope" id="17487bad76ca25aa" memberName="filterEnvelope"
                    virtualName="" explicitFocusOrder="0" pos="608 368 408 144" class="EnvelopePanel"
                    params=""/>
  <SLIDER name="mainVolume" id="8750195bcc6f4ab5" memberName="mainVolume"
          virtualName="" explicitFocusOrder="0" pos="952 160 64 64" min="0.0"
          max="1.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="volumeLabel" id="594cc943c91cef9d" memberName="volumeLabel"
         virtualName="" explicitFocusOrder="0" pos="864 176 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Main volume" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="envAmt" id="ee160c38e2a36d41" memberName="envAmt" virtualName=""
          explicitFocusOrder="0" pos="792 248 80 80" min="0.0" max="1.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Amt" id="8a3f810d8c37fde8" memberName="Amt" virtualName=""
         explicitFocusOrder="0" pos="792 336 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Env. Amount" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="mixerGroiup" id="3a76ea384b35b47d" memberName="mixerGroiup"
                  virtualName="" explicitFocusOrder="0" pos="464 0 128 536" title="Mixer"/>
  <GENERICCOMPONENT name="channel1" id="a653fea415c89991" memberName="ch1Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 16 112 128" class="MixerChannelPanel"
                    params=""/>
  <GENERICCOMPONENT name="channel2" id="f3b06b13d2a0d971" memberName="ch2Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 144 112 128" class="MixerChannelPanel"
                    params=""/>
  <GENERICCOMPONENT name="channel3" id="6786cb4ee8a2b52d" memberName="ch3Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 272 112 128" class="MixerChannelPanel"
                    params=""/>
  <GENERICCOMPONENT name="channel4" id="fff0d24cb459d4de" memberName="ch4Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 400 112 128" class="MixerChannelPanel"
                    params=""/>
  <GENERICCOMPONENT name="osc4Panel" id="e90f31bb4430d4b4" memberName="osc4Panel"
                    virtualName="" explicitFocusOrder="0" pos="240 288 216 240" class="OscillatorPanel"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

