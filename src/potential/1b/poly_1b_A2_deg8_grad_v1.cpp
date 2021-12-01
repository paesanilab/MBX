
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

#include "poly_1b_A2_deg8_v1.h"

/**
 * @file poly_1b_A2_deg8_grad_v1.cpp
 * @brief Contains the implementation of the polynomials with gradients for symmetry A2
 */

/**
 * @namespace mbnrg_A2_deg8
 * @brief Encloses the structure of the polynomial holder for symmetry A2
 */
namespace mbnrg_A2_deg8 {

double poly_A2_deg8_v1::eval(const double x[1], const double a[8], double g[1]) {
    const double t1 = a[0];
    const double t2 = a[1];
    const double t3 = a[2];
    const double t4 = a[3];
    const double t5 = a[4];
    const double t6 = a[5];
    const double t10 = x[0];
    const double t8 = a[7] * t10;
    const double t9 = a[6];
    const double t11 = (t8 + t9) * t10;
    const double t13 = (t6 + t11) * t10;
    const double t15 = (t5 + t13) * t10;
    const double t17 = (t4 + t15) * t10;
    const double t19 = (t3 + t17) * t10;
    const double t21 = (t2 + t19) * t10;
    g[0] =
        ((((((2.0 * t8 + t9) * t10 + t6 + t11) * t10 + t5 + t13) * t10 + t4 + t15) * t10 + t3 + t17) * t10 + t2 + t19) *
            t10 +
        t1 + t21;
    return (t1 + t21) * t10;
}

}  // namespace mbnrg_A2_deg8
