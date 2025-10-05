/*
  ==============================================================================

    CyberpunkLookAndFeel.h
    Created: Ultimate Cyberpunk-style LookAndFeel with black background and neon colors
    Author:  AI Assistant - Cyberpunk Style Master

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CyberpunkLookAndFeel : public LookAndFeel_V4 {

public:
    CyberpunkLookAndFeel();
    ~CyberpunkLookAndFeel();

    // Extended cyberpunk color palette with more neon variety
    static const Colour CYBER_BLACK;
    static const Colour CYBER_YELLOW;
    static const Colour CYBER_DARK_YELLOW;
    static const Colour CYBER_BRIGHT_YELLOW;
    static const Colour CYBER_DARK_GREY;
    static const Colour CYBER_MID_GREY;
    static const Colour CYBER_LIGHT_GREY;
    static const Colour CYBER_GLOW;
    
    // Additional neon colors for variation
    static const Colour CYBER_CYAN;
    static const Colour CYBER_BRIGHT_CYAN;
    static const Colour CYBER_DARK_CYAN;
    static const Colour CYBER_MAGENTA;
    static const Colour CYBER_BRIGHT_MAGENTA;
    static const Colour CYBER_DARK_MAGENTA;
    static const Colour CYBER_GREEN;
    static const Colour CYBER_BRIGHT_GREEN;
    static const Colour CYBER_DARK_GREEN;
    static const Colour CYBER_ORANGE;
    static const Colour CYBER_BRIGHT_ORANGE;
    static const Colour CYBER_PURPLE;
    static const Colour CYBER_BRIGHT_PURPLE;

    // Rotary Slider - Futuristic knobs with neon glow
    void drawRotarySlider(Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle, float rotaryEndAngle,
        Slider& slider) override;

    // Linear Slider - Cyberpunk bars (ElegantDark style for horizontal)
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider) override;

    // Button - Neon outlined buttons
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override;

    void drawButtonText(Graphics& g, TextButton& button,
        bool isMouseOverButton, bool isButtonDown) override;

    // Toggle Button - Cyberpunk switches
    void drawToggleButton(Graphics& g, ToggleButton& button,
        bool isMouseOverButton, bool isButtonDown) override;

    // ComboBox - Futuristic dropdown
    void drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
        int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;

    void positionComboBoxText(ComboBox& box, Label& labelToPosition) override;

    // PopupMenu - Dark cyberpunk menu
    void drawPopupMenuBackground(Graphics& g, int width, int height) override;

    void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const String& text,
        const String& shortcutKeyText,
        const Drawable* icon, const Colour* const textColourToUse) override;

    Font getPopupMenuFont() override;

    // Label - Neon text
    void drawLabel(Graphics& g, Label& label) override;

    // GroupComponent - Cyberpunk frames
    void drawGroupComponentOutline(Graphics& g, int w, int h, const String& text,
        const Justification& position, GroupComponent& group) override;

    // TextEditor - Cyberpunk input fields
    void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override;
    void drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor) override;

    // TabBar - Futuristic tabs
    void drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override;
    
    void fillTabButtonShape(TabBarButton& button, Graphics& g, const Path& path,
        bool isMouseOver, bool isMouseDown) override;

    void drawTabButtonText(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override;

    void drawTabbedButtonBarBackground(TabbedButtonBar& bar, Graphics& g) override;

    void drawTabAreaBehindFrontButton(TabbedButtonBar& bar, Graphics& g, int w, int h) override;

    // Scrollbars - Cyberpunk style
    void drawScrollbarButton(Graphics& g, ScrollBar& scrollbar, int width, int height,
        int buttonDirection, bool isScrollbarVertical,
        bool isMouseOverButton, bool isButtonDown) override;

    void drawScrollbar(Graphics& g, ScrollBar& scrollbar, int x, int y, int width, int height,
        bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
        bool isMouseOver, bool isMouseDown) override;

    // Viewport
    void drawCornerResizer(Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;

    // Font overrides for smaller text
    Font getTextButtonFont(TextButton&, int buttonHeight) override;
    Font getComboBoxFont(ComboBox&) override;
    Font getLabelFont(Label&) override;

    // Special effects
    void drawGlowEffect(Graphics& g, const Rectangle<float>& area, const Colour& glowColor, float intensity = 1.0f);
    void drawCyberGrid(Graphics& g, const Rectangle<int>& area, float gridSize = 20.0f);
    void drawNeonBorder(Graphics& g, const Rectangle<float>& area, const Colour& neonColor, float thickness = 2.0f);
    void drawHexagonalPattern(Graphics& g, const Rectangle<int>& area, float hexSize = 15.0f);
    void drawCircuitPattern(Graphics& g, const Rectangle<int>& area, float spacing = 25.0f);
    void drawDataStream(Graphics& g, const Rectangle<int>& area, float speed = 1.0f);

private:
    Font getCyberFont(float size = 10.0f); // Reduced default size
    void drawCyberButton(Graphics& g, const Rectangle<int>& area, bool isPressed, bool isHighlighted, const String& text = "");
    Colour getRandomNeonColor();
    
    // Animation helpers
    float glowAnimationPhase;
    float colorCyclePhase;
    int64 lastUpdateTime;
    void updateAnimations();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CyberpunkLookAndFeel)
};