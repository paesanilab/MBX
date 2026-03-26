from dataclasses import dataclass
import json
import os
import warnings
import numpy as np
from ase.calculators.calculator import Calculator, all_changes

from .mbx_binding import MBXLibrary, KCAL_PER_MOL_TO_EV
from .monomers import get_electrostatic_site_counts


@dataclass(frozen=True)
class EnergyComponentBreakdown:
    """Named MBX energy components in ASE energy units (eV).

    MBX exposes the physically distinct terms that add up to the model energy
    for the current geometry. Returning them as named fields is less error-prone
    than mirroring the legacy Python wrapper's positional tuple.
    """

    one_body: float
    two_body: float
    three_body: float
    four_body: float
    dispersion: float
    buckingham: float
    electrostatics: float

    @property
    def sum_components(self):
        """Return the sum of the explicitly reported MBX energy components."""

        return (
            self.one_body
            + self.two_body
            + self.three_body
            + self.four_body
            + self.dispersion
            + self.buckingham
            + self.electrostatics
        )


@dataclass(frozen=True)
class ElectrostaticEnergyBreakdown:
    """Permanent and induced electrostatic contributions in ASE energy units (eV)."""

    permanent: float
    induced: float

    @property
    def sum_components(self):
        """Return the total electrostatic energy contribution."""

        return self.permanent + self.induced


@dataclass(frozen=True)
class PotentialAndElectricField:
    """Electrostatic potential and field sampled at arbitrary probe points.

    ``potential`` has shape ``(n_points,)`` and ``electric_field`` has shape
    ``(n_points, 3)``. These values stay in MBX's native electrostatic units,
    because ASE does not define canonical units for either quantity.
    """

    potential: np.ndarray
    electric_field: np.ndarray


