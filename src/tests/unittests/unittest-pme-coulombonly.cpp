#include "catch.hpp"

#include "electrostatics.h"
#include "setupwaterbox216.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1E-4;

TEST_CASE("test the electrostatics class for coulomb terms (PME).") {
    // TIP3P test
    double qO = -0.834 * constants::CHARGECON;
    double qH = 0.417 * constants::CHARGECON;
    double qM = 0;
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
    elec.SetCutoff(14);
    elec.SetEwaldAlpha(0.3);
    elec.SetEwaldGridDensity(1.3);
    elec.SetEwaldSplineOrder(6);
    std::vector<double> forces3(3 * n_atoms);
    double energy3 = elec.GetElectrostatics(forces3);
    REQUIRE(energy3 == Approx(ref_energy).margin(TOL));


    // alpha = 0.4
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", box_vectors);
    elec.SetCutoff(9);
    elec.SetEwaldAlpha(0.4);
    elec.SetEwaldGridDensity(2.5);
    elec.SetEwaldSplineOrder(7);
    std::vector<double> forces4(3 * n_atoms);
    double energy4 = elec.GetElectrostatics(forces4);
    REQUIRE(energy4 == Approx(energy3).margin(TOL));
    for(int n = 0; n < 3*n_atoms; ++n) REQUIRE(forces3[n] == Approx(forces4[n]).margin(TOL));

    // alpha = 0.5
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", box_vectors);
    elec.SetCutoff(8);
    elec.SetEwaldAlpha(0.5);
    elec.SetEwaldGridDensity(3.5);
    elec.SetEwaldSplineOrder(7);
    std::vector<double> forces5(3 * n_atoms);
    double energy5 = elec.GetElectrostatics(forces5);
    REQUIRE(energy5 == Approx(energy4).margin(TOL));
    for(int n = 0; n < 3*n_atoms; ++n) REQUIRE(forces4[n] == Approx(forces5[n]).margin(TOL));
}
