
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(freegame)

message(STATUS "cmake orange src dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "cmake orange bin dir: " ${PROJECT_BINARY_DIR})

##################################################
#prepare
##################################################
SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
include(common)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ORANGE_GAME true)
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)

##################################################
###
##################################################

add_subdirectory(src)
add_subdirectory(third_party)

if(NOT IOS)
	add_subdirectory(game/client)
endif()
