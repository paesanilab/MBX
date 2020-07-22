MPICXX=mpiicpc
CXXFLAGS= -Wall -std=c++11  -O2

LIBS = -lmbxlib -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/install/lib/static

INCLUDEDIR = -I$(MBX_HOME)/install/include 

BINDIR = ../../bin

OBJDIR = ../../obj
REPIMDLIB_OBJ = nhc.o  pimd_v2.o sphere.o necklace.o io-frame.o io-xyz.o io-utils.o

AR = /usr/bin/ar

all: librepimd.a

librepimd.a: $(addprefix $(OBJDIR)/, $(REPIMDLIB_OBJ))
	$(AR) cru librepimd.a $(addprefix $(OBJDIR)/, $(REPIMDLIB_OBJ))
	mkdir -p ../../lib
	mv librepimd.a ../../lib

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

$(OBJDIR)/%.o: %.cpp $(OBJDIR)/.sentinel
	$(MPICXX) $(CXXFLAGS) $(INCLUDEDIR) $(LIBDIR) $(LIBS) -c $< -o $@

$(OBJDIR)/.sentinel:
	mkdir -p $(OBJDIR)
	touch $@

clean:
	rm -rf $(addprefix $(OBJDIR)/, $(REPIMDLIB_OBJ)) ../../lib/librepimd.a
