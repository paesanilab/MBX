add_executable(energy_wograd-test energy_wograd-test.cpp)
add_executable(energy_wgrad-test energy_wgrad-test.cpp)
add_executable(io-test io-test.cpp)
add_executable(getset-test getset-test.cpp)
add_executable(system-test system-test.cpp)
add_executable(bad_inputs-test bad_inputs-test.cpp)
add_executable(test_single_point single_point.cpp)
add_executable(n_single_point n_single_point.cpp)
add_executable(get_2b_energies_list get_2b_energies_list.cpp)
add_executable(mbd mbd.cpp)
add_executable(test_virial test_virial.cpp)
add_executable(test_virial_dimer_scan test_virial_dimer_scan.cpp)
add_executable(generate_define_system generate_define_system.cpp)
add_executable(get_timings get_timings.cpp)
add_executable(single_point_external_chg single_point_external_chg.cpp)

#foreach(t combinations-test energy_wograd-test energy_wgrad-test io-test timing elec_tools-test getset-test pbc-test sys-test generate_define_system)
foreach(t n_single_point test_single_point energy_wograd-test energy_wgrad-test io-test getset-test system-test bad_inputs-test get_2b_energies_list mbd test_virial test_virial_dimer_scan generate_define_system get_timings single_point_external_chg)
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

set(LIBS mbxlib)
#set(LIBS bblock io_tools 1b 2b 3b dispersion electrostatics)
#set(LIBS bblock io_tools tools 1b 2b 3b dispersion)

target_link_libraries(${t} ${LIBS})


install(TARGETS ${t}
        RUNTIME DESTINATION bin/tests
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
endforeach()

#add_subdirectory(unittests)
if(${CMAKE_COMPILE_TESTS})
    add_subdirectory(unittests_v0.3.0a)
endif()
