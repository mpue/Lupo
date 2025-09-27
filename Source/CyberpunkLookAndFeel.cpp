/*
  ==============================================================================

    CyberpunkLookAndFeel.cpp
    Created: Ultimate Cyberpunk-style LookAndFeel with black background and neon yellow
    Author:  AI Assistant - Cyberpunk Style Master

  ==============================================================================
*/

#include "CyberpunkLookAndFeel.h"

// Cyberpunk Color Palette
const Colour CyberpunkLookAndFeel::CYBER_BLACK = Colour(0xff0a0a0a);
const Colour CyberpunkLookAndFeel::CYBER_YELLOW = Colour(0xffffff00);
const Colour CyberpunkLookAndFeel::CYBER_DARK_YELLOW = Colour(0xff997700);
const Colour CyberpunkLookAndFeel::CYBER_BRIGHT_YELLOW = Colour(0xffffff99);
const Colour CyberpunkLookAndFeel::CYBER_DARK_GREY = Colour(0xff1a1a1a);
const Colour CyberpunkLookAndFeel::CYBER_MID_GREY = Colour(0xff333333);
const Colour CyberpunkLookAndFeel::CYBER_LIGHT_GREY = Colour(0xff666666);
const Colour CyberpunkLookAndFeel::CYBER_GLOW = Colour(0x88ffff00);

CyberpunkLookAndFeel::CyberpunkLookAndFeel() : glowAnimationPhase(0.0f), lastUpdateTime(Time::currentTimeMillis())
{
    // Set up the cyberpunk color scheme
    setColour(ResizableWindow::backgroundColourId, CYBER_BLACK);
    setColour(DocumentWindow::backgroundColourId, CYBER_BLACK);
    
    // Sliders
    setColour(Slider::thumbColourId, CYBER_YELLOW);
    setColour(Slider::trackColourId, CYBER_DARK_YELLOW);
    setColour(Slider::backgroundColourId, CYBER_DARK_GREY);
    setColour(Slider::rotarySliderFillColourId, CYBER_YELLOW);
    setColour(Slider::rotarySliderOutlineColourId, CYBER_LIGHT_GREY);
    setColour(Slider::textBoxTextColourId, CYBER_YELLOW);
    setColour(Slider::textBoxBackgroundColourId, CYBER_BLACK);
    setColour(Slider::textBoxOutlineColourId, CYBER_YELLOW);
    
    // Buttons
    setColour(TextButton::buttonColourId, CYBER_DARK_GREY);
    setColour(TextButton::buttonOnColourId, CYBER_DARK_YELLOW);
    setColour(TextButton::textColourOffId, CYBER_YELLOW);
    setColour(TextButton::textColourOnId, CYBER_BLACK);
    
    // ComboBox
    setColour(ComboBox::backgroundColourId, CYBER_DARK_GREY);
    setColour(ComboBox::textColourId, CYBER_YELLOW);
    setColour(ComboBox::outlineColourId, CYBER_YELLOW);
    setColour(ComboBox::arrowColourId, CYBER_YELLOW);
    setColour(ComboBox::buttonColourId, CYBER_DARK_YELLOW);
    
    // Labels
    setColour(Label::textColourId, CYBER_YELLOW);
    setColour(Label::backgroundColourId, Colours::transparentBlack);
    setColour(Label::outlineColourId, Colours::transparentBlack);
    
    // GroupComponent
    setColour(GroupComponent::outlineColourId, CYBER_YELLOW);
    setColour(GroupComponent::textColourId, CYBER_BRIGHT_YELLOW);
    
    // TextEditor
    setColour(TextEditor::backgroundColourId, CYBER_BLACK);
    setColour(TextEditor::textColourId, CYBER_YELLOW);
    setColour(TextEditor::outlineColourId, CYBER_YELLOW);
    setColour(TextEditor::focusedOutlineColourId, CYBER_BRIGHT_YELLOW);
    
    // PopupMenu
    setColour(PopupMenu::backgroundColourId, CYBER_BLACK);
    setColour(PopupMenu::textColourId, CYBER_YELLOW);
    setColour(PopupMenu::highlightedBackgroundColourId, CYBER_DARK_YELLOW);
    setColour(PopupMenu::highlightedTextColourId, CYBER_BLACK);
    
    // TabbedComponent
    setColour(TabbedComponent::backgroundColourId, CYBER_BLACK);
    setColour(TabbedComponent::outlineColourId, CYBER_YELLOW);
    setColour(TabbedButtonBar::tabOutlineColourId, CYBER_YELLOW);
    setColour(TabbedButtonBar::tabTextColourId, CYBER_YELLOW);
    setColour(TabbedButtonBar::frontOutlineColourId, CYBER_BRIGHT_YELLOW);
    setColour(TabbedButtonBar::frontTextColourId, CYBER_BLACK);
    
    // ScrollBar
    setColour(ScrollBar::backgroundColourId, CYBER_DARK_GREY);
    setColour(ScrollBar::thumbColourId, CYBER_DARK_YELLOW);
    setColour(ScrollBar::trackColourId, CYBER_BLACK);
}

