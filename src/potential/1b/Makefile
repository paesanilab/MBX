
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/include/potential/1b

#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = $(CU_HOME)/lib
LIBS =   

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
POT_OBJ = poly_1b_nogrd_A1B4_3deg.o poly_1b_A1B4_3deg.o  x1b_A1B4_deg3.o

all: libpot.a

libpot.a: $(addprefix $(OBJDIR)/, $(POT_OBJ))
	$(AR) cru libpot.a $(addprefix $(OBJDIR)/, $(POT_OBJ))
	mkdir -p $(LIBDIR)
	mv libpot.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -L$(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(OBJDIR)/, $(POT_OBJ))

