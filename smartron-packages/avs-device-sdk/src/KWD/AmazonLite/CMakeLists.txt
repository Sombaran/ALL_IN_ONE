cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(AMAZONLITE LANGUAGES CXX)

include(../../build/BuildDefaults.cmake)

if(AMAZONLITE_KEY_WORD_DETECTOR_EMBEDDED_MODEL_CPP_PATH)
	add_definitions(-DAMAZONLITE_USE_EMBEDDED_MODEL)
endif()

add_subdirectory("src")
if(AMAZONLITE_KEY_WORD_DETECTOR_EMBEDDED_MODEL_CPP_PATH)
	add_subdirectory("test")
endif()
