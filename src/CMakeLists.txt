add_subdirectory(bblock)
add_subdirectory(io_tools)
add_subdirectory(potential)
add_subdirectory(tools)
add_subdirectory(main)
add_subdirectory(tests)

add_library(mbnrg)
target_link_libraries(mbnrg bblock potential)

install(TARGETS mbnrg
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY . DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h")
