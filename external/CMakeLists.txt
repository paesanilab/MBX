add_subdirectory(kdtree)
add_subdirectory(json)

install(DIRECTORY kdtree/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kdtree
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY kdtree/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kdtree
        FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY json/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/json
        FILES_MATCHING PATTERN "*.h")
