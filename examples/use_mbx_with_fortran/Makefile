FC = gfortran
FFLAGS = -lstdc++ -g

all: test 

test: test.f90 ../../install/lib/libmbnrg.so
	$(FC) test.f90 ../../install/lib/libmbnrg.so $(FFLAGS) -o test

clean: 
	rm -f test 

.PHONY: all clean
