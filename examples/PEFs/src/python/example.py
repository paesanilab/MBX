import mbx

# User must provide the number of atoms per monomer, atom names,
# monomer names, coordinates and json file.

# In this example, all the information is deduced from the XYZ file
# This function provides with the information that will be needed to initialize the system

def get_system_info(xyz_file):
  # Create monomer dictionary that informs of which atoms compose each known monomer
  monomers = {}
  monomers['nh4+'] = ['N','H','H','H','H']
  monomers['nh3'] = ['N','H','H','H']
  monomers['ch4'] = ['C','H','H','H','H']
  monomers['pf6-'] = ['P','F','F','F','F','F','F']
  monomers['co2'] = ['C','O','O']
  monomers['li'] = ['Li']
  monomers['na'] = ['Na']
  monomers['k'] = ['K']
  monomers['rb'] = ['Rb']
  monomers['cs'] = ['Cs']
  monomers['f'] = ['F']
  monomers['cl'] = ['Cl']
  monomers['br'] = ['Br']
  monomers['i'] = ['I']
  monomers['ar'] = ['Ar']
  monomers['he'] = ['He']
  monomers['h2'] = ['H','H']
  monomers['h2o'] = ['O','H','H']
  monomers['n2o5'] = ['O','N','N','O','O','O','O']

  # Data that will be returned
  mon_id = []
  num_at = []
  ats = []
  xyz = []

  # Open and read XYZ file
  fin = open(xyz_file,'r')
  nat = int(fin.readline().strip().split()[0])
  fin.readline()
  
  for i in range(nat):
    line = fin.readline().strip().split()
    ats.append(line[0])
    xyz += [float(k) for k in line[1:]]
  
  fin.close()

  # Find smallest number of atoms and largest number of atoms in dic
  minat = 100000
  maxat = 0
  for i in monomers:
    if len(monomers[i]) > maxat:
      maxat = len(monomers[i])
    if len(monomers[i]) < minat:
      minat = len(monomers[i])
  
  # Start Checking for our monomers
  first_index = 0
  while True:
    for i in range(minat,maxat+1)[::-1]:
      my_ats = ats[first_index:first_index + i]
      my_mon = ""
      for j in monomers:
        if monomers[j] == my_ats:
          my_mon = j
          break
  
      if my_mon == "":
        continue
  
      mon_id.append(my_mon)
      my_nat = len(monomers[my_mon])
      num_at.append(my_nat)

      first_index += my_nat
      
    if first_index == nat:
      break

    if first_index > nat:
      print("ERROR......")
      break

  return xyz, ats, mon_id, num_at

####################################################
#######    MAIN FUNCTION    ########################
####################################################

xyz_input_file = "input.xyz"
json_file = "mbx.json"

xyz, atom_names, monomer_names, number_of_atoms_per_monomer = get_system_info(xyz_input_file)

# Initialize system
mbx.initialize_system(xyz,number_of_atoms_per_monomer,atom_names,monomer_names,json_file)

# Calculate the energy
energy = mbx.get_energy_nograd(xyz,len(atom_names))

print("Energy(nograd):", energy)

# Calculate energy AND gradients
energy_g,grad = mbx.get_energy_grad(xyz,len(atom_names))

print("Energy(grad):",energy_g)
print("Grads:",grad)

# Delete the system pointer
mbx.finalize_system()
