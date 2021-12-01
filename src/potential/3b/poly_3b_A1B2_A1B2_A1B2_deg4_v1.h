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

#ifndef POLY_3B_MBNRG_A1B2_A1B2_A1B2_DEG4_H
#define POLY_3B_MBNRG_A1B2_A1B2_A1B2_DEG4_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

/**
 * @file poly_3b_A1B2_A1B2_A1B2_deg4_v1.h
 * @brief Contains the structure that allows the evaluation of the polynomial for A1B2_A1B2_A1B2 symmetry.
 */

/**
 * @namespace mbnrg_A1B2_A1B2_A1B2_deg4
 * @brief Namespace that encloses the A1B2_A1B2_A1B2 polynomial structure
 */

namespace mbnrg_A1B2_A1B2_A1B2_deg4 {

class poly_A1B2_A1B2_A1B2_deg4_v1 {
   public:
    // Degree of the polynomial
    static const unsigned degree = 4;

    // Number of variables
    static const unsigned n_vars = 36;

    // Number of terms
    static const unsigned size = 2331;

    /**
     * @brief Evaluates the polynomial of degree 4 for A1B2_A1B2_A1B2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the
     * A1B2_A1B2_A1B2 symmetry
     * @param[in] a Double array of 2331 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 36 with the variable values
     * @return Value of the polynomial
     */
    double eval(const double x[36], const double a[2331]);

    /**
     * @brief Evaluates the polynomial of degree 4 for A1B2_A1B2_A1B2 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the
     * A1B2_A1B2_A1B2 symmetry, and calculates the gradients.
     * @param[in] a Double array of 2331 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 36 with the variable values
     * @param[out] g Double array of length 36 that will store the gradients dP/dxi
     * @return Value of the polynomial
     */
    double eval(const double x[36], const double a[2331], double g[36]);
};

}  // namespace mbnrg_A1B2_A1B2_A1B2_deg4

#endif  // POLY_3B_MBNRG_A1B2_A1B2_A1B2_DEG4_H

// Polynomial input used to generate these files:

//  add_molecule['A1B2']
//  add_molecule['A1B2']
//  add_molecule['A1B2']
//
//  add_variable['A', '1', 'a', 'A', '2', 'b', 'x-inter-A+A-0']
//  add_variable['A', '1', 'a', 'A', '3', 'c', 'x-inter-A+A-0']
//  add_variable['A', '1', 'a', 'B', '1', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '2', 'a', 'x-intra-A+B-1']
//  add_variable['A', '1', 'a', 'B', '3', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '4', 'b', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '5', 'c', 'x-inter-A+B-0']
//  add_variable['A', '1', 'a', 'B', '6', 'c', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'A', '3', 'c', 'x-inter-A+A-0']
//  add_variable['A', '2', 'b', 'B', '1', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '2', 'a', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '3', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '4', 'b', 'x-intra-A+B-1']
//  add_variable['A', '2', 'b', 'B', '5', 'c', 'x-inter-A+B-0']
//  add_variable['A', '2', 'b', 'B', '6', 'c', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '1', 'a', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '2', 'a', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '3', 'b', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '4', 'b', 'x-inter-A+B-0']
//  add_variable['A', '3', 'c', 'B', '5', 'c', 'x-intra-A+B-1']
//  add_variable['A', '3', 'c', 'B', '6', 'c', 'x-intra-A+B-1']
//  add_variable['B', '1', 'a', 'B', '2', 'a', 'x-intra-B+B-1']
//  add_variable['B', '1', 'a', 'B', '3', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '1', 'a', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '3', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '4', 'b', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '2', 'a', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '3', 'b', 'B', '4', 'b', 'x-intra-B+B-1']
//  add_variable['B', '3', 'b', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '3', 'b', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '4', 'b', 'B', '5', 'c', 'x-inter-B+B-0']
//  add_variable['B', '4', 'b', 'B', '6', 'c', 'x-inter-B+B-0']
//  add_variable['B', '5', 'c', 'B', '6', 'c', 'x-intra-B+B-1']
//
//  add_filter['sum-degree', 'x-inter-*+*-*', '0']
//  add_filter['sum-degree', 'x-intra-*+*-*', '2+']
