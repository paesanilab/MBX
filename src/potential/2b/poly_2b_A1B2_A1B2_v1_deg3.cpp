#include "poly_2b_A1B2_A1B2_v1x_deg3.h"

namespace mb_system {

double poly_model::eval(const double a[134], const double x[15])
{
    const double t1 = a[0];
    const double t2 = a[123];
    const double t4 = a[22];
    const double t9 = a[50];
    const double t3 = x[14];
    const double t10 = t3*t9;
    const double t11 = a[13];
    const double t13 = (t10+t11)*t3;
    const double t19 = a[118];
    const double t6 = x[13];
    const double t20 = t6*t19;
    const double t21 = a[91];
    const double t22 = t3*t21;
    const double t23 = a[19];
    const double t31 = t19*t3;
    const double t34 = t6*t9;
    const double t7 = x[12];
    const double t37 = t7*t9;
    const double t46 = a[1];
    const double t47 = a[26];
    const double t49 = a[16];
    const double t51 = (t3*t47+t49)*t3;
    const double t52 = t6*t47;
    const double t53 = a[126];
    const double t54 = t3*t53;
    const double t57 = a[108];
    const double t58 = t7*t57;
    const double t59 = a[73];
    const double t60 = t6*t59;
    const double t61 = a[130];
    const double t62 = t3*t61;
    const double t63 = a[12];
    const double t14 = x[11];
    const double t66 = t14*t57;
    const double t67 = a[78];
    const double t69 = t6*t61;
    const double t70 = t3*t59;
    const double t73 = a[104];
    const double t75 = a[79];
    const double t76 = t14*t75;
    const double t77 = t75*t7;
    const double t78 = a[84];
    const double t79 = t78*t6;
    const double t80 = t3*t78;
    const double t81 = a[15];
    const double t88 = (t3*t57+t63)*t3;
    const double t89 = t6*t57;
    const double t90 = t67*t3;
    const double t93 = t7*t47;
    const double t96 = t14*t47;
    const double t100 = a[122];
    const double t17 = x[10];
    const double t101 = t100*t17;
    const double t102 = a[52];
    const double t103 = t14*t102;
    const double t104 = t7*t102;
    const double t105 = t6*t102;
    const double t106 = t3*t102;
    const double t107 = a[11];
    const double t111 = t14*t78;
    const double t112 = t78*t7;
    const double t113 = t75*t6;
    const double t114 = t3*t75;
    const double t123 = t7*t61;
    const double t127 = a[82];
    const double t128 = t127*t17;
    const double t129 = a[54];
    const double t131 = a[66];
    const double t132 = t7*t131;
    const double t133 = t6*t131;
    const double t134 = a[37];
    const double t136 = a[9];
    const double t18 = x[9];
    const double t139 = t18*t127;
    const double t140 = a[99];
    const double t141 = t140*t17;
    const double t142 = t14*t131;
    const double t145 = t3*t131;
    const double t24 = x[8];
    const double t168 = t100*t24;
    const double t178 = a[59];
    const double t180 = a[10];
    const double t184 = a[109];
    const double t185 = t3*t184;
    const double t189 = a[43];
    const double t199 = a[35];
    const double t201 = a[67];
    const double t202 = t14*t201;
    const double t203 = t201*t7;
    const double t204 = a[132];
    const double t205 = t204*t6;
    const double t206 = t3*t204;
    const double t207 = a[4];
    const double t211 = a[103];
    const double t213 = t204*t14;
    const double t214 = t204*t7;
    const double t215 = t201*t6;
    const double t216 = t3*t201;
    const double t220 = a[24];
    const double t221 = t220*t18;
    const double t222 = t220*t17;
    const double t231 = a[87];
    const double t236 = a[89];
    const double t246 = a[113];
    const double t248 = a[3];
    const double t250 = (t246*t3+t248)*t3;
    const double t251 = t246*t6;
    const double t252 = a[97];
    const double t253 = t252*t3;
    const double t256 = t246*t7;
    const double t257 = a[34];
    const double t258 = t257*t6;
    const double t259 = a[83];
    const double t260 = t259*t3;
    const double t263 = t246*t14;
    const double t266 = t257*t3;
    const double t269 = a[57];
    const double t271 = a[49];
    const double t272 = t271*t14;
    const double t273 = t271*t7;
    const double t274 = a[29];
    const double t275 = t274*t6;
    const double t276 = t274*t3;
    const double t277 = a[7];
    const double t281 = a[39];
    const double t283 = t274*t14;
    const double t284 = t274*t7;
    const double t285 = t271*t6;
    const double t286 = t271*t3;
    const double t289 = a[48];
    const double t291 = a[75];
    const double t292 = t291*t18;
    const double t293 = t291*t17;
    const double t294 = a[44];
    const double t295 = t294*t14;
    const double t296 = a[46];
    const double t297 = t296*t7;
    const double t298 = t294*t6;
    const double t299 = t296*t3;
    const double t300 = a[18];
    const double t304 = a[30];
    const double t306 = t296*t14;
    const double t307 = t294*t7;
    const double t308 = t296*t6;
    const double t309 = t294*t3;
    const double t28 = x[6];
    const double t313 = a[64]*t28;
    const double t314 = a[45];
    const double t317 = a[98];
    const double t320 = a[92];
    const double t321 = t320*t14;
    const double t322 = t320*t7;
    const double t323 = t320*t6;
    const double t324 = t320*t3;
    const double t325 = a[8];
    const double t328 = a[41];
    const double t331 = t7+t3+t14+t6;
    const double t332 = a[100]*t331;
    const double t334 = a[116];
    const double t338 = a[121]*t28;
    const double t343 = a[28];
    const double t345 = a[20];
    const double t347 = (t3*t343+t345)*t3;
    const double t348 = a[27];
    const double t349 = t348*t6;
    const double t350 = a[65];
    const double t351 = t350*t3;
    const double t352 = a[6];
    const double t355 = t343*t7;
    const double t356 = a[95];
    const double t357 = t356*t6;
    const double t358 = a[32];
    const double t359 = t358*t3;
    const double t362 = t348*t14;
    const double t363 = t350*t7;
    const double t364 = a[58];
    const double t366 = t356*t3;
    const double t369 = a[36];
    const double t370 = t369*t17;
    const double t371 = a[61];
    const double t372 = t371*t14;
    const double t373 = a[117];
    const double t374 = t373*t7;
    const double t375 = a[77];
    const double t376 = t375*t6;
    const double t377 = a[80];
    const double t378 = t377*t3;
    const double t379 = a[5];
    const double t382 = t369*t18;
    const double t383 = a[128];
    const double t384 = t383*t17;
    const double t385 = t375*t14;
    const double t386 = t377*t7;
    const double t387 = t371*t6;
    const double t388 = t373*t3;
    const double t391 = a[129];
    const double t393 = a[47];
    const double t394 = t393*t18;
    const double t395 = t393*t17;
    const double t396 = a[53];
    const double t397 = t396*t14;
    const double t398 = a[63];
    const double t399 = t398*t7;
    const double t400 = t396*t6;
    const double t401 = t398*t3;
    const double t402 = a[17];
    const double t405 = a[115];
    const double t407 = a[56];
    const double t408 = t407*t24;
    const double t409 = a[42];
    const double t410 = t409*t18;
    const double t411 = t409*t17;
    const double t412 = a[38];
    const double t413 = t412*t14;
    const double t414 = a[70];
    const double t415 = t414*t7;
    const double t416 = t412*t6;
    const double t417 = t414*t3;
    const double t418 = a[21];
    const double t422 = a[114]*t28;
    const double t423 = a[31];
    const double t425 = a[72];
    const double t427 = a[88];
    const double t428 = t427*t18;
    const double t429 = t427*t17;
    const double t430 = a[94];
    const double t431 = t430*t14;
    const double t432 = a[125];
    const double t433 = t432*t7;
    const double t434 = t430*t6;
    const double t435 = t432*t3;
    const double t436 = a[14];
    const double t440 = a[124]*t28;
    const double t441 = a[131];
    const double t443 = a[85];
    const double t445 = a[101];
    const double t446 = t445*t18;
    const double t447 = t445*t17;
    const double t448 = a[62];
    const double t449 = t448*t14;
    const double t450 = a[71];
    const double t451 = t450*t7;
    const double t457 = a[33]*t28;
    const double t458 = a[68];
    const double t460 = a[119];
    const double t462 = a[102];
    const double t463 = t462*t18;
    const double t464 = t462*t17;
    const double t465 = a[25];
    const double t466 = t465*t14;
    const double t467 = a[105];
    const double t468 = t467*t7;
    const double t71 = x[7];
    const double t97 = x[5];
    const double t116 = x[4];
    const double t473 = t347+(t349+t351+t352)*t6+(t355+t357+t359+t345)*t7+(t364*t6+t352+t362
+t363+t366)*t14+(t370+t372+t374+t376+t378+t379)*t17+(t382+t384+t385+t386+t387+
t388+t379)*t18+(t24*t391+t394+t395+t397+t399+t400+t401+t402)*t24+(t405*t71+t408
+t410+t411+t413+t415+t416+t417+t418)*t71+(t24*t425+t423*t71+t422+t428+t429+t431
+t433+t434+t435+t436)*t28+(t24*t443+t3*t450+t441*t71+t448*t6+t440+t446+t447+
t449+t451)*t97+(t24*t460+t3*t467+t458*t71+t465*t6+t457+t463+t464+t466+t468)*
t116;
    const double t477 = (t3*t348+t352)*t3;
    const double t478 = t343*t6;
    const double t481 = t348*t7;
    const double t482 = t364*t3;
    const double t485 = t343*t14;
    const double t489 = t373*t14;
    const double t490 = t371*t7;
    const double t491 = t377*t6;
    const double t492 = t375*t3;
    const double t495 = t377*t14;
    const double t496 = t375*t7;
    const double t497 = t373*t6;
    const double t498 = t371*t3;
    const double t502 = t414*t14;
    const double t503 = t412*t7;
    const double t504 = t414*t6;
    const double t505 = t412*t3;
    const double t509 = t398*t14;
    const double t510 = t396*t7;
    const double t511 = t398*t6;
    const double t512 = t396*t3;
    const double t517 = t432*t14;
    const double t518 = t430*t7;
    const double t519 = t432*t6;
    const double t520 = t430*t3;
    const double t525 = t450*t14;
    const double t526 = t448*t7;
    const double t531 = a[69];
    const double t534 = a[90]*t331;
    const double t536 = a[127];
    const double t540 = a[106]*t28;
    const double t545 = t467*t14;
    const double t546 = t465*t7;
    const double t175 = x[3];
    const double t551 = t477+(t478+t351+t345)*t6+(t481+t357+t482+t352)*t7+(t358*t6+t345+t363
+t366+t485)*t14+(t370+t489+t490+t491+t492+t379)*t17+(t382+t384+t495+t496+t497+
t498+t379)*t18+(t24*t405+t410+t411+t418+t502+t503+t504+t505)*t24+(t391*t71+t394
+t395+t402+t408+t509+t510+t511+t512)*t71+(t24*t423+t425*t71+t422+t428+t429+t436
+t517+t518+t519+t520)*t28+(t24*t441+t3*t448+t443*t71+t450*t6+t440+t446+t447+
t525+t526)*t97+(t17*t531+t18*t531+t24*t536+t536*t71+t534+t540)*t116+(t24*t458+
t3*t465+t460*t71+t467*t6+t457+t463+t464+t545+t546)*t175;
    const double t581 = a[112];
    const double t593 = a[55]*t28;
    const double t594 = a[110];
    const double t596 = a[107];
    const double t598 = a[93];
    const double t599 = t598*t18;
    const double t600 = t598*t17;
    const double t601 = a[86];
    const double t602 = t601*t14;
    const double t603 = a[40];
    const double t604 = t603*t7;
    const double t611 = t603*t14;
    const double t612 = t601*t7;
    const double t270 = x[2];
    const double t623 = t250+(t251+t260+t248)*t6+(t256+t258+t253+t248)*t7+(t252*t6+t259*t7+
t248+t263+t266)*t14+(t17*t289+t295+t299+t300+t307+t308)*t17+(t17*t304+t18*t289+
t297+t298+t300+t306+t309)*t18+(t24*t269+t272+t276+t277+t284+t285+t292+t293)*t24
+(t24*t281+t269*t71+t273+t275+t277+t283+t286+t292+t293)*t71+(t17*t314+t18*t314+
t24*t317+t317*t71+t313+t321+t322+t323+t324+t325)*t28+(t17*t581+t18*t581+t24*
t581+t28*a[60]+a[120]*t331+t581*t71)*t97+(t24*t596+t3*t603+t594*t71+t6*t601+
t593+t599+t600+t602+t604)*t116+(t24*t594+t3*t601+t596*t71+t6*t603+t593+t599+
t600+t611+t612)*t175+(t17*t334+t18*t334+t24*t328+t328*t71+t332+t338)*t270;
    const double t630 = t350*t6;
    const double t637 = t407*t17;
    const double t640 = t369*t24;
    const double t643 = t369*t71;
    const double t644 = t383*t24;
    const double t647 = t427*t71;
    const double t648 = t427*t24;
    const double t653 = t6+t3;
    const double t657 = t598*t24;
    const double t658 = t598*t71;
    const double t662 = a[111]*t28;
    const double t663 = a[74];
    const double t664 = t663*t71;
    const double t665 = a[51];
    const double t666 = t665*t24;
    const double t667 = t663*t18;
    const double t668 = t665*t17;
    const double t669 = a[81];
    const double t671 = a[96];
    const double t672 = t671*t7;
    const double t673 = t671*t6;
    const double t674 = a[76];
    const double t678 = t665*t71;
    const double t679 = t663*t24;
    const double t680 = t671*t14;
    const double t683 = t671*t3;
    const double t689 = t445*t24;
    const double t690 = t445*t71;
    const double t696 = t462*t24;
    const double t697 = t462*t71;
    const double t404 = x[1];
    const double t700 = t347+(t478+t359+t345)*t6+(t481+t357+t351+t352)*t7+(t364*t7+t352+t362
+t366+t630)*t14+(t17*t391+t397+t401+t402+t510+t511)*t17+(t18*t405+t413+t417+
t418+t503+t504+t637)*t18+(t640+t410+t395+t372+t496+t497+t378+t379)*t24+(t643+
t644+t410+t395+t385+t490+t491+t388+t379)*t71+(t17*t425+t18*t423+t422+t431+t435+
t436+t518+t519+t647+t648)*t28+(t17*t596+t18*t594+t603*t653+t593+t602+t612+t657+
t658)*t97+(t14*t669+t3*t674+t662+t664+t666+t667+t668+t672+t673)*t116+(t6*t674+
t669*t7+t662+t667+t668+t678+t679+t680+t683)*t175+(t17*t443+t18*t441+t450*t653+
t440+t449+t526+t689+t690)*t270+(t17*t460+t18*t458+t467*t653+t457+t466+t546+t696
+t697)*t404;
    const double t728 = t665*t18;
    const double t729 = t663*t17;
    const double t529 = x[0];
    const double t754 = t477+(t349+t482+t352)*t6+(t355+t357+t351+t345)*t7+(t358*t7+t345+t366
+t485+t630)*t14+(t17*t405+t415+t416+t418+t502+t505)*t17+(t18*t391+t399+t400+
t402+t509+t512+t637)*t18+(t640+t394+t411+t489+t386+t387+t492+t379)*t24+(t643+
t644+t394+t411+t495+t374+t376+t498+t379)*t71+(t17*t423+t18*t425+t422+t433+t434+
t436+t517+t520+t647+t648)*t28+(t17*t594+t18*t596+t601*t653+t593+t604+t611+t657+
t658)*t97+(t6*t669+t674*t7+t662+t664+t666+t680+t683+t728+t729)*t116+(t14*t674+
t3*t669+t662+t672+t673+t678+t679+t728+t729)*t175+(t17*t441+t18*t443+t448*t653+
t440+t451+t525+t689+t690)*t270+(t17*t536+t18*t536+t24*t531+t531*t71+t534+t540)*
t404+(t17*t458+t18*t460+t465*t653+t457+t468+t545+t696+t697)*t529;
    return((t1+(t2*t3+t4)*t3)*t3+(t1+t13+(t2*t6+t10+t4)*t6)*t6+(t1+t13+(t20+t22+t23)*t6+(t2*t7+t10+t20+t4)*t7)*t7+(t1+(t31+t23)*t3+(t34+t22+t11)*t6+(t21*t6+
t11+t22+t37)*t7+(t14*t2+t31+t34+t37+t4)*t14)*t14+(t46+t51+(t52+t54+t49)*t6+(t58
+t60+t62+t63)*t7+(t67*t7+t63+t66+t69+t70)*t14+(t17*t73+t76+t77+t79+t80+t81)*t17
)*t17+(t46+t88+(t89+t90+t63)*t6+(t93+t60+t62+t49)*t7+(t53*t7+t49+t69+t70+t96)*
t14+(t101+t103+t104+t105+t106+t107)*t17+(t18*t73+t101+t111+t112+t113+t114+t81)*
t18)*t18+(t46+t51+(t89+t62+t63)*t6+(t93+t60+t54+t49)*t7+(t6*t67+t123+t63+t66+
t70)*t14+(t129*t14+t134*t3+t128+t132+t133+t136)*t17+(t129*t6+t134*t7+t136+t139+
t141+t142+t145)*t18+(t24*t73+t112+t113+t128+t139+t76+t80+t81)*t24)*t24+(t46+t88
+(t52+t62+t49)*t6+(t58+t60+t90+t63)*t7+(t53*t6+t123+t49+t70+t96)*t14+(t129*t7+
t134*t6+t128+t136+t142+t145)*t17+(t129*t3+t134*t14+t132+t133+t136+t139+t141)*
t18+(t140*t18+t103+t104+t105+t106+t107+t141+t168)*t24+(t71*t73+t111+t114+t128+
t139+t168+t77+t79+t81)*t71)*t71+(a[2]+(t178*t3+t180)*t3+(t178*t6+t180+t185)*t6+
(t178*t7+t189*t6+t180+t185)*t7+(t14*t178+t184*t6+t184*t7+t189*t3+t180)*t14+(t17
*t199+t202+t203+t205+t206+t207)*t17+(t17*t211+t18*t199+t207+t213+t214+t215+t216
)*t18+(t199*t24+t202+t206+t207+t214+t215+t221+t222)*t24+(t199*t71+t211*t24+t203
+t205+t207+t213+t216+t221+t222)*t71+(t14*t236+t17*t231+t18*t231+t231*t24+t231*
t71+t236*t3+t236*t6+t236*t7+t28*a[133]+a[23])*t28)*t28+(t250+(t251+t253+t248)*
t6+(t256+t258+t260+t248)*t7+(t252*t7+t259*t6+t248+t263+t266)*t14+(t17*t269+t272
+t273+t275+t276+t277)*t17+(t17*t281+t18*t269+t277+t283+t284+t285+t286)*t18+(t24
*t289+t292+t293+t295+t297+t298+t299+t300)*t24+(t24*t304+t289*t71+t292+t293+t300
+t306+t307+t308+t309)*t71+(t17*t317+t18*t317+t24*t314+t314*t71+t313+t321+t322+
t323+t324+t325)*t28+(t17*t328+t18*t328+t24*t334+t334*t71+t332+t338)*t97)*t97+
t473*t116+t551*t175+t623*t270+t700*t404+t754*t529);

}

} // namespace mb_system
