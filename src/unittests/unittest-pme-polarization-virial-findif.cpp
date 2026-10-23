#include "Catch2/single_include/catch.hpp"

#include "potential/electrostatics/electrostatics.h"
#include "setup_h2o_216.h"

#include <array>
#include <cmath>
#include <vector>

TEST_CASE("polarized PME forces and off-diagonal virial match finite differences for 216 waters") {
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 1.310;
    double polfacH = 0.294;
    double polfacM = 0;
    SETUP_H2O_216

    const std::vector<double> box{34, 0, 0, 0, 34, 0, 0, 0, 34};
    const double step = 1e-5;

    auto energy_at = [&](const std::vector<double>& positions, const std::vector<double>& cell,
                         bool calculate_gradients, std::vector<double>& gradients,
                         std::vector<double>* virial = nullptr) {
        elec::Electrostatics electrostatics;
        electrostatics.Initialize(charges, chg_grad, polfac, pol, positions, monomer_names, sites, first_ind,
                                  mon_type_count, islocal, sys_at_tag, calculate_gradients, 1e-16, 100, "iter", cell);
        electrostatics.SetCutoff(16.5);
        electrostatics.SetEwaldAlpha(0.25);
        electrostatics.SetEwaldGridDensity(2.5);
        electrostatics.SetEwaldSplineOrder(6);
        // Keep the PME grid fixed while the cell is strained.
        electrostatics.SetFFTDimension({88, 88, 88});
        return electrostatics.GetElectrostatics(gradients, virial);
    };

    std::vector<double> gradients(3 * n_atoms, 0.0);
    std::vector<double> virial(9, 0.0);
    const double energy = energy_at(coords, box, true, gradients, &virial);
    REQUIRE(std::isfinite(energy));
    REQUIRE(energy == Approx(-2531.4416733178).epsilon(5e-5));

    std::vector<double> scratch(3 * n_atoms, 0.0);

    SECTION("sampled Cartesian forces") {
        // Sample two molecules rather than all 2592 coordinates to keep the test practical.
        for (int dof : {0, 1, 2, 12, 13, 14}) {
            std::vector<double> plus = coords;
            std::vector<double> minus = coords;
            plus[dof] += step;
            minus[dof] -= step;
            const double numerical_force =
                -(energy_at(plus, box, false, scratch) - energy_at(minus, box, false, scratch)) / (2 * step);
            const double analytic_force = -gradients[dof];
            INFO("coordinate " << dof << ": analytic force " << analytic_force << ", finite difference "
                               << numerical_force);
            CHECK(analytic_force == Approx(numerical_force).margin(1e-4));
        }
    }

    // For each independent shear, strain the cell and every site together.
    // MBX's virial convention is minus the energy derivative with respect to strain.
    auto finite_difference_virial = [&](int i, int j) {
        auto strained_energy = [&](double strain) {
            std::vector<double> strained_coords = coords;
            std::vector<double> strained_box = box;
            for (int atom = 0; atom < n_atoms; ++atom) {
                strained_coords[3 * atom + i] += strain * coords[3 * atom + j];
            }
            for (int vector = 0; vector < 3; ++vector) {
                strained_box[3 * vector + i] += strain * box[3 * vector + j];
            }
            return energy_at(strained_coords, strained_box, false, scratch);
        };
        return -(strained_energy(step) - strained_energy(-step)) / (2 * step);
    };

    const std::array<std::array<int, 2>, 3> shears{{{{0, 1}}, {{0, 2}}, {{1, 2}}}};
    SECTION("off-diagonal virial from affine cell shear") {
        for (const auto& pair : shears) {
            const int i = pair[0];
            const int j = pair[1];
            const double numerical_virial = finite_difference_virial(i, j);
            INFO("virial " << i << j << ": analytic " << virial[3 * i + j] << ", finite difference "
                           << numerical_virial);
            CHECK(virial[3 * i + j] == Approx(numerical_virial).margin(1e-2));
        }
    }
}
