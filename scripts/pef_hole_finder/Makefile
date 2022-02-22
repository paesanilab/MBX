CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -lmbxlib -fopenmp -lfftw3 	-fopenmp
LIBDIR = -L$(MBX_HOME)/install/lib/static

INCLUDEDIR = -I$(MBX_HOME)/install/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: hole_finder_2b hole_finder_3b

hole_finder_2b: hole_finder_2b.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) hole_finder_2b.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp $(LIBS) -o $@

hole_finder_3b: hole_finder_3b.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) hole_finder_3b.cpp random-rotation.cpp mt19937.cpp read-xyz.cpp $(LIBS) -o $@

clean:
	rm -rf hole_finder_2b hole_finder_3b
