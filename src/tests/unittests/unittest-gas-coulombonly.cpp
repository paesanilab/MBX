#include "testutils.h"

#include "electrostatics.h"
#include "setupwaterbox216.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

TEST_CASE("test the electrostatics class for coulomb terms (gas phase).") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 0;
    double polfacH = 0;
    double polfacM = 0;
    SETUP_WATERBOX_216
    elec::Electrostatics elec;
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", std::vector<double>{});
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);

    REQUIRE(energy == Approx(-1814.7158675138).margin(TOL));

    SECTION("finite difference test of the forces") {
        // Check the forces on 5 random water molecules (there are 216 of them)
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, 215);
        for (int molecule = 0; molecule < 5; ++molecule) {
            int moleculeNumber = distribution(generator);
            int atomOffset = 9 * moleculeNumber;
            double stepSize = 0.00001;
            std::vector<double> ignoredForces(3 * n_atoms);
            for (int degreeOfFreedom = atomOffset; degreeOfFreedom < atomOffset + 9; ++degreeOfFreedom) {
                coords[degreeOfFreedom] += stepSize;
                elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count,
                                false, 1E-16, 100, "iter", std::vector<double>{});
                double plusEnergy = elec.GetElectrostatics(ignoredForces);
                coords[degreeOfFreedom] -= 2 * stepSize;
                elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count,
                                false, 1E-16, 100, "iter", std::vector<double>{});
                double minusEnergy = elec.GetElectrostatics(ignoredForces);
                coords[degreeOfFreedom] += stepSize;
                double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
                double error = forces[degreeOfFreedom] - finiteDifferenceForce;
                REQUIRE(forces[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }
}
