/*
  ==============================================================================

    OscGroup.cpp
    Created: 14 Mar 2020 11:53:07am
    Author:  mpue

  ==============================================================================
*/

#include "OscGroup.h"

OscGroup::OscGroup() {
}

OscGroup::~OscGroup() {
}

void OscGroup::addTarget(std::shared_ptr<ModTarget> target)
{
	targets.push_back(target);
}

std::vector<std::shared_ptr<ModTarget>>& OscGroup::getTargets()
{
	return targets;
}

void OscGroup::processModulation()
{

}


