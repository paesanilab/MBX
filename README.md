[![MBX testing suite](https://github.com/paesanilab/MBX-dev/actions/workflows/github-actions-mbx.yml/badge.svg)](https://github.com/paesanilab/MBX-dev/actions/workflows/github-actions-mbx.yml)
[![codecov](https://codecov.io/gh/paesanilab/MBX-dev/branch/master/graph/badge.svg?token=4OE0CPMHGR)](https://codecov.io/gh/paesanilab/MBX-dev)
[![Homepage](https://img.shields.io/badge/google%20groups-mbx--users-green)](https://groups.google.com/g/mbx-users)


# MBX v1.0.3
MBX: A many-body energy and force calculator for data-driven many-body simulations.
[J. Chem. Phys. 159, 054802 (2023)](https://doi.org/10.1063/5.0156036)



MBX is a C++ software that can either be used as a standalone software for calculating energies and forces of MB-nrg potential energy functions (PEFs) for the molecular systems of interest or interfaced with external molecular dynamics and Monte Carlo engines to perform classical and quantum simulations of the molecular system of interest across different thermodynamic states and phases, in both periodic and non-periodic conditions, using the corresponding MB-nrg PEFs.
The current version of MBX provides interfaces to LAMMPS (https://www.lammps.org) and i-PI (http://ipi-code.org) which allow for performing classical and path-integral molecular dynamics simulations using MB-nrg PEFs. 
For details on the MB-pol and MB-nrg PEFs, please visit: https://paesanigroup.ucsd.edu/software/mbx.html.

MBX is periodically updated with performance improvements and the addition of other MB-nrg PEFs. For any questions about MBX, installation issues, or general usage inquiries, please use the MBX Google Group: https://groups.google.com/g/mbx-users.

## Compilation and Installation
The following requirements need to be fulfilled in order to successfully install the software
- g++/gcc v4.9 or higher [and icpc/icc v2018 or higher - optional]
- FFTW libraries
- GSL libraries [optional, needed only for normal_modes executable]
- MPI compilers [optional, needed only for LAMMPS]
- Read the entire README before doing anything!

### Setup
The home directory of MBX will be referred to as `$MBX_HOME`. You must set this environment variable, which can be done with the following command if the home directory of MBX is the current directory:
```console
git clone https://github.com/paesanilab/MBX.git
cd MBX/
export MBX_HOME=$PWD
```

### Compilation


#### Basic installation of MBX (for use with i-PI, Python, Fortran, or standalone. **NOT LAMMPS**)
```console
autoreconf -fi
./configure
make && make install
```
After performing basic installation, you can [run the unit tests](#testing) to make sure everything is working properly.

#### [Alternative installation of MBX_MPI](#lammps) (**LAMMPS only**)
If you want to use MBX with LAMMPS, instead skip to the section about the [LAMMPS plugin](#lammps). This special installation using MPI is **only compatible with LAMMPS** and is incompatible with i-PI, Python, Fortran or standalone usage. If you need to use any of these other plugins, perform a separate [basic installation](#basic-installation-of-mbx-standalone-i-pi-python-or-fortran-not-lammps) in a different directory.

## Testing
After [basic installation](#basic-installation-of-mbx-standalone-i-pi-python-or-fortran-not-lammps), running the unit tests is highly recommended. Run the following commands to run the tests:
```
make check
```
All tests must pass. If you encounter any errors, please report them in the [MBX Google Group](https://groups.google.com/g/mbx-users).

Tests will fail if using an MPI compiler instead of `g++`/`icpc`, such as if you performed alternative installation with MBX_MPI with LAMMPS. Please instead perform [basic installation](#basic-installation-of-mbx-standalone-i-pi-python-or-fortran-not-lammps) first with g++ and then run the tests.

## JSON File
To make life easier for you, a JSON configuration file must be used to pass all the information that MBX needs. Usually, one does not need to change anything except a couple of options. In any case, all the options of the json file are explained below.

The JSON file template is the following:
```
{
   "Note" : "This is a  MBX v1.0.2 configuration file",
   "MBX" : {
       "box" : [21.0,0.0,0.0,0.0,21.0,0.0,0.0,0.0,21.0],
       "twobody_cutoff"   : 9.0,
       "threebody_cutoff" : 7.0,
       "dipole_tolerance" : 1E-08,
       "dipole_max_it"    : 100,
       "dipole_method"     : "cg",
       "alpha_ewald_elec" : 0.6,
       "grid_density_elec" : 2.5,
       "spline_order_elec" : 6,
       "alpha_ewald_disp" : 0.6,
       "grid_density_disp" : 2.5,
       "spline_order_disp" : 6,
       "ignore_2b_poly" : [],
       "ignore_3b_poly" : []
   } ,
   "i-pi" : {
       "localhost" : "localhost3",
       "port" : 34567
   }
}
```
In this file:
- `box` is either a 9 element list, comma-separated and limited by brackets with the 3 vectors of the box: ax, ay, az, bx, by, bz, cx, cy, cz, or an empty list if one wants to run gas-phase calculations.
- `twobody_cutoff` is the distance at which the 2-body interactions will be cut in the real space. If you are using polynomials, that should be the largest polynomial cutoff that you are using (usually 9.0 Angstrom) in periodic boundary conditions. In gas phase calculations, that should be set to a large number so the real space electrostatics and dispersion are properly calculated and fully accounted for.
- `threebody_cutoff` is the cutoff for the 3-body polynomials. If only water is used, one can set that to 4.5, but if alkali metal ions or halides are used, it should be set to the maximum cutoff in any of the trimers used (7.0).
- `dipole_tolerance` is the tolerance accepted for the induced dipoles iterative calculation. From one iteration to the other one, |mu(i,t+1) - mu(i,t)|^2 < dipole tolerance for any i. A value of 1E-08 is usually small enough. However, if the dipole solver used is aspc, the magnitude of the tolerance may have to be decreased up to 1E-10 or 1E-12. It is recommended to run a few thousand steps using aspc and cg for the dipole solver, and decide which is the dipole tolerance needed.
- `dipole_max_it` is the maximum number of iterations allowed in the dipole iterative method calculation. If the number of iterations exceeds this value, MBX will throw an error message saying that the dipoles have diverged.
- `dipole_method` is the method adopted to calculate the induced dipoles. Current options are `iter` (iterative), `cg` (conjugate gradient, faster than iter), and `aspc` (always stable predictor-corrector). `aspc` is only suitable for simulations. All three solvers are implemented and available through i-PI or LAMMPS. 
- `alpha_ewald_{elec/disp}` is the alpha used in the reciprocal space. It should be set to 0 when running gas-phase calculations/simulations.
- `grid_density_{elec/disp}` is the Ewald grid points density.
- `spline_order_{elec/disp}` is the order of the splines used for interpolation.
- `ignore_2b_poly` a list of 2 element lists with the monomer pairs for which the 2-body polynomials will not be calculated. Example: `"ignore_2b_poly" : [["na+","h2o"]]`
- `ignore_3b_poly` has a similar format as ignore_2b_poly, but with the difference that the list is a list of 3-element list. If a set of three monomer types is specified in this list, MBX won't calculate the 3-body polynomials for that given trimer. Example: `"ignore_3b_poly" : [["na+","h2o","h2o"]]`
- `localhost` is the name of the socket. It MUST match the name in the XML file, otherwise it will send an error saying that the socket was not found.
- `port` is used when interfacing with i-pi. It is the port that will hold the socket. Should be greater than `34500`.

## Main executables
After installation, there will be the main executables in `$MBX_HOME/bin/`.
- `single_point` will return the energy (Binding Energy) in kcal/mol for a given configuration. One can have multiple systems in the nrg file, and single point will return the energies of each one of them. If `PRINT_GRADS` is manually enabled in the source code in (`$MBX_HOME/src/main/single_point.cpp`) it will also print the gradients.
- `optimize` will optimize a given configuration. You can optimize a single nrg system, or pass an XYZ file with a set of configurations, in which all of them will be optimized.
- `mb_decomp` will compute energies for subsystems for the given system(s), then performs many-body decomposition and prints the n-body contribution for all subsystems. If the flag to skip many-body decomposition ("-e") is activated, it prints the binding energy for all subsystems.
- `order_frames` will compute the energies for a given list of configurations from the XYZ file, and rearranges the configuration frames in the order of increasing energy.
- `normal_modes` will compute the normal modes for an optimized nrg file.

## PEFs implemented
All the PEFs implemented, along with examples of input files and scripts to run various types of simulations, are provided in `$MBX_HOME/examples/PEFs`. If a surface is not there, its usage is not recommended because it is either not tested or not finalized. Example calls with C++, Fortran and Python are located inside each of the corresponding folders.

Please cite the corresponding manuscript whenever using MBX:
- [J. Chem. Phys. 159, 054802 (2023)](https://doi.org/10.1063/5.0156036)

Please cite the following manuscripts if any of the following PEFs is used:
- MB-pol for water
  * [J. Chem. Theory Comput. 9, 5395 (2013)](https://doi.org/10.1021/ct400863t)
  * [J. Chem. Theory Comput. 10, 1599 (2014)](https://doi.org/10.1021/ct500079y)
  * [J. Chem. Theory Comput. 10, 2906 (2014)](https://doi.org/10.1021/ct5004115)
  * [J. Chem. Phys. 145, 194504 (2016)](https://doi.org/10.1063/1.4967719)
  * [Acc. Chem. Res. 49, 1844 (2016)](https://doi.org/10.1021/acs.accounts.6b00285)
- MB-nrg PEFs for alkali-metal ions in water
  * [J. Chem. Phys. 147, 161715 (2017)](https://doi.org/10.1063/1.4993213)
  * [J. Phys. Chem. A 122, 5811 (2018)](https://doi.org/10.1021/acs.jpca.8b04106)
  * [J. Phys. Chem. Lett. 10, 406 (2019)](https://doi.org/10.1021/acs.jpclett.8b03829)
  * [J. Chem. Phys. 153, 044306 (2020)](https://doi.org/10.1063/5.0013101)
  * [J. Chem. Theory Comput. 16, 3055 (2020)](https://doi.org/10.1021/acs.jctc.0c00082)
  * [J. Phys. Chem. B. 126, 9349 (2022)](https://doi.org/10.1021/acs.jpcb.2c05674)
- MB-nrg PEFs for halide ions in water
  * [J. Chem. Theory Comput. 12, 2698 (2016)](https://doi.org/10.1063/5.0059445)
  * [J. Chem. Phys. 148, 102321 (2018)](https://doi.org/10.1063/1.5005540)
  * [J. Comp. Theory. Comput. 15, 2983 (2019)](https://doi.org/10.1021/acs.jctc.9b00064)
  * [Nat. Chem. 11, 367 (2019)](https://www.nature.com/articles/s41557-019-0220-2)
  * [J. Phys. Chem. Lett. 10, 2823 (2019)](https://doi.org/10.1021/acs.jpclett.9b00899)
  * [J. Phys. Chem. A 123, 2843 (2019)](https://doi.org/10.1021/acs.jpca.9b00816)
  * [Adv. Phys. X 4, 1631212 (2019)](https://doi.org/10.1080/23746149.2019.1631212)
  * [J. Chem. Phys. 155, 064502 (2021)](https://doi.org/10.1063/5.0059445)
  * [J. Phys. Chem. B 126, 8266 (2022)](https://doi.org/10.1021/acs.jpcb.2c04698)
- MB-nrg PEFs for CO<sub>2</sub> and CO<sub>2</sub>/H<sub>2</sub>O mixtures
  * [J. Chem. Theory Comput. 16, 2246 (2020)](https://doi.org/10.1021/acs.jctc.9b01175)
  * [J. Chem. Phys. 156, 104503 (2022)](https://doi.org/10.1063/5.0080061)
- MB-nrg PEFs for CH<sub>4</sub> and CH<sub>4</sub>/H<sub>2</sub>O mixtures
  * [J. Phys. Chem. B 124, 11207 (2020)](https://doi.org/10.1021/acs.jpcb.0c08728)
  * [J. Chem. Phys. 156, 194504 (2022)](https://doi.org/10.1063/5.0089773)
- MB-nrg PEF for N<sub>2</sub>O<sub>5</sub> in water
  * [J. Chem. Theory Comput. 17, 3931 (2021)](https://doi.org/10.1021/acs.jctc.1c00069)

## Interfaces
### Fortran90 and Python
In `${MBX_HOME}/examples/PEFs` there are sample scripts on how to use MBX called from Fortran90 and Python. Please remember to update the `LD_LIBRARY_PATH` variable and, if using python, the `PYTHONPATH` variable.
```console
export LD_LIBRARY_PATH=MBX_HOME/lib/:$LD_LIBRARY_PATH
export PYTHONPATH=${PYTHONPATH}:${MBX_HOME}/plugins/python/mbx
```


### LAMMPS
MBX can interface with [LAMMPS](https://lammps.sandia.gov/) using a plugin for LAMMPS. In order to use MBX with LAMMPS, you must first install MBX with MPI compilers. You must use `mpicxx` or `mpiicpc` instead of `g++` or `icpc`. This can be done by running the following commands:
```console
# install MBX with MPI
autoreconf -fi
./configure --enable-mpi CXX=mpiicpc
make && make install
```

 After installing MBX, you can then download the stable branch of LAMMPS and then compile it with the MBX plugin:
```console
git clone -b stable git@github.com:lammps/lammps.git LAMMPS-stable
export $LAMMPS_HOME=$PWD/LAMMPS-stable

cp -rf $MBX_HOME/plugins/lammps/USER-MBX $LAMMPS_HOME/src
cd $LAMMPS_HOME/src/
make yes-USER-MBX yes-MOLECULE yes-KSPACE yes-RIGID yes-EXTRA-PAIR
make mpi_mbx -j 4 CXX=mpiicpc
```
After this, a new executable `lmp_mpi_mbx` in `$LAMMPS_HOME/src` should appear, and that is the executable you have to use for LAMMPS.

Additional documentation will follow up. For now, please look at the examples in `MBX_HOME/plugins/lammps` to see how it is run. For any questions, please use the MBX Google Group: https://groups.google.com/g/mbx-users.


### i-PI
This software is already interfaced with i-PI. In order to run molecular dynamics using the MB-nrg PEFs, you will need to install i-PI first.
```console
git clone https://github.com/i-pi/i-pi.git
```
 Before continuing with this, make sure i-PI is working. If you have any problems with the i-PI installation, you can ask a question in [the i-pi-user forum](https://groups.google.com/forum/#!forum/ipi-users). If you want to skip the testing (PROCEED AT YOUR OWN RISK), you can skip testing i-PI and assume it works.

After making sure that i-PI is working on your machine and having compiled MBX, you can use the i-PI driver located in `$MBX_HOME/bin/ipi_driver` to run MD using i-PI. To confirm that the driver is working go to the i-PI examples folder in MBX:
```console
cd $MBX_HOME/plugins/i-pi/test/molecular_dynamics/gas_phase/3h2o/100K/1-nvt
```

This folder contains five files:
- `config.xyz` is the input for the coordinates for i-PI, while `config.nrg` is the input format for MBX. These two files `xyz` and `nrg` are related, so if you make any changes to the xyz file you must correspondingly update the nrg file using `$MBX_HOME/scripts/format_conversion config.xyz`
- `config.xml` is the i-PI input file. This simulation will run an NVT MD at 100K. Refer to the i-pi user manual for more information.
- `mbx.json` is the MBX configuration file
- `run_i-pi.sh` will run the test. Make sure you `source  env.sh` in the i-PI folder before running the test, or most likely it will fail.

These should initialize i-PI and start the simulation. Once the simulation is completed, terminate the i-pi instance and then run the NVE simulation in `$MBX_HOME/plugins/i-pi/test/molecular_dynamics/gas_phase/3h2o/100K/2-nve`.
```
cd $MBX_HOME/plugins/i-pi/examples/molecular_dynamics/gas_phase/3h2o/100K/2-nve
cp ../1-nvt/RESTART ./config.xml
./run_i-pi.sh
```

There are more tests for other types of simulations, including condensed phase simulations and replica-exchange simulations. For more information about what kind of simulations can i-PI run, please refer to the i-PI user manual.


## Coverage
The unit tests implemented should cover a big part of the code. This sunburst graph gives an idea of the coverage from top (center) to bottom (periphery). Our goal is to keep it as green as possible, being green good coverage, and red bad coverage.
<p align="center">
  <img src="https://codecov.io/gh/paesanilab/MBX-dev/branch/master/graphs/sunburst.svg?token=4OE0CPMHGR" />
</p>


