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

#include "energy3b.h"
#include <iostream>

namespace e3b {

double get_3b_energy(std::string mon1, std::string mon2, std::string mon3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3) {
    // Order the three monomer names and corresponding xyz
    // Check if mon1 is the largest
    if (mon1 > mon2 and mon1 > mon3) {
        std::swap(mon1, mon3);
        std::swap(xyz1, xyz3);
        // Check if mon2 is the largest
    } else if (mon2 > mon1 and mon2 > mon3) {
        std::swap(mon2, mon3);
        std::swap(xyz2, xyz3);
    }

    // At this point mon3 is always the largest
    // Now sort mon1 and mon2
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
    }

    if (mon1 == "h2o" and mon2 == "h2o" and mon3 == "h2o") {
        x2o::x3b_v2x pot;
        return pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), nm);
    } else if (mon1 == "h2o" and mon2 == "h2o" and (mon3 == "li" or mon3 == "na" or mon3 == "k" or mon3 == "rb")) {
        x3b_h2o_ion_v1x_deg4_filtered pot(mon3);
        return pot(xyz1.data(), xyz2.data(), xyz3.data(), nm);
    } else if (mon1 == "cs" and mon2 == "h2o" and mon3 == "h2o") {
        x3b_h2o_ion_v1x_deg4_filtered pot(mon1);
        return pot(xyz2.data(), xyz3.data(), xyz1.data(), nm);
        // =====>> BEGIN SECTION 3B_NO_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (mon1 == "ch4" and mon2 == "h2o" and mon3 == "h2o") {
        mbnrg_A1B4_C1D2_C1D2_deg3::mbnrg_A1B4_C1D2_C1D2_deg3_v1 pot(mon1, mon2, mon3);
        return pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), nm);

        // =====>> END SECTION 3B_NO_GRADIENT <<=====
    } else {
        return 0.0;
    }
}

double get_3b_energy(std::string mon1, std::string mon2, std::string mon3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3, std::vector<double> &grad1,
                     std::vector<double> &grad2, std::vector<double> &grad3, std::vector<double> *virial) {
    size_t index1(1), index2(2), index3(3);

    // Order the three monomer names and corresponding xyz
    // Check if mon1 is the largest
    if (mon1 > mon2 and mon1 > mon3) {
        std::swap(mon1, mon3);
        std::swap(xyz1, xyz3);
        std::swap(grad1, grad3);
        std::swap(index1, index3);
        // Check if mon2 is the largest
    } else if (mon2 > mon1 and mon2 > mon3) {
        std::swap(mon2, mon3);
        std::swap(xyz2, xyz3);
        std::swap(grad2, grad3);
        std::swap(index2, index3);
    }

    // At this point mon3 is always the largest
    // Now sort mon1 and mon2
    if (mon1 > mon2) {
        std::swap(mon1, mon2);
        std::swap(xyz1, xyz2);
        std::swap(grad1, grad2);
        std::swap(index1, index2);
    }

    double energy = 0.0;
    // Note: in the conditional, mon2 >= mon1 ALWAYS
    if (mon1 == "h2o" and mon2 == "h2o" and mon3 == "h2o") {
        x2o::x3b_v2x pot;
        energy = pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), nm, virial);
    } else if (mon1 == "h2o" and mon2 == "h2o" and (mon3 == "li" or mon3 == "na" or mon3 == "k" or mon3 == "rb")) {
        x3b_h2o_ion_v1x_deg4_filtered pot(mon3);
        energy = pot(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), nm, virial);
    } else if (mon1 == "cs" and mon2 == "h2o" and mon3 == "h2o") {
        x3b_h2o_ion_v1x_deg4_filtered pot(mon1);
        energy = pot(xyz2.data(), xyz3.data(), xyz1.data(), grad2.data(), grad3.data(), grad1.data(), nm, virial);
        // =====>> BEGIN SECTION 3B_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (mon1 == "ch4" and mon2 == "h2o" and mon3 == "h2o") {
        mbnrg_A1B4_C1D2_C1D2_deg3::mbnrg_A1B4_C1D2_C1D2_deg3_v1 pot(mon1, mon2, mon3);
        energy = pot.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), nm, virial);

        // =====>> END SECTION 3B_GRADIENT <<=====
    } else {
        energy = 0.0;
    }

    if (index1 != 1) {
        if (index2 == 1) {
            std::swap(index1, index2);
            std::swap(grad1, grad2);
        } else if (index3 == 1) {
            std::swap(index1, index3);
            std::swap(grad1, grad3);
        }  // else { error
    }

    if (index2 != 2) {
        std::swap(index2, index3);
        std::swap(grad2, grad3);
    }

    return energy;
}

}  // namespace e3b
