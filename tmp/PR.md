# ASE MBX Observable Getter Update

## What Is New

This change extends the ASE MBX calculator to expose MBX observables beyond the
standard ASE `energy`, `forces`, and `stress` results.

Files covered by this update:

- `calculator.py`
- `mbx_binding.py`
- `monomers.py`
- `README.md`

New ASE-facing getters:

- `get_energy_component_breakdown(atoms=None)`
- `get_electrostatic_energy_breakdown(atoms=None)`
- `get_electrostatic_site_coordinates(atoms=None)`
- `get_electrostatic_site_charges(atoms=None)`
- `get_electrostatic_site_polarizabilities(atoms=None)`
- `get_induced_site_dipoles(atoms=None)`
- `get_potential_and_electric_field_at_points(points, atoms=None)`
- `get_external_field_energy_contribution(atoms=None)`
- `get_electrostatic_site_counts()`
- `get_electrostatic_site_count()`

The ASE wrapper now returns named dataclasses for the structured quantities
instead of positional tuples where appropriate, and the new API is documented
in the ASE README.

## Testing Summary

Validation covered both gas-phase and periodic 3H2O systems.

Test system:

- Gas-phase 3H2O geometry was copied from
  `plugins/i-pi/examples/molecular_dynamics/gas_phase/3h2o/100K/2-nve/config.nrg`
- Gas-phase MBX settings were copied from
  `plugins/i-pi/examples/molecular_dynamics/gas_phase/3h2o/100K/2-nve/mbx.json`
- Periodic validation used the same 3H2O geometry as above in a `20 x 20 x 20`
  Angstrom cubic cell with periodic MBX settings

## Executable Comparisons

### Gas Phase: `single_point`

The ASE calculator total energy matches the standalone executable.

| Quantity | ASE | `bin/single_point` | Result |
| --- | ---: | ---: | --- |
| Total energy (eV) | `0.280709898344` | `0.280709898343` | Match |

### Gas Phase: `mb_decomp -e -v -o 3`

The ASE calculator was used to recompute the total energy of every monomer,
dimer, and trimer subsystem. These values match the verbose `mb_decomp`
subsystem totals.

| Subsystem | ASE total (kcal/mol) | `mb_decomp -e` (kcal/mol) | Result |
| --- | ---: | ---: | --- |
| `(0,)` | `1.677856711000` | `1.677856711000` | Match |
| `(1,)` | `0.391396983600` | `0.391396983600` | Match |
| `(2,)` | `0.391396881800` | `0.391396881800` | Match |
| `(0,1)` | `0.024340120350` | `0.024340120350` | Match |
| `(0,2)` | `5.666150880000` | `5.666150880000` | Match |
| `(1,2)` | `2.973775568000` | `2.973775568000` | Match |
| `(0,1,2)` | `6.473322385245` | `6.473322385000` | Match within executable print precision |

### Gas Phase: `mb_decomp -v -o 3`

The ASE calculator was also used to reconstruct the full many-body
decomposition by inclusion-exclusion from the same subsystem energies. These
values match the verbose `mb_decomp` output for every subsystem.

| Subsystem | ASE many-body (kcal/mol) | `mb_decomp` (kcal/mol) | Result |
| --- | ---: | ---: | --- |
| `(0,)` | `1.677856711000` | `1.677856711000` | Match |
| `(1,)` | `0.391396983600` | `0.391396983600` | Match |
| `(2,)` | `0.391396881800` | `0.391396881800` | Match |
| `(0,1)` | `-2.044913574250` | `-2.044913574000` | Match within executable print precision |
| `(0,2)` | `3.596897287444` | `3.596897287000` | Match within executable print precision |
| `(1,2)` | `2.190981702957` | `2.190981703000` | Match within executable print precision |
| `(0,1,2)` | `0.269706393249` | `0.269706393200` | Match within executable print precision |

The sum of the `mb_decomp` many-body contributions also matches
`single_point`/ASE total energy within executable print precision.

### Periodic: `single_point`

The periodic ASE calculator total energy also matches the standalone
executable.

