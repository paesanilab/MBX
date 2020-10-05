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

#ifndef UNITTESTS_SETUP_H2O_4_MBPOL_H
#define UNITTESTS_SETUP_H2O_4_MBPOL_H

#define SETUP_H2O_4_MBPOL                                                                                           \
    const size_t n_monomers = 4;                                                                                    \
    std::vector<std::string> atom_names{"O", "H", "H", "O", "H", "H", "O", "H", "H", "O", "H", "H"};                \
    std::vector<std::string> monomer_names = {"h2o", "h2o", "h2o", "h2o"};                                          \
    std::vector<size_t> n_atoms_vector = {3, 3, 3, 3};                                                              \
    std::vector<size_t> n_sites_vector = {4, 4, 4, 4};                                                              \
    std::vector<size_t> first_index = {0, 4, 8, 12};                                                                \
    std::vector<size_t> first_index_realSites = {0, 3, 6, 9};                                                       \
    std::vector<double> box = {15.0, 0.0, 0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 15.0};                                     \
    const size_t n_atoms = 12;                                                                                      \
    const size_t n_sites = 16;                                                                                      \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"h2o", n_monomers}};                                \
    std::vector<double> virial_2b = {142.5164914, -2.77679254, 20.36809273, -2.77679254, 143.21701721,              \
                                     18.01441205, 20.36809273, 18.01441205, 15.16367113};                           \
    std::vector<double> virial_1b = {-65.6751912, 1.01228011,  -7.29605641, 1.01228011,  -65.90463671,              \
                                     -6.50700287, -7.29605641, -6.50700287, -19.74820746};                          \
    std::vector<double> virial_3b = {1.94198136,  0.01424641,  -0.11033437, 0.01424641, 1.94777964,                 \
                                     -0.09726458, -0.11033437, -0.09726458, 2.63176386};                            \
    std::vector<double> virial_disp = {-44.56931166, 0.91999283,  -6.74421606, 0.91999283, -44.79899618,            \
                                       -5.96481836,  -6.74421606, -5.96481836, -2.4001912};                         \
    std::vector<double> virial_disp_pbc = {-44.57839388, 0.91995937,  -6.74407266, 0.91995937, -44.80807839,        \
                                           -5.96467495,  -6.74407266, -5.96467495, -2.40690727};                    \
    std::vector<double> virial_elec = {-60.22370937, 0.93956023,  -7.00282027, 0.93956023,  -60.44956979,           \
                                       -6.2082935,   -7.00282027, -6.2082935,  -16.43788241};                       \
    std::vector<double> virial_elec_pbc = {-60.23064054, -1.39674235, -7.31945507, -1.39674235, -60.45889101,       \
                                           -5.83962715,  -7.31945507, -5.83962715, -16.42404398};                   \
    std::vector<double> real_coords =                                                                               \
        std::vector<double>{-1.139900000, -10.96620000, -0.2631000000, -1.303100000,  -10.00270000, -0.1396000000,  \
                            -1.558100000, -11.18080000, -1.105900000,  1.585500000,   -10.92780000, 0.09230000000,  \
                            1.797400000,  -11.56490000, 0.7855000000,  0.6279000000,  -11.06660000, -0.09240000000, \
                            1.489300000,  -8.215300000, 0.5263000000,  2.131200000,   -7.803500000, -0.06510000000, \
                            1.647500000,  -9.183200000, 0.4342000000,  -1.213700000,  -8.233900000, 0.02700000000,  \
                            -1.649200000, -7.794500000, 0.7678000000,  -0.2513000000, -8.090900000, 0.1807000000};
#endif
