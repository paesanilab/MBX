#!/bin/bash

i-pi config.xml &
echo $! > ipi.id

sleep 10

export OMP_NUM_THREADS=1

../../../bin/driver config.nrg mbx.json

id=`cat ipi.id`
kill $id