| Quantity | ASE | `bin/single_point` | Result |
| --- | ---: | ---: | --- |
| Total energy (eV) | `1.833700220331` | `1.833700220327` | Match |

### Periodic: `mb_decomp -e -v -o 3`

The periodic ASE calculator was used to recompute the total energy of every
monomer, dimer, and trimer subsystem in the periodic box. These values match
the verbose `mb_decomp` subsystem totals.

| Subsystem | ASE total (kcal/mol) | `mb_decomp -e` (kcal/mol) | Result |
| --- | ---: | ---: | --- |
| `(0,)` | `1.666581961503` | `1.666581962000` | Match within executable print precision |
| `(1,)` | `0.379006571594` | `0.379006571600` | Match |
| `(2,)` | `0.379052477406` | `0.379052477400` | Match |
| `(0,1)` | `11.682334948683` | `11.682334948000` | Match within executable print precision |
| `(0,2)` | `24.616822338809` | `24.616822339000` | Match within executable print precision |
| `(1,2)` | `8.141599213237` | `8.141599213000` | Match within executable print precision |
| `(0,1,2)` | `42.286120846107` | `42.286120846000` | Match within executable print precision |

### Periodic: `mb_decomp -v -o 3`

The periodic ASE calculator was also used to reconstruct the full many-body
decomposition by inclusion-exclusion from the same subsystem energies. These
values match the verbose `mb_decomp` output for every subsystem.

| Subsystem | ASE many-body (kcal/mol) | `mb_decomp` (kcal/mol) | Result |
| --- | ---: | ---: | --- |
| `(0,)` | `1.666581961503` | `1.666581962000` | Match within executable print precision |
| `(1,)` | `0.379006571594` | `0.379006571600` | Match |
| `(2,)` | `0.379052477406` | `0.379052477400` | Match |
| `(0,1)` | `9.636746415586` | `9.636746416000` | Match within executable print precision |
| `(0,2)` | `22.571187899899` | `22.571187900000` | Match within executable print precision |
| `(1,2)` | `7.383540164236` | `7.383540164000` | Match within executable print precision |
| `(0,1,2)` | `0.270005355882` | `0.270005355900` | Match within executable print precision |

## Additional Observable Cross-Checks

Cross-check results against the legacy Python wrapper:

- Gas phase:
  total energy, energy component breakdown, electrostatic breakdown, site
  coordinates, site charges, site polarizabilities, induced site dipoles, probe
  electric field, and external-field energy contribution all matched.
- Periodic:
  total energy, forces, stress, energy component breakdown, electrostatic
  breakdown, site coordinates, site charges, site polarizabilities, induced
  site dipoles, and external-field energy contribution all matched.

One residual note:

- Probe potential is state-history sensitive in MBX. The ASE and legacy wrapper
  electric fields match exactly, but the scalar potential can differ depending
  on the sequence of preceding MBX calls in the same process. Because of that,
  probe potential is currently kept as diagnostic output rather than a hard
  pass/fail assertion.
- The periodic probe-point path was tested separately and aborts on both the
  ASE side and the legacy wrapper side, so it is not treated as an ASE
  regression and is not part of the passing periodic validation suite.

## How Polarizabilities And Induced Dipoles Were Compared

These quantities were validated by comparing the new ASE getters to the
existing MBX Python wrapper for the exact same system geometry and MBX
settings.

Comparison method:

- ASE side:
  `get_electrostatic_site_polarizabilities(atoms)` and
  `get_induced_site_dipoles(atoms)`
- Legacy MBX Python side:
  `get_polarizabilities(nsites)` and `get_induced_dipoles(nsites)`

For the 3H2O test system, `nsites = 12` because each water contributes four
electrostatic sites, including the virtual M-site.

The legacy path was evaluated only after MBX had already computed the current
configuration, so the induced dipoles were populated before being read. The
returned arrays were then compared element-by-element with tight numerical
tolerances.

Shapes compared:

- polarizabilities: `(12,)`
- induced dipoles: `(12, 3)`

These comparisons passed in both gas phase and PBC.
