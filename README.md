[![Build Status](https://travis-ci.org/chemphys/MBX.svg?branch=master)](https://travis-ci.org/chemphys/MBX)
[![codecov](https://codecov.io/gh/chemphys/MBX/branch/master/graph/badge.svg)](https://codecov.io/gh/chemphys/MBX)

# MBX
This is the version 0.2.1a. This software should not be used unless the Paesani Lab has given explicit permission and specified which parts can be used. this software is still under development, and might not work as expected for situations that have not been tested.

What's new in v0.2.1a
- The version name

## Compilation and Installation
The following requirements need to be fulfilled in order to succesfully install the software
- CMake v3.9 or higher
- g++/gcc v4.9 or higher [and icpc/icc v2017 or higher - optional]
- Read the entire README before doing anything

In order to compile the code, do the following in the software home directory. 
If you want to run the code in parallel, set the `-DUSE_OPENMP:BOOL=TRUE`, and it will compile it to use OpenMP parallelization. In that case, set the environment variable `OMP_NUM_THREADS=N`, where `N` is the number of threads you wanna use. At 05/11/2018:
- Systems with less than 16 monomers should not use more than 2 threads
- Systems with less than 64 monomers should not use more than 8 threads
- Any system should use more than one node.
At this point, a folder called `install` should have been created.
```
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DUSE_OPENMP:BOOL=TRUE/FALSE -H. -Bbuild
cd build
make
make install
```
If you have intel compilers in your machine, it is highly recommended to replace g++/gcc by icpc/icc

## Testing
In order to make sure that the installations has been done properly, run the tests. In the home directory of the software:
```
cd tests
./run_tests.sh 2> out
```
If all tests are passing, you are good to go!

## Json File
To make life easier for you, a json configuration file can be used to pass all the information that MBX needs. Usually, one does not need to change anything except a couple of options. In any case, all the options of the json file are explained below.

The jason file template is the following:
```
{
   "Note" : "This is a cofiguration file",
   "MBX" : {
       "box" : [21.0,0.0,0.0,0.0,21.0,0.0,0.0,0.0,21.0],
       "twobody_cutoff"   : 9.0,
       "threebody_cutoff" : 7.0,
       "max_n_eval_1b"    : 500,
       "max_n_eval_2b"    : 500,
       "max_n_eval_3b"    : 500,
       "dipole_tolerance" : 1E-16,
       "dipole_max_it"    : 100,
       "dipole_method"     : "aspc",
       "alpha_ewald_elec" : 0.25,
       "grid_density_elec" : 2.5,
       "spline_order_elec" : 6,
       "alpha_ewald_disp" : 0.25,
       "grid_density_disp" : 2.5,
       "spline_order_disp" : 6,
       "ttm_pairs" : [],
       "ignore_2b_poly" : [],
       "ignore_3b_poly" : []
   } ,
   "i-pi" : {
       "port" : 34567,
       "localhost" : "localhost3"
   }
}
```
In this file:
- `box` is either a 9 element list, coma separated and limited by brackets with the 3 vectors of the box: ax, ay, az, bx, by, bz, cx, cy, cz, or an empty list if one wants to run gas phase calculations.
- `twobody_cutoff` is the distance at which the 2b interactions will be cut in the real space. If you are using polynomials, that should be the largest polynomial cutoff that you are using (usually 9.0 Angstrom) in periodic boundary conditions. In gas phase calculations, that should be set to a large number so the real space electrostatics and dispersion are properly calculated and fully accounted for.
- `threebody_cutoff` is the cutoff for the 3b polynomials. If only water is used, one can set that to 4.5, but if alkali metal ions or halides are used, it should be set to the maximum cutoff in any of the trimers used (7.0).
- `max_n_eval_xb` is the number of evaluations that will be passed at once to the polynomials. Currently it has not much effect, since the polynomial files are not vectorized. It should be set at 500 or just removed from mbx.json.
- `dipole_tolerance` is the tolerance accepted for the induced dipoles iterative calculation. From one iteration to the other one, |mu(i,t+1) - mu(i,t)|^2 < dipole tolerance for any i. 
- `dipole_max_it` is the maximum number of iterations allowed in the dipole iterative method calculation. If the number of iterations exceeds this value, MBX will throw an error message saying that the dipoles have diverged.
- `dipole_method` is the method that will be used to calculate the induced dipoles. Current options are `iter` (iterative), `cg` (conjugate gradient, faster than iter), and `aspc` (always stable predictor corrector), whoch should only be used in simulations.
- `alpha_ewald_XX` is the alpha used in the reciprocal space. Should be set to 0 when runing a gas phase calculation.
- `grid_density_XX` is the number of grid points density.
- `spline_order_XX` is the order of the splines used for interpolation.
- `ttm_pairs` a list of 2 element lists with the monomer pairs for which the repulsion will be calculated using the buckingham. If a pure TTM-nrg calculation is being performed, `ignore_2b_poly` should contain the same pairs as `ttm_pairs`. Example: `"ttm_pairs" : [["f","h2o"],["na","h2o"]]`
- `ignore_2b_poly` has the same format as `ttm_pairs`, but this will make MBX not to calculate the polynomials for the pairs specified.
- `ignore_3b_poly` has a similar format as 2b, but with the difference that the list is a list of 3-element list. If a set of three monomer types is specified in this list, MBX won't add the polynomial correction of that given trimer. Example: `"ignore_3b_poly" : [["na","h2o","h2o"]]`
- `port` is used when interfacing with i-pi. Is the port that will hold the socket. Should be greater than 34500.
- `localhost` is the name of the socket. It MUST match the name in the xml file, otherwise it will send an error saying that the socket was not found.

## Run TTM-nrg with MBX
It is possible to use the classical polarizable model TTM-nrg with MB-pol using MBX. An example is provided in `MBX_HOME/examples/ttm-nrg_with_mbx`. To do so, just prepare a json file as usual, but add the pairs that you want to calculate with TTM-nrg in the `ttm_pairs` section of the `mbx.json` file. It is recommended to add also the pairs in `ignore_2b_poly` and the trimers involving that species in `ignore_3b_poly` sections (see example).

## Interface
### Fortran90
In `examples/use_mbx_with_fortran` there is an example on how to use the recently compiled libraries from fortran. Please see the files `test_pbc.f90` (for pbc calculations with fortran) and `test_gas_phase.f90` (for gas phase calculations with fortran) to see how the energy function must be called. In order to compile and run the fortran test (replace `MBX_HOME` by the actual path to MBX home):
```
export LD_LIBRARY_PATH=MBX_HOME/install/lib/:$LD_LIBRARY_PATH
cd MBX_HOME/examples/use_mbx_with_fortran
make
./test_gas_phase
./test_pbc
```
The output should be the same as the `expected_output` for each one of the tests.

### i-pi
This software is already interfaced with i-pi. In order to run molecular dynamics using the MB-nrg PEFs, you will need to install i-pi first. Please go to [the i-pi github page](https://github.com/i-pi/i-pi) and clone and follow the instructions to install i-pi. Before continuing with this, make sure i-pi is working. If you have any problems with the i-pi installation, you can ask a question in [the i-pi-user forum](https://groups.google.com/forum/#!forum/ipi-users). However, there is no need to install anything in i-pi. Just have it on your computer, so if you want skip the testing (PROCEED AT YOUR OWN RISK), you can skip testing i-pi and assume it works.

After making sure that i-pi is working on yor machine:
```
cd plugins/i-pi/src/main/
export MBX_HOME="FULL/PATH/TO/CLUSTERS/ULTIMATE/HOME"
make
```

A new file will be generated in `../../bin/`, called `driver`. Now we can run MD using i-pi. Go to the i-pi test folder in MBX:
```
cd $MBX_HOME/plugins/i-pi/test/MD/3h2o/100K/1-nvt/
```

This folder contains 5 files:
- `3h2o.nrg` is the energy software input. It needs to be in this same format. If you have more water molecules, just add the `MOLECULE` and `MONOMER` sections, add the OHH coordinates, and end the sections with ENDMON and ENDMOL.
- `3h2o.xyz` is the input for the coordinates for i-pi. The two files, `nrg` and `xyz`, should have exactly the same order, but the coordinates in the nrg file are not required to be the same as the ones in the XYZ file. XYZ will overwrite NRG.
- `3h2o.xml` is the i-pi input file. This simulation will run an NVT MD at 100K. Refer to the i-pi user manual for more information.
- `mbx.json` is the MBX configuration file
- `run.sh` will run the test. Make sure you sourced the env.sh in the i-pi folder before running the test, or most likely it will fail.

These should initialize i-pi and start the simulation. Once the simulation is completed, terminate the i-pi instance and then run the NVE simulation in `$CU_HOME/plugins/i-pi/test/MD/3h2o/100K/2-nve`.
```
cd $CU_HOME/plugins/i-pi/test/MD/3h2o/100K/2-nve
cp ../1-nvt/RESTART ./3h2o.xml
./run.sh
```

There are other tests for gas phase and PBC in the `plugins/i-pi/tests/` folder. i-pi can also perform PIMD, REMD, and REPIMD. Refer to the manual and the examples in i-pi to see how to set them up.

## Current Timings (v20190927)
These timings were obtained using 1, 2, 4, and 8 cores in our local workstation, performing 30 evaluations of the energy. When using PBC, the following settings for the PME part have been used (see `src/tests/n_single_point.cpp` for details)
- Electrostatics: Ewald alpha = 0.6, Grid density = 2.5, Spline order = 6
- Dispersion: Ewald alpha = 0.5, Grid density = 2.5, Spline order = 6

NOTE: The PBC scaling is not good due to the PBC part of the electrostatics. We are working on it.

Gas Phase
| Number of cores     | 256 H2O molecules | 512 H2O molecules | 
----------- | -------- | -------- |
 1 | 0.62603 s/eval | 1.79393  s/eval |  
 2 | 0.32677 s/eval | 0.90950  s/eval |
 4 | 0.17067 s/eval | 0.47167  s/eval |
 8 | 0.10073 s/eval | 0.27103  s/eval |

Periodic Boundary Conditions
| Number of cores     | 256 H2O molecules | 512 H2O molecules | 
----------- | -------- | -------- |
 1 | 1.32957 s/eval | 3.53920  s/eval |  
 2 | 0.75570 s/eval | 2.03077  s/eval |
 4 | 0.47490 s/eval | 1.26917  s/eval |
 8 | 0.32820 s/eval | 1.09107  s/eval |














