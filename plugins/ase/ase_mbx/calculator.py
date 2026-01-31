import os
import numpy as np
from ase.calculators.calculator import Calculator, all_changes

from .mbx_binding import MBXLibrary, KCAL_PER_MOL_TO_EV


class MBXCalculator(Calculator):
    implemented_properties = ["energy", "forces", "stress"]

    def __init__(
        self,
        json_file,
        nat_monomers,
        monomer_names,
        atom_names=None,
        mbx_home=None,
        use_pbc_from_atoms=True,
        **kwargs,
    ):
        super().__init__(**kwargs)
        if not os.path.isfile(json_file):
            raise FileNotFoundError(f"MBX json_file not found: {json_file}")
        self.json_file = json_file
        self.nat_monomers = list(nat_monomers)
        self.monomer_names = list(monomer_names)
        self.atom_names = list(atom_names) if atom_names is not None else None
        self.use_pbc_from_atoms = use_pbc_from_atoms
        self.mbx = MBXLibrary(mbx_home=mbx_home)
        self._initialized = False

    def _ensure_initialized(self, atoms):
        if self._initialized:
            return
        if self.atom_names is None:
            self.atom_names = atoms.get_chemical_symbols()
        if sum(self.nat_monomers) != len(self.atom_names):
            raise ValueError(
                "Sum of nat_monomers must match number of atom_names."
            )
        if not self.monomer_names or len(self.monomer_names) != len(self.nat_monomers):
            raise ValueError("monomer_names must match nat_monomers length.")
        coords = atoms.get_positions()
        self.mbx.initialize_system(
            coords,
            self.nat_monomers,
            self.atom_names,
            self.monomer_names,
            self.json_file,
        )
        self._initialized = True

    def calculate(self, atoms=None, properties=("energy", "forces"), system_changes=all_changes):
        super().calculate(atoms, properties, system_changes)
        self._ensure_initialized(self.atoms)

        coords = self.atoms.get_positions()
        pbc = bool(np.any(self.atoms.get_pbc())) and self.use_pbc_from_atoms

        if pbc:
            cell = self.atoms.get_cell().array
            energy_mbx, grads = self.mbx.get_energy_forces_pbc(coords, cell)
            virial = self.mbx.get_virial()
        else:
            energy_mbx, grads = self.mbx.get_energy_forces(coords)
            virial = None

        energy = energy_mbx * KCAL_PER_MOL_TO_EV
        forces = -grads * KCAL_PER_MOL_TO_EV

        self.results["energy"] = energy
        self.results["forces"] = forces

        if virial is not None:
            # Convert virial (kcal/mol) to stress (eV/Å^3) using ASE convention: stress = -virial / V
            volume = self.atoms.get_volume()
            if volume > 0.0:
                stress = -virial * KCAL_PER_MOL_TO_EV / volume
                self.results["stress"] = stress

    def __del__(self):
        try:
            self.mbx.finalize()
        except Exception:
            pass
