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

#ifndef UNITTESTS_SETUP_H2O_1_F_1_H
#define UNITTESTS_SETUP_H2O_1_F_1_H

#define SETUP_H2O_1_F_1                                                                                        \
    double one_body_energy = 3.9139698364e-01;                                                                 \
    double two_body_energy = -1.7569884704e-01;                                                                \
    double three_body_energy = 0.0000000000e+00;                                                               \
    double dispersion_energy = -1.6532585284e-01;                                                              \
    double buckingham_energy = 9.4544594801e-02;                                                               \
    double electrostatic_energy = -3.3339628603e+00;                                                           \
    double total_energy = -3.2835905766e+00;                                                                   \
    double total_energy_ttm = -3.0133471347e+00;                                                               \
    const int n_monomers = 2;                                                                                  \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"f-", "h2o"}};                               \
    std::vector<std::vector<std::string>> ignore_2b = {{"f-", "h2o"}};                                          \
    std::vector<std::vector<std::string>> ignore_3b = {{"f-", "h2o", "h2o"}};                                   \
    std::vector<double> coords{0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  9.5953519000e-01,       \
                               9.9304800000e-03, -2.8171150000e-02, -3.1093854000e-01, 7.0163128000e-01,       \
                               5.7674150000e-01, 1.3838032710e-01,  1.5181414998e-01,  1.1703937180e-01,       \
                               0.0000000000e+00, 0.0000000000e+00,  4.0000000000e+00};                         \
    std::vector<double> real_coords{0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  9.5953519000e-01,  \
                                    9.9304800000e-03, -2.8171150000e-02, -3.1093854000e-01, 7.0163128000e-01,  \
                                    5.7674150000e-01, 0.0000000000e+00,  0.0000000000e+00,  4.0000000000e+00}; \
    std::vector<std::string> atom_names{"O", "H", "H", "F"};                                                   \
    std::vector<std::string> monomer_names = {"h2o", "f-"};                                                     \
    std::vector<size_t> islocal = {1, 1};
std::vector<size_t> n_atoms_vector = {3, 1};
std::vector<size_t> n_sites_vector = {4, 1};
std::vector<size_t> first_index = {0, 4};
std::vector<size_t> first_index_realSites = {0, 3};
const size_t n_atoms = 4;
const size_t n_sites = 5;
std::vector<double> polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00, 2.4669000000e+00};
std::vector<double> pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, 2.4669000000e+00};
std::vector<double> charges{0.0000000000e+00, 5.8639592027e-01, 5.8639592156e-01, -1.1727918418e+00, -1.0000000000e+00};
std::vector<double> real_polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 2.4669000000e+00};
std::vector<double> real_pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 2.4669000000e+00};
std::vector<double> real_charges{0.0000000000e+00, 5.8639592027e-01, 5.8639592156e-01, -1.0000000000e+00};
std::vector<double> C6_long_range{1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00, 2.5564127502e+01};
std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"h2o", 1}, {"f-", 1}};
std::vector<size_t> internal_original_to_current_order{0, 1};
std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}, {1, 4}};
std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}, {1, 3}};
std::vector<std::string> internal_monomer_names{"h2o", "f-"};
#endif
