add_library(electrostatics electrostatics.cpp fields.cpp gammq.cpp) 
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 
