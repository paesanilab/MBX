#!/bin/bash

cd $IPI_HOME
source env.sh
cd -

i-pi config.xml &
echo $! > ipi.id

sleep 10

export OMP_NUM_THREADS=4

$MBX_HOME/plugins/i-pi/bin/driver config.nrg mbx.json

id=`cat ipi.id`
kill $id

