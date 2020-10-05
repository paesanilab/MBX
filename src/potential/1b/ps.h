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

#ifndef CU_INCLUDE_POTENTIAL_1B_PS_H
#define CU_INCLUDE_POTENTIAL_1B_PS_H

#include <algorithm>
#include <vector>
#include <iomanip>
#include <iostream>

/**
 * @file ps.h
 * @brief This file contains the calls for the energy of water
 */

/**
 * @namespace ps
 * @brief Namespace of the water energy functions
 */
namespace ps {

/**
 * @brief Computes the energy and gradients for a water monomer
 *
 * Given the coordinates of a water monomer, it returns the 1b energy of
 * that water monomer.
 * @param[in] rr Coordinates of the water molecule (OHH)
 * @param[out] dr Gradients of the atoms of the water molecule (OxyzHxyzHxyz)
 * @return Deformation energy of the water molecule
 */
// double pot_nasa(const double* rr, double* dr);

/**
 * @brief Computes the energy and gradients for a water monomer
 *
 * Given the coordinates of a water monomer, it returns the 1b energy of
 * all the water molecules in a vector.
 * @param[in] rr Coordinates of the all the water molecule
 * @param[out] dr Gradients of all the water molecules
 * @param[in] nw Number of water molecules
 * @return Deformation energy of all the water molecules in a vector
 */
std::vector<double> pot_nasa(const double* rr, double* dr, size_t nw, std::vector<double>* virial = 0);

/**
 * @brief Computes the dipole surface and the charges
 *
 * Given the coordinates, this function return fills the charge vector and the
 * derivatives due to position dependent charges.
 * @param[in] dms_param1 Parameters to correct for DMS if TTM3 is used.
 * @param[in] dms_param2 Parameters to correct for DMS if TTM3 is used.
 * @param[in] dms_param3 Parameters to correct for DMS if TTM3 is used.
 * @param[in] rr Coordinates of the water molecule
 * @param[out] q3 Charges of the water molecule
 * @param[out] dq3 Charge derivates of the water molecule
 * @param[in] ttm3 Boolean specifying if we are using TTM3.
 * Should be set to false in this software.
 */
void dms_nasa(const double& dms_param1, const double& dms_param2, const double& dms_param3, const double* rr,
              double* q3, double* dq3, std::vector<double>* aux_data = 0);

}  // namespace ps

#endif  // CU_INCLUDE_POTENTIAL_1B_PS_H
