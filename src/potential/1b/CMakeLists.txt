set(ONEB_SOURCES ps.cpp energy1b.cpp)

#add_library(1b ${ONEB_SOURCES}) 
#target_include_directories(1b PRIVATE ${CMAKE_SOURCE_DIR}) 

add_library(1b OBJECT ${ONEB_SOURCES})
target_include_directories(1b PRIVATE ${CMAKE_SOURCE_DIR}) 

#install(TARGETS 1bobj
#        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
