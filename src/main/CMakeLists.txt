add_executable(clusters_ultimate energy.cpp)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(clusters_ultimate PRIVATE ${PROJECT_SOURCE_DIR}/../external/kdtree)

target_link_libraries(clusters_ultimate bblock)
target_link_libraries(clusters_ultimate io_tools)
target_link_libraries(clusters_ultimate 1b)
