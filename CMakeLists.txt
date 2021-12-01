cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)  # 3.3: EP_add(DEPENDS interfacelib
# project_VERSION* variables populated from project(... VERSION x.x.x) string

if(POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

project(MBX
        VERSION 0.2.3
        LANGUAGES C CXX)

set(PN ${PROJECT_NAME})
set(MBX_AUTHORS      "")
set(MBX_DESCRIPTION  "MBX")
set(MBX_URL          "https://github.com/paesanilab/MBX")
set(MBX_LICENSE      "BSD 3-clause")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

############################  Options: Build How?  #############################
#MRR
#SET(USE_OPENMP TRUE CACHE BOOL "Use OpenMP")
OPTION(USE_OPENMP "Use OpenMP" TRUE)
if (USE_OPENMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()
# END MRR
include(psi4OptionsTools)
option_with_flags(ENABLE_XHOST "Enables processor-specific optimization" ON
                  "-xHost" "-march=native")
option_with_default(CMAKE_BUILD_TYPE "Build type (Release or Debug)" Release)
option_with_default(CMAKE_INSTALL_LIBDIR "Directory to which libraries installed" "lib")
option_with_default(CMAKE_INSTALL_OBJDIR "Directory to which objects are installed" "obj")
option_with_default(CMAKE_COMPILE_TESTS "Directory to which objects are installed" TRUE)
option_with_default(CMAKE_INSTALL_UNITTESTS "Directory to which unittests are installed" "bin/unittests")
option_with_default(CMAKE_INSTALL_INCLUDEDIR "Directory to which include files are installed" "include")
option_with_default(PYMOD_INSTALL_LIBDIR "Location within CMAKE_INSTALL_LIBDIR to which python modules are installed" /)
option_with_default(ENABLE_GENERIC "Enables mostly static linking of system libraries for shared library" OFF)
option_with_default(MBX_CXX_STANDARD "Specify C++ standard for core MBX" 11)

########################  Process & Validate Options  ##########################
include(GNUInstallDirs)
include(autocmake_safeguards)
include(custom_color_messages)

# <<< Export Config to allow other projects to automagically import this >>>
# GNUInstallDirs "DATADIR" wrong here; CMake search path wants "share".
set(CMAKECONFIG_INSTALL_DIR "share/cmake/${PN}")
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/${PN}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PN}Config.cmake"
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PN}ConfigVersion.cmake
                                 VERSION ${${PN}_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PN}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PN}ConfigVersion.cmake
              cmake/FindFFTW.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})


if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install path" FORCE)
endif()
message(STATUS "MBX install: ${CMAKE_INSTALL_PREFIX}")

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
# add_subdirectory(external/upstream)

# external projects manage their own OpenMP and c++YY flags, so only add to CXX_FLAGS for psi4-core
if (USE_OPENMP)
    include(autocmake_omp)
endif()

include(custom_cxxstandard)
include(custom_static_library)
enable_testing()
################################  Main Project  ################################
include(ExternalProject)
ExternalProject_Add(MBX-core
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
              -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
              -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
              -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
              -DCMAKE_CXX_STANDARD=${MBX_CXX_STANDARD}
              -DCMAKE_CXX_STANDARD_REQUIRED=ON
              -DCMAKE_CXX_EXTENSIONS=OFF
              -DCMAKE_COMPILE_TESTS=${CMAKE_COMPILE_TESTS}
              -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
              -DCMAKE_INSTALL_OBJDIR=${CMAKE_INSTALL_OBJDIR}
              -DCMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}
              -DCMAKE_INSTALL_UNITTESTS=${CMAKE_INSTALL_UNITTESTS}
              -DCMAKE_INSTALL_DATADIR=${CMAKE_INSTALL_DATADIR}
              -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}
              -DCMAKECONFIG_INSTALL_DIR=${CMAKECONFIG_INSTALL_DIR}
              -DLIBC_INTERJECT=${LIBC_INTERJECT}
              -DDESTDIR=${CMAKE_BINARY_DIR}/stage
   CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
   BUILD_ALWAYS 1
   INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install)

add_subdirectory(external)


# <<<  Install  >>>

install(DIRECTORY ${STAGED_INSTALL_PREFIX}/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/
        USE_SOURCE_PERMISSIONS
        PATTERN "*.pyc" EXCLUDE)

