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

#ifndef UNITTESTS_SETUP_H2O_1_CS_1_H
#define UNITTESTS_SETUP_H2O_1_CS_1_H

#define SETUP_H2O_1_CS_1                                                                                               \
    double one_body_energy = 1.0718689712e-02;                                                                         \
    double two_body_energy = 1.8014372674e-01;                                                                         \
    double three_body_energy = 0.0000000000e+00;                                                                       \
    double dispersion_energy = -3.3470831242e-01;                                                                      \
    double buckingham_energy = 4.8108608305e-01;                                                                       \
    double electrostatic_energy = -9.8280059598e+00;                                                                   \
    double total_energy = -9.9718518557e+00;                                                                           \
    double total_energy_ttm = -9.6709094994e+00;                                                                       \
    const int n_monomers = 2;                                                                                          \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"cs+", "h2o"}};                                      \
    std::vector<std::vector<std::string>> ignore_2b = {{"cs+", "h2o"}};                                                 \
    std::vector<std::vector<std::string>> ignore_3b = {{"cs+", "h2o", "h2o"}};                                          \
    std::vector<double> coords{1.1292506410e+00,  -2.6597104230e+00, -7.9736006350e-01, 8.5503188680e-01,              \
                               -3.5764630390e+00, -8.7975543530e-01, 2.0219157720e+00,  -2.6519239090e+00,             \
                               -1.1516320130e+00, 1.2611983036e+00,  -2.8536414686e+00, -8.9052453907e-01,             \
                               -4.0992737580e-01, 6.0250776290e-01,  2.8940956800e-01};                                \
    std::vector<double> real_coords{1.1292506410e+00,  -2.6597104230e+00, -7.9736006350e-01, 8.5503188680e-01,         \
                                    -3.5764630390e+00, -8.7975543530e-01, 2.0219157720e+00,  -2.6519239090e+00,        \
                                    -1.1516320130e+00, -4.0992737580e-01, 6.0250776290e-01,  2.8940956800e-01};        \
    std::vector<std::string> atom_names{"O", "H", "H", "Cs"};                                                          \
    std::vector<std::string> monomer_names = {"h2o", "cs+"};                                                            \
    std::vector<size_t> n_atoms_vector = {3, 1};                                                                       \
    std::vector<size_t> n_sites_vector = {4, 1};                                                                       \
    std::vector<size_t> first_index = {0, 4};                                                                          \
    std::vector<size_t> first_index_realSites = {0, 3};                                                                \
    const size_t n_atoms = 4;                                                                                          \
    const size_t n_sites = 5;                                                                                          \
    std::vector<double> polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,                 \
                               2.3660000000e+00};                                                                      \
    std::vector<double> pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, 2.3660000000e+00}; \
    std::vector<double> charges{0.0000000000e+00, 5.7511954207e-01, 5.7511954216e-01, -1.1502390842e+00,               \
                                1.0000000000e+00};                                                                     \
    std::vector<double> real_polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 2.3660000000e+00};           \
    std::vector<double> real_pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 2.3660000000e+00};              \
    std::vector<double> real_charges{0.0000000000e+00, 5.7511954207e-01, 5.7511954216e-01, 1.0000000000e+00};          \
    std::vector<double> C6_long_range{1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00, 6.5762558189e+01};         \
    std::vector<double> virial_1b = {-2.25373, -0.098751, 0.877896, -0.098751, -2.55759,                               \
                                     -0.49551, 0.877896,  -0.49551, -0.45364};                                         \
    std::vector<double> virial_2b = {0.609642, -0.52781,  -0.292091, -0.52781, 1.78304,                                \
                                     0.4733,   -0.292091, 0.4733,    0.17854};                                         \
    std::vector<double> virial_3b = {0, 0, 0, 0, 0, 0, 0, 0, 0};                                                       \
    std::vector<double> virial_elec = {-1.90935, 6.70993, 1.74504,  6.70993, -16.1033,                                 \
                                       -4.95037, 1.74504, -4.95037, -1.41505};                                         \
    std::vector<double> virial_disp = {-0.346724, 0.672904, 0.236766,  0.672904, -1.39149,                             \
                                       -0.473235, 0.236766, -0.473235, -0.163905};                                     \
    std::vector<double> virial_buck = {0.931849, -1.95515,  -0.655256, -1.95515, 4.12684,                              \
                                       1.37863,  -0.655256, 1.37863,   0.461358};                                      \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"h2o", 1}, {"cs+", 1}};                        \
    std::vector<size_t> internal_original_to_current_order{0, 1};                                                      \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}, {1, 4}};                                     \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}, {1, 3}};                          \
    std::vector<std::string> internal_monomer_names{"h2o", "cs+"};
#endif
