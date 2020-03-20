add_library(io_tools OBJECT read_nrg.cpp write_nrg.cpp read_xyz.cpp write_xyz.cpp
read_connectivity.cpp
write_connectivity.cpp
) 
target_link_libraries(io_tools PUBLIC fftw::fftw)
target_include_directories(io_tools PRIVATE ${CMAKE_SOURCE_DIR}) 
target_include_directories(io_tools PRIVATE ${CMAKE_SOURCE_DIR}/../external/) 
