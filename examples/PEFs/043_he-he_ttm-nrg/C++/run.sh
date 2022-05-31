#!/bin/bash

if [ -z "${MBX_HOME}" ]; then
  echo "MBX_HOME is not set"
  exit
fi 

EXE_GAS=${MBX_HOME}/examples/PEFs/src/C++/example
EXE_PBC=${MBX_HOME}/examples/PEFs/src/C++/example_pbc

$EXE_GAS &> output
$EXE_PBC &> output_pbc
