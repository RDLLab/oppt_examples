## OPPT_EXAMPLES REPOSITORY
This repository is meant for the testing and development of example problems implemented using oppt


## Repository Structure
- cfg/ : This directory will contain the config files for OPPT problem implementations
- models/ : This directory will contain the environment model files used for OPPT problem implementations
- src/plugins/ : This directory will contain the example plugins used for OPPT problem implementations


## Compiling the plugins

** If oppt was installed outside of a systems library:**
If <oppt_install_location> used for the installation of oppt was not in a systems library, 
compiling plugins separately might present the problem of not being able to find the oppt libraries. 

To get around this issue, we can include installed oppt libraries by setting the environment variable "oppt_DIR"
```
export oppt_DIR=<oppt_install_location>/lib/cmake/oppt
```


The separate compilation of the plugins can then be done by:
```
cd <oppt_repo_location>/src/plugins
mkdir build
cd build

# Have to set the CMAKE_INSTALL_DATADIR flag to select install dir
cmake -DCMAKE_INSTALL_DATADIR=${PWD} ..
make -j$(nproc) # This option uses more cores to compile
make install


# In this example
PWD is set to ${HOME}/oppt_examples/src/plugins/build/oppt/plugins
```	

## Setting OPPT References to the compiled plugins
Make sure that the ${OPPT_RESOURCE_PATH} environment variable points to the location of the plugins
```
## TIGER PROBLEM OPPT MODELS AND RESOURCES
export TIGER_PLUGINS=${HOME}/oppt_examples/src/plugins/build/oppt/plugins

# MODELS
export TIGER_MODELS=${HOME}/oppt_examples/models
export GAZEBO_MODEL_PATH=${GAZEBO_MODEL_PATH}:${TIGER_MODELS}
export GAZEBO_MODEL_DATABASE_URI=

# APPEND TO OPPT RESOURCE PATH
export OPPT_RESOURCE_PATH=${OPPT_RESOURCE_PATH}:${TIGER_PLUGINS}
```