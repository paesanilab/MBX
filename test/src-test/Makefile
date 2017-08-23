
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/src/bblock \
          -I$(CU_HOME)/src/tools \
          -I$(CU_HOME)/src/io_tools 
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = -L$(CU_HOME)/lib 
#LIBS = ../bblock/libbblock.a
LIBS = -liotools -lbblock 
#LIBS = -liotools -lbblock -lpot

BINDIR = $(CU_HOME)/test/bin-test

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/test/obj-test
EXES = io-test dimers-test
#EXES = io-test energy-test
#ENERGY_OBJ = 

all: $(EXES)

#energy: $(addprefix $(OBJDIR)/, $(ENERGY_OBJ))
energy-test: energy-test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBDIR) $< $(LIBS) -o $@
	mkdir -p $(BINDIR)
	mv energy-test $(BINDIR)

io-test: io-test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBDIR) $< $(LIBS) -o $@
	mkdir -p $(BINDIR)
	mv io-test $(BINDIR)

dimers-test: dimers-test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBDIR) $< $(LIBS) -o $@
	mkdir -p $(BINDIR)
	mv dimers-test $(BINDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(BINDIR)/, $(EXES)) 

