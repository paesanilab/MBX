import sys

# Create monomer, bonds, angles, dihedrals, and masses dictionaries
monomers = {}
bonds = {}
angles = {}
dihedrals = {}
masses = {}
atomtypes = {}
bondtypes = {}
angltypes = {}
dihetypes = {}

# NH4+
monomers['nh4+'] = ['N','H','H','H','H']
bonds['nh4+']     = [[0,1],[0,2],[0,3],[0,4]]
angles['nh4+']    = [[1,0,2],[1,0,3],[1,0,4],[2,0,3],[2,0,4],[3,0,4]]
dihedrals['nh4+'] = []
masses['nh4+']    = [14.0067,1.00797,1.00797,1.00797,1.00797]
atomtypes['nh4+'] = [0,1,1,1,1]
bondtypes['nh4+'] = [0,0,0,0]
angltypes['nh4+'] = [0,0,0,0,0,0]
dihetypes['nh4+'] = []

# NH3
monomers['nh3'] = ['N','H','H','H']
bonds['nh3']     = [[0,1],[0,2],[0,3]]
angles['nh3']    = [[1,0,2],[1,0,3],[2,0,3]]
dihedrals['nh3'] = []
masses['nh3']    = [14.0067,1.00797,1.00797,1.00797]
atomtypes['nh3'] = [0,1,1,1]
bondtypes['nh3'] = [0,0,0]
angltypes['nh3'] = [0,0,0]
dihetypes['nh3'] = []

# CH4
monomers['ch4'] = ['C','H','H','H','H']
bonds['ch4']     = [[0,1],[0,2],[0,3],[0,4]]
angles['ch4']    = [[1,0,2],[1,0,3],[1,0,4],[2,0,3],[2,0,4],[3,0,4]]
dihedrals['ch4'] = []
masses['ch4']    = [12.011,1.00797,1.00797,1.00797,1.00797]
atomtypes['ch4'] = [0,1,1,1,1]
bondtypes['ch4'] = [0,0,0,0]
angltypes['ch4'] = [0,0,0,0,0,0]
dihetypes['ch4'] = []

# CO2
monomers['co2'] = ['C','O','O']
bonds['co2']     = [[0,1],[0,2]]
angles['co2']    = [[1,0,2]]
dihedrals['co2'] = []
masses['co2']    = [12.011,15.9994,15.9994]
atomtypes['co2'] = [0,1,1]
bondtypes['co2'] = [0,0]
angltypes['co2'] = [0]
dihetypes['co2'] = []

# Monoatomic Ions
monomers['li+'] = ['Li']
bonds['li+']     = []
angles['li+']    = []
dihedrals['li+'] = []
masses['li+']    = [6.941]
atomtypes['li+'] = [0]
bondtypes['li+'] = []
angltypes['li+'] = []
dihetypes['li+'] = []

monomers['na+'] = ['Na']
bonds['na+']     = []
angles['na+']    = []
dihedrals['na+'] = []
masses['na+']    = [22.98977]
atomtypes['na+'] = [0]
bondtypes['na+'] = []
angltypes['na+'] = []
dihetypes['na+'] = []

monomers['k+'] = ['K']
bonds['k+']     = []
angles['k+']    = []
dihedrals['k+'] = []
masses['k+']    = [39.0983]
atomtypes['k+'] = [0]
bondtypes['k+'] = []
angltypes['k+'] = []
dihetypes['k+'] = []

monomers['rb+'] = ['Rb']
bonds['rb+']     = []
angles['rb+']    = []
dihedrals['rb+'] = []
masses['rb+']    = [85.4678]
atomtypes['rb+'] = [0]
bondtypes['rb+'] = []
angltypes['rb+'] = []
dihetypes['rb+'] = []

monomers['cs+'] = ['Cs']
bonds['cs+']     = []
angles['cs+']    = []
dihedrals['cs+'] = []
masses['cs+']    = [132.9054]
atomtypes['cs+'] = [0]
bondtypes['cs+'] = []
angltypes['cs+'] = []
dihetypes['cs+'] = []

monomers['f-'] = ['F']
bonds['f-']     = []
angles['f-']    = []
dihedrals['f-'] = []
masses['f-']    = [18.998403]
atomtypes['f-'] = [0]
bondtypes['f-'] = []
angltypes['f-'] = []
dihetypes['f-'] = []

monomers['cl-'] = ['Cl']
bonds['cl-']     = []
angles['cl-']    = []
dihedrals['cl-'] = []
masses['cl-']    = [35.453]
atomtypes['cl-'] = [0]
bondtypes['cl-'] = []
angltypes['cl-'] = []
dihetypes['cl-'] = []

monomers['br-'] = ['Br']
bonds['br-']     = []
angles['br-']    = []
dihedrals['br-'] = []
masses['br-']    = [79.904]
atomtypes['br-'] = [0]
bondtypes['br-'] = []
angltypes['br-'] = []
dihetypes['br-'] = []

