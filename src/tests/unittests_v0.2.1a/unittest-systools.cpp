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

#include "bblock/sys_tools.h"
#include "tools/math_tools.h"
#include "setup_co2_2_h2o_2.h"
#include "setup_monomer_mix.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("sys_tools::SetupMonomers") {
    SECTION("General behavior co2_2 H2o_2") {
        // Set up variables to create a system of 2 co2 and 2 h2o
        SETUP_CO2_2_H2O_2

        // Prepare variables for SetUpMonomers
        std::vector<size_t> sites_out;
        std::vector<size_t> nat_out;
        std::vector<size_t> first_index_out;
        // Run SetUpMonomers
        try {
            systools::SetUpMonomers(monomer_names, sites_out, nat_out, first_index_out);
        } catch (CUException &e) {
            std::cerr << e.what();
        }

        SECTION("Sites Vector") { REQUIRE(VectorsAreEqual(sites_out, n_sites_vector)); }

        SECTION("Number of Atoms Vector") { REQUIRE(VectorsAreEqual(nat_out, n_atoms_vector)); }

        SECTION("First Index Vector") { REQUIRE(VectorsAreEqual(first_index_out, first_index_realSites)); }
    }

    SECTION("General behavior monomer mix") {
        SETUP_MONMIX

        // Prepare variables for SetUpMonomers
        std::vector<size_t> sites_out;
        std::vector<size_t> nat_out;
        std::vector<size_t> first_index_out;
        // Run SetUpMonomers
        try {
            systools::SetUpMonomers(monomer_names, sites_out, nat_out, first_index_out);
        } catch (CUException &e) {
            std::cerr << e.what();
        }

        SECTION("Sites Vector") { REQUIRE(VectorsAreEqual(sites_out, n_sites_vector)); }

        SECTION("Number of Atoms Vector") { REQUIRE(VectorsAreEqual(nat_out, n_atoms_vector)); }

        SECTION("First Index Vector") { REQUIRE(VectorsAreEqual(first_index_out, first_index_realSites)); }
    }

    SECTION("Assertions") {
        // Dummy variables
        std::vector<size_t> tmpsites;
        std::vector<size_t> tmpnat;
        std::vector<size_t> tmpfirst_index;

        SECTION("Set up an empty monomer list") {
            std::vector<std::string> monomers_empty;
            bool not_possible_to_setup_monomers = false;
            try {
                systools::SetUpMonomers(monomers_empty, tmpsites, tmpnat, tmpfirst_index);
            } catch (CUException &e) {
                not_possible_to_setup_monomers = true;
            }
            REQUIRE(not_possible_to_setup_monomers);
        }

        SECTION("Add a monomer not in the database") {
            std::vector<std::string> monomers_not_in_db = {"not", "in", "data", "base", "at", "all"};
            bool monomer_not_in_database = false;
            try {
                systools::SetUpMonomers(monomers_not_in_db, tmpsites, tmpnat, tmpfirst_index);
            } catch (CUException &e) {
                monomer_not_in_database = true;
            }
            REQUIRE(monomer_not_in_database);
        }
    }
}

