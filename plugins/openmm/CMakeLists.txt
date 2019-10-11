#---------------------------------------------------
# OpenMM MBnrg Plugin
#----------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# We need to know where OpenMM is installed so we can access the headers and libraries.
#SET(OPENMM_DIR "/usr/local/openmm" CACHE PATH "Where OpenMM is installed")
SET(OPENMM_DIR "/home/mrierari/codes/miniconda3/" CACHE PATH "Where OpenMM is installed")
SET(MBNRG_DIR "/home/mrierari/codes/MBX/install" CACHE PATH "Where MBnrg is installed")
SET(PYTHON_EXECUTABLE "/home/mrierari/codes/miniconda3/bin/python3" CACHE PATH "Where the python3 executable is")
SET(CMAKE_INSTALL_PREFIX ${OPENMM_DIR})
INCLUDE_DIRECTORIES("${OPENMM_DIR}/include" "${MBNRG_DIR}/include")
LINK_DIRECTORIES("${OPENMM_DIR}/lib" "${OPENMM_DIR}/lib/plugins" "${MBNRG_DIR}/lib/static")

set(CMAKE_CXX_STANDARD 11)

# Setting some CMAKE arguments:
#set(MBNRG_LINK_FLAGS "-Wl,--whole-archive"
#                     mbnrg 
#                     "-Wl,--no-whole-archive" )

# set flags for linking on mac
IF(APPLE)
    SET (CMAKE_INSTALL_NAME_DIR "@rpath")
    SET(EXTRA_COMPILE_FLAGS "-msse2 -stdlib=libc++")
ENDIF(APPLE)

# Select where to install
IF(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
    IF(WIN32)
        SET(CMAKE_INSTALL_PREFIX "$ENV{ProgramFiles}/OpenMM" CACHE PATH "Where to install the plugin" FORCE)
    ELSE(WIN32)
        SET(CMAKE_INSTALL_PREFIX "${OPENMM_DIR}" CACHE PATH "Where to install the plugin" FORCE)
    ENDIF(WIN32)
ENDIF(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})

SET(OPENMM_MAJOR_VERSION 7 CACHE INTEGER "OpenMM major version, e.g. 7 in 7.2.0")
SET(OPENMM_MINOR_VERSION 2 CACHE INTEGER "OpenMM minor version, e.g. 2 in 7.2.0")

set(EXTRA_COMPILE_FLAGS "${EXTRA_COMPILE_FLAGS} ")
#MRR
find_package(OpenMP)
SET(USE_OPENMP TRUE CACHE BOOL "Use OpenMP")
if (OPENMP_FOUND AND USE_OPENMP)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
# END MRR

# The source is organized into subdirectories, but we handle them all from
# this CMakeLists file rather than letting CMake visit them as SUBDIRS.
#SET(EXAMPLE_PLUGIN_SOURCE_SUBDIRS openmmapi serialization)
SET(MBNRG_PLUGIN_SOURCE_SUBDIRS openmmapi)

# Set the library name
SET(MBNRG_LIBRARY_NAME MBnrgPlugin)
SET(SHARED_MBNRG_TARGET ${MBNRG_LIBRARY_NAME})

# These are all the places to search for header files which are to be part of the API.
SET(API_INCLUDE_DIRS "openmmapi/include" "openmmapi/include/internal")

