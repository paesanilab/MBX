set(REP_SOURCES lj.cpp ljtools.cpp)

add_library(lennardjones OBJECT ${REP_SOURCES})
target_link_libraries(lennardjones PUBLIC fftw::fftw)
target_include_directories(lennardjones PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(lennardjones PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

set_property(TARGET lennardjones PROPERTY POSITION_INDEPENDENT_CODE TRUE)

