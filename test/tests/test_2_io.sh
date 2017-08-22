#!/bin/bash

cat << EOF > input.nrg
SYSTEM water
MOLECULE
MONOMER h2o
 O         -0.0044590985       -0.0513425796        0.0000158138
 H          0.9861302114       -0.0745730984        0.0000054324
 H         -0.1597470923        0.8967180895       -0.0000164932
ENDMON
ENDMOL
ENDSYS
SYSTEM water
MOLECULE
MONOMER h2o
 O         -0.0044590985       -0.0513425796        0.0000158138
 H          0.9861302114       -0.0745730984        0.0000054324
 H         -0.1597470923        0.8967180895       -0.0000164932
ENDMON
ENDMOL
ENDSYS
EOF

$CU_HOME/test/bin-test/io-test input.nrg

filename=$(basename "$0")
filename="${filename%.*}"

if diff expected/${filename}.out output.nrg &> /dev/null ; then
  echo "TEST $filename PASSED" 
else
  echo "TEST $filename FAILED"
fi

rm -f CU.log input.nrg output.nrg 
