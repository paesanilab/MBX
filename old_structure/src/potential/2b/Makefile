
CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/include/potential/2b \
          -I$(CU_HOME)/include/tools

#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = $(CU_HOME)/lib
LIBS =   

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
POT_OBJ = poly_2b_A1B2_A1B2_v1_deg4.o poly_2b_A1B2_A1B2_v1x_deg4.o \
          x2b_A1B2_A1B2_v1x_deg4.o

all: libpot2b.a

libpot2b.a: $(addprefix $(OBJDIR)/, $(POT_OBJ))
	$(AR) cru libpot2b.a $(addprefix $(OBJDIR)/, $(POT_OBJ))
	mkdir -p $(LIBDIR)
	mv libpot2b.a $(LIBDIR)

#$(OBJDIR)/%.o: %.cpp %.h $(OBJDIR)/.sentinel
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDE) -L$(LIBDIR) -c $< $(LIBS) -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(OBJDIR)/, $(POT_OBJ))

