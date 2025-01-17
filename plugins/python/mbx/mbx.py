import ctypes
from ctypes import cdll
import numpy as np
import os,sys

MBXLENGTH2AU = 1.8897259886
AULENGTH2MBX = 1.0 / MBXLENGTH2AU

MBXENERGY2AU = 1.0 / 627.50956
AUENERGY2MBX = 627.50956

MBXELECPOT2AU = AULENGTH2MBX
MBXELECFIELD2AU = AULENGTH2MBX * AULENGTH2MBX
AUELECFIELD2MBX = MBXLENGTH2AU * MBXLENGTH2AU
AUELECPOT2MBX = MBXLENGTH2AU

MBX_HOME = os.getenv('MBX_HOME')
if MBX_HOME == "" or MBX_HOME == None:
  print("MBX_HOME is not set.")
  sys.exit()

mbxlib = ctypes.CDLL(MBX_HOME + '/lib/libmbx.so',mode=os.RTLD_LAZY)

c_double_array = np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags='C_CONTIGUOUS')
int_pointer = ctypes.POINTER(ctypes.c_int)


def initialize_system(coordinates,numer_of_atoms_of_monomers,atom_names,monomer_names,json_file,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX

  # Convert inputs to units
  coordinates = [coordinates[i] * l_inp2mbx for i in range(len(coordinates))]

  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
  nats = (ctypes.c_int * len(numer_of_atoms_of_monomers)) (*numer_of_atoms_of_monomers)

  ats_b = [bytes(i,encoding='utf8') for i in atom_names]
  ats = (ctypes.c_char_p * len(ats_b)) (*ats_b)

  mons_b = [bytes(i,encoding='utf8') for i in monomer_names]
  mons = (ctypes.c_char_p * len(mons_b)) (*mons_b)

  nmon = ctypes.c_int(len(mons))

  jsn = (ctypes.c_char_p) (bytes(json_file,encoding='utf8'))

  mbxlib.initialize_system_py_(crd,nats,ats,mons,ctypes.byref(nmon),jsn)

def get_energy_nograd(coordinates,number_of_atoms,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  # Convert inputs to units
  coordinates = [coordinates[i] * l_inp2mbx for i in range(len(coordinates))]

  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
  nat = ctypes.c_int(number_of_atoms)

  energy = ctypes.c_double(0.0)

  mbxlib.get_energy_(crd,ctypes.byref(nat),ctypes.byref(energy))

  return energy.value * e_mbx2inp

def get_energy_grad(coordinates,number_of_atoms,units="mbx"):
   # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  # Convert inputs to units
  coordinates = [coordinates[i] * l_inp2mbx for i in range(len(coordinates))]

  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
  nat = ctypes.c_int(number_of_atoms)

  energy = ctypes.c_double(0.0)
  gradl = [0.0]*len(coordinates)
  grad = (ctypes.c_double * len(coordinates)) (*gradl)

  mbxlib.get_energy_g_(crd,ctypes.byref(nat),ctypes.byref(energy),grad)

  grad_out = [grad[i]*e_mbx2inp/l_mbx2inp for i in range(len(grad))]

  return energy.value * e_mbx2inp, grad_out

def get_energy_pbc_nograd(coordinates,number_of_atoms,cell_vectors,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  # Convert inputs to units
  coordinates = [coordinates[i] * l_inp2mbx for i in range(len(coordinates))]
  cell_vectors = [cell_vectors[i] * l_inp2mbx for i in range(len(cell_vectors))]

  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
  nat = ctypes.c_int(number_of_atoms)
  box = (ctypes.c_double * len(cell_vectors)) (*cell_vectors)

  energy = ctypes.c_double(0.0)

  mbxlib.get_energy_pbc_(crd,ctypes.byref(nat),box,ctypes.byref(energy))

  return energy.value * e_mbx2inp

def get_energy_pbc_grad(coordinates,number_of_atoms,cell_vectors,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  # Convert inputs to units
  coordinates = [coordinates[i] * l_inp2mbx for i in range(len(coordinates))]
  cell_vectors = [cell_vectors[i] * l_inp2mbx for i in range(len(cell_vectors))]

  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
  nat = ctypes.c_int(number_of_atoms)
  box = (ctypes.c_double * len(cell_vectors)) (*cell_vectors)

  energy = ctypes.c_double(0.0)
  gradl = [0.0]*len(coordinates)
  grad = (ctypes.c_double * len(coordinates)) (*gradl)

  mbxlib.get_energy_pbc_g_(crd,ctypes.byref(nat),box,ctypes.byref(energy),grad)

  grad_out = [grad[i]*e_mbx2inp/l_mbx2inp for i in range(len(grad))]

  return energy.value*e_mbx2inp, grad_out

def set_coordinates(coordinates,number_of_atoms,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  # Convert inputs to units
  coordinates = [coordinates[i] * l_inp2mbx for i in range(len(coordinates))]

  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
  nat = ctypes.c_int(number_of_atoms)

  mbxlib.set_real_xyz_(crd,ctypes.byref(nat))

def get_induced_dipoles(number_of_atoms,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  mux = [0.0]*(3*number_of_atoms)
  mu =  (ctypes.c_double * len(mux)) (*mux)
  #mu = np.zeros(3*number_of_atoms,dtype=np.float64)
  mbxlib.get_induced_dipoles_(mu)
  mu_out = [mu[i]*l_mbx2inp for i in range(len(mu))]
  return mu_out

def redistribute_gradients(grads_in, number_of_atoms,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  phi_mbx2inp = 1.0
  phi_inp2mbx = 1.0
  ef_mbx2inp = 1.0
  ef_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX
    phi_mbx2inp = MBXELECPOT2AU
    phi_inp2mbx = AUELECPOT2MBX
    ef_mbx2inp = MBXELECFIELD2AU
    ef_inp2mbx = AUELECFIELD2MBX

  # Convert inputs to units
  grads_in = [grads_in[i] *e_inp2mbx / l_inp2mbx for i in range(len(grads_in))]

  grads_out = np.array(grads_in,dtype=np.float64)
  nat = ctypes.c_int(number_of_atoms)

  mbxlib.redistribute_gradients_(grads_out,ctypes.byref(nat))

  return grads_out*e_mbx2inp/l_mbx2inp


# Need to specify the types
mbxlib.get_potential_and_electric_field_on_points_.argtypes = [c_double_array,c_double_array,c_double_array,int_pointer]

def get_potential_and_electric_field_on_points(coordinates, number_of_atoms, units="mbx"):
   # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  phi_mbx2inp = 1.0
  phi_inp2mbx = 1.0
  ef_mbx2inp = 1.0
  ef_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX
    phi_mbx2inp = MBXELECPOT2AU
    phi_inp2mbx = AUELECPOT2MBX
    ef_mbx2inp = MBXELECFIELD2AU
    ef_inp2mbx = AUELECFIELD2MBX

  crd = np.array(coordinates,dtype=np.float64) * l_inp2mbx
  nat = ctypes.c_int(number_of_atoms)
  
  phi = np.zeros(number_of_atoms,dtype=np.float64)
  ef = np.zeros(3*number_of_atoms,dtype=np.float64)
  
  mbxlib.get_potential_and_electric_field_on_points_(crd,phi,ef,ctypes.byref(nat))

  phi = phi*phi_mbx2inp
  ef = ef*ef_mbx2inp

  return phi,ef

#def get_potential_and_electric_field_on_points(coordinates, number_of_atoms):
#  crd = (ctypes.c_double * len(coordinates)) (*coordinates)
#  nat = ctypes.c_int(number_of_atoms)
#
#  phil = [0.0]*number_of_atoms
#  phi = (ctypes.c_double * number_of_atoms) (*phil)
#
#  efl = [0.0]*len(coordinates)
#  ef = (ctypes.c_double * len(coordinates)) (*efl)
#
#  mbxlib.get_potential_and_electric_field_on_points_(crd,phi,ef,ctypes.byref(nat))
#
#  phi_out = [phi[i] for i in range(len(phi))]
#  ef_out = [ef[i] for i in range(len(ef))]
#
#  return phi_out,ef_out

def set_potential_and_electric_field_on_sites(phi,ef,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  phi_mbx2inp = 1.0
  phi_inp2mbx = 1.0
  ef_mbx2inp = 1.0
  ef_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX
    phi_mbx2inp = MBXELECPOT2AU
    phi_inp2mbx = AUELECPOT2MBX
    ef_mbx2inp = MBXELECFIELD2AU
    ef_inp2mbx = AUELECFIELD2MBX

  # Convert inputs to units
  phi = [phi[i] * phi_inp2mbx for i in range(len(phi))]
  ef = [ef[i] * ef_inp2mbx for i in range(len(ef))]

  phiv = (ctypes.c_double * len(phi)) (*phi)
  efv = (ctypes.c_double * len(ef)) (*ef)
  nsites = ctypes.c_int(len(phi))
  
  mbxlib.set_potential_and_electric_field_on_sites_(phiv,efv,ctypes.byref(nsites))

def get_xyz(number_of_electrostatic_sites,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  phi_mbx2inp = 1.0
  phi_inp2mbx = 1.0
  ef_mbx2inp = 1.0
  ef_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX
    phi_mbx2inp = MBXELECPOT2AU
    phi_inp2mbx = AUELECPOT2MBX
    ef_mbx2inp = MBXELECFIELD2AU
    ef_inp2mbx = AUELECFIELD2MBX

  ns = ctypes.c_int(number_of_electrostatic_sites)
  xyzl = [0.0]*(number_of_electrostatic_sites*3)
  xyz = (ctypes.c_double * (3*number_of_electrostatic_sites)) (*xyzl)

  mbxlib.get_xyz_(xyz)

  xyz_out = [xyz[i]*l_mbx2inp for i in range(len(xyz))]
  return xyz_out

def set_box(box,units="mbx"):
  # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  phi_mbx2inp = 1.0
  phi_inp2mbx = 1.0
  ef_mbx2inp = 1.0
  ef_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX
    phi_mbx2inp = MBXELECPOT2AU
    phi_inp2mbx = AUELECPOT2MBX
    ef_mbx2inp = MBXELECFIELD2AU
    ef_inp2mbx = AUELECFIELD2MBX

  box = [box[i]*l_inp2mbx for i in range(len(box))]
  length = len(box)
  l = ctypes.c_int(length)
  boxv = (ctypes.c_double * len(box)) (*box)

  mbxlib.set_box_(ctypes.byref(l),boxv)

def get_charges(number_of_electrostatic_sites,units="mbx"):
  ns = ctypes.c_int(number_of_electrostatic_sites)
  chgl = [0.0]*(number_of_electrostatic_sites)
  chg = (ctypes.c_double * (number_of_electrostatic_sites)) (*chgl)

  mbxlib.get_charges_(chg)

  chg_out = [chg[i] for i in range(len(chg))]
  return chg_out

def get_polarizabilities(number_of_electrostatic_sites):
  ns = ctypes.c_int(number_of_electrostatic_sites)
  poll = [0.0]*(number_of_electrostatic_sites)
  pol = (ctypes.c_double * (number_of_electrostatic_sites)) (*poll)

  mbxlib.get_polarizabilities_(pol)

  pol_out = [pol[i] for i in range(len(pol))]
  return pol_out

def get_external_field_contribution_to_energy(units="mbx"):
   # Define conversion factors
  l_mbx2inp = 1.0
  l_inp2mbx = 1.0
  e_mbx2inp = 1.0
  e_inp2mbx = 1.0
  if units == "au":
    l_mbx2inp = MBXLENGTH2AU
    l_inp2mbx = AULENGTH2MBX
    e_mbx2inp = MBXENERGY2AU
    e_inp2mbx = AUENERGY2MBX

  # Convert inputs to units
  energy = ctypes.c_double(0.0)

  mbxlib.get_external_field_contribution_to_energy_(ctypes.byref(energy))

  return energy.value * e_mbx2inp

def finalize_system():
  mbxlib.finalize_system_()

