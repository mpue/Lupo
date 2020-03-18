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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OscillatorPanel  : public Component,
                         public Slider::Listener,
                         public Button::Listener
{
public:
    //==============================================================================
    OscillatorPanel ();
    ~OscillatorPanel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* oscillator_square_48_png;
    static const int oscillator_square_48_pngSize;
    static const char* oscillator_sine_48_png;
    static const int oscillator_sine_48_pngSize;
    static const char* oscillator_saw_48_png;
    static const int oscillator_saw_48_pngSize;
    static const char* oscillator_noise_48_png;
    static const int oscillator_noise_48_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<GroupComponent> groupComponent4;
    std::unique_ptr<Slider> oscPitch;
    std::unique_ptr<Label> pitchlabel1;
    std::unique_ptr<Slider> oscFine;
    std::unique_ptr<Label> fineLabel;
    std::unique_ptr<ImageButton> pulseButton;
    std::unique_ptr<ImageButton> sawButton;
    std::unique_ptr<ImageButton> sineButton;
    std::unique_ptr<ImageButton> noiseButton;
    std::unique_ptr<Slider> pwSlider;
    std::unique_ptr<Label> pwLabel;
    std::unique_ptr<ToggleButton> syncButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorPanel)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

