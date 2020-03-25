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
#include "LFOPanel.h"
#include "ChorusPanel.h"
#include "Model.h"
#include "AttachmentFactory.h"
#include "PresetDialog.h"
#include "AudioEngine/Oszillator.h"
//[/Headers]

#include "MainUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainUI::MainUI (LupoAudioProcessor* processor, AttachmentFactory* factory)
{
    //[Constructor_pre] You can add your own custom stuff here..
	this->processor = processor;
	this->model = processor->getModel();
	this->synth = processor->getSynth();
	Logger::getCurrentLogger()->writeToLog("GUI instance created.");
    //[/Constructor_pre]

    ModulationGroup.reset (new GroupComponent ("ModulationGroup",
                                               TRANS("Modulation")));
    addAndMakeVisible (ModulationGroup.get());

    ModulationGroup->setBounds (8, 584, 992, 144);

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS("Amplifier")));
    addAndMakeVisible (groupComponent.get());

    groupComponent->setBounds (592, 48, 408, 232);

    groupComponent3.reset (new GroupComponent ("new group",
                                               TRANS("Oscilators")));
    addAndMakeVisible (groupComponent3.get());

    groupComponent3->setBounds (8, 48, 456, 536);

    groupComponent6.reset (new GroupComponent ("new group",
                                               TRANS("Filter")));
    addAndMakeVisible (groupComponent6.get());

    groupComponent6->setBounds (592, 280, 408, 304);

    fltCutoff.reset (new Slider ("fltCutoff"));
    addAndMakeVisible (fltCutoff.get());
    fltCutoff->setRange (0, 18000, 10);
    fltCutoff->setSliderStyle (Slider::RotaryVerticalDrag);
    fltCutoff->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    fltCutoff->addListener (this);

    fltCutoff->setBounds (608, 312, 80, 80);

    fltResonance.reset (new Slider ("fltResonance"));
    addAndMakeVisible (fltResonance.get());
    fltResonance->setRange (0, 5, 0.01);
    fltResonance->setSliderStyle (Slider::RotaryVerticalDrag);
    fltResonance->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    fltResonance->addListener (this);

    fltResonance->setBounds (696, 312, 80, 80);

    Cutoff.reset (new Label ("Cutoff",
                             TRANS("Cutoff\n")));
    addAndMakeVisible (Cutoff.get());
    Cutoff->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Cutoff->setJustificationType (Justification::centredLeft);
    Cutoff->setEditable (false, false, false);
    Cutoff->setColour (TextEditor::textColourId, Colours::black);
    Cutoff->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Cutoff->setBounds (624, 400, 56, 24);

    Res.reset (new Label ("Res",
                          TRANS("Q")));
    addAndMakeVisible (Res.get());
    Res->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Res->setJustificationType (Justification::centredLeft);
    Res->setEditable (false, false, false);
    Res->setColour (TextEditor::textColourId, Colours::black);
    Res->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Res->setBounds (728, 400, 24, 24);

    osc1Panel.reset (new OscillatorPanel (model, factory));
    addAndMakeVisible (osc1Panel.get());
    osc1Panel->setName ("osc1Panel");

    osc1Panel->setBounds (16, 64, 216, 248);

    osc3Panel.reset (new OscillatorPanel (model, factory));
    addAndMakeVisible (osc3Panel.get());
    osc3Panel->setName ("osc3Panel");

    osc3Panel->setBounds (16, 336, 216, 240);

    osc2Panel.reset (new OscillatorPanel (model, factory));
    addAndMakeVisible (osc2Panel.get());
    osc2Panel->setName ("osc2Panel");

    osc2Panel->setBounds (240, 64, 216, 248);

    ampEnvelope.reset (new EnvelopePanel (model, factory));
    addAndMakeVisible (ampEnvelope.get());
    ampEnvelope->setName ("ampEnvelope");

    ampEnvelope->setBounds (608, 72, 376, 124);

    filterEnvelope.reset (new EnvelopePanel (model, factory));
    addAndMakeVisible (filterEnvelope.get());
    filterEnvelope->setName ("filterEnvelope");

    filterEnvelope->setBounds (608, 448, 376, 124);

    mainVolume.reset (new Slider ("mainVolume"));
    addAndMakeVisible (mainVolume.get());
    mainVolume->setRange (0, 2, 0.01);
    mainVolume->setSliderStyle (Slider::RotaryVerticalDrag);
    mainVolume->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    mainVolume->addListener (this);

    mainVolume->setBounds (920, 208, 64, 64);

    volumeLabel.reset (new Label ("volumeLabel",
                                  TRANS("Main volume")));
    addAndMakeVisible (volumeLabel.get());
    volumeLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    volumeLabel->setJustificationType (Justification::centredLeft);
    volumeLabel->setEditable (false, false, false);
    volumeLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    volumeLabel->setBounds (824, 224, 96, 24);

    envAmt.reset (new Slider ("envAmt"));
    addAndMakeVisible (envAmt.get());
    envAmt->setRange (0, 1, 0.01);
    envAmt->setSliderStyle (Slider::RotaryVerticalDrag);
    envAmt->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    envAmt->addListener (this);

    envAmt->setBounds (792, 312, 80, 80);

    Amt.reset (new Label ("Amt",
                          TRANS("Env. Amount")));
    addAndMakeVisible (Amt.get());
    Amt->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    Amt->setJustificationType (Justification::centredLeft);
    Amt->setEditable (false, false, false);
    Amt->setColour (TextEditor::textColourId, Colours::black);
    Amt->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    Amt->setBounds (792, 400, 88, 24);

    mixerGroiup.reset (new GroupComponent ("mixerGroiup",
                                           TRANS("Mixer")));
    addAndMakeVisible (mixerGroiup.get());

    mixerGroiup->setBounds (464, 48, 128, 536);

    ch1Panel.reset (new MixerChannelPanel (model, factory));
    addAndMakeVisible (ch1Panel.get());
    ch1Panel->setName ("channel1");

    ch1Panel->setBounds (472, 64, 112, 128);

    ch2Panel.reset (new MixerChannelPanel (model, factory));
    addAndMakeVisible (ch2Panel.get());
    ch2Panel->setName ("channel2");

    ch2Panel->setBounds (472, 192, 112, 128);

    ch3Panel.reset (new MixerChannelPanel (model, factory));
    addAndMakeVisible (ch3Panel.get());
    ch3Panel->setName ("channel3");

    ch3Panel->setBounds (472, 320, 112, 128);

    ch4Panel.reset (new MixerChannelPanel (model, factory));
    addAndMakeVisible (ch4Panel.get());
    ch4Panel->setName ("channel4");

    ch4Panel->setBounds (472, 448, 112, 128);

    osc4Panel.reset (new OscillatorPanel (model, factory));
    addAndMakeVisible (osc4Panel.get());
    osc4Panel->setName ("osc4Panel");

    osc4Panel->setBounds (240, 336, 216, 240);

    lfo1.reset (new LFOPanel (model, factory));
    addAndMakeVisible (lfo1.get());
    lfo1->setName ("lfo1");

    lfo1->setBounds (24, 608, 240, 96);

    lfo2.reset (new LFOPanel (model, factory));
    addAndMakeVisible (lfo2.get());
    lfo2->setName ("lfo2");

    lfo2->setBounds (280, 608, 240, 96);

    FXGroup.reset (new GroupComponent ("FXGroup",
                                       TRANS("FX")));
    addAndMakeVisible (FXGroup.get());

    FXGroup->setBounds (8, 728, 992, 136);

    reverbPanel.reset (new ReverbPanel (model, factory));
    addAndMakeVisible (reverbPanel.get());
    reverbPanel->setName ("reverbPanel");

    reverbPanel->setBounds (16, 744, 392, 112);

    auxEnvelope.reset (new EnvelopePanel (model, factory));
    addAndMakeVisible (auxEnvelope.get());
    auxEnvelope->setName ("auxEnvelope");

    auxEnvelope->setBounds (608, 600, 376, 124);

    delayPanel.reset (new DelayPanel (model, factory));
    addAndMakeVisible (delayPanel.get());
    delayPanel->setName ("delayPanel");

    delayPanel->setBounds (408, 744, 264, 112);

    chorusPanel.reset (new ChorusPanel (model, factory));
    addAndMakeVisible (chorusPanel.get());
    chorusPanel->setName ("new component");

    chorusPanel->setBounds (672, 744, 320, 112);

    presetButton.reset (new TextButton ("presetButton"));
    addAndMakeVisible (presetButton.get());
    presetButton->setButtonText (TRANS("Presets"));
    presetButton->addListener (this);
    presetButton->setColour (TextButton::buttonColourId, Colours::grey);

    presetButton->setBounds (16, 16, 64, 24);

    saveButton.reset (new TextButton ("saveButton"));
    addAndMakeVisible (saveButton.get());
    saveButton->setButtonText (TRANS("Save"));
    saveButton->addListener (this);
    saveButton->setColour (TextButton::buttonColourId, Colours::grey);

    saveButton->setBounds (320, 16, 64, 24);

    presetCombo.reset (new ComboBox ("presetCombo"));
    addAndMakeVisible (presetCombo.get());
    presetCombo->setEditableText (true);
    presetCombo->setJustificationType (Justification::centredLeft);
    presetCombo->setTextWhenNothingSelected (String());
    presetCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    presetCombo->addListener (this);

    presetCombo->setBounds (88, 16, 224, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1300, 900);


    //[Constructor] You can add your own custom stuff here..

	int x = getScreenX();
	int y = getScreenY();
	dlg = new PresetDialog(presetCombo.get(), model);
	dlg->setBounds(x, y, getWidth(), getHeight());
	addChildComponent(dlg);


	osc1Panel.get()->SetTitle("Osc 1");
	osc2Panel.get()->SetTitle("Osc 2");
	osc3Panel.get()->SetTitle("Osc 3");


	ch1Panel.get()->SetTitle("Ch 1");
	ch2Panel.get()->SetTitle("Ch 2");
	ch3Panel.get()->SetTitle("Ch 3");
	ch4Panel.get()->SetTitle("Ch 4");

	osc1Panel.get()->addChangeListener(this);
	osc2Panel.get()->addChangeListener(this);
	osc3Panel.get()->addChangeListener(this);
	osc4Panel.get()->addChangeListener(this);

	ch1Panel.get()->addChangeListener(this);
	ch2Panel.get()->addChangeListener(this);
	ch3Panel.get()->addChangeListener(this);
	ch4Panel.get()->addChangeListener(this);

	ampEnvelope.get()->addChangeListener(this);
	filterEnvelope.get()->addChangeListener(this);
	auxEnvelope.get()->addChangeListener(this);
	delayPanel.get()->addChangeListener(this);
	reverbPanel.get()->addChangeListener(this);
	chorusPanel.get()->addChangeListener(this);

	addChangeListener(synth);
	addMouseListener(this, true);

	factory->initState();

	factory->createSliderAttachment("cutoff", fltCutoff.get());
	factory->createSliderAttachment("resonance", fltResonance.get());
	factory->createSliderAttachment("mainVol", mainVolume.get());
	factory->createSliderAttachment("envAmt", envAmt.get());

	osc1Panel.get()->initAttachments();
	osc2Panel.get()->initAttachments();
	osc3Panel.get()->initAttachments();
	osc4Panel.get()->initAttachments();

	ch1Panel.get()->initAttachments();
	ch2Panel.get()->initAttachments();
	ch3Panel.get()->initAttachments();
	ch4Panel.get()->initAttachments();

	ampEnvelope.get()->initAttachments();
	filterEnvelope.get()->initAttachments();
	auxEnvelope.get()->initAttachments();

	lfo1.get()->initAttachments();
	lfo2.get()->initAttachments();

	reverbPanel.get()->initAttachments();
	chorusPanel.get()->initAttachments();
	delayPanel.get()->initAttachments();

	synth->addChangeListener(this);

    //[/Constructor]
}

