import sys, os

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

    angles.appeng(ang)

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




















