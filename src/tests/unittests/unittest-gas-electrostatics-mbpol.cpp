/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "tools/testutils.h"

#include "potential/electrostatics/electrostatics.h"
#include "setup_h2o_3.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

constexpr double TOL = 1E-6;

TEST_CASE("Test the electrostatics class for mbpol (gas phase).") {
    // MBpol test
    SETUP_H2O_3
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
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, islocal,
                    true, 1E-16, 100, "iter", std::vector<double>{});
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
