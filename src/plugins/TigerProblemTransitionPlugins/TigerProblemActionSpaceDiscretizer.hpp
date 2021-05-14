#ifndef _SKD_ACTION_SPACE_DISCRETIZER_
#define _SKD_ACTION_SPACE_DISCRETIZER_
#include "oppt/robotHeaders/ActionSpaceDiscretizer.hpp"
#include "../TigerProblemUtils/TigerProblemGeneralUtils.hpp"

namespace oppt {

class TigerProblemActionSpaceDiscretizer: public ActionSpaceDiscretizer {
public:
	TigerProblemActionSpaceDiscretizer(ActionSpaceSharedPtr &actionSpace):
		ActionSpaceDiscretizer(actionSpace) {
			LOGGING("Custom Tiger Action Space Discretizer");
	}

	virtual ~TigerProblemActionSpaceDiscretizer() {}

	virtual std::vector<ActionSharedPtr> getAllActionsInOrder(const unsigned int& numStepsPerDimension) const {		
		std::vector<ActionSharedPtr> allActionsOrdered_;
		long code = 0;


		// Append open left action representation in vector form
		VectorFloat openLeftActionVec{TIGER_PROBLEM_ACTIONS::OPEN_LEFT};
		ActionSharedPtr openLeftAction(new DiscreteVectorAction(openLeftActionVec));
		openLeftAction->as<DiscreteVectorAction>()->setBinNumber(code);
		code++;
		allActionsOrdered_.push_back(openLeftAction);


		// Print size of action space
		std::cout << "SIZE OF THE ACTION SPACE IS " << allActionsOrdered_.size() << std::endl;
		getchar();


		// Append south west action according to gazebo system
		VectorFloat openRightActionVec{TIGER_PROBLEM_ACTIONS::OPEN_RIGHT};
		ActionSharedPtr openRightAction(new DiscreteVectorAction(openRightActionVec));
		openRightAction->as<DiscreteVectorAction>()->setBinNumber(code);
		code++;
		allActionsOrdered_.push_back(openRightAction);


		// Add the no movement action for the pedestrian
		VectorFloat listenActionVec{TIGER_PROBLEM_ACTIONS::LISTEN};
		ActionSharedPtr listenAction(new DiscreteVectorAction(listenActionVec));
		listenAction->as<DiscreteVectorAction>()->setBinNumber(code);
		code++;
		allActionsOrdered_.push_back(listenAction);	



		// Print out list of actions to verify correctness
		// // Print actions
		std::cout << "PRINTING ACTION SPACE " << std::endl;
		for(auto actions : allActionsOrdered_){
			actions->print(cout);
			std::cout << std::endl;
		}

		getchar();


		return allActionsOrdered_;
	}
};

}

#endif