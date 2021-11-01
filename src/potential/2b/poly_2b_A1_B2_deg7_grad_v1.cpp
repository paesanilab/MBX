
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
 * @file poly_2b_A1_B2_deg7_grad_v1.cpp
 * @brief Contains the implementation of the polynomials with gradients for symmetry A1_B2
 */

/**
 * @namespace mbnrg_A1_B2_deg7
 * @brief Encloses the structure of the polynomial holder for symmetry A1_B2
 */
namespace mbnrg_A1_B2_deg7 {

double poly_A1_B2_deg7_v1::eval(const double x[3], const double a[62], double g[3]) {
    const double t1 = a[0];
    const double t2 = a[1];
    const double t3 = a[4];
    const double t4 = a[11];
    const double t5 = a[20];
    const double t6 = x[2];
    const double t7 = t6 * a[57];
    const double t8 = a[33];
    const double t10 = (t7 + t8) * t6;
    const double t12 = (t5 + t10) * t6;
    const double t14 = (t4 + t12) * t6;
    const double t16 = (t3 + t14) * t6;
    const double t18 = (t2 + t16) * t6;
    const double t19 = a[2];
    const double t20 = a[8];
    const double t21 = a[12];
    const double t22 = a[22];
    const double t24 = t6 * a[46];
    const double t25 = a[28];
    const double t27 = (t24 + t25) * t6;
    const double t29 = (t22 + t27) * t6;
    const double t31 = (t21 + t29) * t6;
    const double t33 = (t20 + t31) * t6;
    const double t34 = a[5];
    const double t35 = a[9];
    const double t36 = a[26];
    const double t38 = a[48] * t6;
    const double t39 = a[30];
    const double t41 = (t38 + t39) * t6;
    const double t43 = (t36 + t41) * t6;
    const double t45 = (t35 + t43) * t6;
    const double t46 = a[16];
    const double t47 = a[19];
    const double t49 = t6 * a[50];
    const double t50 = a[41];
    const double t52 = (t49 + t50) * t6;
    const double t54 = (t47 + t52) * t6;
    const double t55 = a[23];
    const double t57 = t6 * a[47];
    const double t58 = a[40];
    const double t60 = (t57 + t58) * t6;
    const double t61 = a[44];
    const double t68 = x[1];
    const double t62 = t68 * t61;
    const double t63 = a[43];
    const double t64 = t6 * t63;
    const double t65 = a[36];
    const double t67 = (t62 + t64 + t65) * t68;
    const double t69 = (t55 + t60 + t67) * t68;
    const double t71 = (t46 + t54 + t69) * t68;
    const double t73 = (t34 + t45 + t71) * t68;
    const double t75 = (t19 + t33 + t73) * t68;
    const double t78 = a[3];
    const double t79 = a[7];
    const double t80 = a[14];
    const double t81 = a[17];
    const double t83 = t6 * a[56];
    const double t84 = a[42];
    const double t86 = (t83 + t84) * t6;
    const double t88 = (t81 + t86) * t6;
    const double t90 = (t80 + t88) * t6;
    const double t92 = (t79 + t90) * t6;
    const double t93 = a[6];
    const double t94 = a[10];
    const double t95 = a[24];
    const double t97 = t6 * a[60];
    const double t98 = a[37];
    const double t100 = (t97 + t98) * t6;
    const double t102 = (t95 + t100) * t6;
    const double t104 = (t94 + t102) * t6;
    const double t105 = a[13];
    const double t106 = a[25];
    const double t108 = t6 * a[61];
    const double t109 = a[31];
    const double t111 = (t108 + t109) * t6;
    const double t113 = (t106 + t111) * t6;
    const double t114 = a[21];
    const double t116 = t6 * a[59];
    const double t117 = a[34];
    const double t119 = (t116 + t117) * t6;
    const double t120 = a[45];
    const double t121 = t68 * t120;
    const double t122 = a[52];
    const double t123 = t6 * t122;
    const double t124 = a[35];
    const double t126 = (t121 + t123 + t124) * t68;
    const double t128 = (t114 + t119 + t126) * t68;
    const double t130 = (t105 + t113 + t128) * t68;
    const double t132 = (t93 + t104 + t130) * t68;
    const double t134 = (t78 + t92 + t132) * t68;
    const double t135 = a[15];
    const double t136 = a[18];
    const double t138 = t6 * a[51];
    const double t139 = a[39];
    const double t141 = (t138 + t139) * t6;
    const double t143 = (t136 + t141) * t6;
    const double t144 = a[27];
    const double t146 = t6 * a[54];
    const double t147 = a[38];
    const double t149 = (t146 + t147) * t6;
    const double t151 = t68 * a[53];
    const double t152 = a[58];
    const double t153 = t6 * t152;
    const double t154 = a[29];
    const double t156 = (t151 + t153 + t154) * t68;
    const double t158 = (t144 + t149 + t156) * t68;
    const double t160 = (t135 + t143 + t158) * t68;
    const double t162 = (t93 + t104 + t160) * t68;
    const double t164 = t68 * a[55];
    const double t165 = a[49];
    const double t166 = t6 * t165;
    const double t167 = a[32];
    const double t169 = (t164 + t166 + t167) * t68;
    const double t171 = (t144 + t149 + t169) * t68;
    const double t173 = (t105 + t113 + t171) * t68;
    const double t175 = (t164 + t153 + t154) * t68;
    const double t177 = (t114 + t119 + t175) * t68;
    const double t179 = (t151 + t123 + t124) * t68;
    const double t181 = x[0];
    const double t180 = t181 * t61;
    const double t182 = (t180 + t121 + t64 + t65) * t181;
    const double t184 = (t55 + t60 + t179 + t182) * t181;
    const double t186 = (t46 + t54 + t177 + t184) * t181;
    const double t188 = (t34 + t45 + t173 + t186) * t181;
    const double t190 = (t19 + t33 + t162 + t188) * t181;
    const double t226 = 2.0 * t151;
    const double t233 = 2.0 * t164;
    const double t260 = ((((2.0 * t7 + t8) * t6 + t5 + t10) * t6 + t4 + t12) * t6 + t3 + t14) * t6;
    const double t267 = (((2.0 * t24 + t25) * t6 + t22 + t27) * t6 + t21 + t29) * t6;
    const double t272 = ((2.0 * t38 + t39) * t6 + t36 + t41) * t6;
    const double t275 = (2.0 * t49 + t50) * t6;
    const double t277 = 2.0 * t57;
    const double t299 = ((2.0 * t97 + t98) * t6 + t95 + t100) * t6;
    const double t302 = (2.0 * t108 + t109) * t6;
    const double t303 = t68 * t122;
    const double t304 = 2.0 * t116;
    const double t316 = t68 * t152;
    const double t317 = 2.0 * t146;
    g[0] =
        (((((2.0 * t180 + t121 + t64 + t65) * t181 + t55 + t60 + t179 + t182) * t181 + t46 + t54 + t177 + t184) * t181 +
          t34 + t45 + t173 + t186) *
             t181 +
         t19 + t33 + t162 + t188) *
            t181 +
        t1 + t18 + t134 + t190;
    g[1] =
        (((((2.0 * t62 + t64 + t65) * t68 + t55 + t60 + t67) * t68 + t46 + t54 + t69) * t68 + t34 + t45 + t71) * t68 +
         t19 + t33 + t73) *
            t68 +
        t1 + t18 + t75 +
        (((((2.0 * t121 + t123 + t124) * t68 + t114 + t119 + t126) * t68 + t105 + t113 + t128) * t68 + t93 + t104 +
          t130) *
             t68 +
         t78 + t92 + t132 +
         ((((t226 + t153 + t154) * t68 + t144 + t149 + t156) * t68 + t135 + t143 + t158) * t68 + t93 + t104 + t160 +
          (((t233 + t166 + t167) * t68 + t144 + t149 + t169) * t68 + t105 + t113 + t171 +
           ((t233 + t153 + t154) * t68 + t114 + t119 + t175 + (t120 * t181 + t123 + t124 + t226) * t181) * t181) *
              t181) *
             t181) *
            t181;
    g[2] = (t260 + t2 + t16 +
            (t267 + t20 + t31 + (t272 + t35 + t43 + (t275 + t47 + t52 + (t63 * t68 + t277 + t58) * t68) * t68) * t68) *
                t68) *
               t68 +
           (t260 + t2 + t16 +
            ((((2.0 * t83 + t84) * t6 + t81 + t86) * t6 + t80 + t88) * t6 + t79 + t90 +
             (t299 + t94 + t102 + (t302 + t106 + t111 + (t303 + t304 + t117) * t68) * t68) * t68) *
                t68 +
            (t267 + t20 + t31 +
             (t299 + t94 + t102 + ((2.0 * t138 + t139) * t6 + t136 + t141 + (t316 + t317 + t147) * t68) * t68) * t68 +
             (t272 + t35 + t43 + (t302 + t106 + t111 + (t165 * t68 + t147 + t317) * t68) * t68 +
              (t275 + t47 + t52 + (t316 + t304 + t117) * t68 + (t63 * t181 + t277 + t303 + t58) * t181) * t181) *
                 t181) *
                t181) *
               t181;
    return (t1 + t18 + t75) * t68 + (t1 + t18 + t134 + t190) * t181;
}

}  // namespace mbnrg_A1_B2_deg7
