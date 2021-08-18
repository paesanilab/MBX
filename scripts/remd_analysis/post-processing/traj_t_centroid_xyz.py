import os,sys

from tools import *

if len(sys.argv) != 3:
  print("Usage: " + sys.argv[0] + " trajectory nbeads")
  sys.exit()

traj_in = sys.argv[1]
traj_out = traj_in + ".xyz"

nbeads = int(sys.argv[2])

with open(traj_in,'r') as ffi,open(traj_out,'w') as ffo:
  end_code = ""
  while end_code != "done":
    end_code,positions,mass,temp,time = read_trajectory_frame(ffi)
    if end_code == "error":
      print("Something went wrong in the frame to xyz conversion...")
      sys.exit()

    if end_code == "done":
      break

    new_pos = find_centroid_from_pimd(positions,nbeads)
    nat = int(len(new_pos)/3)
    mass_bead = mass[0:nat]
    at_names = [mass_to_atom(i) for i in mass_bead]

    write_xyz(ffo,nat,at_names,new_pos)
