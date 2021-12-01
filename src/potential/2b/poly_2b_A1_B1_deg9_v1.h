
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

#ifndef POLY_2B_MBNRG_A1_B1_DEG9_H
#define POLY_2B_MBNRG_A1_B1_DEG9_H

/**
 * @file poly_2b_A1_B1_deg9_v1.h
 * @brief Contains the structure of the polynomial for symmetry A1_B1
 */

/**
 * @namespace mbnrg_A1_B1_deg9
 * @brief Encloses the structure of the polynomial for symmetry A1_B1
 */

namespace mbnrg_A1_B1_deg9 {

struct poly_A1_B1_deg9_v1 {
    // Degree of the polynomial
    static const unsigned degree = 9;

    // Number of variables
    static const unsigned n_vars = 1;

    // Number of terms
    static const unsigned size = 9;

    /**
     * @brief Evaluates the polynomial of degree 9 for A1_B1 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1_B1 symmetry.
     * @param[in] x Double array of length 1 with the variable values
     * @param[in] a Double array of 9 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval(const double x[1], const double a[9]);

    /**
     * @brief Evaluates the polynomial of degree 9 for A1_B1 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1_B1 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 1 with the variable values
     * @param[in] a Double array of 9 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval_direct(const double x[1], const double a[9]);

    /**
     * @brief Evaluates the polynomial of degree 9 for A1_B1 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1_B1 symmetry.
     * @param[in] x Double array of length 1 with the variable values
     * @param[in] a Double array of 9 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 1 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval(const double x[1], const double a[9], double g[1]);

    /**
     * @brief Evaluates the polynomial of degree 9 for A1_B1 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1_B1 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 1 with the variable values
     * @param[in] a Double array of 9 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 1 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval_direct(const double x[1], const double a[9], double g[1]);
};

}  // namespace mbnrg_A1_B1_deg9

#endif  // POLY_2B_MBNRG_A1_B1_DEG9_H

// Polynomial input used to generate these files:

//  add_molecule['A1']
//  add_molecule['B1']
//
//  add_variable['A', '1', 'a', 'B', '1', 'b', 'x-inter-A+B-0']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']