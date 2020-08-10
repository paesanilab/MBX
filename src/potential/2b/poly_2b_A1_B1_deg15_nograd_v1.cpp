#include "poly_2b_A1_B1_deg15_v1.h"

namespace mbnrg_A1_B1_deg15 {

double poly_A1_B1_deg15_v1::eval(const double x[1], const double a[15]) {
    const double t15 = x[0];
    return (
        (a[0] +
         (a[1] +
          (a[2] +
           (a[3] +
            (a[4] +
             (a[5] +
              (a[6] +
               (a[7] +
                (a[8] + (a[9] + (a[10] + (a[11] + (a[12] + (a[14] * t15 + a[13]) * t15) * t15) * t15) * t15) * t15) *
                    t15) *
                   t15) *
                  t15) *
                 t15) *
                t15) *
               t15) *
              t15) *
             t15) *
        t15);
}

}  // namespace mbnrg_A1_B1_deg15
