CXX=g++
CXXFLAGS= -Wall -std=c++11 -O2 -g

LIBS = -lmbnrglib -fopenmp
LIBDIR = -L$(HOME)/codes/clusters_ultimate/install/lib/static

INCLUDEDIR = -I$(HOME)/codes/clusters_ultimate/install/include

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
