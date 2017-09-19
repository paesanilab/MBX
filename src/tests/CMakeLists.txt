add_executable(combinations-test combinations-test.cpp)
add_executable(energy_wograd-test energy_wograd-test.cpp)
add_executable(energy_wgrad-test energy_wgrad-test.cpp)
add_executable(io-test io-test.cpp)

foreach(t combinations-test energy_wograd-test energy_wgrad-test io-test)
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree)

set(LIBS bblock io_tools tools 1b 2b)

target_link_libraries(${t} ${LIBS})

install(TARGETS ${t}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
endforeach()
