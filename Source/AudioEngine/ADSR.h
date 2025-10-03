/***
* ADSR.h
* Created by Nigel Redmon on 12/18/12.
* EarLevel Engineering: earlevel.com
* Copyright 2012 Nigel Redmon
* For a complete explanation of the ADSR envelope generator and code,
* read the series of articles by the author, starting here:
* http://www.earlevel.com/main/2013/06/01/envelope-generators/
*
*  Improved and revised by Matthias Pueski on 01.06.2016
* 
*/

#ifndef ADRS_h
#define ADRS_h

#include <iostream>
#include "Modulator.h"
#include "ModTarget.h"

#include "../JuceLibraryCode/JuceHeader.h"

using namespace std;

namespace SynthLab {


	class ADSR : public Modulator {
	public:
		ADSR(void);
		~ADSR(void);
		float process(void);
		float getOutput(void) {
			return output;
		}
		int getState(void);
		void gate(int on);
		void setAttackRate(float rate);
		float getAttackRate();
		void setDecayRate(float rate);
		float getDecayRate();
		void setReleaseRate(float rate);
		float getReleaseRate();
		void setSustainLevel(float level);
		float getSustainLevel();
		void setTargetRatioA(float targetRatio);
		void setTargetRatioDR(float targetRatio);
		void reset(void);

		enum envState {
			env_idle = 0,
			env_attack,
			env_decay,
			env_sustain,
			env_release
		};

	protected:
		int state;
		float output;
		float attackRate;
		float decayRate;
		float releaseRate;
		float attackCoef;
		float decayCoef;
		float releaseCoef;
		float sustainLevel;
		float targetRatioA;
		float targetRatioDR;
		float attackBase;
		float decayBase;
		float releaseBase;
		float currentEnvValue;
		int velocity;

		float calcCoef(float rate, float targetRatio);

	};

}

#endif
