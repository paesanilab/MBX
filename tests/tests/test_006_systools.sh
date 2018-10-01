#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

mkdir -p outputs

../../install/bin/elec_tools-test inputs/${filename}.nrg > outputs/${filename}.out
