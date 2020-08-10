#include "poly_2b_A1_B1_deg15_v1.h"

namespace mbnrg_A1_B1_deg15 {

double poly_A1_B1_deg15_v1::eval(const double x[1], const double a[15], double g[1]) {
    const double t1 = a[0];
    const double t2 = a[1];
    const double t3 = a[2];
    const double t4 = a[3];
    const double t5 = a[4];
    const double t6 = a[5];
    const double t7 = a[6];
    const double t8 = a[7];
    const double t9 = a[8];
    const double t10 = a[9];
    const double t11 = a[10];
    const double t12 = a[11];
    const double t13 = a[12];
    const double t17 = x[0];
    const double t15 = a[14] * t17;
    const double t16 = a[13];
    const double t18 = (t15 + t16) * t17;
    const double t20 = (t13 + t18) * t17;
    const double t22 = (t12 + t20) * t17;
    const double t24 = (t11 + t22) * t17;
    const double t26 = (t10 + t24) * t17;
    const double t28 = (t9 + t26) * t17;
    const double t30 = (t8 + t28) * t17;
    const double t32 = (t7 + t30) * t17;
    const double t34 = (t6 + t32) * t17;
    const double t36 = (t5 + t34) * t17;
    const double t38 = (t4 + t36) * t17;
    const double t40 = (t3 + t38) * t17;
    const double t42 = (t2 + t40) * t17;
    g[0] = (((((((((((((2.0 * t15 + t16) * t17 + t13 + t18) * t17 + t12 + t20) * t17 + t11 + t22) * t17 + t10 + t24) *
                       t17 +
                   t9 + t26) *
                      t17 +
                  t8 + t28) *
                     t17 +
                 t7 + t30) *
                    t17 +
                t6 + t32) *
                   t17 +
               t5 + t34) *
                  t17 +
              t4 + t36) *
                 t17 +
             t3 + t38) *
                t17 +
            t2 + t40) *
               t17 +
           t1 + t42;
    return (t1 + t42) * t17;
}

}  // namespace mbnrg_A1_B1_deg15
