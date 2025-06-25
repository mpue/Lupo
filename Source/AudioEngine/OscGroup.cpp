/*
  ==============================================================================

    OscGroup.cpp
    Created: 14 Mar 2020 11:53:07am
    Author:  mpue

  ==============================================================================
*/

#include "OscGroup.h"

OscGroup::OscGroup() {
	modulator = nullptr;
}

OscGroup::~OscGroup() {
}

void OscGroup::setModulator(Modulator * mod)
{
	if (mod == nullptr) {
		return;
	}

	if (modulator == mod) {
		return; // no change
	}

	for (int i = 0; i < targets.size();i++) {
		if (targets.at(i) != nullptr) {
			targets.at(i)->setModulator(mod);			
		}
	}
}

void OscGroup::addTarget(ModTarget * target)
{
	targets.push_back(target);
}

std::vector<ModTarget*> OscGroup::getTargets()
{
	return targets;
}


