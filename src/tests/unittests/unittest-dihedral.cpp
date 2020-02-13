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
#include "potential/force_field/dihedral.h"
#include "potential/force_field/topology.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test dummy h4 dihedral") {
    // Create the system
    SETUP_H4_DUMMY

    SECTION("Dihedral-cos") {
        std::string functional_form = "cos";
        Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);
        obj.SetParameters(cos_linear_parameters, cos_nonlinear_parameters);
        SECTION("Cosine Energy") { REQUIRE(obj.GetEnergy(dihedral_phi) == Approx(ff_dihedral_cos_energy).margin(TOL)); }
        SECTION("Cosine Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(dihedral_phi);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_dihedral_cos_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Cosine Gradient") {
            REQUIRE(obj.GetTopologyGradient(dihedral_phi) == Approx(ff_dihedral_cos_grad).margin(TOL));
        }
    }

    SECTION("Dihedral-harmonic") {
        std::string functional_form = "harm";
        Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);
        obj.SetParameters(harm_linear_parameters, harm_nonlinear_parameters);
        SECTION("Harmonic Energy") {
            REQUIRE(obj.GetEnergy(dihedral_phi) == Approx(ff_dihedral_harm_energy).margin(TOL));
        }
        SECTION("Harmonic Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(dihedral_phi);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_dihedral_harm_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Harmonic Gradient") {
            REQUIRE(obj.GetTopologyGradient(dihedral_phi) == Approx(ff_dihedral_harm_grad).margin(TOL));
        }
    }

    SECTION("Dihedral-hcos") {
        std::string functional_form = "hcos";
        Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);
        obj.SetParameters(hcos_linear_parameters, hcos_nonlinear_parameters);
        SECTION("Harmonic Cosine Energy") {
            REQUIRE(obj.GetEnergy(dihedral_phi) == Approx(ff_dihedral_hcos_energy).margin(TOL));
        }
        SECTION("Harmonic Cosine Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(dihedral_phi);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_dihedral_hcos_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Harmonic Cosine Gradient") {
            REQUIRE(obj.GetTopologyGradient(dihedral_phi) == Approx(ff_dihedral_hcos_grad).margin(TOL));
        }
    }

    SECTION("Dihedral-cos3") {
        std::string functional_form = "cos3";
        Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);
        obj.SetParameters(cos3_linear_parameters, cos3_nonlinear_parameters);
        SECTION("Cosine 3 Energy") {
            REQUIRE(obj.GetEnergy(dihedral_phi) == Approx(ff_dihedral_cos3_energy).margin(TOL));
        }
        SECTION("Cosine 3 Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(dihedral_phi);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_dihedral_cos3_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("Cosine 3 Gradient") {
            REQUIRE(obj.GetTopologyGradient(dihedral_phi) == Approx(ff_dihedral_cos3_grad).margin(TOL));
        }
    }

    SECTION("Dihedral-none") {
        std::string functional_form = "none";
        Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);
        obj.SetParameters(none_linear_parameters, none_nonlinear_parameters);
        SECTION("None Energy") { REQUIRE(obj.GetEnergy(dihedral_phi) == Approx(ff_dihedral_none_energy).margin(TOL)); }
        SECTION("None Non Linear Value") {
            std::vector<double> non_lin = obj.GetNonLinearValue(dihedral_phi);
            for (int i = 0; i < non_lin.size(); i++) {
                REQUIRE(non_lin[i] == Approx(ff_dihedral_none_non_lin_val[i]).margin(TOL));
            }
        }
        SECTION("None Gradient") {
            REQUIRE(obj.GetTopologyGradient(dihedral_phi) == Approx(ff_dihedral_none_grad).margin(TOL));
        }
    }

    SECTION("Dihedral-undefined-func-form") {
        std::string functional_form = "abcd";
        bool not_possible_to_setup_dihedral = false;
        try {
            Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);
        } catch (CUException &e) {
            not_possible_to_setup_dihedral = true;
        }
        REQUIRE(not_possible_to_setup_dihedral);
    }

    SECTION("Dihedral-include-exclude") {
        std::string functional_form = "none";
        bool possible_to_set_exclude = true;
        std::string include_exclude = "Exclude";
        Dihedral obj(dihedral_connectivity, dihedral_type, dihedral_indexes, functional_form);

        SECTION("Set Include Exclude") {
            try {
                obj.SetIncludeExclude(include_exclude);
            } catch (CUException &e) {
                possible_to_set_exclude = false;
            }
            REQUIRE(possible_to_set_exclude);
            REQUIRE(obj.GetIncludeExclude() == include_exclude);
        }
    }
}
