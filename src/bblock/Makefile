
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I../../include
LIBS = -lnetcdf -lgsl -lgslcblas

AR = /usr/bin/ar
OBJDIR = ../obj
BBLOCK_OBJ = system.o molecule.o monomer.o

all: bblock.a

bblock.a: $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))
	$(AR) cru bblock.a $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf *.o *.a

