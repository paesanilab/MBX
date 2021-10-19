import sys, os
from tools import *

if len(sys.argv) != 5:
  print("Usage: " + sys.argv[0] + " trajectory_prefix nrep run_prefix number_of_frames_eq")
  sys.exit()

tpref = sys.argv[1]
nrep = int(sys.argv[2])
run_prefix = sys.argv[3]
nignore_first_frames = int(sys.argv[4])

# Check if inputs exists
inputs_exist = os.path.isdir("inputs")
if not inputs_exist:
  print("This code is expecting a folder 'inputs' in the same directory as all the {} folders with the initial.X files".format(run_prefix))
  sys.exit()

# Check that there is at least one run
all_runs = [x for x in [x[0].split("/")[-1] for x in os.walk(".")] if x.startswith(run_prefix)]
if len(all_runs) < 1:
  print("It seems like the prefix is misspelled. Please run this code in the same directory as 'inputs' and your runs")
  sys.exit()

# Theck that output folder does not exist
out_run_dir = "all_" + run_prefix
all_runs_exist = os.path.isdir(out_run_dir)
if all_runs_exist:
  print("It seems that the folder {} already exists. Will exit now.".format(out_run_dir))
  sys.exit()

os.system("cp -r inputs " + out_run_dir)

# Open outputs file handlers
offs = []
for i in range(nrep):
  index = i+1
  traj = tpref + "." + str(index).zfill(2)
  offs.append(open(out_run_dir + "/" + traj,'w'))

total_frame_count = 0
# For each run folder, read trajectory and put full frames in output folder.
for j in range(len(all_runs)):
  iffs = []
  runfolder = run_prefix + str(j+1)
  print("Working on {}".format(runfolder))

  for i in range(nrep):
    index = i+1
    traj = tpref + "." + str(index).zfill(2)
    iffs.append(open(runfolder + "/" + traj,'r'))

  temps = [""]*nrep
  strings = [""]*nrep
  endcodes = [""]*nrep
  
  alldone = False
  frameno = 0

  while not alldone:
    frameno += 1
    total_frame_count += 1
    for i in range(nrep):
      endcodes[i], temps[i], strings[i] = read_trajectory_frame_to_string(iffs[i])
      
    if endcodes.count("done") == nrep:
      alldone = True
      break
    elif endcodes.count("done") > 0:
      print("Something is wrong. Some trajectories seem to be longer than other. Skipping frame {}".format(frameno))
      continue
    elif endcodes.count("error") > 0:
      print("Something is wrong. Some trajectories seem to have errors. Skipping frame {}".format(frameno))
      continue
    
    if total_frame_count > nignore_first_frames:
      for i in range(nrep): 
        offs[i].write(strings[i])


  for i in range(nrep):
    index = i+1
    iffs[i].close()

for i in offs:
  i.close()