monomers['i-'] = ['I']
bonds['i-']     = []
angles['i-']    = []
dihedrals['i-'] = []
masses['i-']    = [126.9045]
atomtypes['i-'] = [0]
bondtypes['i-'] = []
angltypes['i-'] = []
dihetypes['i-'] = []

# Noble Gases
monomers['ar'] = ['Ar']
bonds['ar']     = []
angles['ar']    = []
dihedrals['ar'] = []
masses['ar']    = [39.948]
atomtypes['ar'] = [0]
bondtypes['ar'] = []
angltypes['ar'] = []
dihetypes['ar'] = []

monomers['he'] = ['He']
bonds['he']     = []
angles['he']    = []
dihedrals['he'] = []
masses['he']    = [4.00260]
atomtypes['he'] = [0]
bondtypes['he'] = []
angltypes['he'] = []
dihetypes['he'] = []

# H2
monomers['h2'] = ['H','H']
bonds['h2']     = [[0,1]]
angles['h2']    = []
dihedrals['h2'] = []
masses['h2']    = [1.00797,1.00797]
atomtypes['h2'] = [0,0]
bondtypes['h2'] = [0]
angltypes['h2'] = []
dihetypes['h2'] = []

# N2O5
monomers['n2o5'] = ['O','N','N','O','O','O','O']
bonds['n2o5']     = [[0,1],[0,2],[1,3],[1,4],[2,6],[2,5]]
angles['n2o5']    = [[1,0,2],[5,2,6],[0,2,6],[0,2,5],[0,1,4],[0,1,3],[3,1,4]]
dihedrals['n2o5'] = [[1,0,2,5],[1,0,2,6],[2,0,1,3],[2,0,1,4]]
masses['n2o5']    = [15.9994,14.0067,14.0067,15.9994,15.9994,15.9994,15.9994]
atomtypes['n2o5'] = [0,1,1,2,2,2,2]
bondtypes['n2o5'] = [0,0,1,1,1,1]
angltypes['n2o5'] = [0,1,2,2,2,1,2]
dihetypes['n2o5'] = [0,0,0,0]

# SO4
monomers['so4a'] = ['S','O','O','O','O']
bonds['so4a']     = [[0,1],[0,2],[0,3],[0,4]]
angles['so4a']    = [[1,0,2],[1,0,3],[1,0,4],[2,0,3],[2,0,4],[3,0,4]]
dihedrals['so4a'] = []
masses['so4a']    = [32.066,15.9994,15.9994,15.9994,15.9994]
atomtypes['so4a'] = [0,1,1,1,1]
bondtypes['so4a'] = [0,0,0,0]
angltypes['so4a'] = [0,0,0,0,0,0]
dihetypes['so4a'] = []

# NO3
monomers['no3a'] = ['N','O','O','O']
bonds['no3a']     = [[0,1],[0,2],[0,3]]
angles['no3a']    = [[1,0,2],[1,0,3],[2,0,3]]
dihedrals['no3a'] = []
masses['no3a']    = [14.0067,15.9994,15.9994,15.9994]
atomtypes['no3a'] = [0,1,1,1]
bondtypes['no3a'] = [0,0,0]
angltypes['no3a'] = [0,0,0]
dihetypes['no3a'] = []

# CO3
monomers['co3a'] = ['C','O','O','O']
bonds['co3a']     = [[0,1],[0,2],[0,3]]
angles['co3a']    = [[1,0,2],[1,0,3],[2,0,3]]
dihedrals['co3a'] = []
masses['co3a']    = [12.011,15.9994,15.9994,15.9994]
atomtypes['co3a'] = [0,1,1,1]
bondtypes['co3a'] = [0,0,0]
angltypes['co3a'] = [0,0,0]
dihetypes['co3a'] = []

monomers['h2o'] = ['O','H','H']
bonds['h2o']     = [[0,1],[0,2]]
angles['h2o']    = [[1,0,2]]
dihedrals['h2o'] = []
masses['h2o']    = [15.9994,1.00797,1.00797]
atomtypes['h2o'] = [0,1,1]
bondtypes['h2o'] = [0,0]
angltypes['h2o'] = [0]
dihetypes['h2o'] = []

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

sysmon = []
# Find smallest number of atoms and largest number of atoms in dic
minat = 100000
maxat = 0
for i in monomers:
    if len(monomers[i]) > maxat:
        maxat = len(monomers[i])
    if len(monomers[i]) < minat:
        minat = len(monomers[i])

print(minat,maxat)
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

        sysmon.append(my_mon)
        my_nat = len(monomers[my_mon])
        first_index += my_nat
        break

    if first_index == nat:
        break

    if first_index > nat:
        print("ERROR......")
        break

# Write the data file
ff = open("lammps.data",'w')

# Write Comment
ff.write("Data file generated by the amazing script written by ChemPhys. Yes, you're welcome.\n\n")

