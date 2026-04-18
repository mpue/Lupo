/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioEngine/MultimodeOscillator.h"
#include "AttachmentFactory.h"

//==============================================================================
LupoAudioProcessor::LupoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();


	model.reset(new Model());
	matrix = std::make_unique<ModMatrix>();

	lupo.reset(new LupoSynth(model.get(),matrix.get()));
	parameters = new AudioProcessorValueTreeState(*this, nullptr);

	factory = std::make_unique<AttachmentFactory>(this, lupo.get());

	factory->createParam("cutoff1", "Cutoff", 20.0f, 20000.0f, 12000.0f, 0.25f);
	factory->createParam("resonance1", "Resonance", 0.01f, 5.0f, 1.0);
	factory->createParam("envAmt1", "Filter amount", 0.01f, 1.0f, 1.0);
	factory->createParam("filterMode1", "Filter Mode", 0.0f, 1.0f, 0.0);

	factory->createParam("cutoff2", "Cutoff", 20.0f, 20000.0f, 12000.0f, 0.25f);
	factory->createParam("resonance2", "Resonance", 0.01f, 5.0f, 1.0);
	factory->createParam("envAmt2", "Filter amount", 0.01f, 1.0f, 1.0);
	factory->createParam("filterMode2", "Filter Mode", 0.0f, 1.0f, 0.0);

	factory->createParam("mainVolume", "Main Volume", 0.01f, 2.0f, 1.0);

	factory->createParam("ampAttack", "Attack", 0.0f, 10.0, 0.0);
	factory->createParam("ampDecay", "Decay", 0.0f, 10.0f, 1.0);
	factory->createParam("ampSustain", "Sustain", 0.0f, 1.0f, 0.0);
	factory->createParam("ampRelease", "Release", 0.0f, 10.0f, 1.0);

	factory->createParam("auxAttack1", "ENV 1 Attack", 0.0f, 10.0, 0.0);
	factory->createParam("auxDecay1", "ENV 1 Decay", 0.0f, 10.0f, 1.0);
	factory->createParam("auxSustain1", "ENV 1 Sustain", 0.0f, 1.0f, 0.0);
	factory->createParam("auxRelease1", "ENV 1Release", 0.0f, 10.0f, 1.0);

	factory->createParam("auxAttack2", "ENV 2 Attack", 0.0f, 10.0, 0.0);
	factory->createParam("auxDecay2", "ENV 2 Decay", 0.0f, 10.0f, 1.0);
	factory->createParam("auxSustain2", "ENV 2 Sustain", 0.0f, 1.0f, 0.0);
	factory->createParam("auxRelease2", "ENV 2 Release", 0.0f, 10.0f, 1.0);
	factory->createParam("auxAmt2", "ENV 2 Amount", 0.0f, 100.0f, 1.0);

	factory->createParam("dlyTimeLeft", "DelayL", 0.0f, 1000.0, 0.0);
	factory->createParam("dlyTimeRight", "DelayR", 0.0f, 1000.0f, 1.0);
	factory->createParam("dlyFeedback", "Feedback", 0.0f, 1.0f, 0.0);
	factory->createParam("dlyMix", "Mix", 0.0f, 1.0f, 0.0);

	factory->createParam("rvbRoomSize", "RoomSize", 0.0f, 1.0, 0.0);
	factory->createParam("rvbDamping", "Damping", 0.0f, 1.0f, 1.0);
	factory->createParam("rvbWetLevel", "WetLevel", 0.0f, 1.0f, 0.0);
	factory->createParam("rvbDryLevel", "DryLevel", 0.0f, 1.0f, 0.0);
	factory->createParam("rvbWidth", "Width", 0.0f, 1.0f, 0.0);
	factory->createParam("rvbFreezeMode", "Freeze", 0.0f, 1.0f, 0.0);
	factory->createParam("rvbEnabled", "Reverb enabled", 0, 1.0, 0);

	factory->createParam("chrDelay", "Delay", 0.0f, 1.0, 0.0);
	factory->createParam("chrModulation", "MOdulation", 0.0f, 1.0f, 1.0);
	factory->createParam("chrFeedback", "Feedback", 0.0f, 1.0f, 0.0);
	factory->createParam("chrMix", "Mix", 0.0f, 1.0f, 0.0);

	factory->createParam("osc1Pitch", "Osc1 pitch", -36, 36, 0);
	factory->createParam("osc1Fine", "Osc1 fine", -1.0, 1.0, 0);
	factory->createParam("osc1Volume", "Osc1 volume", 0, 1.0, 1);
	factory->createParam("osc1Pan", "Osc1 pan", -1.0, 1.0, 0);
	factory->createParam("osc1Shape", "Osc1 shape", 0, 3.0, 0);
	factory->createParam("osc1Spread", "Osc1 spread", 0, 1.0, 0);
	factory->createParam("osc1Width",  "Osc1 width",  0, 1.0, 0);
	factory->createParam("osc1Enabled", "Osc1 enabled", 0, 1.0, 0);
	factory->createParam("osc1Sync", "Osc1 sync", 0, 1.0, 1);

	factory->createParam("osc2Pitch", "Osc2 pitch", -36, 36, 0);
	factory->createParam("osc2Fine", "Osc2 fine", -1.0, 1.0, 0);
	factory->createParam("osc2Volume", "Osc2 volume", 0, 1.0, 1);
	factory->createParam("osc2Pan", "Osc2 pan", -1.0, 1.0, 0);
	factory->createParam("osc2Shape", "Osc2 shape", 0, 3.0, 0);
	factory->createParam("osc2Spread", "Osc2 spread", 0, 1.0, 0);
	factory->createParam("osc2Width",  "Osc2 width",  0, 1.0, 0);
	factory->createParam("osc2Enabled", "Osc2 enabled", 0, 1.0, 0);

	factory->createParam("osc3Pitch", "Osc3 pitch", -36, 36, 0);
	factory->createParam("osc3Fine", "Osc3 fine", -1.0, 1.0, 0);
	factory->createParam("osc3Volume", "Osc3 volume", 0, 1.0, 1);
	factory->createParam("osc3Pan", "Osc3 pan", -1.0, 1.0, 0);
	factory->createParam("osc3Shape", "Osc3 shape", 0, 3.0, 0);
	factory->createParam("osc3Spread", "Osc3 spread", 0, 1.0, 0);
	factory->createParam("osc3Width",  "Osc3 width",  0, 1.0, 0);
	factory->createParam("osc3Enabled", "Osc3 enabled", 0, 1.0, 0);

	factory->createParam("osc4Pitch", "Osc4 pitch", -36, 36, 0);
	factory->createParam("osc4Fine", "Osc4 fine", -1.0, 1.0, 0);
	factory->createParam("osc4Volume", "Osc4 volume", 0, 1.0, 1);
	factory->createParam("osc4Pan", "Osc4 pan", -1.0, 1.0, 0);
	factory->createParam("osc4Shape", "Osc4 shape", 0, 3.0, 0);
	factory->createParam("osc4Spread", "Osc4 spread", 0, 1.0, 0);
	factory->createParam("osc4Width",  "Osc4 width",  0, 1.0, 0);
	factory->createParam("osc4Enabled", "Osc4 enabled", 0, 1.0, 0);

	factory->createParam("lfo1Shape", "Lfo1 shape", 0, 4.0, 0);
	factory->createParam("lfo2Shape", "Lfo2 shape", 0, 4.0, 0);
	factory->createParam("lfo3Shape", "Lfo3 shape", 0, 4.0, 0);

	factory->createParam("lfo1Speed", "Lfo1 speed", 0, 10.0, 0);
	factory->createParam("lfo2Speed", "Lfo2 speed", 0, 10.0, 0);
	factory->createParam("lfo3Speed", "Lfo3 speed", 0, 10.0, 0);

	factory->createParam("lfo1Amount", "Lfo1 amount", 0, 10.0, 0);
	factory->createParam("lfo2Amount", "Lfo2 amount", 0, 10.0, 0);
	factory->createParam("lfo3Amount", "Lfo3 amount", 0, 10.0, 0);

	factory->createParam("fmAmount", "FM amount", 0, 1.0, 0);

	factory->createParam("distDrive", "Drive", 0, 5.0, 0);
	factory->createParam("distMix", "Distortion Mix", 0, 1.0, 0);
	factory->createParam("distMode", "Distortion MOde", 0, 3.0, 0);

	factory->createParam("portamentoTime",   "Portamento Duration",  0.0f, 2.0f, 0.5f);
	factory->createParam("portamentoAmount", "Portamento Intensity", 0.0f, 1.0f, 0.0f);

	factory->createParam("arpEnabled", "Arp enabled", 0, 1.0, 0);
	factory->createParam("arpSpeed", "Arp speed", 0, 3.0, 2);  // Changed from 0, 1.0, 0 to 0, 3.0, 2 to match speedSlider range
	factory->createParam("arpTempo", "Arp tempo", 60.0f, 200.0f, 120.0f);
	factory->createParam("arpClockMode", "Arp clock mode", 0, 1.0, 0);  // 0=Internal, 1=MIDI
	factory->createParam("arpOctaves", "Arp Octaves", 0, 3.0, 0);  // Changed from 0, 3.0, 0 to match octaveCombo (1-4 items, 0-3 index)
	factory->createParam("arpMode", "Arp mode", 0, 2.0, 0);  // 0=Up, 1=Down, 2=Random
	
	factory->createParam("filterMode", "Filter mode", 0, 1.0, 0);
	factory->createParam("cutoffLink", "Cutoff link", 0, 1.0, 0);

	factory->createParam("filterChar1", "Filter 1 character", 0.0f, 4.0f, 0.0f);
	factory->createParam("filterChar2", "Filter 2 character", 0.0f, 4.0f, 0.0f);

	// Step Sequencer (disabled by default)
	factory->createParam("seqEnabled",   "Seq enabled",   0.0f, 1.0f,   0.0f);
	factory->createParam("seqTempo",     "Seq tempo",    20.0f, 300.0f, 120.0f);
	factory->createParam("seqSteps",     "Seq steps",     1.0f, 32.0f,  16.0f);
	factory->createParam("seqDivision",  "Seq division",  0.0f,  3.0f,   2.0f);  // default 1/16
	factory->createParam("seqDirection", "Seq direction", 0.0f,  3.0f,   0.0f);
	factory->createParam("seqSwing",     "Seq swing",     0.0f,  0.75f,  0.0f);

	// 8-Band Parametric EQ (bypassed by default: eqEnabled=0)
	factory->createParam("eqEnabled", "EQ Active", 0.0f, 1.0f, 0.0f);
	{
		float eqDefaultFreqs[8] = {60.0f, 200.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 14000.0f};
		float eqDefaultQ[8]     = {0.7f,  1.0f,   1.0f,   1.0f,    1.0f,    1.0f,    1.0f,    0.7f};
		for (int i = 1; i <= 8; ++i) {
			String b = String(i);
			factory->createParam("eqGain" + b, "EQ Gain " + b, -24.0f, 24.0f,           0.0f);
			factory->createParam("eqFreq" + b, "EQ Freq " + b,  20.0f, 20000.0f, eqDefaultFreqs[i-1], 0.25f);
			factory->createParam("eqQ"    + b, "EQ Q "    + b,   0.1f,  10.0f,   eqDefaultQ[i-1], 0.5f);
		}
	}

	for (int i = 0; i < 6; i++) {
		factory->createParam("Source_" + String(i), "Matrix source " + String(i), 0.0f, 4.0f, 0.0);
		factory->createParam("Target_" + String(i), "Matrix target " + String(i), 0.0f, 5.0f, 0.0);
		factory->createParam("Amount_" + String(i), "Matrix amount " + String(i), 0.0f, 20.0f, 0.0);
	}

	bypass = parameters->createAndAddParameter("bypass", "bypass", "Bypass", NormalisableRange<float>(0, 1),0, nullptr, nullptr);

	// Initialize the ValueTree state AFTER all parameters have been created.
	// The deprecated createAndAddParameter API does not create the state tree
	// automatically, so we must do it here. Without this, the state is invalid
	// and any call to state.createXml() (e.g. preset saving) will return nullptr.
	parameters->state = ValueTree(Identifier("LupoState"));

	midiLearnManager = std::make_unique<MidiLearnManager>();
	midiLearnManager->loadFromFile(getMidiLearnFile());

	Logger::getCurrentLogger()->writeToLog("Building preset list");

	refreshPresetList();

	Logger::getCurrentLogger()->writeToLog("Found "+String(programNames.size())+" presets");
	
}

