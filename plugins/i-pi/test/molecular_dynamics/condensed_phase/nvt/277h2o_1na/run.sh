#!/bin/bash

i-pi 277h2o_1na.xml &

sleep 10

export OMP_NUM_THREADS=8

../../../bin/driver 277h2o_1na.nrg mbx.json
