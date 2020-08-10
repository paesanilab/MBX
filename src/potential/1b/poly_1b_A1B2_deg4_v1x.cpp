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
    return (t1 + t9) * t6 + (t12 + t20 + t35) * t25 + (t12 + t20 + t52 + t61) * t53;
}

}  // namespace x1b_A1B2_deg4
