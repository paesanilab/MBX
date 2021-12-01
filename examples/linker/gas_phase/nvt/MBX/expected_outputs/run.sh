#!/bin/bash

module load gcc
cd $HOME/codes/i-pi/
source env.sh
cd -

i-pi config.xml &
echo $! > ipi.id

sleep 10

export OMP_NUM_THREADS=4

../../../../../plugins/i-pi/bin/driver config.nrg mbx.json

id=`cat ipi.id`
kill $id
