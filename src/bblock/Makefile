
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/include/bblock \
          -I$(CU_HOME)/include/tools \
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = $(CU_HOME)/lib
LIBS =   

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
BBLOCK_OBJ = system.o molecule.o monomer.o mon_h2o.o

all: libbblock.a

libbblock.a: $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))
	$(AR) cru libbblock.a $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))
	mkdir -p $(LIBDIR)
	mv libbblock.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -L$(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(OBJDIR)/, $(BBLOCK_OBJ))

