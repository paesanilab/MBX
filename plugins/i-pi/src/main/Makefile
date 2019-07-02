CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -lmbnrglib -fopenmp -lfftw3
LIBDIR = -L$(CU_HOME)/install/lib/static

INCLUDEDIR = -I$(CU_HOME)/install/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: driver driver_pbc 

infrared: infrared.cpp $(OBJDIR)/perturb.o $(OBJDIR)/read_xyz.o $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) infrared.cpp $(OBJDIR)/perturb.o $(OBJDIR)/read_xyz.o $(LIBS) -o $@
	mv infrared $(BINDIR)/infrared

raman: raman.cpp $(OBJDIR)/perturb.o $(OBJDIR)/read_xyz.o $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) raman.cpp $(OBJDIR)/perturb.o $(OBJDIR)/read_xyz.o $(LIBS) -o $@
	mv raman $(BINDIR)/raman

driver-noneq: driver-noneq.cpp $(OBJDIR)/perturb.o $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver-noneq.cpp $(OBJDIR)/perturb.o  $(LIBS) -o $@
	mv driver-noneq $(BINDIR)/driver-noneq

driver: driver.cpp $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver.cpp $(LIBS) -o $@
	mv driver $(BINDIR)/driver

driver_pbc: driver_pbc.cpp $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver_pbc.cpp $(LIBS) -o $@
	mv driver_pbc $(BINDIR)/driver_pbc

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf $(BINDIR)
