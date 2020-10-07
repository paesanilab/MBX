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

#include "potential/dispersion/dispersion.h"
#include "setup_h2o_2_mbpol.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;
constexpr double CUTOFF = 10.0;

TEST_CASE("Test the dispersion class.") {
    // Create the bromide -- water system
    SETUP_H2O_2_MBPOL

    std::vector<double> box2 = {1000.0, 0.0, 0.0, 0.0, 1000.0, 0.0, 0.0, 0.0, 1000.0};
    box = box2;

    // create dispersion class
    disp::Dispersion my_disp;
    my_disp.Initialize(C6_long_range, real_coords, monomer_names, n_atoms_vector, internal_mon_type_count, islocal,
                       false, box);

    SECTION("Switch is 1") {
        // Get dispersion with no grads
        std::vector<double> dummy(3 * n_atoms, 0.0);
        my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
        double energy_nograd = my_disp.GetDispersion(dummy);

        // Set do_grads to true
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box);
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);
                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Switch is 0") {
        // Get dispersion with no grads
        std::vector<double> dummy(3 * n_atoms, 0.0);
        real_coords[11] = CUTOFF + 1.0;
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box);
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Switch is between 0 and 1") {
        // Get dispersion with no grads
        std::vector<double> dummy(3 * n_atoms, 0.0);
        real_coords[11] = CUTOFF - 0.5;
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box);
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }
}
