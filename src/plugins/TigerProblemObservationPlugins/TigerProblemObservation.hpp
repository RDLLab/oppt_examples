#ifndef _NCAP_COLLISION_OBSERVATION_HPP_
#define _NCAP_COLLISION_OBSERVATION_HPP_
#include <oppt/robotHeaders/Observation.hpp>
#include <limits>

using std::cout;
using std::endl;


namespace oppt {


// ENUMERATED DISCRETE CAR_INTENTIONS
enum OBSERVATION_INFO{REL_LONGIT, REL_HORIZONTAL}; 



class TigerProblemObservation: public VectorObservation {
public:
	TigerProblemObservation(VectorFloat& observationVec):
		VectorObservation(observationVec) {

	}


	virtual ~TigerProblemObservation() {}

	virtual FloatType distanceTo(const Observation& otherObservation) const override {
		// Inside the distance To function
		//Return big distance for different observations (bins). Otherwise, observation is the same
		return 1;
	}


};
}

#endif