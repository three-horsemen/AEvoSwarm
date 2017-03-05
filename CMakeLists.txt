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
		src/shared/Utils.cpp include/shared/Utils.hpp
		src/shared/AntCharacter.cpp
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

###Map Module
add_library(map
		src/map/Coordinate.cpp
		src/map/Tile.cpp
		src/map/Energy.cpp
		src/map/Map.cpp
		)
target_link_libraries(
		map
		shared
)

###UI Module
add_library(ui
		src/ui/AsciiMap.cpp
		)
target_link_libraries(
		ui
		map
)

add_executable(
		unit_test_asciimap
		src/unit_test/ui/asciimap.cpp
)
target_link_libraries(
		unit_test_asciimap
		ui
)