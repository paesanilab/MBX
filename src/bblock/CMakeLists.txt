add_library(bblock system.cpp) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}/tools/) 
