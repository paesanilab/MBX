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

#ifndef UNITTESTS_SETUP_H2O_2_K_1_H
#define UNITTESTS_SETUP_H2O_2_K_1_H

#define SETUP_H2O_2_K_1                                                                                               \
    double one_body_energy = 1.0803424170e-01;                                                                        \
    double two_body_energy = 4.2055773093e+00;                                                                        \
    double three_body_energy = 2.5989082359e-01;                                                                      \
    double dispersion_energy = -2.3266025337e+00;                                                                     \
    double buckingham_energy = 3.2125022331e+00;                                                                      \
    double electrostatic_energy = -2.9939326850e+01;                                                                  \
    double total_energy = -2.7692427010e+01;                                                                          \
    double total_energy_ttm = -2.7490168845e+01;                                                                      \
    const int n_monomers = 3;                                                                                         \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"k+", "h2o"}};                                      \
    std::vector<std::vector<std::string>> ignore_2b = {{"k+", "h2o"}};                                                 \
    std::vector<std::vector<std::string>> ignore_3b = {{"k+", "h2o", "h2o"}};                                          \
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
    std::vector<std::string> atom_names{"K", "O", "H", "H", "O", "H", "H"};                                           \
    std::vector<std::string> monomer_names = {"k+", "h2o", "h2o"};                                                     \
    std::vector<size_t> n_atoms_vector = {1, 3, 3};                                                                   \
    std::vector<size_t> n_sites_vector = {1, 4, 4};                                                                   \
    std::vector<size_t> first_index = {0, 1, 5};                                                                      \
    std::vector<size_t> first_index_realSites = {0, 1, 4};                                                            \
    const size_t n_atoms = 7;                                                                                         \
    const size_t n_sites = 9;                                                                                         \
    std::vector<double> polfac{                                                                                       \
        8.1840000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,                     \
        1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00};                                      \
    std::vector<double> pol{8.1840000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00, \
                            1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00};                  \
    std::vector<double> charges{1.0000000000e+00, 0.0000000000e+00,  5.8013590789e-01,                                \
                                5.7940591362e-01, -1.1595418215e+00, 0.0000000000e+00,                                \
                                5.7555409696e-01, 5.7555409698e-01,  -1.1511081939e+00};                              \
    std::vector<double> real_polfac{8.1840000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,           \
                                    1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                            \
    std::vector<double> real_pol{8.1840000000e-01, 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,              \
                                 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                               \
    std::vector<double> real_charges{1.0000000000e+00, 0.0000000000e+00, 5.8013590789e-01, 5.7940591362e-01,          \
                                     0.0000000000e+00, 5.7555409696e-01, 5.7555409698e-01};                           \
    std::vector<double> C6_long_range{3.7631363500e+01, 1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00,         \
                                      1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00};                          \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"k+", 1}, {"h2o", 2}};                        \
    std::vector<size_t> internal_original_to_current_order{0, 1, 2};                                                  \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}, {1, 1}, {2, 5}};                            \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}, {1, 1}, {2, 4}};                 \
    std::vector<std::string> internal_monomer_names{"k+", "h2o", "h2o"};
#endif
