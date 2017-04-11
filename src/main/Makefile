
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I../../include/bblock \
          -I../../include/tools
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = -L../../lib 
#LIBS = ../bblock/libbblock.a
LIBS = -lbblock -ltools

BINDIR = ../../bin

AR = /usr/bin/ar
OBJDIR = ../obj
#ENERGY_OBJ = 

all: energy

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
	rm -rf $(addprefix $(OBJDIR)/, $(ENERGY_OBJ)) *.a

