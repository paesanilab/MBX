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

#ifndef POLY_2B_V6X_H
#define POLY_2B_V6X_H

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif

#include <vector>
#include <cstddef>

/**
 * @file poly-2b-v6x.h
 * @brief Contains the structure that allows the evaluation of the polynomial for water (MB-pol).
 */

/**
 * @namespace x2o
 * @brief Namespace that encloses the MB-pol 2b polynomial structure
 */

namespace x2o {

struct poly_2b_v6x {
    // Degree of the polynomial
    static const unsigned degree = 4;
    // Number of varables
    static const unsigned n_vars = 31;
    // Number of terms
    static const unsigned size = 1153;

    /**
     * @brief Evaluates the polynomial of degree 4 for MB-pol 2b
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the two-body
     * interactions for water. This is the 2b correction for MB-pol water
     * @param[in] nd Number of dimers to be evaluated
     * @param[in] a Double array of 1153 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 31*nd with the variable values. It is in vectorized form, which means that
     * the order is var1_sys1 var1_sys2 ... var1_sysnd var2_sys1 ...
     * @return Vector of nd elements with the polynomial evaluation of each system
     */
    static std::vector<double> eval(const size_t nd, const double* a, const double* x);

    /**
     * @brief Evaluates the polynomial of degree 4 for MB-pol 2b
     *
     * Given the linear parameters and the value of the polynomial variables, evaluates the polynomial for the two-body
     * interactions for water. This is the 2b correction for MB-pol water. It also calculates the gradients dP/dxi.
     * @param[in] nd Number of dimers to be evaluated
     * @param[in] a Double array of 1153 elements with the linear parameters of the polynomial
     * @param[in] x Double array of length 31*nd with the variable values. It is in vectorized form, which means that
     * the order is var1_sys1 var1_sys2 ... var1_sysnd var2_sys1 ...
     * @param[out] g Double array of length 31*nd that will store the gradients dP/dxi. It is in vectorized form, which
     * means that the order is var1_sys1 var1_sys2 ... var1_sysnd var2_sys1 ...
     * @return Vector of nd elements with the polynomial evaluation of each system
     */
    static std::vector<double> eval(const size_t nd, const double* a, const double* x, double* g);
};

}  // namespace x2o

#endif  // POLY_2B_V6X_H
