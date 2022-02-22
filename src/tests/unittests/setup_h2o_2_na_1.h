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

#ifndef UNITTESTS_SETUP_H2O_2_NA_1_H
#define UNITTESTS_SETUP_H2O_2_NA_1_H

#define SETUP_H2O_2_NA_1                                                                                              \
    double one_body_energy = 1.0803424170e-01;                                                                        \
    double two_body_energy = 2.8736601016e+00;                                                                        \
    double three_body_energy = 1.9055043094e-01;                                                                      \
    double dispersion_energy = -1.7064606812e+00;                                                                     \
    double buckingham_energy = 1.0327578345e+00;                                                                      \
    double electrostatic_energy = -2.9678893074e+01;                                                                  \
    double total_energy = -2.8213108981e+01;                                                                          \
    double total_energy_ttm = -2.8789337615e+01;                                                                      \
    const int n_monomers = 3;                                                                                         \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"na+", "h2o"}};                                     \
    std::vector<std::vector<std::string>> ignore_2b = {{"na+", "h2o"}};                                                \
    std::vector<std::vector<std::string>> ignore_3b = {{"na+", "h2o", "h2o"}};                                         \
    std::vector<double> coords{1.5980544090e-08,  5.3083405620e-08,  -4.0769717940e-08, 8.7887527370e-02,             \
                               -2.0220321020e+00, -2.0329422940e+00, 2.6040558380e-01,  -2.3586724350e+00,            \
                               -2.9133890250e+00, -2.4761302750e-01, -2.7638337240e+00, -1.5163609540e+00,            \
                               5.3114650505e-02,  -2.2521214040e+00, -2.1105742272e+00, -8.9158544110e-01,            \
                               -3.5899668850e+00, 4.1503146600e-01,  -1.8000740920e+00, -3.8966250020e+00,            \
                               4.8017063390e-01,  -3.8249819440e-01, -4.2501894050e+00, 8.9312177110e-01,             \
                               -9.7679910503e-01, -3.7962541959e+00, 5.3093134332e-01};                               \
    std::vector<double> real_coords{1.5980544090e-08,  5.3083405620e-08,  -4.0769717940e-08, 8.7887527370e-02,        \
                                    -2.0220321020e+00, -2.0329422940e+00, 2.6040558380e-01,  -2.3586724350e+00,       \
                                    -2.9133890250e+00, -2.4761302750e-01, -2.7638337240e+00, -1.5163609540e+00,       \
                                    -8.9158544110e-01, -3.5899668850e+00, 4.1503146600e-01,  -1.8000740920e+00,       \
                                    -3.8966250020e+00, 4.8017063390e-01,  -3.8249819440e-01, -4.2501894050e+00,       \
                                    8.9312177110e-01};                                                                \
    std::vector<std::string> atom_names{"Na", "O", "H", "H", "O", "H", "H"};                                          \
    std::vector<std::string> monomer_names = {"na+", "h2o", "h2o"};                                                    \
    std::vector<size_t> n_atoms_vector = {1, 3, 3};                                                                   \
    std::vector<size_t> n_sites_vector = {1, 4, 4};                                                                   \
    std::vector<size_t> first_index = {0, 1, 5};                                                                      \
    std::vector<size_t> first_index_realSites = {0, 1, 4};                                                            \
    const size_t n_atoms = 7;                                                                                         \
    const size_t n_sites = 9;                                                                                         \
    std::vector<double> polfac{                                                                                       \
        1.4760000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,                     \
        1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00};                                      \
    std::vector<double> pol{1.4760000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, \
                            1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00};                  \
    std::vector<double> charges{1.0000000000e+00, 0.0000000000e+00,  5.8013590789e-01,                                \
                                5.7940591362e-01, -1.1595418215e+00, 0.0000000000e+00,                                \
                                5.7555409696e-01, 5.7555409698e-01,  -1.1511081939e+00};                              \
    std::vector<double> real_polfac{1.4760000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,           \
                                    1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                            \
    std::vector<double> real_pol{1.4760000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,              \
                                 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                               \
    std::vector<double> real_charges{1.0000000000e+00, 0.0000000000e+00, 5.8013590789e-01, 5.7940591362e-01,          \
                                     0.0000000000e+00, 5.7555409696e-01, 5.7555409698e-01};                           \
    std::vector<double> C6_long_range{1.6027878723e+01, 1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00,         \
                                      1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00};                          \
    std::vector<double> virial_1b = {-5.58441, -1.81631, 2.40254, -1.81631, -4.42154,                                 \
                                     6.24358,  2.40254,  6.24358, -7.33326};                                          \
    std::vector<double> virial_2b = {2.40048,   1.73389,  -4.50587,  1.73389, 8.8705,                                 \
                                     -0.462716, -4.50587, -0.462716, 18.2374};                                        \
    std::vector<double> virial_3b = {-0.15670969213232,  0.353288315887017,  -0.567006694513452,                      \
                                     0.353288315887017,  1.24674516345045,   0.0368221203315391,                      \
                                     -0.567006694513452, 0.0368221203315391, 1.74858088055828};                       \
    std::vector<double> virial_elec = {1.41759,  -0.858596, 5.09369,  -0.858596, -28.7583,                            \
                                       -20.1132, 5.09369,   -20.1132, -36.3972};                                      \
    std::vector<double> virial_disp = {-0.763606, -1.04449, 1.89534, -1.04449, -3.03916,                              \
                                       1.57541,   1.89534,  1.57541, -6.24503};                                       \
    std::vector<double> virial_buck = {0.044093, -0.075866, -0.230262, -0.075866, 5.95973,                            \
                                       5.30237,  -0.230262, 5.30237,   5.3376};                                       \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"na+", 1}, {"h2o", 2}};                       \
    std::vector<size_t> internal_original_to_current_order{0, 1, 2};                                                  \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}, {1, 1}, {2, 5}};                            \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}, {1, 1}, {2, 4}};                 \
    std::vector<std::string> internal_monomer_names{"na+", "h2o", "h2o"};
#endif
