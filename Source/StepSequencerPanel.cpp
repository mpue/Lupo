#include "StepSequencerPanel.h"

using namespace juce;

//==============================================================================
// Helpers
//==============================================================================

static const char* kNoteNames[] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" };

String StepCell::noteName(int n)
{
    n = jlimit(0, 127, n);
    return String(kNoteNames[n % 12]) + String(n / 12 - 1);
}

Colour StepCell::cellColour(int /*index*/)
{
    return Colour(0xff4d9eff);
}

//==============================================================================
// StepCell
//==============================================================================

StepCell::StepCell(int index)
    : stepIndex(index)
{
    setInterceptsMouseClicks(true, false);
}

void StepCell::setStep(StepSequencer::Step* s) { step = s; }
void StepCell::setPlaying(bool p) { isPlaying = p; }

void StepCell::paint(Graphics& g)
{
    auto b     = getLocalBounds();
    int  W     = b.getWidth();
    int  H     = b.getHeight();
    bool active = step && step->active.load();
    int  vel    = step ? jlimit(0, 127, step->velocity.load()) : 100;
    int  note   = step ? jlimit(0, 127, step->note.load())     : 60;
    Colour col  = cellColour(stepIndex);

    // Overall background
    g.setColour(Colour(0xff0d1117));
    g.fillRect(b);

    // Step number
    g.setFont(Font(8.0f));
    g.setColour(Colours::grey);
    g.drawText(String(stepIndex + 1), 0, 0, W, 10, Justification::centred);

    // Active / inactive body
    Rectangle<int> body(1, 10, W - 2, H - 25);

    if (active)
    {
        Colour fill = isPlaying ? col.brighter(0.5f) : col.withAlpha(0.85f);
        g.setColour(fill);
        g.fillRect(body);

        g.setFont(Font(9.0f, Font::bold));
        g.setColour(Colours::white);
        g.drawText(noteName(note), body, Justification::centred);
    }
    else
    {
        g.setColour(isPlaying ? col.withAlpha(0.20f) : Colour(0xff161622));
        g.fillRect(body);

        g.setFont(Font(8.0f));
        g.setColour(Colour(0xff404060));
        g.drawText(noteName(note), body, Justification::centred);
    }

    // Body border
    g.setColour(isPlaying ? col.withAlpha(0.9f) : (active ? col.darker(0.4f) : Colour(0xff252540)));
    g.drawRect(body, 1);

    // Velocity bar
    Rectangle<int> velBar(1, H - 14, W - 2, 13);
    g.setColour(Colour(0xff12121e));
    g.fillRect(velBar);

    float fraction = vel / 127.0f;
    g.setColour(active ? col.withAlpha(0.7f) : Colour(0xff282840));
    g.fillRect(Rectangle<int>(velBar.getX(), velBar.getY(),
                              (int)(velBar.getWidth() * fraction), velBar.getHeight()));

    // Outer cell border
    g.setColour(Colour(0xff202035));
    g.drawRect(b, 1);
}

void StepCell::mouseDown(const MouseEvent& e)
{
    if (step == nullptr) return;
    dragStartY   = e.getPosition().y;
    dragStartVel = step->velocity.load();
    isDragging   = false;
}

void StepCell::mouseDrag(const MouseEvent& e)
{
    if (step == nullptr) return;
    int dy = e.getPosition().y - dragStartY;
    if (std::abs(dy) > 4)
    {
        isDragging = true;
        // Drag up → louder
        int newVel = jlimit(1, 127, dragStartVel - dy);
        step->velocity.store(newVel);
        repaint();
    }
}

void StepCell::mouseUp(const MouseEvent&)
{
    if (step == nullptr) return;
    if (!isDragging)
    {
        step->active.store(!step->active.load());
        repaint();
    }
    isDragging = false;
}

void StepCell::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& w)
{
    if (step == nullptr) return;
    int delta = (w.deltaY > 0.0f) ? 1 : -1;
    if (w.isReversed) delta = -delta;

    int current = step->note.load();
    int next;

    // When Auto chord mode is on, snap to scale notes only
    if (chordManager != nullptr && chordManager->isAutoChord())
        next = chordManager->nextScaleNote(current, delta);
    else
        next = jlimit(0, 127, current + delta);

    step->note.store(next);
    repaint();
}

//==============================================================================
// StepSequencerPanel helpers
//==============================================================================

static void styleToggleBtn(TextButton& btn, Colour accentColour)
{
    btn.setColour(TextButton::buttonColourId,   Colour(0xff1a1a2e));
    btn.setColour(TextButton::buttonOnColourId,  accentColour);
    btn.setColour(TextButton::textColourOffId,   Colours::grey);
    btn.setColour(TextButton::textColourOnId,    Colours::white);
    btn.setClickingTogglesState(false);
}

