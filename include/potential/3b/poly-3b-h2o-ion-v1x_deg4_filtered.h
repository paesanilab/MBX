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

#ifndef POLY_3B_H2O_ION_V1X_DEG4_FILTERED_H
#define POLY_3B_H2O_ION_V1X_DEG4_FILTERED_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

/**
 * @file poly-3b-h2o-ion-v1x_deg4_filtered.h
 * @brief Contains the structure that allows the evaluation of the polynomial for monoatomic ion - water - water
 * structure symmetry.
 */

/**
 * @namespace h2o_ion
 * @brief Namespace that encloses the monoatomic ion - water - water structure polynomial structure
 */
namespace h2o_ion {

class poly_3b_h2o_ion_v1x_deg4_filtered {
   public:
    // Number of variables
    static const unsigned n_vars = 21;

    // Number of terms
    static const unsigned size = 1016;

    /**
     * @brief Evaluates the polynomial of degree 4 for monoatomic ion - water - water structure symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the
     * monoatomic ion - water - water structure symmetry, and calculates the gradients.
     * @param[in] a Double array of 1016 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 21 with the variable values
     * @param[out] g Double array of length 21 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    static double eval(const double a[1016], const double x[21], double g[21]);
};

}  // namespace h2o_ion

#endif  // POLY_3B_H2O_ION_V1X_H
