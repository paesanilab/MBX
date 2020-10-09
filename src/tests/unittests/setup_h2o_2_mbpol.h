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

#ifndef UNITTESTS_SETUP_H2O_2_MBPOL_H
#define UNITTESTS_SETUP_H2O_2_MBPOL_H

#define SETUP_H2O_2_MBPOL                                                                                   \
    double one_body_energy = 2.0692536942e+00;                                                              \
    double two_body_energy = 1.6715908234e+00;                                                              \
    double three_body_energy = 0.0000000000e+00;                                                            \
    double dispersion_energy = -1.1353491370e+00;                                                           \
    double buckingham_energy = 0.0000000000e+00;                                                            \
    double electrostatic_energy = -2.5811552627e+00;                                                        \
    double total_energy = 2.4340117882e-02;                                                                 \
    double total_energy_ttm = 2.4340117882e-02;                                                             \
    const size_t n_monomers = 2;                                                                            \
    std::vector<std::string> atom_names{"O", "H", "H", "O", "H", "H"};                                      \
    std::vector<std::string> monomer_names = {"h2o", "h2o"};                                                \
    std::vector<size_t> islocal = {1, 1};                                                                   \
    std::vector<size_t> n_atoms_vector = {3, 3};                                                            \
    std::vector<size_t> n_sites_vector = {4, 4};                                                            \
    std::vector<size_t> first_index = {0, 4};                                                               \
    std::vector<size_t> first_index_realSites = {0, 3};                                                     \
    const size_t n_atoms = 6;                                                                               \
    const size_t n_sites = 8;                                                                               \
    std::vector<double> box = {15.0, 0.0, 0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 15.0};                             \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"h2o", n_monomers}};                        \
    std::vector<double> virial_1b = {-21.11278681, 21.40377629, -5.88668738, 21.40377629, -3.20344168,      \
                                     1.01648662,   -5.88668738, 1.01648662,  -3.40683556};                  \
    std::vector<double> virial_elec = {-9.96615679, 1.27681405, 1.78454589, 1.27681405, 1.35367352,         \
                                       2.13099187,  1.78454589, 2.13099187, 0.95763862};                    \
    std::vector<double> virial_elec_pbc = {-10.18097514, 1.12288241, 1.66385755, 1.12288241, 1.25711759,    \
                                           2.08840583,   1.66385755, 2.08840583, 0.97532983};               \
    std::vector<double> virial_disp_pbc = {-6.51400574, -0.61599665, -0.45173996, -0.61599665, -0.18810851, \
                                           -0.12214125, -0.45173996, -0.12214125, -0.11525956};             \
    std::vector<double> virial_2b = {14.17836998, 1.04493547, 0.4985588,   1.04493547, 0.09813098,          \
                                     -0.59318356, 0.4985588,  -0.59318356, 0.19999283};                     \
    std::vector<double> virial_disp = {-6.51128107, -0.61599187, -0.45178059, -0.61599187, -0.18641611,     \
                                       -0.12214221, -0.45178059, -0.12214221, -0.11356955};                 \
    std::vector<double> real_coords =                                                                       \
        std::vector<double>{-1.58972425, 1.04337922, -0.08780840, -0.63591971, 0.97898520, 0.00000000,      \
                            -1.90066280, 1.74501050, -0.66454990, 1.64924507,  1.08594656, 0.00000000,      \
                            2.60878026,  1.09587704, -0.02817115, 1.33830653,  1.78757784, 0.57674150};     \
    std::vector<double> coords =                                                                            \
        std::vector<double>{-1.5897242500e+00, 1.0433792200e+00,  -8.7808400000e-02, -6.3591971000e-01,     \
                            9.7898520000e-01,  0.0000000000e+00,  -1.9006628000e+00, 1.7450105000e+00,      \
                            -6.6454990000e-01, -1.4525665789e+00, 1.1793359822e+00,  -1.9212395930e-01,     \
                            1.6492450700e+00,  1.0859465600e+00,  0.0000000000e+00,  2.6087802600e+00,      \
                            1.0958770400e+00,  -2.8171150000e-02, 1.3383065300e+00,  1.7875778400e+00,      \
                            5.7674150000e-01,  1.7876253971e+00,  1.2377607100e+00,  1.1703937180e-01};     \
    std::vector<double> polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00,      \
                               1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00};     \
    std::vector<double> pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00,         \
                            1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00};        \
    std::vector<double> charges{0.0000000000e+00, 5.9876992622e-01, 5.9876992679e-01, -1.1975398530e+00,    \
                                0.0000000000e+00, 5.8639592027e-01, 5.8639592156e-01, -1.1727918418e+00};   \
    std::vector<double> real_polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,                   \
                                    1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                  \
    std::vector<double> real_pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01,                      \
                                 1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                     \
    std::vector<double> real_charges{0.0000000000e+00, 5.9876992622e-01, 5.9876992679e-01,                  \
                                     0.0000000000e+00, 5.8639592027e-01, 5.8639592156e-01};                 \
    std::vector<double> C6_long_range{1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00,                 \
                                      1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00};                \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"h2o", 2}};                        \
    std::vector<size_t> internal_original_to_current_order{0, 1};                                           \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}, {1, 4}};                          \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}, {1, 3}};               \
    std::vector<std::string> internal_monomer_names{"h2o", "h2o"};
#endif
