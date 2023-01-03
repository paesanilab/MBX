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

#ifndef UNITTESTS_SETUP_CO2_1_H
#define UNITTESTS_SETUP_CO2_1_H

#define SETUP_CO2_1                                                                                           \
    double classic_energy = 10.28986;                                                                         \
    double one_body_energy = 9.9150780942e+00;                                                                \
    double two_body_energy = 0.0000000000e+00;                                                                \
    double three_body_energy = 0.0000000000e+00;                                                              \
    double dispersion_energy = 0.0000000000e+00;                                                              \
    double buckingham_energy = 0.0000000000e+00;                                                              \
    double electrostatic_energy = 0.0000000000e+00;                                                           \
    double total_energy = 9.9150780942e+00;                                                                   \
    double total_energy_ttm = 10.28986;                                                                       \
    const int n_monomers = 1;                                                                                 \
    std::vector<std::string> ff_mon = {"co2_archive"};                                                        \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"co2_archive", "h2o"}};                    \
    std::vector<std::string> ignore_1b = {"co2_archive"};                                                     \
    std::vector<std::vector<std::string>> ignore_2b = {{"co2_archive", "h2o"}};                               \
    std::vector<std::vector<std::string>> ignore_3b = {{"co2_archive", "h2o", "h2o"}};                        \
    std::vector<double> coords{-3.7350600000e-03, 2.1493800000e-02,  -2.0987560000e-02,                       \
                               1.1921700000e-03,  -7.0306800000e-03, 1.2319206500e+00,                        \
                               1.1921700000e-03,  -7.0306800000e-03, -1.2182866600e+00};                      \
    std::vector<double> real_coords{-3.7350600000e-03, 2.1493800000e-02,  -2.0987560000e-02,                  \
                                    1.1921700000e-03,  -7.0306800000e-03, 1.2319206500e+00,                   \
                                    1.1921700000e-03,  -7.0306800000e-03, -1.2182866600e+00};                 \
    std::vector<std::string> atom_names{"C", "O", "O"};                                                       \
    std::vector<std::string> monomer_names = {"co2_archive"};                                                 \
    std::vector<size_t> n_atoms_vector = {3};                                                                 \
    std::vector<size_t> n_sites_vector = {3};                                                                 \
    std::vector<size_t> first_index = {0};                                                                    \
    std::vector<size_t> first_index_realSites = {0};                                                          \
    const size_t n_atoms = 3;                                                                                 \
    const size_t n_sites = 3;                                                                                 \
    std::vector<double> polfac{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01};                         \
    std::vector<double> pol{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01};                            \
    std::vector<double> charges{7.0602700000e-01, -3.5301350000e-01, -3.5301350000e-01};                      \
    std::vector<double> real_charges{0.0000000000e+00, 5.7511954207e-01, 5.7511954216e-01, 1.0000000000e+00}; \
    std::vector<double> real_polfac{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01};                    \
    std::vector<double> real_pol{1.4716770000e+00, 7.6979000000e-01, 7.6979000000e-01};                       \
    std::vector<double> C6_long_range{1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01};                  \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"co2_archive", 1}};                  \
    std::vector<size_t> internal_original_to_current_order{0};                                                \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}};                                    \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}};                         \
    std::vector<std::string> internal_monomer_names{"co2_archive"};                                           \
    eff::Bond co2_bond1("bond", std::vector<size_t>{1, 2}, "morse");                                          \
    eff::Bond co2_bond2("bond", std::vector<size_t>{1, 3}, "morse");                                          \
    eff::Angles co2_angle1("angle", std::vector<size_t>{2, 1, 3}, "harm");                                    \
    std::vector<double> bond_morse_linear_parameters = {189.05};                                              \
    std::vector<double> bond_morse_nonlinear_parameters = {2.57, 1.16};                                       \
    std::vector<double> angle_harm_linear_parameters1 = {87.87};                                              \
    std::vector<double> angle_harm_nonlinear_parameters1 = {3.14159265};
#endif
