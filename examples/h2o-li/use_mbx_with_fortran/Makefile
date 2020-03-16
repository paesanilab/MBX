FC = gfortran
FFLAGS = -lstdc++ -g

all: test_gas_phase test_pbc

test_pbc: test_pbc.f90 ../../../install/lib/libmbx.so
	$(FC) test_pbc.f90 ../../../install/lib/libmbx.so $(FFLAGS) -o test_pbc

test_gas_phase: test_gas_phase.f90 ../../../install/lib/libmbx.so
	$(FC) test_gas_phase.f90 ../../../install/lib/libmbx.so $(FFLAGS) -o test_gas_phase

clean: 
	rm -f test_gas_phase test_pbc

.PHONY: all clean
