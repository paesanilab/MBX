#include "catch.hpp"

#include "electrostatics.h"
#include "setupwaterbox216.h"

#include <vector>
#include <iostream>

constexpr double TOL = 1E-8;
constexpr double angtobohr = 1.8897259886;

TEST_CASE("test the electrostatics class for coulomb terms (PME).") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    SETUP_WATERBOX_216
    elec::Electrostatics elec;
    std::vector<double> box_vectors{25 * angtobohr, 0, 0, 0, 25 * angtobohr, 0, 0, 0, 25 * angtobohr};
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", box_vectors);
    elec.SetCutoff(12 * angtobohr);
    elec.SetEwaldAlpha(0.3 / angtobohr);
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);
    std::cout << energy * 627.509 << std::endl;

    REQUIRE(energy == Approx(-3.63370).margin(TOL));
}