TEST_CASE("sys_tools::OrderMonomers") {
    std::vector<std::string> in_mons = {"c", "b", "b", "a"};
    std::vector<size_t> in_islocal = {1, 1, 0, 0};
    std::vector<size_t> in_sites = {3, 1, 1, 2};
    std::vector<size_t> in_nats = {2, 1, 1, 2};
    std::vector<size_t> out_original2current;
    std::vector<std::pair<size_t, size_t>> out_original_order;
    std::vector<std::pair<size_t, size_t>> out_original_order_realSites;
    std::vector<std::pair<std::string, size_t>> out_mon_type_count;

    std::vector<std::string> expected_mons = {"c", "a", "b", "b"};
    std::vector<size_t> expected_islocal = {1, 0, 1, 0};
    std::vector<size_t> expected_sites = {3, 2, 1, 1};
    std::vector<size_t> expected_nats = {2, 2, 1, 1};
    std::vector<size_t> expected_original2current = {0, 2, 3, 1};
    std::vector<std::pair<size_t, size_t>> expected_original_order = {{0, 0}, {3, 5}, {1, 3}, {2, 4}};
    std::vector<std::pair<size_t, size_t>> expected_original_order_realSites = {{0, 0}, {3, 4}, {1, 2}, {2, 3}};
    std::vector<std::pair<std::string, size_t>> expected_mon_type_count = {{"c", 1}, {"a", 1}, {"b", 2}};

    SECTION("General behavior") {
        try {
            out_mon_type_count = systools::OrderMonomers(in_mons, in_islocal, in_sites, in_nats, out_original2current,
                                                         out_original_order, out_original_order_realSites);
        } catch (CUException &e) {
            std::cerr << e.what();
        }

        SECTION("Monomer type count") { REQUIRE(VectorsAreEqual(expected_mon_type_count, out_mon_type_count)); }

        SECTION("Internal Order monomer names") { REQUIRE(VectorsAreEqual(expected_mons, in_mons)); }

        SECTION("Original to current vector") {
            REQUIRE(VectorsAreEqual(expected_original2current, out_original2current));
        }

        SECTION("Original Order Vector") { REQUIRE(VectorsAreEqual(expected_original_order, out_original_order)); }

        SECTION("Original Order for Real Sites Vector") {
            REQUIRE(VectorsAreEqual(expected_original_order_realSites, out_original_order_realSites));
        }
    }

    SECTION("Assertions") {
        SECTION("Monomer vector is empty") {
            std::vector<std::string> monomers_empty;
            std::vector<std::pair<std::string, size_t>> mon_type_count2;
            std::vector<size_t> original2current2;
            std::vector<std::pair<size_t, size_t>> original_order2;
            std::vector<std::pair<size_t, size_t>> original_order_realSites2;
            std::vector<size_t> islocal_empty;
            bool not_possible_to_order_monomers = false;
            try {
                mon_type_count2 =
                    systools::OrderMonomers(monomers_empty, in_islocal, in_sites, in_nats, original2current2,
                                            original_order2, original_order_realSites2);
            } catch (CUException &e) {
                not_possible_to_order_monomers = true;
            }
            REQUIRE(not_possible_to_order_monomers);
        }

        SECTION("Monomer vector size not matching atom or sites vector") {
            std::vector<std::string> mon_names2 = in_mons;
            std::vector<size_t> nats2(in_nats.size() + 1, 3);
            std::vector<size_t> sites2(in_nats.size() - 1, 3);
            std::vector<std::pair<std::string, size_t>> mon_type_count2;
            std::vector<size_t> original2current2;
            std::vector<std::pair<size_t, size_t>> original_order2;
            std::vector<std::pair<size_t, size_t>> original_order_realSites2;
            bool sites_vector_not_matching_monomer_size = false;
            try {
                mon_type_count2 = systools::OrderMonomers(in_mons, in_islocal, sites2, in_nats, original2current2,
                                                          original_order2, original_order_realSites2);
            } catch (CUException &e) {
                sites_vector_not_matching_monomer_size = true;
            }
            REQUIRE(sites_vector_not_matching_monomer_size);

            bool atoms_vector_not_matching_monomer_size = false;
            try {
                mon_type_count2 = systools::OrderMonomers(in_mons, in_islocal, in_sites, nats2, original2current2,
                                                          original_order2, original_order_realSites2);
            } catch (CUException &e) {
                atoms_vector_not_matching_monomer_size = true;
            }
            REQUIRE(atoms_vector_not_matching_monomer_size);
        }
    }
}

TEST_CASE("sys_tools::GetAdd") {
    SECTION("General behavior") {
        double add_water_12 = systools::GetAdd(true, false, false, "h2o");
        REQUIRE(add_water_12 == Approx(0.626).margin(TOL));

        double add_water_13 = systools::GetAdd(false, true, false, "h2o");
        REQUIRE(add_water_13 == Approx(0.055).margin(TOL));

        double add_water_14 = systools::GetAdd(false, false, true, "h2o");
        REQUIRE(add_water_14 == Approx(0.055).margin(TOL));

        double add_other_12 = systools::GetAdd(true, false, false, "co2");
        REQUIRE(add_other_12 == Approx(0.3).margin(TOL));

        double add_other_13 = systools::GetAdd(false, true, false, "co2");
        REQUIRE(add_other_13 == Approx(0.3).margin(TOL));

        double add_other_14 = systools::GetAdd(false, false, true, "co2");
        REQUIRE(add_other_14 == Approx(0.055).margin(TOL));
    }
}

