add_library(3b 
            poly-3b-v2x.cpp 
            poly-3b-v2x-nogrd.cpp 
            energy3b.cpp
            x3b-v2x.cpp) 
SET_SOURCE_FILES_PROPERTIES(poly-3b-v2x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-3b-v2x-nogrd.cpp PROPERTIES COMPILE_FLAGS -O0)
target_include_directories(3b PRIVATE ${CMAKE_SOURCE_DIR}) 
