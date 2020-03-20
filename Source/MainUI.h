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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioEngine/LupoSynth.h"
#include "OscillatorPanel.h"
#include "EnvelopePanel.h"
#include "LFOPanel.h"
#include "ReverbPanel.h"
#include "MixerChannelPanel.h"
class Model;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainUI  : public Component,
                public ChangeBroadcaster,
                public ChangeListener,
                public Slider::Listener
{
public:
    //==============================================================================
    MainUI (Model* model, LupoSynth* synth);
    ~MainUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback(ChangeBroadcaster* source);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Model* model;
	LupoSynth* synth;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> ModulationGroup;
    std::unique_ptr<GroupComponent> groupComponent;
    std::unique_ptr<GroupComponent> groupComponent3;
    std::unique_ptr<GroupComponent> groupComponent6;
    std::unique_ptr<Slider> fltCutoff;
    std::unique_ptr<Slider> fltResonance;
    std::unique_ptr<Label> Cutoff;
    std::unique_ptr<Label> Res;
    std::unique_ptr<OscillatorPanel> osc1Panel;
    std::unique_ptr<OscillatorPanel> osc3Panel;
    std::unique_ptr<OscillatorPanel> osc2Panel;
    std::unique_ptr<EnvelopePanel> ampEnvelope;
    std::unique_ptr<EnvelopePanel> filterEnvelope;
    std::unique_ptr<Slider> mainVolume;
    std::unique_ptr<Label> volumeLabel;
    std::unique_ptr<Slider> envAmt;
    std::unique_ptr<Label> Amt;
    std::unique_ptr<GroupComponent> mixerGroiup;
    std::unique_ptr<MixerChannelPanel> ch1Panel;
    std::unique_ptr<MixerChannelPanel> ch2Panel;
    std::unique_ptr<MixerChannelPanel> ch3Panel;
    std::unique_ptr<MixerChannelPanel> ch4Panel;
    std::unique_ptr<OscillatorPanel> osc4Panel;
    std::unique_ptr<LFOPanel> lfo1;
    std::unique_ptr<LFOPanel> lfo2;
    std::unique_ptr<GroupComponent> FXGroup;
    std::unique_ptr<ReverbPanel> reverbPanel;
    std::unique_ptr<EnvelopePanel> auxEnvelope;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

