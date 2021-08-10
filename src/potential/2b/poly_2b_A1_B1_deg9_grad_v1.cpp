
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

#include "poly_2b_A1_B1_deg9_v1.h"

/**
 * @file poly_2b_A1_B1_deg9_grad_v1.cpp
 * @brief Contains the implementation of the polynomials with gradients for symmetry A1_B1
 */

/**
 * @namespace mbnrg_A1_B1_deg9
 * @brief Encloses the structure of the polynomial holder for symmetry A1_B1
 */
namespace mbnrg_A1_B1_deg9 {

double poly_A1_B1_deg9_v1::eval(const double x[1], const double a[9], double g[1]) {
    const double t1 = a[0];
    const double t2 = a[1];
    const double t3 = a[2];
    const double t4 = a[3];
    const double t5 = a[4];
    const double t6 = a[5];
    const double t7 = a[6];
    const double t11 = x[0];
    const double t9 = a[8] * t11;
    const double t10 = a[7];
    const double t12 = (t9 + t10) * t11;
    const double t14 = (t7 + t12) * t11;
    const double t16 = (t6 + t14) * t11;
    const double t18 = (t5 + t16) * t11;
    const double t20 = (t4 + t18) * t11;
    const double t22 = (t3 + t20) * t11;
    const double t24 = (t2 + t22) * t11;
    g[0] = (((((((2.0 * t9 + t10) * t11 + t7 + t12) * t11 + t6 + t14) * t11 + t5 + t16) * t11 + t4 + t18) * t11 + t3 +
             t20) *
                t11 +
            t2 + t22) *
               t11 +
           t1 + t24;
    return (t1 + t24) * t11;
}

}  // namespace mbnrg_A1_B1_deg9
