#!/bin/bash
driver_direc=$MBX_HOME/plugins/i-pi/bin


export OMP_NUM_THREADS=8

i-pi npt_equil.xml &
sleep 10s
${driver_direc}/driver 118_co2.nrg mbx.json

