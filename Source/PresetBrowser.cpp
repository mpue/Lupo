#include "PresetBrowser.h"
#include "PluginProcessor.h"

// ============================================================
// StarRatingComponent
// ============================================================

void StarRatingComponent::paint(Graphics& g)
{
    const float w     = (float)getWidth();
    const float h     = (float)getHeight();
    const float cellW = w / 5.0f;
    const float starH = h - 4.0f;
    g.setFont(Font(starH));

    for (int i = 0; i < 5; i++)
    {
        g.setColour(i < rating ? Colour(0xff4d9eff) : Colour(0xff555555));
        String star = (i < rating) ? String::fromUTF8("\xe2\x98\x85")
                                   : String::fromUTF8("\xe2\x98\x86");
        g.drawText(star, (int)(i * cellW), 2, (int)cellW, (int)starH,
                   Justification::centred, false);
    }
}

void StarRatingComponent::mouseDown(const MouseEvent& e)
{
    int clicked = (int)(e.x / (getWidth() / 5.0f)) + 1;
    clicked = jlimit(1, 5, clicked);
    rating  = (rating == clicked) ? 0 : clicked;  // toggle off if same star clicked
    repaint();
    if (onRatingChanged) onRatingChanged(rating);
}

// ============================================================
// CategoryModel
// ============================================================

void PresetBrowser::CategoryModel::paintListBoxItem(int row, Graphics& g,
                                                    int w, int h, bool sel)
{
    if (sel)
    {
        g.setColour(Colour(0xff4d9eff).withAlpha(0.85f));
        g.fillRect(0, 0, w, h);
        g.setColour(Colours::black);
    }
    else
    {
        g.setColour(row % 2 == 0 ? Colour(0xff1a1a1a) : Colour(0xff222222));
        g.fillRect(0, 0, w, h);
        g.setColour(Colours::white);
    }
    if (row < items.size())
    {
        g.setFont(Font(13.0f));
        g.drawText(items[row], 8, 0, w - 8, h, Justification::centredLeft, true);
    }
}

// ============================================================
// PresetBrowser – constructor / destructor
// ============================================================

