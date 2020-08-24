#!/bin/bash

#PBS -N compile_MBX
#PBS -q home
#PBS -l walltime=01:00:00,nodes=1:ppn=16
#PBS -e stderr 
#PBS -o stdout 
#PBS -M your_email_address
#PBS -m abe
##PBS -m ae
#PBS -m n 
#PBS -V

export MBX_HOME=$HOME/software/MBX

module load intelmpi gsl openmpi_ib fftw
export PATH=$FFTWHOME/lib:$FFTWHOME/include:$PATH

cd $MBX_HOME 
rm -rf build install
cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS="-DHAVE_MPI=1 -fPIC -O2 -Wall -L$FFTWHOME/lib -I$FFTWHOME/include" -DCMAKE_CXX_COMPILER=mpiicpc -DCMAKE_C_COMPILER=mpiicc -H. -Bbuild
cd build
make -j 8 CXX=mpiicpc CC=mpiicc
make install
cd ../

echo
echo "MBX LIBRARY COMPILED!"
echo
