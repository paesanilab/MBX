#!/bin/bash

cd $HOME/codes/i-pi/
source env.sh
cd -

i-pi config.xml &
echo $! > ipi.id

sleep 10

export OMP_NUM_THREADS=4

/home/mrierari/codes/MBX/plugins/i-pi/bin/driver config.nrg mbx.json

id=`cat ipi.id`
kill $id

# COnvert to cm-1
convert_vib_eigvals_au_to_cm-1.py simulation.phonons.eigval > frequencies.dat