# Locate header files.
SET(API_INCLUDE_FILES)
FOREACH(dir ${API_INCLUDE_DIRS})
    FILE(GLOB fullpaths ${dir}/*.h)
    SET(API_INCLUDE_FILES ${API_INCLUDE_FILES} ${fullpaths})
ENDFOREACH(dir)

# Locate all .o files in installation
file(GLOB_RECURSE mbnrg_obj_files "${MBNRG_DIR}/obj/*.o")

# Collect up source files
SET(SOURCE_FILES) # empty
SET(SOURCE_INCLUDE_FILES)
FOREACH(subdir ${MBNRG_PLUGIN_SOURCE_SUBDIRS})
    FILE(GLOB src_files  ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/src/*.cpp)
    FILE(GLOB incl_files ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/src/*.h)
    SET(SOURCE_FILES         ${SOURCE_FILES}         ${src_files})   #append
    SET(SOURCE_INCLUDE_FILES ${SOURCE_INCLUDE_FILES} ${incl_files})

    ## Make sure we find these locally before looking in OpenMM/include if
    ## OpenMM was previously installed there.
    INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/include)
ENDFOREACH(subdir)

# Create the library.

ADD_LIBRARY(${SHARED_MBNRG_TARGET} SHARED ${mbnrg_obj_files} ${SOURCE_FILES} ${SOURCE_INCLUDE_FILES} ${API_INCLUDE_FILES})
SET_TARGET_PROPERTIES(${SHARED_MBNRG_TARGET}
    PROPERTIES COMPILE_FLAGS "-DMBNRG_BUILDING_SHARED_LIBRARY ${EXTRA_COMPILE_FLAGS} ${MBNRG_COMPILE_FLAGS}"
    LINK_FLAGS "${EXTRA_COMPILE_FLAGS}")
TARGET_LINK_LIBRARIES(${SHARED_MBNRG_TARGET} OpenMM ${MBNRG_LINK_FLAGS})
INSTALL_TARGETS(/lib RUNTIME_DIRECTORY /lib ${SHARED_MBNRG_TARGET})

# install headers
FILE(GLOB API_ONLY_INCLUDE_FILES "openmmapi/include/*.h")
INSTALL (FILES ${API_ONLY_INCLUDE_FILES} DESTINATION include)
FILE(GLOB API_ONLY_INCLUDE_FILES_INTERNAL "openmmapi/include/internal/*.h")
INSTALL (FILES ${API_ONLY_INCLUDE_FILES_INTERNAL} DESTINATION include/internal)
install(DIRECTORY "${MBNRG_DIR}/include/" DESTINATION include)

# Enable testing

ENABLE_TESTING()
#ADD_SUBDIRECTORY(serialization/tests)

# Build the implementations for different platforms

ADD_SUBDIRECTORY(platforms/reference)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
#FIND_PACKAGE(OpenCL QUIET)
#IF(OPENCL_FOUND)
#    SET(EXAMPLE_BUILD_OPENCL_LIB ON CACHE BOOL "Build implementation for OpenCL")
#ELSE(OPENCL_FOUND)
#    SET(EXAMPLE_BUILD_OPENCL_LIB OFF CACHE BOOL "Build implementation for OpenCL")
#ENDIF(OPENCL_FOUND)
#IF(EXAMPLE_BUILD_OPENCL_LIB)
#    ADD_SUBDIRECTORY(platforms/opencl)
#ENDIF(EXAMPLE_BUILD_OPENCL_LIB)
#
#FIND_PACKAGE(CUDA QUIET)
#IF(CUDA_FOUND)
#    SET(EXAMPLE_BUILD_CUDA_LIB ON CACHE BOOL "Build implementation for CUDA")
#ELSE(CUDA_FOUND)
#    SET(EXAMPLE_BUILD_CUDA_LIB OFF CACHE BOOL "Build implementation for CUDA")
#ENDIF(CUDA_FOUND)
#IF(EXAMPLE_BUILD_CUDA_LIB)
#    ADD_SUBDIRECTORY(platforms/cuda)
#ENDIF(EXAMPLE_BUILD_CUDA_LIB)

# Build the Python API

#FIND_PROGRAM(PYTHON_EXECUTABLE python)
#execute_process(
#  COMMAND bash -c "which python"
#  OUTPUT_VARIABLE PYTHON_EXECUTABLE
#)
string(STRIP ${PYTHON_EXECUTABLE} PYTHON_EXECUTABLE)
FIND_PROGRAM(SWIG_EXECUTABLE swig)
IF(PYTHON_EXECUTABLE AND SWIG_EXECUTABLE)
    SET(EXAMPLE_BUILD_PYTHON_WRAPPERS ON CACHE BOOL "Build wrappers for Python")
ELSE(PYTHON_EXECUTABLE AND SWIG_EXECUTABLE)
    SET(EXAMPLE_BUILD_PYTHON_WRAPPERS OFF CACHE BOOL "Build wrappers for Python")
ENDIF(PYTHON_EXECUTABLE AND SWIG_EXECUTABLE)
IF(EXAMPLE_BUILD_PYTHON_WRAPPERS)
    ADD_SUBDIRECTORY(python)
ENDIF(EXAMPLE_BUILD_PYTHON_WRAPPERS)

