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
#include "setup_h2o_2.h"

#include <vector>
#include <iostream>
#include <iomanip>

constexpr double TOL = 1E-8;

TEST_CASE("test the electrostatics class for coulomb and polarization terms (PME) - finite differences.") {
    // TIP3P test
    double qO = -0.834;
    double qH = 0.417;
    double qM = 0;
    double polfacO = 0;
    double polfacH = 0;
    double polfacM = 0;
    SETUP_H2O_2
    double ref_energy = -0.1744839641;

    elec::Electrostatics elec;
    std::vector<double> box_vectors{30, 0, 0, 0, 30, 0, 0, 0, 30};

    /*
     * Ensure that computed properties are invariant to changes in the Ewald attenuation parameter
     */
    double alpha = 0.3;
    double grid_density = 2.5;
    int spline_order = 6;
    double cutoff = 10;
    const char *method = "iter";
    elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count, islocal,
                    true, 1E-16, 100, method, box_vectors);
    elec.SetCutoff(cutoff);
    elec.SetEwaldAlpha(alpha);
    elec.SetEwaldGridDensity(grid_density);
    elec.SetEwaldSplineOrder(spline_order);
    std::vector<double> forces(3 * n_atoms);
    double energy = elec.GetElectrostatics(forces);
    std::cout << "Energy: " << energy << std::endl;
    REQUIRE(energy == Approx(ref_energy).margin(TOL));

    double stepSize = 0.00001;
    const std::vector<std::string> labels = {"x", "y", "z"};
    std::vector<double> ignoredForces(3 * n_atoms);
    std::cout << " DoF      Analytic         Numerical       Difference" << std::endl;
    for (int degreeOfFreedom = 0; degreeOfFreedom < 3 * n_atoms; ++degreeOfFreedom) {
        coords[degreeOfFreedom] += stepSize;
        elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count,
                        islocal, false, 1E-16, 100, method, box_vectors);
        elec.SetCutoff(cutoff);
        elec.SetEwaldAlpha(alpha);
        elec.SetEwaldGridDensity(grid_density);
        elec.SetEwaldSplineOrder(spline_order);
        double plusEnergy = elec.GetElectrostatics(ignoredForces);
        coords[degreeOfFreedom] -= 2 * stepSize;
        elec.Initialize(charges, chg_grad, polfac, pol, coords, monomer_names, sites, first_ind, mon_type_count,
                        islocal, false, 1E-16, 100, method, box_vectors);
        elec.SetCutoff(cutoff);
        elec.SetEwaldAlpha(alpha);
        elec.SetEwaldGridDensity(grid_density);
        elec.SetEwaldSplineOrder(spline_order);
        double minusEnergy = elec.GetElectrostatics(ignoredForces);
        coords[degreeOfFreedom] += stepSize;
        double finiteDifferenceForce = (plusEnergy - minusEnergy) / (2 * stepSize);
        double error = forces[degreeOfFreedom] - finiteDifferenceForce;
        int atom = degreeOfFreedom / 3;
        int xyz = degreeOfFreedom % 3;
        std::cout << std::setw(3) << atom + 1 << labels[xyz] << std::setprecision(10) << std::setw(16)
                  << forces[degreeOfFreedom] << "  " << std::setprecision(10) << std::setw(16) << finiteDifferenceForce
                  << "  " << std::setprecision(10) << std::setw(16) << forces[degreeOfFreedom] - finiteDifferenceForce;
        if (std::abs(forces[degreeOfFreedom] - finiteDifferenceForce) > TOL) std::cout << " <---- BAD!";
        std::cout << std::endl;

        REQUIRE(forces[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
    }
}
