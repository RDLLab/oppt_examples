




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
#ifndef _TIGER_PROBLEM_UTILS_HPP_
#define _TIGER_PROBLEM_UTILS_HPP_
#include "oppt/plugin/PluginOptions.hpp"

namespace oppt
{


// DEFINE THE ENCODED VALUE FOR BOTH STATES
enum TIGER_STATES_INFO{TIGER_STATE_INDEX = 0};
enum TIGER_ACTIONS_INFO{TIGER_ACTION_INDEX = 0};
enum TIGER_STATES{TIGER_LEFT = 0, TIGER_RIGHT = 1}; 
enum TIGER_PROBLEM_ACTIONS{OPEN_LEFT = 0, OPEN_RIGHT = 1, LISTEN = 2};



// Options object to parse information from configuration file
class TigerProblemGeneralOptions: public PluginOptions
{
public:
    TigerProblemGeneralOptions() = default;

    virtual ~TigerProblemGeneralOptions() = default;


    // Initial belief variables
    VectorFloat upperBound;
    VectorFloat lowerBound;
    
    // Transition variables
    FloatType observationNoise = 0.0;
    // VectorUInt actionSpaceDiscretization;




    // Reward variables
    FloatType correctGuessReward = 0.0;
    FloatType wrongGuessPenalty = 0.0;
    FloatType stepPenalty = 0.0; 




    static std::unique_ptr<options::OptionParser> makeParser() {
        std::unique_ptr<options::OptionParser> parser =
            PluginOptions::makeParser();
        addGeneralPluginOptions(parser.get());
        return std::move(parser);
    }

    // Add the transition plugin options
    static void addGeneralPluginOptions(options::OptionParser* parser) {


     
        /*** Initial belief options ***/
        // Lower starting bound
        parser->addOption<VectorFloat>("initialBeliefOptions",
                                       "lowerBound",
                                       &TigerProblemGeneralOptions::lowerBound);
        // Upper starting bound
	     parser->addOption<VectorFloat>("initialBeliefOptions",
                                       "upperBound",
                                       &TigerProblemGeneralOptions::upperBound);


    	parser->addOption<FloatType>("observationPluginOptions",
                                         "observationNoise",
                                         &TigerProblemGeneralOptions::observationNoise);



        /*** Reward Plugin options ***/
        parser->addOption<FloatType>("rewardPluginOptions",
                                         "correctGuessReward",
                                         &TigerProblemGeneralOptions::correctGuessReward);

        parser->addOption<FloatType>("rewardPluginOptions",
                                         "wrongGuessPenalty",
                                         &TigerProblemGeneralOptions::wrongGuessPenalty);

        parser->addOption<FloatType>("rewardPluginOptions",
                                         "stepPenalty",
                                         &TigerProblemGeneralOptions::stepPenalty);


    }

};




/*** HELPER FUNCTIONS ***/
// Computes the magnitude of the given vector
inline FloatType getMagnitude(VectorFloat& vec) {
    FloatType result = 0.0;

    for(auto& component : vec){
        result += (component*component);
    }
    return sqrt(result);
}



/*** Function to retrieve either the scoped(0) or unscoped(1) part of a link or collision object name ***/
std::string getScopingIndexName(std::string name, int index){
    std::string result;
        if (name.find("::") != std::string::npos) {
            VectorString nameElems;
            split(name, "::", nameElems);
            // result = nameElems[index];
            std::vector<std::string> list;
            list.push_back(nameElems[0]);
            list.push_back(nameElems[1]);

            std::string joined = boost::algorithm::join(list, "::");

            result = joined;
        }
    return result;
}


















}

#endif

