CXX=g++
CXXFLAGS= -Wall -std=c++11 -O2 -g

LIBS = -lmbnrglib -fopenmp
LIBDIR = -L$(CU_HOME)/install/lib/static

INCLUDEDIR = -I$(CU_HOME)/install/include

BINDIR = ../../bin

all: driver

driver: driver.cpp $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver.cpp $(LIBS) -o $@
	mv driver $(BINDIR)/driver

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf $(BINDIR)/driver
