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

#ifndef _SKD_KAMIKAZE_OBSERVATION_PLUGIN_HPP_
#define _SKD_KAMIKAZE_OBSERVATION_PLUGIN_HPP_



#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"
#include "TigerProblemObservation.hpp"
#include "../TigerProblemUtils/TigerProblemGeneralUtils.hpp"



namespace oppt
{
class TigerProblemObservationPlugin: public ObservationPlugin
{
public :
    TigerProblemObservationPlugin():
        ObservationPlugin() {
    }

    virtual ~TigerProblemObservationPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        // Parse options files
        parseOptions_<TigerProblemGeneralOptions>(optionsFile);
        generalOptions_ = static_cast<TigerProblemGeneralOptions*>(options_.get());
       
        return true;
    }

    virtual ObservationResultSharedPtr getObservation(const ObservationRequest* observationRequest) const override {
        // Check if the listen action was executed
        // Container for the results information
        ObservationResultSharedPtr observationResult = std::make_shared<ObservationResult>();
        observationResult->state = observationRequest->currentState.get();
        observationResult->action = observationRequest->action;
        auto robot = robotEnvironment_->getRobot();

        // Retrieve state information from the observation request
        VectorFloat stateVector = observationRequest->currentState->as<VectorState>()->asVector();
        VectorFloat actionVector = observationRequest->action->as<VectorAction>()->asVector();

        // Create a default observation vector
        VectorFloat observationVector(1, 0);

        // Set sample thresholds based on action taken
        FloatType obsThreshold = 0.5;
        // Check action sampled
        if(actionVector[TIGER_ACTIONS_INFO::TIGER_ACTION_INDEX] == TIGER_PROBLEM_ACTIONS::LISTEN){
            // std::cout << "Testing observationNoise from generalOptions_" << std::endl;
            // std::cout << generalOptions_->observationNoise << std::endl;
            // getchar();
            obsThreshold = 1 - generalOptions_->observationNoise;
        } else {
            obsThreshold = 0.5;
        }


        // Populate observation value according to action taken
        // Sample from uniform distribution to get noisy observation from state and action
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed1);
        std::uniform_real_distribution<double> distribution(0, 1);
        // Pack it into the oppt structures
        FloatType tigerObsSample = (FloatType) distribution(generator);

        // Change according to sample
        if(tigerObsSample >= obsThreshold){
            // Match the tiger in the true state
            observationVector[TIGER_STATES_INFO::TIGER_STATE_INDEX] = stateVector[TIGER_STATES_INFO::TIGER_STATE_INDEX];
        } else{
            if(stateVector[TIGER_STATES_INFO::TIGER_STATE_INDEX] == TIGER_STATES::TIGER_LEFT){
                // Return alternative observation
                observationVector[TIGER_STATES_INFO::TIGER_STATE_INDEX] = TIGER_STATES::TIGER_RIGHT;
            } else {
                observationVector[TIGER_STATES_INFO::TIGER_STATE_INDEX] = TIGER_STATES::TIGER_LEFT;
            }
        }


        // Wrap the observation into the corresponding object
        observationResult->observation = std::make_shared<TigerProblemObservation>(observationVector);
        

        // Add the observation error to the observationVector
        return observationResult;
    }

    virtual FloatType calcLikelihood(const RobotStateSharedPtr& state,
                                     const Action *action,
                                     const Observation *observation) const  override{
        // Get the nominal observation
        VectorFloat stateVector = state->as<VectorState>()->asVector();
        VectorFloat obsVector = observation->as<VectorObservation>()->asVector();
        VectorFloat actionVector = action->as<VectorAction>()->asVector();

        // Check if the action was a LISTEN ACTION
        if(actionVector[TIGER_STATES_INFO::TIGER_STATE_INDEX] == TIGER_PROBLEM_ACTIONS::LISTEN){
            if(obsVector[TIGER_STATES_INFO::TIGER_STATE_INDEX] == stateVector[TIGER_STATES_INFO::TIGER_STATE_INDEX]){
                // Listen was accurate
                return 1.0 - generalOptions_->observationNoise;
            } else {
                // Listening noise
                return generalOptions_->observationNoise;
            }
        }
       

        // If action was not listen. Return equal likelihood
        return 0.5;
    }

  

    

private:
    TigerProblemGeneralOptions* generalOptions_;

};

OPPT_REGISTER_OBSERVATION_PLUGIN(TigerProblemObservationPlugin)

}

#endif