TEST_CASE("sys_tools::FixMonomerCoordinates") {
    SECTION("General behavior: cubic box -> A=B=C=10.0, a=b=c=90.0") {
        std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        std::vector<double> box_inv = InvertUnitCell(box);

        std::vector<double> coordinates_fixed_1 = {4.9, 4.9, 4.9, 5.9, 4.9, 4.9, 4.9, 5.9, 4.9, 4.9, 4.9, 5.9};
        std::vector<double> coordinates_1 = {4.9, 4.9, 4.9, -4.1, 4.9, 4.9, 4.9, -4.1, 4.9, 4.9, 4.9, -4.1};
        std::vector<double> coordinates_fixed_2 = {-4.9, -4.9, -4.9, -5.9, -4.9, -4.9,
                                                   -4.9, -5.9, -4.9, -4.9, -4.9, -5.9};
        std::vector<double> coordinates_2 = {-4.9, -4.9, -4.9, 4.1, -4.9, -4.9, -4.9, 4.1, -4.9, -4.9, -4.9, 4.1};

        std::vector<double> coordinates_fixed_3 = {4.9, 4.9, 4.9, -4.9, -4.9, -4.9};
        std::vector<double> coordinates_3 = {-5.1, -5.1, -5.1, 5.1, 5.1, 5.1};
        std::vector<size_t> nats = {4};
        std::vector<size_t> nats_3 = {1, 1};
        std::vector<size_t> first_index = {0};
        std::vector<size_t> first_index_3 = {0, 1};

        systools::FixMonomerCoordinates(coordinates_1, box, box_inv, nats, first_index);
        for (size_t i = 0; i < coordinates_1.size(); i++) {
            REQUIRE(coordinates_1[i] == Approx(coordinates_fixed_1[i]).margin(TOL));
        }

        systools::FixMonomerCoordinates(coordinates_2, box, box_inv, nats, first_index);
        for (size_t i = 0; i < coordinates_2.size(); i++) {
            REQUIRE(coordinates_2[i] == Approx(coordinates_fixed_2[i]).margin(TOL));
        }

        systools::FixMonomerCoordinates(coordinates_3, box, box_inv, nats_3, first_index_3);
        for (size_t i = 0; i < coordinates_3.size(); i++) {
            REQUIRE(coordinates_3[i] == Approx(coordinates_fixed_3[i]).margin(TOL));
        }
    }

    SECTION("General behavior: hexagonal box -> A=B=10.0, C=20.0, a=b=90.0, c=60.0") {
        std::vector<double> box = {10.0, 0.0, 0.0, 5.0, 8.660254037844386, 0.0, 0.0, 0.0, 20.0};
        std::vector<double> box_inv = InvertUnitCell(box);

        std::vector<double> v1 = {10.0, 0.0, 0.0};
        std::vector<double> v2 = {5.0, 8.660254037844386, 0.0};
        std::vector<double> v3 = {0.0, 0.0, 20.0};

        // One monomer, 7 atoms 0,0,0 0,1,0 1,0,0 0,0,1 -1,0,0 0,-1,0 0,0,-1
        std::vector<double> s1_coordinates = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1};
        std::vector<double> s1_coordinates_fixed = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1};
        for (size_t i = 0; i < s1_coordinates.size() / 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                s1_coordinates[3 * i + j] += 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                s1_coordinates_fixed[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
            }
        }

        // One monomer, 7 atoms 0,0,0 0,1,0 1,0,0 0,0,1 -1,0,0 0,-1,0 0,0,-1
        std::vector<double> s2_coordinates = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1};
        std::vector<double> s2_coordinates_fixed = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1};
        for (size_t i = 0; i < s2_coordinates.size() / 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                s2_coordinates[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                s2_coordinates_fixed[3 * i + j] += 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
            }
        }

        std::vector<size_t> s1_s2_nats = {7};
        std::vector<size_t> s1_s2_first_index = {0};

        // Two monomer, 7 and 2 atoms
        std::vector<double> s3_m1_coordinates = {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, -1, 0, 0, 0, -1, 0, 0, 0, -1};
        std::vector<double> s3_m1_coordinates_fixed = {0, 0,  0, 0, 1, 0,  1, 0, 0, 0, 0,
                                                       1, -1, 0, 0, 0, -1, 0, 0, 0, -1};
        std::vector<double> s3_m2_coordinates = {0, 0, 0, -1, 1, 1};
        std::vector<double> s3_m2_coordinates_fixed = {0, 0, 0, -1, 1, 1};
        for (size_t i = 0; i < s3_m1_coordinates.size() / 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                s3_m1_coordinates[3 * i + j] -= 2.51 * v1[j] + 2.51 * v2[j];
                s3_m1_coordinates_fixed[3 * i + j] += 0.49 * v1[j] + 0.49 * v2[j];
            }
        }

        for (size_t i = 0; i < s3_m2_coordinates.size() / 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                s3_m2_coordinates[3 * i + j] += 2.51 * v1[j] + 2.51 * v2[j] - 0.51 * v3[j];
                s3_m2_coordinates_fixed[3 * i + j] += -0.49 * v1[j] + -0.49 * v2[j] + 0.49 * v3[j];
            }
        }

        std::vector<double> s3_coordinates = s3_m1_coordinates;
        std::vector<double> s3_coordinates_fixed = s3_m1_coordinates_fixed;

        for (size_t i = 0; i < s3_m2_coordinates.size(); i++) {
            s3_coordinates.push_back(s3_m2_coordinates[i]);
            s3_coordinates_fixed.push_back(s3_m2_coordinates_fixed[i]);
        }

        std::vector<size_t> s3_nats = {7, 2};
        std::vector<size_t> s3_first_index = {0, 7};

        systools::FixMonomerCoordinates(s1_coordinates, box, box_inv, s1_s2_nats, s1_s2_first_index);
        for (size_t i = 0; i < s1_coordinates.size(); i++) {
            REQUIRE(s1_coordinates[i] == Approx(s1_coordinates_fixed[i]).margin(TOL));
        }

        systools::FixMonomerCoordinates(s2_coordinates, box, box_inv, s1_s2_nats, s1_s2_first_index);
        for (size_t i = 0; i < s2_coordinates.size(); i++) {
            REQUIRE(s2_coordinates[i] == Approx(s2_coordinates_fixed[i]).margin(TOL));
        }

        systools::FixMonomerCoordinates(s3_coordinates, box, box_inv, s3_nats, s3_first_index);
        for (size_t i = 0; i < s3_coordinates.size(); i++) {
            REQUIRE(s3_coordinates[i] == Approx(s3_coordinates_fixed[i]).margin(TOL));
        }
    }

    SECTION("Assertions") {
        std::vector<size_t> nats_3 = {1, 1};
        std::vector<size_t> first_index_3 = {0, 1};
        std::vector<double> coordinates_3 = {-5.1, -5.1, -5.1, 5.1, 5.1, 5.1};
        std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        std::vector<double> box_inv = InvertUnitCell(box);
        SECTION("Box size must be 9") {
            std::vector<double> bad_box = {1.0, 3.0};
            bool box_size_is_not_9 = false;
            try {
                systools::FixMonomerCoordinates(coordinates_3, bad_box, box_inv, nats_3, first_index_3);
            } catch (CUException &e) {
                box_size_is_not_9 = true;
            }
            REQUIRE(box_size_is_not_9);
        }

        SECTION("Box inverse size must be 9") {
            std::vector<double> bad_box_inv = {1.0, 3.0};
            bool box_inverse_size_is_not_9 = false;
            try {
                systools::FixMonomerCoordinates(coordinates_3, box, bad_box_inv, nats_3, first_index_3);
            } catch (CUException &e) {
                box_inverse_size_is_not_9 = true;
            }
            REQUIRE(box_inverse_size_is_not_9);
        }

        SECTION("Number of atoms and first index vector sizes must match") {
            std::vector<size_t> bad_nat = {1};
            bool nat_and_first_index_sizes_not_equal = false;
            try {
                systools::FixMonomerCoordinates(coordinates_3, box, box_inv, bad_nat, first_index_3);
            } catch (CUException &e) {
                nat_and_first_index_sizes_not_equal = true;
            }
            REQUIRE(nat_and_first_index_sizes_not_equal);
        }

        SECTION("Number of coordinates must be correct") {
            std::vector<double> coords_short = {-5.1, -5.1, -5.1, 5.1, 5.1};
            std::vector<double> coords_long = {-5.1, -5.1, -5.1, 5.1, 5.1, 5.1, 5.1};
            bool coords_vector_is_too_short = false;
            try {
                systools::FixMonomerCoordinates(coords_short, box, box_inv, nats_3, first_index_3);
            } catch (CUException &e) {
                coords_vector_is_too_short = true;
            }
            REQUIRE(coords_vector_is_too_short);
        }
    }
}

