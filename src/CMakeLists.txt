list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake)
find_package(FFTW REQUIRED)

enable_testing ()
add_subdirectory(bblock)
add_subdirectory(io_tools)
add_subdirectory(potential)
add_subdirectory(tools)
add_subdirectory(main)

add_subdirectory(tests)

add_library(mbnrg SHARED $<TARGET_OBJECTS:bblock> 
                         $<TARGET_OBJECTS:io_tools> 
                         $<TARGET_OBJECTS:tools> 
                         $<TARGET_OBJECTS:1b> 
                         $<TARGET_OBJECTS:2b> 
                         $<TARGET_OBJECTS:3b> 
                         $<TARGET_OBJECTS:dispersion> 
                         $<TARGET_OBJECTS:electrostatics> )

target_link_libraries(mbnrg PUBLIC fftw::fftw)

add_library(mbnrglib STATIC $<TARGET_OBJECTS:bblock> 
                         $<TARGET_OBJECTS:io_tools> 
                         $<TARGET_OBJECTS:tools> 
                         $<TARGET_OBJECTS:1b> 
                         $<TARGET_OBJECTS:2b> 
                         $<TARGET_OBJECTS:3b> 
                         $<TARGET_OBJECTS:dispersion> 
                         $<TARGET_OBJECTS:electrostatics> )

target_link_libraries(mbnrglib PUBLIC fftw::fftw)

set(PN ${PROJECT_NAME})
install(EXPORT "${PN}Targets"
        NAMESPACE "${PN}::"
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(TARGETS mbnrg
        EXPORT "${PN}Targets"
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/dynamic
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS mbnrglib
        EXPORT "${PN}Targets"
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY . DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h")

