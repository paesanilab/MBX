import sys, os
from tools import *

if len(sys.argv) < 4:
  print("Usage: " + sys.argv[0] + " unmixed_xyz_trajectory_prefix nrep sorted_T_file [pops.json]")
  sys.exit()

tpref = sys.argv[1]
nrep = int(sys.argv[2])
tempfile = sys.argv[3]

if not os.path.isfile("reference1.xyz"):
  print("Please have a reference1.xyz file with the configurations of the main reference (example: cs(h2o) without tags)")
  sys.exit()

if not os.path.isfile("input1.nrg"):
  print("Please have a input1.nrg file with the main reference monomer structure (example: cs(h2o) without tags)")
  print("Only one configuration is needed.")
  sys.exit()

if not os.path.isfile("reference2.xyz"):
  print("Please have a reference2.xyz file with the configurations of the secondary reference (example: cs(h2o) with tags)")
  sys.exit()

if not os.path.isfile("input2.nrg"):
  print("Please have a input2.nrg file with the secondary reference monomer structure (example: cs(h2o) without tags)")
  print("Only one configuration is needed.")
  sys.exit()

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
pops_ref2 = []
for i in range(nrep):
  print("Working on replica " + str(i))
  index = i+1
  traj = tpref + "." + str(index).zfill(2) + ".xyz"
  out_pref = "replica_" + str(index).zfill(2)
  os.system(MBX_HOME + "/scripts/remd_analysis/rmsd_tools/bin/double_pops input2.nrg " + traj + " reference2.xyz input1.nrg reference1.xyz " + out_pref + " " + jsonf)
  with open(out_pref + "_pops_ref.dat",'r') as ff:
    pops.append([float(p) for p in ff.readline().strip().split()])
  with open(out_pref + "_pops_ref2.dat",'r') as ff:
    pops_ref2.append([float(p) for p in ff.readline().strip().split()])

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
pops_ref2_ordered = []
for i in range(nrep):
  pops_ordered.append(pops[temp_index[i] -1])
  pops_ref2_ordered.append(pops_ref2[temp_index[i] -1])

# Write a pops.dat file with the temperatures and populations
with open("pops.dat",'w') as ff:
  for i in range(nrep):
    ff.write("{:10.4f}".format(temp_legend[i]))
    for p in pops_ordered[i]:
      ff.write("{:10.4f}".format(p))
    ff.write("\n")

with open("pops_ref2.dat",'w') as ff:
  for i in range(nrep):
    ff.write("{:10.4f}".format(temp_legend[i]))
    for p in pops_ref2_ordered[i]:
      ff.write("{:10.4f}".format(p))
    ff.write("\n")
