CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -Wall 
INCLUDE = -I$(CU_HOME)/src/tools
#LIBS = -lnetcdf -lgsl -lgslcblas
LIBDIR = $(CU_HOME)/lib 
LIBS = 

AR = /usr/bin/ar
OBJDIR = $(CU_HOME)/obj
ONE_BODY = ps.o
TWO_BODY =
THREE_BODY = 
POT_OBJ = $(ONE_BODY) $(TWO_BODY) $(THREE_BODY) 

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

