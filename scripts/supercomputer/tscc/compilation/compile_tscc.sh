#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

export MODULEPATH=/projects/builder-group/jpg/modulefiles/applications:$MODULEPATH
module load cmake/3.13.4

module load intel gsl openmpi_ib fftw
export PATH=$FFTWHOME/lib:$FFTWHOME/include:$PATH

if [ "$1" == "gnu" ]; then
  rm -rf build install
  cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -L$FFTWHOME/lib -I$FFTWHOME/include" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make -j 2 CXX=g++ CC=gcc
  make install
  cd ../

elif [ "$1" == "intel" ]; then
  echo "Sorry, this is not available here yet..."

fi
