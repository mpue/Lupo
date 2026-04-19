#include "AudioEngine/LupoSynth.h"
#include "OscillatorPanel.h"
#include "LFOPanel.h"
#include "ChorusPanel.h"
#include "Model.h"
#include "AttachmentFactory.h"
#include "PresetDialog.h"
#include "AudioEngine/Oszillator.h"
#include "Panel.h"
#include <math.h>
#include "MainUI.h"


MainUI::MainUI(LupoAudioProcessor* processor, AttachmentFactory* factory)
{

	this->processor = processor;
	this->model = processor->getModel();
	this->synth = processor->getSynth();
	this->factory = factory;
	
	Arpeggiator* arp = this->synth->getArpeggiator();
	Logger::getCurrentLogger()->writeToLog("GUI instance created.");

	for (int i = 0; i < factory->getSliderParams().size(); i++) {
		processor->parameters->addParameterListener(factory->getSliderParams().getReference(i), this);
		Logger::getCurrentLogger()->writeToLog("Listening to parameter: " + factory->getSliderParams().getReference(i));
	}
	for (int i = 0; i < 6; i++) {
		processor->parameters->addParameterListener("Source_" + String(i), this);
		processor->parameters->addParameterListener("Target_" + String(i), this);
		processor->parameters->addParameterListener("Amount_" + String(i), this);
	}
	processor->parameters->addParameterListener("arpEnabled", this);
	processor->parameters->addParameterListener("arpMode", this);
	processor->parameters->addParameterListener("arpLatch", this);
	processor->parameters->addParameterListener("seqEnabled", this);

	groupComponent.reset(new GroupComponent("new group",TRANS("Amplifier")));
	addAndMakeVisible(groupComponent.get());
	groupComponent->setBounds(612, 48, 392, 210);

	groupComponent3.reset(new GroupComponent("new group",TRANS("Oscilators")));
	addAndMakeVisible(groupComponent3.get());
	groupComponent3->setBounds(8, 48, 456, 496);

	filterGroup1.reset(new GroupComponent("filterGroup1",TRANS("Filter 1")));
	addAndMakeVisible(filterGroup1.get());

	filterGroup1->setBounds(612, 260, 392, 128);

	osc1Panel.reset(new OscillatorPanel(model, factory));
	addAndMakeVisible(osc1Panel.get());
	osc1Panel->setName("osc1Panel");

	osc1Panel->setBounds(16, 64, 216, 232);

	osc3Panel.reset(new OscillatorPanel(model, factory));
	addAndMakeVisible(osc3Panel.get());
	osc3Panel->setName("osc3Panel");

	osc3Panel->setBounds(16, 304, 216, 232);

	osc2Panel.reset(new OscillatorPanel(model, factory));
	addAndMakeVisible(osc2Panel.get());
	osc2Panel->setName("osc2Panel");

	osc2Panel->setBounds(240, 64, 216, 232);

	ampEnvelope.reset(new EnvelopePanel(model, factory));
	addAndMakeVisible(ampEnvelope.get());
	ampEnvelope->setName("ampEnvelope");
	ampEnvelope->setBounds(628, 64, 288, 80);
	
	filterEnvelopeGroup.reset(new GroupComponent("filterEnvelopeGroup", "Filter Env."));
	addAndMakeVisible(filterEnvelopeGroup.get());
	filterEnvelopeGroup->setBounds(612, 552, 392, 216);

	filterEnvelope.reset(new EnvelopePanel(model, factory));
	filterEnvelope->setName("auxEnvelope1");
	filterEnvelope->setDecayTime(3.0f);
	addAndMakeVisible(filterEnvelope.get());
	filterEnvelope->setBounds(628, 562, 288, 150);
	
	mainVolume.reset(new Slider("mainVolume"));
	addAndMakeVisible(mainVolume.get());
	mainVolume->setRange(0, 2, 0.01);
	mainVolume->setSliderStyle(Slider::RotaryVerticalDrag);
	mainVolume->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	mainVolume->addListener(this);

	mainVolume->setBounds(924, 72, 56, 56);

	volumeLabel.reset(new Label("volumeLabel",TRANS("Main volume")));
	addAndMakeVisible(volumeLabel.get());
	volumeLabel->setFont(Font(12.00f, Font::plain).withTypefaceStyle("Regular"));
	volumeLabel->setJustificationType(Justification::centredLeft);
	volumeLabel->setEditable(false, false, false);
	volumeLabel->setColour(TextEditor::textColourId, Colours::black);
	volumeLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	volumeLabel->setBounds(904, 120, 64, 24);

	mixerGriup.reset(new GroupComponent("mixerGroiup",TRANS("Mixer")));
	addAndMakeVisible(mixerGriup.get());

	mixerGriup->setBounds(474, 48, 128, 496);

	ch1Panel.reset(new MixerChannelPanel(model, factory));
	addAndMakeVisible(ch1Panel.get());
	ch1Panel->setName("channel1");

	ch1Panel->setBounds(482, 64, 112, 96);

	ch2Panel.reset(new MixerChannelPanel(model, factory));
	addAndMakeVisible(ch2Panel.get());
	ch2Panel->setName("channel2");

	ch2Panel->setBounds(482, 160, 112, 96);

	ch3Panel.reset(new MixerChannelPanel(model, factory));
	addAndMakeVisible(ch3Panel.get());
	ch3Panel->setName("channel3");

	ch3Panel->setBounds(482, 256, 112, 96);

	ch4Panel.reset(new MixerChannelPanel(model, factory));
	addAndMakeVisible(ch4Panel.get());
	ch4Panel->setName("channel4");

	ch4Panel->setBounds(482, 352, 112, 96);

	osc4Panel.reset(new OscillatorPanel(model, factory));
	addAndMakeVisible(osc4Panel.get());
	osc4Panel->setName("osc4Panel");

	osc4Panel->setBounds(240, 304, 216, 232);

	// Create the main tabbed component to consolidate all modulation and FX components
	rightSideTabs.reset(new TabbedComponent(TabbedButtonBar::TabsAtTop));
	addAndMakeVisible(rightSideTabs.get());
	rightSideTabs->setTabBarDepth(30);

	// Create FX container panel
	fxContainerPanel.reset(new Component());

	// Create and add FX components to container
	reverbPanel.reset(new ReverbPanel(model, factory));
	reverbPanel->setName("reverbPanel");
	fxContainerPanel->addAndMakeVisible(reverbPanel.get());
	reverbPanel->setBounds(8, 8, 460, 112);

	delayPanel.reset(new DelayPanel(model, factory));
	delayPanel->setName("delayPanel");
	fxContainerPanel->addAndMakeVisible(delayPanel.get());
	delayPanel->setBounds(8, 128, 460, 112);

	chorusPanel.reset(new ChorusPanel(model, factory));
	chorusPanel->setName("chorusPanel");
	fxContainerPanel->addAndMakeVisible(chorusPanel.get());
	chorusPanel->setBounds(8, 248, 284, 112);

	distortionPanel.reset(new DistortionPanel(model, factory));
	distortionPanel->setName("distortionPanel");
	fxContainerPanel->addAndMakeVisible(distortionPanel.get());
	distortionPanel->setBounds(296, 248, 172, 112);

	// Create Arpeggiator panel
	arpPanel.reset(new ArpPanel(factory, arp));
	arpPanel->setName("arpPanel");

	// Create modulation matrix viewport and panel
	modMatrixViewport.reset(new Viewport("ModMatrix"));
	modMatrix.reset(new ModPanel(this->synth->getModMatrix(), model, factory));
	modMatrixViewport->setViewedComponent(modMatrix.get());
	modMatrixViewport->setScrollBarsShown(true, false);
	modMatrix->setName("modMatrix");
	modMatrix->setBounds(0, 0, 460, 440);
	
	// Create LFO/Modulation container
	modulationTab.reset(new TabbedComponent(TabbedButtonBar::TabsAtBottom));
	modulationTab->setTabBarDepth(22);
	modulationTab->addTab(TRANS("LFO 1"), Colour(0x00000000), new LFOPanel(model, factory), true);
	modulationTab->addTab(TRANS("LFO 2"), Colour(0x00000000), new LFOPanel(model, factory), true);
	modulationTab->addTab(TRANS("LFO 3"), Colour(0x00000000), new LFOPanel(model, factory), true);
	modulationTab->addTab(TRANS("AUX ENV"), Colour(0x00000000), new EnvelopePanel(model, factory), true);
	modulationTab->setCurrentTabIndex(0);

	// EQ panel
	eqPanel.reset(new EQPanel(model, factory, synth->getEQ(), synth->getSeq(), synth->getEqAuto()));
	eqPanel->setName("eqPanel");

	// Step Sequencer panel
	seqPanel.reset(new StepSequencerPanel(synth->getSeq(), synth->getChordManager()));
	seqPanel->setName("seqPanel");

	// Add all tabs to the main tabbed component
	rightSideTabs->addTab(TRANS("FX"),         Colour(0xff1a1a1a), fxContainerPanel.get(), false);
	rightSideTabs->addTab(TRANS("EQ"),         Colour(0xff1a1a1a), eqPanel.get(),          false);
	rightSideTabs->addTab(TRANS("Sequencer"),  Colour(0xff1a1a1a), seqPanel.get(),         false);
	rightSideTabs->addTab(TRANS("Arpeggiator"),Colour(0xff1a1a1a), arpPanel.get(),         false);
	rightSideTabs->addTab(TRANS("Mod Matrix"), Colour(0xff1a1a1a), modMatrixViewport.get(),false);
	rightSideTabs->addTab(TRANS("Modulation"), Colour(0xff1a1a1a), modulationTab.get(),    false);
	rightSideTabs->setCurrentTabIndex(0);
	rightSideTabs->setBounds(1020, 48, 480, 520);
	presetButton.reset(new TextButton("presetButton"));
	addAndMakeVisible(presetButton.get());
	presetButton->setButtonText(TRANS("Presets"));
	presetButton->addListener(this);
	presetButton->setBounds(16, 16, 64, 24);

	saveButton.reset(new TextButton("saveButton"));
	addAndMakeVisible(saveButton.get());
	saveButton->setButtonText(TRANS("Save"));
	saveButton->addListener(this);
	saveButton->setBounds(336, 16, 64, 24);

	presetCombo.reset(new ComboBox("presetCombo"));
	addAndMakeVisible(presetCombo.get());
	presetCombo->setEditableText(true);
	presetCombo->setJustificationType(Justification::centredLeft);
	presetCombo->setTextWhenNothingSelected(String());
	presetCombo->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
	presetCombo->addListener(this);

	presetCombo->setBounds(88, 16, 192, 24);

	prevPresetButton.reset(new TextButton("prevPresetButton"));
	addAndMakeVisible(prevPresetButton.get());
	prevPresetButton->setButtonText("<");
	prevPresetButton->addListener(this);
	prevPresetButton->setBounds(284, 16, 24, 24);

	nextPresetButton.reset(new TextButton("nextPresetButton"));
	addAndMakeVisible(nextPresetButton.get());
	nextPresetButton->setButtonText(">");
	nextPresetButton->addListener(this);
	nextPresetButton->setBounds(312, 16, 24, 24);

	fmSlider.reset(new Slider("fmSlider"));
	addAndMakeVisible(fmSlider.get());
	fmSlider->setRange(0, 1, 0);
	fmSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	fmSlider->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	fmSlider->addListener(this);

	fmSlider->setBounds(496, 464, 64, 56);

	distGroup.reset(new GroupComponent("distGroup",TRANS("Distortion")));
	fxContainerPanel->addAndMakeVisible(distGroup.get());
	distGroup->setVisible(true);
	distGroup->setBounds(1030, 456, 392, 88);

	filterPanel1.reset(new FilterPanel(model, factory));
	addAndMakeVisible(filterPanel1.get());
	filterPanel1->setName("filterPanel1");

	filterPanel1->setBounds(628, 268, 360, 116);

	filterGroup2.reset(new GroupComponent("filterGroup2",TRANS("Filter 2")));
	addAndMakeVisible(filterGroup2.get());

	filterGroup2->setBounds(612, 416, 392, 128);

	filterPanel2.reset(new FilterPanel(model, factory));
	addAndMakeVisible(filterPanel2.get());
	filterPanel2->setName("filterPanel2");

	filterPanel2->setBounds(628, 424, 360, 112);

	filterModeLabel.reset(new Label("filterModeLabel",TRANS("Filter mode")));
	addAndMakeVisible(filterModeLabel.get());
	filterModeLabel->setFont(Font(12.00f, Font::plain).withTypefaceStyle("Regular"));
	filterModeLabel->setJustificationType(Justification::centredLeft);
	filterModeLabel->setEditable(false, false, false);
	filterModeLabel->setColour(TextEditor::textColourId, Colours::black);
	filterModeLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	filterModeLabel->setBounds(824, 390, 64, 24);
	
	filterModeCombo.reset(new ComboBox("new combo box"));
	addAndMakeVisible(filterModeCombo.get());
	filterModeCombo->setEditableText(false);
	filterModeCombo->setJustificationType(Justification::centredLeft);
	filterModeCombo->setTextWhenNothingSelected(String());
	filterModeCombo->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
	filterModeCombo->addItem(TRANS("Serial"), 1);
	filterModeCombo->addItem(TRANS("Parallel"), 2);
	filterModeCombo->addListener(this);

	filterModeCombo->setBounds(896, 390, 88, 24);

	cutoffLink.reset(new ToggleButton("cutoffLink"));
	addAndMakeVisible(cutoffLink.get());
	cutoffLink->setButtonText(String());
	cutoffLink->addListener(this);

	cutoffLink->setBounds(688, 390, 32, 24);

	label.reset(new Label("new label",
		TRANS("Cutoff link")));
	addAndMakeVisible(label.get());
	label->setFont(Font(12.00f, Font::plain).withTypefaceStyle("Regular"));
	label->setJustificationType(Justification::centredLeft);
	label->setEditable(false, false, false);
	label->setColour(TextEditor::textColourId, Colours::black);
	label->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	label->setBounds(720, 390, 64, 24);

	mainDisplay.reset(new TextEditor("mainDisplay"));
	addAndMakeVisible(mainDisplay.get());
	mainDisplay->setMultiLine(false);
	mainDisplay->setReturnKeyStartsNewLine(false);
	mainDisplay->setReadOnly(true);
	mainDisplay->setScrollbarsShown(false);
	mainDisplay->setCaretVisible(false);
	mainDisplay->setPopupMenuEnabled(false);
	mainDisplay->setColour(TextEditor::textColourId, Colour(0xff4d9eff));
	mainDisplay->setColour(TextEditor::backgroundColourId, Colour(0xff1a1a1a));
	mainDisplay->setText(String());

	mainDisplay->setBounds(20, 550, 560, 200);

	int x = getScreenX();
	int y = getScreenY();
	dlg = std::make_unique<PresetDialog>(presetCombo.get(), model);
	dlg->setBounds(x, y, getWidth(), getHeight());
	addChildComponent(dlg.get());

	presetBrowser.reset(new PresetBrowser(processor));
	addChildComponent(presetBrowser.get());
	presetBrowser->onPresetSelected = [this, processor](const String& name)
	{
		processor->setSelectedProgram(name);
		updatePresetList();
	};
	presetBrowser->onClose = [this]()
	{
		presetBrowser->setVisible(false);
	};

	osc1Panel.get()->SetTitle("Osc 1");
	osc2Panel.get()->SetTitle("Osc 2");
	osc3Panel.get()->SetTitle("Osc 3");
	osc4Panel.get()->SetTitle("Osc 4");

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

	LFOPanel* lfo1 = dynamic_cast<LFOPanel*> (modulationTab.get()->getTabContentComponent(0));
	LFOPanel* lfo2 = dynamic_cast<LFOPanel*> (modulationTab.get()->getTabContentComponent(1));
	LFOPanel* lfo3 = dynamic_cast<LFOPanel*> (modulationTab.get()->getTabContentComponent(2));
	EnvelopePanel* auxEnv2 = dynamic_cast<EnvelopePanel*> (modulationTab.get()->getTabContentComponent(3));

	delayPanel.get()->addChangeListener(this);
	reverbPanel.get()->addChangeListener(this);
	chorusPanel.get()->addChangeListener(this);

	addChangeListener(synth);
	addMouseListener(this, true);

	// NOTE: Do NOT call factory->initState() here!
	// The ValueTree state is already initialized when parameters are created 
	// in LupoAudioProcessor's constructor. Calling initState() every time 
	// the editor is recreated (e.g. Ableton's "Configure" button) would 
	// replace the entire state tree with an empty one, destroying all 
	// parameter values and causing a crash.

	factory->createSliderAttachment("fmAmount", fmSlider.get());
	factory->createSliderAttachment("mainVolume", mainVolume.get());
	factory->createComboAttachment("filterMode", filterModeCombo.get());

	factory->createButtonAttachment("cutoffLink", cutoffLink.get());

	filterPanel1->initAttachments();
	filterPanel2->initAttachments();

	osc1Panel.get()->initAttachments();
	osc2Panel.get()->initAttachments();
	osc3Panel.get()->initAttachments();
	osc4Panel.get()->initAttachments();

	ch1Panel.get()->initAttachments();
	ch2Panel.get()->initAttachments();
	ch3Panel.get()->initAttachments();
	ch4Panel.get()->initAttachments();

	ampEnvelope.get()->initAttachments();
	filterEnvelope->initAttachments();
	
	if (lfo1 != nullptr) { lfo1->setName("lfo1"); lfo1->initAttachments(); }
	if (lfo2 != nullptr) { lfo2->setName("lfo2"); lfo2->initAttachments(); }
	if (lfo3 != nullptr) { lfo3->setName("lfo3"); lfo3->initAttachments(); }

	if (auxEnv2 != nullptr) {
		auxEnv2->setName("auxEnvelope2");
		auxEnv2->initAttachments();
	}

	reverbPanel.get()->initAttachments();
	chorusPanel.get()->initAttachments();
	delayPanel.get()->initAttachments();
	distortionPanel.get()->initAttachments();
	eqPanel.get()->initAttachments();
	arpPanel.get()->initAttachments();
	modMatrix.get()->initAttachments();

	synth->addChangeListener(this);

	presetCombo->setText("init");

	mainDisplay.get()->setFont(Font(Typeface::createSystemTypefaceFor(BinaryData::DSEG14ClassicRegular_ttf,
		BinaryData::DSEG14ClassicRegular_ttfSize)));

	mainDisplay.get()->setText("Lupo V1.0");

	leftGainSlider.reset(new Slider("leftGainSlider"));
	addAndMakeVisible(leftGainSlider.get());
	leftGainSlider->setRange(0, 1, 0.01);
	leftGainSlider->setSliderStyle(Slider::LinearHorizontal);
	leftGainSlider->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	leftGainSlider->setBounds(1050, 600, 400, 10);

	rightGainSlider.reset(new Slider("rightGainSlider"));
	addAndMakeVisible(rightGainSlider.get());
	rightGainSlider->setRange(0, 1, 0.01);
	rightGainSlider->setSliderStyle(Slider::LinearHorizontal);
	rightGainSlider->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
	rightGainSlider->setBounds(1050, 615, 400, 10);

	// Chord buttons — below VU meter, same total width (400px), split equally
	chordButton.reset(new TextButton("chordButton"));
	addAndMakeVisible(chordButton.get());
	chordButton->setButtonText("Chord");
	chordButton->setClickingTogglesState(true);
	chordButton->setColour(TextButton::buttonOnColourId,  Colour(0xff2a52c8));
	chordButton->setColour(TextButton::textColourOffId,  Colour(0xff7a99c0));
	chordButton->setColour(TextButton::textColourOnId,   Colour(0xffe8f0ff));
	chordButton->addListener(this);
	chordButton->setBounds(1050, 632, 126, 24);

	createChordButton.reset(new TextButton("createChordButton"));
	addAndMakeVisible(createChordButton.get());
	createChordButton->setButtonText("Create");
	createChordButton->setColour(TextButton::textColourOffId, Colour(0xff55aa66));
	createChordButton->addListener(this);
	createChordButton->setBounds(1184, 632, 126, 24);

	autoChordButton.reset(new TextButton("autoChordButton"));
	addAndMakeVisible(autoChordButton.get());
	autoChordButton->setButtonText("Auto");
	autoChordButton->setClickingTogglesState(true);
	autoChordButton->setColour(TextButton::buttonOnColourId,  Colour(0xff0a8888));
	autoChordButton->setColour(TextButton::textColourOffId,  Colour(0xff50a0a0));
	autoChordButton->setColour(TextButton::textColourOnId,   Colour(0xffe8f0ff));
	autoChordButton->addListener(this);
	autoChordButton->setBounds(1318, 632, 132, 24);

	// Key selector
	chordKeyCombo.reset(new ComboBox("chordKeyCombo"));
	addAndMakeVisible(chordKeyCombo.get());
	for (const char* n : { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" })
		chordKeyCombo->addItem(n, chordKeyCombo->getNumItems() + 1);
	chordKeyCombo->setSelectedId(1, dontSendNotification);
	chordKeyCombo->addListener(this);
	chordKeyCombo->setBounds(1050, 660, 192, 20);

	// Scale selector
	chordScaleCombo.reset(new ComboBox("chordScaleCombo"));
	addAndMakeVisible(chordScaleCombo.get());
	for (const char* n : { "Major","Minor","Dorian","Phrygian","Lydian","Mixolydian" })
		chordScaleCombo->addItem(n, chordScaleCombo->getNumItems() + 1);
	chordScaleCombo->setSelectedId(1, dontSendNotification);
	chordScaleCombo->addListener(this);
	chordScaleCombo->setBounds(1250, 660, 200, 20);

	transposeDownButton.reset(new TextButton("transposeDown"));
	addAndMakeVisible(transposeDownButton.get());
	transposeDownButton->setButtonText("-");
	transposeDownButton->addListener(this);
	transposeDownButton->setBounds(1050, 686, 24, 20);

	transposeLED.reset(new TransposeLEDDisplay());
	addAndMakeVisible(transposeLED.get());
	transposeLED->setBounds(1078, 686, 110, 20);

	transposeUpButton.reset(new TextButton("transposeUp"));
	addAndMakeVisible(transposeUpButton.get());
	transposeUpButton->setButtonText("+");
	transposeUpButton->addListener(this);
	transposeUpButton->setBounds(1192, 686, 24, 20);

	resized();

	startTimer(30);

}

MainUI::~MainUI()
{
	stopTimer();

	// Remove parameter listeners to prevent callbacks on a destroyed MainUI
	if (factory != nullptr) {
		auto params = factory->getSliderParams();
		for (int i = 0; i < params.size(); i++) {
			processor->parameters->removeParameterListener(params.getReference(i), this);
		}
		for (int i = 0; i < 6; i++) {
			processor->parameters->removeParameterListener("Source_" + String(i), this);
			processor->parameters->removeParameterListener("Target_" + String(i), this);
			processor->parameters->removeParameterListener("Amount_" + String(i), this);
		}
		processor->parameters->removeParameterListener("arpEnabled", this);
		processor->parameters->removeParameterListener("arpMode", this);
		processor->parameters->removeParameterListener("arpLatch", this);
		processor->parameters->removeParameterListener("seqEnabled", this);
	}

	processor->getFactory()->clearAttachments();
	removeAllChangeListeners();

	dlg = nullptr;
	presetBrowser = nullptr;
	ModulationGroup = nullptr;
	groupComponent = nullptr;
	groupComponent3 = nullptr;
	filterGroup1 = nullptr;
	osc1Panel = nullptr;
	osc3Panel = nullptr;
	osc2Panel = nullptr;
	osc4Panel = nullptr;
	ampEnvelope = nullptr;
	filterEnvelope = nullptr;
	mainVolume = nullptr;
	volumeLabel = nullptr;
	mixerGriup = nullptr;
	ch1Panel = nullptr;
	ch2Panel = nullptr;
	ch3Panel = nullptr;
	ch4Panel = nullptr;
	osc4Panel = nullptr;
	FXGroup = nullptr;
	reverbPanel = nullptr;
	delayPanel = nullptr;
	chorusPanel = nullptr;
	presetButton = nullptr;
	prevPresetButton = nullptr;
	nextPresetButton = nullptr;
	saveButton = nullptr;
	presetCombo = nullptr;
	fmSlider = nullptr;
	distGroup = nullptr;
	distortionPanel = nullptr;
	arpGroup = nullptr;
	arpPanel = nullptr;
	modulationTab = nullptr;
	filterPanel1 = nullptr;
	filterGroup2 = nullptr;
	filterPanel2 = nullptr;
	filterModeLabel = nullptr;
	filterModeCombo = nullptr;
	cutoffLink = nullptr;
	label = nullptr;
	mainDisplay = nullptr;
	modMatrix = nullptr;
	eqPanel  = nullptr;
	seqPanel = nullptr;
	leftGainSlider = nullptr;
	rightGainSlider = nullptr;
	chordButton       = nullptr;
	createChordButton = nullptr;
	autoChordButton   = nullptr;
	chordKeyCombo     = nullptr;
	chordScaleCombo   = nullptr;
	transposeLED         = nullptr;
	transposeDownButton  = nullptr;
	transposeUpButton    = nullptr;
}

void MainUI::paint(Graphics& g)
{
	g.fillAll(Colour(0xff333333));
	g.drawImageAt(juce::ImageCache::getFromMemory(BinaryData::lupo_ui_png, BinaryData::lupo_ui_pngSize), 0, 0);
}

void MainUI::resized()
{
}

void MainUI::sliderValueChanged(Slider* sliderThatWasMoved)
{
	if (sliderThatWasMoved == mainVolume.get())
	{
		model->mainVolume = sliderThatWasMoved->getValue();
	}
	else if (sliderThatWasMoved == fmSlider.get())
	{
		// TODO : remove or cobble together with fmAmount attachment
	}

	sendChangeMessage();

}

void MainUI::buttonClicked(Button* buttonThatWasClicked)
{

	auto* chord = synth->getChordManager();

	if (buttonThatWasClicked == chordButton.get())
	{
		bool on = chordButton->getToggleState();
		if (chord) { chord->setEnabled(on); chord->setAutoChord(false); }
		// mutually exclusive with Auto
		if (on && autoChordButton) autoChordButton->setToggleState(false, dontSendNotification);
	}
	else if (buttonThatWasClicked == autoChordButton.get())
	{
		bool on = autoChordButton->getToggleState();
		if (chord) { chord->setAutoChord(on); chord->setEnabled(on); }
		// mutually exclusive with manual Chord
		if (on && chordButton) chordButton->setToggleState(false, dontSendNotification);
	}
	else if (buttonThatWasClicked == createChordButton.get())
	{
		if (chord)
		{
			chord->startCapture();
			// Give visual feedback: button text while capturing
			createChordButton->setButtonText("...");
			// Poll until capture finishes, then restore label
			startTimer(30);
		}
	}
	else if (buttonThatWasClicked == transposeDownButton.get())
	{
		int v = synth->getGlobalTranspose() - 1;
		synth->setGlobalTranspose(v);
		transposeLED->setValue(synth->getGlobalTranspose());
	}
	else if (buttonThatWasClicked == transposeUpButton.get())
	{
		int v = synth->getGlobalTranspose() + 1;
		synth->setGlobalTranspose(v);
		transposeLED->setValue(synth->getGlobalTranspose());
	}
	else if (buttonThatWasClicked == prevPresetButton.get())
	{
		int idx = presetCombo->getSelectedItemIndex();
		if (idx > 0)
			presetCombo->setSelectedItemIndex(idx - 1, sendNotification);
	}
	else if (buttonThatWasClicked == nextPresetButton.get())
	{
		int idx = presetCombo->getSelectedItemIndex();
		if (idx < presetCombo->getNumItems() - 1)
			presetCombo->setSelectedItemIndex(idx + 1, sendNotification);
	}
	else if (buttonThatWasClicked == presetButton.get())
	{
		presetBrowser->setBounds(getLocalBounds());
		presetBrowser->setVisible(true);
		presetBrowser->toFront(true);
	}
	else if (buttonThatWasClicked == saveButton.get())
	{
		String presetName = presetCombo->getText().trim();

		if (presetName.isEmpty()) {
			return;
		}

		processor->getValueTreeState()->getParameter("osc1Shape")->setValue(model->osc1Shape);
		processor->getValueTreeState()->getParameter("osc2Shape")->setValue(model->osc2Shape);
		processor->getValueTreeState()->getParameter("osc3Shape")->setValue(model->osc3Shape);
		processor->getValueTreeState()->getParameter("osc4Shape")->setValue(model->osc4Shape);

		// Sync seq enabled state (enableButton writes directly to the sequencer,
		// not to the APVTS, so we sync it manually here before serialising).
		processor->getValueTreeState()->getParameter("seqEnabled")->setValue(
			synth->getSeq()->isRunning() ? 1.0f : 0.0f);

		auto xml = processor->getValueTreeState()->state.createXml();

		if (xml == nullptr) {
			Logger::getCurrentLogger()->writeToLog("ERROR: Failed to create XML from state tree - state may not be initialized");
			return;
		}

		String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();

		String basePath = appDataPath + "/Audio/Presets/pueski/";
		String presetPath = basePath + "Lupo/";

		File baseDir = File(basePath);
		File presetDir = File(presetPath);

		if (!baseDir.exists()) {
			baseDir.createDirectory();
		}

		if (!presetDir.exists()) {
			presetDir.createDirectory();
		}

		File preset = File(presetPath + presetName + ".xml");

		// we read and write the mod matrix state to a separate file, 
		// because it is not part of the AudioProcessorValueTreeState
		// and we want to be able to edit it without touching the rest of the preset

		File modMatrixConfig = File(presetPath + presetName + ".matrix");
		
		bool proceed = true;

		if (proceed) {

			bool success = false;

			if (modMatrixConfig.exists()) {
				success = modMatrixConfig.replaceWithText(modMatrix->getGridStateAsString());
			}
			else {
				success = modMatrixConfig.create();
				modMatrixConfig.appendText(modMatrix->getGridStateAsString());
			}

			if (!success) {
				return;
			}

			xml->writeToFile(preset, "");

			// Save step sequencer pattern to a separate .seq file
			File seqFile = File(presetPath + presetName + ".seq");
			String seqState = synth->getSeq()->getStateAsString();
			if (seqFile.exists())
				seqFile.replaceWithText(seqState);
			else {
				seqFile.create();
				seqFile.appendText(seqState);
			}

			// Save EQ automation to a separate .eqauto file
			File eqAutoFile = File(presetPath + presetName + ".eqauto");
			String eqAutoState = synth->getEqAuto()->getStateAsString();
			if (eqAutoFile.exists())
				eqAutoFile.replaceWithText(eqAutoState);
			else {
				eqAutoFile.create();
				eqAutoFile.appendText(eqAutoState);
			}

			// Save chord manager state to a separate .chord file
				File chordFile = File(presetPath + presetName + ".chord");
				String chordStateStr = synth->getChordManager()->getStateAsString();
				if (chordFile.exists())
					chordFile.replaceWithText(chordStateStr);
				else {
					chordFile.create();
					chordFile.appendText(chordStateStr);
				}

				// DEBUG
				ValueTree state = ValueTree::fromXml(*xml);

			for (int i = 0; i < state.getNumChildren(); i++) {

				String id = state.getChild(i).getProperty("id").toString();
				String value = state.getChild(i).getProperty("value").toString();
				Logger::getCurrentLogger()->writeToLog("Storing : " + id + " with value " + value);
			}
		}

		// Refresh the processor's internal preset list from disk so the
		// newly saved preset is included, then update the ComboBox.
		processor->selectedProgram = presetName;
		processor->refreshPresetList();
		updatePresetList();
	}
	else if (buttonThatWasClicked == cutoffLink.get())
	{
		// TODO : remove or cobble together with attachment
	}

}

void MainUI::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{

	if (comboBoxThatHasChanged == presetCombo.get())
	{
		String presetName = comboBoxThatHasChanged->getText().trim();
		if (presetName.isNotEmpty()) {
			processor->setSelectedProgram(presetName);
			if (eqPanel != nullptr)
				eqPanel->syncPlayButtonState();
		}
	}
	else if (comboBoxThatHasChanged == filterModeCombo.get())
	{
	}
	else if (comboBoxThatHasChanged == chordKeyCombo.get())
	{
		auto* chord = synth->getChordManager();
		if (chord) chord->setKey(chordKeyCombo->getSelectedId() - 1);  // selectedId is 1-based
	}
	else if (comboBoxThatHasChanged == chordScaleCombo.get())
	{
		auto* chord = synth->getChordManager();
		if (chord) chord->setScale((ChordManager::Scale)(chordScaleCombo->getSelectedId() - 1));
	}

}

void MainUI::timerCallback()
{
	leftGainSlider->setValue(processor->getSynth()->getLeftPeak(), juce::NotificationType::dontSendNotification);
	rightGainSlider->setValue(processor->getSynth()->getRighPeak(), juce::NotificationType::dontSendNotification);

	// Update "Create" button label while chord capture is active
	if (createChordButton != nullptr)
	{
		auto* chord = synth->getChordManager();
		if (chord && chord->isCapturing())
			createChordButton->setButtonText("...");
		else
			createChordButton->setButtonText("Create");
	}

	// Per-instance mod matrix state update (replaces global FastBus)
	if (model->modMatrixStateChanged.exchange(false))
	{
		modMatrix->setGridStateFromString(model->pendingModMatrixState);
		repaint();
	}

	// Chord manager UI sync after preset load
	if (model->chordStateChanged.exchange(false))
	{
		auto* chord = synth->getChordManager();
		if (chord && chordButton && autoChordButton && chordKeyCombo && chordScaleCombo)
		{
			bool isAuto   = chord->isAutoChord();
			bool isManual = chord->isEnabled() && !isAuto;
			chordButton->setToggleState(isManual, dontSendNotification);
			autoChordButton->setToggleState(isAuto, dontSendNotification);
			chordKeyCombo->setSelectedId(chord->getKey() + 1, dontSendNotification);
			chordScaleCombo->setSelectedId((int)chord->getScale() + 1, dontSendNotification);
		}
	}

	// Apply buffered MIDI CC values to parameters (must run on message thread)
	auto* mlm = processor->getMidiLearnManager();
	mlm->applyPendingUpdates(*processor->getValueTreeState());

	// MIDI Learn feedback
	bool isLearning = mlm->isCurrentlyLearning();
	if (isLearning)
	{
		mainDisplay->setText("MIDI Learn: " + mlm->getCurrentLearningParam() + " - wiggle a knob");
		midiLearnWasActive = true;
	}
	else if (midiLearnWasActive)
	{
		midiLearnWasActive = false;
		mlm->saveToFile(LupoAudioProcessor::getMidiLearnFile());
		mainDisplay->setText("MIDI CC assigned!");
		midiLearnFeedbackCountdown = 66; // ~2 s at 30 ms tick
	}
	else if (midiLearnFeedbackCountdown > 0)
	{
		--midiLearnFeedbackCountdown;
	}
}

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
			presetCombo->setSelectedItemIndex(i, juce::NotificationType::dontSendNotification);
			break;
		}
	}

}

