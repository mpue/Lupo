/*
  ==============================================================================

    OscGroup.h
    Created: 14 Mar 2020 11:53:07am
    Author:  mpue

  ==============================================================================
*/

#pragma once
#include "MultimodeOscillator.h"
#include "Modulator.h"
#include "ModTarget.h"
#include <vector>

class OscGroup : public ModTarget {

public:
	OscGroup();
	~OscGroup();
	
	void addTarget(std::shared_ptr<ModTarget> target);
	std::vector<std::shared_ptr<ModTarget>>& getTargets();
	
	void processModulation() override;

	virtual void addPwmModulator(Modulator* mod) override {
		for (auto target : targets) {
			target->addPwmModulator(mod);
		}
	};

	virtual void removePwmModulator(Modulator* mod) override {
		for (auto target : targets) {
			target->removePwmModulator(mod);
		}
	};

	virtual void addModulator(Modulator* mod) override {
		this->modulator = mod;
		for (auto target : targets) {
			target->addModulator(mod);
		}
	}

	virtual void removeModulator(Modulator* mod) override {
		this->modulator = nullptr;
		for (auto target : targets) {
			target->removeModulator(mod);
		}
	}

private:

	std::vector<std::shared_ptr<ModTarget>> targets;

	Modulator* modulator = nullptr;
};