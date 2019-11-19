#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

CMAKE_EXE=$HOME/codes/cmake-3.14.3/bin/cmake

if [ "$1" == "gnu" ]; then
  rm -rf build install
  $CMAKE_EXE -DCMAKE_BUILD_TYPE=Debug -DUSE_OPENMP=True -DCMAKE_CXX_FLAGS=" -fPIC -O2 -Wall -ftree-vectorize -ftree-vectorizer-verbose=2" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make CXX=g++ CC=gcc
  make install
  cd ../

else
  echo "Comet still doesn't have a version newr than 2016, so compilation with intel is not possible."
fi
