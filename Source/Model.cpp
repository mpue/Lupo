#include "Model.h"

Model::Model() {
    // Init simple components
    lfo1 = std::make_unique<MultimodeOscillator>();
    lfo2 = std::make_unique<MultimodeOscillator>();
   
    filter = std::make_unique<Filter>();

    // Init modulation envelopes (3 Stück)
    for (int i = 0; i < 3; ++i) {
        modEnvelopes.push_back(std::make_unique<SynthLab::ADSR>());
    }

    // Init voices – z. B. 8 Stimmen
    for (int i = 0; i < 8; ++i) {
        voices.push_back(new Voice(48000)); // Ownership bleibt extern geregelt
    }
}

MultimodeOscillator* Model::getLfo1() const {
    return lfo1.get();
}

MultimodeOscillator* Model::getLfo2() const {
    return lfo2.get();
}


Filter* Model::getFilter() const {
    return filter.get();
}

std::vector<std::unique_ptr<SynthLab::ADSR>>& Model::getModEnvelopes() {
    return modEnvelopes;
}

std::vector<Voice*>& Model::getVoices() {
    return voices;
}

int Model::getModSource(int index) const {
    return modSource[index];
}

int Model::getModTarget(int index) const {
    return modTarget[index];
}

void Model::setModSource(int index, int source) {
    modSource[index] = source;
}

void Model::setModTarget(int index, int target) {
    modTarget[index] = target;
}
