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
    // Create the system
    SETUP_H2O_2_MBPOL

    std::vector<double> box2 = {30.0, 0.0, 0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 30.0};

    SECTION("Check continuity of the energy across switching region and cutoff") {
        disp::Dispersion my_disp;
        double alpha = 0.6;
        double grid_density = 1.5;
        int spline_order = 4;
        std::vector<double> dummy(3 * n_atoms, 0.0);
        double start = 8.5;
        double delta = 0.01;

        std::vector<double> original_coord = real_coords;
        //        double original_coord = real_coords[11];
        my_disp.Initialize(C6_long_range, real_coords, monomer_names, n_atoms_vector, internal_mon_type_count, islocal,
                           false, box2);
        for (size_t i = 3; i < 6; i++) {
            for (size_t j = 0; j < 3; j++) {
                real_coords[3 * i + j] = original_coord[3 * i + j] - original_coord[j];
            }
            real_coords[3 * i] += start;
        }
        //        real_coords[9] = start;
        my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
        my_disp.SetEwaldAlpha(alpha);
        my_disp.SetEwaldGridDensity(grid_density);
        my_disp.SetEwaldSplineOrder(spline_order);
        double old_energy = my_disp.GetDispersion(dummy);
        for (double coord = start + delta; coord < CUTOFF + 0.2; coord += delta) {
            //            //real_coords[11] = coord;
            for (size_t i = 3; i < 6; i++) {
                for (size_t j = 0; j < 3; j++) {
                    real_coords[3 * i + j] = original_coord[3 * i + j] - original_coord[j];
                }
                real_coords[3 * i] += coord;
            }
            my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
            my_disp.SetEwaldAlpha(alpha);
            my_disp.SetEwaldGridDensity(grid_density);
            my_disp.SetEwaldSplineOrder(spline_order);
            double energy = my_disp.GetDispersion(dummy);
            double dE = energy - old_energy;
            std::cout << std::setw(16) << std::setprecision(10) << coord << ",  " << std::setw(16)
                      << std::setprecision(10) << energy << ",  " << std::setw(16) << std::setprecision(10) << dE
                      << std::endl;
            REQUIRE(dE == Approx(0).margin(2e-4));
            old_energy = energy;
        }
    }

    SECTION("Check energies and gradients are invariant to the Ewald alpha value") {
        disp::Dispersion ewald_disp;

        std::vector<double> forces3(3 * n_atoms, 0.0);
        ewald_disp.Initialize(C6_long_range, real_coords, monomer_names, n_atoms_vector, internal_mon_type_count,
                              islocal, true, box2);
        ewald_disp.SetNewParameters(real_coords, true, CUTOFF, box2);
        ewald_disp.SetEwaldAlpha(0.3);
        ewald_disp.SetEwaldGridDensity(2);
        ewald_disp.SetEwaldSplineOrder(6);
        double energy3 = ewald_disp.GetDispersion(forces3);
        std::cout << energy3 << std::endl;

        std::vector<double> forces4(3 * n_atoms, 0.0);
        ewald_disp.SetNewParameters(real_coords, true, CUTOFF, box2);
        ewald_disp.SetEwaldAlpha(0.4);
        ewald_disp.SetEwaldGridDensity(3);
        ewald_disp.SetEwaldSplineOrder(7);
        double energy4 = ewald_disp.GetDispersion(forces4);
        REQUIRE(energy3 == Approx(energy4).margin(TOL));

        std::vector<double> forces5(3 * n_atoms, 0.0);
        ewald_disp.SetNewParameters(real_coords, true, CUTOFF, box2);
        ewald_disp.SetEwaldAlpha(0.5);
        ewald_disp.SetEwaldGridDensity(4);
        ewald_disp.SetEwaldSplineOrder(8);
        double energy5 = ewald_disp.GetDispersion(forces5);
        REQUIRE(energy4 == Approx(energy5).margin(TOL));

        for (int degreeOfFreedom = 0; degreeOfFreedom < 3 * n_atoms; ++degreeOfFreedom) {
            REQUIRE(forces3[degreeOfFreedom] == Approx(forces4[degreeOfFreedom]).margin(TOL));
            REQUIRE(forces4[degreeOfFreedom] == Approx(forces5[degreeOfFreedom]).margin(TOL));
        }
    }

    SECTION("Switch is 1") {
        disp::Dispersion my_disp;
        my_disp.Initialize(C6_long_range, real_coords, monomer_names, n_atoms_vector, internal_mon_type_count, islocal,
                           false, box);
        // Get dispersion with no grads
        std::vector<double> dummy(3 * n_atoms, 0.0);
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box2);
        my_disp.SetEwaldAlpha(0.3);
        my_disp.SetEwaldGridDensity(2);
        my_disp.SetEwaldSplineOrder(6);
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Switch is 0") {
        disp::Dispersion my_disp;
        my_disp.Initialize(C6_long_range, real_coords, monomer_names, n_atoms_vector, internal_mon_type_count, islocal,
                           false, box);
        // Get dispersion with no grads
        std::vector<double> dummy(3 * n_atoms, 0.0);
        double original_coord = real_coords[11];
        real_coords[11] = CUTOFF + 1.0;
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box2);
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
        real_coords[11] = original_coord;
    }

    SECTION("Switch is between 0 and 1") {
        disp::Dispersion my_disp;
        my_disp.Initialize(C6_long_range, real_coords, monomer_names, n_atoms_vector, internal_mon_type_count, islocal,
                           false, box);
        // Get dispersion with no grads
        std::vector<double> dummy(3 * n_atoms, 0.0);
        double original_coord = real_coords[11];
        real_coords[11] = CUTOFF - 0.5;
        my_disp.SetNewParameters(real_coords, true, CUTOFF, box2);
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_grad = my_disp.GetDispersion(grad);

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < real_coords.size(); ++degreeOfFreedom) {
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double plusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double plusplusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] -= 4 * stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double minusminusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double minusEnergy = my_disp.GetDispersion(dummy);
                real_coords[degreeOfFreedom] += stepSize;
                my_disp.SetNewParameters(real_coords, false, CUTOFF, box2);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
        real_coords[11] = original_coord;
    }
}
