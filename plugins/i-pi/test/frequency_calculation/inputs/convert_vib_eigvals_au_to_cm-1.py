#!/usr/bin/env python

import sys
import numpy as np
#from numpy import linalg as LA
#import periodictable as pt
#from copy import deepcopy as copy

np.set_printoptions(4, linewidth=10, suppress=True)

### speed of light in natural units (~ hbar/E_h):
#c = 137.035999
### (~ Angstroms / Bohr):
bohr2ang = 0.52917721067

#!# from QCElemental (NIST 2014):
## Avogadro's constant (~ 1/mol):
N_A = 6.022140857e23  # 'avogadro constant'
## (~ Joule / Hartree):
hartree2joule = 4.359744650e-18  # 'hartree-joule relationship'
## speed of light in Hz (?):
c =  299792458  # 'inverse meter-hertz relationship'
## (~ meter / Bohr):
bohr2meter = 0.52917721067e-10 # 'bohr radius'

#!# from psi4 (see: psi4/psi4/driver/qcdb/vib.py)
## unit conversion for Hessian eigenvalues in a.u.: E_h/((a_0)^2) --> cm^-1:
uconv_cm_1 = (np.sqrt(N_A * hartree2joule * 1.0e19) / (2 * np.pi * c * bohr2ang))  # psi4
#            1.0e19 is (Ang / a_0)^2 * 1.e3  ^^^



if len(sys.argv) != 2:
  sys.exit('usage: 1.eigenvalue file printed by i-pi (eigenvalues in atomic units')


au_evals = np.genfromtxt(sys.argv[1])
#print(au_evals)

frequency_cm_1 = np.lib.scimath.sqrt(au_evals) * uconv_cm_1  # psi4

for v in sorted(frequency_cm_1, key=lambda x: x.real):
  print("{0:15.6f}{1:15.6f}\n".format(v.real * 42.6263736598, v.imag * 42.6263736598))

