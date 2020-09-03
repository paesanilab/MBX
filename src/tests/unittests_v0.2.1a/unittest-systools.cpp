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

            bool coords_vector_is_too_long = false;
            try {
                systools::FixMonomerCoordinates(coords_long, box, box_inv, nats_3, first_index_3);
            } catch (CUException &e) {
                coords_vector_is_too_long = true;
            }
            REQUIRE(coords_vector_is_too_long);
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
