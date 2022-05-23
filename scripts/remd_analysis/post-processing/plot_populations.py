import os,sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
from matplotlib import rcParams
import matplotlib as mpl

if len(sys.argv) < 2:
  print("Usage: " + sys.argv[0] + " pops.dat [minpop=0.2]")
  sys.exit()

popfile = sys.argv[1]

if len(sys.argv) > 2:
  minpop = float(sys.argv[2])
else:
  minpop = 0.2

popdat = np.transpose(np.loadtxt(popfile))

nisomers = len(popdat) - 1

# Plot only isomers that have a pop gretaer than minpop for any temperature
plot_isomer = []
ncolors = 0
for i in range(1,len(popdat)):
  if max(popdat[i]) > minpop:
    plot_isomer.append(True)
    ncolors += 1
  else:
    plot_isomer.append(False)

# Prepare plot features
# This is for a color gradient
norm = mpl.colors.Normalize(vmin=0, vmax=ncolors)
# Using Blues (https://matplotlib.org/3.1.0/tutorials/colors/colormaps.html)
cmap = mpl.cm.ScalarMappable(norm=norm, cmap=mpl.cm.hsv)
# Fill the colors list
colors = []
for i in range(ncolors):
    colors.append(cmap.to_rgba(i))

# Markers
markers = ['o','s','v','^','<','>','P','x','X','D','d','p','*','+','1','2','3','4']

rcParams['font.family'] = 'Helvetica'

# Now the plot itself
fig, axs = plt.subplots(1, 1, figsize=(6,6))

# Prepare legends
legends = []
color_count = 0
for i in range(nisomers):
  if plot_isomer[i]:
    #axs.plot(popdat[0],popdat[i+1],marker=markers[i],color=colors[i],linestyle='-',linewidth=1.5)
    axs.plot(popdat[0],popdat[i+1],marker="",color=colors[color_count],linestyle='-',linewidth=1.5)
    legends.append("Isomer {}".format(i+1))
    color_count += 1

axs.set_xlim(min(popdat[0]) - 5, max(popdat[0]) + 5)
axs.set_ylim([-0.05,1.05])
axs.xaxis.set_minor_locator(MultipleLocator(5))
axs.yaxis.set_minor_locator(MultipleLocator(0.05))
axs.xaxis.set_major_locator(MultipleLocator(20))
axs.yaxis.set_major_locator(MultipleLocator(0.1))
axs.tick_params(top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=6)
axs.tick_params(which='minor', top='on', bottom='on', left='on', right='on', direction='in', labelsize=15, length=4)

axs.set_xlabel('Temperature (K)', fontsize=16)
axs.set_ylabel('Fraction', fontsize=16)
axs.legend(legends)

#Tight layout
plt.tight_layout()

#Save images
name = popfile.replace(".dat","")

figName = name +'.png'
plt.savefig(figName, format='png', dpi=1000)
figName = name +'.pdf'
plt.savefig(figName, format='pdf', dpi=1000)

