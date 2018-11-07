#include "energy2b.h"

/**
 * @file energy2b.cpp
 * @brief Contains the implementation of the 2b energy calls
 */

namespace e2b {

double get_2b_energy(std::string m1, std::string m2, size_t nm, std::vector<double> xyz1, std::vector<double> xyz2) {
    // Order the two monomer names and corresponding xyz
    if (m2 < m1) {
        std::string tmp = m1;
        m1 = m2;
        m2 = tmp;
        std::vector<double> tmp2 = std::move(xyz1);
        xyz1 = std::move(xyz2);
        xyz2 = std::move(tmp2);
    }

    // Water water
    if (m1 == "h2o" and m2 == "h2o") {
        x2o::x2b_v9x pot;
        return pot.eval(xyz1.data(), xyz2.data(), nm);
        // Ion water
    } else if ((m1 == "f" or m1 == "cl" or m1 == "br" or m1 == "cs") and m2 == "h2o") {
        // The order is bc the poly were generated this way
        // First water and then ion
        h2o_ion::x2b_h2o_ion_v2x pot(m2, m1);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
        // More ion water
    } else if (m1 == "h2o" and (m2 == "i" or m2 == "li" or m2 == "na" or m2 == "k" or m2 == "rb")) {
        h2o_ion::x2b_h2o_ion_v2x pot(m1, m2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else {
        return 0.0;
    }
}

double get_2b_energy(std::string m1, std::string m2, size_t nm, std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grd1, std::vector<double> &grd2) {
    // Order the two monomer names and corresponding xyz
    bool swaped = false;
    if (m2 < m1) {
        std::string tmp = std::move(m1);
        m1 = std::move(m2);
        m2 = std::move(tmp);
        std::vector<double> tmp2 = std::move(xyz1);
        xyz1 = std::move(xyz2);
        xyz2 = std::move(tmp2);
        tmp2 = std::move(grd1);
        grd1 = std::move(grd2);
        grd2 = std::move(tmp2);
        swaped = true;
    }

    double energy = 0.0;
    // Note: in the conditional, m2 >= m1 ALWAYS
    if (m1 == "h2o" and m2 == "h2o") {
        x2o::x2b_v9x pot;
        energy = pot.eval(xyz1.data(), xyz2.data(), grd1.data(), grd2.data(), nm);
    } else if ((m1 == "f" or m1 == "cl" or m1 == "br" or m1 == "cs") and m2 == "h2o") {
        // The order is bc the poly were generated this way
        // First water and then ion
        h2o_ion::x2b_h2o_ion_v2x pot(m2, m1);
        energy = pot.eval(xyz2.data(), xyz1.data(), grd2.data(), grd1.data(), nm);
    } else if (m1 == "h2o" and (m2 == "i" or m2 == "li" or m2 == "na" or m2 == "k" or m2 == "rb")) {
        h2o_ion::x2b_h2o_ion_v2x pot(m1, m2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grd1.data(), grd2.data(), nm);
    } else {
        return 0.0;
    }

    // Reseting gradients in original order if necessary
    if (swaped) {
        std::vector<double> tmp2 = std::move(grd2);
        grd2 = std::move(grd1);
        grd1 = std::move(tmp2);
    }

    return energy;
}

}  // namespace e2b
