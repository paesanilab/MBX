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

#include "bblock/system.h"
#include "setup_h2o_256_pbc.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

constexpr double TOL = 1E-4;
const size_t NPOINTS = 20;

TEST_CASE("Test MB-pol One-body gradients finite differences") {
    srand(time(0));
    // MB-pol test
    SETUP_H2O_256_PBC

    bblock::System my_sys;

    // Initialize system
    for (size_t i = 0; i < n_monomers; i++) {
        std::vector<double> xyz(coords.begin() + 3 * n_at * i, coords.begin() + 3 * n_at * (i + 1));
        std::vector<std::string> ats(atom_names.begin() + i * n_at, atom_names.begin() + (i + 1) * n_at);
        my_sys.AddMonomer(xyz, ats, monomer_names[i]);
    }

    my_sys.Initialize();

    // Set PBC features
    my_sys.SetPBC(box);
    my_sys.SetDipoleMethod("cg");
    my_sys.Set2bCutoff(9.0);
    my_sys.SetEwald(0.4, 1.5, 6);

    size_t n_atoms = my_sys.GetNumRealSites();
    std::vector<double> real_xyz = my_sys.GetRealXyz();

    SECTION("One body (polynomials)") {
        std::vector<double> grad;
        double energy_nograd = my_sys.Dispersion(false);
        double energy_grad = my_sys.Dispersion(true);

        grad = my_sys.GetRealGrads();

        SECTION("Compare energy with and without gradients") {
            REQUIRE(energy_nograd == Approx(energy_grad).margin(TOL));
        }

        SECTION("Numerical gradients vs analitical gradients") {
            size_t atomOffset = 0;
            double stepSize = 0.001;
            for (size_t i = 0; i < NPOINTS; ++i) {
                size_t degreeOfFreedom = (rand() % (3 * n_atoms));
                real_xyz[degreeOfFreedom] += stepSize;
                my_sys.SetRealXyz(real_xyz);
                double plusEnergy = my_sys.Dispersion(false);

                real_xyz[degreeOfFreedom] -= 2 * stepSize;
                my_sys.SetRealXyz(real_xyz);
                double minusEnergy = my_sys.Dispersion(false);

                real_xyz[degreeOfFreedom] += stepSize;
                my_sys.SetRealXyz(real_xyz);

                double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
                double error = grad[degreeOfFreedom] - finiteDifferenceForce;
                std::cout << std::scientific << std::setw(4) << degreeOfFreedom << std::setw(20)
                          << grad[degreeOfFreedom] << std::setw(6) << "<==>" << std::setw(20) << finiteDifferenceForce
                          << std::endl;
                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }
}