LupoAudioProcessor::~LupoAudioProcessor()
{
	if (JUCEApplication::isStandaloneApp()) {				
		factory = nullptr;
		lupo.reset();
		model.reset();
		matrix = nullptr;
	}
// delete messageBus;
	Logger::getCurrentLogger()->writeToLog("Lupo is dead.");
}

//==============================================================================
const String LupoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LupoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LupoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LupoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LupoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LupoAudioProcessor::getNumPrograms()
{
	return std::max(1,(int)programNames.size());
}

int LupoAudioProcessor::getCurrentProgram()
{
	return std::max(0, (int)currentProgramNumber);
}

void LupoAudioProcessor::setCurrentProgram (int index)
{
	Logger::getCurrentLogger()->writeToLog("Setting program index "+String(index));

	if (index < 0 || index >= (int)programNames.size()) {
		return;
	}

	if (programNames.size() > 0) {
		String name = programNames.at(index);
		this->currentProgramNumber = index;
		this->setSelectedProgram(name);
	}

}

void LupoAudioProcessor::setSelectedProgram(juce::String name) {

	Logger::getCurrentLogger()->writeToLog("Loading preset "+name);

	if (!prepared) {
		return;
	}

	if (name.trim().isEmpty()) {
		return;
	}

	String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
	String presetPath = appDataPath + "/Audio/Presets/pueski/Lupo/";

	String filename = name + ".xml";
	String matrixConf = name + ".matrix";
	File preset = File(presetPath + filename);
	File matrixFile = File(presetPath + matrixConf);

	if (!preset.exists()) {
		Logger::getCurrentLogger()->writeToLog("Preset file not found: " + preset.getFullPathName());
		return;
	}

	std::unique_ptr<XmlElement> xml = XmlDocument(preset).getDocumentElement();

	if (xml == nullptr) {
		Logger::getCurrentLogger()->writeToLog("ERROR: Failed to parse preset XML: " + preset.getFullPathName());
		return;
	}

	ValueTree state = ValueTree::fromXml(*xml);
	xml = nullptr;

	if (!state.isValid()) {
		Logger::getCurrentLogger()->writeToLog("ERROR: Invalid ValueTree from preset: " + name);
		return;
	}

	this->selectedProgram = name;

	// Get the mod matrix state from the preset
	String modMatrixState = "5,7;0,0,0,0,0,0,0;0,0,0,0,0,0,0;0,0,0,0,0,0,0;0,0,0,0,0,0,0;0,0,0,0,0,0,0";

	if (matrixFile.exists()) {
		 modMatrixState = matrixFile.loadFileAsString();
	}

	// Stop the sequencer before loading the new preset so it doesn't keep
	// running with stale pattern/timing during the state transition.
	lupo->getSeq()->setEnabled(false);

	// Load step sequencer pattern
	File seqFile = File(presetPath + name + ".seq");
	if (seqFile.exists())
		lupo->getSeq()->loadStateFromString(seqFile.loadFileAsString());

	// Load EQ automation
	File eqAutoFile = File(presetPath + name + ".eqauto");
	if (eqAutoFile.exists())
		lupo->getEqAuto()->loadStateFromString(eqAutoFile.loadFileAsString());

	// Use replaceState() for thread-safe state replacement.
	// This triggers valueTreeRedirected -> updateParameterConnectionsToChildTrees
	// which reconnects all parameter adapters and fires parameterChanged callbacks.
	lupo->running = false;
	getValueTreeState()->replaceState(state);
	lupo->updateState(state, modMatrixState);
	lupo->running = true;

	Logger::getCurrentLogger()->writeToLog("Updating synth state");
}

