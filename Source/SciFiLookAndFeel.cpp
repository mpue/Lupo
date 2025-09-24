/*
  ==============================================================================

    SciFiLookAndFeel.cpp
    Created: Sci-Fi themed LookAndFeel based on TrioLookAndFeel
    Author:  Modified from Matthias Pueski's original

  ==============================================================================
*/

#include "SciFiLookAndFeel.h"

SciFiLookAndFeel::SciFiLookAndFeel() {
    // Sci-Fi Color Scheme
    setColour(ResizableWindow::backgroundColourId, Colour(0xbbbbbbbb));
    setColour(DocumentWindow::backgroundColourId,  Colour(0xbbbbbbbb));

    // Neon accents
    setColour(Slider::thumbColourId, Colour(0xff00ffff));
    setColour(Slider::trackColourId, Colour(0xff1a1a2e));
    setColour(Slider::backgroundColourId, Colour(0xff0f0f1a));

    setColour(TextButton::buttonColourId, Colour(0xff16213e));
    setColour(TextButton::textColourOffId, Colour(0xff00d4ff));
    setColour(TextButton::textColourOnId, Colour(0xffffffff));

    setColour(ComboBox::backgroundColourId, Colour(0xff16213e));
    setColour(ComboBox::textColourId, Colour(0xff00d4ff));
    setColour(ComboBox::arrowColourId, Colour(0xff00ffff));
}

SciFiLookAndFeel::~SciFiLookAndFeel() {

}

void SciFiLookAndFeel::drawRotarySlider(Graphics& g,
    int x, int y,
    int width, int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    Slider& slider)
{
    // Keep the original knob graphics implementation
    Image myStrip = ImageCache::getFromMemory(BinaryData::Knob_64_png, BinaryData::Knob_64_pngSize);
    Image dropShadow = ImageCache::getFromMemory(BinaryData::knob_shadow_64_png, BinaryData::knob_shadow_64_pngSize);

    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    const float radius = jmin(width / 2.0f, height / 2.0f);
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius - 1.0f;
    const float ry = centreY - radius - 1.0f;
    const float rw = radius * 2.0f;
    const float thickness = 0.9f;

    const double fractRotation = (slider.getValue() - slider.getMinimum()) /
        (slider.getMaximum() - slider.getMinimum());

    const int nFrames = myStrip.getHeight() / myStrip.getWidth();
    const int frameIdx = (int)ceil(sliderPosProportional * ((double)nFrames - 1.0));

    // Sci-Fi arc with glow effect
    {
        // Outer glow
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        Path glowArc;
        glowArc.addPieSegment(rx - 2, ry - 2, rw + 4, rw + 4, rotaryStartAngle, angle, thickness);
        g.fillPath(glowArc);

        // Inner bright arc
        g.setColour(Colour(0xff00ffff));
        Path filledArc;
        filledArc.addPieSegment(rx + 1, ry + 1, rw - 0.5, rw - 0.5, rotaryStartAngle, angle, thickness);
        g.fillPath(filledArc);

        // Inner core glow
        g.setColour(Colour(0xffffffff).withAlpha(0.8f));
        Path coreArc;
        coreArc.addPieSegment(rx + 2, ry + 2, rw - 4, rw - 4, rotaryStartAngle, angle, thickness);
        g.fillPath(coreArc);
    }

    // Enhanced shadow with blue tint
    g.setColour(Colour(0xff001133).withAlpha(0.8f));
    g.drawImage(dropShadow, 0, 0, slider.getWidth(), slider.getWidth(),
        0, 0, dropShadow.getWidth(), dropShadow.getHeight());

    // Original knob image
    g.drawImage(myStrip,
        (int)rx, (int)ry,
        2 * (int)radius, 2 * (int)radius,   // Dest
        0, frameIdx * myStrip.getWidth(),
        myStrip.getWidth(), myStrip.getWidth()); // Source

    // Add subtle outer ring glow
    g.setColour(Colour(0xff00ffff).withAlpha(0.2f));
    g.drawEllipse(rx - 3, ry - 3, rw + 6, rw + 6, 2.0f);
}

void SciFiLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    auto size = jmin(bounds.getWidth(), bounds.getHeight());
    auto buttonArea = Rectangle<float>(0, 0, size, size);

    // Sci-fi toggle with hexagonal shape
    Path hexagon;
    auto centre = buttonArea.getCentre();
    auto radius = size * 0.4f;

    for (int i = 0; i < 6; ++i) {
        auto angle = i * MathConstants<float>::pi / 3.0f;
        auto x = centre.x + radius * std::cos(angle);
        auto y = centre.y + radius * std::sin(angle);

        if (i == 0)
            hexagon.startNewSubPath(x, y);
        else
            hexagon.lineTo(x, y);
    }
    hexagon.closeSubPath();

    if (button.getToggleState()) {
        // Active state - glowing cyan
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        g.fillPath(hexagon, AffineTransform::scale(1.1f, 1.1f, centre.x, centre.y));

        g.setColour(Colour(0xff00ffff));
        g.fillPath(hexagon);

        g.setColour(Colour(0xffffffff));
        g.strokePath(hexagon, PathStrokeType(1.5f));
    }
    else {
        // Inactive state - dark with subtle outline
        g.setColour(Colour(0xff1a1a2e));
        g.fillPath(hexagon);

        g.setColour(Colour(0xff00d4ff).withAlpha(0.5f));
        g.strokePath(hexagon, PathStrokeType(1.0f));
    }

    // Text
    float fontSize = jmin(15.0f, button.getHeight() * 0.6f);
    g.setColour(button.getToggleState() ? Colour(0xff000000) : Colour(0xff00d4ff));
    g.setFont(Font("Arial", "bold", fontSize));

    const int textX = button.getWidth() / 2 + 5;
    g.drawFittedText(button.getButtonText(),
        textX, 0,
        button.getWidth() - textX - 2, button.getHeight(),
        Justification::centredLeft, 10);
}

Font SciFiLookAndFeel::getPopupMenuFont()
{
    return Font("Arial", "regular", 13.0f);
}

