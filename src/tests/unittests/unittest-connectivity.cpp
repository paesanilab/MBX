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

#include "setup_connectivity.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test Connectivity") {
    // Create the system
    SETUP_CONNECTIVITY

    // Write the linear and nonlinear parameters because they can not be set in
    // the macro
    mon1_bond.SetParameters(morse_linear_parameters, morse_nonlinear_parameters);
    std::vector<eff::Bond> mon1_bond_vec = {mon1_bond, mon1_bond};
    mon1_angle.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
    std::vector<eff::Angles> mon1_angle_vec = {mon1_angle, mon1_angle, mon1_angle};
    mon1_dihedral.SetParameters(cos_linear_parameters, cos_nonlinear_parameters);
    std::vector<eff::Dihedral> mon1_dihedral_vec = {mon1_dihedral};
    mon1_inversion.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
    std::vector<eff::Inversion> mon1_inversion_vec = {mon1_inversion};

    SECTION("Connectivity constructor") {
        bool not_possible_to_setup_connectivity = false;
        try {
            eff::Conn connectivity =
                eff::Conn(mon_id, mon1_bond_vec, mon1_angle_vec, mon1_dihedral_vec, mon1_inversion_vec);
        } catch (CUException &e) {
            not_possible_to_setup_connectivity = true;
        }
        REQUIRE(!not_possible_to_setup_connectivity);
    }

    SECTION("Connectivity Getters") {
        eff::Conn connectivity =
            eff::Conn(mon_id, mon1_bond_vec, mon1_angle_vec, mon1_dihedral_vec, mon1_inversion_vec);
        REQUIRE(connectivity.GetMonId() == mon_id);
        REQUIRE(connectivity.GetBondVec() == mon1_bond_vec);
        REQUIRE(connectivity.GetAnglesVec() == mon1_angle_vec);
        REQUIRE(connectivity.GetDihedralVec() == mon1_dihedral_vec);
        REQUIRE(connectivity.GetInversionVec() == mon1_inversion_vec);
    }
}
