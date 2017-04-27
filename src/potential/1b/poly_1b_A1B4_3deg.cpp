#include "poly_1b_A1B4_deg3.h"

namespace A1B4 {

double poly_1b_v1x::eval(const double a[29], const double x[10],
                        double g[10])
{
    const double t1 = a[0];
    const double t2 = a[26];
    const double t5 = x[9];
    const double t3 = t5*t2;
    const double t4 = a[7];
    const double t6 = (t3+t4)*t5;
    const double t9 = a[20];
    const double t10 = t5*t9;
    const double t11 = a[2];
    const double t13 = (t10+t11)*t5;
    const double t12 = x[8];
    const double t14 = t12*t2;
    const double t16 = (t14+t10+t4)*t12;
    const double t19 = t12*t9;
    const double t20 = a[11];
    const double t21 = t20*t5;
    const double t23 = (t19+t21+t11)*t12;
    const double t18 = x[7];
    const double t24 = t18*t2;
    const double t26 = (t24+t19+t10+t4)*t18;
    const double t29 = a[21];
    const double t30 = t5*t29;
    const double t32 = (t19+t30+t11)*t12;
    const double t33 = a[27];
    const double t34 = t33*t18;
    const double t35 = a[13];
    const double t36 = t12*t35;
    const double t37 = t5*t35;
    const double t38 = a[5];
    const double t40 = (t34+t36+t37+t38)*t18;
    const double t28 = x[6];
    const double t41 = t28*t2;
    const double t43 = (t41+t34+t19+t10+t4)*t28;
    const double t46 = t12*t33;
    const double t48 = (t46+t37+t38)*t12;
    const double t49 = t18*t9;
    const double t51 = (t49+t36+t30+t11)*t18;
    const double t52 = t28*t9;
    const double t53 = t18*t35;
    const double t55 = (t52+t53+t36+t21+t11)*t28;
    const double t45 = x[5];
    const double t56 = t45*t2;
    const double t58 = (t56+t52+t49+t46+t10+t4)*t45;
    const double t61 = t33*t5;
    const double t63 = (t61+t38)*t5;
    const double t65 = (t19+t37+t11)*t12;
    const double t66 = t12*t29;
    const double t68 = (t49+t66+t37+t11)*t18;
    const double t69 = t20*t12;
    const double t71 = (t52+t53+t69+t37+t11)*t28;
    const double t72 = t45*t9;
    const double t73 = t29*t28;
    const double t74 = t20*t18;
    const double t76 = (t72+t73+t74+t36+t37+t11)*t45;
    const double t62 = x[4];
    const double t77 = t62*t2;
    const double t79 = (t77+t72+t52+t49+t19+t61+t4)*t62;
    const double t82 = a[1];
    const double t83 = a[24];
    const double t84 = t5*t83;
    const double t85 = a[4];
    const double t87 = (t84+t85)*t5;
    const double t88 = t12*t83;
    const double t89 = a[18];
    const double t90 = t5*t89;
    const double t92 = (t88+t90+t85)*t12;
    const double t93 = a[14];
    const double t94 = t18*t93;
    const double t95 = a[12];
    const double t96 = t95*t12;
    const double t97 = t5*t95;
    const double t98 = a[3];
    const double t100 = (t94+t96+t97+t98)*t18;
    const double t101 = t28*t83;
    const double t102 = a[17];
    const double t103 = t102*t18;
    const double t104 = t12*t89;
    const double t106 = (t101+t103+t104+t90+t85)*t28;
    const double t107 = t45*t93;
    const double t108 = t95*t28;
    const double t109 = a[9];
    const double t110 = t109*t18;
    const double t111 = t102*t12;
    const double t113 = (t107+t108+t110+t111+t97+t98)*t45;
    const double t114 = t62*t93;
    const double t115 = t45*t109;
    const double t116 = t102*t5;
    const double t118 = (t114+t115+t108+t110+t96+t116+t98)*t62;
    const double t119 = a[28];
    const double t86 = x[3];
    const double t120 = t86*t119;
    const double t121 = a[22];
    const double t122 = t62*t121;
    const double t123 = t45*t121;
    const double t124 = a[16];
    const double t125 = t124*t28;
    const double t126 = t121*t18;
    const double t127 = t12*t124;
    const double t128 = t5*t124;
    const double t129 = a[6];
    const double t131 = (t120+t122+t123+t125+t126+t127+t128+t129)*t86;
    const double t134 = t12*t93;
    const double t136 = (t134+t97+t98)*t12;
    const double t137 = t18*t83;
    const double t139 = (t137+t96+t90+t85)*t18;
    const double t140 = t28*t93;
    const double t141 = t12*t109;
    const double t143 = (t140+t103+t141+t97+t98)*t28;
    const double t144 = t45*t83;
    const double t145 = t18*t89;
    const double t147 = (t144+t108+t145+t111+t90+t85)*t45;
    const double t148 = t45*t95;
    const double t149 = t28*t109;
    const double t150 = t95*t18;
    const double t152 = (t114+t148+t149+t150+t141+t116+t98)*t62;
    const double t153 = a[19];
    const double t154 = t153*t86;
    const double t155 = a[15];
    const double t156 = t155*t62;
    const double t157 = a[23];
    const double t158 = t45*t157;
    const double t159 = t28*t157;
    const double t160 = t18*t157;
    const double t161 = t12*t157;
    const double t162 = a[10];
    const double t163 = t162*t5;
    const double t164 = a[8];
    const double t166 = (t154+t156+t158+t159+t160+t161+t163+t164)*t86;
    const double t133 = x[2];
    const double t167 = t133*t119;
    const double t168 = t45*t124;
    const double t169 = t28*t121;
    const double t170 = t18*t124;
    const double t171 = t121*t12;
    const double t173 = (t167+t154+t122+t168+t169+t170+t171+t128+t129)*t133;
    const double t176 = t5*t93;
    const double t178 = (t176+t98)*t5;
    const double t180 = (t88+t97+t85)*t12;
    const double t182 = (t137+t104+t97+t85)*t18;
    const double t183 = t5*t109;
    const double t185 = (t140+t103+t96+t183+t98)*t28;
    const double t187 = (t107+t149+t150+t111+t183+t98)*t45;
    const double t188 = t83*t62;
    const double t190 = (t188+t148+t108+t145+t104+t116+t85)*t62;
    const double t191 = t62*t157;
    const double t192 = t155*t45;
    const double t193 = t162*t12;
    const double t194 = t5*t157;
    const double t196 = (t154+t191+t192+t159+t160+t193+t194+t164)*t86;
    const double t197 = t133*t153;
    const double t198 = a[25];
    const double t199 = t198*t86;
    const double t200 = t155*t28;
    const double t201 = t162*t18;
    const double t203 = (t197+t199+t191+t158+t200+t201+t161+t194+t164)*t133;
    const double t177 = x[1];
    const double t204 = t177*t119;
    const double t205 = t62*t124;
    const double t206 = t121*t5;
    const double t208 = (t204+t197+t154+t205+t123+t169+t170+t127+t206+t129)*t177;
    const double t212 = (t134+t183+t98)*t12;
    const double t214 = (t94+t141+t183+t98)*t18;
    const double t216 = (t101+t103+t96+t97+t85)*t28;
    const double t217 = t89*t28;
    const double t219 = (t144+t217+t150+t111+t97+t85)*t45;
    const double t220 = t89*t45;
    const double t222 = (t188+t220+t217+t150+t96+t116+t85)*t62;
    const double t223 = t162*t28;
    const double t224 = t155*t18;
    const double t226 = (t154+t191+t158+t223+t224+t161+t194+t164)*t86;
    const double t227 = t162*t45;
    const double t228 = t155*t12;
    const double t230 = (t197+t199+t191+t227+t159+t160+t228+t194+t164)*t133;
    const double t231 = t177*t153;
    const double t232 = t133*t198;
    const double t233 = t162*t62;
    const double t234 = t155*t5;
    const double t236 = (t231+t232+t199+t233+t158+t159+t160+t161+t234+t164)*t177;
    const double t210 = x[0];
    const double t237 = t210*t119;
    const double t238 = t237+t231+t197+t154+t205+t168+t125+t126+t171+t206+t129;
    const double t239 = t238*t210;
    const double t240 = t82+t178+t212+t214+t216+t219+t222+t226+t230+t236+t239;
    const double t244 = 2.0*t237+t231+t197+t154+t205+t168+t125+t126+t171+t206+t129;
    const double t246 = t244*t210+t178+t212+t214+t216+t219+t222+t226+t230+t236+t239+t82;
    const double t250 = t153*t210;
    const double t252 = t250+2.0*t231+t232+t199+t233+t158+t159+t160+t161+t234+t164;
    const double t254 = (2.0*t204+t197+t154+t205+t123+t169+t170+t127+t206+t129)*t177+t82+
t178+t180+t182+t185+t187+t190+t196+t203+t208+t252*t210;
    const double t258 = 2.0*t197;
    const double t261 = t198*t177;
    const double t262 = t250+t261+t258+t199+t191+t227+t159+t160+t228+t194+t164;
    const double t264 = (2.0*t167+t154+t122+t168+t169+t170+t171+t128+t129)*t133+t82+t87+t136
+t139+t143+t147+t152+t166+t173+(t231+t258+t199+t191+t158+t200+t201+t161+t194+
t164)*t177+t262*t210;
    const double t268 = 2.0*t154;
    const double t273 = t250+t261+t232+t268+t191+t158+t223+t224+t161+t194+t164;
    const double t275 = (2.0*t120+t122+t123+t125+t126+t127+t128+t129)*t86+t82+t87+t92+t100+
t106+t113+t118+t131+(t197+t268+t156+t158+t159+t160+t161+t163+t164)*t133+(t231+
t232+t268+t191+t192+t159+t160+t193+t194+t164)*t177+t273*t210;
    const double t279 = t121*t86;
    const double t280 = 2.0*t114;
    const double t283 = t121*t133;
    const double t284 = t155*t86;
    const double t287 = t124*t177;
    const double t288 = t157*t133;
    const double t289 = t157*t86;
    const double t290 = 2.0*t188;
    const double t293 = t124*t210;
    const double t295 = t162*t177+t116+t150+t217+t220+t288+t289+t290+t293+t85+t96;
    const double t297 = (2.0*t77+t72+t52+t49+t19+t61+t4)*t62+t1+t63+t65+t68+t71+t76+t79+(
t279+t280+t115+t108+t110+t96+t116+t98)*t86+(t283+t284+t280+t148+t149+t150+t141+
t116+t98)*t133+(t287+t288+t289+t290+t148+t108+t145+t104+t116+t85)*t177+t295*
t210;
    const double t301 = t9*t62;
    const double t305 = t109*t62;
    const double t306 = 2.0*t107;
    const double t309 = t124*t133;
    const double t310 = t95*t62;
    const double t311 = 2.0*t144;
    const double t314 = t121*t177;
    const double t317 = t157*t177;
    const double t318 = t162*t133;
    const double t319 = t89*t62;
    const double t320 = t293+t317+t318+t289+t319+t311+t217+t150+t111+t97+t85;
    const double t322 = (2.0*t56+t52+t49+t46+t10+t4)*t45+t1+t13+t48+t51+t55+t58+(t301+2.0*
t72+t73+t74+t36+t37+t11)*t62+(t279+t305+t306+t108+t110+t111+t97+t98)*t86+(t309+
t289+t310+t311+t108+t145+t111+t90+t85)*t133+(t314+t288+t284+t310+t306+t149+t150
+t111+t183+t98)*t177+t320*t210;
    const double t326 = 2.0*t52;
    const double t332 = t124*t86;
    const double t333 = 2.0*t101;
    const double t336 = 2.0*t140;
    const double t339 = t155*t133;
    const double t342 = t162*t86;
    const double t343 = t293+t317+t288+t342+t319+t220+t333+t103+t96+t97+t85;
    const double t345 = (2.0*t41+t34+t19+t10+t4)*t28+t1+t13+t32+t40+t43+(t72+t326+t53+t36+
t21+t11)*t45+(t29*t45+t11+t301+t326+t37+t53+t69)*t62+(t332+t310+t148+t333+t103+
t104+t90+t85)*t86+(t283+t289+t305+t148+t336+t103+t141+t97+t98)*t133+(t314+t339+
t289+t310+t115+t336+t103+t96+t183+t98)*t177+t343*t210;
    const double t353 = t35*t28;
    const double t354 = 2.0*t49;
    const double t360 = t102*t28;
    const double t361 = 2.0*t94;
    const double t364 = 2.0*t137;
    const double t369 = t121*t210;
    const double t370 = t369+t317+t288+t284+t310+t148+t360+t361+t141+t183+t98;
    const double t372 = (2.0*t24+t19+t10+t4)*t18+t1+t13+t23+t26+(t33*t28+2.0*t34+t36+t37+t38
)*t28+(t72+t353+t354+t36+t30+t11)*t45+(t20*t45+t11+t301+t353+t354+t37+t66)*t62+
(t279+t305+t115+t360+t361+t96+t97+t98)*t86+(t309+t289+t310+t220+t360+t364+t96+
t90+t85)*t133+(t287+t318+t289+t319+t148+t360+t364+t104+t97+t85)*t177+t370*t210;
    const double t376 = 2.0*t19;
    const double t385 = t35*t45;
    const double t386 = t20*t28;
    const double t387 = t29*t18;
    const double t390 = t102*t45;
    const double t391 = 2.0*t88;
    const double t394 = 2.0*t134;
    const double t399 = t369+t317+t339+t289+t310+t390+t108+t110+t394+t183+t98;
    const double t401 = (2.0*t14+t10+t4)*t12+t1+t13+t16+(t49+t376+t21+t11)*t18+(t52+t53+t376
+t30+t11)*t28+(t33*t45+t353+t37+t38+2.0*t46+t53)*t45+(t301+t385+t386+t387+t376+
t37+t11)*t62+(t332+t310+t390+t217+t150+t391+t90+t85)*t86+(t283+t289+t305+t390+
t149+t150+t394+t97+t98)*t133+(t287+t288+t342+t319+t390+t108+t145+t391+t97+t85)*
t177+t399*t210;
    const double t405 = 2.0*t10;
    const double t418 = t102*t62;
    const double t419 = 2.0*t84;
    const double t424 = 2.0*t176;
    const double t428 = t155*t177+t108+t110+t141+t148+t288+t289+t369+t418+t424+t98;
    const double t430 = (2.0*t3+t4)*t5+t1+t6+(t19+t405+t11)*t12+(t49+t69+t405+t11)*t18+(t52+
t53+t66+t405+t11)*t28+(t72+t386+t387+t36+t405+t11)*t45+(t33*t62+t353+t36+t38+
t385+t53+2.0*t61)*t62+(t332+t418+t148+t217+t150+t104+t419+t85)*t86+(t309+t342+
t418+t220+t108+t145+t96+t419+t85)*t133+(t314+t288+t289+t418+t115+t149+t150+t96+
t424+t98)*t177+t428*t210;
    g[0] = t246;
    g[1] = t254;
    g[2] = t264;
    g[3] = t275;
    g[4] = t297;
    g[5] = t322;
    g[6] = t345;
    g[7] = t372;
    g[8] = t401;
    g[9] = t430;
    return (t1+t6)*t5+(t1+t13+t16)*t12+(t1+t13+t23+t26)*t18+(t1+t13+t32+
t40+t43)*t28+(t1+t13+t48+t51+t55+t58)*t45+(t1+t63+t65+t68+t71+t76+t79)*t62+(t82
+t87+t92+t100+t106+t113+t118+t131)*t86+(t82+t87+t136+t139+t143+t147+t152+t166+
t173)*t133+(t82+t178+t180+t182+t185+t187+t190+t196+t203+t208)*t177+t240*t210;

}

} // namespace A1B4
