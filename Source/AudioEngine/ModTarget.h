//
//  ModTarget.hpp
//  Trio
//
//  Created by Matthias Pueski on 09.12.16.
//
//

#ifndef ModTarget_hpp
#define ModTarget_hpp

#include <stdio.h>
#include "Modulator.h"

class ModTarget {

public:
    
    virtual ~ModTarget() {};
    
    virtual void addModulator(Modulator* mod) {
		modulators.push_back(mod);
    }

    virtual void removeModulator(Modulator* mod) {
		modulators.erase(std::remove(modulators.begin(), modulators.end(), mod), modulators.end());
    }

    virtual void processModulation() = 0;

protected:
	
	std::vector<Modulator*> modulators;

};


#endif /* ModTarget_hpp */
