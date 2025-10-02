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

	virtual void addPwmModulator(std::shared_ptr<Modulator> mod) override {
		for (auto target : targets) {
			target->addPwmModulator(mod);
		}
	};

	virtual void removePwmModulator(std::shared_ptr<Modulator> mod) override {
		for (auto target : targets) {
			target->removePwmModulator(mod);
		}
	};

	virtual void addModulator(std::shared_ptr<Modulator> mod) override {
		for (auto target : targets) {
			target->addModulator(mod);
		}
	}

	virtual void removeModulator(std::shared_ptr<Modulator> mod) override {
		for (auto target : targets) {
			target->removeModulator(mod);
		}
	}

private:

	std::vector<std::shared_ptr<ModTarget>> targets;

	
};