MPICXX=mpiicpc
CXXFLAGS= -Wall -std=c++11 -O2

LIBS = -lmbxlib -fopenmp -lfftw3 -lrepimd
LIBDIR = -L$(MBX_HOME)/install/lib/static -L../../lib

INCLUDEDIR = -I$(MBX_HOME)/install/include -I../tools

BINDIR = ../../bin

OBJDIR = ../../obj

AR = /usr/bin/ar

all:  repimd 

repimd: repimd.cpp $(BINDIR)/.sentinel
	$(MPICXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) repimd.cpp $(LIBS) -o $@
	mv repimd $(BINDIR)/repimd

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf $(BINDIR)  
