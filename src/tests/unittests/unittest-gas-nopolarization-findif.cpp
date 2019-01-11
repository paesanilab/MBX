#include "catch.hpp"

#include "electrostatics.h"
#include "setupwaterbox2.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1E-5;

TEST_CASE("test the electrostatics class for only coulomb terms (GAS) - finite differences.") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 0;
    double polfacH = 0;
    double polfacM = 0;
    SETUP_WATERBOX_2
    double ref_energy = -0.162541;

    elec::Electrostatics elec;
    std::vector<double> box_vectors;

    int spline_order = 6;
    const char *method = "cg";
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, method, box_vectors);
    elec.SetCutoff(12);
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);
    std::cout << "Energy: " << energy << std::endl;
    REQUIRE(energy == Approx(ref_energy).margin(TOL));

    double stepSize = 0.00001;
    const std::vector<std::string> labels = {"x", "y", "z"};
    std::vector<double> ignoredForces(3 * n_atoms);
    std::cout << " DoF      Analytic         Numerical       Difference" << std::endl;
    for (int degreeOfFreedom = 0; degreeOfFreedom < 3 * n_atoms; ++degreeOfFreedom) {
        coords[degreeOfFreedom] += stepSize;
        elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, false,
                        1E-16, 100, method, box_vectors);
        double plusEnergy = elec.GetElectrostatics(ignoredForces);
        coords[degreeOfFreedom] -= 2 * stepSize;
        elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, false,
                        1E-16, 100, method, box_vectors);
        double minusEnergy = elec.GetElectrostatics(ignoredForces);
        coords[degreeOfFreedom] += stepSize;
        double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
        double error = forces[degreeOfFreedom] - finiteDifferenceForce;
        int atom = degreeOfFreedom / 3;
        int xyz = degreeOfFreedom % 3;
        std::cout << std::setw(3) << atom + 1 << labels[xyz] << std::setprecision(10) << std::setw(16)
                  << forces[degreeOfFreedom] << "  " << std::setprecision(10) << std::setw(16) << finiteDifferenceForce
                  << "  " << std::setprecision(10) << std::setw(16) << forces[degreeOfFreedom] - finiteDifferenceForce;
        if (std::abs(forces[degreeOfFreedom] - finiteDifferenceForce) > TOL) std::cout << " <---- BAD!";
        std::cout << std::endl;

        REQUIRE(forces[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
    }
}
