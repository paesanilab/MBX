set(DISP_SOURCES dispersion2b.cpp disptools.cpp)

#add_library(dispersion ${DISP_SOURCES}) 
#target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}) 

add_library(dispersion OBJECT ${DISP_SOURCES})
target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}) 

#install(TARGETS dispersionobj
#        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
