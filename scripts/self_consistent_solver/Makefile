CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -l:libmbx.a -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/lib/

INCLUDEDIR = -I$(MBX_HOME)/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: sc

sc: sc.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) sc.cpp $(LIBS) -o $@

clean:
	rm -rf sc
