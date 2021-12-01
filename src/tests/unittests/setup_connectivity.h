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

#ifndef UNITTESTS_SETUP_CONNECTIVITY
#define UNITTESTS_SETUP_CONNECTIVITY
#include "potential/force_field/angles.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/topology.h"
#include "potential/force_field/connectivity.h"
#include <vector>
#include "tools/custom_exceptions.h"

/**
 * @file setup_connectivity.h
 * @brief This file is used to test the connectivity class
 */

#define SETUP_CONNECTIVITY                                                                          \
    std::string mon_id = "h4_dummy";                                                                \
    std::string bond_connectivity = "bond";                                                         \
    std::vector<size_t> bond_indexes = {1, 2};                                                      \
    std::string bond_functional_form = "morse";                                                     \
    std::string angle_connectivity = "angle";                                                       \
    std::vector<size_t> angle_index = {1, 2, 3};                                                    \
    std::string angle_functional_form = "harm";                                                     \
    std::string dihedral_connectivity = "dihedral";                                                 \
    std::vector<size_t> dihedral_indexes = {1, 2, 3, 4};                                            \
    std::string dihedral_functional_form = "cos";                                                   \
    std::string inversion_connectivity = "inversion";                                               \
    std::vector<size_t> inversion_indexes = {1, 2, 3, 4};                                           \
    std::string inversion_functional_form = "harm";                                                 \
    std::vector<double> harm_linear_parameters = {2.0};                                             \
    std::vector<double> harm_nonlinear_parameters = {4.0};                                          \
    std::vector<double> morse_linear_parameters = {2.0};                                            \
    std::vector<double> morse_nonlinear_parameters = {1.0, 4.0};                                    \
    std::vector<double> quartic_linear_parameters = {1.0, 3.0, 4.0};                                \
    std::vector<double> quartic_nonlinear_parameters = {2.0};                                       \
    std::vector<double> cos_linear_parameters = {1.0};                                              \
    std::vector<double> cos_nonlinear_parameters = {1.0, 0.5};                                      \
    std::vector<double> hcos_linear_parameters = {1.0};                                             \
    std::vector<double> hcos_nonlinear_parameters = {2.0};                                          \
    std::vector<double> cos3_linear_parameters = {1.0, 2.0, 3.0};                                   \
    std::vector<double> cos3_nonlinear_parameters = {};                                             \
    std::vector<double> none_linear_parameters = {};                                                \
    std::vector<double> none_nonlinear_parameters = {};                                             \
    eff::Bond mon1_bond(bond_connectivity, bond_indexes, bond_functional_form);                     \
    eff::Angles mon1_angle(angle_connectivity, angle_index, angle_functional_form);                 \
    eff::Dihedral mon1_dihedral(dihedral_connectivity, dihedral_indexes, dihedral_functional_form); \
    eff::Inversion mon1_inversion(inversion_connectivity, inversion_indexes, inversion_functional_form);

#endif
