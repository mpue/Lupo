#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>

class LupoAudioProcessor;

struct PresetMeta
{
    String category;
    String description;
    int rating = 0;
};

class StarRatingComponent : public Component
{
public:
    StarRatingComponent() = default;

    std::function<void(int)> onRatingChanged;
    int rating = 0;

    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent& e) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StarRatingComponent)
};

class PresetBrowser : public Component,
                      public ListBoxModel,
                      public Button::Listener,
                      public KeyListener,
                      public TextEditor::Listener
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

    // ListBoxModel (preset list)
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemDoubleClicked(int row, const MouseEvent&) override;
    void selectedRowsChanged(int lastRowSelected) override;

    // KeyListener
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    // Button::Listener
    void buttonClicked(Button*) override;

    // TextEditor::Listener
    void textEditorTextChanged(TextEditor&) override {}
    void textEditorReturnKeyPressed(TextEditor&) override {}
    void textEditorEscapeKeyPressed(TextEditor&) override {}
    void textEditorFocusLost(TextEditor& editor) override;

private:
    struct CategoryModel : public ListBoxModel
    {
        StringArray items;
        std::function<void(int)> onSelectionChanged;
        int getNumRows() override { return items.size(); }
        void paintListBoxItem(int row, Graphics& g, int w, int h, bool sel) override;
        void selectedRowsChanged(int row) override
        {
            if (onSelectionChanged) onSelectionChanged(row);
        }
    } categoryModel;

    LupoAudioProcessor* processor;
    StringArray presetNames;
    StringArray filteredPresets;
    std::map<String, PresetMeta> metadata;
    StringArray allCategories;
    String selectedCategory { "All" };
    String currentDetailPreset;

    std::unique_ptr<ListBox>      listBox;
    std::unique_ptr<ListBox>      categoryBox;
    std::unique_ptr<TextButton>   closeButton;
    std::unique_ptr<TextButton>   addCategoryButton;
    std::unique_ptr<TextButton>   deleteCategoryButton;
    std::unique_ptr<TextButton>   assignCategoryButton;
    std::unique_ptr<Label>        titleLabel;
    std::unique_ptr<Label>        hintLabel;
    std::unique_ptr<Label>        descLabel;
    std::unique_ptr<TextEditor>   descriptionEditor;
    std::unique_ptr<StarRatingComponent> starRating;
    std::unique_ptr<Label>        ratingLabel;
    std::unique_ptr<Label>        categoryLabel;

    static File getMetadataFile();
    void loadMetadata();
    void saveMetadata();
    void updateCategoryList();
    void updateFilteredPresets();
    void updateDetailPanel(const String& presetName);
    void saveCurrentDescription();
    void confirmSelection();
    void addCategory();
    void deleteCategory();
    void assignCategory();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetBrowser)
};
