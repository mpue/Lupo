#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "AudioEngine/LupoSynth.h"
#include "MainUI.h"
#include <map>
class AttachmentFactory
{
public:
	AttachmentFactory(LupoAudioProcessor* processor, LupoSynth* lupo, MainUI* ui) {
		this->processor = processor;
		this->lupo = lupo;
		this->ui = ui;
		this->attachments = new vector<AudioProcessorValueTreeState::SliderAttachment*>();
	}

	void initState() {		
		processor->getValueTreeState()->state = ValueTree(Identifier("default"));
	}

	void createParam(String name, String label , float min, float max, float defaultValue) {
		processor->getValueTreeState()->createAndAddParameter(name, name, label, NormalisableRange<float>(min,max), defaultValue, nullptr, nullptr);
		processor->getValueTreeState()->addParameterListener(name, lupo);
	}

	void  createAttachment(LupoAudioProcessor* processor, String name,Slider* comp) {
		AudioProcessorValueTreeState::SliderAttachment* att = new AudioProcessorValueTreeState::SliderAttachment(*processor->getValueTreeState(), name, *comp);
		attachments->push_back(att);
	}

	~AttachmentFactory() {
		for (int i = 0; i < attachments->size(); i++) {
			delete attachments->at(i);
		}
		delete attachments;
	};

private:

	vector<AudioProcessorValueTreeState::SliderAttachment*>* attachments;

	class ParamDescriptor {
	public:
		ParamDescriptor(NormalisableRange<float> range, float defaultValue) {
			this->range = range;
			this->defaultValue = defaultValue;
		}
	private:
		NormalisableRange<float> range;
		float defaultValue;
	};

	/*


	MultimodeOscillator::OscMode osc1Shape;
	MultimodeOscillator::OscMode osc2Shape;
	MultimodeOscillator::OscMode osc3Shape;
	MultimodeOscillator::OscMode osc4Shape;
*/
	StringArray AttachmentFactory::getSliderParams()
	{
		return {
			"cutoff","resonance","mainVolume","envAmt",
			"ampAttack","ampDecay","ampSustain","ampRelease",
			"fltAttack", "fltDecay", "fltSustain", "fltRelease",
			"auxAttack", "auxDecay","auxSustain","auxRelease",
			"osc1Pitch","osc1Fine","osc1Volume","osc1Pan",
			"osc2Pitch","osc2Fine","osc2Volume","osc2Pan"
			"osc3Pitch","osc3Fine","osc3Volume","osc3Pan",
			"osc4Pitch","osc2Fine","osc4Volume","osc4Pan"
			"dlyTimeLeft", "dlyTimeRight", "dlyFeedback","dlyMix",
			"rvbRoomSize", "rvbDdamping", "rvbWetLevel", "rvbDryLevel", "rvbWidth", "rvbFreezeMode",
			"chrDelay", "chrModulation", "chrFeedback", "chrMix"
		};
	}

	LupoAudioProcessor* processor;
	LupoSynth* lupo;
	MainUI* ui;
};

