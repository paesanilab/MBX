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

#ifndef POLY_3B_V2X_H
#define POLY_3B_V2X_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

#include <stdlib.h>
#include <vector>

/**
 * @file poly-3b-v2x.h
 * @brief Contains the structure that allows the evaluation of the polynomial for MB-pol.
 */

/**
 * @namespace x2o
 * @brief Namespace that encloses the MB-pol polynomial structure
 */
namespace x2o {

class poly_3b_v2x {
   public:
    // Number of variables
    static const unsigned n_vars = 36;

    // Number of terms
    static const unsigned size = 1163;

    /**
     * @brief Evaluates the polynomial of degree 4 for MB-pol.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for MB-pol
     * @param[in] a Double array of 1163 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 36 with the variable values
     * @return Value of the polynomial
     */
    static double eval(const double* a, const double* x);

    /**
     * @brief Evaluates the polynomial of degree 4 for MB-pol.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for MB-pol
     * and calculates the gradients.
     * @param[in] a Double array of 1163 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 36 with the variable values
     * @param[in] t Double array of length 32609 used as intermediate values for the polynomial evaluation
     * @param[out] g Double array of length 36 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    static double eval(const double* a, const double* x, double* t, double* g);
};

}  // namespace x2o

#endif  // POLY_3B_V2X_H
