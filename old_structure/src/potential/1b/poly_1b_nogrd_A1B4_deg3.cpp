#include "poly_1b_A1B4_deg3.h"

namespace A1B4 {

double poly_1b_v1x::eval(const double a[29], const double x[10])
{
    const double t1 = a[0];
    const double t2 = a[26];
    const double t4 = a[7];
    const double t9 = a[20];
    const double t3 = x[9];
    const double t10 = t3*t9;
    const double t11 = a[2];
    const double t13 = (t10+t11)*t3;
    const double t6 = x[8];
    const double t19 = t6*t9;
    const double t20 = a[11];
    const double t21 = t20*t3;
    const double t29 = a[21];
    const double t30 = t3*t29;
    const double t33 = a[27];
    const double t7 = x[7];
    const double t34 = t7*t33;
    const double t35 = a[13];
    const double t36 = t6*t35;
    const double t37 = t3*t35;
    const double t38 = a[5];
    const double t46 = t33*t6;
    const double t49 = t7*t9;
    const double t8 = x[6];
    const double t52 = t8*t9;
    const double t53 = t7*t35;
    const double t61 = t33*t3;
    const double t12 = x[5];
    const double t72 = t12*t9;
    const double t82 = a[1];
    const double t83 = a[24];
    const double t85 = a[4];
    const double t87 = (t83*t3+t85)*t3;
    const double t88 = t6*t83;
    const double t89 = a[18];
    const double t90 = t3*t89;
    const double t93 = a[14];
    const double t94 = t7*t93;
    const double t95 = a[12];
    const double t96 = t6*t95;
    const double t97 = t3*t95;
    const double t98 = a[3];
    const double t101 = t8*t83;
    const double t102 = a[17];
    const double t103 = t102*t7;
    const double t104 = t6*t89;
    const double t107 = t12*t93;
    const double t108 = t95*t8;
    const double t109 = a[9];
    const double t110 = t109*t7;
    const double t111 = t102*t6;
    const double t16 = x[4];
    const double t114 = t16*t93;
    const double t116 = t102*t3;
    const double t119 = a[28];
    const double t121 = a[22];
    const double t122 = t16*t121;
    const double t123 = t12*t121;
    const double t124 = a[16];
    const double t125 = t124*t8;
    const double t126 = t121*t7;
    const double t127 = t6*t124;
    const double t128 = t3*t124;
    const double t129 = a[6];
    const double t134 = t6*t93;
    const double t137 = t7*t83;
    const double t140 = t8*t93;
    const double t141 = t6*t109;
    const double t144 = t12*t83;
    const double t145 = t7*t89;
    const double t148 = t12*t95;
    const double t149 = t8*t109;
    const double t150 = t95*t7;
    const double t153 = a[19];
    const double t17 = x[3];
    const double t154 = t153*t17;
    const double t155 = a[15];
    const double t157 = a[23];
    const double t158 = t12*t157;
    const double t159 = t8*t157;
    const double t160 = t7*t157;
    const double t161 = t6*t157;
    const double t162 = a[10];
    const double t164 = a[8];
    const double t168 = t12*t124;
    const double t169 = t8*t121;
    const double t170 = t7*t124;
    const double t171 = t121*t6;
    const double t178 = (t93*t3+t98)*t3;
    const double t183 = t3*t109;
    const double t188 = t16*t83;
    const double t191 = t16*t157;
    const double t194 = t3*t157;
    const double t23 = x[2];
    const double t197 = t23*t153;
    const double t198 = a[25];
    const double t199 = t198*t17;
    const double t205 = t16*t124;
    const double t206 = t121*t3;
    const double t217 = t89*t8;
    const double t24 = x[1];
    const double t231 = t24*t153;
    const double t25 = x[0];
    const double t238 = t119*t25+t125+t126+t129+t154+t168+t171+t197+t205+t206+t231;
    const double t240 = t82+t178+(t134+t183+t98)*t6+(t94+t141+t183+t98)*t7+(t101+t103+t96+
t97+t85)*t8+(t144+t217+t150+t111+t97+t85)*t12+(t89*t12+t116+t150+t188+t217+t85+
t96)*t16+(t155*t7+t162*t8+t154+t158+t161+t164+t191+t194)*t17+(t162*t12+t155*t6+
t159+t160+t164+t191+t194+t197+t199)*t23+(t155*t3+t162*t16+t198*t23+t158+t159+
t160+t161+t164+t199+t231)*t24+t238*t25;
    return((t1+(t2*t3+t4)*t3)*t3+(t1+t13+(t2*t6+t10+t4)*t6)*t6+(t1+t13+(t19+t21+t11)*t6+(t2*t7+t10+t19+t4)*t7)*t7+(t1+t13+(t19+t30+t11)*t6+(t34+t36+t37+t38)*
t7+(t2*t8+t10+t19+t34+t4)*t8)*t8+(t1+t13+(t46+t37+t38)*t6+(t49+t36+t30+t11)*t7+
(t52+t53+t36+t21+t11)*t8+(t2*t12+t10+t4+t46+t49+t52)*t12)*t12+(t1+(t61+t38)*t3+
(t19+t37+t11)*t6+(t29*t6+t11+t37+t49)*t7+(t20*t6+t11+t37+t52+t53)*t8+(t20*t7+
t29*t8+t11+t36+t37+t72)*t12+(t2*t16+t19+t4+t49+t52+t61+t72)*t16)*t16+(t82+t87+(
t88+t90+t85)*t6+(t94+t96+t97+t98)*t7+(t101+t103+t104+t90+t85)*t8+(t107+t108+
t110+t111+t97+t98)*t12+(t109*t12+t108+t110+t114+t116+t96+t98)*t16+(t119*t17+
t122+t123+t125+t126+t127+t128+t129)*t17)*t17+(t82+t87+(t134+t97+t98)*t6+(t137+
t96+t90+t85)*t7+(t140+t103+t141+t97+t98)*t8+(t144+t108+t145+t111+t90+t85)*t12+(
t114+t148+t149+t150+t141+t116+t98)*t16+(t155*t16+t162*t3+t154+t158+t159+t160+
t161+t164)*t17+(t119*t23+t122+t128+t129+t154+t168+t169+t170+t171)*t23)*t23+(t82
+t178+(t88+t97+t85)*t6+(t137+t104+t97+t85)*t7+(t140+t103+t96+t183+t98)*t8+(t107
+t149+t150+t111+t183+t98)*t12+(t188+t148+t108+t145+t104+t116+t85)*t16+(t155*t12
+t162*t6+t154+t159+t160+t164+t191+t194)*t17+(t155*t8+t162*t7+t158+t161+t164+
t191+t194+t197+t199)*t23+(t119*t24+t123+t127+t129+t154+t169+t170+t197+t205+t206
)*t24)*t24+t240*t25);

}

} // namespace A1B4
