CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/src/bblock \
          -I$(CU_HOME)/src/tools
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = $(CU_HOME)/lib 
LIBS = -lbblock 

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

