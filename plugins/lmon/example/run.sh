#!/bin/bash

if [ -z "$MBX_HOME" ]; then
  echo "MBX_HOME is not defined"
  exit
fi

# Get MBX and fortran input files
python3 $MBX_HOME/scripts/format_conversion/xyz2nrg.py input.xyz
python3 $MBX_HOME/scripts/format_conversion/xyz2f90.py input.xyz

# Calculate hessian and harmonic freqs
../bin/calc_hess > nm.dat

# Calculate displacements and assign modes
../bin/vib input.nrg mbx.json 500.0 > vibfreq.dat

# Calculate local monomer modes and get the line spectra with intensities
../bin/3d

# Calculate gaussian spectra from line spectra
../tools/gaussian_spectrum.py line_spectra.dat > ir_spectra.dat