CyberpunkLookAndFeel::~CyberpunkLookAndFeel()
{
}

void CyberpunkLookAndFeel::updateAnimations()
{
    int64 currentTime = Time::currentTimeMillis();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;
    
    glowAnimationPhase += deltaTime * 2.0f; // 2 Hz animation
    if (glowAnimationPhase > MathConstants<float>::twoPi)
        glowAnimationPhase -= MathConstants<float>::twoPi;
}

Font CyberpunkLookAndFeel::getCyberFont(float size)
{
    // Use a monospace font for that cyberpunk terminal feel
    return Font(Font::getDefaultMonospacedFontName(), size, Font::bold);
}

void CyberpunkLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    updateAnimations();
    
    const float radius = jmin(width / 2, height / 2) - 4.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    // Draw outer glow ring
    float glowIntensity = 0.7f + 0.3f * std::sin(glowAnimationPhase);
    g.setColour(CYBER_GLOW.withAlpha(glowIntensity * 0.6f));
    g.fillEllipse(rx - 6, ry - 6, rw + 12, rw + 12);
    
    // Draw outer ring with hexagonal pattern
    g.setColour(CYBER_LIGHT_GREY);
    g.drawEllipse(rx - 2, ry - 2, rw + 4, rw + 4, 2.0f);
    
    // Draw main knob background
    g.setColour(CYBER_DARK_GREY);
    g.fillEllipse(rx, ry, rw, rw);
    
    // Draw inner circuit pattern
    g.setColour(CYBER_DARK_YELLOW.withAlpha(0.3f));
    for (int i = 0; i < 8; ++i)
    {
        float lineAngle = i * MathConstants<float>::pi / 4.0f;
        float innerRadius = radius * 0.3f;
        float outerRadius = radius * 0.7f;
        
        float x1 = centreX + innerRadius * std::cos(lineAngle);
        float y1 = centreY + innerRadius * std::sin(lineAngle);
        float x2 = centreX + outerRadius * std::cos(lineAngle);
        float y2 = centreY + outerRadius * std::sin(lineAngle);
        
        g.drawLine(x1, y1, x2, y2, 1.0f);
    }
    
    // Draw value arc
    Path valueArc;
    valueArc.addCentredArc(centreX, centreY, radius * 0.8f, radius * 0.8f, 0.0f,
                          rotaryStartAngle, angle, true);
    
    g.setColour(CYBER_YELLOW);
    g.strokePath(valueArc, PathStrokeType(3.0f, PathStrokeType::curved, PathStrokeType::rounded));
    
    // Draw glowing value arc
    g.setColour(CYBER_GLOW.withAlpha(glowIntensity));
    g.strokePath(valueArc, PathStrokeType(1.0f));
    
    // Draw pointer
    float pointerLength = radius * 0.6f;
    float pointerThickness = 3.0f;
    float pointerX = centreX + pointerLength * std::cos(angle - MathConstants<float>::halfPi);
    float pointerY = centreY + pointerLength * std::sin(angle - MathConstants<float>::halfPi);
    
    g.setColour(CYBER_BRIGHT_YELLOW);
    g.drawLine(centreX, centreY, pointerX, pointerY, pointerThickness);
    
    // Draw center dot with glow
    g.setColour(CYBER_GLOW.withAlpha(glowIntensity));
    g.fillEllipse(centreX - 4, centreY - 4, 8, 8);
    g.setColour(CYBER_YELLOW);
    g.fillEllipse(centreX - 2, centreY - 2, 4, 4);
}

void CyberpunkLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const Slider::SliderStyle style, Slider& slider)
{
    updateAnimations();
    
    const bool isHorizontal = (style == Slider::LinearHorizontal || style == Slider::LinearBar);
    const float trackWidth = isHorizontal ? height * 0.25f : width * 0.25f;
    const float trackIndent = isHorizontal ? height * 0.375f : width * 0.375f;
    
    Rectangle<float> trackArea;
    if (isHorizontal)
        trackArea = Rectangle<float>(x, y + trackIndent, width, trackWidth);
    else
        trackArea = Rectangle<float>(x + trackIndent, y, trackWidth, height);
    
    // Draw track background with glow
    float glowIntensity = 0.5f + 0.3f * std::sin(glowAnimationPhase);
    drawGlowEffect(g, trackArea.expanded(4), CYBER_GLOW, glowIntensity * 0.3f);
    
    g.setColour(CYBER_DARK_GREY);
    g.fillRoundedRectangle(trackArea, trackWidth * 0.5f);
    
    g.setColour(CYBER_YELLOW);
    g.drawRoundedRectangle(trackArea, trackWidth * 0.5f, 1.0f);
    
    // Draw filled portion
    Rectangle<float> filledArea = trackArea;
    if (isHorizontal)
        filledArea.setWidth(sliderPos - x);
    else
    {
        filledArea.setY(sliderPos);
        filledArea.setHeight(y + height - sliderPos);
    }
    
    g.setColour(CYBER_DARK_YELLOW);
    g.fillRoundedRectangle(filledArea, trackWidth * 0.5f);
    
    // Draw thumb (for non-bar styles)
    if (style != Slider::LinearBar)
    {
        const float thumbSize = isHorizontal ? height * 0.8f : width * 0.8f;
        
        Rectangle<float> thumbArea;
        if (isHorizontal)
            thumbArea = Rectangle<float>(sliderPos - thumbSize * 0.5f, y + height * 0.1f, thumbSize, thumbSize);
        else
            thumbArea = Rectangle<float>(x + width * 0.1f, sliderPos - thumbSize * 0.5f, thumbSize, thumbSize);
        
        // Draw thumb glow
        drawGlowEffect(g, thumbArea.expanded(3), CYBER_GLOW, glowIntensity);
        
        // Draw thumb
        g.setColour(CYBER_DARK_GREY);
        g.fillRoundedRectangle(thumbArea, thumbSize * 0.25f);
        
        g.setColour(CYBER_YELLOW);
        g.drawRoundedRectangle(thumbArea, thumbSize * 0.25f, 2.0f);
        
        // Draw center indicator
        Rectangle<float> centerArea = thumbArea.reduced(thumbSize * 0.3f);
        g.setColour(CYBER_BRIGHT_YELLOW);
        g.fillRoundedRectangle(centerArea, centerArea.getHeight() * 0.5f);
    }
}

void CyberpunkLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
    bool isMouseOverButton, bool isButtonDown)
{
    updateAnimations();
    
    Rectangle<int> buttonArea = button.getLocalBounds();
    Rectangle<float> area = buttonArea.toFloat().reduced(1);
    
    float glowIntensity = 0.4f + 0.2f * std::sin(glowAnimationPhase);
    if (isMouseOverButton) glowIntensity += 0.3f;
    if (isButtonDown) glowIntensity += 0.5f;
    
    // Draw outer glow
    drawGlowEffect(g, area.expanded(4), CYBER_GLOW, glowIntensity);
    
    // Draw main button
    Colour buttonColour = isButtonDown ? CYBER_DARK_YELLOW : (isMouseOverButton ? CYBER_MID_GREY : CYBER_DARK_GREY);
    
    g.setColour(buttonColour);
    g.fillRoundedRectangle(area, 4.0f);
    
    // Draw border with corner accents
    g.setColour(isButtonDown ? CYBER_BRIGHT_YELLOW : CYBER_YELLOW);
    g.drawRoundedRectangle(area, 4.0f, 1.5f);
    
    // Draw corner accents
    float cornerSize = 8.0f;
    g.setColour(CYBER_BRIGHT_YELLOW);
    
    // Top-left corner
    g.drawLine(area.getX(), area.getY() + cornerSize, area.getX(), area.getY(), 2.0f);
    g.drawLine(area.getX(), area.getY(), area.getX() + cornerSize, area.getY(), 2.0f);
    
    // Top-right corner
    g.drawLine(area.getRight() - cornerSize, area.getY(), area.getRight(), area.getY(), 2.0f);
    g.drawLine(area.getRight(), area.getY(), area.getRight(), area.getY() + cornerSize, 2.0f);
    
    // Bottom-left corner
    g.drawLine(area.getX(), area.getBottom() - cornerSize, area.getX(), area.getBottom(), 2.0f);
    g.drawLine(area.getX(), area.getBottom(), area.getX() + cornerSize, area.getBottom(), 2.0f);
    
    // Bottom-right corner
    g.drawLine(area.getRight() - cornerSize, area.getBottom(), area.getRight(), area.getBottom(), 2.0f);
    g.drawLine(area.getRight(), area.getBottom(), area.getRight(), area.getBottom() - cornerSize, 2.0f);
}

void CyberpunkLookAndFeel::drawButtonText(Graphics& g, TextButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    Font font = getCyberFont(button.getHeight() * 0.4f);
    g.setFont(font);
    
    Colour textColour = isButtonDown ? CYBER_BLACK : CYBER_YELLOW;
    if (isMouseOverButton && !isButtonDown) textColour = CYBER_BRIGHT_YELLOW;
    
    g.setColour(textColour);
    
    const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
    const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;
    
    const int fontHeight = roundToInt(font.getHeight() * 0.6f);
    const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;
    
    if (textWidth > 0)
    {
        g.drawFittedText(button.getButtonText(),
                        leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                        Justification::centred, 2);
    }
}

void CyberpunkLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    updateAnimations();
    
    Rectangle<int> buttonArea = button.getLocalBounds();
    const int height = buttonArea.getHeight();
    const int width = height; // Make it square
    const int x = buttonArea.getX();
    const int y = buttonArea.getY();
    
    Rectangle<float> toggleArea(x, y, width, height);
    toggleArea.reduce(2, 2);
    
    float glowIntensity = 0.4f + 0.2f * std::sin(glowAnimationPhase);
    if (isMouseOverButton) glowIntensity += 0.3f;
    
    // Draw glow
    drawGlowEffect(g, toggleArea.expanded(3), CYBER_GLOW, glowIntensity);
    
    // Draw background
    g.setColour(CYBER_DARK_GREY);
    g.fillRoundedRectangle(toggleArea, 3.0f);
    
    g.setColour(CYBER_YELLOW);
    g.drawRoundedRectangle(toggleArea, 3.0f, 1.5f);
    
    // Draw toggle state
    if (button.getToggleState())
    {
        Rectangle<float> innerArea = toggleArea.reduced(4);
        g.setColour(CYBER_BRIGHT_YELLOW);
        g.fillRoundedRectangle(innerArea, 2.0f);
        
        // Draw "ON" indicator - a glowing center
        Rectangle<float> onIndicator = innerArea.reduced(6);
        drawGlowEffect(g, onIndicator, CYBER_GLOW, 1.0f);
        g.setColour(CYBER_YELLOW);
        g.fillEllipse(onIndicator);
    }
    
    // Draw text
    if (button.getButtonText().isNotEmpty())
    {
        g.setFont(getCyberFont(height * 0.3f));
        g.setColour(button.getToggleState() ? CYBER_BRIGHT_YELLOW : CYBER_YELLOW);
        
        Rectangle<int> textArea(x + width + 8, y, button.getWidth() - width - 8, height);
        g.drawFittedText(button.getButtonText(), textArea, Justification::centredLeft, 1);
    }
}

void CyberpunkLookAndFeel::drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
    int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
{
    updateAnimations();
    
    Rectangle<float> boxArea(0, 0, width, height);
    boxArea.reduce(1, 1);
    
    float glowIntensity = 0.3f + 0.2f * std::sin(glowAnimationPhase);
    if (box.hasKeyboardFocus(true)) glowIntensity += 0.4f;
    if (isButtonDown) glowIntensity += 0.3f;
    
    // Draw glow
    drawGlowEffect(g, boxArea.expanded(2), CYBER_GLOW, glowIntensity);
    
    // Draw main box
    g.setColour(CYBER_DARK_GREY);
    g.fillRoundedRectangle(boxArea, 3.0f);
    
    g.setColour(CYBER_YELLOW);
    g.drawRoundedRectangle(boxArea, 3.0f, 1.0f);
    
    // Draw arrow button area
    Rectangle<float> arrowArea(buttonX, buttonY, buttonW, buttonH);
    arrowArea.reduce(2, 2);
    
    if (isButtonDown)
    {
        g.setColour(CYBER_DARK_YELLOW);
        g.fillRoundedRectangle(arrowArea, 2.0f);
    }
    
    // Draw arrow
    float arrowSize = jmin(arrowArea.getWidth(), arrowArea.getHeight()) * 0.3f;
    float arrowX = arrowArea.getCentreX();
    float arrowY = arrowArea.getCentreY();
    
    Path arrow;
    arrow.addTriangle(arrowX - arrowSize * 0.5f, arrowY - arrowSize * 0.25f,
                     arrowX + arrowSize * 0.5f, arrowY - arrowSize * 0.25f,
                     arrowX, arrowY + arrowSize * 0.5f);
    
    g.setColour(CYBER_BRIGHT_YELLOW);
    g.fillPath(arrow);
}

void CyberpunkLookAndFeel::positionComboBoxText(ComboBox& box, Label& labelToPosition)
{
    labelToPosition.setBounds(8, 1, box.getWidth() - 30, box.getHeight() - 2);
    labelToPosition.setFont(getCyberFont(box.getHeight() * 0.5f));
}

void CyberpunkLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height)
{
    Rectangle<float> area(0, 0, width, height);
    
    // Draw dark background with glow border
    g.setColour(CYBER_BLACK.withAlpha(0.95f));
    g.fillRoundedRectangle(area, 5.0f);
    
    drawNeonBorder(g, area, CYBER_YELLOW, 2.0f);
    
    // Draw subtle grid pattern
    drawCyberGrid(g, area.toNearestInt(), 15.0f);
}

