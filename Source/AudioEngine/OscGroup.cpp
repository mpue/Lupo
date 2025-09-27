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

void OscGroup::addTarget(ModTarget * target)
{
	targets.push_back(target);
}

std::vector<ModTarget*> OscGroup::getTargets()
{
	return targets;
}

void OscGroup::processModulation()
{

}


