# - Find the FFTW library
#
# Usage:
#   find_package(FFTW [REQUIRED] [QUIET] )
#
# It sets the following variables:
#   FFTW_FOUND               ... true if fftw is found on the system
#   FFTW_LIBRARIES           ... full path to fftw library
#   FFTW_INCLUDES            ... fftw include directory
#
# The following variables will be checked by the function
#   FFTW_USE_STATIC_LIBS    ... if true, only static libraries are found
#   FFTW_ROOT               ... if set, the libraries are exclusively searched
#                               under this path
#   FFTW_LIBRARY            ... fftw library to use
#   FFTW_INCLUDE_DIR        ... fftw include directory
#

#If environment variable FFTWDIR is specified, it has same effect as FFTW_ROOT
if( NOT FFTW_ROOT AND EXISTS "$ENV{FFTW_ROOT}")
  set( FFTW_ROOT $ENV{FFTW_ROOT} )
endif()

# Check if we can use PkgConfig
find_package(PkgConfig)

#Determine from PKG
if( PKG_CONFIG_FOUND AND NOT FFTW_ROOT )
  pkg_check_modules( PKG_FFTW QUIET "fftw3" )
endif()

set( CMAKE_FIND_LIBRARY_SUFFIXES_SAV ${CMAKE_FIND_LIBRARY_SUFFIXES} )
set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX}  ${CMAKE_SHARED_LIBRARY_SUFFIX} )

include(CheckTypeSize)
CHECK_TYPE_SIZE("void*" SIZE_OF_VOIDP)
if ("${SIZE_OF_VOIDP}" EQUAL 8)
  set(MKL_LIB_DIR_SUFFIX "intel64")
else()
  set(MKL_LIB_DIR_SUFFIX "ia32")
endif()

if( FFTW_ROOT )
  message( "Searching for FFTW in " ${FFTW_ROOT})

  #find libs
  find_library(
    FFTW_LIB
    NAMES "fftw3" "mkl_rt"
    PATHS ${FFTW_ROOT}
    PATH_SUFFIXES "lib" "lib64" "lib/${MKL_LIB_DIR_SUFFIX}"
    NO_DEFAULT_PATH
  )

  find_library(
    FFTWF_LIB
    NAMES "fftw3f" "mkl_rt"
    PATHS ${FFTW_ROOT}
    PATH_SUFFIXES "lib" "lib64" "lib/${MKL_LIB_DIR_SUFFIX}"
    NO_DEFAULT_PATH
  )

  find_library(
    FFTWL_LIB
    NAMES "fftw3l" # N.B. MKL does not support long double precision
    PATHS ${FFTW_ROOT}
    PATH_SUFFIXES "lib" "lib64" "lib/${MKL_LIB_DIR_SUFFIX}"
    NO_DEFAULT_PATH
  )

  #find includes
  find_path(
    FFTW_INCLUDES
    NAMES "fftw3.h"
    PATHS ${FFTW_ROOT}
    PATH_SUFFIXES "include" "include/fftw"
    NO_DEFAULT_PATH
  )

else()

  find_library(
    FFTW_LIB
    NAMES "fftw3"
    PATHS ${PKG_FFTW_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
  )

  find_library(
    FFTWF_LIB
    NAMES "fftw3f"
    PATHS ${PKG_FFTW_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
  )

  find_library(
    FFTWL_LIB
    NAMES "fftw3l"
    PATHS ${PKG_FFTW_LIBRARY_DIRS} ${LIB_INSTALL_DIR}
  )

  find_path(
    FFTW_INCLUDES
    NAMES "fftw3.h"
    PATHS ${PKG_FFTW_INCLUDE_DIRS} ${INCLUDE_INSTALL_DIR}
  )

endif( FFTW_ROOT )

set( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAV} )

if(FFTWF_LIB)
    message(STATUS "${Cyan}Found single precision FFTW: ${FFTWF_LIB}${ColourReset}")
    add_library(fftw::fftwf INTERFACE IMPORTED)
    set_property(TARGET fftw::fftwf PROPERTY INTERFACE_LINK_LIBRARIES ${FFTWF_LIB})
    set_property(TARGET fftw::fftwf PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${FFTW_INCLUDES})
    set_property(TARGET fftw::fftwf PROPERTY INTERFACE_COMPILE_DEFINITIONS HAVE_FFTWF=1)
else()
    message(STATUS "${Red}Single precision FFTW not found${ColourReset}")
endif()

if(FFTW_LIB)
    message(STATUS "${Cyan}Found double precision FFTW: ${FFTW_LIB}${ColourReset}")
    add_library(fftw::fftwd INTERFACE IMPORTED)
    set_property(TARGET fftw::fftwd PROPERTY INTERFACE_LINK_LIBRARIES ${FFTW_LIB})
    set_property(TARGET fftw::fftwd PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${FFTW_INCLUDES})
    set_property(TARGET fftw::fftwd PROPERTY INTERFACE_COMPILE_DEFINITIONS HAVE_FFTWD=1)
else()
    message(STATUS "${Red}Double precision FFTW not found${ColourReset}")
endif()

if(FFTWL_LIB)
    message(STATUS "${Cyan}Found long double precision FFTW: ${FFTWL_LIB}${ColourReset}")
    add_library(fftw::fftwl INTERFACE IMPORTED)
    set_property(TARGET fftw::fftwl PROPERTY INTERFACE_LINK_LIBRARIES ${FFTWL_LIB})
    set_property(TARGET fftw::fftwl PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${FFTW_INCLUDES})
    set_property(TARGET fftw::fftwl PROPERTY INTERFACE_COMPILE_DEFINITIONS HAVE_FFTWL=1)
else()
    message(STATUS "${Red}Long double precision FFTW not found${ColourReset}")
endif()

# not the official target name and no separate F/D/L targets
#   looks like this is done properly upstream, just no one distributes the pkg
#   build from cmake
add_library(fftw::fftw INTERFACE IMPORTED)
if(TARGET fftw::fftwd)
    set_property(TARGET fftw::fftw APPEND PROPERTY INTERFACE_LINK_LIBRARIES fftw::fftwd)
    set(FFTW_LIBRARIES ${FFTW_LIBRARIES} $<TARGET_PROPERTY:fftw::fftwd,INTERFACE_LINK_LIBRARIES>)
endif()
if(TARGET fftw::fftwf)
    set_property(TARGET fftw::fftw APPEND PROPERTY INTERFACE_LINK_LIBRARIES fftw::fftwf)
    set(FFTW_LIBRARIES ${FFTW_LIBRARIES} $<TARGET_PROPERTY:fftw::fftwf,INTERFACE_LINK_LIBRARIES>)
endif()
if(TARGET fftw::fftwl)
    set_property(TARGET fftw::fftw APPEND PROPERTY INTERFACE_LINK_LIBRARIES fftw::fftwl)
    set(FFTW_LIBRARIES ${FFTW_LIBRARIES} $<TARGET_PROPERTY:fftw::fftwl,INTERFACE_LINK_LIBRARIES>)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFTW DEFAULT_MSG
                                  FFTW_INCLUDES FFTW_LIBRARIES)
mark_as_advanced(FFTW_INCLUDES FFTW_LIBRARIES FFTW_LIB FFTWF_LIB FFTWL_LIB)
