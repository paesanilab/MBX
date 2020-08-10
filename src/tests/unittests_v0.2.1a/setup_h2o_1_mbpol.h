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

#ifndef UNITTESTS_SETUP_H2O_1_MBPOL_H
#define UNITTESTS_SETUP_H2O_1_MBPOL_H

#define SETUP_H2O_1_MBPOL                                                                                  \
    const size_t n_monomers = 1;                                                                           \
    std::vector<std::string> atom_names{"O", "H", "H"};                                                    \
    std::vector<std::string> monomer_names = {"h2o"};                                                      \
    std::vector<size_t> n_atoms_vector = {3};                                                              \
    std::vector<size_t> n_sites_vector = {4};                                                              \
    std::vector<size_t> first_index = {0};                                                                 \
    std::vector<size_t> first_index_realSites = {0};                                                       \
    const size_t n_atoms = 3;                                                                              \
    const size_t n_sites = 4;                                                                              \
    std::vector<double> box = {};                                                                          \
    std::vector<double> virial = {-12.99560229, 18.52621893, 0, 18.52621893, -3.89464627, 0, 0, 0, 0};     \
    std::vector<double> polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 1.3100000000e+00};    \
    std::vector<double> pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01, 0.0000000000e+00};       \
    std::vector<double> charges{0.0000000000e+00, 5.9876992410e-01, 5.9876992432e-01, -1.1975398484e+00};  \
    std::vector<double> real_polfac{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                 \
    std::vector<double> real_pol{1.3100000000e+00, 2.9400000000e-01, 2.9400000000e-01};                    \
    std::vector<double> real_charges{0.0000000000e+00, 5.9876992410e-01, 5.9876992432e-01};                \
    std::vector<double> C6_long_range{1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00};               \
    std::vector<std::pair<std::string, size_t>> internal_mon_type_count{{"h2o", 1}};                       \
    std::vector<size_t> internal_original_to_current_order{0};                                             \
    std::vector<std::pair<size_t, size_t>> internal_orginal_order{{0, 0}};                                 \
    std::vector<std::pair<size_t, size_t>> internal_original_order_realSites{{0, 0}};                      \
    std::vector<std::string> internal_monomer_names{"h2o"};                                                \
    std::vector<double> real_coords{-1.59117306, 1.07433216,  0.00000000, -0.63591971, 0.97898520,         \
                                    0.00000000,  -1.91162765, 1.97926799, 0.00000000};                     \
    std::vector<double> coords{-1.5911730600e+00, 1.0743321600e+00,  0.0000000000e+00,  -6.3591971000e-01, \
                               9.7898520000e-01,  0.0000000000e+00,  -1.9116276500e+00, 1.9792679900e+00,  \
                               0.0000000000e+00,  -1.4557365602e+00, 1.2470607312e+00,  0.0000000000e+00};
#endif
