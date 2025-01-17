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

#ifndef ENERGY4B_H
#define ENERGY4B_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <iomanip>

// 4B
// =====>> BEGIN SECTION INCLUDE4B <<=====
// ===>> PASTE YOUR CODE BELOW <<===
#include "mbnrg_4b_A1B2_A1B2_A1B2_A1B2_deg4_v1.h"
// =====>> END SECTION INCLUDE4B <<=====
/**
 * @file energy4b.h
 * @brief Contains the calls to the four-body energy with and without gradients
 */

/**
 * @namespace e4b
 * @brief Sets the namespace for the energy calls for the 4 body energy
 */
namespace e4b {

/**
 * @brief Gets the four body energy for a given set of tetramers
 *
 * Given the monomer ids of the tetramer
 * and the number of monomers, will return the total sum
 * of the 4b energy of those tetramers. A tetramer will be mon1-mon2-mon3-mon4.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] m3 Monomer 3 id
 * @param[in] m4 Monomer 4 id
 * @param[in] nm number of monomers (m1, m2, m3, and m4 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in] xyz3 coordinates of the monomer 3
 * @param[in] xyz4 coordinates of the monomer 4
 * @return Sum of the four-body energies of all the tetramers passed as arguments
 */
double get_4b_energy(std::string m1, std::string m2, std::string m3, std::string m4, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2, std::vector<double> xyz3,
                     std::vector<double> xyz4);

/**
 * @brief Gets the four body energy for a given set of tetramers
 *
 * Given the monomer ids of the tetramer
 * and the number of monomers, will return the total sum
 * of the 4b energy of those tetramers. A tetramer will be mon1-mon2-mon3-mon4.
 * @param[in] m1 Monomer 1 id
 * @param[in] m2 Monomer 2 id
 * @param[in] m3 Monomer 3 id
 * @param[in] m4 Monomer 4 id
 * @param[in] nm number of monomers (m1, m2, m3, and m4 need to match)
 * @param[in] xyz1 coordinates of the monomer 1
 * @param[in] xyz2 coordinates of the monomer 2
 * @param[in] xyz3 coordinates of the monomer 3
 * @param[in] xyz4 coordinates of the monomer 4
 * @param[in,out] grad1 gradients of the monomer 1. Gradients will be updated
 * @param[in,out] grad2 gradients of the monomer 2. Gradients will be updated
 * @param[in,out] grad3 gradients of the monomer 3. Gradients will be updated
 * @param[in,out] grad4 gradients of the monomer 4. Gradients will be updated
 * @return Sum of the four-body energies of all the tetramers passed as arguments
 */
double get_4b_energy(std::string m1, std::string m2, std::string m3, std::string m4, size_t nm,
                     std::vector<double> xyz1, std::vector<double> xyz2, std::vector<double> xyz3,
                     std::vector<double> xyz4, std::vector<double> &grd1, std::vector<double> &grd2,
                     std::vector<double> &grd3, std::vector<double> &grd4, std::vector<double> *virial = 0);

}  // namespace e4b
#endif
