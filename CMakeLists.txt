cmake_minimum_required(VERSION 2.6)
project(sftp)

add_compile_options(-std=c++11)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.54 COMPONENTS log log_setup thread system date_time REQUIRED)

find_package(MPI REQUIRED)

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
		${Boost_LIBRARIES}
		pthread
		shared
)

add_executable(
		unit_test_neural_network
		src/unit_test/brain/unit_test_neural_network.cpp
)
target_link_libraries(
		unit_test_neural_network
		brain
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
		agent
		shared
)

add_executable(
		unit_test_mpi_map_comm
		src/unit_test/map/unit_test_mpi_map_comm.cpp
)
target_link_libraries(
		unit_test_mpi_map_comm
		environment
		${MPI_LIBRARIES}
)


###Agent module
add_library(agent
		src/agent/Agent.cpp
		src/agent/AgentCharacter.cpp
		src/agent/PerceptiveField.cpp include/agent/PerceptiveField.hpp)
target_link_libraries(
		agent
		${Boost_LIBRARIES}
		pthread
		brain
		environment
)

###Ant module
add_library(ant
		src/agent/Ant.cpp
		src/unit_test/agent/unit_test_ant_rules.cpp)
target_link_libraries(
		ant
		${Boost_LIBRARIES}
		pthread
		agent
)

add_executable(
		unit_test_ant_rules
		src/unit_test/agent/unit_test_ant_rules.cpp
)
target_link_libraries(
		unit_test_ant_rules
		ant
		ui
)

###UI module
add_library(ui
		src/ui/AsciiEnvironment.cpp
		)
target_link_libraries(
		ui
		environment
		agent
)

add_executable(
		unit_test_asciienvironment
		src/unit_test/ui/asciienvironment.cpp
)
target_link_libraries(
		unit_test_asciienvironment
		ui
		shared
)
add_executable(unit_test_controlant
		src/unit_test/ui/controlant.cpp
		)
target_link_libraries(
		unit_test_controlant
		ui
		ant
		shared
)