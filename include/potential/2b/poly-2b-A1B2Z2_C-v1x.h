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

#ifndef POLY_2B_A1B2Z2_C_V1X_H
#define POLY_2B_A1B2Z2_C_V1X_H

#include <vector>
#include <cstddef>

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

/**
 * @file poly-2b-A1B2Z2_C-v1x.h
 * @brief Contains the structure that allows the evaluation of the polynomial for A1B2Z2_C1 symmetry.
 */

/**
 * @namespace h2o_ion
 * @brief Namespace that encloses the A1B2Z2_C1 polynomial structure for monoatomic ions- water
 */

namespace h2o_ion {

struct poly_2b_h2o_ion_v1x {
    // Degree of the polynomial
    static const unsigned degree = 5;
    // Number of varables
    static const unsigned n_vars = 8;
    // Number of terms
    static const unsigned size = 429;

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2Z2_C1 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B2Z2_C1
     * symmetry
     * @param[in] nd Number of dimers to be evaluated
     * @param[in] a Double array of 429 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 8*nd with the variable values. It is in vectorized form, which means that the
     * order is var1_sys1 var1_sys2 ... var1_sysnd var2_sys1 ...
     * @return Vector of nd elements with the polynomial evaluation of each system
     */
    static std::vector<double> eval(const size_t nd, const double a[429], const double* x);

    /**
     * @brief Evaluates the polynomial of degree 5 for A1B2Z2_C1 symmetry.
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the A1B2Z2_C1
    symmetry, and calculates the gradients.
     * @param[in] nd Number of dimers to be evaluated
     * @param[in] a Double array of 429 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 8*nd with the variable values. It is in vectorized form, which means that the
    o rder is var1_sys1 var1_sys2 ... var1_sysnd var2_sys1 ...
     * @param[out] g Double array of length 8*nd that will store the gradients dP/dxi. It is in vectorized form, which
    means that the order is var1_sys1 var1_sys2 ... var1_sysnd var2_sys1 ...
     * @return Vector of nd elements with the polynomial evaluation of each system
     */
    static std::vector<double> eval(const size_t nd, const double a[429], const double* x, double* g);
};

}  // namespace h2o_ion

#endif  // POLY_2B_H2O_ION_V1X_H