TEST_CASE("sys_tools::GetCloseDimerImage") {
    SECTION("General behavior: cubic box") {
        std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        std::vector<double> box_inv = InvertUnitCell(box);

        std::vector<double> v1 = {10.0, 0.0, 0.0};
        std::vector<double> v2 = {0.0, 10.0, 0.0};
        std::vector<double> v3 = {0.0, 0.0, 10.0};

        SECTION("Monoatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0};
            std::vector<double> m2_coordinates = {1.0, 1.0, 1.0};
            std::vector<double> m2_coordinates_close = {1.0, 1.0, 1.0};

            size_t nat1 = 1;
            size_t nat2 = 1;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }

            systools::GetCloseDimerImage(box, box_inv, nat1, nat2, 1, m1_coordinates.data(), m2_coordinates.data());
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }

            systools::GetCloseDimerImage(box, box_inv, nat1, nat2, 1, m1_coordinates.data(), m2_coordinates.data());
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic, multiple dimers") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                                  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }

            systools::GetCloseDimerImage(box, box_inv, nat1, nat2, 2, m1_coordinates.data(), m2_coordinates.data());
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
        }
    }

    SECTION("General behavior: standard triclinic box") {
        std::vector<double> box = {10.0, 0.0, 0.0, 10.0, 15.0, 0.0, 5.0, 5.0, 20.0};
        std::vector<double> box_inv = InvertUnitCell(box);

        std::vector<double> v1 = {10.0, 0.0, 0.0};
        std::vector<double> v2 = {10.0, 15.0, 0.0};
        std::vector<double> v3 = {5.0, 5.0, 20.0};

        SECTION("Monoatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0};
            std::vector<double> m2_coordinates = {1.0, 1.0, 1.0};
            std::vector<double> m2_coordinates_close = {1.0, 1.0, 1.0};

            size_t nat1 = 1;
            size_t nat2 = 1;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }

            systools::GetCloseDimerImage(box, box_inv, nat1, nat2, 1, m1_coordinates.data(), m2_coordinates.data());
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }

            systools::GetCloseDimerImage(box, box_inv, nat1, nat2, 1, m1_coordinates.data(), m2_coordinates.data());
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic, multiple dimers") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                                  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }

            systools::GetCloseDimerImage(box, box_inv, nat1, nat2, 2, m1_coordinates.data(), m2_coordinates.data());
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
        }
    }
}

