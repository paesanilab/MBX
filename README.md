[![MBX testing suite](https://github.com/paesanilab/MBX-dev/actions/workflows/github-actions-mbx.yml/badge.svg)](https://github.com/paesanilab/MBX-dev/actions/workflows/github-actions-mbx.yml)
[![codecov](https://codecov.io/gh/paesanilab/MBX-dev/branch/master/graph/badge.svg?token=4OE0CPMHGR)](https://codecov.io/gh/paesanilab/MBX-dev)
[![Homepage](https://img.shields.io/badge/google%20groups-mbx--users-green)](https://groups.google.com/g/mbx-users)

# MBX v0.8
MBX is a C++ software that provides an interface for MD drivers, such as LAMMPS (https://www.lammps.org) and i-PI (http://ipi-code.org), to perform classical and path-integral molecular dynamics simulations using our many-body potential energy functions. The current version of MBX includes the MB-pol many-body water potential (https://doi.org/10.1021/ct400863t, https://doi.org/10.1021/ct500079y, https://pubs.acs.org/doi/abs/10.1021/ct5004115) and the MB-nrg many-body potentials for neat CO2 and CO2/H2O mixtures (https://doi.org/10.1021/acs.jctc.9b01175, https://doi.org/10.1063/5.0080061), and neat CH4 and CH4/H2O mixtures (https://doi.org/10.1021/acs.jpcb.0c08728). MBX also includes the TTM-nrg potentials for the halide (https://doi.org/10.1021/acs.jpcb.5b09562) and alkali-metal (https://doi.org/10.1039/C6CP02553F) ions in water. The MB-nrg many-body potentials for the halide (https://doi.org/10.1021/acs.jctc.6b00302) and alkali-metal (https://doi.org/10.1063/1.4993213) ions in water will become available in the next release of MBX. For more details about the MB-pol, MB-nrg, and TTM-nrg potentials in MBX, please visit: https://paesanigroup.ucsd.edu/software/mbx.html.

MBX is periodically updated with performance improvements and the addition of other many-body potentials. For any questions about MBX, installation issues, or general usage inquiries, please use the MBX Google Group: https://groups.google.com/g/mbx-users.

## Compilation and Installation
The following requirements need to be fulfilled in order to successfully install the software
- g++/gcc v4.9 or higher [and icpc/icc v2017 or higher - optional]
- Read the entire README before doing anything

### Setup
The home directory of MBX will be referred to as `MBX_HOME`. You must set this environment variable, which can be done with the following command if the home directory of MBX is the current directory:
`export MBX_HOME=$PWD`

### Compilation
Please read the INSTALL.md instructions. After installation, a folder 
called `install` should have been created if no prefix has been given to `configure`.

## Testing
After installation, running the unit tests is highly recommended. Run the following commands to run the tests:
```
make check
```
All tests must pass. Please contact the code owners if there is any issue.
Tests won't pass if the compiler is an MPI compiler. Please compile first with g++
and check the tests.

## JSON File
To make life easier for you, a JSON configuration file must be used to pass 
all the information that MBX needs. Usually, one does not need to change 
anything except a couple of options. In any case, all the options of the 
json file are explained below.

The JSON file template is the following:
```
{
   "Note" : "This is a configuration file",
   "MBX" : {
       "box" : [21.0,0.0,0.0,0.0,21.0,0.0,0.0,0.0,21.0],
       "twobody_cutoff"   : 9.0,
       "threebody_cutoff" : 7.0,
       "dipole_tolerance" : 1E-8,
       "dipole_max_it"    : 100,
       "dipole_method"     : "cg",
       "alpha_ewald_elec" : 0.6,
       "grid_density_elec" : 2.5,
       "spline_order_elec" : 6,
       "alpha_ewald_disp" : 0.6,
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
- `box` is either a 9 element list, comma-separated and limited by brackets with the 3 vectors of the box: ax, ay, az, bx, by, bz, cx, cy, cz, or an empty list if one wants to run gas-phase calculations.
- `twobody_cutoff` is the distance at which the 2b interactions will be cut in the real space. If you are using polynomials, that should be the largest polynomial cutoff that you are using (usually 9.0 Angstrom) in periodic boundary conditions. In gas phase calculations, that should be set to a large number so the real space electrostatics and dispersion are properly calculated and fully accounted for.
- `threebody_cutoff` is the cutoff for the 3b polynomials. If only water is used, one can set that to 4.5, but if alkali metal ions or halides are used, it should be set to the maximum cutoff in any of the trimers used (7.0).
- `dipole_tolerance` is the tolerance accepted for the induced dipoles iterative calculation. From one iteration to the other one, |mu(i,t+1) - mu(i,t)|^2 < dipole tolerance for any i. 
- `dipole_max_it` is the maximum number of iterations allowed in the dipole iterative method calculation. If the number of iterations exceeds this value, MBX will throw an error message saying that the dipoles have diverged.
- `dipole_method` is the method adopted to calculate the induced dipoles. Current options are `iter` (iterative), `cg` (conjugate gradient, faster than iter), and `aspc` (always stable predictor-corrector), which should only be used in simulations. Since `aspc` is currently not interfaced with LAMMPS, the best option to run simulations with LAMMPS is to use `cg` with `dipole_tolerance` = 1E-08 (1E-06 may also be sufficient in some cases but, if it is used, it is recommended to verity the convergence of both structural and thermodynamic properties relative to simulations carried out with 1E-08).
- `alpha_ewald_XX` is the alpha used in the reciprocal space. It should be set to 0 when running gas-phase calculations/simulations.
- `grid_density_XX` is the number of grid points density.
- `spline_order_XX` is the order of the splines used for interpolation.
- `ttm_pairs` a list of 2 element lists with the monomer pairs for which the repulsion will be calculated using a Buckingham potential. If a pure TTM-nrg calculation is being performed, `ignore_2b_poly` should contain the same pairs as `ttm_pairs`. Example: `"ttm_pairs" : [["f","h2o"],["na","h2o"]]`
- `ignore_2b_poly` has the same format as `ttm_pairs`, but this will make MBX not to calculate the polynomials for the pairs specified.
- `ignore_3b_poly` has a similar format as 2b, but with the difference that the list is a list of 3-element list. If a set of three monomer types is specified in this list, MBX won't add the polynomial correction of that given trimer. Example: `"ignore_3b_poly" : [["na","h2o","h2o"]]`
- `port` is used when interfacing with i-pi. It is the port that will hold the socket. Should be greater than 34500.
- `localhost` is the name of the socket. It MUST match the name in the XML file, otherwise it will send an error saying that the socket was not found.

## Main executables
After installation, there will be the main executables in `$MBX_HOME/install/bin`.
- `single_point` will return the energy (Binding Energy) in kcal/mol for a given configuration. One can have multiple systems in the nrg file, and single point will return the energies of each one of them. If the flag to print gradients is activated (`PRINT_GRADS`; see source code in `$MBX_HOME/src/main/single_point.cpp`) it will also print the gradients.
- `optimize` will optimize a given configuration. You can optimize a single nrg system, or pass an XYZ file with a set of configurations, in which all of them will be optimized.

## PEFs implemented
All the PEFs implemented, along with examples of input files and scripts to run various types of simulations, are provided in `$MBX_HOME/examples/PEFs`. If a surface is not there, its usage is not recommended because it is either not tested or not finalized. Example calls with C++, Fortran and Python are located inside each of the corresponding folders.

Please cite the following manuscripts if any of the following PEFs is used:
- MB-pol
  * J. Chem. Theory Comput. 2013, 9, 12, 5395–5403
  * J. Chem. Theory Comput. 2014, 10, 4, 1599–1607
  * J. Chem. Theory Comput. 2014, 10, 8, 2906–2910
  * J. Chem. Phys. 145, 194504 (2016)
- MB-nrg PEFs for alkali-metal ions in water
  * Phys. Chem. Chem. Phys., 2016,18, 30334-30343 (TTM-nrg)
  * J. Chem. Phys. 147, 161715 (2017) (MB-nrg)
  * J. Phys. Chem. A 2018, 122, 27, 5811–5821 (MB-nrg)
- Halide Ions and water
  * J. Chem. Phys. 155, 064502 (2021) (Chloride MB-nrg)
  * J. Phys. Chem. B 2022, 126, 41, 8266–8278 (Bromide & Iodide MB-nrg)
- MB-nrg PEFs for CO2 and CO2/H2O mixtures
  * J. Chem. Theory Comput. 2020, 16, 4, 2246–2257
  * J. Chem. Phys. 2022, 156, 104503
- MB-nrg PEFs for CH4 and CH4/H2O mixtures
  * J. Phys. Chem. B 2020, 124, 49, 11207–11221
  * J. Chem. Phys. 2022, 156, 194504
- MB-nrg PEF for N2O5 in water
  * J. Chem. Theory Comput. 2021, 17, 7, 3931–3945
- TTM-nrg PEFs for halide and alkali-metal ions in water
  * J. Phys. Chem. B 2016, 120, 1822–1832
  * Phys. Chem. Chem. Phys. 2016,18, 30334-30343

## Interfaces
### Fortran90 and Python
In `${MBX_HOME}/examples/PEFs` there are sample scripts on how to use MBX called from Fortran90 and Python. Please remember to update the `LD_LIBRARY_PATH` variable and, if using python, the `PYTHONPATH` variable.
```
export LD_LIBRARY_PATH=MBX_HOME/install/lib/:$LD_LIBRARY_PATH
export PYTHONPATH=${PYTHONPATH}:${MBX_HOME}/plugins/python/mbx
```

Note that for these interfaces to work, they need the dynamic library of MBX.
You may need to rerun the `configure` script with the --enable-shared option.


### LAMMPS
You need to download LAMMPS from https://lammps.sandia.gov/download.html. The current version of MBX supports the LAMMPS version from September 29, 2021 that can be downloaded from GitHub: ```git clone -b stable_29Sep2021_update3 git@github.com:lammps/lammps.git```


MBX needs to be installed following the instructions provided in the previous sections. After installation:
```
cd MBX_HOME/plugins/lammps
vi Makefile.mpi_mbx
```
Make sure that the `MBX` variable is pointing to the right place. If you have your `MBX_HOME` environment variable set, you should be fine.

Let's call the directory where LAMMPS has been put/unpacked `LAMMPS_HOME`. 
Do the following:
```
cp Makefile.mpi_mbx LAMMPS_HOME/src/MAKE/Makefile.mpi_mbx
cd LAMMPS_HOME/src/
make yes-USER-MBX yes-MOLECULE yes-KSPACE yes-RIGID yes-EXTRA-PAIR
make yes-USER-MBX
make mpi_mbx -j 4
```
It is possible that there is a compilation error at this point regarding an undefined reference to `FIX_MBX`.
If so, while being in the src folder in LAMMPS, do the following:
```
rm style_fix.h style_pair.h 
make yes-USER-MBX yes-MOLECULE yes-KSPACE yes-RIGID yes-EXTRA-PAIR
make yes-USER-MBX
touch fix_mbx.* pair_mbx.*
make mpi_mbx -j 8
```

After this, a new executable `lmp_mpi_mbx` in `src` should appear, and that is the executable you have to use for LAMMPS.

Further doucmentation will follow up. For now, look at the examples in `MBX_HOME/plugins/lammps` to see how it is run. 

Additional documentation will follow up. For now, please look at the examples in `MBX_HOME/plugins/lammps` to see how it is run. For any questions, please use the MBX Google Group: https://groups.google.com/g/mbx-users.


### i-PI
This software is already interfaced with i-PI. In order to run molecular dynamics using the MB-nrg PEFs, you will need to install i-PI first. Please go to the [i-PI GitHub page] (https://github.com/i-pi/i-pi) and clone and follow the instructions to install i-PI. Before continuing with this, make sure i-PI is working. If you have any problems with the i-PI installation, you can ask a question in [the i-pi-user forum](https://groups.google.com/forum/#!forum/ipi-users). However, there is no need to install anything in i-PI. If you want to skip the testing (PROCEED AT YOUR OWN RISK), you can skip testing i-PI and assume it works.

After making sure that i-PI is working on your machine:
```
cd plugins/i-pi/src/main/
make
```

A new file will be generated in `../../bin/`, called `driver`. Now we can run MD using i-PI. Go to the i-PI test folder in MBX:
```
cd $MBX_HOME/plugins/i-pi/test/molecular_dynamics/gas_phase/3h2o/100K/1-nvt
```

This folder contains five files:
- `config.nrg` is the energy software input. It needs to be in this same format. If you have more water molecules, just add the `MOLECULE` and `MONOMER` sections, add the OHH coordinates, and end the sections with ENDMON and ENDMOL.
- `config.xyz` is the input for the coordinates for i-PI. The two files, `nrg` and `xyz`, should have exactly the same order, but the coordinates in the nrg file are not required to be the same as the ones in the XYZ file. XYZ will overwrite NRG.
- `config.xml` is the i-PI input file. This simulation will run an NVT MD at 100K. Refer to the i-pi user manual for more information.
- `mbx.json` is the MBX configuration file
- `run_i-pi.sh` will run the test. Make sure you sourced the env.sh in the i-PI folder before running the test, or most likely it will fail.

These should initialize i-PI and start the simulation. Once the simulation is completed, terminate the i-pi instance and then run the NVE simulation in `$MBX_HOME/plugins/i-pi/test/molecular_dynamics/gas_phase/3h2o/100K/2-nve`.
```
cd $MBX_HOME/plugins/i-pi/test/molecular_dynamics/gas_phase/3h2o/100K/2-nve
cp ../1-nvt/RESTART ./config.xml
./run_i-pi.sh
```

There are more tests for other types of simulations, including condensed phase simulations and replica-exchange simulations. For more information about what kind of simulations can i-PI run, please refer to the i-PI user manual.


## Coverage
The unit tests implemented should cover a big part of the code. This sunburst graph gives an idea of the coverage from top (center) to bottom (periphery). Our goal is to keep it as green as possible, being green good coverage, and red bad covergae.
<p align="center">
  <img src="https://codecov.io/gh/paesanilab/MBX-dev/branch/master/graphs/sunburst.svg?token=4OE0CPMHGR" />
</p>


