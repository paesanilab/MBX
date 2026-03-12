#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Convert a LAMMPS data file to a minimal extxyz (species + positions)."
    )
    parser.add_argument("--input", required=True, help="Path to LAMMPS data file.")
    parser.add_argument("--output", required=True, help="Path to output extxyz file.")
    parser.add_argument(
        "--pbc",
        action=argparse.BooleanOptionalAction,
        default=True,
        help="Enable/disable periodic boundary conditions (default: enabled).",
    )
    parser.add_argument(
        "--reorder-water",
        action="store_true",
        help="Reorder water atoms as O-H-H using a distance cutoff.",
    )
    parser.add_argument(
        "--oh-cutoff",
        type=float,
        default=1.3,
        help="O-H cutoff in Angstrom for water reordering (default: 1.3).",
    )
    return parser.parse_args()


def _read_sections(lines: list[str]) -> dict[str, list[str]]:
    sections: dict[str, list[str]] = {"Masses": [], "Atoms": []}
    current = None
    section_re = re.compile(r"^(Masses|Atoms)\b")
    stop_re = re.compile(
        r"^(Bonds|Angles|Dihedrals|Impropers|Velocities|Pair Coeffs|Bond Coeffs|"
        r"Angle Coeffs|Dihedral Coeffs|Improper Coeffs)\b"
    )
    for line in lines:
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            continue
        if section_re.match(stripped):
            current = "Masses" if stripped.startswith("Masses") else "Atoms"
            continue
        if stop_re.match(stripped):
            current = None
            continue
        if current is not None:
            sections[current].append(stripped)
    return sections


def _parse_box(lines: list[str]) -> list[float]:
    bounds = []
    for line in lines:
        if "xlo" in line or "ylo" in line or "zlo" in line:
            parts = line.split()
            bounds.append((float(parts[0]), float(parts[1])))
    if len(bounds) != 3:
        raise ValueError("Failed to parse xlo/xhi, ylo/yhi, zlo/zhi from header.")
    return [bounds[0][1] - bounds[0][0], bounds[1][1] - bounds[1][0], bounds[2][1] - bounds[2][0]]


def _parse_masses(section: list[str]) -> dict[int, float]:
    masses: dict[int, float] = {}
    for line in section:
        parts = line.split()
        if len(parts) < 2:
            continue
        masses[int(parts[0])] = float(parts[1])
    return masses


def _mass_to_element(mass: float) -> str:
    # Simple mapping for water systems.
    if abs(mass - 15.999) < 0.2:
        return "O"
    if abs(mass - 1.008) < 0.2:
        return "H"
    if abs(mass - 22.989) < 0.5:
        return "Na"
    return "X"


def _parse_atoms(section: list[str], type_to_elem: dict[int, str]) -> list[tuple[str, float, float, float]]:
    atoms = []
    for line in section:
        parts = line.split()
        if len(parts) < 5:
            continue
        # Handle common LAMMPS atom styles:
        # full:  id mol type q x y z ...
        # molecular: id mol type x y z ...
        # atomic: id type x y z ...
        if len(parts) >= 7:
            atom_type = int(parts[2])
            x, y, z = map(float, parts[4:7])
        elif len(parts) == 6:
            atom_type = int(parts[2])
            x, y, z = map(float, parts[3:6])
        else:
            atom_type = int(parts[1])
            x, y, z = map(float, parts[2:5])
        elem = type_to_elem.get(atom_type, "X")
        atoms.append((elem, x, y, z))
    return atoms


def _minimum_image(dx: float, box_len: float) -> float:
    return dx - round(dx / box_len) * box_len


def _distance(a, b, box, use_pbc: bool) -> float:
    dx = a[1] - b[1]
    dy = a[2] - b[2]
    dz = a[3] - b[3]
    if use_pbc:
        dx = _minimum_image(dx, box[0])
        dy = _minimum_image(dy, box[1])
        dz = _minimum_image(dz, box[2])
    return (dx * dx + dy * dy + dz * dz) ** 0.5


def _reorder_waters(atoms, box, cutoff, use_pbc):
    oxy = [a for a in atoms if a[0] == "O"]
    hyd = [a for a in atoms if a[0] == "H"]
    other = [a for a in atoms if a[0] not in {"O", "H"}]

    used_h = set()
    ordered = []
    for o in oxy:
        # Find two nearest H within cutoff.
        candidates = []
        for idx, h in enumerate(hyd):
            if idx in used_h:
                continue
            dist = _distance(o, h, box, use_pbc)
            if dist <= cutoff:
                candidates.append((dist, idx, h))
        candidates.sort()
        if len(candidates) >= 2:
            _, idx1, h1 = candidates[0]
            _, idx2, h2 = candidates[1]
            used_h.update({idx1, idx2})
            ordered.extend([o, h1, h2])
        else:
            ordered.append(o)

    # Append leftover H and other species.
    for idx, h in enumerate(hyd):
        if idx not in used_h:
            ordered.append(h)
    ordered.extend(other)
    return ordered


def main() -> None:
    args = parse_args()
    in_path = Path(args.input)
    out_path = Path(args.output)

    lines = in_path.read_text().splitlines()
    box = _parse_box(lines)
    sections = _read_sections(lines)
    masses = _parse_masses(sections.get("Masses", []))
    type_to_elem = {t: _mass_to_element(m) for t, m in masses.items()}
    atoms = _parse_atoms(sections.get("Atoms", []), type_to_elem)

    if not atoms:
        raise ValueError("No atoms parsed from LAMMPS data file.")

    if args.reorder_water:
        atoms = _reorder_waters(atoms, box, args.oh_cutoff, args.pbc)

    unknown = sum(1 for a in atoms if a[0] == "X")
    if unknown:
        print(f"Warning: {unknown} atoms have unknown element 'X'.")

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as f:
        f.write(f"{len(atoms)}\n")
        pbc = "T T T" if args.pbc else "F F F"
        f.write(
            'Lattice="{0:.6f} 0 0 0 {1:.6f} 0 0 0 {2:.6f}" '
            'Properties=species:S:1:pos:R:3 pbc="{3}"\n'.format(box[0], box[1], box[2], pbc)
        )
        for elem, x, y, z in atoms:
            f.write(f"{elem} {x:.6f} {y:.6f} {z:.6f}\n")

    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
