set(DISP_SOURCES dispersion.cpp disptools.cpp)
#set(DISP_SOURCES dispersion2b.cpp disptools.cpp dispersion.cpp disptools_new.cpp)

#add_library(dispersion ${DISP_SOURCES}) 
#target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}) 

add_library(dispersion OBJECT ${DISP_SOURCES})
target_link_libraries(dispersion PUBLIC fftw::fftw)
target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(dispersion PRIVATE ${CMAKE_SOURCE_DIR}/../external/)


#install(TARGETS dispersionobj
#        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
