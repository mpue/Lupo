/*
  ==============================================================================

    ElegantDarkLookAndFeel.h
    Created: 27 Sep 2025 9:53:04pm
    Author:  mpue

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ElegantDarkLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ElegantDarkLookAndFeel()
    {
        // Basis Farbschema
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::DocumentWindow::backgroundColourId, juce::Colour(0xff1a1a1a));

        // Text Farben
        setColour(juce::Label::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::TextEditor::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::TextEditor::highlightColourId, juce::Colour(0x664d9eff));
        setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour(0xff4d9eff));

        // Button Farben
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff4d9eff));
        setColour(juce::TextButton::textColourOffId, juce::Colour(0xffe8e8e8));
        setColour(juce::TextButton::textColourOnId, juce::Colour(0xffffffff));

        // ComboBox
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::ComboBox::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::ComboBox::buttonColourId, juce::Colour(0xff404040));
        setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffe8e8e8));

        // PopupMenu
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::PopupMenu::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff4d9eff));
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xffffffff));

        // Slider
        setColour(juce::Slider::backgroundColourId, juce::Colour(0xff404040));
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff4d9eff));
        setColour(juce::Slider::trackColourId, juce::Colour(0xff666666));
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff4d9eff));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff404040));
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff404040));

        // ToggleButton
        setColour(juce::ToggleButton::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::ToggleButton::tickColourId, juce::Colour(0xff4d9eff));
        setColour(juce::ToggleButton::tickDisabledColourId, juce::Colour(0xff666666));

        // ListBox
        setColour(juce::ListBox::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::ListBox::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::ListBox::textColourId, juce::Colour(0xffe8e8e8));

        // TreeView
        setColour(juce::TreeView::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::TreeView::linesColourId, juce::Colour(0xff404040));
        setColour(juce::TreeView::dragAndDropIndicatorColourId, juce::Colour(0xff4d9eff));
        setColour(juce::TreeView::selectedItemBackgroundColourId, juce::Colour(0x664d9eff));

        // Scrollbar
        setColour(juce::ScrollBar::backgroundColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::ScrollBar::thumbColourId, juce::Colour(0xff4d9eff));
        setColour(juce::ScrollBar::trackColourId, juce::Colour(0xff2a2a2a));

        // TabbedComponent
        setColour(juce::TabbedComponent::backgroundColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::TabbedComponent::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::TabbedButtonBar::tabOutlineColourId, juce::Colour(0xff404040));
        setColour(juce::TabbedButtonBar::frontOutlineColourId, juce::Colour(0xff4d9eff));

        // TableListBox
        setColour(juce::TableListBox::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::TableListBox::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colour(0xff333333));
        setColour(juce::TableHeaderComponent::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::TableHeaderComponent::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::TableHeaderComponent::highlightColourId, juce::Colour(0xff4d9eff));

        // ProgressBar
        setColour(juce::ProgressBar::backgroundColourId, juce::Colour(0xff404040));
        setColour(juce::ProgressBar::foregroundColourId, juce::Colour(0xff4d9eff));

        // GroupComponent
        setColour(juce::GroupComponent::outlineColourId, juce::Colour(0xff404040));
        setColour(juce::GroupComponent::textColourId, juce::Colour(0xffe8e8e8));

        // DirectoryContentsDisplayComponent
        setColour(juce::DirectoryContentsDisplayComponent::highlightColourId, juce::Colour(0x664d9eff));
        setColour(juce::DirectoryContentsDisplayComponent::textColourId, juce::Colour(0xffe8e8e8));

        // FileBrowserComponent
        setColour(juce::FileBrowserComponent::currentPathBoxBackgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::FileBrowserComponent::currentPathBoxTextColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::FileBrowserComponent::currentPathBoxArrowColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::FileBrowserComponent::filenameBoxBackgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::FileBrowserComponent::filenameBoxTextColourId, juce::Colour(0xffe8e8e8));

        // AlertWindow
        setColour(juce::AlertWindow::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::AlertWindow::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::AlertWindow::outlineColourId, juce::Colour(0xff404040));

        // TooltipWindow
        setColour(juce::TooltipWindow::backgroundColourId, juce::Colour(0xff333333));
        setColour(juce::TooltipWindow::textColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::TooltipWindow::outlineColourId, juce::Colour(0xff404040));

        // CodeEditorComponent
        setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::CodeEditorComponent::highlightColourId, juce::Colour(0x664d9eff));
        setColour(juce::CodeEditorComponent::defaultTextColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::CodeEditorComponent::lineNumberBackgroundId, juce::Colour(0xff2a2a2a));
        setColour(juce::CodeEditorComponent::lineNumberTextId, juce::Colour(0xff999999));

        // CaretComponent
        setColour(juce::CaretComponent::caretColourId, juce::Colour(0xff4d9eff));

        // HyperlinkButton
        setColour(juce::HyperlinkButton::textColourId, juce::Colour(0xff4d9eff));

        // KeyboardComponentBase
        setColour(juce::KeyboardComponentBase::upDownButtonBackgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::KeyboardComponentBase::upDownButtonArrowColourId, juce::Colour(0xffe8e8e8));

        // MidiKeyboardComponent
        setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colour(0xffe8e8e8));
        setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colour(0xff404040));
        setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colour(0x664d9eff));
        setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, juce::Colour(0x994d9eff));
        setColour(juce::MidiKeyboardComponent::textLabelColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::MidiKeyboardComponent::upDownButtonBackgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::MidiKeyboardComponent::upDownButtonArrowColourId, juce::Colour(0xffe8e8e8));

		setColour(juce::TabbedComponent::backgroundColourId, juce::Colour(0xff1a1a1a));


        // Schriftart setzen
        setDefaultSansSerifTypefaceName("Arial");
    }

    // Square LED toggle button
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool isMouseOverButton, bool isButtonDown) override
    {
        const bool  on      = button.getToggleState();
        const float ledSize = juce::jmin(14.0f, (float)button.getHeight() * 0.55f);
        const float ledX    = 2.0f;
        const float ledY    = ((float)button.getHeight() - ledSize) * 0.5f;

        juce::Rectangle<float> led(ledX, ledY, ledSize, ledSize);

        // Outer frame
        g.setColour(juce::Colour(0xff404040));
        g.drawRect(led, 1.0f);

        if (on)
        {
            // Glow halo
            juce::Colour glowCol = juce::Colour(0xff4d9eff).withAlpha(0.25f);
            g.setColour(glowCol);
            g.fillRect(led.expanded(2.5f));

            // Bright fill
            g.setColour(juce::Colour(0xff4d9eff));
            g.fillRect(led.reduced(1.0f));

            // Inner highlight
            g.setColour(juce::Colours::white.withAlpha(0.35f));
            g.fillRect(led.reduced(1.0f).removeFromTop(ledSize * 0.35f).removeFromLeft(ledSize * 0.55f));
        }
        else
        {
            // Dark fill
            g.setColour(juce::Colour(0xff1a1a1a));
            g.fillRect(led.reduced(1.0f));

            // Subtle dim centre
            g.setColour(juce::Colour(0xff2a3040));
            g.fillRect(led.reduced(3.0f));
        }

        // Label text
        g.setColour(button.findColour(juce::ToggleButton::textColourId)
                          .withAlpha(button.isEnabled() ? 1.0f : 0.4f));
        g.setFont(juce::Font(juce::jmin(13.0f, (float)button.getHeight() * 0.55f)));
        g.drawText(button.getButtonText(),
                   (int)(ledX + ledSize + 6.0f), 0,
                   button.getWidth() - (int)(ledX + ledSize + 6.0f), button.getHeight(),
                   juce::Justification::centredLeft, false);
    }

    // Custom Button Drawing mit abgerundeten Ecken und Glow-Effekt
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        auto cornerSize = 8.0f;

        if (isButtonDown)
        {
            g.setColour(backgroundColour.brighter(0.2f));
        }
        else if (isMouseOverButton)
        {
            g.setColour(backgroundColour.brighter(0.1f));

            // Glow-Effekt f�r Hover
            juce::Path glowPath;
            glowPath.addRoundedRectangle(bounds.expanded(2.0f), cornerSize + 2.0f);
            juce::ColourGradient glow(juce::Colour(0x334d9eff), bounds.getCentreX(), bounds.getCentreY(),
                juce::Colour(0x004d9eff), bounds.getRight(), bounds.getBottom(), true);
            g.setGradientFill(glow);
            g.fillPath(glowPath);
        }
        else
        {
            g.setColour(backgroundColour);
        }

        g.fillRoundedRectangle(bounds, cornerSize);

        // Subtiler Outline
        g.setColour(juce::Colour(0xff404040));
        g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
    }

    // Rotary slider using image strip (128 frames), resolution chosen by slider size.
    // Tick-mark scale drawn in the outer ring around the knob.
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& /*slider*/) override
    {
        const int sliderSize = juce::jmin(width, height);

        juce::Image strip;
        if (sliderSize <= 40)
            strip = juce::ImageCache::getFromMemory(BinaryData::Knob_32_png,  BinaryData::Knob_32_pngSize);
        else if (sliderSize <= 56)
            strip = juce::ImageCache::getFromMemory(BinaryData::Knob_48_png,  BinaryData::Knob_48_pngSize);
        else if (sliderSize <= 96)
            strip = juce::ImageCache::getFromMemory(BinaryData::Knob_64_png,  BinaryData::Knob_64_pngSize);
        else
            strip = juce::ImageCache::getFromMemory(BinaryData::Knob_128_png, BinaryData::Knob_128_pngSize);

        juce::Image shadow = juce::ImageCache::getFromMemory(BinaryData::knob_shadow_64_png,
                                                             BinaryData::knob_shadow_64_pngSize);

        const float outerRadius = (float)juce::jmin(width / 2, height / 2);
        const float centreX     = (float)x + (float)width  * 0.5f;
        const float centreY     = (float)y + (float)height * 0.5f;

        // Knob occupies the inner 78 %; ticks use the outer ring
        const float knobRadius = outerRadius * 0.78f;
        const float kx = centreX - knobRadius;
        const float ky = centreY - knobRadius;
        const float kw = knobRadius * 2.0f;

        // --- Tick scale ---
        // 33 ticks spanning rotaryStartAngle … rotaryEndAngle;
        // every 4th tick (indices 0,4,8,…,32) is a major tick.
        const int   numTicks = 33;
        const float angleRange = rotaryEndAngle - rotaryStartAngle;

        for (int i = 0; i < numTicks; ++i)
        {
            const float t         = (float)i / (float)(numTicks - 1);
            const float tickAngle = rotaryStartAngle + t * angleRange;
            const bool  isMajor   = (i % 4 == 0);
            const bool  isActive  = (t <= sliderPos + 0.001f);

            // Major ticks start at the knob edge; minor ticks start slightly further out
            const float iR = isMajor ? outerRadius * 0.78f : outerRadius * 0.83f;
            const float oR = isMajor ? outerRadius * 0.99f : outerRadius * 0.93f;

            const float sinA = std::sin(tickAngle);
            const float cosA = std::cos(tickAngle);

            const float px1 = centreX + iR * sinA;
            const float py1 = centreY - iR * cosA;
            const float px2 = centreX + oR * sinA;
            const float py2 = centreY - oR * cosA;

            juce::Colour col;
            if (isActive)
                col = isMajor ? juce::Colour(0xff4d9eff) : juce::Colour(0xaa4d9eff);
            else
                col = isMajor ? juce::Colour(0xff555555) : juce::Colour(0xff383838);

            g.setColour(col);
            g.drawLine(px1, py1, px2, py2, isMajor ? 1.5f : 1.0f);
        }

        // Drop shadow
        g.drawImage(shadow,
                    (int)kx, (int)ky, (int)kw, (int)kw,
                    0, 0, shadow.getWidth(), shadow.getHeight());

        // Knob frame
        const int nFrames  = strip.getHeight() / strip.getWidth();
        const int frameIdx = juce::jlimit(0, nFrames - 1,
                                (int)std::ceil(sliderPos * (double)(nFrames - 1)));

        g.drawImage(strip,
                    (int)kx, (int)ky, (int)kw, (int)kw,
                    0, frameIdx * strip.getWidth(), strip.getWidth(), strip.getWidth());
    }

    /*
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (slider.isBar())
        {
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.fillRect(slider.isHorizontal() ? juce::Rectangle<float>(static_cast<float>(x), (float)y + (float)height * 0.4f,
                sliderPos - (float)x, (float)height * 0.2f)
                : juce::Rectangle<float>((float)x + (float)width * 0.4f, sliderPos,
                    (float)width * 0.2f, (float)y + (float)height - sliderPos));
        }
        else
        {
            auto trackWidth = juce::jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f);

            juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

            juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                slider.isHorizontal() ? startPoint.y : (float)y);

            juce::Path backgroundTrack;
            backgroundTrack.startNewSubPath(startPoint);
            backgroundTrack.lineTo(endPoint);
            g.setColour(slider.findColour(juce::Slider::backgroundColourId));
            g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

            juce::Path valueTrack;
            juce::Point<float> minPoint, maxPoint, thumbPoint;

            if (slider.isHorizontal())
            {
                minPoint = startPoint;
                maxPoint = { sliderPos, startPoint.y };
                thumbPoint = { sliderPos, startPoint.y };
            }
            else
            {
                minPoint = { startPoint.x, sliderPos };
                maxPoint = endPoint;
                thumbPoint = { startPoint.x, sliderPos };
            }

            valueTrack.startNewSubPath(minPoint);
            valueTrack.lineTo(maxPoint);
            g.setColour(slider.findColour(juce::Slider::trackColourId));
            g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

            // Thumb mit Glow
            auto thumbRadius = trackWidth * 1.5f;
            g.setColour(juce::Colour(0x664d9eff));
            g.fillEllipse(juce::Rectangle<float>(thumbRadius * 2.0f, thumbRadius * 2.0f).withCentre(thumbPoint));
            g.setColour(slider.findColour(juce::Slider::thumbColourId));
            g.fillEllipse(juce::Rectangle<float>(thumbRadius, thumbRadius).withCentre(thumbPoint));
        }
    }
    */

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearHorizontal)
        {
            // Hintergrund
            g.setColour(juce::Colour(0xFF1a2332));
            g.fillRoundedRectangle(x, y, width, height, 4.0f);

            // VU-Meter Bereich berechnen
            float fillWidth = sliderPos - x;

            // Gradient f�r VU-Meter erstellen (Blaut�ne)
            juce::ColourGradient gradient(
                juce::Colour(0xFF4a90e2), x, y,  // Helles Blau (links)
                juce::Colour(0xFF2e5c8a), sliderPos, y,  // Dunkles Blau (rechts)
                false
            );

            // Optional: Warnstufe bei hohen Werten (�ber 80%)
            float valueRange = slider.getMaximum() - slider.getMinimum();
            float normalizedValue = (slider.getValue() - slider.getMinimum()) / valueRange;

            if (normalizedValue > 0.8f)
            {
                gradient = juce::ColourGradient(
                    juce::Colour(0xFF4a90e2), x, y,
                    juce::Colour(0xFF5a7fa0), sliderPos, y,
                    false
                );
            }

            g.setGradientFill(gradient);
            g.fillRoundedRectangle(x, y, fillWidth, height, 4.0f);

            // Segmentierte Darstellung (optional)
            g.setColour(juce::Colour(0xFF1a2332));
            int segments = 20;
            float segmentWidth = width / (float)segments;
            for (int i = 1; i < segments; ++i)
            {
                float segX = x + i * segmentWidth;
                if (segX < sliderPos)
                {
                    g.drawLine(segX, y + 2, segX, y + height - 2, 2.0f);
                }
            }

            // Glanzeffekt oben
            g.setGradientFill(juce::ColourGradient(
                juce::Colour(0x40ffffff), x, y,
                juce::Colour(0x00ffffff), x, y + height * 0.5f,
                false
            ));
            g.fillRoundedRectangle(x, y, fillWidth, height * 0.5f, 4.0f);

            // Rahmen
            g.setColour(juce::Colour(0xFF0d1419));
            g.drawRoundedRectangle(x, y, width, height, 4.0f, 1.5f);
        }
        else
        {
            // Fallback f�r andere Slider-Stile
            LookAndFeel_V4::drawLinearSlider(g, x, y, width, height,
                sliderPos, minSliderPos, maxSliderPos,
                style, slider);
        }
    }

    // Custom ComboBox Arrow
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
        int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override
    {
        auto cornerSize = 6.0f;
        auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat();

        g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
        g.fillRoundedRectangle(bounds, cornerSize);

        g.setColour(box.findColour(juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle(bounds.reduced(0.5f, 0.5f), cornerSize, 1.0f);

        juce::Rectangle<int> arrowZone(width - 30, 0, 20, height);
        juce::Path path;
        path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 2.0f);
        path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f);
        path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f);

        g.setColour(box.findColour(juce::ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : 0.2f));
        g.strokePath(path, juce::PathStrokeType(2.0f));
    }

    // Custom ScrollBar
    void drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height,
        bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override
    {
        juce::Rectangle<int> thumbBounds;

        if (isScrollbarVertical)
            thumbBounds = { x + 2, thumbStartPosition + 2, width - 4, thumbSize - 4 };
        else
            thumbBounds = { thumbStartPosition + 2, y + 2, thumbSize - 4, height - 4 };

        auto cornerSize = (float)juce::jmin(thumbBounds.getWidth(), thumbBounds.getHeight()) * 0.5f;

        g.setColour(scrollbar.findColour(juce::ScrollBar::thumbColourId).withAlpha(isMouseOver ? 0.8f : 0.6f));
        g.fillRoundedRectangle(thumbBounds.toFloat(), cornerSize);
    }

    // Bessere Fonts f�r bessere Lesbarkeit
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
    {
        return juce::Font(juce::jmin(15.0f, (float)buttonHeight * 0.6f), juce::Font::FontStyleFlags::plain);
    }

    juce::Font getPopupMenuFont() override
    {
        return juce::Font(14.0f);
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        return juce::Font(14.0f);
    }

    juce::Font getLabelFont(juce::Label&) override
    {
        return juce::Font(14.0f);
    }

    // Besseres PopupMenu Styling
    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        g.fillAll(findColour(juce::PopupMenu::backgroundColourId));

        g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.1f));
        g.drawRect(0, 0, width, height);
    }


    /*void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
        bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
        const juce::String& text, const juce::String& shortcutKeyText,
        const juce::Drawable* icon, const juce::Colour* textColour) override
    {
        if (isSeparator)
        {
            auto r = area.reduced(5, 0);
            r.removeFromTop(r.getHeight() / 2);
            g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.3f));
            g.fillRect(r.removeFromTop(1));
        }
        else
        {
            auto textColourToUse = (textColour == nullptr ? findColour(juce::PopupMenu::textColourId) : *textColour);

            if (isHighlighted)
            {
                g.setColour(findColour(juce::PopupMenu::highlightedBackgroundColourId));
                g.fillRect(area.reduced(1));
                g.setColour(findColour(juce::PopupMenu::highlightedTextColourId));
            }
            else
            {
                g.setColour(textColourToUse);
            }

            if (!isActive)
                g.setOpacity(0.3f);

            juce::Font font(getPopupMenuFont());
            auto maxFontHeight = (float)area.getHeight() / 1.3f;

            if (font.getHeight() > maxFontHeight)
                font.setHeight(maxFontHeight);

            g.setFont(font);

            auto& iconArea = area.removeFromLeft(static_cast<int>(area.getHeight())).reduced(3).toFloat();

            if (icon != nullptr)
            {
                icon->drawWithin(g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
            }
            else if (isTicked)
            {
                auto tick = getTickShape(1.0f);
                g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
            }

            if (hasSubMenu)
            {
                auto arrowH = 0.6f * getPopupMenuFont().getAscent();
                auto x = static_cast<float>(area.getRight() - area.getHeight());
                auto halfH = static_cast<float>(area.getHeight()) * 0.5f;

                juce::Path path;
                path.startNewSubPath(x, halfH - arrowH * 0.5f);
                path.lineTo(x + arrowH * 0.6f, halfH);
                path.lineTo(x, halfH + arrowH * 0.5f);

                g.strokePath(path, juce::PathStrokeType(2.0f));
            }

            auto r = area.reduced(iconArea.getWidth(), 0);

            if (!shortcutKeyText.isEmpty())
            {
                auto f2 = font;
                f2.setHeight(f2.getHeight() * 0.75f);
                f2.setHorizontalScale(0.95f);
                g.setFont(f2);

                g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
            }

            g.setFont(font);
            g.drawFittedText(text, r.reduced(2, 0), juce::Justification::centredLeft, 1);
        }
    }*/
};