Component* MainUI::findComponentAtMousePosition(Point<int> mousePos, Component* parent)
{
	for (int i = 0; i < parent->getNumChildComponents(); i++) {
		Component* child = parent->getChildComponent(i);
		if (child->getBounds().contains(mousePos)) {
			return child;
		}
		else {
			return findComponentAtMousePosition(mousePos, child);
		}
	}

	return nullptr;
}

void MainUI::parameterChanged(const String& parameterID, float newValue)
{
	// Sync arp enabled button when preset is loaded
	if (parameterID == "arpEnabled")
	{
		juce::Component::SafePointer<MainUI> safeThis(this);
		juce::MessageManager::callAsync([safeThis, newValue]()
		{
			if (safeThis == nullptr) return;
			safeThis->arpPanel->syncArpEnabled(newValue > 0.5f);
		});
		return;
	}

	// Sync arp mode combo when preset is loaded
	if (parameterID == "arpMode")
	{
		juce::Component::SafePointer<MainUI> safeThis(this);
		juce::MessageManager::callAsync([safeThis, newValue]()
		{
			if (safeThis == nullptr) return;
			safeThis->arpPanel->syncModeCombo(juce::roundToInt(newValue));
		});
		return;
	}

	// Sync arp latch button when preset is loaded
	if (parameterID == "arpLatch")
	{
		juce::Component::SafePointer<MainUI> safeThis(this);
		juce::MessageManager::callAsync([safeThis, newValue]()
		{
			if (safeThis == nullptr) return;
			safeThis->arpPanel->syncLatch(newValue > 0.5f);
		});
		return;
	}

	// Sync sequencer enabled button when preset is loaded
	if (parameterID == "seqEnabled")
	{
		juce::Component::SafePointer<MainUI> safeThis(this);
		juce::MessageManager::callAsync([safeThis, newValue]()
		{
			if (safeThis == nullptr) return;
			safeThis->seqPanel->syncEnabled(newValue > 0.5f);
		});
		return;
	}

	// Sync cutoff sliders when link is active.
	// Guard: only update the other side if its value differs to avoid ping-pong.
	if (cutoffLink->getToggleState())
	{
		AudioProcessorValueTreeState* apvts = processor->getValueTreeState();
		auto syncCutoff = [apvts](const String& targetID, float value)
		{
			auto* param = apvts->getParameter(targetID);
			if (param == nullptr) return;
			auto range = apvts->getParameterRange(targetID);
			float currentValue = range.convertFrom0to1(param->getValue());
			if (std::abs(currentValue - value) > 0.001f)
				param->setValueNotifyingHost(range.convertTo0to1(value));
		};

		if (parameterID == "cutoff1")
			syncCutoff("cutoff2", newValue);
		else if (parameterID == "cutoff2")
			syncCutoff("cutoff1", newValue);
	}

	for (int i = 0; i < processor->getNumParameters(); i++) {
		if (processor->getParameterID(i) == parameterID) {
			juce::Component::SafePointer<MainUI> safeThis(this);
			juce::MessageManager::callAsync([safeThis, newValue, i]()
			{
				if (safeThis == nullptr)
					return;
				auto* self = safeThis.getComponent();
				// Don't overwrite MIDI Learn feedback while it's still being shown
				if (self->midiLearnWasActive || self->midiLearnFeedbackCountdown > 0)
					return;
				self->mainDisplay->setText(self->processor->getParameters().getUnchecked(i)->getLabel() + " - " + String(newValue, 2));
			});

			break;
		}
	}
}

