import mbx

xyz = [  2.983563686e-01 ,  5.824427983e-01 , -2.325086078e-01,
    -1.776582611e+00 ,  2.121118287e-01,  -2.285075919e+00,
    -1.607676270e+00 ,  3.967313465e-02,  -3.214692479e+00,
    -2.612427540e+00 ,  6.851661022e-01,  -2.274554186e+00,
     1.155776529e+00 ,  2.947707568e+00,   4.081835272e-01,
     1.941023561e+00 ,  2.684200283e+00,   8.998914589e-01,
     1.120605744e+00 ,  3.903764893e+00,   4.393509883e-01]



number_of_atoms_per_monomer = [1,3,3]
atom_names = ["Na","O","H","H","O","H","H"]
monomer_names = ["na","h2o","h2o"]
json_file = "d"

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

p = [ 4.0 ,4.0 ,4.0,2.000001 ,4.00000001, 4.0,4.0, -2.0 ,4.0000000001]
box = []
mbx.set_coordinates(xyz,7)
mbx.set_box(box)
phi, ef = mbx.get_potential_and_electric_field_on_points(p,3)

print("Potential",phi)

xyz = mbx.get_xyz(9)

print(xyz)

chg = mbx.get_charges(9)

print(chg)

mbx.finalize_system()
