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

#include "testutils.h"

#include "bblock/system.h"
#include "setup_h2o_5_br_1.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test the system class.") {
    // Create the bromide -- water system
    SETUP_H2O_5_BR_1

    // Now we create a system that will be the same as the one read
    bblock::System my_system;

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

    SECTION("GetNumMon()") {
        size_t n = my_system.GetNumMon();
        REQUIRE(n == n_monomers);
    }

    SECTION("GetNumRealSites()") {
        size_t n = my_system.GetNumRealSites();
        REQUIRE(n == n_atoms);
    }

    SECTION("GetNumSites()") {
        size_t n = my_system.GetNumSites();
        REQUIRE(n == n_sites);
    }

    SECTION("GetRealAtomNames()") {
        std::vector<std::string> v = my_system.GetRealAtomNames();
        REQUIRE(VectorsAreEqual(v, atom_names));
    }

    SECTION("GetMonNumAt()") {
        for (size_t i = 0; i < n_monomers; i++) {
            size_t n = my_system.GetMonNumAt(i);
            REQUIRE(n == n_atoms_vector[i]);
        }
    }

    SECTION("GetFirstIndex()") {
        for (size_t i = 0; i < n_monomers; i++) {
            size_t n = my_system.GetFirstInd(i);
            REQUIRE(n == first_index[i]);
        }
    }

    SECTION("GetXyz()") {
        std::vector<double> xyz = my_system.GetXyz();
        REQUIRE(VectorsAreEqual(xyz, coords, TOL));
    }

    SECTION("GetRealXyz()") {
        std::vector<double> xyz = my_system.GetRealXyz();
        REQUIRE(VectorsAreEqual(xyz, real_coords, TOL));
    }

    SECTION("GetCharges()") {
        std::vector<double> chg = my_system.GetCharges();
        REQUIRE(VectorsAreEqual(chg, charges, TOL));
    }

    SECTION("GetRealCharges()") {
        std::vector<double> chg = my_system.GetRealCharges();
        REQUIRE(VectorsAreEqual(chg, real_charges, TOL));
    }

    SECTION("GetPolarizabilities()") {
        std::vector<double> polar = my_system.GetPolarizabilities();
        REQUIRE(VectorsAreEqual(polar, pol, TOL));
    }

    SECTION("GetRealPolarizabilities()") {
        std::vector<double> polar = my_system.GetRealPolarizabilities();
        REQUIRE(VectorsAreEqual(polar, real_pol, TOL));
    }

    SECTION("GetPolarizabilityFactors()") {
        std::vector<double> polarfac = my_system.GetPolarizabilityFactors();
        REQUIRE(VectorsAreEqual(polarfac, polfac, TOL));
    }

    SECTION("GetRealPolarizabilityFactors()") {
        std::vector<double> polarfac = my_system.GetRealPolarizabilityFactors();
        REQUIRE(VectorsAreEqual(polarfac, real_polfac, TOL));
    }

    SECTION("SetXyz()") {
        // Get current XYZ
        std::vector<double> xyz = my_system.GetXyz();
        SECTION("Proper behavior of the function") {
            // Modify xyz adding 1 to all coordinates
            for (size_t i = 0; i < xyz.size(); i++) xyz[i] += 1.0;
            // Setting it
            my_system.SetXyz(xyz);
            // Getting it
            std::vector<double> new_xyz = my_system.GetXyz();
            REQUIRE(VectorsAreEqual(xyz, new_xyz, TOL));
            // Reset xyz to original coordinates
            my_system.SetXyz(coords);
        }

        SECTION("Set non-valid xyz") {
            std::vector<double> v(2, 1.0);
            bool xyz_cannot_be_set = false;
            try {
                my_system.SetXyz(v);
            } catch (CUException &e) {
                xyz_cannot_be_set = true;
            }
            REQUIRE(xyz_cannot_be_set);
        }
    }

    SECTION("SetRealXyz()") {
        // Get current XYZ
        std::vector<double> xyz = my_system.GetRealXyz();
        SECTION("Proper behavior of the function") {
            // Modify xyz adding 1 to all coordinates
            for (size_t i = 0; i < xyz.size(); i++) xyz[i] += 1.0;
            // Setting it
            my_system.SetRealXyz(xyz);
            // Getting it
            std::vector<double> new_xyz = my_system.GetRealXyz();
            REQUIRE(VectorsAreEqual(xyz, new_xyz, TOL));
            // Reset xyz to original coordinates
            my_system.SetXyz(coords);
        }

        SECTION("Set non-valid xyz") {
            std::vector<double> v(2, 1.0);
            bool xyz_cannot_be_set = false;
            try {
                my_system.SetRealXyz(v);
            } catch (CUException &e) {
                xyz_cannot_be_set = true;
            }
            REQUIRE(xyz_cannot_be_set);
        }
    }

    SECTION("Initialize()") {
        SECTION("Initialize an already initialized system") {
            bool system_cannot_be_initialized = false;
            try {
                my_system.Initialize();
            } catch (CUException &e) {
                system_cannot_be_initialized = true;
            }
            REQUIRE(system_cannot_be_initialized);
        }
    }

    SECTION("AddMonomer()") {
        SECTION("Add monomer to an initialized system") {
            bool monomer_not_added_to_initialized_system = false;
            try {
                std::vector<double> v(3, 0.0);
                std::string id_v = "cl";
                std::vector<std::string> at_v = {"Cl"};
                my_system.AddMonomer(v, at_v, id_v);
            } catch (CUException &e) {
                monomer_not_added_to_initialized_system = true;
            }
            REQUIRE(monomer_not_added_to_initialized_system);
        }
    }

    SECTION("AddMonomerInfo()") {
        SECTION("Initialize system with less than 3 coordinates") {
            // Creating a fake system
            std::vector<double> v(2, 1.0);
            std::string id_v = "cl";
            std::vector<std::string> at_v = {"Cl"};

            bblock::System s_wrong;
            s_wrong.AddMonomer(v, at_v, id_v);

            bool system_too_short = false;

            // Testing that with less than 3 coordinates you cannot have a system
            try {
                s_wrong.Initialize();
            } catch (CUException &e) {
                system_too_short = true;
            }

            REQUIRE(system_too_short);
        }

        SECTION("Initialize system with more coordinates than expected") {
            // Creating a fake system
            std::vector<double> v(4, 1.0);
            std::string id_v = "cl";
            std::vector<std::string> at_v = {"Cl"};

            bblock::System s_wrong;
            s_wrong.AddMonomer(v, at_v, id_v);

            bool system_too_long = false;

            // Testing that with less than 3 coordinates you cannot have a system
            try {
                s_wrong.Initialize();
            } catch (CUException &e) {
                system_too_long = true;
            }

            REQUIRE(system_too_long);
        }
    }

    SECTION("GetPairList()") {
        SECTION("Get clusters of more than 3 molecules") {
            bool not_possible_to_get_clusters = false;
            try {
                my_system.GetPairList(5, 3.0, 0, 1);
            } catch (CUException &e) {
                not_possible_to_get_clusters = true;
            }
            REQUIRE(not_possible_to_get_clusters);
        }
    }

    SECTION("SetPBC()") {
        SECTION("Wrong box size") {
            bool not_possible_to_set_box = false;
            try {
                std::vector<double> box(5, 10.0);
                my_system.SetPBC(box);
            } catch (CUException &e) {
                not_possible_to_set_box = true;
            }
            REQUIRE(not_possible_to_set_box);
        }
    }

    //   REQUIRE( == Approx(finiteDifferenceForce).margin(TOL));
}

