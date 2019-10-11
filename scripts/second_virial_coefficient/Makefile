CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -lmbxlib -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/install/lib/static

INCLUDEDIR = -I$(MBX_HOME)/install/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: second_virial 

second_virial: virial2-rigid.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) virial2-rigid.cpp $(LIBS) -o $@

clean:
	rm -rf second_virial
