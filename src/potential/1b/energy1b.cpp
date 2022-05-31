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
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < xyz1.size(); i++) {
        std::cerr << xyz1[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    std::vector<double> energies;
    // Look for the proper call to energy depending on the monomer id
    if (mon1 == "h2o") {
        energies = ps::pot_nasa(xyz1.data(), 0, nm);

    } else if (mon1 == "ch4") {
        x1b_A1B4_deg5_exp0::x1b_A1B4_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "co2_archive") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot("co2");
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "nh3") {
        mbnrg_A1B3_deg5::mbnrg_A1B3_deg5_v1 pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "nh3pbe0d3bj") {
        mbnrg_A1B3_deg5::mbnrg_A1B3_deg5_v1 pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "co2") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "co2cm5100" || mon1 == "co2cm595" || mon1 == "co2cm590" || mon1 == "co2cm5875" ||
               mon1 == "co2cm585" || mon1 == "co2cm580") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot("co2");
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "h2") {
        mbnrg_A2_deg8::mbnrg_A2_deg8_v1 pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "n2o5") {
        x1b_A1B2C4_deg5::x1b_A1B2C4_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
    } else if (mon1 == "mbpbe") {
        mbnrg_A1B2_deg6::mbnrg_A1B2_deg6_vmbpbe pot(mon1);
        energies = pot.eval(xyz1.data(), nm);
        // =====>> BEGIN SECTION 1B_NO_GRADIENT <<=====
        // =====>> PASTE YOUR CODE BELOW <<=====

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

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Output bad indexes for " << mon1 << ":\n";
    for (size_t i = 0; i < bad_idxs.size(); i++) {
        std::cerr << bad_idxs[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Individual energies:\n";
    for (size_t i = 0; i < energies.size(); i++) {
        std::cerr << energies[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output energy: " << e << std::endl;
#endif

    // Return energy
    return e;
}

double get_1b_energy(std::string mon1, size_t nm, std::vector<double> xyz1, std::vector<double> &grad1,
                     std::vector<size_t> &bad_idxs, std::vector<double> *virial) {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < xyz1.size(); i++) {
        std::cerr << xyz1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input gradients for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input virial:" << std::endl;
    for (size_t i = 0; i < (*virial).size(); i++) {
        std::cerr << (*virial)[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    std::vector<double> energies;
    // Look for the proper call to energy depending on the monomer id
    if (mon1 == "h2o") {
        energies = ps::pot_nasa(xyz1.data(), grad1.data(), nm, virial);

        // =====>> BEGIN SECTION 1B_GRADIENT <<=====
        // ====>> PASTE YOUR CODE BELOW <<====
    } else if (mon1 == "ch4") {
        x1b_A1B4_deg5_exp0::x1b_A1B4_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "co2_archive") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot("co2");
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "nh3") {
        mbnrg_A1B3_deg5::mbnrg_A1B3_deg5_v1 pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "nh3pbe0d3bj") {
        mbnrg_A1B3_deg5::mbnrg_A1B3_deg5_v1 pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "co2") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "co2cm5100" || mon1 == "co2cm595" || mon1 == "co2cm590" || mon1 == "co2cm5875" ||
               mon1 == "co2cm585" || mon1 == "co2cm580") {
        x1b_A1B2_deg4::x1b_A1B2_v1x pot("co2");
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "h2") {
        mbnrg_A2_deg8::mbnrg_A2_deg8_v1 pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "n2o5") {
        x1b_A1B2C4_deg5::x1b_A1B2C4_v1x pot(mon1);
        energies = pot.eval(xyz1.data(), grad1.data(), nm, virial);
    } else if (mon1 == "mbpbe") {
        mbnrg_A1B2_deg6::mbnrg_A1B2_deg6_vmbpbe pot(mon1);
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

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Output bad indexes for " << mon1 << ":\n";
    for (size_t i = 0; i < bad_idxs.size(); i++) {
        std::cerr << bad_idxs[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output gradients for " << nm << " monomers of type " << mon1 << ":\n";
    for (size_t i = 0; i < grad1.size(); i++) {
        std::cerr << grad1[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output virial:" << std::endl;
    for (size_t i = 0; i < (*virial).size(); i++) {
        std::cerr << (*virial)[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Individual energies:\n";
    for (size_t i = 0; i < energies.size(); i++) {
        std::cerr << energies[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output energy: " << e << std::endl;
#endif

    // Return energy
    return e;
}

}  // namespace e1b