void SciFiLookAndFeel::drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
    const bool isSeparator, const bool isActive,
    const bool isHighlighted, const bool isTicked,
    const bool hasSubMenu, const String& text,
    const String& shortcutKeyText,
    const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator) {
        Rectangle<int> r(area.reduced(5, 0));
        r.removeFromTop(r.getHeight() / 2 - 1);

        // Sci-fi separator with glow
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        g.fillRect(r.removeFromTop(2));

        g.setColour(Colour(0xff00ffff));
        g.fillRect(r.removeFromTop(1));
    }
    else {
        Colour textColour(Colour(0xff00d4ff));

        if (textColourToUse != nullptr)
            textColour = *textColourToUse;

        Rectangle<int> r(area.reduced(1));

        if (isHighlighted) {
            // Holographic highlight effect
            g.setGradientFill(ColourGradient(Colour(0xff00ffff).withAlpha(0.2f), 0, r.getY(),
                Colour(0xff0066ff).withAlpha(0.1f), 0, r.getBottom(), false));
            g.fillRect(r);

            // Subtle border
            g.setColour(Colour(0xff00ffff).withAlpha(0.5f));
            g.drawRect(r, 1);

            g.setColour(Colour(0xffffffff));
        }
        else {
            g.setColour(textColour);
        }

        if (!isActive)
            g.setOpacity(0.3f);

        Font font(getPopupMenuFont());
        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight(maxFontHeight);

        g.setFont(font);

        Rectangle<float> iconArea(r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).toFloat());

        if (icon != nullptr) {
            icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }
        else if (isTicked) {
            const Path tick(getTickShape(1.0f));
            g.setColour(Colour(0xff00ffff));
            g.fillPath(tick, tick.getTransformToScaleToFit(iconArea, true));
        }

        if (hasSubMenu) {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();
            const float x = (float)r.removeFromRight((int)arrowH).getX();
            const float halfH = (float)r.getCentreY();

            Path p;
            p.addTriangle(x, halfH - arrowH * 0.5f,
                x, halfH + arrowH * 0.5f,
                x + arrowH * 0.6f, halfH);

            g.setColour(Colour(0xff00ffff));
            g.fillPath(p);
        }

        r.removeFromRight(3);
        g.drawFittedText(text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty()) {
            Font f2(font);
            f2.setHeight(f2.getHeight() * 0.75f);
            f2.setHorizontalScale(0.95f);
            g.setFont(f2);

            g.setColour(Colour(0xff00d4ff).withAlpha(0.7f));
            g.drawText(shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

void SciFiLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height)
{
    // Dark space background with subtle grid
    g.fillAll(Colour(0xff0a0a12));

    // Subtle grid pattern
    g.setColour(Colour(0xff001133).withAlpha(0.3f));
    for (int x = 0; x < width; x += 20) {
        g.drawVerticalLine(x, 0, height);
    }
    for (int y = 0; y < height; y += 20) {
        g.drawHorizontalLine(y, 0, width);
    }

    // Glowing border
    g.setColour(Colour(0xff00ffff).withAlpha(0.5f));
    g.drawRect(0, 0, width, height, 1);

    // Inner glow
    g.setColour(Colour(0xff00ffff).withAlpha(0.1f));
    g.drawRect(1, 1, width - 2, height - 2, 1);
}

void SciFiLookAndFeel::drawLabel(Graphics& g, Label& label)
{
    g.fillAll(Colour(0xff16213e).withAlpha(0.8f));

    if (!label.isBeingEdited()) {
        const float alpha = label.isEnabled() ? 1.0f : 0.5f;
        const Font font(getLabelFont(label));

        // Holographic text effect
        g.setColour(Colour(0xff00d4ff).withMultipliedAlpha(alpha));
        g.setFont(Font("Arial", "regular", 12.0f));

        Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()).expanded(3, 1));

        // Text shadow for glow effect
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        g.drawFittedText(label.getText(), textArea.translated(1, 1), label.getJustificationType(),
            jmax(1, (int)(textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        // Main text
        g.setColour(Colour(0xff00d4ff).withMultipliedAlpha(alpha));
        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
            jmax(1, (int)(textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        g.setColour(Colour(0xff00ffff).withMultipliedAlpha(alpha * 0.5f));
    }
    else if (label.isEnabled()) {
        g.setColour(Colour(0xff00ffff));
    }

    // Holographic border
    g.setColour(Colour(0xff00ffff).withAlpha(0.6f));
    g.drawRect(label.getLocalBounds(), 1);
}

void SciFiLookAndFeel::drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
    int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
{
    // Dark background with subtle glow
    g.fillAll(Colour(0xff16213e));

    // Holographic border
    if (box.isEnabled() && box.hasKeyboardFocus(false)) {
        g.setColour(Colour(0xff00ffff));
        g.drawRect(0, 0, width, height, 2);

        // Glow effect
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        g.drawRect(-1, -1, width + 2, height + 2, 1);
    }
    else {
        g.setColour(Colour(0xff00d4ff).withAlpha(0.7f));
        g.drawRect(0, 0, width, height, 1);
    }

    const float outlineThickness = box.isEnabled() ? (isButtonDown ? 1.2f : 0.5f) : 0.3f;
    const Colour baseColour = Colour(0xff1a1a2e);

    // Button area with sci-fi styling
    Rectangle<float> buttonArea(buttonX + outlineThickness, buttonY + outlineThickness,
        buttonW - outlineThickness * 2.0f, buttonH - outlineThickness * 2.0f);

    g.setGradientFill(ColourGradient(baseColour.brighter(0.1f), 0, buttonArea.getY(),
        baseColour.darker(0.1f), 0, buttonArea.getBottom(), false));
    g.fillRoundedRectangle(buttonArea, 2.0f);

    if (box.isEnabled()) {
        // Futuristic arrow design
        const float arrowX = 0.3f;
        const float arrowH = 0.15f;

        Path p;
        // Double arrow for sci-fi look
        p.addTriangle(buttonX + buttonW * 0.5f, buttonY + buttonH * (0.4f - arrowH),
            buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.4f,
            buttonX + buttonW * arrowX, buttonY + buttonH * 0.4f);

        p.addTriangle(buttonX + buttonW * 0.5f, buttonY + buttonH * (0.6f + arrowH),
            buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.6f,
            buttonX + buttonW * arrowX, buttonY + buttonH * 0.6f);

        g.setColour(Colour(0xff00ffff));
        g.fillPath(p);

        // Add glow to arrows
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        g.fillPath(p, AffineTransform::scale(1.2f, 1.2f, buttonX + buttonW * 0.5f, buttonY + buttonH * 0.5f));
    }
}

void SciFiLookAndFeel::drawGroupComponentOutline(Graphics& g, int width, int height,
    const String& text, const Justification& position,
    GroupComponent& group)
{
    const float textH = 15.0f;
    const float indent = 3.0f;
    const float textEdgeGap = 4.0f;
    auto cs = 2.0f;

    Font f(textH);

    Path p;
    auto x = indent;
    auto y = f.getAscent() + 0.0f;
    auto w = jmax(0.0f, width - x * 2.0f);
    auto h = jmax(0.0f, height - y - indent);
    cs = jmin(cs, w * 0.5f, h * 0.5f);
    auto cs2 = 2.0f * cs;

    auto textW = text.isEmpty() ? 0 : jlimit(0.0f, jmax(0.0f, w - cs2 - textEdgeGap * 2), f.getStringWidth(text) + textEdgeGap * 2.0f);
    auto textX = cs + textEdgeGap;

    if (position.testFlags(Justification::horizontallyCentred))
        textX = cs + (w - cs2 - textW) * 0.5f;
    else if (position.testFlags(Justification::right))
        textX = w - cs - textW - textEdgeGap;

    p.startNewSubPath(x + textX + textW, y);
    p.lineTo(x + w - cs, y);
    p.addArc(x + w - cs2, y, cs2, cs2, 0, MathConstants<float>::halfPi);
    p.lineTo(x + w, y + h - cs);
    p.addArc(x + w - cs2, y + h - cs2, cs2, cs2, MathConstants<float>::halfPi, MathConstants<float>::pi);
    p.lineTo(x + cs, y + h);
    p.addArc(x, y + h - cs2, cs2, cs2, MathConstants<float>::pi, MathConstants<float>::pi * 1.5f);
    p.lineTo(x, y + cs);
    p.addArc(x, y, cs2, cs2, MathConstants<float>::pi * 1.5f, MathConstants<float>::twoPi);
    p.lineTo(x + textX, y);

    auto alpha = group.isEnabled() ? 1.0f : 0.5f;

    // Dark background with transparency
    g.setColour(Colour(0xff1a1a2e).withMultipliedAlpha(alpha * 0.8f));
    g.fillPath(p);

    // Glowing outline
    g.setColour(Colour(0xff00ffff).withMultipliedAlpha(alpha * 0.7f));
    g.strokePath(p, PathStrokeType(1.0f));

    // Additional inner glow
    g.setColour(Colour(0xff00ffff).withMultipliedAlpha(alpha * 0.2f));
    g.strokePath(p, PathStrokeType(3.0f));

    // Text background
    g.setColour(Colour(0xff0a0a12).withMultipliedAlpha(alpha));
    g.fillRect(roundToInt(x + textX), 3, roundToInt(textW), roundToInt(textH + 2));

    // Text border
    g.setColour(Colour(0xff00ffff).withMultipliedAlpha(alpha * 0.5f));
    g.drawRect(roundToInt(x + textX), 3, roundToInt(textW), roundToInt(textH + 2));

    // Holographic text
    g.setColour(Colour(0xff00d4ff).withMultipliedAlpha(alpha));
    g.setFont(f);
    g.drawText(text,
        roundToInt(x + textX), 3,
        roundToInt(textW), roundToInt(textH),
        Justification::centred, true);
}

// Continue with the rest of the methods...
void SciFiLookAndFeel::drawScrollbarButton(Graphics& g, ScrollBar& bar,
    int width, int height, int buttonDirection,
    bool isScrollbarVertical,
    bool isMouseOverButton,
    bool isButtonDown)
{
    // Sci-fi scrollbar buttons with holographic styling
    g.fillAll(Colour(0xff1a1a2e));

    if (isScrollbarVertical)
        width -= 2;
    else
        height -= 2;

    Path p;

    if (buttonDirection == 0)
        p.addTriangle(width * 0.5f, height * 0.2f,
            width * 0.1f, height * 0.7f,
            width * 0.9f, height * 0.7f);
    else if (buttonDirection == 1)
        p.addTriangle(width * 0.8f, height * 0.5f,
            width * 0.3f, height * 0.1f,
            width * 0.3f, height * 0.9f);
    else if (buttonDirection == 2)
        p.addTriangle(width * 0.5f, height * 0.8f,
            width * 0.1f, height * 0.3f,
            width * 0.9f, height * 0.3f);
    else if (buttonDirection == 3)
        p.addTriangle(width * 0.2f, height * 0.5f,
            width * 0.7f, height * 0.1f,
            width * 0.7f, height * 0.9f);

    if (isButtonDown) {
        g.setColour(Colour(0xff00ffff));
    }
    else if (isMouseOverButton) {
        g.setColour(Colour(0xff00d4ff).withAlpha(0.8f));
    }
    else {
        g.setColour(Colour(0xff00d4ff).withAlpha(0.5f));
    }

    g.fillPath(p);

    // Glow effect
    g.setColour(Colour(0xff00ffff).withAlpha(0.2f));
    g.strokePath(p, PathStrokeType(1.0f));
}

void SciFiLookAndFeel::drawScrollbar(Graphics& g, ScrollBar& bar,
    int x, int y, int width, int height,
    bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
    bool isMouseOver, bool isMouseDown)
{
    // Space-age scrollbar track
    g.fillAll(Colour(0xff0a0a12));

    g.setColour(Colour(0xff1a1a2e).withAlpha((isMouseOver || isMouseDown) ? 0.6f : 0.3f));

    if (thumbSize > 0.0f) {
        Rectangle<int> thumb;

        if (isScrollbarVertical) {
            width -= 2;
            g.fillRect(x + roundToInt(width * 0.3f), y, roundToInt(width * 0.4f), height);

            thumb.setBounds(x + 1, thumbStartPosition, width - 2, thumbSize);
        }
        else {
            height -= 2;
            g.fillRect(x, y + roundToInt(height * 0.3f), width, roundToInt(height * 0.4f));

            thumb.setBounds(thumbStartPosition, y + 1, thumbSize, height - 2);
        }

        // Holographic thumb
        g.setGradientFill(ColourGradient(Colour(0xff00ffff).withAlpha(0.8f), thumb.getX(), thumb.getY(),
            Colour(0xff0066ff).withAlpha(0.4f), thumb.getRight(), thumb.getBottom(), false));
        g.fillRect(thumb);

        // Thumb glow
        g.setColour(Colour(0xff00ffff).withAlpha(0.3f));
        g.drawRect(thumb.expanded(1), 1);

        // Inner highlight
        g.setColour(Colour(0xffffffff).withAlpha(0.2f));
        g.drawRect(thumb.reduced(1), 1);
    }
}

void SciFiLookAndFeel::drawShinyButtonShape(Graphics& g,
    float x, float y, float w, float h,
    float maxCornerSize,
    const Colour& baseColour,
    const float strokeWidth,
    const bool flatOnLeft,
    const bool flatOnRight,
    const bool flatOnTop,
    const bool flatOnBottom)
{
    if (w <= strokeWidth * 1.1f || h <= strokeWidth * 1.1f)
        return;

    const float cs = jmin(maxCornerSize, w * 0.5f, h * 0.5f);

    Path outline;
    outline.addRoundedRectangle(x, y, w, h, cs, cs,
        !(flatOnLeft || flatOnTop),
        !(flatOnRight || flatOnTop),
        !(flatOnLeft || flatOnBottom),
        !(flatOnRight || flatOnBottom));

    // Sci-fi gradient with cyan accent
    ColourGradient cg(baseColour.overlaidWith(Colour(0x1100ffff)), 0.0f, y,
        baseColour.darker(0.3f), 0.0f, y + h, false);

    cg.addColour(0.3, baseColour.overlaidWith(Colour(0x3300ffff)));
    cg.addColour(0.7, baseColour.darker(0.1f));

    g.setGradientFill(cg);
    g.fillPath(outline);

    // Holographic border
    g.setColour(Colour(0xff00ffff).withAlpha(0.7f));
    g.strokePath(outline, PathStrokeType(strokeWidth));

    // Inner glow
    g.setColour(Colour(0xff00ffff).withAlpha(0.2f));
    g.strokePath(outline, PathStrokeType(strokeWidth * 2.0f));
}

void SciFiLookAndFeel::drawTableHeaderBackground(Graphics& g, TableHeaderComponent& header)
{
    g.fillAll(Colour(0xff16213e));

    Rectangle<int> area(header.getLocalBounds());

    // Holographic header gradient
    g.setGradientFill(ColourGradient(Colour(0xff1a1a2e), 0.0f, (float)area.getY(),
        Colour(0xff0a0a12), 0.0f, (float)area.getBottom(), false));
    g.fillRect(area);

    // Glowing bottom line
    g.setColour(Colour(0xff00ffff).withAlpha(0.6f));
    g.fillRect(area.removeFromBottom(2));

    // Column separators with glow
    g.setColour(Colour(0xff00d4ff).withAlpha(0.4f));
    for (int i = header.getNumColumns(true); --i >= 0;)
        g.fillRect(header.getColumnPosition(i).removeFromRight(1));
}

void SciFiLookAndFeel::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    Path tabShape;
    createTabButtonShape(button, tabShape, isMouseOver, isMouseDown);

    auto activeArea = button.getActiveArea();
    tabShape.applyTransform(AffineTransform::translation((float)activeArea.getX(),
        (float)activeArea.getY()));

    fillTabButtonShape(button, g, tabShape, isMouseOver, isMouseDown);
    drawTabButtonText(button, g, isMouseOver, isMouseDown);
}

void SciFiLookAndFeel::fillTabButtonShape(TabBarButton& button, Graphics& g, const Path& path,
    bool isMouseOver, bool isMouseDown)
{
    const bool isFrontTab = button.isFrontTab();

    if (isFrontTab) {
        // Active tab - bright holographic
        g.setGradientFill(ColourGradient(Colour(0xff00d4ff).withAlpha(0.8f), 0, path.getBounds().getY(),
            Colour(0xff1a1a2e), 0, path.getBounds().getBottom(), false));
        g.fillPath(path);

        // Glow outline
        g.setColour(Colour(0xff00ffff));
        g.strokePath(path, PathStrokeType(1.5f));
    }
    else {
        // Inactive tab - subtle
        g.setColour(Colour(0xff16213e).withAlpha(isMouseOver ? 0.8f : 0.6f));
        g.fillPath(path);

        g.setColour(Colour(0xff00d4ff).withAlpha(0.5f));
        g.strokePath(path, PathStrokeType(0.5f));
    }
}