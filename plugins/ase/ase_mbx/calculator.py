import json
import os
import warnings
import numpy as np
from ase.calculators.calculator import Calculator, all_changes

from .mbx_binding import MBXLibrary, KCAL_PER_MOL_TO_EV


class MBXCalculator(Calculator):
    """ASE calculator wrapper around MBX.

    When ``use_pbc_from_atoms`` is ``True``, the calculator follows ``atoms.pbc``
    and requires either full 3D periodicity or a fully non-periodic system.
    When it is ``False``, MBX is always evaluated as non-periodic even if the
    ASE ``Atoms`` object has PBC flags set.
    """

    implemented_properties = ["energy", "forces", "stress"]

    @staticmethod
    def _stress_tensor_to_voigt(stress_tensor):
        return np.array(
            [
                stress_tensor[0, 0],
                stress_tensor[1, 1],
                stress_tensor[2, 2],
                stress_tensor[1, 2],
                stress_tensor[0, 2],
                stress_tensor[0, 1],
            ],
            dtype=float,
        )

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
        # Ignore atoms.pbc when requested and force MBX to run as non-periodic.
        self.use_pbc_from_atoms = use_pbc_from_atoms
        self._ewald_params = self._load_ewald_params(json_file)
        self._warned_periodic_zero_alpha = False
        self.mbx = MBXLibrary(mbx_home=mbx_home)
        self._initialized = False

    @staticmethod
    def _load_ewald_params(json_file):
        with open(json_file, encoding="utf-8") as handle:
            config = json.load(handle)
        mbx_config = config.get("MBX", {})
        return {
            "alpha_ewald_elec": float(mbx_config.get("alpha_ewald_elec", 0.0)),
            "alpha_ewald_disp": float(mbx_config.get("alpha_ewald_disp", 0.0)),
        }

    def _validate_periodicity_settings(self, periodic, cell):
        elec_alpha = self._ewald_params["alpha_ewald_elec"]
        disp_alpha = self._ewald_params["alpha_ewald_disp"]

        if elec_alpha < 0.0 or disp_alpha < 0.0:
            raise ValueError("alpha_ewald_elec and alpha_ewald_disp must be non-negative.")

        if periodic:
            if abs(np.linalg.det(cell)) <= 1e-12:
                raise ValueError("Periodic MBX calculations require a valid simulation cell.")
            if (elec_alpha == 0.0 or disp_alpha == 0.0) and not self._warned_periodic_zero_alpha:
                warnings.warn(
                    "System is periodic, but alpha_ewald_elec or alpha_ewald_disp is zero in "
                    f"{self.json_file}.",
                    RuntimeWarning,
                    stacklevel=2,
                )
                self._warned_periodic_zero_alpha = True
            return

        if elec_alpha > 0.0:
            raise ValueError(
                f"alpha_ewald_elec = {elec_alpha} requires periodic boundary conditions."
            )
        if disp_alpha > 0.0:
            raise ValueError(
                f"alpha_ewald_disp = {disp_alpha} requires periodic boundary conditions."
            )

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
        pbc_flags = np.array(self.atoms.get_pbc(), dtype=bool)
        if (
            self.use_pbc_from_atoms
            and np.any(pbc_flags)
            and not np.all(pbc_flags)
        ):
            raise ValueError(
                "MBXCalculator does not support partial PBC; use either full 3D PBC "
                "(True, True, True) or non-periodic boundaries."
            )
        pbc = bool(np.all(pbc_flags)) and self.use_pbc_from_atoms
        cell = self.atoms.get_cell().array
        self._validate_periodicity_settings(pbc, cell)

        if pbc:
            energy_mbx, grads = self.mbx.get_energy_forces_pbc(coords, cell)
            virial = self.mbx.get_virial()
        else:
            energy_mbx, grads = self.mbx.get_energy_forces(coords)
            virial = None

        energy = energy_mbx * KCAL_PER_MOL_TO_EV
        forces = -grads * KCAL_PER_MOL_TO_EV

        self.results["energy"] = energy
        self.results["forces"] = forces

        stress_voigt = np.zeros(6, dtype=float)
        if virial is not None:
            # Convert virial (kcal/mol) to stress (eV/Å^3) using ASE convention: stress = -virial / V
            try:
                volume = self.atoms.get_volume()
            except ValueError:
                volume = 0.0
            if volume > 0.0:
                stress_tensor = -virial * KCAL_PER_MOL_TO_EV / volume
                stress_voigt = self._stress_tensor_to_voigt(stress_tensor)
        self.results["stress"] = stress_voigt

    def __del__(self):
        try:
            self.mbx.finalize()
        except Exception:
            pass
