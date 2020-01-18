#!/bin/bash

#PBS -N compile_MBX
#PBS -q debug
#PBS -e stderr 
#PBS -o stdout 
#PBS -M mrierari@ucsd.edu
#PBS -l walltime=0:30:00
#PBS -l select=1:ncpus=36:mpiprocs=36
#PBS -A AFOSR40693129
#PBS -j oe
#PBS -V
#PBS -S /bin/bash

MBX_HOME=$HOME/codes/MBX
CMAKE_EXE=$HOME/bin/bin/cmake

module load gcc-compilers
module load intel-compilers
module load costinit
module load fftw3/intel/3.3.5

cd $MBX_HOME 
rm -rf build install
$CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -L$FFTW3_HOME/lib -I$FFTW3_HOME/include" -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc -H. -Bbuild
cd build
make -j $(($BC_CORES_PER_NODE / 2)) CXX=icpc CC=icc
make install
cd ../
