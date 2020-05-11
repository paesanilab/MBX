# same version as top-level project
cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)
find_package(FFTW REQUIRED)

enable_testing ()
add_subdirectory(bblock)
add_subdirectory(io_tools)
add_subdirectory(potential)
add_subdirectory(tools)
add_subdirectory(main)

add_subdirectory(tests)

add_library(mbx SHARED $<TARGET_OBJECTS:bblock> 
                         $<TARGET_OBJECTS:io_tools> 
                         $<TARGET_OBJECTS:tools> 
                         $<TARGET_OBJECTS:ff>
                         $<TARGET_OBJECTS:1b> 
                         $<TARGET_OBJECTS:2b> 
                         $<TARGET_OBJECTS:3b> 
                         $<TARGET_OBJECTS:dispersion> 
                         $<TARGET_OBJECTS:buckingham> 
                         $<TARGET_OBJECTS:electrostatics> )

target_link_libraries(mbx PUBLIC fftw::fftw)

add_library(mbxlib STATIC $<TARGET_OBJECTS:bblock> 
                         $<TARGET_OBJECTS:io_tools> 
                         $<TARGET_OBJECTS:tools> 
                         $<TARGET_OBJECTS:ff>
                         $<TARGET_OBJECTS:1b> 
                         $<TARGET_OBJECTS:2b> 
                         $<TARGET_OBJECTS:3b> 
                         $<TARGET_OBJECTS:dispersion> 
                         $<TARGET_OBJECTS:buckingham> 
                         $<TARGET_OBJECTS:electrostatics> )

target_link_libraries(mbxlib PUBLIC fftw::fftw)

set(PN MBX)
install(EXPORT "${PN}Targets"
        NAMESPACE "${PN}::"
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(TARGETS mbx
        EXPORT "${PN}Targets"
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/dynamic
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS mbxlib
        EXPORT "${PN}Targets"
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY . DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h")

