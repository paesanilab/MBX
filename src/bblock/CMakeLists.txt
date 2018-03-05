set(BBLOCK_SOURCES system.cpp sys_tools.cpp)

add_library(bblock ${BBLOCK_SOURCES}) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 

add_library(bblockobj OBJECT ${BBLOCK_SOURCES})
target_include_directories(bblockobj PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(bblockobj PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/)

install(TARGETS bblockobj
        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})
