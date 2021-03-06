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
#include "../TigerProblemUtils/TigerProblemGeneralUtils.hpp"
#include <cmath> 

using std::endl;
using std::cout;

namespace oppt
{
class TigerProblemRewardPlugin: public RewardPlugin
{
public :
    TigerProblemRewardPlugin():
        RewardPlugin() {

    }

    virtual ~TigerProblemRewardPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        parseOptions_<TigerProblemGeneralOptions>(optionsFile);
        generalOptions_ = static_cast<TigerProblemGeneralOptions*>(options_.get());

        return true;
    }

    virtual FloatType getReward(const PropagationResultSharedPtr& propagationResult) const override {\

    
        // Retrieve next state (s') from the propagation result
        VectorFloat stateVec = propagationResult->nextState->as<VectorState>()->asVector();
        VectorFloat actionVec = propagationResult->action->as<VectorAction>()->asVector(); 


        // Reward received from the environment
        FloatType reward = 0;


        // Check if there was a correct guess on the left door
        if(actionVec[TIGER_ACTIONS_INFO::TIGER_ACTION_INDEX] == TIGER_PROBLEM_ACTIONS::OPEN_LEFT){
            if(stateVec[TIGER_STATES_INFO::TIGER_STATE_INDEX] == TIGER_STATES::TIGER_LEFT){
                return generalOptions_->correctGuessReward;
            } else {
                return generalOptions_->wrongGuessPenalty;
            }
        }

        // Check if there was a correct guess on the right door
        if(actionVec[TIGER_ACTIONS_INFO::TIGER_ACTION_INDEX] == TIGER_PROBLEM_ACTIONS::OPEN_RIGHT){
            if(stateVec[TIGER_STATES_INFO::TIGER_STATE_INDEX] == TIGER_STATES::TIGER_RIGHT){
                return generalOptions_->correctGuessReward;
            } else {
                return generalOptions_->wrongGuessPenalty;
            }
        }

        // Otherwise, apply a step penalty
        return generalOptions_->stepPenalty;
    }

    virtual std::pair<FloatType, FloatType> getMinMaxReward() const override {
        return std::make_pair(generalOptions_->wrongGuessPenalty,
                              generalOptions_->correctGuessReward);
    }

private:
    TigerProblemGeneralOptions* generalOptions_;

};

OPPT_REGISTER_REWARD_PLUGIN(TigerProblemRewardPlugin)

}
