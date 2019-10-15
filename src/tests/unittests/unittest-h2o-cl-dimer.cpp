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

#include "bblock/system.h"
#include "setup_h2o_1_cl_1.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test water -- chloride energy terms") {
    // Create the bromide -- water system
    SETUP_H2O_1_CL_1

    // Now we create a system that will be the same as the one read
    bblock::System my_system;

    // Add monomers to the system
    size_t count = 0;
    for (size_t i = 0; i < n_monomers; i++) {
        std::vector<double> xyz(real_coords.begin() + 3 * count,
                                real_coords.begin() + 3 * count + 3 * n_atoms_vector[i]);
        std::vector<std::string> ats(atom_names.begin() + count, atom_names.begin() + count + n_atoms_vector[i]);
        std::string monid = monomer_names[i];
        my_system.AddMonomer(xyz, ats, monid);
        count += n_atoms_vector[i];
    }

    // Initialize the system to fill in the information
    my_system.Initialize();

    SECTION("One-Body") {
        double energy_nograd = my_system.OneBodyEnergy(false);
        double energy_grad = my_system.OneBodyEnergy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(one_body_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(one_body_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 0.00001;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);
                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Two-Body") {
        double energy_nograd = my_system.TwoBodyEnergy(false);
        double energy_grad = my_system.TwoBodyEnergy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(two_body_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(two_body_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Three-Body") {
        double energy_nograd = my_system.ThreeBodyEnergy(false);
        double energy_grad = my_system.ThreeBodyEnergy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(three_body_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(three_body_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Dispersion") {
        double energy_nograd = my_system.Dispersion(false);
        double energy_grad = my_system.Dispersion(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Electrostatics") {
        double energy_nograd = my_system.Electrostatics(false);
        double energy_grad = my_system.Electrostatics(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(electrostatic_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(electrostatic_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Total Energy (MB-nrg)") {
        double energy_nograd = my_system.Energy(false);
        double energy_grad = my_system.Energy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(total_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(total_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    // Set the pair to be calculated as buckingham
    my_system.Set2bIgnorePoly(ignore_2b);
    my_system.Set3bIgnorePoly(ignore_3b);
    my_system.SetTTMnrgPairs(ttm_pairs);

    SECTION("Buckingham") {
        double energy_nograd = my_system.Buckingham(false);
        double energy_grad = my_system.Buckingham(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(buckingham_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(buckingham_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Buckingham(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Buckingham(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Buckingham(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Buckingham(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Total Energy (TTM-nrg)") {
        double energy_nograd = my_system.Energy(false);
        double energy_grad = my_system.Energy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(total_energy_ttm).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(total_energy_ttm).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }
}
