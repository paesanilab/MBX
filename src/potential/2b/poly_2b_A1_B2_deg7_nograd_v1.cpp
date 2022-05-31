
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

#include "poly_2b_A1_B2_deg7_v1.h"

/**
 * @file poly_2b_A1_B2_deg7_nograd_v1.cpp
 * @brief Contains the implementation of the polynomials without gradients for symmetry A1_B2
 */

/**
 * @namespace mbnrg_A1_B2_deg7
 * @brief Encloses the structure of the polynomial for symmetry A1_B2
 */

namespace mbnrg_A1_B2_deg7 {

double poly_A1_B2_deg7_v1::eval(const double x[3], const double a[62]) {
    const double t1 = a[0];
    const double t7 = x[2];
    const double t18 = (a[1] + (a[4] + (a[11] + (a[20] + (a[57] * t7 + a[33]) * t7) * t7) * t7) * t7) * t7;
    const double t19 = a[2];
    const double t33 = (a[8] + (a[12] + (a[22] + (a[46] * t7 + a[28]) * t7) * t7) * t7) * t7;
    const double t34 = a[5];
    const double t45 = (a[9] + (a[26] + (a[48] * t7 + a[30]) * t7) * t7) * t7;
    const double t46 = a[16];
    const double t54 = (a[19] + (a[50] * t7 + a[41]) * t7) * t7;
    const double t55 = a[23];
    const double t60 = (a[47] * t7 + a[40]) * t7;
    const double t61 = a[44];
    const double t64 = a[43] * t7;
    const double t65 = a[36];
    const double t93 = a[6];
    const double t104 = (a[10] + (a[24] + (a[60] * t7 + a[37]) * t7) * t7) * t7;
    const double t105 = a[13];
    const double t113 = (a[25] + (a[61] * t7 + a[31]) * t7) * t7;
    const double t114 = a[21];
    const double t119 = (a[59] * t7 + a[34]) * t7;
    const double t88 = x[1];
    const double t121 = a[45] * t88;
    const double t123 = a[52] * t7;
    const double t124 = a[35];
    const double t144 = a[27];
    const double t149 = (a[54] * t7 + a[38]) * t7;
    const double t151 = a[53] * t88;
    const double t153 = a[58] * t7;
    const double t154 = a[29];
    const double t164 = a[55] * t88;
    const double t182 = x[0];
    return ((t1 + t18 +
             (t19 + t33 + (t34 + t45 + (t46 + t54 + (t55 + t60 + (t61 * t88 + t64 + t65) * t88) * t88) * t88) * t88) *
                 t88) *
                t88 +
            (t1 + t18 +
             (a[3] + (a[7] + (a[14] + (a[17] + (a[56] * t7 + a[42]) * t7) * t7) * t7) * t7 +
              (t93 + t104 + (t105 + t113 + (t114 + t119 + (t121 + t123 + t124) * t88) * t88) * t88) * t88) *
                 t88 +
             (t19 + t33 +
              (t93 + t104 +
               (a[15] + (a[18] + (a[51] * t7 + a[39]) * t7) * t7 + (t144 + t149 + (t151 + t153 + t154) * t88) * t88) *
                   t88) *
                  t88 +
              (t34 + t45 + (t105 + t113 + (t144 + t149 + (a[49] * t7 + t164 + a[32]) * t88) * t88) * t88 +
               (t46 + t54 + (t114 + t119 + (t164 + t153 + t154) * t88) * t88 +
                (t55 + t60 + (t151 + t123 + t124) * t88 + (t61 * t182 + t121 + t64 + t65) * t182) * t182) *
                   t182) *
                  t182) *
                 t182) *
                t182);
}

}  // namespace mbnrg_A1_B2_deg7
