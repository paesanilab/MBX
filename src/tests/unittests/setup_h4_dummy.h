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

#ifndef UNITTESTS_SETUP_H4_DUMMY_H
#define UNITTESTS_SETUP_H4_DUMMY_H
#include "bblock/system.h"
#include "testutils.h"

#include "potential/force_field/angles.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/topology.h"
#include "potential/force_field/connectivity.h"
#include <vector>
#include "tools/custom_exceptions.h"

/**
 * @file setup_h4_dummy.h
 * @brief This file is used to do full molecule testing for the classic
 *        potential of bonds, angles, dihedrals, inversions on a dummy H4 system
 */

#define SETUP_H4_DUMMY                                                                             \
    double classic_energy = 1.6090302673e+01;                                                      \
    const int n_monomers = 1;                                                                      \
    std::vector<double> real_coords{-1.35000000000, -0.05000000, 0.44000000000,  0.900000000000,   \
                                    -1.98000000000, 0.670000000, 1.100000000000, 1.0000000000000,  \
                                    -1.05000000000, 1.880000000, -1.83760000000, 1.0500000000000}; \
    std::vector<std::string> ff_mon = {"h4_dummy"};                                                \
    std::vector<std::pair<std::string, std::string>> ttm_pairs = {{"h4_dummy", "h4_dummy"}};       \
    std::vector<std::string> ignore_1b = {"h4_dummy"};                                             \
    std::vector<std::vector<std::string>> ignore_2b = {{"h4_dummy", "h4_dummy"}};                  \
    std::vector<std::vector<std::string>> ignore_3b = {{"h4_dummy", "h4_dummy", "h4_dummy"}};      \
    std::vector<std::string> atom_names{"H", "H", "H", "H"};                                       \
    std::vector<std::string> monomer_names = {"h4_dummy"};                                         \
    std::vector<size_t> n_atoms_vector = {4};                                                      \
    std::vector<size_t> n_sites_vector = {4};                                                      \
    const size_t n_atoms = 4;                                                                      \
    const size_t n_sites = 4;                                                                      \
    std::string mon_id = "h4_dummy";                                                               \
    std::string bond_connectivity = "bond";                                                        \
    std::vector<size_t> bond_indexes1 = {1, 2};                                                    \
    std::vector<size_t> bond_indexes2 = {2, 3};                                                    \
    std::vector<size_t> bond_indexes3 = {3, 4};                                                    \
    std::string bond_functional_form = "harm";                                                     \
    std::string angle_connectivity = "angle";                                                      \
    std::vector<size_t> angle_index1 = {1, 2, 3};                                                  \
    std::vector<size_t> angle_index2 = {2, 3, 4};                                                  \
    std::string angle_functional_form = "harm";                                                    \
    std::string dihedral_connectivity = "dihedral";                                                \
    std::vector<size_t> dihedral_indexes = {1, 2, 3, 4};                                           \
    std::string dihedral_functional_form = "cos";                                                  \
    std::string inversion_connectivity = "inversion";                                              \
    std::vector<size_t> inversion_indexes = {1, 2, 3, 4};                                          \
    std::string inversion_functional_form = "harm";                                                \
    std::vector<double> bond_harm_linear_parameters = {2.0};                                       \
    std::vector<double> bond_harm_nonlinear_parameters = {4.0};                                    \
    std::vector<double> angle_harm_linear_parameters1 = {1.0};                                     \
    std::vector<double> angle_harm_nonlinear_parameters1 = {2.0};                                  \
    std::vector<double> angle_harm_linear_parameters2 = {3.0};                                     \
    std::vector<double> angle_harm_nonlinear_parameters2 = {3.14159265};                           \
    std::vector<double> inversion_harm_linear_parameters = {1.0};                                  \
    std::vector<double> inversion_harm_nonlinear_parameters = {2.0};                               \
    std::vector<double> dihedral_cos_linear_parameters = {1.0};                                    \
    std::vector<double> dihedral_cos_nonlinear_parameters = {1.0, 0.5};                            \
    eff::Bond bond1(bond_connectivity, bond_indexes1, bond_functional_form);                       \
    eff::Bond bond2(bond_connectivity, bond_indexes2, bond_functional_form);                       \
    eff::Bond bond3(bond_connectivity, bond_indexes3, bond_functional_form);                       \
    eff::Angles angle1(angle_connectivity, angle_index1, angle_functional_form);                   \
    eff::Angles angle2(angle_connectivity, angle_index2, angle_functional_form);                   \
    eff::Dihedral dihedral1(dihedral_connectivity, dihedral_indexes, dihedral_functional_form);    \
    eff::Inversion inversion1(inversion_connectivity, inversion_indexes, inversion_functional_form);

#endif
