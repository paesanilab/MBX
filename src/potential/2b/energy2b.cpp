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

double get_2b_energy(std::string mon1, std::string mon2, size_t nm, std::vector<double> xyz1, std::vector<double> xyz2) {
    // Order the two monomer names and corresponding xyz
    if (mon2 < mon1) {
        std::string tmp = mon1;
        mon1 = mon2;
        mon2 = tmp;
        std::vector<double> tmp2 = std::move(xyz1);
        xyz1 = std::move(xyz2);
        xyz2 = std::move(tmp2);
    }

    // Water water
    if (mon1 == "h2o" and mon2 == "h2o") {
        x2o::x2b_v9x pot;
        return pot.eval(xyz1.data(), xyz2.data(), nm);
        // Ion water
    } else if ((mon1 == "ar" or mon1 == "f" or mon1 == "cl" or mon1 == "br" or mon1 == "cs") and mon2 == "h2o") {
        // The order is bc the poly were generated this way
        // First water and then ion
        h2o_ion::x2b_h2o_ion_v2x pot(mon2, mon1);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
        // More ion water
    } else if (mon1 == "h2o" and (mon2 == "i" or mon2 == "li" or mon2 == "na" or mon2 == "k" or mon2 == "rb")) {
        h2o_ion::x2b_h2o_ion_v2x pot(mon1, mon2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);

        // =====>> BEGIN SECTION 2B_NO_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (mon1 == "h2o" and mon2 == "he") {
        x2b_A1B2Z2_C1_deg5::x2b_A1B2Z2_C1_v1x pot(mon1,mon2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (mon1 == "he" and mon2 == "he") {
        x2b_A1_A1_deg23::x2b_A1_A1_v1x pot(mon1,mon2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (mon1 == "ch4" && mon2 == "ch4") {
        x2b_A1B4_A1B4_deg4_exp0::x2b_A1B4_A1B4_v1x pot(mon1,mon2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (mon1 == "co2" and mon2 == "co2") {
        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x pot(mon1,mon2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
    } else if (mon1 == "co2" and mon2 == "h2o") {
        x2b_A1B2Z2_C1D2_deg4::x2b_A1B2Z2_C1D2_v1x pot(mon2,mon1);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
    } else if (mon1 == "ch4" and mon2 == "h2o") {
        x2b_A1B2Z2_C1D4_deg3_exp0::x2b_A1B2Z2_C1D4_v1x pot(mon2,mon1);
        return pot.eval(xyz2.data(), xyz1.data(), nm);
    } else if (mon1 == "ar" and mon2 == "cs") {
        mbnrg_A1_B1_deg15::mbnrg_A1_B1_deg15_v1 pot(mon1, mon2);
        return pot.eval(xyz1.data(), xyz2.data(), nm);
        // =====>> END SECTION 2B_NO_GRADIENT <<=====

    } else {
        return 0.0;
    }
}

double get_2b_energy(std::string mon1, std::string mon2, size_t nm, std::vector<double> xyz1, std::vector<double> xyz2,
                     std::vector<double> &grad1, std::vector<double> &grad2, std::vector<double> *virial) {
    // Order the two monomer names and corresponding xyz
    bool swaped = false;
    if (mon2 < mon1) {
        std::string tmp = std::move(mon1);
        mon1 = std::move(mon2);
        mon2 = std::move(tmp);
        std::vector<double> tmp2 = std::move(xyz1);
        xyz1 = std::move(xyz2);
        xyz2 = std::move(tmp2);
        tmp2 = std::move(grad1);
        grad1 = std::move(grad2);
        grad2 = std::move(tmp2);
        swaped = true;
    }

    double energy = 0.0;
    // Note: in the conditional, mon2 >= mon1 ALWAYS
    if (mon1 == "h2o" and mon2 == "h2o") {
        x2o::x2b_v9x pot;
        energy = pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);
    } else if ((mon1 == "ar" or mon1 == "f" or mon1 == "cl" or mon1 == "br" or mon1 == "cs") and mon2 == "h2o") {
        // The order is bc the poly were generated this way
        // First water and then ion
        h2o_ion::x2b_h2o_ion_v2x pot(mon2, mon1);
        energy = pot.eval(xyz2.data(), xyz1.data(), grad2.data(), grad1.data(), nm, virial);
    } else if (mon1 == "h2o" and (mon2 == "i" or mon2 == "li" or mon2 == "na" or mon2 == "k" or mon2 == "rb")) {
        h2o_ion::x2b_h2o_ion_v2x pot(mon1, mon2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);

        // =====>> BEGIN SECTION 2B_GRADIENT <<=====
        // ====>> PASTE YOUR CODE BELOW <<====
    } else if (mon1 == "h2o" and mon2 == "he") {
        x2b_A1B2Z2_C1_deg5::x2b_A1B2Z2_C1_v1x pot(mon1,mon2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);
    } else if (mon1 == "he" and mon2 == "he") {
        x2b_A1_A1_deg23::x2b_A1_A1_v1x pot(mon1,mon2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);
    } else if (mon1 == "ch4" && mon2 == "ch4") {
        x2b_A1B4_A1B4_deg4_exp0::x2b_A1B4_A1B4_v1x pot(mon1,mon2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);
    } else if (mon1 == "co2" and mon2 == "co2") {
        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x pot(mon1,mon2);
        energy = pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);
    } else if (mon1 == "co2" and mon2 == "h2o") {
        x2b_A1B2Z2_C1D2_deg4::x2b_A1B2Z2_C1D2_v1x pot(mon2,mon1);
        energy = pot.eval(xyz2.data(), xyz1.data(), grad2.data(), grad1.data(), nm, virial);
    } else if (mon1 == "ch4" and mon2 == "h2o") {
        x2b_A1B2Z2_C1D4_deg3_exp0::x2b_A1B2Z2_C1D4_v1x pot(mon2,mon1);
        energy = pot.eval(xyz2.data(), xyz1.data(), grad2.data(), grad1.data(), nm, virial);
    } else if (mon1 == "ar" and mon2 == "cs") {
        mbnrg_A1_B1_deg15::mbnrg_A1_B1_deg15_v1 pot(mon1, mon2);
        energy =  pot.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), nm, virial);
        // =====>> END SECTION 2B_GRADIENT <<=====
    } else {
        energy = 0.0;
    }

    // Reseting gradients in original order if necessary
    if (swaped) {
        std::vector<double> tmp2 = std::move(grad2);
        grad2 = std::move(grad1);
        grad1 = std::move(tmp2);
    }

    return energy;
}

}  // namespace e2b
