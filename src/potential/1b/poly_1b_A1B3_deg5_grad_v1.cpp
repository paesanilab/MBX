#include "poly_1b_A1B3_deg5_v1.h"

namespace mbnrg_A1B3_deg5 {

double poly_A1B3_deg5_v1::eval(const double x[6], const double a[102], double g[6]) {
    const double t1 = a[1];
    const double t2 = a[3];
    const double t3 = a[18];
    const double t4 = a[100];
    const double t7 = x[5];
    const double t5 = t7 * t4;
    const double t6 = a[43];
    const double t8 = (t5 + t6) * t7;
    const double t10 = (t3 + t8) * t7;
    const double t12 = (t2 + t10) * t7;
    const double t15 = a[5];
    const double t16 = a[11];
    const double t17 = a[84];
    const double t18 = t7 * t17;
    const double t19 = a[25];
    const double t21 = (t18 + t19) * t7;
    const double t23 = (t16 + t21) * t7;
    const double t25 = (t15 + t23) * t7;
    const double t26 = a[53];
    const double t27 = t7 * t26;
    const double t28 = a[26];
    const double t30 = (t27 + t28) * t7;
    const double t32 = (t16 + t30) * t7;
    const double t34 = (t27 + t19) * t7;
    const double t33 = x[4];
    const double t35 = t33 * t4;
    const double t37 = (t35 + t18 + t6) * t33;
    const double t39 = (t3 + t34 + t37) * t33;
    const double t41 = (t2 + t32 + t39) * t33;
    const double t44 = a[21];
    const double t45 = a[73];
    const double t46 = t7 * t45;
    const double t47 = a[35];
    const double t49 = (t46 + t47) * t7;
    const double t51 = (t44 + t49) * t7;
    const double t52 = a[77];
    const double t53 = t7 * t52;
    const double t55 = (t53 + t47) * t7;
    const double t56 = t33 * t17;
    const double t58 = (t56 + t46 + t19) * t33;
    const double t60 = (t16 + t55 + t58) * t33;
    const double t62 = (t15 + t51 + t60) * t33;
    const double t63 = t33 * t26;
    const double t65 = (t63 + t53 + t28) * t33;
    const double t67 = (t16 + t55 + t65) * t33;
    const double t69 = (t63 + t46 + t19) * t33;
    const double t66 = x[3];
    const double t70 = t66 * t4;
    const double t72 = (t70 + t56 + t18 + t6) * t66;
    const double t74 = (t3 + t34 + t69 + t72) * t66;
    const double t76 = (t2 + t32 + t67 + t74) * t66;
    const double t79 = a[0];
    const double t80 = a[7];
    const double t81 = a[9];
    const double t82 = a[56];
    const double t83 = t7 * t82;
    const double t84 = a[41];
    const double t86 = (t83 + t84) * t7;
    const double t88 = (t81 + t86) * t7;
    const double t90 = (t80 + t88) * t7;
    const double t91 = a[15];
    const double t92 = a[88];
    const double t93 = t7 * t92;
    const double t94 = a[47];
    const double t96 = (t93 + t94) * t7;
    const double t98 = (t91 + t96) * t7;
    const double t99 = a[74];
    const double t100 = t7 * t99;
    const double t102 = (t100 + t94) * t7;
    const double t103 = t33 * t82;
    const double t105 = (t103 + t93 + t84) * t33;
    const double t107 = (t81 + t102 + t105) * t33;
    const double t109 = (t80 + t98 + t107) * t33;
    const double t110 = a[2];
    const double t111 = a[19];
    const double t112 = a[89];
    const double t113 = t7 * t112;
    const double t114 = a[45];
    const double t116 = (t113 + t114) * t7;
    const double t118 = (t111 + t116) * t7;
    const double t119 = a[94];
    const double t120 = t7 * t119;
    const double t121 = a[30];
    const double t123 = (t120 + t121) * t7;
    const double t124 = t33 * t112;
    const double t126 = (t124 + t120 + t114) * t33;
    const double t128 = (t111 + t123 + t126) * t33;
    const double t129 = a[12];
    const double t130 = a[59];
    const double t131 = t7 * t130;
    const double t132 = a[46];
    const double t134 = (t131 + t132) * t7;
    const double t135 = t33 * t130;
    const double t136 = a[67];
    const double t137 = t7 * t136;
    const double t139 = (t135 + t137 + t132) * t33;
    const double t140 = a[57];
    const double t141 = t66 * t140;
    const double t142 = a[62];
    const double t143 = t33 * t142;
    const double t144 = t7 * t142;
    const double t145 = a[22];
    const double t147 = (t141 + t143 + t144 + t145) * t66;
    const double t149 = (t129 + t134 + t139 + t147) * t66;
    const double t151 = (t110 + t118 + t128 + t149) * t66;
    const double t152 = a[4];
    const double t153 = a[8];
    const double t154 = a[90];
    const double t155 = t7 * t154;
    const double t156 = a[39];
    const double t158 = (t155 + t156) * t7;
    const double t160 = (t153 + t158) * t7;
    const double t161 = a[98];
    const double t162 = t7 * t161;
    const double t163 = a[33];
    const double t165 = (t162 + t163) * t7;
    const double t166 = t33 * t154;
    const double t168 = (t166 + t162 + t156) * t33;
    const double t170 = (t153 + t165 + t168) * t33;
    const double t171 = a[10];
    const double t172 = a[68];
    const double t173 = t7 * t172;
    const double t174 = a[36];
    const double t176 = (t173 + t174) * t7;
    const double t177 = t33 * t172;
    const double t178 = a[50];
    const double t179 = t7 * t178;
    const double t181 = (t177 + t179 + t174) * t33;
    const double t182 = a[85];
    const double t183 = t66 * t182;
    const double t184 = a[81];
    const double t185 = t33 * t184;
    const double t186 = t7 * t184;
    const double t187 = a[42];
    const double t189 = (t183 + t185 + t186 + t187) * t66;
    const double t191 = (t171 + t176 + t181 + t189) * t66;
    const double t192 = a[13];
    const double t193 = a[51];
    const double t194 = t7 * t193;
    const double t195 = a[24];
    const double t197 = (t194 + t195) * t7;
    const double t198 = t33 * t193;
    const double t199 = a[99];
    const double t200 = t7 * t199;
    const double t202 = (t198 + t200 + t195) * t33;
    const double t203 = a[65];
    const double t204 = t66 * t203;
    const double t205 = a[72];
    const double t206 = t33 * t205;
    const double t207 = t7 * t205;
    const double t208 = a[40];
    const double t210 = (t204 + t206 + t207 + t208) * t66;
    const double t211 = a[101];
    const double t196 = x[2];
    const double t212 = t196 * t211;
    const double t213 = a[97];
    const double t214 = t213 * t66;
    const double t215 = a[79];
    const double t216 = t33 * t215;
    const double t217 = t7 * t215;
    const double t218 = a[38];
    const double t220 = (t212 + t214 + t216 + t217 + t218) * t196;
    const double t222 = (t192 + t197 + t202 + t210 + t220) * t196;
    const double t224 = (t152 + t160 + t170 + t191 + t222) * t196;
    const double t227 = t33 * t140;
    const double t229 = (t227 + t144 + t145) * t33;
    const double t231 = (t129 + t134 + t229) * t33;
    const double t233 = (t110 + t118 + t231) * t33;
    const double t235 = (t143 + t137 + t132) * t33;
    const double t237 = (t111 + t123 + t235) * t33;
    const double t239 = (t135 + t120 + t114) * t33;
    const double t240 = t66 * t82;
    const double t242 = (t240 + t124 + t93 + t84) * t66;
    const double t244 = (t81 + t102 + t239 + t242) * t66;
    const double t246 = (t80 + t98 + t237 + t244) * t66;
    const double t247 = a[6];
    const double t248 = a[17];
    const double t249 = a[86];
    const double t250 = t7 * t249;
    const double t251 = a[44];
    const double t253 = (t250 + t251) * t7;
    const double t255 = (t248 + t253) * t7;
    const double t256 = a[20];
    const double t257 = a[92];
    const double t258 = t7 * t257;
    const double t259 = a[37];
    const double t261 = (t258 + t259) * t7;
    const double t262 = a[58];
    const double t263 = t33 * t262;
    const double t264 = a[80];
    const double t265 = t7 * t264;
    const double t266 = a[48];
    const double t268 = (t263 + t265 + t266) * t33;
    const double t270 = (t256 + t261 + t268) * t33;
    const double t271 = a[78];
    const double t272 = t33 * t271;
    const double t273 = a[63];
    const double t274 = t7 * t273;
    const double t275 = a[32];
    const double t277 = (t272 + t274 + t275) * t33;
    const double t278 = t66 * t262;
    const double t280 = (t278 + t272 + t265 + t266) * t66;
    const double t282 = (t256 + t261 + t277 + t280) * t66;
    const double t283 = a[16];
    const double t284 = a[60];
    const double t285 = t7 * t284;
    const double t286 = a[31];
    const double t288 = (t285 + t286) * t7;
    const double t289 = a[66];
    const double t290 = t33 * t289;
    const double t291 = a[70];
    const double t292 = t7 * t291;
    const double t293 = a[28];
    const double t295 = (t290 + t292 + t293) * t33;
    const double t296 = a[71];
    const double t297 = t66 * t296;
    const double t298 = a[82];
    const double t299 = t33 * t298;
    const double t300 = a[93];
    const double t301 = t7 * t300;
    const double t302 = a[29];
    const double t304 = (t297 + t299 + t301 + t302) * t66;
    const double t305 = a[75];
    const double t306 = t305 * t196;
    const double t307 = a[96];
    const double t308 = t66 * t307;
    const double t309 = a[54];
    const double t310 = t33 * t309;
    const double t311 = a[87];
    const double t312 = t7 * t311;
    const double t313 = a[49];
    const double t315 = (t306 + t308 + t310 + t312 + t313) * t196;
    const double t317 = (t283 + t288 + t295 + t304 + t315) * t196;
    const double t319 = (t247 + t255 + t270 + t282 + t317) * t196;
    const double t320 = t33 * t182;
    const double t322 = (t320 + t186 + t187) * t33;
    const double t324 = (t171 + t176 + t322) * t33;
    const double t326 = (t185 + t179 + t174) * t33;
    const double t327 = t66 * t154;
    const double t329 = (t327 + t177 + t162 + t156) * t66;
    const double t331 = (t153 + t165 + t326 + t329) * t66;
    const double t332 = t33 * t296;
    const double t334 = (t332 + t301 + t302) * t33;
    const double t335 = t66 * t289;
    const double t337 = (t335 + t299 + t292 + t293) * t66;
    const double t338 = a[83];
    const double t339 = t196 * t338;
    const double t340 = a[55];
    const double t341 = t66 * t340;
    const double t342 = t33 * t340;
    const double t343 = a[76];
    const double t344 = t343 * t7;
    const double t345 = a[27];
    const double t347 = (t339 + t341 + t342 + t344 + t345) * t196;
    const double t349 = (t283 + t288 + t334 + t337 + t347) * t196;
    const double t350 = t33 * t203;
    const double t352 = (t350 + t207 + t208) * t33;
    const double t353 = t66 * t193;
    const double t355 = (t353 + t206 + t200 + t195) * t66;
    const double t356 = t66 * t309;
    const double t357 = t33 * t307;
    const double t359 = (t339 + t356 + t357 + t312 + t313) * t196;
    const double t348 = x[1];
    const double t360 = t348 * t211;
    const double t361 = t66 * t215;
    const double t362 = t213 * t33;
    const double t364 = (t360 + t306 + t361 + t362 + t217 + t218) * t348;
    const double t366 = (t192 + t197 + t352 + t355 + t359 + t364) * t348;
    const double t368 = (t152 + t160 + t324 + t331 + t349 + t366) * t348;
    const double t371 = t7 * t140;
    const double t373 = (t371 + t145) * t7;
    const double t375 = (t129 + t373) * t7;
    const double t377 = (t110 + t375) * t7;
    const double t379 = (t144 + t132) * t7;
    const double t381 = (t111 + t379) * t7;
    const double t383 = (t131 + t114) * t7;
    const double t385 = (t103 + t113 + t84) * t33;
    const double t387 = (t81 + t383 + t385) * t33;
    const double t389 = (t80 + t381 + t387) * t33;
    const double t391 = (t137 + t121) * t7;
    const double t392 = t33 * t92;
    const double t394 = (t392 + t120 + t94) * t33;
    const double t396 = (t91 + t391 + t394) * t33;
    const double t397 = t33 * t99;
    const double t399 = (t397 + t120 + t94) * t33;
    const double t401 = (t240 + t392 + t113 + t84) * t66;
    const double t403 = (t81 + t383 + t399 + t401) * t66;
    const double t405 = (t80 + t381 + t396 + t403) * t66;
    const double t406 = t7 * t262;
    const double t408 = (t406 + t266) * t7;
    const double t410 = (t256 + t408) * t7;
    const double t412 = (t265 + t259) * t7;
    const double t413 = t33 * t249;
    const double t415 = (t413 + t258 + t251) * t33;
    const double t417 = (t248 + t412 + t415) * t33;
    const double t418 = t7 * t271;
    const double t420 = (t418 + t275) * t7;
    const double t421 = t33 * t257;
    const double t423 = (t421 + t274 + t259) * t33;
    const double t424 = t33 * t264;
    const double t426 = (t278 + t424 + t418 + t266) * t66;
    const double t428 = (t256 + t420 + t423 + t426) * t66;
    const double t429 = t7 * t289;
    const double t431 = (t429 + t293) * t7;
    const double t432 = t33 * t284;
    const double t434 = (t432 + t292 + t286) * t33;
    const double t435 = t33 * t300;
    const double t436 = t7 * t298;
    const double t438 = (t297 + t435 + t436 + t302) * t66;
    const double t439 = t33 * t311;
    const double t440 = t7 * t309;
    const double t442 = (t306 + t308 + t439 + t440 + t313) * t196;
    const double t444 = (t283 + t431 + t434 + t438 + t442) * t196;
    const double t446 = (t247 + t410 + t417 + t428 + t444) * t196;
    const double t448 = (t263 + t418 + t266) * t33;
    const double t450 = (t256 + t420 + t448) * t33;
    const double t452 = (t424 + t274 + t259) * t33;
    const double t453 = t66 * t249;
    const double t455 = (t453 + t421 + t258 + t251) * t66;
    const double t457 = (t248 + t412 + t452 + t455) * t66;
    const double t458 = a[14];
    const double t459 = a[69];
    const double t460 = t7 * t459;
    const double t461 = a[23];
    const double t463 = (t460 + t461) * t7;
    const double t464 = t33 * t459;
    const double t465 = a[52];
    const double t466 = t7 * t465;
    const double t468 = (t464 + t466 + t461) * t33;
    const double t469 = t66 * t459;
    const double t470 = t33 * t465;
    const double t472 = (t469 + t470 + t466 + t461) * t66;
    const double t473 = a[61];
    const double t474 = t196 * t473;
    const double t475 = a[95];
    const double t476 = t475 * t66;
    const double t477 = a[64];
    const double t478 = t33 * t477;
    const double t479 = t7 * t477;
    const double t480 = a[34];
    const double t482 = (t474 + t476 + t478 + t479 + t480) * t196;
    const double t484 = (t458 + t463 + t468 + t472 + t482) * t196;
    const double t486 = (t332 + t436 + t302) * t33;
    const double t487 = t66 * t284;
    const double t489 = (t487 + t435 + t292 + t286) * t66;
    const double t490 = a[91];
    const double t491 = t490 * t196;
    const double t492 = t66 * t477;
    const double t493 = t475 * t33;
    const double t495 = (t491 + t492 + t493 + t479 + t480) * t196;
    const double t496 = t348 * t305;
    const double t497 = t66 * t311;
    const double t499 = (t496 + t474 + t497 + t357 + t440 + t313) * t348;
    const double t501 = (t283 + t431 + t486 + t489 + t495 + t499) * t348;
    const double t503 = (t247 + t410 + t450 + t457 + t484 + t501) * t348;
    const double t504 = t7 * t182;
    const double t506 = (t504 + t187) * t7;
    const double t508 = (t171 + t506) * t7;
    const double t510 = (t186 + t174) * t7;
    const double t512 = (t166 + t173 + t156) * t33;
    const double t514 = (t153 + t510 + t512) * t33;
    const double t515 = t33 * t161;
    const double t517 = (t515 + t179 + t163) * t33;
    const double t519 = (t327 + t515 + t173 + t156) * t66;
    const double t521 = (t153 + t510 + t517 + t519) * t66;
    const double t522 = t7 * t296;
    const double t524 = (t522 + t302) * t7;
    const double t526 = (t432 + t301 + t286) * t33;
    const double t527 = t33 * t291;
    const double t529 = (t335 + t527 + t436 + t293) * t66;
    const double t530 = t343 * t33;
    const double t531 = t7 * t340;
    const double t533 = (t339 + t341 + t530 + t531 + t345) * t196;
    const double t535 = (t283 + t524 + t526 + t529 + t533) * t196;
    const double t537 = (t290 + t436 + t293) * t33;
    const double t539 = (t487 + t527 + t301 + t286) * t66;
    const double t540 = t475 * t7;
    const double t542 = (t491 + t492 + t478 + t540 + t480) * t196;
    const double t543 = t348 * t338;
    const double t544 = t343 * t66;
    const double t546 = (t543 + t491 + t544 + t342 + t531 + t345) * t348;
    const double t548 = (t283 + t524 + t537 + t539 + t542 + t546) * t348;
    const double t549 = t7 * t203;
    const double t551 = (t549 + t208) * t7;
    const double t553 = (t198 + t207 + t195) * t33;
    const double t554 = t33 * t199;
    const double t556 = (t353 + t554 + t207 + t195) * t66;
    const double t557 = t7 * t307;
    const double t559 = (t339 + t356 + t439 + t557 + t313) * t196;
    const double t561 = (t543 + t474 + t497 + t310 + t557 + t313) * t348;
    const double t550 = x[0];
    const double t562 = t550 * t211;
    const double t563 = t213 * t7;
    const double t565 = (t562 + t496 + t306 + t361 + t216 + t563 + t218) * t550;
    const double t567 = (t192 + t551 + t553 + t556 + t559 + t561 + t565) * t550;
    const double t569 = (t152 + t508 + t514 + t521 + t535 + t548 + t567) * t550;
    const double t593 = 2.0 * t543;
    const double t596 = t305 * t550;
    const double t611 = 2.0 * t306;
    const double t616 = 2.0 * t339;
    const double t632 = t473 * t348;
    const double t633 = 2.0 * t491;
    const double t673 = 2.0 * t240;
    const double t678 = 2.0 * t278;
    const double t681 = t307 * t196;
    const double t682 = 2.0 * t297;
    const double t687 = 2.0 * t327;
    const double t690 = t340 * t196;
    const double t691 = 2.0 * t335;
    const double t694 = t215 * t348;
    const double t695 = t309 * t196;
    const double t696 = 2.0 * t353;
    const double t716 = t475 * t196;
    const double t720 = t311 * t348;
    const double t721 = t477 * t196;
    const double t722 = 2.0 * t487;
    const double t734 = t215 * t550;
    const double t754 = 2.0 * t63;
    const double t757 = t17 * t66;
    const double t764 = 2.0 * t103;
    const double t772 = t142 * t66;
    const double t773 = 2.0 * t135;
    const double t778 = 2.0 * t166;
    const double t781 = t184 * t66;
    const double t785 = t215 * t196;
    const double t786 = t205 * t66;
    const double t787 = 2.0 * t198;
    const double t802 = t112 * t66;
    const double t807 = 2.0 * t263;
    const double t810 = t271 * t66;
    const double t814 = t298 * t66;
    const double t815 = 2.0 * t290;
    const double t823 = t172 * t66;
    const double t827 = 2.0 * t332;
    const double t845 = t92 * t66;
    const double t854 = t264 * t66;
    const double t858 = t311 * t196;
    const double t859 = t300 * t66;
    const double t860 = 2.0 * t432;
    const double t867 = t257 * t66;
    const double t871 = t465 * t66;
    const double t875 = t307 * t348;
    const double t882 = t161 * t66;
    const double t886 = t343 * t196;
    const double t887 = t291 * t66;
    const double t890 = t340 * t348;
    const double t893 = t309 * t348;
    const double t894 = t199 * t66;
    const double t913 = ((2.0 * t18 + t19) * t7 + t16 + t21) * t7;
    const double t914 = 2.0 * t27;
    const double t916 = (t914 + t28) * t7;
    const double t926 = t45 * t33;
    const double t927 = 2.0 * t53;
    const double t945 = ((2.0 * t83 + t84) * t7 + t81 + t86) * t7;
    const double t948 = (2.0 * t93 + t94) * t7;
    const double t949 = 2.0 * t100;
    const double t956 = (2.0 * t113 + t114) * t7;
    const double t957 = t119 * t33;
    const double t958 = 2.0 * t120;
    const double t961 = t136 * t33;
    const double t962 = 2.0 * t131;
    const double t969 = (2.0 * t155 + t156) * t7;
    const double t970 = 2.0 * t162;
    const double t973 = t178 * t33;
    const double t974 = 2.0 * t173;
    const double t977 = 2.0 * t194;
    const double t997 = 2.0 * t258;
    const double t1000 = t273 * t33;
    const double t1003 = 2.0 * t285;
    const double t1027 = (2.0 * t144 + t132) * t7;
    const double t1041 = (2.0 * t406 + t266) * t7;
    const double t1042 = 2.0 * t265;
    const double t1045 = 2.0 * t418;
    const double t1048 = 2.0 * t429;
    const double t1067 = 2.0 * t186;
    const double t1072 = 2.0 * t522;
    g[0] = (((2.0 * t562 + t496 + t306 + t361 + t216 + t563 + t218) * t550 + t192 + t551 + t553 + t556 + t559 + t561 +
             t565) *
                t550 +
            t152 + t508 + t514 + t521 + t535 + t548 + t567) *
               t550 +
           t79 + t377 + t389 + t405 + t446 + t503 + t569;
    g[1] = (((2.0 * t360 + t306 + t361 + t362 + t217 + t218) * t348 + t192 + t197 + t352 + t355 + t359 + t364) * t348 +
            t152 + t160 + t324 + t331 + t349 + t366) *
               t348 +
           t79 + t90 + t233 + t246 + t319 + t368 +
           (((2.0 * t496 + t474 + t497 + t357 + t440 + t313) * t348 + t283 + t431 + t486 + t489 + t495 + t499) * t348 +
            t247 + t410 + t450 + t457 + t484 + t501 +
            ((t593 + t491 + t544 + t342 + t531 + t345) * t348 + t283 + t524 + t537 + t539 + t542 + t546 +
             (t596 + t593 + t474 + t497 + t310 + t557 + t313) * t550) *
                t550) *
               t550;
    g[2] = (((2.0 * t212 + t214 + t216 + t217 + t218) * t196 + t192 + t197 + t202 + t210 + t220) * t196 + t152 + t160 +
            t170 + t191 + t222) *
               t196 +
           t79 + t90 + t109 + t151 + t224 +
           (((t611 + t308 + t310 + t312 + t313) * t196 + t283 + t288 + t295 + t304 + t315) * t196 + t247 + t255 + t270 +
            t282 + t317 +
            ((t616 + t341 + t342 + t344 + t345) * t196 + t283 + t288 + t334 + t337 + t347 +
             (t496 + t616 + t356 + t357 + t312 + t313) * t348) *
                t348) *
               t348 +
           (((t611 + t308 + t439 + t440 + t313) * t196 + t283 + t431 + t434 + t438 + t442) * t196 + t247 + t410 + t417 +
            t428 + t444 +
            ((2.0 * t474 + t476 + t478 + t479 + t480) * t196 + t458 + t463 + t468 + t472 + t482 +
             (t632 + t633 + t492 + t493 + t479 + t480) * t348) *
                t348 +
            ((t616 + t341 + t530 + t531 + t345) * t196 + t283 + t524 + t526 + t529 + t533 +
             (t348 * t490 + t478 + t480 + t492 + t540 + t633) * t348 +
             (t596 + t632 + t616 + t356 + t439 + t557 + t313) * t550) *
                t550) *
               t550;
    g[3] =
        (((2.0 * t70 + t56 + t18 + t6) * t66 + t3 + t34 + t69 + t72) * t66 + t2 + t32 + t67 + t74) * t66 + t1 + t25 +
        t62 + t76 +
        (((2.0 * t141 + t143 + t144 + t145) * t66 + t129 + t134 + t139 + t147) * t66 + t110 + t118 + t128 + t149 +
         ((2.0 * t183 + t185 + t186 + t187) * t66 + t171 + t176 + t181 + t189 +
          (t196 * t213 + 2.0 * t204 + t206 + t207 + t208) * t196) *
             t196) *
            t196 +
        (((t673 + t124 + t93 + t84) * t66 + t81 + t102 + t239 + t242) * t66 + t80 + t98 + t237 + t244 +
         ((t678 + t272 + t265 + t266) * t66 + t256 + t261 + t277 + t280 + (t681 + t682 + t299 + t301 + t302) * t196) *
             t196 +
         ((t687 + t177 + t162 + t156) * t66 + t153 + t165 + t326 + t329 + (t690 + t691 + t299 + t292 + t293) * t196 +
          (t694 + t695 + t696 + t206 + t200 + t195) * t348) *
             t348) *
            t348 +
        (((t673 + t392 + t113 + t84) * t66 + t81 + t383 + t399 + t401) * t66 + t80 + t381 + t396 + t403 +
         ((t678 + t424 + t418 + t266) * t66 + t256 + t420 + t423 + t426 + (t681 + t682 + t435 + t436 + t302) * t196) *
             t196 +
         ((2.0 * t453 + t421 + t258 + t251) * t66 + t248 + t412 + t452 + t455 +
          (t716 + 2.0 * t469 + t470 + t466 + t461) * t196 + (t720 + t721 + t722 + t435 + t292 + t286) * t348) *
             t348 +
         ((t687 + t515 + t173 + t156) * t66 + t153 + t510 + t517 + t519 + (t690 + t691 + t527 + t436 + t293) * t196 +
          (t343 * t348 + t286 + t301 + t527 + t721 + t722) * t348 +
          (t734 + t720 + t695 + t696 + t554 + t207 + t195) * t550) *
             t550) *
            t550;
    g[4] =
        (((2.0 * t35 + t18 + t6) * t33 + t3 + t34 + t37) * t33 + t2 + t32 + t39) * t33 + t1 + t25 + t41 +
        (((2.0 * t56 + t46 + t19) * t33 + t16 + t55 + t58) * t33 + t15 + t51 + t60 +
         ((t754 + t53 + t28) * t33 + t16 + t55 + t65 + (t757 + t754 + t46 + t19) * t66) * t66) *
            t66 +
        (((t764 + t93 + t84) * t33 + t81 + t102 + t105) * t33 + t80 + t98 + t107 +
         ((2.0 * t124 + t120 + t114) * t33 + t111 + t123 + t126 + (t772 + t773 + t137 + t132) * t66) * t66 +
         ((t778 + t162 + t156) * t33 + t153 + t165 + t168 + (t781 + 2.0 * t177 + t179 + t174) * t66 +
          (t785 + t786 + t787 + t200 + t195) * t196) *
             t196) *
            t196 +
        (((2.0 * t227 + t144 + t145) * t33 + t129 + t134 + t229) * t33 + t110 + t118 + t231 +
         ((2.0 * t143 + t137 + t132) * t33 + t111 + t123 + t235 + (t802 + t773 + t120 + t114) * t66) * t66 +
         ((t807 + t265 + t266) * t33 + t256 + t261 + t268 + (t810 + 2.0 * t272 + t274 + t275) * t66 +
          (t695 + t814 + t815 + t292 + t293) * t196) *
             t196 +
         ((2.0 * t320 + t186 + t187) * t33 + t171 + t176 + t322 + (t823 + 2.0 * t185 + t179 + t174) * t66 +
          (t690 + t814 + t827 + t301 + t302) * t196 + (t213 * t348 + t207 + t208 + 2.0 * t350 + t681 + t786) * t348) *
             t348) *
            t348 +
        (((t764 + t113 + t84) * t33 + t81 + t383 + t385) * t33 + t80 + t381 + t387 +
         ((2.0 * t392 + t120 + t94) * t33 + t91 + t391 + t394 + (t845 + 2.0 * t397 + t120 + t94) * t66) * t66 +
         ((2.0 * t413 + t258 + t251) * t33 + t248 + t412 + t415 + (t854 + 2.0 * t421 + t274 + t259) * t66 +
          (t858 + t859 + t860 + t292 + t286) * t196) *
             t196 +
         ((t807 + t418 + t266) * t33 + t256 + t420 + t448 + (t867 + 2.0 * t424 + t274 + t259) * t66 +
          (t721 + t871 + 2.0 * t464 + t466 + t461) * t196 + (t875 + t716 + t859 + t827 + t436 + t302) * t348) *
             t348 +
         ((t778 + t173 + t156) * t33 + t153 + t510 + t512 + (t882 + 2.0 * t515 + t179 + t163) * t66 +
          (t886 + t887 + t860 + t301 + t286) * t196 + (t890 + t721 + t887 + t815 + t436 + t293) * t348 +
          (t734 + t893 + t858 + t894 + t787 + t207 + t195) * t550) *
             t550) *
            t550;
    g[5] = (((2.0 * t5 + t6) * t7 + t3 + t8) * t7 + t2 + t10) * t7 + t1 + t12 +
           (t913 + t15 + t23 + (t916 + t16 + t30 + (t56 + t914 + t19) * t33) * t33) * t33 +
           (t913 + t15 + t23 + ((2.0 * t46 + t47) * t7 + t44 + t49 + (t926 + t927 + t47) * t33) * t33 +
            (t916 + t16 + t30 + (t33 * t52 + t47 + t927) * t33 + (t757 + t926 + t914 + t19) * t66) * t66) *
               t66 +
           (t945 + t80 + t88 + (t948 + t91 + t96 + (t392 + t949 + t94) * t33) * t33 +
            (t956 + t111 + t116 + (t957 + t958 + t121) * t33 + (t772 + t961 + t962 + t132) * t66) * t66 +
            (t969 + t153 + t158 + (t515 + t970 + t163) * t33 + (t781 + t973 + t974 + t174) * t66 +
             (t785 + t786 + t554 + t977 + t195) * t196) *
                t196) *
               t196 +
           (t945 + t80 + t88 + (t956 + t111 + t116 + (t143 + t962 + t132) * t33) * t33 +
            (t948 + t91 + t96 + (t961 + t958 + t121) * t33 + (t845 + t957 + t949 + t94) * t66) * t66 +
            ((2.0 * t250 + t251) * t7 + t248 + t253 + (t424 + t997 + t259) * t33 + (t854 + t1000 + t997 + t259) * t66 +
             (t858 + t859 + t527 + t1003 + t286) * t196) *
                t196 +
            (t969 + t153 + t158 + (t185 + t974 + t174) * t33 + (t882 + t973 + t970 + t163) * t66 +
             (t886 + t887 + t435 + t1003 + t286) * t196 + (t694 + t858 + t894 + t206 + t977 + t195) * t348) *
                t348) *
               t348 +
           (((2.0 * t371 + t145) * t7 + t129 + t373) * t7 + t110 + t375 +
            (t1027 + t111 + t379 + (t124 + t962 + t114) * t33) * t33 +
            (t1027 + t111 + t379 + (t957 + 2.0 * t137 + t121) * t33 + (t802 + t957 + t962 + t114) * t66) * t66 +
            (t1041 + t256 + t408 + (t421 + t1042 + t259) * t33 + (t810 + t1000 + t1045 + t275) * t66 +
             (t695 + t814 + t527 + t1048 + t293) * t196) *
                t196 +
            (t1041 + t256 + t408 + (t272 + t1045 + t275) * t33 + (t867 + t1000 + t1042 + t259) * t66 +
             (t721 + t871 + t470 + 2.0 * t460 + t461) * t196 + (t893 + t721 + t887 + t299 + t1048 + t293) * t348) *
                t348 +
            ((2.0 * t504 + t187) * t7 + t171 + t506 + (t177 + t1067 + t174) * t33 + (t823 + t973 + t1067 + t174) * t66 +
             (t690 + t814 + t435 + t1072 + t302) * t196 + (t890 + t716 + t859 + t299 + t1072 + t302) * t348 +
             (t213 * t550 + t206 + t208 + 2.0 * t549 + t681 + t786 + t875) * t550) *
                t550) *
               t550;
    return (t1 + t12) * t7 + (t1 + t25 + t41) * t33 + (t1 + t25 + t62 + t76) * t66 +
           (t79 + t90 + t109 + t151 + t224) * t196 + (t79 + t90 + t233 + t246 + t319 + t368) * t348 +
           (t79 + t377 + t389 + t405 + t446 + t503 + t569) * t550;
}

}  // namespace mbnrg_A1B3_deg5
