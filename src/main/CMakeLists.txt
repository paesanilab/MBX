add_executable(clusters_ultimate energy.cpp)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/io_tools/)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/bblock/)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/potential/1b/)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/tools/)

target_link_libraries(clusters_ultimate bblock)
target_link_libraries(clusters_ultimate io_tools)
target_link_libraries(clusters_ultimate tools)
target_link_libraries(clusters_ultimate 1b)

install(TARGETS clusters_ultimate
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
