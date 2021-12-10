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

#include "bblock/system.h"
#include "setup_co2_2_h2o_2.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test the system class.") {
    // Create the bromide -- water system
    SETUP_CO2_2_H2O_2

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

    SECTION("Get2bCutoff") {
        double old_cutoff = my_system.Get2bCutoff();
        my_system.Set2bCutoff(9.12345);
        double new_cutoff = my_system.Get2bCutoff();
        REQUIRE(new_cutoff == Approx(9.12345).margin(TOL));
        my_system.Set2bCutoff(old_cutoff);
    }

    SECTION("Get3bCutoff") {
        double old_cutoff = my_system.Get3bCutoff();
        my_system.Set3bCutoff(5.6789);
        double new_cutoff = my_system.Get3bCutoff();
        REQUIRE(new_cutoff == Approx(5.6789).margin(TOL));
        my_system.Set3bCutoff(old_cutoff);
    }

    SECTION("TTM pairs") {
        std::vector<std::pair<std::string, std::string> > empty_vec;
        std::vector<std::pair<std::string, std::string> > ttm_pairs = {{"h2o", "cl-"}, {"h2o", "i-"}};
        std::vector<std::pair<std::string, std::string> > ttm_pairs_expected = {{"cl-", "h2o"}, {"h2o", "i-"}};

        for (size_t i = 0; i < ttm_pairs.size(); i++) {
            my_system.AddTTMnrgPair(ttm_pairs[i].first, ttm_pairs[i].second);
        }

        std::vector<std::pair<std::string, std::string> > ttm_pairs_system = my_system.GetTTMnrgPairs();
        for (size_t i = 0; i < ttm_pairs.size(); i++) {
            REQUIRE(ttm_pairs_system[i] == ttm_pairs_expected[i]);
        }

        my_system.SetTTMnrgPairs(empty_vec);
        ttm_pairs_system = my_system.GetTTMnrgPairs();
        REQUIRE(ttm_pairs_system == empty_vec);

        my_system.SetTTMnrgPairs(ttm_pairs);
        ttm_pairs_system = my_system.GetTTMnrgPairs();
        for (size_t i = 0; i < ttm_pairs.size(); i++) {
            REQUIRE(ttm_pairs_system[i] == ttm_pairs_expected[i]);
        }
    }

    SECTION("Ignore 2b poly") {
        std::vector<std::vector<std::string> > empty_vec;
        std::vector<std::vector<std::string> > ignore2b = {{"h2o", "cl-"}, {"h2o", "i-"}};
        std::vector<std::vector<std::string> > ignore2b_expected = {{"cl-", "h2o"}, {"h2o", "i-"}};

        for (size_t i = 0; i < ignore2b.size(); i++) {
            my_system.Add2bIgnorePoly(ignore2b[i][0], ignore2b[i][1]);
        }

        std::vector<std::vector<std::string> > ignore2b_system = my_system.Get2bIgnorePoly();
        for (size_t i = 0; i < ignore2b.size(); i++) {
            REQUIRE(ignore2b_system[i] == ignore2b_expected[i]);
        }

        my_system.Set2bIgnorePoly(empty_vec);
        ignore2b_system = my_system.Get2bIgnorePoly();
        REQUIRE(ignore2b_system == empty_vec);

        my_system.Set2bIgnorePoly(ignore2b);
        ignore2b_system = my_system.Get2bIgnorePoly();
        for (size_t i = 0; i < ignore2b.size(); i++) {
            REQUIRE(ignore2b_system[i] == ignore2b_expected[i]);
        }
    }

    SECTION("Ignore 3b poly") {
        std::vector<std::vector<std::string> > empty_vec;
        std::vector<std::vector<std::string> > ignore3b = {{"h2o", "cl-", "h2o"}, {"h2o", "i-", "h2o"}};
        std::vector<std::vector<std::string> > ignore3b_expected = {{"cl-", "h2o", "h2o"}, {"h2o", "h2o", "i-"}};

        for (size_t i = 0; i < ignore3b.size(); i++) {
            my_system.Add3bIgnorePoly(ignore3b[i][0], ignore3b[i][1], ignore3b[i][2]);
        }

        std::vector<std::vector<std::string> > ignore3b_system = my_system.Get3bIgnorePoly();
        for (size_t i = 0; i < ignore3b.size(); i++) {
            REQUIRE(ignore3b_system[i] == ignore3b_expected[i]);
        }

        my_system.Set3bIgnorePoly(empty_vec);
        ignore3b_system = my_system.Get3bIgnorePoly();
        REQUIRE(ignore3b_system == empty_vec);

        my_system.Set3bIgnorePoly(ignore3b);
        ignore3b_system = my_system.Get3bIgnorePoly();
        for (size_t i = 0; i < ignore3b.size(); i++) {
            REQUIRE(ignore3b_system[i] == ignore3b_expected[i]);
        }
    }

    SECTION("JSON settings") {
        nlohmann::json j = {{"Note", "This is a cofiguration file"},
                            {"MBX",
                             {{"box", nlohmann::json::array()},
                              {"twobody_cutoff", 100.0},
                              {"threebody_cutoff", 6.5},
                              {"max_n_eval_1b", 500},
                              {"max_n_eval_2b", 500},
                              {"max_n_eval_3b", 500},
                              {"dipole_tolerance", 1E-016},
                              {"dipole_max_it", 100},
                              {"dipole_method", "cg"},
                              {"alpha_ewald_elec", 0.0},
                              {"grid_density_elec", 2.5},
                              {"spline_order_elec", 6},
                              {"alpha_ewald_disp", 0.0},
                              {"grid_density_disp", 2.5},
                              {"spline_order_disp", 6},
                              {"ttm_pairs", nlohmann::json::array()},
                              {"ff_mons", nlohmann::json::array()},
                              {"connectivity_file", ""},
                              {"ignore_1b_poly", nlohmann::json::array()},
                              {"ignore_2b_poly", nlohmann::json::array()},
                              {"ignore_3b_poly", nlohmann::json::array()}}},
                            {"i-pi", {{"port", 34543}, {"localhost", "localhost"}}}};

        // Write this configuration into a file
        std::ofstream of("mbx.json");
        of << std::setw(4) << j;
        of.close();

        nlohmann::json j_sys_default;

        SECTION("Compare default json") {
            my_system.SetUpFromJson();
            j_sys_default = my_system.GetJsonConfig();
            REQUIRE(j_sys_default["MBX"] == j["MBX"]);
        }

        // Update json parameters
        j["MBX"]["box"] = nlohmann::json::array();
        j["MBX"]["twobody_cutoff"] = 9.5;
        j["MBX"]["threebody_cutoff"] = 7.5;
        j["MBX"]["max_n_eval_1b"] = 400;
        j["MBX"]["max_n_eval_2b"] = 400;
        j["MBX"]["max_n_eval_3b"] = 300;
        j["MBX"]["dipole_tolerance"] = 1E-14;
        j["MBX"]["dipole_max_it"] = 150;
        j["MBX"]["dipole_method"] = "iter";
        j["MBX"]["alpha_ewald_elec"] = 0.01;
        j["MBX"]["grid_density_elec"] = 2.8;
        j["MBX"]["spline_order_elec"] = 5;
        j["MBX"]["alpha_ewald_disp"] = 0.01;
        j["MBX"]["grid_density_disp"] = 2.7;
        j["MBX"]["spline_order_disp"] = 4;
        j["MBX"]["ttm_pairs"] = nlohmann::json::array({{"h2o", "i-"}, {"cs+", "h2o"}});
        j["MBX"]["ff_mons"] = nlohmann::json::array({"co2"});
        j["MBX"]["ignore_1b_poly"] = nlohmann::json::array({"co2"});
        j["MBX"]["ignore_2b_poly"] = nlohmann::json::array({{"h2o", "i-"}, {"cs+", "h2o"}});
        j["MBX"]["ignore_3b_poly"] = nlohmann::json::array({{"h2o", "i-"}, {"cs+", "h2o"}});

        // Write the new json file
        std::ofstream off("mbx_mod.json");
        off << std::setw(4) << j;
        off.close();

        // Set the json config in system from the file
        my_system.SetUpFromJson((char *)"mbx_mod.json");
        // Retrieve it
        nlohmann::json j_sys_mod = my_system.GetJsonConfig();

        SECTION("Compare modified json") { REQUIRE(j_sys_mod["MBX"] == j["MBX"]); }

        SECTION("Compare attributes in system set from json") {
            std::vector<double> box = my_system.GetBox();
            REQUIRE(box == j["MBX"]["box"]);

            double cutoff2b = my_system.Get2bCutoff();
            REQUIRE(cutoff2b == j["MBX"]["twobody_cutoff"]);

            double cutoff3b = my_system.Get3bCutoff();
            REQUIRE(cutoff3b == j["MBX"]["threebody_cutoff"]);

            size_t neval1b = my_system.GetMaxEval1b();
            REQUIRE(neval1b == j["MBX"]["max_n_eval_1b"]);

            size_t neval2b = my_system.GetMaxEval2b();
            REQUIRE(neval2b == j["MBX"]["max_n_eval_2b"]);

            size_t neval3b = my_system.GetMaxEval3b();
            REQUIRE(neval3b == j["MBX"]["max_n_eval_3b"]);

            double dipole_tolerance = my_system.GetDipoleTolerance();
            REQUIRE(dipole_tolerance == j["MBX"]["dipole_tolerance"]);

            double dipole_max_it = my_system.GetMaxIterationsDipoles();
            REQUIRE(dipole_max_it == j["MBX"]["dipole_max_it"]);

            std::string dipole_method = my_system.GetDipoleMethod();
            REQUIRE(dipole_method == j["MBX"]["dipole_method"]);

            double alpha = 0.0;
            double grid = 0.0;
            size_t spline = 0;
            my_system.GetEwaldParamsElectrostatics(alpha, grid, spline);
            REQUIRE(alpha == j["MBX"]["alpha_ewald_elec"]);
            REQUIRE(grid == j["MBX"]["grid_density_elec"]);
            REQUIRE(spline == j["MBX"]["spline_order_elec"]);

            my_system.GetEwaldParamsDispersion(alpha, grid, spline);
            REQUIRE(alpha == j["MBX"]["alpha_ewald_disp"]);
            REQUIRE(grid == j["MBX"]["grid_density_disp"]);
            REQUIRE(spline == j["MBX"]["spline_order_disp"]);

            std::vector<std::pair<std::string, std::string> > ttmpairs = my_system.GetTTMnrgPairs();
            REQUIRE(ttmpairs == j["MBX"]["ttm_pairs"]);

            std::vector<std::string> ffmons = my_system.GetFFMons();
            REQUIRE(ffmons == j["MBX"]["ff_mons"]);

            // std::vector<std::string> connectivity_file = my_system.GetConnectivityFile();
            // REQUIRE(connectivity_file == j["MBX"]["connectivity_file"]);

            std::vector<std::string> ignore1b = my_system.Get1bIgnorePoly();
            REQUIRE(ignore1b == j["MBX"]["ignore_1b_poly"]);

            std::vector<std::vector<std::string> > ignore2b = my_system.Get2bIgnorePoly();
            REQUIRE(ignore2b == j["MBX"]["ignore_2b_poly"]);

            std::vector<std::vector<std::string> > ignore3b = my_system.Get3bIgnorePoly();
            REQUIRE(ignore3b == j["MBX"]["ignore_3b_poly"]);
        }

        // Set back the defaults
        my_system.SetUpFromJson();
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
                std::string id_v = "cl-";
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
            std::string id_v = "cl-";
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
            std::string id_v = "cl-";
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
    SETUP_CO2_2_H2O_2

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
