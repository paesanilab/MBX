#include "catch.hpp"

#include "dispersion2b.h"
#include "setupwaterbox2.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 5E-6;

TEST_CASE("test the two body dispersion code (GAS) - finite differences.") {
    double ref_energy = -0.162541;

    const int natoms = 6;
    std::vector<double> coords = std::vector<double>{                                             
        -11.5109336618, 1.0563960610,  6.6033301068,  0.8781403410,  5.7020569050,  
        -12.2938264730, 7.1259281936,  -11.6417034996, 1.0063049360,
        -11.5501833807, -1.5847863244, -1.5602148182,  -0.8169061174, -1.9286305994, 
        -11.2385781621, -2.0926390799, -11.4605158991, -1.5472825089};
    std::vector<double> box;
    std::vector<double> grd1(3*natoms, 0);
    std::vector<double> grd2(3*natoms, 0);
    int spline_order = 6;
    const double cutoff = 12;

    disp::GetDispersion("h2o", "h2o", 2, true, coords, coords, grd1, grd2, 12, true, 0, box);
            #if 0
    elec.SetCutoff(12);
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);
    std::cout << "Energy: " << energy << std::endl;
    REQUIRE(energy == Approx(ref_energy).epsilon(TOL));

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

        REQUIRE(forces[degreeOfFreedom] == Approx(finiteDifferenceForce).epsilon(TOL));
    }
    #endif
}
