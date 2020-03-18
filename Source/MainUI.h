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
#include "MixerChannelPanel.h"
#include "MessageBus/BusListener.h"
#include "MessageBus/Topic.h"
#include "MessageBus/MessageBus.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainUI  : public Component,
                public BusListener,
                public Slider::Listener
{
public:
    //==============================================================================
    MainUI (LupoSynth* lupo);
    ~MainUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void topicChanged(Topic* t) override;

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	LupoSynth* lupo;
    //[/UserVariables]

    //==============================================================================
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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

