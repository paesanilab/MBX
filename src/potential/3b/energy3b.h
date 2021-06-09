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

#ifndef ENERGY3B_H
#define ENERGY3B_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>

// 3B
#include "potential/3b/x3b-v2x.h"
#include "potential/3b/poly-3b-v2x.h"
#include "potential/3b/x3b-h2o-ion-v2x-deg4-filtered.h"
// =====>> BEGIN SECTION INCLUDE3B <<=====
// ===>> PASTE YOUR CODE BELOW <<===
#include "potential/3b/mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1.h"
#include "potential/3b/mbnrg_3b_A1B2_A1B2_A1B2_deg4_v1.h"
#include "potential/3b/mbnrg_3b_A1_B1C2X2_B1C2X2_deg4_v1.h"
// =====>> END SECTION INCLUDE3B <<=====
/**
 * @file energy3b.h
 * @brief Contains the calls to the three-body energy with and without gradients
 */

/**
 * @namespace e3b
 * @brief Sets the namespace for the energy calls for the 3 body energy
 */
namespace e3b {

/**
 * @brief Gets the two body energy for a given set of trimers
 *
 * Given the monomer ids of the trimer
 * and the number of monomers, will return the total sum
 * of the 3b energy of those trimers. A trimer will be mon1-mon2-mon3.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] m3 Monomer 3 id
 * @param[in] nm number of monomers (m1, m2, and m3 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in] xyz3 coordinates of the monomer 3
 * @return Sum of the three-body energies of all the trimers passed as arguments
 */
double get_3b_energy(std::string m1, std::string m2, std::string m3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3);

/**
 * @brief Gets the three body energy for a given set of trimers
 *
 * Given the monomer ids of the trimer
 * and the number of monomers, will return the total sum
 * of the 3b energy of those trimers. A trimer will be mon1-mon2-mon3.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] m3 Monomer 3 id
 * @param[in] nm number of monomers (both m1 and m2 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in] xyz3 coordinates of the monomer 3
 * @param[in,out] grad1 gradients of the monomer 1. Gradients will be updated
 * @param[in,out] grad2 gradients of the monomer 2. Gradients will be updated
 * @param[in,out] grad3 gradients of the monomer 3. Gradients will be updated
 * @return Sum of the three-body energies of all the trimers passed as arguments
 */
double get_3b_energy(std::string m1, std::string m2, std::string m3, size_t nm, std::vector<double> xyz1,
                     std::vector<double> xyz2, std::vector<double> xyz3, std::vector<double> &grd1,
                     std::vector<double> &grd2, std::vector<double> &grd3, std::vector<double> *virial = 0);

}  // namespace e3b
#endif
