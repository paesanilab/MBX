ifndef MBX_HOME
$(info "MBX_HOME is not set. Please set it or ignore if the default $(HOME)/codes/MBX is OK")
MBX_HOME = $(HOME)/codes/MBX
endif

F90 = ifort 
F90FLAGS = -O3 -lstdc++
F90LFLAGS = -L/usr/lib64 -L$(MBX_HOME)/install/lib/static/
F90OBJDIR = ../obj_f90/

CXX = icpc
CXXFLAGS = -std=c++11  -O0 -g
CXXLFLAGS = -L$(MBX_HOME)/install/lib/static  -I$(MBX_HOME)/install/include 
CXXOBJDIR = ../obj_cxx/

CXXLIBS = -lmbxlib -fopenmp -lfftw3 

F90LIBS = -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lmbxlib 

F90_COMPILE  = $(F90) $(F90FLAGS) -c
F90_LOAD     = $(F90) $(F90FLAGS) $(F90LFLAGS) 

CXX_COMPILE  = $(CXX) $(CXXFLAGS) -c
CXX_LOAD     = $(CXX) $(CXXFLAGS) $(CXXLFLAGS)

EXES = dirs calc_hess vib 3d
OBJS_H = globals.o fac.o matrixos4.o calc_hess.o
OBJS_VIB = AllocDouble.o diagon.o eigsrt.o jacobi.o matmultAB.o matmultAtB.o matmultABt.o matadd.o mataddinc.o matcopy.o trace.o printmat.o
OBJS_3D = globals.o fac.o matrixos4.o 3d.o

all: $(EXES)

dirs:
	mkdir -p ../bin ../obj_f90 ../obj_cxx

calc_hess: $(addprefix $(F90OBJDIR)/,$(OBJS_H))
	$(F90_LOAD) $(addprefix $(F90OBJDIR)/,$(OBJS_H)) $(F90LIBS) -o ../bin/calc_hess 

vib: vib.cpp $(addprefix $(CXXOBJDIR)/,$(OBJS_VIB))
	$(CXX_LOAD) $(addprefix $(CXXOBJDIR)/,$(OBJS_VIB)) vib.cpp $(CXXLIBS) -o ../bin/vib

3d: $(addprefix $(F90OBJDIR)/,$(OBJS_3D))
	$(F90_LOAD) $(addprefix $(F90OBJDIR)/,$(OBJS_3D)) $(F90LIBS) -o ../bin/3d 

$(F90OBJDIR)/%.o: %.f90
	$(F90_COMPILE) $< -o $@
$(F90OBJDIR)/%.mod: %.f90
	$(F90_COMPILE) $< -o $@

$(CXXOBJDIR)/%.o: %.cpp
	$(CXX_COMPILE) $< -o $@

clean:
	rm -f ../obj_f90 ../obj_cxx ../bin


