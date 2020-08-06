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

// Vecotr based
template <typename T>
void MatrixTimesVector(const std::vector<T> &A, const std::vector<T> &b, std::vector<T> &c) {
    // Check indexes match
    if (A.size() % b.size() != 0) {
        std::cerr << "ERROR: Indexes do not match. " << A.size() << " is not a multiple of " << b.size() << std::endl;
    }

    size_t ncols = b.size();
    size_t nrows = A.size() / ncols;
    size_t nthreads = 1;

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

template <typename T>
T DotProduct(const std::vector<T> &a, const std::vector<T> &b) {
    // Check that sizes are the same
    if (a.size() != b.size()) {
        std::string text = "Indexes do not match. " + std::to_string(a.size()) + " does not match " + std::to_string(b.size());
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    T c = 0;
    for (size_t i = 0; i < b.size(); i++) {
        c += a[i] * b[i];
    }

    return c;
}

std::vector<double> InvertUnitCell(const std::vector<double> &box);

double switch_function(const double &r, const double &ri, const double &ro, double &g);

bool IsZero(double a, double t = 1E-06);
std::vector<double> BoxVecToBoxABCabc(std::vector<double>);
std::vector<double> BoxABCabcToBoxVec(std::vector<double>);

template <typename T>
void MatrixTimesVector(const T *A, const T *b, T *c, size_t sizeA, size_t sizeb) {
    // Check indexes match
    if (sizeA % sizeb != 0) {
        std::cerr << "ERROR: Indexes do not match. " << sizeA << " is not a multiple of " << sizeb << std::endl;
    }

    size_t ncols = sizeb;
    size_t nrows = sizeA / ncols;

    for (size_t i = 0; i < nrows; i++) {
        size_t incols = i * ncols;
        c[i] = 0;
        for (size_t j = 0; j < ncols; j++) {
            c[i] += A[incols + j] * b[j];
        }
    }
}

#endif