const String LupoAudioProcessor::getProgramName (int index)
{
	if (programNames.size() < 1) {
		return "init";
	}

	if (getNumPrograms() > 0 && index < getNumPrograms() && index >= 0) {
		return programNames.at(index);
	}

	return "init";
}

void LupoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LupoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	Logger::getCurrentLogger()->writeToLog("Preparing playback");
	lupo->prepareToPlay(sampleRate, samplesPerBlock);

	if (!prepared)
	{
		prepared = true;

		if (pendingPresetName.isNotEmpty())
		{
			// A preset was stored before prepareToPlay – load it now.
			setSelectedProgram(pendingPresetName);
			pendingPresetName = {};
		}
		else
		{
			// No preset to load: push all current APVTS default values into the synth
			// so it is fully initialized even without an editor or a saved project.
			StringArray ids = factory->getSliderParams();
			for (int i = 0; i < ids.size(); ++i)
			{
				const String& pid = ids.getReference(i);
				auto range = parameters->getParameterRange(pid);
				if (auto* param = parameters->getParameter(pid))
					lupo->parameterChanged(pid, range.convertFrom0to1(param->getValue()));
			}
		}
	}
}

void LupoAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LupoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LupoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	// Buffer MIDI CC messages for MIDI Learn (applied to parameters on the message thread)
	for (const auto& meta : midiMessages)
	{
		auto msg = meta.getMessage();
		if (msg.isController())
			midiLearnManager->processMidiCC(msg.getControllerNumber(), msg.getControllerValue());
	}

	lupo->setPlayHead(getPlayHead());
	lupo->processBlock(buffer,midiMessages);
}

