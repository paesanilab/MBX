#!/bin/bash

module load gcc
module load intel
module unload mpi
module load intel/mpi

EXE=../../../bin/repimd

export OMP_NUM_THREADS=1
date > date.dat
mpirun -np 80 ${EXE} -N 1 \
       -n 10000 -p 500 -t 500 -s 0.0002 -R 15.0 -U 100.0 \
       -i initial -f final -o output -j trajectory -T 80 \
       -I initial.nrg -J mbx.json
date >> date.dat
