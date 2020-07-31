set(TWOB_SOURCES energy2b.cpp 
                 x2b-v9x.cpp 
                 poly-2b-v6x-nogrd.cpp 
                 poly-2b-v6x.cpp 
# BEGIN SECTION CMAKELISTS
                 x2b-A1B2Z2_C-v1x.cpp 
                 poly-2b-A1B2Z2_C-v1x.cpp
                 poly-2b-A1B2Z2_C-v1.cpp 
                 poly_2b_A1B2_A1B2_v1x.cpp 
                 poly_2b_A1B2_A1B2_v1.cpp
                 x2b_A1B2_A1B2_v1x.cpp 
                 x2b_A1B2Z2_C1D2_v1x.cpp 
                 poly_2b_A1B2Z2_C1D2_v1x.cpp 
                 poly_2b_A1B2Z2_C1D2_v1.cpp
                 poly_2b_A1B4_A1B4_deg4_exp0_v1.cpp
                 poly_2b_A1B4_A1B4_deg4_exp0_v1x.cpp
                 x2b_A1B4_A1B4_deg4_exp0_v1x.cpp
                 x2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp
                 poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x.cpp
                 poly_2b_A1B2Z2_C1D4_deg3_exp0_v1.cpp
                 mbnrg_2b_A1_B1_deg15_v1.cpp
                 poly_2b_A1_B1_deg15_nograd_v1.cpp
                 poly_2b_A1_B1_deg15_grad_v1.cpp
                 poly_2b_A1_A1_deg23_v1.cpp
                 poly_2b_A1_A1_deg23_v1x.cpp
                 x2b_A1_A1_deg23_v1x.cpp
                 poly_2b_A1B2Z2_C1_deg5_v1.cpp
                 poly_2b_A1B2Z2_C1_deg5_v1x.cpp
                 x2b_A1B2Z2_C1_deg5_v1x.cpp
# END SECTION CMAKELISTS
)

#SET_SOURCE_FILES_PROPERTIES(poly-2b-v6x.cpp PROPERTIES COMPILE_FLAGS -O0)

add_library(2b OBJECT ${TWOB_SOURCES})
target_include_directories(2b PRIVATE ${CMAKE_SOURCE_DIR}) 
set_property(TARGET 2b PROPERTY POSITION_INDEPENDENT_CODE TRUE)
