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
./run_tests.sh 2> out
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

### i-pi
This software is already interfaced with i-pi. In order to run molecular dynamics using the MB-nrg PEFs, you will need to install i-pi first. Please go to [the i-pi github page](https://github.com/cosmo-epfl/i-pi-dev) and clone and follow the instructions to install i-pi. Before continuing with this, make sure i-pi is working. If you have any problems with the i-pi installation, you can ask a question in [the i-pi-user forum](https://groups.google.com/forum/#!forum/ipi-users)

After making sure that i-pi is working on yor machine:
```
cd plugins/i-pi/src/main/
export CU_HOME="FULL/PATH/TO/CLUSTERS/ULTIMATE/HOME"
make
```

A new file will be generated in `../../bin/`, called `driver`. Now we can run MD using i-pi. Go to the i-pi test folder in clusters_ultimate:
```
cd $CU_HOME/plugins/i-pi/test/MD/3h2o/100K/1-nvt/
```

This folder contains 3 files:
- `3h2o.nrg` is the energy software input. It needs to be in this same format. If you have more water molecules, just add the `MOLECULE` and `MONOMER` sections, add the OHH coordinates, and end the sections with ENDMON and ENDMOL.
- `3h2o.xyz` is the input for the coordinates for i-pi. The two files, `nrg` and `xyz`, should have exactly the same order, but the coordinates in the nrg file are not required to be the same as the ones in the XYZ file. XYZ will overwrite NRG.
- `3h2o.xml` is the i-pi input file. This simulation will run an NVT MD at 100K. Refer to the i-pi user manual for more information.

To run the simulation, in that same folder:
```
i-pi 3h2o.xml &
../../../../../bin/driver 3h2o.nrg 34543 localhost
```

These should initialize i-pi and start the simulation. Once the simulation is completed, terminate the i-pi instance and then run the NVE simulation in `$CU_HOME/plugins/i-pi/test/MD/3h2o/100K/2-nve`.
```
cd $CU_HOME/plugins/i-pi/test/MD/3h2o/100K/2-nve
cp ../1-nvt/RESTART ./restart.xml
i-pi restart.xml &
../../../../../bin/driver 3h2o.nrg 34543 localhost
```

i-pi can also perform PIMD, REMD, and REPIMD. Refer to the manual and the examples in i-pi to see how to set them up.

## Current Timings (v20190927)
These timings were obtained using 1, 2, 4, and 8 cores in our local workstation, performing 30 evaluations of the energy. When using PBC, the following settings for the PME part have been used (see `src/tests/n_single_point.cpp` for details)
- Electrostatics: Ewald alpha = 0.6, Grid density = 2.5, Spline order = 6
- Dispersion: Ewald alpha = 0.5, Grid density = 2.5, Spline order = 6

NOTE: The PBC scaling is not good due to the PBC part of the electrostatics. We are working on it.

|        Gas Phase                  |||
| Number of cores     | 256 H2O molecules | 512 H2O molecules | 
----------- | -------- | -------- |
 1 | 0.62603 s/eval | 1.79393  s/eval |  
 2 | 0.32677 s/eval | 0.90950  s/eval |
 4 | 0.17067 s/eval | 0.47167  s/eval |
 8 | 0.10073 s/eval | 0.27103  s/eval |

|        Periodic Boundary Conditions                  |||
| Number of cores     | 256 H2O molecules | 512 H2O molecules | 
----------- | -------- | -------- |
 1 | 1.32957 s/eval | 3.53920  s/eval |  
 2 | 0.75570 s/eval | 2.03077  s/eval |
 4 | 0.47490 s/eval | 1.26917  s/eval |
 8 | 0.32820 s/eval | 1.09107  s/eval |














