CXX=icpc
CXXFLAGS= -Wall -std=c++11 -O2 -g

LIBS = -lmbnrglib -fopenmp -lfftw3
LIBDIR = -L$(CU_HOME)/install/lib/static

INCLUDEDIR = -I$(CU_HOME)/install/include

BINDIR = ../../bin

all: driver driver_pbc

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
	rm -rf $(BINDIR)/driver $(BINDIR)/driver_pbc
