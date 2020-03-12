add_executable(single_point single_point.cpp)
add_executable(optimize optimize.cpp)

foreach(t single_point optimize)
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${t} PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

target_link_libraries(${t} mbxlib)


install(TARGETS ${t}
        RUNTIME DESTINATION bin/main
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
endforeach()
