#include "poly_1b_A1B2_deg6_vmbpbe.h"

namespace mbnrg_A1B2_deg6 {

double poly_A1B2_deg6_vmbpbe::eval(const double x[3], const double a[49], double g[3]) {
    const double t1 = a[1];
    const double t2 = a[5];
    const double t3 = a[9];
    const double t4 = a[19];
    const double t8 = x[2];
    const double t6 = a[35] * t8;
    const double t7 = a[27];
    const double t9 = (t6 + t7) * t8;
    const double t11 = (t4 + t9) * t8;
    const double t13 = (t3 + t11) * t8;
    const double t15 = (t2 + t13) * t8;
    const double t18 = a[0];
    const double t19 = a[2];
    const double t20 = a[6];
    const double t21 = a[14];
    const double t23 = a[39] * t8;
    const double t24 = a[24];
    const double t26 = (t23 + t24) * t8;
    const double t28 = (t21 + t26) * t8;
    const double t30 = (t20 + t28) * t8;
    const double t32 = (t19 + t30) * t8;
    const double t33 = a[3];
    const double t34 = a[11];
    const double t35 = a[15];
    const double t37 = a[33] * t8;
    const double t38 = a[26];
    const double t40 = (t37 + t38) * t8;
    const double t42 = (t35 + t40) * t8;
    const double t44 = (t34 + t42) * t8;
    const double t45 = a[7];
    const double t46 = a[12];
    const double t48 = a[36] * t8;
    const double t49 = a[31];
    const double t51 = (t48 + t49) * t8;
    const double t53 = (t46 + t51) * t8;
    const double t54 = a[20];
    const double t56 = a[47] * t8;
    const double t57 = a[23];
    const double t59 = (t56 + t57) * t8;
    const double t60 = a[42];
    const double t58 = x[1];
    const double t61 = t60 * t58;
    const double t62 = a[46];
    const double t63 = t62 * t8;
    const double t64 = a[28];
    const double t66 = (t61 + t63 + t64) * t58;
    const double t68 = (t54 + t59 + t66) * t58;
    const double t70 = (t45 + t53 + t68) * t58;
    const double t72 = (t33 + t44 + t70) * t58;
    const double t75 = a[4];
    const double t76 = a[10];
    const double t77 = a[17];
    const double t79 = a[48] * t8;
    const double t80 = a[21];
    const double t82 = (t79 + t80) * t8;
    const double t84 = (t77 + t82) * t8;
    const double t86 = (t76 + t84) * t8;
    const double t87 = a[8];
    const double t88 = a[13];
    const double t90 = a[43] * t8;
    const double t91 = a[29];
    const double t93 = (t90 + t91) * t8;
    const double t95 = (t88 + t93) * t8;
    const double t96 = a[16];
    const double t98 = a[37] * t8;
    const double t99 = a[30];
    const double t101 = (t98 + t99) * t8;
    const double t102 = a[41];
    const double t103 = t102 * t58;
    const double t104 = a[40];
    const double t105 = t104 * t8;
    const double t106 = a[25];
    const double t108 = (t103 + t105 + t106) * t58;
    const double t110 = (t96 + t101 + t108) * t58;
    const double t112 = (t87 + t95 + t110) * t58;
    const double t114 = (t75 + t86 + t112) * t58;
    const double t115 = a[18];
    const double t117 = a[45] * t8;
    const double t118 = a[22];
    const double t120 = (t117 + t118) * t8;
    const double t122 = a[34] * t58;
    const double t123 = a[44];
    const double t124 = t123 * t8;
    const double t125 = a[32];
    const double t127 = (t122 + t124 + t125) * t58;
    const double t129 = (t115 + t120 + t127) * t58;
    const double t131 = (t87 + t95 + t129) * t58;
    const double t133 = a[38] * t58;
    const double t135 = (t133 + t124 + t125) * t58;
    const double t137 = (t96 + t101 + t135) * t58;
    const double t139 = (t122 + t105 + t106) * t58;
    const double t136 = x[0];
    const double t140 = t60 * t136;
    const double t142 = (t140 + t103 + t63 + t64) * t136;
    const double t144 = (t54 + t59 + t139 + t142) * t136;
    const double t146 = (t45 + t53 + t137 + t144) * t136;
    const double t148 = (t33 + t44 + t131 + t146) * t136;
    const double t178 = 2.0 * t122;
    const double t211 = (((2.0 * t23 + t24) * t8 + t21 + t26) * t8 + t20 + t28) * t8;
    const double t216 = ((2.0 * t37 + t38) * t8 + t35 + t40) * t8;
    const double t219 = (2.0 * t48 + t49) * t8;
    const double t221 = 2.0 * t56;
    const double t237 = (2.0 * t90 + t91) * t8;
    const double t238 = t104 * t58;
    const double t239 = 2.0 * t98;
    const double t246 = t123 * t58;
    g[0] =
        ((((2.0 * t140 + t103 + t63 + t64) * t136 + t54 + t59 + t139 + t142) * t136 + t45 + t53 + t137 + t144) * t136 +
         t33 + t44 + t131 + t146) *
            t136 +
        t18 + t32 + t114 + t148;
    g[1] =
        ((((2.0 * t61 + t63 + t64) * t58 + t54 + t59 + t66) * t58 + t45 + t53 + t68) * t58 + t33 + t44 + t70) * t58 +
        t18 + t32 + t72 +
        ((((2.0 * t103 + t105 + t106) * t58 + t96 + t101 + t108) * t58 + t87 + t95 + t110) * t58 + t75 + t86 + t112 +
         (((t178 + t124 + t125) * t58 + t115 + t120 + t127) * t58 + t87 + t95 + t129 +
          ((2.0 * t133 + t124 + t125) * t58 + t96 + t101 + t135 + (t102 * t136 + t105 + t106 + t178) * t136) * t136) *
             t136) *
            t136;
    g[2] = ((((2.0 * t6 + t7) * t8 + t4 + t9) * t8 + t3 + t11) * t8 + t2 + t13) * t8 + t1 + t15 +
           (t211 + t19 + t30 + (t216 + t34 + t42 + (t219 + t46 + t51 + (t62 * t58 + t221 + t57) * t58) * t58) * t58) *
               t58 +
           (t211 + t19 + t30 +
            (((2.0 * t79 + t80) * t8 + t77 + t82) * t8 + t76 + t84 +
             (t237 + t88 + t93 + (t238 + t239 + t99) * t58) * t58) *
                t58 +
            (t216 + t34 + t42 + (t237 + t88 + t93 + (t246 + 2.0 * t117 + t118) * t58) * t58 +
             (t219 + t46 + t51 + (t246 + t239 + t99) * t58 + (t62 * t136 + t221 + t238 + t57) * t136) * t136) *
                t136) *
               t136;
    return (t1 + t15) * t8 + (t18 + t32 + t72) * t58 + (t18 + t32 + t114 + t148) * t136;
}

}  // namespace mbnrg_A1B2_deg6
