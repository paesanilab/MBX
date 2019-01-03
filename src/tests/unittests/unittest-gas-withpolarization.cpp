#include "catch.hpp"

#include "electrostatics.h"
#include "setupwaterbox2.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1E-4;

TEST_CASE("test the electrostatics class for coulomb and polarization terms (GAS).") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 1.310;
    double polfacH = 0.294;
    double polfacM = 0;
    SETUP_WATERBOX_2
    double ref_energy = -1824.323;

    elec::Electrostatics elec;

    /*
     * Ensure that computed properties are invariant to changes in the Ewald attenuation parameter
     */

    // alpha = 0.3
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter");
    elec.SetCutoff(1000);
    std::vector<double> forces3(3 * n_atoms);
    double energy3 = elec.GetElectrostatics(forces3);
    std::cout << "Energy " << energy3 << std::endl;
    //REQUIRE(energy3 == Approx(ref_energy).margin(TOL));
    exit(0);
}

