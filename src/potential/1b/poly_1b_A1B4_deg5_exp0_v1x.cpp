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

#include "poly_1b_A1B4_deg5_exp0_v1x.h"

namespace x1b_A1B4_deg5_exp0 {

double poly_1b_A1B4_deg5_exp0_v1x::eval(const double a[207], const double x[10], double g[10]) {
    const double t1 = a[0];
    const double t2 = a[7];
    const double t3 = a[26];
    const double t4 = a[203];
    const double t7 = x[9];
    const double t5 = t7 * t4;
    const double t6 = a[45];
    const double t8 = (t5 + t6) * t7;
    const double t10 = (t3 + t8) * t7;
    const double t12 = (t2 + t10) * t7;
    const double t15 = a[2];
    const double t16 = a[20];
    const double t17 = a[128];
    const double t18 = t7 * t17;
    const double t19 = a[31];
    const double t21 = (t18 + t19) * t7;
    const double t23 = (t16 + t21) * t7;
    const double t25 = (t15 + t23) * t7;
    const double t26 = a[103];
    const double t27 = t7 * t26;
    const double t28 = a[29];
    const double t30 = (t27 + t28) * t7;
    const double t32 = (t16 + t30) * t7;
    const double t34 = (t27 + t19) * t7;
    const double t33 = x[8];
    const double t35 = t33 * t4;
    const double t37 = (t35 + t18 + t6) * t33;
    const double t39 = (t3 + t34 + t37) * t33;
    const double t41 = (t2 + t32 + t39) * t33;
    const double t44 = a[11];
    const double t45 = a[126];
    const double t46 = t7 * t45;
    const double t47 = a[47];
    const double t49 = (t46 + t47) * t7;
    const double t51 = (t44 + t49) * t7;
    const double t52 = a[148];
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
    const double t66 = x[7];
    const double t70 = t66 * t4;
    const double t72 = (t70 + t56 + t18 + t6) * t66;
    const double t74 = (t3 + t34 + t69 + t72) * t66;
    const double t76 = (t2 + t32 + t67 + t74) * t66;
    const double t79 = a[21];
    const double t80 = a[131];
    const double t81 = t7 * t80;
    const double t82 = a[34];
    const double t84 = (t81 + t82) * t7;
    const double t86 = (t79 + t84) * t7;
    const double t87 = a[85];
    const double t88 = t7 * t87;
    const double t90 = (t88 + t82) * t7;
    const double t92 = (t56 + t81 + t19) * t33;
    const double t94 = (t16 + t90 + t92) * t33;
    const double t96 = (t15 + t86 + t94) * t33;
    const double t97 = a[5];
    const double t98 = a[13];
    const double t99 = a[145];
    const double t100 = t7 * t99;
    const double t101 = a[51];
    const double t103 = (t100 + t101) * t7;
    const double t105 = (t98 + t103) * t7;
    const double t106 = a[99];
    const double t107 = t7 * t106;
    const double t108 = a[30];
    const double t110 = (t107 + t108) * t7;
    const double t111 = t33 * t99;
    const double t113 = (t111 + t107 + t101) * t33;
    const double t115 = (t98 + t110 + t113) * t33;
    const double t116 = a[27];
    const double t117 = a[136];
    const double t118 = t7 * t117;
    const double t119 = a[57];
    const double t121 = (t118 + t119) * t7;
    const double t122 = t33 * t117;
    const double t123 = a[176];
    const double t124 = t7 * t123;
    const double t126 = (t122 + t124 + t119) * t33;
    const double t127 = a[165];
    const double t128 = t66 * t127;
    const double t129 = a[112];
    const double t130 = t33 * t129;
    const double t131 = t7 * t129;
    const double t132 = a[66];
    const double t134 = (t128 + t130 + t131 + t132) * t66;
    const double t136 = (t116 + t121 + t126 + t134) * t66;
    const double t138 = (t97 + t105 + t115 + t136) * t66;
    const double t140 = (t63 + t88 + t28) * t33;
    const double t142 = (t16 + t90 + t140) * t33;
    const double t143 = a[87];
    const double t144 = t7 * t143;
    const double t146 = (t122 + t144 + t119) * t33;
    const double t147 = a[200];
    const double t148 = t66 * t147;
    const double t149 = a[102];
    const double t150 = t33 * t149;
    const double t151 = t7 * t149;
    const double t152 = a[79];
    const double t154 = (t148 + t150 + t151 + t152) * t66;
    const double t156 = (t116 + t121 + t146 + t154) * t66;
    const double t158 = (t63 + t81 + t19) * t33;
    const double t160 = (t148 + t130 + t131 + t132) * t66;
    const double t155 = x[6];
    const double t161 = t155 * t4;
    const double t163 = (t161 + t128 + t56 + t18 + t6) * t155;
    const double t165 = (t3 + t34 + t158 + t160 + t163) * t155;
    const double t167 = (t2 + t32 + t142 + t156 + t165) * t155;
    const double t170 = t33 * t127;
    const double t172 = (t170 + t131 + t132) * t33;
    const double t174 = (t116 + t121 + t172) * t33;
    const double t176 = (t97 + t105 + t174) * t33;
    const double t178 = (t130 + t124 + t119) * t33;
    const double t180 = (t98 + t110 + t178) * t33;
    const double t182 = (t122 + t107 + t101) * t33;
    const double t183 = t66 * t17;
    const double t185 = (t183 + t111 + t81 + t19) * t66;
    const double t187 = (t16 + t90 + t182 + t185) * t66;
    const double t189 = (t15 + t86 + t180 + t187) * t66;
    const double t191 = (t130 + t144 + t119) * t33;
    const double t193 = (t98 + t110 + t191) * t33;
    const double t194 = a[191];
    const double t195 = t33 * t194;
    const double t196 = a[206];
    const double t197 = t7 * t196;
    const double t198 = a[56];
    const double t200 = (t195 + t197 + t198) * t33;
    const double t201 = t66 * t129;
    const double t203 = (t201 + t195 + t144 + t119) * t66;
    const double t205 = (t98 + t110 + t200 + t203) * t66;
    const double t206 = t66 * t149;
    const double t208 = (t206 + t195 + t124 + t119) * t66;
    const double t209 = t155 * t17;
    const double t211 = (t209 + t201 + t111 + t46 + t19) * t155;
    const double t213 = (t16 + t55 + t182 + t208 + t211) * t155;
    const double t215 = (t15 + t51 + t193 + t205 + t213) * t155;
    const double t216 = t33 * t147;
    const double t218 = (t216 + t151 + t152) * t33;
    const double t220 = (t116 + t121 + t218) * t33;
    const double t222 = (t150 + t144 + t119) * t33;
    const double t223 = t66 * t26;
    const double t225 = (t223 + t122 + t88 + t28) * t66;
    const double t227 = (t16 + t90 + t222 + t225) * t66;
    const double t229 = (t150 + t124 + t119) * t33;
    const double t230 = t66 * t117;
    const double t232 = (t230 + t195 + t107 + t101) * t66;
    const double t233 = t155 * t26;
    const double t235 = (t233 + t230 + t122 + t53 + t28) * t155;
    const double t237 = (t16 + t55 + t229 + t232 + t235) * t155;
    const double t239 = (t216 + t131 + t132) * t33;
    const double t241 = (t223 + t130 + t81 + t19) * t66;
    const double t242 = t66 * t99;
    const double t244 = (t233 + t242 + t130 + t46 + t19) * t155;
    const double t236 = x[5];
    const double t245 = t236 * t4;
    const double t247 = (t245 + t209 + t183 + t170 + t18 + t6) * t236;
    const double t249 = (t3 + t34 + t239 + t241 + t244 + t247) * t236;
    const double t251 = (t2 + t32 + t220 + t227 + t237 + t249) * t236;
    const double t254 = t7 * t127;
    const double t256 = (t254 + t132) * t7;
    const double t258 = (t116 + t256) * t7;
    const double t260 = (t97 + t258) * t7;
    const double t262 = (t131 + t119) * t7;
    const double t264 = (t98 + t262) * t7;
    const double t266 = (t118 + t101) * t7;
    const double t268 = (t56 + t100 + t19) * t33;
    const double t270 = (t16 + t266 + t268) * t33;
    const double t272 = (t15 + t264 + t270) * t33;
    const double t274 = (t124 + t108) * t7;
    const double t275 = t33 * t80;
    const double t277 = (t275 + t107 + t82) * t33;
    const double t279 = (t79 + t274 + t277) * t33;
    const double t280 = t33 * t87;
    const double t282 = (t280 + t107 + t82) * t33;
    const double t284 = (t183 + t275 + t100 + t19) * t66;
    const double t286 = (t16 + t266 + t282 + t284) * t66;
    const double t288 = (t15 + t264 + t279 + t286) * t66;
    const double t290 = (t144 + t108) * t7;
    const double t291 = t33 * t45;
    const double t293 = (t291 + t107 + t47) * t33;
    const double t295 = (t44 + t290 + t293) * t33;
    const double t296 = t7 * t194;
    const double t298 = (t296 + t198) * t7;
    const double t299 = t33 * t106;
    const double t301 = (t299 + t197 + t108) * t33;
    const double t302 = t33 * t143;
    const double t304 = (t201 + t302 + t296 + t119) * t66;
    const double t306 = (t98 + t298 + t301 + t304) * t66;
    const double t307 = t33 * t52;
    const double t309 = (t307 + t107 + t47) * t33;
    const double t310 = t33 * t123;
    const double t312 = (t206 + t310 + t296 + t119) * t66;
    const double t314 = (t209 + t201 + t291 + t100 + t19) * t155;
    const double t316 = (t16 + t266 + t309 + t312 + t314) * t155;
    const double t318 = (t15 + t264 + t295 + t306 + t316) * t155;
    const double t320 = (t130 + t296 + t119) * t33;
    const double t322 = (t98 + t298 + t320) * t33;
    const double t324 = (t302 + t197 + t108) * t33;
    const double t325 = t66 * t45;
    const double t327 = (t325 + t299 + t107 + t47) * t66;
    const double t329 = (t44 + t290 + t324 + t327) * t66;
    const double t331 = (t310 + t197 + t108) * t33;
    const double t332 = t66 * t123;
    const double t333 = t33 * t196;
    const double t335 = (t332 + t333 + t197 + t108) * t66;
    const double t336 = t155 * t80;
    const double t337 = t66 * t143;
    const double t339 = (t336 + t337 + t299 + t107 + t82) * t155;
    const double t341 = (t79 + t274 + t331 + t335 + t339) * t155;
    const double t343 = (t150 + t296 + t119) * t33;
    const double t344 = t66 * t52;
    const double t346 = (t344 + t310 + t107 + t47) * t66;
    const double t347 = t155 * t87;
    const double t348 = t66 * t106;
    const double t350 = (t347 + t348 + t302 + t107 + t82) * t155;
    const double t351 = t236 * t17;
    const double t353 = (t351 + t336 + t325 + t130 + t100 + t19) * t236;
    const double t355 = (t16 + t266 + t343 + t346 + t350 + t353) * t236;
    const double t357 = (t15 + t264 + t322 + t329 + t341 + t355) * t236;
    const double t358 = t7 * t147;
    const double t360 = (t358 + t152) * t7;
    const double t362 = (t116 + t360) * t7;
    const double t364 = (t151 + t119) * t7;
    const double t366 = (t63 + t118 + t28) * t33;
    const double t368 = (t16 + t364 + t366) * t33;
    const double t370 = (t280 + t144 + t82) * t33;
    const double t372 = (t223 + t280 + t118 + t28) * t66;
    const double t374 = (t16 + t364 + t370 + t372) * t66;
    const double t376 = (t307 + t124 + t47) * t33;
    const double t378 = (t230 + t299 + t296 + t101) * t66;
    const double t380 = (t233 + t230 + t307 + t118 + t28) * t155;
    const double t382 = (t16 + t364 + t376 + t378 + t380) * t155;
    const double t384 = (t122 + t296 + t101) * t33;
    const double t386 = (t344 + t299 + t124 + t47) * t66;
    const double t388 = (t347 + t348 + t299 + t144 + t82) * t155;
    const double t389 = t236 * t26;
    const double t391 = (t389 + t347 + t344 + t122 + t118 + t28) * t236;
    const double t393 = (t16 + t364 + t384 + t386 + t388 + t391) * t236;
    const double t395 = (t358 + t132) * t7;
    const double t397 = (t63 + t131 + t19) * t33;
    const double t399 = (t223 + t275 + t131 + t19) * t66;
    const double t401 = (t233 + t242 + t291 + t131 + t19) * t155;
    const double t403 = (t389 + t336 + t325 + t111 + t131 + t19) * t236;
    const double t394 = x[4];
    const double t404 = t394 * t4;
    const double t406 = (t404 + t351 + t209 + t183 + t56 + t254 + t6) * t394;
    const double t408 = (t3 + t395 + t397 + t399 + t401 + t403 + t406) * t394;
    const double t410 = (t2 + t362 + t368 + t374 + t382 + t393 + t408) * t394;
    const double t413 = a[1];
    const double t414 = a[4];
    const double t415 = a[24];
    const double t416 = a[199];
    const double t417 = t7 * t416;
    const double t418 = a[54];
    const double t420 = (t417 + t418) * t7;
    const double t422 = (t415 + t420) * t7;
    const double t424 = (t414 + t422) * t7;
    const double t425 = a[18];
    const double t426 = a[187];
    const double t427 = t7 * t426;
    const double t428 = a[36];
    const double t430 = (t427 + t428) * t7;
    const double t432 = (t425 + t430) * t7;
    const double t433 = a[93];
    const double t434 = t7 * t433;
    const double t436 = (t434 + t428) * t7;
    const double t437 = t33 * t416;
    const double t439 = (t437 + t427 + t418) * t33;
    const double t441 = (t415 + t436 + t439) * t33;
    const double t443 = (t414 + t432 + t441) * t33;
    const double t444 = a[3];
    const double t445 = a[12];
    const double t446 = a[129];
    const double t447 = t7 * t446;
    const double t448 = a[35];
    const double t450 = (t447 + t448) * t7;
    const double t452 = (t445 + t450) * t7;
    const double t453 = a[163];
    const double t454 = t7 * t453;
    const double t455 = a[77];
    const double t457 = (t454 + t455) * t7;
    const double t458 = t33 * t446;
    const double t460 = (t458 + t454 + t448) * t33;
    const double t462 = (t445 + t457 + t460) * t33;
    const double t463 = a[14];
    const double t464 = a[104];
    const double t465 = t7 * t464;
    const double t466 = a[61];
    const double t468 = (t465 + t466) * t7;
    const double t469 = t33 * t464;
    const double t470 = a[175];
    const double t471 = t7 * t470;
    const double t473 = (t469 + t471 + t466) * t33;
    const double t474 = a[113];
    const double t475 = t66 * t474;
    const double t476 = a[120];
    const double t477 = t33 * t476;
    const double t478 = t7 * t476;
    const double t479 = a[65];
    const double t481 = (t475 + t477 + t478 + t479) * t66;
    const double t483 = (t463 + t468 + t473 + t481) * t66;
    const double t485 = (t444 + t452 + t462 + t483) * t66;
    const double t486 = a[202];
    const double t487 = t7 * t486;
    const double t488 = a[81];
    const double t490 = (t487 + t488) * t7;
    const double t491 = t33 * t426;
    const double t493 = (t491 + t487 + t428) * t33;
    const double t495 = (t425 + t490 + t493) * t33;
    const double t496 = a[17];
    const double t497 = a[137];
    const double t498 = t7 * t497;
    const double t499 = a[68];
    const double t501 = (t498 + t499) * t7;
    const double t502 = t33 * t497;
    const double t503 = a[121];
    const double t504 = t7 * t503;
    const double t506 = (t502 + t504 + t499) * t33;
    const double t507 = a[172];
    const double t508 = t66 * t507;
    const double t509 = a[159];
    const double t510 = t33 * t509;
    const double t511 = t7 * t509;
    const double t512 = a[53];
    const double t514 = (t508 + t510 + t511 + t512) * t66;
    const double t516 = (t496 + t501 + t506 + t514) * t66;
    const double t517 = t33 * t433;
    const double t519 = (t517 + t487 + t428) * t33;
    const double t520 = a[116];
    const double t521 = t66 * t520;
    const double t522 = a[109];
    const double t523 = t33 * t522;
    const double t524 = t7 * t522;
    const double t525 = a[48];
    const double t527 = (t521 + t523 + t524 + t525) * t66;
    const double t528 = t155 * t416;
    const double t529 = a[169];
    const double t530 = t529 * t66;
    const double t532 = (t528 + t530 + t491 + t427 + t418) * t155;
    const double t534 = (t415 + t436 + t519 + t527 + t532) * t155;
    const double t536 = (t414 + t432 + t495 + t516 + t534) * t155;
    const double t537 = t529 * t33;
    const double t539 = (t537 + t524 + t525) * t33;
    const double t541 = (t496 + t501 + t539) * t33;
    const double t542 = a[9];
    const double t543 = a[100];
    const double t544 = t7 * t543;
    const double t545 = a[33];
    const double t547 = (t544 + t545) * t7;
    const double t548 = a[122];
    const double t549 = t33 * t548;
    const double t550 = a[114];
    const double t551 = t7 * t550;
    const double t552 = a[62];
    const double t554 = (t549 + t551 + t552) * t33;
    const double t555 = a[89];
    const double t556 = t66 * t555;
    const double t557 = a[188];
    const double t558 = t33 * t557;
    const double t559 = a[158];
    const double t560 = t7 * t559;
    const double t561 = a[32];
    const double t563 = (t556 + t558 + t560 + t561) * t66;
    const double t565 = (t542 + t547 + t554 + t563) * t66;
    const double t567 = (t523 + t504 + t499) * t33;
    const double t568 = a[96];
    const double t569 = t66 * t568;
    const double t570 = a[119];
    const double t571 = t33 * t570;
    const double t573 = (t569 + t571 + t551 + t552) * t66;
    const double t574 = t155 * t446;
    const double t575 = t66 * t548;
    const double t577 = (t574 + t575 + t502 + t454 + t448) * t155;
    const double t579 = (t445 + t457 + t567 + t573 + t577) * t155;
    const double t580 = t33 * t520;
    const double t582 = (t580 + t511 + t512) * t33;
    const double t583 = a[173];
    const double t584 = t66 * t583;
    const double t585 = t33 * t568;
    const double t587 = (t584 + t585 + t560 + t561) * t66;
    const double t588 = t155 * t464;
    const double t589 = t66 * t557;
    const double t591 = (t588 + t589 + t510 + t471 + t466) * t155;
    const double t592 = t236 * t474;
    const double t593 = t155 * t476;
    const double t594 = t33 * t507;
    const double t596 = (t592 + t593 + t556 + t594 + t478 + t479) * t236;
    const double t598 = (t463 + t468 + t582 + t587 + t591 + t596) * t236;
    const double t600 = (t444 + t452 + t541 + t565 + t579 + t598) * t236;
    const double t601 = t529 * t7;
    const double t603 = (t601 + t525) * t7;
    const double t605 = (t496 + t603) * t7;
    const double t607 = (t524 + t499) * t7;
    const double t609 = (t458 + t498 + t448) * t33;
    const double t611 = (t445 + t607 + t609) * t33;
    const double t612 = t7 * t548;
    const double t614 = (t612 + t552) * t7;
    const double t615 = t33 * t543;
    const double t617 = (t615 + t551 + t545) * t33;
    const double t618 = t33 * t559;
    const double t619 = t7 * t557;
    const double t621 = (t556 + t618 + t619 + t561) * t66;
    const double t623 = (t542 + t614 + t617 + t621) * t66;
    const double t624 = t33 * t453;
    const double t626 = (t624 + t504 + t455) * t33;
    const double t627 = t33 * t550;
    const double t628 = t7 * t570;
    const double t630 = (t569 + t627 + t628 + t552) * t66;
    const double t632 = (t574 + t575 + t624 + t498 + t448) * t155;
    const double t634 = (t445 + t607 + t626 + t630 + t632) * t155;
    const double t636 = (t549 + t628 + t552) * t33;
    const double t637 = a[162];
    const double t638 = t66 * t637;
    const double t639 = a[184];
    const double t640 = t33 * t639;
    const double t641 = t7 * t639;
    const double t642 = a[71];
    const double t644 = (t638 + t640 + t641 + t642) * t66;
    const double t645 = t155 * t543;
    const double t646 = t66 * t639;
    const double t648 = (t645 + t646 + t627 + t551 + t545) * t155;
    const double t649 = t236 * t555;
    const double t650 = t155 * t559;
    const double t652 = (t649 + t650 + t638 + t585 + t619 + t561) * t236;
    const double t654 = (t542 + t614 + t636 + t644 + t648 + t652) * t236;
    const double t655 = t7 * t520;
    const double t657 = (t655 + t512) * t7;
    const double t659 = (t469 + t511 + t466) * t33;
    const double t660 = t7 * t568;
    const double t662 = (t584 + t618 + t660 + t561) * t66;
    const double t663 = t33 * t470;
    const double t665 = (t588 + t589 + t663 + t511 + t466) * t155;
    const double t666 = t236 * t583;
    const double t668 = (t666 + t650 + t638 + t558 + t660 + t561) * t236;
    const double t669 = t394 * t474;
    const double t670 = t507 * t7;
    const double t672 = (t669 + t649 + t593 + t556 + t477 + t670 + t479) * t394;
    const double t674 = (t463 + t657 + t659 + t662 + t665 + t668 + t672) * t394;
    const double t676 = (t444 + t605 + t611 + t623 + t634 + t654 + t674) * t394;
    const double t677 = a[6];
    const double t678 = a[16];
    const double t679 = a[117];
    const double t680 = t7 * t679;
    const double t681 = a[76];
    const double t683 = (t680 + t681) * t7;
    const double t685 = (t678 + t683) * t7;
    const double t686 = a[97];
    const double t687 = t7 * t686;
    const double t688 = a[41];
    const double t690 = (t687 + t688) * t7;
    const double t691 = t33 * t679;
    const double t693 = (t691 + t687 + t681) * t33;
    const double t695 = (t678 + t690 + t693) * t33;
    const double t696 = a[22];
    const double t697 = a[125];
    const double t698 = t7 * t697;
    const double t699 = a[40];
    const double t701 = (t698 + t699) * t7;
    const double t702 = t33 * t697;
    const double t703 = a[133];
    const double t704 = t703 * t7;
    const double t706 = (t702 + t704 + t699) * t33;
    const double t707 = a[178];
    const double t708 = t66 * t707;
    const double t709 = a[170];
    const double t710 = t33 * t709;
    const double t711 = t7 * t709;
    const double t712 = a[46];
    const double t714 = (t708 + t710 + t711 + t712) * t66;
    const double t716 = (t696 + t701 + t706 + t714) * t66;
    const double t717 = t33 * t686;
    const double t718 = a[198];
    const double t719 = t7 * t718;
    const double t721 = (t717 + t719 + t688) * t33;
    const double t722 = a[193];
    const double t723 = t66 * t722;
    const double t724 = a[101];
    const double t725 = t33 * t724;
    const double t726 = t7 * t724;
    const double t727 = a[44];
    const double t729 = (t723 + t725 + t726 + t727) * t66;
    const double t730 = t155 * t679;
    const double t731 = a[149];
    const double t732 = t66 * t731;
    const double t734 = (t730 + t732 + t717 + t687 + t681) * t155;
    const double t736 = (t678 + t690 + t721 + t729 + t734) * t155;
    const double t737 = t731 * t33;
    const double t739 = (t737 + t726 + t727) * t33;
    const double t740 = a[164];
    const double t741 = t66 * t740;
    const double t742 = a[166];
    const double t743 = t33 * t742;
    const double t744 = a[130];
    const double t745 = t7 * t744;
    const double t746 = a[50];
    const double t748 = (t741 + t743 + t745 + t746) * t66;
    const double t749 = t155 * t697;
    const double t750 = t66 * t742;
    const double t752 = (t749 + t750 + t725 + t704 + t699) * t155;
    const double t753 = t236 * t707;
    const double t754 = t155 * t709;
    const double t755 = t33 * t722;
    const double t757 = (t753 + t754 + t741 + t755 + t711 + t712) * t236;
    const double t759 = (t696 + t701 + t739 + t748 + t752 + t757) * t236;
    const double t760 = t731 * t7;
    const double t762 = (t760 + t727) * t7;
    const double t764 = (t702 + t726 + t699) * t33;
    const double t765 = t33 * t744;
    const double t766 = t7 * t742;
    const double t768 = (t741 + t765 + t766 + t746) * t66;
    const double t769 = t703 * t33;
    const double t771 = (t749 + t750 + t769 + t726 + t699) * t155;
    const double t772 = t236 * t740;
    const double t773 = t155 * t744;
    const double t774 = a[147];
    const double t775 = t66 * t774;
    const double t777 = (t772 + t773 + t775 + t743 + t766 + t746) * t236;
    const double t778 = t394 * t707;
    const double t779 = t7 * t722;
    const double t781 = (t778 + t772 + t754 + t741 + t710 + t779 + t712) * t394;
    const double t783 = (t696 + t762 + t764 + t768 + t771 + t777 + t781) * t394;
    const double t784 = a[28];
    const double t785 = a[91];
    const double t786 = t7 * t785;
    const double t787 = a[74];
    const double t789 = (t786 + t787) * t7;
    const double t790 = t33 * t785;
    const double t791 = a[156];
    const double t792 = t7 * t791;
    const double t794 = (t790 + t792 + t787) * t33;
    const double t795 = a[84];
    const double t796 = t66 * t795;
    const double t797 = a[150];
    const double t798 = t33 * t797;
    const double t799 = t7 * t797;
    const double t800 = a[38];
    const double t802 = (t796 + t798 + t799 + t800) * t66;
    const double t803 = t155 * t785;
    const double t804 = a[108];
    const double t805 = t804 * t66;
    const double t806 = t33 * t791;
    const double t808 = (t803 + t805 + t806 + t792 + t787) * t155;
    const double t809 = t236 * t795;
    const double t810 = t155 * t797;
    const double t811 = a[182];
    const double t812 = t66 * t811;
    const double t813 = t804 * t33;
    const double t815 = (t809 + t810 + t812 + t813 + t799 + t800) * t236;
    const double t816 = t394 * t795;
    const double t817 = t236 * t811;
    const double t818 = t804 * t7;
    const double t820 = (t816 + t817 + t810 + t812 + t798 + t818 + t800) * t394;
    const double t821 = a[157];
    const double t788 = x[3];
    const double t822 = t788 * t821;
    const double t823 = a[161];
    const double t824 = t394 * t823;
    const double t825 = t236 * t823;
    const double t826 = a[88];
    const double t827 = t826 * t155;
    const double t828 = t66 * t823;
    const double t829 = t33 * t826;
    const double t830 = t7 * t826;
    const double t831 = a[73];
    const double t833 = (t822 + t824 + t825 + t827 + t828 + t829 + t830 + t831) * t788;
    const double t835 = (t784 + t789 + t794 + t802 + t808 + t815 + t820 + t833) * t788;
    const double t837 = (t677 + t685 + t695 + t716 + t736 + t759 + t783 + t835) * t788;
    const double t840 = t33 * t474;
    const double t842 = (t840 + t478 + t479) * t33;
    const double t844 = (t463 + t468 + t842) * t33;
    const double t846 = (t444 + t452 + t844) * t33;
    const double t848 = (t477 + t471 + t466) * t33;
    const double t850 = (t445 + t457 + t848) * t33;
    const double t852 = (t469 + t454 + t448) * t33;
    const double t853 = t66 * t416;
    const double t855 = (t853 + t458 + t427 + t418) * t66;
    const double t857 = (t415 + t436 + t852 + t855) * t66;
    const double t859 = (t414 + t432 + t850 + t857) * t66;
    const double t860 = t33 * t555;
    const double t862 = (t860 + t560 + t561) * t33;
    const double t864 = (t542 + t547 + t862) * t33;
    const double t866 = (t558 + t551 + t552) * t33;
    const double t868 = (t530 + t549 + t524 + t525) * t66;
    const double t870 = (t496 + t501 + t866 + t868) * t66;
    const double t871 = t33 * t583;
    const double t873 = (t871 + t560 + t561) * t33;
    const double t875 = (t521 + t585 + t511 + t512) * t66;
    const double t876 = t155 * t474;
    const double t878 = (t876 + t508 + t860 + t478 + t479) * t155;
    const double t880 = (t463 + t468 + t873 + t875 + t878) * t155;
    const double t882 = (t444 + t452 + t864 + t870 + t880) * t155;
    const double t884 = (t594 + t511 + t512) * t33;
    const double t886 = (t496 + t501 + t884) * t33;
    const double t888 = (t510 + t504 + t499) * t33;
    const double t889 = t66 * t426;
    const double t891 = (t889 + t502 + t487 + t428) * t66;
    const double t893 = (t425 + t490 + t888 + t891) * t66;
    const double t895 = (t585 + t551 + t552) * t33;
    const double t896 = t66 * t522;
    const double t898 = (t896 + t571 + t504 + t499) * t66;
    const double t899 = t66 * t509;
    const double t901 = (t593 + t899 + t558 + t471 + t466) * t155;
    const double t903 = (t445 + t457 + t895 + t898 + t901) * t155;
    const double t905 = (t580 + t524 + t525) * t33;
    const double t906 = t66 * t433;
    const double t908 = (t906 + t523 + t487 + t428) * t66;
    const double t909 = t66 * t497;
    const double t911 = (t588 + t909 + t549 + t454 + t448) * t155;
    const double t912 = t236 * t416;
    const double t914 = (t912 + t574 + t889 + t537 + t427 + t418) * t236;
    const double t916 = (t415 + t436 + t905 + t908 + t911 + t914) * t236;
    const double t918 = (t414 + t432 + t886 + t893 + t903 + t916) * t236;
    const double t920 = (t860 + t619 + t561) * t33;
    const double t922 = (t542 + t614 + t920) * t33;
    const double t924 = (t618 + t551 + t545) * t33;
    const double t925 = t66 * t446;
    const double t927 = (t925 + t615 + t498 + t448) * t66;
    const double t929 = (t445 + t607 + t924 + t927) * t66;
    const double t930 = t33 * t637;
    const double t932 = (t930 + t641 + t642) * t33;
    const double t934 = (t575 + t640 + t628 + t552) * t66;
    const double t935 = t155 * t555;
    const double t937 = (t935 + t569 + t930 + t619 + t561) * t155;
    const double t939 = (t542 + t614 + t932 + t934 + t937) * t155;
    const double t941 = (t585 + t628 + t552) * t33;
    const double t942 = t66 * t453;
    const double t944 = (t942 + t627 + t504 + t455) * t66;
    const double t945 = t66 * t550;
    const double t947 = (t650 + t945 + t640 + t551 + t545) * t155;
    const double t948 = t236 * t446;
    const double t950 = (t948 + t645 + t942 + t549 + t498 + t448) * t236;
    const double t952 = (t445 + t607 + t941 + t944 + t947 + t950) * t236;
    const double t954 = (t871 + t660 + t561) * t33;
    const double t955 = t66 * t464;
    const double t957 = (t955 + t618 + t511 + t466) * t66;
    const double t958 = t155 * t583;
    const double t960 = (t958 + t589 + t930 + t660 + t561) * t155;
    const double t961 = t236 * t464;
    const double t962 = t66 * t470;
    const double t964 = (t961 + t650 + t962 + t558 + t511 + t466) * t236;
    const double t965 = t236 * t476;
    const double t966 = t476 * t66;
    const double t968 = (t669 + t965 + t935 + t966 + t860 + t670 + t479) * t394;
    const double t970 = (t463 + t657 + t954 + t957 + t960 + t964 + t968) * t394;
    const double t972 = (t444 + t605 + t922 + t929 + t939 + t952 + t970) * t394;
    const double t973 = a[8];
    const double t974 = a[10];
    const double t975 = a[194];
    const double t976 = t7 * t975;
    const double t977 = a[58];
    const double t979 = (t976 + t977) * t7;
    const double t981 = (t974 + t979) * t7;
    const double t982 = a[23];
    const double t983 = a[94];
    const double t984 = t7 * t983;
    const double t985 = a[39];
    const double t987 = (t984 + t985) * t7;
    const double t988 = a[153];
    const double t989 = t33 * t988;
    const double t990 = a[105];
    const double t991 = t7 * t990;
    const double t992 = a[49];
    const double t994 = (t989 + t991 + t992) * t33;
    const double t996 = (t982 + t987 + t994) * t33;
    const double t997 = a[154];
    const double t998 = t33 * t997;
    const double t999 = a[140];
    const double t1000 = t7 * t999;
    const double t1001 = a[70];
    const double t1003 = (t998 + t1000 + t1001) * t33;
    const double t1004 = t66 * t988;
    const double t1006 = (t1004 + t998 + t991 + t992) * t66;
    const double t1008 = (t982 + t987 + t1003 + t1006) * t66;
    const double t1009 = a[86];
    const double t1010 = t33 * t1009;
    const double t1011 = a[151];
    const double t1012 = t7 * t1011;
    const double t1013 = a[43];
    const double t1015 = (t1010 + t1012 + t1013) * t33;
    const double t1016 = a[167];
    const double t1017 = t66 * t1016;
    const double t1018 = a[106];
    const double t1019 = t33 * t1018;
    const double t1020 = a[179];
    const double t1021 = t7 * t1020;
    const double t1022 = a[42];
    const double t1024 = (t1017 + t1019 + t1021 + t1022) * t66;
    const double t1025 = t155 * t988;
    const double t1027 = (t1025 + t1017 + t1010 + t991 + t992) * t155;
    const double t1029 = (t982 + t987 + t1015 + t1024 + t1027) * t155;
    const double t1030 = t33 * t1016;
    const double t1032 = (t1030 + t1021 + t1022) * t33;
    const double t1033 = t66 * t1009;
    const double t1035 = (t1033 + t1019 + t1012 + t1013) * t66;
    const double t1036 = t155 * t997;
    const double t1037 = t66 * t1018;
    const double t1039 = (t1036 + t1037 + t1019 + t1000 + t1001) * t155;
    const double t1040 = t236 * t988;
    const double t1042 = (t1040 + t1036 + t1033 + t1030 + t991 + t992) * t236;
    const double t1044 = (t982 + t987 + t1032 + t1035 + t1039 + t1042) * t236;
    const double t1045 = a[15];
    const double t1046 = a[144];
    const double t1047 = t1046 * t7;
    const double t1048 = a[78];
    const double t1050 = (t1047 + t1048) * t7;
    const double t1051 = a[135];
    const double t1052 = t33 * t1051;
    const double t1053 = a[92];
    const double t1054 = t7 * t1053;
    const double t1055 = a[64];
    const double t1057 = (t1052 + t1054 + t1055) * t33;
    const double t1058 = t66 * t1051;
    const double t1059 = a[138];
    const double t1060 = t33 * t1059;
    const double t1062 = (t1058 + t1060 + t1054 + t1055) * t66;
    const double t1063 = t155 * t1051;
    const double t1064 = a[118];
    const double t1065 = t66 * t1064;
    const double t1066 = a[185];
    const double t1067 = t33 * t1066;
    const double t1069 = (t1063 + t1065 + t1067 + t1054 + t1055) * t155;
    const double t1070 = t236 * t1051;
    const double t1071 = t155 * t1059;
    const double t1072 = t66 * t1066;
    const double t1073 = t33 * t1064;
    const double t1075 = (t1070 + t1071 + t1072 + t1073 + t1054 + t1055) * t236;
    const double t1076 = a[204];
    const double t1077 = t394 * t1076;
    const double t1078 = a[83];
    const double t1079 = t236 * t1078;
    const double t1080 = t155 * t1078;
    const double t1081 = t66 * t1078;
    const double t1082 = t33 * t1078;
    const double t1083 = a[115];
    const double t1084 = t7 * t1083;
    const double t1085 = a[52];
    const double t1087 = (t1077 + t1079 + t1080 + t1081 + t1082 + t1084 + t1085) * t394;
    const double t1089 = (t1045 + t1050 + t1057 + t1062 + t1069 + t1075 + t1087) * t394;
    const double t1090 = a[19];
    const double t1091 = a[82];
    const double t1092 = t7 * t1091;
    const double t1093 = a[63];
    const double t1095 = (t1092 + t1093) * t7;
    const double t1096 = a[142];
    const double t1097 = t33 * t1096;
    const double t1098 = a[95];
    const double t1099 = t7 * t1098;
    const double t1100 = a[37];
    const double t1102 = (t1097 + t1099 + t1100) * t33;
    const double t1103 = a[107];
    const double t1104 = t66 * t1103;
    const double t1105 = a[146];
    const double t1106 = t33 * t1105;
    const double t1107 = a[98];
    const double t1108 = t7 * t1107;
    const double t1109 = a[55];
    const double t1111 = (t1104 + t1106 + t1108 + t1109) * t66;
    const double t1112 = t155 * t1096;
    const double t1113 = a[168];
    const double t1114 = t66 * t1113;
    const double t1115 = a[90];
    const double t1116 = t33 * t1115;
    const double t1118 = (t1112 + t1114 + t1116 + t1099 + t1100) * t155;
    const double t1119 = t236 * t1103;
    const double t1120 = t155 * t1105;
    const double t1121 = a[127];
    const double t1122 = t66 * t1121;
    const double t1123 = t33 * t1113;
    const double t1125 = (t1119 + t1120 + t1122 + t1123 + t1108 + t1109) * t236;
    const double t1126 = a[195];
    const double t1127 = t394 * t1126;
    const double t1128 = a[180];
    const double t1129 = t236 * t1128;
    const double t1130 = a[110];
    const double t1131 = t155 * t1130;
    const double t1132 = t66 * t1128;
    const double t1133 = t33 * t1130;
    const double t1134 = a[189];
    const double t1135 = t1134 * t7;
    const double t1136 = a[69];
    const double t1138 = (t1127 + t1129 + t1131 + t1132 + t1133 + t1135 + t1136) * t394;
    const double t1139 = a[190];
    const double t1140 = t1139 * t788;
    const double t1141 = a[201];
    const double t1142 = t394 * t1141;
    const double t1143 = a[134];
    const double t1144 = t236 * t1143;
    const double t1145 = a[139];
    const double t1146 = t1145 * t155;
    const double t1147 = t66 * t1143;
    const double t1148 = t33 * t1145;
    const double t1149 = a[183];
    const double t1150 = t7 * t1149;
    const double t1151 = a[59];
    const double t1153 = (t1140 + t1142 + t1144 + t1146 + t1147 + t1148 + t1150 + t1151) * t788;
    const double t1155 = (t1090 + t1095 + t1102 + t1111 + t1118 + t1125 + t1138 + t1153) * t788;
    const double t1157 = (t973 + t981 + t996 + t1008 + t1029 + t1044 + t1089 + t1155) * t788;
    const double t1158 = t33 * t707;
    const double t1160 = (t1158 + t711 + t712) * t33;
    const double t1162 = (t696 + t701 + t1160) * t33;
    const double t1164 = (t710 + t704 + t699) * t33;
    const double t1165 = t66 * t679;
    const double t1167 = (t1165 + t702 + t687 + t681) * t66;
    const double t1169 = (t678 + t690 + t1164 + t1167) * t66;
    const double t1170 = t33 * t740;
    const double t1172 = (t1170 + t745 + t746) * t33;
    const double t1174 = (t732 + t743 + t726 + t727) * t66;
    const double t1175 = t155 * t707;
    const double t1177 = (t1175 + t723 + t1170 + t711 + t712) * t155;
    const double t1179 = (t696 + t701 + t1172 + t1174 + t1177) * t155;
    const double t1181 = (t755 + t726 + t727) * t33;
    const double t1182 = t66 * t686;
    const double t1184 = (t1182 + t725 + t719 + t688) * t66;
    const double t1185 = t66 * t724;
    const double t1187 = (t754 + t1185 + t743 + t704 + t699) * t155;
    const double t1188 = t236 * t679;
    const double t1190 = (t1188 + t749 + t1182 + t737 + t687 + t681) * t236;
    const double t1192 = (t678 + t690 + t1181 + t1184 + t1187 + t1190) * t236;
    const double t1194 = (t1170 + t766 + t746) * t33;
    const double t1195 = t66 * t697;
    const double t1197 = (t1195 + t765 + t726 + t699) * t66;
    const double t1198 = t155 * t740;
    const double t1199 = t33 * t774;
    const double t1201 = (t1198 + t750 + t1199 + t766 + t746) * t155;
    const double t1202 = t236 * t697;
    const double t1203 = t703 * t66;
    const double t1205 = (t1202 + t773 + t1203 + t743 + t726 + t699) * t236;
    const double t1206 = t236 * t709;
    const double t1207 = t66 * t709;
    const double t1209 = (t778 + t1206 + t1198 + t1207 + t1170 + t779 + t712) * t394;
    const double t1211 = (t696 + t762 + t1194 + t1197 + t1201 + t1205 + t1209) * t394;
    const double t1212 = t33 * t1103;
    const double t1214 = (t1212 + t1108 + t1109) * t33;
    const double t1215 = t66 * t1096;
    const double t1217 = (t1215 + t1106 + t1099 + t1100) * t66;
    const double t1218 = t155 * t1103;
    const double t1219 = t33 * t1121;
    const double t1221 = (t1218 + t1114 + t1219 + t1108 + t1109) * t155;
    const double t1222 = t236 * t1096;
    const double t1223 = t66 * t1115;
    const double t1225 = (t1222 + t1120 + t1223 + t1123 + t1099 + t1100) * t236;
    const double t1226 = t236 * t1130;
    const double t1227 = t155 * t1128;
    const double t1228 = t66 * t1130;
    const double t1229 = t33 * t1128;
    const double t1231 = (t1127 + t1226 + t1227 + t1228 + t1229 + t1135 + t1136) * t394;
    const double t1232 = a[152];
    const double t1233 = t788 * t1232;
    const double t1234 = a[186];
    const double t1235 = t394 * t1234;
    const double t1236 = a[143];
    const double t1237 = t236 * t1236;
    const double t1238 = t155 * t1236;
    const double t1239 = t66 * t1236;
    const double t1240 = t33 * t1236;
    const double t1241 = a[205];
    const double t1242 = t7 * t1241;
    const double t1243 = a[67];
    const double t1245 = (t1233 + t1235 + t1237 + t1238 + t1239 + t1240 + t1242 + t1243) * t788;
    const double t1247 = (t1090 + t1095 + t1214 + t1217 + t1221 + t1225 + t1231 + t1245) * t788;
    const double t1248 = t33 * t795;
    const double t1250 = (t1248 + t799 + t800) * t33;
    const double t1251 = t66 * t785;
    const double t1253 = (t1251 + t798 + t792 + t787) * t66;
    const double t1254 = t155 * t795;
    const double t1255 = t33 * t811;
    const double t1257 = (t1254 + t805 + t1255 + t799 + t800) * t155;
    const double t1258 = t236 * t785;
    const double t1259 = t66 * t791;
    const double t1261 = (t1258 + t810 + t1259 + t813 + t792 + t787) * t236;
    const double t1262 = t236 * t797;
    const double t1263 = t155 * t811;
    const double t1264 = t66 * t797;
    const double t1266 = (t816 + t1262 + t1263 + t1264 + t1255 + t818 + t800) * t394;
    const double t1267 = t1145 * t236;
    const double t1268 = t1143 * t155;
    const double t1269 = t1145 * t66;
    const double t1270 = t33 * t1143;
    const double t1272 = (t1233 + t1142 + t1267 + t1268 + t1269 + t1270 + t1150 + t1151) * t788;
    const double t1246 = x[2];
    const double t1273 = t1246 * t821;
    const double t1274 = t236 * t826;
    const double t1275 = t155 * t823;
    const double t1276 = t66 * t826;
    const double t1277 = t33 * t823;
    const double t1279 = (t1273 + t1140 + t824 + t1274 + t1275 + t1276 + t1277 + t830 + t831) * t1246;
    const double t1281 = (t784 + t789 + t1250 + t1253 + t1257 + t1261 + t1266 + t1272 + t1279) * t1246;
    const double t1283 = (t677 + t685 + t1162 + t1169 + t1179 + t1192 + t1211 + t1247 + t1281) * t1246;
    const double t1286 = t7 * t474;
    const double t1288 = (t1286 + t479) * t7;
    const double t1290 = (t463 + t1288) * t7;
    const double t1292 = (t444 + t1290) * t7;
    const double t1294 = (t478 + t466) * t7;
    const double t1296 = (t445 + t1294) * t7;
    const double t1298 = (t465 + t448) * t7;
    const double t1300 = (t437 + t447 + t418) * t33;
    const double t1302 = (t415 + t1298 + t1300) * t33;
    const double t1304 = (t414 + t1296 + t1302) * t33;
    const double t1306 = (t471 + t455) * t7;
    const double t1308 = (t491 + t454 + t428) * t33;
    const double t1310 = (t425 + t1306 + t1308) * t33;
    const double t1312 = (t517 + t454 + t428) * t33;
    const double t1314 = (t853 + t491 + t447 + t418) * t66;
    const double t1316 = (t415 + t1298 + t1312 + t1314) * t66;
    const double t1318 = (t414 + t1296 + t1310 + t1316) * t66;
    const double t1319 = t7 * t555;
    const double t1321 = (t1319 + t561) * t7;
    const double t1323 = (t542 + t1321) * t7;
    const double t1325 = (t560 + t545) * t7;
    const double t1327 = (t458 + t544 + t448) * t33;
    const double t1329 = (t445 + t1325 + t1327) * t33;
    const double t1331 = (t619 + t552) * t7;
    const double t1333 = (t502 + t551 + t499) * t33;
    const double t1335 = (t530 + t523 + t612 + t525) * t66;
    const double t1337 = (t496 + t1331 + t1333 + t1335) * t66;
    const double t1338 = t7 * t583;
    const double t1340 = (t1338 + t561) * t7;
    const double t1342 = (t469 + t560 + t466) * t33;
    const double t1344 = (t521 + t510 + t660 + t512) * t66;
    const double t1346 = (t876 + t508 + t477 + t1319 + t479) * t155;
    const double t1348 = (t463 + t1340 + t1342 + t1344 + t1346) * t155;
    const double t1350 = (t444 + t1323 + t1329 + t1337 + t1348) * t155;
    const double t1352 = (t537 + t612 + t525) * t33;
    const double t1354 = (t496 + t1331 + t1352) * t33;
    const double t1356 = (t523 + t551 + t499) * t33;
    const double t1358 = (t925 + t502 + t544 + t448) * t66;
    const double t1360 = (t445 + t1325 + t1356 + t1358) * t66;
    const double t1361 = t7 * t637;
    const double t1363 = (t1361 + t642) * t7;
    const double t1365 = (t549 + t641 + t552) * t33;
    const double t1367 = (t575 + t571 + t641 + t552) * t66;
    const double t1369 = (t935 + t569 + t558 + t1361 + t561) * t155;
    const double t1371 = (t542 + t1363 + t1365 + t1367 + t1369) * t155;
    const double t1373 = (t580 + t660 + t512) * t33;
    const double t1375 = (t955 + t510 + t560 + t466) * t66;
    const double t1377 = (t958 + t589 + t585 + t1361 + t561) * t155;
    const double t1379 = (t592 + t935 + t966 + t594 + t1319 + t479) * t236;
    const double t1381 = (t463 + t1340 + t1373 + t1375 + t1377 + t1379) * t236;
    const double t1383 = (t444 + t1323 + t1354 + t1360 + t1371 + t1381) * t236;
    const double t1385 = (t670 + t512) * t7;
    const double t1387 = (t496 + t1385) * t7;
    const double t1389 = (t511 + t499) * t7;
    const double t1391 = (t491 + t498 + t428) * t33;
    const double t1393 = (t425 + t1389 + t1391) * t33;
    const double t1394 = t33 * t486;
    const double t1396 = (t1394 + t504 + t488) * t33;
    const double t1398 = (t889 + t1394 + t498 + t428) * t66;
    const double t1400 = (t425 + t1389 + t1396 + t1398) * t66;
    const double t1402 = (t660 + t552) * t7;
    const double t1404 = (t624 + t551 + t455) * t33;
    const double t1405 = t33 * t503;
    const double t1407 = (t896 + t1405 + t628 + t499) * t66;
    const double t1409 = (t593 + t899 + t663 + t619 + t466) * t155;
    const double t1411 = (t445 + t1402 + t1404 + t1407 + t1409) * t155;
    const double t1413 = (t523 + t628 + t499) * t33;
    const double t1415 = (t942 + t1405 + t551 + t455) * t66;
    const double t1417 = (t650 + t945 + t627 + t641 + t545) * t155;
    const double t1419 = (t965 + t650 + t962 + t510 + t619 + t466) * t236;
    const double t1421 = (t445 + t1402 + t1413 + t1415 + t1417 + t1419) * t236;
    const double t1423 = (t655 + t525) * t7;
    const double t1425 = (t517 + t524 + t428) * t33;
    const double t1427 = (t906 + t1394 + t524 + t428) * t66;
    const double t1429 = (t588 + t909 + t624 + t612 + t448) * t155;
    const double t1431 = (t961 + t645 + t942 + t502 + t612 + t448) * t236;
    const double t1432 = t394 * t416;
    const double t1434 = (t1432 + t948 + t574 + t889 + t491 + t601 + t418) * t394;
    const double t1436 = (t415 + t1423 + t1425 + t1427 + t1429 + t1431 + t1434) * t394;
    const double t1438 = (t414 + t1387 + t1393 + t1400 + t1411 + t1421 + t1436) * t394;
    const double t1439 = t7 * t988;
    const double t1441 = (t1439 + t992) * t7;
    const double t1443 = (t982 + t1441) * t7;
    const double t1445 = (t991 + t985) * t7;
    const double t1446 = t33 * t975;
    const double t1448 = (t1446 + t984 + t977) * t33;
    const double t1450 = (t974 + t1445 + t1448) * t33;
    const double t1451 = t7 * t997;
    const double t1453 = (t1451 + t1001) * t7;
    const double t1454 = t33 * t983;
    const double t1456 = (t1454 + t1000 + t985) * t33;
    const double t1457 = t33 * t990;
    const double t1459 = (t1004 + t1457 + t1451 + t992) * t66;
    const double t1461 = (t982 + t1453 + t1456 + t1459) * t66;
    const double t1462 = t7 * t1009;
    const double t1464 = (t1462 + t1013) * t7;
    const double t1466 = (t1454 + t1012 + t985) * t33;
    const double t1467 = t33 * t1020;
    const double t1468 = t7 * t1018;
    const double t1470 = (t1017 + t1467 + t1468 + t1022) * t66;
    const double t1472 = (t1025 + t1017 + t1457 + t1462 + t992) * t155;
    const double t1474 = (t982 + t1464 + t1466 + t1470 + t1472) * t155;
    const double t1475 = t7 * t1051;
    const double t1477 = (t1475 + t1055) * t7;
    const double t1478 = t1046 * t33;
    const double t1480 = (t1478 + t1054 + t1048) * t33;
    const double t1481 = t33 * t1053;
    const double t1482 = t7 * t1059;
    const double t1484 = (t1058 + t1481 + t1482 + t1055) * t66;
    const double t1485 = t1066 * t7;
    const double t1487 = (t1063 + t1065 + t1481 + t1485 + t1055) * t155;
    const double t1488 = t236 * t1076;
    const double t1489 = t33 * t1083;
    const double t1490 = t7 * t1078;
    const double t1492 = (t1488 + t1080 + t1081 + t1489 + t1490 + t1085) * t236;
    const double t1494 = (t1045 + t1477 + t1480 + t1484 + t1487 + t1492) * t236;
    const double t1495 = t7 * t1016;
    const double t1497 = (t1495 + t1022) * t7;
    const double t1499 = (t1454 + t1021 + t985) * t33;
    const double t1500 = t33 * t1011;
    const double t1502 = (t1033 + t1500 + t1468 + t1013) * t66;
    const double t1503 = t33 * t999;
    const double t1505 = (t1036 + t1037 + t1503 + t1468 + t1001) * t155;
    const double t1506 = t7 * t1064;
    const double t1508 = (t1079 + t1071 + t1072 + t1481 + t1506 + t1055) * t236;
    const double t1509 = t394 * t988;
    const double t1511 = (t1509 + t1070 + t1036 + t1033 + t1457 + t1495 + t992) * t394;
    const double t1513 = (t982 + t1497 + t1499 + t1502 + t1505 + t1508 + t1511) * t394;
    const double t1514 = t7 * t1096;
    const double t1516 = (t1514 + t1100) * t7;
    const double t1517 = t33 * t1091;
    const double t1519 = (t1517 + t1099 + t1093) * t33;
    const double t1520 = t33 * t1107;
    const double t1521 = t7 * t1105;
    const double t1523 = (t1104 + t1520 + t1521 + t1109) * t66;
    const double t1524 = t33 * t1098;
    const double t1525 = t7 * t1115;
    const double t1527 = (t1112 + t1114 + t1524 + t1525 + t1100) * t155;
    const double t1528 = t236 * t1126;
    const double t1529 = t1134 * t33;
    const double t1530 = t7 * t1130;
    const double t1532 = (t1528 + t1131 + t1132 + t1529 + t1530 + t1136) * t236;
    const double t1533 = t394 * t1103;
    const double t1534 = t7 * t1113;
    const double t1536 = (t1533 + t1129 + t1120 + t1122 + t1520 + t1534 + t1109) * t394;
    const double t1537 = t394 * t1143;
    const double t1538 = t1141 * t236;
    const double t1539 = t1149 * t33;
    const double t1540 = t7 * t1145;
    const double t1542 = (t1140 + t1537 + t1538 + t1146 + t1147 + t1539 + t1540 + t1151) * t788;
    const double t1544 = (t1090 + t1516 + t1519 + t1523 + t1527 + t1532 + t1536 + t1542) * t788;
    const double t1546 = (t973 + t1443 + t1450 + t1461 + t1474 + t1494 + t1513 + t1544) * t788;
    const double t1548 = (t989 + t1451 + t992) * t33;
    const double t1550 = (t982 + t1453 + t1548) * t33;
    const double t1552 = (t1457 + t1000 + t985) * t33;
    const double t1553 = t66 * t975;
    const double t1555 = (t1553 + t1454 + t984 + t977) * t66;
    const double t1557 = (t974 + t1445 + t1552 + t1555) * t66;
    const double t1559 = (t1052 + t1482 + t1055) * t33;
    const double t1560 = t66 * t1046;
    const double t1562 = (t1560 + t1481 + t1054 + t1048) * t66;
    const double t1563 = t155 * t1076;
    const double t1564 = t66 * t1083;
    const double t1566 = (t1563 + t1564 + t1082 + t1490 + t1085) * t155;
    const double t1568 = (t1045 + t1477 + t1559 + t1562 + t1566) * t155;
    const double t1570 = (t1030 + t1468 + t1022) * t33;
    const double t1571 = t66 * t983;
    const double t1573 = (t1571 + t1467 + t1012 + t985) * t66;
    const double t1574 = t66 * t1053;
    const double t1576 = (t1080 + t1574 + t1073 + t1485 + t1055) * t155;
    const double t1577 = t66 * t990;
    const double t1579 = (t1040 + t1063 + t1577 + t1030 + t1462 + t992) * t236;
    const double t1581 = (t982 + t1464 + t1570 + t1573 + t1576 + t1579) * t236;
    const double t1583 = (t1010 + t1468 + t1013) * t33;
    const double t1585 = (t1571 + t1500 + t1021 + t985) * t66;
    const double t1587 = (t1080 + t1574 + t1067 + t1506 + t1055) * t155;
    const double t1588 = t236 * t997;
    const double t1589 = t999 * t66;
    const double t1591 = (t1588 + t1071 + t1589 + t1019 + t1468 + t1001) * t236;
    const double t1593 = (t1509 + t1588 + t1063 + t1577 + t1010 + t1495 + t992) * t394;
    const double t1595 = (t982 + t1497 + t1583 + t1585 + t1587 + t1591 + t1593) * t394;
    const double t1596 = a[25];
    const double t1597 = a[141];
    const double t1598 = t7 * t1597;
    const double t1599 = a[75];
    const double t1601 = (t1598 + t1599) * t7;
    const double t1602 = t33 * t1597;
    const double t1603 = a[192];
    const double t1604 = t7 * t1603;
    const double t1606 = (t1602 + t1604 + t1599) * t33;
    const double t1607 = t66 * t1597;
    const double t1608 = t33 * t1603;
    const double t1610 = (t1607 + t1608 + t1604 + t1599) * t66;
    const double t1611 = a[197];
    const double t1612 = t155 * t1611;
    const double t1613 = a[177];
    const double t1614 = t1613 * t66;
    const double t1615 = a[181];
    const double t1616 = t33 * t1615;
    const double t1617 = t7 * t1615;
    const double t1618 = a[60];
    const double t1620 = (t1612 + t1614 + t1616 + t1617 + t1618) * t155;
    const double t1621 = t236 * t1611;
    const double t1622 = a[155];
    const double t1623 = t155 * t1622;
    const double t1624 = t1615 * t66;
    const double t1625 = t1613 * t33;
    const double t1627 = (t1621 + t1623 + t1624 + t1625 + t1617 + t1618) * t236;
    const double t1628 = t394 * t1611;
    const double t1629 = t236 * t1622;
    const double t1630 = t1613 * t7;
    const double t1632 = (t1628 + t1629 + t1623 + t1624 + t1616 + t1630 + t1618) * t394;
    const double t1633 = a[196];
    const double t1634 = t1633 * t788;
    const double t1635 = a[171];
    const double t1636 = t394 * t1635;
    const double t1637 = t236 * t1635;
    const double t1638 = a[111];
    const double t1639 = t1638 * t155;
    const double t1640 = a[123];
    const double t1641 = t1640 * t66;
    const double t1642 = a[124];
    const double t1643 = t33 * t1642;
    const double t1644 = t7 * t1642;
    const double t1645 = a[72];
    const double t1647 = (t1634 + t1636 + t1637 + t1639 + t1641 + t1643 + t1644 + t1645) * t788;
    const double t1649 = (t1596 + t1601 + t1606 + t1610 + t1620 + t1627 + t1632 + t1647) * t788;
    const double t1651 = (t1212 + t1521 + t1109) * t33;
    const double t1652 = t66 * t1091;
    const double t1654 = (t1652 + t1520 + t1099 + t1093) * t66;
    const double t1655 = t155 * t1126;
    const double t1656 = t1134 * t66;
    const double t1658 = (t1655 + t1656 + t1229 + t1530 + t1136) * t155;
    const double t1659 = t66 * t1098;
    const double t1661 = (t1222 + t1131 + t1659 + t1123 + t1525 + t1100) * t236;
    const double t1662 = t236 * t1105;
    const double t1663 = t66 * t1107;
    const double t1665 = (t1533 + t1662 + t1227 + t1663 + t1219 + t1534 + t1109) * t394;
    const double t1666 = a[132];
    const double t1667 = t1666 * t788;
    const double t1668 = t1638 * t236;
    const double t1669 = t155 * t1635;
    const double t1670 = t1642 * t66;
    const double t1671 = t1640 * t33;
    const double t1673 = (t1667 + t1636 + t1668 + t1669 + t1670 + t1671 + t1644 + t1645) * t788;
    const double t1674 = t1246 * t1139;
    const double t1675 = t155 * t1141;
    const double t1676 = t66 * t1149;
    const double t1678 = (t1674 + t1634 + t1537 + t1267 + t1675 + t1676 + t1270 + t1540 + t1151) * t1246;
    const double t1680 = (t1090 + t1516 + t1651 + t1654 + t1658 + t1661 + t1665 + t1673 + t1678) * t1246;
    const double t1682 = (t973 + t1443 + t1550 + t1557 + t1568 + t1581 + t1595 + t1649 + t1680) * t1246;
    const double t1683 = t7 * t707;
    const double t1685 = (t1683 + t712) * t7;
    const double t1687 = (t696 + t1685) * t7;
    const double t1689 = (t711 + t699) * t7;
    const double t1691 = (t691 + t698 + t681) * t33;
    const double t1693 = (t678 + t1689 + t1691) * t33;
    const double t1695 = (t717 + t704 + t688) * t33;
    const double t1697 = (t1165 + t717 + t698 + t681) * t66;
    const double t1699 = (t678 + t1689 + t1695 + t1697) * t66;
    const double t1700 = t7 * t740;
    const double t1702 = (t1700 + t746) * t7;
    const double t1704 = (t702 + t745 + t699) * t33;
    const double t1706 = (t732 + t725 + t766 + t727) * t66;
    const double t1708 = (t1175 + t723 + t710 + t1700 + t712) * t155;
    const double t1710 = (t696 + t1702 + t1704 + t1706 + t1708) * t155;
    const double t1712 = (t737 + t766 + t727) * t33;
    const double t1714 = (t1195 + t725 + t745 + t699) * t66;
    const double t1715 = t7 * t774;
    const double t1717 = (t1198 + t750 + t743 + t1715 + t746) * t155;
    const double t1719 = (t753 + t1198 + t1207 + t755 + t1700 + t712) * t236;
    const double t1721 = (t696 + t1702 + t1712 + t1714 + t1717 + t1719) * t236;
    const double t1723 = (t779 + t727) * t7;
    const double t1725 = (t717 + t726 + t688) * t33;
    const double t1726 = t33 * t718;
    const double t1728 = (t1182 + t1726 + t726 + t688) * t66;
    const double t1730 = (t754 + t1185 + t769 + t766 + t699) * t155;
    const double t1732 = (t1206 + t773 + t1203 + t725 + t766 + t699) * t236;
    const double t1733 = t394 * t679;
    const double t1735 = (t1733 + t1202 + t749 + t1182 + t717 + t760 + t681) * t394;
    const double t1737 = (t678 + t1723 + t1725 + t1728 + t1730 + t1732 + t1735) * t394;
    const double t1738 = t7 * t1103;
    const double t1740 = (t1738 + t1109) * t7;
    const double t1742 = (t1517 + t1108 + t1093) * t33;
    const double t1744 = (t1215 + t1524 + t1521 + t1100) * t66;
    const double t1745 = t7 * t1121;
    const double t1747 = (t1218 + t1114 + t1520 + t1745 + t1109) * t155;
    const double t1748 = t7 * t1128;
    const double t1750 = (t1528 + t1227 + t1228 + t1529 + t1748 + t1136) * t236;
    const double t1751 = t394 * t1096;
    const double t1753 = (t1751 + t1226 + t1120 + t1223 + t1524 + t1534 + t1100) * t394;
    const double t1754 = t394 * t1236;
    const double t1755 = t236 * t1234;
    const double t1756 = t33 * t1241;
    const double t1757 = t7 * t1236;
    const double t1759 = (t1233 + t1754 + t1755 + t1238 + t1239 + t1756 + t1757 + t1243) * t788;
    const double t1761 = (t1090 + t1740 + t1742 + t1744 + t1747 + t1750 + t1753 + t1759) * t788;
    const double t1763 = (t1097 + t1521 + t1100) * t33;
    const double t1765 = (t1652 + t1524 + t1108 + t1093) * t66;
    const double t1767 = (t1655 + t1656 + t1133 + t1748 + t1136) * t155;
    const double t1769 = (t1119 + t1227 + t1663 + t1123 + t1745 + t1109) * t236;
    const double t1771 = (t1751 + t1662 + t1131 + t1659 + t1116 + t1534 + t1100) * t394;
    const double t1772 = t1638 * t394;
    const double t1773 = t1640 * t7;
    const double t1775 = (t1667 + t1772 + t1637 + t1669 + t1670 + t1643 + t1773 + t1645) * t788;
    const double t1776 = t1246 * t1232;
    const double t1777 = t155 * t1234;
    const double t1778 = t66 * t1241;
    const double t1780 = (t1776 + t1667 + t1754 + t1237 + t1777 + t1778 + t1240 + t1757 + t1243) * t1246;
    const double t1782 = (t1090 + t1740 + t1763 + t1765 + t1767 + t1769 + t1771 + t1775 + t1780) * t1246;
    const double t1783 = t795 * t7;
    const double t1785 = (t1783 + t800) * t7;
    const double t1787 = (t790 + t799 + t787) * t33;
    const double t1789 = (t1251 + t806 + t799 + t787) * t66;
    const double t1790 = t7 * t811;
    const double t1792 = (t1254 + t805 + t798 + t1790 + t800) * t155;
    const double t1794 = (t809 + t1263 + t1264 + t813 + t1790 + t800) * t236;
    const double t1795 = t394 * t785;
    const double t1797 = (t1795 + t1262 + t810 + t1259 + t806 + t818 + t787) * t394;
    const double t1798 = t394 * t1145;
    const double t1799 = t7 * t1143;
    const double t1801 = (t1233 + t1798 + t1538 + t1268 + t1269 + t1539 + t1799 + t1151) * t788;
    const double t1803 = (t1776 + t1634 + t1798 + t1144 + t1675 + t1676 + t1148 + t1799 + t1151) * t1246;
    const double t1784 = x[1];
    const double t1804 = t1784 * t821;
    const double t1805 = t394 * t826;
    const double t1806 = t7 * t823;
    const double t1808 = (t1804 + t1674 + t1140 + t1805 + t825 + t1275 + t1276 + t829 + t1806 + t831) * t1784;
    const double t1810 = (t784 + t1785 + t1787 + t1789 + t1792 + t1794 + t1797 + t1801 + t1803 + t1808) * t1784;
    const double t1812 = (t677 + t1687 + t1693 + t1699 + t1710 + t1721 + t1737 + t1761 + t1782 + t1810) * t1784;
    const double t1816 = (t840 + t1319 + t479) * t33;
    const double t1818 = (t463 + t1340 + t1816) * t33;
    const double t1820 = (t444 + t1323 + t1818) * t33;
    const double t1822 = (t860 + t1361 + t561) * t33;
    const double t1824 = (t542 + t1363 + t1822) * t33;
    const double t1826 = (t871 + t1361 + t561) * t33;
    const double t1828 = (t475 + t860 + t1319 + t479) * t66;
    const double t1830 = (t463 + t1340 + t1826 + t1828) * t66;
    const double t1832 = (t444 + t1323 + t1824 + t1830) * t66;
    const double t1834 = (t477 + t560 + t466) * t33;
    const double t1836 = (t445 + t1325 + t1834) * t33;
    const double t1838 = (t558 + t641 + t552) * t33;
    const double t1840 = (t508 + t585 + t660 + t512) * t66;
    const double t1842 = (t496 + t1331 + t1838 + t1840) * t66;
    const double t1844 = (t469 + t544 + t448) * t33;
    const double t1846 = (t521 + t549 + t612 + t525) * t66;
    const double t1848 = (t528 + t530 + t458 + t447 + t418) * t155;
    const double t1850 = (t415 + t1298 + t1844 + t1846 + t1848) * t155;
    const double t1852 = (t414 + t1296 + t1836 + t1842 + t1850) * t155;
    const double t1854 = (t594 + t660 + t512) * t33;
    const double t1856 = (t496 + t1331 + t1854) * t33;
    const double t1858 = (t585 + t641 + t552) * t33;
    const double t1860 = (t966 + t558 + t560 + t466) * t66;
    const double t1862 = (t445 + t1325 + t1858 + t1860) * t66;
    const double t1864 = (t510 + t551 + t499) * t33;
    const double t1866 = (t899 + t571 + t551 + t499) * t66;
    const double t1867 = t426 * t155;
    const double t1869 = (t1867 + t896 + t502 + t454 + t428) * t155;
    const double t1871 = (t425 + t1306 + t1864 + t1866 + t1869) * t155;
    const double t1873 = (t580 + t612 + t525) * t33;
    const double t1875 = (t955 + t549 + t544 + t448) * t66;
    const double t1876 = t155 * t433;
    const double t1878 = (t1876 + t909 + t523 + t454 + t428) * t155;
    const double t1880 = (t912 + t1867 + t925 + t537 + t447 + t418) * t236;
    const double t1882 = (t415 + t1298 + t1873 + t1875 + t1878 + t1880) * t236;
    const double t1884 = (t414 + t1296 + t1856 + t1862 + t1871 + t1882) * t236;
    const double t1886 = (t477 + t619 + t466) * t33;
    const double t1888 = (t445 + t1402 + t1886) * t33;
    const double t1890 = (t618 + t641 + t545) * t33;
    const double t1892 = (t966 + t618 + t619 + t466) * t66;
    const double t1894 = (t445 + t1402 + t1890 + t1892) * t66;
    const double t1896 = (t663 + t551 + t455) * t33;
    const double t1898 = (t899 + t627 + t628 + t499) * t66;
    const double t1900 = (t1867 + t896 + t624 + t498 + t428) * t155;
    const double t1902 = (t425 + t1389 + t1896 + t1898 + t1900) * t155;
    const double t1904 = (t510 + t628 + t499) * t33;
    const double t1906 = (t962 + t627 + t551 + t455) * t66;
    const double t1907 = t155 * t486;
    const double t1908 = t66 * t503;
    const double t1910 = (t1907 + t1908 + t1405 + t504 + t488) * t155;
    const double t1911 = t236 * t426;
    const double t1913 = (t1911 + t1907 + t942 + t523 + t498 + t428) * t236;
    const double t1915 = (t425 + t1389 + t1904 + t1906 + t1910 + t1913) * t236;
    const double t1917 = (t469 + t612 + t448) * t33;
    const double t1919 = (t955 + t615 + t612 + t448) * t66;
    const double t1921 = (t1876 + t909 + t624 + t524 + t428) * t155;
    const double t1922 = t236 * t433;
    const double t1924 = (t1922 + t1907 + t942 + t502 + t524 + t428) * t236;
    const double t1926 = (t1432 + t1911 + t1867 + t925 + t458 + t601 + t418) * t394;
    const double t1928 = (t415 + t1423 + t1917 + t1919 + t1921 + t1924 + t1926) * t394;
    const double t1930 = (t414 + t1387 + t1888 + t1894 + t1902 + t1915 + t1928) * t394;
    const double t1932 = (t989 + t1462 + t992) * t33;
    const double t1934 = (t982 + t1464 + t1932) * t33;
    const double t1936 = (t1052 + t1485 + t1055) * t33;
    const double t1937 = t66 * t1076;
    const double t1939 = (t1937 + t1082 + t1490 + t1085) * t66;
    const double t1941 = (t1045 + t1477 + t1936 + t1939) * t66;
    const double t1943 = (t1457 + t1012 + t985) * t33;
    const double t1945 = (t1564 + t1481 + t1054 + t1048) * t66;
    const double t1946 = t155 * t975;
    const double t1948 = (t1946 + t1560 + t1454 + t984 + t977) * t155;
    const double t1950 = (t974 + t1445 + t1943 + t1945 + t1948) * t155;
    const double t1952 = (t1081 + t1073 + t1482 + t1055) * t66;
    const double t1953 = t155 * t983;
    const double t1955 = (t1953 + t1574 + t1467 + t1000 + t985) * t155;
    const double t1956 = t155 * t990;
    const double t1958 = (t1040 + t1956 + t1058 + t1030 + t1451 + t992) * t236;
    const double t1960 = (t982 + t1453 + t1570 + t1952 + t1955 + t1958) * t236;
    const double t1962 = (t998 + t1468 + t1001) * t33;
    const double t1964 = (t1081 + t1060 + t1506 + t1055) * t66;
    const double t1966 = (t1953 + t1574 + t1503 + t1021 + t985) * t155;
    const double t1967 = t236 * t1009;
    const double t1968 = t155 * t1011;
    const double t1970 = (t1967 + t1968 + t1072 + t1019 + t1468 + t1013) * t236;
    const double t1972 = (t1509 + t1967 + t1956 + t1058 + t998 + t1495 + t992) * t394;
    const double t1974 = (t982 + t1497 + t1962 + t1964 + t1966 + t1970 + t1972) * t394;
    const double t1976 = (t1097 + t1525 + t1100) * t33;
    const double t1977 = t66 * t1126;
    const double t1979 = (t1977 + t1133 + t1530 + t1136) * t66;
    const double t1980 = t155 * t1091;
    const double t1982 = (t1980 + t1656 + t1524 + t1099 + t1093) * t155;
    const double t1983 = t1107 * t155;
    const double t1985 = (t1119 + t1983 + t1132 + t1123 + t1521 + t1109) * t236;
    const double t1986 = t236 * t1121;
    const double t1988 = (t1533 + t1986 + t1983 + t1132 + t1106 + t1534 + t1109) * t394;
    const double t1989 = t1149 * t155;
    const double t1990 = t1141 * t66;
    const double t1992 = (t1140 + t1537 + t1144 + t1989 + t1990 + t1148 + t1540 + t1151) * t788;
    const double t1994 = (t1090 + t1516 + t1976 + t1979 + t1982 + t1985 + t1988 + t1992) * t788;
    const double t1996 = (t973 + t1443 + t1934 + t1941 + t1950 + t1960 + t1974 + t1994) * t788;
    const double t1997 = t33 * t1076;
    const double t1999 = (t1997 + t1490 + t1085) * t33;
    const double t2001 = (t1045 + t1477 + t1999) * t33;
    const double t2003 = (t1082 + t1485 + t1055) * t33;
    const double t2005 = (t1004 + t1052 + t1462 + t992) * t66;
    const double t2007 = (t982 + t1464 + t2003 + t2005) * t66;
    const double t2009 = (t1082 + t1482 + t1055) * t33;
    const double t2011 = (t1017 + t1073 + t1468 + t1022) * t66;
    const double t2013 = (t1025 + t1017 + t1052 + t1451 + t992) * t155;
    const double t2015 = (t982 + t1453 + t2009 + t2011 + t2013) * t155;
    const double t2017 = (t1489 + t1054 + t1048) * t33;
    const double t2019 = (t1577 + t1481 + t1012 + t985) * t66;
    const double t2020 = t66 * t1020;
    const double t2022 = (t1956 + t2020 + t1481 + t1000 + t985) * t155;
    const double t2023 = t236 * t975;
    const double t2025 = (t2023 + t1953 + t1571 + t1478 + t984 + t977) * t236;
    const double t2027 = (t974 + t1445 + t2017 + t2019 + t2022 + t2025) * t236;
    const double t2029 = (t1082 + t1506 + t1055) * t33;
    const double t2030 = t66 * t997;
    const double t2032 = (t2030 + t1060 + t1468 + t1001) * t66;
    const double t2033 = t155 * t1009;
    const double t2035 = (t2033 + t1037 + t1067 + t1468 + t1013) * t155;
    const double t2036 = t236 * t983;
    const double t2038 = (t2036 + t1968 + t1589 + t1481 + t1021 + t985) * t236;
    const double t2039 = t236 * t990;
    const double t2041 = (t1509 + t2039 + t2033 + t2030 + t1052 + t1495 + t992) * t394;
    const double t2043 = (t982 + t1497 + t2029 + t2032 + t2035 + t2038 + t2041) * t394;
    const double t2044 = t33 * t1611;
    const double t2046 = (t2044 + t1617 + t1618) * t33;
    const double t2047 = t66 * t1611;
    const double t2048 = t33 * t1622;
    const double t2050 = (t2047 + t2048 + t1617 + t1618) * t66;
    const double t2051 = t155 * t1597;
    const double t2053 = (t2051 + t1614 + t1616 + t1604 + t1599) * t155;
    const double t2054 = t236 * t1597;
    const double t2055 = t155 * t1603;
    const double t2057 = (t2054 + t2055 + t1624 + t1625 + t1604 + t1599) * t236;
    const double t2058 = t236 * t1615;
    const double t2059 = t1615 * t155;
    const double t2060 = t66 * t1622;
    const double t2062 = (t1628 + t2058 + t2059 + t2060 + t2048 + t1630 + t1618) * t394;
    const double t2063 = t1640 * t236;
    const double t2064 = t1642 * t155;
    const double t2065 = t1635 * t66;
    const double t2066 = t1638 * t33;
    const double t2068 = (t1634 + t1636 + t2063 + t2064 + t2065 + t2066 + t1644 + t1645) * t788;
    const double t2070 = (t1596 + t1601 + t2046 + t2050 + t2053 + t2057 + t2062 + t2068) * t788;
    const double t2071 = t33 * t1126;
    const double t2073 = (t2071 + t1530 + t1136) * t33;
    const double t2075 = (t1215 + t1133 + t1525 + t1100) * t66;
    const double t2077 = (t1218 + t1114 + t1229 + t1521 + t1109) * t155;
    const double t2078 = t236 * t1091;
    const double t2080 = (t2078 + t1983 + t1659 + t1529 + t1099 + t1093) * t236;
    const double t2081 = t236 * t1107;
    const double t2082 = t155 * t1121;
    const double t2083 = t66 * t1105;
    const double t2085 = (t1533 + t2081 + t2082 + t2083 + t1229 + t1534 + t1109) * t394;
    const double t2086 = t1642 * t236;
    const double t2087 = t1640 * t155;
    const double t2088 = t1638 * t66;
    const double t2089 = t33 * t1635;
    const double t2091 = (t1667 + t1636 + t2086 + t2087 + t2088 + t2089 + t1644 + t1645) * t788;
    const double t2092 = t1149 * t236;
    const double t2093 = t1141 * t33;
    const double t2095 = (t1674 + t1634 + t1537 + t2092 + t1268 + t1269 + t2093 + t1540 + t1151) * t1246;
    const double t2097 = (t1090 + t1516 + t2073 + t2075 + t2077 + t2080 + t2085 + t2091 + t2095) * t1246;
    const double t2099 = (t973 + t1443 + t2001 + t2007 + t2015 + t2027 + t2043 + t2070 + t2097) * t1246;
    const double t2100 = t7 * t1076;
    const double t2102 = (t2100 + t1085) * t7;
    const double t2104 = (t1045 + t2102) * t7;
    const double t2106 = (t1490 + t1055) * t7;
    const double t2108 = (t989 + t1475 + t992) * t33;
    const double t2110 = (t982 + t2106 + t2108) * t33;
    const double t2112 = (t1010 + t1485 + t1013) * t33;
    const double t2114 = (t1004 + t1010 + t1475 + t992) * t66;
    const double t2116 = (t982 + t2106 + t2112 + t2114) * t66;
    const double t2118 = (t998 + t1482 + t1001) * t33;
    const double t2120 = (t1017 + t1019 + t1506 + t1022) * t66;
    const double t2122 = (t1025 + t1017 + t998 + t1475 + t992) * t155;
    const double t2124 = (t982 + t2106 + t2118 + t2120 + t2122) * t155;
    const double t2126 = (t1030 + t1506 + t1022) * t33;
    const double t2128 = (t2030 + t1019 + t1482 + t1001) * t66;
    const double t2130 = (t2033 + t1037 + t1019 + t1485 + t1013) * t155;
    const double t2132 = (t1040 + t2033 + t2030 + t1030 + t1475 + t992) * t236;
    const double t2134 = (t982 + t2106 + t2126 + t2128 + t2130 + t2132) * t236;
    const double t2136 = (t1084 + t1048) * t7;
    const double t2138 = (t1457 + t1054 + t985) * t33;
    const double t2140 = (t1577 + t1500 + t1054 + t985) * t66;
    const double t2142 = (t1956 + t2020 + t1503 + t1054 + t985) * t155;
    const double t2144 = (t2039 + t1968 + t1589 + t1467 + t1054 + t985) * t236;
    const double t2145 = t394 * t975;
    const double t2147 = (t2145 + t2036 + t1953 + t1571 + t1454 + t1047 + t977) * t394;
    const double t2149 = (t974 + t2136 + t2138 + t2140 + t2142 + t2144 + t2147) * t394;
    const double t2150 = t7 * t1611;
    const double t2152 = (t2150 + t1618) * t7;
    const double t2154 = (t1602 + t1617 + t1599) * t33;
    const double t2155 = t7 * t1622;
    const double t2157 = (t2047 + t1616 + t2155 + t1618) * t66;
    const double t2159 = (t2051 + t1614 + t1608 + t1617 + t1599) * t155;
    const double t2161 = (t1621 + t2059 + t2060 + t1625 + t2155 + t1618) * t236;
    const double t2162 = t394 * t1597;
    const double t2164 = (t2162 + t2058 + t2055 + t1624 + t1608 + t1630 + t1599) * t394;
    const double t2165 = t1640 * t394;
    const double t2166 = t1638 * t7;
    const double t2168 = (t1634 + t2165 + t1637 + t2064 + t2065 + t1643 + t2166 + t1645) * t788;
    const double t2170 = (t1596 + t2152 + t2154 + t2157 + t2159 + t2161 + t2164 + t2168) * t788;
    const double t2172 = (t2044 + t2155 + t1618) * t33;
    const double t2174 = (t1607 + t1616 + t1617 + t1599) * t66;
    const double t2176 = (t1612 + t1614 + t2048 + t2155 + t1618) * t155;
    const double t2177 = t1603 * t66;
    const double t2179 = (t2054 + t2059 + t2177 + t1625 + t1617 + t1599) * t236;
    const double t2180 = t236 * t1603;
    const double t2182 = (t2162 + t2180 + t2059 + t2177 + t1616 + t1630 + t1599) * t394;
    const double t2183 = a[160];
    const double t2184 = t2183 * t788;
    const double t2185 = a[174];
    const double t2186 = t394 * t2185;
    const double t2187 = t236 * t2185;
    const double t2188 = t155 * t2185;
    const double t2189 = t66 * t2185;
    const double t2190 = t33 * t2185;
    const double t2191 = t7 * t2185;
    const double t2192 = a[80];
    const double t2194 = (t2184 + t2186 + t2187 + t2188 + t2189 + t2190 + t2191 + t2192) * t788;
    const double t2195 = t1246 * t1633;
    const double t2197 = (t2195 + t2184 + t2165 + t2086 + t1669 + t1670 + t2089 + t2166 + t1645) * t1246;
    const double t2199 = (t1596 + t2152 + t2172 + t2174 + t2176 + t2179 + t2182 + t2194 + t2197) * t1246;
    const double t2200 = t7 * t1126;
    const double t2202 = (t2200 + t1136) * t7;
    const double t2204 = (t1097 + t1530 + t1100) * t33;
    const double t2206 = (t1215 + t1116 + t1530 + t1100) * t66;
    const double t2208 = (t1218 + t1114 + t1106 + t1748 + t1109) * t155;
    const double t2210 = (t1119 + t2082 + t2083 + t1123 + t1748 + t1109) * t236;
    const double t2211 = t394 * t1091;
    const double t2213 = (t2211 + t2081 + t1983 + t1659 + t1524 + t1135 + t1093) * t394;
    const double t2214 = t394 * t1642;
    const double t2215 = t7 * t1635;
    const double t2217 = (t1667 + t2214 + t1637 + t2087 + t2088 + t1643 + t2215 + t1645) * t788;
    const double t2218 = t1246 * t1666;
    const double t2220 = (t2218 + t2184 + t2214 + t2063 + t1669 + t1670 + t2066 + t2215 + t1645) * t1246;
    const double t2221 = t1784 * t1139;
    const double t2222 = t1149 * t394;
    const double t2223 = t1141 * t7;
    const double t2225 = (t2221 + t2195 + t1634 + t2222 + t1144 + t1268 + t1269 + t1148 + t2223 + t1151) * t1784;
    const double t2227 = (t1090 + t2202 + t2204 + t2206 + t2208 + t2210 + t2213 + t2217 + t2220 + t2225) * t1784;
    const double t2229 = (t973 + t2104 + t2110 + t2116 + t2124 + t2134 + t2149 + t2170 + t2199 + t2227) * t1784;
    const double t2231 = (t1158 + t1700 + t712) * t33;
    const double t2233 = (t696 + t1702 + t2231) * t33;
    const double t2235 = (t1170 + t1715 + t746) * t33;
    const double t2237 = (t708 + t1170 + t1700 + t712) * t66;
    const double t2239 = (t696 + t1702 + t2235 + t2237) * t66;
    const double t2241 = (t710 + t745 + t699) * t33;
    const double t2243 = (t723 + t743 + t766 + t727) * t66;
    const double t2245 = (t730 + t732 + t702 + t698 + t681) * t155;
    const double t2247 = (t678 + t1689 + t2241 + t2243 + t2245) * t155;
    const double t2249 = (t755 + t766 + t727) * t33;
    const double t2251 = (t1207 + t743 + t745 + t699) * t66;
    const double t2252 = t686 * t155;
    const double t2254 = (t2252 + t1185 + t725 + t704 + t688) * t155;
    const double t2256 = (t1188 + t2252 + t1195 + t737 + t698 + t681) * t236;
    const double t2258 = (t678 + t1689 + t2249 + t2251 + t2254 + t2256) * t236;
    const double t2260 = (t710 + t766 + t699) * t33;
    const double t2262 = (t1207 + t765 + t766 + t699) * t66;
    const double t2264 = (t2252 + t1185 + t769 + t726 + t688) * t155;
    const double t2265 = t236 * t686;
    const double t2266 = t718 * t155;
    const double t2268 = (t2265 + t2266 + t1203 + t725 + t726 + t688) * t236;
    const double t2270 = (t1733 + t2265 + t2252 + t1195 + t702 + t760 + t681) * t394;
    const double t2272 = (t678 + t1723 + t2260 + t2262 + t2264 + t2268 + t2270) * t394;
    const double t2274 = (t1212 + t1745 + t1109) * t33;
    const double t2276 = (t1977 + t1229 + t1748 + t1136) * t66;
    const double t2278 = (t1980 + t1656 + t1520 + t1108 + t1093) * t155;
    const double t2279 = t1098 * t155;
    const double t2281 = (t1222 + t2279 + t1228 + t1123 + t1521 + t1100) * t236;
    const double t2282 = t236 * t1115;
    const double t2284 = (t1751 + t2282 + t2279 + t1228 + t1106 + t1534 + t1100) * t394;
    const double t2285 = t1241 * t155;
    const double t2286 = t1234 * t66;
    const double t2288 = (t1233 + t1754 + t1237 + t2285 + t2286 + t1240 + t1757 + t1243) * t788;
    const double t2290 = (t1090 + t1740 + t2274 + t2276 + t2278 + t2281 + t2284 + t2288) * t788;
    const double t2292 = (t2071 + t1748 + t1136) * t33;
    const double t2294 = (t1104 + t1229 + t1745 + t1109) * t66;
    const double t2296 = (t1112 + t1114 + t1133 + t1521 + t1100) * t155;
    const double t2298 = (t2078 + t2279 + t1663 + t1529 + t1108 + t1093) * t236;
    const double t2299 = t236 * t1098;
    const double t2300 = t155 * t1115;
    const double t2302 = (t1751 + t2299 + t2300 + t2083 + t1133 + t1534 + t1100) * t394;
    const double t2304 = (t1667 + t1772 + t2086 + t2064 + t2065 + t2089 + t1773 + t1645) * t788;
    const double t2305 = t1241 * t236;
    const double t2306 = t1234 * t33;
    const double t2308 = (t1776 + t1667 + t1754 + t2305 + t1238 + t1239 + t2306 + t1757 + t1243) * t1246;
    const double t2310 = (t1090 + t1740 + t2292 + t2294 + t2296 + t2298 + t2302 + t2304 + t2308) * t1246;
    const double t2312 = (t1212 + t1748 + t1109) * t33;
    const double t2314 = (t1104 + t1219 + t1748 + t1109) * t66;
    const double t2316 = (t1112 + t1114 + t1106 + t1530 + t1100) * t155;
    const double t2318 = (t1222 + t2300 + t2083 + t1123 + t1530 + t1100) * t236;
    const double t2320 = (t2211 + t2299 + t2279 + t1663 + t1520 + t1135 + t1093) * t394;
    const double t2322 = (t1667 + t2214 + t1668 + t2064 + t2065 + t1671 + t2215 + t1645) * t788;
    const double t2324 = (t2218 + t2184 + t2214 + t2086 + t1639 + t1641 + t2089 + t2215 + t1645) * t1246;
    const double t2325 = t1784 * t1232;
    const double t2326 = t1241 * t394;
    const double t2327 = t1234 * t7;
    const double t2329 = (t2325 + t2218 + t1667 + t2326 + t1237 + t1238 + t1239 + t1240 + t2327 + t1243) * t1784;
    const double t2331 = (t1090 + t2202 + t2312 + t2314 + t2316 + t2318 + t2320 + t2322 + t2324 + t2329) * t1784;
    const double t2333 = (t1248 + t1790 + t800) * t33;
    const double t2335 = (t796 + t1255 + t1790 + t800) * t66;
    const double t2337 = (t803 + t805 + t798 + t799 + t787) * t155;
    const double t2338 = t791 * t155;
    const double t2340 = (t1258 + t2338 + t1264 + t813 + t799 + t787) * t236;
    const double t2341 = t236 * t791;
    const double t2343 = (t1795 + t2341 + t2338 + t1264 + t798 + t818 + t787) * t394;
    const double t2345 = (t1233 + t1798 + t1267 + t1989 + t1990 + t1270 + t1799 + t1151) * t788;
    const double t2347 = (t1776 + t1634 + t1798 + t2092 + t1146 + t1147 + t2093 + t1799 + t1151) * t1246;
    const double t2349 = (t2325 + t2195 + t1634 + t2222 + t1267 + t1146 + t1147 + t1270 + t2223 + t1151) * t1784;
    const double t2330 = x[0];
    const double t2350 = t2330 * t821;
    const double t2351 = t2350 + t2221 + t1674 + t1140 + t1805 + t1274 + t827 + t828 + t1277 + t1806 + t831;
    const double t2352 = t2351 * t2330;
    const double t2353 = t784 + t1785 + t2333 + t2335 + t2337 + t2340 + t2343 + t2345 + t2347 + t2349 + t2352;
    const double t2354 = t2353 * t2330;
    const double t2355 = t677 + t1687 + t2233 + t2239 + t2247 + t2258 + t2272 + t2290 + t2310 + t2331 + t2354;
    const double t2356 = t2355 * t2330;
    const double t2357 = t413 + t1292 + t1820 + t1832 + t1852 + t1884 + t1930 + t1996 + t2099 + t2229 + t2356;
    const double t2361 = 2.0 * t2350 + t2221 + t1674 + t1140 + t1805 + t1274 + t827 + t828 + t1277 + t1806 + t831;
    const double t2363 =
        t2330 * t2361 + t1785 + t2333 + t2335 + t2337 + t2340 + t2343 + t2345 + t2347 + t2349 + t2352 + t784;
    const double t2365 =
        t2330 * t2363 + t1687 + t2233 + t2239 + t2247 + t2258 + t2272 + t2290 + t2310 + t2331 + t2354 + t677;
    const double t2367 =
        t2330 * t2365 + t1292 + t1820 + t1832 + t1852 + t1884 + t1930 + t1996 + t2099 + t2229 + t2356 + t413;
    const double t2371 = (2.0 * t1804 + t1674 + t1140 + t1805 + t825 + t1275 + t1276 + t829 + t1806 + t831) * t1784 +
                         t784 + t1785 + t1787 + t1789 + t1792 + t1794 + t1797 + t1801 + t1803 + t1808;
    const double t2373 = t1784 * t2371 + t1687 + t1693 + t1699 + t1710 + t1721 + t1737 + t1761 + t1782 + t1810 + t677;
    const double t2378 = (2.0 * t2221 + t2195 + t1634 + t2222 + t1144 + t1268 + t1269 + t1148 + t2223 + t1151) * t1784 +
                         t1090 + t2202 + t2204 + t2206 + t2208 + t2210 + t2213 + t2217 + t2220 + t2225;
    const double t2380 = 2.0 * t2325;
    const double t2383 = t2330 * t1139;
    const double t2384 = t2383 + t2380 + t2195 + t1634 + t2222 + t1267 + t1146 + t1147 + t1270 + t2223 + t1151;
    const double t2386 = (t2380 + t2218 + t1667 + t2326 + t1237 + t1238 + t1239 + t1240 + t2327 + t1243) * t1784 +
                         t1090 + t2202 + t2312 + t2314 + t2316 + t2318 + t2320 + t2322 + t2324 + t2329 + t2384 * t2330;
    const double t2388 =
        t1784 * t2378 + t2330 * t2386 + t2104 + t2110 + t2116 + t2124 + t2134 + t2149 + t2170 + t2199 + t2227 + t973;
    const double t2390 =
        t1784 * t2373 + t2330 * t2388 + t1292 + t1304 + t1318 + t1350 + t1383 + t1438 + t1546 + t1682 + t1812 + t413;
    const double t2398 = 2.0 * t1674;
    const double t2403 = 2.0 * t1776;
    const double t2408 = (t2403 + t1667 + t1754 + t1237 + t1777 + t1778 + t1240 + t1757 + t1243) * t1246 + t1090 +
                         t1740 + t1763 + t1765 + t1767 + t1769 + t1771 + t1775 + t1780 +
                         (t2221 + t2403 + t1634 + t1798 + t1144 + t1675 + t1676 + t1148 + t1799 + t1151) * t1784;
    const double t2410 = ((t2398 + t1634 + t1537 + t1267 + t1675 + t1676 + t1270 + t1540 + t1151) * t1246 + t1090 +
                          t1516 + t1651 + t1654 + t1658 + t1661 + t1665 + t1673 + t1678) *
                             t1246 +
                         t973 + t1443 + t1550 + t1557 + t1568 + t1581 + t1595 + t1649 + t1680 + t2408 * t1784;
    const double t2419 = t1784 * t1633;
    const double t2420 = 2.0 * t2218;
    const double t2423 = (2.0 * t2195 + t2184 + t2165 + t2086 + t1669 + t1670 + t2089 + t2166 + t1645) * t1246 + t1596 +
                         t2152 + t2172 + t2174 + t2176 + t2179 + t2182 + t2194 + t2197 +
                         (t2419 + t2420 + t2184 + t2214 + t2063 + t1669 + t1670 + t2066 + t2215 + t1645) * t1784;
    const double t2427 = t1784 * t1666;
    const double t2430 = t2383 + t2419 + t2403 + t1634 + t1798 + t2092 + t1146 + t1147 + t2093 + t1799 + t1151;
    const double t2432 = (t2403 + t1667 + t1754 + t2305 + t1238 + t1239 + t2306 + t1757 + t1243) * t1246 + t1090 +
                         t1740 + t2292 + t2294 + t2296 + t2298 + t2302 + t2304 + t2308 +
                         (t2427 + t2420 + t2184 + t2214 + t2086 + t1639 + t1641 + t2089 + t2215 + t1645) * t1784 +
                         t2430 * t2330;
    const double t2434 = ((t2398 + t1634 + t1537 + t2092 + t1268 + t1269 + t2093 + t1540 + t1151) * t1246 + t1090 +
                          t1516 + t2073 + t2075 + t2077 + t2080 + t2085 + t2091 + t2095) *
                             t1246 +
                         t973 + t1443 + t2001 + t2007 + t2015 + t2027 + t2043 + t2070 + t2097 + t2423 * t1784 +
                         t2432 * t2330;
    const double t2436 = (((2.0 * t1273 + t1140 + t824 + t1274 + t1275 + t1276 + t1277 + t830 + t831) * t1246 + t784 +
                           t789 + t1250 + t1253 + t1257 + t1261 + t1266 + t1272 + t1279) *
                              t1246 +
                          t677 + t685 + t1162 + t1169 + t1179 + t1192 + t1211 + t1247 + t1281) *
                             t1246 +
                         t413 + t424 + t846 + t859 + t882 + t918 + t972 + t1157 + t1283 + t2410 * t1784 + t2434 * t2330;
    const double t2444 = 2.0 * t1140;
    const double t2449 = 2.0 * t1233;
    const double t2462 = 2.0 * t1634;
    const double t2465 = 2.0 * t1667;
    const double t2476 = (t2449 + t1754 + t1755 + t1238 + t1239 + t1756 + t1757 + t1243) * t788 + t1090 + t1740 +
                         t1742 + t1744 + t1747 + t1750 + t1753 + t1759 +
                         (t2218 + t2465 + t1772 + t1637 + t1669 + t1670 + t1643 + t1773 + t1645) * t1246 +
                         (t2221 + t2195 + t2449 + t1798 + t1538 + t1268 + t1269 + t1539 + t1799 + t1151) * t1784;
    const double t2478 = ((t2444 + t1537 + t1538 + t1146 + t1147 + t1539 + t1540 + t1151) * t788 + t1090 + t1516 +
                          t1519 + t1523 + t1527 + t1532 + t1536 + t1542) *
                             t788 +
                         t973 + t1443 + t1450 + t1461 + t1474 + t1494 + t1513 + t1544 +
                         ((t2462 + t1636 + t1637 + t1639 + t1641 + t1643 + t1644 + t1645) * t788 + t1596 + t1601 +
                          t1606 + t1610 + t1620 + t1627 + t1632 + t1647 +
                          (t2195 + t2465 + t1636 + t1668 + t1669 + t1670 + t1671 + t1644 + t1645) * t1246) *
                             t1246 +
                         t2476 * t1784;
    const double t2492 = t1246 * t2183;
    const double t2498 = (t2462 + t2165 + t1637 + t2064 + t2065 + t1643 + t2166 + t1645) * t788 + t1596 + t2152 +
                         t2154 + t2157 + t2159 + t2161 + t2164 + t2168 +
                         (t2492 + 2.0 * t2184 + t2186 + t2187 + t2188 + t2189 + t2190 + t2191 + t2192) * t1246 +
                         (t2419 + t2492 + t2465 + t2214 + t1637 + t2087 + t2088 + t1643 + t2215 + t1645) * t1784;
    const double t2506 = t2383 + t2419 + t2195 + t2449 + t1798 + t1267 + t1989 + t1990 + t1270 + t1799 + t1151;
    const double t2508 =
        (t2449 + t1754 + t1237 + t2285 + t2286 + t1240 + t1757 + t1243) * t788 + t1090 + t1740 + t2274 + t2276 + t2278 +
        t2281 + t2284 + t2288 + (t2218 + t2465 + t1772 + t2086 + t2064 + t2065 + t2089 + t1773 + t1645) * t1246 +
        (t2427 + t2492 + t2465 + t2214 + t1668 + t2064 + t2065 + t1671 + t2215 + t1645) * t1784 + t2506 * t2330;
    const double t2510 = ((t2444 + t1537 + t1144 + t1989 + t1990 + t1148 + t1540 + t1151) * t788 + t1090 + t1516 +
                          t1976 + t1979 + t1982 + t1985 + t1988 + t1992) *
                             t788 +
                         t973 + t1443 + t1934 + t1941 + t1950 + t1960 + t1974 + t1994 +
                         ((t2462 + t1636 + t2063 + t2064 + t2065 + t2066 + t1644 + t1645) * t788 + t1596 + t1601 +
                          t2046 + t2050 + t2053 + t2057 + t2062 + t2068 +
                          (t2195 + t2465 + t1636 + t2086 + t2087 + t2088 + t2089 + t1644 + t1645) * t1246) *
                             t1246 +
                         t2498 * t1784 + t2508 * t2330;
    const double t2512 = (((2.0 * t822 + t824 + t825 + t827 + t828 + t829 + t830 + t831) * t788 + t784 + t789 + t794 +
                           t802 + t808 + t815 + t820 + t833) *
                              t788 +
                          t677 + t685 + t695 + t716 + t736 + t759 + t783 + t835) *
                             t788 +
                         t413 + t424 + t443 + t485 + t536 + t600 + t676 + t837 +
                         (((t2444 + t1142 + t1144 + t1146 + t1147 + t1148 + t1150 + t1151) * t788 + t1090 + t1095 +
                           t1102 + t1111 + t1118 + t1125 + t1138 + t1153) *
                              t788 +
                          t973 + t981 + t996 + t1008 + t1029 + t1044 + t1089 + t1155 +
                          ((t2449 + t1235 + t1237 + t1238 + t1239 + t1240 + t1242 + t1243) * t788 + t1090 + t1095 +
                           t1214 + t1217 + t1221 + t1225 + t1231 + t1245 +
                           (t1674 + t2449 + t1142 + t1267 + t1268 + t1269 + t1270 + t1150 + t1151) * t1246) *
                              t1246) *
                             t1246 +
                         t2478 * t1784 + t2510 * t2330;
    const double t2520 = 2.0 * t669;
    const double t2525 = 2.0 * t778;
    const double t2528 = t788 * t823;
    const double t2529 = 2.0 * t816;
    const double t2543 = t788 * t1141;
    const double t2544 = 2.0 * t1127;
    const double t2551 = t788 * t1234;
    const double t2554 = t1246 * t823;
    const double t2561 = 2.0 * t1432;
    const double t2566 = 2.0 * t1509;
    const double t2569 = t788 * t1143;
    const double t2570 = 2.0 * t1533;
    const double t2577 = t788 * t1635;
    const double t2578 = 2.0 * t1628;
    const double t2581 = t1246 * t1143;
    const double t2586 = 2.0 * t1733;
    const double t2589 = t788 * t1236;
    const double t2590 = 2.0 * t1751;
    const double t2593 = t1246 * t1236;
    const double t2594 = t788 * t1638;
    const double t2597 = t1784 * t826;
    const double t2598 = t1246 * t1145;
    const double t2599 = t788 * t1145;
    const double t2600 = 2.0 * t1795;
    const double t2603 = (t2586 + t1202 + t749 + t1182 + t717 + t760 + t681) * t394 + t678 + t1723 + t1725 + t1728 +
                         t1730 + t1732 + t1735 +
                         (t2589 + t2590 + t1226 + t1120 + t1223 + t1524 + t1534 + t1100) * t788 +
                         (t2593 + t2594 + t2590 + t1662 + t1131 + t1659 + t1116 + t1534 + t1100) * t1246 +
                         (t2597 + t2598 + t2599 + t2600 + t1262 + t810 + t1259 + t806 + t818 + t787) * t1784;
    const double t2605 =
        ((t2561 + t948 + t574 + t889 + t491 + t601 + t418) * t394 + t415 + t1423 + t1425 + t1427 + t1429 + t1431 +
         t1434) *
            t394 +
        t414 + t1387 + t1393 + t1400 + t1411 + t1421 + t1436 +
        ((t2566 + t1070 + t1036 + t1033 + t1457 + t1495 + t992) * t394 + t982 + t1497 + t1499 + t1502 + t1505 + t1508 +
         t1511 + (t2569 + t2570 + t1129 + t1120 + t1122 + t1520 + t1534 + t1109) * t788) *
            t788 +
        ((t2566 + t1588 + t1063 + t1577 + t1010 + t1495 + t992) * t394 + t982 + t1497 + t1583 + t1585 + t1587 + t1591 +
         t1593 + (t2577 + t2578 + t1629 + t1623 + t1624 + t1616 + t1630 + t1618) * t788 +
         (t2581 + t2577 + t2570 + t1662 + t1227 + t1663 + t1219 + t1534 + t1109) * t1246) *
            t1246 +
        t2603 * t1784;
    const double t2628 = t788 * t1640;
    const double t2629 = 2.0 * t2162;
    const double t2632 = t1246 * t1640;
    const double t2633 = t788 * t2185;
    const double t2636 = t1784 * t1149;
    const double t2637 = t1246 * t1642;
    const double t2638 = t788 * t1642;
    const double t2639 = 2.0 * t2211;
    const double t2642 = (2.0 * t2145 + t2036 + t1953 + t1571 + t1454 + t1047 + t977) * t394 + t974 + t2136 + t2138 +
                         t2140 + t2142 + t2144 + t2147 +
                         (t2628 + t2629 + t2058 + t2055 + t1624 + t1608 + t1630 + t1599) * t788 +
                         (t2632 + t2633 + t2629 + t2180 + t2059 + t2177 + t1616 + t1630 + t1599) * t1246 +
                         (t2636 + t2637 + t2638 + t2639 + t2081 + t1983 + t1659 + t1524 + t1135 + t1093) * t1784;
    const double t2653 = t2330 * t826;
    const double t2654 = t2653 + t2636 + t2598 + t2599 + t2600 + t2341 + t2338 + t1264 + t798 + t818 + t787;
    const double t2656 =
        (t2586 + t2265 + t2252 + t1195 + t702 + t760 + t681) * t394 + t678 + t1723 + t2260 + t2262 + t2264 + t2268 +
        t2270 + (t2589 + t2590 + t2282 + t2279 + t1228 + t1106 + t1534 + t1100) * t788 +
        (t2593 + t2594 + t2590 + t2299 + t2300 + t2083 + t1133 + t1534 + t1100) * t1246 +
        (t1241 * t1784 + t1093 + t1135 + t1520 + t1663 + t2279 + t2299 + t2637 + t2638 + t2639) * t1784 + t2654 * t2330;
    const double t2658 =
        ((t2561 + t1911 + t1867 + t925 + t458 + t601 + t418) * t394 + t415 + t1423 + t1917 + t1919 + t1921 + t1924 +
         t1926) *
            t394 +
        t414 + t1387 + t1888 + t1894 + t1902 + t1915 + t1928 +
        ((t2566 + t1967 + t1956 + t1058 + t998 + t1495 + t992) * t394 + t982 + t1497 + t1962 + t1964 + t1966 + t1970 +
         t1972 + (t2569 + t2570 + t1986 + t1983 + t1132 + t1106 + t1534 + t1109) * t788) *
            t788 +
        ((t2566 + t2039 + t2033 + t2030 + t1052 + t1495 + t992) * t394 + t982 + t1497 + t2029 + t2032 + t2035 + t2038 +
         t2041 + (t2577 + t2578 + t2058 + t2059 + t2060 + t2048 + t1630 + t1618) * t788 +
         (t2581 + t2577 + t2570 + t2081 + t2082 + t2083 + t1229 + t1534 + t1109) * t1246) *
            t1246 +
        t2642 * t1784 + t2656 * t2330;
    const double t2660 =
        (((2.0 * t404 + t351 + t209 + t183 + t56 + t254 + t6) * t394 + t3 + t395 + t397 + t399 + t401 + t403 + t406) *
             t394 +
         t2 + t362 + t368 + t374 + t382 + t393 + t408) *
            t394 +
        t1 + t260 + t272 + t288 + t318 + t357 + t410 +
        (((t2520 + t649 + t593 + t556 + t477 + t670 + t479) * t394 + t463 + t657 + t659 + t662 + t665 + t668 + t672) *
             t394 +
         t444 + t605 + t611 + t623 + t634 + t654 + t674 +
         ((t2525 + t772 + t754 + t741 + t710 + t779 + t712) * t394 + t696 + t762 + t764 + t768 + t771 + t777 + t781 +
          (t2528 + t2529 + t817 + t810 + t812 + t798 + t818 + t800) * t788) *
             t788) *
            t788 +
        (((t2520 + t965 + t935 + t966 + t860 + t670 + t479) * t394 + t463 + t657 + t954 + t957 + t960 + t964 + t968) *
             t394 +
         t444 + t605 + t922 + t929 + t939 + t952 + t970 +
         ((2.0 * t1077 + t1079 + t1080 + t1081 + t1082 + t1084 + t1085) * t394 + t1045 + t1050 + t1057 + t1062 + t1069 +
          t1075 + t1087 + (t2543 + t2544 + t1129 + t1131 + t1132 + t1133 + t1135 + t1136) * t788) *
             t788 +
         ((t2525 + t1206 + t1198 + t1207 + t1170 + t779 + t712) * t394 + t696 + t762 + t1194 + t1197 + t1201 + t1205 +
          t1209 + (t2551 + t2544 + t1226 + t1227 + t1228 + t1229 + t1135 + t1136) * t788 +
          (t2554 + t2543 + t2529 + t1262 + t1263 + t1264 + t1255 + t818 + t800) * t1246) *
             t1246) *
            t1246 +
        t2605 * t1784 + t2658 * t2330;
    const double t2673 = 2.0 * t389;
    const double t2676 = t394 * t17;
    const double t2683 = 2.0 * t592;
    const double t2691 = t394 * t555;
    const double t2697 = 2.0 * t753;
    const double t2700 = t394 * t740;
    const double t2704 = t394 * t811;
    const double t2705 = 2.0 * t809;
    const double t2712 = 2.0 * t912;
    const double t2720 = t394 * t476;
    const double t2721 = 2.0 * t961;
    const double t2726 = 2.0 * t1040;
    const double t2729 = t394 * t1078;
    const double t2733 = t394 * t1128;
    const double t2734 = 2.0 * t1119;
    const double t2739 = 2.0 * t1188;
    const double t2742 = t394 * t709;
    const double t2746 = t394 * t1130;
    const double t2747 = 2.0 * t1222;
    const double t2750 = t1246 * t826;
    const double t2751 = t394 * t797;
    const double t2752 = 2.0 * t1258;
    const double t2766 = t394 * t446;
    const double t2774 = t394 * t1051;
    const double t2778 = 2.0 * t1528;
    const double t2785 = t394 * t997;
    const double t2789 = t394 * t1622;
    const double t2790 = 2.0 * t1621;
    const double t2793 = t394 * t1105;
    const double t2800 = t394 * t697;
    const double t2808 = t1784 * t823;
    const double t2811 = (t2697 + t1198 + t1207 + t755 + t1700 + t712) * t236 + t696 + t1702 + t1712 + t1714 + t1717 +
                         t1719 + (t2800 + 2.0 * t1206 + t773 + t1203 + t725 + t766 + t699) * t394 +
                         (t2551 + t2746 + t2778 + t1227 + t1228 + t1529 + t1748 + t1136) * t788 +
                         (t2593 + t2577 + t2793 + t2734 + t1227 + t1663 + t1123 + t1745 + t1109) * t1246 +
                         (t2808 + t2581 + t2543 + t2751 + t2705 + t1263 + t1264 + t813 + t1790 + t800) * t1784;
    const double t2813 =
        ((t2683 + t935 + t966 + t594 + t1319 + t479) * t236 + t463 + t1340 + t1373 + t1375 + t1377 + t1379) * t236 +
        t444 + t1323 + t1354 + t1360 + t1371 + t1381 +
        ((2.0 * t965 + t650 + t962 + t510 + t619 + t466) * t236 + t445 + t1402 + t1413 + t1415 + t1417 + t1419 +
         (t2766 + t2721 + t645 + t942 + t502 + t612 + t448) * t394) *
            t394 +
        ((2.0 * t1488 + t1080 + t1081 + t1489 + t1490 + t1085) * t236 + t1045 + t1477 + t1480 + t1484 + t1487 + t1492 +
         (t2774 + 2.0 * t1079 + t1071 + t1072 + t1481 + t1506 + t1055) * t394 +
         (t2543 + t2733 + t2778 + t1131 + t1132 + t1529 + t1530 + t1136) * t788) *
            t788 +
        ((t2726 + t1063 + t1577 + t1030 + t1462 + t992) * t236 + t982 + t1464 + t1570 + t1573 + t1576 + t1579 +
         (t2785 + 2.0 * t1588 + t1071 + t1589 + t1019 + t1468 + t1001) * t394 +
         (t2577 + t2789 + t2790 + t1623 + t1624 + t1625 + t1617 + t1618) * t788 +
         (t2598 + t2594 + t2793 + t2747 + t1131 + t1659 + t1123 + t1525 + t1100) * t1246) *
            t1246 +
        t2811 * t1784;
    const double t2822 = t394 * t426;
    const double t2830 = t394 * t1009;
    const double t2834 = t394 * t1121;
    const double t2842 = t394 * t990;
    const double t2846 = t394 * t1615;
    const double t2847 = 2.0 * t2054;
    const double t2850 = t1246 * t1149;
    const double t2851 = t394 * t1107;
    const double t2852 = 2.0 * t2078;
    const double t2859 = t394 * t983;
    const double t2865 = t394 * t1603;
    const double t2868 = t1784 * t1143;
    const double t2871 = (t2726 + t2033 + t2030 + t1030 + t1475 + t992) * t236 + t982 + t2106 + t2126 + t2128 + t2130 +
                         t2132 + (t2859 + 2.0 * t2039 + t1968 + t1589 + t1467 + t1054 + t985) * t394 +
                         (t2577 + t2846 + t2790 + t2059 + t2060 + t1625 + t2155 + t1618) * t788 +
                         (t2637 + t2633 + t2865 + t2847 + t2059 + t2177 + t1625 + t1617 + t1599) * t1246 +
                         (t2868 + t2632 + t2577 + t2851 + t2734 + t2082 + t2083 + t1123 + t1748 + t1109) * t1784;
    const double t2875 = t394 * t686;
    const double t2879 = t394 * t1115;
    const double t2882 = t1246 * t1241;
    const double t2883 = t394 * t1098;
    const double t2886 = t1784 * t1236;
    const double t2889 = t1784 * t1145;
    const double t2890 = t394 * t791;
    const double t2891 = t2653 + t2889 + t2850 + t2599 + t2890 + t2752 + t2338 + t1264 + t813 + t799 + t787;
    const double t2893 = (t2739 + t2252 + t1195 + t737 + t698 + t681) * t236 + t678 + t1689 + t2249 + t2251 + t2254 +
                         t2256 + (t2875 + 2.0 * t2265 + t2266 + t1203 + t725 + t726 + t688) * t394 +
                         (t2589 + t2879 + t2747 + t2279 + t1228 + t1123 + t1521 + t1100) * t788 +
                         (t2882 + t2638 + t2883 + t2852 + t2279 + t1663 + t1529 + t1108 + t1093) * t1246 +
                         (t2886 + t2637 + t2594 + t2883 + t2747 + t2300 + t2083 + t1123 + t1530 + t1100) * t1784 +
                         t2891 * t2330;
    const double t2895 =
        ((t2712 + t1867 + t925 + t537 + t447 + t418) * t236 + t415 + t1298 + t1873 + t1875 + t1878 + t1880) * t236 +
        t414 + t1296 + t1856 + t1862 + t1871 + t1882 +
        ((2.0 * t1911 + t1907 + t942 + t523 + t498 + t428) * t236 + t425 + t1389 + t1904 + t1906 + t1910 + t1913 +
         (t2822 + 2.0 * t1922 + t1907 + t942 + t502 + t524 + t428) * t394) *
            t394 +
        ((t2726 + t1956 + t1058 + t1030 + t1451 + t992) * t236 + t982 + t1453 + t1570 + t1952 + t1955 + t1958 +
         (t2830 + 2.0 * t1967 + t1968 + t1072 + t1019 + t1468 + t1013) * t394 +
         (t2569 + t2834 + t2734 + t1983 + t1132 + t1123 + t1521 + t1109) * t788) *
            t788 +
        ((2.0 * t2023 + t1953 + t1571 + t1478 + t984 + t977) * t236 + t974 + t1445 + t2017 + t2019 + t2022 + t2025 +
         (t2842 + 2.0 * t2036 + t1968 + t1589 + t1481 + t1021 + t985) * t394 +
         (t2628 + t2846 + t2847 + t2055 + t1624 + t1625 + t1604 + t1599) * t788 +
         (t2850 + t2638 + t2851 + t2852 + t1983 + t1659 + t1529 + t1099 + t1093) * t1246) *
            t1246 +
        t2871 * t1784 + t2893 * t2330;
    const double t2897 =
        (((2.0 * t245 + t209 + t183 + t170 + t18 + t6) * t236 + t3 + t34 + t239 + t241 + t244 + t247) * t236 + t2 +
         t32 + t220 + t227 + t237 + t249) *
            t236 +
        t1 + t25 + t176 + t189 + t215 + t251 +
        (((2.0 * t351 + t336 + t325 + t130 + t100 + t19) * t236 + t16 + t266 + t343 + t346 + t350 + t353) * t236 + t15 +
         t264 + t322 + t329 + t341 + t355 +
         ((t2673 + t347 + t344 + t122 + t118 + t28) * t236 + t16 + t364 + t384 + t386 + t388 + t391 +
          (t2676 + t2673 + t336 + t325 + t111 + t131 + t19) * t394) *
             t394) *
            t394 +
        (((t2683 + t593 + t556 + t594 + t478 + t479) * t236 + t463 + t468 + t582 + t587 + t591 + t596) * t236 + t444 +
         t452 + t541 + t565 + t579 + t598 +
         ((2.0 * t649 + t650 + t638 + t585 + t619 + t561) * t236 + t542 + t614 + t636 + t644 + t648 + t652 +
          (t2691 + 2.0 * t666 + t650 + t638 + t558 + t660 + t561) * t394) *
             t394 +
         ((t2697 + t754 + t741 + t755 + t711 + t712) * t236 + t696 + t701 + t739 + t748 + t752 + t757 +
          (t2700 + 2.0 * t772 + t773 + t775 + t743 + t766 + t746) * t394 +
          (t2528 + t2704 + t2705 + t810 + t812 + t813 + t799 + t800) * t788) *
             t788) *
            t788 +
        (((t2712 + t574 + t889 + t537 + t427 + t418) * t236 + t415 + t436 + t905 + t908 + t911 + t914) * t236 + t414 +
         t432 + t886 + t893 + t903 + t916 +
         ((2.0 * t948 + t645 + t942 + t549 + t498 + t448) * t236 + t445 + t607 + t941 + t944 + t947 + t950 +
          (t2720 + t2721 + t650 + t962 + t558 + t511 + t466) * t394) *
             t394 +
         ((t2726 + t1036 + t1033 + t1030 + t991 + t992) * t236 + t982 + t987 + t1032 + t1035 + t1039 + t1042 +
          (t2729 + 2.0 * t1070 + t1071 + t1072 + t1073 + t1054 + t1055) * t394 +
          (t2569 + t2733 + t2734 + t1120 + t1122 + t1123 + t1108 + t1109) * t788) *
             t788 +
         ((t2739 + t749 + t1182 + t737 + t687 + t681) * t236 + t678 + t690 + t1181 + t1184 + t1187 + t1190 +
          (t2742 + 2.0 * t1202 + t773 + t1203 + t743 + t726 + t699) * t394 +
          (t2589 + t2746 + t2747 + t1120 + t1223 + t1123 + t1099 + t1100) * t788 +
          (t2750 + t2599 + t2751 + t2752 + t810 + t1259 + t813 + t792 + t787) * t1246) *
             t1246) *
            t1246 +
        t2813 * t1784 + t2895 * t2330;
    const double t2905 = 2.0 * t209;
    const double t2910 = 2.0 * t233;
    const double t2926 = t236 * t80;
    const double t2927 = 2.0 * t347;
    const double t2943 = 2.0 * t528;
    const double t2948 = 2.0 * t574;
    const double t2951 = 2.0 * t588;
    const double t2958 = t236 * t559;
    const double t2966 = 2.0 * t730;
    const double t2969 = 2.0 * t749;
    const double t2972 = t236 * t744;
    const double t2975 = t788 * t826;
    const double t2976 = 2.0 * t803;
    const double t2983 = 2.0 * t876;
    const double t2988 = 2.0 * t593;
    const double t2995 = 2.0 * t935;
    const double t2998 = t236 * t543;
    const double t2999 = 2.0 * t650;
    const double t3002 = 2.0 * t958;
    const double t3007 = 2.0 * t1025;
    const double t3010 = 2.0 * t1036;
    const double t3013 = t236 * t1059;
    const double t3014 = 2.0 * t1063;
    const double t3017 = 2.0 * t1112;
    const double t3022 = 2.0 * t1175;
    const double t3025 = 2.0 * t754;
    const double t3028 = 2.0 * t1198;
    const double t3031 = 2.0 * t1218;
    const double t3034 = 2.0 * t1254;
    const double t3072 = 2.0 * t1080;
    const double t3077 = 2.0 * t1612;
    const double t3080 = t1246 * t1141;
    const double t3081 = 2.0 * t1655;
    const double t3094 = t1246 * t1234;
    const double t3099 = (t3022 + t723 + t710 + t1700 + t712) * t155 + t696 + t1702 + t1704 + t1706 + t1708 +
                         (t772 + t3028 + t750 + t743 + t1715 + t746) * t236 +
                         (t2800 + t2972 + t3025 + t1185 + t769 + t766 + t699) * t394 +
                         (t2589 + t2793 + t1129 + t3031 + t1114 + t1520 + t1745 + t1109) * t788 +
                         (t3094 + t2577 + t2746 + t1129 + t3081 + t1656 + t1133 + t1748 + t1136) * t1246 +
                         (t2808 + t3080 + t2569 + t2751 + t817 + t3034 + t805 + t798 + t1790 + t800) * t1784;
    const double t3101 = ((t2983 + t508 + t477 + t1319 + t479) * t155 + t463 + t1340 + t1342 + t1344 + t1346) * t155 +
                         t444 + t1323 + t1329 + t1337 + t1348 +
                         ((t2995 + t569 + t558 + t1361 + t561) * t155 + t542 + t1363 + t1365 + t1367 + t1369 +
                          (t649 + t3002 + t589 + t585 + t1361 + t561) * t236) *
                             t236 +
                         ((t2988 + t899 + t663 + t619 + t466) * t155 + t445 + t1402 + t1404 + t1407 + t1409 +
                          (t2958 + t2999 + t945 + t627 + t641 + t545) * t236 +
                          (t2766 + t2998 + t2951 + t909 + t624 + t612 + t448) * t394) *
                             t394 +
                         ((t3007 + t1017 + t1457 + t1462 + t992) * t155 + t982 + t1464 + t1466 + t1470 + t1472 +
                          (t1079 + t3014 + t1065 + t1481 + t1485 + t1055) * t236 +
                          (t2785 + t3013 + t3010 + t1037 + t1503 + t1468 + t1001) * t394 +
                          (t2599 + t2793 + t1226 + t3017 + t1114 + t1524 + t1525 + t1100) * t788) *
                             t788 +
                         ((2.0 * t1563 + t1564 + t1082 + t1490 + t1085) * t155 + t1045 + t1477 + t1559 + t1562 + t1566 +
                          (t1070 + t3072 + t1574 + t1073 + t1485 + t1055) * t236 +
                          (t2774 + t3013 + t3072 + t1574 + t1067 + t1506 + t1055) * t394 +
                          (t2594 + t2789 + t1629 + t3077 + t1614 + t1616 + t1617 + t1618) * t788 +
                          (t3080 + t2577 + t2733 + t1226 + t3081 + t1656 + t1229 + t1530 + t1136) * t1246) *
                             t1246 +
                         t3099 * t1784;
    const double t3107 = 2.0 * t1867;
    const double t3110 = 2.0 * t1876;
    const double t3117 = t236 * t486;
    const double t3128 = 2.0 * t1953;
    const double t3131 = t236 * t1011;
    const double t3134 = t788 * t1149;
    const double t3135 = 2.0 * t1980;
    const double t3142 = 2.0 * t1956;
    const double t3145 = 2.0 * t2033;
    const double t3148 = 2.0 * t2051;
    const double t3163 = t1246 * t1635;
    const double t3168 = (t3007 + t1017 + t998 + t1475 + t992) * t155 + t982 + t2106 + t2118 + t2120 + t2122 +
                         (t1967 + t3145 + t1037 + t1019 + t1485 + t1013) * t236 +
                         (t2859 + t3131 + t3142 + t2020 + t1503 + t1054 + t985) * t394 +
                         (t2638 + t2865 + t2058 + t3148 + t1614 + t1608 + t1617 + t1599) * t788 +
                         (t3163 + t2633 + t2846 + t2058 + t3077 + t1614 + t2048 + t2155 + t1618) * t1246 +
                         (t2868 + t3163 + t2628 + t2851 + t1986 + t3031 + t1114 + t1106 + t1748 + t1109) * t1784;
    const double t3172 = 2.0 * t2252;
    const double t3178 = t788 * t1241;
    const double t3183 = t1246 * t1638;
    const double t3186 = t2653 + t2889 + t2598 + t3134 + t2890 + t2341 + t2976 + t805 + t798 + t799 + t787;
    const double t3188 = (t2966 + t732 + t702 + t698 + t681) * t155 + t678 + t1689 + t2241 + t2243 + t2245 +
                         (t2265 + t3172 + t1185 + t725 + t704 + t688) * t236 +
                         (t236 * t718 + t1185 + t2875 + t3172 + t688 + t726 + t769) * t394 +
                         (t3178 + t2883 + t2299 + t3135 + t1656 + t1520 + t1108 + t1093) * t788 +
                         (t2593 + t2638 + t2879 + t2299 + t3017 + t1114 + t1133 + t1521 + t1100) * t1246 +
                         (t2886 + t3183 + t2638 + t2883 + t2282 + t3017 + t1114 + t1106 + t1530 + t1100) * t1784 +
                         t3186 * t2330;
    const double t3190 = ((t2943 + t530 + t458 + t447 + t418) * t155 + t415 + t1298 + t1844 + t1846 + t1848) * t155 +
                         t414 + t1296 + t1836 + t1842 + t1850 +
                         ((t3107 + t896 + t502 + t454 + t428) * t155 + t425 + t1306 + t1864 + t1866 + t1869 +
                          (t1911 + t3110 + t909 + t523 + t454 + t428) * t236) *
                             t236 +
                         ((t3107 + t896 + t624 + t498 + t428) * t155 + t425 + t1389 + t1896 + t1898 + t1900 +
                          (t3117 + 2.0 * t1907 + t1908 + t1405 + t504 + t488) * t236 +
                          (t2822 + t3117 + t3110 + t909 + t624 + t524 + t428) * t394) *
                             t394 +
                         ((2.0 * t1946 + t1560 + t1454 + t984 + t977) * t155 + t974 + t1445 + t1943 + t1945 + t1948 +
                          (t2039 + t3128 + t1574 + t1467 + t1000 + t985) * t236 +
                          (t2842 + t3131 + t3128 + t1574 + t1503 + t1021 + t985) * t394 +
                          (t3134 + t2851 + t2081 + t3135 + t1656 + t1524 + t1099 + t1093) * t788) *
                             t788 +
                         ((t3007 + t1017 + t1052 + t1451 + t992) * t155 + t982 + t1453 + t2009 + t2011 + t2013 +
                          (t2036 + t3142 + t2020 + t1481 + t1000 + t985) * t236 +
                          (t2830 + t3131 + t3145 + t1037 + t1067 + t1468 + t1013) * t394 +
                          (t2638 + t2846 + t2180 + t3148 + t1614 + t1616 + t1604 + t1599) * t788 +
                          (t2581 + t2628 + t2834 + t2081 + t3031 + t1114 + t1229 + t1521 + t1109) * t1246) *
                             t1246 +
                         t3168 * t1784 + t3188 * t2330;
    const double t3192 = (((2.0 * t161 + t128 + t56 + t18 + t6) * t155 + t3 + t34 + t158 + t160 + t163) * t155 + t2 +
                          t32 + t142 + t156 + t165) *
                             t155 +
                         t1 + t25 + t96 + t138 + t167 +
                         (((t2905 + t201 + t111 + t46 + t19) * t155 + t16 + t55 + t182 + t208 + t211) * t155 + t15 +
                          t51 + t193 + t205 + t213 +
                          ((t2910 + t230 + t122 + t53 + t28) * t155 + t16 + t55 + t229 + t232 + t235 +
                           (t351 + t2910 + t242 + t130 + t46 + t19) * t236) *
                              t236) *
                             t236 +
                         (((t2905 + t201 + t291 + t100 + t19) * t155 + t16 + t266 + t309 + t312 + t314) * t155 + t15 +
                          t264 + t295 + t306 + t316 +
                          ((2.0 * t336 + t337 + t299 + t107 + t82) * t155 + t79 + t274 + t331 + t335 + t339 +
                           (t2926 + t2927 + t348 + t302 + t107 + t82) * t236) *
                              t236 +
                          ((t2910 + t230 + t307 + t118 + t28) * t155 + t16 + t364 + t376 + t378 + t380 +
                           (t236 * t87 + t144 + t2927 + t299 + t348 + t82) * t236 +
                           (t2676 + t2926 + t2910 + t242 + t291 + t131 + t19) * t394) *
                              t394) *
                             t394 +
                         (((t2943 + t530 + t491 + t427 + t418) * t155 + t415 + t436 + t519 + t527 + t532) * t155 +
                          t414 + t432 + t495 + t516 + t534 +
                          ((t2948 + t575 + t502 + t454 + t448) * t155 + t445 + t457 + t567 + t573 + t577 +
                           (t965 + t2951 + t589 + t510 + t471 + t466) * t236) *
                              t236 +
                          ((t2948 + t575 + t624 + t498 + t448) * t155 + t445 + t607 + t626 + t630 + t632 +
                           (t2958 + 2.0 * t645 + t646 + t627 + t551 + t545) * t236 +
                           (t2720 + t2958 + t2951 + t589 + t663 + t511 + t466) * t394) *
                              t394 +
                          ((t2966 + t732 + t717 + t687 + t681) * t155 + t678 + t690 + t721 + t729 + t734 +
                           (t1206 + t2969 + t750 + t725 + t704 + t699) * t236 +
                           (t2742 + t2972 + t2969 + t750 + t769 + t726 + t699) * t394 +
                           (t2975 + t2751 + t1262 + t2976 + t805 + t806 + t792 + t787) * t788) *
                              t788) *
                             t788 +
                         (((t2983 + t508 + t860 + t478 + t479) * t155 + t463 + t468 + t873 + t875 + t878) * t155 +
                          t444 + t452 + t864 + t870 + t880 +
                          ((t2988 + t899 + t558 + t471 + t466) * t155 + t445 + t457 + t895 + t898 + t901 +
                           (t948 + t2951 + t909 + t549 + t454 + t448) * t236) *
                              t236 +
                          ((t2995 + t569 + t930 + t619 + t561) * t155 + t542 + t614 + t932 + t934 + t937 +
                           (t2998 + t2999 + t945 + t640 + t551 + t545) * t236 +
                           (t2691 + t2958 + t3002 + t589 + t930 + t660 + t561) * t394) *
                              t394 +
                          ((t3007 + t1017 + t1010 + t991 + t992) * t155 + t982 + t987 + t1015 + t1024 + t1027 +
                           (t1588 + t3010 + t1037 + t1019 + t1000 + t1001) * t236 +
                           (t2729 + t3013 + t3014 + t1065 + t1067 + t1054 + t1055) * t394 +
                           (t2599 + t2746 + t1662 + t3017 + t1114 + t1116 + t1099 + t1100) * t788) *
                              t788 +
                          ((t3022 + t723 + t1170 + t711 + t712) * t155 + t696 + t701 + t1172 + t1174 + t1177 +
                           (t1202 + t3025 + t1185 + t743 + t704 + t699) * t236 +
                           (t2700 + t2972 + t3028 + t750 + t1199 + t766 + t746) * t394 +
                           (t2589 + t2733 + t1662 + t3031 + t1114 + t1219 + t1108 + t1109) * t788 +
                           (t2554 + t2569 + t2704 + t1262 + t3034 + t805 + t1255 + t799 + t800) * t1246) *
                              t1246) *
                             t1246 +
                         t3101 * t1784 + t3190 * t2330;
    const double t3205 = 2.0 * t148;
    const double t3215 = 2.0 * t183;
    const double t3220 = 2.0 * t201;
    const double t3223 = t155 * t129;
    const double t3224 = 2.0 * t206;
    const double t3229 = 2.0 * t223;
    const double t3232 = t155 * t117;
    const double t3233 = 2.0 * t230;
    const double t3236 = t155 * t99;
    const double t3256 = t155 * t143;
    const double t3260 = t236 * t45;
    const double t3261 = t155 * t106;
    const double t3262 = 2.0 * t344;
    const double t3280 = 2.0 * t475;
    const double t3285 = 2.0 * t508;
    const double t3288 = t155 * t529;
    const double t3289 = 2.0 * t521;
    const double t3294 = 2.0 * t556;
    const double t3297 = t155 * t548;
    const double t3298 = 2.0 * t569;
    const double t3301 = t155 * t557;
    const double t3302 = 2.0 * t584;
    const double t3311 = t236 * t637;
    const double t3312 = t155 * t639;
    const double t3320 = 2.0 * t708;
    const double t3323 = t155 * t731;
    const double t3324 = 2.0 * t723;
    const double t3327 = t155 * t742;
    const double t3328 = 2.0 * t741;
    const double t3334 = t155 * t804;
    const double t3335 = 2.0 * t796;
    const double t3342 = 2.0 * t853;
    const double t3347 = 2.0 * t530;
    const double t3350 = t155 * t507;
    const double t3355 = 2.0 * t889;
    const double t3358 = t155 * t509;
    const double t3359 = 2.0 * t896;
    const double t3362 = t155 * t497;
    const double t3363 = 2.0 * t906;
    const double t3368 = 2.0 * t925;
    const double t3371 = t155 * t568;
    const double t3372 = 2.0 * t575;
    const double t3375 = t236 * t453;
    const double t3376 = t155 * t550;
    const double t3377 = 2.0 * t942;
    const double t3380 = t236 * t470;
    const double t3381 = 2.0 * t955;
    const double t3386 = 2.0 * t1004;
    const double t3389 = t155 * t1016;
    const double t3390 = 2.0 * t1017;
    const double t3393 = t155 * t1018;
    const double t3394 = 2.0 * t1033;
    const double t3397 = t236 * t1066;
    const double t3398 = t155 * t1064;
    const double t3399 = 2.0 * t1058;
    const double t3402 = t155 * t1113;
    const double t3403 = 2.0 * t1104;
    const double t3408 = 2.0 * t1165;
    const double t3411 = t155 * t722;
    const double t3412 = 2.0 * t732;
    const double t3415 = t155 * t724;
    const double t3416 = 2.0 * t1182;
    const double t3419 = t236 * t703;
    const double t3420 = 2.0 * t1195;
    const double t3423 = 2.0 * t1215;
    const double t3426 = 2.0 * t1251;
    const double t3480 = t155 * t1053;
    const double t3481 = 2.0 * t1571;
    const double t3484 = t236 * t999;
    const double t3487 = t155 * t1613;
    const double t3488 = 2.0 * t1607;
    const double t3491 = t155 * t1134;
    const double t3492 = 2.0 * t1652;
    const double t3511 = (t3408 + t717 + t698 + t681) * t66 + t678 + t1689 + t1695 + t1697 +
                         (t3411 + t3412 + t725 + t766 + t727) * t155 +
                         (t1206 + t3327 + t3420 + t725 + t745 + t699) * t236 +
                         (t2875 + t3419 + t3415 + t3416 + t1726 + t726 + t688) * t394 +
                         (t2589 + t2879 + t1226 + t3402 + t3423 + t1524 + t1521 + t1100) * t788 +
                         (t2882 + t2638 + t2883 + t2081 + t3491 + t3492 + t1524 + t1108 + t1093) * t1246 +
                         (t2597 + t2850 + t2599 + t2890 + t1262 + t3334 + t3426 + t806 + t799 + t787) * t1784;
    const double t3513 =
        ((t3342 + t491 + t447 + t418) * t66 + t415 + t1298 + t1312 + t1314) * t66 + t414 + t1296 + t1310 + t1316 +
        ((t3347 + t523 + t612 + t525) * t66 + t496 + t1331 + t1333 + t1335 +
         (t3350 + t3289 + t510 + t660 + t512) * t155) *
            t155 +
        ((t3368 + t502 + t544 + t448) * t66 + t445 + t1325 + t1356 + t1358 +
         (t3371 + t3372 + t571 + t641 + t552) * t155 + (t965 + t3301 + t3381 + t510 + t560 + t466) * t236) *
            t236 +
        ((t3355 + t1394 + t498 + t428) * t66 + t425 + t1389 + t1396 + t1398 +
         (t3358 + t3359 + t1405 + t628 + t499) * t155 + (t3380 + t3376 + t3377 + t1405 + t551 + t455) * t236 +
         (t2822 + t3375 + t3362 + t3363 + t1394 + t524 + t428) * t394) *
            t394 +
        ((t3386 + t1457 + t1451 + t992) * t66 + t982 + t1453 + t1456 + t1459 +
         (t3389 + t3390 + t1467 + t1468 + t1022) * t155 + (t1079 + t3398 + t3399 + t1481 + t1482 + t1055) * t236 +
         (t2830 + t3397 + t3393 + t3394 + t1500 + t1468 + t1013) * t394 +
         (t2569 + t2834 + t1129 + t3402 + t3403 + t1520 + t1521 + t1109) * t788) *
            t788 +
        ((2.0 * t1553 + t1454 + t984 + t977) * t66 + t974 + t1445 + t1552 + t1555 +
         (t1083 * t155 + t1048 + t1054 + t1481 + 2.0 * t1560) * t155 +
         (t2039 + t3480 + t3481 + t1467 + t1012 + t985) * t236 +
         (t2842 + t3484 + t3480 + t3481 + t1500 + t1021 + t985) * t394 +
         (t2628 + t2846 + t2058 + t3487 + t3488 + t1608 + t1604 + t1599) * t788 +
         (t2850 + t2638 + t2851 + t2299 + t3491 + t3492 + t1520 + t1099 + t1093) * t1246) *
            t1246 +
        t3511 * t1784;
    const double t3525 = 2.0 * t966;
    const double t3528 = t155 * t522;
    const double t3529 = 2.0 * t899;
    const double t3540 = t155 * t503;
    const double t3555 = 2.0 * t1081;
    const double t3560 = 2.0 * t1977;
    const double t3569 = t155 * t1020;
    const double t3570 = 2.0 * t1577;
    const double t3573 = 2.0 * t2030;
    const double t3576 = 2.0 * t2047;
    const double t3597 = (t3386 + t1010 + t1475 + t992) * t66 + t982 + t2106 + t2112 + t2114 +
                         (t3389 + t3390 + t1019 + t1506 + t1022) * t155 +
                         (t1588 + t3393 + t3573 + t1019 + t1482 + t1001) * t236 +
                         (t2859 + t3484 + t3569 + t3570 + t1500 + t1054 + t985) * t394 +
                         (t2577 + t2846 + t1629 + t3487 + t3576 + t1616 + t2155 + t1618) * t788 +
                         (t2637 + t2633 + t2865 + t2180 + t3487 + t3488 + t1616 + t1617 + t1599) * t1246 +
                         (t2889 + t2637 + t2594 + t2883 + t1662 + t3402 + t3423 + t1116 + t1530 + t1100) * t1784;
    const double t3603 = 2.0 * t1207;
    const double t3614 = t2330 * t823;
    const double t3615 = t3614 + t2868 + t2581 + t2543 + t2751 + t1262 + t3334 + t3335 + t1255 + t1790 + t800;
    const double t3617 =
        (t3320 + t1170 + t1700 + t712) * t66 + t696 + t1702 + t2235 + t2237 +
        (t3323 + t3324 + t743 + t766 + t727) * t155 + (t1202 + t3415 + t3603 + t743 + t745 + t699) * t236 +
        (t2800 + t3419 + t3415 + t3603 + t765 + t766 + t699) * t394 +
        (t2551 + t2746 + t1226 + t3491 + t3560 + t1229 + t1748 + t1136) * t788 +
        (t2593 + t2577 + t2793 + t2081 + t3402 + t3403 + t1229 + t1745 + t1109) * t1246 +
        (t2886 + t2632 + t2577 + t2851 + t1662 + t3402 + t3403 + t1219 + t1748 + t1109) * t1784 + t3615 * t2330;
    const double t3619 =
        ((t3280 + t860 + t1319 + t479) * t66 + t463 + t1340 + t1826 + t1828) * t66 + t444 + t1323 + t1824 + t1830 +
        ((t3285 + t585 + t660 + t512) * t66 + t496 + t1331 + t1838 + t1840 +
         (t3288 + t3289 + t549 + t612 + t525) * t155) *
            t155 +
        ((t3525 + t558 + t560 + t466) * t66 + t445 + t1325 + t1858 + t1860 +
         (t3528 + t3529 + t571 + t551 + t499) * t155 + (t948 + t3362 + t3381 + t549 + t544 + t448) * t236) *
            t236 +
        ((t3525 + t618 + t619 + t466) * t66 + t445 + t1402 + t1890 + t1892 +
         (t3528 + t3529 + t627 + t628 + t499) * t155 + (t3375 + t3540 + 2.0 * t962 + t627 + t551 + t455) * t236 +
         (t2766 + t3375 + t3362 + t3381 + t615 + t612 + t448) * t394) *
            t394 +
        ((2.0 * t1937 + t1082 + t1490 + t1085) * t66 + t1045 + t1477 + t1936 + t1939 +
         (t1046 * t155 + t1048 + t1054 + t1481 + 2.0 * t1564) * t155 +
         (t1070 + t3480 + t3555 + t1073 + t1482 + t1055) * t236 +
         (t2774 + t3397 + t3480 + t3555 + t1060 + t1506 + t1055) * t394 +
         (t2543 + t2733 + t1129 + t3491 + t3560 + t1133 + t1530 + t1136) * t788) *
            t788 +
        ((t3386 + t1052 + t1462 + t992) * t66 + t982 + t1464 + t2003 + t2005 +
         (t3389 + t3390 + t1073 + t1468 + t1022) * t155 + (t2036 + t3569 + t3570 + t1481 + t1012 + t985) * t236 +
         (t2785 + t3484 + t3393 + t3573 + t1060 + t1468 + t1001) * t394 +
         (t2577 + t2789 + t2058 + t3487 + t3576 + t2048 + t1617 + t1618) * t788 +
         (t2598 + t2594 + t2793 + t2299 + t3402 + t3423 + t1133 + t1525 + t1100) * t1246) *
            t1246 +
        t3597 * t1784 + t3617 * t2330;
    const double t3621 =
        (((2.0 * t70 + t56 + t18 + t6) * t66 + t3 + t34 + t69 + t72) * t66 + t2 + t32 + t67 + t74) * t66 + t1 + t25 +
        t62 + t76 +
        (((2.0 * t128 + t130 + t131 + t132) * t66 + t116 + t121 + t126 + t134) * t66 + t97 + t105 + t115 + t136 +
         ((t3205 + t150 + t151 + t152) * t66 + t116 + t121 + t146 + t154 +
          (t127 * t155 + t130 + t131 + t132 + t3205) * t155) *
             t155) *
            t155 +
        (((t3215 + t111 + t81 + t19) * t66 + t16 + t90 + t182 + t185) * t66 + t15 + t86 + t180 + t187 +
         ((t3220 + t195 + t144 + t119) * t66 + t98 + t110 + t200 + t203 + (t3223 + t3224 + t195 + t124 + t119) * t155) *
             t155 +
         ((t3229 + t122 + t88 + t28) * t66 + t16 + t90 + t222 + t225 + (t3232 + t3233 + t195 + t107 + t101) * t155 +
          (t351 + t3236 + t3229 + t130 + t81 + t19) * t236) *
             t236) *
            t236 +
        (((t3215 + t275 + t100 + t19) * t66 + t16 + t266 + t282 + t284) * t66 + t15 + t264 + t279 + t286 +
         ((t3220 + t302 + t296 + t119) * t66 + t98 + t298 + t301 + t304 + (t3223 + t3224 + t310 + t296 + t119) * t155) *
             t155 +
         ((2.0 * t325 + t299 + t107 + t47) * t66 + t44 + t290 + t324 + t327 +
          (t3256 + 2.0 * t332 + t333 + t197 + t108) * t155 + (t3260 + t3261 + t3262 + t310 + t107 + t47) * t236) *
             t236 +
         ((t3229 + t280 + t118 + t28) * t66 + t16 + t364 + t370 + t372 + (t3232 + t3233 + t299 + t296 + t101) * t155 +
          (t236 * t52 + t124 + t299 + t3261 + t3262 + t47) * t236 +
          (t2676 + t3260 + t3236 + t3229 + t275 + t131 + t19) * t394) *
             t394) *
            t394 +
        (((t3280 + t477 + t478 + t479) * t66 + t463 + t468 + t473 + t481) * t66 + t444 + t452 + t462 + t483 +
         ((t3285 + t510 + t511 + t512) * t66 + t496 + t501 + t506 + t514 +
          (t3288 + t3289 + t523 + t524 + t525) * t155) *
             t155 +
         ((t3294 + t558 + t560 + t561) * t66 + t542 + t547 + t554 + t563 + (t3297 + t3298 + t571 + t551 + t552) * t155 +
          (t649 + t3301 + t3302 + t585 + t560 + t561) * t236) *
             t236 +
         ((t3294 + t618 + t619 + t561) * t66 + t542 + t614 + t617 + t621 + (t3297 + t3298 + t627 + t628 + t552) * t155 +
          (t3311 + t3312 + 2.0 * t638 + t640 + t641 + t642) * t236 +
          (t2691 + t3311 + t3301 + t3302 + t618 + t660 + t561) * t394) *
             t394 +
         ((t3320 + t710 + t711 + t712) * t66 + t696 + t701 + t706 + t714 + (t3323 + t3324 + t725 + t726 + t727) * t155 +
          (t772 + t3327 + t3328 + t743 + t745 + t746) * t236 +
          (t236 * t774 + t2700 + t3327 + t3328 + t746 + t765 + t766) * t394 +
          (t2528 + t2704 + t817 + t3334 + t3335 + t798 + t799 + t800) * t788) *
             t788) *
            t788 +
        (((t3342 + t458 + t427 + t418) * t66 + t415 + t436 + t852 + t855) * t66 + t414 + t432 + t850 + t857 +
         ((t3347 + t549 + t524 + t525) * t66 + t496 + t501 + t866 + t868 +
          (t3350 + t3289 + t585 + t511 + t512) * t155) *
             t155 +
         ((t3355 + t502 + t487 + t428) * t66 + t425 + t490 + t888 + t891 + (t3358 + t3359 + t571 + t504 + t499) * t155 +
          (t1911 + t3362 + t3363 + t523 + t487 + t428) * t236) *
             t236 +
         ((t3368 + t615 + t498 + t448) * t66 + t445 + t607 + t924 + t927 + (t3371 + t3372 + t640 + t628 + t552) * t155 +
          (t3375 + t3376 + t3377 + t627 + t504 + t455) * t236 +
          (t2720 + t3380 + t3301 + t3381 + t618 + t511 + t466) * t394) *
             t394 +
         ((t3386 + t998 + t991 + t992) * t66 + t982 + t987 + t1003 + t1006 +
          (t3389 + t3390 + t1019 + t1021 + t1022) * t155 + (t1967 + t3393 + t3394 + t1019 + t1012 + t1013) * t236 +
          (t2729 + t3397 + t3398 + t3399 + t1060 + t1054 + t1055) * t394 +
          (t2569 + t2733 + t1986 + t3402 + t3403 + t1106 + t1108 + t1109) * t788) *
             t788 +
         ((t3408 + t702 + t687 + t681) * t66 + t678 + t690 + t1164 + t1167 +
          (t3411 + t3412 + t743 + t726 + t727) * t155 + (t2265 + t3415 + t3416 + t725 + t719 + t688) * t236 +
          (t2742 + t3419 + t3327 + t3420 + t765 + t726 + t699) * t394 +
          (t2589 + t2746 + t2282 + t3402 + t3423 + t1106 + t1099 + t1100) * t788 +
          (t2750 + t2599 + t2751 + t2341 + t3334 + t3426 + t798 + t792 + t787) * t1246) *
             t1246) *
            t1246 +
        t3513 * t1784 + t3619 * t2330;
    const double t3629 = 2.0 * t56;
    const double t3634 = 2.0 * t63;
    const double t3650 = 2.0 * t122;
    const double t3670 = 2.0 * t130;
    const double t3679 = t66 * t194;
    const double t3687 = 2.0 * t216;
    const double t3690 = 2.0 * t150;
    const double t3709 = t66 * t80;
    const double t3710 = 2.0 * t280;
    const double t3721 = t155 * t45;
    const double t3722 = 2.0 * t307;
    const double t3732 = t66 * t196;
    const double t3736 = t236 * t129;
    const double t3743 = t66 * t87;
    const double t3746 = t155 * t52;
    const double t3749 = t236 * t117;
    const double t3752 = t236 * t99;
    const double t3759 = 2.0 * t437;
    const double t3764 = 2.0 * t458;
    const double t3767 = 2.0 * t469;
    const double t3772 = 2.0 * t491;
    const double t3775 = 2.0 * t502;
    const double t3778 = 2.0 * t517;
    const double t3783 = 2.0 * t537;
    const double t3786 = 2.0 * t549;
    const double t3789 = t66 * t570;
    const double t3790 = 2.0 * t523;
    const double t3793 = t236 * t507;
    const double t3794 = 2.0 * t580;
    const double t3801 = t66 * t559;
    const double t3805 = t155 * t453;
    const double t3806 = 2.0 * t624;
    const double t3809 = t236 * t568;
    const double t3812 = t236 * t557;
    const double t3813 = t155 * t470;
    const double t3818 = 2.0 * t691;
    const double t3821 = 2.0 * t702;
    const double t3824 = 2.0 * t717;
    const double t3827 = t236 * t722;
    const double t3828 = 2.0 * t737;
    const double t3831 = t236 * t742;
    const double t3832 = t155 * t703;
    const double t3833 = t66 * t744;
    const double t3836 = t236 * t804;
    const double t3837 = 2.0 * t790;
    const double t3844 = 2.0 * t840;
    const double t3849 = 2.0 * t477;
    const double t3856 = 2.0 * t860;
    const double t3859 = 2.0 * t558;
    const double t3862 = 2.0 * t871;
    const double t3867 = 2.0 * t594;
    const double t3870 = 2.0 * t510;
    const double t3873 = 2.0 * t585;
    const double t3876 = t236 * t529;
    const double t3883 = t66 * t543;
    const double t3884 = 2.0 * t618;
    const double t3887 = t155 * t637;
    const double t3891 = t236 * t548;
    const double t3898 = 2.0 * t989;
    const double t3901 = 2.0 * t998;
    const double t3904 = 2.0 * t1010;
    const double t3907 = t236 * t1016;
    const double t3908 = 2.0 * t1030;
    const double t3911 = t236 * t1064;
    const double t3912 = t155 * t1066;
    const double t3913 = t66 * t1059;
    const double t3914 = 2.0 * t1052;
    const double t3917 = t236 * t1113;
    const double t3918 = 2.0 * t1097;
    const double t3923 = 2.0 * t1158;
    const double t3926 = 2.0 * t710;
    const double t3929 = 2.0 * t1170;
    const double t3932 = t236 * t731;
    const double t3933 = 2.0 * t755;
    const double t3936 = t155 * t774;
    const double t3939 = 2.0 * t1212;
    const double t3942 = 2.0 * t1248;
    const double t3979 = t66 * t486;
    const double t3985 = t236 * t509;
    const double t3988 = t236 * t497;
    const double t3996 = 2.0 * t1454;
    const double t4005 = t236 * t1053;
    const double t4006 = t155 * t999;
    const double t4007 = t66 * t1011;
    const double t4010 = t236 * t1134;
    const double t4011 = 2.0 * t1517;
    const double t4018 = 2.0 * t1457;
    const double t4025 = t236 * t1018;
    const double t4028 = t236 * t1613;
    const double t4029 = 2.0 * t1602;
    const double t4044 = t236 * t724;
    const double t4045 = t66 * t718;
    const double t4054 = (t3818 + t698 + t681) * t33 + t678 + t1689 + t1691 + (t1182 + t3824 + t704 + t688) * t66 +
                         (t754 + t1185 + t3821 + t745 + t699) * t155 +
                         (t3827 + t3327 + t1185 + t3828 + t766 + t727) * t236 +
                         (t2875 + t4044 + t3832 + t4045 + t3824 + t726 + t688) * t394 +
                         (t3178 + t2883 + t4010 + t1983 + t1659 + t4011 + t1108 + t1093) * t788 +
                         (t2593 + t2638 + t2879 + t3917 + t1131 + t1659 + t3918 + t1521 + t1100) * t1246 +
                         (t2597 + t2598 + t3134 + t2890 + t3836 + t810 + t1259 + t3837 + t799 + t787) * t1784;
    const double t4056 =
        ((t3759 + t447 + t418) * t33 + t415 + t1298 + t1300) * t33 + t414 + t1296 + t1302 +
        ((t3772 + t454 + t428) * t33 + t425 + t1306 + t1308 + (t889 + t3778 + t454 + t428) * t66) * t66 +
        ((t3764 + t544 + t448) * t33 + t445 + t1325 + t1327 + (t896 + t3775 + t551 + t499) * t66 +
         (t593 + t899 + t3767 + t560 + t466) * t155) *
            t155 +
        ((t3783 + t612 + t525) * t33 + t496 + t1331 + t1352 + (t909 + t3790 + t551 + t499) * t66 +
         (t3301 + t3789 + t3786 + t641 + t552) * t155 + (t3793 + t3371 + t899 + t3794 + t660 + t512) * t236) *
            t236 +
        ((t3772 + t498 + t428) * t33 + t425 + t1389 + t1391 + (t3979 + 2.0 * t1394 + t504 + t488) * t66 +
         (t3813 + t1908 + t3806 + t551 + t455) * t155 + (t3985 + t3376 + t1908 + t3790 + t628 + t499) * t236 +
         (t2822 + t3988 + t3805 + t3979 + t3778 + t524 + t428) * t394) *
            t394 +
        ((2.0 * t1446 + t984 + t977) * t33 + t974 + t1445 + t1448 + (t1577 + t3996 + t1000 + t985) * t66 +
         (t1956 + t2020 + t3996 + t1012 + t985) * t155 +
         (t1083 * t236 + t1048 + t1054 + 2.0 * t1478 + t1574 + t3480) * t236 +
         (t2842 + t4005 + t4006 + t4007 + t3996 + t1021 + t985) * t394 +
         (t3134 + t2851 + t4010 + t2279 + t1663 + t4011 + t1099 + t1093) * t788) *
            t788 +
        ((t3898 + t1451 + t992) * t33 + t982 + t1453 + t1548 + (t1571 + t4018 + t1000 + t985) * t66 +
         (t1080 + t1574 + t3914 + t1482 + t1055) * t155 + (t3907 + t3398 + t2020 + t3908 + t1468 + t1022) * t236 +
         (t2830 + t4025 + t3912 + t4007 + t3904 + t1468 + t1013) * t394 +
         (t2638 + t2846 + t4028 + t2059 + t2177 + t4029 + t1604 + t1599) * t788 +
         (t2581 + t2628 + t2834 + t3917 + t1227 + t1663 + t3939 + t1521 + t1109) * t1246) *
            t1246 +
        t4054 * t1784;
    const double t4093 = t236 * t522;
    const double t4117 = 2.0 * t1082;
    const double t4128 = 2.0 * t2044;
    const double t4131 = 2.0 * t2071;
    const double t4144 = t236 * t1020;
    const double t4153 = (t3898 + t1475 + t992) * t33 + t982 + t2106 + t2108 + (t1033 + t3904 + t1485 + t1013) * t66 +
                         (t1036 + t1037 + t3901 + t1482 + t1001) * t155 +
                         (t3907 + t3393 + t1037 + t3908 + t1506 + t1022) * t236 +
                         (t2859 + t4144 + t4006 + t4007 + t4018 + t1054 + t985) * t394 +
                         (t2638 + t2865 + t4028 + t2055 + t1624 + t4029 + t1617 + t1599) * t788 +
                         (t3163 + t2633 + t2846 + t4028 + t1623 + t1624 + t4128 + t2155 + t1618) * t1246 +
                         (t2889 + t3183 + t2638 + t2883 + t3917 + t1120 + t1223 + t3918 + t1530 + t1100) * t1784;
    const double t4171 = t3614 + t2868 + t3080 + t2569 + t2751 + t3836 + t810 + t812 + t3942 + t1790 + t800;
    const double t4173 =
        (t3923 + t1700 + t712) * t33 + t696 + t1702 + t2231 + (t741 + t3929 + t1715 + t746) * t66 +
        (t749 + t750 + t3926 + t745 + t699) * t155 + (t3932 + t3415 + t750 + t3933 + t766 + t727) * t236 +
        (t2800 + t4044 + t3832 + t3833 + t3926 + t766 + t699) * t394 +
        (t2589 + t2793 + t3917 + t1983 + t1132 + t3939 + t1745 + t1109) * t788 +
        (t3094 + t2577 + t2746 + t4010 + t1131 + t1132 + t4131 + t1748 + t1136) * t1246 +
        (t2886 + t3163 + t2628 + t2851 + t3917 + t1120 + t1122 + t3939 + t1748 + t1109) * t1784 + t4171 * t2330;
    const double t4175 =
        ((t3844 + t1319 + t479) * t33 + t463 + t1340 + t1816) * t33 + t444 + t1323 + t1818 +
        ((t3856 + t1361 + t561) * t33 + t542 + t1363 + t1822 + (t556 + t3862 + t1361 + t561) * t66) * t66 +
        ((t3849 + t560 + t466) * t33 + t445 + t1325 + t1834 + (t569 + t3859 + t641 + t552) * t66 +
         (t574 + t575 + t3767 + t544 + t448) * t155) *
            t155 +
        ((t3867 + t660 + t512) * t33 + t496 + t1331 + t1854 + (t589 + t3873 + t641 + t552) * t66 +
         (t3362 + t3789 + t3870 + t551 + t499) * t155 + (t3876 + t3528 + t575 + t3794 + t612 + t525) * t236) *
            t236 +
        ((t3849 + t619 + t466) * t33 + t445 + t1402 + t1886 + (t3801 + t3884 + t641 + t545) * t66 +
         (t3805 + t945 + 2.0 * t663 + t551 + t455) * t155 + (t4093 + t3540 + t945 + t3870 + t628 + t499) * t236 +
         (t2766 + t3988 + t3805 + t3883 + t3767 + t612 + t448) * t394) *
            t394 +
        ((t3898 + t1462 + t992) * t33 + t982 + t1464 + t1932 + (t1081 + t3914 + t1485 + t1055) * t66 +
         (t1953 + t1574 + t4018 + t1012 + t985) * t155 + (t3907 + t3569 + t1065 + t3908 + t1468 + t1022) * t236 +
         (t2785 + t4025 + t4006 + t3913 + t3901 + t1468 + t1001) * t394 +
         (t2599 + t2793 + t3917 + t2279 + t1228 + t3918 + t1525 + t1100) * t788) *
            t788 +
        ((2.0 * t1997 + t1490 + t1085) * t33 + t1045 + t1477 + t1999 + (t1058 + t4117 + t1485 + t1055) * t66 +
         (t1063 + t1065 + t4117 + t1482 + t1055) * t155 +
         (t1046 * t236 + t1048 + t1054 + 2.0 * t1489 + t1574 + t3480) * t236 +
         (t2774 + t4005 + t3912 + t3913 + t4117 + t1506 + t1055) * t394 +
         (t2594 + t2789 + t4028 + t2059 + t2060 + t4128 + t1617 + t1618) * t788 +
         (t3080 + t2577 + t2733 + t4010 + t1227 + t1228 + t4131 + t1530 + t1136) * t1246) *
            t1246 +
        t4153 * t1784 + t4173 * t2330;
    const double t4177 =
        (((2.0 * t35 + t18 + t6) * t33 + t3 + t34 + t37) * t33 + t2 + t32 + t39) * t33 + t1 + t25 + t41 +
        (((t3629 + t46 + t19) * t33 + t16 + t55 + t58) * t33 + t15 + t51 + t60 +
         ((t3634 + t53 + t28) * t33 + t16 + t55 + t65 + (t183 + t3634 + t46 + t19) * t66) * t66) *
            t66 +
        (((t3629 + t81 + t19) * t33 + t16 + t90 + t92) * t33 + t15 + t86 + t94 +
         ((2.0 * t111 + t107 + t101) * t33 + t98 + t110 + t113 + (t201 + t3650 + t124 + t119) * t66) * t66 +
         ((t3634 + t88 + t28) * t33 + t16 + t90 + t140 + (t206 + t3650 + t144 + t119) * t66 +
          (t209 + t201 + t3634 + t81 + t19) * t155) *
             t155) *
            t155 +
        (((2.0 * t170 + t131 + t132) * t33 + t116 + t121 + t172) * t33 + t97 + t105 + t174 +
         ((t3670 + t124 + t119) * t33 + t98 + t110 + t178 + (t242 + t3650 + t107 + t101) * t66) * t66 +
         ((t3670 + t144 + t119) * t33 + t98 + t110 + t191 + (t3679 + 2.0 * t195 + t197 + t198) * t66 +
          (t3236 + t3679 + t3650 + t107 + t101) * t155) *
             t155 +
         ((t3687 + t151 + t152) * t33 + t116 + t121 + t218 + (t230 + t3690 + t144 + t119) * t66 +
          (t3232 + t3679 + t3690 + t124 + t119) * t155 + (t127 * t236 + t131 + t132 + t201 + t3223 + t3687) * t236) *
             t236) *
            t236 +
        (((t3629 + t100 + t19) * t33 + t16 + t266 + t268) * t33 + t15 + t264 + t270 +
         ((2.0 * t275 + t107 + t82) * t33 + t79 + t274 + t277 + (t3709 + t3710 + t107 + t82) * t66) * t66 +
         ((2.0 * t291 + t107 + t47) * t33 + t44 + t290 + t293 + (t337 + 2.0 * t299 + t197 + t108) * t66 +
          (t3721 + t332 + t3722 + t107 + t47) * t155) *
             t155 +
         ((t3670 + t296 + t119) * t33 + t98 + t298 + t320 + (t348 + 2.0 * t302 + t197 + t108) * t66 +
          (t3261 + t3732 + 2.0 * t310 + t197 + t108) * t155 + (t3736 + t3256 + t332 + t3690 + t296 + t119) * t236) *
             t236 +
         ((t3634 + t118 + t28) * t33 + t16 + t364 + t366 + (t3743 + t3710 + t144 + t82) * t66 +
          (t3746 + t348 + t3722 + t124 + t47) * t155 + (t3749 + t3261 + t348 + t3650 + t296 + t101) * t236 +
          (t2676 + t3752 + t3721 + t3709 + t3634 + t131 + t19) * t394) *
             t394) *
            t394 +
        (((t3759 + t427 + t418) * t33 + t415 + t436 + t439) * t33 + t414 + t432 + t441 +
         ((t3764 + t454 + t448) * t33 + t445 + t457 + t460 + (t966 + t3767 + t471 + t466) * t66) * t66 +
         ((t3772 + t487 + t428) * t33 + t425 + t490 + t493 + (t899 + t3775 + t504 + t499) * t66 +
          (t1867 + t896 + t3778 + t487 + t428) * t155) *
             t155 +
         ((t3783 + t524 + t525) * t33 + t496 + t501 + t539 + (t589 + t3786 + t551 + t552) * t66 +
          (t3362 + t3789 + t3790 + t504 + t499) * t155 + (t3793 + t3358 + t569 + t3794 + t511 + t512) * t236) *
             t236 +
         ((t3764 + t498 + t448) * t33 + t445 + t607 + t609 + (t3801 + 2.0 * t615 + t551 + t545) * t66 +
          (t3805 + t945 + t3806 + t504 + t455) * t155 + (t3809 + t3376 + t646 + t3786 + t628 + t552) * t236 +
          (t2720 + t3812 + t3813 + t3801 + t3767 + t511 + t466) * t394) *
             t394 +
         ((t3818 + t687 + t681) * t33 + t678 + t690 + t693 + (t1207 + t3821 + t704 + t699) * t66 +
          (t2252 + t1185 + t3824 + t719 + t688) * t155 + (t3827 + t3415 + t750 + t3828 + t726 + t727) * t236 +
          (t2742 + t3831 + t3832 + t3833 + t3821 + t726 + t699) * t394 +
          (t2975 + t2751 + t3836 + t2338 + t1264 + t3837 + t792 + t787) * t788) *
             t788) *
            t788 +
        (((t3844 + t478 + t479) * t33 + t463 + t468 + t842) * t33 + t444 + t452 + t844 +
         ((t3849 + t471 + t466) * t33 + t445 + t457 + t848 + (t925 + t3767 + t454 + t448) * t66) * t66 +
         ((t3856 + t560 + t561) * t33 + t542 + t547 + t862 + (t575 + t3859 + t551 + t552) * t66 +
          (t935 + t569 + t3862 + t560 + t561) * t155) *
             t155 +
         ((t3867 + t511 + t512) * t33 + t496 + t501 + t884 + (t909 + t3870 + t504 + t499) * t66 +
          (t3301 + t3789 + t3873 + t551 + t552) * t155 + (t3876 + t3297 + t896 + t3794 + t524 + t525) * t236) *
             t236 +
         ((t3856 + t619 + t561) * t33 + t542 + t614 + t920 + (t3883 + t3884 + t551 + t545) * t66 +
          (t3887 + t646 + 2.0 * t930 + t641 + t642) * t155 + (t3891 + t3312 + t945 + t3873 + t628 + t552) * t236 +
          (t2691 + t3812 + t3887 + t3801 + t3862 + t660 + t561) * t394) *
             t394 +
         ((t3898 + t991 + t992) * t33 + t982 + t987 + t994 + (t2030 + t3901 + t1000 + t1001) * t66 +
          (t2033 + t1037 + t3904 + t1012 + t1013) * t155 + (t3907 + t3393 + t1037 + t3908 + t1021 + t1022) * t236 +
          (t2729 + t3911 + t3912 + t3913 + t3914 + t1054 + t1055) * t394 +
          (t2599 + t2746 + t3917 + t2300 + t2083 + t3918 + t1099 + t1100) * t788) *
             t788 +
         ((t3923 + t711 + t712) * t33 + t696 + t701 + t1160 + (t1195 + t3926 + t704 + t699) * t66 +
          (t1198 + t750 + t3929 + t745 + t746) * t155 + (t3932 + t3327 + t1185 + t3933 + t726 + t727) * t236 +
          (t2700 + t3831 + t3936 + t3833 + t3929 + t766 + t746) * t394 +
          (t2589 + t2733 + t3917 + t2082 + t2083 + t3939 + t1108 + t1109) * t788 +
          (t2554 + t2569 + t2704 + t3836 + t1263 + t1264 + t3942 + t799 + t800) * t1246) *
             t1246) *
            t1246 +
        t4056 * t1784 + t4175 * t2330;
    const double t4189 = ((2.0 * t18 + t19) * t7 + t16 + t21) * t7;
    const double t4190 = 2.0 * t27;
    const double t4192 = (t4190 + t28) * t7;
    const double t4201 = (2.0 * t46 + t47) * t7;
    const double t4202 = 2.0 * t53;
    const double t4217 = (2.0 * t81 + t82) * t7;
    const double t4218 = 2.0 * t88;
    const double t4225 = (2.0 * t100 + t101) * t7;
    const double t4226 = 2.0 * t107;
    const double t4229 = 2.0 * t118;
    const double t4281 = (2.0 * t131 + t119) * t7;
    const double t4286 = 2.0 * t124;
    const double t4293 = 2.0 * t144;
    const double t4296 = 2.0 * t296;
    const double t4313 = 2.0 * t358;
    const double t4316 = 2.0 * t151;
    const double t4336 = ((2.0 * t417 + t418) * t7 + t415 + t420) * t7;
    const double t4339 = (2.0 * t427 + t428) * t7;
    const double t4340 = 2.0 * t434;
    const double t4347 = (2.0 * t447 + t448) * t7;
    const double t4348 = 2.0 * t454;
    const double t4351 = 2.0 * t465;
    const double t4356 = 2.0 * t487;
    const double t4359 = 2.0 * t498;
    const double t4368 = 2.0 * t544;
    const double t4379 = (2.0 * t601 + t525) * t7;
    const double t4380 = 2.0 * t524;
    const double t4383 = 2.0 * t612;
    const double t4390 = t394 * t507;
    const double t4391 = 2.0 * t655;
    const double t4398 = (2.0 * t680 + t681) * t7;
    const double t4399 = 2.0 * t687;
    const double t4402 = 2.0 * t698;
    const double t4409 = t394 * t722;
    const double t4410 = 2.0 * t760;
    const double t4413 = t394 * t804;
    const double t4414 = 2.0 * t786;
    const double t4464 = 2.0 * t984;
    const double t4477 = t394 * t1134;
    const double t4478 = 2.0 * t1092;
    const double t4505 = ((2.0 * t1286 + t479) * t7 + t463 + t1288) * t7;
    const double t4508 = (2.0 * t478 + t466) * t7;
    const double t4513 = 2.0 * t471;
    const double t4522 = (2.0 * t1319 + t561) * t7;
    const double t4523 = 2.0 * t560;
    const double t4526 = 2.0 * t619;
    const double t4529 = 2.0 * t1338;
    const double t4538 = 2.0 * t1361;
    const double t4547 = (2.0 * t670 + t512) * t7;
    const double t4548 = 2.0 * t511;
    const double t4553 = 2.0 * t660;
    const double t4558 = t394 * t529;
    const double t4565 = (2.0 * t1439 + t992) * t7;
    const double t4566 = 2.0 * t991;
    const double t4569 = 2.0 * t1451;
    const double t4572 = 2.0 * t1462;
    const double t4575 = 2.0 * t1475;
    const double t4578 = t394 * t1016;
    const double t4579 = 2.0 * t1495;
    const double t4582 = t394 * t1113;
    const double t4583 = 2.0 * t1514;
    const double t4598 = t394 * t1613;
    const double t4599 = 2.0 * t1598;
    const double t4608 = (2.0 * t1683 + t712) * t7;
    const double t4609 = 2.0 * t711;
    const double t4614 = 2.0 * t1700;
    const double t4619 = t394 * t731;
    const double t4620 = 2.0 * t779;
    const double t4623 = 2.0 * t1738;
    const double t4628 = 2.0 * t1783;
    const double t4631 = t4608 + t696 + t1685 + (t702 + t4609 + t699) * t33 + (t1195 + t769 + t4609 + t699) * t66 +
                         (t1198 + t750 + t765 + t4614 + t746) * t155 +
                         (t772 + t3936 + t3833 + t743 + t4614 + t746) * t236 +
                         (t4619 + t3831 + t3327 + t1185 + t725 + t4620 + t727) * t394 +
                         (t2589 + t4582 + t1129 + t2082 + t2083 + t1520 + t4623 + t1109) * t788 +
                         (t2593 + t2628 + t4582 + t1986 + t1227 + t1663 + t1106 + t4623 + t1109) * t1246 +
                         (t2808 + t2581 + t2569 + t4413 + t817 + t1263 + t1264 + t798 + t4628 + t800) * t1784;
    const double t4633 =
        t4505 + t444 + t1290 + (t4508 + t445 + t1294 + (t458 + t4351 + t448) * t33) * t33 +
        (t4508 + t445 + t1294 + (t624 + t4513 + t455) * t33 + (t925 + t624 + t4351 + t448) * t66) * t66 +
        (t4522 + t542 + t1321 + (t615 + t4523 + t545) * t33 + (t575 + t627 + t4526 + t552) * t66 +
         (t935 + t569 + t618 + t4529 + t561) * t155) *
            t155 +
        (t4522 + t542 + t1321 + (t549 + t4526 + t552) * t33 + (t3883 + t627 + t4523 + t545) * t66 +
         (t3887 + t646 + t640 + t4538 + t642) * t155 + (t649 + t3887 + t3801 + t585 + t4529 + t561) * t236) *
            t236 +
        (t4547 + t496 + t1385 + (t502 + t4548 + t499) * t33 + (t909 + t1405 + t4548 + t499) * t66 +
         (t3301 + t3789 + t627 + t4553 + t552) * t155 + (t3812 + t3312 + t945 + t571 + t4553 + t552) * t236 +
         (t4558 + t3891 + t3297 + t896 + t523 + t4391 + t525) * t394) *
            t394 +
        (t4565 + t982 + t1441 + (t1454 + t4566 + t985) * t33 + (t2030 + t1503 + t4569 + t1001) * t66 +
         (t2033 + t1037 + t1500 + t4572 + t1013) * t155 + (t1079 + t3912 + t3913 + t1481 + t4575 + t1055) * t236 +
         (t4578 + t3911 + t3393 + t1037 + t1467 + t4579 + t1022) * t394 +
         (t2599 + t4582 + t1226 + t2300 + t2083 + t1524 + t4583 + t1100) * t788) *
            t788 +
        (t4565 + t982 + t1441 + (t998 + t4569 + t1001) * t33 + (t1571 + t1503 + t4566 + t985) * t66 +
         (t1080 + t1574 + t1060 + t4575 + t1055) * t155 + (t1967 + t3912 + t4007 + t1019 + t4572 + t1013) * t236 +
         (t4578 + t4025 + t3398 + t2020 + t1019 + t4579 + t1022) * t394 +
         (t2638 + t4598 + t2058 + t2059 + t2177 + t1608 + t4599 + t1599) * t788 +
         (t2598 + t2638 + t4582 + t2282 + t1131 + t1659 + t1106 + t4583 + t1100) * t1246) *
            t1246 +
        t4631 * t1784;
    const double t4708 = 2.0 * t1490;
    const double t4721 = 2.0 * t2150;
    const double t4726 = t1784 * t1141;
    const double t4727 = 2.0 * t2200;
    const double t4730 = (2.0 * t2100 + t1085) * t7 + t1045 + t2102 + (t1052 + t4708 + t1055) * t33 +
                         (t1058 + t1067 + t4708 + t1055) * t66 + (t1063 + t1065 + t1060 + t4708 + t1055) * t155 +
                         (t1070 + t3912 + t3913 + t1073 + t4708 + t1055) * t236 +
                         (t1046 * t394 + t1048 + 2.0 * t1084 + t1481 + t1574 + t3480 + t4005) * t394 +
                         (t2594 + t4598 + t1629 + t2059 + t2060 + t1616 + t4721 + t1618) * t788 +
                         (t3183 + t2633 + t4598 + t2058 + t1623 + t1624 + t2048 + t4721 + t1618) * t1246 +
                         (t4726 + t3163 + t2577 + t4477 + t1129 + t1227 + t1228 + t1133 + t4727 + t1136) * t1784;
    const double t4749 = t3614 + t4726 + t2581 + t2569 + t4413 + t1262 + t810 + t812 + t1255 + t4628 + t800;
    const double t4751 =
        t4608 + t696 + t1685 + (t1170 + t4614 + t746) * t33 + (t741 + t1199 + t4614 + t746) * t66 +
        (t749 + t750 + t765 + t4609 + t699) * t155 + (t1202 + t3832 + t3833 + t743 + t4609 + t699) * t236 +
        (t4619 + t4044 + t3415 + t750 + t743 + t4620 + t727) * t394 +
        (t2589 + t4582 + t1662 + t1983 + t1132 + t1219 + t4623 + t1109) * t788 +
        (t2593 + t2628 + t4582 + t2081 + t1120 + t1122 + t1229 + t4623 + t1109) * t1246 +
        (t1234 * t1784 + t1131 + t1132 + t1136 + t1226 + t1229 + t2577 + t3163 + t4477 + t4727) * t1784 + t4749 * t2330;
    const double t4753 =
        t4505 + t444 + t1290 + (t4522 + t542 + t1321 + (t860 + t4529 + t561) * t33) * t33 +
        (t4522 + t542 + t1321 + (t930 + t4538 + t642) * t33 + (t556 + t930 + t4529 + t561) * t66) * t66 +
        (t4508 + t445 + t1294 + (t618 + t4523 + t545) * t33 + (t569 + t640 + t4526 + t552) * t66 +
         (t574 + t575 + t615 + t4351 + t448) * t155) *
            t155 +
        (t4508 + t445 + t1294 + (t585 + t4526 + t552) * t33 + (t3801 + t640 + t4523 + t545) * t66 +
         (t3805 + t945 + t627 + t4513 + t455) * t155 + (t948 + t3805 + t3883 + t549 + t4351 + t448) * t236) *
            t236 +
        (t4547 + t496 + t1385 + (t558 + t4553 + t552) * t33 + (t589 + t640 + t4553 + t552) * t66 +
         (t3362 + t3789 + t627 + t4548 + t499) * t155 + (t3988 + t3540 + t945 + t571 + t4548 + t499) * t236 +
         (t4558 + t4093 + t3528 + t575 + t549 + t4391 + t525) * t394) *
            t394 +
        (t4565 + t982 + t1441 + (t1010 + t4572 + t1013) * t33 + (t1081 + t1067 + t4575 + t1055) * t66 +
         (t1953 + t1574 + t1500 + t4566 + t985) * t155 + (t1588 + t4006 + t3913 + t1019 + t4569 + t1001) * t236 +
         (t4578 + t4025 + t3569 + t1065 + t1019 + t4579 + t1022) * t394 +
         (t2599 + t4582 + t1662 + t2279 + t1228 + t1116 + t4583 + t1100) * t788) *
            t788 +
        (t4565 + t982 + t1441 + (t1082 + t4575 + t1055) * t33 + (t1033 + t1067 + t4572 + t1013) * t66 +
         (t1036 + t1037 + t1060 + t4569 + t1001) * t155 + (t2036 + t4006 + t4007 + t1481 + t4566 + t985) * t236 +
         (t4578 + t4144 + t3393 + t1037 + t1073 + t4579 + t1022) * t394 +
         (t2638 + t4598 + t2180 + t2055 + t1624 + t1616 + t4599 + t1599) * t788 +
         (t2598 + t2638 + t4582 + t2299 + t1120 + t1223 + t1133 + t4583 + t1100) * t1246) *
            t1246 +
        t4730 * t1784 + t4751 * t2330;
    const double t4755 =
        (((2.0 * t5 + t6) * t7 + t3 + t8) * t7 + t2 + t10) * t7 + t1 + t12 +
        (t4189 + t15 + t23 + (t4192 + t16 + t30 + (t56 + t4190 + t19) * t33) * t33) * t33 +
        (t4189 + t15 + t23 + (t4201 + t44 + t49 + (t291 + t4202 + t47) * t33) * t33 +
         (t4192 + t16 + t30 + (t307 + t4202 + t47) * t33 + (t183 + t291 + t4190 + t19) * t66) * t66) *
            t66 +
        (t4189 + t15 + t23 + (t4217 + t79 + t84 + (t275 + t4218 + t82) * t33) * t33 +
         (t4225 + t98 + t103 + (t299 + t4226 + t108) * t33 + (t201 + t310 + t4229 + t119) * t66) * t66 +
         (t4192 + t16 + t30 + (t280 + t4218 + t82) * t33 + (t206 + t302 + t4229 + t119) * t66 +
          (t209 + t201 + t275 + t4190 + t19) * t155) *
             t155) *
            t155 +
        (t4189 + t15 + t23 + (t4225 + t98 + t103 + (t130 + t4229 + t119) * t33) * t33 +
         (t4217 + t79 + t84 + (t310 + t4226 + t108) * t33 + (t3709 + t299 + t4218 + t82) * t66) * t66 +
         (t4201 + t44 + t49 + (t302 + t4226 + t108) * t33 + (t337 + t333 + t4226 + t108) * t66 +
          (t3721 + t332 + t299 + t4202 + t47) * t155) *
             t155 +
         (t4192 + t16 + t30 + (t150 + t4229 + t119) * t33 + (t3743 + t302 + t4218 + t82) * t66 +
          (t3746 + t348 + t310 + t4202 + t47) * t155 + (t351 + t3721 + t3709 + t130 + t4190 + t19) * t236) *
             t236) *
            t236 +
        (((2.0 * t254 + t132) * t7 + t116 + t256) * t7 + t97 + t258 +
         (t4281 + t98 + t262 + (t111 + t4229 + t101) * t33) * t33 +
         (t4281 + t98 + t262 + (t299 + t4286 + t108) * t33 + (t242 + t299 + t4229 + t101) * t66) * t66 +
         (t4281 + t98 + t262 + (t299 + t4293 + t108) * t33 + (t3679 + t333 + t4296 + t198) * t66 +
          (t3236 + t3679 + t299 + t4229 + t101) * t155) *
             t155 +
         (t4281 + t98 + t262 + (t195 + t4296 + t198) * t33 + (t348 + t333 + t4293 + t108) * t66 +
          (t3261 + t3732 + t333 + t4286 + t108) * t155 + (t3752 + t3261 + t348 + t195 + t4229 + t101) * t236) *
             t236 +
         ((t4313 + t152) * t7 + t116 + t360 + (t122 + t4316 + t119) * t33 + (t230 + t302 + t4316 + t119) * t66 +
          (t3232 + t3679 + t310 + t4316 + t119) * t155 + (t3749 + t3256 + t332 + t195 + t4316 + t119) * t236 +
          (t127 * t394 + t130 + t132 + t201 + t3223 + t3736 + t4313) * t394) *
             t394) *
            t394 +
        (t4336 + t414 + t422 + (t4339 + t425 + t430 + (t491 + t4340 + t428) * t33) * t33 +
         (t4347 + t445 + t450 + (t624 + t4348 + t455) * t33 + (t966 + t663 + t4351 + t466) * t66) * t66 +
         (t4339 + t425 + t430 + (t1394 + t4356 + t488) * t33 + (t899 + t1405 + t4359 + t499) * t66 +
          (t1867 + t896 + t1394 + t4340 + t428) * t155) *
             t155 +
         (t4347 + t445 + t450 + (t523 + t4359 + t499) * t33 + (t3801 + t627 + t4368 + t545) * t66 +
          (t3805 + t945 + t1405 + t4348 + t455) * t155 + (t965 + t3813 + t3801 + t510 + t4351 + t466) * t236) *
             t236 +
         (t4379 + t496 + t603 + (t502 + t4380 + t499) * t33 + (t589 + t627 + t4383 + t552) * t66 +
          (t3362 + t3789 + t1405 + t4380 + t499) * t155 + (t3812 + t3376 + t646 + t571 + t4383 + t552) * t236 +
          (t4390 + t3809 + t3358 + t569 + t510 + t4391 + t512) * t394) *
             t394 +
         (t4398 + t678 + t683 + (t717 + t4399 + t688) * t33 + (t1207 + t769 + t4402 + t699) * t66 +
          (t2252 + t1185 + t1726 + t4399 + t688) * t155 + (t1206 + t3832 + t3833 + t725 + t4402 + t699) * t236 +
          (t4409 + t3831 + t3415 + t750 + t725 + t4410 + t727) * t394 +
          (t2975 + t4413 + t1262 + t2338 + t1264 + t806 + t4414 + t787) * t788) *
             t788) *
            t788 +
        (t4336 + t414 + t422 + (t4347 + t445 + t450 + (t477 + t4351 + t466) * t33) * t33 +
         (t4339 + t425 + t430 + (t663 + t4348 + t455) * t33 + (t889 + t624 + t4340 + t428) * t66) * t66 +
         (t4347 + t445 + t450 + (t618 + t4368 + t545) * t33 + (t896 + t627 + t4359 + t499) * t66 +
          (t593 + t899 + t618 + t4351 + t466) * t155) *
             t155 +
         (t4339 + t425 + t430 + (t510 + t4359 + t499) * t33 + (t3979 + t1405 + t4356 + t488) * t66 +
          (t3813 + t1908 + t627 + t4348 + t455) * t155 + (t1911 + t3805 + t3979 + t523 + t4340 + t428) * t236) *
             t236 +
         (t4379 + t496 + t603 + (t558 + t4383 + t552) * t33 + (t909 + t627 + t4380 + t499) * t66 +
          (t3301 + t3789 + t640 + t4383 + t552) * t155 + (t3988 + t3376 + t1908 + t571 + t4380 + t499) * t236 +
          (t4390 + t3985 + t3371 + t899 + t585 + t4391 + t512) * t394) *
             t394 +
         ((2.0 * t976 + t977) * t7 + t974 + t979 + (t1457 + t4464 + t985) * t33 + (t1577 + t1503 + t4464 + t985) * t66 +
          (t1956 + t2020 + t1500 + t4464 + t985) * t155 + (t2039 + t4006 + t4007 + t1467 + t4464 + t985) * t236 +
          (t1083 * t394 + 2.0 * t1047 + t1048 + t1481 + t1574 + t3480 + t4005) * t394 +
          (t3134 + t4477 + t2081 + t2279 + t1663 + t1524 + t4478 + t1093) * t788) *
             t788 +
         (t4398 + t678 + t683 + (t710 + t4402 + t699) * t33 + (t1182 + t769 + t4399 + t688) * t66 +
          (t754 + t1185 + t765 + t4402 + t699) * t155 + (t2265 + t3832 + t4045 + t725 + t4399 + t688) * t236 +
          (t4409 + t4044 + t3327 + t1185 + t743 + t4410 + t727) * t394 +
          (t3178 + t4477 + t2299 + t1983 + t1659 + t1520 + t4478 + t1093) * t788 +
          (t2750 + t3134 + t4413 + t2341 + t810 + t1259 + t798 + t4414 + t787) * t1246) *
             t1246) *
            t1246 +
        t4633 * t1784 + t4753 * t2330;
    g[0] = t2367;
    g[1] = t2390;
    g[2] = t2436;
    g[3] = t2512;
    g[4] = t2660;
    g[5] = t2897;
    g[6] = t3192;
    g[7] = t3621;
    g[8] = t4177;
    g[9] = t4755;
    double e = (t1 + t12) * t7 + (t1 + t25 + t41) * t33 + (t1 + t25 + t62 + t76) * t66 +
               (t1 + t25 + t96 + t138 + t167) * t155 + (t1 + t25 + t176 + t189 + t215 + t251) * t236 +
               (t1 + t260 + t272 + t288 + t318 + t357 + t410) * t394 +
               (t413 + t424 + t443 + t485 + t536 + t600 + t676 + t837) * t788 +
               (t413 + t424 + t846 + t859 + t882 + t918 + t972 + t1157 + t1283) * t1246 +
               (t413 + t1292 + t1304 + t1318 + t1350 + t1383 + t1438 + t1546 + t1682 + t1812) * t1784 + t2357 * t2330;

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(20);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input linear constants (a):\n";
    for (size_t j = 0; j < 207; j++) {
        std::cerr << a[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input polynomial variables (x):\n";
    for (size_t j = 0; j < 10; j++) {
        std::cerr << x[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output polynomial gradients (g):\n";
    for (size_t j = 0; j < 10; j++) {
        std::cerr << g[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output polynomial energy: " << e << std::endl;
#endif

    return e;
}

}  // namespace x1b_A1B4_deg5_exp0
