import sys, os
import math

# Some useful constants
pi = math.pi
deg2rad = pi/180.0
rad2deg = 1.0/deg2rad

# Create dictionaries
# Dictionary with number of parameters per functional form
nparams = {}
nparams["harm"] = 2
nparams["cos"] = 3

# Dictionary with MBX name for that functional form
mbxname = {}
mbxname["harm"] = "harm"
mbxname["cos"] = "cos"

# Dictionary with convertion factors from params in DLPOLY to MBX units.
#    Functional form is dlpoly name!
tombxunits = {}
tombxunits["bonds"] = {}
tombxunits["bonds"]["harm"] = [1.0,1.0]

tombxunits["angles"] = {}
tombxunits["angles"]["harm"] = [1.0,deg2rad]

tombxunits["dihedrals"] = {}
tombxunits["dihedrals"]["cos"] = [1.0,deg2rad,1.0]

# Dictionary with reordering indexes.
#    Functional form is dlpoly name!
tombxorder = {}
tombxorder["harm"] = [0,1]

tombxorder["harm"] = [0,1]

tombxorder["cos"] = [0,2,1]


# Ensure a FIELD file is present
if not os.path.isfile("FIELD"):
  print("Please have a FIELD file in this directory:")
  print(os.getcwd())
  sys.exit()

# Process the file
with open("FIELD",'r') as f:
  # First and seconf line do not have info
  f.readline()
  f.readline()
  
  # Read number of types 
  ntypes = int(f.readline().strip().split()[2])
  
  monids = []
  num_atoms = []
  atoms = []
  charges = []
  polfac = []
  pol = []

  bonds = []
  angles = []
  dihedrals = []
  inversions = []

  vdw = []

  # For each type, read the info
  for mon in range(ntypes):
    monids.append(f.readline().strip().split()[0])
    
    # We don't care about number of molecules of that type
    f.readline()
    
    # Read atoms, chg, pol, polfac
    nats = int(f.readline().strip().split()[1])
    ats = []
    chg = []
    p = []
    pf = []
    for at in range(nats):
      line = f.readline().strip().split();
      ats.append(line[0])
      chg.append(line[2])
      p.append(line[4])
      pf.append(line[3])

    # Update in main lists
    atoms.append(ats)
    charges.append(chg)
    pol.append(p)
    polfac.append(pf)
    num_atoms.append(nats)
    
    # Try reading connectivity info
    line = f.readline().strip()
    
    # Read bonds
    if "BONDS" in line:
      # There are bonds
      nbonds = int(line.split()[1])

      # Read bonds
      bond = []
      for b in range(nbonds):
        bond.append(f.readline().strip().split())

    bonds.append(bond)

    # Read angles

    line = f.readline().strip()

    if "ANGLES" in line:
      # There are bonds
      nang = int(line.split()[1])

      # Read angles
      ang = []
      for b in range(nang):
        ang.append(f.readline().strip().split())

    angles.append(ang)

    # Read dihedrals
    
    line = f.readline().strip()
    
    if "DIHEDRALS" in line:
      # There are bonds
      ndhs = int(line.split()[1])

      # Read dihedrals
      dh = []
      for b in range(ndhs):
        dh.append(f.readline().strip().split())

    dihedrals.append(dh)

    line = f.readline().strip()

    # Read inversions
    if "INVERSIONS" in line:
      # There are bonds
      ninv = int(line.split()[1])

      # Read dihedrals
      inv = []
      for b in range(ninv):
        inv.append(f.readline().strip().split())

    inversions.append(inv)

    # There must be a finish keyword after inversions:
    line = f.readline().strip()
    if line != "FINISH":
      print("Seems like field file is not well formated.")
      sys.exit()
    
  # Read the vdw interactions
  nvdw = int(f.readline().strip().split()[1])  
  
  for v in range(nvdw):
    vdw.append(f.readline().strip().split())

  # Make sure last word is close
  line = f.readline().strip()
  if line != "CLOSE":
    print("Seems like field file is not well finalized. Expecting CLOSE at the end.")
    sys.exit()

# FIELD is read, now time to create the json files.  

### CONNECTIVITY file

