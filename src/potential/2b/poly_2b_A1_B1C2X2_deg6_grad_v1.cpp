
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

#include "poly_2b_A1_B1C2X2_deg6_v1.h"

/**
 * @file poly_2b_A1_B1C2X2_deg6_grad_v1.cpp
 * @brief Contains the implementation of the polynomials with gradients for symmetry A1_B1C2X2
 */

/**
 * @namespace mbnrg_A1_B1C2X2_deg6
 * @brief Encloses the structure of the polynomial holder for symmetry A1_B1C2X2
 */
namespace mbnrg_A1_B1C2X2_deg6 {

double poly_A1_B1C2X2_deg6_v1::eval(const double x[8],
            const double a[906],
                  double g[8]) {
    const double t1 = a[2];
    const double t2 = a[11];
    const double t3 = a[26];
    const double t4 = a[97];
    const double t5 = a[692];
    const double t8 = x[4];
    const double t6 = t8*t5;
    const double t7 = a[305];
    const double t9 = (t6+t7)*t8;
    const double t11 = (t4+t9)*t8;
    const double t13 = (t3+t11)*t8;
    const double t15 = (t2+t13)*t8;
    const double t18 = a[9];
    const double t19 = a[24];
    const double t20 = a[87];
    const double t21 = a[703];
    const double t22 = t8*t21;
    const double t23 = a[187];
    const double t25 = (t22+t23)*t8;
    const double t27 = (t20+t25)*t8;
    const double t29 = (t19+t27)*t8;
    const double t31 = (t18+t29)*t8;
    const double t32 = a[91];
    const double t34 = t8*a[431];
    const double t35 = a[198];
    const double t37 = (t34+t35)*t8;
    const double t39 = (t32+t37)*t8;
    const double t41 = (t19+t39)*t8;
    const double t43 = t8*a[521];
    const double t45 = (t43+t35)*t8;
    const double t47 = (t20+t45)*t8;
    const double t49 = (t34+t23)*t8;
    const double t48 = x[3];
    const double t50 = t48*t5;
    const double t52 = (t50+t22+t7)*t48;
    const double t54 = (t4+t49+t52)*t48;
    const double t56 = (t3+t47+t54)*t48;
    const double t58 = (t2+t41+t56)*t48;
    const double t61 = a[1];
    const double t62 = a[13];
    const double t63 = a[30];
    const double t64 = a[139];
    const double t59 = x[7];
    const double t66 = t59*a[899];
    const double t67 = a[284];
    const double t69 = (t66+t67)*t59;
    const double t71 = (t64+t69)*t59;
    const double t73 = (t63+t71)*t59;
    const double t75 = (t62+t73)*t59;
    const double t76 = a[3];
    const double t77 = a[15];
    const double t78 = a[101];
    const double t80 = t59*a[522];
    const double t81 = a[310];
    const double t83 = (t80+t81)*t59;
    const double t85 = (t78+t83)*t59;
    const double t87 = (t77+t85)*t59;
    const double t88 = a[38];
    const double t89 = a[58];
    const double t91 = t59*a[762];
    const double t92 = a[383];
    const double t94 = (t91+t92)*t59;
    const double t96 = (t89+t94)*t59;
    const double t97 = a[57];
    const double t99 = t59*a[738];
    const double t100 = a[161];
    const double t102 = (t99+t100)*t59;
    const double t103 = a[455];
    const double t101 = x[6];
    const double t104 = t101*t103;
    const double t105 = a[393];
    const double t106 = t59*t105;
    const double t107 = a[352];
    const double t109 = (t104+t106+t107)*t101;
    const double t111 = (t97+t102+t109)*t101;
    const double t113 = (t88+t96+t111)*t101;
    const double t115 = (t76+t87+t113)*t101;
    const double t116 = a[20];
    const double t117 = a[65];
    const double t119 = t59*a[728];
    const double t120 = a[172];
    const double t122 = (t119+t120)*t59;
    const double t124 = (t117+t122)*t59;
    const double t126 = (t116+t124)*t59;
    const double t127 = a[16];
    const double t128 = a[134];
    const double t130 = t59*a[878];
    const double t131 = a[212];
    const double t133 = (t130+t131)*t59;
    const double t135 = (t128+t133)*t59;
    const double t136 = a[67];
    const double t138 = t59*a[726];
    const double t139 = a[379];
    const double t141 = (t138+t139)*t59;
    const double t142 = a[590];
    const double t143 = t101*t142;
    const double t144 = a[583];
    const double t145 = t59*t144;
    const double t146 = a[331];
    const double t148 = (t143+t145+t146)*t101;
    const double t150 = (t136+t141+t148)*t101;
    const double t152 = (t127+t135+t150)*t101;
    const double t153 = a[83];
    const double t155 = t59*a[580];
    const double t156 = a[182];
    const double t158 = (t155+t156)*t59;
    const double t160 = (t153+t158)*t59;
    const double t161 = a[95];
    const double t163 = t59*a[718];
    const double t164 = a[207];
    const double t166 = (t163+t164)*t59;
    const double t168 = t101*a[494];
    const double t169 = a[763];
    const double t170 = t59*t169;
    const double t171 = a[233];
    const double t173 = (t168+t170+t171)*t101;
    const double t175 = (t161+t166+t173)*t101;
    const double t177 = t59*a[584];
    const double t178 = a[340];
    const double t180 = (t177+t178)*t59;
    const double t182 = t101*a[453];
    const double t183 = a[416];
    const double t184 = t59*t183;
    const double t185 = a[147];
    const double t187 = (t182+t184+t185)*t101;
    const double t188 = a[555];
    const double t189 = t101*t188;
    const double t190 = a[725];
    const double t191 = t59*t190;
    const double t194 = x[5];
    const double t193 = (t189+t191)*t194;
    const double t195 = (t180+t187+t193)*t194;
    const double t197 = (t160+t175+t195)*t194;
    const double t199 = (t126+t152+t197)*t194;
    const double t200 = a[5];
    const double t201 = a[42];
    const double t202 = a[112];
    const double t204 = t59*a[716];
    const double t205 = a[163];
    const double t207 = (t204+t205)*t59;
    const double t209 = (t202+t207)*t59;
    const double t211 = (t201+t209)*t59;
    const double t212 = a[35];
    const double t213 = a[128];
    const double t215 = t59*a[752];
    const double t216 = a[165];
    const double t218 = (t215+t216)*t59;
    const double t220 = (t213+t218)*t59;
    const double t221 = a[107];
    const double t223 = t59*a[553];
    const double t224 = a[152];
    const double t226 = (t223+t224)*t59;
    const double t227 = a[454];
    const double t228 = t101*t227;
    const double t229 = a[837];
    const double t230 = t59*t229;
    const double t231 = a[258];
    const double t233 = (t228+t230+t231)*t101;
    const double t235 = (t221+t226+t233)*t101;
    const double t237 = (t212+t220+t235)*t101;
    const double t238 = a[133];
    const double t240 = t59*a[462];
    const double t241 = a[157];
    const double t243 = (t240+t241)*t59;
    const double t245 = (t238+t243)*t59;
    const double t246 = a[59];
    const double t248 = t59*a[489];
    const double t249 = a[312];
    const double t251 = (t248+t249)*t59;
    const double t252 = a[776];
    const double t253 = t101*t252;
    const double t254 = a[766];
    const double t255 = t59*t254;
    const double t256 = a[339];
    const double t258 = (t253+t255+t256)*t101;
    const double t260 = (t246+t251+t258)*t101;
    const double t262 = t59*a[417];
    const double t263 = a[270];
    const double t265 = (t262+t263)*t59;
    const double t267 = t101*a[693];
    const double t268 = a[595];
    const double t269 = t59*t268;
    const double t270 = a[255];
    const double t272 = (t267+t269+t270)*t101;
    const double t273 = a[760];
    const double t274 = t101*t273;
    const double t275 = a[851];
    const double t276 = t59*t275;
    const double t278 = (t274+t276)*t194;
    const double t280 = (t265+t272+t278)*t194;
    const double t282 = (t245+t260+t280)*t194;
    const double t283 = a[43];
    const double t284 = a[89];
    const double t286 = t59*a[868];
    const double t287 = a[285];
    const double t289 = (t286+t287)*t59;
    const double t291 = (t284+t289)*t59;
    const double t292 = a[126];
    const double t294 = t59*a[811];
    const double t295 = a[329];
    const double t297 = (t294+t295)*t59;
    const double t298 = a[613];
    const double t299 = t101*t298;
    const double t300 = a[735];
    const double t301 = t59*t300;
    const double t302 = a[293];
    const double t304 = (t299+t301+t302)*t101;
    const double t306 = (t292+t297+t304)*t101;
    const double t308 = t59*a[654];
    const double t309 = a[227];
    const double t311 = (t308+t309)*t59;
    const double t312 = a[567];
    const double t313 = t101*t312;
    const double t314 = a[526];
    const double t315 = t59*t314;
    const double t316 = a[249];
    const double t318 = (t313+t315+t316)*t101;
    const double t319 = a[753];
    const double t320 = t101*t319;
    const double t321 = a[512];
    const double t322 = t59*t321;
    const double t324 = (t320+t322)*t194;
    const double t326 = (t311+t318+t324)*t194;
    const double t327 = a[137];
    const double t329 = t59*a[831];
    const double t330 = a[196];
    const double t332 = (t329+t330)*t59;
    const double t333 = a[832];
    const double t334 = t101*t333;
    const double t335 = a[696];
    const double t336 = t59*t335;
    const double t337 = a[216];
    const double t339 = (t334+t336+t337)*t101;
    const double t340 = a[642];
    const double t341 = t101*t340;
    const double t342 = a[392];
    const double t343 = t59*t342;
    const double t344 = t341+t343;
    const double t345 = t344*t194;
    const double t346 = a[708];
    const double t347 = t8*t346;
    const double t348 = a[765];
    const double t349 = t101*t348;
    const double t350 = a[558];
    const double t351 = t59*t350;
    const double t352 = a[357];
    const double t354 = (t347+t349+t351+t352)*t8;
    const double t356 = (t327+t332+t339+t345+t354)*t8;
    const double t358 = (t283+t291+t306+t326+t356)*t8;
    const double t360 = (t200+t211+t237+t282+t358)*t8;
    const double t361 = a[21];
    const double t362 = a[79];
    const double t364 = t59*a[507];
    const double t365 = a[185];
    const double t367 = (t364+t365)*t59;
    const double t369 = (t362+t367)*t59;
    const double t370 = a[96];
    const double t372 = t59*a[443];
    const double t373 = a[320];
    const double t375 = (t372+t373)*t59;
    const double t376 = a[497];
    const double t377 = t101*t376;
    const double t378 = a[745];
    const double t379 = t59*t378;
    const double t380 = a[186];
    const double t382 = (t377+t379+t380)*t101;
    const double t384 = (t370+t375+t382)*t101;
    const double t386 = t59*a[624];
    const double t387 = a[307];
    const double t389 = (t386+t387)*t59;
    const double t390 = a[614];
    const double t391 = t101*t390;
    const double t392 = a[427];
    const double t393 = t59*t392;
    const double t394 = a[297];
    const double t396 = (t391+t393+t394)*t101;
    const double t397 = a[668];
    const double t398 = t101*t397;
    const double t399 = a[476];
    const double t400 = t59*t399;
    const double t402 = (t398+t400)*t194;
    const double t404 = (t389+t396+t402)*t194;
    const double t405 = a[130];
    const double t407 = t59*a[575];
    const double t408 = a[277];
    const double t410 = (t407+t408)*t59;
    const double t411 = a[744];
    const double t412 = t101*t411;
    const double t413 = a[706];
    const double t414 = t59*t413;
    const double t415 = a[279];
    const double t417 = (t412+t414+t415)*t101;
    const double t418 = a[795];
    const double t419 = t101*t418;
    const double t420 = a[424];
    const double t421 = t59*t420;
    const double t422 = t419+t421;
    const double t423 = t422*t194;
    const double t424 = a[609];
    const double t425 = t8*t424;
    const double t426 = a[749];
    const double t427 = t101*t426;
    const double t428 = a[561];
    const double t429 = t59*t428;
    const double t430 = a[251];
    const double t432 = (t425+t427+t429+t430)*t8;
    const double t434 = (t405+t410+t417+t423+t432)*t8;
    const double t436 = (t361+t369+t384+t404+t434)*t8;
    const double t438 = t8*a[429];
    const double t439 = a[786];
    const double t440 = t101*t439;
    const double t441 = a[649];
    const double t442 = t59*t441;
    const double t443 = a[238];
    const double t445 = (t438+t440+t442+t443)*t8;
    const double t447 = (t405+t410+t417+t423+t445)*t8;
    const double t449 = (t438+t427+t429+t430)*t8;
    const double t450 = t48*t346;
    const double t452 = (t450+t425+t349+t351+t352)*t48;
    const double t454 = (t327+t332+t339+t345+t449+t452)*t48;
    const double t456 = (t283+t291+t306+t326+t447+t454)*t48;
    const double t458 = (t200+t211+t237+t282+t436+t456)*t48;
    const double t459 = a[8];
    const double t460 = a[19];
    const double t461 = a[138];
    const double t463 = t59*a[840];
    const double t464 = a[156];
    const double t466 = (t463+t464)*t59;
    const double t468 = (t461+t466)*t59;
    const double t470 = (t460+t468)*t59;
    const double t471 = a[37];
    const double t472 = a[119];
    const double t474 = t59*a[781];
    const double t475 = a[374];
    const double t477 = (t474+t475)*t59;
    const double t479 = (t472+t477)*t59;
    const double t480 = a[121];
    const double t482 = t59*a[773];
    const double t483 = a[241];
    const double t485 = (t482+t483)*t59;
    const double t486 = a[737];
    const double t487 = t101*t486;
    const double t488 = a[483];
    const double t489 = t59*t488;
    const double t490 = a[375];
    const double t492 = (t487+t489+t490)*t101;
    const double t494 = (t480+t485+t492)*t101;
    const double t496 = (t471+t479+t494)*t101;
    const double t497 = a[110];
    const double t499 = t59*a[858];
    const double t500 = a[276];
    const double t502 = (t499+t500)*t59;
    const double t504 = (t497+t502)*t59;
    const double t505 = a[76];
    const double t507 = t59*a[751];
    const double t508 = a[364];
    const double t510 = (t507+t508)*t59;
    const double t511 = a[559];
    const double t512 = t101*t511;
    const double t513 = a[623];
    const double t514 = t59*t513;
    const double t515 = a[300];
    const double t517 = (t512+t514+t515)*t101;
    const double t519 = (t505+t510+t517)*t101;
    const double t521 = t59*a[554];
    const double t522 = a[160];
    const double t524 = (t521+t522)*t59;
    const double t526 = t101*a[533];
    const double t527 = a[625];
    const double t528 = t59*t527;
    const double t529 = a[291];
    const double t531 = (t526+t528+t529)*t101;
    const double t532 = a[394];
    const double t533 = t101*t532;
    const double t534 = a[638];
    const double t535 = t59*t534;
    const double t537 = (t533+t535)*t194;
    const double t539 = (t524+t531+t537)*t194;
    const double t541 = (t504+t519+t539)*t194;
    const double t542 = a[44];
    const double t543 = a[63];
    const double t545 = t59*a[465];
    const double t546 = a[230];
    const double t548 = (t545+t546)*t59;
    const double t550 = (t543+t548)*t59;
    const double t551 = a[93];
    const double t553 = t59*a[541];
    const double t554 = a[222];
    const double t556 = (t553+t554)*t59;
    const double t557 = a[872];
    const double t558 = t101*t557;
    const double t559 = a[525];
    const double t560 = t59*t559;
    const double t561 = a[153];
    const double t563 = (t558+t560+t561)*t101;
    const double t565 = (t551+t556+t563)*t101;
    const double t567 = t59*a[783];
    const double t568 = a[267];
    const double t570 = (t567+t568)*t59;
    const double t571 = a[731];
    const double t572 = t101*t571;
    const double t573 = a[548];
    const double t574 = t59*t573;
    const double t575 = a[318];
    const double t577 = (t572+t574+t575)*t101;
    const double t578 = a[505];
    const double t579 = t101*t578;
    const double t580 = a[422];
    const double t581 = t59*t580;
    const double t583 = (t579+t581)*t194;
    const double t585 = (t570+t577+t583)*t194;
    const double t586 = a[125];
    const double t588 = t59*a[450];
    const double t589 = a[373];
    const double t591 = (t588+t589)*t59;
    const double t592 = a[810];
    const double t593 = t101*t592;
    const double t594 = a[588];
    const double t595 = t59*t594;
    const double t596 = a[326];
    const double t598 = (t593+t595+t596)*t101;
    const double t599 = a[519];
    const double t600 = t101*t599;
    const double t601 = a[471];
    const double t602 = t59*t601;
    const double t603 = t600+t602;
    const double t604 = t603*t194;
    const double t605 = a[806];
    const double t606 = t8*t605;
    const double t607 = a[388];
    const double t608 = t101*t607;
    const double t609 = a[612];
    const double t610 = t59*t609;
    const double t611 = a[313];
    const double t613 = (t606+t608+t610+t611)*t8;
    const double t615 = (t586+t591+t598+t604+t613)*t8;
    const double t617 = (t542+t550+t565+t585+t615)*t8;
    const double t618 = a[77];
    const double t620 = t59*a[576];
    const double t621 = a[346];
    const double t623 = (t620+t621)*t59;
    const double t624 = a[720];
    const double t625 = t101*t624;
    const double t626 = a[436];
    const double t627 = t59*t626;
    const double t628 = a[159];
    const double t630 = (t625+t627+t628)*t101;
    const double t631 = a[843];
    const double t632 = t101*t631;
    const double t633 = a[782];
    const double t634 = t59*t633;
    const double t635 = t632+t634;
    const double t636 = t635*t194;
    const double t637 = a[664];
    const double t638 = t8*t637;
    const double t639 = a[774];
    const double t640 = t101*t639;
    const double t641 = a[800];
    const double t642 = t59*t641;
    const double t643 = a[148];
    const double t645 = (t638+t640+t642+t643)*t8;
    const double t647 = (t618+t623+t630+t636+t645)*t8;
    const double t649 = t8*a[739];
    const double t651 = (t649+t640+t642+t643)*t8;
    const double t652 = t48*t605;
    const double t654 = (t652+t638+t608+t610+t611)*t48;
    const double t656 = (t586+t591+t598+t604+t651+t654)*t48;
    const double t658 = (t542+t550+t565+t585+t647+t656)*t48;
    const double t659 = a[22];
    const double t660 = a[129];
    const double t662 = t59*a[648];
    const double t663 = a[348];
    const double t665 = (t662+t663)*t59;
    const double t667 = (t660+t665)*t59;
    const double t668 = a[105];
    const double t670 = t59*a[655];
    const double t671 = a[166];
    const double t673 = (t670+t671)*t59;
    const double t674 = a[472];
    const double t675 = t101*t674;
    const double t676 = a[501];
    const double t677 = t59*t676;
    const double t678 = a[206];
    const double t680 = (t675+t677+t678)*t101;
    const double t682 = (t668+t673+t680)*t101;
    const double t684 = t59*a[711];
    const double t685 = a[334];
    const double t687 = (t684+t685)*t59;
    const double t688 = a[408];
    const double t689 = t101*t688;
    const double t690 = a[403];
    const double t691 = t59*t690;
    const double t692 = a[332];
    const double t694 = (t689+t691+t692)*t101;
    const double t695 = a[802];
    const double t696 = t101*t695;
    const double t697 = a[538];
    const double t698 = t59*t697;
    const double t700 = (t696+t698)*t194;
    const double t702 = (t687+t694+t700)*t194;
    const double t703 = a[72];
    const double t705 = t59*a[437];
    const double t706 = a[359];
    const double t708 = (t705+t706)*t59;
    const double t709 = a[557];
    const double t710 = t101*t709;
    const double t711 = a[605];
    const double t712 = t59*t711;
    const double t713 = a[264];
    const double t715 = (t710+t712+t713)*t101;
    const double t716 = a[440];
    const double t717 = t101*t716;
    const double t718 = a[895];
    const double t719 = t59*t718;
    const double t720 = t717+t719;
    const double t721 = t720*t194;
    const double t722 = a[448];
    const double t723 = t8*t722;
    const double t724 = a[874];
    const double t725 = t101*t724;
    const double t726 = a[856];
    const double t727 = t59*t726;
    const double t728 = a[316];
    const double t730 = (t723+t725+t727+t728)*t8;
    const double t732 = (t703+t708+t715+t721+t730)*t8;
    const double t733 = a[793];
    const double t734 = t8*t733;
    const double t735 = a[456];
    const double t736 = t101*t735;
    const double t737 = a[524];
    const double t738 = t59*t737;
    const double t739 = a[376];
    const double t741 = (t734+t736+t738+t739)*t8;
    const double t742 = t48*t722;
    const double t744 = (t742+t734+t725+t727+t728)*t48;
    const double t746 = (t703+t708+t715+t721+t741+t744)*t48;
    const double t747 = a[123];
    const double t749 = t59*a[757];
    const double t750 = a[354];
    const double t752 = (t749+t750)*t59;
    const double t753 = a[866];
    const double t754 = t101*t753;
    const double t755 = a[695];
    const double t756 = t59*t755;
    const double t757 = a[236];
    const double t759 = (t754+t756+t757)*t101;
    const double t760 = a[707];
    const double t761 = t101*t760;
    const double t762 = a[663];
    const double t763 = t59*t762;
    const double t764 = t761+t763;
    const double t765 = t764*t194;
    const double t766 = a[517];
    const double t767 = t8*t766;
    const double t768 = a[864];
    const double t769 = t101*t768;
    const double t770 = a[730];
    const double t771 = t59*t770;
    const double t772 = a[202];
    const double t774 = (t767+t769+t771+t772)*t8;
    const double t775 = t48*t766;
    const double t776 = a[570];
    const double t777 = t8*t776;
    const double t779 = (t775+t777+t769+t771+t772)*t48;
    const double t780 = a[585];
    const double t792 = x[2];
    const double t781 = t792*t780;
    const double t782 = a[821];
    const double t783 = t48*t782;
    const double t784 = t8*t782;
    const double t785 = a[615];
    const double t786 = t101*t785;
    const double t787 = a[446];
    const double t788 = t59*t787;
    const double t789 = a[174];
    const double t791 = (t781+t783+t784+t786+t788+t789)*t792;
    const double t793 = (t747+t752+t759+t765+t774+t779+t791)*t792;
    const double t795 = (t659+t667+t682+t702+t732+t746+t793)*t792;
    const double t797 = (t459+t470+t496+t541+t617+t658+t795)*t792;
    const double t800 = t191*t101;
    const double t802 = (t180+t800)*t101;
    const double t804 = (t160+t802)*t101;
    const double t806 = (t126+t804)*t101;
    const double t808 = (t189+t184+t185)*t101;
    const double t810 = (t161+t166+t808)*t101;
    const double t812 = (t127+t135+t810)*t101;
    const double t814 = (t182+t170+t171)*t101;
    const double t816 = (t136+t141+t814)*t101;
    const double t818 = (t168+t145+t146)*t101;
    const double t819 = t194*t103;
    const double t821 = (t819+t143+t106+t107)*t194;
    const double t823 = (t97+t102+t818+t821)*t194;
    const double t825 = (t88+t96+t816+t823)*t194;
    const double t827 = (t76+t87+t812+t825)*t194;
    const double t828 = t276*t101;
    const double t830 = (t265+t828)*t101;
    const double t832 = (t245+t830)*t101;
    const double t834 = (t274+t269+t270)*t101;
    const double t836 = (t246+t251+t834)*t101;
    const double t838 = (t267+t255+t256)*t101;
    const double t839 = t194*t227;
    const double t841 = (t839+t253+t230+t231)*t194;
    const double t843 = (t221+t226+t838+t841)*t194;
    const double t845 = (t212+t220+t836+t843)*t194;
    const double t846 = t322*t101;
    const double t848 = (t311+t846)*t101;
    const double t850 = (t320+t315+t316)*t101;
    const double t851 = t194*t298;
    const double t853 = (t851+t313+t301+t302)*t194;
    const double t855 = (t292+t297+t850+t853)*t194;
    const double t856 = t342*t101;
    const double t857 = t856*t59;
    const double t858 = t194*t333;
    const double t860 = (t858+t341+t336+t337)*t194;
    const double t861 = t194*t348;
    const double t863 = (t347+t861+t351+t352)*t8;
    const double t865 = (t327+t332+t857+t860+t863)*t8;
    const double t867 = (t283+t291+t848+t855+t865)*t8;
    const double t869 = (t200+t211+t832+t845+t867)*t8;
    const double t870 = t400*t101;
    const double t872 = (t389+t870)*t101;
    const double t874 = (t398+t393+t394)*t101;
    const double t875 = t194*t376;
    const double t877 = (t875+t391+t379+t380)*t194;
    const double t879 = (t370+t375+t874+t877)*t194;
    const double t880 = t420*t101;
    const double t881 = t880*t59;
    const double t882 = t194*t411;
    const double t884 = (t882+t419+t414+t415)*t194;
    const double t885 = t194*t426;
    const double t887 = (t425+t885+t429+t430)*t8;
    const double t889 = (t405+t410+t881+t884+t887)*t8;
    const double t891 = (t361+t369+t872+t879+t889)*t8;
    const double t892 = t194*t439;
    const double t894 = (t438+t892+t442+t443)*t8;
    const double t896 = (t405+t410+t881+t884+t894)*t8;
    const double t898 = (t438+t885+t429+t430)*t8;
    const double t900 = (t450+t425+t861+t351+t352)*t48;
    const double t902 = (t327+t332+t857+t860+t898+t900)*t48;
    const double t904 = (t283+t291+t848+t855+t896+t902)*t48;
    const double t906 = (t200+t211+t832+t845+t891+t904)*t48;
    const double t907 = a[6];
    const double t908 = a[39];
    const double t909 = a[116];
    const double t911 = t59*a[396];
    const double t912 = a[281];
    const double t914 = (t911+t912)*t59;
    const double t916 = (t909+t914)*t59;
    const double t918 = (t908+t916)*t59;
    const double t919 = a[48];
    const double t920 = a[53];
    const double t922 = t59*a[606];
    const double t923 = a[240];
    const double t925 = (t922+t923)*t59;
    const double t927 = (t920+t925)*t59;
    const double t928 = a[132];
    const double t930 = t59*a[515];
    const double t931 = a[304];
    const double t933 = (t930+t931)*t59;
    const double t934 = a[389];
    const double t935 = t101*t934;
    const double t936 = a[511];
    const double t937 = t59*t936;
    const double t938 = a[221];
    const double t940 = (t935+t937+t938)*t101;
    const double t942 = (t928+t933+t940)*t101;
    const double t944 = (t919+t927+t942)*t101;
    const double t945 = a[145];
    const double t947 = t59*a[818];
    const double t948 = a[260];
    const double t950 = (t947+t948)*t59;
    const double t951 = a[826];
    const double t952 = t101*t951;
    const double t953 = a[477];
    const double t954 = t59*t953;
    const double t955 = a[272];
    const double t957 = (t952+t954+t955)*t101;
    const double t959 = (t945+t950+t957)*t101;
    const double t961 = t101*a[841];
    const double t963 = (t961+t954+t955)*t101;
    const double t964 = t194*t934;
    const double t966 = (t964+t952+t937+t938)*t194;
    const double t968 = (t928+t933+t963+t966)*t194;
    const double t970 = (t919+t927+t959+t968)*t194;
    const double t971 = a[32];
    const double t972 = a[62];
    const double t974 = t59*a[402];
    const double t975 = a[311];
    const double t977 = (t974+t975)*t59;
    const double t979 = (t972+t977)*t59;
    const double t980 = a[146];
    const double t982 = t59*a[882];
    const double t983 = a[167];
    const double t985 = (t982+t983)*t59;
    const double t986 = a[627];
    const double t987 = t101*t986;
    const double t988 = a[637];
    const double t989 = t59*t988;
    const double t990 = a[319];
    const double t992 = (t987+t989+t990)*t101;
    const double t994 = (t980+t985+t992)*t101;
    const double t995 = a[884];
    const double t996 = t101*t995;
    const double t997 = a[833];
    const double t998 = t59*t997;
    const double t999 = a[266];
    const double t1001 = (t996+t998+t999)*t101;
    const double t1002 = t194*t986;
    const double t1004 = (t1002+t996+t989+t990)*t194;
    const double t1006 = (t980+t985+t1001+t1004)*t194;
    const double t1007 = a[92];
    const double t1009 = t59*a[867];
    const double t1010 = a[363];
    const double t1012 = (t1009+t1010)*t59;
    const double t1013 = a[412];
    const double t1014 = t101*t1013;
    const double t1015 = a[537];
    const double t1016 = t59*t1015;
    const double t1017 = a[169];
    const double t1019 = (t1014+t1016+t1017)*t101;
    const double t1020 = t194*t1013;
    const double t1021 = a[686];
    const double t1022 = t101*t1021;
    const double t1024 = (t1020+t1022+t1016+t1017)*t194;
    const double t1025 = a[503];
    const double t1026 = t8*t1025;
    const double t1027 = a[528];
    const double t1028 = t194*t1027;
    const double t1029 = t101*t1027;
    const double t1030 = a[563];
    const double t1031 = t59*t1030;
    const double t1032 = a[265];
    const double t1034 = (t1026+t1028+t1029+t1031+t1032)*t8;
    const double t1036 = (t1007+t1012+t1019+t1024+t1034)*t8;
    const double t1038 = (t971+t979+t994+t1006+t1036)*t8;
    const double t1039 = a[124];
    const double t1041 = t59*a[644];
    const double t1042 = a[330];
    const double t1044 = (t1041+t1042)*t59;
    const double t1045 = a[530];
    const double t1046 = t101*t1045;
    const double t1047 = a[628];
    const double t1048 = t59*t1047;
    const double t1049 = a[325];
    const double t1051 = (t1046+t1048+t1049)*t101;
    const double t1052 = t194*t1045;
    const double t1053 = a[510];
    const double t1054 = t101*t1053;
    const double t1056 = (t1052+t1054+t1048+t1049)*t194;
    const double t1057 = a[617];
    const double t1058 = t8*t1057;
    const double t1059 = a[604];
    const double t1060 = t194*t1059;
    const double t1061 = t101*t1059;
    const double t1062 = a[873];
    const double t1063 = t59*t1062;
    const double t1064 = a[358];
    const double t1066 = (t1058+t1060+t1061+t1063+t1064)*t8;
    const double t1068 = (t1039+t1044+t1051+t1056+t1066)*t8;
    const double t1070 = t8*a[514];
    const double t1072 = (t1070+t1060+t1061+t1063+t1064)*t8;
    const double t1073 = t48*t1025;
    const double t1075 = (t1073+t1058+t1028+t1029+t1031+t1032)*t48;
    const double t1077 = (t1007+t1012+t1019+t1024+t1072+t1075)*t48;
    const double t1079 = (t971+t979+t994+t1006+t1068+t1077)*t48;
    const double t1080 = a[17];
    const double t1081 = a[82];
    const double t1083 = t59*a[478];
    const double t1084 = a[263];
    const double t1086 = (t1083+t1084)*t59;
    const double t1088 = (t1081+t1086)*t59;
    const double t1089 = a[64];
    const double t1091 = t59*a[721];
    const double t1092 = a[323];
    const double t1094 = (t1091+t1092)*t59;
    const double t1095 = a[713];
    const double t1096 = t101*t1095;
    const double t1097 = a[540];
    const double t1098 = t59*t1097;
    const double t1099 = a[351];
    const double t1101 = (t1096+t1098+t1099)*t101;
    const double t1103 = (t1089+t1094+t1101)*t101;
    const double t1104 = a[113];
    const double t1106 = t59*a[791];
    const double t1107 = a[254];
    const double t1109 = (t1106+t1107)*t59;
    const double t1110 = a[748];
    const double t1111 = t101*t1110;
    const double t1112 = a[447];
    const double t1113 = t59*t1112;
    const double t1114 = a[188];
    const double t1116 = (t1111+t1113+t1114)*t101;
    const double t1117 = a[887];
    const double t1118 = t194*t1117;
    const double t1119 = a[643];
    const double t1120 = t101*t1119;
    const double t1121 = a[890];
    const double t1122 = t59*t1121;
    const double t1123 = a[315];
    const double t1125 = (t1118+t1120+t1122+t1123)*t194;
    const double t1127 = (t1104+t1109+t1116+t1125)*t194;
    const double t1128 = a[140];
    const double t1130 = t59*a[420];
    const double t1131 = a[347];
    const double t1133 = (t1130+t1131)*t59;
    const double t1134 = a[838];
    const double t1135 = t101*t1134;
    const double t1136 = a[474];
    const double t1137 = t59*t1136;
    const double t1138 = a[287];
    const double t1140 = (t1135+t1137+t1138)*t101;
    const double t1141 = a[709];
    const double t1142 = t194*t1141;
    const double t1143 = a[661];
    const double t1144 = t101*t1143;
    const double t1145 = a[656];
    const double t1146 = t59*t1145;
    const double t1147 = a[150];
    const double t1149 = (t1142+t1144+t1146+t1147)*t194;
    const double t1150 = a[828];
    const double t1151 = t8*t1150;
    const double t1152 = a[545];
    const double t1153 = t194*t1152;
    const double t1154 = a[592];
    const double t1155 = t101*t1154;
    const double t1156 = a[502];
    const double t1157 = t59*t1156;
    const double t1158 = a[294];
    const double t1160 = (t1151+t1153+t1155+t1157+t1158)*t8;
    const double t1162 = (t1128+t1133+t1140+t1149+t1160)*t8;
    const double t1163 = a[897];
    const double t1164 = t8*t1163;
    const double t1165 = a[798];
    const double t1166 = t194*t1165;
    const double t1167 = a[819];
    const double t1168 = t101*t1167;
    const double t1169 = a[484];
    const double t1170 = t59*t1169;
    const double t1171 = a[256];
    const double t1173 = (t1164+t1166+t1168+t1170+t1171)*t8;
    const double t1174 = t48*t1150;
    const double t1176 = (t1174+t1164+t1153+t1155+t1157+t1158)*t48;
    const double t1178 = (t1128+t1133+t1140+t1149+t1173+t1176)*t48;
    const double t1179 = a[52];
    const double t1181 = t59*a[697];
    const double t1182 = a[239];
    const double t1184 = (t1181+t1182)*t59;
    const double t1185 = a[799];
    const double t1186 = t101*t1185;
    const double t1187 = a[673];
    const double t1188 = t59*t1187;
    const double t1189 = a[380];
    const double t1191 = (t1186+t1188+t1189)*t101;
    const double t1192 = a[387];
    const double t1193 = t194*t1192;
    const double t1194 = a[594];
    const double t1195 = t101*t1194;
    const double t1196 = a[902];
    const double t1197 = t59*t1196;
    const double t1198 = a[353];
    const double t1200 = (t1193+t1195+t1197+t1198)*t194;
    const double t1201 = a[409];
    const double t1202 = t8*t1201;
    const double t1203 = a[808];
    const double t1204 = t194*t1203;
    const double t1205 = a[634];
    const double t1206 = t101*t1205;
    const double t1207 = a[904];
    const double t1208 = t59*t1207;
    const double t1209 = a[278];
    const double t1211 = (t1202+t1204+t1206+t1208+t1209)*t8;
    const double t1212 = t48*t1201;
    const double t1213 = a[458];
    const double t1214 = t8*t1213;
    const double t1216 = (t1212+t1214+t1204+t1206+t1208+t1209)*t48;
    const double t1217 = a[796];
    const double t1218 = t792*t1217;
    const double t1219 = a[466];
    const double t1220 = t48*t1219;
    const double t1221 = t8*t1219;
    const double t1222 = a[539];
    const double t1223 = t194*t1222;
    const double t1224 = a[572];
    const double t1225 = t101*t1224;
    const double t1226 = a[754];
    const double t1227 = t59*t1226;
    const double t1228 = a[372];
    const double t1230 = (t1218+t1220+t1221+t1223+t1225+t1227+t1228)*t792;
    const double t1232 = (t1179+t1184+t1191+t1200+t1211+t1216+t1230)*t792;
    const double t1234 = (t1080+t1088+t1103+t1127+t1162+t1178+t1232)*t792;
    const double t1236 = (t907+t918+t944+t970+t1038+t1079+t1234)*t792;
    const double t1237 = t535*t101;
    const double t1239 = (t524+t1237)*t101;
    const double t1241 = (t504+t1239)*t101;
    const double t1243 = (t533+t528+t529)*t101;
    const double t1245 = (t505+t510+t1243)*t101;
    const double t1247 = (t526+t514+t515)*t101;
    const double t1248 = t194*t486;
    const double t1250 = (t1248+t512+t489+t490)*t194;
    const double t1252 = (t480+t485+t1247+t1250)*t194;
    const double t1254 = (t471+t479+t1245+t1252)*t194;
    const double t1255 = t581*t101;
    const double t1257 = (t570+t1255)*t101;
    const double t1259 = (t579+t574+t575)*t101;
    const double t1260 = t194*t557;
    const double t1262 = (t1260+t572+t560+t561)*t194;
    const double t1264 = (t551+t556+t1259+t1262)*t194;
    const double t1265 = t602*t101;
    const double t1266 = t194*t592;
    const double t1268 = (t1266+t600+t595+t596)*t194;
    const double t1269 = t194*t607;
    const double t1271 = (t606+t1269+t610+t611)*t8;
    const double t1273 = (t586+t591+t1265+t1268+t1271)*t8;
    const double t1275 = (t542+t550+t1257+t1264+t1273)*t8;
    const double t1276 = t633*t101;
    const double t1277 = t1276*t59;
    const double t1278 = t194*t624;
    const double t1280 = (t1278+t632+t627+t628)*t194;
    const double t1281 = t194*t639;
    const double t1283 = (t638+t1281+t642+t643)*t8;
    const double t1285 = (t618+t623+t1277+t1280+t1283)*t8;
    const double t1287 = (t649+t1281+t642+t643)*t8;
    const double t1289 = (t652+t638+t1269+t610+t611)*t48;
    const double t1291 = (t586+t591+t1265+t1268+t1287+t1289)*t48;
    const double t1293 = (t542+t550+t1257+t1264+t1285+t1291)*t48;
    const double t1294 = t101*t1117;
    const double t1296 = (t1294+t1122+t1123)*t101;
    const double t1298 = (t1104+t1109+t1296)*t101;
    const double t1300 = (t1120+t1113+t1114)*t101;
    const double t1301 = t194*t1095;
    const double t1303 = (t1301+t1111+t1098+t1099)*t194;
    const double t1305 = (t1089+t1094+t1300+t1303)*t194;
    const double t1306 = t101*t1141;
    const double t1308 = (t1306+t1146+t1147)*t101;
    const double t1309 = t194*t1134;
    const double t1311 = (t1309+t1144+t1137+t1138)*t194;
    const double t1312 = t194*t1154;
    const double t1313 = t101*t1152;
    const double t1315 = (t1151+t1312+t1313+t1157+t1158)*t8;
    const double t1317 = (t1128+t1133+t1308+t1311+t1315)*t8;
    const double t1318 = t194*t1167;
    const double t1319 = t101*t1165;
    const double t1321 = (t1164+t1318+t1319+t1170+t1171)*t8;
    const double t1323 = (t1174+t1164+t1312+t1313+t1157+t1158)*t48;
    const double t1325 = (t1128+t1133+t1308+t1311+t1321+t1323)*t48;
    const double t1326 = a[135];
    const double t1328 = t59*a[736];
    const double t1329 = a[250];
    const double t1331 = (t1328+t1329)*t59;
    const double t1332 = a[761];
    const double t1333 = t101*t1332;
    const double t1334 = a[676];
    const double t1335 = t59*t1334;
    const double t1336 = a[164];
    const double t1338 = (t1333+t1335+t1336)*t101;
    const double t1339 = t194*t1332;
    const double t1340 = a[764];
    const double t1341 = t101*t1340;
    const double t1343 = (t1339+t1341+t1335+t1336)*t194;
    const double t1344 = a[683];
    const double t1345 = t8*t1344;
    const double t1346 = a[516];
    const double t1347 = t194*t1346;
    const double t1348 = t101*t1346;
    const double t1349 = a[741];
    const double t1350 = t59*t1349;
    const double t1351 = a[195];
    const double t1353 = (t1345+t1347+t1348+t1350+t1351)*t8;
    const double t1354 = t48*t1344;
    const double t1355 = a[419];
    const double t1356 = t8*t1355;
    const double t1358 = (t1354+t1356+t1347+t1348+t1350+t1351)*t48;
    const double t1360 = t792*a[769];
    const double t1361 = a[593];
    const double t1362 = t48*t1361;
    const double t1363 = t8*t1361;
    const double t1364 = a[680];
    const double t1365 = t194*t1364;
    const double t1366 = a[652];
    const double t1367 = t101*t1366;
    const double t1368 = a[425];
    const double t1369 = t59*t1368;
    const double t1370 = a[253];
    const double t1372 = (t1360+t1362+t1363+t1365+t1367+t1369+t1370)*t792;
    const double t1374 = (t1326+t1331+t1338+t1343+t1353+t1358+t1372)*t792;
    const double t1376 = (t1080+t1088+t1298+t1305+t1317+t1325+t1374)*t792;
    const double t1377 = t698*t101;
    const double t1379 = (t687+t1377)*t101;
    const double t1381 = (t696+t691+t692)*t101;
    const double t1382 = t194*t674;
    const double t1384 = (t1382+t689+t677+t678)*t194;
    const double t1386 = (t668+t673+t1381+t1384)*t194;
    const double t1387 = t718*t101;
    const double t1388 = t1387*t59;
    const double t1389 = t194*t709;
    const double t1391 = (t1389+t717+t712+t713)*t194;
    const double t1392 = t194*t724;
    const double t1394 = (t723+t1392+t727+t728)*t8;
    const double t1396 = (t703+t708+t1388+t1391+t1394)*t8;
    const double t1397 = t194*t735;
    const double t1399 = (t734+t1397+t738+t739)*t8;
    const double t1401 = (t742+t734+t1392+t727+t728)*t48;
    const double t1403 = (t703+t708+t1388+t1391+t1399+t1401)*t48;
    const double t1404 = t101*t1192;
    const double t1406 = (t1404+t1197+t1198)*t101;
    const double t1407 = t194*t1185;
    const double t1409 = (t1407+t1195+t1188+t1189)*t194;
    const double t1410 = t194*t1205;
    const double t1411 = t101*t1203;
    const double t1413 = (t1202+t1410+t1411+t1208+t1209)*t8;
    const double t1415 = (t1212+t1214+t1410+t1411+t1208+t1209)*t48;
    const double t1417 = t792*a[619];
    const double t1418 = t194*t1366;
    const double t1419 = t101*t1364;
    const double t1421 = (t1417+t1362+t1363+t1418+t1419+t1369+t1370)*t792;
    const double t1423 = (t1179+t1184+t1406+t1409+t1413+t1415+t1421)*t792;
    const double t1424 = t762*t101;
    const double t1425 = t1424*t59;
    const double t1426 = t194*t753;
    const double t1428 = (t1426+t761+t756+t757)*t194;
    const double t1429 = t194*t768;
    const double t1431 = (t767+t1429+t771+t772)*t8;
    const double t1433 = (t775+t777+t1429+t771+t772)*t48;
    const double t1434 = t194*t1224;
    const double t1435 = t101*t1222;
    const double t1437 = (t1360+t1220+t1221+t1434+t1435+t1227+t1228)*t792;
    const double t1440 = x[1];
    const double t1438 = t1440*t780;
    const double t1439 = t194*t785;
    const double t1441 = (t1438+t1218+t783+t784+t1439+t788+t789)*t1440;
    const double t1443 = (t747+t752+t1425+t1428+t1431+t1433+t1437+t1441)*t1440;
    const double t1445 = (t659+t667+t1379+t1386+t1396+t1403+t1423+t1443)*t1440;
    const double t1447 = (t459+t470+t1241+t1254+t1275+t1293+t1376+t1445)*t1440;
    const double t1450 = a[0];
    const double t1451 = a[4];
    const double t1452 = a[45];
    const double t1453 = a[71];
    const double t1455 = t59*a[770];
    const double t1456 = a[292];
    const double t1458 = (t1455+t1456)*t59;
    const double t1460 = (t1453+t1458)*t59;
    const double t1462 = (t1452+t1460)*t59;
    const double t1463 = a[27];
    const double t1464 = a[108];
    const double t1466 = t59*a[857];
    const double t1467 = a[244];
    const double t1469 = (t1466+t1467)*t59;
    const double t1471 = (t1464+t1469)*t59;
    const double t1472 = a[141];
    const double t1474 = t59*a[839];
    const double t1475 = a[274];
    const double t1477 = (t1474+t1475)*t59;
    const double t1478 = a[441];
    const double t1479 = t101*t1478;
    const double t1480 = a[675];
    const double t1481 = t59*t1480;
    const double t1482 = a[344];
    const double t1484 = (t1479+t1481+t1482)*t101;
    const double t1486 = (t1472+t1477+t1484)*t101;
    const double t1488 = (t1463+t1471+t1486)*t101;
    const double t1490 = (t1451+t1462+t1488)*t101;
    const double t1491 = a[25];
    const double t1492 = a[60];
    const double t1494 = t59*a[560];
    const double t1495 = a[209];
    const double t1497 = (t1494+t1495)*t59;
    const double t1499 = (t1492+t1497)*t59;
    const double t1500 = a[114];
    const double t1502 = t59*a[688];
    const double t1503 = a[183];
    const double t1505 = (t1502+t1503)*t59;
    const double t1506 = a[807];
    const double t1507 = t101*t1506;
    const double t1508 = a[824];
    const double t1509 = t59*t1508;
    const double t1510 = a[299];
    const double t1512 = (t1507+t1509+t1510)*t101;
    const double t1514 = (t1500+t1505+t1512)*t101;
    const double t1516 = (t1491+t1499+t1514)*t101;
    const double t1518 = t101*a[546];
    const double t1519 = a[410];
    const double t1520 = t59*t1519;
    const double t1521 = a[229];
    const double t1523 = (t1518+t1520+t1521)*t101;
    const double t1525 = (t1500+t1505+t1523)*t101;
    const double t1527 = (t1518+t1509+t1510)*t101;
    const double t1528 = t194*t1478;
    const double t1530 = (t1528+t1507+t1481+t1482)*t194;
    const double t1532 = (t1472+t1477+t1527+t1530)*t194;
    const double t1534 = (t1463+t1471+t1525+t1532)*t194;
    const double t1536 = (t1451+t1462+t1516+t1534)*t194;
    const double t1537 = a[7];
    const double t1538 = a[23];
    const double t1539 = a[136];
    const double t1541 = t59*a[788];
    const double t1542 = a[321];
    const double t1544 = (t1541+t1542)*t59;
    const double t1546 = (t1539+t1544)*t59;
    const double t1547 = a[144];
    const double t1549 = t59*a[690];
    const double t1550 = a[203];
    const double t1552 = (t1549+t1550)*t59;
    const double t1553 = a[803];
    const double t1554 = t101*t1553;
    const double t1555 = a[727];
    const double t1556 = t59*t1555;
    const double t1557 = a[205];
    const double t1559 = (t1554+t1556+t1557)*t101;
    const double t1561 = (t1547+t1552+t1559)*t101;
    const double t1563 = (t1538+t1546+t1561)*t101;
    const double t1564 = a[55];
    const double t1566 = t59*a[645];
    const double t1567 = a[259];
    const double t1569 = (t1566+t1567)*t59;
    const double t1570 = a[581];
    const double t1571 = t101*t1570;
    const double t1572 = a[473];
    const double t1573 = t59*t1572;
    const double t1574 = a[345];
    const double t1576 = (t1571+t1573+t1574)*t101;
    const double t1578 = (t1564+t1569+t1576)*t101;
    const double t1580 = t101*a[401];
    const double t1582 = (t1580+t1573+t1574)*t101;
    const double t1583 = t194*t1553;
    const double t1585 = (t1583+t1571+t1556+t1557)*t194;
    const double t1587 = (t1547+t1552+t1582+t1585)*t194;
    const double t1589 = (t1538+t1546+t1578+t1587)*t194;
    const double t1590 = a[29];
    const double t1591 = a[106];
    const double t1593 = t59*a[682];
    const double t1594 = a[234];
    const double t1596 = (t1593+t1594)*t59;
    const double t1597 = a[747];
    const double t1598 = t101*t1597;
    const double t1599 = a[704];
    const double t1600 = t59*t1599;
    const double t1601 = a[360];
    const double t1603 = (t1598+t1600+t1601)*t101;
    const double t1605 = (t1591+t1596+t1603)*t101;
    const double t1606 = a[871];
    const double t1607 = t101*t1606;
    const double t1608 = a[439];
    const double t1609 = t59*t1608;
    const double t1610 = a[257];
    const double t1612 = (t1607+t1609+t1610)*t101;
    const double t1613 = t194*t1597;
    const double t1615 = (t1613+t1607+t1600+t1601)*t194;
    const double t1617 = (t1591+t1596+t1612+t1615)*t194;
    const double t1618 = a[99];
    const double t1619 = a[700];
    const double t1620 = t101*t1619;
    const double t1621 = a[900];
    const double t1622 = t59*t1621;
    const double t1623 = a[327];
    const double t1625 = (t1620+t1622+t1623)*t101;
    const double t1626 = t194*t1619;
    const double t1627 = a[804];
    const double t1628 = t101*t1627;
    const double t1630 = (t1626+t1628+t1622+t1623)*t194;
    const double t1631 = a[480];
    const double t1632 = t8*t1631;
    const double t1633 = a[825];
    const double t1634 = t194*t1633;
    const double t1635 = t101*t1633;
    const double t1636 = a[154];
    const double t1638 = (t1632+t1634+t1635+t1636)*t8;
    const double t1640 = (t1618+t1625+t1630+t1638)*t8;
    const double t1642 = (t1590+t1605+t1617+t1640)*t8;
    const double t1644 = (t1537+t1563+t1589+t1642)*t8;
    const double t1645 = a[34];
    const double t1646 = a[70];
    const double t1648 = t59*a[701];
    const double t1649 = a[336];
    const double t1651 = (t1648+t1649)*t59;
    const double t1652 = a[566];
    const double t1653 = t101*t1652;
    const double t1654 = a[500];
    const double t1655 = t59*t1654;
    const double t1656 = a[303];
    const double t1658 = (t1653+t1655+t1656)*t101;
    const double t1660 = (t1646+t1651+t1658)*t101;
    const double t1661 = a[853];
    const double t1662 = t101*t1661;
    const double t1663 = a[835];
    const double t1664 = t59*t1663;
    const double t1665 = a[243];
    const double t1667 = (t1662+t1664+t1665)*t101;
    const double t1668 = t194*t1652;
    const double t1670 = (t1668+t1662+t1655+t1656)*t194;
    const double t1672 = (t1646+t1651+t1667+t1670)*t194;
    const double t1673 = a[85];
    const double t1674 = a[658];
    const double t1675 = t101*t1674;
    const double t1676 = a[506];
    const double t1677 = t59*t1676;
    const double t1678 = a[308];
    const double t1680 = (t1675+t1677+t1678)*t101;
    const double t1681 = t194*t1674;
    const double t1682 = a[848];
    const double t1683 = t101*t1682;
    const double t1685 = (t1681+t1683+t1677+t1678)*t194;
    const double t1686 = a[552];
    const double t1687 = t8*t1686;
    const double t1688 = a[755];
    const double t1689 = t194*t1688;
    const double t1690 = t101*t1688;
    const double t1691 = a[309];
    const double t1693 = (t1687+t1689+t1690+t1691)*t8;
    const double t1695 = (t1673+t1680+t1685+t1693)*t8;
    const double t1697 = (t1645+t1660+t1672+t1695)*t8;
    const double t1699 = t8*a[633];
    const double t1700 = a[599];
    const double t1701 = t194*t1700;
    const double t1702 = t101*t1700;
    const double t1703 = a[193];
    const double t1705 = (t1699+t1701+t1702+t1703)*t8;
    const double t1707 = (t1673+t1680+t1685+t1705)*t8;
    const double t1709 = (t1699+t1689+t1690+t1691)*t8;
    const double t1710 = t48*t1631;
    const double t1712 = (t1710+t1687+t1634+t1635+t1636)*t48;
    const double t1714 = (t1618+t1625+t1630+t1709+t1712)*t48;
    const double t1716 = (t1590+t1605+t1617+t1707+t1714)*t48;
    const double t1718 = (t1537+t1563+t1589+t1697+t1716)*t48;
    const double t1719 = a[12];
    const double t1720 = a[33];
    const double t1721 = a[54];
    const double t1723 = t59*a[445];
    const double t1724 = a[268];
    const double t1726 = (t1723+t1724)*t59;
    const double t1728 = (t1721+t1726)*t59;
    const double t1730 = (t1720+t1728)*t59;
    const double t1731 = a[41];
    const double t1732 = a[111];
    const double t1734 = t59*a[413];
    const double t1735 = a[377];
    const double t1737 = (t1734+t1735)*t59;
    const double t1739 = (t1732+t1737)*t59;
    const double t1740 = a[74];
    const double t1742 = t59*a[678];
    const double t1743 = a[199];
    const double t1745 = (t1742+t1743)*t59;
    const double t1746 = a[646];
    const double t1747 = t101*t1746;
    const double t1748 = a[449];
    const double t1749 = t59*t1748;
    const double t1750 = a[175];
    const double t1752 = (t1747+t1749+t1750)*t101;
    const double t1754 = (t1740+t1745+t1752)*t101;
    const double t1756 = (t1731+t1739+t1754)*t101;
    const double t1757 = a[47];
    const double t1758 = a[115];
    const double t1760 = t59*a[406];
    const double t1761 = a[184];
    const double t1763 = (t1760+t1761)*t59;
    const double t1765 = (t1758+t1763)*t59;
    const double t1766 = a[103];
    const double t1768 = t59*a[812];
    const double t1769 = a[246];
    const double t1771 = (t1768+t1769)*t59;
    const double t1772 = a[444];
    const double t1773 = t101*t1772;
    const double t1774 = a[860];
    const double t1775 = t59*t1774;
    const double t1776 = a[181];
    const double t1778 = (t1773+t1775+t1776)*t101;
    const double t1780 = (t1766+t1771+t1778)*t101;
    const double t1781 = a[100];
    const double t1783 = t59*a[529];
    const double t1784 = a[306];
    const double t1786 = (t1783+t1784)*t59;
    const double t1788 = t101*a[635];
    const double t1789 = a[532];
    const double t1790 = t59*t1789;
    const double t1791 = a[180];
    const double t1793 = (t1788+t1790+t1791)*t101;
    const double t1794 = a[817];
    const double t1795 = t194*t1794;
    const double t1796 = a[780];
    const double t1797 = t101*t1796;
    const double t1798 = a[520];
    const double t1799 = t59*t1798;
    const double t1800 = a[208];
    const double t1802 = (t1795+t1797+t1799+t1800)*t194;
    const double t1804 = (t1781+t1786+t1793+t1802)*t194;
    const double t1806 = (t1757+t1765+t1780+t1804)*t194;
    const double t1807 = a[28];
    const double t1808 = a[56];
    const double t1810 = t59*a[569];
    const double t1811 = a[248];
    const double t1813 = (t1810+t1811)*t59;
    const double t1815 = (t1808+t1813)*t59;
    const double t1816 = a[75];
    const double t1818 = t59*a[586];
    const double t1819 = a[252];
    const double t1821 = (t1818+t1819)*t59;
    const double t1822 = a[875];
    const double t1823 = t101*t1822;
    const double t1824 = a[415];
    const double t1825 = t59*t1824;
    const double t1826 = a[367];
    const double t1828 = (t1823+t1825+t1826)*t101;
    const double t1830 = (t1816+t1821+t1828)*t101;
    const double t1831 = a[94];
    const double t1833 = t59*a[896];
    const double t1834 = a[314];
    const double t1836 = (t1833+t1834)*t59;
    const double t1837 = a[487];
    const double t1838 = t101*t1837;
    const double t1839 = a[535];
    const double t1840 = t59*t1839;
    const double t1841 = a[210];
    const double t1843 = (t1838+t1840+t1841)*t101;
    const double t1844 = a[677];
    const double t1845 = t194*t1844;
    const double t1846 = a[883];
    const double t1847 = t101*t1846;
    const double t1848 = a[852];
    const double t1849 = t59*t1848;
    const double t1850 = a[342];
    const double t1852 = (t1845+t1847+t1849+t1850)*t194;
    const double t1854 = (t1831+t1836+t1843+t1852)*t194;
    const double t1855 = a[73];
    const double t1857 = t59*a[880];
    const double t1858 = a[226];
    const double t1860 = (t1857+t1858)*t59;
    const double t1861 = a[679];
    const double t1862 = t101*t1861;
    const double t1863 = a[421];
    const double t1864 = t59*t1863;
    const double t1865 = a[333];
    const double t1867 = (t1862+t1864+t1865)*t101;
    const double t1868 = a[734];
    const double t1869 = t194*t1868;
    const double t1870 = a[805];
    const double t1871 = t101*t1870;
    const double t1872 = a[404];
    const double t1873 = t59*t1872;
    const double t1874 = a[155];
    const double t1876 = (t1869+t1871+t1873+t1874)*t194;
    const double t1877 = a[621];
    const double t1878 = t8*t1877;
    const double t1879 = a[498];
    const double t1880 = t194*t1879;
    const double t1881 = a[888];
    const double t1882 = t101*t1881;
    const double t1883 = a[639];
    const double t1884 = t59*t1883;
    const double t1885 = a[365];
    const double t1887 = (t1878+t1880+t1882+t1884+t1885)*t8;
    const double t1889 = (t1855+t1860+t1867+t1876+t1887)*t8;
    const double t1891 = (t1807+t1815+t1830+t1854+t1889)*t8;
    const double t1892 = a[109];
    const double t1894 = t59*a[777];
    const double t1895 = a[225];
    const double t1897 = (t1894+t1895)*t59;
    const double t1898 = a[423];
    const double t1899 = t101*t1898;
    const double t1900 = a[901];
    const double t1901 = t59*t1900;
    const double t1902 = a[201];
    const double t1904 = (t1899+t1901+t1902)*t101;
    const double t1905 = a[689];
    const double t1906 = t194*t1905;
    const double t1907 = a[407];
    const double t1908 = t101*t1907;
    const double t1909 = a[536];
    const double t1910 = t59*t1909;
    const double t1911 = a[218];
    const double t1913 = (t1906+t1908+t1910+t1911)*t194;
    const double t1914 = a[630];
    const double t1915 = t8*t1914;
    const double t1916 = a[814];
    const double t1917 = t194*t1916;
    const double t1918 = a[659];
    const double t1919 = t101*t1918;
    const double t1920 = a[861];
    const double t1921 = t59*t1920;
    const double t1922 = a[355];
    const double t1924 = (t1915+t1917+t1919+t1921+t1922)*t8;
    const double t1926 = (t1892+t1897+t1904+t1913+t1924)*t8;
    const double t1928 = t8*a[597];
    const double t1930 = (t1928+t1917+t1919+t1921+t1922)*t8;
    const double t1931 = t48*t1877;
    const double t1933 = (t1931+t1915+t1880+t1882+t1884+t1885)*t48;
    const double t1935 = (t1855+t1860+t1867+t1876+t1930+t1933)*t48;
    const double t1937 = (t1807+t1815+t1830+t1854+t1926+t1935)*t48;
    const double t1938 = a[31];
    const double t1939 = a[142];
    const double t1941 = t59*a[434];
    const double t1942 = a[189];
    const double t1944 = (t1941+t1942)*t59;
    const double t1946 = (t1939+t1944)*t59;
    const double t1947 = a[131];
    const double t1949 = t59*a[565];
    const double t1950 = a[204];
    const double t1952 = (t1949+t1950)*t59;
    const double t1953 = a[513];
    const double t1954 = t101*t1953;
    const double t1955 = a[504];
    const double t1956 = t59*t1955;
    const double t1957 = a[370];
    const double t1959 = (t1954+t1956+t1957)*t101;
    const double t1961 = (t1947+t1952+t1959)*t101;
    const double t1962 = a[68];
    const double t1964 = t59*a[771];
    const double t1965 = a[151];
    const double t1967 = (t1964+t1965)*t59;
    const double t1968 = a[534];
    const double t1969 = t101*t1968;
    const double t1970 = a[719];
    const double t1971 = t59*t1970;
    const double t1972 = a[298];
    const double t1974 = (t1969+t1971+t1972)*t101;
    const double t1975 = a[830];
    const double t1976 = t194*t1975;
    const double t1977 = a[885];
    const double t1978 = t101*t1977;
    const double t1979 = a[687];
    const double t1980 = t59*t1979;
    const double t1981 = a[301];
    const double t1983 = (t1976+t1978+t1980+t1981)*t194;
    const double t1985 = (t1962+t1967+t1974+t1983)*t194;
    const double t1986 = a[118];
    const double t1988 = t59*a[493];
    const double t1989 = a[275];
    const double t1991 = (t1988+t1989)*t59;
    const double t1992 = a[523];
    const double t1993 = t101*t1992;
    const double t1994 = a[877];
    const double t1995 = t59*t1994;
    const double t1996 = a[337];
    const double t1998 = (t1993+t1995+t1996)*t101;
    const double t1999 = a[729];
    const double t2000 = t194*t1999;
    const double t2001 = a[418];
    const double t2002 = t101*t2001;
    const double t2003 = a[845];
    const double t2004 = t59*t2003;
    const double t2005 = a[170];
    const double t2007 = (t2000+t2002+t2004+t2005)*t194;
    const double t2008 = a[870];
    const double t2009 = t8*t2008;
    const double t2010 = a[775];
    const double t2011 = t194*t2010;
    const double t2012 = a[496];
    const double t2013 = t101*t2012;
    const double t2014 = a[809];
    const double t2015 = t59*t2014;
    const double t2016 = a[368];
    const double t2018 = (t2009+t2011+t2013+t2015+t2016)*t8;
    const double t2020 = (t1986+t1991+t1998+t2007+t2018)*t8;
    const double t2021 = a[705];
    const double t2022 = t8*t2021;
    const double t2023 = a[631];
    const double t2024 = t194*t2023;
    const double t2025 = a[564];
    const double t2026 = t101*t2025;
    const double t2027 = a[385];
    const double t2028 = t59*t2027;
    const double t2029 = a[350];
    const double t2031 = (t2022+t2024+t2026+t2028+t2029)*t8;
    const double t2032 = t48*t2008;
    const double t2034 = (t2032+t2022+t2011+t2013+t2015+t2016)*t48;
    const double t2036 = (t1986+t1991+t1998+t2007+t2031+t2034)*t48;
    const double t2037 = a[88];
    const double t2039 = t59*a[495];
    const double t2040 = a[245];
    const double t2042 = (t2039+t2040)*t59;
    const double t2043 = a[823];
    const double t2044 = t101*t2043;
    const double t2045 = a[542];
    const double t2046 = t59*t2045;
    const double t2047 = a[178];
    const double t2049 = (t2044+t2046+t2047)*t101;
    const double t2050 = a[767];
    const double t2051 = t194*t2050;
    const double t2052 = a[598];
    const double t2053 = t101*t2052;
    const double t2054 = a[574];
    const double t2055 = t59*t2054;
    const double t2056 = a[382];
    const double t2058 = (t2051+t2053+t2055+t2056)*t194;
    const double t2059 = a[854];
    const double t2060 = t8*t2059;
    const double t2061 = a[438];
    const double t2062 = t194*t2061;
    const double t2063 = a[400];
    const double t2064 = t101*t2063;
    const double t2065 = a[451];
    const double t2066 = t59*t2065;
    const double t2067 = a[362];
    const double t2069 = (t2060+t2062+t2064+t2066+t2067)*t8;
    const double t2070 = t48*t2059;
    const double t2071 = a[452];
    const double t2072 = t8*t2071;
    const double t2074 = (t2070+t2072+t2062+t2064+t2066+t2067)*t48;
    const double t2075 = a[822];
    const double t2076 = t792*t2075;
    const double t2077 = a[636];
    const double t2078 = t48*t2077;
    const double t2079 = t8*t2077;
    const double t2080 = a[836];
    const double t2081 = t194*t2080;
    const double t2082 = a[681];
    const double t2083 = t101*t2082;
    const double t2084 = a[641];
    const double t2085 = t59*t2084;
    const double t2086 = a[335];
    const double t2088 = (t2076+t2078+t2079+t2081+t2083+t2085+t2086)*t792;
    const double t2090 = (t2037+t2042+t2049+t2058+t2069+t2074+t2088)*t792;
    const double t2092 = (t1938+t1946+t1961+t1985+t2020+t2036+t2090)*t792;
    const double t2094 = (t1719+t1730+t1756+t1806+t1891+t1937+t2092)*t792;
    const double t2095 = t101*t1794;
    const double t2097 = (t2095+t1799+t1800)*t101;
    const double t2099 = (t1781+t1786+t2097)*t101;
    const double t2101 = (t1757+t1765+t2099)*t101;
    const double t2103 = (t1797+t1790+t1791)*t101;
    const double t2105 = (t1766+t1771+t2103)*t101;
    const double t2107 = (t1788+t1775+t1776)*t101;
    const double t2108 = t194*t1746;
    const double t2110 = (t2108+t1773+t1749+t1750)*t194;
    const double t2112 = (t1740+t1745+t2107+t2110)*t194;
    const double t2114 = (t1731+t1739+t2105+t2112)*t194;
    const double t2115 = t101*t1844;
    const double t2117 = (t2115+t1849+t1850)*t101;
    const double t2119 = (t1831+t1836+t2117)*t101;
    const double t2121 = (t1847+t1840+t1841)*t101;
    const double t2122 = t194*t1822;
    const double t2124 = (t2122+t1838+t1825+t1826)*t194;
    const double t2126 = (t1816+t1821+t2121+t2124)*t194;
    const double t2127 = t101*t1868;
    const double t2129 = (t2127+t1873+t1874)*t101;
    const double t2130 = t194*t1861;
    const double t2132 = (t2130+t1871+t1864+t1865)*t194;
    const double t2133 = t194*t1881;
    const double t2134 = t101*t1879;
    const double t2136 = (t1878+t2133+t2134+t1884+t1885)*t8;
    const double t2138 = (t1855+t1860+t2129+t2132+t2136)*t8;
    const double t2140 = (t1807+t1815+t2119+t2126+t2138)*t8;
    const double t2141 = t101*t1905;
    const double t2143 = (t2141+t1910+t1911)*t101;
    const double t2144 = t194*t1898;
    const double t2146 = (t2144+t1908+t1901+t1902)*t194;
    const double t2147 = t194*t1918;
    const double t2148 = t101*t1916;
    const double t2150 = (t1915+t2147+t2148+t1921+t1922)*t8;
    const double t2152 = (t1892+t1897+t2143+t2146+t2150)*t8;
    const double t2154 = (t1928+t2147+t2148+t1921+t1922)*t8;
    const double t2156 = (t1931+t1915+t2133+t2134+t1884+t1885)*t48;
    const double t2158 = (t1855+t1860+t2129+t2132+t2154+t2156)*t48;
    const double t2160 = (t1807+t1815+t2119+t2126+t2152+t2158)*t48;
    const double t2161 = a[40];
    const double t2162 = a[51];
    const double t2164 = t59*a[778];
    const double t2165 = a[228];
    const double t2167 = (t2164+t2165)*t59;
    const double t2169 = (t2162+t2167)*t59;
    const double t2170 = a[90];
    const double t2172 = t59*a[710];
    const double t2173 = a[288];
    const double t2175 = (t2172+t2173)*t59;
    const double t2176 = a[685];
    const double t2177 = t101*t2176;
    const double t2178 = a[863];
    const double t2179 = t59*t2178;
    const double t2180 = a[211];
    const double t2182 = (t2177+t2179+t2180)*t101;
    const double t2184 = (t2170+t2175+t2182)*t101;
    const double t2185 = a[482];
    const double t2186 = t101*t2185;
    const double t2187 = a[829];
    const double t2188 = t59*t2187;
    const double t2189 = a[324];
    const double t2191 = (t2186+t2188+t2189)*t101;
    const double t2192 = t194*t2176;
    const double t2194 = (t2192+t2186+t2179+t2180)*t194;
    const double t2196 = (t2170+t2175+t2191+t2194)*t194;
    const double t2197 = a[50];
    const double t2199 = t59*a[531];
    const double t2200 = a[378];
    const double t2202 = (t2199+t2200)*t59;
    const double t2203 = a[391];
    const double t2204 = t101*t2203;
    const double t2205 = a[660];
    const double t2206 = t59*t2205;
    const double t2207 = a[371];
    const double t2209 = (t2204+t2206+t2207)*t101;
    const double t2210 = t194*t2203;
    const double t2211 = a[905];
    const double t2212 = t101*t2211;
    const double t2214 = (t2210+t2212+t2206+t2207)*t194;
    const double t2215 = a[411];
    const double t2216 = t8*t2215;
    const double t2217 = a[820];
    const double t2218 = t194*t2217;
    const double t2219 = t101*t2217;
    const double t2220 = a[881];
    const double t2221 = t59*t2220;
    const double t2222 = a[328];
    const double t2224 = (t2216+t2218+t2219+t2221+t2222)*t8;
    const double t2226 = (t2197+t2202+t2209+t2214+t2224)*t8;
    const double t2227 = a[486];
    const double t2228 = t8*t2227;
    const double t2229 = a[669];
    const double t2230 = t194*t2229;
    const double t2231 = t101*t2229;
    const double t2232 = a[442];
    const double t2233 = t59*t2232;
    const double t2234 = a[217];
    const double t2236 = (t2228+t2230+t2231+t2233+t2234)*t8;
    const double t2237 = t48*t2215;
    const double t2239 = (t2237+t2228+t2218+t2219+t2221+t2222)*t48;
    const double t2241 = (t2197+t2202+t2209+t2214+t2236+t2239)*t48;
    const double t2242 = a[127];
    const double t2244 = t59*a[491];
    const double t2245 = a[269];
    const double t2247 = (t2244+t2245)*t59;
    const double t2248 = a[591];
    const double t2249 = t101*t2248;
    const double t2250 = a[461];
    const double t2251 = t59*t2250;
    const double t2252 = a[295];
    const double t2254 = (t2249+t2251+t2252)*t101;
    const double t2255 = a[893];
    const double t2256 = t194*t2255;
    const double t2257 = a[891];
    const double t2258 = t101*t2257;
    const double t2259 = a[626];
    const double t2260 = t59*t2259;
    const double t2261 = a[191];
    const double t2263 = (t2256+t2258+t2260+t2261)*t194;
    const double t2264 = a[571];
    const double t2265 = t8*t2264;
    const double t2266 = a[433];
    const double t2267 = t194*t2266;
    const double t2268 = a[651];
    const double t2269 = t101*t2268;
    const double t2270 = a[715];
    const double t2271 = t59*t2270;
    const double t2272 = a[361];
    const double t2274 = (t2265+t2267+t2269+t2271+t2272)*t8;
    const double t2275 = t48*t2264;
    const double t2276 = a[397];
    const double t2277 = t8*t2276;
    const double t2279 = (t2275+t2277+t2267+t2269+t2271+t2272)*t48;
    const double t2280 = a[792];
    const double t2281 = t792*t2280;
    const double t2282 = a[865];
    const double t2283 = t48*t2282;
    const double t2284 = t8*t2282;
    const double t2285 = a[582];
    const double t2286 = t194*t2285;
    const double t2287 = a[849];
    const double t2288 = t101*t2287;
    const double t2289 = a[386];
    const double t2290 = t59*t2289;
    const double t2291 = a[322];
    const double t2293 = (t2281+t2283+t2284+t2286+t2288+t2290+t2291)*t792;
    const double t2295 = (t2242+t2247+t2254+t2263+t2274+t2279+t2293)*t792;
    const double t2297 = (t2161+t2169+t2184+t2196+t2226+t2241+t2295)*t792;
    const double t2298 = t101*t1975;
    const double t2300 = (t2298+t1980+t1981)*t101;
    const double t2302 = (t1962+t1967+t2300)*t101;
    const double t2304 = (t1978+t1971+t1972)*t101;
    const double t2305 = t194*t1953;
    const double t2307 = (t2305+t1969+t1956+t1957)*t194;
    const double t2309 = (t1947+t1952+t2304+t2307)*t194;
    const double t2310 = t101*t1999;
    const double t2312 = (t2310+t2004+t2005)*t101;
    const double t2313 = t194*t1992;
    const double t2315 = (t2313+t2002+t1995+t1996)*t194;
    const double t2316 = t194*t2012;
    const double t2317 = t101*t2010;
    const double t2319 = (t2009+t2316+t2317+t2015+t2016)*t8;
    const double t2321 = (t1986+t1991+t2312+t2315+t2319)*t8;
    const double t2322 = t194*t2025;
    const double t2323 = t101*t2023;
    const double t2325 = (t2022+t2322+t2323+t2028+t2029)*t8;
    const double t2327 = (t2032+t2022+t2316+t2317+t2015+t2016)*t48;
    const double t2329 = (t1986+t1991+t2312+t2315+t2325+t2327)*t48;
    const double t2330 = t101*t2255;
    const double t2332 = (t2330+t2260+t2261)*t101;
    const double t2333 = t194*t2248;
    const double t2335 = (t2333+t2258+t2251+t2252)*t194;
    const double t2336 = t194*t2268;
    const double t2337 = t101*t2266;
    const double t2339 = (t2265+t2336+t2337+t2271+t2272)*t8;
    const double t2341 = (t2275+t2277+t2336+t2337+t2271+t2272)*t48;
    const double t2343 = t792*a[657];
    const double t2344 = a[732];
    const double t2345 = t48*t2344;
    const double t2346 = t8*t2344;
    const double t2347 = a[666];
    const double t2348 = t194*t2347;
    const double t2349 = t101*t2347;
    const double t2350 = a[789];
    const double t2351 = t59*t2350;
    const double t2352 = a[384];
    const double t2354 = (t2343+t2345+t2346+t2348+t2349+t2351+t2352)*t792;
    const double t2356 = (t2242+t2247+t2332+t2335+t2339+t2341+t2354)*t792;
    const double t2357 = t101*t2050;
    const double t2359 = (t2357+t2055+t2056)*t101;
    const double t2360 = t194*t2043;
    const double t2362 = (t2360+t2053+t2046+t2047)*t194;
    const double t2363 = t194*t2063;
    const double t2364 = t101*t2061;
    const double t2366 = (t2060+t2363+t2364+t2066+t2067)*t8;
    const double t2368 = (t2070+t2072+t2363+t2364+t2066+t2067)*t48;
    const double t2369 = t194*t2287;
    const double t2370 = t101*t2285;
    const double t2372 = (t2343+t2283+t2284+t2369+t2370+t2290+t2291)*t792;
    const double t2373 = t1440*t2075;
    const double t2374 = t194*t2082;
    const double t2375 = t101*t2080;
    const double t2377 = (t2373+t2281+t2078+t2079+t2374+t2375+t2085+t2086)*t1440;
    const double t2379 = (t2037+t2042+t2359+t2362+t2366+t2368+t2372+t2377)*t1440;
    const double t2381 = (t1938+t1946+t2302+t2309+t2321+t2329+t2356+t2379)*t1440;
    const double t2383 = (t1719+t1730+t2101+t2114+t2140+t2160+t2297+t2381)*t1440;
    const double t2384 = a[10];
    const double t2385 = a[14];
    const double t2386 = a[143];
    const double t2388 = t59*a[618];
    const double t2389 = a[149];
    const double t2391 = (t2388+t2389)*t59;
    const double t2393 = (t2386+t2391)*t59;
    const double t2394 = a[120];
    const double t2396 = t59*a[647];
    const double t2397 = a[215];
    const double t2399 = (t2396+t2397)*t59;
    const double t2400 = a[464];
    const double t2401 = t101*t2400;
    const double t2402 = a[603];
    const double t2403 = t59*t2402;
    const double t2404 = a[286];
    const double t2406 = (t2401+t2403+t2404)*t101;
    const double t2408 = (t2394+t2399+t2406)*t101;
    const double t2410 = (t2385+t2393+t2408)*t101;
    const double t2411 = a[80];
    const double t2413 = t59*a[879];
    const double t2414 = a[247];
    const double t2416 = (t2413+t2414)*t59;
    const double t2417 = a[797];
    const double t2418 = t101*t2417;
    const double t2419 = a[414];
    const double t2420 = t59*t2419;
    const double t2421 = a[317];
    const double t2423 = (t2418+t2420+t2421)*t101;
    const double t2425 = (t2411+t2416+t2423)*t101;
    const double t2427 = t101*a[550];
    const double t2429 = (t2427+t2420+t2421)*t101;
    const double t2430 = t194*t2400;
    const double t2432 = (t2430+t2418+t2403+t2404)*t194;
    const double t2434 = (t2394+t2399+t2429+t2432)*t194;
    const double t2436 = (t2385+t2393+t2425+t2434)*t194;
    const double t2437 = a[36];
    const double t2438 = a[78];
    const double t2440 = t59*a[653];
    const double t2441 = a[176];
    const double t2443 = (t2440+t2441)*t59;
    const double t2444 = a[468];
    const double t2445 = t101*t2444;
    const double t2446 = a[459];
    const double t2447 = t59*t2446;
    const double t2448 = a[343];
    const double t2450 = (t2445+t2447+t2448)*t101;
    const double t2452 = (t2438+t2443+t2450)*t101;
    const double t2453 = a[470];
    const double t2454 = t101*t2453;
    const double t2455 = a[632];
    const double t2456 = t59*t2455;
    const double t2457 = a[194];
    const double t2459 = (t2454+t2456+t2457)*t101;
    const double t2460 = t194*t2444;
    const double t2462 = (t2460+t2454+t2447+t2448)*t194;
    const double t2464 = (t2438+t2443+t2459+t2462)*t194;
    const double t2465 = a[81];
    const double t2466 = a[481];
    const double t2467 = t101*t2466;
    const double t2468 = a[717];
    const double t2469 = t59*t2468;
    const double t2470 = a[280];
    const double t2472 = (t2467+t2469+t2470)*t101;
    const double t2473 = t194*t2466;
    const double t2474 = a[724];
    const double t2475 = t101*t2474;
    const double t2477 = (t2473+t2475+t2469+t2470)*t194;
    const double t2478 = a[547];
    const double t2479 = t8*t2478;
    const double t2480 = a[602];
    const double t2481 = t194*t2480;
    const double t2482 = t101*t2480;
    const double t2483 = a[232];
    const double t2485 = (t2479+t2481+t2482+t2483)*t8;
    const double t2487 = (t2465+t2472+t2477+t2485)*t8;
    const double t2489 = (t2437+t2452+t2464+t2487)*t8;
    const double t2490 = a[104];
    const double t2491 = a[691];
    const double t2492 = t101*t2491;
    const double t2493 = a[746];
    const double t2494 = t59*t2493;
    const double t2495 = a[271];
    const double t2497 = (t2492+t2494+t2495)*t101;
    const double t2498 = t194*t2491;
    const double t2499 = a[543];
    const double t2500 = t101*t2499;
    const double t2502 = (t2498+t2500+t2494+t2495)*t194;
    const double t2503 = a[815];
    const double t2504 = t8*t2503;
    const double t2505 = a[432];
    const double t2506 = t194*t2505;
    const double t2507 = t101*t2505;
    const double t2508 = a[381];
    const double t2510 = (t2504+t2506+t2507+t2508)*t8;
    const double t2512 = (t2490+t2497+t2502+t2510)*t8;
    const double t2514 = t8*a[722];
    const double t2516 = (t2514+t2506+t2507+t2508)*t8;
    const double t2517 = t48*t2478;
    const double t2519 = (t2517+t2504+t2481+t2482+t2483)*t48;
    const double t2521 = (t2465+t2472+t2477+t2516+t2519)*t48;
    const double t2523 = (t2437+t2452+t2464+t2512+t2521)*t48;
    const double t2524 = a[46];
    const double t2525 = a[69];
    const double t2527 = t59*a[889];
    const double t2528 = a[190];
    const double t2530 = (t2527+t2528)*t59;
    const double t2532 = (t2525+t2530)*t59;
    const double t2533 = a[98];
    const double t2535 = t59*a[390];
    const double t2536 = a[296];
    const double t2538 = (t2535+t2536)*t59;
    const double t2539 = a[457];
    const double t2540 = t101*t2539;
    const double t2541 = a[463];
    const double t2542 = t59*t2541;
    const double t2543 = a[273];
    const double t2545 = (t2540+t2542+t2543)*t101;
    const double t2547 = (t2533+t2538+t2545)*t101;
    const double t2548 = a[49];
    const double t2550 = t59*a[568];
    const double t2551 = a[283];
    const double t2553 = (t2550+t2551)*t59;
    const double t2554 = a[759];
    const double t2555 = t101*t2554;
    const double t2556 = a[509];
    const double t2557 = t59*t2556;
    const double t2558 = a[338];
    const double t2560 = (t2555+t2557+t2558)*t101;
    const double t2561 = a[834];
    const double t2562 = t194*t2561;
    const double t2563 = a[508];
    const double t2564 = t101*t2563;
    const double t2565 = a[694];
    const double t2566 = t59*t2565;
    const double t2567 = a[231];
    const double t2569 = (t2562+t2564+t2566+t2567)*t194;
    const double t2571 = (t2548+t2553+t2560+t2569)*t194;
    const double t2572 = a[122];
    const double t2574 = t59*a[399];
    const double t2575 = a[171];
    const double t2577 = (t2574+t2575)*t59;
    const double t2578 = a[573];
    const double t2579 = t101*t2578;
    const double t2580 = a[475];
    const double t2581 = t59*t2580;
    const double t2582 = a[214];
    const double t2584 = (t2579+t2581+t2582)*t101;
    const double t2585 = a[714];
    const double t2586 = t194*t2585;
    const double t2587 = a[750];
    const double t2588 = t101*t2587;
    const double t2589 = a[398];
    const double t2590 = t59*t2589;
    const double t2591 = a[158];
    const double t2593 = (t2586+t2588+t2590+t2591)*t194;
    const double t2594 = a[650];
    const double t2595 = t8*t2594;
    const double t2596 = a[859];
    const double t2597 = t194*t2596;
    const double t2598 = a[620];
    const double t2599 = t101*t2598;
    const double t2600 = a[527];
    const double t2601 = t59*t2600;
    const double t2602 = a[366];
    const double t2604 = (t2595+t2597+t2599+t2601+t2602)*t8;
    const double t2606 = (t2572+t2577+t2584+t2593+t2604)*t8;
    const double t2607 = a[801];
    const double t2608 = t8*t2607;
    const double t2609 = a[596];
    const double t2610 = t194*t2609;
    const double t2611 = a[428];
    const double t2612 = t101*t2611;
    const double t2613 = a[785];
    const double t2614 = t59*t2613;
    const double t2615 = a[223];
    const double t2617 = (t2608+t2610+t2612+t2614+t2615)*t8;
    const double t2618 = t48*t2594;
    const double t2620 = (t2618+t2608+t2597+t2599+t2601+t2602)*t48;
    const double t2622 = (t2572+t2577+t2584+t2593+t2617+t2620)*t48;
    const double t2623 = a[117];
    const double t2625 = t59*a[869];
    const double t2626 = a[173];
    const double t2628 = (t2625+t2626)*t59;
    const double t2629 = a[587];
    const double t2630 = t101*t2629;
    const double t2631 = a[672];
    const double t2632 = t59*t2631;
    const double t2633 = a[179];
    const double t2635 = (t2630+t2632+t2633)*t101;
    const double t2636 = a[743];
    const double t2637 = t194*t2636;
    const double t2638 = a[430];
    const double t2639 = t101*t2638;
    const double t2640 = a[898];
    const double t2641 = t59*t2640;
    const double t2642 = a[237];
    const double t2644 = (t2637+t2639+t2641+t2642)*t194;
    const double t2645 = a[816];
    const double t2646 = t8*t2645;
    const double t2647 = a[405];
    const double t2648 = t194*t2647;
    const double t2649 = a[756];
    const double t2650 = t101*t2649;
    const double t2651 = a[740];
    const double t2652 = t59*t2651;
    const double t2653 = a[235];
    const double t2655 = (t2646+t2648+t2650+t2652+t2653)*t8;
    const double t2656 = t48*t2645;
    const double t2657 = a[665];
    const double t2658 = t8*t2657;
    const double t2660 = (t2656+t2658+t2648+t2650+t2652+t2653)*t48;
    const double t2661 = a[490];
    const double t2662 = t792*t2661;
    const double t2663 = a[544];
    const double t2664 = t48*t2663;
    const double t2665 = t8*t2663;
    const double t2666 = a[787];
    const double t2667 = t194*t2666;
    const double t2668 = a[467];
    const double t2669 = t101*t2668;
    const double t2670 = a[790];
    const double t2671 = t59*t2670;
    const double t2672 = a[192];
    const double t2674 = (t2662+t2664+t2665+t2667+t2669+t2671+t2672)*t792;
    const double t2676 = (t2623+t2628+t2635+t2644+t2655+t2660+t2674)*t792;
    const double t2678 = (t2524+t2532+t2547+t2571+t2606+t2622+t2676)*t792;
    const double t2679 = t101*t2561;
    const double t2681 = (t2679+t2566+t2567)*t101;
    const double t2683 = (t2548+t2553+t2681)*t101;
    const double t2685 = (t2564+t2557+t2558)*t101;
    const double t2686 = t194*t2539;
    const double t2688 = (t2686+t2555+t2542+t2543)*t194;
    const double t2690 = (t2533+t2538+t2685+t2688)*t194;
    const double t2691 = t101*t2585;
    const double t2693 = (t2691+t2590+t2591)*t101;
    const double t2694 = t194*t2578;
    const double t2696 = (t2694+t2588+t2581+t2582)*t194;
    const double t2697 = t194*t2598;
    const double t2698 = t101*t2596;
    const double t2700 = (t2595+t2697+t2698+t2601+t2602)*t8;
    const double t2702 = (t2572+t2577+t2693+t2696+t2700)*t8;
    const double t2703 = t194*t2611;
    const double t2704 = t101*t2609;
    const double t2706 = (t2608+t2703+t2704+t2614+t2615)*t8;
    const double t2708 = (t2618+t2608+t2697+t2698+t2601+t2602)*t48;
    const double t2710 = (t2572+t2577+t2693+t2696+t2706+t2708)*t48;
    const double t2711 = a[84];
    const double t2713 = t59*a[610];
    const double t2714 = a[302];
    const double t2716 = (t2713+t2714)*t59;
    const double t2717 = a[674];
    const double t2718 = t101*t2717;
    const double t2719 = a[772];
    const double t2720 = t59*t2719;
    const double t2721 = a[341];
    const double t2723 = (t2718+t2720+t2721)*t101;
    const double t2724 = t194*t2717;
    const double t2725 = a[485];
    const double t2726 = t101*t2725;
    const double t2728 = (t2724+t2726+t2720+t2721)*t194;
    const double t2729 = a[758];
    const double t2730 = t8*t2729;
    const double t2731 = a[435];
    const double t2732 = t194*t2731;
    const double t2733 = t101*t2731;
    const double t2734 = a[562];
    const double t2735 = t59*t2734;
    const double t2736 = a[220];
    const double t2738 = (t2730+t2732+t2733+t2735+t2736)*t8;
    const double t2739 = t48*t2729;
    const double t2740 = a[622];
    const double t2741 = t8*t2740;
    const double t2743 = (t2739+t2741+t2732+t2733+t2735+t2736)*t48;
    const double t2744 = a[629];
    const double t2745 = t792*t2744;
    const double t2746 = a[813];
    const double t2747 = t48*t2746;
    const double t2748 = t8*t2746;
    const double t2749 = a[842];
    const double t2750 = t194*t2749;
    const double t2751 = a[549];
    const double t2752 = t101*t2751;
    const double t2753 = a[589];
    const double t2754 = t59*t2753;
    const double t2755 = a[197];
    const double t2757 = (t2745+t2747+t2748+t2750+t2752+t2754+t2755)*t792;
    const double t2759 = (t2711+t2716+t2723+t2728+t2738+t2743+t2757)*t792;
    const double t2760 = t101*t2636;
    const double t2762 = (t2760+t2641+t2642)*t101;
    const double t2763 = t194*t2629;
    const double t2765 = (t2763+t2639+t2632+t2633)*t194;
    const double t2766 = t194*t2649;
    const double t2767 = t101*t2647;
    const double t2769 = (t2646+t2766+t2767+t2652+t2653)*t8;
    const double t2771 = (t2656+t2658+t2766+t2767+t2652+t2653)*t48;
    const double t2773 = t792*a[784];
    const double t2774 = t194*t2751;
    const double t2775 = t101*t2749;
    const double t2777 = (t2773+t2747+t2748+t2774+t2775+t2754+t2755)*t792;
    const double t2778 = t1440*t2661;
    const double t2779 = t194*t2668;
    const double t2780 = t101*t2666;
    const double t2782 = (t2778+t2745+t2664+t2665+t2779+t2780+t2671+t2672)*t1440;
    const double t2784 = (t2623+t2628+t2762+t2765+t2769+t2771+t2777+t2782)*t1440;
    const double t2786 = (t2524+t2532+t2683+t2690+t2702+t2710+t2759+t2784)*t1440;
    const double t2787 = a[18];
    const double t2788 = a[86];
    const double t2790 = t59*a[577];
    const double t2791 = a[224];
    const double t2793 = (t2790+t2791)*t59;
    const double t2794 = a[862];
    const double t2795 = t101*t2794;
    const double t2796 = a[607];
    const double t2797 = t59*t2796;
    const double t2798 = a[177];
    const double t2800 = (t2795+t2797+t2798)*t101;
    const double t2802 = (t2788+t2793+t2800)*t101;
    const double t2803 = a[850];
    const double t2804 = t101*t2803;
    const double t2805 = a[551];
    const double t2806 = t59*t2805;
    const double t2807 = a[356];
    const double t2809 = (t2804+t2806+t2807)*t101;
    const double t2810 = t194*t2794;
    const double t2812 = (t2810+t2804+t2797+t2798)*t194;
    const double t2814 = (t2788+t2793+t2809+t2812)*t194;
    const double t2815 = a[61];
    const double t2816 = a[733];
    const double t2817 = t101*t2816;
    const double t2818 = a[894];
    const double t2819 = t59*t2818;
    const double t2820 = a[289];
    const double t2822 = (t2817+t2819+t2820)*t101;
    const double t2823 = t194*t2816;
    const double t2824 = a[395];
    const double t2825 = t101*t2824;
    const double t2827 = (t2823+t2825+t2819+t2820)*t194;
    const double t2828 = a[794];
    const double t2829 = t8*t2828;
    const double t2830 = a[469];
    const double t2831 = t194*t2830;
    const double t2832 = t101*t2830;
    const double t2833 = a[162];
    const double t2835 = (t2829+t2831+t2832+t2833)*t8;
    const double t2837 = (t2815+t2822+t2827+t2835)*t8;
    const double t2838 = a[600];
    const double t2839 = t8*t2838;
    const double t2840 = a[460];
    const double t2841 = t194*t2840;
    const double t2842 = t101*t2840;
    const double t2843 = a[200];
    const double t2845 = (t2839+t2841+t2842+t2843)*t8;
    const double t2846 = t48*t2828;
    const double t2848 = (t2846+t2839+t2831+t2832+t2833)*t48;
    const double t2850 = (t2815+t2822+t2827+t2845+t2848)*t48;
    const double t2851 = a[66];
    const double t2853 = t59*a[670];
    const double t2854 = a[369];
    const double t2856 = (t2853+t2854)*t59;
    const double t2857 = a[662];
    const double t2858 = t101*t2857;
    const double t2859 = a[847];
    const double t2860 = t59*t2859;
    const double t2861 = a[262];
    const double t2863 = (t2858+t2860+t2861)*t101;
    const double t2864 = a[556];
    const double t2865 = t194*t2864;
    const double t2866 = a[699];
    const double t2867 = t101*t2866;
    const double t2868 = a[712];
    const double t2869 = t59*t2868;
    const double t2870 = a[349];
    const double t2872 = (t2865+t2867+t2869+t2870)*t194;
    const double t2873 = a[608];
    const double t2874 = t8*t2873;
    const double t2875 = a[844];
    const double t2876 = t194*t2875;
    const double t2877 = a[886];
    const double t2878 = t101*t2877;
    const double t2879 = a[667];
    const double t2880 = t59*t2879;
    const double t2881 = a[213];
    const double t2883 = (t2874+t2876+t2878+t2880+t2881)*t8;
    const double t2884 = t48*t2873;
    const double t2885 = a[723];
    const double t2886 = t8*t2885;
    const double t2888 = (t2884+t2886+t2876+t2878+t2880+t2881)*t48;
    const double t2889 = a[426];
    const double t2890 = t792*t2889;
    const double t2891 = a[855];
    const double t2892 = t48*t2891;
    const double t2893 = t8*t2891;
    const double t2894 = a[892];
    const double t2895 = t194*t2894;
    const double t2896 = a[876];
    const double t2897 = t101*t2896;
    const double t2898 = a[601];
    const double t2899 = t59*t2898;
    const double t2900 = a[282];
    const double t2902 = (t2890+t2892+t2893+t2895+t2897+t2899+t2900)*t792;
    const double t2904 = (t2851+t2856+t2863+t2872+t2883+t2888+t2902)*t792;
    const double t2905 = t101*t2864;
    const double t2907 = (t2905+t2869+t2870)*t101;
    const double t2908 = t194*t2857;
    const double t2910 = (t2908+t2867+t2860+t2861)*t194;
    const double t2911 = t194*t2877;
    const double t2912 = t101*t2875;
    const double t2914 = (t2874+t2911+t2912+t2880+t2881)*t8;
    const double t2916 = (t2884+t2886+t2911+t2912+t2880+t2881)*t48;
    const double t2917 = a[479];
    const double t2918 = t792*t2917;
    const double t2919 = a[640];
    const double t2920 = t48*t2919;
    const double t2921 = t8*t2919;
    const double t2922 = a[903];
    const double t2923 = t194*t2922;
    const double t2924 = t101*t2922;
    const double t2925 = a[611];
    const double t2926 = t59*t2925;
    const double t2927 = a[242];
    const double t2929 = (t2918+t2920+t2921+t2923+t2924+t2926+t2927)*t792;
    const double t2930 = t1440*t2889;
    const double t2931 = t194*t2896;
    const double t2932 = t101*t2894;
    const double t2934 = (t2930+t2918+t2892+t2893+t2931+t2932+t2899+t2900)*t1440;
    const double t2936 = (t2851+t2856+t2907+t2910+t2914+t2916+t2929+t2934)*t1440;
    const double t2937 = a[102];
    const double t2938 = a[698];
    const double t2939 = t101*t2938;
    const double t2940 = a[578];
    const double t2941 = t59*t2940;
    const double t2942 = a[290];
    const double t2944 = (t2939+t2941+t2942)*t101;
    const double t2945 = t194*t2938;
    const double t2946 = a[488];
    const double t2947 = t101*t2946;
    const double t2949 = (t2945+t2947+t2941+t2942)*t194;
    const double t2950 = a[518];
    const double t2951 = t8*t2950;
    const double t2952 = a[702];
    const double t2953 = t194*t2952;
    const double t2954 = t101*t2952;
    const double t2955 = a[261];
    const double t2957 = (t2951+t2953+t2954+t2955)*t8;
    const double t2958 = t48*t2950;
    const double t2959 = a[579];
    const double t2960 = t8*t2959;
    const double t2962 = (t2958+t2960+t2953+t2954+t2955)*t48;
    const double t2963 = a[827];
    const double t2964 = t792*t2963;
    const double t2965 = a[742];
    const double t2966 = t48*t2965;
    const double t2967 = t8*t2965;
    const double t2968 = a[492];
    const double t2969 = t194*t2968;
    const double t2970 = a[671];
    const double t2971 = t101*t2970;
    const double t2972 = a[779];
    const double t2973 = t59*t2972;
    const double t2974 = a[219];
    const double t2976 = (t2964+t2966+t2967+t2969+t2971+t2973+t2974)*t792;
    const double t2977 = t1440*t2963;
    const double t2978 = a[616];
    const double t2979 = t792*t2978;
    const double t2980 = t194*t2970;
    const double t2981 = t101*t2968;
    const double t2983 = (t2977+t2979+t2966+t2967+t2980+t2981+t2973+t2974)*t1440;
    const double t2982 = x[0];
    const double t2985 = t2982*a[499];
    const double t2986 = a[768];
    const double t2987 = t1440*t2986;
    const double t2988 = t792*t2986;
    const double t2989 = a[684];
    const double t2990 = t48*t2989;
    const double t2991 = t8*t2989;
    const double t2992 = a[846];
    const double t2993 = t194*t2992;
    const double t2994 = t101*t2992;
    const double t2995 = a[168];
    const double t2997 = (t2985+t2987+t2988+t2990+t2991+t2993+t2994+t2995)*t2982;
    const double t2999 = (t2937+t2944+t2949+t2957+t2962+t2976+t2983+t2997)*t2982;
    const double t3001 = (t2787+t2802+t2814+t2837+t2850+t2904+t2936+t2999)*t2982;
    const double t3003 = (t2384+t2410+t2436+t2489+t2523+t2678+t2786+t3001)*t2982;
    const double t3041 = t2982*t2986;
    const double t3068 = 2.0*t1360;
    const double t3097 = 2.0*t2343;
    const double t3148 = 2.0*t450;
    const double t3155 = 2.0*t652;
    const double t3160 = 2.0*t742;
    const double t3163 = t792*t782;
    const double t3164 = 2.0*t775;
    const double t3184 = 2.0*t1174;
    const double t3187 = t792*t1219;
    const double t3188 = 2.0*t1212;
    const double t3201 = t792*t1361;
    const double t3211 = t1440*t782;
    const double t3227 = 2.0*t1931;
    const double t3232 = 2.0*t2032;
    const double t3235 = t792*t2077;
    const double t3236 = 2.0*t2070;
    const double t3250 = t792*t2282;
    const double t3251 = 2.0*t2275;
    const double t3258 = t792*t2344;
    const double t3261 = t1440*t2077;
    const double t3273 = 2.0*t2618;
    const double t3276 = t792*t2663;
    const double t3277 = 2.0*t2656;
    const double t3284 = t792*t2746;
    const double t3288 = t1440*t2663;
    const double t3296 = t792*t2891;
    const double t3297 = 2.0*t2884;
    const double t3300 = t1440*t2891;
    const double t3301 = t792*t2919;
    const double t3304 = t2982*t2989;
    const double t3305 = t1440*t2965;
    const double t3306 = t792*t2965;
    const double t3333 = 2.0*t34;
    const double t3350 = 2.0*t347;
    const double t3357 = 2.0*t425;
    const double t3362 = 2.0*t438;
    const double t3365 = t48*t424;
    const double t3372 = 2.0*t606;
    const double t3377 = 2.0*t638;
    const double t3380 = t48*t637;
    const double t3381 = 2.0*t649;
    const double t3386 = 2.0*t723;
    const double t3389 = t48*t733;
    const double t3390 = 2.0*t734;
    const double t3393 = t48*t776;
    const double t3394 = 2.0*t767;
    const double t3435 = 2.0*t1151;
    const double t3438 = t48*t1163;
    const double t3439 = 2.0*t1164;
    const double t3442 = t48*t1213;
    const double t3443 = 2.0*t1202;
    const double t3496 = 2.0*t1699;
    const double t3506 = 2.0*t1878;
    const double t3511 = 2.0*t1915;
    const double t3514 = t48*t1914;
    const double t3515 = 2.0*t1928;
    const double t3520 = 2.0*t2009;
    const double t3523 = t48*t2021;
    const double t3524 = 2.0*t2022;
    const double t3527 = t48*t2071;
    const double t3528 = 2.0*t2060;
    const double t3552 = t48*t2276;
    const double t3553 = 2.0*t2265;
    const double t3584 = 2.0*t2595;
    const double t3587 = t48*t2607;
    const double t3588 = 2.0*t2608;
    const double t3591 = t48*t2657;
    const double t3592 = 2.0*t2646;
    const double t3616 = t48*t2885;
    const double t3617 = 2.0*t2874;
    const double t3640 = (2.0*t278+t265+t272)*t194;
    const double t3641 = 2.0*t324;
    const double t3648 = t422*t8;
    const double t3659 = 2.0*t583;
    const double t3688 = ((2.0*t839+t253+t230+t231)*t194+t221+t226+t838+t841)*t194;
    const double t3691 = (2.0*t851+t313+t301+t302)*t194;
    const double t3692 = t8*t348;
    const double t3693 = 2.0*t858;
    const double t3703 = t8*t426;
    const double t3704 = 2.0*t882;
    const double t3709 = t8*t439;
    const double t3712 = t48*t348;
    const double t3726 = (2.0*t1002+t996+t989+t990)*t194;
    const double t3727 = t8*t1027;
    const double t3728 = 2.0*t1020;
    const double t3733 = t8*t1059;
    const double t3737 = t48*t1027;
    const double t3745 = t8*t1152;
    const double t3746 = 2.0*t1142;
    const double t3749 = t48*t1152;
    const double t3750 = t8*t1165;
    const double t3753 = t792*t1222;
    const double t3754 = t48*t1203;
    const double t3755 = t8*t1203;
    const double t3770 = (2.0*t1260+t572+t560+t561)*t194;
    const double t3771 = t8*t607;
    const double t3772 = 2.0*t1266;
    const double t3777 = t8*t639;
    const double t3781 = t48*t607;
    const double t3789 = t8*t1154;
    const double t3790 = 2.0*t1309;
    const double t3793 = t48*t1154;
    const double t3794 = t8*t1167;
    const double t3797 = t792*t1364;
    const double t3798 = t48*t1346;
    const double t3799 = t8*t1346;
    const double t3808 = t8*t724;
    const double t3809 = 2.0*t1389;
    const double t3812 = t48*t724;
    const double t3813 = t8*t735;
    const double t3816 = t792*t1366;
    const double t3817 = t48*t1205;
    const double t3818 = t8*t1205;
    const double t3823 = t792*t1224;
    const double t3824 = t48*t768;
    const double t3825 = t8*t768;
    const double t3846 = ((2.0*t1583+t1571+t1556+t1557)*t194+t1547+t1552+t1582+t1585)*t194;
    const double t3849 = (2.0*t1613+t1607+t1600+t1601)*t194;
    const double t3850 = t8*t1633;
    const double t3851 = 2.0*t1626;
    const double t3861 = t8*t1688;
    const double t3862 = 2.0*t1681;
    const double t3867 = t8*t1700;
    const double t3870 = t48*t1633;
    const double t3884 = (2.0*t1845+t1847+t1849+t1850)*t194;
    const double t3885 = t8*t1879;
    const double t3886 = 2.0*t1869;
    const double t3891 = t8*t1916;
    const double t3895 = t48*t1879;
    const double t3903 = t8*t2010;
    const double t3904 = 2.0*t2000;
    const double t3907 = t48*t2010;
    const double t3908 = t8*t2023;
    const double t3912 = t48*t2061;
    const double t3913 = t8*t2061;
    const double t3928 = (2.0*t2122+t1838+t1825+t1826)*t194;
    const double t3929 = t8*t1881;
    const double t3930 = 2.0*t2130;
    const double t3935 = t8*t1918;
    const double t3939 = t48*t1881;
    const double t3947 = t8*t2217;
    const double t3948 = 2.0*t2210;
    const double t3951 = t48*t2217;
    const double t3952 = t8*t2229;
    const double t3955 = t792*t2285;
    const double t3956 = t48*t2266;
    const double t3957 = t8*t2266;
    const double t3966 = t8*t2012;
    const double t3967 = 2.0*t2313;
    const double t3970 = t48*t2012;
    const double t3971 = t8*t2025;
    const double t3974 = t792*t2347;
    const double t3975 = t48*t2268;
    const double t3976 = t8*t2268;
    const double t3981 = t792*t2287;
    const double t3982 = t48*t2063;
    const double t3983 = t8*t2063;
    const double t3998 = (2.0*t2460+t2454+t2447+t2448)*t194;
    const double t3999 = t8*t2480;
    const double t4000 = 2.0*t2473;
    const double t4005 = t8*t2505;
    const double t4009 = t48*t2480;
    const double t4017 = t8*t2596;
    const double t4018 = 2.0*t2586;
    const double t4021 = t48*t2596;
    const double t4022 = t8*t2609;
    const double t4026 = t48*t2647;
    const double t4027 = t8*t2647;
    const double t4036 = t8*t2598;
    const double t4037 = 2.0*t2694;
    const double t4040 = t48*t2598;
    const double t4041 = t8*t2611;
    const double t4044 = t792*t2749;
    const double t4045 = t48*t2731;
    const double t4046 = t8*t2731;
    const double t4051 = t792*t2751;
    const double t4052 = t48*t2649;
    const double t4053 = t8*t2649;
    const double t4062 = t8*t2830;
    const double t4063 = 2.0*t2823;
    const double t4066 = t48*t2830;
    const double t4067 = t8*t2840;
    const double t4071 = t48*t2875;
    const double t4072 = t8*t2875;
    const double t4077 = t792*t2922;
    const double t4078 = t48*t2877;
    const double t4079 = t8*t2877;
    const double t4083 = t2982*t2992;
    const double t4086 = t48*t2952;
    const double t4087 = t8*t2952;
    const double t4110 = 2.0*t168;
    const double t4114 = 2.0*t182;
    const double t4125 = ((2.0*t228+t230+t231)*t101+t221+t226+t233)*t101;
    const double t4130 = 2.0*t267;
    const double t4134 = ((2.0*t253+t255+t256)*t101+t246+t251+t258+(t194*t273+t269+t270+
t4130)*t194)*t194;
    const double t4137 = (2.0*t299+t301+t302)*t101;
    const double t4141 = (t194*t319+2.0*t313+t315+t316)*t194;
    const double t4142 = t194*t340;
    const double t4143 = 2.0*t334;
    const double t4157 = t194*t418;
    const double t4158 = 2.0*t412;
    const double t4180 = 2.0*t526;
    const double t4187 = (2.0*t558+t560+t561)*t101;
    const double t4191 = (t194*t578+2.0*t572+t574+t575)*t194;
    const double t4192 = t194*t599;
    const double t4193 = 2.0*t593;
    const double t4198 = t194*t631;
    const double t4213 = t194*t716;
    const double t4214 = 2.0*t710;
    const double t4220 = t194*t760;
    const double t4251 = (2.0*t828+t265)*t101;
    const double t4259 = ((2.0*t274+t269+t270)*t101+t246+t251+t834+(t194*t252+t255+t256+
t4130)*t194)*t194;
    const double t4260 = 2.0*t846;
    const double t4264 = (t194*t312+t315+t316+2.0*t320)*t194;
    const double t4265 = t4142+t343;
    const double t4277 = (t4157+t421)*t8;
    const double t4301 = (2.0*t987+t989+t990)*t101;
    const double t4305 = (t194*t995+2.0*t996+t998+t999)*t194;
    const double t4306 = t194*t1021;
    const double t4307 = 2.0*t1014;
    const double t4327 = t194*t1143;
    const double t4328 = 2.0*t1135;
    const double t4333 = t194*t1194;
    const double t4352 = 2.0*t1255;
    const double t4356 = (t194*t571+t574+t575+2.0*t579)*t194;
    const double t4357 = t4192+t602;
    const double t4373 = 2.0*t1306;
    const double t4389 = t4213+t719;
    const double t4415 = 2.0*t1518;
    const double t4429 = ((2.0*t1554+t1556+t1557)*t101+t1547+t1552+t1559)*t101;
    const double t4438 = ((2.0*t1571+t1573+t1574)*t101+t1564+t1569+t1576+(t1570*t194+t1573+
t1574+2.0*t1580)*t194)*t194;
    const double t4441 = (2.0*t1598+t1600+t1601)*t101;
    const double t4445 = (t1606*t194+2.0*t1607+t1609+t1610)*t194;
    const double t4446 = t194*t1627;
    const double t4447 = 2.0*t1620;
    const double t4461 = t194*t1682;
    const double t4462 = 2.0*t1675;
    const double t4484 = 2.0*t1788;
    const double t4491 = (2.0*t1823+t1825+t1826)*t101;
    const double t4495 = (t1846*t194+2.0*t1838+t1840+t1841)*t194;
    const double t4496 = t194*t1870;
    const double t4497 = 2.0*t1862;
    const double t4502 = t194*t1907;
    const double t4517 = t194*t2001;
    const double t4518 = 2.0*t1993;
    const double t4524 = t194*t2052;
    const double t4547 = (2.0*t2115+t1849+t1850)*t101;
    const double t4551 = (t1837*t194+t1840+t1841+2.0*t1847)*t194;
    const double t4552 = 2.0*t2127;
    const double t4571 = t194*t2211;
    const double t4572 = 2.0*t2204;
    const double t4577 = t194*t2257;
    const double t4590 = 2.0*t2310;
    const double t4622 = (2.0*t2445+t2447+t2448)*t101;
    const double t4626 = (t194*t2453+2.0*t2454+t2456+t2457)*t194;
    const double t4627 = t194*t2474;
    const double t4628 = 2.0*t2467;
    const double t4648 = t194*t2587;
    const double t4649 = 2.0*t2579;
    const double t4655 = t194*t2638;
    const double t4668 = 2.0*t2691;
    const double t4690 = t194*t2824;
    const double t4691 = 2.0*t2817;
    const double t4697 = t194*t2866;
    const double t4724 = (((2.0*t66+t67)*t59+t64+t69)*t59+t63+t71)*t59;
    const double t4729 = ((2.0*t80+t81)*t59+t78+t83)*t59;
    const double t4732 = (2.0*t91+t92)*t59;
    const double t4734 = 2.0*t99;
    const double t4745 = ((2.0*t119+t120)*t59+t117+t122)*t59;
    const double t4748 = (2.0*t130+t131)*t59;
    const double t4749 = t101*t144;
    const double t4750 = 2.0*t138;
    const double t4757 = (2.0*t155+t156)*t59;
    const double t4758 = t101*t169;
    const double t4759 = 2.0*t163;
    const double t4763 = t101*t183;
    const double t4764 = 2.0*t177;
    const double t4775 = ((2.0*t204+t205)*t59+t202+t207)*t59;
    const double t4778 = (2.0*t215+t216)*t59;
    const double t4780 = 2.0*t223;
    const double t4784 = (t4778+t213+t218+(t101*t229+t224+t4780)*t101)*t101;
    const double t4787 = (2.0*t240+t241)*t59;
    const double t4788 = t101*t254;
    const double t4789 = 2.0*t248;
    const double t4793 = t101*t268;
    const double t4794 = 2.0*t262;
    const double t4798 = (t4787+t238+t243+(t4788+t4789+t249)*t101+(t194*t275+t263+t4793+
t4794)*t194)*t194;
    const double t4801 = (2.0*t286+t287)*t59;
    const double t4803 = 2.0*t294;
    const double t4805 = (t101*t300+t295+t4803)*t101;
    const double t4807 = t101*t314;
    const double t4808 = 2.0*t308;
    const double t4810 = (t194*t321+t309+t4807+t4808)*t194;
    const double t4811 = t8*t350;
    const double t4812 = t194*t342;
    const double t4813 = t101*t335;
    const double t4814 = 2.0*t329;
    const double t4823 = (2.0*t364+t365)*t59;
    const double t4825 = 2.0*t372;
    const double t4829 = t101*t392;
    const double t4830 = 2.0*t386;
    const double t4833 = t8*t428;
    const double t4834 = t194*t420;
    const double t4835 = t101*t413;
    const double t4836 = 2.0*t407;
    const double t4841 = t8*t441;
    const double t4844 = t48*t350;
    const double t4855 = ((2.0*t463+t464)*t59+t461+t466)*t59;
    const double t4858 = (2.0*t474+t475)*t59;
    const double t4860 = 2.0*t482;
    const double t4867 = (2.0*t499+t500)*t59;
    const double t4868 = t101*t513;
    const double t4869 = 2.0*t507;
    const double t4873 = t101*t527;
    const double t4874 = 2.0*t521;
    const double t4881 = (2.0*t545+t546)*t59;
    const double t4883 = 2.0*t553;
    const double t4885 = (t101*t559+t4883+t554)*t101;
    const double t4887 = t101*t573;
    const double t4888 = 2.0*t567;
    const double t4890 = (t194*t580+t4887+t4888+t568)*t194;
    const double t4891 = t8*t609;
    const double t4892 = t194*t601;
    const double t4893 = t101*t594;
    const double t4894 = 2.0*t588;
    const double t4899 = t8*t641;
    const double t4902 = 2.0*t620;
    const double t4905 = t48*t609;
    const double t4912 = (2.0*t662+t663)*t59;
    const double t4914 = 2.0*t670;
    const double t4918 = t101*t690;
    const double t4919 = 2.0*t684;
    const double t4922 = t8*t726;
    const double t4923 = t194*t718;
    const double t4924 = t101*t711;
    const double t4925 = 2.0*t705;
    const double t4928 = t48*t726;
    const double t4929 = t8*t737;
    const double t4933 = t48*t770;
    const double t4934 = t8*t770;
    const double t4937 = 2.0*t749;
    const double t4970 = (t4787+t238+t243+(t101*t275+t263+t4794)*t101)*t101;
    const double t4977 = (t4778+t213+t218+(t4793+t4789+t249)*t101+(t194*t229+t224+t4780+
t4788)*t194)*t194;
    const double t4980 = (t101*t321+t309+t4808)*t101;
    const double t4983 = (t194*t300+t295+t4803+t4807)*t194;
    const double t4984 = t194*t335;
    const double t4997 = t194*t413;
    const double t5017 = (2.0*t922+t923)*t59;
    const double t5019 = 2.0*t930;
    const double t5024 = t101*t953;
    const double t5035 = (2.0*t974+t975)*t59;
    const double t5037 = 2.0*t982;
    const double t5039 = (t101*t988+t5037+t983)*t101;
    const double t5043 = (t101*t997+t194*t988+t5037+t983)*t194;
    const double t5045 = t194*t1015;
    const double t5046 = t101*t1015;
    const double t5047 = 2.0*t1009;
    const double t5052 = t8*t1062;
    const double t5065 = (2.0*t1083+t1084)*t59;
    const double t5067 = 2.0*t1091;
    const double t5071 = t101*t1112;
    const double t5072 = 2.0*t1106;
    const double t5075 = t8*t1156;
    const double t5076 = t194*t1145;
    const double t5077 = t101*t1136;
    const double t5078 = 2.0*t1130;
    const double t5081 = t48*t1156;
    const double t5082 = t8*t1169;
    const double t5085 = t792*t1226;
    const double t5086 = t48*t1207;
    const double t5087 = t8*t1207;
    const double t5090 = 2.0*t1181;
    const double t5111 = (t101*t580+t4888+t568)*t101;
    const double t5114 = (t194*t559+t4883+t4887+t554)*t194;
    const double t5115 = t194*t594;
    const double t5116 = t101*t601;
    const double t5134 = t194*t1136;
    const double t5135 = t101*t1145;
    const double t5140 = t792*t1368;
    const double t5156 = t194*t711;
    const double t5179 = ((2.0*t1455+t1456)*t59+t1453+t1458)*t59;
    const double t5182 = (2.0*t1466+t1467)*t59;
    const double t5184 = 2.0*t1474;
    const double t5194 = t101*t1508;
    const double t5195 = 2.0*t1502;
    const double t5212 = (2.0*t1541+t1542)*t59;
    const double t5214 = 2.0*t1549;
    const double t5218 = (t5212+t1539+t1544+(t101*t1555+t1550+t5214)*t101)*t101;
    const double t5219 = t101*t1572;
    const double t5227 = (t5212+t1539+t1544+(t5219+2.0*t1566+t1567)*t101+(t1555*t194+t1550+
t5214+t5219)*t194)*t194;
    const double t5229 = 2.0*t1593;
    const double t5231 = (t101*t1599+t1594+t5229)*t101;
    const double t5235 = (t101*t1608+t1599*t194+t1594+t5229)*t194;
    const double t5238 = t101*t1621+t1621*t194;
    const double t5245 = 2.0*t1648;
    const double t5255 = (t101*t1676+t1676*t194)*t8;
    const double t5267 = ((2.0*t1723+t1724)*t59+t1721+t1726)*t59;
    const double t5270 = (2.0*t1734+t1735)*t59;
    const double t5272 = 2.0*t1742;
    const double t5279 = (2.0*t1760+t1761)*t59;
    const double t5280 = t101*t1774;
    const double t5281 = 2.0*t1768;
    const double t5285 = t101*t1789;
    const double t5286 = 2.0*t1783;
    const double t5293 = (2.0*t1810+t1811)*t59;
    const double t5295 = 2.0*t1818;
    const double t5297 = (t101*t1824+t1819+t5295)*t101;
    const double t5299 = t101*t1839;
    const double t5300 = 2.0*t1833;
    const double t5302 = (t1848*t194+t1834+t5299+t5300)*t194;
    const double t5303 = t8*t1883;
    const double t5304 = t194*t1872;
    const double t5305 = t101*t1863;
    const double t5306 = 2.0*t1857;
    const double t5311 = t8*t1920;
    const double t5314 = 2.0*t1894;
    const double t5317 = t48*t1883;
    const double t5324 = (2.0*t1941+t1942)*t59;
    const double t5326 = 2.0*t1949;
    const double t5330 = t101*t1970;
    const double t5331 = 2.0*t1964;
    const double t5334 = t8*t2014;
    const double t5335 = t194*t2003;
    const double t5336 = t101*t1994;
    const double t5337 = 2.0*t1988;
    const double t5340 = t48*t2014;
    const double t5341 = t8*t2027;
    const double t5345 = t48*t2065;
    const double t5346 = t8*t2065;
    const double t5349 = 2.0*t2039;
    const double t5370 = (t101*t1848+t1834+t5300)*t101;
    const double t5373 = (t1824*t194+t1819+t5295+t5299)*t194;
    const double t5374 = t194*t1863;
    const double t5375 = t101*t1872;
    const double t5392 = 2.0*t2172;
    const double t5400 = t194*t2205;
    const double t5401 = t101*t2205;
    const double t5402 = 2.0*t2199;
    const double t5409 = t792*t2289;
    const double t5410 = t48*t2270;
    const double t5411 = t8*t2270;
    const double t5414 = 2.0*t2244;
    const double t5425 = t194*t1994;
    const double t5426 = t101*t2003;
    const double t5447 = (2.0*t2388+t2389)*t59;
    const double t5449 = 2.0*t2396;
    const double t5454 = t101*t2419;
    const double t5464 = 2.0*t2440;
    const double t5466 = (t101*t2446+t2441+t5464)*t101;
    const double t5470 = (t101*t2455+t194*t2446+t2441+t5464)*t194;
    const double t5473 = t101*t2468+t194*t2468;
    const double t5486 = (2.0*t2527+t2528)*t59;
    const double t5488 = 2.0*t2535;
    const double t5492 = t101*t2556;
    const double t5493 = 2.0*t2550;
    const double t5496 = t8*t2600;
    const double t5497 = t194*t2589;
    const double t5498 = t101*t2580;
    const double t5499 = 2.0*t2574;
    const double t5502 = t48*t2600;
    const double t5503 = t8*t2613;
    const double t5507 = t48*t2651;
    const double t5508 = t8*t2651;
    const double t5511 = 2.0*t2625;
    const double t5522 = t194*t2580;
    const double t5523 = t101*t2589;
    const double t5528 = t792*t2753;
    const double t5544 = 2.0*t2790;
    const double t5553 = t101*t2818+t194*t2818;
    const double t5557 = t48*t2879;
    const double t5558 = t8*t2879;
    const double t5561 = 2.0*t2853;
    g[0] = ((((2.0*t2985+t2987+t2988+t2990+t2991+t2993+t2994+t2995)*
t2982+t2937+t2944+t2949+t2957+t2962+t2976+t2983+t2997)*t2982+t2787+t2802+t2814+
t2837+t2850+t2904+t2936+t2999)*t2982+t2384+t2410+t2436+t2489+t2523+t2678+t2786+
t3001)*t2982+t1450+t1490+t1536+t1644+t1718+t2094+t2383+t3003;
    g[1] = ((((2.0*t1438+t1218+t783+t784+t1439+t788+t789)*t1440+t747+
t752+t1425+t1428+t1431+t1433+t1437+t1441)*t1440+t659+t667+t1379+t1386+t1396+
t1403+t1423+t1443)*t1440+t459+t470+t1241+t1254+t1275+t1293+t1376+t1445)*t1440+
t61+t75+t806+t827+t869+t906+t1236+t1447+((((2.0*t2373+t2281+t2078+t2079+t2374+
t2375+t2085+t2086)*t1440+t2037+t2042+t2359+t2362+t2366+t2368+t2372+t2377)*t1440
+t1938+t1946+t2302+t2309+t2321+t2329+t2356+t2379)*t1440+t1719+t1730+t2101+t2114
+t2140+t2160+t2297+t2381+(((2.0*t2778+t2745+t2664+t2665+t2779+t2780+t2671+t2672
)*t1440+t2623+t2628+t2762+t2765+t2769+t2771+t2777+t2782)*t1440+t2524+t2532+
t2683+t2690+t2702+t2710+t2759+t2784+((2.0*t2930+t2918+t2892+t2893+t2931+t2932+
t2899+t2900)*t1440+t2851+t2856+t2907+t2910+t2914+t2916+t2929+t2934+(t3041+2.0*
t2977+t2979+t2966+t2967+t2980+t2981+t2973+t2974)*t2982)*t2982)*t2982)*t2982;
    g[2] = ((((2.0*t781+t783+t784+t786+t788+t789)*t792+t747+t752+t759+
t765+t774+t779+t791)*t792+t659+t667+t682+t702+t732+t746+t793)*t792+t459+t470+
t496+t541+t617+t658+t795)*t792+t61+t75+t115+t199+t360+t458+t797+((((2.0*t1218+
t1220+t1221+t1223+t1225+t1227+t1228)*t792+t1179+t1184+t1191+t1200+t1211+t1216+
t1230)*t792+t1080+t1088+t1103+t1127+t1162+t1178+t1232)*t792+t907+t918+t944+t970
+t1038+t1079+t1234+(((t3068+t1362+t1363+t1365+t1367+t1369+t1370)*t792+t1326+
t1331+t1338+t1343+t1353+t1358+t1372)*t792+t1080+t1088+t1298+t1305+t1317+t1325+
t1374+((2.0*t1417+t1362+t1363+t1418+t1419+t1369+t1370)*t792+t1179+t1184+t1406+
t1409+t1413+t1415+t1421+(t1217*t1440+t1220+t1221+t1227+t1228+t1434+t1435+t3068)
*t1440)*t1440)*t1440)*t1440+((((2.0*t2076+t2078+t2079+t2081+t2083+t2085+t2086)*
t792+t2037+t2042+t2049+t2058+t2069+t2074+t2088)*t792+t1938+t1946+t1961+t1985+
t2020+t2036+t2090)*t792+t1719+t1730+t1756+t1806+t1891+t1937+t2092+(((2.0*t2281+
t2283+t2284+t2286+t2288+t2290+t2291)*t792+t2242+t2247+t2254+t2263+t2274+t2279+
t2293)*t792+t2161+t2169+t2184+t2196+t2226+t2241+t2295+((t3097+t2345+t2346+t2348
+t2349+t2351+t2352)*t792+t2242+t2247+t2332+t2335+t2339+t2341+t2354+(t1440*t2280
+t2283+t2284+t2290+t2291+t2369+t2370+t3097)*t1440)*t1440)*t1440+(((2.0*t2662+
t2664+t2665+t2667+t2669+t2671+t2672)*t792+t2623+t2628+t2635+t2644+t2655+t2660+
t2674)*t792+t2524+t2532+t2547+t2571+t2606+t2622+t2676+((2.0*t2745+t2747+t2748+
t2750+t2752+t2754+t2755)*t792+t2711+t2716+t2723+t2728+t2738+t2743+t2757+(t1440*
t2744+t2747+t2748+t2754+t2755+2.0*t2773+t2774+t2775)*t1440)*t1440+((2.0*t2890+
t2892+t2893+t2895+t2897+t2899+t2900)*t792+t2851+t2856+t2863+t2872+t2883+t2888+
t2902+(t1440*t2917+2.0*t2918+t2920+t2921+t2923+t2924+t2926+t2927)*t1440+(t1440*
t2978+2.0*t2964+t2966+t2967+t2969+t2971+t2973+t2974+t3041)*t2982)*t2982)*t2982)
*t2982;
    g[3] = ((((2.0*t50+t22+t7)*t48+t4+t49+t52)*t48+t3+t47+t54)*t48+t2+
t41+t56)*t48+t1+t31+t58+((((t3148+t425+t349+t351+t352)*t48+t327+t332+t339+t345+
t449+t452)*t48+t283+t291+t306+t326+t447+t454)*t48+t200+t211+t237+t282+t436+t456
+(((t3155+t638+t608+t610+t611)*t48+t586+t591+t598+t604+t651+t654)*t48+t542+t550
+t565+t585+t647+t656+((t3160+t734+t725+t727+t728)*t48+t703+t708+t715+t721+t741+
t744+(t3163+t3164+t777+t769+t771+t772)*t792)*t792)*t792)*t792+((((t3148+t425+
t861+t351+t352)*t48+t327+t332+t857+t860+t898+t900)*t48+t283+t291+t848+t855+t896
+t902)*t48+t200+t211+t832+t845+t891+t904+(((2.0*t1073+t1058+t1028+t1029+t1031+
t1032)*t48+t1007+t1012+t1019+t1024+t1072+t1075)*t48+t971+t979+t994+t1006+t1068+
t1077+((t3184+t1164+t1153+t1155+t1157+t1158)*t48+t1128+t1133+t1140+t1149+t1173+
t1176+(t3187+t3188+t1214+t1204+t1206+t1208+t1209)*t792)*t792)*t792+(((t3155+
t638+t1269+t610+t611)*t48+t586+t591+t1265+t1268+t1287+t1289)*t48+t542+t550+
t1257+t1264+t1285+t1291+((t3184+t1164+t1312+t1313+t1157+t1158)*t48+t1128+t1133+
t1308+t1311+t1321+t1323+(t3201+2.0*t1354+t1356+t1347+t1348+t1350+t1351)*t792)*
t792+((t3160+t734+t1392+t727+t728)*t48+t703+t708+t1388+t1391+t1399+t1401+(t3201
+t3188+t1214+t1410+t1411+t1208+t1209)*t792+(t3211+t3187+t3164+t777+t1429+t771+
t772)*t1440)*t1440)*t1440)*t1440+((((2.0*t1710+t1687+t1634+t1635+t1636)*t48+
t1618+t1625+t1630+t1709+t1712)*t48+t1590+t1605+t1617+t1707+t1714)*t48+t1537+
t1563+t1589+t1697+t1716+(((t3227+t1915+t1880+t1882+t1884+t1885)*t48+t1855+t1860
+t1867+t1876+t1930+t1933)*t48+t1807+t1815+t1830+t1854+t1926+t1935+((t3232+t2022
+t2011+t2013+t2015+t2016)*t48+t1986+t1991+t1998+t2007+t2031+t2034+(t3235+t3236+
t2072+t2062+t2064+t2066+t2067)*t792)*t792)*t792+(((t3227+t1915+t2133+t2134+
t1884+t1885)*t48+t1855+t1860+t2129+t2132+t2154+t2156)*t48+t1807+t1815+t2119+
t2126+t2152+t2158+((2.0*t2237+t2228+t2218+t2219+t2221+t2222)*t48+t2197+t2202+
t2209+t2214+t2236+t2239+(t3250+t3251+t2277+t2267+t2269+t2271+t2272)*t792)*t792+
((t3232+t2022+t2316+t2317+t2015+t2016)*t48+t1986+t1991+t2312+t2315+t2325+t2327+
(t3258+t3251+t2277+t2336+t2337+t2271+t2272)*t792+(t3261+t3250+t3236+t2072+t2363
+t2364+t2066+t2067)*t1440)*t1440)*t1440+(((2.0*t2517+t2504+t2481+t2482+t2483)*
t48+t2465+t2472+t2477+t2516+t2519)*t48+t2437+t2452+t2464+t2512+t2521+((t3273+
t2608+t2597+t2599+t2601+t2602)*t48+t2572+t2577+t2584+t2593+t2617+t2620+(t3276+
t3277+t2658+t2648+t2650+t2652+t2653)*t792)*t792+((t3273+t2608+t2697+t2698+t2601
+t2602)*t48+t2572+t2577+t2693+t2696+t2706+t2708+(t3284+2.0*t2739+t2741+t2732+
t2733+t2735+t2736)*t792+(t3288+t3284+t3277+t2658+t2766+t2767+t2652+t2653)*t1440
)*t1440+((2.0*t2846+t2839+t2831+t2832+t2833)*t48+t2815+t2822+t2827+t2845+t2848+
(t3296+t3297+t2886+t2876+t2878+t2880+t2881)*t792+(t3300+t3301+t3297+t2886+t2911
+t2912+t2880+t2881)*t1440+(t3304+t3305+t3306+2.0*t2958+t2960+t2953+t2954+t2955)
*t2982)*t2982)*t2982)*t2982;
    g[4] = ((((2.0*t6+t7)*t8+t4+t9)*t8+t3+t11)*t8+t2+t13)*t8+t1+t15+((((
2.0*t22+t23)*t8+t20+t25)*t8+t19+t27)*t8+t18+t29+(((t3333+t35)*t8+t32+t37)*t8+
t19+t39+((2.0*t43+t35)*t8+t20+t45+(t21*t48+t23+t3333)*t48)*t48)*t48)*t48+((((
t3350+t349+t351+t352)*t8+t327+t332+t339+t345+t354)*t8+t283+t291+t306+t326+t356)
*t8+t200+t211+t237+t282+t358+(((t3357+t427+t429+t430)*t8+t405+t410+t417+t423+
t432)*t8+t361+t369+t384+t404+t434+((t3362+t440+t442+t443)*t8+t405+t410+t417+
t423+t445+(t3365+t3362+t427+t429+t430)*t48)*t48)*t48+(((t3372+t608+t610+t611)*
t8+t586+t591+t598+t604+t613)*t8+t542+t550+t565+t585+t615+((t3377+t640+t642+t643
)*t8+t618+t623+t630+t636+t645+(t3380+t3381+t640+t642+t643)*t48)*t48+((t3386+
t725+t727+t728)*t8+t703+t708+t715+t721+t730+(t3389+t3390+t736+t738+t739)*t48+(
t3163+t3393+t3394+t769+t771+t772)*t792)*t792)*t792)*t792+((((t3350+t861+t351+
t352)*t8+t327+t332+t857+t860+t863)*t8+t283+t291+t848+t855+t865)*t8+t200+t211+
t832+t845+t867+(((t3357+t885+t429+t430)*t8+t405+t410+t881+t884+t887)*t8+t361+
t369+t872+t879+t889+((t3362+t892+t442+t443)*t8+t405+t410+t881+t884+t894+(t3365+
t3362+t885+t429+t430)*t48)*t48)*t48+(((2.0*t1026+t1028+t1029+t1031+t1032)*t8+
t1007+t1012+t1019+t1024+t1034)*t8+t971+t979+t994+t1006+t1036+((2.0*t1058+t1060+
t1061+t1063+t1064)*t8+t1039+t1044+t1051+t1056+t1066+(t1057*t48+t1060+t1061+
t1063+t1064+2.0*t1070)*t48)*t48+((t3435+t1153+t1155+t1157+t1158)*t8+t1128+t1133
+t1140+t1149+t1160+(t3438+t3439+t1166+t1168+t1170+t1171)*t48+(t3187+t3442+t3443
+t1204+t1206+t1208+t1209)*t792)*t792)*t792+(((t3372+t1269+t610+t611)*t8+t586+
t591+t1265+t1268+t1271)*t8+t542+t550+t1257+t1264+t1273+((t3377+t1281+t642+t643)
*t8+t618+t623+t1277+t1280+t1283+(t3380+t3381+t1281+t642+t643)*t48)*t48+((t3435+
t1312+t1313+t1157+t1158)*t8+t1128+t1133+t1308+t1311+t1315+(t3438+t3439+t1318+
t1319+t1170+t1171)*t48+(t1355*t48+2.0*t1345+t1347+t1348+t1350+t1351+t3201)*t792
)*t792+((t3386+t1392+t727+t728)*t8+t703+t708+t1388+t1391+t1394+(t3389+t3390+
t1397+t738+t739)*t48+(t3201+t3442+t3443+t1410+t1411+t1208+t1209)*t792+(t3211+
t3187+t3393+t3394+t1429+t771+t772)*t1440)*t1440)*t1440)*t1440+((((2.0*t1632+
t1634+t1635+t1636)*t8+t1618+t1625+t1630+t1638)*t8+t1590+t1605+t1617+t1640)*t8+
t1537+t1563+t1589+t1642+(((2.0*t1687+t1689+t1690+t1691)*t8+t1673+t1680+t1685+
t1693)*t8+t1645+t1660+t1672+t1695+((t3496+t1701+t1702+t1703)*t8+t1673+t1680+
t1685+t1705+(t1686*t48+t1689+t1690+t1691+t3496)*t48)*t48)*t48+(((t3506+t1880+
t1882+t1884+t1885)*t8+t1855+t1860+t1867+t1876+t1887)*t8+t1807+t1815+t1830+t1854
+t1889+((t3511+t1917+t1919+t1921+t1922)*t8+t1892+t1897+t1904+t1913+t1924+(t3514
+t3515+t1917+t1919+t1921+t1922)*t48)*t48+((t3520+t2011+t2013+t2015+t2016)*t8+
t1986+t1991+t1998+t2007+t2018+(t3523+t3524+t2024+t2026+t2028+t2029)*t48+(t3235+
t3527+t3528+t2062+t2064+t2066+t2067)*t792)*t792)*t792+(((t3506+t2133+t2134+
t1884+t1885)*t8+t1855+t1860+t2129+t2132+t2136)*t8+t1807+t1815+t2119+t2126+t2138
+((t3511+t2147+t2148+t1921+t1922)*t8+t1892+t1897+t2143+t2146+t2150+(t3514+t3515
+t2147+t2148+t1921+t1922)*t48)*t48+((2.0*t2216+t2218+t2219+t2221+t2222)*t8+
t2197+t2202+t2209+t2214+t2224+(t2227*t48+2.0*t2228+t2230+t2231+t2233+t2234)*t48
+(t3250+t3552+t3553+t2267+t2269+t2271+t2272)*t792)*t792+((t3520+t2316+t2317+
t2015+t2016)*t8+t1986+t1991+t2312+t2315+t2319+(t3523+t3524+t2322+t2323+t2028+
t2029)*t48+(t3258+t3552+t3553+t2336+t2337+t2271+t2272)*t792+(t3261+t3250+t3527+
t3528+t2363+t2364+t2066+t2067)*t1440)*t1440)*t1440+(((2.0*t2479+t2481+t2482+
t2483)*t8+t2465+t2472+t2477+t2485)*t8+t2437+t2452+t2464+t2487+((2.0*t2504+t2506
+t2507+t2508)*t8+t2490+t2497+t2502+t2510+(t2503*t48+t2506+t2507+t2508+2.0*t2514
)*t48)*t48+((t3584+t2597+t2599+t2601+t2602)*t8+t2572+t2577+t2584+t2593+t2604+(
t3587+t3588+t2610+t2612+t2614+t2615)*t48+(t3276+t3591+t3592+t2648+t2650+t2652+
t2653)*t792)*t792+((t3584+t2697+t2698+t2601+t2602)*t8+t2572+t2577+t2693+t2696+
t2700+(t3587+t3588+t2703+t2704+t2614+t2615)*t48+(t2740*t48+2.0*t2730+t2732+
t2733+t2735+t2736+t3284)*t792+(t3288+t3284+t3591+t3592+t2766+t2767+t2652+t2653)
*t1440)*t1440+((2.0*t2829+t2831+t2832+t2833)*t8+t2815+t2822+t2827+t2835+(t2838*
t48+2.0*t2839+t2841+t2842+t2843)*t48+(t3296+t3616+t3617+t2876+t2878+t2880+t2881
)*t792+(t3300+t3301+t3616+t3617+t2911+t2912+t2880+t2881)*t1440+(t2959*t48+2.0*
t2951+t2953+t2954+t2955+t3304+t3305+t3306)*t2982)*t2982)*t2982)*t2982;
    g[5] = (((2.0*t193+t180+t187)*t194+t160+t175+t195)*t194+t126+t152+
t197+(t3640+t245+t260+t280+(t344*t8+t311+t318+t3641)*t8)*t8+(t3640+t245+t260+
t280+(2.0*t402+t389+t396+t3648)*t8+(t344*t48+t311+t318+t3641+t3648)*t48)*t48+((
2.0*t537+t524+t531)*t194+t504+t519+t539+(t603*t8+t3659+t570+t577)*t8+(t48*t603+
t635*t8+t3659+t570+t577)*t48+(t48*t720+t720*t8+t764*t792+t687+t694+2.0*t700)*
t792)*t792)*t792+((((2.0*t819+t143+t106+t107)*t194+t97+t102+t818+t821)*t194+t88
+t96+t816+t823)*t194+t76+t87+t812+t825+(t3688+t212+t220+t836+t843+(t3691+t292+
t297+t850+t853+(t3692+t3693+t341+t336+t337)*t8)*t8)*t8+(t3688+t212+t220+t836+
t843+((2.0*t875+t391+t379+t380)*t194+t370+t375+t874+t877+(t3703+t3704+t419+t414
+t415)*t8)*t8+(t3691+t292+t297+t850+t853+(t3709+t3704+t419+t414+t415)*t8+(t3712
+t3703+t3693+t341+t336+t337)*t48)*t48)*t48+(((2.0*t964+t952+t937+t938)*t194+
t928+t933+t963+t966)*t194+t919+t927+t959+t968+(t3726+t980+t985+t1001+t1004+(
t3727+t3728+t1022+t1016+t1017)*t8)*t8+(t3726+t980+t985+t1001+t1004+(t3733+2.0*
t1052+t1054+t1048+t1049)*t8+(t3737+t3733+t3728+t1022+t1016+t1017)*t48)*t48+((
2.0*t1118+t1120+t1122+t1123)*t194+t1104+t1109+t1116+t1125+(t3745+t3746+t1144+
t1146+t1147)*t8+(t3749+t3750+t3746+t1144+t1146+t1147)*t48+(t3753+t3754+t3755+
2.0*t1193+t1195+t1197+t1198)*t792)*t792)*t792+(((2.0*t1248+t512+t489+t490)*t194
+t480+t485+t1247+t1250)*t194+t471+t479+t1245+t1252+(t3770+t551+t556+t1259+t1262
+(t3771+t3772+t600+t595+t596)*t8)*t8+(t3770+t551+t556+t1259+t1262+(t3777+2.0*
t1278+t632+t627+t628)*t8+(t3781+t3777+t3772+t600+t595+t596)*t48)*t48+((2.0*
t1301+t1111+t1098+t1099)*t194+t1089+t1094+t1300+t1303+(t3789+t3790+t1144+t1137+
t1138)*t8+(t3793+t3794+t3790+t1144+t1137+t1138)*t48+(t3797+t3798+t3799+2.0*
t1339+t1341+t1335+t1336)*t792)*t792+((2.0*t1382+t689+t677+t678)*t194+t668+t673+
t1381+t1384+(t3808+t3809+t717+t712+t713)*t8+(t3812+t3813+t3809+t717+t712+t713)*
t48+(t3816+t3817+t3818+2.0*t1407+t1195+t1188+t1189)*t792+(t1440*t785+2.0*t1426+
t3823+t3824+t3825+t756+t757+t761)*t1440)*t1440)*t1440)*t1440+((((2.0*t1528+
t1507+t1481+t1482)*t194+t1472+t1477+t1527+t1530)*t194+t1463+t1471+t1525+t1532)*
t194+t1451+t1462+t1516+t1534+(t3846+t1538+t1546+t1578+t1587+(t3849+t1591+t1596+
t1612+t1615+(t3850+t3851+t1628+t1622+t1623)*t8)*t8)*t8+(t3846+t1538+t1546+t1578
+t1587+((2.0*t1668+t1662+t1655+t1656)*t194+t1646+t1651+t1667+t1670+(t3861+t3862
+t1683+t1677+t1678)*t8)*t8+(t3849+t1591+t1596+t1612+t1615+(t3867+t3862+t1683+
t1677+t1678)*t8+(t3870+t3861+t3851+t1628+t1622+t1623)*t48)*t48)*t48+(((2.0*
t1795+t1797+t1799+t1800)*t194+t1781+t1786+t1793+t1802)*t194+t1757+t1765+t1780+
t1804+(t3884+t1831+t1836+t1843+t1852+(t3885+t3886+t1871+t1873+t1874)*t8)*t8+(
t3884+t1831+t1836+t1843+t1852+(t3891+2.0*t1906+t1908+t1910+t1911)*t8+(t3895+
t3891+t3886+t1871+t1873+t1874)*t48)*t48+((2.0*t1976+t1978+t1980+t1981)*t194+
t1962+t1967+t1974+t1983+(t3903+t3904+t2002+t2004+t2005)*t8+(t3907+t3908+t3904+
t2002+t2004+t2005)*t48+(t2080*t792+2.0*t2051+t2053+t2055+t2056+t3912+t3913)*
t792)*t792)*t792+(((2.0*t2108+t1773+t1749+t1750)*t194+t1740+t1745+t2107+t2110)*
t194+t1731+t1739+t2105+t2112+(t3928+t1816+t1821+t2121+t2124+(t3929+t3930+t1871+
t1864+t1865)*t8)*t8+(t3928+t1816+t1821+t2121+t2124+(t3935+2.0*t2144+t1908+t1901
+t1902)*t8+(t3939+t3935+t3930+t1871+t1864+t1865)*t48)*t48+((2.0*t2192+t2186+
t2179+t2180)*t194+t2170+t2175+t2191+t2194+(t3947+t3948+t2212+t2206+t2207)*t8+(
t3951+t3952+t3948+t2212+t2206+t2207)*t48+(t3955+t3956+t3957+2.0*t2256+t2258+
t2260+t2261)*t792)*t792+((2.0*t2305+t1969+t1956+t1957)*t194+t1947+t1952+t2304+
t2307+(t3966+t3967+t2002+t1995+t1996)*t8+(t3970+t3971+t3967+t2002+t1995+t1996)*
t48+(t3974+t3975+t3976+2.0*t2333+t2258+t2251+t2252)*t792+(t1440*t2082+t2046+
t2047+t2053+2.0*t2360+t3981+t3982+t3983)*t1440)*t1440)*t1440+(((2.0*t2430+t2418
+t2403+t2404)*t194+t2394+t2399+t2429+t2432)*t194+t2385+t2393+t2425+t2434+(t3998
+t2438+t2443+t2459+t2462+(t3999+t4000+t2475+t2469+t2470)*t8)*t8+(t3998+t2438+
t2443+t2459+t2462+(t4005+2.0*t2498+t2500+t2494+t2495)*t8+(t4009+t4005+t4000+
t2475+t2469+t2470)*t48)*t48+((2.0*t2562+t2564+t2566+t2567)*t194+t2548+t2553+
t2560+t2569+(t4017+t4018+t2588+t2590+t2591)*t8+(t4021+t4022+t4018+t2588+t2590+
t2591)*t48+(t2666*t792+2.0*t2637+t2639+t2641+t2642+t4026+t4027)*t792)*t792+((
2.0*t2686+t2555+t2542+t2543)*t194+t2533+t2538+t2685+t2688+(t4036+t4037+t2588+
t2581+t2582)*t8+(t4040+t4041+t4037+t2588+t2581+t2582)*t48+(t4044+t4045+t4046+
2.0*t2724+t2726+t2720+t2721)*t792+(t1440*t2668+t2632+t2633+t2639+2.0*t2763+
t4051+t4052+t4053)*t1440)*t1440+((2.0*t2810+t2804+t2797+t2798)*t194+t2788+t2793
+t2809+t2812+(t4062+t4063+t2825+t2819+t2820)*t8+(t4066+t4067+t4063+t2825+t2819+
t2820)*t48+(t2894*t792+2.0*t2865+t2867+t2869+t2870+t4071+t4072)*t792+(t1440*
t2896+t2860+t2861+t2867+2.0*t2908+t4077+t4078+t4079)*t1440+(t1440*t2970+t2968*
t792+t2941+t2942+2.0*t2945+t2947+t4083+t4086+t4087)*t2982)*t2982)*t2982)*t2982;
    g[6] = ((((2.0*t104+t106+t107)*t101+t97+t102+t109)*t101+t88+t96+t111
)*t101+t76+t87+t113+(((2.0*t143+t145+t146)*t101+t136+t141+t148)*t101+t127+t135+
t150+((t4110+t170+t171)*t101+t161+t166+t173+(t188*t194+t184+t185+t4114)*t194)*
t194)*t194+(t4125+t212+t220+t235+t4134+(t4137+t292+t297+t304+t4141+(t3692+t4142
+t4143+t336+t337)*t8)*t8)*t8+(t4125+t212+t220+t235+t4134+((2.0*t377+t379+t380)*
t101+t370+t375+t382+(t194*t397+2.0*t391+t393+t394)*t194+(t3703+t4157+t4158+t414
+t415)*t8)*t8+(t4137+t292+t297+t304+t4141+(t3709+t4157+t4158+t414+t415)*t8+(
t3712+t3703+t4142+t4143+t336+t337)*t48)*t48)*t48+(((2.0*t487+t489+t490)*t101+
t480+t485+t492)*t101+t471+t479+t494+((2.0*t512+t514+t515)*t101+t505+t510+t517+(
t194*t532+t4180+t528+t529)*t194)*t194+(t4187+t551+t556+t563+t4191+(t3771+t4192+
t4193+t595+t596)*t8)*t8+(t4187+t551+t556+t563+t4191+(t3777+t4198+2.0*t625+t627+
t628)*t8+(t3781+t3777+t4192+t4193+t595+t596)*t48)*t48+((2.0*t675+t677+t678)*
t101+t668+t673+t680+(t194*t695+2.0*t689+t691+t692)*t194+(t3808+t4213+t4214+t712
+t713)*t8+(t3812+t3813+t4213+t4214+t712+t713)*t48+(t785*t792+t3824+t3825+t4220+
2.0*t754+t756+t757)*t792)*t792)*t792)*t792+(((2.0*t800+t180)*t101+t160+t802)*
t101+t126+t804+(((2.0*t189+t184+t185)*t101+t161+t166+t808)*t101+t127+t135+t810+
((t4114+t170+t171)*t101+t136+t141+t814+(t142*t194+t145+t146+t4110)*t194)*t194)*
t194+(t4251+t245+t830+t4259+(t4265*t8+t311+t4260+t4264)*t8)*t8+(t4251+t245+t830
+t4259+(2.0*t870+t389+(t194*t390+t393+t394+2.0*t398)*t194+t4277)*t8+(t4265*t48+
t311+t4260+t4264+t4277)*t48)*t48+(((2.0*t935+t937+t938)*t101+t928+t933+t940)*
t101+t919+t927+t942+((2.0*t952+t954+t955)*t101+t945+t950+t957+(t194*t951+t954+
t955+2.0*t961)*t194)*t194+(t4301+t980+t985+t992+t4305+(t3727+t4306+t4307+t1016+
t1017)*t8)*t8+(t4301+t980+t985+t992+t4305+(t1053*t194+2.0*t1046+t1048+t1049+
t3733)*t8+(t3737+t3733+t4306+t4307+t1016+t1017)*t48)*t48+((2.0*t1096+t1098+
t1099)*t101+t1089+t1094+t1101+(t1119*t194+2.0*t1111+t1113+t1114)*t194+(t3789+
t4327+t4328+t1137+t1138)*t8+(t3793+t3794+t4327+t4328+t1137+t1138)*t48+(t3823+
t3817+t3818+t4333+2.0*t1186+t1188+t1189)*t792)*t792)*t792+((2.0*t1237+t524)*
t101+t504+t1239+((2.0*t533+t528+t529)*t101+t505+t510+t1243+(t194*t511+t4180+
t514+t515)*t194)*t194+(t4357*t8+t4352+t4356+t570)*t8+(t4352+t570+t4356+(t4198+
t634)*t8+t4357*t48)*t48+((2.0*t1294+t1122+t1123)*t101+t1104+t1109+t1296+(t1110*
t194+t1113+t1114+2.0*t1120)*t194+(t3745+t4327+t4373+t1146+t1147)*t8+(t3749+
t3750+t4327+t4373+t1146+t1147)*t48+(t1340*t194+2.0*t1333+t1335+t1336+t3798+
t3799+t3816)*t792)*t792+(2.0*t1377+t687+(t194*t688+t691+t692+2.0*t696)*t194+
t4389*t8+t4389*t48+(t3797+t3754+t3755+t4333+2.0*t1404+t1197+t1198)*t792+(t3753+
t4220+t763)*t1440)*t1440)*t1440)*t1440+((((2.0*t1479+t1481+t1482)*t101+t1472+
t1477+t1484)*t101+t1463+t1471+t1486)*t101+t1451+t1462+t1488+(((2.0*t1507+t1509+
t1510)*t101+t1500+t1505+t1512)*t101+t1491+t1499+t1514+((t4415+t1520+t1521)*t101
+t1500+t1505+t1523+(t1506*t194+t1509+t1510+t4415)*t194)*t194)*t194+(t4429+t1538
+t1546+t1561+t4438+(t4441+t1591+t1596+t1603+t4445+(t3850+t4446+t4447+t1622+
t1623)*t8)*t8)*t8+(t4429+t1538+t1546+t1561+t4438+((2.0*t1653+t1655+t1656)*t101+
t1646+t1651+t1658+(t1661*t194+2.0*t1662+t1664+t1665)*t194+(t3861+t4461+t4462+
t1677+t1678)*t8)*t8+(t4441+t1591+t1596+t1603+t4445+(t3867+t4461+t4462+t1677+
t1678)*t8+(t3870+t3861+t4446+t4447+t1622+t1623)*t48)*t48)*t48+(((2.0*t1747+
t1749+t1750)*t101+t1740+t1745+t1752)*t101+t1731+t1739+t1754+((2.0*t1773+t1775+
t1776)*t101+t1766+t1771+t1778+(t1796*t194+t1790+t1791+t4484)*t194)*t194+(t4491+
t1816+t1821+t1828+t4495+(t3929+t4496+t4497+t1864+t1865)*t8)*t8+(t4491+t1816+
t1821+t1828+t4495+(t3935+t4502+2.0*t1899+t1901+t1902)*t8+(t3939+t3935+t4496+
t4497+t1864+t1865)*t48)*t48+((2.0*t1954+t1956+t1957)*t101+t1947+t1952+t1959+(
t194*t1977+2.0*t1969+t1971+t1972)*t194+(t3966+t4517+t4518+t1995+t1996)*t8+(
t3970+t3971+t4517+t4518+t1995+t1996)*t48+(t2082*t792+2.0*t2044+t2046+t2047+
t3982+t3983+t4524)*t792)*t792)*t792+(((2.0*t2095+t1799+t1800)*t101+t1781+t1786+
t2097)*t101+t1757+t1765+t2099+((2.0*t1797+t1790+t1791)*t101+t1766+t1771+t2103+(
t1772*t194+t1775+t1776+t4484)*t194)*t194+(t4547+t1831+t1836+t2117+t4551+(t3885+
t4496+t4552+t1873+t1874)*t8)*t8+(t4547+t1831+t1836+t2117+t4551+(t3891+t4502+2.0
*t2141+t1910+t1911)*t8+(t3895+t3891+t4496+t4552+t1873+t1874)*t48)*t48+((2.0*
t2177+t2179+t2180)*t101+t2170+t2175+t2182+(t194*t2185+2.0*t2186+t2188+t2189)*
t194+(t3947+t4571+t4572+t2206+t2207)*t8+(t3951+t3952+t4571+t4572+t2206+t2207)*
t48+(t3981+t3975+t3976+t4577+2.0*t2249+t2251+t2252)*t792)*t792+((2.0*t2298+
t1980+t1981)*t101+t1962+t1967+t2300+(t194*t1968+t1971+t1972+2.0*t1978)*t194+(
t3903+t4517+t4590+t2004+t2005)*t8+(t3907+t3908+t4517+t4590+t2004+t2005)*t48+(
t3974+t3956+t3957+t4577+2.0*t2330+t2260+t2261)*t792+(t1440*t2080+t2055+t2056+
2.0*t2357+t3912+t3913+t3955+t4524)*t1440)*t1440)*t1440+(((2.0*t2401+t2403+t2404
)*t101+t2394+t2399+t2406)*t101+t2385+t2393+t2408+((2.0*t2418+t2420+t2421)*t101+
t2411+t2416+t2423+(t194*t2417+t2420+t2421+2.0*t2427)*t194)*t194+(t4622+t2438+
t2443+t2450+t4626+(t3999+t4627+t4628+t2469+t2470)*t8)*t8+(t4622+t2438+t2443+
t2450+t4626+(t194*t2499+2.0*t2492+t2494+t2495+t4005)*t8+(t4009+t4005+t4627+
t4628+t2469+t2470)*t48)*t48+((2.0*t2540+t2542+t2543)*t101+t2533+t2538+t2545+(
t194*t2563+2.0*t2555+t2557+t2558)*t194+(t4036+t4648+t4649+t2581+t2582)*t8+(
t4040+t4041+t4648+t4649+t2581+t2582)*t48+(t2668*t792+2.0*t2630+t2632+t2633+
t4052+t4053+t4655)*t792)*t792+((2.0*t2679+t2566+t2567)*t101+t2548+t2553+t2681+(
t194*t2554+t2557+t2558+2.0*t2564)*t194+(t4017+t4648+t4668+t2590+t2591)*t8+(
t4021+t4022+t4648+t4668+t2590+t2591)*t48+(t194*t2725+2.0*t2718+t2720+t2721+
t4045+t4046+t4051)*t792+(t1440*t2666+t2641+t2642+2.0*t2760+t4026+t4027+t4044+
t4655)*t1440)*t1440+((2.0*t2795+t2797+t2798)*t101+t2788+t2793+t2800+(t194*t2803
+2.0*t2804+t2806+t2807)*t194+(t4062+t4690+t4691+t2819+t2820)*t8+(t4066+t4067+
t4690+t4691+t2819+t2820)*t48+(t2896*t792+2.0*t2858+t2860+t2861+t4078+t4079+
t4697)*t792+(t1440*t2894+t2869+t2870+2.0*t2905+t4071+t4072+t4077+t4697)*t1440+(
t1440*t2968+t194*t2946+t2970*t792+2.0*t2939+t2941+t2942+t4083+t4086+t4087)*
t2982)*t2982)*t2982)*t2982;
    g[7] = (t4724+t62+t73+(t4729+t77+t85+(t4732+t89+t94+(t101*t105+t100+
t4734)*t101)*t101)*t101+(t4745+t116+t124+(t4748+t128+t133+(t4749+t4750+t139)*
t101)*t101+(t4757+t153+t158+(t4758+t4759+t164)*t101+(t190*t194+t178+t4763+t4764
)*t194)*t194)*t194+(t4775+t201+t209+t4784+t4798+(t4801+t284+t289+t4805+t4810+(
t4811+t4812+t4813+t4814+t330)*t8)*t8)*t8+(t4775+t201+t209+t4784+t4798+(t4823+
t362+t367+(t101*t378+t373+t4825)*t101+(t194*t399+t387+t4829+t4830)*t194+(t4833+
t4834+t4835+t4836+t408)*t8)*t8+(t4801+t284+t289+t4805+t4810+(t4841+t4834+t4835+
t4836+t408)*t8+(t4844+t4833+t4812+t4813+t4814+t330)*t48)*t48)*t48+(t4855+t460+
t468+(t4858+t472+t477+(t101*t488+t483+t4860)*t101)*t101+(t4867+t497+t502+(t4868
+t4869+t508)*t101+(t194*t534+t4873+t4874+t522)*t194)*t194+(t4881+t543+t548+
t4885+t4890+(t4891+t4892+t4893+t4894+t589)*t8)*t8+(t4881+t543+t548+t4885+t4890+
(t101*t626+t194*t633+t4899+t4902+t621)*t8+(t4905+t4899+t4892+t4893+t4894+t589)*
t48)*t48+(t4912+t660+t665+(t101*t676+t4914+t671)*t101+(t194*t697+t4918+t4919+
t685)*t194+(t4922+t4923+t4924+t4925+t706)*t8+(t4928+t4929+t4923+t4924+t4925+
t706)*t48+(t101*t755+t194*t762+t787*t792+t4933+t4934+t4937+t750)*t792)*t792)*
t792)*t792+(t4724+t62+t73+(t4745+t116+t124+(t4757+t153+t158+(t101*t190+t178+
t4764)*t101)*t101)*t101+(t4729+t77+t85+(t4748+t128+t133+(t4763+t4759+t164)*t101
)*t101+(t4732+t89+t94+(t4758+t4750+t139)*t101+(t105*t194+t100+t4734+t4749)*t194
)*t194)*t194+(t4775+t201+t209+t4970+t4977+(t4801+t284+t289+t4980+t4983+(t4811+
t4984+t856+t4814+t330)*t8)*t8)*t8+(t4775+t201+t209+t4970+t4977+(t4823+t362+t367
+(t101*t399+t387+t4830)*t101+(t194*t378+t373+t4825+t4829)*t194+(t4833+t4997+
t880+t4836+t408)*t8)*t8+(t4801+t284+t289+t4980+t4983+(t4841+t4997+t880+t4836+
t408)*t8+(t4844+t4833+t4984+t856+t4814+t330)*t48)*t48)*t48+(((2.0*t911+t912)*
t59+t909+t914)*t59+t908+t916+(t5017+t920+t925+(t101*t936+t5019+t931)*t101)*t101
+(t5017+t920+t925+(t5024+2.0*t947+t948)*t101+(t194*t936+t5019+t5024+t931)*t194)
*t194+(t5035+t972+t977+t5039+t5043+(t1030*t8+t1010+t5045+t5046+t5047)*t8)*t8+(
t5035+t972+t977+t5039+t5043+(t101*t1047+t1047*t194+2.0*t1041+t1042+t5052)*t8+(
t1030*t48+t1010+t5045+t5046+t5047+t5052)*t48)*t48+(t5065+t1081+t1086+(t101*
t1097+t1092+t5067)*t101+(t1121*t194+t1107+t5071+t5072)*t194+(t5075+t5076+t5077+
t5078+t1131)*t8+(t5081+t5082+t5076+t5077+t5078+t1131)*t48+(t101*t1187+t1196*
t194+t1182+t5085+t5086+t5087+t5090)*t792)*t792)*t792+(t4855+t460+t468+(t4867+
t497+t502+(t101*t534+t4874+t522)*t101)*t101+(t4858+t472+t477+(t4873+t4869+t508)
*t101+(t194*t488+t483+t4860+t4868)*t194)*t194+(t4881+t543+t548+t5111+t5114+(
t4891+t5115+t5116+t4894+t589)*t8)*t8+(t4881+t543+t548+t5111+t5114+(t194*t626+
t1276+t4899+t4902+t621)*t8+(t4905+t4899+t5115+t5116+t4894+t589)*t48)*t48+(t5065
+t1081+t1086+(t101*t1121+t1107+t5072)*t101+(t1097*t194+t1092+t5067+t5071)*t194+
(t5075+t5134+t5135+t5078+t1131)*t8+(t5081+t5082+t5134+t5135+t5078+t1131)*t48+(
t101*t1334+t1334*t194+t1349*t48+t1349*t8+2.0*t1328+t1329+t5140)*t792)*t792+(
t4912+t660+t665+(t101*t697+t4919+t685)*t101+(t194*t676+t4914+t4918+t671)*t194+(
t4922+t5156+t1387+t4925+t706)*t8+(t4928+t4929+t5156+t1387+t4925+t706)*t48+(t101
*t1196+t1187*t194+t1182+t5086+t5087+t5090+t5140)*t792+(t1440*t787+t194*t755+
t1424+t4933+t4934+t4937+t5085+t750)*t1440)*t1440)*t1440)*t1440+((t5179+t1452+
t1460+(t5182+t1464+t1469+(t101*t1480+t1475+t5184)*t101)*t101)*t101+(t5179+t1452
+t1460+((2.0*t1494+t1495)*t59+t1492+t1497+(t5194+t5195+t1503)*t101)*t101+(t5182
+t1464+t1469+(t101*t1519+t1503+t5195)*t101+(t1480*t194+t1475+t5184+t5194)*t194)
*t194)*t194+(t5218+t5227+(t5238*t8+t5231+t5235)*t8)*t8+(t5218+t5227+((t101*
t1654+t1649+t5245)*t101+(t101*t1663+t1654*t194+t1649+t5245)*t194+t5255)*t8+(t48
*t5238+t5231+t5235+t5255)*t48)*t48+(t5267+t1720+t1728+(t5270+t1732+t1737+(t101*
t1748+t1743+t5272)*t101)*t101+(t5279+t1758+t1763+(t5280+t5281+t1769)*t101+(
t1798*t194+t1784+t5285+t5286)*t194)*t194+(t5293+t1808+t1813+t5297+t5302+(t5303+
t5304+t5305+t5306+t1858)*t8)*t8+(t5293+t1808+t1813+t5297+t5302+(t101*t1900+
t1909*t194+t1895+t5311+t5314)*t8+(t5317+t5311+t5304+t5305+t5306+t1858)*t48)*t48
+(t5324+t1939+t1944+(t101*t1955+t1950+t5326)*t101+(t194*t1979+t1965+t5330+t5331
)*t194+(t5334+t5335+t5336+t5337+t1989)*t8+(t5340+t5341+t5335+t5336+t5337+t1989)
*t48+(t101*t2045+t194*t2054+t2084*t792+t2040+t5345+t5346+t5349)*t792)*t792)*
t792+(t5267+t1720+t1728+(t5279+t1758+t1763+(t101*t1798+t1784+t5286)*t101)*t101+
(t5270+t1732+t1737+(t5285+t5281+t1769)*t101+(t1748*t194+t1743+t5272+t5280)*t194
)*t194+(t5293+t1808+t1813+t5370+t5373+(t5303+t5374+t5375+t5306+t1858)*t8)*t8+(
t5293+t1808+t1813+t5370+t5373+(t101*t1909+t1900*t194+t1895+t5311+t5314)*t8+(
t5317+t5311+t5374+t5375+t5306+t1858)*t48)*t48+((2.0*t2164+t2165)*t59+t2162+
t2167+(t101*t2178+t2173+t5392)*t101+(t101*t2187+t194*t2178+t2173+t5392)*t194+(
t2220*t8+t2200+t5400+t5401+t5402)*t8+(t2220*t48+t2232*t8+t2200+t5400+t5401+
t5402)*t48+(t101*t2250+t194*t2259+t2245+t5409+t5410+t5411+t5414)*t792)*t792+(
t5324+t1939+t1944+(t101*t1979+t1965+t5331)*t101+(t194*t1955+t1950+t5326+t5330)*
t194+(t5334+t5425+t5426+t5337+t1989)*t8+(t5340+t5341+t5425+t5426+t5337+t1989)*
t48+(t101*t2259+t194*t2250+t2350*t792+t2245+t5410+t5411+t5414)*t792+(t101*t2054
+t1440*t2084+t194*t2045+t2040+t5345+t5346+t5349+t5409)*t1440)*t1440)*t1440+((
t5447+t2386+t2391+(t101*t2402+t2397+t5449)*t101)*t101+(t5447+t2386+t2391+(t5454
+2.0*t2413+t2414)*t101+(t194*t2402+t2397+t5449+t5454)*t194)*t194+(t5473*t8+
t5466+t5470)*t8+(t5466+t5470+(t101*t2493+t194*t2493)*t8+t5473*t48)*t48+(t5486+
t2525+t2530+(t101*t2541+t2536+t5488)*t101+(t194*t2565+t2551+t5492+t5493)*t194+(
t5496+t5497+t5498+t5499+t2575)*t8+(t5502+t5503+t5497+t5498+t5499+t2575)*t48+(
t101*t2631+t194*t2640+t2670*t792+t2626+t5507+t5508+t5511)*t792)*t792+(t5486+
t2525+t2530+(t101*t2565+t2551+t5493)*t101+(t194*t2541+t2536+t5488+t5492)*t194+(
t5496+t5522+t5523+t5499+t2575)*t8+(t5502+t5503+t5522+t5523+t5499+t2575)*t48+(
t101*t2719+t194*t2719+t2734*t48+t2734*t8+2.0*t2713+t2714+t5528)*t792+(t101*
t2640+t1440*t2670+t194*t2631+t2626+t5507+t5508+t5511+t5528)*t1440)*t1440+((t101
*t2796+t2791+t5544)*t101+(t101*t2805+t194*t2796+t2791+t5544)*t194+t5553*t8+
t5553*t48+(t101*t2859+t194*t2868+t2898*t792+t2854+t5557+t5558+t5561)*t792+(t101
*t2868+t1440*t2898+t194*t2859+t2925*t792+t2854+t5557+t5558+t5561)*t1440+(t101*
t2940+t1440*t2972+t194*t2940+t2972*t792)*t2982)*t2982)*t2982)*t2982;
    return (t1+t15)*t8+(t1+t31+t58)*t48+(t61+t75+t115+t199+t360+t458+
t797)*t792+(t61+t75+t806+t827+t869+t906+t1236+t1447)*t1440+(t1450+t1490+t1536+
t1644+t1718+t2094+t2383+t3003)*t2982;
}

} // namespace mbnrg_A1_B1C2X2_deg6

