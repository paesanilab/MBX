set(FF_SOURCES angles.cpp 
               bond.cpp   
               dihedral.cpp 
               inversion.cpp 
               topology.cpp
               energyff.cpp 
               calculators.cpp  
               connectivity.cpp
)

add_library(ff OBJECT ${FF_SOURCES})
set_property(TARGET ff PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_include_directories(ff PRIVATE ${CMAKE_SOURCE_DIR}) 
