#!/bin/bash

module load gcc
module unload mpi
module load intel/mpi

LAMMPS_HOME=$HOME/codes/LAMMPS-stable/
export OMP_NUM_THREADS=1
mpirun -np 1 $LAMMPS_HOME/lmp_mpi_mbx -in lammps.in > out

