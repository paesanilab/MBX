
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I../../include
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = ../../lib 
#LIBS = ../bblock/libbblock.a
LIBS = -l bblock

AR = /usr/bin/ar
OBJDIR = ../obj
TOOLS_OBJ = read-nrg.o

all: libtools.a

libtools.a: $(addprefix $(OBJDIR)/, $(TOOLS_OBJ))
	$(AR) cru libtools.a $(addprefix $(OBJDIR)/, $(TOOLS_OBJ))
	mkdir -p $(LIBDIR)
	mv libtools.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -L $(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(OBJDIR) *.a

