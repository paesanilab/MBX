#Add any new tests to this list or the one below!
set( SOURCES_UNITTESTS_TESTS
#    unittest-gas-electrostatics-mbpol.cpp
#    unittest-dispersion-gas-cutoff.cpp
#    unittest-dispersion-pme.cpp
#    unittest-gas-nbodyterms-mbpol.cpp
#    unittest-mathtools.cpp
#    unittest-gas-nopolarization-findif.cpp
#    unittest-gas-withpolarization-findif.cpp
#    unittest-pme-nopolarization.cpp
#    unittest-pme-nopolarization-findif.cpp
#    unittest-pme-withpolarization.cpp
#    unittest-pme-withpolarization-findif.cpp
    unittest-gamma.cpp
#    unittest-pbc-1b-mbpol-findif.cpp
#    unittest-pbc-2bpoly-mbpol-findif.cpp
#    unittest-pbc-dispersion-mbpol-findif.cpp
#    unittest-pbc-3bpoly-mbpol-findif.cpp
#    unittest-pbc-electrostatics-mbpol-findif.cpp
#    unittest-h2o-virial.cpp
#    unittest-dummy-monomer.cpp
#    unittest-co2-monomer.cpp
#    unittest-system.cpp 
#    unittest-systools.cpp
)
    
set( SOURCES_UNITTESTS_MAIN unittest-main.cpp )
set( UNITTEST_DIR ${PROJECT_SOURCE_DIR}/tests/unittests_v0.2.1a)

set( HEADER_DIR ${PROJECT_SOURCE_DIR}/../external/Catch2/single_include )

set( SOURCES_ALL
    ${SOURCES_UNITTESTS_MAIN}
    ${SOURCES_UNITTESTS_TESTS}
)

# Create serial target names.
string( REPLACE ".cpp" "" BASENAMES_UNITTESTS_TESTS "${SOURCES_UNITTESTS_TESTS}" )
set( TARGETS_UNITTESTS_TESTS ${BASENAMES_UNITTESTS_TESTS} )

set( HEADER_DIR ${PROJECT_SOURCE_DIR}/../external/Catch2/single_include )

## Define program targets:
add_library( CatchMain OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCES_UNITTESTS_MAIN} ${HEADER_DIR}/catch.hpp )
target_include_directories(CatchMain PRIVATE ${CMAKE_SOURCE_DIR}/../external/)
#install(TARGETS CatchMain
#        RUNTIME DESTINATION bin/unittests
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib/static)

foreach(t ${TARGETS_UNITTESTS_TESTS} )
    add_executable( ${t} ${CMAKE_CURRENT_SOURCE_DIR}/${t}.cpp $<TARGET_OBJECTS:CatchMain> ${HEADER_DIR}/catch.hpp )
    target_link_libraries( ${t} PRIVATE mbxlib fftw::fftw)
    target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR})
    target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR}/../external/)
    install(TARGETS ${t}
            RUNTIME DESTINATION bin/unittests
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/static)
    add_test(NAME ${t}
             COMMAND ${t} 
             WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin/unittests)
endforeach()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CTestTestfile.cmake
        DESTINATION ${CMAKE_INSTALL_UNITTESTS})



##
## Stuff below shouldn't need to be modified
##
#
#include_directories(${PROJECT_SOURCE_DIR}
#                    ${PROJECT_SOURCE_DIR}/../external/
#                    ${PROJECT_SOURCE_DIR}/potential/electrostatics )
#
#set( SOURCES_UNITTESTS_MAIN unittest-main.cpp )
#
## A function to check that the given hard-coded list contains all headers + sources in the given folder.
#function(CheckFileList LIST_VAR FOLDER)
#  set(MESSAGE " should be added to the variable ${LIST_VAR}")
#  set(MESSAGE "${MESSAGE} in ${CMAKE_CURRENT_LIST_FILE}\n")
#  file(GLOB GLOBBED_LIST "${FOLDER}/*.cpp"
#                         "${FOLDER}/*.hpp")
#  list(REMOVE_ITEM GLOBBED_LIST ${${LIST_VAR}})
#  foreach(EXTRA_ITEM ${GLOBBED_LIST})
#    string(REPLACE "${CATCH_DIR}/" "" RELATIVE_FILE_NAME "${EXTRA_ITEM}")
#    message(AUTHOR_WARNING "The file \"${RELATIVE_FILE_NAME}\"${MESSAGE}")
#  endforeach()
#endfunction()
#
#set( UNITTEST_DIR ${PROJECT_SOURCE_DIR}/tests/unittests )
#set( HEADER_DIR ${PROJECT_SOURCE_DIR}/../external/Catch2/single_include )
#
## Check if all sources are listed; warn if not.
#set( SOURCES_ALL
#    ${SOURCES_UNITTESTS_MAIN}
#    ${SOURCES_UNITTESTS_TESTS}
#)
#foreach( name ${SOURCES_ALL} )
#    list( APPEND SOURCES_ALL_PATH ${UNITTEST_DIR}/${name} )
#endforeach()
#CheckFileList( SOURCES_ALL_PATH ${UNITTEST_DIR} )
#
## Create serial target names.
#string( REPLACE ".cpp" "" BASENAMES_UNITTESTS_TESTS "${SOURCES_UNITTESTS_TESTS}" )
#set( TARGETS_UNITTESTS_TESTS ${BASENAMES_UNITTESTS_TESTS} )
#
#set( TARGETS_ALL ${TARGETS_UNITTESTS_TESTS} CatchMain )
#
## Define program targets:
#add_library( CatchMain OBJECT ${UNITTEST_DIR}/${SOURCES_UNITTESTS_MAIN} ${HEADER_DIR}/catch.hpp )
#
#foreach( name ${TARGETS_UNITTESTS_TESTS} )
#    add_executable( ${name} ${name}.cpp $<TARGET_OBJECTS:CatchMain> ${HEADER_DIR}/catch.hpp )
#    target_link_libraries( ${name} PRIVATE mbxlib fftw::fftw)
#    add_dependencies( ${name} mbxlib )
#    add_test(NAME ${name}
#             COMMAND ${name})
#endforeach()
#
#foreach( name ${TARGETS_ALL} )
#    target_include_directories( ${name} PRIVATE ${HEADER_DIR} )
#
#    set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
#
#    # GCC specific warning go here
#    if ( CMAKE_CXX_COMPILER_ID MATCHES "GCC" )
#        # Actually keep these
#        target_compile_options( ${name}  PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn -Wno-int-in-bool-context)
#    endif()
#    # Clang specific warning go here
#    if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
#        # Actually keep these
#        target_compile_options( ${name}  PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors)
#    endif()
#    if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
#        target_compile_options( ${name}  PRIVATE /W4 /w44265 /WX )
#    endif()
#endforeach()
