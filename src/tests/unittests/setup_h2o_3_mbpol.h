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

#ifndef UNITTESTS_SETUP_H2O_3_MBPOL_H
#define UNITTESTS_SETUP_H2O_3_MBPOL_H

#define SETUP_H2O_3_MBPOL                                                                                             \
    const size_t n_monomers = 3;                                                                                      \
    std::vector<std::string> atom_names{"O", "H", "H", "O", "H", "H", "O", "H", "H"};                                 \
    std::vector<std::string> monomer_names = {"h2o", "h2o", "h2o"};                                                   \
    std::vector<size_t> n_atoms_vector = {3, 3, 3};                                                                   \
    std::vector<size_t> n_sites_vector = {4, 4, 4};                                                                   \
    std::vector<size_t> islocal = {1, 1, 1};                                                                          \
    std::vector<size_t> first_index = {0, 4, 8};                                                                      \
    std::vector<size_t> first_index_realSites = {0, 3, 6};                                                            \
    std::vector<double> box = {15.0, 0.0, 0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 15.0};                                       \
    const size_t n_atoms = 9;                                                                                         \
    const size_t n_sites = 12;                                                                                        \
    std::vector<std::pair<std::string, size_t>> mon_type_count{{"h2o", n_monomers}};                                  \
    std::vector<double> virial_2b = {21.21728011, 2.92523901, 3.75776769,  2.92523901, 9.6500956,                     \
                                     13.60719407, 3.75776769, 13.60719407, 21.19627151};                              \
    std::vector<double> virial_1b = {-29.82600382, 30.71343212, -5.88668738, 30.71343212, -5.32311185,                \
                                     1.01648662,   -5.88668738, 1.01648662,  -3.40683556};                            \
    std::vector<double> virial_3b = {-0.39645076, 0.28589149,  -0.03809345, 0.28589149, 0.13231023,                   \
                                     0.73785134,  -0.03809345, 0.73785134,  0.81031549};                              \
    std::vector<double> virial_disp = {-9.84445507, -0.04295674, 0.80621893,  -0.04295674, -3.11563098,               \
                                       -4.61445985, 0.80621893,  -4.61445985, -7.50791109};                           \
    std::vector<double> virial_disp_pbc = {-9.84971319, -0.04303824, 0.8061783,   -0.04303824, -3.11969407,           \
                                           -4.61424474, 0.8061783,   -4.61424474, -7.5123088};                        \
    std::vector<double> virial_elec = {-7.97980402, 5.39667782, 7.09870937, 5.39667782, 2.6833174,                    \
                                       5.058413,    7.09870937, 5.058413,   8.18444073};                              \
    std::vector<double> virial_elec_pbc = {-8.33417782, 5.68568356, 7.2082218,  5.68568356, 2.40088432,               \
                                           5.17655354,  7.2082218,  5.17655354, 8.25317878};                          \
    std::vector<double> real_coords =                                                                                 \
        std::vector<double>{-1.58972425, 1.04337922,  -0.08780840, -0.63591971, 0.97898520,  0.00000000, -1.90066280, \
                            1.74501050,  -0.66454990, 1.64924507,  1.08594656,  0.00000000,  2.60878026, 1.09587704,  \
                            -0.02817115, 1.33830653,  1.78757784,  0.57674150,  -0.61315209, 2.46976336, 2.07005086,  \
                            0.34684791,  2.46976336,  2.07005086,  -0.93360667, 3.37469919,  2.07005086};
#endif
