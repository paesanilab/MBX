set(FF_SOURCES angles.cpp 
               bond.cpp   
               dihedral.cpp 
               inversion.cpp 
               topology.cpp
               connectivity.cpp
#               energyff.cpp 
#               calculators.cpp  
)

add_library(ff OBJECT ${FF_SOURCES})
target_include_directories(ff PRIVATE ${CMAKE_SOURCE_DIR}) 
