#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "AudioEngine/LupoSynth.h"
#include "MainUI.h"
#include <vector>
#include <memory>

class AttachmentFactory
{
public:
    AttachmentFactory(LupoAudioProcessor* processor, LupoSynth* lupo)
        : processor(processor), lupo(lupo)
    {
    }

    void clearAttachments() {
        attachments.clear();
        buttonAttachments.clear();
        comboAttachments.clear();
    }

    void initState() {
        processor->getValueTreeState()->state = ValueTree(Identifier("default"));
    }

    void createParam(String name, String label, float min, float max, float defaultValue) {
        processor->getValueTreeState()->createAndAddParameter(
            name, name, label,
            NormalisableRange<float>(min, max),
            defaultValue, nullptr, nullptr
        );
        processor->getValueTreeState()->addParameterListener(name, lupo);
    }

    void createSliderAttachment(String name, Slider* comp) {
        attachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
                *processor->getValueTreeState(), name, *comp
            )
        );
    }

    void createComboAttachment(String name, ComboBox* comp) {
        comboAttachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
                *processor->getValueTreeState(), name, *comp
            )
        );
    }

    void createButtonAttachment(String name, Button* comp) {
        buttonAttachments.push_back(
            std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(
                *processor->getValueTreeState(), name, *comp
            )
        );
    }

    LupoSynth* getSynth() {
        return lupo;
    }

    StringArray getSliderParams() {
        return {
            "cutoff1","resonance1","mainVolume","envAmt1",
            "cutoff2","resonance2","envAmt2",
            "ampAttack","ampDecay","ampSustain","ampRelease",
            "fltAttack", "fltDecay", "fltSustain", "fltRelease",
            "auxAttack1", "auxDecay1","auxSustain1","auxRelease1",
            "auxAttack2", "auxDecay2","auxSustain2","auxRelease2",
            "lfo1Shape", "lfo1Speed","lfo1Amount",
            "lfo2Shape", "lfo2Speed","lfo2Amount",
            "lfo3Shape", "lfo3Speed","lfo3Amount",
            "osc1Pitch","osc1Fine","osc1Volume","osc1Pan",
            "osc2Pitch","osc2Fine","osc2Volume","osc2Pan",  // Fixed: added comma
            "osc3Pitch","osc3Fine","osc3Volume","osc3Pan",
            "osc4Pitch","osc4Fine","osc4Volume","osc4Pan",
            "dlyTimeLeft", "dlyTimeRight", "dlyFeedback","dlyMix",
            "rvbRoomSize", "rvbDamping", "rvbWetLevel", "rvbDryLevel", "rvbWidth", "rvbFreezeMode",  // Fixed typo
            "chrDelay", "chrModulation", "chrFeedback", "chrMix",
            "filterMode","cutoffLink"
            // "arpSpeed", "arpTempo", "arpMode","arpClockMode","arpOctaves","arpEnabled"  // Added arpeggiator parameters
        };
    }

private:
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> attachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>> comboAttachments;

    LupoAudioProcessor* processor;
    LupoSynth* lupo;
};