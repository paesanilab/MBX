from __future__ import annotations

from functools import lru_cache


# TODO: Move this monomer selection logic into the MBX side.
MONOMER_PATTERNS = {
    "h2o": ("O", "H", "H"),
    "li+": ("Li",),
    "na+": ("Na",),
    "k+": ("K",),
    "rb+": ("Rb",),
    "cs+": ("Cs",),
    "f-": ("F",),
    "cl-": ("Cl",),
    "br-": ("Br",),
    "i-": ("I",),
    "co2": ("C", "O", "O"),
    "ch4": ("C", "H", "H", "H", "H"),
    "he": ("He",),
    "ar": ("Ar",),
    "h2": ("H", "H"),
    "n2o5": ("O", "N", "N", "O", "O", "O", "O"),
    "so4a": ("S", "O", "O", "O", "O"),
    "co3a": ("C", "O", "O", "O"),
    "no3a": ("N", "O", "O", "O"),
    "dp2": ("X", "X"),
    "nma": ("C", "O", "C", "H", "H", "H", "N", "H", "C", "H", "H", "H"),
}

_PATTERNS_BY_FIRST_SYMBOL: dict[str, list[tuple[str, tuple[str, ...]]]] = {}
for _name, _pattern in MONOMER_PATTERNS.items():
    _PATTERNS_BY_FIRST_SYMBOL.setdefault(_pattern[0], []).append((_name, _pattern))

_NO_PARSE = object()
_AMBIGUOUS = object()


def infer_mbx_monomers(symbols):
    symbols = tuple(symbols)

    @lru_cache(maxsize=None)
    def parse_from(index):
        if index == len(symbols):
            return ()

        candidates = _PATTERNS_BY_FIRST_SYMBOL.get(symbols[index], ())
        if not candidates:
            return _NO_PARSE

        first_solution = None
        for monomer_name, pattern in candidates:
            end = index + len(pattern)
            if symbols[index:end] != pattern:
                continue
            tail = parse_from(end)
            if tail is _NO_PARSE:
                continue
            if tail is _AMBIGUOUS:
                return _AMBIGUOUS
            solution = ((monomer_name, len(pattern)),) + tail
            if first_solution is None:
                first_solution = solution
            elif solution != first_solution:
                return _AMBIGUOUS

        return first_solution if first_solution is not None else _NO_PARSE

    parsed = parse_from(0)
    if parsed is _AMBIGUOUS:
        raise ValueError(
            "Atom ordering matches multiple supported MBX monomer decompositions. "
            "Reorder the input so monomer grouping is unambiguous."
        )
    if parsed is _NO_PARSE:
        unsupported = next((sym for sym in symbols if sym not in _PATTERNS_BY_FIRST_SYMBOL), None)
        if unsupported is not None:
            # TODO: Add support for more monomer types here.
            raise ValueError(f"Monomer for atom '{unsupported}' is not available.")
        raise ValueError(
            "Could not infer supported MBX monomers from the input atom ordering. "
            "Ensure atoms are grouped in the expected MBX monomer order."
        )

    monomer_names = [name for name, _ in parsed]
    nat_monomers = [nat for _, nat in parsed]
    return monomer_names, nat_monomers
