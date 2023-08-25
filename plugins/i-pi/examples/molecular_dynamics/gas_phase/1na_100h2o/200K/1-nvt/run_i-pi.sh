#!/bin/bash

cd $IPI_HOME
source env.sh
cd -

i-pi droplet.xml &
echo $! > ipi.id

sleep 10

export OMP_NUM_THREADS=4

$MBX_HOME/bin/ipi_driver droplet.nrg mbx.json

id=`cat ipi.id`
kill $id

