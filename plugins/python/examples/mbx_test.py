import mbx
import ctypes

# Define input parametrs
# Coordinates
xyz = [-1.58972425,    1.04337922,   -0.08780840,
       -0.63591971,    0.97898520,    0.00000000,
       -1.90066280,    1.74501050,   -0.66454990,
        1.64924507,    1.08594656,    0.00000000,
        2.60878026,    1.09587704,   -0.02817115,
        1.33830653,    1.78757784,    0.57674150,
       -0.61315209,    2.46976336,    2.07005086,
        0.34684791,    2.46976336,    2.07005086,
       -0.93360667,    3.37469919,    2.07005086,
       -1.32483472,    5.35321954,    2.07944727]

# Number of atoms per monomer
number_of_atoms_per_monomer = [3,3,3,1]

# Number of sites per monomer (4 for h2o, same as number of atoms for everything else)
number_of_sites_per_monomer = [4,4,4,1]

# Atom names
atom_names = ["O","H","H","O","H","H","O","H","H","F"]

# Monomer id
monomer_names = ["h2o","h2o","h2o","f-"]

# json file (see readme for info)
json_file = "mbx.json"

# Initialize system
mbx.initialize_system(xyz,number_of_atoms_per_monomer,atom_names,monomer_names,json_file)

# Get energy (no grads, gas phase)
energy = mbx.get_energy_nograd(xyz,len(atom_names))

print("Energy(nograd):", energy)

# Get energy and gradients (grads, gas phase)
energy_g,grad = mbx.get_energy_grad(xyz,len(atom_names))

print("Energy(grad):",energy_g)
print("Grads:",grad)

# Define a 9-element box
box = [10.0,0.0,0.0,0.0,10.0,0.0,0.0,0.0,10.0]

# Get energy, no grads, pbc
energy = mbx.get_energy_pbc_nograd(xyz,len(atom_names),box)

print("Energy(nograd) PBC:", energy)

# Get energy and grads, PBC
energy_g,grad = mbx.get_energy_pbc_grad(xyz,len(atom_names),box)

print("Energy(grad) PBC:",energy_g)
print("Grads:",grad)

# Get the charges
nsites = sum(number_of_sites_per_monomer)
charges = mbx.get_charges(nsites)

# Get their positions
coordinates = mbx.get_xyz(nsites)

# Get polarizabilities
pols = mbx.get_polarizabilities(nsites)

xp = [0.0,0.0,0.0]
npp = 1
phi,ef = mbx.get_potential_and_electric_field_on_points(xp,npp)
print("POT:",phi,"EFQ",ef)

# Get dipoles
mu = mbx.get_induced_dipoles(nsites)


print("Charges:",charges)
print("Coordinates of charges:",coordinates)
print("Polarizabilities:",pols)
print("Dipoles:",mu)

virial = (ctypes.c_double * 9)()

mbx.mbxlib.get_virial_(virial)

print(f"Virial: {[item for item in virial]}")

mbx.finalize_system()
