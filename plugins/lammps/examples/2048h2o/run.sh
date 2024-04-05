#!/bin/bash


if [ $# -ne 2 ]; then
  echo "Usage: $0 n_omp n_mpi"
  exit
fi

MBX_HOME=$HOME/MBX-dev+LAMMPS/MBX-dev
LAMMPS_HOME=$HOME/MBX-dev+LAMMPS
gnv OMP_NUM_THREADS=$1 mpirun -np $2 $LAMMPS_HOME/lmp_mpi_mbx -in lammps.in > out

