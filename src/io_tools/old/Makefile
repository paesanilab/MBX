CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/include/bblock \
          -I$(CU_HOME)/include/bblock/monomers \
          -I$(CU_HOME)/include/bblock/dimers \
          -I$(CU_HOME)/include/bblock/trimers \
					-I$(CU_HOME)/include/tools \
					-I$(CU_HOME)/include/io_tools \
					-I$(CU_HOME)/include/potential/1b
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = $(CU_HOME)/lib 
LIBS = -lbblock -lpot

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
TOOLS_OBJ = read_nrg.o write_nrg.o 

all: libiotools.a

libiotools.a: $(addprefix $(OBJDIR)/, $(TOOLS_OBJ))
	$(AR) cru libiotools.a $(addprefix $(OBJDIR)/, $(TOOLS_OBJ))
	mkdir -p $(LIBDIR)
	mv libiotools.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -L$(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(OBJDIR)/, $(TOOLS_OBJ)) 

