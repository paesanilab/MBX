#include "energy1b.h"

namespace e1b {

double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz, bool &good) {
    std::vector<double> energies;
    // Look for the proper call to energy depending on the monomer id
    if (mon == "h2o") {
        energies = ps::pot_nasa(xyz.data(), 0, nm);
    } else {
        return 0.0;
    }

    // Add total energy, and check for too high energies
    double e = 0.0;
    for (int i = 0; i < nm; i++) {
        e += energies[i];
        if (energies[i] > EMAX1B) good = false;
    }

    // Return energy
    return e;
}

double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz, std::vector<double> &grad, bool &good) {
    std::vector<double> energies;
    // Look for the proper call to energy depending on the monomer id
    if (mon == "h2o") {
        energies = ps::pot_nasa(xyz.data(), grad.data(), nm);
    } else {
        return 0.0;
    }

    // Add total energy, and check for too high energies
    double e = 0.0;
    for (int i = 0; i < nm; i++) {
        e += energies[i];
        if (energies[i] > EMAX1B) good = false;
    }

    // Return energy
    return e;
}

}  // namespace e1b
