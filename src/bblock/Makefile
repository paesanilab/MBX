
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I../../include
#LIBS = -lnetcdf -lgsl -lgslcblas

AR = /usr/bin/ar
OBJDIR = ../obj
LIBDIR = ../../lib
BBLOCK_OBJ = system.o molecule.o monomer.o

all: libbblock.a

libbblock.a: $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))
	$(AR) cru libbblock.a $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))
	mkdir -p $(LIBDIR)
	mv libbblock.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(OBJDIR) *.a

