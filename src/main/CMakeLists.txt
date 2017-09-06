add_executable(energy energy.cpp)
target_include_directories(energy PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(energy PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree)
target_include_directories(energy PRIVATE ${CMAKE_SOURCE_DIR}/io_tools/)
target_include_directories(energy PRIVATE ${CMAKE_SOURCE_DIR}/bblock/)
target_include_directories(energy PRIVATE ${CMAKE_SOURCE_DIR}/potential/1b/)
target_include_directories(energy PRIVATE ${CMAKE_SOURCE_DIR}/tools/)

target_link_libraries(energy bblock)
target_link_libraries(energy io_tools)
target_link_libraries(energy tools)
target_link_libraries(energy 1b)

install(TARGETS energy
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
