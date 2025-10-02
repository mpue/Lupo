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
#include <algorithm>

class ModTargetGroup : public ModTarget {
public:
    ModTargetGroup() {};
    ~ModTargetGroup() = default;

    void addTarget(std::shared_ptr<ModTarget> target) {
        if (!target) return;

        auto it = std::find_if(targets.begin(), targets.end(),
            [target](const std::shared_ptr<ModTarget>& existing) {
                return existing.get() == target.get();
            });

        if (it == targets.end()) {
            targets.push_back(target);
        }
    }

    void removeTarget(std::shared_ptr<ModTarget>target) {

        if (target.get() == nullptr) 
            return;  // Null-Check

        targets.erase(
            std::remove_if(targets.begin(), targets.end(),
                [target](std::shared_ptr <ModTarget> t) { return t.get() == target.get(); }),
            targets.end()
        );

    }

    bool hasTarget(ModTarget* target) const {
        // return std::find(targets.begin(), targets.end(), target) != targets.end();
    }

    size_t getTargetCount() const {
        return targets.size();
    }

    void clearTargets() {
        targets.clear();
    }

    const std::vector<shared_ptr<ModTarget>>& getModTargets() const {
        return targets;
    }

    void addModulator(std::shared_ptr<Modulator> mod) override {
        if (mod == nullptr) return;

        for (auto& target : targets) {
            if (target != nullptr) {
                target->addModulator(mod);
            }
        }
    }

    void removeModulator(std::shared_ptr<Modulator> mod) override {
        if (mod == nullptr) return;

        for (auto& target : targets) {
            if (target != nullptr) {
                target->removeModulator(mod);
            }
        }
    }

    void clearModulators() override {
        for (auto& target : targets) {
            if (target != nullptr) {
                target->clearModulators();
            }
        }
    }

    void processModulation() override {
        for (auto& target : targets) {
            if (target != nullptr) {
                target->processModulation();
            }
        }
    }

    void addPwmModulator(std::shared_ptr<Modulator> mod) override {
        if (mod == nullptr) return;

        for (auto& target : targets) {
            if (target != nullptr) {
                target->addPwmModulator(mod);
            }
        }
    }

    void removePwmModulator(std::shared_ptr<Modulator> mod) override {
        if (mod == nullptr) return;

        for (auto& target : targets) {
            if (target != nullptr) {
                target->removePwmModulator(mod);
            }
        }
    }

protected:
    std::vector<shared_ptr<ModTarget>> targets;
};