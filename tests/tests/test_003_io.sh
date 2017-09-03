#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

$CU_HOME/test/bin-test/io-test inputs/${filename}.nrg

if diff expected/${filename}.out output.nrg &> /dev/null ; then
  echo "TEST $filename PASSED" 
else
  echo "TEST $filename FAILED"
fi

rm -f CU.log input.nrg output.nrg 
