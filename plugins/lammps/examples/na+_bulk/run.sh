#!/bin/bash

# Uncomment for Paesanilab machines
#module unload mpi
#module load gcc intel intel/mpi

LAMMPS_HOME=$HOME/codes/LAMMPS-stable

env OMP_NUM_THREADS=2 mpirun -np 4 $LAMMPS_HOME/lmp_mpi_mbx -in lammps.in
