#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

mkdir -p outputs

../../install/bin/systools-test inputs/${filename}.nrg > outputs/${filename}.out
