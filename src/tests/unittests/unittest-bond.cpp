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

#include "setup_h4_dummy.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/topology.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test dummy h4 bond") {
    // Create the system
    SETUP_H4_DUMMY

    SECTION("Bond-harmonic") {
        std::string functional_form = "harm";
        Bond obj(bond_connectivity, bond_type, bond_indexes, functional_form);
        obj.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        SECTION("Harmonic Energy") { REQUIRE(obj.GetEnergy(distance) == Approx(ff_bond_harm_energy).margin(TOL)); }
        SECTION("Harmonic Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(distance);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_bond_harm_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Harmonic Gradient") {
            REQUIRE(obj.GetTopologyGradient(distance) == Approx(ff_bond_harm_grad).margin(TOL));
        }
    }

    SECTION("Bond-morse") {
        std::string functional_form = "morse";
        Bond obj(bond_connectivity, bond_type, bond_indexes, functional_form);
        obj.SetParameters(morse_linear_parameters, morse_nonlinear_parameters);
        SECTION("Morse Energy") { REQUIRE(obj.GetEnergy(distance) == Approx(ff_bond_morse_energy).margin(TOL)); }
        SECTION("Morse Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(distance);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_bond_morse_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Morse Gradient") {
            REQUIRE(obj.GetTopologyGradient(distance) == Approx(ff_bond_morse_grad).margin(TOL));
        }
    }

    SECTION("Bond-quartic") {
        std::string functional_form = "quartic";
        Bond obj(bond_connectivity, bond_type, bond_indexes, functional_form);
        obj.SetParameters(quartic_linear_parameters, quartic_nonlinear_parameters);
        SECTION("Quartic Energy") { REQUIRE(obj.GetEnergy(distance) == Approx(ff_bond_quartic_energy).margin(TOL)); }
        SECTION("Quartic Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(distance);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_bond_quartic_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Quartic Gradient") {
            REQUIRE(obj.GetTopologyGradient(distance) == Approx(ff_bond_quartic_grad).margin(TOL));
        }
    }

    SECTION("Bond-none") {
        std::string functional_form = "none";
        Bond obj(bond_connectivity, bond_type, bond_indexes, functional_form);
        obj.SetParameters(none_linear_parameters, none_nonlinear_parameters);
        SECTION("None Energy") { REQUIRE(obj.GetEnergy(distance) == Approx(ff_bond_none_energy).margin(TOL)); }
        SECTION("None Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(distance);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_bond_none_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("None Gradient") {
            REQUIRE(obj.GetTopologyGradient(distance) == Approx(ff_bond_none_grad).margin(TOL));
        }
    }

    SECTION("Bond-undefined-func-form") {
        std::string functional_form = "abcd";
        bool not_possible_to_setup_bond = false;
        try {
            Bond obj(bond_connectivity, bond_type, bond_indexes, functional_form);
        } catch (CUException &e) {
            not_possible_to_setup_bond = true;
        }
        REQUIRE(not_possible_to_setup_bond);
    }
}