PresetBrowser::PresetBrowser(LupoAudioProcessor* proc)
    : processor(proc)
{
    // ---- Title ----
    titleLabel = std::make_unique<Label>("title", "Presets");
    titleLabel->setFont(Font(28.0f, Font::bold));
    titleLabel->setJustificationType(Justification::centred);
    titleLabel->setColour(Label::textColourId, Colour(0xff4d9eff));
    addAndMakeVisible(titleLabel.get());

    // ---- Category section ----
    categoryLabel = std::make_unique<Label>("catLabel", "Categories");
    categoryLabel->setFont(Font(11.0f, Font::bold));
    categoryLabel->setJustificationType(Justification::centredLeft);
    categoryLabel->setColour(Label::textColourId, Colour(0xff888888));
    addAndMakeVisible(categoryLabel.get());

    categoryBox = std::make_unique<ListBox>("categoryList", &categoryModel);
    categoryBox->setRowHeight(26);
    categoryBox->setColour(ListBox::backgroundColourId, Colour(0xff1a1a1a));
    categoryBox->setColour(ListBox::outlineColourId,    Colour(0xff444444));
    categoryBox->setOutlineThickness(1);
    addAndMakeVisible(categoryBox.get());

    categoryModel.onSelectionChanged = [this](int row)
    {
        if (row >= 0 && row < categoryModel.items.size())
        {
            selectedCategory = categoryModel.items[row];
            updateFilteredPresets();
        }
    };

    addCategoryButton = std::make_unique<TextButton>("+");
    addCategoryButton->setTooltip("Add category");
    addCategoryButton->addListener(this);
    addCategoryButton->setColour(TextButton::buttonColourId,  Colour(0xff2a2a2a));
    addCategoryButton->setColour(TextButton::textColourOffId, Colour(0xff4d9eff));
    addAndMakeVisible(addCategoryButton.get());

    deleteCategoryButton = std::make_unique<TextButton>("-");
    deleteCategoryButton->setTooltip("Delete selected category");
    deleteCategoryButton->addListener(this);
    deleteCategoryButton->setColour(TextButton::buttonColourId,  Colour(0xff2a2a2a));
    deleteCategoryButton->setColour(TextButton::textColourOffId, Colour(0xff888888));
    addAndMakeVisible(deleteCategoryButton.get());

    // ---- Preset list ----
    listBox = std::make_unique<ListBox>("presetList", this);
    listBox->setRowHeight(32);
    listBox->setColour(ListBox::backgroundColourId, Colour(0xff1a1a1a));
    listBox->setColour(ListBox::outlineColourId,    Colour(0xff444444));
    listBox->setOutlineThickness(1);
    listBox->addKeyListener(this);
    addAndMakeVisible(listBox.get());

    // ---- Detail panel ----
    descLabel = std::make_unique<Label>("descLabel", "Description:");
    descLabel->setFont(Font(11.0f));
    descLabel->setJustificationType(Justification::topLeft);
    descLabel->setColour(Label::textColourId, Colour(0xff888888));
    addAndMakeVisible(descLabel.get());

    descriptionEditor = std::make_unique<TextEditor>("description");
    descriptionEditor->setMultiLine(true, true);
    descriptionEditor->setReturnKeyStartsNewLine(true);
    descriptionEditor->setScrollbarsShown(false);
    descriptionEditor->setColour(TextEditor::backgroundColourId, Colour(0xff111111));
    descriptionEditor->setColour(TextEditor::textColourId,       Colours::white);
    descriptionEditor->setColour(TextEditor::outlineColourId,    Colour(0xff444444));
    descriptionEditor->setFont(Font(13.0f));
    descriptionEditor->addListener(this);
    addAndMakeVisible(descriptionEditor.get());

    ratingLabel = std::make_unique<Label>("ratingLabel", "Rating:");
    ratingLabel->setFont(Font(11.0f));
    ratingLabel->setColour(Label::textColourId, Colour(0xff888888));
    addAndMakeVisible(ratingLabel.get());

    starRating = std::make_unique<StarRatingComponent>();
    starRating->onRatingChanged = [this](int r)
    {
        if (currentDetailPreset.isNotEmpty())
        {
            metadata[currentDetailPreset].rating = r;
            saveMetadata();
            int row = filteredPresets.indexOf(currentDetailPreset);
            if (row >= 0) listBox->repaintRow(row);
        }
    };
    addAndMakeVisible(starRating.get());

    assignCategoryButton = std::make_unique<TextButton>("Category: (None)");
    assignCategoryButton->setTooltip("Assign this preset to a category");
    assignCategoryButton->addListener(this);
    assignCategoryButton->setColour(TextButton::buttonColourId,  Colour(0xff333333));
    assignCategoryButton->setColour(TextButton::textColourOffId, Colours::white);
    addAndMakeVisible(assignCategoryButton.get());

    // ---- Bottom bar ----
    closeButton = std::make_unique<TextButton>("closeButton");
    closeButton->setButtonText("Close");
    closeButton->addListener(this);
    closeButton->setColour(TextButton::buttonColourId,  Colour(0xff444444));
    closeButton->setColour(TextButton::textColourOffId, Colours::white);
    addAndMakeVisible(closeButton.get());

    hintLabel = std::make_unique<Label>("hint",
        "Double-click or Enter to load  |  Esc to close");
    hintLabel->setFont(Font(12.0f));
    hintLabel->setJustificationType(Justification::centred);
    hintLabel->setColour(Label::textColourId, Colour(0xff888888));
    addAndMakeVisible(hintLabel.get());

    setWantsKeyboardFocus(true);
    addKeyListener(this);
}

PresetBrowser::~PresetBrowser()
{
    saveCurrentDescription();
    if (listBox) listBox->removeKeyListener(this);
    removeKeyListener(this);
    if (descriptionEditor) descriptionEditor->removeListener(this);

    assignCategoryButton = nullptr;
    starRating           = nullptr;
    ratingLabel          = nullptr;
    descriptionEditor    = nullptr;
    descLabel            = nullptr;
    categoryBox          = nullptr;
    categoryLabel        = nullptr;
    addCategoryButton    = nullptr;
    deleteCategoryButton = nullptr;
    closeButton          = nullptr;
    hintLabel            = nullptr;
    listBox              = nullptr;
    titleLabel           = nullptr;
}

// ============================================================
// Metadata – load / save
// ============================================================

File PresetBrowser::getMetadataFile()
{
    String appDataPath =
        File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
    return File(appDataPath + "/Audio/Presets/pueski/Lupo/preset_metadata.xml");
}

