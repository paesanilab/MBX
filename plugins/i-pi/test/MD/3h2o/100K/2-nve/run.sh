#!/bin/bash

i-pi 3h2o.xml &

sleep 10

export OMP_NUM_THREADS=2

../../../../../bin/driver 3h2o.nrg mbx.json
