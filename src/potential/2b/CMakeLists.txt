add_library(2b energy2b.cpp x2b-v9x.cpp poly-2b-v6x-nogrd.cpp 
               poly-2b-v6x.cpp x2b-A1B2Z2_C-v1x.cpp poly-2b-A1B2Z2_C-v1x.cpp
               poly-2b-A1B2Z2_C-v1.cpp) 
SET_SOURCE_FILES_PROPERTIES(poly-2b-v6x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-2b-v6x-nogrd.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-2b-A1B2Z2_C-v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-2b-A1B2Z2_C-v1.cpp PROPERTIES COMPILE_FLAGS -O0)
target_include_directories(2b PRIVATE ${CMAKE_SOURCE_DIR}) 
