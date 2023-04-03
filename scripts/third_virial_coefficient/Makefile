CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g  -O2

LIBS = -lmbxlib -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/lib/static

INCLUDEDIR = -I$(MBX_HOME)/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: 1-generate_confs 2-get_energy_list 3-get_virial 1-generate_confs_r12r13

1-generate_confs: 1-generate_confs.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) 1-generate_confs.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp $(LIBS) -o $@

1-generate_confs_r12r13: 1-generate_confs_r12r13.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) 1-generate_confs_r12r13.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp $(LIBS) -o $@

2-get_energy_list: 2-get_energy_list.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) 2-get_energy_list.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp $(LIBS) -o $@

3-get_virial: 3-get_virial.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) 3-get_virial.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp $(LIBS) -o $@

clean:
	rm -rf 1-generate_confs 2-get_energy_list 3-get_virial *.o
