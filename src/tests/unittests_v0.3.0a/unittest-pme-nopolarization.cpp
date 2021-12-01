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

#include "Catch2/single_include/catch.hpp"

#include "potential/electrostatics/electrostatics.h"
#include "setup_h2o_216.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1e-5;

TEST_CASE("test the electrostatics class for only coulomb terms (PME) - ewald alpha sweep.") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 0;
    double polfacH = 0;
    double polfacM = 0;
    SETUP_H2O_216
    double ref_energy = -1820.686249;

    elec::Electrostatics elec;
    std::vector<double> box_vectors{34, 0, 0, 0, 34, 0, 0, 0, 34};

    const char *method = "iter";

    /*
     * Ensure that computed properties are invariant to changes in the Ewald attenuation parameter
     */
    // alpha = 0.3

    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, islocal,
                    sys_at_tag, true, 1E-16, 100, method, box_vectors);
    elec.SetCutoff(16.5);
    elec.SetEwaldAlpha(0.25);
    elec.SetEwaldGridDensity(2.5);
    elec.SetEwaldSplineOrder(6);
    std::vector<double> forces3(3 * n_atoms);
    double energy3 = elec.GetElectrostatics(forces3);
    REQUIRE(energy3 == Approx(ref_energy).epsilon(TOL));

    // alpha = 0.4
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, islocal,
                    sys_at_tag, true, 1E-16, 100, method, box_vectors);
    elec.SetCutoff(13);
    elec.SetEwaldAlpha(0.35);
    elec.SetEwaldGridDensity(2.5);
    elec.SetEwaldSplineOrder(7);
    std::vector<double> forces4(3 * n_atoms);
    double energy4 = elec.GetElectrostatics(forces4);
    REQUIRE(energy4 == Approx(energy3).epsilon(TOL));
    for (int n = 0; n < 3 * n_atoms; ++n) REQUIRE(forces3[n] == Approx(forces4[n]).epsilon(TOL));

    // alpha = 0.5
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, islocal,
                    sys_at_tag, true, 1E-16, 100, method, box_vectors);
    elec.SetCutoff(10);
    elec.SetEwaldAlpha(0.45);
    elec.SetEwaldGridDensity(3.5);
    elec.SetEwaldSplineOrder(8);
    std::vector<double> forces5(3 * n_atoms);
    double energy5 = elec.GetElectrostatics(forces5);
    REQUIRE(energy5 == Approx(energy4).epsilon(TOL));
    for (int n = 0; n < 3 * n_atoms; ++n) REQUIRE(forces4[n] == Approx(forces5[n]).epsilon(TOL));

    std::cout << "Energies:" << std::endl;
    std::cout << "alpha = 0.25: " << std::setw(16) << std::setprecision(10) << energy3 << std::endl;
    std::cout << "alpha = 0.35: " << std::setw(16) << std::setprecision(10) << energy4 << std::endl;
    std::cout << "alpha = 0.45: " << std::setw(16) << std::setprecision(10) << energy5 << std::endl;
    std::cout << "\nForces:" << std::endl;
    std::cout << "        alpha = 0.25    alpha = 0.35    alpha = 0.45" << std::endl;
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
