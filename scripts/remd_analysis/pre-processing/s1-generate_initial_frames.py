import sys,os

from tools import *

if len(sys.argv) != 6:
  print("Usage: " + sys.argv[0] + " Tmin Tmax nrep nbeads input.xyz")
  sys.exit()

MBX_HOME = os.getenv('MBX_HOME')
if MBX_HOME is None:
  print("Please set up the MBX_HOME environment variable")
  sys.exit()

tmin = float(sys.argv[1])
tmax = float(sys.argv[2])
nrep = int(sys.argv[3])
nbead = int(sys.argv[4])

xyzfile = sys.argv[5]

temps = get_remd_temperatures(tmin,tmax,nrep)

for i in range(len(temps)):
  index = i + 1
  suffix = str(index).zfill(2)

  os.system(MBX_HOME + "/plugins/repimd/bin/xyz2frame {:.2f} {} {} > initial.{}".format(temps[i],nbead,xyzfile,suffix))