//==============================================================================
// StepSequencerPanel
//==============================================================================

StepSequencerPanel::StepSequencerPanel(StepSequencer* sequencer, ChordManager* chordManager)
    : seq(sequencer)
{
    // Enable button
    enableButton = std::make_unique<ToggleButton>("SEQ ON");
    enableButton->setButtonText("SEQ ON");
    enableButton->setColour(ToggleButton::tickColourId,         Colour(0xff4d9eff));
    enableButton->setColour(ToggleButton::tickDisabledColourId, Colours::grey);
    enableButton->setColour(ToggleButton::textColourId,         Colours::white);
    enableButton->onStateChange = [this] {
        seq->setEnabled(enableButton->getToggleState());
    };
    addAndMakeVisible(*enableButton);

    // Step cells
    for (int i = 0; i < StepSequencer::MAX_STEPS; ++i)
    {
        cells[i] = std::make_unique<StepCell>(i);
        cells[i]->setStep(&seq->getStep(i));
        cells[i]->setChordManager(chordManager);
        addAndMakeVisible(*cells[i]);
    }

    // Swing
    swingSlider = std::make_unique<Slider>(Slider::LinearHorizontal, Slider::NoTextBox);
    swingSlider->setRange(0.0, 0.75, 0.01);
    swingSlider->setValue(seq->getSwing(), dontSendNotification);
    swingSlider->setColour(Slider::trackColourId,     Colour(0xff303050));
    swingSlider->setColour(Slider::thumbColourId,     Colour(0xffffaa00));
    swingSlider->onValueChange = [this] { seq->setSwing((float)swingSlider->getValue()); };
    addAndMakeVisible(*swingSlider);

    swingLabel = std::make_unique<Label>("", "SWING");
    swingLabel->setFont(Font(9.0f, Font::bold));
    swingLabel->setColour(Label::textColourId, Colours::grey);
    swingLabel->setJustificationType(Justification::centred);
    addAndMakeVisible(*swingLabel);

    // Steps buttons: 8 / 16 / 32
    stepsLabel = std::make_unique<Label>("", "STEPS");
    stepsLabel->setFont(Font(9.0f, Font::bold));
    stepsLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(*stepsLabel);

    const char* stepsCaptions[] = { "8", "16", "32" };
    const int   stepsValues[]   = { 8, 16, 32 };
    for (int i = 0; i < 3; ++i)
    {
        stepsButtons[i] = std::make_unique<TextButton>(stepsCaptions[i]);
        styleToggleBtn(*stepsButtons[i], Colour(0xff4d9eff));
        int val = stepsValues[i];
        stepsButtons[i]->onClick = [this, val] {
            seq->setNumSteps(val);
            updateStepsButtons();
        };
        addAndMakeVisible(*stepsButtons[i]);
    }

    // Division buttons: 1/4 / 1/8 / 1/16 / 1/32
    divLabel = std::make_unique<Label>("", "DIV");
    divLabel->setFont(Font(9.0f, Font::bold));
    divLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(*divLabel);

    const char* divCaptions[] = { "1/4", "1/8", "1/16", "1/32" };
    for (int i = 0; i < 4; ++i)
    {
        divButtons[i] = std::make_unique<TextButton>(divCaptions[i]);
        styleToggleBtn(*divButtons[i], Colour(0xff4d9eff));
        int idx = i;
        divButtons[i]->onClick = [this, idx] {
            seq->setDivision(idx);
            updateDivButtons();
        };
        addAndMakeVisible(*divButtons[i]);
    }

    // Direction buttons
    dirLabel = std::make_unique<Label>("", "DIR");
    dirLabel->setFont(Font(9.0f, Font::bold));
    dirLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(*dirLabel);

    const char* dirCaptions[] = { ">", "<", "<>", "~" };
    for (int i = 0; i < 4; ++i)
    {
        dirButtons[i] = std::make_unique<TextButton>(dirCaptions[i]);
        styleToggleBtn(*dirButtons[i], Colour(0xff4d9eff));
        int idx = i;
        dirButtons[i]->onClick = [this, idx] {
            seq->setDirection(idx);
            updateDirButtons();
        };
        addAndMakeVisible(*dirButtons[i]);
    }

    updateStepsButtons();
    updateDivButtons();
    updateDirButtons();

    startTimerHz(30);
}

StepSequencerPanel::~StepSequencerPanel()
{
    stopTimer();
}

void StepSequencerPanel::updateStepsButtons()
{
    int n = seq->getNumSteps();
    int indices[] = { 8, 16, 32 };
    for (int i = 0; i < 3; ++i)
    {
        bool sel = (n == indices[i]);
        stepsButtons[i]->setToggleState(sel, dontSendNotification);
        stepsButtons[i]->setColour(TextButton::buttonColourId,
                                   sel ? Colour(0xff1c2a50) : Colour(0xff1a1a2e));
        stepsButtons[i]->setColour(TextButton::textColourOffId,
                                   sel ? Colour(0xff4d9eff) : Colours::grey);
    }
}

