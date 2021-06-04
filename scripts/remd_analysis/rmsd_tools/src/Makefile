CXX=icpc
CXXFLAGS= -Wall -std=c++11  -g

OBJ_INSTALL = ../obj
LIB_INSTALL = ../lib
BIN_INSTALL = ../bin

LIBS_LIB = -lmbxlib -fopenmp -lfftw3
LIBS = -lmbxlib -fopenmp -lfftw3 -lrsc
LIBDIR = -L$(MBX_HOME)/install/lib/static -L$(LIB_INSTALL)

INCLUDEDIR = -I$(MBX_HOME)/install/include 

AR = /usr/bin/ar

RSC_OBJ = json_tools.o rmsd_tools.o rmsd.o

all: librsc.a rsc pops

librsc.a: $(addprefix $(OBJ_INSTALL)/, $(RSC_OBJ))
	echo $(INCLUDEDIR)
	$(AR) cru librsc.a $(addprefix $(OBJ_INSTALL)/, $(RSC_OBJ))
	mkdir -p $(LIB_INSTALL)
	mv librsc.a $(LIB_INSTALL)

rsc: rsc.cpp $(BIN_INSTALL)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) rsc.cpp $(LIBS) -o $@
	mv rsc $(BIN_INSTALL)/rsc

pops: pops.cpp $(BIN_INSTALL)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) pops.cpp $(LIBS) -o $@
	mv pops $(BIN_INSTALL)/pops

$(OBJ_INSTALL)/%.o: %.cpp $(OBJ_INSTALL)/.sentinel
	$(CXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) -c $< $(LIBS) -o $@

$(BIN_INSTALL)/.sentinel:
	mkdir -p $(BIN_INSTALL)
	touch $@

$(OBJ_INSTALL)/.sentinel:
	mkdir -p $(OBJ_INSTALL)
	touch $@

$(LIB_INSTALL)/.sentinel:
	mkdir -p $(LIB_INSTALL)
	touch $@


clean:
	rm -rf $(BIN_INSTALL) $(OBJ_INSTALL) $(LIB_INSTALL)
