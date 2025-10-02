#ifndef ModTarget_hpp
#define ModTarget_hpp

#include <vector>
#include <algorithm>
#include "Modulator.h"

class ModTarget {
public:
    virtual ~ModTarget() = default;

    // PWM modulator interface (pure virtual)
    virtual void addPwmModulator(Modulator* mod)  {
        pwmModulators.push_back(mod);
    }

    virtual void removePwmModulator(Modulator* mod)  {
        pwmModulators.erase(
            std::remove(pwmModulators.begin(), pwmModulators.end(), mod),
            pwmModulators.end()
        );
    }

    // General modulator interface (has default implementation)
    virtual void addModulator(Modulator* mod) {
        modulators.push_back(mod);
    }

    virtual void removeModulator(Modulator* mod) {
        modulators.erase(
            std::remove(modulators.begin(), modulators.end(), mod),
            modulators.end()
        );
    }

    virtual void clearModulators() {
        modulators.clear();
    }

    // Process all modulation
    virtual void processModulation() = 0;

protected:
    std::vector<Modulator*> modulators;
    std::vector<Modulator*> pwmModulators;
};

#endif /* ModTarget_hpp */