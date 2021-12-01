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

#ifndef POLY_2B_A1B2_A1B2_DEG5_H
#define POLY_2B_A1B2_A1B2_DEG5_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

/**
 * @file poly_2b_A1B2_A1B2_v1x.h
 * @brief Contains the structure that allows the evaluation of the polynomial for A1B2_A1B2 symmetry.
 */

/**
 * @namespace x2b_A1B2_A1B2_deg5
 * @brief Namespace that encloses the A1B2_A1B2 polynomial structure
 */

namespace x2b_A1B2_A1B2_deg5 {

struct poly_2b_A1B2_A1B2_deg5_v1x {
    // Number of variables
    static const unsigned n_vars = 15;
    // Number of terms
    static const unsigned size = 2269;

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2_A1B2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B2_A1B2
     * symmetry
     * @param[in] a Double array of 2269 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 15 with the variable values
     * @return Value of the polynomial
     */
    static double eval(const double a[2269], const double x[15]);

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2_A1B2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B2_A1B2
     * symmetry, and calculates the gradients.
     * @param[in] a Double array of 2269 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 15 with the variable values
     * @param[out] g Double array of length 15 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    static double eval(const double a[2269], const double x[15], double g[15]);

   public:
    /**
     * @brief Returns the number of variables of the polynomial
     *
     * @return Number of variables of the polynomial
     */
    unsigned report_nvars() { return n_vars; };

    /**
     * @brief Returns the number of terms of the polynomial
     *
     * @return Number of terms of the polynomial
     */

    unsigned report_size() { return size; };
};

}  // namespace x2b_A1B2_A1B2_deg5

#endif  // POLY_MODEL_H
