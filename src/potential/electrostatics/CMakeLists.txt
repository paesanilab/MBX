set (ELEC_SOURCES electrostatics.cpp 
                  fields.cpp 
                  gammq.cpp)

add_library(electrostatics OBJECT ${ELEC_SOURCES})
target_link_libraries(electrostatics PUBLIC fftw::fftw)
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}/../external/)
set_property(TARGET electrostatics PROPERTY POSITION_INDEPENDENT_CODE TRUE)

