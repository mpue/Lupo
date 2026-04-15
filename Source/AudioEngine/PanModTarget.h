#pragma once
#include "ModTarget.h"
#include "OscGroup.h"
#include "Oszillator.h"

/**
 * A ModTarget that applies LFO/ENV modulation to the stereo pan position
 * of all oscillators in an OscGroup. basePan is the static pan set by the
 * mixer channel knob; modulator outputs are added on top each audio block.
 */
class PanModTarget : public ModTarget
{
public:
    PanModTarget(OscGroup* group, float basePan)
        : oscGroup(group), basePan(basePan) {}

    void setBasePan(float pan) { basePan = pan; }

    void processModulation() override
    {
        float modSum = 0.0f;
        for (auto& m : modulators)
            modSum += m->getOutput() * m->getModAmount();

        float newPan = juce::jlimit(-1.0f, 1.0f, basePan + modSum);

        for (auto& target : oscGroup->getTargets())
        {
            auto* osc = dynamic_cast<Oszillator*>(target.get());
            if (osc) osc->setPan(newPan);
        }
    }

private:
    OscGroup* oscGroup;
    float     basePan = 0.0f;
};
