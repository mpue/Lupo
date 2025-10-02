/*
  ==============================================================================

    ModTargetGroup.h

    This is a proxy class that groups multiple ModTarget instances together.
	When a modulator is added to the group, it is added to all contained targets.

    Created: 1 Oct 2025 1:30:39pm
    Author:  mpue

  ==============================================================================
*/

#pragma once

#include "ModTarget.h"

class ModTargetGroup : public ModTarget {

public:

    ModTargetGroup() {};
    ~ModTargetGroup() = default;

    void addTarget(ModTarget* target) {
        if (!target || target == this) return;
        if (std::find(targets.begin(), targets.end(), target) == targets.end()) {
            targets.push_back(target);
        }
    }

    void removeTarget(ModTarget* target) {
        targets.erase(std::remove(targets.begin(), targets.end(), target),
            targets.end());
    }

    const std::vector<ModTarget*>& getModTargets() {
        return targets;
    }


    void addModulator(Modulator* mod) override {
        for (auto& target : targets) {
            target->addModulator(mod);
        }
    }

    void removeModulator(Modulator* mod) override {
        for (auto& target : targets) {
            target->removeModulator(mod);
        }
    }
    void clearModulators() override {
        for (auto& target : targets) {
            target->clearModulators();
        }
    }
    void processModulation() override {
        for (auto& target : targets) {
            target->processModulation();
        }
    }

    void addPwmModulator(Modulator* mod) override
    {
        for (auto& target : targets) {
            target->addPwmModulator(mod);
        }
    }

    void removePwmModulator(Modulator* mod) override   {
        for (auto& target : targets) {
            target->removePwmModulator(mod);
        }
    }

protected:
    
    std::vector<ModTarget*> targets;


};