void CyberpunkLookAndFeel::drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
    const bool isSeparator, const bool isActive,
    const bool isHighlighted, const bool isTicked,
    const bool hasSubMenu, const String& text,
    const String& shortcutKeyText,
    const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator)
    {
        Rectangle<float> r = area.toFloat().reduced(5, 0);
        r.removeFromTop(r.getHeight() * 0.5f - 0.5f);
        r.setHeight(1.0f);
        
        g.setColour(CYBER_DARK_YELLOW);
        g.fillRect(r);
        return;
    }
    
    Rectangle<float> r = area.toFloat().reduced(1.0f, 0);
    
    if (isHighlighted)
    {
        drawGlowEffect(g, r, CYBER_GLOW, 0.5f);
        g.setColour(CYBER_DARK_YELLOW.withAlpha(0.8f));
        g.fillRoundedRectangle(r, 3.0f);
    }
    
    const float leftBorder = (float)area.getX() + 10.0f;
    const float rightBorder = (float)area.getRight() - 4.0f;
    const float textY = (float)area.getY();
    
    if (isTicked)
    {
        Rectangle<float> tick(leftBorder - 8, textY, 8, (float)area.getHeight());
        g.setColour(CYBER_BRIGHT_YELLOW);
        g.drawFittedText("?", tick.toNearestInt(), Justification::centred, 1);
    }
    
    if (hasSubMenu)
    {
        float arrowH = 0.6f * (float)area.getHeight();
        float x = rightBorder - arrowH;
        float halfH = arrowH * 0.5f;
        float centreY = (float)area.getCentreY();
        
        Path p;
        p.addTriangle(x, centreY - halfH, x, centreY + halfH, x + halfH, centreY);
        
        g.setColour(CYBER_YELLOW);
        g.fillPath(p);
    }
    
    Rectangle<float> textArea(leftBorder, textY, rightBorder - leftBorder, (float)area.getHeight());
    
    if (!isActive)
        g.setOpacity(0.3f);
    
    Font font = getCyberFont((float)area.getHeight() * 0.5f);
    g.setFont(font);
    
    Colour textColour = isHighlighted ? CYBER_BLACK : CYBER_YELLOW;
    if (textColourToUse != nullptr)
        textColour = *textColourToUse;
    
    g.setColour(textColour);
    g.drawFittedText(text, textArea.toNearestInt(), Justification::centredLeft, 1);
    
    if (shortcutKeyText.isNotEmpty())
    {
        Font shortcutFont = getCyberFont((float)area.getHeight() * 0.4f);
        g.setFont(shortcutFont);
        g.setColour(textColour.withAlpha(0.7f));
        g.drawFittedText(shortcutKeyText, textArea.toNearestInt(), Justification::centredRight, 1);
    }
}

Font CyberpunkLookAndFeel::getPopupMenuFont()
{
    return getCyberFont(15.0f);
}

void CyberpunkLookAndFeel::drawLabel(Graphics& g, Label& label)
{
    g.fillAll(label.findColour(Label::backgroundColourId));
    
    if (!label.isBeingEdited())
    {
        const float alpha = label.isEnabled() ? 1.0f : 0.5f;
        const Font font = getCyberFont(label.getHeight() * 0.8f);
        
        g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
        g.setFont(font);
        
        Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
        
        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                        jmax(1, (int)(textArea.getHeight() / font.getHeight())),
                        label.getMinimumHorizontalScale());
        
        g.setColour(label.findColour(Label::outlineColourId).withMultipliedAlpha(alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(Label::outlineColourId));
    }
    
    g.drawRect(label.getLocalBounds());
}

void CyberpunkLookAndFeel::drawGroupComponentOutline(Graphics& g, int w, int h, const String& text,
    const Justification& position, GroupComponent& group)
{
    updateAnimations();
    
    const float indent = 3.0f;
    const float textEdgeGap = 4.0f;
    auto cs = 5.0f;
    
    Font f = getCyberFont(15.0f);
    
    Path p;
    auto x = indent;
    auto y = f.getAscent() - 3.0f;
    auto w2 = w - x * 2.0f;
    auto h2 = h - y - indent;
    cs = jmin(cs, w2 * 0.5f, h2 * 0.5f);
    auto cs2 = 2.0f * cs;
    
    auto textW = text.isEmpty() ? 0 : jlimit(0.0f, jmax(0.0f, w2 - cs2 - textEdgeGap * 2), f.getStringWidth(text) + textEdgeGap * 2.0f);
    auto textX = cs + textEdgeGap;
    
    if (position.testFlags(Justification::horizontallyCentred))
        textX = cs + (w2 - cs2 - textW) * 0.5f;
    else if (position.testFlags(Justification::right))
        textX = w2 - cs - textW;
    
    p.startNewSubPath(x + textX + textW, y);
    p.lineTo(x + w2 - cs, y);
    
    p.addArc(x + w2 - cs2, y, cs2, cs2, 0, MathConstants<float>::halfPi);
    p.lineTo(x + w2, y + h2 - cs);
    
    p.addArc(x + w2 - cs2, y + h2 - cs2, cs2, cs2, MathConstants<float>::halfPi, MathConstants<float>::pi);
    p.lineTo(x + cs, y + h2);
    
    p.addArc(x, y + h2 - cs2, cs2, cs2, MathConstants<float>::pi, MathConstants<float>::pi * 1.5f);
    p.lineTo(x, y + cs);
    
    p.addArc(x, y, cs2, cs2, MathConstants<float>::pi * 1.5f, MathConstants<float>::twoPi);
    p.lineTo(x + textX, y);
    
    auto alpha = group.isEnabled() ? 1.0f : 0.5f;
    
    // Draw glow effect
    float glowIntensity = 0.3f + 0.2f * std::sin(glowAnimationPhase);
    g.setColour(CYBER_GLOW.withAlpha(glowIntensity * alpha));
    g.strokePath(p, PathStrokeType(3.0f));
    
    g.setColour(group.findColour(GroupComponent::outlineColourId).withMultipliedAlpha(alpha));
    g.strokePath(p, PathStrokeType(2.0f));
    
    g.setColour(group.findColour(GroupComponent::textColourId).withMultipliedAlpha(alpha));
    g.setFont(f);
    g.drawText(text,
              roundToInt(x + textX), 0,
              roundToInt(textW),
              roundToInt(f.getHeight()),
              Justification::centred, true);
}

void CyberpunkLookAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
    if (dynamic_cast<AlertWindow*> (textEditor.getParentComponent()) != nullptr)
    {
        g.setColour(textEditor.findColour(TextEditor::backgroundColourId));
        g.fillRect(0, 0, width, height);
        
        g.setColour(textEditor.findColour(TextEditor::outlineColourId));
        g.drawHorizontalLine(height - 1, 0.0f, static_cast<float> (width));
    }
    else
    {
        Rectangle<float> area(0, 0, width, height);
        
        if (textEditor.hasKeyboardFocus(true))
        {
            drawGlowEffect(g, area.expanded(2), CYBER_GLOW, 0.6f);
        }
        
        g.setColour(textEditor.findColour(TextEditor::backgroundColourId));
        g.fillRoundedRectangle(area, 4.0f);
    }
}

void CyberpunkLookAndFeel::drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor)
{
    if (dynamic_cast<AlertWindow*> (textEditor.getParentComponent()) == nullptr)
    {
        if (textEditor.isEnabled())
        {
            if (textEditor.hasKeyboardFocus(true) && !textEditor.isReadOnly())
            {
                g.setColour(textEditor.findColour(TextEditor::focusedOutlineColourId));
                g.drawRoundedRectangle(0, 0, width, height, 4.0f, 2.0f);
            }
            else
            {
                g.setColour(textEditor.findColour(TextEditor::outlineColourId));
                g.drawRoundedRectangle(0, 0, width, height, 4.0f, 1.0f);
            }
        }
    }
}

void CyberpunkLookAndFeel::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    updateAnimations();
    
    Rectangle<int> activeArea = button.getActiveArea();
    
    const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();
    
    const Colour bkg(button.getTabBackgroundColour());
    
    if (button.isFrontTab())
    {
        float glowIntensity = 0.5f + 0.3f * std::sin(glowAnimationPhase);
        drawGlowEffect(g, activeArea.toFloat(), CYBER_GLOW, glowIntensity);
        
        g.setColour(CYBER_DARK_YELLOW);
        g.fillRect(activeArea);
        
        g.setColour(CYBER_BRIGHT_YELLOW);
        
        if (o != TabbedButtonBar::TabsAtBottom)   g.fillRect(activeArea.removeFromTop(2));
        if (o != TabbedButtonBar::TabsAtTop)      g.fillRect(activeArea.removeFromBottom(2));
        if (o != TabbedButtonBar::TabsAtRight)    g.fillRect(activeArea.removeFromLeft(2));
        if (o != TabbedButtonBar::TabsAtLeft)     g.fillRect(activeArea.removeFromRight(2));
    }
    else
    {
        Point<int> p1, p2;
        
        switch (o)
        {
            case TabbedButtonBar::TabsAtBottom:   p1 = activeArea.getBottomLeft();  p2 = activeArea.getTopLeft();    break;
            case TabbedButtonBar::TabsAtTop:      p1 = activeArea.getTopLeft();     p2 = activeArea.getBottomLeft(); break;
            case TabbedButtonBar::TabsAtRight:    p1 = activeArea.getTopRight();    p2 = activeArea.getTopLeft();    break;
            case TabbedButtonBar::TabsAtLeft:     p1 = activeArea.getTopLeft();     p2 = activeArea.getTopRight();   break;
            default:                              jassertfalse; break;
        }
        
        g.setColour(CYBER_MID_GREY);
        g.fillRect(activeArea);
        
        if (isMouseOver || isMouseDown)
        {
            g.setColour(CYBER_DARK_YELLOW.withAlpha(0.6f));
            g.fillRect(activeArea);
        }
        
        g.setColour(CYBER_YELLOW);
        g.drawLine(p1.x, p1.y, p2.x, p2.y);
    }
}

void CyberpunkLookAndFeel::fillTabButtonShape(TabBarButton& button, Graphics& g, const Path& path,
    bool isMouseOver, bool isMouseDown)
{
    const Colour tabBackground(button.getTabBackgroundColour());
    
    g.setColour(isMouseDown ? tabBackground.contrasting(0.9f)
                            : tabBackground.contrasting(0.1f));
    g.fillPath(path);
}

