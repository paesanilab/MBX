FC = gfortran
FFLAGS = -lstdc++ -g

all: test test_pointer

test: test.f90 ../../install/lib/libmbnrg.so
	$(FC) test.f90 ../../install/lib/libmbnrg.so $(FFLAGS) -o test

test_pointer: test_pointer.f90 ../../install/lib/libmbnrg.so
	$(FC) test_pointer.f90 ../../install/lib/libmbnrg.so $(FFLAGS) -o test_pointer

clean: 
	rm -f test test_pointer

.PHONY: all clean
