set(BBLOCK_SOURCES system.cpp sys_tools.cpp external_call.cpp)

#add_library(bblock SHARED ${BBLOCK_SOURCES}) 
#target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}) 
#target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 

add_library(bblock OBJECT ${BBLOCK_SOURCES})
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(bblock PRIVATE ${CMAKE_SOURCE_DIR}/../external/)
target_link_libraries(bblock PUBLIC fftw::fftw)
set_property(TARGET bblock PROPERTY POSITION_INDEPENDENT_CODE TRUE)

install(TARGETS bblock
        OBJECTS DESTINATION ${CMAKE_INSTALL_OBJDIR})

#install(TARGETS bblock
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/dynamic
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
