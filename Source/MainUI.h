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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainUI  : public Component,
                public Slider::Listener
{
public:
    //==============================================================================
    MainUI (LupoSynth* lupo);
    ~MainUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	LupoSynth* lupo;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupComponent7;
    std::unique_ptr<GroupComponent> groupComponent3;
    std::unique_ptr<GroupComponent> groupComponent5;
    std::unique_ptr<GroupComponent> groupComponent4;
    std::unique_ptr<GroupComponent> groupComponent;
    std::unique_ptr<GroupComponent> groupComponent2;
    std::unique_ptr<Slider> osc1Pitch;
    std::unique_ptr<Slider> osc2Pitch;
    std::unique_ptr<Label> pitchlabel1;
    std::unique_ptr<Label> pitchlabel2;
    std::unique_ptr<Slider> amp_attack;
    std::unique_ptr<Slider> amp_decay;
    std::unique_ptr<Slider> amp_sustain;
    std::unique_ptr<Slider> amp_release;
    std::unique_ptr<Label> attack;
    std::unique_ptr<Label> decay;
    std::unique_ptr<Label> sustain;
    std::unique_ptr<Label> Release;
    std::unique_ptr<Slider> osc1Fine;
    std::unique_ptr<Slider> osc2Fine;
    std::unique_ptr<Label> fineLabel;
    std::unique_ptr<Label> fineLabel2;
    std::unique_ptr<Slider> flt_attack;
    std::unique_ptr<Slider> flt_decay;
    std::unique_ptr<Slider> flt_sustain;
    std::unique_ptr<Slider> flt_release;
    std::unique_ptr<Label> attack2;
    std::unique_ptr<Label> decay2;
    std::unique_ptr<Label> sustain2;
    std::unique_ptr<Label> Release2;
    std::unique_ptr<Slider> flt_release2;
    std::unique_ptr<Label> EnvAmt;
    std::unique_ptr<GroupComponent> groupComponent6;
    std::unique_ptr<Slider> flt_cutoff;
    std::unique_ptr<Slider> flt_res;
    std::unique_ptr<Label> Cutoff;
    std::unique_ptr<Label> Res;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainUI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

