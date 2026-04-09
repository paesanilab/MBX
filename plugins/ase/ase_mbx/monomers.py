from __future__ import annotations

import json
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
    "nma": ("C", "O", "C", "H", "H", "H", "N", "H", "C", "H", "H", "H"),
}

# Most supported monomers use one electrostatic site per real atom. Water is
# the only ASE-supported monomer here that introduces an extra virtual M-site.
DEFAULT_MONOMER_SITE_COUNTS = {name: len(pattern) for name, pattern in MONOMER_PATTERNS.items()}
DEFAULT_MONOMER_SITE_COUNTS["h2o"] = 4

_PATTERNS_BY_FIRST_SYMBOL: dict[str, list[tuple[str, tuple[str, ...]]]] = {}
for _name, _pattern in MONOMER_PATTERNS.items():
    _PATTERNS_BY_FIRST_SYMBOL.setdefault(_pattern[0], []).append((_name, _pattern))

_NO_PARSE = object()
_AMBIGUOUS = object()


def _format_pattern(pattern):
    return "-".join(pattern)


def _format_candidate_patterns(symbol):
    candidates = _PATTERNS_BY_FIRST_SYMBOL.get(symbol, ())
    return ", ".join(
        f"{name} ({_format_pattern(pattern)})" for name, pattern in candidates
    )


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

    @lru_cache(maxsize=None)
    def first_failure_index(index):
        if index == len(symbols):
            return None

        candidates = _PATTERNS_BY_FIRST_SYMBOL.get(symbols[index], ())
        if not candidates:
            return index

        matched_pattern = False
        failure_indices = []
        for _, pattern in candidates:
            end = index + len(pattern)
            if symbols[index:end] != pattern:
                continue
            matched_pattern = True
            failure_index = first_failure_index(end)
            if failure_index is None:
                return None
            failure_indices.append(failure_index)

        if not matched_pattern:
            return index

        return min(failure_indices) if failure_indices else index

    parsed = parse_from(0)
    if parsed is _AMBIGUOUS:
        raise ValueError(
            "Monomer inference is ambiguous for the current atom ordering. "
            "Reorder the atoms so each supported MBX monomer is grouped contiguously."
        )
    if parsed is _NO_PARSE:
        unsupported_index = next(
            (i for i, sym in enumerate(symbols) if sym not in _PATTERNS_BY_FIRST_SYMBOL),
            None,
        )
        if unsupported_index is not None:
            unsupported = symbols[unsupported_index]
            # TODO: Add support for more monomer types here.
            raise ValueError(
                f"Unsupported monomer atom '{unsupported}' at atom index {unsupported_index}. "
                "This atom does not map to any monomer currently available in the ASE MBX interface."
            )
        failure_index = first_failure_index(0)
        failure_symbol = symbols[failure_index]
        expected = _format_candidate_patterns(failure_symbol)
        raise ValueError(
            "Could not infer supported MBX monomers from the input atom ordering. "
            f"First unmatched atom is index {failure_index} ('{failure_symbol}'). "
            f"Monomers starting with '{failure_symbol}' expect one of: {expected}. "
            "Make sure each monomer is grouped contiguously in the MBX atom order."
        )

    monomer_names = [name for name, _ in parsed]
    nat_monomers = [nat for _, nat in parsed]
    return monomer_names, nat_monomers


def _load_json_config(json_file):
    if json_file is None:
        return {}

    try:
        with open(json_file, encoding="utf-8") as handle:
            config = json.load(handle)
    except (FileNotFoundError, OSError, json.JSONDecodeError):
        return {}

    return config if isinstance(config, dict) else {}


def resolve_electrostatic_site_counts(monomer_names, json_file=None):
    """Return the electrostatic-site count for each monomer in order.

    The ASE wrapper still only infers monomer names listed in
    ``MONOMER_PATTERNS``. For those supported monomers, the default site counts
    are derived from the built-in MBX definitions. If ``json_file`` contains a
    top-level monomer override with a ``"sites"`` field, that override wins and
    matches MBX's own ``SetUpMonomers`` path.
    """

    config = _load_json_config(json_file)
    site_counts = []
    for monomer_name in monomer_names:
        monomer_config = config.get(monomer_name, {})
        if isinstance(monomer_config, dict) and "sites" in monomer_config:
            try:
                site_counts.append(int(monomer_config["sites"]))
                continue
            except (TypeError, ValueError) as exc:
                raise ValueError(
                    f"Invalid electrostatic site count override for monomer '{monomer_name}'."
                ) from exc

        try:
            site_counts.append(DEFAULT_MONOMER_SITE_COUNTS[monomer_name])
        except KeyError as exc:
            raise ValueError(
                f"Electrostatic site count for monomer '{monomer_name}' is not defined "
                "in the ASE MBX wrapper."
            ) from exc

    return site_counts
