#include "poly_2b_A1_A1_deg23_v1x.h"

namespace x2b_A1_A1_deg23 {

double poly_2b_A1_A1_deg23_v1x::eval(const double a[23], const double x[1]) {
    const double t23 = x[0];
    return (
        (a[0] +
         (a[1] +
          (a[2] +
           (a[3] +
            (a[4] +
             (a[5] +
              (a[6] +
               (a[7] +
                (a[8] +
                 (a[9] +
                  (a[10] +
                   (a[11] +
                    (a[12] +
                     (a[13] +
                      (a[14] +
                       (a[15] +
                        (a[16] +
                         (a[17] + (a[18] + (a[19] + (a[20] + (a[22] * t23 + a[21]) * t23) * t23) * t23) * t23) * t23) *
                            t23) *
                           t23) *
                          t23) *
                         t23) *
                        t23) *
                       t23) *
                      t23) *
                     t23) *
                    t23) *
                   t23) *
                  t23) *
                 t23) *
                t23) *
               t23) *
              t23) *
             t23) *
        t23);
}

}  // namespace x2b_A1_A1_deg23