TEST_CASE("sys_tools::GetCloseTrimerImage") {
    SECTION("General behavior: cubic box") {
        std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        std::vector<double> box_inv = InvertUnitCell(box);

        std::vector<double> v1 = {10.0, 0.0, 0.0};
        std::vector<double> v2 = {0.0, 10.0, 0.0};
        std::vector<double> v3 = {0.0, 0.0, 10.0};

        SECTION("Monoatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0};
            std::vector<double> m2_coordinates = {1.0, 1.0, 1.0};
            std::vector<double> m2_coordinates_close = {1.0, 1.0, 1.0};
            std::vector<double> m3_coordinates = {-1.0, -1.0, -1.0};
            std::vector<double> m3_coordinates_close = {-1.0, -1.0, -1.0};

            size_t nat1 = 1;
            size_t nat2 = 1;
            size_t nat3 = 1;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                    m3_coordinates[3 * i + j] -= 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m3_coordinates_close[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                }
            }

            systools::GetCloseTrimerImage(box, box_inv, nat1, nat2, nat3, 1, m1_coordinates, m2_coordinates,
                                          m3_coordinates);
            for (size_t i = 0; i < m1_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
                REQUIRE(m3_coordinates[i] == Approx(m3_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m3_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};
            std::vector<double> m3_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;
            size_t nat3 = 4;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m3_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m3_coordinates[3 * i + j] -= 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m3_coordinates_close[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                }
            }

            systools::GetCloseTrimerImage(box, box_inv, nat1, nat2, nat3, 1, m1_coordinates, m2_coordinates,
                                          m3_coordinates);
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
            for (size_t i = 0; i < m3_coordinates.size(); i++) {
                REQUIRE(m3_coordinates[i] == Approx(m3_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic, multiple dimers") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                                  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m3_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0,
                                                  2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};
            std::vector<double> m3_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0,
                                                        2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;
            size_t nat3 = 4;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m3_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m3_coordinates[3 * i + j] -= 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m3_coordinates_close[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                }
            }

            systools::GetCloseTrimerImage(box, box_inv, nat1, nat2, nat3, 2, m1_coordinates, m2_coordinates,
                                          m3_coordinates);
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
            for (size_t i = 0; i < m3_coordinates.size(); i++) {
                REQUIRE(m3_coordinates[i] == Approx(m3_coordinates_close[i]).margin(TOL));
            }
        }
    }

    SECTION("General behavior: standard triclinic box") {
        std::vector<double> box = {10.0, 0.0, 0.0, 10.0, 15.0, 0.0, 5.0, 5.0, 20.0};
        std::vector<double> box_inv = InvertUnitCell(box);

        std::vector<double> v1 = {10.0, 0.0, 0.0};
        std::vector<double> v2 = {10.0, 15.0, 0.0};
        std::vector<double> v3 = {5.0, 5.0, 20.0};

        SECTION("Monoatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0};
            std::vector<double> m2_coordinates = {1.0, 1.0, 1.0};
            std::vector<double> m2_coordinates_close = {1.0, 1.0, 1.0};
            std::vector<double> m3_coordinates = {-1.0, -1.0, -1.0};
            std::vector<double> m3_coordinates_close = {-1.0, -1.0, -1.0};

            size_t nat1 = 1;
            size_t nat2 = 1;
            size_t nat3 = 1;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                    m3_coordinates[3 * i + j] -= 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m3_coordinates_close[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                }
            }

            systools::GetCloseTrimerImage(box, box_inv, nat1, nat2, nat3, 1, m1_coordinates, m2_coordinates,
                                          m3_coordinates);
            for (size_t i = 0; i < m1_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
                REQUIRE(m3_coordinates[i] == Approx(m3_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m3_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};
            std::vector<double> m3_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;
            size_t nat3 = 4;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m3_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m3_coordinates[3 * i + j] -= 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m3_coordinates_close[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                }
            }

            systools::GetCloseTrimerImage(box, box_inv, nat1, nat2, nat3, 1, m1_coordinates, m2_coordinates,
                                          m3_coordinates);
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
            for (size_t i = 0; i < m3_coordinates.size(); i++) {
                REQUIRE(m3_coordinates[i] == Approx(m3_coordinates_close[i]).margin(TOL));
            }
        }

        SECTION("Poliatomic, multiple dimers") {
            std::vector<double> m1_coordinates = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                                  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0};
            std::vector<double> m2_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m2_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 2.0};
            std::vector<double> m3_coordinates = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0,
                                                  2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};
            std::vector<double> m3_coordinates_close = {2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0,
                                                        2.0, 2.0, 2.0, 2.0, 3.0, 2.0, 2.0, 2.0, 3.0, 3.0, 2.0, 2.0};

            size_t nat1 = 3;
            size_t nat2 = 2;
            size_t nat3 = 4;

            for (size_t i = 0; i < m1_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m1_coordinates[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m2_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m2_coordinates[3 * i + j] += 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m2_coordinates_close[3 * i + j] -= 0.49 * v1[j] + 0.49 * v2[j] + 0.49 * v3[j];
                }
            }
            for (size_t i = 0; i < m3_coordinates.size() / 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    m3_coordinates[3 * i + j] -= 1.51 * v1[j] + 3.51 * v2[j] + 2.51 * v3[j];
                    m3_coordinates_close[3 * i + j] -= 0.51 * v1[j] + 0.51 * v2[j] + 0.51 * v3[j];
                }
            }

            systools::GetCloseTrimerImage(box, box_inv, nat1, nat2, nat3, 2, m1_coordinates, m2_coordinates,
                                          m3_coordinates);
            for (size_t i = 0; i < m2_coordinates.size(); i++) {
                REQUIRE(m2_coordinates[i] == Approx(m2_coordinates_close[i]).margin(TOL));
            }
            for (size_t i = 0; i < m3_coordinates.size(); i++) {
                REQUIRE(m3_coordinates[i] == Approx(m3_coordinates_close[i]).margin(TOL));
            }
        }
    }
}

