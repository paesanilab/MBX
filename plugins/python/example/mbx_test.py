import mbx

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

number_of_atoms_per_monomer = [3,3,3,1]
atom_names = ["O","H","H","O","H","H","O","H","H","F"]
monomer_names = ["h2o","h2o","h2o","f-"]
json_file = "mbx.json"

mbx.initialize_system(xyz,number_of_atoms_per_monomer,atom_names,monomer_names,json_file)

energy = mbx.get_energy_nograd(xyz,len(atom_names))

print("Energy(nograd):", energy)

energy_g,grad = mbx.get_energy_grad(xyz,len(atom_names))

print("Energy(grad):",energy_g)
print("Grads:",grad)

box = [10.0,0.0,0.0,0.0,10.0,0.0,0.0,0.0,10.0]

energy = mbx.get_energy_pbc_nograd(xyz,len(atom_names),box)

print("Energy(nograd) PBC:", energy)

energy_g,grad = mbx.get_energy_pbc_grad(xyz,len(atom_names),box)

print("Energy(grad) PBC:",energy_g)
print("Grads:",grad)

mbx.finalize_system()
