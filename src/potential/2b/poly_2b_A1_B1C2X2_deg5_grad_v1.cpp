
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

#include "poly_2b_A1_B1C2X2_deg5_v1.h"

/**
 * @file poly_2b_A1_B1C2X2_deg5_grad_v1.cpp
 * @brief Contains the implementation of the polynomials with gradients for symmetry A1_B1C2X2
 */

/**
 * @namespace mbnrg_A1_B1C2X2_deg5
 * @brief Encloses the structure of the polynomial holder for symmetry A1_B1C2X2
 */
namespace mbnrg_A1_B1C2X2_deg5 {

double poly_A1_B1C2X2_deg5_v1::eval(const double x[8], const double a[496], double g[8]) {
    const double t1 = a[2];
    const double t2 = a[14];
    const double t3 = a[19];
    const double t4 = x[7];
    const double t5 = t4 * a[359];
    const double t6 = a[84];
    const double t8 = (t5 + t6) * t4;
    const double t10 = (t3 + t8) * t4;
    const double t12 = (t2 + t10) * t4;
    const double t13 = a[12];
    const double t14 = a[34];
    const double t16 = t4 * a[350];
    const double t17 = a[182];
    const double t19 = (t16 + t17) * t4;
    const double t21 = (t14 + t19) * t4;
    const double t22 = a[62];
    const double t24 = t4 * a[232];
    const double t25 = a[127];
    const double t27 = (t24 + t25) * t4;
    const double t28 = a[407];
    const double t35 = x[6];
    const double t29 = t35 * t28;
    const double t30 = a[375];
    const double t31 = t4 * t30;
    const double t32 = a[128];
    const double t34 = (t29 + t31 + t32) * t35;
    const double t36 = (t22 + t27 + t34) * t35;
    const double t38 = (t13 + t21 + t36) * t35;
    const double t39 = a[54];
    const double t41 = t4 * a[246];
    const double t42 = a[150];
    const double t44 = (t41 + t42) * t4;
    const double t45 = a[273];
    const double t46 = t35 * t45;
    const double t47 = a[419];
    const double t48 = t4 * t47;
    const double t49 = a[176];
    const double t51 = (t46 + t48 + t49) * t35;
    const double t53 = (t39 + t44 + t51) * t35;
    const double t55 = t35 * a[249];
    const double t57 = (t55 + t48 + t49) * t35;
    const double t63 = x[5];
    const double t58 = t63 * t28;
    const double t60 = (t58 + t46 + t31 + t32) * t63;
    const double t62 = (t22 + t27 + t57 + t60) * t63;
    const double t64 = (t13 + t21 + t53 + t62) * t63;
    const double t65 = a[15];
    const double t66 = a[23];
    const double t68 = t4 * a[292];
    const double t69 = a[173];
    const double t71 = (t68 + t69) * t4;
    const double t73 = (t66 + t71) * t4;
    const double t74 = a[56];
    const double t76 = t4 * a[423];
    const double t77 = a[186];
    const double t79 = (t76 + t77) * t4;
    const double t80 = a[314];
    const double t81 = t35 * t80;
    const double t82 = a[204];
    const double t83 = t4 * t82;
    const double t84 = a[121];
    const double t86 = (t81 + t83 + t84) * t35;
    const double t88 = (t74 + t79 + t86) * t35;
    const double t89 = a[201];
    const double t90 = t35 * t89;
    const double t91 = a[368];
    const double t92 = t4 * t91;
    const double t93 = a[171];
    const double t95 = (t90 + t92 + t93) * t35;
    const double t96 = t63 * t80;
    const double t98 = (t96 + t90 + t83 + t84) * t63;
    const double t100 = (t74 + t79 + t95 + t98) * t63;
    const double t101 = a[33];
    const double t103 = t4 * a[348];
    const double t104 = a[130];
    const double t106 = (t103 + t104) * t4;
    const double t107 = a[217];
    const double t108 = t35 * t107;
    const double t109 = a[304];
    const double t110 = t4 * t109;
    const double t111 = a[183];
    const double t113 = (t108 + t110 + t111) * t35;
    const double t114 = t63 * t107;
    const double t115 = a[250];
    const double t116 = t35 * t115;
    const double t118 = (t114 + t116 + t110 + t111) * t63;
    const double t119 = a[390];
    const double t133 = x[4];
    const double t120 = t133 * t119;
    const double t121 = a[416];
    const double t122 = t63 * t121;
    const double t123 = t35 * t121;
    const double t124 = a[483];
    const double t125 = t124 * t4;
    const double t126 = a[125];
    const double t128 = (t120 + t122 + t123 + t125 + t126) * t133;
    const double t130 = (t101 + t106 + t113 + t118 + t128) * t133;
    const double t132 = (t65 + t73 + t88 + t100 + t130) * t133;
    const double t135 = a[9];
    const double t136 = a[42];
    const double t138 = t4 * a[323];
    const double t139 = a[156];
    const double t141 = (t138 + t139) * t4;
    const double t143 = (t136 + t141) * t4;
    const double t144 = a[65];
    const double t146 = t4 * a[289];
    const double t147 = a[139];
    const double t149 = (t146 + t147) * t4;
    const double t150 = a[261];
    const double t151 = t35 * t150;
    const double t152 = a[208];
    const double t153 = t4 * t152;
    const double t154 = a[168];
    const double t156 = (t151 + t153 + t154) * t35;
    const double t158 = (t144 + t149 + t156) * t35;
    const double t159 = a[282];
    const double t160 = t35 * t159;
    const double t161 = a[392];
    const double t162 = t4 * t161;
    const double t163 = a[178];
    const double t165 = (t160 + t162 + t163) * t35;
    const double t166 = t63 * t150;
    const double t168 = (t166 + t160 + t153 + t154) * t63;
    const double t170 = (t144 + t149 + t165 + t168) * t63;
    const double t171 = a[31];
    const double t173 = t4 * a[220];
    const double t174 = a[102];
    const double t176 = (t173 + t174) * t4;
    const double t177 = a[357];
    const double t178 = t35 * t177;
    const double t179 = a[227];
    const double t180 = t4 * t179;
    const double t181 = a[158];
    const double t183 = (t178 + t180 + t181) * t35;
    const double t184 = t63 * t177;
    const double t185 = a[281];
    const double t186 = t35 * t185;
    const double t188 = (t184 + t186 + t180 + t181) * t63;
    const double t189 = a[248];
    const double t190 = t189 * t133;
    const double t191 = a[456];
    const double t192 = t63 * t191;
    const double t193 = t35 * t191;
    const double t194 = a[276];
    const double t195 = t4 * t194;
    const double t196 = a[113];
    const double t198 = (t190 + t192 + t193 + t195 + t196) * t133;
    const double t200 = (t171 + t176 + t183 + t188 + t198) * t133;
    const double t202 = (t135 + t143 + t158 + t170 + t200) * t133;
    const double t204 = t133 * a[262];
    const double t205 = a[448];
    const double t206 = t63 * t205;
    const double t207 = t35 * t205;
    const double t208 = a[415];
    const double t209 = t4 * t208;
    const double t210 = a[117];
    const double t212 = (t204 + t206 + t207 + t209 + t210) * t133;
    const double t214 = (t171 + t176 + t183 + t188 + t212) * t133;
    const double t216 = (t204 + t192 + t193 + t195 + t196) * t133;
    const double t222 = x[3];
    const double t217 = t222 * t119;
    const double t219 = (t217 + t190 + t122 + t123 + t125 + t126) * t222;
    const double t221 = (t101 + t106 + t113 + t118 + t216 + t219) * t222;
    const double t223 = (t65 + t73 + t88 + t100 + t214 + t221) * t222;
    const double t226 = a[1];
    const double t227 = a[17];
    const double t228 = a[39];
    const double t230 = t4 * a[365];
    const double t231 = a[187];
    const double t233 = (t230 + t231) * t4;
    const double t235 = (t228 + t233) * t4;
    const double t237 = (t227 + t235) * t4;
    const double t238 = a[3];
    const double t239 = a[20];
    const double t241 = t4 * a[396];
    const double t242 = a[133];
    const double t244 = (t241 + t242) * t4;
    const double t246 = (t239 + t244) * t4;
    const double t247 = a[50];
    const double t249 = t4 * a[494];
    const double t250 = a[79];
    const double t252 = (t249 + t250) * t4;
    const double t253 = a[453];
    const double t254 = t35 * t253;
    const double t255 = a[216];
    const double t256 = t4 * t255;
    const double t257 = a[78];
    const double t259 = (t254 + t256 + t257) * t35;
    const double t261 = (t247 + t252 + t259) * t35;
    const double t263 = (t238 + t246 + t261) * t35;
    const double t264 = a[13];
    const double t265 = a[27];
    const double t267 = t4 * a[230];
    const double t268 = a[87];
    const double t270 = (t267 + t268) * t4;
    const double t272 = (t265 + t270) * t4;
    const double t273 = a[22];
    const double t275 = t4 * a[280];
    const double t276 = a[179];
    const double t278 = (t275 + t276) * t4;
    const double t279 = a[428];
    const double t280 = t35 * t279;
    const double t281 = a[490];
    const double t282 = t4 * t281;
    const double t283 = a[89];
    const double t285 = (t280 + t282 + t283) * t35;
    const double t287 = (t273 + t278 + t285) * t35;
    const double t288 = a[38];
    const double t290 = t4 * a[242];
    const double t291 = a[109];
    const double t293 = (t290 + t291) * t4;
    const double t295 = t35 * a[306];
    const double t296 = a[272];
    const double t297 = t4 * t296;
    const double t298 = a[123];
    const double t300 = (t295 + t297 + t298) * t35;
    const double t301 = a[450];
    const double t302 = t63 * t301;
    const double t303 = a[197];
    const double t304 = t35 * t303;
    const double t305 = a[438];
    const double t306 = t4 * t305;
    const double t307 = a[165];
    const double t309 = (t302 + t304 + t306 + t307) * t63;
    const double t311 = (t288 + t293 + t300 + t309) * t63;
    const double t313 = (t264 + t272 + t287 + t311) * t63;
    const double t314 = a[5];
    const double t315 = a[55];
    const double t317 = t4 * a[219];
    const double t318 = a[145];
    const double t320 = (t317 + t318) * t4;
    const double t322 = (t315 + t320) * t4;
    const double t323 = a[46];
    const double t325 = t4 * a[222];
    const double t326 = a[169];
    const double t328 = (t325 + t326) * t4;
    const double t329 = a[334];
    const double t330 = t35 * t329;
    const double t331 = a[203];
    const double t332 = t4 * t331;
    const double t333 = a[140];
    const double t335 = (t330 + t332 + t333) * t35;
    const double t337 = (t323 + t328 + t335) * t35;
    const double t338 = a[45];
    const double t340 = t4 * a[210];
    const double t341 = a[177];
    const double t343 = (t340 + t341) * t4;
    const double t344 = a[436];
    const double t345 = t35 * t344;
    const double t346 = a[400];
    const double t347 = t4 * t346;
    const double t348 = a[80];
    const double t350 = (t345 + t347 + t348) * t35;
    const double t351 = a[410];
    const double t352 = t63 * t351;
    const double t353 = a[331];
    const double t354 = t35 * t353;
    const double t355 = a[347];
    const double t356 = t4 * t355;
    const double t357 = a[196];
    const double t359 = (t352 + t354 + t356 + t357) * t63;
    const double t361 = (t338 + t343 + t350 + t359) * t63;
    const double t362 = a[57];
    const double t364 = t4 * a[366];
    const double t365 = a[115];
    const double t367 = (t364 + t365) * t4;
    const double t368 = a[377];
    const double t369 = t35 * t368;
    const double t370 = a[425];
    const double t371 = t4 * t370;
    const double t372 = a[166];
    const double t374 = (t369 + t371 + t372) * t35;
    const double t375 = a[279];
    const double t376 = t63 * t375;
    const double t377 = a[325];
    const double t378 = t35 * t377;
    const double t379 = a[299];
    const double t380 = t4 * t379;
    const double t381 = a[190];
    const double t383 = (t376 + t378 + t380 + t381) * t63;
    const double t384 = a[462];
    const double t385 = t133 * t384;
    const double t386 = a[266];
    const double t387 = t63 * t386;
    const double t388 = a[286];
    const double t389 = t35 * t388;
    const double t390 = a[259];
    const double t391 = t4 * t390;
    const double t392 = a[184];
    const double t394 = (t385 + t387 + t389 + t391 + t392) * t133;
    const double t396 = (t362 + t367 + t374 + t383 + t394) * t133;
    const double t398 = (t314 + t322 + t337 + t361 + t396) * t133;
    const double t399 = a[28];
    const double t401 = t4 * a[245];
    const double t402 = a[104];
    const double t404 = (t401 + t402) * t4;
    const double t405 = a[247];
    const double t406 = t35 * t405;
    const double t407 = a[412];
    const double t408 = t4 * t407;
    const double t409 = a[124];
    const double t411 = (t406 + t408 + t409) * t35;
    const double t412 = a[443];
    const double t413 = t63 * t412;
    const double t414 = a[381];
    const double t415 = t35 * t414;
    const double t416 = a[393];
    const double t417 = t4 * t416;
    const double t418 = a[98];
    const double t420 = (t413 + t415 + t417 + t418) * t63;
    const double t421 = a[327];
    const double t422 = t133 * t421;
    const double t423 = a[488];
    const double t424 = t63 * t423;
    const double t425 = a[360];
    const double t426 = t35 * t425;
    const double t427 = a[356];
    const double t428 = t4 * t427;
    const double t429 = a[172];
    const double t431 = (t422 + t424 + t426 + t428 + t429) * t133;
    const double t433 = (t399 + t404 + t411 + t420 + t431) * t133;
    const double t435 = t133 * a[312];
    const double t437 = (t435 + t424 + t426 + t428 + t429) * t133;
    const double t438 = t222 * t384;
    const double t440 = (t438 + t422 + t387 + t389 + t391 + t392) * t222;
    const double t442 = (t362 + t367 + t374 + t383 + t437 + t440) * t222;
    const double t444 = (t314 + t322 + t337 + t361 + t433 + t442) * t222;
    const double t445 = a[8];
    const double t446 = a[26];
    const double t448 = t4 * a[209];
    const double t449 = a[185];
    const double t451 = (t448 + t449) * t4;
    const double t453 = (t446 + t451) * t4;
    const double t454 = a[49];
    const double t456 = t4 * a[484];
    const double t457 = a[154];
    const double t459 = (t456 + t457) * t4;
    const double t460 = a[485];
    const double t461 = t35 * t460;
    const double t462 = a[316];
    const double t463 = t4 * t462;
    const double t464 = a[157];
    const double t466 = (t461 + t463 + t464) * t35;
    const double t468 = (t454 + t459 + t466) * t35;
    const double t469 = a[59];
    const double t471 = t4 * a[355];
    const double t472 = a[143];
    const double t474 = (t471 + t472) * t4;
    const double t475 = a[385];
    const double t476 = t35 * t475;
    const double t477 = a[472];
    const double t478 = t4 * t477;
    const double t479 = a[100];
    const double t481 = (t476 + t478 + t479) * t35;
    const double t482 = a[376];
    const double t483 = t63 * t482;
    const double t484 = a[373];
    const double t485 = t35 * t484;
    const double t486 = a[215];
    const double t487 = t4 * t486;
    const double t488 = a[67];
    const double t490 = (t483 + t485 + t487 + t488) * t63;
    const double t492 = (t469 + t474 + t481 + t490) * t63;
    const double t493 = a[60];
    const double t495 = t4 * a[302];
    const double t496 = a[85];
    const double t498 = (t495 + t496) * t4;
    const double t499 = a[205];
    const double t500 = t35 * t499;
    const double t501 = a[294];
    const double t502 = t4 * t501;
    const double t503 = a[119];
    const double t505 = (t500 + t502 + t503) * t35;
    const double t506 = a[402];
    const double t507 = t63 * t506;
    const double t508 = a[409];
    const double t509 = t35 * t508;
    const double t510 = a[344];
    const double t511 = t4 * t510;
    const double t512 = a[93];
    const double t514 = (t507 + t509 + t511 + t512) * t63;
    const double t515 = a[401];
    const double t516 = t133 * t515;
    const double t517 = a[237];
    const double t518 = t63 * t517;
    const double t519 = a[421];
    const double t520 = t35 * t519;
    const double t521 = a[482];
    const double t522 = t4 * t521;
    const double t523 = a[164];
    const double t525 = (t516 + t518 + t520 + t522 + t523) * t133;
    const double t527 = (t493 + t498 + t505 + t514 + t525) * t133;
    const double t528 = a[198];
    const double t529 = t133 * t528;
    const double t530 = a[337];
    const double t531 = t63 * t530;
    const double t532 = a[214];
    const double t533 = t35 * t532;
    const double t534 = a[445];
    const double t535 = t4 * t534;
    const double t536 = a[101];
    const double t538 = (t529 + t531 + t533 + t535 + t536) * t133;
    const double t539 = t222 * t515;
    const double t541 = (t539 + t529 + t518 + t520 + t522 + t523) * t222;
    const double t543 = (t493 + t498 + t505 + t514 + t538 + t541) * t222;
    const double t544 = a[29];
    const double t546 = t4 * a[447];
    const double t547 = a[170];
    const double t549 = (t546 + t547) * t4;
    const double t550 = a[271];
    const double t551 = t35 * t550;
    const double t552 = a[223];
    const double t553 = t4 * t552;
    const double t554 = a[137];
    const double t556 = (t551 + t553 + t554) * t35;
    const double t557 = a[211];
    const double t558 = t63 * t557;
    const double t559 = a[429];
    const double t560 = t35 * t559;
    const double t561 = a[431];
    const double t562 = t4 * t561;
    const double t563 = a[120];
    const double t565 = (t558 + t560 + t562 + t563) * t63;
    const double t566 = a[406];
    const double t567 = t133 * t566;
    const double t568 = a[463];
    const double t569 = t63 * t568;
    const double t570 = a[341];
    const double t571 = t35 * t570;
    const double t572 = a[466];
    const double t573 = t4 * t572;
    const double t574 = a[95];
    const double t576 = (t567 + t569 + t571 + t573 + t574) * t133;
    const double t577 = t222 * t566;
    const double t578 = a[486];
    const double t579 = t133 * t578;
    const double t581 = (t577 + t579 + t569 + t571 + t573 + t574) * t222;
    const double t582 = a[233];
    const double t596 = x[2];
    const double t583 = t596 * t582;
    const double t584 = a[267];
    const double t585 = t222 * t584;
    const double t586 = t133 * t584;
    const double t587 = a[455];
    const double t588 = t63 * t587;
    const double t589 = a[310];
    const double t590 = t35 * t589;
    const double t591 = a[458];
    const double t592 = t591 * t4;
    const double t593 = a[161];
    const double t595 = (t583 + t585 + t586 + t588 + t590 + t592 + t593) * t596;
    const double t597 = (t544 + t549 + t556 + t565 + t576 + t581 + t595) * t596;
    const double t599 = (t445 + t453 + t468 + t492 + t527 + t543 + t597) * t596;
    const double t602 = t35 * t301;
    const double t604 = (t602 + t306 + t307) * t35;
    const double t606 = (t288 + t293 + t604) * t35;
    const double t608 = (t264 + t272 + t606) * t35;
    const double t610 = (t304 + t297 + t298) * t35;
    const double t612 = (t273 + t278 + t610) * t35;
    const double t614 = (t295 + t282 + t283) * t35;
    const double t615 = t63 * t253;
    const double t617 = (t615 + t280 + t256 + t257) * t63;
    const double t619 = (t247 + t252 + t614 + t617) * t63;
    const double t621 = (t238 + t246 + t612 + t619) * t63;
    const double t622 = t35 * t351;
    const double t624 = (t622 + t356 + t357) * t35;
    const double t626 = (t338 + t343 + t624) * t35;
    const double t628 = (t354 + t347 + t348) * t35;
    const double t629 = t63 * t329;
    const double t631 = (t629 + t345 + t332 + t333) * t63;
    const double t633 = (t323 + t328 + t628 + t631) * t63;
    const double t634 = t35 * t375;
    const double t636 = (t634 + t380 + t381) * t35;
    const double t637 = t63 * t368;
    const double t639 = (t637 + t378 + t371 + t372) * t63;
    const double t640 = t63 * t388;
    const double t641 = t35 * t386;
    const double t643 = (t385 + t640 + t641 + t391 + t392) * t133;
    const double t645 = (t362 + t367 + t636 + t639 + t643) * t133;
    const double t647 = (t314 + t322 + t626 + t633 + t645) * t133;
    const double t648 = t35 * t412;
    const double t650 = (t648 + t417 + t418) * t35;
    const double t651 = t63 * t405;
    const double t653 = (t651 + t415 + t408 + t409) * t63;
    const double t654 = t63 * t425;
    const double t655 = t35 * t423;
    const double t657 = (t422 + t654 + t655 + t428 + t429) * t133;
    const double t659 = (t399 + t404 + t650 + t653 + t657) * t133;
    const double t661 = (t435 + t654 + t655 + t428 + t429) * t133;
    const double t663 = (t438 + t422 + t640 + t641 + t391 + t392) * t222;
    const double t665 = (t362 + t367 + t636 + t639 + t661 + t663) * t222;
    const double t667 = (t314 + t322 + t626 + t633 + t659 + t665) * t222;
    const double t668 = a[6];
    const double t669 = a[51];
    const double t671 = t4 * a[362];
    const double t672 = a[151];
    const double t674 = (t671 + t672) * t4;
    const double t676 = (t669 + t674) * t4;
    const double t677 = a[66];
    const double t679 = t4 * a[315];
    const double t680 = a[73];
    const double t682 = (t679 + t680) * t4;
    const double t683 = a[293];
    const double t684 = t35 * t683;
    const double t685 = a[389];
    const double t686 = t4 * t685;
    const double t687 = a[175];
    const double t689 = (t684 + t686 + t687) * t35;
    const double t691 = (t677 + t682 + t689) * t35;
    const double t692 = a[351];
    const double t693 = t35 * t692;
    const double t694 = a[336];
    const double t695 = t4 * t694;
    const double t696 = a[194];
    const double t698 = (t693 + t695 + t696) * t35;
    const double t699 = t63 * t683;
    const double t701 = (t699 + t693 + t686 + t687) * t63;
    const double t703 = (t677 + t682 + t698 + t701) * t63;
    const double t704 = a[41];
    const double t706 = t4 * a[397];
    const double t707 = a[83];
    const double t709 = (t706 + t707) * t4;
    const double t710 = a[411];
    const double t711 = t35 * t710;
    const double t712 = a[224];
    const double t713 = t4 * t712;
    const double t714 = a[195];
    const double t716 = (t711 + t713 + t714) * t35;
    const double t717 = t63 * t710;
    const double t718 = a[343];
    const double t719 = t35 * t718;
    const double t721 = (t717 + t719 + t713 + t714) * t63;
    const double t722 = a[342];
    const double t723 = t133 * t722;
    const double t724 = a[226];
    const double t725 = t63 * t724;
    const double t726 = t35 * t724;
    const double t727 = a[471];
    const double t728 = t727 * t4;
    const double t729 = a[118];
    const double t731 = (t723 + t725 + t726 + t728 + t729) * t133;
    const double t733 = (t704 + t709 + t716 + t721 + t731) * t133;
    const double t734 = a[465];
    const double t735 = t734 * t133;
    const double t736 = a[420];
    const double t737 = t63 * t736;
    const double t738 = t35 * t736;
    const double t739 = a[426];
    const double t740 = t739 * t4;
    const double t741 = a[162];
    const double t743 = (t735 + t737 + t738 + t740 + t741) * t133;
    const double t744 = t222 * t722;
    const double t746 = (t744 + t735 + t725 + t726 + t728 + t729) * t222;
    const double t748 = (t704 + t709 + t716 + t721 + t743 + t746) * t222;
    const double t749 = a[24];
    const double t751 = t4 * a[340];
    const double t752 = a[108];
    const double t754 = (t751 + t752) * t4;
    const double t755 = a[452];
    const double t756 = t35 * t755;
    const double t757 = a[417];
    const double t758 = t4 * t757;
    const double t759 = a[86];
    const double t761 = (t756 + t758 + t759) * t35;
    const double t762 = a[405];
    const double t763 = t63 * t762;
    const double t764 = a[251];
    const double t765 = t35 * t764;
    const double t766 = a[330];
    const double t767 = t4 * t766;
    const double t768 = a[146];
    const double t770 = (t763 + t765 + t767 + t768) * t63;
    const double t771 = a[378];
    const double t772 = t133 * t771;
    const double t773 = a[200];
    const double t774 = t63 * t773;
    const double t775 = a[369];
    const double t776 = t35 * t775;
    const double t777 = a[446];
    const double t778 = t4 * t777;
    const double t779 = a[188];
    const double t781 = (t772 + t774 + t776 + t778 + t779) * t133;
    const double t782 = t222 * t771;
    const double t783 = a[332];
    const double t784 = t133 * t783;
    const double t786 = (t782 + t784 + t774 + t776 + t778 + t779) * t222;
    const double t787 = a[481];
    const double t788 = t787 * t596;
    const double t789 = a[358];
    const double t790 = t222 * t789;
    const double t791 = t133 * t789;
    const double t792 = a[457];
    const double t793 = t792 * t63;
    const double t794 = a[491];
    const double t795 = t794 * t35;
    const double t796 = a[313];
    const double t797 = t796 * t4;
    const double t798 = a[72];
    const double t800 = (t788 + t790 + t791 + t793 + t795 + t797 + t798) * t596;
    const double t802 = (t749 + t754 + t761 + t770 + t781 + t786 + t800) * t596;
    const double t804 = (t668 + t676 + t691 + t703 + t733 + t748 + t802) * t596;
    const double t805 = t35 * t482;
    const double t807 = (t805 + t487 + t488) * t35;
    const double t809 = (t469 + t474 + t807) * t35;
    const double t811 = (t485 + t478 + t479) * t35;
    const double t812 = t63 * t460;
    const double t814 = (t812 + t476 + t463 + t464) * t63;
    const double t816 = (t454 + t459 + t811 + t814) * t63;
    const double t817 = t35 * t506;
    const double t819 = (t817 + t511 + t512) * t35;
    const double t820 = t63 * t499;
    const double t822 = (t820 + t509 + t502 + t503) * t63;
    const double t823 = t63 * t519;
    const double t824 = t35 * t517;
    const double t826 = (t516 + t823 + t824 + t522 + t523) * t133;
    const double t828 = (t493 + t498 + t819 + t822 + t826) * t133;
    const double t829 = t63 * t532;
    const double t830 = t35 * t530;
    const double t832 = (t529 + t829 + t830 + t535 + t536) * t133;
    const double t834 = (t539 + t529 + t823 + t824 + t522 + t523) * t222;
    const double t836 = (t493 + t498 + t819 + t822 + t832 + t834) * t222;
    const double t837 = t35 * t762;
    const double t839 = (t837 + t767 + t768) * t35;
    const double t840 = t63 * t755;
    const double t842 = (t840 + t765 + t758 + t759) * t63;
    const double t843 = t63 * t775;
    const double t844 = t35 * t773;
    const double t846 = (t772 + t843 + t844 + t778 + t779) * t133;
    const double t848 = (t782 + t784 + t843 + t844 + t778 + t779) * t222;
    const double t850 = t596 * a[329];
    const double t851 = a[258];
    const double t852 = t222 * t851;
    const double t853 = t851 * t133;
    const double t854 = a[221];
    const double t855 = t63 * t854;
    const double t856 = t35 * t854;
    const double t857 = a[326];
    const double t858 = t857 * t4;
    const double t859 = a[180];
    const double t861 = (t850 + t852 + t853 + t855 + t856 + t858 + t859) * t596;
    const double t863 = (t749 + t754 + t839 + t842 + t846 + t848 + t861) * t596;
    const double t864 = t35 * t557;
    const double t866 = (t864 + t562 + t563) * t35;
    const double t867 = t63 * t550;
    const double t869 = (t867 + t560 + t553 + t554) * t63;
    const double t870 = t63 * t570;
    const double t871 = t35 * t568;
    const double t873 = (t567 + t870 + t871 + t573 + t574) * t133;
    const double t875 = (t577 + t579 + t870 + t871 + t573 + t574) * t222;
    const double t876 = t63 * t794;
    const double t877 = t35 * t792;
    const double t879 = (t850 + t790 + t791 + t876 + t877 + t797 + t798) * t596;
    const double t883 = x[1];
    const double t880 = t883 * t582;
    const double t881 = t589 * t63;
    const double t882 = t587 * t35;
    const double t884 = (t880 + t788 + t585 + t586 + t881 + t882 + t592 + t593) * t883;
    const double t886 = (t544 + t549 + t866 + t869 + t873 + t875 + t879 + t884) * t883;
    const double t888 = (t445 + t453 + t809 + t816 + t828 + t836 + t863 + t886) * t883;
    const double t891 = a[0];
    const double t892 = a[10];
    const double t893 = a[48];
    const double t895 = t4 * a[213];
    const double t896 = a[68];
    const double t898 = (t895 + t896) * t4;
    const double t900 = (t893 + t898) * t4;
    const double t902 = (t892 + t900) * t4;
    const double t903 = a[4];
    const double t904 = a[61];
    const double t906 = t4 * a[374];
    const double t907 = a[94];
    const double t909 = (t906 + t907) * t4;
    const double t911 = (t904 + t909) * t4;
    const double t912 = a[35];
    const double t914 = t4 * a[319];
    const double t915 = a[141];
    const double t917 = (t914 + t915) * t4;
    const double t918 = a[442];
    const double t919 = t35 * t918;
    const double t920 = a[353];
    const double t921 = t4 * t920;
    const double t922 = a[189];
    const double t924 = (t919 + t921 + t922) * t35;
    const double t926 = (t912 + t917 + t924) * t35;
    const double t928 = (t903 + t911 + t926) * t35;
    const double t929 = a[32];
    const double t931 = t4 * a[275];
    const double t932 = a[81];
    const double t934 = (t931 + t932) * t4;
    const double t935 = a[384];
    const double t936 = t35 * t935;
    const double t937 = a[243];
    const double t938 = t4 * t937;
    const double t939 = a[147];
    const double t941 = (t936 + t938 + t939) * t35;
    const double t943 = (t929 + t934 + t941) * t35;
    const double t945 = t35 * a[301];
    const double t947 = (t945 + t938 + t939) * t35;
    const double t948 = t63 * t918;
    const double t950 = (t948 + t936 + t921 + t922) * t63;
    const double t952 = (t912 + t917 + t947 + t950) * t63;
    const double t954 = (t903 + t911 + t943 + t952) * t63;
    const double t955 = a[7];
    const double t956 = a[21];
    const double t958 = t4 * a[404];
    const double t959 = a[76];
    const double t961 = (t958 + t959) * t4;
    const double t963 = (t956 + t961) * t4;
    const double t964 = a[30];
    const double t966 = t4 * a[413];
    const double t967 = a[181];
    const double t969 = (t966 + t967) * t4;
    const double t970 = a[270];
    const double t971 = t35 * t970;
    const double t972 = a[268];
    const double t973 = t4 * t972;
    const double t974 = a[193];
    const double t976 = (t971 + t973 + t974) * t35;
    const double t978 = (t964 + t969 + t976) * t35;
    const double t979 = a[444];
    const double t980 = t35 * t979;
    const double t981 = a[335];
    const double t982 = t4 * t981;
    const double t983 = a[75];
    const double t985 = (t980 + t982 + t983) * t35;
    const double t986 = t63 * t970;
    const double t988 = (t986 + t980 + t973 + t974) * t63;
    const double t990 = (t964 + t969 + t985 + t988) * t63;
    const double t991 = a[37];
    const double t993 = t4 * a[399];
    const double t994 = a[163];
    const double t996 = (t993 + t994) * t4;
    const double t997 = a[467];
    const double t998 = t35 * t997;
    const double t999 = a[307];
    const double t1000 = t4 * t999;
    const double t1001 = a[138];
    const double t1003 = (t998 + t1000 + t1001) * t35;
    const double t1004 = t63 * t997;
    const double t1005 = a[333];
    const double t1006 = t35 * t1005;
    const double t1008 = (t1004 + t1006 + t1000 + t1001) * t63;
    const double t1009 = a[206];
    const double t1010 = t133 * t1009;
    const double t1011 = a[422];
    const double t1012 = t63 * t1011;
    const double t1013 = t35 * t1011;
    const double t1014 = a[454];
    const double t1015 = t1014 * t4;
    const double t1016 = a[131];
    const double t1018 = (t1010 + t1012 + t1013 + t1015 + t1016) * t133;
    const double t1020 = (t991 + t996 + t1003 + t1008 + t1018) * t133;
    const double t1022 = (t955 + t963 + t978 + t990 + t1020) * t133;
    const double t1023 = a[44];
    const double t1025 = t4 * a[459];
    const double t1026 = a[105];
    const double t1028 = (t1025 + t1026) * t4;
    const double t1029 = a[388];
    const double t1030 = t35 * t1029;
    const double t1031 = a[433];
    const double t1032 = t4 * t1031;
    const double t1033 = a[92];
    const double t1035 = (t1030 + t1032 + t1033) * t35;
    const double t1036 = t63 * t1029;
    const double t1037 = a[318];
    const double t1038 = t35 * t1037;
    const double t1040 = (t1036 + t1038 + t1032 + t1033) * t63;
    const double t1041 = a[395];
    const double t1042 = t1041 * t133;
    const double t1043 = a[394];
    const double t1044 = t63 * t1043;
    const double t1045 = t35 * t1043;
    const double t1046 = a[469];
    const double t1047 = t1046 * t4;
    const double t1048 = a[111];
    const double t1050 = (t1042 + t1044 + t1045 + t1047 + t1048) * t133;
    const double t1052 = (t1023 + t1028 + t1035 + t1040 + t1050) * t133;
    const double t1054 = t133 * a[256];
    const double t1056 = (t1054 + t1044 + t1045 + t1047 + t1048) * t133;
    const double t1057 = t222 * t1009;
    const double t1059 = (t1057 + t1042 + t1012 + t1013 + t1015 + t1016) * t222;
    const double t1061 = (t991 + t996 + t1003 + t1008 + t1056 + t1059) * t222;
    const double t1063 = (t955 + t963 + t978 + t990 + t1052 + t1061) * t222;
    const double t1064 = a[16];
    const double t1065 = a[43];
    const double t1067 = t4 * a[345];
    const double t1068 = a[74];
    const double t1070 = (t1067 + t1068) * t4;
    const double t1072 = (t1065 + t1070) * t4;
    const double t1073 = a[53];
    const double t1075 = t4 * a[487];
    const double t1076 = a[144];
    const double t1078 = (t1075 + t1076) * t4;
    const double t1079 = a[234];
    const double t1080 = t35 * t1079;
    const double t1081 = a[263];
    const double t1082 = t4 * t1081;
    const double t1083 = a[97];
    const double t1085 = (t1080 + t1082 + t1083) * t35;
    const double t1087 = (t1073 + t1078 + t1085) * t35;
    const double t1088 = a[64];
    const double t1090 = t4 * a[244];
    const double t1091 = a[149];
    const double t1093 = (t1090 + t1091) * t4;
    const double t1094 = a[241];
    const double t1095 = t35 * t1094;
    const double t1096 = a[321];
    const double t1097 = t4 * t1096;
    const double t1098 = a[135];
    const double t1100 = (t1095 + t1097 + t1098) * t35;
    const double t1101 = a[274];
    const double t1102 = t63 * t1101;
    const double t1103 = a[240];
    const double t1104 = t35 * t1103;
    const double t1105 = a[391];
    const double t1106 = t4 * t1105;
    const double t1107 = a[132];
    const double t1109 = (t1102 + t1104 + t1106 + t1107) * t63;
    const double t1111 = (t1088 + t1093 + t1100 + t1109) * t63;
    const double t1112 = a[36];
    const double t1114 = t4 * a[324];
    const double t1115 = a[77];
    const double t1117 = (t1114 + t1115) * t4;
    const double t1118 = a[476];
    const double t1119 = t35 * t1118;
    const double t1120 = a[328];
    const double t1121 = t4 * t1120;
    const double t1122 = a[99];
    const double t1124 = (t1119 + t1121 + t1122) * t35;
    const double t1125 = a[440];
    const double t1126 = t63 * t1125;
    const double t1127 = a[277];
    const double t1128 = t35 * t1127;
    const double t1129 = a[403];
    const double t1130 = t4 * t1129;
    const double t1131 = a[122];
    const double t1133 = (t1126 + t1128 + t1130 + t1131) * t63;
    const double t1134 = a[474];
    const double t1135 = t133 * t1134;
    const double t1136 = a[207];
    const double t1137 = t63 * t1136;
    const double t1138 = a[430];
    const double t1139 = t35 * t1138;
    const double t1140 = a[298];
    const double t1141 = t4 * t1140;
    const double t1142 = a[96];
    const double t1144 = (t1135 + t1137 + t1139 + t1141 + t1142) * t133;
    const double t1146 = (t1112 + t1117 + t1124 + t1133 + t1144) * t133;
    const double t1147 = a[460];
    const double t1148 = t133 * t1147;
    const double t1149 = a[288];
    const double t1150 = t63 * t1149;
    const double t1151 = a[265];
    const double t1152 = t35 * t1151;
    const double t1153 = a[297];
    const double t1154 = t4 * t1153;
    const double t1155 = a[142];
    const double t1157 = (t1148 + t1150 + t1152 + t1154 + t1155) * t133;
    const double t1158 = t222 * t1134;
    const double t1160 = (t1158 + t1148 + t1137 + t1139 + t1141 + t1142) * t222;
    const double t1162 = (t1112 + t1117 + t1124 + t1133 + t1157 + t1160) * t222;
    const double t1163 = a[40];
    const double t1165 = t4 * a[252];
    const double t1166 = a[191];
    const double t1168 = (t1165 + t1166) * t4;
    const double t1169 = a[479];
    const double t1170 = t35 * t1169;
    const double t1171 = a[269];
    const double t1172 = t4 * t1171;
    const double t1173 = a[174];
    const double t1175 = (t1170 + t1172 + t1173) * t35;
    const double t1176 = a[386];
    const double t1177 = t63 * t1176;
    const double t1178 = a[382];
    const double t1179 = t35 * t1178;
    const double t1180 = a[202];
    const double t1181 = t4 * t1180;
    const double t1182 = a[90];
    const double t1184 = (t1177 + t1179 + t1181 + t1182) * t63;
    const double t1185 = a[477];
    const double t1186 = t133 * t1185;
    const double t1187 = a[228];
    const double t1188 = t63 * t1187;
    const double t1189 = a[434];
    const double t1190 = t35 * t1189;
    const double t1191 = a[354];
    const double t1192 = t4 * t1191;
    const double t1193 = a[153];
    const double t1195 = (t1186 + t1188 + t1190 + t1192 + t1193) * t133;
    const double t1196 = t222 * t1185;
    const double t1197 = a[451];
    const double t1198 = t133 * t1197;
    const double t1200 = (t1196 + t1198 + t1188 + t1190 + t1192 + t1193) * t222;
    const double t1201 = a[432];
    const double t1202 = t596 * t1201;
    const double t1203 = a[470];
    const double t1204 = t222 * t1203;
    const double t1205 = t133 * t1203;
    const double t1206 = a[493];
    const double t1207 = t1206 * t63;
    const double t1208 = a[238];
    const double t1209 = t1208 * t35;
    const double t1210 = a[320];
    const double t1211 = t1210 * t4;
    const double t1212 = a[114];
    const double t1214 = (t1202 + t1204 + t1205 + t1207 + t1209 + t1211 + t1212) * t596;
    const double t1216 = (t1163 + t1168 + t1175 + t1184 + t1195 + t1200 + t1214) * t596;
    const double t1218 = (t1064 + t1072 + t1087 + t1111 + t1146 + t1162 + t1216) * t596;
    const double t1219 = t35 * t1101;
    const double t1221 = (t1219 + t1106 + t1107) * t35;
    const double t1223 = (t1088 + t1093 + t1221) * t35;
    const double t1225 = (t1104 + t1097 + t1098) * t35;
    const double t1226 = t63 * t1079;
    const double t1228 = (t1226 + t1095 + t1082 + t1083) * t63;
    const double t1230 = (t1073 + t1078 + t1225 + t1228) * t63;
    const double t1231 = t35 * t1125;
    const double t1233 = (t1231 + t1130 + t1131) * t35;
    const double t1234 = t63 * t1118;
    const double t1236 = (t1234 + t1128 + t1121 + t1122) * t63;
    const double t1237 = t63 * t1138;
    const double t1238 = t35 * t1136;
    const double t1240 = (t1135 + t1237 + t1238 + t1141 + t1142) * t133;
    const double t1242 = (t1112 + t1117 + t1233 + t1236 + t1240) * t133;
    const double t1243 = t63 * t1151;
    const double t1244 = t35 * t1149;
    const double t1246 = (t1148 + t1243 + t1244 + t1154 + t1155) * t133;
    const double t1248 = (t1158 + t1148 + t1237 + t1238 + t1141 + t1142) * t222;
    const double t1250 = (t1112 + t1117 + t1233 + t1236 + t1246 + t1248) * t222;
    const double t1251 = a[52];
    const double t1253 = t4 * a[300];
    const double t1254 = a[71];
    const double t1256 = (t1253 + t1254) * t4;
    const double t1257 = a[278];
    const double t1258 = t35 * t1257;
    const double t1259 = a[370];
    const double t1260 = t4 * t1259;
    const double t1261 = a[116];
    const double t1263 = (t1258 + t1260 + t1261) * t35;
    const double t1264 = t63 * t1257;
    const double t1265 = a[418];
    const double t1266 = t35 * t1265;
    const double t1268 = (t1264 + t1266 + t1260 + t1261) * t63;
    const double t1269 = a[424];
    const double t1270 = t133 * t1269;
    const double t1271 = a[480];
    const double t1272 = t63 * t1271;
    const double t1273 = t35 * t1271;
    const double t1274 = a[489];
    const double t1275 = t1274 * t4;
    const double t1276 = a[70];
    const double t1278 = (t1270 + t1272 + t1273 + t1275 + t1276) * t133;
    const double t1279 = t222 * t1269;
    const double t1280 = a[287];
    const double t1281 = t1280 * t133;
    const double t1283 = (t1279 + t1281 + t1272 + t1273 + t1275 + t1276) * t222;
    const double t1284 = a[414];
    const double t1285 = t1284 * t596;
    const double t1286 = a[468];
    const double t1287 = t222 * t1286;
    const double t1288 = t133 * t1286;
    const double t1289 = a[254];
    const double t1290 = t1289 * t63;
    const double t1291 = a[379];
    const double t1292 = t1291 * t35;
    const double t1293 = a[346];
    const double t1294 = t1293 * t4;
    const double t1295 = a[167];
    const double t1297 = (t1285 + t1287 + t1288 + t1290 + t1292 + t1294 + t1295) * t596;
    const double t1299 = (t1251 + t1256 + t1263 + t1268 + t1278 + t1283 + t1297) * t596;
    const double t1300 = t35 * t1176;
    const double t1302 = (t1300 + t1181 + t1182) * t35;
    const double t1303 = t63 * t1169;
    const double t1305 = (t1303 + t1179 + t1172 + t1173) * t63;
    const double t1306 = t63 * t1189;
    const double t1307 = t35 * t1187;
    const double t1309 = (t1186 + t1306 + t1307 + t1192 + t1193) * t133;
    const double t1311 = (t1196 + t1198 + t1306 + t1307 + t1192 + t1193) * t222;
    const double t1313 = t596 * a[495];
    const double t1314 = t1291 * t63;
    const double t1315 = t1289 * t35;
    const double t1317 = (t1313 + t1287 + t1288 + t1314 + t1315 + t1294 + t1295) * t596;
    const double t1318 = t883 * t1201;
    const double t1319 = t1208 * t63;
    const double t1320 = t1206 * t35;
    const double t1322 = (t1318 + t1285 + t1204 + t1205 + t1319 + t1320 + t1211 + t1212) * t883;
    const double t1324 = (t1163 + t1168 + t1302 + t1305 + t1309 + t1311 + t1317 + t1322) * t883;
    const double t1326 = (t1064 + t1072 + t1223 + t1230 + t1242 + t1250 + t1299 + t1324) * t883;
    const double t1327 = a[11];
    const double t1328 = a[58];
    const double t1330 = t4 * a[464];
    const double t1331 = a[160];
    const double t1333 = (t1330 + t1331) * t4;
    const double t1335 = (t1328 + t1333) * t4;
    const double t1336 = a[18];
    const double t1338 = t4 * a[199];
    const double t1339 = a[192];
    const double t1341 = (t1338 + t1339) * t4;
    const double t1342 = a[367];
    const double t1343 = t35 * t1342;
    const double t1344 = a[285];
    const double t1345 = t4 * t1344;
    const double t1346 = a[155];
    const double t1348 = (t1343 + t1345 + t1346) * t35;
    const double t1350 = (t1336 + t1341 + t1348) * t35;
    const double t1351 = a[408];
    const double t1352 = t35 * t1351;
    const double t1353 = a[322];
    const double t1354 = t4 * t1353;
    const double t1355 = a[106];
    const double t1357 = (t1352 + t1354 + t1355) * t35;
    const double t1358 = t63 * t1342;
    const double t1360 = (t1358 + t1352 + t1345 + t1346) * t63;
    const double t1362 = (t1336 + t1341 + t1357 + t1360) * t63;
    const double t1363 = a[47];
    const double t1365 = t4 * a[473];
    const double t1366 = a[126];
    const double t1368 = (t1365 + t1366) * t4;
    const double t1369 = a[441];
    const double t1370 = t35 * t1369;
    const double t1371 = a[235];
    const double t1372 = t4 * t1371;
    const double t1373 = a[103];
    const double t1375 = (t1370 + t1372 + t1373) * t35;
    const double t1376 = t63 * t1369;
    const double t1377 = a[257];
    const double t1378 = t35 * t1377;
    const double t1380 = (t1376 + t1378 + t1372 + t1373) * t63;
    const double t1381 = a[305];
    const double t1382 = t133 * t1381;
    const double t1383 = a[361];
    const double t1384 = t63 * t1383;
    const double t1385 = t35 * t1383;
    const double t1386 = a[398];
    const double t1387 = t1386 * t4;
    const double t1388 = a[107];
    const double t1390 = (t1382 + t1384 + t1385 + t1387 + t1388) * t133;
    const double t1392 = (t1363 + t1368 + t1375 + t1380 + t1390) * t133;
    const double t1393 = a[492];
    const double t1394 = t1393 * t133;
    const double t1395 = a[349];
    const double t1396 = t63 * t1395;
    const double t1397 = t35 * t1395;
    const double t1398 = a[383];
    const double t1399 = t1398 * t4;
    const double t1400 = a[136];
    const double t1402 = (t1394 + t1396 + t1397 + t1399 + t1400) * t133;
    const double t1403 = t222 * t1381;
    const double t1405 = (t1403 + t1394 + t1384 + t1385 + t1387 + t1388) * t222;
    const double t1407 = (t1363 + t1368 + t1375 + t1380 + t1402 + t1405) * t222;
    const double t1408 = a[63];
    const double t1410 = t4 * a[253];
    const double t1411 = a[91];
    const double t1413 = (t1410 + t1411) * t4;
    const double t1414 = a[352];
    const double t1415 = t35 * t1414;
    const double t1416 = a[380];
    const double t1417 = t4 * t1416;
    const double t1418 = a[129];
    const double t1420 = (t1415 + t1417 + t1418) * t35;
    const double t1421 = a[303];
    const double t1422 = t63 * t1421;
    const double t1423 = a[435];
    const double t1424 = t35 * t1423;
    const double t1425 = a[364];
    const double t1426 = t4 * t1425;
    const double t1427 = a[69];
    const double t1429 = (t1422 + t1424 + t1426 + t1427) * t63;
    const double t1430 = a[475];
    const double t1431 = t133 * t1430;
    const double t1432 = a[212];
    const double t1433 = t63 * t1432;
    const double t1434 = a[284];
    const double t1435 = t35 * t1434;
    const double t1436 = a[229];
    const double t1437 = t4 * t1436;
    const double t1438 = a[159];
    const double t1440 = (t1431 + t1433 + t1435 + t1437 + t1438) * t133;
    const double t1441 = t222 * t1430;
    const double t1442 = a[295];
    const double t1443 = t133 * t1442;
    const double t1445 = (t1441 + t1443 + t1433 + t1435 + t1437 + t1438) * t222;
    const double t1446 = a[255];
    const double t1447 = t596 * t1446;
    const double t1448 = a[308];
    const double t1449 = t222 * t1448;
    const double t1450 = t133 * t1448;
    const double t1451 = a[311];
    const double t1452 = t1451 * t63;
    const double t1453 = a[239];
    const double t1454 = t1453 * t35;
    const double t1455 = a[231];
    const double t1456 = t1455 * t4;
    const double t1457 = a[152];
    const double t1459 = (t1447 + t1449 + t1450 + t1452 + t1454 + t1456 + t1457) * t596;
    const double t1461 = (t1408 + t1413 + t1420 + t1429 + t1440 + t1445 + t1459) * t596;
    const double t1462 = t35 * t1421;
    const double t1464 = (t1462 + t1426 + t1427) * t35;
    const double t1465 = t63 * t1414;
    const double t1467 = (t1465 + t1424 + t1417 + t1418) * t63;
    const double t1468 = t63 * t1434;
    const double t1469 = t35 * t1432;
    const double t1471 = (t1431 + t1468 + t1469 + t1437 + t1438) * t133;
    const double t1473 = (t1441 + t1443 + t1468 + t1469 + t1437 + t1438) * t222;
    const double t1474 = a[260];
    const double t1475 = t1474 * t596;
    const double t1476 = a[291];
    const double t1477 = t222 * t1476;
    const double t1478 = t1476 * t133;
    const double t1479 = a[439];
    const double t1480 = t63 * t1479;
    const double t1481 = t35 * t1479;
    const double t1482 = a[387];
    const double t1483 = t1482 * t4;
    const double t1484 = a[110];
    const double t1486 = (t1475 + t1477 + t1478 + t1480 + t1481 + t1483 + t1484) * t596;
    const double t1487 = t883 * t1446;
    const double t1488 = t1453 * t63;
    const double t1489 = t1451 * t35;
    const double t1491 = (t1487 + t1475 + t1449 + t1450 + t1488 + t1489 + t1456 + t1457) * t883;
    const double t1493 = (t1408 + t1413 + t1464 + t1467 + t1471 + t1473 + t1486 + t1491) * t883;
    const double t1494 = a[25];
    const double t1496 = t4 * a[309];
    const double t1497 = a[148];
    const double t1499 = (t1496 + t1497) * t4;
    const double t1500 = a[236];
    const double t1501 = t35 * t1500;
    const double t1502 = a[296];
    const double t1503 = t4 * t1502;
    const double t1504 = a[112];
    const double t1506 = (t1501 + t1503 + t1504) * t35;
    const double t1507 = t63 * t1500;
    const double t1508 = a[461];
    const double t1509 = t35 * t1508;
    const double t1511 = (t1507 + t1509 + t1503 + t1504) * t63;
    const double t1512 = a[218];
    const double t1513 = t133 * t1512;
    const double t1514 = a[371];
    const double t1515 = t63 * t1514;
    const double t1516 = t35 * t1514;
    const double t1517 = a[437];
    const double t1518 = t1517 * t4;
    const double t1519 = a[82];
    const double t1521 = (t1513 + t1515 + t1516 + t1518 + t1519) * t133;
    const double t1522 = t222 * t1512;
    const double t1523 = a[264];
    const double t1524 = t1523 * t133;
    const double t1526 = (t1522 + t1524 + t1515 + t1516 + t1518 + t1519) * t222;
    const double t1527 = a[363];
    const double t1528 = t596 * t1527;
    const double t1529 = a[283];
    const double t1530 = t222 * t1529;
    const double t1531 = t133 * t1529;
    const double t1532 = a[449];
    const double t1533 = t1532 * t63;
    const double t1534 = a[339];
    const double t1535 = t1534 * t35;
    const double t1536 = a[478];
    const double t1537 = t1536 * t4;
    const double t1538 = a[88];
    const double t1540 = (t1528 + t1530 + t1531 + t1533 + t1535 + t1537 + t1538) * t596;
    const double t1541 = t883 * t1527;
    const double t1542 = a[317];
    const double t1543 = t1542 * t596;
    const double t1544 = t1534 * t63;
    const double t1545 = t1532 * t35;
    const double t1547 = (t1541 + t1543 + t1530 + t1531 + t1544 + t1545 + t1537 + t1538) * t883;
    const double t1546 = x[0];
    const double t1549 = t1546 * a[225];
    const double t1550 = a[290];
    const double t1551 = t883 * t1550;
    const double t1552 = t1550 * t596;
    const double t1553 = a[338];
    const double t1554 = t222 * t1553;
    const double t1555 = t1553 * t133;
    const double t1556 = a[372];
    const double t1557 = t63 * t1556;
    const double t1558 = t35 * t1556;
    const double t1559 = a[427];
    const double t1560 = t1559 * t4;
    const double t1561 = a[134];
    const double t1563 = (t1549 + t1551 + t1552 + t1554 + t1555 + t1557 + t1558 + t1560 + t1561) * t1546;
    const double t1565 = (t1494 + t1499 + t1506 + t1511 + t1521 + t1526 + t1540 + t1547 + t1563) * t1546;
    const double t1567 = (t1327 + t1335 + t1350 + t1362 + t1392 + t1407 + t1461 + t1493 + t1565) * t1546;
    const double t1594 = t1550 * t1546;
    const double t1615 = 2.0 * t850;
    const double t1662 = 2.0 * t438;
    const double t1667 = 2.0 * t539;
    const double t1670 = t584 * t596;
    const double t1671 = 2.0 * t577;
    const double t1685 = t789 * t596;
    const double t1686 = 2.0 * t782;
    const double t1693 = t851 * t596;
    const double t1696 = t584 * t883;
    const double t1708 = 2.0 * t1158;
    const double t1711 = t1203 * t596;
    const double t1712 = 2.0 * t1196;
    const double t1719 = t1286 * t596;
    const double t1723 = t1203 * t883;
    const double t1731 = t1448 * t596;
    const double t1732 = 2.0 * t1441;
    const double t1735 = t1448 * t883;
    const double t1736 = t1476 * t596;
    const double t1739 = t1553 * t1546;
    const double t1740 = t1529 * t883;
    const double t1741 = t1529 * t596;
    const double t1762 = 2.0 * t204;
    const double t1772 = 2.0 * t385;
    const double t1777 = 2.0 * t422;
    const double t1780 = t421 * t222;
    const double t1781 = 2.0 * t435;
    const double t1786 = 2.0 * t516;
    const double t1789 = t528 * t222;
    const double t1790 = 2.0 * t529;
    const double t1793 = t578 * t222;
    const double t1794 = 2.0 * t567;
    const double t1818 = t783 * t222;
    const double t1819 = 2.0 * t772;
    const double t1850 = 2.0 * t1135;
    const double t1853 = t1147 * t222;
    const double t1854 = 2.0 * t1148;
    const double t1857 = t1197 * t222;
    const double t1858 = 2.0 * t1186;
    const double t1882 = t1442 * t222;
    const double t1883 = 2.0 * t1431;
    const double t1901 = ((2.0 * t58 + t46 + t31 + t32) * t63 + t22 + t27 + t57 + t60) * t63;
    const double t1904 = (2.0 * t96 + t90 + t83 + t84) * t63;
    const double t1905 = t121 * t133;
    const double t1906 = 2.0 * t114;
    const double t1916 = t191 * t133;
    const double t1917 = 2.0 * t184;
    const double t1922 = t205 * t133;
    const double t1925 = t121 * t222;
    const double t1939 = (2.0 * t352 + t354 + t356 + t357) * t63;
    const double t1940 = t386 * t133;
    const double t1941 = 2.0 * t376;
    const double t1946 = t423 * t133;
    const double t1950 = t386 * t222;
    const double t1958 = t517 * t133;
    const double t1959 = 2.0 * t507;
    const double t1962 = t517 * t222;
    const double t1963 = t530 * t133;
    const double t1967 = t568 * t222;
    const double t1968 = t568 * t133;
    const double t1983 = (2.0 * t629 + t345 + t332 + t333) * t63;
    const double t1984 = t388 * t133;
    const double t1985 = 2.0 * t637;
    const double t1990 = t425 * t133;
    const double t1994 = t388 * t222;
    const double t2002 = t724 * t133;
    const double t2003 = 2.0 * t717;
    const double t2006 = t724 * t222;
    const double t2007 = t736 * t133;
    const double t2010 = t792 * t596;
    const double t2011 = t773 * t222;
    const double t2012 = t773 * t133;
    const double t2021 = t519 * t133;
    const double t2022 = 2.0 * t820;
    const double t2025 = t519 * t222;
    const double t2026 = t532 * t133;
    const double t2029 = t854 * t596;
    const double t2030 = t775 * t222;
    const double t2031 = t775 * t133;
    const double t2036 = t794 * t596;
    const double t2037 = t570 * t222;
    const double t2038 = t570 * t133;
    const double t2053 = (2.0 * t986 + t980 + t973 + t974) * t63;
    const double t2054 = t1011 * t133;
    const double t2055 = 2.0 * t1004;
    const double t2060 = t1043 * t133;
    const double t2064 = t1011 * t222;
    const double t2072 = t1136 * t133;
    const double t2073 = 2.0 * t1126;
    const double t2076 = t1136 * t222;
    const double t2077 = t1149 * t133;
    const double t2081 = t1187 * t222;
    const double t2082 = t1187 * t133;
    const double t2091 = t1138 * t133;
    const double t2092 = 2.0 * t1234;
    const double t2095 = t1138 * t222;
    const double t2096 = t1151 * t133;
    const double t2099 = t1289 * t596;
    const double t2100 = t1271 * t222;
    const double t2101 = t1271 * t133;
    const double t2106 = t1291 * t596;
    const double t2107 = t1189 * t222;
    const double t2108 = t1189 * t133;
    const double t2117 = t1383 * t133;
    const double t2118 = 2.0 * t1376;
    const double t2121 = t1383 * t222;
    const double t2122 = t1395 * t133;
    const double t2126 = t1432 * t222;
    const double t2127 = t1432 * t133;
    const double t2132 = t1479 * t596;
    const double t2133 = t1434 * t222;
    const double t2134 = t1434 * t133;
    const double t2138 = t1556 * t1546;
    const double t2141 = t1514 * t222;
    const double t2142 = t1514 * t133;
    const double t2155 = ((2.0 * t29 + t31 + t32) * t35 + t22 + t27 + t34) * t35;
    const double t2164 =
        ((2.0 * t46 + t48 + t49) * t35 + t39 + t44 + t51 + (t45 * t63 + t48 + t49 + 2.0 * t55) * t63) * t63;
    const double t2167 = (2.0 * t81 + t83 + t84) * t35;
    const double t2171 = (t63 * t89 + 2.0 * t90 + t92 + t93) * t63;
    const double t2172 = t115 * t63;
    const double t2173 = 2.0 * t108;
    const double t2187 = t185 * t63;
    const double t2188 = 2.0 * t178;
    const double t2210 = 2.0 * t295;
    const double t2217 = (2.0 * t330 + t332 + t333) * t35;
    const double t2221 = (t353 * t63 + 2.0 * t345 + t347 + t348) * t63;
    const double t2222 = t63 * t377;
    const double t2223 = 2.0 * t369;
    const double t2228 = t63 * t414;
    const double t2243 = t63 * t508;
    const double t2244 = 2.0 * t500;
    const double t2250 = t63 * t559;
    const double t2273 = (2.0 * t622 + t356 + t357) * t35;
    const double t2277 = (t344 * t63 + t347 + t348 + 2.0 * t354) * t63;
    const double t2278 = 2.0 * t634;
    const double t2297 = t63 * t718;
    const double t2298 = 2.0 * t711;
    const double t2303 = t63 * t764;
    const double t2316 = 2.0 * t817;
    const double t2348 = (2.0 * t971 + t973 + t974) * t35;
    const double t2352 = (t63 * t979 + 2.0 * t980 + t982 + t983) * t63;
    const double t2353 = t63 * t1005;
    const double t2354 = 2.0 * t998;
    const double t2374 = t63 * t1127;
    const double t2375 = 2.0 * t1119;
    const double t2381 = t63 * t1178;
    const double t2394 = 2.0 * t1231;
    const double t2416 = t63 * t1377;
    const double t2417 = 2.0 * t1370;
    const double t2423 = t63 * t1423;
    const double t2446 = ((2.0 * t5 + t6) * t4 + t3 + t8) * t4;
    const double t2449 = (2.0 * t16 + t17) * t4;
    const double t2451 = 2.0 * t24;
    const double t2455 = (t2449 + t14 + t19 + (t30 * t35 + t2451 + t25) * t35) * t35;
    const double t2456 = t35 * t47;
    const double t2464 =
        (t2449 + t14 + t19 + (t2456 + 2.0 * t41 + t42) * t35 + (t30 * t63 + t2451 + t2456 + t25) * t63) * t63;
    const double t2467 = (2.0 * t68 + t69) * t4;
    const double t2469 = 2.0 * t76;
    const double t2471 = (t35 * t82 + t2469 + t77) * t35;
    const double t2475 = (t35 * t91 + t63 * t82 + t2469 + t77) * t63;
    const double t2477 = t63 * t109;
    const double t2478 = t35 * t109;
    const double t2479 = 2.0 * t103;
    const double t2490 = 2.0 * t146;
    const double t2497 = t133 * t194;
    const double t2498 = t63 * t179;
    const double t2499 = t35 * t179;
    const double t2500 = 2.0 * t173;
    const double t2519 = ((2.0 * t230 + t231) * t4 + t228 + t233) * t4;
    const double t2522 = (2.0 * t241 + t242) * t4;
    const double t2524 = 2.0 * t249;
    const double t2531 = (2.0 * t267 + t268) * t4;
    const double t2532 = t35 * t281;
    const double t2533 = 2.0 * t275;
    const double t2537 = t35 * t296;
    const double t2538 = 2.0 * t290;
    const double t2545 = (2.0 * t317 + t318) * t4;
    const double t2547 = 2.0 * t325;
    const double t2549 = (t331 * t35 + t2547 + t326) * t35;
    const double t2551 = t35 * t346;
    const double t2552 = 2.0 * t340;
    const double t2554 = (t355 * t63 + t2551 + t2552 + t341) * t63;
    const double t2555 = t133 * t390;
    const double t2556 = t63 * t379;
    const double t2557 = t35 * t370;
    const double t2558 = 2.0 * t364;
    const double t2563 = t133 * t427;
    const double t2566 = 2.0 * t401;
    const double t2569 = t222 * t390;
    const double t2576 = (2.0 * t448 + t449) * t4;
    const double t2578 = 2.0 * t456;
    const double t2582 = t35 * t477;
    const double t2583 = 2.0 * t471;
    const double t2586 = t133 * t521;
    const double t2587 = t63 * t510;
    const double t2588 = t35 * t501;
    const double t2589 = 2.0 * t495;
    const double t2592 = t222 * t521;
    const double t2593 = t133 * t534;
    const double t2597 = t222 * t572;
    const double t2598 = t133 * t572;
    const double t2601 = 2.0 * t546;
    const double t2622 = (t35 * t355 + t2552 + t341) * t35;
    const double t2625 = (t331 * t63 + t2547 + t2551 + t326) * t63;
    const double t2626 = t63 * t370;
    const double t2627 = t35 * t379;
    const double t2644 = 2.0 * t679;
    const double t2652 = t63 * t712;
    const double t2653 = t35 * t712;
    const double t2654 = 2.0 * t706;
    const double t2661 = t596 * t796;
    const double t2662 = t222 * t777;
    const double t2663 = t133 * t777;
    const double t2666 = 2.0 * t751;
    const double t2677 = t63 * t501;
    const double t2678 = t35 * t510;
    const double t2704 = (2.0 * t906 + t907) * t4;
    const double t2706 = 2.0 * t914;
    const double t2711 = t35 * t937;
    const double t2722 = (2.0 * t958 + t959) * t4;
    const double t2724 = 2.0 * t966;
    const double t2726 = (t35 * t972 + t2724 + t967) * t35;
    const double t2730 = (t35 * t981 + t63 * t972 + t2724 + t967) * t63;
    const double t2732 = t63 * t999;
    const double t2733 = t35 * t999;
    const double t2734 = 2.0 * t993;
    const double t2739 = t133 * t1046;
    const double t2752 = (2.0 * t1067 + t1068) * t4;
    const double t2754 = 2.0 * t1075;
    const double t2758 = t35 * t1096;
    const double t2759 = 2.0 * t1090;
    const double t2762 = t133 * t1140;
    const double t2763 = t63 * t1129;
    const double t2764 = t35 * t1120;
    const double t2765 = 2.0 * t1114;
    const double t2768 = t222 * t1140;
    const double t2769 = t133 * t1153;
    const double t2773 = t222 * t1191;
    const double t2774 = t133 * t1191;
    const double t2777 = 2.0 * t1165;
    const double t2788 = t63 * t1120;
    const double t2789 = t35 * t1129;
    const double t2794 = t596 * t1293;
    const double t2813 = 2.0 * t1338;
    const double t2821 = t63 * t1371;
    const double t2822 = t35 * t1371;
    const double t2823 = 2.0 * t1365;
    const double t2831 = t222 * t1436;
    const double t2832 = t133 * t1436;
    const double t2835 = 2.0 * t1410;
    g[0] = (((2.0 * t1549 + t1551 + t1552 + t1554 + t1555 + t1557 + t1558 + t1560 + t1561) * t1546 + t1494 + t1499 +
             t1506 + t1511 + t1521 + t1526 + t1540 + t1547 + t1563) *
                t1546 +
            t1327 + t1335 + t1350 + t1362 + t1392 + t1407 + t1461 + t1493 + t1565) *
               t1546 +
           t891 + t902 + t928 + t954 + t1022 + t1063 + t1218 + t1326 + t1567;
    g[1] = (((2.0 * t880 + t788 + t585 + t586 + t881 + t882 + t592 + t593) * t883 + t544 + t549 + t866 + t869 + t873 +
             t875 + t879 + t884) *
                t883 +
            t445 + t453 + t809 + t816 + t828 + t836 + t863 + t886) *
               t883 +
           t226 + t237 + t608 + t621 + t647 + t667 + t804 + t888 +
           (((2.0 * t1318 + t1285 + t1204 + t1205 + t1319 + t1320 + t1211 + t1212) * t883 + t1163 + t1168 + t1302 +
             t1305 + t1309 + t1311 + t1317 + t1322) *
                t883 +
            t1064 + t1072 + t1223 + t1230 + t1242 + t1250 + t1299 + t1324 +
            ((2.0 * t1487 + t1475 + t1449 + t1450 + t1488 + t1489 + t1456 + t1457) * t883 + t1408 + t1413 + t1464 +
             t1467 + t1471 + t1473 + t1486 + t1491 +
             (t1594 + 2.0 * t1541 + t1543 + t1530 + t1531 + t1544 + t1545 + t1537 + t1538) * t1546) *
                t1546) *
               t1546;
    g[2] =
        (((2.0 * t583 + t585 + t586 + t588 + t590 + t592 + t593) * t596 + t544 + t549 + t556 + t565 + t576 + t581 +
          t595) *
             t596 +
         t445 + t453 + t468 + t492 + t527 + t543 + t597) *
            t596 +
        t226 + t237 + t263 + t313 + t398 + t444 + t599 +
        (((2.0 * t788 + t790 + t791 + t793 + t795 + t797 + t798) * t596 + t749 + t754 + t761 + t770 + t781 + t786 +
          t800) *
             t596 +
         t668 + t676 + t691 + t703 + t733 + t748 + t802 +
         ((t1615 + t852 + t853 + t855 + t856 + t858 + t859) * t596 + t749 + t754 + t839 + t842 + t846 + t848 + t861 +
          (t787 * t883 + t1615 + t790 + t791 + t797 + t798 + t876 + t877) * t883) *
             t883) *
            t883 +
        (((2.0 * t1202 + t1204 + t1205 + t1207 + t1209 + t1211 + t1212) * t596 + t1163 + t1168 + t1175 + t1184 + t1195 +
          t1200 + t1214) *
             t596 +
         t1064 + t1072 + t1087 + t1111 + t1146 + t1162 + t1216 +
         ((2.0 * t1285 + t1287 + t1288 + t1290 + t1292 + t1294 + t1295) * t596 + t1251 + t1256 + t1263 + t1268 + t1278 +
          t1283 + t1297 + (t1284 * t883 + t1287 + t1288 + t1294 + t1295 + 2.0 * t1313 + t1314 + t1315) * t883) *
             t883 +
         ((2.0 * t1447 + t1449 + t1450 + t1452 + t1454 + t1456 + t1457) * t596 + t1408 + t1413 + t1420 + t1429 + t1440 +
          t1445 + t1459 + (t1474 * t883 + 2.0 * t1475 + t1477 + t1478 + t1480 + t1481 + t1483 + t1484) * t883 +
          (t1542 * t883 + 2.0 * t1528 + t1530 + t1531 + t1533 + t1535 + t1537 + t1538 + t1594) * t1546) *
             t1546) *
            t1546;
    g[3] =
        (((2.0 * t217 + t190 + t122 + t123 + t125 + t126) * t222 + t101 + t106 + t113 + t118 + t216 + t219) * t222 +
         t65 + t73 + t88 + t100 + t214 + t221) *
            t222 +
        t1 + t12 + t38 + t64 + t202 + t223 +
        (((t1662 + t422 + t387 + t389 + t391 + t392) * t222 + t362 + t367 + t374 + t383 + t437 + t440) * t222 + t314 +
         t322 + t337 + t361 + t433 + t442 +
         ((t1667 + t529 + t518 + t520 + t522 + t523) * t222 + t493 + t498 + t505 + t514 + t538 + t541 +
          (t1670 + t1671 + t579 + t569 + t571 + t573 + t574) * t596) *
             t596) *
            t596 +
        (((t1662 + t422 + t640 + t641 + t391 + t392) * t222 + t362 + t367 + t636 + t639 + t661 + t663) * t222 + t314 +
         t322 + t626 + t633 + t659 + t665 +
         ((2.0 * t744 + t735 + t725 + t726 + t728 + t729) * t222 + t704 + t709 + t716 + t721 + t743 + t746 +
          (t1685 + t1686 + t784 + t774 + t776 + t778 + t779) * t596) *
             t596 +
         ((t1667 + t529 + t823 + t824 + t522 + t523) * t222 + t493 + t498 + t819 + t822 + t832 + t834 +
          (t1693 + t1686 + t784 + t843 + t844 + t778 + t779) * t596 +
          (t1696 + t1685 + t1671 + t579 + t870 + t871 + t573 + t574) * t883) *
             t883) *
            t883 +
        (((2.0 * t1057 + t1042 + t1012 + t1013 + t1015 + t1016) * t222 + t991 + t996 + t1003 + t1008 + t1056 + t1059) *
             t222 +
         t955 + t963 + t978 + t990 + t1052 + t1061 +
         ((t1708 + t1148 + t1137 + t1139 + t1141 + t1142) * t222 + t1112 + t1117 + t1124 + t1133 + t1157 + t1160 +
          (t1711 + t1712 + t1198 + t1188 + t1190 + t1192 + t1193) * t596) *
             t596 +
         ((t1708 + t1148 + t1237 + t1238 + t1141 + t1142) * t222 + t1112 + t1117 + t1233 + t1236 + t1246 + t1248 +
          (t1719 + 2.0 * t1279 + t1281 + t1272 + t1273 + t1275 + t1276) * t596 +
          (t1723 + t1719 + t1712 + t1198 + t1306 + t1307 + t1192 + t1193) * t883) *
             t883 +
         ((2.0 * t1403 + t1394 + t1384 + t1385 + t1387 + t1388) * t222 + t1363 + t1368 + t1375 + t1380 + t1402 + t1405 +
          (t1731 + t1732 + t1443 + t1433 + t1435 + t1437 + t1438) * t596 +
          (t1735 + t1736 + t1732 + t1443 + t1468 + t1469 + t1437 + t1438) * t883 +
          (t1739 + t1740 + t1741 + 2.0 * t1522 + t1524 + t1515 + t1516 + t1518 + t1519) * t1546) *
             t1546) *
            t1546;
    g[4] = (((2.0 * t120 + t122 + t123 + t125 + t126) * t133 + t101 + t106 + t113 + t118 + t128) * t133 + t65 + t73 +
            t88 + t100 + t130) *
               t133 +
           t1 + t12 + t38 + t64 + t132 +
           (((2.0 * t190 + t192 + t193 + t195 + t196) * t133 + t171 + t176 + t183 + t188 + t198) * t133 + t135 + t143 +
            t158 + t170 + t200 +
            ((t1762 + t206 + t207 + t209 + t210) * t133 + t171 + t176 + t183 + t188 + t212 +
             (t189 * t222 + t1762 + t192 + t193 + t195 + t196) * t222) *
                t222) *
               t222 +
           (((t1772 + t387 + t389 + t391 + t392) * t133 + t362 + t367 + t374 + t383 + t394) * t133 + t314 + t322 +
            t337 + t361 + t396 +
            ((t1777 + t424 + t426 + t428 + t429) * t133 + t399 + t404 + t411 + t420 + t431 +
             (t1780 + t1781 + t424 + t426 + t428 + t429) * t222) *
                t222 +
            ((t1786 + t518 + t520 + t522 + t523) * t133 + t493 + t498 + t505 + t514 + t525 +
             (t1789 + t1790 + t531 + t533 + t535 + t536) * t222 +
             (t1670 + t1793 + t1794 + t569 + t571 + t573 + t574) * t596) *
                t596) *
               t596 +
           (((t1772 + t640 + t641 + t391 + t392) * t133 + t362 + t367 + t636 + t639 + t643) * t133 + t314 + t322 +
            t626 + t633 + t645 +
            ((t1777 + t654 + t655 + t428 + t429) * t133 + t399 + t404 + t650 + t653 + t657 +
             (t1780 + t1781 + t654 + t655 + t428 + t429) * t222) *
                t222 +
            ((2.0 * t723 + t725 + t726 + t728 + t729) * t133 + t704 + t709 + t716 + t721 + t731 +
             (t222 * t734 + 2.0 * t735 + t737 + t738 + t740 + t741) * t222 +
             (t1685 + t1818 + t1819 + t774 + t776 + t778 + t779) * t596) *
                t596 +
            ((t1786 + t823 + t824 + t522 + t523) * t133 + t493 + t498 + t819 + t822 + t826 +
             (t1789 + t1790 + t829 + t830 + t535 + t536) * t222 +
             (t1693 + t1818 + t1819 + t843 + t844 + t778 + t779) * t596 +
             (t1696 + t1685 + t1793 + t1794 + t870 + t871 + t573 + t574) * t883) *
                t883) *
               t883 +
           (((2.0 * t1010 + t1012 + t1013 + t1015 + t1016) * t133 + t991 + t996 + t1003 + t1008 + t1018) * t133 + t955 +
            t963 + t978 + t990 + t1020 +
            ((2.0 * t1042 + t1044 + t1045 + t1047 + t1048) * t133 + t1023 + t1028 + t1035 + t1040 + t1050 +
             (t1041 * t222 + t1044 + t1045 + t1047 + t1048 + 2.0 * t1054) * t222) *
                t222 +
            ((t1850 + t1137 + t1139 + t1141 + t1142) * t133 + t1112 + t1117 + t1124 + t1133 + t1144 +
             (t1853 + t1854 + t1150 + t1152 + t1154 + t1155) * t222 +
             (t1711 + t1857 + t1858 + t1188 + t1190 + t1192 + t1193) * t596) *
                t596 +
            ((t1850 + t1237 + t1238 + t1141 + t1142) * t133 + t1112 + t1117 + t1233 + t1236 + t1240 +
             (t1853 + t1854 + t1243 + t1244 + t1154 + t1155) * t222 +
             (t1280 * t222 + 2.0 * t1270 + t1272 + t1273 + t1275 + t1276 + t1719) * t596 +
             (t1723 + t1719 + t1857 + t1858 + t1306 + t1307 + t1192 + t1193) * t883) *
                t883 +
            ((2.0 * t1382 + t1384 + t1385 + t1387 + t1388) * t133 + t1363 + t1368 + t1375 + t1380 + t1390 +
             (t1393 * t222 + 2.0 * t1394 + t1396 + t1397 + t1399 + t1400) * t222 +
             (t1731 + t1882 + t1883 + t1433 + t1435 + t1437 + t1438) * t596 +
             (t1735 + t1736 + t1882 + t1883 + t1468 + t1469 + t1437 + t1438) * t883 +
             (t1523 * t222 + 2.0 * t1513 + t1515 + t1516 + t1518 + t1519 + t1739 + t1740 + t1741) * t1546) *
                t1546) *
               t1546;
    g[5] = (t1901 + t13 + t21 + t53 + t62 +
            (t1904 + t74 + t79 + t95 + t98 + (t1905 + t1906 + t116 + t110 + t111) * t133) * t133) *
               t133 +
           (t1901 + t13 + t21 + t53 + t62 +
            ((2.0 * t166 + t160 + t153 + t154) * t63 + t144 + t149 + t165 + t168 +
             (t1916 + t1917 + t186 + t180 + t181) * t133) *
                t133 +
            (t1904 + t74 + t79 + t95 + t98 + (t1922 + t1917 + t186 + t180 + t181) * t133 +
             (t1925 + t1916 + t1906 + t116 + t110 + t111) * t222) *
                t222) *
               t222 +
           (((2.0 * t302 + t304 + t306 + t307) * t63 + t288 + t293 + t300 + t309) * t63 + t264 + t272 + t287 + t311 +
            (t1939 + t338 + t343 + t350 + t359 + (t1940 + t1941 + t378 + t380 + t381) * t133) * t133 +
            (t1939 + t338 + t343 + t350 + t359 + (t1946 + 2.0 * t413 + t415 + t417 + t418) * t133 +
             (t1950 + t1946 + t1941 + t378 + t380 + t381) * t222) *
                t222 +
            ((2.0 * t483 + t485 + t487 + t488) * t63 + t469 + t474 + t481 + t490 +
             (t1958 + t1959 + t509 + t511 + t512) * t133 + (t1962 + t1963 + t1959 + t509 + t511 + t512) * t222 +
             (t587 * t596 + t1967 + t1968 + 2.0 * t558 + t560 + t562 + t563) * t596) *
                t596) *
               t596 +
           (((2.0 * t615 + t280 + t256 + t257) * t63 + t247 + t252 + t614 + t617) * t63 + t238 + t246 + t612 + t619 +
            (t1983 + t323 + t328 + t628 + t631 + (t1984 + t1985 + t378 + t371 + t372) * t133) * t133 +
            (t1983 + t323 + t328 + t628 + t631 + (t1990 + 2.0 * t651 + t415 + t408 + t409) * t133 +
             (t1994 + t1990 + t1985 + t378 + t371 + t372) * t222) *
                t222 +
            ((2.0 * t699 + t693 + t686 + t687) * t63 + t677 + t682 + t698 + t701 +
             (t2002 + t2003 + t719 + t713 + t714) * t133 + (t2006 + t2007 + t2003 + t719 + t713 + t714) * t222 +
             (t2010 + t2011 + t2012 + 2.0 * t763 + t765 + t767 + t768) * t596) *
                t596 +
            ((2.0 * t812 + t476 + t463 + t464) * t63 + t454 + t459 + t811 + t814 +
             (t2021 + t2022 + t509 + t502 + t503) * t133 + (t2025 + t2026 + t2022 + t509 + t502 + t503) * t222 +
             (t2029 + t2030 + t2031 + 2.0 * t840 + t765 + t758 + t759) * t596 +
             (t589 * t883 + t2036 + t2037 + t2038 + t553 + t554 + t560 + 2.0 * t867) * t883) *
                t883) *
               t883 +
           (((2.0 * t948 + t936 + t921 + t922) * t63 + t912 + t917 + t947 + t950) * t63 + t903 + t911 + t943 + t952 +
            (t2053 + t964 + t969 + t985 + t988 + (t2054 + t2055 + t1006 + t1000 + t1001) * t133) * t133 +
            (t2053 + t964 + t969 + t985 + t988 + (t2060 + 2.0 * t1036 + t1038 + t1032 + t1033) * t133 +
             (t2064 + t2060 + t2055 + t1006 + t1000 + t1001) * t222) *
                t222 +
            ((2.0 * t1102 + t1104 + t1106 + t1107) * t63 + t1088 + t1093 + t1100 + t1109 +
             (t2072 + t2073 + t1128 + t1130 + t1131) * t133 + (t2076 + t2077 + t2073 + t1128 + t1130 + t1131) * t222 +
             (t1206 * t596 + 2.0 * t1177 + t1179 + t1181 + t1182 + t2081 + t2082) * t596) *
                t596 +
            ((2.0 * t1226 + t1095 + t1082 + t1083) * t63 + t1073 + t1078 + t1225 + t1228 +
             (t2091 + t2092 + t1128 + t1121 + t1122) * t133 + (t2095 + t2096 + t2092 + t1128 + t1121 + t1122) * t222 +
             (t2099 + t2100 + t2101 + 2.0 * t1264 + t1266 + t1260 + t1261) * t596 +
             (t1208 * t883 + t1172 + t1173 + t1179 + 2.0 * t1303 + t2106 + t2107 + t2108) * t883) *
                t883 +
            ((2.0 * t1358 + t1352 + t1345 + t1346) * t63 + t1336 + t1341 + t1357 + t1360 +
             (t2117 + t2118 + t1378 + t1372 + t1373) * t133 + (t2121 + t2122 + t2118 + t1378 + t1372 + t1373) * t222 +
             (t1451 * t596 + 2.0 * t1422 + t1424 + t1426 + t1427 + t2126 + t2127) * t596 +
             (t1453 * t883 + t1417 + t1418 + t1424 + 2.0 * t1465 + t2132 + t2133 + t2134) * t883 +
             (t1532 * t596 + t1534 * t883 + t1503 + t1504 + 2.0 * t1507 + t1509 + t2138 + t2141 + t2142) * t1546) *
                t1546) *
               t1546;
    g[6] =
        (t2155 + t13 + t21 + t36 + t2164 +
         (t2167 + t74 + t79 + t86 + t2171 + (t1905 + t2172 + t2173 + t110 + t111) * t133) * t133) *
            t133 +
        (t2155 + t13 + t21 + t36 + t2164 +
         ((2.0 * t151 + t153 + t154) * t35 + t144 + t149 + t156 + (t159 * t63 + 2.0 * t160 + t162 + t163) * t63 +
          (t1916 + t2187 + t2188 + t180 + t181) * t133) *
             t133 +
         (t2167 + t74 + t79 + t86 + t2171 + (t1922 + t2187 + t2188 + t180 + t181) * t133 +
          (t1925 + t1916 + t2172 + t2173 + t110 + t111) * t222) *
             t222) *
            t222 +
        (((2.0 * t254 + t256 + t257) * t35 + t247 + t252 + t259) * t35 + t238 + t246 + t261 +
         ((2.0 * t280 + t282 + t283) * t35 + t273 + t278 + t285 + (t303 * t63 + t2210 + t297 + t298) * t63) * t63 +
         (t2217 + t323 + t328 + t335 + t2221 + (t1984 + t2222 + t2223 + t371 + t372) * t133) * t133 +
         (t2217 + t323 + t328 + t335 + t2221 + (t1990 + t2228 + 2.0 * t406 + t408 + t409) * t133 +
          (t1994 + t1990 + t2222 + t2223 + t371 + t372) * t222) *
             t222 +
         ((2.0 * t461 + t463 + t464) * t35 + t454 + t459 + t466 + (t484 * t63 + 2.0 * t476 + t478 + t479) * t63 +
          (t2021 + t2243 + t2244 + t502 + t503) * t133 + (t2025 + t2026 + t2243 + t2244 + t502 + t503) * t222 +
          (t589 * t596 + t2037 + t2038 + t2250 + 2.0 * t551 + t553 + t554) * t596) *
             t596) *
            t596 +
        (((2.0 * t602 + t306 + t307) * t35 + t288 + t293 + t604) * t35 + t264 + t272 + t606 +
         ((2.0 * t304 + t297 + t298) * t35 + t273 + t278 + t610 + (t279 * t63 + t2210 + t282 + t283) * t63) * t63 +
         (t2273 + t338 + t343 + t624 + t2277 + (t1940 + t2222 + t2278 + t380 + t381) * t133) * t133 +
         (t2273 + t338 + t343 + t624 + t2277 + (t1946 + t2228 + 2.0 * t648 + t417 + t418) * t133 +
          (t1950 + t1946 + t2222 + t2278 + t380 + t381) * t222) *
             t222 +
         ((2.0 * t684 + t686 + t687) * t35 + t677 + t682 + t689 + (t63 * t692 + 2.0 * t693 + t695 + t696) * t63 +
          (t2002 + t2297 + t2298 + t713 + t714) * t133 + (t2006 + t2007 + t2297 + t2298 + t713 + t714) * t222 +
          (t2036 + t2030 + t2031 + t2303 + 2.0 * t756 + t758 + t759) * t596) *
             t596 +
         ((2.0 * t805 + t487 + t488) * t35 + t469 + t474 + t807 + (t475 * t63 + t478 + t479 + 2.0 * t485) * t63 +
          (t1958 + t2243 + t2316 + t511 + t512) * t133 + (t1962 + t1963 + t2243 + t2316 + t511 + t512) * t222 +
          (t2029 + t2011 + t2012 + t2303 + 2.0 * t837 + t767 + t768) * t596 +
          (t587 * t883 + t1967 + t1968 + t2010 + t2250 + t562 + t563 + 2.0 * t864) * t883) *
             t883) *
            t883 +
        (((2.0 * t919 + t921 + t922) * t35 + t912 + t917 + t924) * t35 + t903 + t911 + t926 +
         ((2.0 * t936 + t938 + t939) * t35 + t929 + t934 + t941 + (t63 * t935 + t938 + t939 + 2.0 * t945) * t63) * t63 +
         (t2348 + t964 + t969 + t976 + t2352 + (t2054 + t2353 + t2354 + t1000 + t1001) * t133) * t133 +
         (t2348 + t964 + t969 + t976 + t2352 + (t1037 * t63 + 2.0 * t1030 + t1032 + t1033 + t2060) * t133 +
          (t2064 + t2060 + t2353 + t2354 + t1000 + t1001) * t222) *
             t222 +
         ((2.0 * t1080 + t1082 + t1083) * t35 + t1073 + t1078 + t1085 +
          (t1103 * t63 + 2.0 * t1095 + t1097 + t1098) * t63 + (t2091 + t2374 + t2375 + t1121 + t1122) * t133 +
          (t2095 + t2096 + t2374 + t2375 + t1121 + t1122) * t222 +
          (t1208 * t596 + 2.0 * t1170 + t1172 + t1173 + t2107 + t2108 + t2381) * t596) *
             t596 +
         ((2.0 * t1219 + t1106 + t1107) * t35 + t1088 + t1093 + t1221 +
          (t1094 * t63 + t1097 + t1098 + 2.0 * t1104) * t63 + (t2072 + t2374 + t2394 + t1130 + t1131) * t133 +
          (t2076 + t2077 + t2374 + t2394 + t1130 + t1131) * t222 +
          (t1265 * t63 + 2.0 * t1258 + t1260 + t1261 + t2100 + t2101 + t2106) * t596 +
          (t1206 * t883 + t1181 + t1182 + 2.0 * t1300 + t2081 + t2082 + t2099 + t2381) * t883) *
             t883 +
         ((2.0 * t1343 + t1345 + t1346) * t35 + t1336 + t1341 + t1348 +
          (t1351 * t63 + 2.0 * t1352 + t1354 + t1355) * t63 + (t2117 + t2416 + t2417 + t1372 + t1373) * t133 +
          (t2121 + t2122 + t2416 + t2417 + t1372 + t1373) * t222 +
          (t1453 * t596 + 2.0 * t1415 + t1417 + t1418 + t2133 + t2134 + t2423) * t596 +
          (t1451 * t883 + t1426 + t1427 + 2.0 * t1462 + t2126 + t2127 + t2132 + t2423) * t883 +
          (t1508 * t63 + t1532 * t883 + t1534 * t596 + 2.0 * t1501 + t1503 + t1504 + t2138 + t2141 + t2142) * t1546) *
             t1546) *
            t1546;
    g[7] =
        (t2446 + t2 + t10 + t2455 + t2464 +
         (t2467 + t66 + t71 + t2471 + t2475 + (t124 * t133 + t104 + t2477 + t2478 + t2479) * t133) * t133) *
            t133 +
        (t2446 + t2 + t10 + t2455 + t2464 +
         ((2.0 * t138 + t139) * t4 + t136 + t141 + (t152 * t35 + t147 + t2490) * t35 +
          (t152 * t63 + t161 * t35 + t147 + t2490) * t63 + (t2497 + t2498 + t2499 + t2500 + t174) * t133) *
             t133 +
         (t2467 + t66 + t71 + t2471 + t2475 + (t133 * t208 + t174 + t2498 + t2499 + t2500) * t133 +
          (t124 * t222 + t104 + t2477 + t2478 + t2479 + t2497) * t222) *
             t222) *
            t222 +
        (t2519 + t227 + t235 + (t2522 + t239 + t244 + (t255 * t35 + t250 + t2524) * t35) * t35 +
         (t2531 + t265 + t270 + (t2532 + t2533 + t276) * t35 + (t305 * t63 + t2537 + t2538 + t291) * t63) * t63 +
         (t2545 + t315 + t320 + t2549 + t2554 + (t2555 + t2556 + t2557 + t2558 + t365) * t133) * t133 +
         (t2545 + t315 + t320 + t2549 + t2554 + (t35 * t407 + t416 * t63 + t2563 + t2566 + t402) * t133 +
          (t2569 + t2563 + t2556 + t2557 + t2558 + t365) * t222) *
             t222 +
         (t2576 + t446 + t451 + (t35 * t462 + t2578 + t457) * t35 + (t486 * t63 + t2582 + t2583 + t472) * t63 +
          (t2586 + t2587 + t2588 + t2589 + t496) * t133 + (t2592 + t2593 + t2587 + t2588 + t2589 + t496) * t222 +
          (t35 * t552 + t561 * t63 + t591 * t596 + t2597 + t2598 + t2601 + t547) * t596) *
             t596) *
            t596 +
        (t2519 + t227 + t235 + (t2531 + t265 + t270 + (t305 * t35 + t2538 + t291) * t35) * t35 +
         (t2522 + t239 + t244 + (t2537 + t2533 + t276) * t35 + (t255 * t63 + t250 + t2524 + t2532) * t63) * t63 +
         (t2545 + t315 + t320 + t2622 + t2625 + (t2555 + t2626 + t2627 + t2558 + t365) * t133) * t133 +
         (t2545 + t315 + t320 + t2622 + t2625 + (t35 * t416 + t407 * t63 + t2563 + t2566 + t402) * t133 +
          (t2569 + t2563 + t2626 + t2627 + t2558 + t365) * t222) *
             t222 +
         ((2.0 * t671 + t672) * t4 + t669 + t674 + (t35 * t685 + t2644 + t680) * t35 +
          (t35 * t694 + t63 * t685 + t2644 + t680) * t63 + (t133 * t727 + t2652 + t2653 + t2654 + t707) * t133 +
          (t133 * t739 + t222 * t727 + t2652 + t2653 + t2654 + t707) * t222 +
          (t35 * t757 + t63 * t766 + t2661 + t2662 + t2663 + t2666 + t752) * t596) *
             t596 +
         (t2576 + t446 + t451 + (t35 * t486 + t2583 + t472) * t35 + (t462 * t63 + t2578 + t2582 + t457) * t63 +
          (t2586 + t2677 + t2678 + t2589 + t496) * t133 + (t2592 + t2593 + t2677 + t2678 + t2589 + t496) * t222 +
          (t35 * t766 + t596 * t857 + t63 * t757 + t2662 + t2663 + t2666 + t752) * t596 +
          (t35 * t561 + t552 * t63 + t591 * t883 + t2597 + t2598 + t2601 + t2661 + t547) * t883) *
             t883) *
            t883 +
        (((2.0 * t895 + t896) * t4 + t893 + t898) * t4 + t892 + t900 +
         (t2704 + t904 + t909 + (t35 * t920 + t2706 + t915) * t35) * t35 +
         (t2704 + t904 + t909 + (t2711 + 2.0 * t931 + t932) * t35 + (t63 * t920 + t2706 + t2711 + t915) * t63) * t63 +
         (t2722 + t956 + t961 + t2726 + t2730 + (t1014 * t133 + t2732 + t2733 + t2734 + t994) * t133) * t133 +
         (t2722 + t956 + t961 + t2726 + t2730 + (t1031 * t35 + t1031 * t63 + 2.0 * t1025 + t1026 + t2739) * t133 +
          (t1014 * t222 + t2732 + t2733 + t2734 + t2739 + t994) * t222) *
             t222 +
         (t2752 + t1065 + t1070 + (t1081 * t35 + t1076 + t2754) * t35 + (t1105 * t63 + t1091 + t2758 + t2759) * t63 +
          (t2762 + t2763 + t2764 + t2765 + t1115) * t133 + (t2768 + t2769 + t2763 + t2764 + t2765 + t1115) * t222 +
          (t1171 * t35 + t1180 * t63 + t1210 * t596 + t1166 + t2773 + t2774 + t2777) * t596) *
             t596 +
         (t2752 + t1065 + t1070 + (t1105 * t35 + t1091 + t2759) * t35 + (t1081 * t63 + t1076 + t2754 + t2758) * t63 +
          (t2762 + t2788 + t2789 + t2765 + t1115) * t133 + (t2768 + t2769 + t2788 + t2789 + t2765 + t1115) * t222 +
          (t1259 * t35 + t1259 * t63 + t1274 * t133 + t1274 * t222 + 2.0 * t1253 + t1254 + t2794) * t596 +
          (t1171 * t63 + t1180 * t35 + t1210 * t883 + t1166 + t2773 + t2774 + t2777 + t2794) * t883) *
             t883 +
         ((2.0 * t1330 + t1331) * t4 + t1328 + t1333 + (t1344 * t35 + t1339 + t2813) * t35 +
          (t1344 * t63 + t1353 * t35 + t1339 + t2813) * t63 + (t133 * t1386 + t1366 + t2821 + t2822 + t2823) * t133 +
          (t133 * t1398 + t1386 * t222 + t1366 + t2821 + t2822 + t2823) * t222 +
          (t1416 * t35 + t1425 * t63 + t1455 * t596 + t1411 + t2831 + t2832 + t2835) * t596 +
          (t1416 * t63 + t1425 * t35 + t1455 * t883 + t1482 * t596 + t1411 + t2831 + t2832 + t2835) * t883 +
          (t133 * t1517 + t1502 * t35 + t1502 * t63 + t1517 * t222 + t1536 * t596 + t1536 * t883 + t1546 * t1559 +
           2.0 * t1496 + t1497) *
              t1546) *
             t1546) *
            t1546;
    return (t1 + t12 + t38 + t64 + t132) * t133 + (t1 + t12 + t38 + t64 + t202 + t223) * t222 +
           (t226 + t237 + t263 + t313 + t398 + t444 + t599) * t596 +
           (t226 + t237 + t608 + t621 + t647 + t667 + t804 + t888) * t883 +
           (t891 + t902 + t928 + t954 + t1022 + t1063 + t1218 + t1326 + t1567) * t1546;
}

}  // namespace mbnrg_A1_B1C2X2_deg5
