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

#include "setup_h4_dummy.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test monomer H4 energy terms") {
    // Create the system
    SETUP_H4_DUMMY

    bond1.SetParameters(bond_harm_linear_parameters, bond_harm_nonlinear_parameters);
    bond2.SetParameters(bond_harm_linear_parameters, bond_harm_nonlinear_parameters);
    bond3.SetParameters(bond_harm_linear_parameters, bond_harm_nonlinear_parameters);
    std::vector<eff::Bond> bond_vec = {bond1, bond2, bond3};
    angle1.SetParameters(angle_harm_linear_parameters1, angle_harm_nonlinear_parameters1);
    angle2.SetParameters(angle_harm_linear_parameters2, angle_harm_nonlinear_parameters2);
    std::vector<eff::Angles> angle_vec = {angle1, angle2};
    dihedral1.SetParameters(dihedral_cos_linear_parameters, dihedral_cos_nonlinear_parameters);
    std::vector<eff::Dihedral> dihedral_vec = {dihedral1};
    inversion1.SetParameters(inversion_harm_linear_parameters, inversion_harm_nonlinear_parameters);
    std::vector<eff::Inversion> inversion_vec = {inversion1};
    eff::Conn connectivity = eff::Conn(mon_id, bond_vec, angle_vec, dihedral_vec, inversion_vec);
    std::unordered_map<std::string, eff::Conn> connectivity_map = {std::make_pair("h4_dummy", connectivity)};

    // Now we create a system that will be the same as the one read
    bblock::System my_system;
    my_system.SetConnectivity(connectivity_map);

    // Add monomers to the system
    size_t count = 0;
    for (size_t i = 0; i < n_monomers; i++) {
        std::vector<double> xyz(real_coords.begin() + 3 * count,
                                real_coords.begin() + 3 * count + 3 * n_atoms_vector[i]);
        std::vector<std::string> ats(atom_names.begin() + count, atom_names.begin() + count + n_atoms_vector[i]);
        std::string monid = monomer_names[i];
        my_system.AddMonomer(xyz, ats, monid);
        count += n_atoms_vector[i];
    }

    // Initialize the system to fill in the information
    my_system.Initialize();

    // Indicate we want the classical energy
    my_system.Set2bIgnorePoly(ignore_2b);
    my_system.Set3bIgnorePoly(ignore_3b);
    my_system.SetTTMnrgPairs(ttm_pairs);
    my_system.Set1bIgnorePoly(ignore_1b);
    my_system.SetFFMons(ff_mon);

    SECTION("Classic Potential") {
        double energy_nograd = my_system.ClassicPotential(false);
        double energy_grad = my_system.ClassicPotential(true);
        double energy_nograd_system = my_system.Energy(false);
        double energy_grad_system = my_system.Energy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients (ClassicPotential)") {
            REQUIRE(energy_nograd == Approx(classic_energy).margin(TOL));
        }

        SECTION("Energy with gradients (ClassicPotential)") {
            REQUIRE(energy_grad == Approx(classic_energy).margin(TOL));
        }

        SECTION("Energy without gradients (Energy)") {
            REQUIRE(energy_nograd_system == Approx(classic_energy).margin(TOL));
        }

        SECTION("Energy with gradients (Energy)") { REQUIRE(energy_grad_system == Approx(classic_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 0.0001;  // passes first test
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.ClassicPotential(false);
                double plusEnergyG = my_system.ClassicPotential(true);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.ClassicPotential(false);
                double plusplusEnergyG = my_system.ClassicPotential(true);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusplusEnergy = my_system.ClassicPotential(false);
                double plusplusplusEnergyG = my_system.ClassicPotential(true);
                all_xyz[degreeOfFreedom] -= 6 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusminusEnergy = my_system.ClassicPotential(false);
                double minusminusminusEnergyG = my_system.ClassicPotential(true);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.ClassicPotential(false);
                double minusminusEnergyG = my_system.ClassicPotential(true);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.ClassicPotential(false);
                double minusEnergyG = my_system.ClassicPotential(true);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce = (-1 * minusminusminusEnergy + 9 * minusminusEnergy - 45 * minusEnergy +
                                                45 * plusEnergy - 9 * plusplusEnergy + plusplusplusEnergy) /
                                               (60 * stepSize);
                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }
}
