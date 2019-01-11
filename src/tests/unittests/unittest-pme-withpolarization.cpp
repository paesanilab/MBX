#include "catch.hpp"

#include "electrostatics.h"
#include "setupwaterbox216.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1E5;  // ACS CHANGE THIS WHEN DEBUGGED!

TEST_CASE("test the electrostatics class for only coulomb terms (PME) - ewald alpha sweep.") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 1.310;
    double polfacH = 0.294;
    double polfacM = 0;
    polfacO = polfacH = 0;
    SETUP_WATERBOX_216
    double ref_energy = 11034.8000795751;

    elec::Electrostatics elec;
    std::vector<double> box_vectors{30, 0, 0, 0, 30, 0, 0, 0, 30};

    const char *method = "iter";

    /*
     * Ensure that computed properties are invariant to changes in the Ewald attenuation parameter
     */
    // alpha = 0.3
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, method, box_vectors);
    elec.SetCutoff(14.5);
    elec.SetEwaldAlpha(0.25);
    elec.SetEwaldGridDensity(1);
    elec.SetEwaldSplineOrder(6);
    std::vector<double> forces3(3 * n_atoms);
    double energy3 = elec.GetElectrostatics(forces3);
    //    REQUIRE(energy3 == Approx(ref_energy).margin(TOL));

    // alpha = 0.4
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, method, box_vectors);
    elec.SetCutoff(10);
    elec.SetEwaldAlpha(0.35);
    elec.SetEwaldGridDensity(1.5);
    elec.SetEwaldSplineOrder(6);
    std::vector<double> forces4(3 * n_atoms);
    double energy4 = elec.GetElectrostatics(forces4);
    //    REQUIRE(energy4 == Approx(energy3).margin(TOL));
    //    for(int n = 0; n < 3*n_atoms; ++n) REQUIRE(forces3[n] == Approx(forces4[n]).margin(TOL));

    // alpha = 0.5
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, method, box_vectors);
    elec.SetCutoff(8);
    elec.SetEwaldAlpha(0.45);
    elec.SetEwaldGridDensity(2);
    elec.SetEwaldSplineOrder(7);
    std::vector<double> forces5(3 * n_atoms);
    double energy5 = elec.GetElectrostatics(forces5);
    //    REQUIRE(energy5 == Approx(energy4).margin(TOL));
    //    for(int n = 0; n < 3*n_atoms; ++n) REQUIRE(forces4[n] == Approx(forces5[n]).margin(TOL));

    std::cout << "Energies:" << std::endl;
    std::cout << "alpha = 0.3: " << std::setw(16) << std::setprecision(10) << energy3 << std::endl;
    std::cout << "alpha = 0.4: " << std::setw(16) << std::setprecision(10) << energy4 << std::endl;
    std::cout << "alpha = 0.5: " << std::setw(16) << std::setprecision(10) << energy5 << std::endl;
    std::cout << "\nForces:" << std::endl;
    std::cout << "         alpha = 0.3     alpha = 0.4     alpha = 0.5" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << i << "x  " << std::setw(16) << std::setprecision(10) << forces3[3 * i + 0];
        std::cout << std::setw(16) << std::setprecision(10) << forces4[3 * i + 0];
        std::cout << std::setw(16) << std::setprecision(10) << forces5[3 * i + 0] << std::endl;
        std::cout << i << "y  " << std::setw(16) << std::setprecision(10) << forces3[3 * i + 1];
        std::cout << std::setw(16) << std::setprecision(10) << forces4[3 * i + 1];
        std::cout << std::setw(16) << std::setprecision(10) << forces5[3 * i + 1] << std::endl;
        std::cout << i << "z  " << std::setw(16) << std::setprecision(10) << forces3[3 * i + 2];
        std::cout << std::setw(16) << std::setprecision(10) << forces4[3 * i + 2];
        std::cout << std::setw(16) << std::setprecision(10) << forces5[3 * i + 2] << std::endl;
    }
}