class MBXCalculator(Calculator):
    """ASE calculator wrapper around MBX.

    The calculator follows ``atoms.pbc`` directly and requires either full 3D
    periodicity or a fully non-periodic system.
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
        **kwargs,
    ):
        super().__init__(**kwargs)
        if not os.path.isfile(json_file):
            raise FileNotFoundError(f"MBX json_file not found: {json_file}")
        self.json_file = json_file
        self.nat_monomers = list(nat_monomers)
        self.monomer_names = list(monomer_names)
        self.atom_names = list(atom_names) if atom_names is not None else None
        self._ewald_params = self._load_ewald_params(json_file)
        self._warned_periodic_zero_alpha = False
        self.mbx = MBXLibrary(mbx_home=mbx_home)
        self._initialized = False
        self._site_counts = get_electrostatic_site_counts(self.monomer_names)

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

    def _get_configuration_context(self, atoms=None):
        """Synchronize MBX with the requested geometry and return its state.

        The observable getters below all rely on MBX's current internal state.
        Calling ``get_property("energy", ...)`` lets ASE's cache decide whether
        a fresh MBX evaluation is required, which keeps repeated getter calls
        cheap while still guaranteeing the state is current after geometry
        changes.
        """

        current_atoms = atoms if atoms is not None else self.atoms
        if current_atoms is None:
            raise ValueError(
                "No Atoms object is attached to this calculator. Pass atoms=... or "
                "attach the calculator to an ASE Atoms object first."
            )

        self.get_property("energy", current_atoms)
        coords = self.atoms.get_positions()
        pbc_flags = np.array(self.atoms.get_pbc(), dtype=bool)
        pbc = bool(np.all(pbc_flags))
        cell = self.atoms.get_cell().array
        return coords, pbc, cell

    def get_electrostatic_site_counts(self):
        """Return the per-monomer electrostatic site counts used by MBX."""

        return list(self._site_counts)

    def get_electrostatic_site_count(self):
        """Return the total number of electrostatic sites in the MBX system."""

        return int(sum(self._site_counts))

    def get_energy_component_breakdown(self, atoms=None):
        """Return MBX's named energy components for the current geometry.

        The values are converted to ASE energy units (eV) so they can be
        compared directly to ``atoms.get_potential_energy()``. The returned
        object intentionally uses explicit field names because the legacy
        wrapper's positional tuple is easy to misread.
        """

        coords, pbc, cell = self._get_configuration_context(atoms)
        if pbc:
            components = self.mbx.get_energy_component_breakdown_pbc(coords, cell)
        else:
            components = self.mbx.get_energy_component_breakdown(coords)
        components_ev = [value * KCAL_PER_MOL_TO_EV for value in components]
        return EnergyComponentBreakdown(*components_ev)

    def get_electrostatic_energy_breakdown(self, atoms=None):
        """Return the permanent and induced electrostatic terms in eV."""

        coords, pbc, cell = self._get_configuration_context(atoms)
        if pbc:
            permanent, induced = self.mbx.get_electrostatic_energy_breakdown_pbc(coords, cell)
        else:
            permanent, induced = self.mbx.get_electrostatic_energy_breakdown(coords)
        return ElectrostaticEnergyBreakdown(
            permanent=permanent * KCAL_PER_MOL_TO_EV,
            induced=induced * KCAL_PER_MOL_TO_EV,
        )

    def get_electrostatic_site_coordinates(self, atoms=None):
        """Return all MBX electrostatic-site coordinates as an ``(n_sites, 3)`` array.

        The site ordering follows MBX's input ordering and includes virtual
        electrostatic sites such as the water M-site.
        """

        self._get_configuration_context(atoms)
        return self.mbx.get_electrostatic_site_coordinates(self.get_electrostatic_site_count())

    def get_electrostatic_site_charges(self, atoms=None):
        """Return the electrostatic-site charges in MBX input order."""

        self._get_configuration_context(atoms)
        return self.mbx.get_electrostatic_site_charges(self.get_electrostatic_site_count())

    def get_electrostatic_site_polarizabilities(self, atoms=None):
        """Return the electrostatic-site polarizabilities in MBX input order."""

        self._get_configuration_context(atoms)
        return self.mbx.get_electrostatic_site_polarizabilities(self.get_electrostatic_site_count())

    def get_induced_site_dipoles(self, atoms=None):
        """Return induced site dipoles as an ``(n_sites, 3)`` array.

        These are the per-site induced dipole vectors that MBX solves during its
        polarization step. They are ordered by electrostatic site, not by real
        atom, so systems with virtual sites will have more dipole vectors than
        real atoms.
        """

        self._get_configuration_context(atoms)
        return self.mbx.get_induced_site_dipoles(self.get_electrostatic_site_count())

    def get_potential_and_electric_field_at_points(self, points, atoms=None):
        """Sample the total electrostatic potential and field at probe points.

        ``points`` must be convertible to an ``(n_points, 3)`` array of
        Cartesian coordinates in Angstrom. The return value keeps the potential
        and field in MBX's native electrostatic units because ASE does not
        define standard units for these observables.
        """

        self._get_configuration_context(atoms)
        potential, electric_field = self.mbx.get_potential_and_electric_field_at_points(points)
        return PotentialAndElectricField(potential=potential, electric_field=electric_field)

    def get_external_field_energy_contribution(self, atoms=None):
        """Return the external-field electrostatic energy contribution in eV."""

        self._get_configuration_context(atoms)
        energy = self.mbx.get_external_field_energy_contribution()
        return energy * KCAL_PER_MOL_TO_EV

    def calculate(self, atoms=None, properties=("energy", "forces"), system_changes=all_changes):
        super().calculate(atoms, properties, system_changes)
        self._ensure_initialized(self.atoms)

        coords = self.atoms.get_positions()
        pbc_flags = np.array(self.atoms.get_pbc(), dtype=bool)
        if np.any(pbc_flags) and not np.all(pbc_flags):
            raise ValueError(
                "MBXCalculator does not support partial PBC; use either full 3D PBC "
                "(True, True, True) or non-periodic boundaries."
            )
        pbc = bool(np.all(pbc_flags))
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
