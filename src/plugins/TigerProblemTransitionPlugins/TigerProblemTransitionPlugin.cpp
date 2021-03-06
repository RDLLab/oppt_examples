/**
 * Copyright 2017
 *
 * This file is part of On-line POMDP Planning Toolkit (OPPT).
 * OPPT is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License published by the Free Software Foundation,
 * either version 2 of the License, or (at your option) any later version.
 *
 * OPPT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with OPPT.
 * If not, see http://www.gnu.org/licenses/.
 */
#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"
#include "oppt/gazeboInterface/GazeboInterface.hpp"
#include "../TigerProblemUtils/TigerProblemGeneralUtils.hpp"
#include "TigerProblemActionSpaceDiscretizer.hpp"
#include <boost/timer.hpp>
#include "oppt/opptCore/CollisionObject.hpp"
#include "oppt/opptCore/CollisionRequest.hpp"
#include <random>
#include <chrono>


using std::cout;
using std::endl;


namespace oppt
{
class TigerProblemTransitionPlugin: public TransitionPlugin
{
public:
    TigerProblemTransitionPlugin():
         TransitionPlugin() {
    }

    virtual ~TigerProblemTransitionPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        // Parse options files
        parseOptions_<TigerProblemGeneralOptions>(optionsFile);
        generalOptions_ = static_cast<TigerProblemGeneralOptions*>(options_.get());

        // // Overwrite action space with custom one for individual actions
        auto actionSpace = robotEnvironment_->getRobot()->getActionSpace();
        
      
        std::shared_ptr<ActionSpaceDiscretizer> tigerActionSpaceDiscretizer(new TigerProblemActionSpaceDiscretizer(actionSpace));
        actionSpace->setActionSpaceDiscretizer(tigerActionSpaceDiscretizer);
        
        return true;
    }



    virtual PropagationResultSharedPtr propagateState(const PropagationRequest* propagationRequest) const override {
        

        // Copy information from propagationRequest to propagationResult
        PropagationResultSharedPtr propagationResult(new PropagationResult());
        propagationResult->previousState = propagationRequest->currentState.get();
        propagationResult->action = propagationRequest->action;
       
    
        // Extract information from propagationRequest as vectors
        VectorFloat actionApplied = propagationRequest->action->as<VectorAction>()->asVector();
        VectorFloat stateVector = propagationRequest->currentState->as<VectorState>()->asVector();

        // Initialize the next state to be the same as the previous state by default
        VectorFloat resultingState(stateVector);

        // If an open action was taken, reset the state 
        if(actionApplied[TIGER_ACTIONS_INFO::TIGER_ACTION_INDEX] != TIGER_PROBLEM_ACTIONS::LISTEN){
            // Restart the problem
            // Sample from uniform distribution to make the transition on the intention value
            unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed1);
            std::uniform_real_distribution<double> distribution(0, 1);

            // Pack it into the oppt structures
            FloatType tigerRightSample = (FloatType) distribution(generator);
            // Change according to sample
            if(tigerRightSample < 0.5){
                // Init the state with tiger right
                resultingState[TIGER_STATES_INFO::TIGER_STATE_INDEX] = TIGER_STATES::TIGER_LEFT;
            } else {
                 // Init the state with tiger right
                resultingState[TIGER_STATES_INFO::TIGER_STATE_INDEX] = TIGER_STATES::TIGER_RIGHT;
            }
        }

        // Create a robotState object from resulting state
        RobotStateSharedPtr nextRobotState = std::make_shared<oppt::VectorState>(resultingState);
        propagationResult->nextState = nextRobotState;

  
        return propagationResult;
    }

    virtual Distribution<FloatType>* const getErrorDistribution() const override {
        return errorDistribution_.get();
    }


private:

    std::unique_ptr<Distribution<FloatType>> errorDistribution_;

    // General options variables
    TigerProblemGeneralOptions* generalOptions_;

    std::unordered_map<std::string, gazebo::physics::Link*> linkMapTrans_;
    // Unorder map collision Objects associated with each link
    std::unordered_map<std::string, OpptCollisionObject*> collisionLinkMap_;




private:

    // Clamp a value
    FloatType clamp(const FloatType val, const FloatType bound1, const FloatType bound2) const{
        FloatType min = bound1;
        FloatType max = bound2;

        // Check for max and min bounds
        if(min >= max){
            // Reverse bounds
            min = bound2;
            max = bound1;
        }

        if(val < min){
            return min;
        } else if (val > max){
            return max;
        }

        // Does not exceed bounds
        return val;
    }



    // Create a map to the different useful joints of the robot arms and populate the lsits of joint limits
    void populateLinkMap() {
        // Populate the joint map with their correspoding pointers
        std::vector<gazebo::physics::Link*> links = robotEnvironment_->getGazeboInterface()->getLinks();
        for (auto& currentLink : links) {
            linkMapTrans_.insert(std::pair<std::string, gazebo::physics::Link*>(currentLink->GetName(), currentLink));
        }
    }

    // Gets a sample from a uniform distribution with the support [lower, upper]
    FloatType getUniformDistSample(FloatType lower, FloatType upper) const{

         // Update components of the resulting vector
            // Sample from uniform distribution to make the transition on the intention value
            unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed1); //gen(time(NULL))
           // std::default_random_engine generator;
            std::uniform_real_distribution<FloatType> distribution(lower, upper);
            FloatType sample = (FloatType) distribution(generator);

            // std::cout << " SAMPLE FOR LOWER " << lower << "AND UPPER" << upper << "WAS " << sample << std::endl;
            return sample;
    }





};

OPPT_REGISTER_TRANSITION_PLUGIN(TigerProblemTransitionPlugin)

}
