add_library(bblock system.cpp sys_tools.cpp) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 
