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
./configure --enable-shared --prefix="$MBX_HOME"
make
make install
```

## Install the ASE plugin
From the MBX repo root:
```
cd plugins/ase
pip install -e .
```

## Run an example
From the MBX repo root:
```
export MBX_HOME="$PWD"
cd plugins/ase/examples/water
python ../ase_mbx_energy.py
```

You should see non-zero energy and forces printed to the console.

More examples (bulk water and ion + water) are under `plugins/ase/examples/water` and `plugins/ase/examples/ion_water`.

## Notes
- MBX uses kcal/mol and Angstrom; energies and forces are converted to ASE units (eV, eV/A).
- MBX needs the O/H/H ordering per water. If the input file isn't ordered as O H H for each monomer (e.g., all O's first, then H's), the grouping will be wrong and MBX results will be incorrect. In that case you must reorder the atoms or explicitly provide nat_monomers/monomer_names/atom_names that match the correct per-monomer ordering.
