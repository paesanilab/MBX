#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

CMAKE_EXE=$HOME/codes/cmake-3.14.3/bin/cmake

module load gnu

if [ "$1" == "gnu" ]; then
  rm -rf build install
  $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -ftree-vectorize -ftree-vectorizer-verbose=2" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make CXX=g++ CC=gcc
  make install
  cd ../
  
  export MBX_HOME=$PWD
  cd plugins/i-pi/src/main/
  cp Makefile_comet_gnu Makefile
  make 
  cd -

elif [ "$1" == "mpi" ]; then
  module load intel
  module load intelmpi
  rm -rf build install
  $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS="-O0 -DHAVE_MPI=1 -fPIC -Wall" -DCMAKE_CXX_COMPILER=mpiicpc -DCMAKE_C_COMPILER=mpiicc -H. -Bbuild
  cd build
  make CXX=mpiicpc CC=mpiicc
  make install
  cd ../
  
  export MBX_HOME=$PWD
  cd plugins/i-pi/src/main/
  cp Makefile_comet_intel Makefile
  make 
  cd -
  
else
  module load intel
  rm -rf build install
  $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -Wall" -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc -H. -Bbuild
  cd build
  make CXX=icpc CC=icc
  make install
  cd ../
  
  export MBX_HOME=$PWD
  cd plugins/i-pi/src/main/
  cp Makefile_comet_intel Makefile
  make 
  cd -
fi
