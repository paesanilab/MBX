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

#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H

#include <vector>
#include <iostream>
#include <cmath>
#include "tools/custom_exceptions.h"

#ifdef _OPENMP
#include <omp.h>
#endif

/**
 * @file math_tools.h
 * @brief Function definitions for some mathematical functions and utilities
 */

/**
 * @brief Inverts a matrix
 *
 * Given a 9 element vector (a 3x3 matrix) with the unit cell vectors in rows (v1;v2;v3), return the inverse of that
 * matrix.
 * @param[in] box Matrix ordered by rows witht he cell vectors
 * @return Vector with the inverse (row1,row2,row3) of the input matrix
 */
std::vector<double> InvertUnitCell(const std::vector<double> &box);

/**
 * @brief Calculates the switch function
 *
 * Given a distance, an inner cutoff and an outer cutoff, calculates the switch function with formula sw(r) = [1.0 +
 * cos( [r-ri]/[ro - ri]*PI ) ] /2.0, and its gradient with respect to r.
 * @param[in] r Distance
 * @param[in] ri Inner cutoff
 * @param[in] ro Outer cutoff
 * @param[out] g Gradient with respect to r
 * @return Value of the switch for r
 */
double switch_function(const double &r, const double &ri, const double &ro, double &g);

/**
 * @brief Returns if a double, given a threshold, is 0
 *
 * Given a double and a threshold for which a double will be considered 0, returns true if that double is lower than the
 * threshold (i.e., is 0)
 * @param[in] a Double to inquire.
 * @param[in] t Optional threshold. Default set to 1E-06.
 * @return True if |a| < |t|, false otherwise
 */
bool IsZero(double a, double t = 1E-06);

/**
 * @brief Given a box defined by three vectors, returns that box defined by three magnitudes and three angles (ABC alpha
 * beta gamma notation)
 *
 * This function calculates the ABC notation for a box given in a 9-element vector with the three vectors of the box,
 * one after the other one: {v1x,v1y,v1z,v2x...}. For reference, A,B and C are the modulus of the three vectors, while
 * alpha is the angle between v1 and v3, beta the angle between v2 and b3, and gamma the angle between v1 and v2. V1 is
 * supposed to be oriented along the x-axis, v2 must be in the xy plane, and v3 must have a z component.
 * @param[in] box Vector containing the 9-element box
 * @return Vector of 6 elements with the ABC notation: {A,B,C,alpha,beta,gamma}
 */
std::vector<double> BoxVecToBoxABCabc(std::vector<double> box);

/**
 * @brief Given a box defined by ABC notation, returns the three vectors of the box
 *
 * This function calculates the three vectors from the ABC notation for a box given in a 6-element vector with
 * A,B,C,alpha,beta,gamma. For reference, A,B and C are the modulus of the three vectors, while alpha is the angle
 * between v1 and v3, beta the angle between v2 and b3, and gamma the angle between v1 and v2. V1 is supposed to be
 * oriented along the x-axis, v2 must be in the xy plane, and v3 must have a z component.
 * @param[in] box Vector containing the 6-element box ABCabc
 * @return Vector of 9 elements with the three vectors of the box: {v1,v2,v3}.
 */
std::vector<double> BoxABCabcToBoxVec(std::vector<double> box_abc);

/**
 * @brief Fills c with the product A*b
 *
 * Given a matrix A (m x n) in vector form {row1,row2,row3...) and a vector b(nx1), fills in in c(mx1) the product A*b.
 * @param[in] A Matrix m x n in vector form (row1, row2...row m)
 * @param[in] b Vector n x 1
 * @param[out] c Vector m x 1 with the product A*b
 */
template <typename T>
void MatrixTimesVector(const std::vector<T> &A, const std::vector<T> &b, std::vector<T> &c) {
    // Check indexes match

    size_t ncols = b.size();
    size_t nrows = A.size() / ncols;
    size_t nthreads = 1;

    c = std::vector<T>(nrows, 0.0);

    if (A.size() % b.size() != 0) {
        std::string text = "ERROR: Indexes do not match. " + std::to_string(A.size()) + " is not a multiple of " +
                           std::to_string(b.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

#ifdef _OPENMP
#pragma omp parallel
    {
        if (omp_get_thread_num() == 0) nthreads = omp_get_num_threads();
    }
#endif

    size_t istart = 0;
    size_t iend = nrows;

#ifdef _OPENMP
#pragma omp parallel private(istart, iend)
    {
        int thread_id = omp_get_thread_num();
        size_t size = nrows / nthreads;
        istart = size * thread_id;
        iend = thread_id == nthreads - 1 ? nrows : istart + size;
#endif
        for (size_t i = istart; i < iend; i++) {
            size_t incols = i * ncols;
            T a = 0;
            for (size_t j = 0; j < ncols; j++) {
                a += A[incols + j] * b[j];
            }
            c[i] = a;
        }

#ifdef _OPENMP
    }
#endif
}

/**
 * @brief Returns the dot product a.b
 *
 * Given two vectors of the same length, returns the dot product between them
 * @param[in] a Vector with the first vector
 * @param[in] b Vector with the second vector
 * @return Dot product between a and b
 */
template <typename T>
T DotProduct(const std::vector<T> &a, const std::vector<T> &b) {
    // Check that sizes are the same
    if (a.size() != b.size()) {
        std::string text =
            "Indexes do not match. " + std::to_string(a.size()) + " does not match " + std::to_string(b.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    T c = 0;
    for (size_t i = 0; i < b.size(); i++) {
        c += a[i] * b[i];
    }

    return c;
}

#endif
