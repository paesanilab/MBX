add_executable(combinations-test combinations-test.cpp)
add_executable(energy_wograd-test energy_wograd-test.cpp)
add_executable(energy_wgrad-test energy_wgrad-test.cpp)
add_executable(io-test io-test.cpp)
add_executable(timing timing.cpp)
add_executable(elec_tools-test elec_tools-test.cpp)

foreach(t combinations-test energy_wograd-test energy_wgrad-test io-test timing elec_tools-test)
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree)

set(LIBS bblock io_tools 1b 2b 3b dispersion electrostatics)
#set(LIBS bblock io_tools tools 1b 2b 3b dispersion)

target_link_libraries(${t} ${LIBS})

install(TARGETS ${t}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
endforeach()