with open("connectivity.dat",'w') as ff:

  # loop over monomer ids
  for i in range(len(monids)):
    # Write monomer id
    ff.write(monids[i].lower() + "\n")

    # Write bonds for this monomer
    for bond in bonds[i]:
      funcform = bond[0].lower()
      idxs = [int(k) for k in bond[1:3]]
      
      np = nparams[funcform]
      mbxn = mbxname[funcform]
      d2mbx = tombxunits["bonds"][funcform]
      d2mbxorder = tombxorder[funcform]

      constants_converted = [str(k*l) for k,l in zip([float(m) for m in bond[3:3+np]],d2mbx)]
      constants_ordered = [constants_converted[d2mbxorder[k]] for k in range(np)]

      constants_string = " ".join(constants_ordered)

      ff.write("{} {} {} {} {}\n".format("bond",idxs[0],idxs[1],mbxn,constants_string))

    # Write angles for this monomer
    for ang in angles[i]:
      funcform = ang[0].lower()
      idxs = [int(k) for k in ang[1:4]]

      np = nparams[funcform]
      mbxn = mbxname[funcform]
      d2mbx = tombxunits["angles"][funcform]
      d2mbxorder = tombxorder[funcform]

      constants_converted = [str(k*l) for k,l in zip([float(m) for m in ang[4:4+np]],d2mbx)]
      constants_ordered = [constants_converted[d2mbxorder[k]] for k in range(np)]

      constants_string = " ".join(constants_ordered)

      ff.write("{} {} {} {} {} {}\n".format("angle",idxs[0],idxs[1],idxs[2],mbxn,constants_string))    

    # Write dihedrals for this monomer
    for dh in dihedrals[i]:
      funcform = dh[0].lower()
      idxs = [int(k) for k in dh[1:5]]

      np = nparams[funcform]
      mbxn = mbxname[funcform]
      d2mbx = tombxunits["dihedrals"][funcform]
      d2mbxorder = tombxorder[funcform]

      constants_converted = [str(k*l) for k,l in zip([float(m) for m in dh[5:5+np]],d2mbx)]
      constants_ordered = [constants_converted[d2mbxorder[k]] for k in range(np)]

      constants_string = " ".join(constants_ordered)

      ff.write("{} {} {} {} {} {} {}\n".format("dihedral",idxs[0],idxs[1],idxs[2],idxs[3],mbxn,constants_string))

    # Write inversion for this monomer
    for inv in inversions[i]:
      funcform = inv[0].lower()
      idxs = [int(k) for k in inv[1:5]]

      np = nparams[funcform]
      mbxn = mbxname[funcform]

      constants_string = " ".join(inv[5:5+np])

      ff.write("{} {} {} {} {} {} {}\n".format("inversion",idxs[0],idxs[1],idxs[2],idxs[3],mbxn,constants_string))

    ff.write("ENDMON\n")

### MONOMERS json file
with open("monomers.json",'w') as ff:

  ff.write("{\n")
  nspaces = 4
  # loop over monomer ids
  for i in range(len(monids)):  
    spaces = " "*4
    ff.write(spaces + '"' + monids[i].lower() + '" : {\n')
    spaces2 = spaces + spaces
    
    # Write number of sites
    ff.write("{}{} : {},\n".format(spaces2,'"sites"',num_atoms[i]))
    
    # Write numebr of atoms
    ff.write("{}{} : {},\n".format(spaces2,'"nat"',num_atoms[i]))

    # Write excluded pair list
    list_of_excluded = []
    # Excluded 12
    excl = "["
    for j in range(len(bonds[i])):
      c = bonds[i][j]
      idxs = sorted([int(k)-1 for k in [c[1],c[2]]])
      if idxs not in list_of_excluded:
        excl += "[{},{}],".format(idxs[0],idxs[1])
        list_of_excluded.append(idxs)

    if len(excl) != 1:
      excl = excl[:-1] 
    excl += "]"

    ff.write("{}{} : {},\n".format(spaces2,'"exc12"',excl))

    # Excluded 13
    excl = "["
    for j in range(len(angles[i])):
      c = angles[i][j]
      idxs = sorted([int(k)-1 for k in [c[1],c[3]]])
      if idxs not in list_of_excluded:
        list_of_excluded.append(idxs)
        excl += "[{},{}],".format(idxs[0],idxs[1])

    if len(excl) != 1:
      excl = excl[:-1] 
    excl += "]"

    ff.write("{}{} : {},\n".format(spaces2,'"exc13"',excl))

    # Excluded 14
    excl = "["
    for j in range(len(dihedrals[i])):
      c = dihedrals[i][j]
      idxs = sorted([int(k)-1 for k in [c[1],c[4]]])
      scale_factor = int(float(c[-1]) + 1E-20)
      if idxs not in list_of_excluded and scale_factor == 0:
        list_of_excluded.append(idxs)
        excl += "[{},{}],".format(idxs[0],idxs[1])

    if len(excl) != 1:
      excl = excl[:-1]
    excl += "]"

    ff.write("{}{} : {},\n".format(spaces2,'"exc14"',excl))

    # Write charges
    chg_string = ",".join(charges[i])
    chg_string = "[" + chg_string + "]"
    ff.write("{}{} : {},\n".format(spaces2,'"charges"',chg_string))

    # Write pol
    pol_string = ",".join(pol[i])
    pol_string = "[" + pol_string + "]"
    ff.write("{}{} : {},\n".format(spaces2,'"pol"',pol_string))

    # Write polfac
    polfac_string = ",".join(polfac[i])
    polfac_string = "[" + polfac_string + "]"
    ff.write("{}{} : {},\n".format(spaces2,'"polfac"',polfac_string))

    # Write c6lr
    # TODO prepare for buckingham in case is needed
    c6lr = ["0.0"]*len(charges[i])
    c6lr_string = ",".join(c6lr)
    c6lr_string = "[" + c6lr_string + "]"
    ff.write("{}{} : {}\n".format(spaces2,'"c6lr"',c6lr_string))

    # Close this monomer
    ff.write(spaces + "}\n")

  # Close the object
  ff.write("}\n")

