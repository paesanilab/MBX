#include "poly_1b_A1B2_deg6_vmbpbe.h"

namespace mbnrg_A1B2_deg6 {

double poly_A1B2_deg6_vmbpbe::eval(const double x[3], const double a[49]) {
    const double t18 = a[0];
    const double t5 = x[2];
    const double t32 = (a[2] + (a[6] + (a[14] + (a[39] * t5 + a[24]) * t5) * t5) * t5) * t5;
    const double t33 = a[3];
    const double t44 = (a[11] + (a[15] + (a[33] * t5 + a[26]) * t5) * t5) * t5;
    const double t45 = a[7];
    const double t53 = (a[12] + (a[36] * t5 + a[31]) * t5) * t5;
    const double t54 = a[20];
    const double t59 = (a[47] * t5 + a[23]) * t5;
    const double t60 = a[42];
    const double t63 = a[46] * t5;
    const double t64 = a[28];
    const double t87 = a[8];
    const double t95 = (a[13] + (a[43] * t5 + a[29]) * t5) * t5;
    const double t96 = a[16];
    const double t101 = (a[37] * t5 + a[30]) * t5;
    const double t56 = x[1];
    const double t103 = a[41] * t56;
    const double t105 = a[40] * t5;
    const double t106 = a[25];
    const double t122 = a[34] * t56;
    const double t124 = a[44] * t5;
    const double t125 = a[32];
    const double t142 = x[0];
    return ((a[1] + (a[5] + (a[9] + (a[19] + (a[35] * t5 + a[27]) * t5) * t5) * t5) * t5) * t5 +
            (t18 + t32 + (t33 + t44 + (t45 + t53 + (t54 + t59 + (t60 * t56 + t63 + t64) * t56) * t56) * t56) * t56) *
                t56 +
            (t18 + t32 +
             (a[4] + (a[10] + (a[17] + (a[48] * t5 + a[21]) * t5) * t5) * t5 +
              (t87 + t95 + (t96 + t101 + (t103 + t105 + t106) * t56) * t56) * t56) *
                 t56 +
             (t33 + t44 + (t87 + t95 + (a[18] + (a[45] * t5 + a[22]) * t5 + (t122 + t124 + t125) * t56) * t56) * t56 +
              (t45 + t53 + (t96 + t101 + (a[38] * t56 + t124 + t125) * t56) * t56 +
               (t54 + t59 + (t122 + t105 + t106) * t56 + (t60 * t142 + t103 + t63 + t64) * t142) * t142) *
                  t142) *
                 t142) *
                t142);
}

}  // namespace mbnrg_A1B2_deg6
