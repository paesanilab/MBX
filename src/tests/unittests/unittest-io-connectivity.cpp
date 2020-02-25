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

    // Write the linear and nonlinear parameters because they can not be set in
    // the macro
    // obj1
    bond1.SetParameters(morse_linear_parameters, morse_nonlinear_parameters);
    std::vector<Bond> bond_vec = {bond1, bond1};
    angle1.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
    std::vector<Angles> angle_vec = {angle1, angle1, angle1};
    dihedral1.SetParameters(cos_linear_parameters, cos_nonlinear_parameters);
    std::vector<Dihedral> dihedral_vec = {dihedral1};
    inversion1.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
    std::vector<Inversion> inversion_vec = {inversion1};

    // obj2
    bond2.SetParameters(harm_linear_parameters, harm_linear_parameters);
    std::vector<Bond> bond_vec2 = {bond2};
    angle2.SetParameters(quartic_linear_parameters, quartic_nonlinear_parameters);
    angle3.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
    std::vector<Angles> angle_vec2 = {angle2, angle3};
    dihedral2.SetParameters(hcos_linear_parameters, hcos_nonlinear_parameters);
    std::vector<Dihedral> dihedral_vec2 = {dihedral2};
    inversion2.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);

    std::vector<Inversion> inversion_vec2 = {inversion2};
    connectivity::Conn connectivityObj1 = connectivity::Conn(mon_id, bond_vec, angle_vec, dihedral_vec, inversion_vec);
    connectivity::Conn connectivityObj2 =
        connectivity::Conn("h4_dummy2", bond_vec2, angle_vec2, dihedral_vec2, inversion_vec2);
    std::unordered_map<std::string, connectivity::Conn> connectivity_map = {
        std::make_pair("h4_dummy", connectivityObj1), std::make_pair("h4_dummy2", connectivityObj2)};

    // Write out the map
    tools::WriteConnectivity("connectivity_write.out", connectivity_map);

    // Visually check the output file is correct

    // Read in the map here
    std::unordered_map<std::string, connectivity::Conn> read_connectivity_map;
    tools::ReadConnectivity("connectivity_write.out", read_connectivity_map);

    SECTION("Test connectivity map is correct") { REQUIRE(read_connectivity_map == connectivity_map); }
}
