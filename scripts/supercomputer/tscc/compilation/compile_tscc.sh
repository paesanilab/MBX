#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

export MBX_HOME=$HOME/codes/MBX

export MODULEPATH=/projects/builder-group/jpg/modulefiles/applications:$MODULEPATH
module load cmake/3.13.4

module load intel/2018.1.163 gsl openmpi_ib fftw
module load gnu

export PATH=$FFTWHOME/lib:$FFTWHOME/include:$PATH

rm -rf build install
cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -L$FFTWHOME/lib -I$FFTWHOME/include" -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc -H. -Bbuild
cd build
make -j 2 CXX=icpc CC=icc
make install
cd ../

# Compile the driver
cd plugins/i-pi/src/main
cp Makefile_tscc Makefile
make
cd -


