import os

from ase import Atoms

from ase_mbx import MBXCalculator

# Update these paths and system definitions for your MBX setup
if not os.getenv("MBX_HOME"):
    raise RuntimeError("MBX_HOME is not set. Example: export MBX_HOME=/path/to/MBX")

json_file = "mbx.json"
nat_monomers = [3]
monomer_names = ["h2o"]
atom_names = ["O", "H", "H"]

atoms = Atoms("H2O", positions=[[0.0, 0.0, 0.0], [0.0, 0.0, 1.0], [1.0, 0.0, 0.0]])
atoms.calc = MBXCalculator(
    json_file=json_file,
    nat_monomers=nat_monomers,
    monomer_names=monomer_names,
    atom_names=atom_names,
)

energy = atoms.get_potential_energy()
forces = atoms.get_forces()

print("Energy (eV):", energy)
print("Forces (eV/Å):", forces)
