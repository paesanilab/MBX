import sys, os
from tools import *

if len(sys.argv) != 3:
  print("Usage: " + sys.argv[0] + " trajectory_prefix nrep")
  sys.exit()

tpref = sys.argv[1]
nrep = int(sys.argv[2])


iffs = []
offs = {}
for i in range(nrep):
  index = i+1
  traj = tpref + "." + str(index).zfill(2)
  iffs.append(open(traj,'r'))

# Read first frame for all to find all T
ft = open("temperatures.dat",'w')
tdat = []
for i in range(nrep):
  index = i+1
  traj = tpref + "." + str(index).zfill(2)
  end_code, temp, frame_string = read_trajectory_frame_to_string(iffs[i])
  offs[temp] = open("unmixed_" + traj,'w')
  offs[temp].write(frame_string)
  ft.write("{0:8.4f} {1:6}\n".format(float(temp), index))
  
ft.close()

os.system("sort -n -k 1 temperatures.dat > sorted_temperatures.dat")

temps = [""]*nrep
strings = [""]*nrep
endcodes = [""]*nrep

alldone = False

while not alldone:
  for i in range(nrep):
    endcodes[i], temps[i], strings[i] = read_trajectory_frame_to_string(iffs[i])
    
  if endcodes.count("done") == nrep:
    alldone = True
    break
  elif endcodes.count("done") > 0:
    print("Something is wrong. Some trajectories seem to be longer than other.")
    sys.exit()
  elif endcodes.count("error") > 0:
    print("Something is wrong. Some trajectories seem to have errors.")
    sys.exit()
  
  for i in range(nrep): 
    offs[temps[i]].write(strings[i])


for i in range(nrep):
  index = i+1
  iffs[i].close()

for i in offs:
  offs[i].close()
