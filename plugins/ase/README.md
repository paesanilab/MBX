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

## Notes
- MBX uses kcal/mol and Angstrom; energies and forces are converted to ASE units (eV, eV/A).
- `MBXCalculator` follows `atoms.pbc` directly and supports either full 3D periodicity or a fully non-periodic system.
- Periodic runs should use nonzero `alpha_ewald_elec` and `alpha_ewald_disp` values in `mbx.json`. Non-periodic runs should leave both at `0.0`.
- The ASE `mbx_md.py` example scripts infer monomers directly from the input atom order using the supported MBX monomer definitions (`h2o`, alkali ions, halide ions, `co2`, `ch4`, `he`, `ar`, `h2`, `n2o5`, `so4a`, `co3a`, `no3a`, and `nma`). `dp1` and `dp2` are intentionally not supported here.
- Input atoms must be grouped in the expected MBX monomer order. If the ordering is unsupported or ambiguous, the ASE examples will raise an error instead of guessing.
