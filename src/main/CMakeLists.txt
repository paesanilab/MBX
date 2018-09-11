add_executable(clusters_ultimate clusters_ultimate.cpp)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree)

#target_link_libraries(clusters_ultimate bblock)
#target_link_libraries(clusters_ultimate io_tools)
#target_link_libraries(clusters_ultimate tools)
#target_link_libraries(clusters_ultimate 1b)
#target_link_libraries(clusters_ultimate 2b)
#target_link_libraries(clusters_ultimate 3b)
#target_link_libraries(clusters_ultimate dispersion)
#target_link_libraries(clusters_ultimate electrostatics)
target_link_libraries(clusters_ultimate mbnrg)

install(TARGETS clusters_ultimate
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
