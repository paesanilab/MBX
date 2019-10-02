CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

LIBS = -lmbnrglib -fopenmp -lfftw3
LIBDIR = -L$(CU_HOME)/install/lib/static

INCLUDEDIR = -I$(CU_HOME)/install/include -I../utils

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
