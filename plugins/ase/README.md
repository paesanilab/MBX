# ASE–MBX Plugin

This plugin provides an ASE calculator that calls the MBX library to evaluate
MB-nrg energies and forces for use in ASE molecular dynamics.

## Requirements
- MBX built with shared libraries.
- `MBX_HOME` must be set before running any ASE+MBX scripts.
- Python deps: `ase`, `numpy`.

## Build MBX (from repo root)
From the MBX repo root:
```
export MBX_HOME="$PWD"
autoreconf -fi
./configure --enable-shared
make
make install
```

## Install the ASE plugin
From the MBX repo root:
```
cd plugins/ase
pip install -e .
```

More examples (bulk water and ion + water) are under `plugins/ase/examples/water` and `plugins/ase/examples/ion_water`. Use `plugins/ase/convert_lammps_to_extxyz.py` to convert LAMMPS data files to a minimal `extxyz` for ASE.

## MBXCalculator usage
The ASE `MBXCalculator` requires an `mbx.json` file. The single-water reference values below are for a non-periodic calculation, so `alpha_ewald_elec` and `alpha_ewald_disp` in `mbx.json` must both be `0.0`. The bundled `plugins/ase/examples/mbx.json` is a periodic template. The MD example scripts (`mbx_md.py`) do not require `mbx.json` because they generate it automatically from the run settings.

```python
from ase import Atoms
from ase_mbx import MBXCalculator

atoms = Atoms("H2O", positions=[[0, 0, 0], [0, 0, 1], [1, 0, 0]])

calc = MBXCalculator(
    json_file="mbx.json",
    nat_monomers=[3],
    monomer_names=["h2o"],
    atom_names=["O", "H", "H"],
)

atoms.calc = calc
energy = atoms.get_potential_energy()
forces = atoms.get_forces()
```

Expected output for the non-periodic setup above with `alpha_ewald_elec = 0.0` and `alpha_ewald_disp = 0.0`:
```text
Energy (eV): 0.196946434111
Forces (eV/A):
2.516989500971 -0.000000000000 2.516989500971
-1.058504812470 -0.000000000000 -1.458484688502
-1.458484688502 -0.000000000000 -1.058504812470
```

## Additional observables
The ASE wrapper now exposes the MBX observables that were previously only
available through the legacy `plugins/python/mbx/mbx.py` interface. These are
implemented as explicit calculator methods rather than raw positional tuples so
the meaning of each quantity stays clear at the call site.

```python
from ase import io
from ase_mbx import MBXCalculator

atoms = io.read("initial.extxyz")
calc = MBXCalculator(
    json_file="mbx.json",
    nat_monomers=[3, 3, 3],
    monomer_names=["h2o", "h2o", "h2o"],
)
atoms.calc = calc

components = calc.get_energy_component_breakdown(atoms)
elec = calc.get_electrostatic_energy_breakdown(atoms)
site_xyz = calc.get_electrostatic_site_coordinates(atoms)
site_charges = calc.get_electrostatic_site_charges(atoms)
site_pols = calc.get_electrostatic_site_polarizabilities(atoms)
site_mu_ind = calc.get_induced_site_dipoles(atoms)
probe = calc.get_potential_and_electric_field_at_points([[0.0, 0.0, 0.0]], atoms)
```

Available getters:
- `get_energy_component_breakdown(atoms=None)` returns an `EnergyComponentBreakdown`
  object with named energy terms in eV: `one_body`, `two_body`, `three_body`,
  `four_body`, `dispersion`, `buckingham`, and `electrostatics`. The helper
  property `sum_components` returns their sum.
- `get_electrostatic_energy_breakdown(atoms=None)` returns an
  `ElectrostaticEnergyBreakdown` object with `permanent` and `induced` terms in
  eV.
- `get_electrostatic_site_coordinates(atoms=None)` returns an `(n_sites, 3)`
  array of electrostatic-site coordinates in Angstrom. The ordering matches the
  MBX electrostatic site order and includes virtual sites such as the water
  M-site.
- `get_electrostatic_site_charges(atoms=None)` returns an `(n_sites,)` array of
  electrostatic-site charges in units of `e`.
- `get_electrostatic_site_polarizabilities(atoms=None)` returns an `(n_sites,)`
  array of electrostatic-site polarizabilities in units of `Angstrom^3`. These
  are site quantities in MBX input order, not whole-molecule polarizabilities.
- `get_induced_site_dipoles(atoms=None)` returns an `(n_sites, 3)` array of
  induced electrostatic-site dipoles in units of `e * Angstrom`.
- `get_potential_and_electric_field_at_points(points, atoms=None)` returns a
  `PotentialAndElectricField` object. `points` must be an `(n_points, 3)` array
  in Angstrom, `potential` has shape `(n_points,)`, and `electric_field` has
  shape `(n_points, 3)`. The potential is returned in units of `e / Angstrom`
  and the electric field in units of `e / Angstrom^2`. ASE does not define
  canonical units for either quantity, so the raw MBX values are preserved.
- `get_external_field_energy_contribution(atoms=None)` returns the external
  field electrostatic energy contribution in eV.

Helper methods:
- `get_electrostatic_site_counts(atoms=None)` returns the per-monomer
  electrostatic site counts used by the ASE wrapper. The values are resolved
  lazily from the same `mbx.json` file used to initialize MBX, so top-level
  monomer `sites` overrides are honored.
- `get_electrostatic_site_count(atoms=None)` returns the total electrostatic
  site count.

## Notes
- MBX uses kcal/mol and Angstrom; energies and forces are converted to ASE units (eV, eV/A).
- `MBXCalculator` follows `atoms.pbc` directly and supports either full 3D periodicity or a fully non-periodic system.
- Periodic runs should use nonzero `alpha_ewald_elec` and `alpha_ewald_disp` values in `mbx.json`. Non-periodic runs should leave both at `0.0`.
- The ASE `mbx_md.py` example scripts infer monomers directly from the input atom order using the supported MBX monomer definitions (`h2o`, alkali ions, halide ions, `co2`, `ch4`, `he`, `ar`, `h2`, `n2o5`, `so4a`, `co3a`, `no3a`, and `nma`). `dp1` and `dp2` are intentionally not supported here.
- Electrostatic-site observables resolve their site counts lazily after MBX initialization and follow any monomer-specific `sites` overrides found in `mbx.json`.
- Input atoms must be grouped in the expected MBX monomer order. If the ordering is unsupported or ambiguous, the ASE examples will raise an error instead of guessing.
