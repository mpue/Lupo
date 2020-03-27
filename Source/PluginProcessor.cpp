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
	lupo.reset(new LupoSynth(model.get()));
	parameters = new AudioProcessorValueTreeState(*this, nullptr);

	factory = new AttachmentFactory(this, lupo.get());

	factory->createParam("cutoff", "Cutoff", 0.01f, 15000.0, 12000.0);
	factory->createParam("resonance", "Resonance", 0.01f, 5.0f, 1.0);
	factory->createParam("mainVolume", "Main Volume", 0.01f, 2.0f, 1.0);
	factory->createParam("envAmt", "Filter amount", 0.01f, 1.0f, 1.0);

	factory->createParam("ampAttack", "Attack", 0.0f, 10.0, 0.0);
	factory->createParam("ampDecay", "Decay", 0.0f, 10.0f, 1.0);
	factory->createParam("ampSustain", "Sustain", 0.0f, 1.0f, 0.0);
	factory->createParam("ampRelease", "Release", 0.0f, 10.0f, 1.0);

	factory->createParam("filAttack", "Attack", 0.0f, 10.0, 0.0);
	factory->createParam("filDecay", "Decay", 0.0f, 10.0f, 1.0);
	factory->createParam("filSustain", "Sustain", 0.0f, 1.0f, 0.0);
	factory->createParam("filRelease", "Release", 0.0f, 10.0f, 1.0);

	factory->createParam("auxAttack", "Attack", 0.0f, 10.0, 0.0);
	factory->createParam("auxDecay", "Decay", 0.0f, 10.0f, 1.0);
	factory->createParam("auxSustain", "Sustain", 0.0f, 1.0f, 0.0);
	factory->createParam("auxRelease", "Release", 0.0f, 10.0f, 1.0);

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

	factory->createParam("chrDelay", "Delay", 0.0f, 1.0, 0.0);
	factory->createParam("chrModulation", "MOdulation", 0.0f, 1.0f, 1.0);
	factory->createParam("chrFeedback", "Feedback", 0.0f, 1.0f, 0.0);
	factory->createParam("chrMix", "Mix", 0.0f, 1.0f, 0.0);

	factory->createParam("osc1Pitch", "Osc1 pitch", -36, 36, 0);
	factory->createParam("osc1Fine", "Osc1 fine", -1.0, 1.0, 0);
	factory->createParam("osc1Volume", "Osc1 volume", 0, 1.0, 1);
	factory->createParam("osc1Pan", "Osc1 pan", -1.0, 1.0, 0);
	factory->createParam("osc1Shape", "Osc1 shape", 0, 3.0, 0);

	factory->createParam("osc2Pitch", "Osc2 pitch", -36, 36, 0);
	factory->createParam("osc2Fine", "Osc2 fine", -1.0, 1.0, 0);
	factory->createParam("osc2Volume", "Osc2 volume", 0, 1.0, 1);
	factory->createParam("osc2Pan", "Osc2 pan", -1.0, 1.0, 0);
	factory->createParam("osc2Shape", "Osc2 shape", 0, 3.0, 0);

	factory->createParam("osc3Pitch", "Osc3 pitch", -36, 36, 0);
	factory->createParam("osc3Fine", "Osc3 fine", -1.0, 1.0, 0);
	factory->createParam("osc3Volume", "Osc3 volume", 0, 1.0, 1);
	factory->createParam("osc3Pan", "Osc3 pan", -1.0, 1.0, 0);
	factory->createParam("osc3Shape", "Osc3 shape", 0, 3.0, 0);

	factory->createParam("osc4Pitch", "Osc4 pitch", -36, 36, 0);
	factory->createParam("osc4Fine", "Osc4 fine", -1.0, 1.0, 0);
	factory->createParam("osc4Volume", "Osc4 volume", 0, 1.0, 1);
	factory->createParam("osc4Pan", "Osc4 pan", -1.0, 1.0, 0);
	factory->createParam("osc4Shape", "Osc4 shape", 0, 3.0, 0);

	factory->createParam("lfo1Shape", "Lfo1 shape", 0, 3.0, 0);
	factory->createParam("lfo2Shape", "Lfo2 shape", 0, 3.0, 0);

	factory->createParam("lfo1Speed", "Lfo1 speed", 0, 10.0, 0);
	factory->createParam("lfo2Speed", "Lfo2 speed", 0, 10.0, 0);

	factory->createParam("lfo1Amount", "Lfo1 amount", 0, 1.0, 0);
	factory->createParam("lfo2Amount", "Lfo2 amount", 0, 1.0, 0);


	bypass = parameters->createAndAddParameter("bypass", "bypass", "Bypass", NormalisableRange<float>(0, 1),0, nullptr, nullptr);
	
	Logger::getCurrentLogger()->writeToLog("Building preset list");

	String presetPath = appDataPath + "/Audio/Presets/pueski/Lupo/";
	
	File presets = File(presetPath);

	if (presets.exists() && presets.isDirectory()) {
		ScopedPointer<DirectoryIterator> iter = new DirectoryIterator(presets, false);
		while (iter->next()) {
			if (iter->getFile().exists() && !iter->getFile().isDirectory()) {
				String name = iter->getFile().getFileNameWithoutExtension();
				Logger::getCurrentLogger()->writeToLog("Found preset : " + name);
				programNames.push_back(name);
			}

		}
		iter = nullptr;

	}

	Logger::getCurrentLogger()->writeToLog("Found"+String(programNames.size())+" presets");
	
}

