#!/bin/bash

export MBX_HOME=$HOME/codes/MBX
export MBFIT_HOME=$HOME/codes/potential_fitting
export OMP_NUM_THREADS=1
export PYTHONPATH=$PYTHONPATH:${MBX_HOME}/scripts/remd_analysis/processing_tools

module load gcc
module load intel
module unload mpi
module load intel/mpi

source ${MBFIT_HOME}/sourceme.sh

NREPLICAS=16
NBEADS=1

# Symmetry of the system after taking out the ingonred atoms
SYMMETRY="A1_B1C2_B1C2"

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
  if [ -s "unconverged.xyz" ]; then
    ${MBX_HOME}/install/bin/main/optimize  input.nrg mbx.json 500 0.000001 unconverged.xyz 1
    cat optimized.xyz >> optimized_unmixed_trajectory.${j}.xyz
  fi
  
done

# Get the permutations
# Note: The permutations should only be for the atoms to consider 
# (i.e. the atoms not in the ignore lis in the rsc.json file.
python3 ${MBX_HOME}/scripts/remd_analysis/post-processing/generate_permutations.py $SYMMETRY

# Get Unique isomers in a single file
rm unique_isomers.xyz
for i in `seq 1 1 $NREPLICAS`; do
  printf -v j "%02d" $i
  ${MBX_HOME}/scripts/remd_analysis/rmsd_tools/bin/rsc input.nrg optimized_unmixed_trajectory.${j}.xyz rsc.json
  cat configs_rsc.xyz >> unique_isomers.xyz
done

${MBX_HOME}/scripts/remd_analysis/rmsd_tools/bin/rsc input.nrg unique_isomers.xyz rsc.json
mv configs_rsc.xyz unique_isomers.xyz

# Order the isomers
${MBX_HOME}/install/bin/main/order_frames input.nrg unique_isomers.xyz mbx.json
mv ordered.xyz unique_isomers_ordered.xyz

# Get populations
python3 ${MBX_HOME}/scripts/remd_analysis/post-processing/generate_population_data.py unmixed_trajectory $NREPLICAS sorted_temperatures.dat pops.json

# Plot populations
python3 ${MBX_HOME}/scripts/remd_analysis/post-processing/plot_populations.py pops.dat
