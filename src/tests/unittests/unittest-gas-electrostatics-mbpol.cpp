#include "testutils.h"

#include "electrostatics.h"
#include "setupwaterbox3mbpol.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

TEST_CASE("Test the electrostatics class for mbpol (gas phase).") {
    // TIP3P test
    SETUP_WATERBOX_3_MBPOL
    std::vector<double> elec_grad{-7.1325887311e+00, 9.3838029234e+00,  -7.1120493017e+00, -2.6049669131e+00,
                                  3.1364986514e+00,  3.3810366964e+00,  1.9731689382e+01,  1.1749153059e+01,
                                  2.9275233225e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                  9.9673744707e+00,  2.7394661040e+00,  7.3112083163e+00,  6.1157299974e-01,
                                  -2.3311542181e+01, -1.0377446592e+00, 4.5858113684e+00,  1.9859093843e-01,
                                  -3.2775645200e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                  -4.5647541379e+00, -1.0940195392e+01, 7.1162824039e+00,  -1.2633174232e+00,
                                  -3.9641293215e+00, -3.7085780441e+00, -1.9330821016e+01, 1.1008355219e+01,
                                  -5.6001142142e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00};

    elec::Electrostatics elec;
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", std::vector<double>{});
    std::vector<double> forces(3 * n_sites);
    double energy = elec.GetElectrostatics(forces);
    double perm_elec = elec.GetPermanentElectrostaticEnergy();
    double ind_elec = elec.GetInducedElectrostaticEnergy();

    SECTION("Permanent electrostatic energy.") { REQUIRE(perm_elec == Approx(-1.8567131645e+01).margin(TOL)); }

    SECTION("Induced electrostatic energy.") { REQUIRE(ind_elec == Approx(-6.4991884968e+00).margin(TOL)); }

    SECTION("Total electrostatic energy.") {
        REQUIRE(energy == Approx(-2.5066320142e+01).margin(TOL));

        SECTION("Forces of the total electrostatic energy") {
            for (size_t i = 0; i < elec_grad.size(); i++) REQUIRE(forces[i] == Approx(elec_grad[i]).margin(TOL));
        }
    }
}
