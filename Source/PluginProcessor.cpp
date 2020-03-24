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
}

LupoAudioProcessor::~LupoAudioProcessor()
{	
	 lupo = nullptr;
	 model = nullptr;
	 parameters = nullptr;
// delete messageBus;
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
	return programNames.size();
}

int LupoAudioProcessor::getCurrentProgram()
{
	return this->currentProgramNumber;
}

void LupoAudioProcessor::setCurrentProgram (int index)
{
	String name = programNames.at(index);
	this->currentProgramNumber = index;
	this->setSelectedProgram(name);
}

void LupoAudioProcessor::setSelectedProgram(juce::String name) {

	this->selectedProgram = name;

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
		
	}

	lupo->updateState();

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
	lupo->prepareToPlay(sampleRate, samplesPerBlock);

	if (!prepared) {
		prepared = true;

		if (selectedProgram != "") {
			setSelectedProgram(selectedProgram);
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

	lupo->processBlock(buffer,midiMessages);
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
}

void LupoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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

