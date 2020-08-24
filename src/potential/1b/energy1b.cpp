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

#include "energy1b.h"

namespace e1b {

double get_1b_energy(std::string mon1, size_t nm, std::vector<double> xyz1, std::vector<size_t> &bad_idxs) {
    std::vector<double> energies;
    // Look for the proper call to energy depending on the monomer id
    if (mon1 == "h2o") {
        energies = ps::pot_nasa(xyz1.data(), 0, nm);

        // =====>> BEGIN SECTION 1B_NO_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====
    } else if (mon1 == "ch4") {
        x1b_A1B4_deg5_exp0::x1b_A1B4_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "co2") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
        // =====>> END SECTION 1B_NO_GRADIENT <<=====
    } else {
        return 0.0;
    }

    // Add total energy, and check for too high energies
    double e = 0.0;
    for (int i = 0; i < nm; i++) {
        e += energies[i];
        if (energies[i] > EMAX1B) bad_idxs.push_back(i);
    }

    // Return energy
    return e;
}

double get_1b_energy(std::string mon1, size_t nm, std::vector<double> xyz1, std::vector<double> &grad1,
                     std::vector<size_t> &bad_idxs, std::vector<double> *virial) {
    std::vector<double> energies;
    // Look for the proper call to energy depending on the monomer id
    if (mon1 == "h2o") {
        energies = ps::pot_nasa(xyz1.data(), grad1.data(), nm, virial);

        // =====>> BEGIN SECTION 1B_GRADIENT <<=====
        // ====>> PASTE YOUR CODE BELOW <<====
    } else if (mon1 == "ch4") {
        x1b_A1B4_deg5_exp0::x1b_A1B4_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "co2") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
        // =====>> END SECTION 1B_GRADIENT <<=====
    } else {
        return 0.0;
    }

    // Add total energy, and check for too high energies
    double e = 0.0;
    for (int i = 0; i < nm; i++) {
        e += energies[i];
        if (energies[i] > EMAX1B) bad_idxs.push_back(i);
    }

    // Return energy
    return e;
}

}  // namespace e1b
