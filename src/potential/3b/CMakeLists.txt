set(THREEB_SOURCES poly-3b-v2x.cpp poly-3b-v2x-nogrd.cpp energy3b.cpp x3b-v2x.cpp
x3b-h2o-ion-v2x-deg4-filtered.cpp
poly-3b-h2o-ion-v1x_deg4_filtered.cpp
# BEGIN SECTION CMAKELISTS
poly_3b_A1B4_C1D2_C1D2_deg3_nograd_v1.cpp
poly_3b_A1B4_C1D2_C1D2_deg3_grad_v1.cpp
mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1.cpp
# END SECTION CMAKELISTS
)

#SET_SOURCE_FILES_PROPERTIES(poly-3b-v2x.cpp PROPERTIES COMPILE_FLAGS "-O0")
#SET_SOURCE_FILES_PROPERTIES(poly-3b-v2x-nogrd.cpp PROPERTIES COMPILE_FLAGS "-O0")

add_library(3b OBJECT ${THREEB_SOURCES})
target_include_directories(3b PRIVATE ${CMAKE_SOURCE_DIR}) 
set_property(TARGET 3b PROPERTY POSITION_INDEPENDENT_CODE TRUE)
