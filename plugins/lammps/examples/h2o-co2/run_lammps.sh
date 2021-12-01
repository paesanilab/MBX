#!/bin/sh

env OMP_NUM_THREADS=1 mpirun -np 4 /path_to_lammps/lmp_mpi_mbx -in lmp.in
