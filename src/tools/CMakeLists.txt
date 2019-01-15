set (TOOLS_SOURCES math_tools.cpp)

add_library(tools OBJECT ${TOOLS_SOURCES})
target_include_directories(tools PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(tools PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/)
