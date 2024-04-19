#!/bin/bash

# # Uncomment for Paesanilab machines
# module unload mpi
# module load gcc intel intel/mpi

LAMMPS_HOME=$HOME/software/LAMMPS-stable

MPI_PROC=4        # number of mpi processes
OMP_NUM_THREADS=2 # number of openmp threads per mpi process

# optional command line arguments to override the default mpi and openmp settings
if [ $# -eq 2 ]; then
    MPI_PROC=$1
    OMP_NUM_THREADS=$2
fi

echo "Running MBX+LAMMPS using MPI=$MPI_PROC and OMP=$OMP_NUM_THREADS"

export OMP_NUM_THREADS
mpirun -np ${MPI_PROC} $LAMMPS_HOME/src/lmp_mpi_mbx -in lammps.in

if [ -s log.lammps ] && grep -q "Total wall time" log.lammps; then
    date | tee -a performance.txt
    echo "<$(hostname -s)>" | tee -a performance.txt
    grep 'ns/day' log.lammps | tee -a performance.txt
    grep '% CPU use' log.lammps | tee -a performance.txt
    grep 'Total wall time' log.lammps | tee -a performance.txt
    echo "-----------------------------" | tee -a performance.txt
fi
