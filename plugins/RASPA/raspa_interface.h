#include <vector>
#include <string>
#include <span>

#include "bblock/system.h"

import simulationbox;
import atom;

namespace raspa_interface {

    struct MBXMonomerInfo {
        int numAtoms;
        int monomerName;
    }

    MBXMonomerInfo getMBXMonomerInfo(std::string first_atom_type);

    float energy(
        const SimulationBox &simulationBox,
        std::span<const Atom> moleculeAtoms,
        std::span<const Atom> frameworkAtoms
    );
}
