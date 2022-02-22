#!/bin/bash

MBX_HOME="$HOME/codes/MBX"
folders="2048_h2o_cubic_box 2048_h2o_droplet 256_h2o_cubic_box 342_ch4_228_h2o_rectangular 57_ch4_513_h2o_rectangular clathrate"
ncpulist="1 2 4 8"

for i in $folders; do 
  cd $i
  for j in $ncpulist; do 
    export OMP_NUM_THREADS=$j 
    $MBX_HOME/install/bin/tests/get_timings input.nrg mbx.json > ${j}np_times.dat
  done 
  cd ../ 
done 

cd scripts
python3 plot.py $ncpulist
