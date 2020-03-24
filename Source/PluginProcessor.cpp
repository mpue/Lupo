/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioEngine/MultimodeOscillator.h"

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
	model.reset(new Model());
	lupo.reset(new LupoSynth(model.get()));
	parameters.reset(new AudioProcessorValueTreeState(*this, nullptr));
	bypass = parameters.get()->createAndAddParameter("bypass", "bypass", "Bypass", NormalisableRange<float>(0, 1),0, nullptr, nullptr);
	
	Logger::getCurrentLogger()->writeToLog("Building preset list");

	String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
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
	if (programNames.size() > 0) {

		if (index > programNames.size() - 1) {
			index = programNames.size() - 1;
		}

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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	
	if (parameters == nullptr) {
		return;
	}
	if (parameters.get()->state.isValid()) {
		parameters.get()->state.setProperty("program", getCurrentProgram(), nullptr);
		ScopedPointer<XmlElement> xml(parameters.get()->state.createXml());
		copyXmlToBinary(*xml, destData);
	}
	
}

void LupoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

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
	return this->parameters.get();
}