void CyberpunkLookAndFeel::drawTabButtonText(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    const Rectangle<float> area = button.getTextArea().toFloat();
    
    float length = area.getWidth();
    float depth = area.getHeight();
    
    if (button.getTabbedButtonBar().isVertical())
        std::swap(length, depth);
    
    Font font = getCyberFont(jmin(15.0f, depth * 0.6f));
    font.setUnderline(button.hasKeyboardFocus(false));
    
    AffineTransform t;
    
    switch (button.getTabbedButtonBar().getOrientation())
    {
        case TabbedButtonBar::TabsAtLeft:   t = t.rotated(MathConstants<float>::pi * -0.5f).translated(area.getX(), area.getBottom()); break;
        case TabbedButtonBar::TabsAtRight:  t = t.rotated(MathConstants<float>::pi *  0.5f).translated(area.getRight(), area.getY()); break;
        case TabbedButtonBar::TabsAtTop:
        case TabbedButtonBar::TabsAtBottom: t = t.translated(area.getX(), area.getY()); break;
        default:                            jassertfalse; break;
    }
    
    Colour col = button.isFrontTab() ? CYBER_BLACK : CYBER_YELLOW;
    
    if (isMouseOver && !button.isFrontTab())
        col = CYBER_BRIGHT_YELLOW;
    
    g.setColour(col);
    g.setFont(font);
    g.addTransform(t);
    
    g.drawFittedText(button.getButtonText().trim(),
                    0, 0, (int) length, (int) depth,
                    Justification::centred,
                    jmax(1, ((int) depth) / 12));
}

void CyberpunkLookAndFeel::drawTabbedButtonBarBackground(TabbedButtonBar& bar, Graphics& g)
{
    Rectangle<int> r = bar.getLocalBounds();
    
    g.setColour(CYBER_BLACK);
    g.fillRect(r);
    
    drawCyberGrid(g, r, 20.0f);
}

void CyberpunkLookAndFeel::drawTabAreaBehindFrontButton(TabbedButtonBar& bar, Graphics& g, int w, int h)
{
    const TabbedButtonBar::Orientation o = bar.getOrientation();
    
    switch (o)
    {
        case TabbedButtonBar::TabsAtTop:
        case TabbedButtonBar::TabsAtBottom:
            g.setColour(CYBER_YELLOW);
            g.fillRect(0, (o == TabbedButtonBar::TabsAtTop) ? h - 2 : 0, w, 2);
            break;
            
        case TabbedButtonBar::TabsAtLeft:
        case TabbedButtonBar::TabsAtRight:
            g.setColour(CYBER_YELLOW);
            g.fillRect((o == TabbedButtonBar::TabsAtLeft) ? w - 2 : 0, 0, 2, h);
            break;
            
        default:
            jassertfalse;
            break;
    }
}

void CyberpunkLookAndFeel::drawScrollbarButton(Graphics& g, ScrollBar& scrollbar, int width, int height,
    int buttonDirection, bool isScrollbarVertical,
    bool isMouseOverButton, bool isButtonDown)
{
    Rectangle<float> area(0, 0, (float)width, (float)height);
    
    if (isMouseOverButton)
        drawGlowEffect(g, area, CYBER_GLOW, 0.4f);
    
    g.setColour(isButtonDown ? CYBER_DARK_YELLOW : (isMouseOverButton ? CYBER_MID_GREY : CYBER_DARK_GREY));
    g.fillRect(area);
    
    g.setColour(CYBER_YELLOW);
    g.drawRect(area, 1.0f);
    
    // Draw simple arrow instead of using deprecated function
    Path arrow;
    float arrowSize = jmin(width, height) * 0.3f;
    float centerX = width * 0.5f;
    float centerY = height * 0.5f;
    
    if (isScrollbarVertical)
    {
        if (buttonDirection == 0) // Up
        {
            arrow.addTriangle(centerX - arrowSize, centerY + arrowSize * 0.5f,
                             centerX + arrowSize, centerY + arrowSize * 0.5f,
                             centerX, centerY - arrowSize * 0.5f);
        }
        else // Down
        {
            arrow.addTriangle(centerX - arrowSize, centerY - arrowSize * 0.5f,
                             centerX + arrowSize, centerY - arrowSize * 0.5f,
                             centerX, centerY + arrowSize * 0.5f);
        }
    }
    else
    {
        if (buttonDirection == 2) // Left
        {
            arrow.addTriangle(centerX + arrowSize * 0.5f, centerY - arrowSize,
                             centerX + arrowSize * 0.5f, centerY + arrowSize,
                             centerX - arrowSize * 0.5f, centerY);
        }
        else // Right
        {
            arrow.addTriangle(centerX - arrowSize * 0.5f, centerY - arrowSize,
                             centerX - arrowSize * 0.5f, centerY + arrowSize,
                             centerX + arrowSize * 0.5f, centerY);
        }
    }
    
    g.setColour(CYBER_BRIGHT_YELLOW);
    g.fillPath(arrow);
}

