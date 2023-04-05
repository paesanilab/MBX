#!/bin/bash

if [ -z "${MBX_HOME}" ]; then
  echo "MBX_HOME is not set"
  exit
fi 

export PYTHONPATH=$PYTHONPATH:${MBX_HOME}/plugins/python/mbx

EXE_GAS=${MBX_HOME}/examples/PEFs/src/python/example.py
EXE_PBC=${MBX_HOME}/examples/PEFs/src/python/example_pbc.py

python3 $EXE_GAS &> output
python3 $EXE_PBC &> output_pbc
