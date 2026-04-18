#include "EQPanel.h"
#include "AttachmentFactory.h"
#include "Model.h"

static const Colour kBandColours[8] = {
    Colour (0xffff6b6b),  // coral
    Colour (0xffffd93d),  // yellow
    Colour (0xff6bcb77),  // green
    Colour (0xff4d96ff),  // blue
    Colour (0xffff9f43),  // orange
    Colour (0xffb983ff),  // purple
    Colour (0xff48dbfb),  // teal
    Colour (0xffff6fb7),  // pink
};

//==============================================================================
// FrequencyResponseDisplay
//==============================================================================

FrequencyResponseDisplay::FrequencyResponseDisplay (ParametricEQ* eq)
    : eq (eq)
{
    setInterceptsMouseClicks (true, false);
}

void FrequencyResponseDisplay::setFreqSliders (std::array<Slider*, 8> sliders) { freqSliders = sliders; }
void FrequencyResponseDisplay::setGainSliders (std::array<Slider*, 8> sliders) { gainSliders = sliders; }
void FrequencyResponseDisplay::setAutomation  (StepSequencer* s, EQAutomationTrack* a) { seq = s; eqAuto = a; }

float FrequencyResponseDisplay::freqToX (float freq) const
{
    const float w = (float) getWidth();
    return std::log10 (freq / 20.0f) / 3.0f * w;
}

float FrequencyResponseDisplay::xToFreq (float x) const
{
    const float w = (float) getWidth();
    return 20.0f * std::pow (10.0f, 3.0f * x / w);
}

float FrequencyResponseDisplay::gainToY (float gainDb) const
{
    const float h = (float) getHeight();
    return h * (24.0f - gainDb) / 48.0f;
}

float FrequencyResponseDisplay::yToGain (float y) const
{
    const float h = (float) getHeight();
    return 24.0f - 48.0f * y / h;
}

int FrequencyResponseDisplay::findNearestBand (Point<int> pos) const
{
    int   best = -1;
    float bestDist = std::numeric_limits<float>::max();

    for (int i = 0; i < ParametricEQ::NUM_BANDS; ++i)
    {
        const auto& b = eq->getBand (i);
        float bx = freqToX (b.frequency);
        float by = gainToY (b.gainDb);
        float d  = std::sqrt ((bx - pos.x) * (bx - pos.x) + (by - pos.y) * (by - pos.y));
        if (d < bestDist) { bestDist = d; best = i; }
    }
    return best;
}

