add_library(3b 
            poly-3b-v2x.cpp 
            poly-3b-v2x-nogrd.cpp 
            energy3b.cpp
            x3b-v2x.cpp) 
target_include_directories(3b PRIVATE ${CMAKE_SOURCE_DIR}) 
