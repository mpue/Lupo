#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class LupoAudioProcessor;

class PresetBrowser : public Component,
                      public ListBoxModel,
                      public Button::Listener,
                      public KeyListener
{
public:
    std::function<void(const String&)> onPresetSelected;
    std::function<void()> onClose;

    PresetBrowser(LupoAudioProcessor* proc);
    ~PresetBrowser() override;

    void refresh();

    // Component
    void paint(Graphics& g) override;
    void resized() override;
    void visibilityChanged() override;

    // ListBoxModel
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemDoubleClicked(int row, const MouseEvent&) override;
    void selectedRowsChanged(int lastRowSelected) override;

    // KeyListener
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    // Button::Listener
    void buttonClicked(Button*) override;

private:
    LupoAudioProcessor* processor;
    StringArray presetNames;
    std::unique_ptr<ListBox> listBox;
    std::unique_ptr<TextButton> closeButton;
    std::unique_ptr<Label> titleLabel;
    std::unique_ptr<Label> hintLabel;

    void confirmSelection();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBrowser)
};
