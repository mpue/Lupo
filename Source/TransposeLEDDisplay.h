#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/**  Five-LED transpose indicator.
 *   LEDs 0-1 light red/orange for negative values,
 *   LED 2 (center) lights green at zero,
 *   LEDs 3-4 light cyan for positive values.
 */
class TransposeLEDDisplay : public Component
{
public:
    TransposeLEDDisplay() {}

    void setValue(int semitones)
    {
        value = juce::jlimit(-12, 12, semitones);
        repaint();
    }

    int getValue() const { return value; }

    void paint(Graphics& g) override
    {
        constexpr int n          = 5;
        constexpr int ledDiam    = 12;
        constexpr int gap        = 6;
        constexpr int totalWidth = n * ledDiam + (n - 1) * gap;

        const int startX = (getWidth()  - totalWidth) / 2;
        const int ledY   = (getHeight() - ledDiam)    / 2;

        for (int i = 0; i < n; ++i)
        {
            const int ledX = startX + i * (ledDiam + gap);
            const bool lit = isLedLit(i);

            // Pick colour based on position
            Colour col;
            if (i == 2)
                col = lit ? Colour(0xff00ff55) : Colour(0xff002810);   // green
            else if (i < 2)
                col = lit ? Colour(0xffff5500) : Colour(0xff2a0800);   // orange/red
            else
                col = lit ? Colour(0xff00aaff) : Colour(0xff001828);   // cyan/blue

            // Body
            g.setColour(col);
            g.fillEllipse((float)ledX, (float)ledY, (float)ledDiam, (float)ledDiam);

            // Gloss highlight when lit
            if (lit)
            {
                g.setColour(Colour(0x66ffffff));
                g.fillEllipse((float)ledX + 2.5f, (float)ledY + 1.5f,
                              (float)ledDiam * 0.35f, (float)ledDiam * 0.35f);
            }

            // Bezel
            g.setColour(Colour(0xff555555));
            g.drawEllipse((float)ledX + 0.5f, (float)ledY + 0.5f,
                          (float)ledDiam - 1.0f, (float)ledDiam - 1.0f, 0.75f);
        }

        // Small numeric readout on the right
        String text = (value > 0 ? "+" : "") + String(value);
        g.setColour(Colour(0xffaaaaaa));
        g.setFont(Font(10.0f));
        g.drawText(text,
                   startX + totalWidth + 6, ledY - 1,
                   28, ledDiam + 2,
                   Justification::centredLeft, false);
    }

private:
    int value = 0;

    bool isLedLit(int i) const
    {
        switch (i)
        {
            case 0: return value <= -7;
            case 1: return value >= -6 && value <= -1;
            case 2: return value == 0;
            case 3: return value >= 1  && value <= 6;
            case 4: return value >= 7;
            default: return false;
        }
    }
};