# Get max atom types, angle types...
natstp = 0
nbndtp = 0
nangtp = 0
ndihtp = 0

nbnd = 0
nang = 0
ndih = 0

monomers_visited = []
masses_by_type = []
attp_count = 0
bntp_count = 0
agtp_count = 0
dhtp_count = 0
fi = {}
for i in range(len(sysmon)):
    attp = atomtypes[sysmon[i]]
    bntp = bondtypes[sysmon[i]]
    agtp = angltypes[sysmon[i]]
    dhtp = dihetypes[sysmon[i]]
    mass = masses[sysmon[i]]

    nbnd += len(bntp)
    nang += len(agtp)
    ndih += len(dhtp)

    if sysmon[i] not in monomers_visited:
        fi[sysmon[i]] = {}
        fi[sysmon[i]]['attp'] = natstp
        maxattp = max(attp) + 1
        if len(bntp) > 0:
            fi[sysmon[i]]['bntp'] = nbndtp
            maxbntp = max(bntp) + 1
            nbndtp += maxbntp
        if len(agtp) > 0:
            fi[sysmon[i]]['agtp'] = nangtp
            maxagtp = max(agtp) + 1
            nangtp += maxagtp
        if len(dhtp) > 0:
            fi[sysmon[i]]['dhtp'] = ndihtp
            maxdhtp = max(dhtp) + 1
            ndihtp += dhtp

        natstp += maxattp

        for j in range(len(attp)):
            if attp[j] == j:
                masses_by_type.append(mass[j])

        monomers_visited.append(sysmon[i])


# write it in data file
if nat < 1:
    print("Error: number of atoms cannot be less than 1. Is {}.".format(nat))

ff.write("{} atoms\n".format(nat))
ff.write("{} atom types\n".format(natstp))

if nbnd > 0:
    ff.write("{} bonds\n".format(nbnd))
    ff.write("{} bond types\n".format(nbndtp))

if nang > 0:
    ff.write("{} angles\n".format(nang))
    ff.write("{} angle types\n".format(nangtp))

if ndih > 0:
    ff.write("{} dihedrals\n".format(ndih))
    ff.write("{} dihedral types\n".format(ndihtp))

# Write box info
ff.write("\n")
box = """-50.0 50.0 xlo xhi
-50.0 50.0 ylo yhi
-50.0 50.0 zlo zhi"""
ff.write(box + "\n")

# Write masses
ff.write("\nMasses\n\n")

for i in range(len(masses_by_type)):
    ff.write("{} {}\n".format(i+1,masses_by_type[i]))


# Prepare text for atoms, bonds...
crd_txt = ""
bnd_txt = ""
ang_txt = ""
dih_txt = ""

atcount = 0
bndcount = 0
angcount = 0
dihcount = 0
for i in range(len(sysmon)):
    m = sysmon[i]
    attp = atomtypes[sysmon[i]]
    bntp = bondtypes[sysmon[i]]
    mbnd = bonds[sysmon[i]]
    agtp = angltypes[sysmon[i]]
    mang = angles[sysmon[i]]
    dhtp = dihetypes[sysmon[i]]
    mdih = dihedrals[sysmon[i]]

    fi_at_m = atcount
    for j in range(len(attp)):
        crd_txt += "{} {} {} {} {} {} {}\n".format(atcount + 1,i+1,1+attp[j] + fi[m]['attp'],0,xyz[atcount][0],xyz[atcount][1],xyz[atcount][2])
        atcount += 1

    for j in range(len(bntp)):
        bnd_txt += "{} {} {} {}\n".format(bndcount + 1,1+bntp[j] + fi[m]['bntp'],fi_at_m+1+mbnd[j][0],fi_at_m+1+mbnd[j][1])
        bndcount += 1

    for j in range(len(agtp)):
        ang_txt += "{} {} {} {} {}\n".format(angcount + 1,1+agtp[j] + fi[m]['agtp'],fi_at_m+1+mang[j][0],fi_at_m+1+mang[j][1],fi_at_m+1+mang[j][2])
        angcount += 1

    for j in range(len(dhtp)):
        dih_txt += "{} {} {} {} {}\n".format(dihcount + 1,1+dhtp[j] + fi[m]['dhtp'],fi_at_m+1+mdih[j][0],fi_at_m+1+mdih[j][2],fi_at_m+1+mdih[j][3],fi_at_m+1+mdih[j][4])
        dihcount += 1

# Write atoms
ff.write("\nAtoms\n\n")
ff.write(crd_txt)

# Write bonds
if bndcount > 0:
    ff.write("\nBonds\n\n")
    ff.write(bnd_txt)

if angcount > 0:
    ff.write("\nAngles\n\n")
    ff.write(ang_txt)

if dihcount > 0:
    ff.write("\nDihedrals\n\n")
    ff.write(dih_txt)























