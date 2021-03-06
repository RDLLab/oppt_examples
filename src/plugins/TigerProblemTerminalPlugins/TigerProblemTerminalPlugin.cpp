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
#include "oppt/gazeboInterface/GazeboInterface.hpp"
#include "../TigerProblemUtils/TigerProblemGeneralUtils.hpp"

namespace oppt
{
class TigerProblemTerminalPlugin: public TerminalPlugin
{
public :
    TigerProblemTerminalPlugin():
        TerminalPlugin() {

    }

    virtual ~TigerProblemTerminalPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        parseOptions_<TigerProblemGeneralOptions>(optionsFile);
        // Save pointer to options file
        generalOptions_ = static_cast<TigerProblemGeneralOptions*>(options_.get());
        robotEnvironment_ = robotEnvironment_;
    
        return true;
    }

    virtual ValidityReportSharedPtr isValid(const PropagationResultSharedPtr& propagationResult) override {
        ValidityReportSharedPtr validityReport(new ValidityReport(propagationResult->nextState));
        validityReport->satisfiesConstraints = true;
        validityReport->isValid = true;
       

        return validityReport;
    }

    virtual bool isTerminal(const PropagationResultSharedPtr& propagationResult) override {  
        
        /*** Need to guard against nullptr since Terminal Plugin is used initially.
        // Check in ProblemEnvironment.hpp
        // run(const unsigned int& run, std::ofstream & os, int argc, char const * argv[])
        ***/
        if(propagationResult->action != nullptr){
            // Retrieve the actionVec
            VectorFloat actionVec = propagationResult->action->as<VectorAction>()->asVector(); 
             // Check if an open action was taken 
            if(actionVec[TIGER_ACTIONS_INFO::TIGER_ACTION_INDEX] != TIGER_PROBLEM_ACTIONS::LISTEN){
                // The problem is over after an open action. We consider this terminal
                return true;
            }
        }
     
        
        
        // No Open action was taken
        return false;
    }

private:

    TigerProblemGeneralOptions* generalOptions_;

private:
  

};

OPPT_REGISTER_TERMINAL_PLUGIN(TigerProblemTerminalPlugin)

}




