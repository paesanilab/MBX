#!/bin/bash

export MBX_HOME=$HOME/codes/MBX
export OMP_NUM_THREADS=1
export PYTHONPATH=$PYTHONPATH:${MBX_HOME}/scripts/remd_analysis/post-processing/processing_tools

module load gcc
module load intel
module unload mpi
module load intel/mpi

# Generate initial frames from xyz
python3 ${MBX_HOME}/scripts/remd_analysis/pre-processing/s1-generate_initial_frames.py 100 150 16 1 input.xyz

# Generate nrg file from xyz
python3 ${MBX_HOME}/scripts/format_conversion/xyz2nrg.py input.xyz

# Run REMD simulation
mpirun -np 16 ${MBX_HOME}/plugins/repimd/bin/repimd -N 1 -n 10000 -p 50 -t 50 -s 0.0002 -R 15.0 -U 100.0 -i initial -f final -o output -j trajectory -T 16 -I input.nrg -J mbx.json

# Unmix trajectories
python3 /home/mrierari/codes/MBX/scripts/remd_analysis/post-processing/unmix_trajectories.py trajectory 16

# Convert trajectories to XYZ
for i in `seq 1 1 16`; do 
  printf -v j "%02d" $i
  python3 ${MBX_HOME}/scripts/remd_analysis/post-processing/traj_t_centroid_xyz.py unmixed_trajectory.$j 1
done
