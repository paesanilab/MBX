CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -lmbxlib -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/install/lib/static

INCLUDEDIR = -I$(MBX_HOME)/install/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: second_virial get_2b_energies_list_rigid

second_virial: virial2-rigid.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) virial2-rigid.cpp $(LIBS) -o $@

get_2b_energies_list_rigid: get_2b_energies_list_rigid.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) get_2b_energies_list_rigid.cpp $(LIBS) -o $@

clean:
	rm -rf second_virial
