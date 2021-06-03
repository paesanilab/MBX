#!/bin/bash

export MBX_HOME=$HOME/codes/MBX
export OMP_NUM_THREADS=1
export PYTHONPATH=$PYTHONPATH:${MBX_HOME}/scripts/remd_analysis/post-processing/processing_tools

module load gcc
module load intel
module unload mpi
module load intel/mpi

NREPLICAS=16
NBEADS=1

# Generate initial frames from xyz
python3 ${MBX_HOME}/scripts/remd_analysis/pre-processing/s1-generate_initial_frames.py 100 150 $NREPLICAS $NBEADS input.xyz

# Generate nrg file from xyz
python3 ${MBX_HOME}/scripts/format_conversion/xyz2nrg.py input.xyz

# Run REMD simulation
mpirun -np $NREPLICAS ${MBX_HOME}/plugins/repimd/bin/repimd -N 1 -n 10000 -p 50 -t 50 -s 0.0002 -R 10.0 -U 100.0 -i initial -f final -o output -j trajectory -T $NREPLICAS -I input.nrg -J mbx.json

# Unmix trajectories
python3 /home/mrierari/codes/MBX/scripts/remd_analysis/post-processing/unmix_trajectories.py trajectory $NREPLICAS

# Convert trajectories to XYZ
for i in `seq 1 1 $NREPLICAS`; do 
  printf -v j "%02d" $i
  python3 ${MBX_HOME}/scripts/remd_analysis/post-processing/traj_t_centroid_xyz.py unmixed_trajectory.$j $NBEADS
done

# Optimize each trajectory 
for i in `seq 1 1 $NREPLICAS`; do
  # Optimize trajectory every 20 frames
  rm optimized.xyz unconverged.xyz
  printf -v j "%02d" $i
  ${MBX_HOME}/install/bin/main/optimize  input.nrg mbx.json 500 0.000001 unmixed_trajectory.${j}.xyz 20
  mv optimized.xyz optimized_unmixed_trajectory.${j}.xyz

  # Try converging the ones that did not
  if [ -f "unconverged.xyz" ]; then
    ${MBX_HOME}/install/bin/main/optimize  input.nrg mbx.json 500 0.000001 unconverged.xyz 1
    cat optimized.xyz >> optimized_unmixed_trajectory.${j}.xyz
  fi
  
done


