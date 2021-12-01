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

#ifndef UNITTESTS_SETUP_CH4_1_H
#define UNITTESTS_SETUP_CH4_1_H

#define SETUP_CH4_1                                                                                                    \
    double one_body_energy = 1.8143669355e+01;                                                                         \
    double two_body_energy = 0.0000000000e+00;                                                                         \
    double three_body_energy = 0.0000000000e+00;                                                                       \
    double dispersion_energy = 0.0000000000e+00;                                                                       \
    double buckingham_energy = 0.0000000000e+00;                                                                       \
    double electrostatic_energy = 0.0000000000e+00;                                                                    \
    double total_energy = 1.8143669355e+01;                                                                            \
    double total_energy_ttm = 1.8143669355e+01;                                                                        \
    const int n_monomers = 1;                                                                                          \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"ch4", "h2o"}};                                     \
    std::vector<std::vector<std::string>> ignore_2b = {{"ch4", "h2o"}};                                                \
    std::vector<std::vector<std::string>> ignore_3b = {{"ch4", "h2o", "h2o"}};                                         \
    std::vector<double> coords{1.2627750000e-02,  -2.2268900000e-02, 2.6028000000e-03,  -5.6203350000e-02,             \
                               1.1735084800e+00,  1.0092935000e-01,  -6.7453970000e-02, 2.0652190000e-02,              \
                               -9.6204840000e-01, 8.5135962000e-01,  -4.0337395000e-01, 4.4275125000e-01,              \
                               -8.7027661000e-01, -5.1615751000e-01, 3.7560324000e-01};                                \
    std::vector<double> real_coords{1.2627750000e-02,  -2.2268900000e-02, 2.6028000000e-03,  -5.6203350000e-02,        \
                                    1.1735084800e+00,  1.0092935000e-01,  -6.7453970000e-02, 2.0652190000e-02,         \
                                    -9.6204840000e-01, 8.5135962000e-01,  -4.0337395000e-01, 4.4275125000e-01,         \
                                    -8.7027661000e-01, -5.1615751000e-01, 3.7560324000e-01};                           \
    std::vector<std::string> atom_names{"C", "H", "H", "H", "H"};                                                      \
    std::vector<std::string> monomer_names = {"ch4"};                                                                  \
    std::vector<size_t> n_atoms_vector = {5};                                                                          \
    std::vector<size_t> n_sites_vector = {5};                                                                          \
    std::vector<size_t> first_index = {0};                                                                             \
    std::vector<size_t> first_index_realSites = {0};                                                                   \
    const size_t n_atoms = 5;                                                                                          \
    const size_t n_sites = 5;                                                                                          \
    std::vector<double> polfac{1.3932677000e+00, 3.8978363000e-01, 3.8978363000e-01, 3.8978363000e-01,                 \
                               3.8978363000e-01};                                                                      \
    std::vector<double> pol{1.3932677000e+00, 3.8978363000e-01, 3.8978363000e-01, 3.8978363000e-01, 3.8978363000e-01}; \
    std::vector<double> charges{-5.3857300000e-01, 1.3464325000e-01, 1.3464325000e-01, 1.3464325000e-01,               \
                                1.3464325000e-01};                                                                     \
    std::vector<double> real_charges{-5.3857300000e-01, 1.3464325000e-01, 1.3464325000e-01, 1.3464325000e-01,          \
                                     1.3464325000e-01};                                                                \
    std::vector<double> real_polfac{1.3932677000e+00, 3.8978363000e-01, 3.8978363000e-01, 3.8978363000e-01,            \
                                    3.8978363000e-01};                                                                 \
    std::vector<double> real_pol{1.3932677000e+00, 3.8978363000e-01, 3.8978363000e-01, 3.8978363000e-01,               \
                                 3.8978363000e-01};                                                                    \
    std::vector<double> C6_long_range{1.7413988630e+01, 6.0647480370e+00, 6.0647480370e+00, 6.0647480370e+00,          \
                                      6.0647480370e+00};                                                               \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"ch4", 1}};                                   \
    std::vector<size_t> internal_original_to_current_order{0};                                                         \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}};                                             \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}};                                  \
    std::vector<std::string> internal_monomer_names{"ch4"};
#endif
