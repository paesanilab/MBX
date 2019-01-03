#include "catch.hpp"

#include "electrostatics.h"
#include "setupwaterbox216.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1E-4;

TEST_CASE("test the electrostatics class for coulomb terms (PME).") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 1.310;
    double polfacH = 0.294;
    double polfacM = 0;
//    polfacO = polfacH = 0;
    SETUP_WATERBOX_216
    double ref_energy = -1824.323;

    elec::Electrostatics elec;
    std::vector<double> box_vectors{30, 0, 0, 0, 30, 0, 0, 0, 30};

    /*
     * Ensure that computed properties are invariant to changes in the Ewald attenuation parameter
     */

    // alpha = 0.3
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", box_vectors);
    elec.SetCutoff(14.8);
    elec.SetEwaldAlpha(0.3);
    elec.SetEwaldGridDensity(2.5);
    elec.SetEwaldSplineOrder(6);
    std::vector<double> forces3(3 * n_atoms);
    double energy3 = elec.GetElectrostatics(forces3);
    //REQUIRE(energy3 == Approx(ref_energy).margin(TOL));

    // alpha = 0.4
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", box_vectors);
    elec.SetCutoff(10);
    elec.SetEwaldAlpha(0.4);
    elec.SetEwaldGridDensity(2.5);
    elec.SetEwaldSplineOrder(7);
    std::vector<double> forces4(3 * n_atoms);
    double energy4 = elec.GetElectrostatics(forces4);
    //REQUIRE(energy4 == Approx(energy3).margin(TOL));
    //for(int n = 0; n < 3*n_atoms; ++n) REQUIRE(forces3[n] == Approx(forces4[n]).margin(TOL));

    // alpha = 0.5
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", box_vectors);
    elec.SetCutoff(8);
    elec.SetEwaldAlpha(0.5);
    elec.SetEwaldGridDensity(3.5);
    elec.SetEwaldSplineOrder(7);
    std::vector<double> forces5(3 * n_atoms);
    double energy5 = elec.GetElectrostatics(forces5);
    //REQUIRE(energy5 == Approx(energy4).margin(TOL));
    //for(int n = 0; n < 3*n_atoms; ++n) REQUIRE(forces4[n] == Approx(forces5[n]).margin(TOL));
    std::cout << std::setprecision(10) << energy3 << std::endl;
    std::cout << std::setprecision(10) << energy4 << std::endl;
    std::cout << std::setprecision(10) << energy5 << std::endl;
    for(int i = 0; i < 6; ++i){
        std::cout << 3*i+0 << "  "
                  << std::setprecision(10) << forces3[3*i+0];
        std::cout << std::setprecision(10) << forces4[3*i+0];
        std::cout << std::setprecision(10) << forces5[3*i+0] << std::endl;
        std::cout << 3*i+1 << "  "
                  << std::setprecision(10) << forces3[3*i+1];
        std::cout << std::setprecision(10) << forces4[3*i+1];
        std::cout << std::setprecision(10) << forces5[3*i+1] << std::endl;
        std::cout << 3*i+2 << "  "
                  << std::setprecision(10) << forces3[3*i+2];
        std::cout << std::setprecision(10) << forces4[3*i+2];
        std::cout << std::setprecision(10) << forces5[3*i+2] << std::endl;
    }
}
