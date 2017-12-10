#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

mkdir -p outputs

../../build/stage/usr/local/clusters_ultimate/bin/io-test inputs/${filename}.nrg outputs/${filename}.out 

