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

#ifndef X1b_A1B4_DEG5_EXP0_V1X_H
#define X1b_A1B4_DEG5_EXP0_V1X_H

#include "poly_1b_A1B4_deg5_exp0_v1x.h"

#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>

/**
 * @file x1b_A1B4_deg5_exp0_v1x.h
 * @brief Contains the structure of the polynomial holder for symmetry A1B4
 */

/**
 * @namespace x1b_A1B4_deg5_exp0
 * @brief Encloses the structure of the polynomial holder for symmetry A1B4
 */

namespace x1b_A1B4_deg5_exp0 {

class x1b_A1B4_v1x {
   public:
    // Creates an empty class
    x1b_A1B4_v1x(){};
    /**
     * @brief Creates a class and initializes the parameters corresponding to mon
     * @param[in] mon Monomer id of the monomer of interest
     */
    x1b_A1B4_v1x(const std::string mon);

    // Destroys the class
    ~x1b_A1B4_v1x(){};

    // Polynomial for this symmetry
    typedef poly_1b_A1B4_deg5_exp0_v1x polynomial;

    /**
     * @brief Computes the one body energy for the monomers
     *
     * Given the coordinates of a number of monomers, it calculates the polynomial value for each one of them, and
     * returns a vector with them.
     * @param[in] xyz Pointer to a double array with the coordinates of the monomers.
     * @param[in] nmon Number of monomers passed in the xyz array.
     * @return Vector of doubles with the energies of each monomer, in the same order as the input.
     */
    std::vector<double> eval(const double* xyz, const size_t nmon) const;

    /**
     * @brief Computes the one body energy for the monomers and their gradients
     *
     * Given the coordinates of a number of monomers, it calculates the polynomial value for each one of them, and ret
urns a vector with them. The gradients are also computed.
     * @param[in] xyz Pointer to a double array with the coordinates of the monomers.
     * @param[in,out] grad Pointer to a double array with the gradients of the monomers.
     * @param[in] nmon Number of monomers passed in the xyz array.
     * @param[in,out] virial Vector of 9 doubles with the virial tensor.
     * @return Vector of doubles with the energies of each monomer, in the same order as the input.
     */
    std::vector<double> eval(const double* xyz, double* grad, const size_t nmon, std::vector<double>* virial = 0) const;

   private:
    // Values of the non-linear parameters of the polynomials
    double m_k_AB;
    double m_d_AB;
    double m_k_BB;
    double m_d_BB;

    // Values of the linear parameters of the polynomials
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace x1b_A1B4_deg5_exp0

////////////////////////////////////////////////////////////////////////////////

#endif
