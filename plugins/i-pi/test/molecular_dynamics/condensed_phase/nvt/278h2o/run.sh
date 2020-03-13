#!/bin/bash

i-pi 278h2o.xml &

sleep 10

export OMP_NUM_THREADS=4

../../../bin/driver 278h2o.nrg mbx.json