void CyberpunkLookAndFeel::drawScrollbar(Graphics& g, ScrollBar& scrollbar, int x, int y, int width, int height,
    bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
    bool isMouseOver, bool isMouseDown)
{
    Rectangle<int> thumbBounds;
    
    if (isScrollbarVertical)
        thumbBounds = Rectangle<int>(x, thumbStartPosition, width, thumbSize);
    else
        thumbBounds = Rectangle<int>(thumbStartPosition, y, thumbSize, height);
    
    auto scrollbarBounds = Rectangle<int>(x, y, width, height);
    
    g.setColour(scrollbar.findColour(ScrollBar::backgroundColourId));
    g.fillRect(scrollbarBounds);
    
    g.setColour(CYBER_YELLOW);
    g.drawRect(scrollbarBounds, 1);
    
    if (thumbSize > 0)
    {
        if (isMouseOver || isMouseDown)
            drawGlowEffect(g, thumbBounds.toFloat(), CYBER_GLOW, 0.5f);
        
        auto thumbCol = scrollbar.findColour(ScrollBar::thumbColourId);
        
        if (isMouseOver || isMouseDown)
            thumbCol = thumbCol.brighter();
        
        g.setColour(thumbCol);
        g.fillRect(thumbBounds);
        
        g.setColour(CYBER_YELLOW);
        g.drawRect(thumbBounds, 1);
        
        // Draw grip lines
        if (thumbSize > 16)
        {
            g.setColour(CYBER_BRIGHT_YELLOW);
            auto centre = thumbBounds.getCentre();
            auto gripSize = jmin(thumbSize, isScrollbarVertical ? width : height) / 4;
            
            if (isScrollbarVertical)
            {
                for (int i = -1; i <= 1; ++i)
                    g.drawLine(centre.x - gripSize, centre.y + i * 3,
                              centre.x + gripSize, centre.y + i * 3, 1.0f);
            }
            else
            {
                for (int i = -1; i <= 1; ++i)
                    g.drawLine(centre.x + i * 3, centre.y - gripSize,
                              centre.x + i * 3, centre.y + gripSize, 1.0f);
            }
        }
    }
}

void CyberpunkLookAndFeel::drawCornerResizer(Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging)
{
    g.setColour(isMouseDragging ? CYBER_BRIGHT_YELLOW : (isMouseOver ? CYBER_YELLOW : CYBER_DARK_YELLOW));
    
    const float lineThickness = 1.0f;
    const float spacing = 4.0f;
    
    for (float i = spacing; i < w + h; i += spacing)
    {
        g.drawLine(w - i, h + 1.0f, w + 1.0f, h - i, lineThickness);
    }
}

void CyberpunkLookAndFeel::drawGlowEffect(Graphics& g, const Rectangle<float>& area, const Colour& glowColor, float intensity)
{
    if (intensity <= 0.0f) return;
    
    const int numGlowLayers = 4;
    const float maxRadius = 8.0f;
    
    for (int i = 0; i < numGlowLayers; ++i)
    {
        float alpha = intensity * (1.0f - (float)i / numGlowLayers) * 0.3f;
        float radius = maxRadius * (float)(i + 1) / numGlowLayers;
        
        g.setColour(glowColor.withAlpha(alpha));
        g.fillRoundedRectangle(area.expanded(radius), radius * 0.5f);
    }
}

void CyberpunkLookAndFeel::drawCyberGrid(Graphics& g, const Rectangle<int>& area, float gridSize)
{
    g.setColour(CYBER_DARK_YELLOW.withAlpha(0.1f));
    
    for (float x = area.getX(); x < area.getRight(); x += gridSize)
        g.drawVerticalLine(roundToInt(x), area.getY(), area.getBottom());
    
    for (float y = area.getY(); y < area.getBottom(); y += gridSize)
        g.drawHorizontalLine(roundToInt(y), area.getX(), area.getRight());
}

void CyberpunkLookAndFeel::drawNeonBorder(Graphics& g, const Rectangle<float>& area, const Colour& neonColor, float thickness)
{
    g.setColour(neonColor.withAlpha(0.8f));
    g.drawRoundedRectangle(area, 3.0f, thickness);
    
    g.setColour(neonColor.withAlpha(0.3f));
    g.drawRoundedRectangle(area.expanded(thickness), 3.0f + thickness, 1.0f);
}

void CyberpunkLookAndFeel::drawHexagonalPattern(Graphics& g, const Rectangle<int>& area, float hexSize)
{
    g.setColour(CYBER_DARK_YELLOW.withAlpha(0.08f));
    
    const float hexWidth = hexSize * 2.0f;
    const float hexHeight = hexSize * std::sqrt(3.0f);
    const float xOffset = hexWidth * 0.75f;
    
    for (float y = area.getY() - hexHeight; y < area.getBottom() + hexHeight; y += hexHeight)
    {
        for (float x = area.getX() - hexWidth; x < area.getRight() + hexWidth; x += xOffset)
        {
            float offsetY = y + ((int)(x / xOffset) % 2) * (hexHeight * 0.5f);
            
            Path hexagon;
            for (int i = 0; i < 6; ++i)
            {
                float angle = i * MathConstants<float>::pi / 3.0f;
                float hx = x + hexSize * std::cos(angle);
                float hy = offsetY + hexSize * std::sin(angle);
                
                if (i == 0)
                    hexagon.startNewSubPath(hx, hy);
                else
                    hexagon.lineTo(hx, hy);
            }
            hexagon.closeSubPath();
            
            g.strokePath(hexagon, PathStrokeType(1.0f));
        }
    }
}