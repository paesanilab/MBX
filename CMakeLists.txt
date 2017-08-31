# Minimum Version Required
cmake_minimum_required(VERSION 2.8.12)
project(clusters_ultimate)
set(CMAKE_BUILD_TYPE Release)

# CPP flags
add_definitions(-O0 -g -std=c++11 -Wall)

# Include Directories
set(INCLUDES src/bblock src/io_tools src/main src/potential src/tools)
include_directories(${INCLUDES})

# Set sources for libraries
file(GLOB SOURCES_BBLOCK   "src/bblock/*.cpp")
file(GLOB SOURCES_IOTOOLS  "src/io_tools/*.cpp")
file(GLOB SOURCES_POT_1B   "src/potential/1b/*.cpp")

# Generate libraries
add_library(bblock    STATIC ${SOURCES_BBLOCK})
add_library(iotools   STATIC ${SOURCES_IOTOOLS})
add_library(pot1b     STATIC ${SOURCES_POT_1B})

#Set library location
set(LIBS bblock iotools pot1b)
install(TARGETS ${LIBS} DESTINATION "${CMAKE_SOURCE_DIR}/lib")
