CXX=g++
CXXFLAGS= -Wall -std=c++11 -O2 -g

LIBS = -lmbnrglib
LIBDIR = -L$(CU_HOME)/install/lib/static

INCLUDEDIR = -I$(CU_HOME)/install/include

BINDIR = ../../bin

all: test-gas-noneq-xe32 test-gas-noneq-xe2 test-nrg test-nrg-pbc

test-gas-noneq-xe32: test-gas-noneq-xe32.cpp ../main/perturb.o 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) test-gas-noneq-xe32.cpp ../main/perturb.o  $(LIBS) -o $@

test-gas-noneq-xe2: test-gas-noneq-xe2.cpp ../main/perturb.o 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) test-gas-noneq-xe2.cpp ../main/perturb.o  $(LIBS) -o $@

test-nrg-pbc: test-nrg-pbc.cpp ../main/perturb.o 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) test-nrg-pbc.cpp ../main/perturb.o  $(LIBS) -o $@

test-nrg: test-nrg.cpp ../main/perturb.o 
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) test-nrg.cpp ../main/perturb.o  $(LIBS) -o $@

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf test-gas-noneq-xe32  test-gas-noneq-xe2 test-nrg-pbc test-nrg
