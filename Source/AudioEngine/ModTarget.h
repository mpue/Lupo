#ifndef ModTarget_hpp
#define ModTarget_hpp

#include <vector>
#include <algorithm>
#include "Modulator.h"

class ModTarget {
public:
    virtual ~ModTarget() = default;

    // PWM modulator interface (pure virtual)
    virtual void addPwmModulator(std::shared_ptr<Modulator> mod)  {
        pwmModulators.push_back(mod);
    }

    virtual void removePwmModulator(std::shared_ptr<Modulator> mod)  {
        pwmModulators.erase(
            std::remove(pwmModulators.begin(), pwmModulators.end(), mod),
            pwmModulators.end()
        );
    }

    // General modulator interface (has default implementation)
    virtual void addModulator(std::shared_ptr<Modulator> mod) {
        modulators.push_back(mod);
    }

    virtual void removeModulator(std::shared_ptr<Modulator> mod) {
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
    std::vector<std::shared_ptr<Modulator>> modulators;
    std::vector<std::shared_ptr<Modulator>> pwmModulators;
};

#endif /* ModTarget_hpp */