
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

#ifndef POLY_2B_MBNRG_A1B2Z2_C2_DEG5_H
#define POLY_2B_MBNRG_A1B2Z2_C2_DEG5_H

/**
 * @file poly_2b_A1B2Z2_C2_deg5_v1.h
 * @brief Contains the structure of the polynomial for symmetry A1B2Z2_C2
 */

/**
 * @namespace mbnrg_A1B2Z2_C2_deg5
 * @brief Encloses the structure of the polynomial for symmetry A1B2Z2_C2
 */

namespace mbnrg_A1B2Z2_C2_deg5 {

struct poly_A1B2Z2_C2_deg5_v1 {
    // Degree of the polynomial
    static const unsigned degree = 5;

    // Number of variables
    static const unsigned n_vars = 14;

    // Number of terms
    static const unsigned size = 933;

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2Z2_C2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1B2Z2_C2 symmetry.
     * @param[in] x Double array of length 14 with the variable values
     * @param[in] a Double array of 933 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval(const double x[14], const double a[933]);

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2Z2_C2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1B2Z2_C2 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 14 with the variable values
     * @param[in] a Double array of 933 elements with the linear parameters of the polynomial
     * @return Value of the polynomial
     */
    double eval_direct(const double x[14], const double a[933]);

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2Z2_C2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1B2Z2_C2 symmetry.
     * @param[in] x Double array of length 14 with the variable values
     * @param[in] a Double array of 933 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 14 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval(const double x[14], const double a[933], double g[14]);

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2Z2_C2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables,
     * evaluates the polynomial for the A1B2Z2_C2 symmetry.
     * It uses the direct, non optimized polynomial
     * @param[in] x Double array of length 14 with the variable values
     * @param[in] a Double array of 933 elements with the linear parameters of the polynomial
     * @param[out] g Double array of length 14 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval_direct(const double x[14], const double a[933], double g[14]);
};

}  // namespace mbnrg_A1B2Z2_C2_deg5

#endif  // POLY_2B_MBNRG_A1B2Z2_C2_DEG5_H

// Polynomial input used to generate these files:

//  add_molecule['A1B2Z2']
//  add_molecule['C2']
//
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '2', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'C', '1', 'b', 'x-inter-A+C-0']
//  add_variable['A', '1', 'a', 'C', '2', 'b', 'x-inter-A+C-0']
//  add_variable['B', '1', 'a', 'B', '2', 'a', 'x-intra-B+B-1']
//  add_variable['B', '1', 'a', 'C', '1', 'b', 'x-inter-B+C-0']
//  add_variable['B', '1', 'a', 'C', '2', 'b', 'x-inter-B+C-0']
//  add_variable['B', '2', 'a', 'C', '1', 'b', 'x-inter-B+C-0']
//  add_variable['B', '2', 'a', 'C', '2', 'b', 'x-inter-B+C-0']
//  add_variable['C', '1', 'b', 'C', '2', 'b', 'x-intra-C+C-1']
//  add_variable['C', '1', 'b', 'Z', '1', 'a', 'x-inter-C+Z-0']
//  add_variable['C', '1', 'b', 'Z', '2', 'a', 'x-inter-C+Z-0']
//  add_variable['C', '2', 'b', 'Z', '1', 'a', 'x-inter-C+Z-0']
//  add_variable['C', '2', 'b', 'Z', '2', 'a', 'x-inter-C+Z-0']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']
//  add_filter['sum-degree', '*', '1', 'and', 'sum-degree', 'x-intra-*+*-*', '1+']
//  add_filter['sum-degree', '*', '2', 'and', 'sum-degree', 'x-intra-*+*-*', '2']
//  add_filter['sum-degree', '*', '3', 'and', 'sum-degree', 'x-intra-*+*-*', '3']
//  add_filter['sum-degree', '*', '4', 'and', 'sum-degree', 'x-intra-*+*-*', '1-/3+']
//  add_filter['sum-degree', '*', '5', 'and', 'sum-degree', 'x-intra-*+*-*', '1-/4+']
