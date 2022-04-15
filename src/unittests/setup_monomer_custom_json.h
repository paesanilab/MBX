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

#ifndef UNITTESTS_SETUP_MON_JSON_H
#define UNITTESTS_SETUP_MON_JSON_H

#define SETUP_MON_JSON                                                                                          \
    nlohmann::json user_j = {{"mymon",                                                                          \
                              {{"sites", 5},                                                                    \
                               {"nat", 5},                                                                      \
                               {"exc12", {{0, 1}, {2, 3}}},                                                     \
                               {"exc13", {{0, 2}, {1, 4}}},                                                     \
                               {"exc14", {{0, 3}, {1, 5}}},                                                     \
                               {"charges", {0.1, -0.2, 0.3, -0.4, 0.5}},                                        \
                               {"pol", {-0.1, 0.2, -0.3, 0.4, -0.5}},                                           \
                               {"polfac", {-0.11, 0.21, -0.31, 0.41, -0.51}},                                   \
                               {"c6lr", {10.0, 20.0, 30.0, 40.0, 50.0}}}}};                                     \
    std::vector<std::string> atom_names = {"C", "H", "O", "He", "I"};                                           \
    std::vector<double> atom_masses = {12.0000000, 1.007825032, 15.994914619, 4.00260325413, 126.904471};       \
    std::vector<std::string> monomer_names = {"mymon"};                                                         \
    const int n_monomers = monomer_names.size();                                                                \
    std::vector<size_t> islocal = {1, 1, 1, 1, 1};                                                              \
    std::vector<size_t> n_atoms_vector = {5};                                                                   \
    std::vector<size_t> n_sites_vector = {5};                                                                   \
    std::vector<size_t> first_index = {0};                                                                      \
    std::vector<size_t> first_index_realSites = {0};                                                            \
    const size_t n_atoms = 5;                                                                                   \
    const size_t n_sites = 5;                                                                                   \
    std::vector<double> coords{0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};      \
    std::vector<double> real_coords{0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}; \
    std::vector<double> polfac{-0.11, 0.21, -0.31, 0.41, -0.51};                                                \
    std::vector<double> pol{-0.1, 0.2, -0.3, 0.4, -0.5};                                                        \
    std::vector<double> charges{0.1, -0.2, 0.3, -0.4, 0.5};                                                     \
    std::vector<double> real_polfac{-0.11, 0.21, -0.31, 0.41, -0.51};                                           \
    std::vector<double> real_pol{-0.1, 0.2, -0.3, 0.4, -0.5};                                                   \
    std::vector<double> real_charges{0.1, -0.2, 0.3, -0.4, 0.5};                                                \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"mymon", 1}};                          \
    std::vector<size_t> internal_original_to_current_order{0};                                                  \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}};                                      \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}};                           \
    std::vector<double> C6_long_range{10.0, 20.0, 30.0, 40.0, 50.0};                                            \
    std::vector<std::string> internal_monomer_names{"mymon"};
#endif