void StepSequencerPanel::updateDivButtons()
{
    int d = seq->getDivision();
    for (int i = 0; i < 4; ++i)
    {
        bool sel = (d == i);
        divButtons[i]->setColour(TextButton::buttonColourId,
                                  sel ? Colour(0xff1c2a50) : Colour(0xff1a1a2e));
        divButtons[i]->setColour(TextButton::textColourOffId,
                                  sel ? Colour(0xff4d9eff) : Colours::grey);
    }
}

void StepSequencerPanel::updateDirButtons()
{
    int d = seq->getDirection();
    for (int i = 0; i < 4; ++i)
    {
        bool sel = (d == i);
        dirButtons[i]->setColour(TextButton::buttonColourId,
                                  sel ? Colour(0xff1c2a50) : Colour(0xff1a1a2e));
        dirButtons[i]->setColour(TextButton::textColourOffId,
                                  sel ? Colour(0xff4d9eff) : Colours::grey);
    }
}

void StepSequencerPanel::paint(Graphics& g)
{
    g.setColour(Colour(0xff141414));
    g.fillAll();

    // Header bar
    g.setColour(Colour(0xff1e1e2e));
    g.fillRect(0, 0, getWidth(), 28);

    g.setFont(Font(13.0f, Font::bold));
    g.setColour(Colour(0xff4d9eff));
    g.drawText("STEP SEQUENCER", 10, 0, 240, 28, Justification::centredLeft);

    // Row labels
    g.setFont(Font(8.0f));
    g.setColour(Colour(0xff404060));
    g.drawText("1 - 16", 2, 30, 30, 10, Justification::centredLeft);
    g.drawText("17 - 32", 2, 120, 40, 10, Justification::centredLeft);

    // Control section divider
    g.setColour(Colour(0xff252540));
    g.drawHorizontalLine(212, 0.0f, (float)getWidth());
}

void StepSequencerPanel::resized()
{
    const int W = getWidth();

    // Header
    enableButton->setBounds(W - 90, 2, 84, 24);

    // Step grid: 2 rows, 16 cells each
    const int cellW  = W / 16;
    const int cellH  = 88;
    const int row1Y  = 30;
    const int row2Y  = row1Y + cellH + 2;

    for (int i = 0; i < 16; ++i)
        cells[i]->setBounds(i * cellW, row1Y, cellW, cellH);

    for (int i = 0; i < 16; ++i)
        cells[16 + i]->setBounds(i * cellW, row2Y, cellW, cellH);

    // Controls area starts at y=214
    const int ctrlY = 214;

    // Swing: full width
    swingLabel->setBounds(4, ctrlY, 60, 12);
    swingSlider->setBounds(4, ctrlY + 14, W - 8, 24);

    // Buttons row 1: Steps
    const int btnY1  = ctrlY + 52;
    const int btnH   = 22;
    const int lblW   = 36;

    stepsLabel->setBounds(4, btnY1 + 3, lblW, btnH);
    int bx = lblW + 8;
    int bw = 36;
    for (int i = 0; i < 3; ++i)
        stepsButtons[i]->setBounds(bx + i * (bw + 4), btnY1, bw, btnH);

    // Buttons row 2: Division
    const int btnY2 = btnY1 + btnH + 6;
    divLabel->setBounds(4, btnY2 + 3, lblW, btnH);
    bw = 38;
    for (int i = 0; i < 4; ++i)
        divButtons[i]->setBounds(bx + i * (bw + 4), btnY2, bw, btnH);

    // Buttons row 3: Direction
    const int btnY3 = btnY2 + btnH + 6;
    dirLabel->setBounds(4, btnY3 + 3, lblW, btnH);
    bw = 38;
    for (int i = 0; i < 4; ++i)
        dirButtons[i]->setBounds(bx + i * (bw + 4), btnY3, bw, btnH);
}

void StepSequencerPanel::timerCallback()
{
    int ps = seq->getPlayingStep();
    bool running = seq->isRunning();

    if (ps != lastPlayingStep || !running)
    {
        // Clear previous playing cell
        if (lastPlayingStep >= 0 && lastPlayingStep < StepSequencer::MAX_STEPS)
        {
            cells[lastPlayingStep]->setPlaying(false);
            cells[lastPlayingStep]->repaint();
        }

        if (running && ps >= 0 && ps < StepSequencer::MAX_STEPS)
        {
            cells[ps]->setPlaying(true);
            cells[ps]->repaint();
            lastPlayingStep = ps;
        }
        else
        {
            lastPlayingStep = -1;
        }
    }
}
