add_executable(MBX MBX.cpp)
target_include_directories(MBX PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(MBX PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

target_link_libraries(MBX mbx)

install(TARGETS MBX
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
