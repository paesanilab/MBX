set(TWOB_SOURCES energy2b.cpp 
                 x2b-v9x.cpp 
                 poly-2b-v6x-nogrd.cpp 
                 poly-2b-v6x.cpp 
                 x2b-A1B2Z2_C-v1x.cpp 
                 poly-2b-A1B2Z2_C-v1x.cpp
                 poly-2b-A1B2Z2_C-v1.cpp 
                 poly_2b_A1B2_A1B2_v1x.cpp 
                 poly_2b_A1B2_A1B2_v1.cpp
                 x2b_A1B2_A1B2_v1x.cpp 
                 x2b_A1B2Z2_C1D2_v1x.cpp 
                 poly_2b_A1B2Z2_C1D2_v1x.cpp 
                 poly_2b_A1B2Z2_C1D2_v1.cpp
                 poly_2b_A1B4_C1D2X2_deg4_v1.cpp
                 poly_2b_A1B4_C1D2X2_deg4_v1x.cpp
                 poly_2b_A1B6_C1D2X2_deg3_v1.cpp
                 poly_2b_A1B6_C1D2X2_deg3_v1x.cpp
                 poly_2b_A1B4_C1D6_deg3_v1.cpp
                 poly_2b_A1B4_C1D6_deg3_v1x.cpp
                 x2b_A1B4_C1D6_deg3_v1x.cpp
                 x2b_A1B4_C1D2X2_deg4_v1x.cpp
                 x2b_A1B6_C1D2X2_deg3_v1x.cpp
                 poly_2b_A1B4_A1B4_deg4_exp0_v1.cpp
                 poly_2b_A1B4_A1B4_deg4_exp0_v1x.cpp
                 x2b_A1B4_A1B4_deg4_exp0_v1x.cpp
                 x2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp
                 poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp
                 poly_2b_A1B2Z2_C1D4_deg3_exp0_v1.cpp
)

SET_SOURCE_FILES_PROPERTIES(poly-2b-v6x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-2b-v6x-nogrd.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-2b-A1B2Z2_C-v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-2b-A1B2Z2_C-v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B2_A1B2_v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B2_A1B2_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B2Z2_C1D2_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B2Z2_C1D2_v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B4_C1D2X2_deg4_v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B4_C1D2X2_deg4_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B6_C1D2X2_deg3_v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B6_C1D2X2_deg3_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B4_C1D6_deg3_v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B4_C1D6_deg3_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B4_A1B4_deg4_exp0_v1.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly_2b_A1B4_A1B4_deg4_exp0_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)

#add_library(2b ${TWOB_SOURCES})
#target_include_directories(2b PRIVATE ${CMAKE_SOURCE_DIR}) 

add_library(2b OBJECT ${TWOB_SOURCES})
target_include_directories(2b PRIVATE ${CMAKE_SOURCE_DIR}) 

#install(TARGETS 2bobj
#        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
