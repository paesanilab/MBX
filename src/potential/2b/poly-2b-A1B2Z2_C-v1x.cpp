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

#include "poly-2b-A1B2Z2_C-v1x.h"

namespace h2o_ion {

std::vector<double> poly_2b_h2o_ion_v1x::eval(const size_t nd, const double a[429], const double* x, double* g) {
    std::vector<double> energy(nd, 0.0);
#pragma omp simd
    for (size_t nv = 0; nv < nd; nv++) {
        const double t1 = a[2];
        const double t2 = a[5];
        const double t3 = a[64];
        const double t4 = a[412];
        const double t7 = x[nv + nd * 7];
        const double t5 = t7 * t4;
        const double t6 = a[151];
        const double t8 = (t5 + t6) * t7;
        const double t10 = (t3 + t8) * t7;
        const double t12 = (t2 + t10) * t7;
        const double t15 = a[17];
        const double t16 = a[31];
        const double t17 = a[404];
        const double t18 = t7 * t17;
        const double t19 = a[174];
        const double t21 = (t18 + t19) * t7;
        const double t23 = (t16 + t21) * t7;
        const double t25 = (t15 + t23) * t7;
        const double t27 = t7 * a[311];
        const double t28 = a[182];
        const double t30 = (t27 + t28) * t7;
        const double t32 = (t16 + t30) * t7;
        const double t34 = (t27 + t19) * t7;
        const double t33 = x[nv + nd * 6];
        const double t35 = t33 * t4;
        const double t37 = (t35 + t18 + t6) * t33;
        const double t39 = (t3 + t34 + t37) * t33;
        const double t41 = (t2 + t32 + t39) * t33;
        const double t44 = a[1];
        const double t45 = a[10];
        const double t46 = a[50];
        const double t47 = a[285];
        const double t48 = t7 * t47;
        const double t49 = a[125];
        const double t51 = (t48 + t49) * t7;
        const double t53 = (t46 + t51) * t7;
        const double t55 = (t45 + t53) * t7;
        const double t56 = a[66];
        const double t57 = a[293];
        const double t58 = t7 * t57;
        const double t59 = a[160];
        const double t61 = (t58 + t59) * t7;
        const double t63 = (t56 + t61) * t7;
        const double t65 = t7 * a[426];
        const double t67 = (t65 + t59) * t7;
        const double t68 = t33 * t47;
        const double t70 = (t68 + t58 + t49) * t33;
        const double t72 = (t46 + t67 + t70) * t33;
        const double t74 = (t45 + t63 + t72) * t33;
        const double t75 = a[11];
        const double t76 = a[56];
        const double t77 = a[314];
        const double t78 = t7 * t77;
        const double t79 = a[145];
        const double t81 = (t78 + t79) * t7;
        const double t83 = (t76 + t81) * t7;
        const double t84 = a[271];
        const double t85 = t7 * t84;
        const double t86 = a[167];
        const double t88 = (t85 + t86) * t7;
        const double t89 = t33 * t77;
        const double t91 = (t89 + t85 + t79) * t33;
        const double t93 = (t76 + t88 + t91) * t33;
        const double t94 = a[42];
        const double t95 = a[422];
        const double t96 = t7 * t95;
        const double t97 = a[122];
        const double t99 = (t96 + t97) * t7;
        const double t100 = t33 * t95;
        const double t101 = a[211];
        const double t102 = t7 * t101;
        const double t104 = (t100 + t102 + t97) * t33;
        const double t98 = x[nv + nd * 5];
        const double t106 = t98 * a[425];
        const double t107 = a[269];
        const double t108 = t33 * t107;
        const double t109 = t7 * t107;
        const double t110 = a[154];
        const double t112 = (t106 + t108 + t109 + t110) * t98;
        const double t114 = (t94 + t99 + t104 + t112) * t98;
        const double t116 = (t75 + t83 + t93 + t114) * t98;
        const double t119 = a[0];
        const double t120 = a[3];
        const double t121 = a[22];
        const double t122 = a[323];
        const double t123 = t7 * t122;
        const double t124 = a[126];
        const double t126 = (t123 + t124) * t7;
        const double t128 = (t121 + t126) * t7;
        const double t130 = (t120 + t128) * t7;
        const double t131 = a[46];
        const double t132 = a[359];
        const double t133 = t7 * t132;
        const double t134 = a[155];
        const double t136 = (t133 + t134) * t7;
        const double t138 = (t131 + t136) * t7;
        const double t140 = t7 * a[215];
        const double t142 = (t140 + t134) * t7;
        const double t143 = t33 * t122;
        const double t145 = (t143 + t133 + t124) * t33;
        const double t147 = (t121 + t142 + t145) * t33;
        const double t149 = (t120 + t138 + t147) * t33;
        const double t150 = a[13];
        const double t151 = a[51];
        const double t152 = a[196];
        const double t153 = t7 * t152;
        const double t154 = a[110];
        const double t156 = (t153 + t154) * t7;
        const double t158 = (t151 + t156) * t7;
        const double t159 = a[189];
        const double t160 = t7 * t159;
        const double t161 = a[176];
        const double t163 = (t160 + t161) * t7;
        const double t164 = t33 * t152;
        const double t166 = (t164 + t160 + t154) * t33;
        const double t168 = (t151 + t163 + t166) * t33;
        const double t169 = a[23];
        const double t170 = a[258];
        const double t171 = t7 * t170;
        const double t172 = a[153];
        const double t174 = (t171 + t172) * t7;
        const double t175 = t33 * t170;
        const double t176 = a[275];
        const double t177 = t7 * t176;
        const double t179 = (t175 + t177 + t172) * t33;
        const double t181 = t98 * a[277];
        const double t182 = a[330];
        const double t183 = t33 * t182;
        const double t184 = t7 * t182;
        const double t185 = a[158];
        const double t187 = (t181 + t183 + t184 + t185) * t98;
        const double t189 = (t169 + t174 + t179 + t187) * t98;
        const double t191 = (t150 + t158 + t168 + t189) * t98;
        const double t192 = a[7];
        const double t193 = a[32];
        const double t194 = a[409];
        const double t195 = t7 * t194;
        const double t196 = a[71];
        const double t198 = (t195 + t196) * t7;
        const double t200 = (t193 + t198) * t7;
        const double t201 = a[329];
        const double t202 = t7 * t201;
        const double t203 = a[177];
        const double t205 = (t202 + t203) * t7;
        const double t206 = t33 * t194;
        const double t208 = (t206 + t202 + t196) * t33;
        const double t210 = (t193 + t205 + t208) * t33;
        const double t211 = a[36];
        const double t212 = a[249];
        const double t213 = t7 * t212;
        const double t214 = a[140];
        const double t216 = (t213 + t214) * t7;
        const double t217 = t33 * t212;
        const double t218 = a[242];
        const double t219 = t7 * t218;
        const double t221 = (t217 + t219 + t214) * t33;
        const double t223 = t98 * a[210];
        const double t224 = a[245];
        const double t225 = t33 * t224;
        const double t226 = t7 * t224;
        const double t227 = a[83];
        const double t229 = (t223 + t225 + t226 + t227) * t98;
        const double t231 = (t211 + t216 + t221 + t229) * t98;
        const double t232 = a[62];
        const double t233 = a[233];
        const double t234 = t7 * t233;
        const double t235 = a[69];
        const double t237 = (t234 + t235) * t7;
        const double t238 = t33 * t233;
        const double t239 = a[373];
        const double t240 = t7 * t239;
        const double t242 = (t238 + t240 + t235) * t33;
        const double t244 = t98 * a[335];
        const double t245 = a[357];
        const double t246 = t33 * t245;
        const double t247 = t7 * t245;
        const double t248 = a[113];
        const double t250 = (t244 + t246 + t247 + t248) * t98;
        const double t251 = a[227];
        const double t241 = x[nv + nd * 4];
        const double t252 = t241 * t251;
        const double t253 = a[307];
        const double t254 = t253 * t98;
        const double t255 = a[402];
        const double t256 = t33 * t255;
        const double t257 = t7 * t255;
        const double t258 = a[85];
        const double t260 = (t252 + t254 + t256 + t257 + t258) * t241;
        const double t262 = (t232 + t237 + t242 + t250 + t260) * t241;
        const double t264 = (t192 + t200 + t210 + t231 + t262) * t241;
        const double t267 = a[4];
        const double t268 = a[49];
        const double t269 = a[349];
        const double t270 = t7 * t269;
        const double t271 = a[136];
        const double t273 = (t270 + t271) * t7;
        const double t275 = (t268 + t273) * t7;
        const double t276 = a[235];
        const double t277 = t7 * t276;
        const double t278 = a[179];
        const double t280 = (t277 + t278) * t7;
        const double t281 = t33 * t269;
        const double t283 = (t281 + t277 + t271) * t33;
        const double t285 = (t268 + t280 + t283) * t33;
        const double t286 = a[60];
        const double t287 = a[395];
        const double t288 = t7 * t287;
        const double t289 = a[183];
        const double t291 = (t288 + t289) * t7;
        const double t292 = t33 * t287;
        const double t293 = a[397];
        const double t294 = t7 * t293;
        const double t296 = (t292 + t294 + t289) * t33;
        const double t298 = t98 * a[386];
        const double t299 = a[366];
        const double t300 = t33 * t299;
        const double t301 = t7 * t299;
        const double t302 = a[80];
        const double t304 = (t298 + t300 + t301 + t302) * t98;
        const double t306 = (t286 + t291 + t296 + t304) * t98;
        const double t307 = a[43];
        const double t308 = a[186];
        const double t309 = t7 * t308;
        const double t310 = a[84];
        const double t312 = (t309 + t310) * t7;
        const double t313 = t33 * t308;
        const double t314 = a[250];
        const double t315 = t7 * t314;
        const double t317 = (t313 + t315 + t310) * t33;
        const double t319 = t98 * a[370];
        const double t320 = a[226];
        const double t321 = t33 * t320;
        const double t322 = t7 * t320;
        const double t323 = a[101];
        const double t325 = (t319 + t321 + t322 + t323) * t98;
        const double t326 = a[309];
        const double t327 = t326 * t241;
        const double t328 = a[244];
        const double t329 = t98 * t328;
        const double t330 = a[354];
        const double t331 = t33 * t330;
        const double t332 = t7 * t330;
        const double t333 = a[161];
        const double t335 = (t327 + t329 + t331 + t332 + t333) * t241;
        const double t337 = (t307 + t312 + t317 + t325 + t335) * t241;
        const double t339 = (t267 + t275 + t285 + t306 + t337) * t241;
        const double t341 = t241 * a[302];
        const double t342 = a[394];
        const double t343 = t342 * t98;
        const double t344 = a[383];
        const double t345 = t33 * t344;
        const double t346 = t7 * t344;
        const double t347 = a[185];
        const double t349 = (t341 + t343 + t345 + t346 + t347) * t241;
        const double t351 = (t307 + t312 + t317 + t325 + t349) * t241;
        const double t353 = (t341 + t329 + t331 + t332 + t333) * t241;
        const double t340 = x[nv + nd * 3];
        const double t354 = t340 * t251;
        const double t356 = (t354 + t327 + t254 + t256 + t257 + t258) * t340;
        const double t358 = (t232 + t237 + t242 + t250 + t353 + t356) * t340;
        const double t360 = (t192 + t200 + t210 + t231 + t351 + t358) * t340;
        const double t363 = a[9];
        const double t364 = a[40];
        const double t365 = a[251];
        const double t366 = t7 * t365;
        const double t367 = a[132];
        const double t369 = (t366 + t367) * t7;
        const double t371 = (t364 + t369) * t7;
        const double t373 = (t363 + t371) * t7;
        const double t374 = a[59];
        const double t375 = a[203];
        const double t376 = t7 * t375;
        const double t377 = a[103];
        const double t379 = (t376 + t377) * t7;
        const double t381 = (t374 + t379) * t7;
        const double t383 = t7 * a[333];
        const double t385 = (t383 + t377) * t7;
        const double t386 = t33 * t365;
        const double t388 = (t386 + t376 + t367) * t33;
        const double t390 = (t364 + t385 + t388) * t33;
        const double t392 = (t363 + t381 + t390) * t33;
        const double t393 = a[6];
        const double t394 = a[26];
        const double t395 = a[274];
        const double t396 = t7 * t395;
        const double t397 = a[128];
        const double t399 = (t396 + t397) * t7;
        const double t401 = (t394 + t399) * t7;
        const double t402 = a[312];
        const double t403 = t7 * t402;
        const double t404 = a[91];
        const double t406 = (t403 + t404) * t7;
        const double t407 = t33 * t395;
        const double t409 = (t407 + t403 + t397) * t33;
        const double t411 = (t394 + t406 + t409) * t33;
        const double t412 = a[24];
        const double t413 = a[350];
        const double t414 = t7 * t413;
        const double t415 = a[70];
        const double t417 = (t414 + t415) * t7;
        const double t418 = t33 * t413;
        const double t419 = a[288];
        const double t420 = t7 * t419;
        const double t422 = (t418 + t420 + t415) * t33;
        const double t424 = t98 * a[361];
        const double t425 = a[268];
        const double t426 = t33 * t425;
        const double t427 = t7 * t425;
        const double t428 = a[180];
        const double t430 = (t424 + t426 + t427 + t428) * t98;
        const double t432 = (t412 + t417 + t422 + t430) * t98;
        const double t434 = (t393 + t401 + t411 + t432) * t98;
        const double t435 = a[8];
        const double t436 = a[37];
        const double t437 = a[263];
        const double t438 = t7 * t437;
        const double t439 = a[141];
        const double t441 = (t438 + t439) * t7;
        const double t443 = (t436 + t441) * t7;
        const double t444 = a[264];
        const double t445 = t7 * t444;
        const double t446 = a[150];
        const double t448 = (t445 + t446) * t7;
        const double t449 = t33 * t437;
        const double t451 = (t449 + t445 + t439) * t33;
        const double t453 = (t436 + t448 + t451) * t33;
        const double t454 = a[25];
        const double t455 = a[241];
        const double t456 = t7 * t455;
        const double t457 = a[134];
        const double t459 = (t456 + t457) * t7;
        const double t460 = t33 * t455;
        const double t461 = a[400];
        const double t462 = t7 * t461;
        const double t464 = (t460 + t462 + t457) * t33;
        const double t466 = t98 * a[372];
        const double t467 = a[192];
        const double t468 = t33 * t467;
        const double t469 = t7 * t467;
        const double t470 = a[119];
        const double t472 = (t466 + t468 + t469 + t470) * t98;
        const double t474 = (t454 + t459 + t464 + t472) * t98;
        const double t475 = a[47];
        const double t476 = a[403];
        const double t477 = t7 * t476;
        const double t478 = a[147];
        const double t480 = (t477 + t478) * t7;
        const double t481 = t33 * t476;
        const double t482 = a[389];
        const double t483 = t7 * t482;
        const double t485 = (t481 + t483 + t478) * t33;
        const double t487 = t98 * a[201];
        const double t488 = a[344];
        const double t489 = t33 * t488;
        const double t490 = t7 * t488;
        const double t491 = a[165];
        const double t493 = (t487 + t489 + t490 + t491) * t98;
        const double t494 = a[382];
        const double t495 = t241 * t494;
        const double t496 = a[188];
        const double t497 = t98 * t496;
        const double t498 = a[207];
        const double t499 = t33 * t498;
        const double t500 = t7 * t498;
        const double t501 = a[114];
        const double t503 = (t495 + t497 + t499 + t500 + t501) * t241;
        const double t505 = (t475 + t480 + t485 + t493 + t503) * t241;
        const double t507 = (t435 + t443 + t453 + t474 + t505) * t241;
        const double t508 = a[14];
        const double t509 = a[45];
        const double t510 = a[255];
        const double t511 = t7 * t510;
        const double t512 = a[76];
        const double t514 = (t511 + t512) * t7;
        const double t516 = (t509 + t514) * t7;
        const double t517 = a[193];
        const double t518 = t7 * t517;
        const double t519 = a[146];
        const double t521 = (t518 + t519) * t7;
        const double t522 = t33 * t510;
        const double t524 = (t522 + t518 + t512) * t33;
        const double t526 = (t509 + t521 + t524) * t33;
        const double t527 = a[18];
        const double t528 = a[342];
        const double t529 = t7 * t528;
        const double t530 = a[72];
        const double t532 = (t529 + t530) * t7;
        const double t533 = t33 * t528;
        const double t534 = a[351];
        const double t535 = t7 * t534;
        const double t537 = (t533 + t535 + t530) * t33;
        const double t539 = t98 * a[213];
        const double t540 = a[221];
        const double t541 = t33 * t540;
        const double t542 = t7 * t540;
        const double t543 = a[112];
        const double t545 = (t539 + t541 + t542 + t543) * t98;
        const double t547 = (t527 + t532 + t537 + t545) * t98;
        const double t548 = a[41];
        const double t549 = a[326];
        const double t550 = t7 * t549;
        const double t551 = a[105];
        const double t553 = (t550 + t551) * t7;
        const double t554 = t33 * t549;
        const double t555 = a[286];
        const double t556 = t7 * t555;
        const double t558 = (t554 + t556 + t551) * t33;
        const double t560 = t98 * a[287];
        const double t561 = a[202];
        const double t562 = t33 * t561;
        const double t563 = t7 * t561;
        const double t564 = a[173];
        const double t566 = (t560 + t562 + t563 + t564) * t98;
        const double t567 = a[316];
        const double t568 = t241 * t567;
        const double t569 = a[220];
        const double t570 = t569 * t98;
        const double t571 = a[347];
        const double t572 = t33 * t571;
        const double t573 = t7 * t571;
        const double t574 = a[137];
        const double t576 = (t568 + t570 + t572 + t573 + t574) * t241;
        const double t578 = (t548 + t553 + t558 + t566 + t576) * t241;
        const double t579 = a[30];
        const double t580 = a[336];
        const double t581 = t7 * t580;
        const double t582 = a[102];
        const double t584 = (t581 + t582) * t7;
        const double t585 = t33 * t580;
        const double t586 = a[228];
        const double t587 = t7 * t586;
        const double t589 = (t585 + t587 + t582) * t33;
        const double t591 = t98 * a[304];
        const double t592 = a[219];
        const double t593 = t33 * t592;
        const double t594 = t7 * t592;
        const double t595 = a[168];
        const double t597 = (t591 + t593 + t594 + t595) * t98;
        const double t599 = t241 * a[423];
        const double t600 = a[416];
        const double t601 = t98 * t600;
        const double t602 = a[322];
        const double t603 = t33 * t602;
        const double t604 = t7 * t602;
        const double t605 = a[159];
        const double t607 = (t599 + t601 + t603 + t604 + t605) * t241;
        const double t608 = a[337];
        const double t609 = t340 * t608;
        const double t610 = a[289];
        const double t611 = t241 * t610;
        const double t612 = a[247];
        const double t613 = t612 * t98;
        const double t614 = a[290];
        const double t615 = t33 * t614;
        const double t616 = t7 * t614;
        const double t617 = a[82];
        const double t619 = (t609 + t611 + t613 + t615 + t616 + t617) * t340;
        const double t621 = (t579 + t584 + t589 + t597 + t607 + t619) * t340;
        const double t623 = (t508 + t516 + t526 + t547 + t578 + t621) * t340;
        const double t624 = a[35];
        const double t625 = a[246];
        const double t626 = t7 * t625;
        const double t627 = a[123];
        const double t629 = (t626 + t627) * t7;
        const double t631 = (t624 + t629) * t7;
        const double t632 = a[345];
        const double t633 = t7 * t632;
        const double t634 = a[98];
        const double t636 = (t633 + t634) * t7;
        const double t637 = t33 * t625;
        const double t639 = (t637 + t633 + t627) * t33;
        const double t641 = (t624 + t636 + t639) * t33;
        const double t642 = a[39];
        const double t643 = a[338];
        const double t644 = t7 * t643;
        const double t645 = a[121];
        const double t647 = (t644 + t645) * t7;
        const double t648 = t33 * t643;
        const double t649 = a[310];
        const double t650 = t7 * t649;
        const double t652 = (t648 + t650 + t645) * t33;
        const double t654 = t98 * a[406];
        const double t655 = a[320];
        const double t656 = t33 * t655;
        const double t657 = t7 * t655;
        const double t658 = a[170];
        const double t660 = (t654 + t656 + t657 + t658) * t98;
        const double t662 = (t642 + t647 + t652 + t660) * t98;
        const double t663 = a[58];
        const double t664 = a[325];
        const double t665 = t7 * t664;
        const double t666 = a[144];
        const double t668 = (t665 + t666) * t7;
        const double t669 = t33 * t664;
        const double t670 = a[331];
        const double t671 = t7 * t670;
        const double t673 = (t669 + t671 + t666) * t33;
        const double t675 = t98 * a[223];
        const double t676 = a[214];
        const double t677 = t33 * t676;
        const double t678 = t7 * t676;
        const double t679 = a[99];
        const double t681 = (t675 + t677 + t678 + t679) * t98;
        const double t682 = a[256];
        const double t683 = t241 * t682;
        const double t684 = a[376];
        const double t685 = t684 * t98;
        const double t686 = a[232];
        const double t687 = t33 * t686;
        const double t688 = t7 * t686;
        const double t689 = a[133];
        const double t691 = (t683 + t685 + t687 + t688 + t689) * t241;
        const double t694 = a[52];
        const double t695 = a[300];
        const double t696 = t7 * t695;
        const double t697 = a[67];
        const double t699 = (t696 + t697) * t7;
        const double t700 = t33 * t695;
        const double t701 = a[388];
        const double t702 = t7 * t701;
        const double t704 = (t700 + t702 + t697) * t33;
        const double t706 = t98 * a[261];
        const double t707 = a[399];
        const double t708 = t33 * t707;
        const double t709 = t7 * t707;
        const double t710 = a[178];
        const double t712 = (t706 + t708 + t709 + t710) * t98;
        const double t713 = a[391];
        const double t714 = t241 * t713;
        const double t715 = a[278];
        const double t716 = t98 * t715;
        const double t717 = a[356];
        const double t718 = t33 * t717;
        const double t719 = t7 * t717;
        const double t720 = a[74];
        const double t722 = (t714 + t716 + t718 + t719 + t720) * t241;
        const double t723 = a[362];
        const double t724 = t340 * t723;
        const double t725 = a[428];
        const double t726 = t725 * t241;
        const double t727 = a[392];
        const double t728 = t727 * t98;
        const double t729 = a[204];
        const double t730 = t33 * t729;
        const double t731 = t7 * t729;
        const double t732 = a[118];
        const double t734 = (t724 + t726 + t728 + t730 + t731 + t732) * t340;
        const double t721 = x[nv + nd * 2];
        const double t738 = (t631 + t641 + t662 + (t663 + t668 + t673 + t681 + t691) * t241 +
                             (t694 + t699 + t704 + t712 + t722 + t734) * t340) *
                            t721;
        const double t741 = t241 * t608;
        const double t743 = (t741 + t613 + t615 + t616 + t617) * t241;
        const double t745 = (t579 + t584 + t589 + t597 + t743) * t241;
        const double t747 = (t508 + t516 + t526 + t547 + t745) * t241;
        const double t749 = (t611 + t601 + t603 + t604 + t605) * t241;
        const double t751 = (t548 + t553 + t558 + t566 + t749) * t241;
        const double t753 = (t599 + t570 + t572 + t573 + t574) * t241;
        const double t754 = t340 * t494;
        const double t756 = (t754 + t568 + t497 + t499 + t500 + t501) * t340;
        const double t758 = (t475 + t480 + t485 + t493 + t753 + t756) * t340;
        const double t760 = (t435 + t443 + t453 + t474 + t751 + t758) * t340;
        const double t761 = a[27];
        const double t762 = a[321];
        const double t763 = t7 * t762;
        const double t764 = a[68];
        const double t766 = (t763 + t764) * t7;
        const double t769 = a[317];
        const double t770 = t7 * t769;
        const double t771 = a[184];
        const double t773 = (t770 + t771) * t7;
        const double t774 = t33 * t762;
        const double t776 = (t774 + t770 + t764) * t33;
        const double t779 = a[34];
        const double t780 = a[292];
        const double t781 = t7 * t780;
        const double t782 = a[115];
        const double t784 = (t781 + t782) * t7;
        const double t785 = t33 * t780;
        const double t786 = a[411];
        const double t787 = t7 * t786;
        const double t789 = (t785 + t787 + t782) * t33;
        const double t791 = t98 * a[408];
        const double t792 = a[236];
        const double t793 = t33 * t792;
        const double t794 = t7 * t792;
        const double t795 = a[130];
        const double t797 = (t791 + t793 + t794 + t795) * t98;
        const double t800 = a[20];
        const double t801 = a[216];
        const double t802 = t7 * t801;
        const double t803 = a[117];
        const double t805 = (t802 + t803) * t7;
        const double t806 = t33 * t801;
        const double t807 = a[339];
        const double t808 = t7 * t807;
        const double t810 = (t806 + t808 + t803) * t33;
        const double t812 = t98 * a[360];
        const double t813 = a[303];
        const double t814 = t33 * t813;
        const double t815 = t7 * t813;
        const double t816 = a[138];
        const double t818 = (t812 + t814 + t815 + t816) * t98;
        const double t819 = a[248];
        const double t820 = t241 * t819;
        const double t821 = a[296];
        const double t822 = t821 * t98;
        const double t823 = a[230];
        const double t824 = t33 * t823;
        const double t825 = t7 * t823;
        const double t826 = a[109];
        const double t828 = (t820 + t822 + t824 + t825 + t826) * t241;
        const double t831 = a[421];
        const double t832 = t831 * t241;
        const double t833 = a[217];
        const double t834 = t833 * t98;
        const double t835 = a[358];
        const double t836 = t33 * t835;
        const double t837 = t7 * t835;
        const double t838 = a[78];
        const double t840 = (t832 + t834 + t836 + t837 + t838) * t241;
        const double t841 = t340 * t819;
        const double t843 = (t841 + t832 + t822 + t824 + t825 + t826) * t340;
        const double t846 = (t761 + t766) * t7 + (t761 + t773 + t776) * t33 + (t779 + t784 + t789 + t797) * t98 +
                            (t800 + t805 + t810 + t818 + t828) * t241 +
                            (t800 + t805 + t810 + t818 + t840 + t843) * t340;
        const double t847 = t846 * t721;
        const double t848 = t241 * t723;
        const double t850 = (t848 + t728 + t730 + t731 + t732) * t241;
        const double t854 = (t726 + t716 + t718 + t719 + t720) * t241;
        const double t855 = t340 * t682;
        const double t857 = (t855 + t714 + t685 + t687 + t688 + t689) * t340;
        const double t852 = x[nv + nd * 1];
        const double t861 = (t631 + t641 + t662 + (t694 + t699 + t704 + t712 + t850) * t241 +
                             (t663 + t668 + t673 + t681 + t854 + t857) * t340) *
                            t852;
        const double t864 = a[15];
        const double t865 = a[57];
        const double t866 = a[405];
        const double t867 = t7 * t866;
        const double t868 = a[120];
        const double t870 = (t867 + t868) * t7;
        const double t872 = (t865 + t870) * t7;
        const double t874 = (t864 + t872) * t7;
        const double t875 = a[44];
        const double t876 = a[237];
        const double t877 = t7 * t876;
        const double t878 = a[152];
        const double t880 = (t877 + t878) * t7;
        const double t882 = (t875 + t880) * t7;
        const double t884 = t7 * a[273];
        const double t886 = (t884 + t878) * t7;
        const double t887 = t33 * t866;
        const double t889 = (t887 + t877 + t868) * t33;
        const double t891 = (t865 + t886 + t889) * t33;
        const double t893 = (t864 + t882 + t891) * t33;
        const double t894 = a[16];
        const double t895 = a[29];
        const double t896 = a[393];
        const double t897 = t7 * t896;
        const double t898 = a[116];
        const double t900 = (t897 + t898) * t7;
        const double t902 = (t895 + t900) * t7;
        const double t903 = a[291];
        const double t904 = t7 * t903;
        const double t905 = a[175];
        const double t907 = (t904 + t905) * t7;
        const double t908 = t33 * t896;
        const double t910 = (t908 + t904 + t898) * t33;
        const double t912 = (t895 + t907 + t910) * t33;
        const double t913 = a[33];
        const double t914 = a[340];
        const double t915 = t7 * t914;
        const double t916 = a[149];
        const double t918 = (t915 + t916) * t7;
        const double t919 = t33 * t914;
        const double t920 = a[262];
        const double t921 = t7 * t920;
        const double t923 = (t919 + t921 + t916) * t33;
        const double t925 = t98 * a[424];
        const double t926 = a[299];
        const double t927 = t33 * t926;
        const double t928 = t7 * t926;
        const double t929 = a[92];
        const double t931 = (t925 + t927 + t928 + t929) * t98;
        const double t933 = (t913 + t918 + t923 + t931) * t98;
        const double t935 = (t894 + t902 + t912 + t933) * t98;
        const double t936 = a[12];
        const double t937 = a[21];
        const double t938 = a[315];
        const double t939 = t7 * t938;
        const double t940 = a[111];
        const double t942 = (t939 + t940) * t7;
        const double t944 = (t937 + t942) * t7;
        const double t945 = a[398];
        const double t946 = t7 * t945;
        const double t947 = a[96];
        const double t949 = (t946 + t947) * t7;
        const double t950 = t33 * t938;
        const double t952 = (t950 + t946 + t940) * t33;
        const double t954 = (t937 + t949 + t952) * t33;
        const double t955 = a[48];
        const double t956 = a[199];
        const double t957 = t7 * t956;
        const double t958 = a[90];
        const double t960 = (t957 + t958) * t7;
        const double t961 = t33 * t956;
        const double t962 = a[415];
        const double t963 = t7 * t962;
        const double t965 = (t961 + t963 + t958) * t33;
        const double t967 = t98 * a[298];
        const double t968 = a[224];
        const double t969 = t33 * t968;
        const double t970 = t7 * t968;
        const double t971 = a[135];
        const double t973 = (t967 + t969 + t970 + t971) * t98;
        const double t975 = (t955 + t960 + t965 + t973) * t98;
        const double t976 = a[63];
        const double t977 = a[229];
        const double t978 = t7 * t977;
        const double t979 = a[89];
        const double t981 = (t978 + t979) * t7;
        const double t982 = t33 * t977;
        const double t983 = a[420];
        const double t984 = t7 * t983;
        const double t986 = (t982 + t984 + t979) * t33;
        const double t988 = t98 * a[380];
        const double t989 = a[284];
        const double t990 = t33 * t989;
        const double t991 = t7 * t989;
        const double t992 = a[106];
        const double t994 = (t988 + t990 + t991 + t992) * t98;
        const double t995 = a[318];
        const double t996 = t241 * t995;
        const double t997 = a[266];
        const double t998 = t997 * t98;
        const double t999 = a[341];
        const double t1000 = t33 * t999;
        const double t1001 = t7 * t999;
        const double t1002 = a[171];
        const double t1004 = (t996 + t998 + t1000 + t1001 + t1002) * t241;
        const double t1006 = (t976 + t981 + t986 + t994 + t1004) * t241;
        const double t1008 = (t936 + t944 + t954 + t975 + t1006) * t241;
        const double t1009 = a[65];
        const double t1010 = a[252];
        const double t1011 = t7 * t1010;
        const double t1012 = a[87];
        const double t1014 = (t1011 + t1012) * t7;
        const double t1015 = t33 * t1010;
        const double t1016 = a[427];
        const double t1017 = t7 * t1016;
        const double t1019 = (t1015 + t1017 + t1012) * t33;
        const double t1021 = t98 * a[419];
        const double t1022 = a[368];
        const double t1023 = t33 * t1022;
        const double t1024 = t7 * t1022;
        const double t1025 = a[100];
        const double t1027 = (t1021 + t1023 + t1024 + t1025) * t98;
        const double t1028 = a[280];
        const double t1029 = t1028 * t241;
        const double t1030 = a[407];
        const double t1031 = t1030 * t98;
        const double t1032 = a[319];
        const double t1033 = t33 * t1032;
        const double t1034 = t7 * t1032;
        const double t1035 = a[129];
        const double t1037 = (t1029 + t1031 + t1033 + t1034 + t1035) * t241;
        const double t1039 = (t1009 + t1014 + t1019 + t1027 + t1037) * t241;
        const double t1041 = t241 * a[212];
        const double t1043 = (t1041 + t1031 + t1033 + t1034 + t1035) * t241;
        const double t1044 = t340 * t995;
        const double t1046 = (t1044 + t1029 + t998 + t1000 + t1001 + t1002) * t340;
        const double t1048 = (t976 + t981 + t986 + t994 + t1043 + t1046) * t340;
        const double t1050 = (t936 + t944 + t954 + t975 + t1039 + t1048) * t340;
        const double t1051 = a[38];
        const double t1052 = a[365];
        const double t1053 = t7 * t1052;
        const double t1054 = a[97];
        const double t1056 = (t1053 + t1054) * t7;
        const double t1058 = (t1051 + t1056) * t7;
        const double t1059 = a[363];
        const double t1060 = t7 * t1059;
        const double t1061 = a[88];
        const double t1063 = (t1060 + t1061) * t7;
        const double t1064 = t33 * t1052;
        const double t1066 = (t1064 + t1060 + t1054) * t33;
        const double t1068 = (t1051 + t1063 + t1066) * t33;
        const double t1069 = a[53];
        const double t1070 = a[379];
        const double t1071 = t7 * t1070;
        const double t1072 = a[81];
        const double t1074 = (t1071 + t1072) * t7;
        const double t1075 = t33 * t1070;
        const double t1076 = a[414];
        const double t1077 = t7 * t1076;
        const double t1079 = (t1075 + t1077 + t1072) * t33;
        const double t1081 = t98 * a[364];
        const double t1082 = a[222];
        const double t1083 = t33 * t1082;
        const double t1084 = t7 * t1082;
        const double t1085 = a[142];
        const double t1087 = (t1081 + t1083 + t1084 + t1085) * t98;
        const double t1089 = (t1069 + t1074 + t1079 + t1087) * t98;
        const double t1090 = a[61];
        const double t1091 = a[283];
        const double t1092 = t7 * t1091;
        const double t1093 = a[164];
        const double t1095 = (t1092 + t1093) * t7;
        const double t1096 = t33 * t1091;
        const double t1097 = a[205];
        const double t1098 = t7 * t1097;
        const double t1100 = (t1096 + t1098 + t1093) * t33;
        const double t1102 = t98 * a[396];
        const double t1103 = a[343];
        const double t1104 = t33 * t1103;
        const double t1105 = t7 * t1103;
        const double t1106 = a[172];
        const double t1108 = (t1102 + t1104 + t1105 + t1106) * t98;
        const double t1109 = a[377];
        const double t1110 = t241 * t1109;
        const double t1111 = a[334];
        const double t1112 = t1111 * t98;
        const double t1113 = a[267];
        const double t1114 = t33 * t1113;
        const double t1115 = t7 * t1113;
        const double t1116 = a[169];
        const double t1118 = (t1110 + t1112 + t1114 + t1115 + t1116) * t241;
        const double t1120 = (t1090 + t1095 + t1100 + t1108 + t1118) * t241;
        const double t1121 = a[54];
        const double t1122 = a[218];
        const double t1123 = t7 * t1122;
        const double t1124 = a[94];
        const double t1126 = (t1123 + t1124) * t7;
        const double t1127 = t33 * t1122;
        const double t1128 = a[257];
        const double t1129 = t7 * t1128;
        const double t1131 = (t1127 + t1129 + t1124) * t33;
        const double t1133 = t98 * a[369];
        const double t1134 = a[381];
        const double t1135 = t33 * t1134;
        const double t1136 = t7 * t1134;
        const double t1137 = a[104];
        const double t1139 = (t1133 + t1135 + t1136 + t1137) * t98;
        const double t1140 = a[197];
        const double t1141 = t1140 * t241;
        const double t1142 = a[253];
        const double t1143 = t1142 * t98;
        const double t1144 = a[374];
        const double t1145 = t33 * t1144;
        const double t1146 = t7 * t1144;
        const double t1147 = a[163];
        const double t1149 = (t1141 + t1143 + t1145 + t1146 + t1147) * t241;
        const double t1150 = a[385];
        const double t1151 = t340 * t1150;
        const double t1152 = a[367];
        const double t1153 = t1152 * t241;
        const double t1154 = a[270];
        const double t1155 = t1154 * t98;
        const double t1156 = a[332];
        const double t1157 = t33 * t1156;
        const double t1158 = t7 * t1156;
        const double t1159 = a[162];
        const double t1161 = (t1151 + t1153 + t1155 + t1157 + t1158 + t1159) * t340;
        const double t1163 = (t1121 + t1126 + t1131 + t1139 + t1149 + t1161) * t340;
        const double t1164 = a[239];
        const double t1165 = t7 * t1164;
        const double t1166 = a[107];
        const double t1168 = (t1165 + t1166) * t7;
        const double t1169 = t33 * t1164;
        const double t1170 = a[348];
        const double t1171 = t7 * t1170;
        const double t1173 = (t1169 + t1171 + t1166) * t33;
        const double t1175 = t98 * a[231];
        const double t1176 = a[313];
        const double t1177 = t33 * t1176;
        const double t1178 = t7 * t1176;
        const double t1179 = a[148];
        const double t1181 = (t1175 + t1177 + t1178 + t1179) * t98;
        const double t1182 = a[308];
        const double t1183 = t241 * t1182;
        const double t1184 = a[297];
        const double t1185 = t1184 * t98;
        const double t1186 = a[209];
        const double t1187 = t33 * t1186;
        const double t1188 = t7 * t1186;
        const double t1189 = a[95];
        const double t1192 = a[375];
        const double t1193 = t340 * t1192;
        const double t1194 = a[194];
        const double t1195 = t1194 * t241;
        const double t1196 = a[328];
        const double t1197 = t1196 * t98;
        const double t1198 = a[190];
        const double t1199 = t33 * t1198;
        const double t1200 = t7 * t1198;
        const double t1201 = a[166];
        const double t1205 = (t1168 + t1173 + t1181 + (t1183 + t1185 + t1187 + t1188 + t1189) * t241 +
                              (t1193 + t1195 + t1197 + t1199 + t1200 + t1201) * t340) *
                             t721;
        const double t1207 = (t1058 + t1068 + t1089 + t1120 + t1163 + t1205) * t721;
        const double t1208 = t241 * t1150;
        const double t1210 = (t1208 + t1155 + t1157 + t1158 + t1159) * t241;
        const double t1212 = (t1121 + t1126 + t1131 + t1139 + t1210) * t241;
        const double t1214 = (t1153 + t1143 + t1145 + t1146 + t1147) * t241;
        const double t1215 = t340 * t1109;
        const double t1217 = (t1215 + t1141 + t1112 + t1114 + t1115 + t1116) * t340;
        const double t1219 = (t1090 + t1095 + t1100 + t1108 + t1214 + t1217) * t340;
        const double t1220 = a[208];
        const double t1221 = t7 * t1220;
        const double t1222 = a[86];
        const double t1225 = t33 * t1220;
        const double t1226 = a[384];
        const double t1227 = t7 * t1226;
        const double t1231 = t98 * a[390];
        const double t1232 = a[355];
        const double t1233 = t33 * t1232;
        const double t1234 = t7 * t1232;
        const double t1235 = a[156];
        const double t1238 = a[305];
        const double t1239 = t241 * t1238;
        const double t1240 = a[187];
        const double t1241 = t1240 * t98;
        const double t1242 = a[294];
        const double t1243 = t33 * t1242;
        const double t1244 = t7 * t1242;
        const double t1245 = a[127];
        const double t1248 = t340 * t1238;
        const double t1249 = a[418];
        const double t1250 = t1249 * t241;
        const double t1253 = (t1221 + t1222) * t7 + (t1225 + t1227 + t1222) * t33 +
                             (t1231 + t1233 + t1234 + t1235) * t98 + (t1239 + t1241 + t1243 + t1244 + t1245) * t241 +
                             (t1248 + t1250 + t1241 + t1243 + t1244 + t1245) * t340;
        const double t1254 = t1253 * t721;
        const double t1255 = t241 * t1192;
        const double t1258 = t340 * t1182;
        const double t1262 = (t1168 + t1173 + t1181 + (t1255 + t1197 + t1199 + t1200 + t1201) * t241 +
                              (t1258 + t1195 + t1185 + t1187 + t1188 + t1189) * t340) *
                             t852;
        const double t1264 = (t1058 + t1068 + t1089 + t1212 + t1219 + t1254 + t1262) * t852;
        const double t1265 = a[28];
        const double t1266 = a[410];
        const double t1267 = t7 * t1266;
        const double t1268 = a[131];
        const double t1270 = (t1267 + t1268) * t7;
        const double t1273 = a[378];
        const double t1274 = t7 * t1273;
        const double t1275 = a[73];
        const double t1277 = (t1274 + t1275) * t7;
        const double t1278 = t33 * t1266;
        const double t1280 = (t1278 + t1274 + t1268) * t33;
        const double t1283 = a[55];
        const double t1284 = a[401];
        const double t1285 = t7 * t1284;
        const double t1286 = a[139];
        const double t1288 = (t1285 + t1286) * t7;
        const double t1289 = t33 * t1284;
        const double t1290 = a[327];
        const double t1291 = t7 * t1290;
        const double t1293 = (t1289 + t1291 + t1286) * t33;
        const double t1295 = t98 * a[353];
        const double t1296 = a[346];
        const double t1297 = t33 * t1296;
        const double t1298 = t7 * t1296;
        const double t1299 = a[75];
        const double t1301 = (t1295 + t1297 + t1298 + t1299) * t98;
        const double t1304 = a[19];
        const double t1305 = a[352];
        const double t1306 = t7 * t1305;
        const double t1307 = a[108];
        const double t1309 = (t1306 + t1307) * t7;
        const double t1310 = t33 * t1305;
        const double t1311 = a[240];
        const double t1312 = t7 * t1311;
        const double t1314 = (t1310 + t1312 + t1307) * t33;
        const double t1316 = t98 * a[417];
        const double t1317 = a[243];
        const double t1318 = t33 * t1317;
        const double t1319 = t7 * t1317;
        const double t1320 = a[79];
        const double t1322 = (t1316 + t1318 + t1319 + t1320) * t98;
        const double t1323 = a[195];
        const double t1324 = t241 * t1323;
        const double t1325 = a[265];
        const double t1326 = t1325 * t98;
        const double t1327 = a[238];
        const double t1328 = t33 * t1327;
        const double t1329 = t7 * t1327;
        const double t1330 = a[157];
        const double t1332 = (t1324 + t1326 + t1328 + t1329 + t1330) * t241;
        const double t1335 = a[279];
        const double t1336 = t1335 * t241;
        const double t1337 = a[295];
        const double t1338 = t1337 * t98;
        const double t1339 = a[282];
        const double t1340 = t33 * t1339;
        const double t1341 = t7 * t1339;
        const double t1342 = a[77];
        const double t1344 = (t1336 + t1338 + t1340 + t1341 + t1342) * t241;
        const double t1345 = t340 * t1323;
        const double t1347 = (t1345 + t1336 + t1326 + t1328 + t1329 + t1330) * t340;
        const double t1350 = a[272];
        const double t1351 = t7 * t1350;
        const double t1352 = a[124];
        const double t1354 = (t1351 + t1352) * t7;
        const double t1355 = t33 * t1350;
        const double t1356 = a[259];
        const double t1357 = t7 * t1356;
        const double t1359 = (t1355 + t1357 + t1352) * t33;
        const double t1361 = t98 * a[234];
        const double t1362 = a[324];
        const double t1363 = t33 * t1362;
        const double t1364 = t7 * t1362;
        const double t1365 = a[143];
        const double t1367 = (t1361 + t1363 + t1364 + t1365) * t98;
        const double t1368 = a[276];
        const double t1369 = t241 * t1368;
        const double t1370 = a[206];
        const double t1371 = t1370 * t98;
        const double t1372 = a[254];
        const double t1373 = t33 * t1372;
        const double t1374 = t7 * t1372;
        const double t1375 = a[93];
        const double t1377 = (t1369 + t1371 + t1373 + t1374 + t1375) * t241;
        const double t1378 = a[200];
        const double t1379 = t340 * t1378;
        const double t1380 = a[301];
        const double t1381 = t1380 * t241;
        const double t1382 = a[281];
        const double t1383 = t1382 * t98;
        const double t1384 = a[306];
        const double t1385 = t33 * t1384;
        const double t1386 = t7 * t1384;
        const double t1387 = a[181];
        const double t1389 = (t1379 + t1381 + t1383 + t1385 + t1386 + t1387) * t340;
        const double t1390 = a[260];
        const double t1391 = t7 + t33;
        const double t1392 = t1390 * t1391;
        const double t1393 = a[225];
        const double t1395 = a[387];
        const double t1396 = t1395 * t98;
        const double t1397 = a[198];
        const double t1400 = (t1393 * t241 + t1397 * t340 + t1392 + t1396) * t721;
        const double t1403 = t241 * t1378;
        const double t1405 = (t1403 + t1383 + t1385 + t1386 + t1387) * t241;
        const double t1406 = t340 * t1368;
        const double t1408 = (t1406 + t1381 + t1371 + t1373 + t1374 + t1375) * t340;
        const double t1409 = a[371];
        const double t1411 = a[413];
        const double t1413 = a[191];
        const double t1416 = t1413 * t1391 + t1409 * t98 + t1411 * t241 + t1411 * t340;
        const double t1417 = t1416 * t721;
        const double t1421 = (t1393 * t340 + t1397 * t241 + t1392 + t1396) * t852;
        const double t1415 = x[nv + nd * 0];
        const double t1425 =
            ((t1265 + t1270) * t7 + (t1265 + t1277 + t1280) * t33 + (t1283 + t1288 + t1293 + t1301) * t98 +
             (t1304 + t1309 + t1314 + t1322 + t1332) * t241 + (t1304 + t1309 + t1314 + t1322 + t1344 + t1347) * t340 +
             (t1354 + t1359 + t1367 + t1377 + t1389 + t1400) * t721 +
             (t1354 + t1359 + t1367 + t1405 + t1408 + t1417 + t1421) * t852) *
            t1415;
        const double t1518 = t721 * t1411;
        const double t1539 = 2.0 * t341;
        const double t1558 = 2.0 * t599;
        const double t1628 = t340 * t1194;
        const double t1658 = t340 * t1380;
        const double t1682 = ((2.0 * t181 + t183 + t184 + t185) * t98 + t169 + t174 + t179 + t187) * t98;
        const double t1685 = (2.0 * t223 + t225 + t226 + t227) * t98;
        const double t1687 = 2.0 * t244;
        const double t1697 = t241 * t328;
        const double t1698 = 2.0 * t319;
        const double t1717 = ((2.0 * t424 + t426 + t427 + t428) * t98 + t412 + t417 + t422 + t430) * t98;
        const double t1720 = (2.0 * t466 + t468 + t469 + t470) * t98;
        const double t1722 = 2.0 * t487;
        const double t1729 = (2.0 * t539 + t541 + t542 + t543) * t98;
        const double t1730 = t241 * t569;
        const double t1731 = 2.0 * t560;
        const double t1735 = t241 * t600;
        const double t1736 = 2.0 * t591;
        const double t1743 = (2.0 * t654 + t656 + t657 + t658) * t98;
        const double t1745 = 2.0 * t675;
        const double t1749 = t241 * t715;
        const double t1750 = 2.0 * t706;
        const double t1773 = 2.0 * t812;
        const double t1799 = (2.0 * t967 + t969 + t970 + t971) * t98;
        const double t1801 = 2.0 * t988;
        const double t1806 = t241 * t1030;
        const double t1817 = (2.0 * t1081 + t1083 + t1084 + t1085) * t98;
        const double t1819 = 2.0 * t1102;
        const double t1823 = t241 * t1142;
        const double t1824 = 2.0 * t1133;
        const double t1829 = 2.0 * t1175;
        const double t1855 = 2.0 * t1316;
        const double t1865 = 2.0 * t1361;
        const double t1894 = t98 * t107;
        const double t1906 = ((2.0 * t143 + t133 + t124) * t33 + t121 + t142 + t145) * t33;
        const double t1910 = t98 * t182;
        const double t1915 =
            ((2.0 * t164 + t160 + t154) * t33 + t151 + t163 + t166 + (t1910 + 2.0 * t175 + t177 + t172) * t98) * t98;
        const double t1918 = (2.0 * t206 + t202 + t196) * t33;
        const double t1919 = t98 * t224;
        const double t1922 = (t1919 + 2.0 * t217 + t219 + t214) * t98;
        const double t1923 = t241 * t255;
        const double t1924 = t98 * t245;
        const double t1925 = 2.0 * t238;
        const double t1935 = t98 * t299;
        const double t1939 = t241 * t330;
        const double t1940 = t98 * t320;
        const double t1941 = 2.0 * t313;
        const double t1946 = t241 * t344;
        const double t1949 = t340 * t255;
        const double t1960 = ((2.0 * t386 + t376 + t367) * t33 + t364 + t385 + t388) * t33;
        const double t1964 = t98 * t425;
        const double t1969 =
            ((2.0 * t407 + t403 + t397) * t33 + t394 + t406 + t409 + (t1964 + 2.0 * t418 + t420 + t415) * t98) * t98;
        const double t1972 = (2.0 * t449 + t445 + t439) * t33;
        const double t1973 = t98 * t467;
        const double t1976 = (t1973 + 2.0 * t460 + t462 + t457) * t98;
        const double t1977 = t241 * t498;
        const double t1978 = t98 * t488;
        const double t1979 = 2.0 * t481;
        const double t1986 = (2.0 * t522 + t518 + t512) * t33;
        const double t1987 = t98 * t540;
        const double t1990 = (t1987 + 2.0 * t533 + t535 + t530) * t98;
        const double t1991 = t241 * t571;
        const double t1992 = t98 * t561;
        const double t1993 = 2.0 * t554;
        const double t1996 = t340 * t614;
        const double t1997 = t241 * t602;
        const double t1998 = t98 * t592;
        const double t1999 = 2.0 * t585;
        const double t2006 = (2.0 * t637 + t633 + t627) * t33;
        const double t2007 = t98 * t655;
        const double t2010 = (t2007 + 2.0 * t648 + t650 + t645) * t98;
        const double t2011 = t241 * t686;
        const double t2012 = t98 * t676;
        const double t2013 = 2.0 * t669;
        const double t2016 = t340 * t729;
        const double t2017 = t241 * t717;
        const double t2018 = t98 * t707;
        const double t2019 = 2.0 * t700;
        const double t2026 = t241 * t614;
        const double t2033 = t340 * t498;
        const double t2041 = t98 * t792;
        const double t2045 = t241 * t823;
        const double t2046 = t98 * t813;
        const double t2047 = 2.0 * t806;
        const double t2050 = t340 * t823;
        const double t2051 = t241 * t835;
        const double t2056 = t241 * t729;
        const double t2059 = t340 * t686;
        const double t2074 = t98 * t926;
        const double t2082 = (2.0 * t950 + t946 + t940) * t33;
        const double t2083 = t98 * t968;
        const double t2086 = (t2083 + 2.0 * t961 + t963 + t958) * t98;
        const double t2087 = t241 * t999;
        const double t2088 = t98 * t989;
        const double t2089 = 2.0 * t982;
        const double t2094 = t241 * t1032;
        const double t2095 = t98 * t1022;
        const double t2099 = t340 * t999;
        const double t2106 = (2.0 * t1064 + t1060 + t1054) * t33;
        const double t2107 = t98 * t1082;
        const double t2110 = (t2107 + 2.0 * t1075 + t1077 + t1072) * t98;
        const double t2111 = t241 * t1113;
        const double t2112 = t98 * t1103;
        const double t2113 = 2.0 * t1096;
        const double t2116 = t340 * t1156;
        const double t2117 = t241 * t1144;
        const double t2118 = t98 * t1134;
        const double t2119 = 2.0 * t1127;
        const double t2122 = t340 * t1198;
        const double t2123 = t241 * t1186;
        const double t2124 = t98 * t1176;
        const double t2125 = 2.0 * t1169;
        const double t2130 = t241 * t1156;
        const double t2133 = t340 * t1113;
        const double t2136 = t340 * t1242;
        const double t2137 = t241 * t1242;
        const double t2138 = t98 * t1232;
        const double t2142 = t340 * t1186;
        const double t2143 = t241 * t1198;
        const double t2151 = t98 * t1296;
        const double t2155 = t241 * t1327;
        const double t2156 = t98 * t1317;
        const double t2157 = 2.0 * t1310;
        const double t2160 = t340 * t1327;
        const double t2161 = t241 * t1339;
        const double t2164 = t721 * t1390;
        const double t2165 = t340 * t1384;
        const double t2166 = t241 * t1372;
        const double t2167 = t98 * t1362;
        const double t2168 = 2.0 * t1355;
        const double t2171 = t852 * t1390;
        const double t2172 = t721 * t1413;
        const double t2173 = t340 * t1372;
        const double t2174 = t241 * t1384;
        const double t2194 = 2.0 * t27;
        const double t2237 = ((2.0 * t123 + t124) * t7 + t121 + t126) * t7;
        const double t2246 = ((2.0 * t133 + t134) * t7 + t131 + t136 + (t132 * t33 + t134 + 2.0 * t140) * t33) * t33;
        const double t2259 = ((2.0 * t153 + t154) * t7 + t151 + t156 + (t159 * t33 + 2.0 * t160 + t161) * t33 +
                              (t176 * t33 + 2.0 * t171 + t172 + t1910) * t98) *
                             t98;
        const double t2262 = (2.0 * t195 + t196) * t7;
        const double t2266 = (t201 * t33 + 2.0 * t202 + t203) * t33;
        const double t2270 = (t218 * t33 + t1919 + 2.0 * t213 + t214) * t98;
        const double t2271 = t33 * t239;
        const double t2272 = 2.0 * t234;
        const double t2290 = t33 * t314;
        const double t2291 = 2.0 * t309;
        const double t2308 = ((2.0 * t366 + t367) * t7 + t364 + t369) * t7;
        const double t2317 = ((2.0 * t376 + t377) * t7 + t374 + t379 + (t375 * t33 + t377 + 2.0 * t383) * t33) * t33;
        const double t2330 = ((2.0 * t396 + t397) * t7 + t394 + t399 + (t402 * t33 + 2.0 * t403 + t404) * t33 +
                              (t419 * t33 + t1964 + 2.0 * t414 + t415) * t98) *
                             t98;
        const double t2333 = (2.0 * t438 + t439) * t7;
        const double t2337 = (t444 * t33 + 2.0 * t445 + t446) * t33;
        const double t2341 = (t461 * t33 + t1973 + 2.0 * t456 + t457) * t98;
        const double t2342 = t33 * t482;
        const double t2343 = 2.0 * t477;
        const double t2350 = (2.0 * t511 + t512) * t7;
        const double t2354 = (t517 * t33 + 2.0 * t518 + t519) * t33;
        const double t2358 = (t534 * t33 + t1987 + 2.0 * t529 + t530) * t98;
        const double t2359 = t33 * t555;
        const double t2360 = 2.0 * t550;
        const double t2363 = t33 * t586;
        const double t2364 = 2.0 * t581;
        const double t2371 = (2.0 * t626 + t627) * t7;
        const double t2375 = (t632 * t33 + 2.0 * t633 + t634) * t33;
        const double t2379 = (t649 * t33 + t2007 + 2.0 * t644 + t645) * t98;
        const double t2380 = t33 * t670;
        const double t2381 = 2.0 * t665;
        const double t2384 = t33 * t701;
        const double t2385 = 2.0 * t696;
        const double t2413 = t33 * t807;
        const double t2414 = 2.0 * t802;
        const double t2458 = (2.0 * t939 + t940) * t7;
        const double t2462 = (t945 * t33 + 2.0 * t946 + t947) * t33;
        const double t2466 = (t962 * t33 + t2083 + 2.0 * t957 + t958) * t98;
        const double t2467 = t33 * t983;
        const double t2468 = 2.0 * t978;
        const double t2483 = (2.0 * t1053 + t1054) * t7;
        const double t2487 = (t1059 * t33 + 2.0 * t1060 + t1061) * t33;
        const double t2491 = (t1076 * t33 + 2.0 * t1071 + t1072 + t2107) * t98;
        const double t2492 = t33 * t1097;
        const double t2493 = 2.0 * t1092;
        const double t2496 = t33 * t1128;
        const double t2497 = 2.0 * t1123;
        const double t2500 = t33 * t1170;
        const double t2501 = 2.0 * t1165;
        const double t2529 = t33 * t1311;
        const double t2530 = 2.0 * t1306;
        const double t2535 = t33 * t1356;
        const double t2536 = 2.0 * t1351;
        g[nv + nd * 0] = 2.0 * t1425 + t874 + t893 + t935 + t1008 + t1050 + t1207 + t1264;
        g[nv + nd * 1] = 2.0 * t861 + t373 + t392 + t434 + t747 + t760 + t847 +
                         (2.0 * t1262 + t1058 + t1068 + t1089 + t1212 + t1219 + t1254 +
                          (2.0 * t1421 + t1354 + t1359 + t1367 + t1405 + t1408 + t1417) * t1415) *
                             t1415;
        g[nv + nd * 2] = 2.0 * t738 + t373 + t392 + t434 + t507 + t623 + t846 * t852 +
                         (2.0 * t1205 + t1058 + t1068 + t1089 + t1120 + t1163 + t1253 * t852 +
                          (t1416 * t852 + t1354 + t1359 + t1367 + t1377 + t1389 + 2.0 * t1400) * t1415) *
                             t1415;
        g[nv + nd * 3] =
            (((2.0 * t354 + t327 + t254 + t256 + t257 + t258) * t340 + t232 + t237 + t242 + t250 + t353 + t356) * t340 +
             t192 + t200 + t210 + t231 + t351 + t358) *
                t340 +
            t119 + t130 + t149 + t191 + t339 + t360 +
            (((2.0 * t609 + t611 + t613 + t615 + t616 + t617) * t340 + t579 + t584 + t589 + t597 + t607 + t619) * t340 +
             t508 + t516 + t526 + t547 + t578 + t621 +
             ((2.0 * t724 + t726 + t728 + t730 + t731 + t732) * t340 + t694 + t699 + t704 + t712 + t722 + t734) *
                 t721) *
                t721 +
            (((2.0 * t754 + t568 + t497 + t499 + t500 + t501) * t340 + t475 + t480 + t485 + t493 + t753 + t756) * t340 +
             t435 + t443 + t453 + t474 + t751 + t758 +
             ((2.0 * t841 + t832 + t822 + t824 + t825 + t826) * t340 + t800 + t805 + t810 + t818 + t840 + t843) * t721 +
             ((2.0 * t855 + t714 + t685 + t687 + t688 + t689) * t340 + t663 + t668 + t673 + t681 + t854 + t857) *
                 t852) *
                t852 +
            (((2.0 * t1044 + t1029 + t998 + t1000 + t1001 + t1002) * t340 + t976 + t981 + t986 + t994 + t1043 + t1046) *
                 t340 +
             t936 + t944 + t954 + t975 + t1039 + t1048 +
             ((2.0 * t1151 + t1153 + t1155 + t1157 + t1158 + t1159) * t340 + t1121 + t1126 + t1131 + t1139 + t1149 +
              t1161 + (2.0 * t1193 + t1195 + t1197 + t1199 + t1200 + t1201) * t721) *
                 t721 +
             ((2.0 * t1215 + t1141 + t1112 + t1114 + t1115 + t1116) * t340 + t1090 + t1095 + t1100 + t1108 + t1214 +
              t1217 + (2.0 * t1248 + t1250 + t1241 + t1243 + t1244 + t1245) * t721 +
              (2.0 * t1258 + t1195 + t1185 + t1187 + t1188 + t1189) * t852) *
                 t852 +
             ((2.0 * t1345 + t1336 + t1326 + t1328 + t1329 + t1330) * t340 + t1304 + t1309 + t1314 + t1322 + t1344 +
              t1347 + (t1397 * t721 + 2.0 * t1379 + t1381 + t1383 + t1385 + t1386 + t1387) * t721 +
              (t1393 * t852 + t1371 + t1373 + t1374 + t1375 + t1381 + 2.0 * t1406 + t1518) * t852) *
                 t1415) *
                t1415;
        g[nv + nd * 4] =
            (((2.0 * t252 + t254 + t256 + t257 + t258) * t241 + t232 + t237 + t242 + t250 + t260) * t241 + t192 + t200 +
             t210 + t231 + t262) *
                t241 +
            t119 + t130 + t149 + t191 + t264 +
            (((2.0 * t327 + t329 + t331 + t332 + t333) * t241 + t307 + t312 + t317 + t325 + t335) * t241 + t267 + t275 +
             t285 + t306 + t337 +
             ((t1539 + t343 + t345 + t346 + t347) * t241 + t307 + t312 + t317 + t325 + t349 +
              (t326 * t340 + t1539 + t329 + t331 + t332 + t333) * t340) *
                 t340) *
                t340 +
            (((2.0 * t495 + t497 + t499 + t500 + t501) * t241 + t475 + t480 + t485 + t493 + t503) * t241 + t435 + t443 +
             t453 + t474 + t505 +
             ((2.0 * t568 + t570 + t572 + t573 + t574) * t241 + t548 + t553 + t558 + t566 + t576 +
              (t610 * t340 + t1558 + t601 + t603 + t604 + t605) * t340) *
                 t340 +
             ((2.0 * t683 + t685 + t687 + t688 + t689) * t241 + t663 + t668 + t673 + t681 + t691 +
              (t725 * t340 + 2.0 * t714 + t716 + t718 + t719 + t720) * t340) *
                 t721) *
                t721 +
            (((2.0 * t741 + t613 + t615 + t616 + t617) * t241 + t579 + t584 + t589 + t597 + t743) * t241 + t508 + t516 +
             t526 + t547 + t745 +
             ((2.0 * t611 + t601 + t603 + t604 + t605) * t241 + t548 + t553 + t558 + t566 + t749 +
              (t567 * t340 + t1558 + t570 + t572 + t573 + t574) * t340) *
                 t340 +
             ((2.0 * t820 + t822 + t824 + t825 + t826) * t241 + t800 + t805 + t810 + t818 + t828 +
              (t831 * t340 + 2.0 * t832 + t834 + t836 + t837 + t838) * t340) *
                 t721 +
             ((2.0 * t848 + t728 + t730 + t731 + t732) * t241 + t694 + t699 + t704 + t712 + t850 +
              (t713 * t340 + t716 + t718 + t719 + t720 + 2.0 * t726) * t340) *
                 t852) *
                t852 +
            (((2.0 * t996 + t998 + t1000 + t1001 + t1002) * t241 + t976 + t981 + t986 + t994 + t1004) * t241 + t936 +
             t944 + t954 + t975 + t1006 +
             ((2.0 * t1029 + t1031 + t1033 + t1034 + t1035) * t241 + t1009 + t1014 + t1019 + t1027 + t1037 +
              (t1028 * t340 + t1031 + t1033 + t1034 + t1035 + 2.0 * t1041) * t340) *
                 t340 +
             ((2.0 * t1110 + t1112 + t1114 + t1115 + t1116) * t241 + t1090 + t1095 + t1100 + t1108 + t1118 +
              (t1152 * t340 + 2.0 * t1141 + t1143 + t1145 + t1146 + t1147) * t340 +
              (t1628 + 2.0 * t1183 + t1185 + t1187 + t1188 + t1189) * t721) *
                 t721 +
             ((2.0 * t1208 + t1155 + t1157 + t1158 + t1159) * t241 + t1121 + t1126 + t1131 + t1139 + t1210 +
              (t1140 * t340 + t1143 + t1145 + t1146 + t1147 + 2.0 * t1153) * t340 +
              (t1249 * t340 + 2.0 * t1239 + t1241 + t1243 + t1244 + t1245) * t721 +
              (t1628 + 2.0 * t1255 + t1197 + t1199 + t1200 + t1201) * t852) *
                 t852 +
             ((2.0 * t1324 + t1326 + t1328 + t1329 + t1330) * t241 + t1304 + t1309 + t1314 + t1322 + t1332 +
              (t1335 * t340 + 2.0 * t1336 + t1338 + t1340 + t1341 + t1342) * t340 +
              (t1393 * t721 + 2.0 * t1369 + t1371 + t1373 + t1374 + t1375 + t1658) * t721 +
              (t1397 * t852 + t1383 + t1385 + t1386 + t1387 + 2.0 * t1403 + t1518 + t1658) * t852) *
                 t1415) *
                t1415;
        g[nv + nd * 5] =
            (((2.0 * t106 + t108 + t109 + t110) * t98 + t94 + t99 + t104 + t112) * t98 + t75 + t83 + t93 + t114) * t98 +
            t44 + t55 + t74 + t116 +
            (t1682 + t150 + t158 + t168 + t189 +
             (t1685 + t211 + t216 + t221 + t229 + (t253 * t241 + t1687 + t246 + t247 + t248) * t241) * t241) *
                t241 +
            (t1682 + t150 + t158 + t168 + t189 +
             ((2.0 * t298 + t300 + t301 + t302) * t98 + t286 + t291 + t296 + t304 +
              (t1697 + t1698 + t321 + t322 + t323) * t241) *
                 t241 +
             (t1685 + t211 + t216 + t221 + t229 + (t342 * t241 + t1698 + t321 + t322 + t323) * t241 +
              (t253 * t340 + t1687 + t1697 + t246 + t247 + t248) * t340) *
                 t340) *
                t340 +
            (t1717 + t393 + t401 + t411 + t432 +
             (t1720 + t454 + t459 + t464 + t472 + (t496 * t241 + t1722 + t489 + t490 + t491) * t241) * t241 +
             (t1729 + t527 + t532 + t537 + t545 + (t1730 + t1731 + t562 + t563 + t564) * t241 +
              (t612 * t340 + t1735 + t1736 + t593 + t594 + t595) * t340) *
                 t340 +
             (t1743 + t642 + t647 + t652 + t660 + (t684 * t241 + t1745 + t677 + t678 + t679) * t241 +
              (t727 * t340 + t1749 + t1750 + t708 + t709 + t710) * t340) *
                 t721) *
                t721 +
            (t1717 + t393 + t401 + t411 + t432 +
             (t1729 + t527 + t532 + t537 + t545 + (t612 * t241 + t1736 + t593 + t594 + t595) * t241) * t241 +
             (t1720 + t454 + t459 + t464 + t472 + (t1735 + t1731 + t562 + t563 + t564) * t241 +
              (t496 * t340 + t1722 + t1730 + t489 + t490 + t491) * t340) *
                 t340 +
             ((2.0 * t791 + t793 + t794 + t795) * t98 + t779 + t784 + t789 + t797 +
              (t821 * t241 + t1773 + t814 + t815 + t816) * t241 +
              (t833 * t241 + t821 * t340 + t1773 + t814 + t815 + t816) * t340) *
                 t721 +
             (t1743 + t642 + t647 + t652 + t660 + (t727 * t241 + t1750 + t708 + t709 + t710) * t241 +
              (t684 * t340 + t1745 + t1749 + t677 + t678 + t679) * t340) *
                 t852) *
                t852 +
            (((2.0 * t925 + t927 + t928 + t929) * t98 + t913 + t918 + t923 + t931) * t98 + t894 + t902 + t912 + t933 +
             (t1799 + t955 + t960 + t965 + t973 + (t997 * t241 + t1801 + t990 + t991 + t992) * t241) * t241 +
             (t1799 + t955 + t960 + t965 + t973 + (t1806 + 2.0 * t1021 + t1023 + t1024 + t1025) * t241 +
              (t997 * t340 + t1801 + t1806 + t990 + t991 + t992) * t340) *
                 t340 +
             (t1817 + t1069 + t1074 + t1079 + t1087 + (t1111 * t241 + t1104 + t1105 + t1106 + t1819) * t241 +
              (t1154 * t340 + t1135 + t1136 + t1137 + t1823 + t1824) * t340 +
              (t1184 * t241 + t1196 * t340 + t1177 + t1178 + t1179 + t1829) * t721) *
                 t721 +
             (t1817 + t1069 + t1074 + t1079 + t1087 + (t1154 * t241 + t1135 + t1136 + t1137 + t1824) * t241 +
              (t1111 * t340 + t1104 + t1105 + t1106 + t1819 + t1823) * t340 +
              (t1240 * t241 + t1240 * t340 + 2.0 * t1231 + t1233 + t1234 + t1235) * t721 +
              (t1184 * t340 + t1196 * t241 + t1177 + t1178 + t1179 + t1829) * t852) *
                 t852 +
             ((2.0 * t1295 + t1297 + t1298 + t1299) * t98 + t1283 + t1288 + t1293 + t1301 +
              (t1325 * t241 + t1318 + t1319 + t1320 + t1855) * t241 +
              (t1325 * t340 + t1337 * t241 + t1318 + t1319 + t1320 + t1855) * t340 +
              (t1370 * t241 + t1382 * t340 + t1395 * t721 + t1363 + t1364 + t1365 + t1865) * t721 +
              (t1370 * t340 + t1382 * t241 + t1395 * t852 + t1409 * t721 + t1363 + t1364 + t1365 + t1865) * t852) *
                 t1415) *
                t1415;
        g[nv + nd * 6] =
            (((2.0 * t35 + t18 + t6) * t33 + t3 + t34 + t37) * t33 + t2 + t32 + t39) * t33 + t1 + t25 + t41 +
            (((2.0 * t68 + t58 + t49) * t33 + t46 + t67 + t70) * t33 + t45 + t63 + t72 +
             ((2.0 * t89 + t85 + t79) * t33 + t76 + t88 + t91 + (t1894 + 2.0 * t100 + t102 + t97) * t98) * t98) *
                t98 +
            (t1906 + t120 + t138 + t147 + t1915 +
             (t1918 + t193 + t205 + t208 + t1922 + (t1923 + t1924 + t1925 + t240 + t235) * t241) * t241) *
                t241 +
            (t1906 + t120 + t138 + t147 + t1915 +
             ((2.0 * t281 + t277 + t271) * t33 + t268 + t280 + t283 + (t1935 + 2.0 * t292 + t294 + t289) * t98 +
              (t1939 + t1940 + t1941 + t315 + t310) * t241) *
                 t241 +
             (t1918 + t193 + t205 + t208 + t1922 + (t1946 + t1940 + t1941 + t315 + t310) * t241 +
              (t1949 + t1939 + t1924 + t1925 + t240 + t235) * t340) *
                 t340) *
                t340 +
            (t1960 + t363 + t381 + t390 + t1969 +
             (t1972 + t436 + t448 + t451 + t1976 + (t1977 + t1978 + t1979 + t483 + t478) * t241) * t241 +
             (t1986 + t509 + t521 + t524 + t1990 + (t1991 + t1992 + t1993 + t556 + t551) * t241 +
              (t1996 + t1997 + t1998 + t1999 + t587 + t582) * t340) *
                 t340 +
             (t2006 + t624 + t636 + t639 + t2010 + (t2011 + t2012 + t2013 + t671 + t666) * t241 +
              (t2016 + t2017 + t2018 + t2019 + t702 + t697) * t340) *
                 t721) *
                t721 +
            (t1960 + t363 + t381 + t390 + t1969 +
             (t1986 + t509 + t521 + t524 + t1990 + (t2026 + t1998 + t1999 + t587 + t582) * t241) * t241 +
             (t1972 + t436 + t448 + t451 + t1976 + (t1997 + t1992 + t1993 + t556 + t551) * t241 +
              (t2033 + t1991 + t1978 + t1979 + t483 + t478) * t340) *
                 t340 +
             ((2.0 * t774 + t770 + t764) * t33 + t761 + t773 + t776 + (t2041 + 2.0 * t785 + t787 + t782) * t98 +
              (t2045 + t2046 + t2047 + t808 + t803) * t241 + (t2050 + t2051 + t2046 + t2047 + t808 + t803) * t340) *
                 t721 +
             (t2006 + t624 + t636 + t639 + t2010 + (t2056 + t2018 + t2019 + t702 + t697) * t241 +
              (t2059 + t2017 + t2012 + t2013 + t671 + t666) * t340) *
                 t852) *
                t852 +
            (((2.0 * t887 + t877 + t868) * t33 + t865 + t886 + t889) * t33 + t864 + t882 + t891 +
             ((2.0 * t908 + t904 + t898) * t33 + t895 + t907 + t910 + (t2074 + 2.0 * t919 + t921 + t916) * t98) * t98 +
             (t2082 + t937 + t949 + t952 + t2086 + (t2087 + t2088 + t2089 + t984 + t979) * t241) * t241 +
             (t2082 + t937 + t949 + t952 + t2086 + (t2094 + t2095 + 2.0 * t1015 + t1017 + t1012) * t241 +
              (t2099 + t2094 + t2088 + t2089 + t984 + t979) * t340) *
                 t340 +
             (t2106 + t1051 + t1063 + t1066 + t2110 + (t2111 + t2112 + t2113 + t1098 + t1093) * t241 +
              (t2116 + t2117 + t2118 + t2119 + t1129 + t1124) * t340 +
              (t2122 + t2123 + t2124 + t2125 + t1171 + t1166) * t721) *
                 t721 +
             (t2106 + t1051 + t1063 + t1066 + t2110 + (t2130 + t2118 + t2119 + t1129 + t1124) * t241 +
              (t2133 + t2117 + t2112 + t2113 + t1098 + t1093) * t340 +
              (t2136 + t2137 + t2138 + 2.0 * t1225 + t1227 + t1222) * t721 +
              (t2142 + t2143 + t2124 + t2125 + t1171 + t1166) * t852) *
                 t852 +
             ((2.0 * t1278 + t1274 + t1268) * t33 + t1265 + t1277 + t1280 +
              (t2151 + 2.0 * t1289 + t1291 + t1286) * t98 + (t2155 + t2156 + t2157 + t1312 + t1307) * t241 +
              (t2160 + t2161 + t2156 + t2157 + t1312 + t1307) * t340 +
              (t2164 + t2165 + t2166 + t2167 + t2168 + t1357 + t1352) * t721 +
              (t2171 + t2172 + t2173 + t2174 + t2167 + t2168 + t1357 + t1352) * t852) *
                 t1415) *
                t1415;
        g[nv + nd * 7] =
            (((2.0 * t5 + t6) * t7 + t3 + t8) * t7 + t2 + t10) * t7 + t1 + t12 +
            (((2.0 * t18 + t19) * t7 + t16 + t21) * t7 + t15 + t23 +
             ((t2194 + t28) * t7 + t16 + t30 + (t17 * t33 + t19 + t2194) * t33) * t33) *
                t33 +
            (((2.0 * t48 + t49) * t7 + t46 + t51) * t7 + t45 + t53 +
             ((2.0 * t58 + t59) * t7 + t56 + t61 + (t57 * t33 + t59 + 2.0 * t65) * t33) * t33 +
             ((2.0 * t78 + t79) * t7 + t76 + t81 + (t84 * t33 + 2.0 * t85 + t86) * t33 +
              (t101 * t33 + t1894 + 2.0 * t96 + t97) * t98) *
                 t98) *
                t98 +
            (t2237 + t120 + t128 + t2246 + t2259 +
             (t2262 + t193 + t198 + t2266 + t2270 + (t1923 + t1924 + t2271 + t2272 + t235) * t241) * t241) *
                t241 +
            (t2237 + t120 + t128 + t2246 + t2259 +
             ((2.0 * t270 + t271) * t7 + t268 + t273 + (t276 * t33 + 2.0 * t277 + t278) * t33 +
              (t293 * t33 + t1935 + 2.0 * t288 + t289) * t98 + (t1939 + t1940 + t2290 + t2291 + t310) * t241) *
                 t241 +
             (t2262 + t193 + t198 + t2266 + t2270 + (t1946 + t1940 + t2290 + t2291 + t310) * t241 +
              (t1949 + t1939 + t1924 + t2271 + t2272 + t235) * t340) *
                 t340) *
                t340 +
            (t2308 + t363 + t371 + t2317 + t2330 +
             (t2333 + t436 + t441 + t2337 + t2341 + (t1977 + t1978 + t2342 + t2343 + t478) * t241) * t241 +
             (t2350 + t509 + t514 + t2354 + t2358 + (t1991 + t1992 + t2359 + t2360 + t551) * t241 +
              (t1996 + t1997 + t1998 + t2363 + t2364 + t582) * t340) *
                 t340 +
             (t2371 + t624 + t629 + t2375 + t2379 + (t2011 + t2012 + t2380 + t2381 + t666) * t241 +
              (t2016 + t2017 + t2018 + t2384 + t2385 + t697) * t340) *
                 t721) *
                t721 +
            (t2308 + t363 + t371 + t2317 + t2330 +
             (t2350 + t509 + t514 + t2354 + t2358 + (t2026 + t1998 + t2363 + t2364 + t582) * t241) * t241 +
             (t2333 + t436 + t441 + t2337 + t2341 + (t1997 + t1992 + t2359 + t2360 + t551) * t241 +
              (t2033 + t1991 + t1978 + t2342 + t2343 + t478) * t340) *
                 t340 +
             ((2.0 * t763 + t764) * t7 + t761 + t766 + (t769 * t33 + 2.0 * t770 + t771) * t33 +
              (t786 * t33 + t2041 + 2.0 * t781 + t782) * t98 + (t2045 + t2046 + t2413 + t2414 + t803) * t241 +
              (t2050 + t2051 + t2046 + t2413 + t2414 + t803) * t340) *
                 t721 +
             (t2371 + t624 + t629 + t2375 + t2379 + (t2056 + t2018 + t2384 + t2385 + t697) * t241 +
              (t2059 + t2017 + t2012 + t2380 + t2381 + t666) * t340) *
                 t852) *
                t852 +
            (((2.0 * t867 + t868) * t7 + t865 + t870) * t7 + t864 + t872 +
             ((2.0 * t877 + t878) * t7 + t875 + t880 + (t876 * t33 + t878 + 2.0 * t884) * t33) * t33 +
             ((2.0 * t897 + t898) * t7 + t895 + t900 + (t903 * t33 + 2.0 * t904 + t905) * t33 +
              (t920 * t33 + t2074 + 2.0 * t915 + t916) * t98) *
                 t98 +
             (t2458 + t937 + t942 + t2462 + t2466 + (t2087 + t2088 + t2467 + t2468 + t979) * t241) * t241 +
             (t2458 + t937 + t942 + t2462 + t2466 + (t1016 * t33 + 2.0 * t1011 + t1012 + t2094 + t2095) * t241 +
              (t2099 + t2094 + t2088 + t2467 + t2468 + t979) * t340) *
                 t340 +
             (t2483 + t1051 + t1056 + t2487 + t2491 + (t2111 + t2112 + t2492 + t2493 + t1093) * t241 +
              (t2116 + t2117 + t2118 + t2496 + t2497 + t1124) * t340 +
              (t2122 + t2123 + t2124 + t2500 + t2501 + t1166) * t721) *
                 t721 +
             (t2483 + t1051 + t1056 + t2487 + t2491 + (t2130 + t2118 + t2496 + t2497 + t1124) * t241 +
              (t2133 + t2117 + t2112 + t2492 + t2493 + t1093) * t340 +
              (t1226 * t33 + 2.0 * t1221 + t1222 + t2136 + t2137 + t2138) * t721 +
              (t2142 + t2143 + t2124 + t2500 + t2501 + t1166) * t852) *
                 t852 +
             ((2.0 * t1267 + t1268) * t7 + t1265 + t1270 + (t1273 * t33 + 2.0 * t1274 + t1275) * t33 +
              (t1290 * t33 + 2.0 * t1285 + t1286 + t2151) * t98 + (t2155 + t2156 + t2529 + t2530 + t1307) * t241 +
              (t2160 + t2161 + t2156 + t2529 + t2530 + t1307) * t340 +
              (t2164 + t2165 + t2166 + t2167 + t2535 + t2536 + t1352) * t721 +
              (t2171 + t2172 + t2173 + t2174 + t2167 + t2535 + t2536 + t1352) * t852) *
                 t1415) *
                t1415;
        energy[nv] = (t1 + t12) * t7 + (t1 + t25 + t41) * t33 + (t44 + t55 + t74 + t116) * t98 +
                     (t119 + t130 + t149 + t191 + t264) * t241 + (t119 + t130 + t149 + t191 + t339 + t360) * t340 +
                     (t373 + t392 + t434 + t507 + t623 + t738) * t721 +
                     (t373 + t392 + t434 + t747 + t760 + t847 + t861) * t852 +
                     (t874 + t893 + t935 + t1008 + t1050 + t1207 + t1264 + t1425) * t1415;
    }
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(20);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input linear constants (a):\n";
    for (size_t j = 0; j < 429; j++) {
        std::cerr << a[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Input polynomial variables (x) [vectorizable order!]:\n";
    for (size_t j = 0; j < 8 * nd; j++) {
        std::cerr << x[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output polynomial gradients (g) [vectorizable order!]:\n";
    for (size_t j = 0; j < 8 * nd; j++) {
        std::cerr << g[j] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output energies:\n";
    for (size_t j = 0; j < energy.size(); j++) {
        std::cerr << energy[j] << " , ";
    }
    std::cerr << std::endl;
#endif
    return energy;
}

}  // namespace h2o_ion