MainUI::~MainUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	removeAllChangeListeners();
	delete dlg;
    //[/Destructor_pre]

    ModulationGroup = nullptr;
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
    lfo1 = nullptr;
    lfo2 = nullptr;
    FXGroup = nullptr;
    reverbPanel = nullptr;
    auxEnvelope = nullptr;
    delayPanel = nullptr;
    chorusPanel = nullptr;
    presetButton = nullptr;
    saveButton = nullptr;
    presetCombo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff606060));

    //[UserPaint] Add your own custom painting code here..

	g.setColour(Colours::white);
	if (draggingConnection) {
		g.drawLine(mouseDownPos.x, mouseDownPos.y, mousePos.x, mousePos.y, 2.0f);

	}



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
		model->cutoff = sliderThatWasMoved->getValue();
        //[/UserSliderCode_fltCutoff]
    }
    else if (sliderThatWasMoved == fltResonance.get())
    {
        //[UserSliderCode_fltResonance] -- add your slider handling code here..
		model->resonance = sliderThatWasMoved->getValue();

        //[/UserSliderCode_fltResonance]
    }
    else if (sliderThatWasMoved == mainVolume.get())
    {
        //[UserSliderCode_mainVolume] -- add your slider handling code here..
		model->mainVolume = sliderThatWasMoved->getValue();
        //[/UserSliderCode_mainVolume]
    }
    else if (sliderThatWasMoved == envAmt.get())
    {
        //[UserSliderCode_envAmt] -- add your slider handling code here..
		model->envAmt = sliderThatWasMoved->getValue();
        //[/UserSliderCode_envAmt]
    }

    //[UsersliderValueChanged_Post]
	sendChangeMessage();
    //[/UsersliderValueChanged_Post]
}