LupoAudioProcessor::~LupoAudioProcessor()
{	/*
	lupo.reset();
	model.reset();
	parameters.reset();
	 */	
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

	if (index < 0 || index > programNames.size()) {
		index = 0;
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

	String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
	String presetPath = appDataPath + "/Audio/Presets/pueski/Lupo/";

	String filename = name + ".xml";
	File preset = File(presetPath + filename);

	if (preset.exists()) {

		ScopedPointer<XmlElement> xml = XmlDocument(preset).getDocumentElement();
		ValueTree state = ValueTree::fromXml(*xml.get());
		
		getValueTreeState()->state = state;

		xml = nullptr;
		this->selectedProgram = name;
		lupo->updateState();
		Logger::getCurrentLogger()->writeToLog("Updating synth state");
	}

}

const String LupoAudioProcessor::getProgramName (int index)
{
	if (getNumPrograms() > 0 && index < getNumPrograms() && index >= 0) {
		return programNames.at(index);
	}

	return "Empty";
}

void LupoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LupoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	Logger::getCurrentLogger()->writeToLog("Preparing playback");
	if (!prepared) {
		prepared = true;
		// setSelectedProgram("init");
	}
	lupo->prepareToPlay(sampleRate, samplesPerBlock);

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
	/*
	Logger::getCurrentLogger()->writeToLog("getStateInformation");

	MemoryOutputStream stream;
	parameters->state.writeToStream(stream);	
	destData.append(stream.getData(), stream.getDataSize());
	*/
	
	if (parameters == nullptr) {
		return;
	}
	if (parameters->state.isValid()) {
		parameters->state.setProperty("program", getCurrentProgram(), nullptr);
		ScopedPointer<XmlElement> xml(parameters->state.createXml());
		copyXmlToBinary(*xml, destData);
	}
	
	
}

void LupoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	
	
	if (sizeInBytes > 0) {
		ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
		if (xmlState != nullptr)
			if (xmlState->hasTagName(parameters->state.getType())) {
				parameters->state = ValueTree::fromXml(*xmlState);
				if (parameters->state.hasProperty("program")) {
					setSelectedProgram(getProgramName(parameters->state.getProperty("program")));
				}
			}

	}
	/*
	if (parameters->state.isValid()) {
		MemoryInputStream stream(data, sizeInBytes, false);
		parameters->state.readFromStream(stream);
		lupo->updateState();
	}
	*/
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

