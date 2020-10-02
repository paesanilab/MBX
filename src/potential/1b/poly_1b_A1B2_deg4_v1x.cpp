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

#include "poly_1b_A1B2_deg4_v1x.h"

namespace x1b_A1B2_deg4 {

double poly_1b_A1B2_deg4_v1x::eval(const double a[21], const double x[3], double g[3]) {
    const double t1 = a[1];
    const double t2 = a[4];
    const double t6 = x[2];
    const double t4 = a[20] * t6;
    const double t5 = a[11];
    const double t7 = (t4 + t5) * t6;
    const double t9 = (t2 + t7) * t6;
    const double t12 = a[0];
    const double t13 = a[3];
    const double t15 = a[14] * t6;
    const double t16 = a[8];
    const double t18 = (t15 + t16) * t6;
    const double t20 = (t13 + t18) * t6;
    const double t21 = a[5];
    const double t23 = a[18] * t6;
    const double t24 = a[7];
    const double t26 = (t23 + t24) * t6;
    const double t27 = a[13];
    const double t25 = x[1];
    const double t28 = t27 * t25;
    const double t29 = a[17];
    const double t30 = t29 * t6;
    const double t31 = a[6];
    const double t33 = (t28 + t30 + t31) * t25;
    const double t35 = (t21 + t26 + t33) * t25;
    const double t38 = a[2];
    const double t40 = a[16] * t6;
    const double t41 = a[9];
    const double t43 = (t40 + t41) * t6;
    const double t44 = a[15];
    const double t45 = t44 * t25;
    const double t46 = a[12];
    const double t47 = t46 * t6;
    const double t48 = a[10];
    const double t50 = (t45 + t47 + t48) * t25;
    const double t52 = (t38 + t43 + t50) * t25;
    const double t54 = a[19] * t25;
    const double t56 = (t54 + t47 + t48) * t25;
    const double t53 = x[0];
    const double t57 = t27 * t53;
    const double t59 = (t57 + t45 + t30 + t31) * t53;
    const double t61 = (t21 + t26 + t56 + t59) * t53;
    const double t93 = (2.0 * t15 + t16) * t6;
    const double t95 = 2.0 * t23;
    const double t100 = t46 * t25;
    g[0] = ((2.0 * t57 + t45 + t30 + t31) * t53 + t21 + t26 + t56 + t59) * t53 + t12 + t20 + t52 + t61;
    g[1] = ((2.0 * t28 + t30 + t31) * t25 + t21 + t26 + t33) * t25 + t12 + t20 + t35 +
           ((2.0 * t45 + t47 + t48) * t25 + t38 + t43 + t50 + (t44 * t53 + t47 + t48 + 2.0 * t54) * t53) * t53;
    g[2] = ((2.0 * t4 + t5) * t6 + t2 + t7) * t6 + t1 + t9 + (t93 + t13 + t18 + (t29 * t25 + t24 + t95) * t25) * t25 +
           (t93 + t13 + t18 + (t100 + 2.0 * t40 + t41) * t25 + (t29 * t53 + t100 + t24 + t95) * t53) * t53;
    double e = (t1 + t9) * t6 + (t12 + t20 + t35) * t25 + (t12 + t20 + t52 + t61) * t53;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input linear constants (a):\n";
    for (size_t j = 0; j < 21; j++) {
        std::cerr << a[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input polynomial variables (x):\n";
    for (size_t j = 0; j < 3; j++) {
        std::cerr << x[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output polynomial gradients (g):\n";
    for (size_t j = 0; j < 3; j++) {
        std::cerr << g[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output polynomial energy: " << e << std::endl;
#endif

    return e;
}

}  // namespace x1b_A1B2_deg4
