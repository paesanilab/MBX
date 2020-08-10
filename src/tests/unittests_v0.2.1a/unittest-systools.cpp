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

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

constexpr double TOL = 1E-6;

TEST_CASE("Test the system tools functions (no PBC).") {
    // Create the bromide -- water system
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

    // Prepare variables for OrderMonomers
    std::vector<std::pair<std::string, size_t>> mon_type_count;
    std::vector<size_t> original2current;
    std::vector<std::pair<size_t, size_t>> orginal_order;
    std::vector<std::pair<size_t, size_t>> orginal_order_realSites;
    std::vector<std::string> mon_names = monomer_names;
    // Run OrderMonomers
    try {
        mon_type_count = systools::OrderMonomers(mon_names, islocal, sites_out, nat_out, original2current,
                                                 orginal_order, orginal_order_realSites);
    } catch (CUException &e) {
        std::cerr << e.what();
    }

    SECTION("SetupMonomers") {
        SECTION("Sites Vector") { REQUIRE(VectorsAreEqual(sites_out, n_sites_vector)); }

        SECTION("Number of Atoms Vector") { REQUIRE(VectorsAreEqual(nat_out, n_atoms_vector)); }

        SECTION("First Index Vector") { REQUIRE(VectorsAreEqual(first_index_out, first_index_realSites)); }

        SECTION("Set up an empty monomer list") {
            std::vector<std::string> monomers_empty;
            bool not_possible_to_setup_monomers = false;
            try {
                systools::SetUpMonomers(monomers_empty, sites_out, nat_out, first_index_out);
            } catch (CUException &e) {
                not_possible_to_setup_monomers = true;
            }
            REQUIRE(not_possible_to_setup_monomers);
        }

        SECTION("Add a monomer not in the database") {
            std::vector<std::string> monomers_not_in_db = {"not", "in", "data", "base", "at", "all"};
            bool monomer_not_in_database = false;
            try {
                systools::SetUpMonomers(monomers_not_in_db, sites_out, nat_out, first_index_out);
            } catch (CUException &e) {
                monomer_not_in_database = true;
            }
            REQUIRE(monomer_not_in_database);
        }
    }

    SECTION("OrderMonomers") {
        SECTION("Monomer type count") { REQUIRE(VectorsAreEqual(internal_mon_type_count, mon_type_count)); }

        SECTION("Internal Order monomer names") { REQUIRE(VectorsAreEqual(mon_names, internal_monomer_names)); }

        SECTION("Original to current vector") {
            REQUIRE(VectorsAreEqual(original2current, internal_original_to_current_order));
        }

        SECTION("Original Order Vector") { REQUIRE(VectorsAreEqual(orginal_order, internal_orginal_order)); }

        SECTION("Original Order for Real Sites Vector") {
            REQUIRE(VectorsAreEqual(orginal_order_realSites, internal_original_order_realSites));
        }

        SECTION("Monomer vector is empty") {
            std::vector<std::string> monomers_empty;
            std::vector<std::pair<std::string, size_t>> mon_type_count2;
            std::vector<size_t> original2current2;
            std::vector<std::pair<size_t, size_t>> orginal_order2;
            std::vector<std::pair<size_t, size_t>> orginal_order_realSites2;
            std::vector<size_t> islocal_empty;
            bool not_possible_to_order_monomers = false;
            try {
                mon_type_count2 = systools::OrderMonomers(monomers_empty, islocal_empty, sites_out, nat_out,
                                                          original2current2, orginal_order2, orginal_order_realSites2);
            } catch (CUException &e) {
                not_possible_to_order_monomers = true;
            }
            REQUIRE(not_possible_to_order_monomers);
        }

        SECTION("Monomer vector size not matching atom or sites vector") {
            std::vector<std::string> mon_names2 = monomer_names;
            std::vector<size_t> nats2(nat_out.size() + 1, 3);
            std::vector<size_t> sites2(nat_out.size() - 1, 3);
            std::vector<std::pair<std::string, size_t>> mon_type_count2;
            std::vector<size_t> original2current2;
            std::vector<std::pair<size_t, size_t>> orginal_order2;
            std::vector<std::pair<size_t, size_t>> orginal_order_realSites2;
            bool sites_vector_not_matching_monomer_size = false;
            try {
                mon_type_count2 = systools::OrderMonomers(monomer_names, islocal, sites2, nat_out, original2current2,
                                                          orginal_order2, orginal_order_realSites2);
            } catch (CUException &e) {
                sites_vector_not_matching_monomer_size = true;
            }
            REQUIRE(sites_vector_not_matching_monomer_size);

            bool atoms_vector_not_matching_monomer_size = false;
            try {
                mon_type_count2 = systools::OrderMonomers(monomer_names, islocal, sites_out, nats2, original2current2,
                                                          orginal_order2, orginal_order_realSites2);
            } catch (CUException &e) {
                atoms_vector_not_matching_monomer_size = true;
            }
            REQUIRE(atoms_vector_not_matching_monomer_size);
        }
    }

    SECTION("Test the thole damping retrievement") {
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

    // SECTION("Energy without gradients") { REQUIRE(energy_nograd == Approx(one_body_energy).margin(TOL)); }
}

TEST_CASE("Test functions with PBC") {
    SECTION("FixMonomerCoordinates") {
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
}
