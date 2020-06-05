#!/bin/bash

module load gcc
module load intel
module unload mpi
module load intel/mpi

EXE=../../../bin/repimd

export OMP_NUM_THREADS=1

mpirun -n 8 ${EXE} -N 1 \
       -n 2500 -p 500 -t 500 -s 0.0002 -R 15.0 -U 100.0 \
       -i initial -f final -o output -j trajectory -T 8 \
       -I initial.nrg -J mbx.json

