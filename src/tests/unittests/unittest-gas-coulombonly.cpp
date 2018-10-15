#include "testutils.h"

#include "electrostatics.h"
#include "setupwaterbox216.h"

#include <vector>
#include <iostream>

constexpr double TOL = 1E-6;

TEST_CASE("test the electrostatics class for coulomb terms (gas phase).") {
    SETUP_WATERBOX_216
    elec::Electrostatics elec;
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, true,
                    1E-16, 100, "iter", std::vector<double>{});
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);

    REQUIRE(energy == Approx(-2.9834756566).margin(TOL));

    // Water 1 forces
    REQUIRE(VectorsAreEqual(std::vector<double>(&forces[0], &forces[12]),
                            {-0.0115132, -0.00587286, -0.00278926, 0.00322535, 0.00094877, 0.00383509, 0.00307214,
                             -0.00119489, -0.00205574, 0, 0, 0}));
    // Water 2 forces
    REQUIRE(VectorsAreEqual(std::vector<double>(&forces[12], &forces[24]),
                            {0.0107019, -0.00194389, -0.00634265, -0.00626186, -0.0101777, 0.00453987, -0.00018795,
                             0.00242102, 0.00112099, 0, 0, 0}));
    // Water 215 forces
    REQUIRE(VectorsAreEqual(std::vector<double>(&forces[2568], &forces[2580]),
                            {0.0113128, 0.00704537, -0.00338281, -0.00236944, -0.0122949, 0.00620509, -0.00505203,
                             0.00403866, -0.00150855, 0, 0, 0}));
    // Water 216 forces
    REQUIRE(VectorsAreEqual(std::vector<double>(&forces[2580], &forces[2592]),
                            {-0.00199882, 0.00429957, -0.00355518, 0.000682984, -0.0100953, 0.00230285, 0.00273241,
                             -0.000698474, 0.00179964, 0, 0, 0}));
}
