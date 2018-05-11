FC = gfortran
FFLAGS = -lstdc++ -g

all: test

test: test.f90 ../../install/lib/libmbnrg.so
	$(FC) -o test ../../install/lib/libmbnrg.so test.f90 $(FFLAGS)

clean: 
	rm -f test

.PHONY: all clean
