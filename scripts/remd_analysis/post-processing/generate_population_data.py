import sys, os
from tools import *

if len(sys.argv) < 4:
  print("Usage: " + sys.argv[0] + " unmixed_xyz_trajectory_prefix nrep sorted_T_file [pops.json]")
  sys.exit()

tpref = sys.argv[1]
nrep = int(sys.argv[2])
tempfile = sys.argv[3]

MBX_HOME = os.getenv('MBX_HOME')
if MBX_HOME is None:
  print("Please set up the MBX_HOME environment variable")
  sys.exit()

if len(sys.argv) > 4:
  jsonf = sys.argv[4]
else:
  jsonf = ""

# Generate populations and store them
pops = []
for i in range(nrep):
  index = i+1
  traj = tpref + "." + str(index).zfill(2) + ".xyz"
  os.system(MBX_HOME + "/scripts/remd_analysis/rmsd_tools/bin/pops input.nrg " + traj + " " + jsonf)
  with open("pops_" + traj,'r') as ff:
    pops.append([float(p) for p in ff.readline().strip().split()])

# Read temperature index ledend
temp_legend = []
temp_index = []
with open(tempfile,'r') as ff:
  for i in range(nrep):
    line = ff.readline().strip().split()
    temp_legend.append(float(line[0]))
    temp_index.append(int(line[1]))

# Order the populations
pops_ordered = []
for i in range(nrep):
  pops_ordered.append(pops[temp_index[i] -1])

# Write a pops.dat file with the temperatures and populations
with open("pops.dat",'w') as ff:
  for i in range(nrep):
    ff.write("{:10.4f}".format(temp_legend[i]))
    for p in pops_ordered[i]:
      ff.write("{:10.4f}".format(p))
    ff.write("\n")

