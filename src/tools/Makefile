
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I../../include
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBS = -L ../bblock/ -lbblock

AR = /usr/bin/ar
OBJDIR = ../obj
TOOLS_OBJ = read-nrg.o

all: libtools.a

libtools.a: $(addprefix $(OBJDIR)/, $(TOOLS_OBJ))
	$(AR) cru libtools.a $(addprefix $(OBJDIR)/, $(TOOLS_OBJ))

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBS) -c $< -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(OBJDIR) *.a

