F90=gfortran
F90FLAGS= -lstdc++ -g -O2

ifndef MBX_HOME
$(info "MBX_HOME is not set. Please set it or ignore if the default $(HOME)/codes/MBX is OK")
MBX_HOME = $(HOME)/codes/MBX
endif

LIBS = -lmbx -fopenmp -lfftw3
LIBDIR = -L$(MBX_HOME)/lib

INCLUDEDIR = -I$(MBX_HOME)/include

BINDIR = ./

OBJDIR = ./

all: example example_pbc

example: example.f90 $(BINDIR)/.sentinel
	$(F90) $(F90FLAGS) $(INCLUDEDIR) $(LIBDIR) example.f90 $(LIBS) -o $@

example_pbc: example_pbc.f90 $(BINDIR)/.sentinel
	$(F90) $(F90FLAGS) $(INCLUDEDIR) $(LIBDIR) example_pbc.f90 $(LIBS) -o $@

$(BINDIR)/.sentinel:
	mkdir -p $(BINDIR)
	touch $@

clean:
	rm -rf example example_pbc
