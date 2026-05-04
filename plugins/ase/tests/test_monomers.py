from __future__ import annotations

from pathlib import Path
import sys
import tempfile
import unittest


sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "ase_mbx"))

from monomers import resolve_electrostatic_site_counts


class ResolveElectrostaticSiteCountsTest(unittest.TestCase):
    def test_defaults_use_built_in_site_counts(self):
        counts = resolve_electrostatic_site_counts(["h2o", "li+", "co2"])
        self.assertEqual(counts, [4, 1, 3])

    def test_json_override_wins_for_monomer_specific_sites(self):
        with tempfile.TemporaryDirectory() as tmpdir:
            json_file = Path(tmpdir) / "mbx.json"
            json_file.write_text('{"mymon": {"sites": 5, "nat": 5}}', encoding="utf-8")

            counts = resolve_electrostatic_site_counts(["mymon"], json_file=json_file)

        self.assertEqual(counts, [5])


if __name__ == "__main__":
    unittest.main()