TEST_CASE("sys_tools::ComparePair") {
    std::pair<size_t, double> a = std::make_pair(3, 1.0);
    std::pair<size_t, double> b = std::make_pair(5, 0.0);
    std::pair<size_t, double> c = std::make_pair(10, 4.2);
    std::pair<size_t, double> d = std::make_pair(1002, 9128.0);

    bool a_lt_b = systools::ComparePair(a, b);  // Should be true
    bool b_lt_a = systools::ComparePair(b, a);  // Should be false

    bool c_lt_c = systools::ComparePair(c, c);  // Should be false
    bool c_lt_d = systools::ComparePair(c, d);  // Should be true

    REQUIRE(a_lt_b);
    REQUIRE(!b_lt_a);
    REQUIRE(!c_lt_c);
    REQUIRE(c_lt_d);
}

TEST_CASE("sys_tools::AddClusters") {
    SECTION("Gas Phase") {
        SECTION("Monoatomic") {
            std::vector<double> xyz_monomer = {0.0, 0.0, 0.0};
            size_t nat = xyz_monomer.size() / 3;
            std::vector<double> box;
            std::vector<double> box_inv;
            std::vector<size_t> fi;

            // Generate a cube of side 2*length with coordinates in the vertices, mid of edges, and center of faces, and
            // the center of the cube
            double length = 1.0;
            size_t fi_current = 0;
            std::vector<double> xyz;
            for (size_t i = 0; i < 3; i++) {
                double dx = (double(i) - 1.0) * length;
                for (size_t j = 0; j < 3; j++) {
                    double dy = (double(j) - 1.0) * length;
                    for (size_t k = 0; k < 3; k++) {
                        double dz = (double(k) - 1.0) * length;
                        for (size_t l = 0; l < nat; l++) {
                            xyz.push_back(xyz_monomer[3 * l + 0] + dx);
                            xyz.push_back(xyz_monomer[3 * l + 1] + dy);
                            xyz.push_back(xyz_monomer[3 * l + 2] + dz);
                        }
                        fi.push_back(fi_current);
                        fi_current += nat;
                    }
                }
            }

            size_t nmon = xyz.size() / 3 / nat;
            std::vector<size_t> islocal(nat * nmon, 1);

            SECTION("Dimers") {
                SECTION("Cutoff short, no dimers") {
                    size_t n_max = 2;
                    double cutoff = 0.5 * length;
                    size_t istart = 0;
                    size_t iend = nmon;
                    bool use_pbc = box.size();
                    std::vector<size_t> dimers;
                    std::vector<size_t> trimers;
                    bool use_ghost = false;

                    systools::AddClusters(n_max, cutoff, istart, iend, nmon, use_pbc, box, box_inv, xyz, fi, islocal,
                                          dimers, trimers, use_ghost);
                    REQUIRE(dimers.size() == 0);
                    REQUIRE(trimers.size() == 0);
                }

                SECTION("Cutoff long, all possible dimers") {
                    size_t n_max = 2;
                    double cutoff = 10 * length;
                    size_t istart = 0;
                    size_t iend = nmon;
                    bool use_pbc = box.size();
                    std::vector<size_t> dimers;
                    std::vector<size_t> trimers;
                    bool use_ghost = false;

                    systools::AddClusters(n_max, cutoff, istart, iend, nmon, use_pbc, box, box_inv, xyz, fi, islocal,
                                          dimers, trimers, use_ghost);
                    REQUIRE(dimers.size() == nmon * (nmon - 1)); // Combinations of nmon elements in groups of 2 n!/(n-2)!/2! * 2
                    REQUIRE(trimers.size() == 0);
                }

                SECTION("Cutoff to get all the dimers below 1.1") {
                    size_t n_max = 2;
                    double cutoff = 1.01 * length;
                    size_t istart = 0;
                    size_t iend = nmon;
                    bool use_pbc = box.size();
                    std::vector<size_t> dimers;
                    std::vector<size_t> trimers;
                    bool use_ghost = false;

                    size_t expected_number_of_dimers = 0;
                    for (size_t i = 0; i < nmon-1; i++) {
                        for (size_t j = i+1; j < nmon; j++) {
                            double s = 0.0;
                            for (size_t k = 0; k<3 ; k++) {
                                s += fabs(xyz[3*fi[i] + k] - xyz[3*fi[j] + k]); 
                            }
                            if (s < cutoff) expected_number_of_dimers++;
                        }
                    }

                    systools::AddClusters(n_max, cutoff, istart, iend, nmon, use_pbc, box, box_inv, xyz, fi, islocal,
                                          dimers, trimers, use_ghost);
                    REQUIRE(dimers.size() == expected_number_of_dimers*2); 
                    REQUIRE(trimers.size() == 0);
                }
            }

            SECTION("Trimers") {
                SECTION("Cutoff short, no dimers or trimers") {
                    size_t n_max = 3;
                    double cutoff = 0.5 * length;
                    size_t istart = 0;
                    size_t iend = nmon;
                    bool use_pbc = box.size();
                    std::vector<size_t> dimers;
                    std::vector<size_t> trimers;
                    bool use_ghost = false;

                    systools::AddClusters(n_max, cutoff, istart, iend, nmon, use_pbc, box, box_inv, xyz, fi, islocal,
                                          dimers, trimers, use_ghost);
                    REQUIRE(dimers.size() == 0);
                    REQUIRE(trimers.size() == 0);
                }

                SECTION("Cutoff long, all possible dimers and trimers") {
                    size_t n_max = 3;
                    double cutoff = 10 * length;
                    size_t istart = 0;
                    size_t iend = nmon;
                    bool use_pbc = box.size();
                    std::vector<size_t> dimers;
                    std::vector<size_t> trimers;
                    bool use_ghost = false;

                    systools::AddClusters(n_max, cutoff, istart, iend, nmon, use_pbc, box, box_inv, xyz, fi, islocal,
                                          dimers, trimers, use_ghost);
                    REQUIRE(dimers.size() == nmon * (nmon - 1)); // Combinations of nmon elements in groups of 2 n!/(n-2)!/2! * 2
                    REQUIRE(trimers.size() == nmon * (nmon -1) * (nmon - 2) / 2); // Combinations of nmon elements in groups of 3 n!/(n-3)!/3! * 2
                }

                SECTION("Cutoff to get all the dimers and trimers below 1.1") {
                    size_t n_max = 3;
                    double cutoff = 1.01 * length;
                    size_t istart = 0;
                    size_t iend = nmon;
                    bool use_pbc = box.size();
                    std::vector<size_t> dimers;
                    std::vector<size_t> trimers;
                    bool use_ghost = false;

                    size_t expected_number_of_dimers = 0;
                    for (size_t i = 0; i < nmon-1; i++) {
                        for (size_t j = i+1; j < nmon; j++) {
                            double s = 0.0;
                            for (size_t k = 0; k<3 ; k++) {
                                s += fabs(xyz[3*fi[i] + k] - xyz[3*fi[j] + k]);
                            }
                            if (s < cutoff) expected_number_of_dimers++;
                        }
                    }

                    size_t expected_number_of_trimers = 0;
                    for (size_t i = 0; i < nmon-2; i++) {
                        for (size_t j = i+1; j < nmon-1; j++) {
                            for (size_t k = j+1; k < nmon; k++) {
                                double sij = 0.0;
                                double sik = 0.0;
                                double sjk = 0.0;
                                for (size_t l = 0; l<3 ; l++) {
                                    sij += fabs(xyz[3*fi[i] + l] - xyz[3*fi[j] + l]);
                                    sik += fabs(xyz[3*fi[i] + l] - xyz[3*fi[k] + l]);
                                    sjk += fabs(xyz[3*fi[j] + l] - xyz[3*fi[k] + l]);
                                }
                                if ( (sij < cutoff && sik < cutoff) || (sij < cutoff && sjk < cutoff) || (sik < cutoff && sjk < cutoff)) expected_number_of_trimers++;
                            }
                        }
                    }

                    systools::AddClusters(n_max, cutoff, istart, iend, nmon, use_pbc, box, box_inv, xyz, fi, islocal,
                                          dimers, trimers, use_ghost);
                    REQUIRE(dimers.size() == expected_number_of_dimers*2); 
                    REQUIRE(trimers.size() == expected_number_of_trimers * 3);
                }
            }
        }
    }
}

