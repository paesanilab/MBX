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

#ifndef ENERGY1B_H
#define ENERGY1B_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

// 1B
#include "potential/1b/ps.h"
// ====>> BEGIN SECTION INCLUDE1B <<====
// ===>> PASTE INCLUDE BELOW <<===
#include "potential/1b/x1b_A1B4_deg5_exp0_v1x.h"
#include "potential/1b/x1b_A1B2_deg4_v1x.h"
#include "potential/1b/mbnrg_1b_A1B3_deg5_v1.h"
// ====>> END SECTION INCLUDE1B <<====

#include "tools/definitions.h"

/**
 * @file energy1b.h
 * @brief Contains the calls to the one-body energy with and without gradients
 */

/**
 * @namespace e1b
 * @brief Sets the namespace for the energy calls for the 1 body energy
 */
namespace e1b {

/**
 * @brief Gets the one body energy for a given set of monomers of the same
 * monomer type.
 *
 * Given the monomer id and the number of monomers, will return the total sum
 * of the 1b energy of those monomers.
 * @param[in] mon Monomer id
 * @param[in] nm number of monomers of monomer type "mon"
 * @param[in] xyz coordinates of the monomer
 * @param[in,out] bad_idxs Vector with the indexes o extremely distorted monomers
 * has an energy larger than the value set in definitions.h (EMAX1B)
 * @return Sum of the one-body energies of all the monomers passed as arguments
 */
double get_1b_energy(std::string mon, size_t nm, std::vector<double> xyz1, std::vector<size_t> &bad_idxs);

/**
 * @brief Gets the one body energy for a given set of monomers of the same
 * monomer type, calculating the gradients.
 *
 * Given the monomer id and the number of monomers, will return the total sum
 * of the 1b energy of those monomers and their corresponding gradients.
 * @param[in] mon Monomer id
 * @param[in] nm number of monomers of monomer type "mon"
 * @param[in] xyz coordinates of the monomer
 * @param[in,out] grad Gradients of the one-body energy
 * @param[in,out] bad_idxs Vector with the indexes o extremely distorted monomers
 * has an energy larger than the value set in definitions.h (EMAX1B)
 * @return Sum of the one-body energies of all the monomers passed as arguments
 */
double get_1b_energy(std::string mon1, size_t nm, std::vector<double> xyz1, std::vector<double> &grad1,
                     std::vector<size_t> &bad_idxs, std::vector<double> *virial = 0);

}  // namespace e1b
#endif
