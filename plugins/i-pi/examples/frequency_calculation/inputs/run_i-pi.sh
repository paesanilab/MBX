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

# COnvert to cm-1
python3 convert_vib_eigvals_au_to_cm-1.py simulation.phonons.eigval > frequencies.dat
