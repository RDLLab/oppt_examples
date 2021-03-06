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
#ifndef _KAMIKAZE_TRAJ_GEN_HEURISTIC_PLUGIN_HPP_
#define _KAMIKAZE_TRAJ_GEN_HEURISTIC_PLUGIN_HPP_
#include "oppt/plugin/Plugin.hpp"   

namespace oppt
{
class TigerProblemHeuristicPlugin: public HeuristicPlugin
{
public:
    TigerProblemHeuristicPlugin():
        HeuristicPlugin() {

    }

    virtual ~TigerProblemHeuristicPlugin() {

    }

    virtual bool load(const std::string& optionsFile) override {        
        optionsFile_ = optionsFile;
    

        return true;
    }

  virtual FloatType getHeuristicValue(const HeuristicInfo* heuristicInfo) const override {   
        // Return the same heuristic value
        return 0.5;
    }

  

private:
    // Pointer to structure with parsed options values
    std::string optionsFile_;


};

OPPT_REGISTER_HEURISTIC_PLUGIN(TigerProblemHeuristicPlugin)

}

#endif
