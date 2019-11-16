CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -lmbxlib -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/install/lib/static

INCLUDEDIR = -I$(MBX_HOME)/install/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: diffusion_coefficient

diffusion_coefficient: diffusion_coefficient.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) diffusion_coefficient.cpp $(LIBS) -o $@

clean:
	rm -rf diffusion_coefficient