void FrequencyResponseDisplay::paint (Graphics& g)
{
    const int W = getWidth();
    const int H = getHeight();

    // Background
    g.setColour (Colour (0xff0d1117));
    g.fillRoundedRectangle (getLocalBounds().toFloat(), 6.0f);

    // Grid lines – dB
    g.setFont (Font (9.0f));
    for (int db : { -18, -12, -6, 0, 6, 12, 18 })
    {
        float y = gainToY ((float) db);
        g.setColour (db == 0 ? Colour (0x60ffffff) : Colour (0x25ffffff));
        g.drawHorizontalLine ((int) y, 2.0f, (float) W - 2.0f);
        if (db != 0)
        {
            g.setColour (Colour (0x50ffffff));
            g.drawText (String (db) + "dB", 3, (int) y - 8, 30, 10, Justification::left, false);
        }
    }

    // Grid lines – frequency
    const int freqMarkers[] = { 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
    for (int f : freqMarkers)
    {
        float x = freqToX ((float) f);
        g.setColour (Colour (0x25ffffff));
        g.drawVerticalLine ((int) x, 2.0f, (float) H - 2.0f);
        g.setColour (Colour (0x50ffffff));
        String label = f >= 1000 ? String (f / 1000) + "k" : String (f);
        g.drawText (label, (int) x - 12, H - 14, 24, 12, Justification::centred, false);
    }

    // Frequency response curve
    Path curve;
    bool started = false;
    for (int px = 0; px < W; px += 2)
    {
        float freq = xToFreq ((float) px);
        freq = jlimit (20.0f, 20000.0f, freq);
        float db  = eq->getMagnitudeForFrequency (freq);
        float y   = gainToY (db);
        y = jlimit (0.0f, (float) H, y);

        if (!started) { curve.startNewSubPath ((float) px, y); started = true; }
        else          curve.lineTo ((float) px, y);
    }

    // Filled area under curve
    Path filled = curve;
    filled.lineTo ((float) W, gainToY (0.0f));
    filled.lineTo (0.0f,      gainToY (0.0f));
    filled.closeSubPath();

    g.setColour (Colour (0x224d9eff));
    g.fillPath (filled);

    g.setColour (Colour (0xff4d9eff));
    g.strokePath (curve, PathStrokeType (1.5f));

    // Band nodes
    for (int i = 0; i < ParametricEQ::NUM_BANDS; ++i)
    {
        const auto& b = eq->getBand (i);
        float bx = freqToX (b.frequency);
        float by = gainToY (b.gainDb);
        by = jlimit (0.0f, (float) H, by);

        const float R = (i == dragBand) ? 9.0f : 7.0f;
        g.setColour (kBandColours[i].withAlpha (0.85f));
        g.fillEllipse (bx - R, by - R, R * 2.0f, R * 2.0f);
        g.setColour (kBandColours[i].brighter (0.5f));
        g.drawEllipse (bx - R, by - R, R * 2.0f, R * 2.0f, 1.5f);
        g.setColour (Colours::white.withAlpha (0.9f));
        g.setFont (Font (8.0f, Font::bold));
        g.drawText (String (i + 1), (int) (bx - R), (int) (by - R), (int) (R * 2), (int) (R * 2), Justification::centred, false);
    }
}

void FrequencyResponseDisplay::mouseDown (const MouseEvent& e)
{
    dragBand = findNearestBand (e.getPosition());
    if (dragBand >= 0)
    {
        dragStart      = e.getPosition();
        dragStartFreq  = eq->getBand (dragBand).frequency;
        dragStartGain  = eq->getBand (dragBand).gainDb;
    }
}

void FrequencyResponseDisplay::mouseDrag (const MouseEvent& e)
{
    if (dragBand < 0) return;

    const int W = getWidth();
    const int H = getHeight();

    int dx = e.getPosition().x - dragStart.x;
    int dy = e.getPosition().y - dragStart.y;

    // Horizontal → log-frequency shift
    float logFreq    = std::log10 (dragStartFreq) + 3.0f * (float) dx / (float) W;
    float newFreq    = jlimit (20.0f, 20000.0f, std::pow (10.0f, logFreq));

    // Vertical → dB gain shift
    float newGain    = jlimit (-24.0f, 24.0f, dragStartGain - 48.0f * (float) dy / (float) H);

    if (freqSliders[dragBand]) freqSliders[dragBand]->setValue (newFreq, sendNotificationSync);
    if (gainSliders[dragBand]) gainSliders[dragBand]->setValue (newGain, sendNotificationSync);

    // Record this movement if automation is in record mode
    if (eqAuto != nullptr && eqAuto->isRecording())
        eqAuto->recordEvent (dragBand, newFreq, newGain);

    repaint();
}

void FrequencyResponseDisplay::mouseUp (const MouseEvent&)
{
    dragBand = -1;
    repaint();
}

//==============================================================================
// EQPanel
//==============================================================================

EQPanel::EQPanel (Model* m, AttachmentFactory* f, ParametricEQ* eq_,
                  StepSequencer* seq_, EQAutomationTrack* eqAuto_)
    : model (m), factory (f), eq (eq_), seq (seq_), eqAuto (eqAuto_)
{
    bypassButton = std::make_unique<ToggleButton> ("EQ Active");
    bypassButton->setButtonText ("EQ ON");
    bypassButton->setColour (ToggleButton::textColourId,   Colours::white);
    bypassButton->setColour (ToggleButton::tickColourId,   Colour (0xff4d9eff));
    bypassButton->setColour (ToggleButton::tickDisabledColourId, Colours::grey);
    addAndMakeVisible (*bypassButton);

    recButton = std::make_unique<TextButton> ("REC");
    recButton->setButtonText ("REC");
    recButton->setColour (TextButton::buttonColourId,   Colour (0xff3a1a1a));
    recButton->setColour (TextButton::buttonOnColourId, Colour (0xffdd2222));
    recButton->setColour (TextButton::textColourOffId,  Colour (0xffdd2222));
    recButton->setColour (TextButton::textColourOnId,   Colours::white);
    recButton->setClickingTogglesState (true);
    recButton->addListener (this);
    addAndMakeVisible (*recButton);

    playButton = std::make_unique<TextButton> ("PLAY");
    playButton->setButtonText ("PLAY");
    playButton->setColour (TextButton::buttonOnColourId, Colour (0xff2a52a8));
    playButton->setColour (TextButton::textColourOffId,  Colour (0xff4d9eff));
    playButton->setColour (TextButton::textColourOnId,   Colour (0xffe8f0ff));
    playButton->setClickingTogglesState (true);
    playButton->addListener (this);
    addAndMakeVisible (*playButton);

    clearButton = std::make_unique<TextButton> ("CLEAR");
    clearButton->setButtonText ("CLEAR");
    clearButton->setColour (TextButton::buttonColourId,  Colour (0xff2a2a1a));
    clearButton->setColour (TextButton::textColourOffId, Colours::lightgrey);
    clearButton->addListener (this);
    addAndMakeVisible (*clearButton);

    responseDisplay = std::make_unique<FrequencyResponseDisplay> (eq);
    responseDisplay->setAutomation (seq, eqAuto);
    addAndMakeVisible (*responseDisplay);

    const char* bandTypeNames[8] = { "LS", "PK", "PK", "PK", "PK", "PK", "PK", "HS" };

    for (int i = 0; i < 8; ++i)
    {
        // Band label (type)
        bandLabels[i] = std::make_unique<Label>();
        bandLabels[i]->setText (bandTypeNames[i], dontSendNotification);
        bandLabels[i]->setFont (Font (10.0f, Font::bold));
        bandLabels[i]->setColour (Label::textColourId, kBandColours[i]);
        bandLabels[i]->setJustificationType (Justification::centred);
        addAndMakeVisible (*bandLabels[i]);

        // Gain knob
        gainKnobs[i] = std::make_unique<Slider> (Slider::RotaryVerticalDrag, Slider::NoTextBox);
        gainKnobs[i]->setRange (-24.0, 24.0, 0.1);
        gainKnobs[i]->setValue (0.0, dontSendNotification);
        gainKnobs[i]->setColour (Slider::rotarySliderFillColourId, kBandColours[i]);
        gainKnobs[i]->setColour (Slider::rotarySliderOutlineColourId, kBandColours[i].darker (0.5f));
        gainKnobs[i]->addListener (this);
        addAndMakeVisible (*gainKnobs[i]);

        gainLabels[i] = std::make_unique<Label>();
        gainLabels[i]->setText ("Gain", dontSendNotification);
        gainLabels[i]->setFont (Font (9.0f));
        gainLabels[i]->setColour (Label::textColourId, Colours::grey);
        gainLabels[i]->setJustificationType (Justification::centred);
        addAndMakeVisible (*gainLabels[i]);

        // Freq knob
        freqKnobs[i] = std::make_unique<Slider> (Slider::RotaryVerticalDrag, Slider::NoTextBox);
        freqKnobs[i]->setRange (20.0, 20000.0, 0.1);
        freqKnobs[i]->setSkewFactorFromMidPoint (1000.0);
        freqKnobs[i]->addListener (this);
        addAndMakeVisible (*freqKnobs[i]);

        freqLabels[i] = std::make_unique<Label>();
        freqLabels[i]->setText ("Freq", dontSendNotification);
        freqLabels[i]->setFont (Font (9.0f));
        freqLabels[i]->setColour (Label::textColourId, Colours::grey);
        freqLabels[i]->setJustificationType (Justification::centred);
        addAndMakeVisible (*freqLabels[i]);

        // Q knob
        qKnobs[i] = std::make_unique<Slider> (Slider::RotaryVerticalDrag, Slider::NoTextBox);
        qKnobs[i]->setRange (0.1, 10.0, 0.01);
        qKnobs[i]->addListener (this);
        addAndMakeVisible (*qKnobs[i]);

        qLabels[i] = std::make_unique<Label>();
        qLabels[i]->setText ("Q", dontSendNotification);
        qLabels[i]->setFont (Font (9.0f));
        qLabels[i]->setColour (Label::textColourId, Colours::grey);
        qLabels[i]->setJustificationType (Justification::centred);
        addAndMakeVisible (*qLabels[i]);
    }

    // Pass slider pointers to display for drag interaction
    std::array<Slider*, 8> fs, gs;
    for (int i = 0; i < 8; ++i) { fs[i] = freqKnobs[i].get(); gs[i] = gainKnobs[i].get(); }
    responseDisplay->setFreqSliders (fs);
    responseDisplay->setGainSliders (gs);
}

EQPanel::~EQPanel() {}

void EQPanel::paint (Graphics& g)
{
    g.setColour (Colour (0xff141414));
    g.fillAll();

    // Header bar
    g.setColour (Colour (0xff1e1e2e));
    g.fillRect (0, 0, getWidth(), 28);

    g.setFont (Font (13.0f, Font::bold));
    g.setColour (Colour (0xff4d9eff));
    g.drawText ("8-BAND PARAMETRIC EQ", 10, 0, 250, 28, Justification::centredLeft, false);

    // Separator
    g.setColour (Colour (0xff333355));
    g.drawHorizontalLine (28, 0.0f, (float) getWidth());
}

void EQPanel::resized()
{
    const int W = getWidth();
    const int headerH   = 28;
    const int displayH  = 190;
    const int displayY  = headerH + 2;
    const int controlsY = displayY + displayH + 8;

    bypassButton->setBounds (W - 90, 2, 84, 24);
    // Automation buttons: REC / PLAY / CLEAR  (right of header, left of bypass)
    clearButton->setBounds (W - 270, 2, 56, 24);
    playButton ->setBounds (W - 210, 2, 56, 24);
    recButton  ->setBounds (W - 150, 2, 52, 24);
    responseDisplay->setBounds (4, displayY, W - 8, displayH);

    // 8 equal columns for band controls
    const int colW    = W / 8;
    const int padding = 2;

    for (int i = 0; i < 8; ++i)
    {
        int cx = i * colW + padding;
        int cw = colW - padding * 2;
        int y  = controlsY;

        bandLabels[i]->setBounds (cx, y, cw, 14);     y += 14;

        gainKnobs[i]->setBounds (cx, y, cw, 44);      y += 44;
        gainLabels[i]->setBounds (cx, y, cw, 11);     y += 12;

        freqKnobs[i]->setBounds (cx, y, cw, 36);      y += 36;
        freqLabels[i]->setBounds (cx, y, cw, 11);     y += 12;

        qKnobs[i]->setBounds (cx, y, cw, 30);         y += 30;
        qLabels[i]->setBounds (cx, y, cw, 11);
    }
}

void EQPanel::sliderValueChanged (Slider* /*slider*/)
{
    responseDisplay->repaint();
}

void EQPanel::buttonClicked (Button* button)
{
    if (eqAuto == nullptr) return;

    if (button == recButton.get())
    {
        if (recButton->getToggleState())
        {
            eqAuto->startRecording();
            startTimerHz (30);   // repaint so display reflects live recording
        }
        else
        {
            eqAuto->stopRecording();
            if (!playButton->getToggleState()) stopTimer();
        }
    }
    else if (button == playButton.get())
    {
        bool on = playButton->getToggleState();
        eqAuto->setPlaying (on);
        if (on) startTimerHz (30);
        else if (!recButton->getToggleState()) stopTimer();
    }
    else if (button == clearButton.get())
    {
        eqAuto->clearAll();
        playButton->setToggleState (false, dontSendNotification);
        eqAuto->setPlaying (false);
        stopTimer();
    }
}

void EQPanel::timerCallback()
{
    if (responseDisplay != nullptr)
        responseDisplay->repaint();
}

void EQPanel::syncPlayButtonState()
{
    if (eqAuto == nullptr || playButton == nullptr) return;
    bool isPlaying = eqAuto->isPlaying();
    playButton->setToggleState (isPlaying, dontSendNotification);
    if (isPlaying)
        startTimerHz (30);
    else if (!recButton->getToggleState())
        stopTimer();
}

void EQPanel::initAttachments()
{
    factory->createButtonAttachment ("eqEnabled", bypassButton.get());

    for (int i = 0; i < 8; ++i)
    {
        String b = String (i + 1);
        factory->createSliderAttachment ("eqGain" + b, gainKnobs[i].get());
        factory->createSliderAttachment ("eqFreq" + b, freqKnobs[i].get());
        factory->createSliderAttachment ("eqQ"    + b, qKnobs[i].get());
    }
}
