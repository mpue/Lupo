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
#include "Model.h"
class AttachmentFactory;
class GraphicalEnvelope;
class LupoSynth;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EnvelopePanel  : public Component,
                       public ChangeBroadcaster,
                       public Slider::Listener,
                       public Timer
{
public:
    //==============================================================================
    EnvelopePanel (Model* model, AttachmentFactory* factory);
    ~EnvelopePanel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void initAttachments();
	void drawEnvelopeGraph(Graphics& g);
	void timerCallback() override;
	void updateEnvelopeFromModel();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

	void setDecayTime(float time);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Model* model;
	AttachmentFactory* factory;
	std::unique_ptr<GraphicalEnvelope> graphicalEnvelope;
	LupoSynth* synth = nullptr; // Reference to synthesizer for real-time data
	
	// Phase tracking variables for accurate timing visualization
	double lastPhaseChangeTime = 0.0;
	int lastPhase = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Slider> amp_attack;
    std::unique_ptr<Slider> amp_decay;
    std::unique_ptr<Slider> amp_sustain;
    std::unique_ptr<Slider> amp_release;
    std::unique_ptr<Label> attack;
    std::unique_ptr<Label> decay;
    std::unique_ptr<Label> sustain;
    std::unique_ptr<Label> Release;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopePanel)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

