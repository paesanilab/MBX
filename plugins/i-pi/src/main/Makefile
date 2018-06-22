CXX=g++
CXXFLAGS= -Wall -std=c++11 -O2 -g

LIBS = -lmbnrglib -fopenmp
LIBDIR = -L$(HOME)/codes/clusters_ultimate/install/lib/static

INCLUDEDIR = -I$(HOME)/codes/clusters_ultimate/install/include

BINDIR = ../../bin

all: driver

#SOCKETS = ../external/sockets.o

driver: driver.cpp $(BINDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver.cpp $(LIBS) -o $@
	mv driver $(BINDIR)
#driver: driver.cpp $(SOCKETS) $(BINDIR)/.sentinel
#	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) driver.cpp $(LIBS) -o $@ $(SOCKETS)
#	mv driver $(BINDIR)

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf $(BINDIR)/driver
