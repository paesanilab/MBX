CXX=g++
CXXFLAGS= -Wall -std=c++11  -g -O2

LIBS = -l:libmbx.a -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/lib/

INCLUDEDIR = -I$(MBX_HOME)/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: driver 

driver: driver.cpp $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver.cpp $(LIBS) -o $@
	mv driver $(BINDIR)/driver

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf $(BINDIR)
