#!/bin/bash

cd $IPI_HOME
source env.sh
cd -

i-pi 277h2o_1na.xml &
echo $! > ipi.id

sleep 10

export OMP_NUM_THREADS=8

$MBX_HOME/bin/ipi_driver 277h2o_1na.nrg mbx.json

id=`cat ipi.id`
kill $id
