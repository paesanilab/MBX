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

More examples (bulk water and ion + water) are under `plugins/ase/examples/water` and `plugins/ase/examples/ion_water`.
The helper script `plugins/ase/examples/convert_lammps_to_extxyz.py` converts LAMMPS data files to a minimal `extxyz` for ASE.

## MBXCalculator usage
The ASE `MBXCalculator` requires an `mbx.json` file (see `plugins/ase/examples/mbx.json` for a template). The MD example scripts (`mbx_md.py`) do not require `mbx.json` because they generate it automatically from the run settings.

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

Expected output (verified in conda env `ase_mbx`):
```text
Energy (eV): 0.236574415163
Forces (eV/A):
2.507545266757 -0.000000000000 2.507545266757
-1.060576441254 -0.000000000000 -1.446968825502
-1.446968825502 -0.000000000000 -1.060576441254
```

## Notes
- MBX uses kcal/mol and Angstrom; energies and forces are converted to ASE units (eV, eV/A).
- MBX needs the O/H/H ordering per water. If the input file isn't ordered as O H H for each monomer (e.g., all O's first, then H's), the grouping will be wrong and MBX results will be incorrect. In that case you must reorder the atoms or explicitly provide nat_monomers/monomer_names/atom_names that match the correct per-monomer ordering.
