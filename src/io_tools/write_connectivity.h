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

#ifndef WRITE_CONNECTIVITY_H
#define WRITE_CONNECTIVITY_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "potential/force_field/angles.h"
#include "potential/force_field/bond.h"
#include "potential/force_field/connectivity.h"
#include "tools/custom_exceptions.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/inversion.h"
#include "potential/force_field/topology.h"
#include "bblock/system.h"

/**
 * @file write_connectivity.h
 * @brief Contains the function needed to write a connectivity file
 *
 * An example of a connectivity write is given below. It is for a bond, but all
 * the other topology follow
 *
 * bond         1         2            harm             1.0    2.0
 * ^            ^         ^            ^                ^      ^
 * topology    1st idx  2nd index   functional form     k      r0
 *
 * The parameters are written in the following way:
 *
 * Harmonic/Harmonic Cosine:
 * k    r0/theta0/phi0
 *
 * Morse:
 * E0   k   r0
 *
 * Cosine:
 * A    m   d(delta)
 *
 * Triple Cosine:
 * A1   A2  A3
 *
 * Quartic:
 * k    r0/theta0   k'  k''
 *
 * None
 * (Nothing needs to be here if you specify none functional form)
 *
 */

/**
 * @namespace tools
 * @brief Contains tools needed to read and write xyz, nrg, and connectivity
 * files
 */
namespace tools {

/**
 * @brief Writes a connectivity file
 * @param[in] filename Name of the connectivity file
 * @param[in] connectivity_map Hashmap with the monomer id as the keys and the
 * connectivity objects as values.
 */
void WriteConnectivity(char* filename, std::unordered_map<std::string, eff::Conn> connectivity_map);

/**
 * @brief Helper function that saves the connectivity map object in an output
 * stream
 * @param[in, out] os Output stream where the information must be saved
 * @param[in] connectivity_map Hashmap with the monomer id as the keys and the
 * connectivity objects as values.
 */
void SaveConnectivity(std::ostream& os, std::unordered_map<std::string, eff::Conn> connectivity_map);

/**
 * @brief Helper function that writes out the parameters
 * @param[in, out] os Output stream where the information must be saved
 * @param[in] linear_parameters Vector holding the linear parameters
 * @param[in] nonlinear_parameters Vector holding the nonlinear parameters
 * @param[in] functional_form String representing the current functional form
 */
void WriteParameters(std::ostream& os, std::vector<double>& linear_parameters,
                     std::vector<double>& nonlinear_parameters, std::string functional_form);

}  // namespace tools
#endif
