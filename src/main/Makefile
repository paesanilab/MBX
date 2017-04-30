
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/include/bblock \
          -I$(CU_HOME)/include/tools \
          -I$(CU_HOME)/include/io_tools \
          -I$(CU_HOME)/include/potential/1b 
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = -L$(CU_HOME)/lib 
LIBS = -liotools -lbblock -lpot 

BINDIR = $(CU_HOME)/bin

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
ENERGY_OBJ = energy

all: $(ENERGY_OBJ)

#energy: $(addprefix $(OBJDIR)/, $(ENERGY_OBJ))
energy: energy.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBDIR) $< $(LIBS) -o $@
	mkdir -p $(BINDIR)
	mv energy $(BINDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(BINDIR)/, $(ENERGY_OBJ)) 

