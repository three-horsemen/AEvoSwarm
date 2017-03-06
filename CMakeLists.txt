cmake_minimum_required(VERSION 2.6)
project(sftp)

add_compile_options(-std=c++11)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.54 COMPONENTS log log_setup thread system date_time REQUIRED)

include_directories(
		include
		${Boost_INCLUDE_DIRS}
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

###Map Module
add_library(map
		src/map/Coordinate.cpp
		src/map/Tile.cpp
		src/map/Energy.cpp
		src/map/Map.cpp
		)
target_link_libraries(
		map
		agent
		shared
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
		map
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
		src/ui/AsciiMap.cpp
		)
target_link_libraries(
		ui
		map
		agent
)

add_executable(
		unit_test_asciimap
		src/unit_test/ui/asciimap.cpp
)
target_link_libraries(
		unit_test_asciimap
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