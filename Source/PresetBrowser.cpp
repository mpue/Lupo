#include "PresetBrowser.h"
#include "PluginProcessor.h"

PresetBrowser::PresetBrowser(LupoAudioProcessor* proc)
    : processor(proc)
{
    titleLabel.reset(new Label("title", "Presets"));
    titleLabel->setFont(Font(28.0f, Font::bold));
    titleLabel->setJustificationType(Justification::centred);
    titleLabel->setColour(Label::textColourId, Colour(0xffe28600));
    addAndMakeVisible(titleLabel.get());

    listBox.reset(new ListBox("presetList", this));
    listBox->setRowHeight(32);
    listBox->setColour(ListBox::backgroundColourId, Colour(0xff1a1a1a));
    listBox->setColour(ListBox::outlineColourId, Colour(0xff444444));
    listBox->setOutlineThickness(1);
    listBox->addKeyListener(this);
    addAndMakeVisible(listBox.get());

    closeButton.reset(new TextButton("closeButton"));
    closeButton->setButtonText("Close");
    closeButton->addListener(this);
    closeButton->setColour(TextButton::buttonColourId, Colour(0xff444444));
    closeButton->setColour(TextButton::textColourOffId, Colours::white);
    addAndMakeVisible(closeButton.get());

    hintLabel.reset(new Label("hint", "Double-click or press Enter to load  |  Esc to close"));
    hintLabel->setFont(Font(12.0f));
    hintLabel->setJustificationType(Justification::centred);
    hintLabel->setColour(Label::textColourId, Colour(0xff888888));
    addAndMakeVisible(hintLabel.get());

    setWantsKeyboardFocus(true);
    addKeyListener(this);
}

PresetBrowser::~PresetBrowser()
{
    listBox->removeKeyListener(this);
    removeKeyListener(this);
    closeButton = nullptr;
    listBox = nullptr;
    titleLabel = nullptr;
    hintLabel = nullptr;
}

void PresetBrowser::refresh()
{
    presetNames.clear();
    for (int i = 0; i < processor->getNumPrograms(); i++)
        presetNames.add(processor->getProgramName(i));

    listBox->updateContent();

    // Select the currently active preset
    String current = processor->selectedProgram;
    int idx = presetNames.indexOf(current);
    if (idx >= 0)
    {
        listBox->selectRow(idx);
        listBox->scrollToEnsureRowIsOnscreen(idx);
    }
    else
    {
        listBox->selectRow(0);
    }
}

void PresetBrowser::visibilityChanged()
{
    if (isVisible())
    {
        refresh();
        listBox->grabKeyboardFocus();
    }
}

void PresetBrowser::paint(Graphics& g)
{
    // Semi-transparent dark background covering the whole plugin
    g.fillAll(Colour(0xf0111111));

    // Rounded panel behind the list
    auto panel = getLocalBounds().reduced(60, 40);
    g.setColour(Colour(0xff1e1e1e));
    g.fillRoundedRectangle(panel.toFloat(), 12.0f);
    g.setColour(Colour(0xff444444));
    g.drawRoundedRectangle(panel.toFloat(), 12.0f, 1.5f);
}

void PresetBrowser::resized()
{
    auto area = getLocalBounds().reduced(80, 60);

    titleLabel->setBounds(area.removeFromTop(44));
    area.removeFromTop(8);

    hintLabel->setBounds(area.removeFromBottom(24));
    area.removeFromBottom(8);

    closeButton->setBounds(area.removeFromBottom(36).withSizeKeepingCentre(120, 30));
    area.removeFromBottom(8);

    listBox->setBounds(area);
}

// ---- ListBoxModel ----

int PresetBrowser::getNumRows()
{
    return presetNames.size();
}

void PresetBrowser::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.setColour(Colour(0xffe28600).withAlpha(0.85f));
        g.fillRect(0, 0, width, height);
        g.setColour(Colours::black);
    }
    else
    {
        g.setColour(rowNumber % 2 == 0 ? Colour(0xff1a1a1a) : Colour(0xff222222));
        g.fillRect(0, 0, width, height);
        g.setColour(Colours::white);
    }

    g.setFont(Font(16.0f));
    g.drawText(presetNames[rowNumber], 16, 0, width - 16, height, Justification::centredLeft, true);
}

void PresetBrowser::listBoxItemDoubleClicked(int row, const MouseEvent&)
{
    if (row >= 0 && row < presetNames.size())
    {
        listBox->selectRow(row);
        confirmSelection();
    }
}

void PresetBrowser::selectedRowsChanged(int /*lastRowSelected*/)
{
    // Live preview: load preset as user navigates with arrow keys
    int row = listBox->getSelectedRow();
    if (row >= 0 && row < presetNames.size())
    {
        if (onPresetSelected)
            onPresetSelected(presetNames[row]);
    }
}

// ---- KeyListener ----

bool PresetBrowser::keyPressed(const KeyPress& key, Component* /*originatingComponent*/)
{
    if (key == KeyPress::escapeKey)
    {
        if (onClose)
            onClose();
        return true;
    }

    if (key == KeyPress::returnKey)
    {
        confirmSelection();
        return true;
    }

    return false;
}

// ---- Button::Listener ----

void PresetBrowser::buttonClicked(Button* btn)
{
    if (btn == closeButton.get())
    {
        if (onClose)
            onClose();
    }
}

void PresetBrowser::confirmSelection()
{
    int row = listBox->getSelectedRow();
    if (row >= 0 && row < presetNames.size())
    {
        if (onPresetSelected)
            onPresetSelected(presetNames[row]);
        if (onClose)
            onClose();
    }
}
