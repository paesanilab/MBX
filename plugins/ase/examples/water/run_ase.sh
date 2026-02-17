#!/usr/bin/env bash
set -euo pipefail

OMP_THREADS=10
export OMP_NUM_THREADS=${OMP_THREADS}

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
work_path="${script_dir}"
input_file="${work_path}/initial.extxyz"

MBX_HOME=/home/s7saha/software/MBX_ase/MBX

sim_time=100       # ps
t_step=0.5         # fs
temp=300           # K
interv=100         # steps
pbc=True           # True/False

ensemble="nvt"     # nve | nvt | npt
pressure_bar=1.0   # bar (only used for npt)
npt_coupling="iso" # iso | aniso (only used for npt)
p_damp_fs=500.0    # fs (only used for npt)
t_damp_fs=50.0     # fs = 100*dt

tchain=3
pchain=3
tloop=1
ploop=1

restart_file="${work_path}/restart.pkl"
restart_arg=""
if [[ "${USE_RESTART:-0}" == "1" && -f "${restart_file}" ]]; then
  echo "Found restart: ${restart_file}  -> using restart"
  restart_arg="-restart ${restart_file}"
fi

export MBX_HOME

conda run -n ase_mbx python mbx_md.py \
  -work_path "${work_path}" \
  -init_file "${input_file}" \
  -sim_time "${sim_time}" \
  -time_step "${t_step}" \
  -temp "${temp}" \
  -interval "${interv}" \
  -pbc "${pbc}" \
  -ensemble "${ensemble}" \
  -pressure "${pressure_bar}" \
  -npt_coupling "${npt_coupling}" \
  -t_damp "${t_damp_fs}" \
  -p_damp "${p_damp_fs}" \
  -tchain "${tchain}" \
  -pchain "${pchain}" \
  -tloop "${tloop}" \
  -ploop "${ploop}" \
  ${restart_arg}
