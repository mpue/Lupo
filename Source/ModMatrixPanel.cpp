//
//  ModMatrixPanel .cpp
//  Synthlab - App
//
//  Created by Matthias Pueski on 30.04.18.
//  Copyright © 2018 Pueski. All rights reserved.
//

#include "ModMatrixPanel.h"
#include "../JuceLibraryCode/JuceHeader.h"

#ifdef JUCE_WINDOWS
#include <Windows.h>
#endif // JUCE_WI

#include <map>

using juce::File;
using juce::WildcardFileFilter;
using juce::TableListBox;
using juce::Viewport;
using juce::Colour;
using juce::String;
using juce::DirectoryContentsList;
using juce::ChangeBroadcaster;
using juce::Graphics;
using juce::ValueTree;
using juce::OutputStream;
using juce::XmlElement;
using juce::XmlDocument;
using juce::ScopedPointer;

ModMatrixPanel::ModMatrixPanel (ModMatrixModel* model)  {
    
    // addMouseListener(this, true);
	table = new TableListBox("modMatrixTable", model);
    table->getHeader().addColumn("Source", 1, 200);
    table->getHeader().addColumn("Destination", 2, 200);
	table->getHeader().addColumn("Amount", 3, 50);
    table->setAutoSizeMenuOptionShown(true);
    table->getHeader().setStretchToFitActive(true);
	table->setOpaque(false);
	
    this->model = model;
	view  = new Viewport();
    view->setViewedComponent(table);
	addAndMakeVisible(view);
    

    table->addMouseListener(this, true);
    
    model->update();
	table->updateContent();
    repaint();
}

ModMatrixPanel::~ModMatrixPanel () {
    
    delete table;
    delete view;

}

void ModMatrixPanel::mouseDrag(const juce::MouseEvent &event) {

}

void ModMatrixPanel::paint(juce::Graphics &g) {
	g.fillAll(Colours::darkgrey);
    // g.fillRect (getLocalBounds());

}

void ModMatrixPanel::resized() {
	if (view != nullptr) {
        view->setSize(getWidth(), getHeight());
		view->resized();
	}
	if (table != nullptr) {
        table->setSize(getWidth(), getHeight() - 30);		
	}
	/*
    if (getParentComponent() != nullptr) {
        setSize(getParentWidth()/2, getParentHeight());
    }
	*/
}

void ModMatrixPanel::changeListenerCallback (ChangeBroadcaster* source) {
    table->updateContent();
}


void ModMatrixPanel::mouseDown(const juce::MouseEvent &event) {


}

void ModMatrixPanel::mouseDoubleClick(const juce::MouseEvent &event) {
    
    if (table->getSelectedRow() > 0) {
    }
    else {
    }

}

//===========================================================================
// Model
//===========================================================================

ModMatrixModel::ModMatrixModel(ModMatrix* matrix) {
	this->matrix = matrix;
}

int ModMatrixModel::getNumRows() {
	return 5;
}

void ModMatrixModel::paintCell (Graphics& g,
                int rowNumber,
                int columnId,
                int width, int height,
                bool rowIsSelected) {
    
    
    
    String text = "";
    
    if (columnId == 1) {
		text = "Source";
    }
    else if (columnId == 2) {
		text = "Target";
    }
	else if (columnId == 3) {
		text = "10";
	}
    
	g.setColour(juce::Colours::white);
	g.drawText(text, 0, 0, width, height, juce::Justification::centredLeft);
    	
}

void ModMatrixModel::comboBoxChanged(ComboBox * comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged->getName().startsWith("Source")) {
		if (comboBoxThatHasChanged->getName().endsWith("0")) {
			matrix->getModulations().at(0)->setModulator(matrix->getModulators().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("1")) {
			matrix->getModulations().at(1)->setModulator(matrix->getModulators().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("2")) {
			matrix->getModulations().at(2)->setModulator(matrix->getModulators().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("3")) {
			matrix->getModulations().at(3)->setModulator(matrix->getModulators().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
	}
	if (comboBoxThatHasChanged->getName().startsWith("Target")) {
		if (comboBoxThatHasChanged->getName().endsWith("0")) {
			matrix->getModulations().at(0)->setTarget(matrix->getModTargets().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("1")) {
			matrix->getModulations().at(1)->setTarget(matrix->getModTargets().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("2")) {
			matrix->getModulations().at(2)->setTarget(matrix->getModTargets().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("3")) {
			matrix->getModulations().at(3)->setTarget(matrix->getModTargets().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
		if (comboBoxThatHasChanged->getName().endsWith("4")) {
			matrix->getModulations().at(4)->setTarget(matrix->getModTargets().at(comboBoxThatHasChanged->getSelectedItemIndex()));
		}
	}
}

void ModMatrixModel::sliderValueChanged(Slider * slider)
{
	if (slider->getName().endsWith("0")) {
		matrix->getModulations().at(0)->getModulator()->setModAmount(slider->getValue());
	}
	else if (slider->getName().endsWith("1")) {
		matrix->getModulations().at(1)->getModulator()->setModAmount(slider->getValue());
	}
	else if (slider->getName().endsWith("2")) {
		matrix->getModulations().at(2)->getModulator()->setModAmount(slider->getValue());
	}
	else if (slider->getName().endsWith("3")) {
		matrix->getModulations().at(3)->getModulator()->setModAmount(slider->getValue());
	}

}


void ModMatrixModel::paintRowBackground (Graphics& g,
                         int rowNumber,
                         int width, int height,
                         bool rowIsSelected) {

    
    if (rowIsSelected) {
        g.setColour(juce::Colours::orange);
    }
    else {
		g.setColour(Colours:: darkgrey);
    }
    
    g.fillRect(0,0,width,height);
	
}

void ModMatrixModel::update() {

}

Component * ModMatrixModel::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component * existingComponentToUpdate)
{
	
	if (columnId == 1) {
		ComboBox* sourceCombo = dynamic_cast<ComboBox*>(existingComponentToUpdate);

		if (sourceCombo == nullptr) {
			sourceCombo = new ComboBox("Source_"+String(rowNumber));

			for (std::map<int, String>::iterator it = matrix->getSources()->begin(); it != matrix->getSources()->end();) {			
				sourceCombo->addItem(it->second, it->first);
				it++;
			}
			sourceCombo->addListener(this);
		}

		return sourceCombo;
	}
	else if (columnId == 2) {
		
		ComboBox* targetCombo = dynamic_cast<ComboBox*>(existingComponentToUpdate);
		
		if (targetCombo == nullptr) {
			targetCombo = new ComboBox("Target_"+String(rowNumber));
			for (std::map<int, String>::iterator it = matrix->getTargets()->begin(); it != matrix->getTargets()->end();) {
				targetCombo->addItem(it->second, it->first);
				it++;
			}
			targetCombo->addListener(this);
		}
		return targetCombo;	
	}

	else if (columnId == 3) {
		
		Slider* amountSlider = dynamic_cast<Slider*>(existingComponentToUpdate);

		if (amountSlider == nullptr) {
			amountSlider = new Slider(Slider::SliderStyle::LinearBar,juce::Slider::NoTextBox);
			amountSlider->setName("Amount_" + String(rowNumber));
			amountSlider->setRange(0, 10, 0.1);
			amountSlider->addListener(this);
		}

		return amountSlider;
		
	}

	else {
		return nullptr;
	}
}


