CXX=g++
CXXFLAGS= -Wall -std=c++11 -O2 -g

LIBS = -lmbnrglib 
LIBDIR = -L$(CU_HOME)/install/lib/static

INCLUDEDIR = -I$(CU_HOME)/install/include -I../utils

BINDIR = ../../bin

OBJDIR = ../../obj

all: driver driver-noneq

driver-noneq: driver-noneq.cpp $(OBJDIR)/perturb.o $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver-noneq.cpp $(OBJDIR)/perturb.o  $(LIBS) -o $@
	mv driver-noneq $(BINDIR)/driver-noneq

driver: driver.cpp $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver.cpp $(LIBS) -o $@
	mv driver $(BINDIR)/driver

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf $(BINDIR)
