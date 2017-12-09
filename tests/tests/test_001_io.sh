#!/bin/bash

filename=$(basename "$0")
filename="${filename%.*}"

../../build/stage/usr/local/clusters_ultimate/bin/io-test inputs/${filename}.nrg outputs/${filename}.out 

