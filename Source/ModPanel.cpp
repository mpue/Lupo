/*
  ==============================================================================

    ModPanel.cpp
    Matrix-based modulation panel implementation

  ==============================================================================
*/

#include "ModPanel.h"
#include "AttachmentFactory.h"
#include "Model.h"
#include "ModMatrix.h"

//==============================================================================
ModPanel::ModPanel(ModMatrix* matrix, Model* model, AttachmentFactory* factory)
    : factory(factory), model(model), matrix(matrix)
{
    // Get the number of sources and targets from the matrix
    auto sources = matrix->getSourceNames();
    auto targets = matrix->getTargetNames();
    
    numSources = sources ? sources->size() : 0;
    numTargets = targets ? targets->size() : 0;
    
    createMatrix();
    setupLabels();
    
    setSize(LABEL_WIDTH + numTargets * GRID_SIZE + MARGIN_LEFT * 2, 
            LABEL_HEIGHT + numSources * GRID_SIZE + MARGIN_TOP * 2);
}

ModPanel::~ModPanel()
{
}

void ModPanel::paint(juce::Graphics& g)
{
    // g.fillAll(juce::Colours::findColourForName();
    
    // Draw grid lines
    g.setColour(juce::Colours::darkgrey);
    
    int startX = LABEL_WIDTH + MARGIN_LEFT;
    int startY = LABEL_HEIGHT + MARGIN_TOP * 2;
    
    // Vertical grid lines
    for (int i = 0; i <= numTargets; ++i)
    {
        int x = startX + i * GRID_SIZE;
        g.drawLine(x, startY, x, startY + numSources * GRID_SIZE);
    }
    
    // Horizontal grid lines
    for (int i = 0; i <= numSources; ++i)
    {
        int y = startY + i * GRID_SIZE;
        g.drawLine(startX, y, startX + numTargets * GRID_SIZE, y);
    }
    
    // Draw vertical target labels manually
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(10.0f, juce::Font::plain));
    
    auto targets = matrix->getTargetNames();
    if (targets)
    {
        int labelIndex = 0;
        for (const auto& pair : *targets)
        {
            if (labelIndex < numTargets)
            {
                int labelX = startX + labelIndex * GRID_SIZE + GRID_SIZE / 2;
                int labelY = MARGIN_TOP + LABEL_HEIGHT / 2;
                
                // Save the current transform
                juce::Graphics::ScopedSaveState saveState(g);
                
                // Apply rotation and draw text
                g.addTransform(juce::AffineTransform::rotation(
                    -juce::MathConstants<float>::halfPi, labelX, labelY));
                
                g.drawText(pair.second, 
                          labelX - 50, labelY - 6, 
                          100, 12, 
                          juce::Justification::centred);
                
                labelIndex++;
            }
        }
    }
}

void ModPanel::resized()
{
    int startX = LABEL_WIDTH + MARGIN_LEFT;
    int startY = LABEL_HEIGHT + MARGIN_TOP * 2;
    
    // Position source labels (left side)
    for (int i = 0; i < sourceLabels.size(); ++i)
    {
        sourceLabels[i]->setBounds(MARGIN_LEFT, startY + i * GRID_SIZE, 
                                   LABEL_WIDTH - 5, GRID_SIZE);
    }
    
    // Target labels are now drawn directly in the paint method
    // No positioning needed for targetLabels
    
    // Position grid buttons
    for (int row = 0; row < gridButtons.size(); ++row)
    {
        for (int col = 0; col < gridButtons[row].size(); ++col)
        {
            gridButtons[row][col]->setBounds(startX + col * GRID_SIZE + 1, 
                                           startY + row * GRID_SIZE + 1,
                                           GRID_SIZE - 2, GRID_SIZE - 2);
        }
    }
}

void ModPanel::buttonClicked(juce::Button* button)
{
    auto* gridButton = dynamic_cast<GridButton*>(button);
    if (!gridButton) return;
    
    int sourceIndex = gridButton->getRow();
    int targetIndex = gridButton->getColumn();
    
    // Prevent self-modulation
    if (!isValidConnection(sourceIndex, targetIndex))
    {
        gridButton->setToggleState(false, juce::dontSendNotification);
        return;
    }
    
	Modulator* mod = matrix->getModulators().at(sourceIndex);
	ModTarget* target = matrix->getModTargets().at(targetIndex);


    if (targetIndex < 4) {
		// Target is an oscillator pitch
		// iterate over all voices and add/remove modulator to/from the corresponding oscillator

    }

    if (gridButton->getToggleState())
    {
		target->addModulator(mod);
    }
    else
    {
		target->removeModulator(mod); 
	}

}

void ModPanel::initAttachments()
{
    // Matrix-based approach doesn't use traditional parameter attachments
    // Modulation routing is handled directly through button clicks
}

void ModPanel::updateMatrix()
{
}

void ModPanel::createMatrix()
{
    gridButtons.clear();
    gridButtons.resize(numSources);
    
    for (int row = 0; row < numSources; ++row)
    {
        gridButtons[row].resize(numTargets);
        for (int col = 0; col < numTargets; ++col)
        {
            gridButtons[row][col] = std::make_unique<GridButton>(col, row);
            gridButtons[row][col]->addListener(this);
            addAndMakeVisible(gridButtons[row][col].get());
        }
    }
}

void ModPanel::setupLabels()
{
    // Create source labels
    sourceLabels.clear();
    auto sources = matrix->getSourceNames();
    if (sources)
    {
        for (const auto& pair : *sources)
        {
            auto label = std::make_unique<juce::Label>();
            label->setText(pair.second, juce::dontSendNotification);
            label->setJustificationType(juce::Justification::centredRight);
            label->setColour(juce::Label::textColourId, juce::Colours::white);
            label->setFont(juce::Font(11.0f, juce::Font::plain));
            addAndMakeVisible(label.get());
            sourceLabels.push_back(std::move(label));
        }
    }
    
    // Target labels are now drawn directly in the paint method
    // Clear any existing target labels
    targetLabels.clear();
}

bool ModPanel::isValidConnection(int sourceIndex, int targetIndex)
{
    // Prevent self-modulation: check if the source and target refer to the same object
    const auto& modulators = matrix->getModulators();
    const auto& targets = matrix->getModTargets();
    
    if (sourceIndex >= static_cast<int>(modulators.size()) || targetIndex >= static_cast<int>(targets.size()))
        return false;
        
    // Check if the source modulator is the same object as the target
    // This prevents self-modulation (e.g., LFO1 modulating itself)
    auto* sourceModulator = modulators[sourceIndex];
    auto* targetObject = targets[targetIndex];
    
    // If the source is also a ModTarget, check for identity
    auto* sourceAsModTarget = dynamic_cast<ModTarget*>(sourceModulator);
    if (sourceAsModTarget && sourceAsModTarget == targetObject)
    {
        return false; // Self-modulation not allowed
    }
    
    return true;
}

