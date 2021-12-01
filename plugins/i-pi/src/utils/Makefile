CXX=icpc
CXXFLAGS= -Wall -std=c++11 -g

LIBS = -lmbxlib 
LIBDIR = -L$(MBX_HOME)/install/lib/static

INCLUDEDIR = -I$(MBX_HOME)/install/include

BINDIR = ../../bin
OBJDIR = ../../obj

all: perturb.o read_xyz.o

perturb.o: perturb.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) perturb.cpp -c -o perturb.o
	mv perturb.o $(OBJDIR)/

read_xyz.o: read_xyz.cpp $(OBJDIR)/.sentinel
	$(CXX) $(CXXFLAGS) read_xyz.cpp -c -o read_xyz.o
	mv read_xyz.o $(OBJDIR)/

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(OBJDIR)
