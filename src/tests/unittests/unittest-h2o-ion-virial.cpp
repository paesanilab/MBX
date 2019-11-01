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

#include "testutils.h"

#include "bblock/system.h"
#include "setup_na_h2o_virial.h"
#include "electrostatics.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;


TEST_CASE("Test Na H2O virial contributions") {
    SETUP_Na_H2O_VIRIAL 

    bblock::System my_system;   

    // Add monomers to the system
    size_t count = 0;
    for (size_t i = 0; i < n_monomers; i++) {
        std::vector<double> xyz(real_coords.begin() + 3 * count,
                                real_coords.begin() + 3 * count + 3 * n_atoms_vector[i]);
        std::vector<std::string> ats(atom_names.begin() + count, atom_names.begin() + count + n_atoms_vector[i]);
        std::string monid = monomer_names[i];
        my_system.AddMonomer(xyz, ats, monid);
        count += n_atoms_vector[i];
    }

    // Initialize the system to fill in the information
    my_system.Initialize();

    SECTION("One-Body") {
        double energy_grad = my_system.OneBodyEnergy(true);
        std::vector<double> my_virial = my_system.GetVirial();

            for (size_t i = 0; i < 9; i++) {
                REQUIRE(virial_1b[i] == Approx(my_virial[i]).margin(TOL));
            }

    }


    SECTION("Dispersion") {
        double energy_grad = my_system.Dispersion(true);
        std::vector<double> my_virial = my_system.GetVirial();

            for (size_t i = 0; i < 9; i++) {
                REQUIRE(virial_disp[i] == Approx(my_virial[i]).margin(TOL));
            }

    }
    SECTION("Buckingham") {
        my_system.SetTTMnrgPairs(ttm_pairs);
        double energy_grad = my_system.Buckingham(true);
        std::vector<double> my_virial = my_system.GetVirial();

            for (size_t i = 0; i < 9; i++) {
                REQUIRE(virial_buck[i] == Approx(my_virial[i]).margin(TOL));
            }

    }


    SECTION("Electrostatics") {
        double energy_grad = my_system.Electrostatics(true);
        std::vector<double> my_virial = my_system.GetVirial();

            for (size_t i = 0; i < 9; i++) {
                REQUIRE(virial_elec[i] == Approx(my_virial[i]).margin(TOL));
            }

    }



}
