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

#include "tools/testutils.h"

#include "potential/force_field/inversion.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

constexpr double TOL = 1E-6;

TEST_CASE("inversion::constructor") {
    std::string topo = "InvERsiON";
    std::vector<size_t> idxs = {0, 1, 2, 3};
    std::vector<std::string> funcform = {"NONE", "haRm"};

    std::vector<size_t> expected_num_linear = {0, 1};
    std::vector<size_t> expected_num_nonlinear = {0, 1};
    std::string expected_topo = "inversion";
    std::vector<std::string> expected_funcform = {"none", "harm"};

    for (size_t i = 0; i < funcform.size(); i++) {
        SECTION(funcform[i]) {
            eff::Inversion inversion(topo, idxs, funcform[i]);
            size_t nlinear = inversion.GetNumLinear();
            size_t nnonlinear = inversion.GetNumNonLinear();
            std::string topology = inversion.GetTopology();
            std::string functional_form = inversion.GetFunctionalForm();
            std::vector<size_t> indexes = inversion.GetIndexes();

            REQUIRE(nlinear == expected_num_linear[i]);
            REQUIRE(nnonlinear == expected_num_nonlinear[i]);
            REQUIRE(topology == expected_topo);
            REQUIRE(functional_form == expected_funcform[i]);
            REQUIRE(VectorsAreEqual(indexes, idxs));
        }
    }

    SECTION("Unknown functional form") {
        bool not_possible_to_create_inversion = false;
        try {
            eff::Inversion inversion(topo, idxs, "whatIsThis");
        } catch (...) {
            not_possible_to_create_inversion = true;
        }
        REQUIRE(not_possible_to_create_inversion);
    }
}

TEST_CASE("inversion::EnergyAndGrads") {
    std::string topo = "inversion";
    std::vector<size_t> idxs = {0, 1, 2, 3};
    std::vector<std::string> funcform = {"none", "harm"};

    std::vector<std::vector<double> > linear_params = {{}, {2.0}};
    std::vector<std::vector<double> > nonlinear_params = {{}, {2.5}};

    for (size_t i = 0; i < funcform.size(); i++) {
        double x = 2.4;
        double s = 0.0001;
        SECTION(funcform[i]) {
            eff::Inversion inversion(topo, idxs, funcform[i]);

            inversion.SetParameters(linear_params[i], nonlinear_params[i]);

            double epp = inversion.GetEnergy(x + 2 * s);
            double ep = inversion.GetEnergy(x + s);
            double em = inversion.GetEnergy(x - s);
            double emm = inversion.GetEnergy(x - 2 * s);

            double g = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;

            double gnum = inversion.GetTopologyGradient(x);

            REQUIRE(g == Approx(gnum).margin(TOL));
        }
    }
}

TEST_CASE("inversion::operator==") {
    std::string topo = "inversion";
    std::vector<size_t> idxs = {0, 1, 2, 3};
    std::string funcform = "harm";

    eff::Inversion inversion1(topo, idxs, funcform);
    eff::Inversion inversion2(topo, idxs, funcform);

    std::string funcform3 = "noNe";
    eff::Inversion inversion3(topo, idxs, funcform3);

    std::vector<size_t> idxs4 = {0, 2, 1, 5};
    eff::Inversion inversion4(topo, idxs4, funcform);

    REQUIRE(inversion1 == inversion2);
    REQUIRE(!(inversion1 == inversion3));
    REQUIRE(!(inversion1 == inversion4));
}
