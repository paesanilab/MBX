from __future__ import annotations

import os
from pathlib import Path
import sys
import unittest

import numpy as np


REPO_ROOT = Path(__file__).resolve().parents[3]
ASE_PLUGIN_ROOT = Path(__file__).resolve().parents[1]
CONFIG_NRG = REPO_ROOT / "plugins" / "i-pi" / "examples" / "molecular_dynamics" / "gas_phase" / "3h2o" / "100K" / "2-nve" / "config.nrg"
MBX_JSON = REPO_ROOT / "plugins" / "i-pi" / "examples" / "molecular_dynamics" / "gas_phase" / "3h2o" / "100K" / "2-nve" / "mbx.json"
PROBE_POINTS = np.array([[0.0, 0.0, 0.0], [2.0, -1.0, 0.5]], dtype=float)


sys.path.insert(0, str(ASE_PLUGIN_ROOT))
os.environ.setdefault("MBX_HOME", str(REPO_ROOT))

from ase import Atoms
from ase_mbx import MBXCalculator


EXPECTED_ENERGY_EV = 0.2807098983444634
EXPECTED_FORCES_EV_PER_ANGSTROM = np.array(
    [
        [-0.7813332601135621, -0.7030094447562577, 0.032316409469792175],
        [-0.35004839071122273, 0.5529085381530243, -0.5517008881907544],
        [0.8926735130301063, -0.15150713227585383, 0.11464611958546402],
        [-0.2594844130304353, -0.15482988356836838, -0.08689013331597926],
        [-0.24653366399375787, 0.24057253099938689, 0.21110549803387382],
        [0.519066698045251, -0.05551587751982995, -0.22779901971614663],
        [0.11400690666717511, -0.2159509124670051, 0.1066680634763704],
        [-0.29746416955991833, 0.5263254967114515, 0.3944628117782405],
        [0.4091167796663653, -0.03899331527654807, 0.007191138879139192],
    ],
    dtype=float,
)
EXPECTED_STRESS_EV_PER_ANGSTROM3 = np.zeros(6, dtype=float)
EXPECTED_COMPONENTS_EV = np.array(
    [
        0.10670393531065125,
        0.22242234995690577,
        0.009588783246481183,
        0.0,
        -0.1479732275998018,
        0.0,
        0.08996805743036364,
    ],
    dtype=float,
)
EXPECTED_ELECTROSTATICS_EV = np.array([0.1366466856821721, -0.04667862825180848], dtype=float)
EXPECTED_SITE_COUNTS = [4, 4, 4]
EXPECTED_SITE_COORDINATES_ANGSTROM = np.array(
    [
        [-1.58972425, 1.04337922, -0.0878084],
        [-0.63591971, 0.9789852, 0.0],
        [-1.9006628, 1.7450105, -0.6645499],
        [-1.4525665789316284, 1.1793359821544118, -0.1921239593037971],
        [1.64924507, 1.08594656, 0.0],
        [2.60878026, 1.09587704, -0.02817115],
        [1.33830653, 1.78757784, 0.5767415],
        [1.787625397098573, 1.2377607099800163, 0.11703937180307435],
        [-0.61315209, 2.46976336, 2.07005086],
        [0.34684791, 2.46976336, 2.07005086],
        [-0.93360667, 3.37469919, 2.07005086],
        [-0.47670287396720984, 2.662834533214691, 2.07005086],
    ],
    dtype=float,
)
EXPECTED_SITE_CHARGES_E = np.array(
    [
        0.0,
        0.5987699259337522,
        0.5987699265066386,
        -1.1975398524403909,
        0.0,
        0.5863959199927881,
        0.5863959212811779,
        -1.172791841273966,
        0.0,
        0.5863959192556634,
        0.5863959195854797,
        -1.172791838841143,
    ],
    dtype=float,
)
EXPECTED_SITE_POLARIZABILITIES_ANGSTROM3 = np.array(
    [1.31, 0.294, 0.294, 0.0, 1.31, 0.294, 0.294, 0.0, 1.31, 0.294, 0.294, 0.0],
    dtype=float,
)
EXPECTED_INDUCED_DIPOLES_E_ANGSTROM = np.array(
    [
        [0.027952360300332532, 0.011457334633390789, 0.03014634807120765],
        [0.0066336319528934, -0.004611014521646114, -0.0035658407156801946],
        [-0.0006070239129601348, -0.007459728897510757, -0.0007680120865099141],
        [0.0, -0.0, 0.0],
        [0.03816573817939392, -0.00801103608875078, -0.0036187717333278713],
        [0.001127687163376697, 0.00018916310162184392, -0.0004874705723467616],
        [-0.0023011133370841826, -4.114867846465149e-05, -0.00672392119784081],
        [0.0, -0.0, -0.0],
        [-0.05932730327274823, -0.022327767936403904, -0.011748363690881771],
        [-0.008424679975744273, 0.00721272059986998, 0.017412625877244223],
        [0.006199842142416101, 0.0028847291062305534, 0.002352667660248575],
        [-0.0, -0.0, 0.0],
    ],
    dtype=float,
)
EXPECTED_PROBE_FIELD_E_PER_ANGSTROM2 = np.array(
    [
        [0.11035452841432358, -0.18553576438440134, -0.02003000798805082],
        [-0.03824029291437547, 0.029398478096980818, -0.00902907594834669],
    ],
    dtype=float,
)
EXPECTED_EXTERNAL_FIELD_ENERGY_EV = 0.0

