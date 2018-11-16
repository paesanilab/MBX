set (ELEC_SOURCES electrostatic_tensors.cpp 
                  electrostatic_tensors_short.cpp
                  electrostatics.cpp 
                  fields.cpp 
                  gammq.cpp)

#add_library(electrostatics ${ELEC_SOURCES}) 
#target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}) 
#target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 

add_library(electrostatics OBJECT ${ELEC_SOURCES})
target_link_libraries(electrostatics PUBLIC fftw::fftw)
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/)

#install(TARGETS electrostaticsobj
#        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
