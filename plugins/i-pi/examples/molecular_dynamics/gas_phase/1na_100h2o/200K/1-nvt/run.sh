#!/bin/bash

i-pi droplet.xml &

sleep 10

export OMP_NUM_THREADS=2

../../../../../bin/driver droplet.nrg mbx.json