AudioProcessorParameter * LupoAudioProcessor::getBypassParameter() const
{
	return bypass;
}

//==============================================================================
bool LupoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LupoAudioProcessor::createEditor()
{
    return new LupoAudioProcessorEditor (*this);
}

//==============================================================================
void LupoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	String name = selectedProgram.isNotEmpty() ? selectedProgram : getProgramName(getCurrentProgram());
	destData.append(name.getCharPointer(), name.length());
}

void LupoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	if (data == nullptr || sizeInBytes <= 0)
		return;

	String name = juce::String(static_cast<const char*>(data), (size_t)sizeInBytes).trim();

	if (name.isEmpty())
		return;

	if (prepared)
		setSelectedProgram(name);
	else
		pendingPresetName = name; // will be loaded in prepareToPlay
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LupoAudioProcessor();
}

LupoSynth* LupoAudioProcessor::getSynth() {
	return lupo.get();
}

Model* LupoAudioProcessor::getModel() {
	return model.get();
}

AudioProcessorValueTreeState* LupoAudioProcessor::getValueTreeState() {
	return this->parameters;
}

File LupoAudioProcessor::getMidiLearnFile()
{
	String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
	return File(appDataPath + "/Audio/Presets/pueski/Lupo/midilearn.cfg");
}

void LupoAudioProcessor::refreshPresetList() {
	programNames.clear();

	String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
	String presetPath = appDataPath + "/Audio/Presets/pueski/Lupo/";

	File presets = File(presetPath);

	if (presets.exists() && presets.isDirectory()) {
		ScopedPointer<DirectoryIterator> iter = new DirectoryIterator(presets, false);
		while (iter->next()) {
			if (iter->getFile().exists() && !iter->getFile().isDirectory() && iter->getFile().getFileExtension() == ".xml") {
				String name = iter->getFile().getFileNameWithoutExtension();
				Logger::getCurrentLogger()->writeToLog("Found preset : " + name);
				programNames.push_back(name);
			}
		}
		iter = nullptr;
	}
}