void MainUI::mouseDown(const MouseEvent& event)
{
	if (event.mods.isRightButtonDown())
	{
		Component* comp = event.eventComponent;
		if (dynamic_cast<Slider*>(comp) != nullptr)
		{
			String paramID = factory->getParamForComponent(comp);
			if (paramID.isNotEmpty())
			{
				showMidiLearnMenu(paramID);
				return;
			}
		}
	}
}

void MainUI::showMidiLearnMenu(const String& paramID)
{
	auto* mlm = processor->getMidiLearnManager();
	int existingCC = mlm->getCCForParam(paramID);

	PopupMenu menu;
	menu.addItem(1, "MIDI Learn");
	if (existingCC >= 0)
		menu.addItem(2, "Clear CC " + String(existingCC));
	else
		menu.addItem(2, "Clear CC", false, false);

	juce::Component::SafePointer<MainUI> safeThis(this);
	menu.showMenuAsync(PopupMenu::Options(), [safeThis, paramID](int result)
	{
		if (safeThis == nullptr)
			return;
		auto* self = safeThis.getComponent();
		auto* m = self->processor->getMidiLearnManager();

		if (result == 1)
		{
			m->startLearning(paramID);
			self->midiLearnWasActive = false;
		}
		else if (result == 2)
		{
			m->clearParam(paramID);
			m->saveToFile(LupoAudioProcessor::getMidiLearnFile());
			self->mainDisplay->setText("Cleared MIDI CC for " + paramID);
		}
	});
}

void MainUI::mouseUp(const MouseEvent& event)
{
}

void MainUI::mouseDrag(const MouseEvent& event)
{
}

void MainUI::mouseMove(const MouseEvent& event)
{
}

