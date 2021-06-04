#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>
#include <utility>
#include <algorithm>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"
#include "io_tools/read_xyz.h"
#include "io_tools/write_xyz.h"

#include "bblock/system.h"

#include "tools/xmin.h"

namespace {

// Declare the system to be read from nrg
std::vector<bblock::System> s;

}  // namespace

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <config.nrg> <configs.xyz> [<mbx.json>] \n";
        return 0;
    }

    // Declare atoms, coords and boxes to be read from xyz
    std::vector<std::vector<std::string> > ats;
    std::vector<std::vector<double> > coords;
    std::vector<std::vector<double> > boxes;

    try {
        // Read the NRG file
        tools::ReadNrg(argv[1], s);

        // Read trajectory of inputed
        tools::ReadXYZ(argv[2], ats, coords, boxes);

        if (argc > 3) {
            s[0].SetUpFromJson(argv[3]);
        } else {
            s[0].SetUpFromJson();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    std::vector<std::string> atoms = s[0].GetRealAtomNames();

    std::vector<std::pair<size_t, double> > energies;

    // Check if we need to do the xyz or just the nrg file
    for (size_t i = 0; i < coords.size(); i++) {
        s[0].SetRealXyz(coords[i]);
        double energy = s[0].Energy(false);
        energies.push_back(std::make_pair(i, energy));
    }

    for (size_t i = 0; i < coords.size() - 1; i++) {
        double e_min = energies[i].second;
        for (size_t j = i + 1; j < coords.size(); j++) {
            double e = energies[j].second;
            if (e < e_min) {
                e_min = e;
                std::swap(energies[i], energies[j]);
            }
        }
    }

    // Report optimized and non converged structures
    std::ofstream off;
    std::vector<double> box;
    off.open("ordered.xyz");
    for (size_t i = 0; i < coords.size(); i++) {
        std::stringstream ss;
        ss << std::setprecision(8) << std::scientific << energies[i].second;
        tools::WriteXYZ(off, atoms, coords[energies[i].first], box, ss.str());
    }
    off.close();

    return 0;
}
