# Observation plugin for kamikazeTrajGen experiments
ADD_OBSERVATION_PLUGIN(TigerProblemObservationPlugin ${CMAKE_CURRENT_LIST_DIR}/TigerProblemObservationPlugin.cpp)
target_include_directories(TigerProblemObservationPlugin PRIVATE ${PROJECT_SOURCE_DIR})
