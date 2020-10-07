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

#include "potential/1b/energy1b.h"
#include "potential/2b/energy2b.h"
#include "potential/3b/energy3b.h"
#include "potential/dispersion/dispersion.h"
#include "setup_h2o_3.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

void GetDimers(const std::vector<double> xyz, const size_t nmon, std::vector<double> &xyz1, std::vector<double> &xyz2,
               size_t &ndimers) {
    ndimers = 0;
    xyz1.clear();
    xyz2.clear();
    for (size_t i = 0; i < nmon - 1; i++) {
        size_t offset_i = 9 * i;
        for (size_t j = i + 1; j < nmon; j++) {
            size_t offset_j = 9 * j;
            for (size_t k = 0; k < 9; k++) {
                xyz1.push_back(xyz[offset_i + k]);
                xyz2.push_back(xyz[offset_j + k]);
            }
            ndimers++;
        }
    }
}

TEST_CASE("Test the n-body terms for mbpol (gas phase).") {
    // MB-pol test
    SETUP_H2O_3
    size_t nmon = 3;
    std::vector<size_t> good;
    std::string monomer_name = "h2o";

    // Obtain coordinates without virtual sites
    std::vector<double> real_xyz(3 * n_atoms, 0.0);
    size_t pos = 0;
    for (size_t i = 0; i < nmon; i++) {
        size_t shift = first_ind[i] * 3;
        std::copy(coords.begin() + shift, coords.begin() + shift + 9, real_xyz.begin() + pos);
        pos += 9;
    }

    SECTION("One body (polynomials)") {
        std::vector<double> grad(3 * n_atoms, 0.0);
        double energy_1b_nograd = e1b::get_1b_energy(monomer_name, nmon, real_xyz, good);
        double energy_1b_grad = e1b::get_1b_energy(monomer_name, nmon, real_xyz, grad, good);

        SECTION("Energy without gradients.") { REQUIRE(energy_1b_nograd == Approx(3.2265445802e-01).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_1b_grad == Approx(3.2265445802e-01).margin(TOL)); }

        SECTION("Numerical gradients vs analitical gradients") {
            size_t atomOffset = 0;
            double stepSize = 0.00001;
            for (size_t molecule = 0; molecule < nmon; ++molecule) {
                for (size_t degreeOfFreedom = atomOffset; degreeOfFreedom < atomOffset + 9; ++degreeOfFreedom) {
                    real_xyz[degreeOfFreedom] += stepSize;
                    double plusEnergy = e1b::get_1b_energy(monomer_name, nmon, real_xyz, good);
                    real_xyz[degreeOfFreedom] -= 2 * stepSize;
                    double minusEnergy = e1b::get_1b_energy(monomer_name, nmon, real_xyz, good);
                    real_xyz[degreeOfFreedom] += stepSize;
                    double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
                    double error = grad[degreeOfFreedom] - finiteDifferenceForce;
                    REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
                }
            }
        }
    }

    SECTION("Two body (polynomials)") {
        // Obtain the dimers in the right format
        // Need 2 xyz vectors paired.
        std::vector<double> xyz1;
        std::vector<double> xyz2;
        std::vector<double> grad(3 * n_atoms);
        size_t ndimers = 0;
        GetDimers(real_xyz, nmon, xyz1, xyz2, ndimers);
        // Declare gradient vectors
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);

        double energy_2b_nograd = e2b::get_2b_energy(monomer_name, monomer_name, ndimers, xyz1, xyz2);
        double energy_2b_grad = e2b::get_2b_energy(monomer_name, monomer_name, ndimers, xyz1, xyz2, grad1, grad2);
        // Add the gradients properly
        size_t offset_dimer = 0;
        for (size_t i = 0; i < nmon - 1; i++) {
            size_t offset_i = 9 * i;
            for (size_t j = i + 1; j < nmon; j++) {
                size_t offset_j = 9 * j;
                for (size_t k = 0; k < 9; k++) {
                    grad[offset_i + k] += grad1[offset_dimer + k];
                    grad[offset_j + k] += grad2[offset_dimer + k];
                }
                offset_dimer += 9;
            }
        }

        SECTION("Energy without gradients.") { REQUIRE(energy_2b_nograd == Approx(1.7011845631e+01).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_2b_grad == Approx(1.7011845631e+01).margin(TOL)); }

        SECTION("Numerical gradients vs analitical gradients") {
            size_t atomOffset = 0;
            double stepSize = 0.0001;
            for (size_t molecule = 0; molecule < nmon; ++molecule) {
                for (size_t degreeOfFreedom = atomOffset; degreeOfFreedom < atomOffset + 9; ++degreeOfFreedom) {
                    real_xyz[degreeOfFreedom] += stepSize;
                    GetDimers(real_xyz, nmon, xyz1, xyz2, ndimers);
                    double plusEnergy = e2b::get_2b_energy(monomer_name, monomer_name, ndimers, xyz1, xyz2);
                    real_xyz[degreeOfFreedom] -= 2 * stepSize;
                    GetDimers(real_xyz, nmon, xyz1, xyz2, ndimers);
                    double minusEnergy = e2b::get_2b_energy(monomer_name, monomer_name, ndimers, xyz1, xyz2);
                    real_xyz[degreeOfFreedom] += stepSize;
                    double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
                    double error = grad[degreeOfFreedom] - finiteDifferenceForce;
                    REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
                }
            }
        }
    }

    SECTION("Two body (dispersion)") {
        std::vector<double> box;
        std::vector<double> grad(3 * n_atoms, 0.0);

        // C6 vector that is useless but needed
        std::vector<double> c6_long_range(n_atoms, 0.0);
        disp::Dispersion dispersion_class;
        dispersion_class.Initialize(c6_long_range, real_xyz, monomer_names, nats, mon_type_count, islocal, false, box);

        std::vector<double> grad_dummy(3 * n_atoms, 0.0);
        dispersion_class.SetNewParameters(real_xyz, false, 100.0, box);
        double energy_2b_nograd = dispersion_class.GetDispersion(grad_dummy);
        dispersion_class.SetNewParameters(real_xyz, true, 100.0, box);
        double energy_2b_grad = dispersion_class.GetDispersion(grad);

        SECTION("Energy without gradients.") { REQUIRE(energy_2b_nograd == Approx(-8.1876933060e+00).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_2b_grad == Approx(-8.1876933060e+00).margin(TOL)); }

        SECTION("Numerical gradients vs analitical gradients") {
            size_t atomOffset = 0;
            double stepSize = 0.00001;
            for (size_t molecule = 0; molecule < nmon; ++molecule) {
                for (size_t degreeOfFreedom = atomOffset; degreeOfFreedom < atomOffset + 9; ++degreeOfFreedom) {
                    real_xyz[degreeOfFreedom] += stepSize;
                    dispersion_class.SetNewParameters(real_xyz, false, 100.0, box);
                    double plusEnergy = dispersion_class.GetDispersion(grad_dummy);

                    real_xyz[degreeOfFreedom] -= 2 * stepSize;
                    dispersion_class.SetNewParameters(real_xyz, false, 100.0, box);
                    double minusEnergy = dispersion_class.GetDispersion(grad_dummy);

                    real_xyz[degreeOfFreedom] += stepSize;
                    double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
                    double error = grad[degreeOfFreedom] - finiteDifferenceForce;
                    REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
                }
            }
        }
    }

    SECTION("Three body (polynomials)") {
        std::vector<double> xyz1(9);
        std::vector<double> xyz2(9);
        std::vector<double> xyz3(9);

        std::vector<double> grad1(9);
        std::vector<double> grad2(9);
        std::vector<double> grad3(9);

        std::copy(real_xyz.begin(), real_xyz.begin() + 9, xyz1.begin());
        std::copy(real_xyz.begin() + 9, real_xyz.begin() + 18, xyz2.begin());
        std::copy(real_xyz.begin() + 18, real_xyz.begin() + 27, xyz3.begin());

        std::vector<double> grad(3 * n_atoms);
        double energy_3b_nograd = e3b::get_3b_energy(monomer_name, monomer_name, monomer_name, 1, xyz1, xyz2, xyz3);
        double energy_3b_grad =
            e3b::get_3b_energy(monomer_name, monomer_name, monomer_name, 1, xyz1, xyz2, xyz3, grad1, grad2, grad3);

        // Add the gradients properly
        std::copy(grad1.begin(), grad1.end(), grad.begin());
        std::copy(grad2.begin(), grad2.end(), grad.begin() + 9);
        std::copy(grad3.begin(), grad3.end(), grad.begin() + 18);

        SECTION("Energy without gradients.") { REQUIRE(energy_3b_nograd == Approx(2.2777705108e-01).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_3b_grad == Approx(2.2777705108e-01).margin(TOL)); }

        SECTION("Numerical gradients vs analitical gradients") {
            size_t atomOffset = 0;
            double stepSize = 0.00001;
            for (size_t molecule = 0; molecule < nmon; ++molecule) {
                for (size_t degreeOfFreedom = atomOffset; degreeOfFreedom < atomOffset + 9; ++degreeOfFreedom) {
                    real_xyz[degreeOfFreedom] += stepSize;
                    std::copy(real_xyz.begin(), real_xyz.begin() + 9, xyz1.begin());
                    std::copy(real_xyz.begin() + 9, real_xyz.begin() + 18, xyz2.begin());
                    std::copy(real_xyz.begin() + 18, real_xyz.begin() + 27, xyz3.begin());
                    double plusEnergy =
                        e3b::get_3b_energy(monomer_name, monomer_name, monomer_name, 1, xyz1, xyz2, xyz3);
                    real_xyz[degreeOfFreedom] -= 2 * stepSize;
                    std::copy(real_xyz.begin(), real_xyz.begin() + 9, xyz1.begin());
                    std::copy(real_xyz.begin() + 9, real_xyz.begin() + 18, xyz2.begin());
                    std::copy(real_xyz.begin() + 18, real_xyz.begin() + 27, xyz3.begin());
                    double minusEnergy =
                        e3b::get_3b_energy(monomer_name, monomer_name, monomer_name, 1, xyz1, xyz2, xyz3);
                    real_xyz[degreeOfFreedom] += stepSize;
                    double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
                    double error = grad[degreeOfFreedom] - finiteDifferenceForce;
                    REQUIRE(grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
                }
            }
        }
    }
}
