import sys

# Create monomer dictionary
monomers = {}
#NH4+
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
monomers['n2o5'] = ['O','N','N','O','O','O','O']

#H2O
monomers['h2o'] = ['O','H','H']

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + "<input.xyz>")
    sys.exit()

name = sys.argv[1].split(".xyz")[0]

fin = open(sys.argv[1],'r')
nat = int(fin.readline().strip().split()[0])
fin.readline()

ats = []
xyz = []
for i in range(nat):
    line = fin.readline().strip().split()
    ats.append(line[0])
    xyz.append([float(k) for k in line[1:]])

fin.close()

fou = open(name + ".nrg",'w')
fou.write("SYSTEM NRG\n")

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
    for i in range(maxat, minat-1,-1):
        my_ats = ats[first_index:first_index + i]
        my_mon = ""
        for j in monomers:
            if monomers[j] == my_ats:
                my_mon = j

        if my_mon == "":
            continue

        fou.write("MOLECULE\nMONOMER " + my_mon + "\n")
        my_nat = len(monomers[my_mon])
        for j in range(my_nat):
            fou.write("{} {} {} {}\n".format(ats[first_index + j],xyz[first_index + j][0],xyz[first_index + j][1],xyz[first_index + j][2]))

        fou.write("ENDMON\nENDMOL\n")

        first_index += my_nat
        break

    if first_index == nat:
        fou.write("ENDSYS\n")
        break

    if first_index > nat:
        print("ERROR......")
        break

fou.close()
