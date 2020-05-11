set(ONEB_SOURCES ps.cpp energy1b.cpp 
# BEGIN SECTION CMAKELISTS
                 poly_1b_A1B2_deg4_v1x.cpp x1b_A1B2_deg4_v1x.cpp
                 poly_1b_A1B4_deg5_exp0_v1.cpp poly_1b_A1B4_deg5_exp0_v1x.cpp
                 x1b_A1B4_deg5_exp0_v1x.cpp
                 mbnrg_1b_A1B3_deg6_v1.cpp
                 poly_1b_A1B3_deg6_nograd_v1.cpp
                 poly_1b_A1B3_deg6_grad_v1.cpp
# END SECTION CMAKELISTS
)

#SET_SOURCE_FILES_PROPERTIES(poly_1b_A1B6_deg5_v1x.cpp PROPERTIES COMPILE_FLAGS -O0)

add_library(1b OBJECT ${ONEB_SOURCES})
target_include_directories(1b PRIVATE ${CMAKE_SOURCE_DIR}) 
set_property(TARGET 1b PROPERTY POSITION_INDEPENDENT_CODE TRUE)
