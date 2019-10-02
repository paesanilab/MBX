set(REP_SOURCES buckingham.cpp bucktools.cpp)

add_library(buckingham OBJECT ${REP_SOURCES})
target_include_directories(buckingham PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(buckingham PRIVATE ${CMAKE_SOURCE_DIR}/../external/)

