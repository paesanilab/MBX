#!/bin/bash


if [ $# -ne 2 ]; then
  echo "Usage: $0 n_omp n_mpi"
  exit
fi

MBX_HOME=$HOME/codes/MBX-dev
LAMMPS_HOME=$HOME/codes/lammps
env OMP_NUM_THREADS=$1 mpirun -np $2 $LAMMPS_HOME/lmp_mpi_mbx -in lammps.in > out

