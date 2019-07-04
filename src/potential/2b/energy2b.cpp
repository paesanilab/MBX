/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

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

        // =====>> SECTION TWOBODY_NOGRD <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (m1 == "nh4+" && m2 == "pf6-") {
        x2b_A1B4_C1D6_deg3::x2b_A1B4_C1D6_v1x pot(m1,m2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (m1 == "ch4" && m2 == "ch4") {
        x2b_A1B4_A1B4_deg4_exp0::x2b_A1B4_A1B4_v1x pot(m1,m2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (m1 == "h2o" && m2 == "pf6-") {
        x2b_A1B6_C1D2X2_deg3::x2b_A1B6_C1D2X2_v1x pot(m1,m2);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
    } else if (m1 == "h2o" && m2 == "nh4+") {
        x2b_A1B4_C1D2X2_deg4::x2b_A1B4_C1D2X2_v1x pot(m1,m2);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
    } else if (m1 == "co2" and m2 == "co2") {
        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x pot(m1,m2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (m1 == "co2" and m2 == "h2o") {
        x2b_A1B2Z2_C1D2_deg4::x2b_A1B2Z2_C1D2_v1x pot(m2,m1);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
    } else if (m1 == "ch4" and m2 == "h2o") {
        x2b_A1B2Z2_C1D4_deg3_exp0::x2b_A1B2Z2_C1D4_v1x pot(m2,m1);
        return pot.eval(xyz2.data(), xyz1.data(), nm);

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

        // =====>> SECTION TWOBODY_GRD <<=====
        // ====>> PASTE YOUR CODE BELOW <<====
    } else if (m1 == "nh4+" && m2 == "pf6-") {
        x2b_A1B4_C1D6_deg3::x2b_A1B4_C1D6_v1x pot(m1,m2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grd1.data(), grd2.data(), nm);
    } else if (m1 == "ch4" && m2 == "ch4") {
        x2b_A1B4_A1B4_deg4_exp0::x2b_A1B4_A1B4_v1x pot(m1,m2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grd1.data(), grd2.data(), nm);
    } else if (m1 == "h2o" && m2 == "pf6-") {
        x2b_A1B6_C1D2X2_deg3::x2b_A1B6_C1D2X2_v1x pot(m1,m2);
        energy = pot.eval(xyz2.data(), xyz1.data(), grd2.data(), grd1.data(), nm);
    } else if (m1 == "h2o" && m2 == "nh4+") {
        x2b_A1B4_C1D2X2_deg4::x2b_A1B4_C1D2X2_v1x pot(m1,m2);
        energy = pot.eval(xyz2.data(), xyz1.data(), grd2.data(), grd1.data(), nm);
    } else if (m1 == "co2" and m2 == "co2") {
        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x pot(m1,m2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grd1.data(), grd2.data(), nm);
    } else if (m1 == "co2" and m2 == "h2o") {
        x2b_A1B2Z2_C1D2_deg4::x2b_A1B2Z2_C1D2_v1x pot(m2,m1);
        energy = pot.eval(xyz2.data(), xyz1.data(), grd2.data(), grd1.data(), nm);
    } else if (m1 == "ch4" and m2 == "h2o") {
        x2b_A1B2Z2_C1D4_deg3_exp0::x2b_A1B2Z2_C1D4_v1x pot(m2,m1);
        energy = pot.eval(xyz2.data(), xyz1.data(), grd2.data(), grd1.data(), nm);
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
