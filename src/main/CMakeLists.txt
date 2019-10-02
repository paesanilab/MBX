add_executable(clusters_ultimate clusters_ultimate.cpp)
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(clusters_ultimate PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

target_link_libraries(clusters_ultimate mbnrg)

install(TARGETS clusters_ultimate
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
