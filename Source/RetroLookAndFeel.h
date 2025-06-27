#pragma once
#include <JuceHeader.h>

/**
 * RetroLookAndFeel – A JUCE Look&Feel that mimics a realistic
 * warm retro-golden hardware synthesiser panel (inspired by mockup).
 */
class RetroLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RetroLookAndFeel()
    {
        using namespace juce;

        // Panel background & text colour (warm beige + dark text)
        setColour(ResizableWindow::backgroundColourId, Colour(0xffe3dec7)); // warm light panel
        setColour(Label::textColourId, Colour(0xff2a261e));                // dark brown text

        // Slider & knob colours
        setColour(Slider::rotarySliderFillColourId, Colour(0xffd4caa9));   // warm gold knob
        setColour(Slider::thumbColourId, Colour(0xff3d3a32));              // dark thumb
        setColour(Slider::trackColourId, Colour(0xffa79e88));              // darker gold track

        // Buttons
        setColour(TextButton::buttonColourId, Colour(0xffa79e88));         // button face
        setColour(TextButton::textColourOffId, Colour(0xff2a261e));        // button text
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        using namespace juce;

        const float diameter = jmin(width, height) - 4.0f;
        const float radius = diameter * 0.5f;
        const float centreX = (float)x + width * 0.5f;
        const float centreY = (float)y + height * 0.5f;
        const Rectangle<float> knobArea(centreX - radius, centreY - radius, diameter, diameter);

        // Shadow
        g.setColour(Colours::black.withAlpha(0.35f));
        g.fillEllipse(knobArea.translated(2.0f, 2.5f));

        // Knob body – radial gradient
        ColourGradient grad(Colour(0xffd4caa9), centreX - radius * 0.3f, centreY - radius * 0.3f,
            Colour(0xffb8af94), centreX + radius * 0.3f, centreY + radius * 0.3f, false);
        g.setGradientFill(grad);
        g.fillEllipse(knobArea);

        // Rim – dark outline
        g.setColour(Colour(0xff3d3a32));
        g.drawEllipse(knobArea, 1.5f);

        // Indicator pointer
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        Path p;
        p.addRectangle(-1.0f, -radius * 0.7f, 2.0f, radius * 0.55f);
        g.setColour(Colour(0xff2a261e));
        g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float /*minSliderPos*/, float /*maxSliderPos*/, const juce::Slider::SliderStyle style,
        juce::Slider& slider) override
    {
        using namespace juce;
        if (style != Slider::LinearVertical)
        {
            LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, 0, 0, style, slider);
            return;
        }

        const float trackW = width * 0.25f;
        const float trackX = x + (width - trackW) * 0.5f;

        g.setColour(Colour(0xffa79e88));
        g.fillRoundedRectangle(trackX, (float)y, trackW, (float)height, trackW * 0.5f);

        const float capH = jmin(20.0f, (float)width * 0.9f);
        const Rectangle<float> cap(x + width * 0.1f, sliderPos - capH * 0.5f, width * 0.8f, capH);
        ColourGradient grad(Colour(0xffd4caa9), cap.getTopLeft(), Colour(0xffb8af94), cap.getBottomRight(), false);
        g.setGradientFill(grad);
        g.fillRoundedRectangle(cap, 3.0f);
        g.setColour(Colour(0xff3d3a32));
        g.drawRoundedRectangle(cap, 3.0f, 1.0f);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& b, bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override
    {
        using namespace juce;
        const int size = jmin(b.getHeight(), b.getWidth());
        const Rectangle<int> area(0, 0, size, size);

        bool on = b.getToggleState();
        Colour body = on ? Colour(0xff3d3a32) : Colour(0xffa79e88);

        g.setColour(body);
        g.fillEllipse(area.toFloat());
        g.setColour(Colour(0xff2a261e));
        g.drawEllipse(area.toFloat(), 1.0f);

        const Rectangle<float> led(size * 0.35f, size * 0.35f, size * 0.3f, size * 0.3f);
        g.setColour(on ? Colour(0xfff6b818) : Colours::black.withAlpha(0.4f));
        g.fillEllipse(led);

        g.setColour(findColour(Label::textColourId));
        g.setFont(Font(12.0f, Font::plain));
        const int textX = size + 6;
        const Rectangle<int> textArea(textX, 0, b.getWidth() - textX, b.getHeight());
        g.drawText(b.getButtonText(), textArea, Justification::centredLeft, true);
    }
};
