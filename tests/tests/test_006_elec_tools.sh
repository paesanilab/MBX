#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

../../build/stage/usr/local/clusters_ultimate/bin/elec_tools-test inputs/${filename}.nrg > output

if diff expected/${filename}.out output &> /dev/null ; then
  echo "TEST $filename PASSED" 
  rm -f input.nrg output*
  exit 0
else
  echo "TEST $filename FAILED"
  echo "OUTPUT..."
  cat output
  echo
  echo "EXPECTED"
  cat expected/${filename}.out
  rm -f input.nrg output*
  exit 1
fi
