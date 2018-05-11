set(THREEB_SOURCES poly-3b-v2x.cpp poly-3b-v2x-nogrd.cpp energy3b.cpp x3b-v2x.cpp)

SET_SOURCE_FILES_PROPERTIES(poly-3b-v2x.cpp PROPERTIES COMPILE_FLAGS -O0)
SET_SOURCE_FILES_PROPERTIES(poly-3b-v2x-nogrd.cpp PROPERTIES COMPILE_FLAGS -O0)

#add_library(3b ${THREEB_SOURCES})
#target_include_directories(3b PRIVATE ${CMAKE_SOURCE_DIR}) 

add_library(3b OBJECT ${THREEB_SOURCES})
target_include_directories(3b PRIVATE ${CMAKE_SOURCE_DIR}) 

#install(TARGETS 3bobj
#        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
