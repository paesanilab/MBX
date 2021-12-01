#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

module load cmake/3.12.4
module load gcc

if [ "$1" == "gnu" ]; then
  rm -rf build install
  cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=False -DCMAKE_COMPILE_TESTS=TRUE -DCMAKE_CXX_FLAGS=" -fPIC -O0 -Wall -ftree-vectorize -ftree-vectorizer-verbose=2" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make -j 8 --no-print-directory CXX=g++ CC=gcc
  make install
  cd ../

elif [ "$1" == "gnudebug" ]; then
  rm -rf build install
  cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=False -DCMAKE_COMPILE_TESTS=TRUE -DCMAKE_CXX_FLAGS=" -fPIC -O0 -Wall -ftree-vectorize -DDEBUG -ftree-vectorizer-verbose=2" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make -j 8 --no-print-directory CXX=g++ CC=gcc
  make install
  cd ../

elif [ "$1" == "intel" ]; then
  rm -rf build install
  cmake -DUSE_OPENMP=TRUE -DCMAKE_COMPILE_TESTS=FALSE -DCMAKE_CXX_FLAGS=" -g -Wall -qopt-report -fPIC " -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc -H. -Bbuild
  cd build
  make -j 8 CXX=icpc CC=icc 
  make install
  cd ../

elif [ "$1" == "mpi" ]; then
  module unload mpi
  module load intel/mpi
  rm -rf build install
  cmake -DUSE_OPENMP=TRUE -DCMAKE_CXX_FLAGS=" -DHAVE_MPI=1 -Wall -qopt-report -fPIC " -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=mpiicpc -DCMAKE_C_COMPILER=mpiicc -H. -Bbuild
  cd build
  make -j 8 CXX=mpiicpc CC=mpiicc
  make install
  cd ../

elif [ "$1" == "clang" ]; then
  module load clang
  cmake -DUSE_OPENMP=FALSE -DCMAKE_CXX_FLAGS=" -g -Wall -fPIC -O0" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -H. -Bbuild
  cd build
  make -j 8 CXX=clang++ CC=clang
  make install
  cd ../
fi
