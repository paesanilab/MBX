#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

from ase.io import read, write


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Convert a LAMMPS data file to extxyz using ASE."
    )
    parser.add_argument(
        "--input",
        required=True,
        help="Path to LAMMPS data file (e.g., initial.data).",
    )
    parser.add_argument(
        "--output",
        required=True,
        help="Path to output extxyz file.",
    )
    parser.add_argument(
        "--pbc",
        action="store_true",
        help="Enable periodic boundary conditions on output.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    in_path = Path(args.input)
    out_path = Path(args.output)

    atoms = read(in_path, format="lammps-data")
    if args.pbc:
        atoms.set_pbc(True)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    write(out_path, atoms)
    print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
