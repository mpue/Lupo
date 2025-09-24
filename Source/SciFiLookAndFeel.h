/*
  ==============================================================================

    SciFiLookAndFeel.h
    Created: Sci-Fi themed LookAndFeel based on TrioLookAndFeel
    Author:  Modified from Matthias Pueski's original

  ==============================================================================
*/

#ifndef SCIFILOOKANDFEEL_H_INCLUDED
#define SCIFILOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SciFiLookAndFeel : public LookAndFeel_V2 {

public:

    SciFiLookAndFeel();

    ~SciFiLookAndFeel();

    void drawRotarySlider(Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        Slider& slider) override;

    void drawToggleButton(Graphics& g, ToggleButton& button,
        bool isMouseOverButton, bool isButtonDown) override;

    void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const String& text,
        const String& shortcutKeyText,
        const Drawable* icon, const Colour* const textColourToUse) override;

    void drawPopupMenuBackground(Graphics&, int width, int height) override;

    void drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
        int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;

    Font getPopupMenuFont() override;

    void drawLabel(Graphics& g, Label& label) override;

    void drawGroupComponentOutline(Graphics&, int w, int h, const String& text,
        const Justification&, GroupComponent&) override;

    void drawScrollbarButton(Graphics&, ScrollBar&, int width, int height,
        int buttonDirection, bool isScrollbarVertical,
        bool isMouseOverButton, bool isButtonDown) override;

    void drawScrollbar(Graphics&, ScrollBar&, int x, int y, int width, int height,
        bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
        bool isMouseOver, bool isMouseDown) override;

    void drawShinyButtonShape(Graphics& g,
        float x, float y, float w, float h,
        float maxCornerSize,
        const Colour& baseColour,
        const float strokeWidth,
        const bool flatOnLeft,
        const bool flatOnRight,
        const bool flatOnTop,
        const bool flatOnBottom);

    void drawTableHeaderBackground(Graphics&, TableHeaderComponent&) override;

    void drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override;

    void fillTabButtonShape(TabBarButton& button, Graphics& g, const Path& path,
        bool isMouseOver, bool isMouseDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SciFiLookAndFeel)
};

#endif  // SCIFILOOKANDFEEL_H_INCLUDED