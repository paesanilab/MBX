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

#ifndef UNITTESTS_SETUP_H4_DUMMY_BOND_H
#define UNITTESTS_SETUP_H4_DUMMY_BOND_H

#include "potential/force_field/bond.h"
#include "potential/force_field/topology.h"
#include <vector>
#include "tools/custom_exceptions.h"

#define SETUP_H4_DUMMY_BOND

std::string connectivity = "bond";
size_t bond_type = 1;
std::vector<size_t> indexes = {1, 2};
double distance = 6.06962;

std::vector<double> harm_linear_parameters = {2.0};
std::vector<double> harm_nonlinear_parameters = {4.0};
std::vector<double> morse_linear_parameters = {2.0};
std::vector<double> morse_nonlinear_parameters = {1.0, 4.0};
std::vector<double> quartic_linear_parameters = {1.0, 3.0, 4.0};
std::vector<double> quartic_nonlinear_parameters = {2.0};
std::vector<double> none_linear_parameters = {};
std::vector<double> none_nonlinear_parameters = {};

double ff_bond_harm_energy = 4.28333;
std::vector<double> ff_bond_harm_non_lin_val = {2.14167};
double ff_bond_harm_grad = 4.13924;

double ff_bond_morse_energy = 1.52694;
std::vector<double> ff_bond_morse_non_lin_val = {0.763468};
double ff_bond_morse_grad = 0.441195;

double ff_bond_quartic_energy = 349.975;
std::vector<double> ff_bond_quartic_non_lin_val = {8.28091, 22.4668, 68.5734};
double ff_bond_quartic_grad = 323.356;

double ff_bond_none_energy = 0.0;
std::vector<double> ff_bond_none_non_lin_val = {0.0};
double ff_bond_none_grad = 0.0;

#endif
