
/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "poly_2b_A2_A2_deg6_v1.h"

/**
 * @file poly_2b_A2_A2_deg6_grad_v1.cpp
 * @brief Contains the implementation of the polynomials with gradients for symmetry A2_A2
 */

/**
 * @namespace mbnrg_A2_A2_deg6
 * @brief Encloses the structure of the polynomial holder for symmetry A2_A2
 */
namespace mbnrg_A2_A2_deg6 {

double poly_A2_A2_deg6_v1::eval(const double x[6], const double a[151], double g[6]) {
    const double t1 = a[0];
    const double t2 = a[4];
    const double t3 = a[9];
    const double t4 = a[35];
    const double t5 = a[143];
    const double t8 = x[5];
    const double t6 = t8 * t5;
    const double t7 = a[56];
    const double t9 = (t6 + t7) * t8;
    const double t11 = (t4 + t9) * t8;
    const double t13 = (t3 + t11) * t8;
    const double t15 = (t2 + t13) * t8;
    const double t16 = a[3];
    const double t17 = a[8];
    const double t18 = a[18];
    const double t19 = a[112];
    const double t20 = t8 * t19;
    const double t21 = a[64];
    const double t23 = (t20 + t21) * t8;
    const double t25 = (t18 + t23) * t8;
    const double t27 = (t17 + t25) * t8;
    const double t28 = a[10];
    const double t29 = a[15];
    const double t30 = a[115];
    const double t31 = t8 * t30;
    const double t32 = a[55];
    const double t34 = (t31 + t32) * t8;
    const double t36 = (t29 + t34) * t8;
    const double t37 = a[27];
    const double t38 = a[138];
    const double t39 = t8 * t38;
    const double t40 = a[53];
    const double t42 = (t39 + t40) * t8;
    const double t43 = a[84];
    const double t50 = x[4];
    const double t44 = t50 * t43;
    const double t45 = a[121];
    const double t46 = t45 * t8;
    const double t47 = a[67];
    const double t49 = (t44 + t46 + t47) * t50;
    const double t51 = (t37 + t42 + t49) * t50;
    const double t53 = (t28 + t36 + t51) * t50;
    const double t55 = (t16 + t27 + t53) * t50;
    const double t58 = a[2];
    const double t59 = a[12];
    const double t60 = a[22];
    const double t61 = a[96];
    const double t62 = t8 * t61;
    const double t63 = a[65];
    const double t65 = (t62 + t63) * t8;
    const double t67 = (t60 + t65) * t8;
    const double t69 = (t59 + t67) * t8;
    const double t70 = a[5];
    const double t71 = a[28];
    const double t72 = a[114];
    const double t73 = t8 * t72;
    const double t74 = a[57];
    const double t76 = (t73 + t74) * t8;
    const double t78 = (t71 + t76) * t8;
    const double t79 = a[29];
    const double t80 = a[129];
    const double t81 = t8 * t80;
    const double t82 = a[54];
    const double t84 = (t81 + t82) * t8;
    const double t85 = a[126];
    const double t86 = t50 * t85;
    const double t87 = a[90];
    const double t88 = t8 * t87;
    const double t89 = a[70];
    const double t91 = (t86 + t88 + t89) * t50;
    const double t93 = (t79 + t84 + t91) * t50;
    const double t95 = (t70 + t78 + t93) * t50;
    const double t97 = (t58 + t69 + t95) * t50;
    const double t98 = a[33];
    const double t99 = a[101];
    const double t100 = t8 * t99;
    const double t101 = a[43];
    const double t103 = (t100 + t101) * t8;
    const double t104 = a[78];
    const double t105 = t50 * t104;
    const double t106 = a[125];
    const double t107 = t8 * t106;
    const double t108 = a[62];
    const double t110 = (t105 + t107 + t108) * t50;
    const double t112 = (t98 + t103 + t110) * t50;
    const double t114 = (t70 + t78 + t112) * t50;
    const double t115 = a[91];
    const double t116 = t50 * t115;
    const double t118 = (t116 + t107 + t108) * t50;
    const double t120 = (t79 + t84 + t118) * t50;
    const double t122 = (t105 + t88 + t89) * t50;
    const double t124 = x[3];
    const double t123 = t124 * t43;
    const double t125 = (t123 + t86 + t46 + t47) * t124;
    const double t127 = (t37 + t42 + t122 + t125) * t124;
    const double t129 = (t28 + t36 + t120 + t127) * t124;
    const double t131 = (t16 + t27 + t114 + t129) * t124;
    const double t134 = a[6];
    const double t135 = a[25];
    const double t136 = a[131];
    const double t137 = t8 * t136;
    const double t138 = a[44];
    const double t140 = (t137 + t138) * t8;
    const double t142 = (t135 + t140) * t8;
    const double t144 = (t134 + t142) * t8;
    const double t145 = a[31];
    const double t146 = a[142];
    const double t147 = t8 * t146;
    const double t148 = a[68];
    const double t150 = (t147 + t148) * t8;
    const double t152 = (t145 + t150) * t8;
    const double t153 = a[147];
    const double t154 = t8 * t153;
    const double t155 = a[58];
    const double t157 = (t154 + t155) * t8;
    const double t158 = a[87];
    const double t159 = t8 * t158;
    const double t161 = (t86 + t159 + t89) * t50;
    const double t163 = (t79 + t157 + t161) * t50;
    const double t165 = (t70 + t152 + t163) * t50;
    const double t167 = (t58 + t144 + t165) * t50;
    const double t168 = a[1];
    const double t169 = a[13];
    const double t170 = a[19];
    const double t171 = a[132];
    const double t172 = t8 * t171;
    const double t173 = a[39];
    const double t175 = (t172 + t173) * t8;
    const double t177 = (t170 + t175) * t8;
    const double t179 = (t169 + t177) * t8;
    const double t180 = a[11];
    const double t181 = a[16];
    const double t182 = a[128];
    const double t183 = t8 * t182;
    const double t184 = a[46];
    const double t186 = (t183 + t184) * t8;
    const double t188 = (t181 + t186) * t8;
    const double t189 = a[26];
    const double t190 = a[137];
    const double t191 = t8 * t190;
    const double t192 = a[49];
    const double t194 = (t191 + t192) * t8;
    const double t195 = a[86];
    const double t196 = t50 * t195;
    const double t197 = a[97];
    const double t198 = t8 * t197;
    const double t199 = a[48];
    const double t201 = (t196 + t198 + t199) * t50;
    const double t203 = (t189 + t194 + t201) * t50;
    const double t205 = (t180 + t188 + t203) * t50;
    const double t206 = a[7];
    const double t207 = a[32];
    const double t208 = a[146];
    const double t209 = t8 * t208;
    const double t210 = a[40];
    const double t212 = (t209 + t210) * t8;
    const double t214 = (t207 + t212) * t8;
    const double t215 = a[36];
    const double t216 = a[113];
    const double t217 = t8 * t216;
    const double t218 = a[41];
    const double t220 = (t217 + t218) * t8;
    const double t221 = a[89];
    const double t222 = t50 * t221;
    const double t223 = a[123];
    const double t224 = t8 * t223;
    const double t225 = a[42];
    const double t227 = (t222 + t224 + t225) * t50;
    const double t229 = (t215 + t220 + t227) * t50;
    const double t230 = a[24];
    const double t231 = a[127];
    const double t232 = t8 * t231;
    const double t233 = a[51];
    const double t235 = (t232 + t233) * t8;
    const double t236 = a[92];
    const double t237 = t50 * t236;
    const double t238 = a[139];
    const double t239 = t8 * t238;
    const double t240 = a[47];
    const double t242 = (t237 + t239 + t240) * t50;
    const double t243 = a[124];
    const double t244 = t124 * t243;
    const double t245 = a[98];
    const double t246 = t50 * t245;
    const double t247 = a[95];
    const double t248 = t8 * t247;
    const double t249 = a[59];
    const double t251 = (t244 + t246 + t248 + t249) * t124;
    const double t253 = (t230 + t235 + t242 + t251) * t124;
    const double t255 = (t206 + t214 + t229 + t253) * t124;
    const double t257 = (t168 + t179 + t205 + t255) * t124;
    const double t258 = a[109];
    const double t259 = t8 * t258;
    const double t260 = a[60];
    const double t262 = (t259 + t260) * t8;
    const double t263 = a[136];
    const double t264 = t8 * t263;
    const double t266 = (t105 + t264 + t108) * t50;
    const double t268 = (t98 + t262 + t266) * t50;
    const double t270 = (t70 + t152 + t268) * t50;
    const double t271 = a[140];
    const double t272 = t8 * t271;
    const double t273 = a[63];
    const double t275 = (t272 + t273) * t8;
    const double t276 = a[135];
    const double t277 = t8 * t276;
    const double t279 = (t222 + t277 + t225) * t50;
    const double t281 = (t215 + t275 + t279) * t50;
    const double t282 = a[34];
    const double t283 = a[104];
    const double t284 = t8 * t283;
    const double t285 = a[66];
    const double t287 = (t284 + t285) * t8;
    const double t288 = a[149];
    const double t289 = t50 * t288;
    const double t290 = a[110];
    const double t291 = t8 * t290;
    const double t292 = a[76];
    const double t294 = (t289 + t291 + t292) * t50;
    const double t295 = a[148];
    const double t296 = t124 * t295;
    const double t297 = a[105];
    const double t298 = t50 * t297;
    const double t299 = a[107];
    const double t300 = t8 * t299;
    const double t301 = a[77];
    const double t303 = (t296 + t298 + t300 + t301) * t124;
    const double t305 = (t282 + t287 + t294 + t303) * t124;
    const double t307 = (t206 + t214 + t281 + t305) * t124;
    const double t309 = (t116 + t264 + t108) * t50;
    const double t311 = (t79 + t157 + t309) * t50;
    const double t312 = a[81];
    const double t313 = t8 * t312;
    const double t315 = (t237 + t313 + t240) * t50;
    const double t316 = a[119];
    const double t317 = t124 * t316;
    const double t319 = (t317 + t298 + t300 + t301) * t124;
    const double t321 = (t230 + t235 + t315 + t319) * t124;
    const double t323 = (t105 + t159 + t89) * t50;
    const double t325 = (t296 + t246 + t248 + t249) * t124;
    const double t324 = x[2];
    const double t326 = t324 * t43;
    const double t328 = (t326 + t244 + t86 + t46 + t47) * t324;
    const double t330 = (t37 + t42 + t323 + t325 + t328) * t324;
    const double t332 = (t28 + t36 + t311 + t321 + t330) * t324;
    const double t334 = (t16 + t27 + t270 + t307 + t332) * t324;
    const double t337 = t243 * t50;
    const double t339 = (t337 + t248 + t249) * t50;
    const double t341 = (t230 + t235 + t339) * t50;
    const double t343 = (t206 + t214 + t341) * t50;
    const double t345 = (t168 + t179 + t343) * t50;
    const double t347 = (t246 + t239 + t240) * t50;
    const double t349 = (t215 + t220 + t347) * t50;
    const double t351 = (t180 + t188 + t349) * t50;
    const double t353 = (t237 + t224 + t225) * t50;
    const double t355 = (t189 + t194 + t353) * t50;
    const double t357 = (t222 + t198 + t199) * t50;
    const double t358 = t124 * t85;
    const double t360 = (t358 + t196 + t159 + t89) * t124;
    const double t362 = (t79 + t157 + t357 + t360) * t124;
    const double t364 = (t70 + t152 + t355 + t362) * t124;
    const double t366 = (t58 + t144 + t351 + t364) * t124;
    const double t368 = (t246 + t313 + t240) * t50;
    const double t370 = (t215 + t275 + t368) * t50;
    const double t372 = (t180 + t188 + t370) * t50;
    const double t373 = a[30];
    const double t374 = a[134];
    const double t375 = t8 * t374;
    const double t376 = a[38];
    const double t378 = (t375 + t376) * t8;
    const double t379 = a[80];
    const double t380 = t50 * t379;
    const double t381 = a[130];
    const double t382 = t8 * t381;
    const double t383 = a[69];
    const double t385 = (t380 + t382 + t383) * t50;
    const double t387 = (t373 + t378 + t385) * t50;
    const double t388 = a[133];
    const double t389 = t50 * t388;
    const double t391 = (t389 + t382 + t383) * t50;
    const double t392 = t124 * t245;
    const double t394 = (t392 + t380 + t313 + t240) * t124;
    const double t396 = (t215 + t275 + t391 + t394) * t124;
    const double t398 = (t180 + t188 + t387 + t396) * t124;
    const double t400 = (t237 + t277 + t225) * t50;
    const double t402 = (t189 + t194 + t400) * t50;
    const double t403 = t124 * t297;
    const double t404 = a[116];
    const double t405 = t50 * t404;
    const double t407 = (t403 + t405 + t291 + t292) * t124;
    const double t409 = (t215 + t220 + t391 + t407) * t124;
    const double t411 = (t403 + t380 + t239 + t240) * t124;
    const double t412 = t324 * t85;
    const double t414 = (t412 + t392 + t196 + t88 + t89) * t324;
    const double t416 = (t79 + t84 + t357 + t411 + t414) * t324;
    const double t418 = (t70 + t78 + t402 + t409 + t416) * t324;
    const double t420 = (t58 + t69 + t372 + t398 + t418) * t324;
    const double t421 = t50 * t295;
    const double t423 = (t421 + t300 + t301) * t50;
    const double t425 = (t282 + t287 + t423) * t50;
    const double t427 = (t206 + t214 + t425) * t50;
    const double t429 = (t298 + t291 + t292) * t50;
    const double t431 = (t215 + t275 + t429) * t50;
    const double t433 = (t289 + t277 + t225) * t50;
    const double t434 = t124 * t104;
    const double t436 = (t434 + t222 + t264 + t108) * t124;
    const double t438 = (t98 + t262 + t433 + t436) * t124;
    const double t440 = (t70 + t152 + t431 + t438) * t124;
    const double t442 = (t215 + t220 + t429) * t50;
    const double t444 = (t405 + t382 + t383) * t50;
    const double t445 = t124 * t236;
    const double t447 = (t445 + t389 + t277 + t225) * t124;
    const double t449 = (t189 + t194 + t444 + t447) * t124;
    const double t451 = (t289 + t224 + t225) * t50;
    const double t452 = t124 * t288;
    const double t454 = (t452 + t389 + t224 + t225) * t124;
    const double t455 = t324 * t104;
    const double t457 = (t455 + t445 + t222 + t107 + t108) * t324;
    const double t459 = (t98 + t103 + t451 + t454 + t457) * t324;
    const double t461 = (t70 + t78 + t442 + t449 + t459) * t324;
    const double t462 = t50 * t316;
    const double t464 = (t462 + t300 + t301) * t50;
    const double t466 = (t230 + t235 + t464) * t50;
    const double t468 = (t298 + t313 + t240) * t50;
    const double t469 = t124 * t115;
    const double t471 = (t469 + t237 + t264 + t108) * t124;
    const double t473 = (t79 + t157 + t468 + t471) * t124;
    const double t475 = (t298 + t239 + t240) * t50;
    const double t476 = t124 * t221;
    const double t478 = (t476 + t380 + t198 + t199) * t124;
    const double t479 = t324 * t115;
    const double t481 = (t479 + t476 + t237 + t107 + t108) * t324;
    const double t483 = (t79 + t84 + t475 + t478 + t481) * t324;
    const double t485 = (t421 + t248 + t249) * t50;
    const double t487 = (t434 + t246 + t159 + t89) * t124;
    const double t488 = t124 * t195;
    const double t490 = (t455 + t488 + t246 + t88 + t89) * t324;
    const double t486 = x[1];
    const double t491 = t486 * t43;
    const double t493 = (t491 + t412 + t358 + t337 + t46 + t47) * t486;
    const double t495 = (t37 + t42 + t485 + t487 + t490 + t493) * t486;
    const double t497 = (t28 + t36 + t466 + t473 + t483 + t495) * t486;
    const double t499 = (t16 + t27 + t427 + t440 + t461 + t497) * t486;
    const double t502 = a[14];
    const double t503 = a[20];
    const double t504 = a[99];
    const double t505 = t8 * t504;
    const double t506 = a[74];
    const double t508 = (t505 + t506) * t8;
    const double t510 = (t503 + t508) * t8;
    const double t512 = (t502 + t510) * t8;
    const double t513 = a[21];
    const double t514 = a[145];
    const double t515 = t8 * t514;
    const double t516 = a[37];
    const double t518 = (t515 + t516) * t8;
    const double t520 = (t513 + t518) * t8;
    const double t521 = a[100];
    const double t522 = t8 * t521;
    const double t523 = a[75];
    const double t525 = (t522 + t523) * t8;
    const double t526 = t50 * t45;
    const double t527 = a[108];
    const double t528 = t8 * t527;
    const double t530 = (t526 + t528 + t40) * t50;
    const double t532 = (t29 + t525 + t530) * t50;
    const double t534 = (t17 + t520 + t532) * t50;
    const double t536 = (t2 + t512 + t534) * t50;
    const double t537 = a[17];
    const double t538 = a[141];
    const double t539 = t8 * t538;
    const double t540 = a[71];
    const double t542 = (t539 + t540) * t8;
    const double t544 = (t537 + t542) * t8;
    const double t545 = a[85];
    const double t546 = t8 * t545;
    const double t547 = a[50];
    const double t549 = (t546 + t547) * t8;
    const double t550 = t50 * t158;
    const double t551 = a[106];
    const double t552 = t8 * t551;
    const double t554 = (t550 + t552 + t155) * t50;
    const double t556 = (t145 + t549 + t554) * t50;
    const double t558 = (t134 + t544 + t556) * t50;
    const double t559 = t50 * t263;
    const double t560 = a[111];
    const double t561 = t8 * t560;
    const double t563 = (t559 + t561 + t260) * t50;
    const double t565 = (t145 + t549 + t563) * t50;
    const double t567 = (t559 + t552 + t155) * t50;
    const double t568 = t124 * t45;
    const double t570 = (t568 + t550 + t528 + t40) * t124;
    const double t572 = (t29 + t525 + t567 + t570) * t124;
    const double t574 = (t17 + t520 + t565 + t572) * t124;
    const double t576 = (t2 + t512 + t558 + t574) * t124;
    const double t577 = a[102];
    const double t578 = t8 * t577;
    const double t579 = a[73];
    const double t581 = (t578 + t579) * t8;
    const double t583 = (t537 + t581) * t8;
    const double t584 = a[117];
    const double t585 = t8 * t584;
    const double t587 = (t585 + t547) * t8;
    const double t588 = t50 * t87;
    const double t590 = (t588 + t552 + t82) * t50;
    const double t592 = (t71 + t587 + t590) * t50;
    const double t594 = (t59 + t583 + t592) * t50;
    const double t595 = a[23];
    const double t596 = a[93];
    const double t597 = t8 * t596;
    const double t598 = a[61];
    const double t600 = (t597 + t598) * t8;
    const double t602 = (t595 + t600) * t8;
    const double t603 = a[144];
    const double t604 = t8 * t603;
    const double t605 = a[72];
    const double t607 = (t604 + t605) * t8;
    const double t608 = t50 * t197;
    const double t609 = a[150];
    const double t610 = t8 * t609;
    const double t612 = (t608 + t610 + t192) * t50;
    const double t614 = (t181 + t607 + t612) * t50;
    const double t615 = a[79];
    const double t616 = t8 * t615;
    const double t617 = a[45];
    const double t619 = (t616 + t617) * t8;
    const double t620 = t50 * t276;
    const double t621 = a[120];
    const double t622 = t8 * t621;
    const double t624 = (t620 + t622 + t273) * t50;
    const double t625 = t124 * t247;
    const double t626 = t50 * t312;
    const double t627 = a[103];
    const double t628 = t8 * t627;
    const double t630 = (t625 + t626 + t628 + t233) * t124;
    const double t632 = (t207 + t619 + t624 + t630) * t124;
    const double t634 = (t169 + t602 + t614 + t632) * t124;
    const double t635 = t50 * t106;
    const double t637 = (t635 + t561 + t101) * t50;
    const double t639 = (t71 + t587 + t637) * t50;
    const double t640 = t50 * t223;
    const double t642 = (t640 + t622 + t218) * t50;
    const double t643 = t124 * t299;
    const double t644 = t50 * t290;
    const double t645 = a[82];
    const double t646 = t8 * t645;
    const double t648 = (t643 + t644 + t646 + t285) * t124;
    const double t650 = (t207 + t619 + t642 + t648) * t124;
    const double t652 = (t635 + t552 + t82) * t50;
    const double t653 = t50 * t238;
    const double t655 = (t643 + t653 + t628 + t233) * t124;
    const double t656 = t324 * t45;
    const double t658 = (t656 + t625 + t588 + t528 + t40) * t324;
    const double t660 = (t29 + t525 + t652 + t655 + t658) * t324;
    const double t662 = (t17 + t520 + t639 + t650 + t660) * t324;
    const double t664 = (t2 + t512 + t594 + t634 + t662) * t324;
    const double t665 = t50 * t247;
    const double t667 = (t665 + t628 + t233) * t50;
    const double t669 = (t207 + t619 + t667) * t50;
    const double t671 = (t169 + t602 + t669) * t50;
    const double t673 = (t626 + t622 + t273) * t50;
    const double t675 = (t181 + t607 + t673) * t50;
    const double t677 = (t620 + t610 + t192) * t50;
    const double t678 = t124 * t87;
    const double t680 = (t678 + t608 + t552 + t82) * t124;
    const double t682 = (t71 + t587 + t677 + t680) * t124;
    const double t684 = (t59 + t583 + t675 + t682) * t124;
    const double t686 = (t653 + t622 + t218) * t50;
    const double t688 = (t181 + t607 + t686) * t50;
    const double t689 = t50 * t381;
    const double t690 = a[88];
    const double t691 = t8 * t690;
    const double t693 = (t689 + t691 + t376) * t50;
    const double t694 = t124 * t238;
    const double t696 = (t694 + t689 + t622 + t218) * t124;
    const double t698 = (t181 + t607 + t693 + t696) * t124;
    const double t700 = (t640 + t610 + t192) * t50;
    const double t701 = t124 * t290;
    const double t703 = (t701 + t689 + t622 + t273) * t124;
    const double t704 = t324 * t158;
    const double t705 = t124 * t312;
    const double t707 = (t704 + t705 + t608 + t552 + t155) * t324;
    const double t709 = (t145 + t549 + t700 + t703 + t707) * t324;
    const double t711 = (t134 + t544 + t688 + t698 + t709) * t324;
    const double t712 = t50 * t299;
    const double t714 = (t712 + t646 + t285) * t50;
    const double t716 = (t207 + t619 + t714) * t50;
    const double t718 = (t644 + t622 + t218) * t50;
    const double t719 = t124 * t106;
    const double t721 = (t719 + t640 + t561 + t101) * t124;
    const double t723 = (t71 + t587 + t718 + t721) * t124;
    const double t725 = (t644 + t622 + t273) * t50;
    const double t726 = t124 * t223;
    const double t728 = (t726 + t689 + t610 + t192) * t124;
    const double t729 = t324 * t263;
    const double t730 = t124 * t276;
    const double t732 = (t729 + t730 + t620 + t561 + t260) * t324;
    const double t734 = (t145 + t549 + t725 + t728 + t732) * t324;
    const double t736 = (t712 + t628 + t233) * t50;
    const double t738 = (t719 + t653 + t552 + t82) * t124;
    const double t739 = t124 * t197;
    const double t741 = (t729 + t739 + t626 + t552 + t155) * t324;
    const double t742 = t486 * t45;
    const double t744 = (t742 + t704 + t678 + t665 + t528 + t40) * t486;
    const double t746 = (t29 + t525 + t736 + t738 + t741 + t744) * t486;
    const double t748 = (t17 + t520 + t716 + t723 + t734 + t746) * t486;
    const double t750 = (t2 + t512 + t671 + t684 + t711 + t748) * t486;
    const double t752 = t8 * a[122];
    const double t753 = a[52];
    const double t755 = (t752 + t753) * t8;
    const double t757 = (t503 + t755) * t8;
    const double t759 = t8 * a[83];
    const double t761 = (t759 + t516) * t8;
    const double t762 = t50 * t38;
    const double t764 = (t762 + t522 + t32) * t50;
    const double t766 = (t18 + t761 + t764) * t50;
    const double t768 = (t3 + t757 + t766) * t50;
    const double t770 = t8 * a[94];
    const double t772 = (t770 + t579) * t8;
    const double t773 = t50 * t153;
    const double t775 = (t773 + t585 + t148) * t50;
    const double t777 = (t135 + t772 + t775) * t50;
    const double t778 = t50 * t258;
    const double t780 = (t778 + t585 + t148) * t50;
    const double t781 = t124 * t38;
    const double t783 = (t781 + t773 + t522 + t32) * t124;
    const double t785 = (t18 + t761 + t780 + t783) * t124;
    const double t787 = (t3 + t757 + t777 + t785) * t124;
    const double t789 = (t770 + t540) * t8;
    const double t790 = t50 * t80;
    const double t792 = (t790 + t546 + t74) * t50;
    const double t794 = (t60 + t789 + t792) * t50;
    const double t796 = t8 * a[118];
    const double t798 = (t796 + t598) * t8;
    const double t799 = t50 * t190;
    const double t801 = (t799 + t604 + t184) * t50;
    const double t802 = t124 * t231;
    const double t803 = t50 * t271;
    const double t805 = (t802 + t803 + t616 + t210) * t124;
    const double t807 = (t170 + t798 + t801 + t805) * t124;
    const double t808 = t50 * t99;
    const double t810 = (t808 + t546 + t74) * t50;
    const double t811 = t124 * t283;
    const double t812 = t50 * t216;
    const double t814 = (t811 + t812 + t616 + t210) * t124;
    const double t815 = t324 * t38;
    const double t817 = (t815 + t802 + t790 + t522 + t32) * t324;
    const double t819 = (t18 + t761 + t810 + t814 + t817) * t324;
    const double t821 = (t3 + t757 + t794 + t807 + t819) * t324;
    const double t822 = t50 * t231;
    const double t824 = (t822 + t616 + t210) * t50;
    const double t826 = (t170 + t798 + t824) * t50;
    const double t828 = (t803 + t604 + t184) * t50;
    const double t829 = t124 * t80;
    const double t831 = (t829 + t799 + t546 + t74) * t124;
    const double t833 = (t60 + t789 + t828 + t831) * t124;
    const double t835 = (t812 + t604 + t184) * t50;
    const double t836 = t124 * t216;
    const double t837 = t50 * t374;
    const double t839 = (t836 + t837 + t604 + t184) * t124;
    const double t840 = t324 * t153;
    const double t841 = t124 * t271;
    const double t843 = (t840 + t841 + t799 + t585 + t148) * t324;
    const double t845 = (t135 + t772 + t835 + t839 + t843) * t324;
    const double t846 = t50 * t283;
    const double t848 = (t846 + t616 + t210) * t50;
    const double t849 = t124 * t99;
    const double t851 = (t849 + t812 + t546 + t74) * t124;
    const double t852 = t324 * t258;
    const double t853 = t124 * t190;
    const double t855 = (t852 + t853 + t803 + t585 + t148) * t324;
    const double t856 = t486 * t38;
    const double t858 = (t856 + t840 + t829 + t822 + t522 + t32) * t486;
    const double t860 = (t18 + t761 + t848 + t851 + t855 + t858) * t486;
    const double t862 = (t3 + t757 + t826 + t833 + t845 + t860) * t486;
    const double t864 = (t752 + t506) * t8;
    const double t865 = t50 * t30;
    const double t867 = (t865 + t515 + t21) * t50;
    const double t869 = (t4 + t864 + t867) * t50;
    const double t870 = t50 * t146;
    const double t872 = (t870 + t578 + t138) * t50;
    const double t873 = t124 * t30;
    const double t875 = (t873 + t870 + t515 + t21) * t124;
    const double t877 = (t4 + t864 + t872 + t875) * t124;
    const double t878 = t50 * t72;
    const double t880 = (t878 + t539 + t63) * t50;
    const double t881 = t124 * t208;
    const double t882 = t50 * t182;
    const double t884 = (t881 + t882 + t597 + t173) * t124;
    const double t885 = t324 * t30;
    const double t887 = (t885 + t881 + t878 + t515 + t21) * t324;
    const double t889 = (t4 + t864 + t880 + t884 + t887) * t324;
    const double t890 = t50 * t208;
    const double t892 = (t890 + t597 + t173) * t50;
    const double t893 = t124 * t72;
    const double t895 = (t893 + t882 + t539 + t63) * t124;
    const double t896 = t324 * t146;
    const double t897 = t124 * t182;
    const double t899 = (t896 + t897 + t882 + t578 + t138) * t324;
    const double t900 = t486 * t30;
    const double t902 = (t900 + t896 + t893 + t890 + t515 + t21) * t486;
    const double t904 = (t4 + t864 + t892 + t895 + t899 + t902) * t486;
    const double t905 = t50 * t19;
    const double t907 = (t905 + t505 + t7) * t50;
    const double t908 = t124 * t19;
    const double t909 = t50 * t136;
    const double t911 = (t908 + t909 + t505 + t7) * t124;
    const double t912 = t324 * t19;
    const double t913 = t124 * t171;
    const double t914 = t50 * t61;
    const double t916 = (t912 + t913 + t914 + t505 + t7) * t324;
    const double t917 = t486 * t19;
    const double t918 = t324 * t136;
    const double t919 = t124 * t61;
    const double t920 = t50 * t171;
    const double t922 = (t917 + t918 + t919 + t920 + t505 + t7) * t486;
    const double t921 = x[0];
    const double t925 = t5 * (t486 + t50 + t124 + t324) * t921;
    const double t927 = (t907 + t911 + t916 + t922 + t925) * t921;
    const double t929 = (t869 + t877 + t889 + t904 + t927) * t921;
    const double t931 = (t768 + t787 + t821 + t862 + t929) * t921;
    const double t967 = t921 * t5;
    const double t994 = 2.0 * t455;
    const double t1002 = t486 * t85;
    const double t1023 = 2.0 * t729;
    const double t1081 = 2.0 * t296;
    const double t1110 = 2.0 * t403;
    const double t1113 = t324 * t245;
    const double t1120 = 2.0 * t434;
    const double t1128 = t324 * t236;
    const double t1137 = t324 * t221;
    const double t1141 = t324 * t195;
    const double t1162 = 2.0 * t643;
    const double t1180 = t324 * t312;
    const double t1186 = 2.0 * t719;
    const double t1189 = t324 * t276;
    const double t1194 = t324 * t197;
    const double t1218 = t324 * t271;
    const double t1223 = t324 * t190;
    const double t1237 = t324 * t182;
    const double t1262 = 2.0 * t86;
    const double t1269 = 2.0 * t105;
    const double t1274 = 2.0 * t116;
    const double t1296 = 2.0 * t222;
    const double t1299 = 2.0 * t237;
    const double t1312 = 2.0 * t289;
    const double t1336 = 2.0 * t246;
    const double t1356 = t124 * t379;
    const double t1357 = 2.0 * t389;
    const double t1373 = 2.0 * t421;
    const double t1378 = 2.0 * t298;
    const double t1380 = (t1378 + t291 + t292) * t50;
    const double t1385 = t124 * t388;
    const double t1421 = 2.0 * t559;
    const double t1424 = t124 * t158;
    const double t1439 = 2.0 * t620;
    const double t1444 = 2.0 * t635;
    const double t1447 = 2.0 * t640;
    const double t1450 = t324 * t87;
    const double t1472 = t124 * t381;
    const double t1480 = 2.0 * t712;
    const double t1483 = 2.0 * t644;
    const double t1571 = (((2.0 * t6 + t7) * t8 + t4 + t9) * t8 + t3 + t11) * t8;
    const double t1576 = ((2.0 * t20 + t21) * t8 + t18 + t23) * t8;
    const double t1579 = (2.0 * t31 + t32) * t8;
    const double t1580 = 2.0 * t39;
    const double t1593 = ((2.0 * t62 + t63) * t8 + t60 + t65) * t8;
    const double t1596 = (2.0 * t73 + t74) * t8;
    const double t1597 = 2.0 * t81;
    const double t1604 = 2.0 * t100;
    const double t1623 = ((2.0 * t137 + t138) * t8 + t135 + t140) * t8;
    const double t1626 = (2.0 * t147 + t148) * t8;
    const double t1627 = 2.0 * t154;
    const double t1638 = ((2.0 * t172 + t173) * t8 + t170 + t175) * t8;
    const double t1641 = (2.0 * t183 + t184) * t8;
    const double t1642 = 2.0 * t191;
    const double t1649 = (2.0 * t209 + t210) * t8;
    const double t1650 = 2.0 * t217;
    const double t1653 = 2.0 * t232;
    const double t1660 = 2.0 * t259;
    const double t1665 = 2.0 * t272;
    const double t1668 = 2.0 * t284;
    const double t1730 = t124 * t263;
    const double t1739 = t324 * t106;
    const double t1762 = ((2.0 * t505 + t506) * t8 + t503 + t508) * t8;
    const double t1765 = (2.0 * t515 + t516) * t8;
    const double t1767 = 2.0 * t522;
    const double t1776 = (2.0 * t539 + t540) * t8;
    const double t1777 = t50 * t551;
    const double t1778 = 2.0 * t546;
    const double t1783 = t50 * t560;
    const double t1795 = (2.0 * t578 + t579) * t8;
    const double t1796 = 2.0 * t585;
    const double t1803 = (2.0 * t597 + t598) * t8;
    const double t1804 = t50 * t609;
    const double t1805 = 2.0 * t604;
    const double t1808 = t124 * t627;
    const double t1809 = t50 * t621;
    const double t1810 = 2.0 * t616;
    const double t1827 = t50 * t627;
    const double t1833 = (t1809 + t1805 + t605) * t50;
    const double t1834 = t124 * t551;
    const double t1839 = t124 * t621;
    const double t1843 = t324 * t551;
    const double t1865 = 2.0 * t752;
    const double t1867 = (t1865 + t753) * t8;
    const double t1869 = 2.0 * t759;
    const double t1874 = t50 * t584;
    const double t1875 = 2.0 * t770;
    const double t1883 = t50 * t545;
    const double t1886 = t124 * t615;
    const double t1887 = t50 * t603;
    const double t1888 = 2.0 * t796;
    const double t1896 = t50 * t615;
    const double t1899 = t124 * t545;
    const double t1902 = t324 * t584;
    g[0] = (((2.0 * t925 + t907 + t911 + t916 + t922) * t921 + t869 + t877 + t889 + t904 + t927) * t921 + t768 + t787 +
            t821 + t862 + t929) *
               t921 +
           t536 + t576 + t664 + t750 + t931;
    g[1] = ((((2.0 * t491 + t412 + t358 + t337 + t46 + t47) * t486 + t37 + t42 + t485 + t487 + t490 + t493) * t486 +
             t28 + t36 + t466 + t473 + t483 + t495) *
                t486 +
            t16 + t27 + t427 + t440 + t461 + t497) *
               t486 +
           t1 + t15 + t345 + t366 + t420 + t499 +
           ((((2.0 * t742 + t704 + t678 + t665 + t528 + t40) * t486 + t29 + t525 + t736 + t738 + t741 + t744) * t486 +
             t17 + t520 + t716 + t723 + t734 + t746) *
                t486 +
            t2 + t512 + t671 + t684 + t711 + t748 +
            (((2.0 * t856 + t840 + t829 + t822 + t522 + t32) * t486 + t18 + t761 + t848 + t851 + t855 + t858) * t486 +
             t3 + t757 + t826 + t833 + t845 + t860 +
             ((2.0 * t900 + t896 + t893 + t890 + t515 + t21) * t486 + t4 + t864 + t892 + t895 + t899 + t902 +
              (t967 + 2.0 * t917 + t918 + t919 + t920 + t505 + t7) * t921) *
                 t921) *
                t921) *
               t921;
    g[2] = ((((2.0 * t326 + t244 + t86 + t46 + t47) * t324 + t37 + t42 + t323 + t325 + t328) * t324 + t28 + t36 + t311 +
             t321 + t330) *
                t324 +
            t16 + t27 + t270 + t307 + t332) *
               t324 +
           t1 + t15 + t167 + t257 + t334 +
           ((((2.0 * t412 + t392 + t196 + t88 + t89) * t324 + t79 + t84 + t357 + t411 + t414) * t324 + t70 + t78 +
             t402 + t409 + t416) *
                t324 +
            t58 + t69 + t372 + t398 + t418 +
            (((t994 + t445 + t222 + t107 + t108) * t324 + t98 + t103 + t451 + t454 + t457) * t324 + t70 + t78 + t442 +
             t449 + t459 +
             ((2.0 * t479 + t476 + t237 + t107 + t108) * t324 + t79 + t84 + t475 + t478 + t481 +
              (t1002 + t994 + t488 + t246 + t88 + t89) * t486) *
                 t486) *
                t486) *
               t486 +
           ((((2.0 * t656 + t625 + t588 + t528 + t40) * t324 + t29 + t525 + t652 + t655 + t658) * t324 + t17 + t520 +
             t639 + t650 + t660) *
                t324 +
            t2 + t512 + t594 + t634 + t662 +
            (((2.0 * t704 + t705 + t608 + t552 + t155) * t324 + t145 + t549 + t700 + t703 + t707) * t324 + t134 + t544 +
             t688 + t698 + t709 +
             ((t1023 + t730 + t620 + t561 + t260) * t324 + t145 + t549 + t725 + t728 + t732 +
              (t158 * t486 + t1023 + t155 + t552 + t626 + t739) * t486) *
                 t486) *
                t486 +
            (((2.0 * t815 + t802 + t790 + t522 + t32) * t324 + t18 + t761 + t810 + t814 + t817) * t324 + t3 + t757 +
             t794 + t807 + t819 +
             ((2.0 * t840 + t841 + t799 + t585 + t148) * t324 + t135 + t772 + t835 + t839 + t843 +
              (t153 * t486 + t148 + t585 + t803 + 2.0 * t852 + t853) * t486) *
                 t486 +
             ((2.0 * t885 + t881 + t878 + t515 + t21) * t324 + t4 + t864 + t880 + t884 + t887 +
              (t146 * t486 + t138 + t578 + t882 + 2.0 * t896 + t897) * t486 +
              (t136 * t486 + t505 + t7 + 2.0 * t912 + t913 + t914 + t967) * t921) *
                 t921) *
                t921) *
               t921;
    g[3] =
        ((((2.0 * t123 + t86 + t46 + t47) * t124 + t37 + t42 + t122 + t125) * t124 + t28 + t36 + t120 + t127) * t124 +
         t16 + t27 + t114 + t129) *
            t124 +
        t1 + t15 + t97 + t131 +
        ((((2.0 * t244 + t246 + t248 + t249) * t124 + t230 + t235 + t242 + t251) * t124 + t206 + t214 + t229 + t253) *
             t124 +
         t168 + t179 + t205 + t255 +
         (((t1081 + t298 + t300 + t301) * t124 + t282 + t287 + t294 + t303) * t124 + t206 + t214 + t281 + t305 +
          ((2.0 * t317 + t298 + t300 + t301) * t124 + t230 + t235 + t315 + t319 +
           (t243 * t324 + t1081 + t246 + t248 + t249) * t324) *
              t324) *
             t324) *
            t324 +
        ((((2.0 * t358 + t196 + t159 + t89) * t124 + t79 + t157 + t357 + t360) * t124 + t70 + t152 + t355 + t362) *
             t124 +
         t58 + t144 + t351 + t364 +
         (((2.0 * t392 + t380 + t313 + t240) * t124 + t215 + t275 + t391 + t394) * t124 + t180 + t188 + t387 + t396 +
          ((t1110 + t405 + t291 + t292) * t124 + t215 + t220 + t391 + t407 +
           (t1113 + t1110 + t380 + t239 + t240) * t324) *
              t324) *
             t324 +
         (((t1120 + t222 + t264 + t108) * t124 + t98 + t262 + t433 + t436) * t124 + t70 + t152 + t431 + t438 +
          ((2.0 * t445 + t389 + t277 + t225) * t124 + t189 + t194 + t444 + t447 +
           (t1128 + 2.0 * t452 + t389 + t224 + t225) * t324) *
              t324 +
          ((2.0 * t469 + t237 + t264 + t108) * t124 + t79 + t157 + t468 + t471 +
           (t1137 + 2.0 * t476 + t380 + t198 + t199) * t324 + (t1002 + t1141 + t1120 + t246 + t159 + t89) * t486) *
              t486) *
             t486) *
            t486 +
        ((((2.0 * t568 + t550 + t528 + t40) * t124 + t29 + t525 + t567 + t570) * t124 + t17 + t520 + t565 + t572) *
             t124 +
         t2 + t512 + t558 + t574 +
         (((2.0 * t625 + t626 + t628 + t233) * t124 + t207 + t619 + t624 + t630) * t124 + t169 + t602 + t614 + t632 +
          ((t1162 + t644 + t646 + t285) * t124 + t207 + t619 + t642 + t648 +
           (t247 * t324 + t1162 + t233 + t628 + t653) * t324) *
              t324) *
             t324 +
         (((2.0 * t678 + t608 + t552 + t82) * t124 + t71 + t587 + t677 + t680) * t124 + t59 + t583 + t675 + t682 +
          ((2.0 * t694 + t689 + t622 + t218) * t124 + t181 + t607 + t693 + t696 +
           (t1180 + 2.0 * t701 + t689 + t622 + t273) * t324) *
              t324 +
          ((t1186 + t640 + t561 + t101) * t124 + t71 + t587 + t718 + t721 +
           (t1189 + 2.0 * t726 + t689 + t610 + t192) * t324 + (t486 * t87 + t1186 + t1194 + t552 + t653 + t82) * t486) *
              t486) *
             t486 +
         (((2.0 * t781 + t773 + t522 + t32) * t124 + t18 + t761 + t780 + t783) * t124 + t3 + t757 + t777 + t785 +
          ((2.0 * t802 + t803 + t616 + t210) * t124 + t170 + t798 + t801 + t805 +
           (t231 * t324 + t210 + t616 + 2.0 * t811 + t812) * t324) *
              t324 +
          ((2.0 * t829 + t799 + t546 + t74) * t124 + t60 + t789 + t828 + t831 +
           (t1218 + 2.0 * t836 + t837 + t604 + t184) * t324 +
           (t486 * t80 + t1223 + t546 + t74 + t812 + 2.0 * t849) * t486) *
              t486 +
          ((2.0 * t873 + t870 + t515 + t21) * t124 + t4 + t864 + t872 + t875 +
           (t208 * t324 + t173 + t597 + 2.0 * t881 + t882) * t324 +
           (t486 * t72 + t1237 + t539 + t63 + t882 + 2.0 * t893) * t486 +
           (t171 * t324 + t486 * t61 + t505 + t7 + 2.0 * t908 + t909 + t967) * t921) *
              t921) *
             t921) *
            t921;
    g[4] =
        ((((2.0 * t44 + t46 + t47) * t50 + t37 + t42 + t49) * t50 + t28 + t36 + t51) * t50 + t16 + t27 + t53) * t50 +
        t1 + t15 + t55 +
        ((((t1262 + t88 + t89) * t50 + t79 + t84 + t91) * t50 + t70 + t78 + t93) * t50 + t58 + t69 + t95 +
         (((t1269 + t107 + t108) * t50 + t98 + t103 + t110) * t50 + t70 + t78 + t112 +
          ((t1274 + t107 + t108) * t50 + t79 + t84 + t118 + (t358 + t1269 + t88 + t89) * t124) * t124) *
             t124) *
            t124 +
        ((((t1262 + t159 + t89) * t50 + t79 + t157 + t161) * t50 + t70 + t152 + t163) * t50 + t58 + t144 + t165 +
         (((2.0 * t196 + t198 + t199) * t50 + t189 + t194 + t201) * t50 + t180 + t188 + t203 +
          ((t1296 + t224 + t225) * t50 + t215 + t220 + t227 + (t392 + t1299 + t239 + t240) * t124) * t124) *
             t124 +
         (((t1269 + t264 + t108) * t50 + t98 + t262 + t266) * t50 + t70 + t152 + t268 +
          ((t1296 + t277 + t225) * t50 + t215 + t275 + t279 + (t403 + t1312 + t291 + t292) * t124) * t124 +
          ((t1274 + t264 + t108) * t50 + t79 + t157 + t309 + (t403 + t1299 + t313 + t240) * t124 +
           (t412 + t392 + t1269 + t159 + t89) * t324) *
              t324) *
             t324) *
            t324 +
        ((((2.0 * t337 + t248 + t249) * t50 + t230 + t235 + t339) * t50 + t206 + t214 + t341) * t50 + t168 + t179 +
         t343 +
         (((t1336 + t239 + t240) * t50 + t215 + t220 + t347) * t50 + t180 + t188 + t349 +
          ((t1299 + t224 + t225) * t50 + t189 + t194 + t353 + (t488 + t1296 + t198 + t199) * t124) * t124) *
             t124 +
         (((t1336 + t313 + t240) * t50 + t215 + t275 + t368) * t50 + t180 + t188 + t370 +
          ((2.0 * t380 + t382 + t383) * t50 + t373 + t378 + t385 + (t1356 + t1357 + t382 + t383) * t124) * t124 +
          ((t1299 + t277 + t225) * t50 + t189 + t194 + t400 + (t124 * t404 + t1357 + t382 + t383) * t124 +
           (t1141 + t1356 + t1296 + t198 + t199) * t324) *
              t324) *
             t324 +
         (((t1373 + t300 + t301) * t50 + t282 + t287 + t423) * t50 + t206 + t214 + t425 +
          (t1380 + t215 + t275 + t429 + (t476 + t1312 + t277 + t225) * t124) * t124 +
          (t1380 + t215 + t220 + t429 + (t1385 + 2.0 * t405 + t382 + t383) * t124 +
           (t1137 + t1385 + t1312 + t224 + t225) * t324) *
              t324 +
          ((2.0 * t462 + t300 + t301) * t50 + t230 + t235 + t464 + (t445 + t1378 + t313 + t240) * t124 +
           (t1128 + t1356 + t1378 + t239 + t240) * t324 + (t243 * t486 + t1113 + t1373 + t248 + t249 + t392) * t486) *
              t486) *
             t486) *
            t486 +
        ((((2.0 * t526 + t528 + t40) * t50 + t29 + t525 + t530) * t50 + t17 + t520 + t532) * t50 + t2 + t512 + t534 +
         (((2.0 * t550 + t552 + t155) * t50 + t145 + t549 + t554) * t50 + t134 + t544 + t556 +
          ((t1421 + t561 + t260) * t50 + t145 + t549 + t563 + (t1424 + t1421 + t552 + t155) * t124) * t124) *
             t124 +
         (((2.0 * t588 + t552 + t82) * t50 + t71 + t587 + t590) * t50 + t59 + t583 + t592 +
          ((2.0 * t608 + t610 + t192) * t50 + t181 + t607 + t612 + (t705 + t1439 + t622 + t273) * t124) * t124 +
          ((t1444 + t561 + t101) * t50 + t71 + t587 + t637 + (t701 + t1447 + t622 + t218) * t124 +
           (t1450 + t694 + t1444 + t552 + t82) * t324) *
              t324) *
             t324 +
         (((2.0 * t665 + t628 + t233) * t50 + t207 + t619 + t667) * t50 + t169 + t602 + t669 +
          ((2.0 * t626 + t622 + t273) * t50 + t181 + t607 + t673 + (t739 + t1439 + t610 + t192) * t124) * t124 +
          ((2.0 * t653 + t622 + t218) * t50 + t181 + t607 + t686 + (t1472 + 2.0 * t689 + t691 + t376) * t124 +
           (t1194 + t1472 + t1447 + t610 + t192) * t324) *
              t324 +
          ((t1480 + t646 + t285) * t50 + t207 + t619 + t714 + (t726 + t1483 + t622 + t218) * t124 +
           (t1189 + t1472 + t1483 + t622 + t273) * t324 + (t247 * t486 + t1180 + t1480 + t233 + t628 + t694) * t486) *
              t486) *
             t486 +
         (((2.0 * t762 + t522 + t32) * t50 + t18 + t761 + t764) * t50 + t3 + t757 + t766 +
          ((2.0 * t773 + t585 + t148) * t50 + t135 + t772 + t775 + (t124 * t153 + t148 + t585 + 2.0 * t778) * t124) *
              t124 +
          ((2.0 * t790 + t546 + t74) * t50 + t60 + t789 + t792 + (t841 + 2.0 * t799 + t604 + t184) * t124 +
           (t324 * t80 + t546 + t74 + 2.0 * t808 + t836) * t324) *
              t324 +
          ((2.0 * t822 + t616 + t210) * t50 + t170 + t798 + t824 + (t853 + 2.0 * t803 + t604 + t184) * t124 +
           (t124 * t374 + t1223 + t184 + t604 + 2.0 * t812) * t324 +
           (t231 * t486 + t1218 + t210 + t616 + t836 + 2.0 * t846) * t486) *
              t486 +
          ((2.0 * t865 + t515 + t21) * t50 + t4 + t864 + t867 + (t124 * t146 + t138 + t578 + 2.0 * t870) * t124 +
           (t324 * t72 + t539 + t63 + 2.0 * t878 + t897) * t324 +
           (t208 * t486 + t1237 + t173 + t597 + 2.0 * t890 + t897) * t486 +
           (t124 * t136 + t171 * t486 + t324 * t61 + t505 + t7 + 2.0 * t905 + t967) * t921) *
              t921) *
             t921) *
            t921;
    g[5] =
        (t1571 + t2 + t13 + (t1576 + t17 + t25 + (t1579 + t29 + t34 + (t526 + t1580 + t40) * t50) * t50) * t50) * t50 +
        (t1571 + t2 + t13 + (t1593 + t59 + t67 + (t1596 + t71 + t76 + (t588 + t1597 + t82) * t50) * t50) * t50 +
         (t1576 + t17 + t25 + (t1596 + t71 + t76 + (t635 + t1604 + t101) * t50) * t50 +
          (t1579 + t29 + t34 + (t635 + t1597 + t82) * t50 + (t568 + t588 + t1580 + t40) * t124) * t124) *
             t124) *
            t124 +
        (t1571 + t2 + t13 + (t1623 + t134 + t142 + (t1626 + t145 + t150 + (t550 + t1627 + t155) * t50) * t50) * t50 +
         (t1638 + t169 + t177 + (t1641 + t181 + t186 + (t608 + t1642 + t192) * t50) * t50 +
          (t1649 + t207 + t212 + (t640 + t1650 + t218) * t50 + (t625 + t653 + t1653 + t233) * t124) * t124) *
             t124 +
         (t1576 + t17 + t25 + (t1626 + t145 + t150 + (t559 + t1660 + t260) * t50) * t50 +
          (t1649 + t207 + t212 + (t620 + t1665 + t273) * t50 + (t643 + t644 + t1668 + t285) * t124) * t124 +
          (t1579 + t29 + t34 + (t559 + t1627 + t155) * t50 + (t643 + t626 + t1653 + t233) * t124 +
           (t656 + t625 + t550 + t1580 + t40) * t324) *
              t324) *
             t324) *
            t324 +
        (t1571 + t2 + t13 + (t1638 + t169 + t177 + (t1649 + t207 + t212 + (t665 + t1653 + t233) * t50) * t50) * t50 +
         (t1623 + t134 + t142 + (t1641 + t181 + t186 + (t653 + t1650 + t218) * t50) * t50 +
          (t1626 + t145 + t150 + (t640 + t1642 + t192) * t50 + (t1424 + t608 + t1627 + t155) * t124) * t124) *
             t124 +
         (t1593 + t59 + t67 + (t1641 + t181 + t186 + (t626 + t1665 + t273) * t50) * t50 +
          (t1641 + t181 + t186 + (t689 + 2.0 * t375 + t376) * t50 + (t705 + t689 + t1665 + t273) * t124) * t124 +
          (t1596 + t71 + t76 + (t620 + t1642 + t192) * t50 + (t701 + t689 + t1650 + t218) * t124 +
           (t1450 + t694 + t608 + t1597 + t82) * t324) *
              t324) *
             t324 +
         (t1576 + t17 + t25 + (t1649 + t207 + t212 + (t712 + t1668 + t285) * t50) * t50 +
          (t1626 + t145 + t150 + (t644 + t1665 + t273) * t50 + (t1730 + t620 + t1660 + t260) * t124) * t124 +
          (t1596 + t71 + t76 + (t644 + t1650 + t218) * t50 + (t730 + t689 + t1642 + t192) * t124 +
           (t1739 + t726 + t640 + t1604 + t101) * t324) *
              t324 +
          (t1579 + t29 + t34 + (t712 + t1653 + t233) * t50 + (t1730 + t626 + t1627 + t155) * t124 +
           (t1739 + t739 + t653 + t1597 + t82) * t324 + (t742 + t1450 + t1424 + t665 + t1580 + t40) * t486) *
              t486) *
             t486) *
            t486 +
        ((t1762 + t502 + t510 + (t1765 + t513 + t518 + (t50 * t527 + t1767 + t523) * t50) * t50) * t50 +
         (t1762 + t502 + t510 + (t1776 + t537 + t542 + (t1777 + t1778 + t547) * t50) * t50 +
          (t1765 + t513 + t518 + (t1783 + t1778 + t547) * t50 + (t124 * t527 + t1767 + t1777 + t523) * t124) * t124) *
             t124 +
         (t1762 + t502 + t510 + (t1795 + t537 + t581 + (t1777 + t1796 + t547) * t50) * t50 +
          (t1803 + t595 + t600 + (t1804 + t1805 + t605) * t50 + (t1808 + t1809 + t1810 + t617) * t124) * t124 +
          (t1765 + t513 + t518 + (t1783 + t1796 + t547) * t50 + (t124 * t645 + t1809 + t1810 + t617) * t124 +
           (t324 * t527 + t1767 + t1777 + t1808 + t523) * t324) *
              t324) *
             t324 +
         (t1762 + t502 + t510 + (t1803 + t595 + t600 + (t1827 + t1810 + t617) * t50) * t50 +
          (t1795 + t537 + t581 + t1833 + (t1834 + t1804 + t1796 + t547) * t124) * t124 +
          (t1776 + t537 + t542 + t1833 + (t50 * t690 + t1805 + t1839 + t605) * t124 +
           (t1843 + t1839 + t1804 + t1778 + t547) * t324) *
              t324 +
          (t1765 + t513 + t518 + (t50 * t645 + t1810 + t617) * t50 + (t124 * t560 + t1796 + t1809 + t547) * t124 +
           (t124 * t609 + t324 * t560 + t1778 + t1809 + t547) * t324 +
           (t486 * t527 + t1767 + t1827 + t1834 + t1843 + t523) * t486) *
              t486) *
             t486 +
         ((t1867 + t503 + t755 + (t50 * t521 + t1869 + t516) * t50) * t50 +
          (t1867 + t503 + t755 + (t1874 + t1875 + t579) * t50 + (t124 * t521 + t1869 + t1874 + t516) * t124) * t124 +
          (t1867 + t503 + t755 + (t1883 + t1875 + t540) * t50 + (t1886 + t1887 + t1888 + t598) * t124 +
           (t324 * t521 + t1869 + t1883 + t1886 + t516) * t324) *
              t324 +
          (t1867 + t503 + t755 + (t1896 + t1888 + t598) * t50 + (t1899 + t1887 + t1875 + t540) * t124 +
           (t124 * t603 + t1875 + t1887 + t1902 + t579) * t324 +
           (t486 * t521 + t1869 + t1896 + t1899 + t1902 + t516) * t486) *
              t486 +
          ((t50 * t514 + t1865 + t506) * t50 + (t124 * t514 + t50 * t577 + t1865 + t506) * t124 +
           (t124 * t596 + t324 * t514 + t50 * t538 + t1865 + t506) * t324 +
           (t124 * t538 + t324 * t577 + t486 * t514 + t50 * t596 + t1865 + t506) * t486 +
           (t124 * t504 + t324 * t504 + t486 * t504 + t50 * t504) * t921) *
              t921) *
             t921) *
            t921;
    return (t1 + t15 + t55) * t50 + (t1 + t15 + t97 + t131) * t124 + (t1 + t15 + t167 + t257 + t334) * t324 +
           (t1 + t15 + t345 + t366 + t420 + t499) * t486 + (t536 + t576 + t664 + t750 + t931) * t921;
}

}  // namespace mbnrg_A2_A2_deg6
