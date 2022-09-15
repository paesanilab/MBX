import sys
import copy

# parse command line

argv = sys.argv
if len(argv) != 2:
  print("Syntax: trivial.py in.lammps")
  sys.exit()

infile = sys.argv[1]

from lammps import lammps
lmp = lammps('mpi_mbx')

# run infile all at once
lmp.file(infile)

# Gather number of atoms
natoms = lmp.extract_global("natoms")

# Gather coordinates ([N][3])
x = lmp.extract_atom("x")

# Gather coordinates via gather function, so we can change them
xc = lmp.gather_atoms("x",1,3)

lmp.command('run 0')

# Analytical forces ([N],[3])
frc2 = lmp.gather_atoms('f',1,3)
frc = []
for i in range(natoms):
  frc.append([])
  for j in range(3):
    frc[-1].append(frc2[3*i+j])

energy = lmp.get_thermo('pe')

# Finite differences
step = 1E-05

with open("num_diff.dat",'w') as ff:
  ff.write("Reference potential energy: {} kcal/mol\n".format(energy))
  ff.write("NUMERICAL DIFFERENCES: INDEX ANALYTICAL NUMERICAL DIFFERENCE\n")
  
  for i in range(natoms):
    for j in range(3):
      x0 = xc[3*i+j]
  
      xc[3*i+j] = x0 + step
      lmp.scatter_atoms("x",1,3,xc)
      lmp.command('run 0')
      ep = lmp.get_thermo('pe')
      
      xc[3*i+j] = x0 - step
      lmp.scatter_atoms("x",1,3,xc)
      lmp.command('run 0')
      em = lmp.get_thermo('pe')
      
      df = -( ep - em ) / (2.0 * step)
      
      xc[3*i+j] = x0
    
      ff.write("{0:8d}{1:20.10e}{2:20.10e}{3:20.10e}\n".format(3*i + j, frc[i][j], df, df - frc[i][j]))
      ff.flush()
