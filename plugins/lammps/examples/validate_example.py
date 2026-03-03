# Script to compare thermodynamic sections of two LAMMPS log files
import sys
import difflib
import glob
import os

def extract_thermo_section(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    start, end = None, None
    for i, line in enumerate(lines):
        if line.strip().startswith("Step") and "Time" in line:
            start = i
        if line.strip().startswith("Loop time"):
            end = i
            break
    if start is None or end is None:
        raise ValueError(f"Could not find thermodynamic section in {filename}")
    return [l.rstrip() for l in lines[start:end]]

def compare_sections(section1, section2):
    if section1 == section2:
        print("Thermodynamic sections match!")
        return True
    else:
        print("Differences:")
        for line in difflib.unified_diff(section1, section2, fromfile='log.lammps', tofile='expected.log'):
            print(line)
        return False

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: python3 {sys.argv[0]} example_name")
        sys.exit(1)
        
    expected_log = f"{sys.argv[1]}/log.*.g++.1"
    run_log = f"{sys.argv[1]}/log.lammps"

    expected_log = glob.glob(expected_log)
    if not expected_log or len(expected_log) > 1:
        print(f"Expected log file not found or multiple found: {expected_log}")
        sys.exit(1)

    # verify that the run log exists
    if not os.path.isfile(run_log):
        print(f"Run log file not found: {run_log}")
        sys.exit(1)

    section1 = extract_thermo_section(expected_log[0])
    section2 = extract_thermo_section(run_log)
    is_same = compare_sections(section1, section2)
    if is_same:
        sys.exit(0)
    else:
        print("Failed validation: Thermodynamic sections do not match.")
        sys.exit(1)
