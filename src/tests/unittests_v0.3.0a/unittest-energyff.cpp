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

#include "potential/force_field/energyff.h"
#include "potential/force_field/connectivity.h"

#include "setup_linker.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

constexpr double TOL = 1E-6;

TEST_CASE("energyff::get_ff_energy(no grad)") {
    SETUP_LINKER

    std::vector<double> box, box_inv;
    // Create bond vector
    std::vector<eff::Bond> bonds(bon_idxs.size());
    for (size_t i = 0; i < bon_idxs.size(); i++) {
        bonds[i] = eff::Bond("bond", bon_idxs[i], bon_func_form);
        bonds[i].SetParameters(l_bonds[i], nl_bonds[i]);
    }

    // Create angle vector
    std::vector<eff::Angles> angles(ang_idxs.size());
    for (size_t i = 0; i < ang_idxs.size(); i++) {
        angles[i] = eff::Angles("angle", ang_idxs[i], ang_func_form);
        angles[i].SetParameters(l_angles[i], nl_angles[i]);
    }

    // Create dihedral vector
    std::vector<eff::Dihedral> dhs(dih_idxs.size());
    for (size_t i = 0; i < dih_idxs.size(); i++) {
        dhs[i] = eff::Dihedral("dihedral", dih_idxs[i], dih_func_form);
        dhs[i].SetParameters(l_dhs[i], nl_dhs[i]);
    }

    // Create inversion vector (empty)
    std::vector<eff::Inversion> invs;

    // Generate connectivity object
    std::string monid = "linker";
    eff::Conn connectivity(monid, bonds, angles, dhs, invs);

    SECTION("Normal behavior") {
        bool good = true;
        double e_nograd = eff::get_ff_energy(connectivity, 1, coords, good, n_atoms, box, box_inv);
        double e_nograd_expected = 1.7422580421e+01;
        REQUIRE(e_nograd == Approx(e_nograd_expected).margin(TOL));

        std::vector<double> virial(9, 0.0);
        std::vector<double> grad(coords.size(), 0.0);

        std::vector<double> expected_virial = {};
        std::vector<double> expected_grad = {};
        double e_grad = eff::get_ff_energy(connectivity, 1, coords, grad, good, n_atoms, box, box_inv, &virial);

        // Numerical gradients
        double s = 0.0001;
        for (size_t j = 0; j < coords.size(); j++) {
            coords[j] += s;
            double ep = eff::get_ff_energy(connectivity, 1, coords, good, n_atoms, box, box_inv);
            coords[j] += s;
            double epp = eff::get_ff_energy(connectivity, 1, coords, good, n_atoms, box, box_inv);
            coords[j] -= 4 * s;
            double emm = eff::get_ff_energy(connectivity, 1, coords, good, n_atoms, box, box_inv);
            coords[j] += s;
            double em = eff::get_ff_energy(connectivity, 1, coords, good, n_atoms, box, box_inv);
            coords[j] += s;

            double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
            REQUIRE(numgrad == Approx(grad[j]).margin(TOL));
        }
    }
}
