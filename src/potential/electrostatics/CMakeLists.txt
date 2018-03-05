set (ELEC_SOURCES electrostatics.cpp fields.cpp gammq.cpp)

add_library(electrostatics ${ELEC_SOURCES}) 
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(electrostatics PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 

add_library(electrostaticsobj OBJECT ${ELEC_SOURCES})
target_include_directories(electrostaticsobj PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(electrostaticsobj PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/)

install(TARGETS electrostaticsobj
        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