void MainUI::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == presetButton.get())
    {
        //[UserButtonCode_presetButton] -- add your button handler code here..
        //[/UserButtonCode_presetButton]
    }
    else if (buttonThatWasClicked == saveButton.get())
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
		
		processor->getValueTreeState()->getParameter("osc1Shape")->setValue(model->osc1Shape);
		processor->getValueTreeState()->getParameter("osc2Shape")->setValue(model->osc2Shape);
		processor->getValueTreeState()->getParameter("osc3Shape")->setValue(model->osc3Shape);
		processor->getValueTreeState()->getParameter("osc4Shape")->setValue(model->osc4Shape);
		
		ScopedPointer<XmlElement> xml(processor->getValueTreeState()->state.createXml());

		String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();

		String basePath = appDataPath + "/Audio/Presets/pueski/";
		String presetPath = basePath + "Lupo/";
		String presetName = presetCombo->getText();

		File baseDir = File(basePath);
		File presetDir = File(presetPath);

		if (!baseDir.exists()) {
			baseDir.createDirectory();
		}

		if (!presetDir.exists()) {
			presetDir.createDirectory();
		}

		File preset = File(presetPath + presetName + ".xml");

		bool proceed = true;
		bool presetExists = false;

		if (preset.exists()) {
			proceed = false;
			presetExists = true;
		}

		if (!proceed) {
			proceed = AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Warning", "A preset with this name exists already, overwrite?", "Ok", "Fuck! No!",this);
		}


		if (proceed) {

			xml.get()->writeToFile(preset, "");

			// DEBUG
			ValueTree state = ValueTree::fromXml(*xml.get());

			for (int i = 0; i < state.getNumChildren(); i++) {

				String id = state.getChild(i).getProperty("id").toString();
				String value = state.getChild(i).getProperty("value").toString();
				Logger::getCurrentLogger()->writeToLog("Storing : " + id + " with value " + value);
			}

			if (!presetExists) {

				int itemId = 0;

				for (int i = 0; i < presetCombo->getNumItems(); i++) {
					if (presetCombo->getItemId(i) > itemId) {
						itemId = presetCombo->getItemId(i);
					}
				}

				itemId++;

				this->presetCombo->addItem(presetName, itemId);
				// this->presetBox->setSelectedId(itemId);

			}

			xml = nullptr;
		}

        //[/UserButtonCode_saveButton]
    }

    //[UserbuttonClicked_Post]
	updatePresetList();
    //[/UserbuttonClicked_Post]
}

void MainUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == presetCombo.get())
    {
        //[UserComboBoxCode_presetCombo] -- add your combo box handling code here..
		processor->setSelectedProgram(comboBoxThatHasChanged->getText());
        //[/UserComboBoxCode_presetCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MainUI::changeListenerCallback(ChangeBroadcaster* source) {
	sendChangeMessage();
}

void MainUI::updatePresetList() {
	presetCombo->clear();
	for (int i = 0; i < processor->getNumPrograms(); i++) {
		presetCombo->addItem(processor->getProgramName(i), i + 1);
	}
	for (int i = 0; i < presetCombo.get()->getNumItems(); i++) {
		if (presetCombo.get()->getItemText(i) == processor->selectedProgram) {
			presetCombo->setSelectedItemIndex(i,juce::NotificationType::dontSendNotification);
			break;
		}
	}

}

void MainUI::mouseDown(const MouseEvent & event)
{
	// mouseDownPos = event.getPosition();
}

void MainUI::mouseUp(const MouseEvent & event)
{
	// draggingConnection = false;
}

void MainUI::mouseDrag(const MouseEvent & event)
{
	// draggingConnection = true;
	// repaint();
}

void MainUI::mouseMove(const MouseEvent & event)
{
	// mousePos = event.getPosition();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainUI" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster, public ChangeListener"
                 constructorParams="LupoAudioProcessor* processor" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1300" initialHeight="900">
  <BACKGROUND backgroundColour="ff606060"/>
  <GROUPCOMPONENT name="ModulationGroup" id="bbbc621090753013" memberName="ModulationGroup"
                  virtualName="" explicitFocusOrder="0" pos="8 584 992 144" title="Modulation"/>
  <GROUPCOMPONENT name="new group" id="fddf59086c1c83a4" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="592 48 408 232" title="Amplifier"/>
  <GROUPCOMPONENT name="new group" id="be488b129ef124bc" memberName="groupComponent3"
                  virtualName="" explicitFocusOrder="0" pos="8 48 456 536" title="Oscilators"/>
  <GROUPCOMPONENT name="new group" id="d21f32d49ecf1836" memberName="groupComponent6"
                  virtualName="" explicitFocusOrder="0" pos="592 280 408 304" title="Filter"/>
  <SLIDER name="fltCutoff" id="820aba5f43f549e0" memberName="fltCutoff"
          virtualName="" explicitFocusOrder="0" pos="608 312 80 80" min="0.0"
          max="18000.0" int="10.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="fltResonance" id="e9128200d848646a" memberName="fltResonance"
          virtualName="" explicitFocusOrder="0" pos="696 312 80 80" min="0.0"
          max="5.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Cutoff" id="3a68e51a612b8d7" memberName="Cutoff" virtualName=""
         explicitFocusOrder="0" pos="624 400 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cutoff&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Res" id="25ed1f5df93f7ca0" memberName="Res" virtualName=""
         explicitFocusOrder="0" pos="728 400 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Q" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="osc1Panel" id="de6e8c64c8286c8d" memberName="osc1Panel"
                    virtualName="" explicitFocusOrder="0" pos="16 64 216 248" class="OscillatorPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="osc3Panel" id="6bf55931fc2de34d" memberName="osc3Panel"
                    virtualName="" explicitFocusOrder="0" pos="16 336 216 240" class="OscillatorPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="osc2Panel" id="ffc4e13aca23b88e" memberName="osc2Panel"
                    virtualName="" explicitFocusOrder="0" pos="240 64 216 248" class="OscillatorPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="ampEnvelope" id="2e9f4701bbd4595c" memberName="ampEnvelope"
                    virtualName="" explicitFocusOrder="0" pos="608 72 376 124" class="EnvelopePanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="filterEnvelope" id="17487bad76ca25aa" memberName="filterEnvelope"
                    virtualName="" explicitFocusOrder="0" pos="608 448 376 124" class="EnvelopePanel"
                    params="model, factory"/>
  <SLIDER name="mainVolume" id="8750195bcc6f4ab5" memberName="mainVolume"
          virtualName="" explicitFocusOrder="0" pos="920 208 64 64" min="0.0"
          max="2.0" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="volumeLabel" id="594cc943c91cef9d" memberName="volumeLabel"
         virtualName="" explicitFocusOrder="0" pos="824 224 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Main volume" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="envAmt" id="ee160c38e2a36d41" memberName="envAmt" virtualName=""
          explicitFocusOrder="0" pos="792 312 80 80" min="0.0" max="1.0"
          int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Amt" id="8a3f810d8c37fde8" memberName="Amt" virtualName=""
         explicitFocusOrder="0" pos="792 400 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Env. Amount" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="mixerGroiup" id="3a76ea384b35b47d" memberName="mixerGroiup"
                  virtualName="" explicitFocusOrder="0" pos="464 48 128 536" title="Mixer"/>
  <GENERICCOMPONENT name="channel1" id="a653fea415c89991" memberName="ch1Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 64 112 128" class="MixerChannelPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="channel2" id="f3b06b13d2a0d971" memberName="ch2Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 192 112 128" class="MixerChannelPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="channel3" id="6786cb4ee8a2b52d" memberName="ch3Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 320 112 128" class="MixerChannelPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="channel4" id="fff0d24cb459d4de" memberName="ch4Panel" virtualName=""
                    explicitFocusOrder="0" pos="472 448 112 128" class="MixerChannelPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="osc4Panel" id="e90f31bb4430d4b4" memberName="osc4Panel"
                    virtualName="" explicitFocusOrder="0" pos="240 336 216 240" class="OscillatorPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="lfo1" id="5309e7d420abc98c" memberName="lfo1" virtualName=""
                    explicitFocusOrder="0" pos="24 608 240 96" class="LFOPanel" params="model, factory"/>
  <GENERICCOMPONENT name="lfo2" id="6e5eaff17ef4b84d" memberName="lfo2" virtualName=""
                    explicitFocusOrder="0" pos="280 608 240 96" class="LFOPanel"
                    params="model, factory"/>
  <GROUPCOMPONENT name="FXGroup" id="85cf2fc9be4f7bcf" memberName="FXGroup" virtualName=""
                  explicitFocusOrder="0" pos="8 728 992 136" title="FX"/>
  <GENERICCOMPONENT name="reverbPanel" id="c574c98e1dd9ac8a" memberName="reverbPanel"
                    virtualName="" explicitFocusOrder="0" pos="16 744 392 112" class="ReverbPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="auxEnvelope" id="b597ff273ac7fb29" memberName="auxEnvelope"
                    virtualName="" explicitFocusOrder="0" pos="608 600 376 124" class="EnvelopePanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="delayPanel" id="f7f617a6d8249b19" memberName="delayPanel"
                    virtualName="" explicitFocusOrder="0" pos="408 744 264 112" class="DelayPanel"
                    params="model, factory"/>
  <GENERICCOMPONENT name="new component" id="958d847dca958b2b" memberName="chorusPanel"
                    virtualName="" explicitFocusOrder="0" pos="672 744 320 112" class="ChorusPanel"
                    params="model, factory"/>
  <TEXTBUTTON name="presetButton" id="eace43cbe17dcd89" memberName="presetButton"
              virtualName="" explicitFocusOrder="0" pos="16 16 64 24" bgColOff="ff808080"
              buttonText="Presets" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="saveButton" id="18cedefaea6c1f80" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="320 16 64 24" bgColOff="ff808080"
              buttonText="Save" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="presetCombo" id="d63a422738bbc6a6" memberName="presetCombo"
            virtualName="" explicitFocusOrder="0" pos="88 16 224 24" editable="1"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

