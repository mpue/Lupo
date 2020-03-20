/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
	model = new Model();
	lupo = new LupoSynth(model);
}

LupoAudioProcessor::~LupoAudioProcessor()
{
	/*
	delete lupo;
	*/
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LupoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LupoAudioProcessor::setCurrentProgram (int index)
{
}

const String LupoAudioProcessor::getProgramName (int index)
{
    return {};
}

void LupoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LupoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

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
	return lupo;
}

Model* LupoAudioProcessor::getModel() {
	return model;
}