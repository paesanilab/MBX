#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

CMAKE_EXE=$HOME/bin/bin/cmake


#export MODULEPATH=/projects/builder-group/jpg/modulefiles/applications:$MODULEPATH
#module load cmake/3.13.4

#module load intel gsl openmpi_ib fftw
#export PATH=$FFTWHOME/lib:$FFTWHOME/include:$PATH

if [ "$1" == "gnu" ]; then
  module load gcc-compilers
  module load costinit
  module load fftw3/gnu/3.3.5
  rm -rf build install
  $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -L$FFTW3_HOME/lib -I$FFTW3_HOME/include" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make -j 2 CXX=g++ CC=gcc
  make install
  cd ../

elif [ "$1" == "intel" ]; then
  module load gcc-compilers
  module load intel-compilers
  module load costinit
  module load fftw3/intel/3.3.5
  rm -rf build install
  $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -L$FFTW3_HOME/lib -I$FFTW3_HOME/include" -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc -H. -Bbuild
  cd build
  make -j 2 CXX=icpc CC=icc
  make install
  cd ../
fi
