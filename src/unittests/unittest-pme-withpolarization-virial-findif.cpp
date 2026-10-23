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

#include "Catch2/single_include/catch.hpp"

#include "bblock/system.h"
#include "setup_h2o_216.h"

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

constexpr double TOL = 1e-5;

TEST_CASE("216-water PME with polarization: forces and virial match finite differences") {
    // Let System construct the MB-pol water sites and evaluate the complete force field.
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 1.310;
    double polfacH = 0.294;
    double polfacM = 0;
    SETUP_H2O_216

    const std::vector<double> box{34, 0, 0, 0, 34, 0, 0, 0, 34};
    const double step = 1e-6;
    const std::array<std::array<int, 2>, 6> strains{{{{0, 0}}, {{1, 1}}, {{2, 2}},
                                                     {{0, 1}}, {{0, 2}}, {{1, 2}}}};

    auto energy_at = [&](const std::vector<double>& positions, const std::vector<double>& cell, double alpha,
                         bool calculate_gradients, std::vector<double>* virial = nullptr,
                         std::vector<double>* gradients = nullptr) {
        bblock::System system;
        for (int monomer = 0; monomer < n_monomers; ++monomer) {
            const auto first = positions.begin() + 12 * monomer;
            std::vector<double> real_sites(first, first + 9);
            system.AddMonomer(real_sites, {"O", "H", "H"}, "h2o");
        }
        system.Initialize();

        // varying alpha isolates electrostatic PME.
        nlohmann::json settings = {{"MBX", {{"box", cell},
                                             {"twobody_cutoff", 9.0},
                                             {"threebody_cutoff", 7.0},
                                             {"realspace_cutoff", 8.5},
                                             {"dipole_tolerance", 1e-20},
                                             {"dipole_method", "cg"},
                                             {"alpha_ewald_elec", alpha},
                                             {"alpha_ewald_disp", 0.6},
                                             {"grid_density_elec", 2.5},
                                             {"spline_order_elec", 6},
                                             {"grid_fftdim_elec", {88, 88, 88}},
                                             {"grid_density_disp", 2.5},
                                             {"spline_order_disp", 6},
                                             {"grid_fftdim_disp", {88, 88, 88}}}}};
        system.SetUpFromJson(settings);
        const double energy = system.Energy(calculate_gradients);
        if (virial != nullptr) *virial = system.GetVirial();
        if (gradients != nullptr) *gradients = system.GetRealGrads();
        return energy;
    };

    for (double alpha : {0.0, 0.25}) {
        DYNAMIC_SECTION("electrostatic Ewald alpha = " << alpha) {
            std::cout << "\nElectrostatic Ewald alpha = " << alpha
                      << (alpha == 0.0 ? " (electrostatic PME off)" : " (electrostatic PME on)")
                      << "; dispersion PME on" << std::endl;
            std::vector<double> virial;
            std::vector<double> gradients;
            REQUIRE(std::isfinite(energy_at(coords, box, alpha, true, &virial, &gradients)));
            REQUIRE(virial.size() == 9);
            REQUIRE(gradients.size() == 3 * 3 * n_monomers);

            // Sample two water oxygens.
            for (int dof : {0, 1, 2, 12, 13, 14}) {
                std::vector<double> plus = coords;
                std::vector<double> minus = coords;
                plus[dof] += step;
                minus[dof] -= step;
                const double finite_difference =
                    -(energy_at(plus, box, alpha, false) - energy_at(minus, box, alpha, false)) / (2 * step);
                const int real_dof = 9 * (dof / 12) + dof % 3;
                const double analytic = -gradients[real_dof];
                INFO("alpha " << alpha << ", coordinate " << dof << ": analytic force " << analytic
                               << ", finite difference " << finite_difference);
                CHECK(analytic == Approx(finite_difference).epsilon(TOL));
            }

            for (const auto& strain_component : strains) {
                const int a = strain_component[0];
                const int b = strain_component[1];
                auto strained_energy = [&](double strain) {
                    std::vector<double> strained_positions = coords;
                    std::vector<double> strained_box = box;
                    for (int atom = 0; atom < n_atoms; ++atom) {
                        strained_positions[3 * atom + a] += strain * coords[3 * atom + b];
                    }
                    for (int vector = 0; vector < 3; ++vector) {
                        strained_box[3 * vector + a] += strain * box[3 * vector + b];
                    }
                    return energy_at(strained_positions, strained_box, alpha, false);
                };
                const double finite_difference =
                    -(strained_energy(step) - strained_energy(-step)) / (2 * step);
                const double analytic = virial[3 * b + a];
                INFO("alpha " << alpha << ", strain " << a << b << ": analytic " << analytic
                               << ", finite difference " << finite_difference);
                CHECK(analytic == Approx(finite_difference).epsilon(TOL));
            }
        }
    }
}