### NONBONDED json file
# First get the data needed:

pairs_data = "[\n"
types1_data = "[\n"
types2_data = "[\n"
c6_data = "[\n"
d6_data = "[\n"
a_data = "[\n"
epsilon_data = "[\n"
sigma_data = "[\n"

spaces = " "*4
spacesx = " "*20
spaces2 = " "*24
spaces3 = " "*28

# loop over monomer ids
for i in range(len(monids)):
  for j in range(i,len(monids)):

    # Pairs entry 
    # Check if pairs need to be reordered
    reordered = False
    monid_pair = [monids[i],monids[j]]
    idx1 = i
    idx2 = j
    if sorted(monid_pair) != monid_pair :
      reordered = True
      monid_pair = sorted(monid_pair)
      idx1 = j
      idx2 = i

    # This has to be in alphabetical order 
    pairs_data += "{}[\"{}\",\"{}\"],\n".format(spaces2,monid_pair[0].lower(),monid_pair[1].lower()) 

    # Types 1 entry
    at_string = '"' + '","'.join(atoms[idx1]) + '"'
    types1_data += "{}[{}]\n,".format(spaces2,at_string)

    # Types 2 entry
    at_string = '"' + '","'.join(atoms[idx2]) + '"'
    types2_data += "{}[{}]\n,".format(spaces2,at_string)

    # C6, d6 and a entry TODO read btt6
    c6_string = "{}[\n".format(spaces2)
    d6_string = "{}[\n".format(spaces2)
    a_string = "{}[\n".format(spaces2)
    pairs_added = []
    for ii in range(0,len(atoms[idx1]),1):
      jjstart = 0
      if i == j:
        jjstart = ii
      for jj in range(jjstart,len(atoms[idx2]),1):
        at1 = atoms[idx1][ii]
        at2 = atoms[idx2][jj]
        pair_ats = sorted([at1,at2])
        if pair_ats not in pairs_added:
          c6_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,0.0)
          d6_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,0.0)
          a_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,0.0)
          pairs_added.append(pair_ats)
    c6_string = c6_string[:-2] + "\n{}],\n".format(spaces2)
    d6_string = d6_string[:-2] + "\n{}],\n".format(spaces2)
    a_string = a_string[:-2] + "\n{}],\n".format(spaces2)

    # epsilon and sigma entry
    eps_string = "{}[\n".format(spaces2)
    sig_string = "{}[\n".format(spaces2)
    pairs_added = []
    for ii in range(0,len(atoms[idx1]),1):
      jjstart = 0
      if i == j:
        jjstart = ii
      for jj in range(jjstart,len(atoms[idx2]),1):
        at1 = atoms[idx1][ii]
        at2 = atoms[idx2][jj]
        written = False
        pair_ats = sorted([at1,at2])
        for vdwi in vdw:
          pair_vdw = sorted([vdwi[0],vdwi[1]])
          if pair_vdw == pair_ats and vdwi[2] == "lj" and pair_ats not in pairs_added:
            eps_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,vdwi[3])
            sig_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,vdwi[4])
            pairs_added.append(pair_ats)
            written = True
            break
        if not written and pair_ats not in pairs_added:
          eps_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,"0.0")
          sig_string += "{}[[\"{}\",\"{}\"],{}],\n".format(spaces3,at1,at2,"0.0")
          pairs_added.append(pair_ats)


    eps_string = eps_string[:-2] + "\n{}],\n".format(spaces2)
    sig_string = sig_string[:-2] + "\n{}],\n".format(spaces2)

    # Add strings to the data
    c6_data += c6_string
    d6_data += d6_string
    a_data += a_string
    epsilon_data += eps_string
    sigma_data += sig_string

# Take out the last coma
pairs_data = pairs_data[:-2] + "\n"
types1_data = types1_data[:-2] + "\n"
types2_data = types2_data[:-2] + "\n"
c6_data = c6_data[:-2] + "\n"
d6_data = d6_data[:-2] + "\n"
a_data = a_data[:-2] + "\n"
epsilon_data = epsilon_data[:-2] + "\n"
sigma_data = sigma_data[:-2] + "\n"

with open("repdisp.json",'w') as ff:

  ff.write("{\n")
  ff.write("{}\"pairs\"   :     {}{}],\n".format(spaces,pairs_data,spacesx))
  ff.write("{}\"types1\"  :     {}{}],\n".format(spaces,types1_data,spacesx))
  ff.write("{}\"types2\"  :     {}{}],\n".format(spaces,types2_data,spacesx))
  ff.write("{}\"c6\"      :     {}{}],\n".format(spaces,c6_data,spacesx))
  ff.write("{}\"d6\"      :     {}{}],\n".format(spaces,d6_data,spacesx))
  ff.write("{}\"a\"       :     {}{}],\n".format(spaces,a_data,spacesx))
  ff.write("{}\"epsilon\" :     {}{}],\n".format(spaces,epsilon_data,spacesx))
  ff.write("{}\"sigma\"   :     {}{}]\n".format(spaces,sigma_data,spacesx))
  ff.write("}")