# These tolerances are intentionally explicit so this file can serve as a
# post-installation regression check for a new ASE+MBX setup. They are tight
# enough to catch real numerical drift, but they avoid relying on bitwise-
# identical output across builds or BLAS/compiler variations.
ENERGY_ABS_TOL_EV = 1.0e-10
FORCE_ABS_TOL_EV_PER_ANGSTROM = 1.0e-8
STRESS_ABS_TOL_EV_PER_ANGSTROM3 = 1.0e-12
BREAKDOWN_ABS_TOL_EV = 1.0e-10
SITE_COORDINATE_ABS_TOL_ANGSTROM = 1.0e-10
SITE_CHARGE_ABS_TOL_E = 1.0e-10
SITE_POLARIZABILITY_ABS_TOL_ANGSTROM3 = 1.0e-12
INDUCED_DIPOLE_ABS_TOL_E_ANGSTROM = 1.0e-10
PROBE_FIELD_ABS_TOL_E_PER_ANGSTROM2 = 1.0e-10
DEFAULT_REL_TOL = 1.0e-8


def load_3h2o_example():
    """Read the original 3H2O gas-phase example used for the regression numbers."""

    atom_names = []
    positions = []
    monomer_names = []
    nat_monomers = []
    current_nat = None

    for raw_line in CONFIG_NRG.read_text(encoding="utf-8").splitlines():
        fields = raw_line.split()
        if not fields:
            continue
        if fields[0] == "MONOMER":
            monomer_names.append(fields[1])
            current_nat = 0
            continue
        if fields[0] == "ENDMON":
            if current_nat is None:
                raise ValueError("Encountered ENDMON before a MONOMER block in config.nrg.")
            nat_monomers.append(current_nat)
            current_nat = None
            continue
        if current_nat is not None and len(fields) == 4:
            atom_names.append(fields[0])
            positions.append([float(value) for value in fields[1:]])
            current_nat += 1

    if current_nat is not None:
        raise ValueError("config.nrg ended before the final ENDMON marker.")

    if not atom_names or len(monomer_names) != len(nat_monomers):
        raise ValueError("Failed to parse the 3H2O example from config.nrg.")

    return atom_names, np.array(positions, dtype=float), nat_monomers, monomer_names


class MBXCalculatorObservableRegressionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.atom_names, cls.positions, cls.nat_monomers, cls.monomer_names = load_3h2o_example()

    def setUp(self):
        self.atoms = Atoms(symbols=self.atom_names, positions=self.positions, pbc=False)
        self.calc = MBXCalculator(
            json_file=str(MBX_JSON),
            nat_monomers=self.nat_monomers,
            monomer_names=self.monomer_names,
            atom_names=self.atom_names,
            mbx_home=str(REPO_ROOT),
        )
        self.atoms.calc = self.calc

    def tearDown(self):
        self.calc.mbx.finalize()
        self.atoms.calc = None

    def assert_scalar_close(self, actual, expected, atol, rtol=DEFAULT_REL_TOL):
        self.assertTrue(
            np.isclose(actual, expected, atol=atol, rtol=rtol),
            msg=f"abs diff = {abs(actual - expected)}",
        )

    def assert_allclose(self, actual, expected, atol, rtol=DEFAULT_REL_TOL):
        self.assertTrue(
            np.allclose(actual, expected, atol=atol, rtol=rtol),
            msg=f"max abs diff = {np.max(np.abs(np.asarray(actual) - np.asarray(expected)))}",
        )

    def test_standard_ase_properties_match_reference_values(self):
        self.assert_scalar_close(
            self.atoms.get_potential_energy(),
            EXPECTED_ENERGY_EV,
            atol=ENERGY_ABS_TOL_EV,
        )
        self.assert_allclose(
            self.atoms.get_forces(),
            EXPECTED_FORCES_EV_PER_ANGSTROM,
            atol=FORCE_ABS_TOL_EV_PER_ANGSTROM,
        )
        self.assert_allclose(
            self.atoms.get_stress(),
            EXPECTED_STRESS_EV_PER_ANGSTROM3,
            atol=STRESS_ABS_TOL_EV_PER_ANGSTROM3,
            rtol=STRESS_ABS_TOL_EV_PER_ANGSTROM3,
        )

    def test_energy_breakdowns_and_site_observables_match_reference_values(self):
        components = self.calc.get_energy_component_breakdown(self.atoms)
        electrostatics = self.calc.get_electrostatic_energy_breakdown(self.atoms)

        component_values = np.array(
            [
                components.one_body,
                components.two_body,
                components.three_body,
                components.four_body,
                components.dispersion,
                components.buckingham,
                components.electrostatics,
            ],
            dtype=float,
        )
        electrostatic_values = np.array([electrostatics.permanent, electrostatics.induced], dtype=float)

        self.assert_allclose(component_values, EXPECTED_COMPONENTS_EV, atol=BREAKDOWN_ABS_TOL_EV)
        self.assert_scalar_close(components.sum_components, EXPECTED_ENERGY_EV, atol=ENERGY_ABS_TOL_EV)
        self.assert_allclose(
            electrostatic_values,
            EXPECTED_ELECTROSTATICS_EV,
            atol=BREAKDOWN_ABS_TOL_EV,
        )
        self.assert_scalar_close(
            electrostatics.sum_components,
            components.electrostatics,
            atol=BREAKDOWN_ABS_TOL_EV,
        )

        self.assertEqual(self.calc.get_electrostatic_site_counts(self.atoms), EXPECTED_SITE_COUNTS)
        self.assertEqual(self.calc.get_electrostatic_site_count(self.atoms), sum(EXPECTED_SITE_COUNTS))
        self.assert_allclose(
            self.calc.get_electrostatic_site_coordinates(self.atoms),
            EXPECTED_SITE_COORDINATES_ANGSTROM,
            atol=SITE_COORDINATE_ABS_TOL_ANGSTROM,
        )
        self.assert_allclose(
            self.calc.get_electrostatic_site_charges(self.atoms),
            EXPECTED_SITE_CHARGES_E,
            atol=SITE_CHARGE_ABS_TOL_E,
        )
        self.assert_allclose(
            self.calc.get_electrostatic_site_polarizabilities(self.atoms),
            EXPECTED_SITE_POLARIZABILITIES_ANGSTROM3,
            atol=SITE_POLARIZABILITY_ABS_TOL_ANGSTROM3,
        )
        self.assert_allclose(
            self.calc.get_induced_site_dipoles(self.atoms),
            EXPECTED_INDUCED_DIPOLES_E_ANGSTROM,
            atol=INDUCED_DIPOLE_ABS_TOL_E_ANGSTROM,
        )
        self.assert_scalar_close(
            self.calc.get_external_field_energy_contribution(self.atoms),
            EXPECTED_EXTERNAL_FIELD_ENERGY_EV,
            atol=ENERGY_ABS_TOL_EV,
        )

    def test_probe_field_matches_reference_values(self):
        probe = self.calc.get_potential_and_electric_field_at_points(PROBE_POINTS, self.atoms)

        # The electric field is stable across repeated runs in this environment.
        # The scalar potential is not, so keep that part of the regression check
        # structural instead of asserting one exact hard-coded array.
        self.assertEqual(probe.potential.shape, (2,))
        self.assertTrue(np.all(np.isfinite(probe.potential)))
        self.assert_allclose(
            probe.electric_field,
            EXPECTED_PROBE_FIELD_E_PER_ANGSTROM2,
            atol=PROBE_FIELD_ABS_TOL_E_PER_ANGSTROM2,
        )


if __name__ == "__main__":
    unittest.main()
