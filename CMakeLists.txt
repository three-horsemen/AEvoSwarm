cmake_minimum_required(VERSION 2.6)
project(aevo)

add_compile_options(-std=c++11)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.54 COMPONENTS log log_setup thread system date_time REQUIRED)
find_package(MPI REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
		include
		${Boost_INCLUDE_DIRS}
		${MPI_INCLUDE_PATH}
)

#find_path(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)
#find_library(SQLITE3_LIBRARY NAMES sqlite3)

###Shared module
add_library(shared
		src/shared/logger.cpp
		src/shared/Utils.cpp
		src/agent/AgentCharacter.cpp
		)
target_link_libraries(
		shared
		${Boost_LIBRARIES}
		pthread
)

add_executable(
		unit_test_logger
		src/unit_test/shared/logger.cpp
)
target_link_libraries(
		unit_test_logger
		shared
)

###Brain module
add_library(brain
		src/brain/NeuralNetwork.cpp
		src/brain/FullyConnectedLayer.cpp
		src/brain/Neuron.cpp
		src/brain/InputLayer.cpp
		src/brain/OutputLayer.cpp
		src/brain/Layer.cpp
		)
target_link_libraries(
		brain
		shared
)

###Environment Module
add_library(environment
		src/environment/Coordinate.cpp
		src/environment/Tile.cpp
		src/environment/Energy.cpp
		src/environment/Environment.cpp
		)
target_link_libraries(
		environment
		shared
)

###Agent module
add_library(agent
		src/agent/Agent.cpp
		src/agent/AgentCharacter.cpp
		src/agent/PerceptiveField.cpp include/agent/PerceptiveField.hpp)
target_link_libraries(
		agent
		brain
		environment
)

###Ant module
add_library(ant
		src/agent/Ant.cpp
		)
target_link_libraries(
		ant
		agent
)

###UI module
add_library(ui
		src/ui/AsciiEnvironment.cpp
		src/ui/OpenCV.cpp
		src/ui/OpenCVTile.cpp
		)
target_link_libraries(
		ui
		ant
		${OpenCV_LIBS}
)

###Ant World module
add_library(ant_world
		src/ant_world/AntWorld.cpp
		)
target_link_libraries(
		ant_world
		ui
		${MPI_LIBRARIES}
)


###Unit tests
add_executable(
		unit_test_mpi_map_comm
		src/unit_test/environment/unit_test_mpi_map_comm.cpp
)
target_link_libraries(
		unit_test_mpi_map_comm
		${MPI_LIBRARIES}
		ui
)

add_executable(
		unit_test_neural_network
		src/unit_test/brain/unit_test_neural_network.cpp
)
target_link_libraries(
		unit_test_neural_network
		brain
)

add_executable(
		unit_test_ant_action
		src/unit_test/agent/unit_test_ant_action.cpp
)
target_link_libraries(
		unit_test_ant_action
		ant_world
)

add_executable(
		unit_test_asciienvironment
		src/unit_test/ui/asciienvironment.cpp
)
target_link_libraries(
		unit_test_asciienvironment
		ui
)
add_executable(unit_test_controlant
		src/unit_test/ui/controlant.cpp
		)
target_link_libraries(
		unit_test_controlant
		ui
)

add_executable(
		playback_checkpoints
		src/unit_test/ui/playback_checkpoints.cpp
)
target_link_libraries(
		playback_checkpoints
		ant_world
)

add_executable(
		unit_test_ant_world
		src/unit_test/ant_world/ant_world.cpp
)
target_link_libraries(
		unit_test_ant_world
		ant_world
)

add_executable(
		unit_test_ant_world_mpi
		src/unit_test/ant_world/ant_world_mpi.cpp
)
target_link_libraries(
		unit_test_ant_world_mpi
		ant_world
)