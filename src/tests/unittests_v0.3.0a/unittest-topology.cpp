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

#include "potential/force_field/bond.h"
#include "potential/force_field/topology.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

constexpr double TOL = 1E-6;

TEST_CASE("topology::Getters") {
    std::string topo = "BoNd";
    std::vector<size_t> idxs = {0, 1};
    std::vector<std::string> funcform = {"NONE", "haRm", "Morse", "quartiC"};
    std::vector<std::vector<double> > linear_params = {{}, {2.0}, {2.1}, {10.0, 1.0, 0.1}};
    std::vector<std::vector<double> > nonlinear_params = {{}, {3.0}, {2.5, 5.1}, {1.5}};

    std::vector<size_t> expected_num_linear = {0, 1, 1, 3};
    std::vector<size_t> expected_num_nonlinear = {0, 1, 2, 1};
    std::string expected_topo = "bond";
    std::vector<std::string> expected_funcform = {"none", "harm", "morse", "quartic"};

    for (size_t i = 0; i < funcform.size(); i++) {
        SECTION(funcform[i]) {
            eff::Bond bond(topo, idxs, funcform[i]);
            bond.SetParameters(linear_params[i], nonlinear_params[i]);
            size_t nlinear = bond.GetNumLinear();
            size_t nnonlinear = bond.GetNumNonLinear();
            std::string topology = bond.GetTopology();
            std::string functional_form = bond.GetFunctionalForm();
            std::vector<size_t> indexes = bond.GetIndexes();
            std::vector<double> lp, nlp;
            bond.GetParameters(lp, nlp);

            REQUIRE(nlinear == expected_num_linear[i]);
            REQUIRE(nnonlinear == expected_num_nonlinear[i]);
            REQUIRE(topology == expected_topo);
            REQUIRE(functional_form == expected_funcform[i]);
            REQUIRE(VectorsAreEqual(indexes, idxs));
            REQUIRE(VectorsAreEqual(lp, linear_params[i], TOL));
            REQUIRE(VectorsAreEqual(nlp, nonlinear_params[i], TOL));
        }
    }
}

TEST_CASE("topology::Setters") {
    std::string topo = "BoNd";
    std::vector<size_t> idxs = {0, 1};
    std::vector<std::string> funcform = {"NONE", "haRm", "Morse", "quartiC"};
    std::vector<std::vector<double> > linear_params = {{}, {2.0}, {2.1}, {10.0, 1.0, 0.1}};
    std::vector<std::vector<double> > nonlinear_params = {{}, {3.0}, {2.5, 5.1}, {1.5}};

    std::vector<size_t> expected_num_linear = {0, 1, 1, 3};
    std::vector<size_t> expected_num_nonlinear = {0, 1, 2, 1};
    std::string expected_topo = "bond";
    std::vector<std::string> expected_funcform = {"none", "harm", "morse", "quartic"};

    for (size_t i = 0; i < funcform.size(); i++) {
        SECTION(funcform[i]) {
            // Create a bon from constructor
            eff::Bond bond2(topo, idxs, funcform[i]);
            bond2.SetParameters(linear_params[i], nonlinear_params[i]);

            // Create empty bond and set everything manually
            eff::Bond bond;
            bond.SetTopology("BoNd");
            bond.SetFunctionalForm(funcform[i]);
            bond.SetIndexes(idxs);
            bond.SetNumLinear(linear_params[i].size());
            bond.SetNumNonLinear(nonlinear_params[i].size());
            bond.SetParameters(linear_params[i], nonlinear_params[i]);

            REQUIRE(bond == bond2);
        }
    }
}
