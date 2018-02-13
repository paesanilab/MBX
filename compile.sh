#!/bin/bash
if [ $# -ne 1 ]; then
  echo "Usage: $0 <intel/gnu>"
  exit
fi

if [ "$1" == "gnu" ]; then
  rm -rf build bin
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=" -O0 -Wall -ftree-vectorize -ftree-vectorizer-verbose=2" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -H. -Bbuild
  cd build
  make --no-print-directory CXX=g++ CC=gcc
  cd ../

else
  rm -rf build bin
  cmake  -DCMAKE_CXX_FLAGS="-Wall -qopt-report " -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc -H. -Bbuild
  cd build
  make CXX=icpc CC=icc 
  cd ../

fi
