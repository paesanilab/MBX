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

#ifndef UNITTESTS_SETUP_DUMMY_1_H
#define UNITTESTS_SETUP_DUMMY_1_H

#define SETUP_DUMMY_1                                                                  \
    double one_body_energy = 0.0000000000e+00;                                         \
    double two_body_energy = 0.0000000000e+00;                                         \
    double three_body_energy = 0.0000000000e+00;                                       \
    double dispersion_energy = 0.0000000000e+00;                                       \
    double buckingham_energy = 0.0000000000e+00;                                       \
    double electrostatic_energy = 0.0000000000e+00;                                    \
    double total_energy = 0.0000000000e+00;                                            \
    double total_energy_ttm = 0.0000000000e+00;                                        \
    const int n_monomers = 1;                                                          \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"dummy", "h2o"}};   \
    std::vector<std::vector<std::string>> ignore_2b = {{"dummy", "h2o"}};              \
    std::vector<std::vector<std::string>> ignore_3b = {{"dummy", "h2o", "h2o"}};       \
    std::vector<double> coords{0.0, 0.0, 0.0};                                         \
    std::vector<double> real_coords{0.0, 0.0, 0.0};                                    \
    std::vector<std::string> atom_names{"X"};                                          \
    std::vector<std::string> monomer_names = {"dummy"};                                \
    std::vector<size_t> n_atoms_vector = {1};                                          \
    std::vector<size_t> n_sites_vector = {1};                                          \
    std::vector<size_t> first_index = {0};                                             \
    std::vector<size_t> first_index_realSites = {0};                                   \
    const size_t n_atoms = 3;                                                          \
    const size_t n_sites = 3;                                                          \
    std::vector<double> polfac{0.0};                                                   \
    std::vector<double> pol{0.0};                                                      \
    std::vector<double> charges{0.0};                                                  \
    std::vector<double> real_charges{0.0};                                             \
    std::vector<double> real_polfac{0.0};                                              \
    std::vector<double> real_pol{0.0};                                                 \
    std::vector<double> C6_long_range{0.0};                                            \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"dummy", 1}}; \
    std::vector<size_t> internal_original_to_current_order{0};                         \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}};             \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}};  \
    std::vector<std::string> internal_monomer_names{"dummy"};
#endif
