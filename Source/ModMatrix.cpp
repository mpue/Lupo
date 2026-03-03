/*
  ==============================================================================

    ModMatrix.cpp
    Created: 10 Dec 2016 10:18:41am
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "ModMatrix.h"

ModMatrix::ModMatrix() {
    this->modSources = new std::map<int,String>();
    this->modTargets = new std::map<int,String>();
    this->modulations = new Modulation*[6 + 128]();  // zero-initialize all pointers
}

ModMatrix::~ModMatrix() {
	
    removeAllChangeListeners();

    // Clean up modulations array
    for (int i = 0; i < 6 + 128; ++i) {
        delete modulations[i];
    }
    delete[] modulations;

    modSources->clear();
    modTargets->clear();
    delete modSources;
    delete modTargets;
}

void ModMatrix::addModulator(std::shared_ptr<Modulator> m)
{
	modulators.push_back(m);
}

void ModMatrix::addModTarget(std::shared_ptr <ModTarget> m)
{
	targets.push_back(m);
}

void ModMatrix::registerSource(String source, int id) {
    this->modSources->insert(std::make_pair(id,source));
}

void ModMatrix::registerTarget(String target, int id) {
    this->modTargets->insert(std::make_pair(id,target));
}

map<int,String>* ModMatrix::getSourceNames() {
    return this->modSources;
}

map<int,String>* ModMatrix::getTargetNames() {
    return this->modTargets;
}

double ModMatrix::getSampleRate(){
    return this->sampleRate;
}

void ModMatrix::setSampleRate(double rate) {
    this->sampleRate = rate;
}

Model* ModMatrix::getModel() {
    return this->model;
}

void ModMatrix::setModel(Model *model) {
    this->model = model;
}