void PresetBrowser::loadMetadata()
{
    metadata.clear();
    allCategories.clear();

    File f = getMetadataFile();
    if (!f.exists()) return;

    std::unique_ptr<XmlElement> xml = XmlDocument(f).getDocumentElement();
    if (!xml) return;

    if (XmlElement* catsEl = xml->getChildByName("Categories"))
    {
        forEachXmlChildElement(*catsEl, cat)
        {
            if (cat->hasTagName("Category"))
            {
                String name = cat->getStringAttribute("name");
                if (name.isNotEmpty())
                    allCategories.addIfNotAlreadyThere(name);
            }
        }
    }

    if (XmlElement* presetsEl = xml->getChildByName("Presets"))
    {
        forEachXmlChildElement(*presetsEl, p)
        {
            if (p->hasTagName("Preset"))
            {
                String name = p->getStringAttribute("name");
                if (name.isNotEmpty())
                {
                    PresetMeta m;
                    m.category    = p->getStringAttribute("category");
                    m.description = p->getStringAttribute("description");
                    m.rating      = p->getIntAttribute("rating", 0);
                    metadata[name] = m;
                }
            }
        }
    }
}

void PresetBrowser::saveMetadata()
{
    XmlElement root("PresetMetadata");

    XmlElement* catsEl = root.createNewChildElement("Categories");
    for (int i = 0; i < allCategories.size(); i++)
        catsEl->createNewChildElement("Category")->setAttribute("name", allCategories[i]);

    XmlElement* presetsEl = root.createNewChildElement("Presets");
    for (auto& pair : metadata)
    {
        // Skip entries that carry no information to keep the file lean
        if (pair.second.category.isEmpty()
            && pair.second.description.isEmpty()
            && pair.second.rating == 0)
            continue;

        XmlElement* p = presetsEl->createNewChildElement("Preset");
        p->setAttribute("name",        pair.first);
        p->setAttribute("category",    pair.second.category);
        p->setAttribute("description", pair.second.description);
        p->setAttribute("rating",      pair.second.rating);
    }

    root.writeToFile(getMetadataFile(), "");
}

// ============================================================
// UI helpers
// ============================================================

void PresetBrowser::updateCategoryList()
{
    categoryModel.items.clear();
    categoryModel.items.add("All");
    categoryModel.items.addArray(allCategories);
    categoryBox->updateContent();

    int idx = categoryModel.items.indexOf(selectedCategory);
    if (idx < 0) { selectedCategory = "All"; idx = 0; }
    categoryBox->selectRow(idx);
}

void PresetBrowser::updateFilteredPresets()
{
    filteredPresets.clear();
    for (int i = 0; i < presetNames.size(); i++)
    {
        const String& name = presetNames[i];
        if (selectedCategory == "All")
        {
            filteredPresets.add(name);
        }
        else
        {
            auto it = metadata.find(name);
            if (it != metadata.end() && it->second.category == selectedCategory)
                filteredPresets.add(name);
        }
    }
    listBox->updateContent();
    listBox->deselectAllRows();
    currentDetailPreset = {};
    updateDetailPanel({});
}

void PresetBrowser::updateDetailPanel(const String& presetName)
{
    currentDetailPreset = presetName;

    if (presetName.isEmpty())
    {
        descriptionEditor->setText("", false);
        starRating->rating = 0;
        starRating->repaint();
        assignCategoryButton->setButtonText("Category: (None)");
        return;
    }

    auto it = metadata.find(presetName);
    if (it != metadata.end())
    {
        descriptionEditor->setText(it->second.description, false);
        starRating->rating = it->second.rating;
        String cat = it->second.category.isNotEmpty() ? it->second.category : "(None)";
        assignCategoryButton->setButtonText("Category: " + cat);
    }
    else
    {
        descriptionEditor->setText("", false);
        starRating->rating = 0;
        assignCategoryButton->setButtonText("Category: (None)");
    }
    starRating->repaint();
}

void PresetBrowser::saveCurrentDescription()
{
    if (currentDetailPreset.isEmpty()) return;
    metadata[currentDetailPreset].description = descriptionEditor->getText();
    saveMetadata();
}

// ============================================================
// Public API
// ============================================================

