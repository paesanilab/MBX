#!/bin/sh

#mpirun -np 4 /path_to_lammps/src/lmp_mpi_mbx -in lmp.in
env OMP_NUM_THREADS=4 mpirun -np 1 /path_to_lammps/src/lmp_mpi_mbx -in lmp.in
