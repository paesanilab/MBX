#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

../../build/stage/usr/local/clusters_ultimate/bin/energy_wgrad-test inputs/${filename}.nrg > output

if diff expected/${filename}.out output &> /dev/null ; then
  echo "TEST $filename PASSED" 
  rm -f CU.log input.nrg output*
  exit 0
else
  echo "TEST $filename FAILED"
  rm -f CU.log input.nrg output*
  exit 1
fi
