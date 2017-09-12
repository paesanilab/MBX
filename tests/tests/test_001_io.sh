#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

../../build/stage/usr/local/clusters_ultimate/bin/io-test inputs/${filename}.nrg

if diff expected/${filename}.out output.nrg &> /dev/null ; then
  echo "TEST $filename PASSED" 
  rm -f CU.log input.nrg output.nrg
  exit 0
else
  echo "TEST $filename FAILED"
  echo "OUTPUT..."
  cat output.nrg
  echo
  echo "EXPECTED"
  cat expected/${filename}.out
  rm -f CU.log input.nrg output.nrg
  exit 1
fi