TEST_CASE("sys_tools::ChargeDerivativeForce") {
    SECTION("Single Water Monomer") {
        std::string mon = "h2o";
        size_t nmon = 1;
        size_t nsites = nmon*4;
        size_t first_index_sites = 0;
        size_t first_index_coordinates = 0;
        
        std::vector<double> phi(nsites,0.0);
        std::vector<double> grad(nsites*3,0.0);
        std::vector<double> xyz = {-1.5911730600e+00, 1.0743321600e+00, 0.0000000000e+00, -6.3591971000e-01, 9.7898520000e-01, 0.0000000000e+00, -1.9116276500e+00, 1.9792679900e+00, 0.0000000000e+00, -1.4557365602e+00, 1.2470607312e+00, 0.0000000000e+00};

        std::vector<double> chg_grad = {-1.8989400193e-01,-5.8492316288e-02,-0.0000000000e+00,-1.0353680537e-01,-6.7111909817e-02,-0.0000000000e+00,2.9343080730e-01,1.2560422610e-01,0.0000000000e+00,-4.0477684016e-02,-1.1655657767e-01,-0.0000000000e+00,-1.1507818891e-02,-1.9836494986e-01,-0.0000000000e+00,5.1985502907e-02,3.1492152753e-01,0.0000000000e+00,2.3037168594e-01,1.7504889395e-01,0.0000000000e+00,1.1504462426e-01,2.6547685968e-01,0.0000000000e+00,-3.4541631020e-01,-4.4052575363e-01,-0.0000000000e+00};
        std::vector<double> virial(9,0.0);
        
        systools::ChargeDerivativeForce(mon,nmon, first_index_coordinates, first_index_sites, phi, grad, chg_grad, xyz.data(), &virial);

        for (size_t i = 0; i < grad.size(); i++) {
            REQUIRE(grad[i] == Approx(0.0).margin(TOL));
        }
        for (size_t i = 0; i < virial.size(); i++) {
            REQUIRE(virial[i] == Approx(0.0).margin(TOL));
        }
    }

    SECTION("Two Water Monomers") {
        std::string mon = "h2o";
        size_t nmon = 2;
        size_t nsites = nmon*4;
        size_t first_index_sites = 0;
        size_t first_index_coordinates = 0;

        std::vector<double> phi = {-2.1326567137e-02,-4.3826925160e-02,-1.5115873713e-02,-2.2648794837e-02,4.2939245830e-02,2.1780574543e-02,4.1014905976e-02,3.7686839052e-02};
        std::vector<double> grad(nsites*3,0.0);
        std::vector<double> grad_expected = {-1.9273960629999998, -1.0401458249999997, 0.7649756429999996, 1.5751887596, 0.37091197700000045, -0.23931989299999934, 0.3522073040000002, 0.6692338485, -0.5256557496, 0.0, 0.0, 0.0, -1.6280496752999998, -1.0206275764000003, -0.7605410515000002, 1.3400508063999998, 0.41931509369999986, 0.2871619009999993, 0.2879988690000008, 0.60131248261, 0.47337915047100004, 0.0, 0.0, 0.0};
        std::vector<double> xyz = {-1.5897242500e+00,1.6492450700e+00,1.0433792200e+00,1.0859465600e+00,-8.7808400000e-02,0.0000000000e+00,-6.3591971000e-01,2.6087802600e+00,9.7898520000e-01,1.0958770400e+00,0.0000000000e+00,-2.8171150000e-02,-1.9006628000e+00,1.3383065300e+00,1.7450105000e+00,1.7875778400e+00,-6.6454990000e-01,5.7674150000e-01,-1.4525665789e+00,1.7876253971e+00,1.1793359822e+00,1.2377607100e+00,-1.9212395930e-01,1.1703937180e-01};

        std::vector<double> chg_grad = {-1.9040711750e-01,-4.7529900389e-02,3.1098570197e-02,-1.0418089900e-01,-5.3351273039e-02,3.9036659208e-02,2.9458801649e-01,1.0088117343e-01,-7.0135229406e-02,-4.1663776414e-02,-9.1216434033e-02,7.1885820762e-02,-1.3554183683e-02,-1.5464558539e-01,1.2402463639e-01,5.5217960097e-02,2.4586201943e-01,-1.9591045715e-01,2.3207089391e-01,1.3874633442e-01,-1.0298439096e-01,1.1773508268e-01,2.0799685843e-01,-1.6306129559e-01,-3.4980597659e-01,-3.4674319286e-01,2.6604568655e-01,-1.9073305635e-01,-5.7624631175e-02,-3.9215535777e-02,-8.0449950373e-02,-5.6483280224e-02,-4.2453353590e-02,2.7118300672e-01,1.1410791140e-01,8.1668889366e-02,-4.1733850419e-02,-7.7562002231e-02,-6.0887314328e-02,-5.9964726090e-03,-1.5820321163e-01,-1.2717445613e-01,4.7730323027e-02,2.3576521386e-01,1.8806177046e-01,2.3246690677e-01,1.3518663341e-01,1.0010285010e-01,8.6446422982e-02,2.1468649185e-01,1.6962780972e-01,-3.1891332975e-01,-3.4987312526e-01,-2.6973065983e-01};
        std::vector<double> virial(9,0.0);
        std::vector<double> virial_expected = {-2.5891833192, -0.36106328759999995, -0.0635323633, -0.36106328759999995, -0.8717345355000001, 0.018416471699999715, -0.0635323633, 0.018416471699999715, -0.54708090892};

        systools::ChargeDerivativeForce(mon,nmon, first_index_coordinates, first_index_sites, phi, grad, chg_grad, xyz.data(), &virial);

        for (size_t i = 0; i < grad.size(); i++) {
            REQUIRE(grad[i] == Approx(grad_expected[i]).margin(TOL));
        }
        for (size_t i = 0; i < virial.size(); i++) {
            REQUIRE(virial[i] == Approx(virial_expected[i]).margin(TOL));
        }
    }
}
