TEST_CASE("Test energy from system") {
    // Create the bromide -- water system
    SETUP_H2O_5_BR_1

    // Now we create a system that will be the same as the one read
    bblock::System my_system;

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

    SECTION("One-Body") {
        double energy_nograd = my_system.OneBodyEnergy(false);
        double energy_grad = my_system.OneBodyEnergy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(one_body_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(one_body_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 0.00001;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.OneBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);
                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Two-Body") {
        double energy_nograd = my_system.TwoBodyEnergy(false);
        double energy_grad = my_system.TwoBodyEnergy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(two_body_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(two_body_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.TwoBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Three-Body") {
        double energy_nograd = my_system.ThreeBodyEnergy(false);
        double energy_grad = my_system.ThreeBodyEnergy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(three_body_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(three_body_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.ThreeBodyEnergy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Dispersion") {
        double energy_nograd = my_system.Dispersion(false);
        double energy_grad = my_system.Dispersion(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(dispersion_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Dispersion(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Electrostatics") {
        double energy_nograd = my_system.Electrostatics(false);
        double energy_grad = my_system.Electrostatics(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(electrostatic_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(electrostatic_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Electrostatics(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Total Energy") {
        double energy_nograd = my_system.Energy(false);
        double energy_grad = my_system.Energy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<double> real_xyz = my_system.GetRealXyz();
        std::vector<double> all_xyz = my_system.GetXyz();

        SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(total_energy).margin(TOL)); }

        SECTION("Energy with gradients") { REQUIRE(energy_grad == Approx(total_energy).margin(TOL)); }

        SECTION("Compare analitical gradients with numerical gradients") {
            double stepSize = 1E-04;
            for (size_t degreeOfFreedom = 0; degreeOfFreedom < all_xyz.size(); ++degreeOfFreedom) {
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double plusplusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] -= 4 * stepSize;
                my_system.SetXyz(all_xyz);
                double minusminusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double minusEnergy = my_system.Energy(false);
                all_xyz[degreeOfFreedom] += stepSize;
                my_system.SetXyz(all_xyz);
                double finiteDifferenceForce =
                    (8 * (plusEnergy - minusEnergy) - plusplusEnergy + minusminusEnergy) / (12 * stepSize);

                REQUIRE(all_grad[degreeOfFreedom] == Approx(finiteDifferenceForce).margin(TOL));
            }
        }
    }

    SECTION("Compare real atoms and all atoms") {
        double energy_grad = my_system.Energy(true);
        std::vector<double> real_grad = my_system.GetRealGrads();
        std::vector<double> all_grad = my_system.GetGrads();
        std::vector<std::string> real_atom_names = my_system.GetRealAtomNames();
        std::vector<std::string> all_atom_names = my_system.GetAtomNames();

        SECTION("Atom names") {
            size_t i_real = 0;
            for (size_t i = 0; i < all_atom_names.size(); i++) {
                if (all_atom_names[i] != "virt") {
                    REQUIRE(all_atom_names[i] == real_atom_names[i_real]);
                    REQUIRE(all_atom_names[i] == atom_names[i_real]);
                    i_real += 1;
                }
            }
        }

        SECTION("Gradients") {
            size_t i_real = 0;
            for (size_t i = 0; i < all_atom_names.size(); i++) {
                size_t real_offset = 3 * i_real;
                size_t offset = 3 * i;
                if (all_atom_names[i] != "virt") {
                    for (size_t j = 0; j < 3; j++) {
                        REQUIRE(all_grad[offset] == Approx(real_grad[real_offset]).margin(TOL));
                    }
                    i_real += 1;
                }
            }
        }
    }
}
