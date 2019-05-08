add_library(io_tools OBJECT read_nrg.cpp write_nrg.cpp) 
target_link_libraries(io_tools PUBLIC fftw::fftw)
target_include_directories(io_tools PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(io_tools PRIVATE ${CMAKE_SOURCE_DIR}/../external/kdtree/) 
