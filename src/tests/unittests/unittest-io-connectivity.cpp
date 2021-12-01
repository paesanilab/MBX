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

#include "setup_io_connectivity.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test Connectivity IO") {
    // Create the system
    SETUP_IO_CONNECTIVITY

    SECTION("Test connectivity map is correct") {
        // Write the linear and nonlinear parameters because they can not be set in
        // the macro
        // Set first connectivity object
        mon1_bond.SetParameters(morse_linear_parameters, morse_nonlinear_parameters);
        std::vector<eff::Bond> mon1_bond_vec = {mon1_bond, mon1_bond};
        mon1_angle.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        std::vector<eff::Angles> mon1_angle_vec = {mon1_angle, mon1_angle, mon1_angle};
        mon1_dihedral.SetParameters(cos_linear_parameters, cos_nonlinear_parameters);
        std::vector<eff::Dihedral> mon1_dihedral_vec = {mon1_dihedral};
        mon1_inversion.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        std::vector<eff::Inversion> mon1_inversion_vec = {mon1_inversion};

        // Set second connectivity object
        mon2_bond.SetParameters(harm_linear_parameters, harm_linear_parameters);
        std::vector<eff::Bond> mon2_bond_vec = {mon2_bond};
        mon2_angle1.SetParameters(quartic_linear_parameters, quartic_nonlinear_parameters);
        mon2_angle2.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        std::vector<eff::Angles> mon2_angle_vec = {mon2_angle1, mon2_angle2};
        mon2_dihedral.SetParameters(hcos_linear_parameters, hcos_nonlinear_parameters);
        std::vector<eff::Dihedral> mon2_dihedral_vec = {mon2_dihedral};
        mon2_inversion.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        std::vector<eff::Inversion> mon2_inversion_vec = {mon2_inversion};

        eff::Conn connectivity1 =
            eff::Conn(mon_id, mon1_bond_vec, mon1_angle_vec, mon1_dihedral_vec, mon1_inversion_vec);
        eff::Conn connectivity2 =
            eff::Conn("h4_dummy2", mon2_bond_vec, mon2_angle_vec, mon2_dihedral_vec, mon2_inversion_vec);
        std::unordered_map<std::string, eff::Conn> connectivity_map = {std::make_pair("h4_dummy", connectivity1),
                                                                       std::make_pair("h4_dummy2", connectivity2)};

        // Write out the map
        tools::WriteConnectivity("connectivity_write.out", connectivity_map);

        // Visually check the output file is correct

        // Read in the map here
        std::unordered_map<std::string, eff::Conn> read_connectivity_map;
        tools::ReadConnectivity("connectivity_write.out", read_connectivity_map);

        REQUIRE(read_connectivity_map == connectivity_map);
    }

    SECTION("Test empty string file name") {
        std::unordered_map<std::string, eff::Conn> connectivity_map;

        // Read in the map here
        std::unordered_map<std::string, eff::Conn> read_connectivity_map;
        tools::ReadConnectivity("", read_connectivity_map);

        REQUIRE(read_connectivity_map == connectivity_map);
    }

    SECTION("Test empty file") {
        std::unordered_map<std::string, eff::Conn> connectivity_map;

        // Write out the map
        tools::WriteConnectivity("connectivity_write_empty.out", connectivity_map);

        // Visually check the output file is correct

        // Read in the map here
        std::unordered_map<std::string, eff::Conn> read_connectivity_map;
        tools::ReadConnectivity("connectivity_write_empty.out", read_connectivity_map);

        REQUIRE(read_connectivity_map == connectivity_map);
    }
}
