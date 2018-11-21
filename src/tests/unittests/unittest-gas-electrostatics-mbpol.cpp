#include "testutils.h"

#include "electrostatics.h"
#include "setupwaterbox3mbpol.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

TEST_CASE("test the electrostatics class for mbpol (gas phase).") {
    // TIP3P test
    SETUP_WATERBOX_3_MBPOL
    elec::Electrostatics elec;
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", std::vector<double>{});
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);
    double perm_elec = elec.GetPermanentElectrostaticEnergy();
    double ind_elec = elec.GetInducedElectrostaticEnergy();

    SECTION("Permanent electrostatic energy.") { REQUIRE(perm_elec == Approx(-1.8567131645e+01).margin(TOL)); }

    SECTION("Induced electrostatic energy.") { REQUIRE(ind_elec == Approx(-6.4991884968e+00).margin(TOL)); }

    SECTION("Total electrostatic energy.") {
        REQUIRE(energy == Approx(-2.5066320142e+01).margin(TOL));

        SECTION("Forces of the total electrostatic energy") {
            for (size_t i = 0; i < grad.size(); i++) REQUIRE(forces[i] == Approx(grad[i]).margin(TOL));
        }
    }
}
