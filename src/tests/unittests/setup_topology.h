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

#ifndef UNITTESTS_SETUP_TOPOLOGY_H
#define UNITTESTS_SETUP_TOPOLOGY_H

#include "potential/force_field/bond.h"
#include "potential/force_field/angles.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/topology.h"
#include <vector>
#include "tools/custom_exceptions.h"

/**
 * @file setup_topology.h
 * @brief This file is used to test the bond, angles, dihedral, inversion classes
 *        This file defines all of the necessary components to create the above
 *        classes, including the parameters. Additionally, the energy for each
 *        topology type and their functional form is used to check the
 *        implementation is correct for all the classes
 */

#define SETUP_TOPOLOGY                                               \
    std::string bond_connectivity = "bond";                          \
    size_t bond_type = 1;                                            \
    std::vector<size_t> bond_indexes = {1, 2};                       \
    double distance = 6.06962;                                       \
    std::string angle_connectivity = "angle";                        \
    size_t angle_type = 1;                                           \
    std::vector<size_t> angle_index = {1, 2, 3};                     \
    double theta = 0.230331;                                         \
    std::string dihedral_connectivity = "dihedral";                  \
    size_t dihedral_type = 1;                                        \
    std::vector<size_t> dihedral_indexes = {1, 2, 3, 4};             \
    double dihedral_phi = -0.302514;                                 \
    std::string inversion_connectivity = "inversion";                \
    size_t inversion_type = 1;                                       \
    std::vector<size_t> inversion_indexes = {1, 2, 3, 4};            \
    double inversion_phi = 0.316936;                                 \
    std::vector<double> harm_linear_parameters = {2.0};              \
    std::vector<double> harm_nonlinear_parameters = {4.0};           \
    std::vector<double> morse_linear_parameters = {2.0};             \
    std::vector<double> morse_nonlinear_parameters = {1.0, 4.0};     \
    std::vector<double> quartic_linear_parameters = {1.0, 3.0, 4.0}; \
    std::vector<double> quartic_nonlinear_parameters = {2.0};        \
    std::vector<double> cos_linear_parameters = {1.0};               \
    std::vector<double> cos_nonlinear_parameters = {1.0, 0.5};       \
    std::vector<double> hcos_linear_parameters = {1.0};              \
    std::vector<double> hcos_nonlinear_parameters = {2.0};           \
    std::vector<double> cos3_linear_parameters = {1.0, 2.0, 3.0};    \
    std::vector<double> cos3_nonlinear_parameters = {};              \
    std::vector<double> none_linear_parameters = {};                 \
    std::vector<double> none_nonlinear_parameters = {};              \
    double ff_bond_harm_energy = 4.2833269444e+00;                   \
    double ff_bond_harm_grad = 4.1392400000e+00;                     \
    double ff_bond_morse_energy = 1.5269349493e+00;                  \
    double ff_bond_morse_grad = 4.4119513577e-01;                    \
    double ff_bond_quartic_energy = 3.4997461351e+02;                \
    double ff_bond_quartic_grad = 3.2335608394e+02;                  \
    double ff_bond_none_energy = 0.0000000000e+00;                   \
    double ff_bond_none_grad = 0.0000000000e+00;                     \
    double ff_angles_harm_energy = 1.4210404370e+01;                 \
    double ff_angles_harm_grad = -7.5393380000e+00;                  \
    double ff_angles_quartic_energy = 5.8314641535e+00;              \
    double ff_angles_quartic_grad = -1.4542974339e+01;               \
    double ff_angles_none_energy = 0.0000000000e+00;                 \
    double ff_angles_none_grad = 0.0000000000e+00;                   \
    double ff_dihedral_cos_energy = 1.6949010764e+00;                \
    double ff_dihedral_cos_grad = 7.1910534281e-01;                  \
    double ff_dihedral_harm_energy = 3.9230588271e+00;               \
    double ff_dihedral_harm_grad = 3.9613426144e+00;                 \
    double ff_dihedral_hcos_energy = 9.3946046835e-01;               \
    double ff_dihedral_hcos_grad = 4.0837142899e-01;                 \
    double ff_dihedral_cos3_energy = 3.5783358351e+00;               \
    double ff_dihedral_cos3_grad = 2.5573577421e+00;                 \
    double ff_dihedral_none_energy = 0.0000000000e+00;               \
    double ff_dihedral_none_grad = 0.0000000000e+00;                 \
    double ff_inversion_harm_energy = 1.3564960428e+01;              \
    double ff_inversion_harm_grad = -7.8784654448e+00;               \
    double ff_inversion_none_energy = 0.0000000000e+00;              \
    double ff_inversion_none_grad = 0.0000000000e+00;
#endif
