#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

$CU_HOME/test/bin-test/dimers-test inputs/${filename}.nrg > output

if diff expected/${filename}.out output &> /dev/null ; then
  echo "TEST $filename PASSED" 
else
  echo "TEST $filename FAILED"
fi

rm -f CU.log output 
