[![Build Status](https://travis-ci.org/chemphys/clusters_ultimate.svg?branch=master)](https://travis-ci.org/chemphys/clusters_ultimate)
[![codecov](https://codecov.io/gh/chemphys/clusters_ultimate/branch/master/graph/badge.svg)](https://codecov.io/gh/chemphys/clusters_ultimate)

# clusters_ultimate
DO NOT CLONE. Currently under development. This code will be able to calculate the energy of a set of systems composed by molecules, each molecule composed by monomers. 

## Compilation and Installation
The following requirements need to be fulfilled in order to succesfully install the software
- CMake v3.9 or higher
- g++/gcc v4.9 or higher [and icpc/icc v2017 or higher - optional]
- Read the entire README before doing anything

In order to compile the code, do the following in the software home directory:
```
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DUSE_OPENMP:BOOL=TRUE/FALSE -H. -Bbuild
cd build
make
make install
```
If you have intel compilers in your machine, it is highly recommended to replace g++/gcc by icpc/icc
If you want to run the code in parallel, set the `-DUSE_OPENMP:BOOL=TRUE`, and it will compile it to use OpenMP parallelization. In that case, set the environment variable `OMP_NUM_THREADS=N`, where `N` is the number of threads you wanna use. At 05/11/2018:
- Systems with less than 16 monomers should not use more than 2 threads
- Systems with less than 64 monomers should not use more than 8 threads
- Any system should use more than one node.
At this point, a folder called `install` should have been created.

## Testing
In order to make sure that the installations has been done properly, run the tests. In the home directory of the software:
```
cd tests
./run_tests.sh
```
If all tests are passing, you are good to go!

## Interface
### Fortran90
In tests/fortran there is an example on how to use the recently compiled libraries from fortran. Please see the file `test.f90` to see how the energy function must be called. In order to compile and run the fortran test:
```
export LD_LIBRARY_PATH=$PWD/install/lib/:$LD_LIBRARY_PATH
cd tests/fortran
make
./test
```
The output should be the same as the `expected_output`.
