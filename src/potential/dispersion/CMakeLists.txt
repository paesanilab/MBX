set(DISP_SOURCES dispersion.cpp disptools.cpp)

add_library(dispersion OBJECT ${DISP_SOURCES})
target_link_libraries(dispersion PUBLIC fftw::fftw)
target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

set_property(TARGET dispersion PROPERTY POSITION_INDEPENDENT_CODE TRUE)
