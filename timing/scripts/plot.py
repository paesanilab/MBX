#!/usr/bin/env python
# coding: utf-8

# In[ ]:


#Libraries
import sys,os
import copy
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
from matplotlib import rcParams


# In[ ]:
if len(sys.argv) < 2:
  print("Usage: " + sys.argv[0] + "n1 n2 n3 ... where n is the numebr of cores")
  print("Data names are expected to be Nnp_times.dat, where N is the number of cores used")
  print("It is assumed that first 'n' is the reference (n=1). Otherwise the times might not make sense")
  sys.exit()

folders = ["2048_h2o_droplet","256_h2o_cubic_box","2048_h2o_cubic_box","57_ch4_513_h2o_rectangular","342_ch4_228_h2o_rectangular","clathrate"]

legend_contrib = ["One-body Polynomials","Two-body Polynomials","Three-body Polynomials","Electrostatics","Dispersion","Buckingham","Force field","Lennard-Jones"]

colors = ['greenyellow','lime','darkgreen','brown','orange','royalblue','deeppink','darkviolet']

#Input files
dataFile = "np_times.dat"

#Lists
nprocs = []
all_times = []
for i in range(1,len(sys.argv)):
  nprocs.append(int(sys.argv[i]))
  folder_times = []
  for f in folders:
    ff = "../" + f + "/" + str(nprocs[-1]) + "np_times.dat"
    if not os.path.exists(ff):
      print("File " + ff + " does not exist. Please check.")
      sys.exit()

    with open(ff,'r') as tf:
      # Read 9 lines. Have the times in this order:
      # TOTAL 1B 2B 3B ELECTROSTATICS DISPERSION BUCKINGHAM CLASSICAL1B LENNARDJONES 
      times = []
      for j in range(len(colors) + 1):
        times.append(float(tf.readline().strip().split()[1]))

    folder_times.append(times)
  all_times.append(folder_times)

# All times: first index: ncores, second index: folder, third index: energy contribution

# Two plots per folder
# Plot # 1: Absolute times, stacked bar plot 
# Plot # 2: Scaling of all the contributions

# Need to put contributions of same time in same list for each folder
reordered_times = []
for j in range(len(folders)):
  reordered_times.append([])
  for k in range(1,len(all_times[0][0])):   # each contribution
    reordered_times[-1].append([])
    for i in range(len(all_times)): # num_cores
      reordered_times[j][k-1].append(all_times[i][j][k])

# copy it 
scalings = copy.deepcopy(reordered_times)

for j in range(len(folders)):
  for k in range(1,len(all_times[0][0])):   # each contribution
    for i in range(len(all_times)): # num_cores
      if reordered_times[j][k-1][i] < 0.00001:
        scalings[j][k-1][i] = 1.0
      else:
        scalings[j][k-1][i] = reordered_times[j][k-1][0] / reordered_times[j][k-1][i]

# Make it cumulative
for i in range(len(reordered_times)):
  for j in range(1,len(reordered_times[i])):  
    for k in range(len(reordered_times[i][j])): 
      reordered_times[i][j][k] += reordered_times[i][j-1][k]

tot_scaling = []
for i in range(len(reordered_times)):
  tot_scaling.append([])
  for k in range(len(reordered_times[i][-1])): # num_cores
    if reordered_times[i][-1][k] < 0.00001:
      tot_scaling[-1].append(1.0)
    else:
      tot_scaling[-1].append(reordered_times[i][-1][0] / reordered_times[i][-1][k])
  

### Figure
xi = np.linspace(-100, 100 ,1000, endpoint = True)
yi = xi

for i in range(len(folders)):

  #Creationg of figure
  fig, axs = plt.subplots(1, 1, figsize=(6,6))
  rcParams['font.family'] = 'Helvetica'

  for j in range(len(reordered_times[i]) -1,-1,-1):
    axs.bar([k + 1 for k in range(len(nprocs))], reordered_times[i][j],0.40,tick_label = nprocs, color = colors[j])

  a = [mm for mm in reversed(legend_contrib)]
  axs.legend(a, fontsize=12, loc=0, frameon=False)

  axs.tick_params(top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=6)
  axs.tick_params(which='minor', top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=4)
  #
  axs.set_xlabel('Number of Cores', fontsize=16)
  axs.set_ylabel('Time (ms)', fontsize=16)


  #Tight layout
  plt.tight_layout()
  
  #Save images
  name = 'individual_times_' + folders[i]
  
  figName = name +'.png'
  plt.savefig(figName, format='png', dpi=1000)
  figName = name +'.pdf'
  plt.savefig(figName, format='pdf', dpi=1000)
  
  os.system("cp " + name +".png $HOME")
  

# Now going for the scaling plot

for i in range(len(folders)):

  #Creationg of figure
  fig, axs = plt.subplots(1, 1, figsize=(6,6))
  rcParams['font.family'] = 'Helvetica'

  for j in range(len(scalings[i])):
    axs.plot(nprocs, scalings[i][j],marker='o', color = colors[j],linestyle='-')
  
  axs.plot(nprocs,tot_scaling[i],marker='s',color='black',linestyle='-')
  
  a = copy.deepcopy(legend_contrib)
  a.append("Total")
  axs.legend(a, fontsize=12, loc=0, frameon=False)

  axs.tick_params(top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=6)
  axs.tick_params(which='minor', top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=4)
  axs.set_xlabel('Number of Cores', fontsize=16)
  axs.set_ylabel('Time (ms)', fontsize=16)

  axs.set_xlim([0.5,max(nprocs) + 0.5])
  axs.set_ylim([0.5,max(nprocs) + 0.5])

  axs.plot(xi,yi, '--',color ='grey')

  #Tight layout
  plt.tight_layout()

  #Save images
  name = 'scaling_' + folders[i]

  figName = name +'.png'
  plt.savefig(figName, format='png', dpi=1000)
  figName = name +'.pdf'
  plt.savefig(figName, format='pdf', dpi=1000)

  os.system("cp " + name +".png $HOME")



##Left
#axs.scatter(reference[0], calc[0], marker='s',color='#FF8000', facecolors='none')
#axs.scatter(reference[1], calc[1], marker='^',color='#EE220C', facecolors='none')
#axs.set_xlim([-5,50])
#axs.set_ylim([-5,50])
#axs.xaxis.set_minor_locator(MultipleLocator(5))
#axs.yaxis.set_minor_locator(MultipleLocator(5))
#axs.xaxis.set_major_locator(MultipleLocator(10))
#axs.yaxis.set_major_locator(MultipleLocator(10))
#axs.tick_params(top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=6)
#axs.tick_params(which='minor', top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=4)
#
#axs.set_xlabel('CCSD(T)/AV(DT)Z', fontsize=16)
#axs.set_ylabel('MB-nrg (kcal/mol)', fontsize=16)
#axs.legend(["MB-nrg CM5 " + percentage + ", BE < " + str(int(e_split)) + " kcal/mol", "MB-nrg CM5 " + percentage + ", BE > " + str(int(e_split)) + " kcal/mol"], fontsize=12, loc=2, frameon=False)
##axs.text(-2, 53, r'a) CH$_4$: TTM-nrg', fontsize=15)
#axs.plot(xi,yi, '--',color ='k')

##Tight layout
#plt.tight_layout()
#
##Save images
#name = 'individual_times'
#
#figName = name +'.png'
#plt.savefig(figName, format='png', dpi=1000)
#figName = name +'.pdf'
#plt.savefig(figName, format='pdf', dpi=1000)
#
#os.system("cp " + name +".png $HOME")


