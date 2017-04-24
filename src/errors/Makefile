
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/include/bblock \
          -I$(CU_HOME)/include/tools \
          -I$(CU_HOME)/include/errors 
#LIBS = -lnetcdf -lgsl -lgslcblas

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
LIBDIR = $(CU_HOME)/lib
ERR_OBJ = errorcodes.o

all: liberrors.a

liberrors.a: $(addprefix $(OBJDIR)/, $(ERR_OBJ))
	$(AR) cru liberrors.a $(addprefix $(OBJDIR)/, $(ERR_OBJ))
	mkdir -p $(LIBDIR)
	mv liberrors.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(OBJDIR)/, $(ERR_OBJ))