void PresetBrowser::refresh()
{
    loadMetadata();

    presetNames.clear();
    for (int i = 0; i < processor->getNumPrograms(); i++)
        presetNames.add(processor->getProgramName(i));

    updateCategoryList();
    updateFilteredPresets();

    String current = processor->selectedProgram;
    int idx = filteredPresets.indexOf(current);
    if (idx >= 0)
    {
        listBox->selectRow(idx);
        listBox->scrollToEnsureRowIsOnscreen(idx);
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

// ============================================================
// Component
// ============================================================

void PresetBrowser::paint(Graphics& g)
{
    g.fillAll(Colour(0xf0111111));

    auto panel = getLocalBounds().reduced(60, 40);
    g.setColour(Colour(0xff1e1e1e));
    g.fillRoundedRectangle(panel.toFloat(), 12.0f);
    g.setColour(Colour(0xff444444));
    g.drawRoundedRectangle(panel.toFloat(), 12.0f, 1.5f);

    // Vertical separator between category list and preset list
    auto area = getLocalBounds().reduced(80, 60);
    area.removeFromTop(52);   // title + gap
    area.removeFromBottom(70); // hint + close
    int sepX = area.getX() + 170 + 5;
    g.setColour(Colour(0xff383838));
    g.drawLine((float)sepX, (float)area.getY(),
               (float)sepX, (float)area.getBottom(), 1.0f);

    // Horizontal separator above detail panel
    auto rightArea = area;
    rightArea.removeFromLeft(170 + 10);
    int sepY = rightArea.getBottom() - 108;
    g.drawLine((float)rightArea.getX(), (float)sepY,
               (float)rightArea.getRight(), (float)sepY, 1.0f);
}

void PresetBrowser::resized()
{
    auto area = getLocalBounds().reduced(80, 60);

    // Title
    titleLabel->setBounds(area.removeFromTop(44));
    area.removeFromTop(8);

    // Bottom bar
    hintLabel->setBounds(area.removeFromBottom(24));
    area.removeFromBottom(6);
    closeButton->setBounds(area.removeFromBottom(32).withSizeKeepingCentre(120, 28));
    area.removeFromBottom(10);

    // Left panel: Categories
    auto leftPanel = area.removeFromLeft(170);
    area.removeFromLeft(10);

    auto catBtns = leftPanel.removeFromBottom(28);
    leftPanel.removeFromBottom(4);
    int btnW = (catBtns.getWidth() - 4) / 2;
    addCategoryButton->setBounds(catBtns.removeFromLeft(btnW));
    catBtns.removeFromLeft(4);
    deleteCategoryButton->setBounds(catBtns);

    categoryLabel->setBounds(leftPanel.removeFromTop(20));
    leftPanel.removeFromTop(4);
    categoryBox->setBounds(leftPanel);

    // Right panel: detail panel at bottom, preset list above
    auto detailPanel = area.removeFromBottom(100);
    area.removeFromBottom(8);
    listBox->setBounds(area);

    // Detail panel layout
    auto ratingRow = detailPanel.removeFromBottom(28);
    ratingLabel->setBounds(ratingRow.removeFromLeft(52));
    starRating->setBounds(ratingRow.removeFromLeft(130));
    ratingRow.removeFromLeft(10);
    assignCategoryButton->setBounds(ratingRow.removeFromLeft(180));

    detailPanel.removeFromBottom(4);
    detailPanel.removeFromTop(4);
    descLabel->setBounds(detailPanel.removeFromLeft(80));
    descriptionEditor->setBounds(detailPanel);
}

// ============================================================
// ListBoxModel (preset list)
// ============================================================

int PresetBrowser::getNumRows()
{
    return filteredPresets.size();
}

void PresetBrowser::paintListBoxItem(int rowNumber, Graphics& g,
                                     int width, int height, bool rowIsSelected)
{
    if (rowNumber >= filteredPresets.size()) return;

    if (rowIsSelected)
    {
        g.setColour(Colour(0xff4d9eff).withAlpha(0.85f));
        g.fillRect(0, 0, width, height);
        g.setColour(Colours::black);
    }
    else
    {
        g.setColour(rowNumber % 2 == 0 ? Colour(0xff1a1a1a) : Colour(0xff222222));
        g.fillRect(0, 0, width, height);
        g.setColour(Colours::white);
    }

    const String& name = filteredPresets[rowNumber];

    // Preset name
    g.setFont(Font(16.0f));
    g.drawText(name, 12, 0, width - 85, height, Justification::centredLeft, true);

    // Inline star rating
    auto it = metadata.find(name);
    int rating = (it != metadata.end()) ? it->second.rating : 0;
    if (rating > 0)
    {
        String stars;
        for (int i = 0; i < rating; i++)
            stars += String::fromUTF8("\xe2\x98\x85");
        g.setFont(Font(12.0f));
        g.setColour(rowIsSelected ? Colour(0xff000000).withAlpha(0.55f)
                                  : Colour(0xff4d9eff));
        g.drawText(stars, width - 82, 0, 76, height, Justification::centredRight, false);
    }
}

void PresetBrowser::listBoxItemDoubleClicked(int row, const MouseEvent&)
{
    if (row >= 0 && row < filteredPresets.size())
    {
        listBox->selectRow(row);
        confirmSelection();
    }
}

void PresetBrowser::selectedRowsChanged(int /*lastRowSelected*/)
{
    saveCurrentDescription();

    int row = listBox->getSelectedRow();
    if (row >= 0 && row < filteredPresets.size())
    {
        const String& name = filteredPresets[row];
        updateDetailPanel(name);
        if (onPresetSelected) onPresetSelected(name);
    }
}

// ============================================================
// KeyListener
// ============================================================

bool PresetBrowser::keyPressed(const KeyPress& key, Component*)
{
    if (key == KeyPress::escapeKey)
    {
        saveCurrentDescription();
        if (onClose) onClose();
        return true;
    }
    if (key == KeyPress::returnKey)
    {
        confirmSelection();
        return true;
    }
    return false;
}

// ============================================================
// Button::Listener
// ============================================================

void PresetBrowser::buttonClicked(Button* btn)
{
    if      (btn == closeButton.get())          { saveCurrentDescription(); if (onClose) onClose(); }
    else if (btn == addCategoryButton.get())    { addCategory(); }
    else if (btn == deleteCategoryButton.get()) { deleteCategory(); }
    else if (btn == assignCategoryButton.get()) { assignCategory(); }
}

// ============================================================
// TextEditor::Listener
// ============================================================

void PresetBrowser::textEditorFocusLost(TextEditor& editor)
{
    if (&editor == descriptionEditor.get())
        saveCurrentDescription();
}

// ============================================================
// Internal actions
// ============================================================

void PresetBrowser::confirmSelection()
{
    saveCurrentDescription();
    int row = listBox->getSelectedRow();
    if (row >= 0 && row < filteredPresets.size())
    {
        if (onPresetSelected) onPresetSelected(filteredPresets[row]);
        if (onClose) onClose();
    }
}

void PresetBrowser::addCategory()
{
    auto* aw = new AlertWindow("New Category",
                               "Enter a name for the new category:",
                               AlertWindow::NoIcon);
    aw->addTextEditor("name", "", "Category name:");
    aw->addButton("Add",    1, KeyPress(KeyPress::returnKey));
    aw->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

    aw->enterModalState(true,
        ModalCallbackFunction::create([this, aw](int result)
        {
            if (result == 1)
            {
                String name = aw->getTextEditorContents("name").trim();
                if (name.isNotEmpty() && !allCategories.contains(name))
                {
                    allCategories.add(name);
                    allCategories.sort(true);
                    saveMetadata();
                    updateCategoryList();
                }
            }
            delete aw;
        }),
        true);
}

void PresetBrowser::deleteCategory()
{
    int row = categoryBox->getSelectedRow();
    if (row <= 0) return;  // "All" (index 0) cannot be deleted

    String cat = categoryModel.items[row];

    // Unassign this category from every preset that had it
    for (auto& pair : metadata)
        if (pair.second.category == cat)
            pair.second.category = "";

    allCategories.removeString(cat);
    selectedCategory = "All";
    saveMetadata();
    updateCategoryList();
    updateFilteredPresets();
}

void PresetBrowser::assignCategory()
{
    int row = listBox->getSelectedRow();
    if (row < 0 || row >= filteredPresets.size()) return;

    String presetName = filteredPresets[row];

    PopupMenu menu;
    menu.addItem(1, "(None)");
    menu.addSeparator();
    for (int i = 0; i < allCategories.size(); i++)
        menu.addItem(i + 2, allCategories[i]);

    menu.showMenuAsync(PopupMenu::Options().withTargetComponent(assignCategoryButton.get()),
        [this, presetName](int result)
        {
            if (result == 0) return;

            if (result == 1)
                metadata[presetName].category = "";
            else
                metadata[presetName].category = allCategories[result - 2];

            saveMetadata();
            updateDetailPanel(presetName);

            if (selectedCategory != "All")
                updateFilteredPresets();
            else
            {
                int r = filteredPresets.indexOf(presetName);
                if (r >= 0) listBox->repaintRow(r);
            }
        });
}
