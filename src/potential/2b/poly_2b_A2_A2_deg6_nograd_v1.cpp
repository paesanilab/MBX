
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
 * @file poly_2b_A2_A2_deg6_nograd_v1.cpp
 * @brief Contains the implementation of the polynomials without gradients for symmetry A2_A2
 */

/**
 * @namespace mbnrg_A2_A2_deg6
 * @brief Encloses the structure of the polynomial for symmetry A2_A2
 */

namespace mbnrg_A2_A2_deg6 {

double poly_A2_A2_deg6_v1::eval(const double x[6], const double a[151]) {
    const double t1 = a[0];
    const double t2 = a[4];
    const double t3 = a[9];
    const double t4 = a[35];
    const double t5 = a[143];
    const double t7 = a[56];
    const double t6 = x[5];
    const double t15 = (t2 + (t3 + (t4 + (t5 * t6 + t7) * t6) * t6) * t6) * t6;
    const double t16 = a[3];
    const double t17 = a[8];
    const double t18 = a[18];
    const double t19 = a[112];
    const double t21 = a[64];
    const double t27 = (t17 + (t18 + (t19 * t6 + t21) * t6) * t6) * t6;
    const double t28 = a[10];
    const double t29 = a[15];
    const double t30 = a[115];
    const double t32 = a[55];
    const double t36 = (t29 + (t30 * t6 + t32) * t6) * t6;
    const double t37 = a[27];
    const double t38 = a[138];
    const double t40 = a[53];
    const double t42 = (t38 * t6 + t40) * t6;
    const double t43 = a[84];
    const double t45 = a[121];
    const double t46 = t45 * t6;
    const double t47 = a[67];
    const double t58 = a[2];
    const double t59 = a[12];
    const double t60 = a[22];
    const double t61 = a[96];
    const double t63 = a[65];
    const double t69 = (t59 + (t60 + (t61 * t6 + t63) * t6) * t6) * t6;
    const double t70 = a[5];
    const double t71 = a[28];
    const double t72 = a[114];
    const double t74 = a[57];
    const double t78 = (t71 + (t72 * t6 + t74) * t6) * t6;
    const double t79 = a[29];
    const double t80 = a[129];
    const double t82 = a[54];
    const double t84 = (t80 * t6 + t82) * t6;
    const double t85 = a[126];
    const double t65 = x[4];
    const double t86 = t65 * t85;
    const double t87 = a[90];
    const double t88 = t6 * t87;
    const double t89 = a[70];
    const double t98 = a[33];
    const double t99 = a[101];
    const double t101 = a[43];
    const double t103 = (t99 * t6 + t101) * t6;
    const double t104 = a[78];
    const double t105 = t65 * t104;
    const double t106 = a[125];
    const double t107 = t6 * t106;
    const double t108 = a[62];
    const double t115 = a[91];
    const double t116 = t65 * t115;
    const double t134 = a[6];
    const double t135 = a[25];
    const double t136 = a[131];
    const double t138 = a[44];
    const double t144 = (t134 + (t135 + (t136 * t6 + t138) * t6) * t6) * t6;
    const double t145 = a[31];
    const double t146 = a[142];
    const double t148 = a[68];
    const double t152 = (t145 + (t146 * t6 + t148) * t6) * t6;
    const double t153 = a[147];
    const double t155 = a[58];
    const double t157 = (t153 * t6 + t155) * t6;
    const double t158 = a[87];
    const double t159 = t6 * t158;
    const double t168 = a[1];
    const double t169 = a[13];
    const double t170 = a[19];
    const double t171 = a[132];
    const double t173 = a[39];
    const double t179 = (t169 + (t170 + (t171 * t6 + t173) * t6) * t6) * t6;
    const double t180 = a[11];
    const double t181 = a[16];
    const double t182 = a[128];
    const double t184 = a[46];
    const double t188 = (t181 + (t182 * t6 + t184) * t6) * t6;
    const double t189 = a[26];
    const double t190 = a[137];
    const double t192 = a[49];
    const double t194 = (t190 * t6 + t192) * t6;
    const double t195 = a[86];
    const double t196 = t65 * t195;
    const double t197 = a[97];
    const double t198 = t6 * t197;
    const double t199 = a[48];
    const double t206 = a[7];
    const double t207 = a[32];
    const double t208 = a[146];
    const double t210 = a[40];
    const double t214 = (t207 + (t208 * t6 + t210) * t6) * t6;
    const double t215 = a[36];
    const double t216 = a[113];
    const double t218 = a[41];
    const double t220 = (t216 * t6 + t218) * t6;
    const double t221 = a[89];
    const double t222 = t65 * t221;
    const double t223 = a[123];
    const double t224 = t6 * t223;
    const double t225 = a[42];
    const double t230 = a[24];
    const double t231 = a[127];
    const double t233 = a[51];
    const double t235 = (t231 * t6 + t233) * t6;
    const double t236 = a[92];
    const double t237 = t65 * t236;
    const double t238 = a[139];
    const double t239 = t6 * t238;
    const double t240 = a[47];
    const double t243 = a[124];
    const double t126 = x[3];
    const double t244 = t126 * t243;
    const double t245 = a[98];
    const double t246 = t65 * t245;
    const double t247 = a[95];
    const double t248 = t6 * t247;
    const double t249 = a[59];
    const double t258 = a[109];
    const double t260 = a[60];
    const double t262 = (t258 * t6 + t260) * t6;
    const double t263 = a[136];
    const double t264 = t6 * t263;
    const double t271 = a[140];
    const double t273 = a[63];
    const double t275 = (t271 * t6 + t273) * t6;
    const double t276 = a[135];
    const double t277 = t6 * t276;
    const double t282 = a[34];
    const double t283 = a[104];
    const double t285 = a[66];
    const double t287 = (t283 * t6 + t285) * t6;
    const double t288 = a[149];
    const double t289 = t65 * t288;
    const double t290 = a[110];
    const double t291 = t6 * t290;
    const double t292 = a[76];
    const double t295 = a[148];
    const double t296 = t126 * t295;
    const double t297 = a[105];
    const double t298 = t65 * t297;
    const double t299 = a[107];
    const double t300 = t6 * t299;
    const double t301 = a[77];
    const double t312 = a[81];
    const double t313 = t6 * t312;
    const double t316 = a[119];
    const double t337 = t243 * t65;
    const double t357 = (t222 + t198 + t199) * t65;
    const double t358 = t126 * t85;
    const double t374 = a[134];
    const double t376 = a[38];
    const double t380 = t65 * a[80];
    const double t381 = a[130];
    const double t382 = t6 * t381;
    const double t383 = a[69];
    const double t389 = t65 * a[133];
    const double t391 = (t389 + t382 + t383) * t65;
    const double t392 = t126 * t245;
    const double t403 = t126 * t297;
    const double t405 = t65 * a[116];
    const double t142 = x[2];
    const double t412 = t142 * t85;
    const double t421 = t65 * t295;
    const double t429 = (t298 + t291 + t292) * t65;
    const double t434 = t126 * t104;
    const double t445 = t126 * t236;
    const double t455 = t142 * t104;
    const double t476 = t126 * t221;
    const double t503 = a[20];
    const double t505 = t6 * a[99];
    const double t506 = a[74];
    const double t512 = (a[14] + (t503 + (t505 + t506) * t6) * t6) * t6;
    const double t515 = t6 * a[145];
    const double t516 = a[37];
    const double t520 = (a[21] + (t515 + t516) * t6) * t6;
    const double t522 = t6 * a[100];
    const double t525 = (t522 + a[75]) * t6;
    const double t528 = t6 * a[108];
    const double t537 = a[17];
    const double t539 = t6 * a[141];
    const double t540 = a[71];
    const double t544 = (t537 + (t539 + t540) * t6) * t6;
    const double t546 = t6 * a[85];
    const double t547 = a[50];
    const double t549 = (t546 + t547) * t6;
    const double t550 = t65 * t158;
    const double t552 = t6 * a[106];
    const double t559 = t65 * t263;
    const double t561 = a[111] * t6;
    const double t578 = a[102] * t6;
    const double t579 = a[73];
    const double t583 = (t537 + (t578 + t579) * t6) * t6;
    const double t585 = t6 * a[117];
    const double t587 = (t585 + t547) * t6;
    const double t588 = t65 * t87;
    const double t597 = t6 * a[93];
    const double t598 = a[61];
    const double t602 = (a[23] + (t597 + t598) * t6) * t6;
    const double t604 = t6 * a[144];
    const double t607 = (t604 + a[72]) * t6;
    const double t608 = t65 * t197;
    const double t610 = t6 * a[150];
    const double t616 = t6 * a[79];
    const double t619 = (t616 + a[45]) * t6;
    const double t620 = t65 * t276;
    const double t622 = t6 * a[120];
    const double t625 = t126 * t247;
    const double t626 = t65 * t312;
    const double t628 = a[103] * t6;
    const double t635 = t65 * t106;
    const double t640 = t65 * t223;
    const double t643 = t126 * t299;
    const double t644 = t65 * t290;
    const double t646 = t6 * a[82];
    const double t653 = t65 * t238;
    const double t665 = t65 * t247;
    const double t678 = t126 * t87;
    const double t689 = t65 * t381;
    const double t704 = t142 * t158;
    const double t712 = t65 * t299;
    const double t719 = t126 * t106;
    const double t729 = t142 * t263;
    const double t752 = t6 * a[122];
    const double t757 = (t503 + (t752 + a[52]) * t6) * t6;
    const double t761 = (t6 * a[83] + t516) * t6;
    const double t770 = t6 * a[94];
    const double t772 = (t770 + t579) * t6;
    const double t773 = t65 * t153;
    const double t789 = (t770 + t540) * t6;
    const double t790 = t65 * t80;
    const double t798 = (t6 * a[118] + t598) * t6;
    const double t799 = t65 * t190;
    const double t802 = t126 * t231;
    const double t803 = t65 * t271;
    const double t812 = t65 * t216;
    const double t822 = t65 * t231;
    const double t829 = t126 * t80;
    const double t840 = t142 * t153;
    const double t864 = (t752 + t506) * t6;
    const double t870 = t65 * t146;
    const double t878 = t65 * t72;
    const double t881 = t126 * t208;
    const double t882 = t65 * t182;
    const double t890 = t65 * t208;
    const double t893 = t126 * t72;
    const double t896 = t142 * t146;
    const double t576 = x[1];
    const double t930 = x[0];
    return (
        (t1 + t15 + (t16 + t27 + (t28 + t36 + (t37 + t42 + (t43 * t65 + t46 + t47) * t65) * t65) * t65) * t65) * t65 +
        (t1 + t15 + (t58 + t69 + (t70 + t78 + (t79 + t84 + (t86 + t88 + t89) * t65) * t65) * t65) * t65 +
         (t16 + t27 + (t70 + t78 + (t98 + t103 + (t105 + t107 + t108) * t65) * t65) * t65 +
          (t28 + t36 + (t79 + t84 + (t116 + t107 + t108) * t65) * t65 +
           (t37 + t42 + (t105 + t88 + t89) * t65 + (t43 * t126 + t46 + t47 + t86) * t126) * t126) *
              t126) *
             t126) *
            t126 +
        (t1 + t15 + (t58 + t144 + (t70 + t152 + (t79 + t157 + (t86 + t159 + t89) * t65) * t65) * t65) * t65 +
         (t168 + t179 + (t180 + t188 + (t189 + t194 + (t196 + t198 + t199) * t65) * t65) * t65 +
          (t206 + t214 + (t215 + t220 + (t222 + t224 + t225) * t65) * t65 +
           (t230 + t235 + (t237 + t239 + t240) * t65 + (t244 + t246 + t248 + t249) * t126) * t126) *
              t126) *
             t126 +
         (t16 + t27 + (t70 + t152 + (t98 + t262 + (t105 + t264 + t108) * t65) * t65) * t65 +
          (t206 + t214 + (t215 + t275 + (t222 + t277 + t225) * t65) * t65 +
           (t282 + t287 + (t289 + t291 + t292) * t65 + (t296 + t298 + t300 + t301) * t126) * t126) *
              t126 +
          (t28 + t36 + (t79 + t157 + (t116 + t264 + t108) * t65) * t65 +
           (t230 + t235 + (t237 + t313 + t240) * t65 + (t316 * t126 + t298 + t300 + t301) * t126) * t126 +
           (t37 + t42 + (t105 + t159 + t89) * t65 + (t296 + t246 + t248 + t249) * t126 +
            (t43 * t142 + t244 + t46 + t47 + t86) * t142) *
               t142) *
              t142) *
             t142) *
            t142 +
        (t1 + t15 + (t168 + t179 + (t206 + t214 + (t230 + t235 + (t337 + t248 + t249) * t65) * t65) * t65) * t65 +
         (t58 + t144 + (t180 + t188 + (t215 + t220 + (t246 + t239 + t240) * t65) * t65) * t65 +
          (t70 + t152 + (t189 + t194 + (t237 + t224 + t225) * t65) * t65 +
           (t79 + t157 + t357 + (t358 + t196 + t159 + t89) * t126) * t126) *
              t126) *
             t126 +
         (t58 + t69 + (t180 + t188 + (t215 + t275 + (t246 + t313 + t240) * t65) * t65) * t65 +
          (t180 + t188 + (a[30] + (t374 * t6 + t376) * t6 + (t380 + t382 + t383) * t65) * t65 +
           (t215 + t275 + t391 + (t392 + t380 + t313 + t240) * t126) * t126) *
              t126 +
          (t70 + t78 + (t189 + t194 + (t237 + t277 + t225) * t65) * t65 +
           (t215 + t220 + t391 + (t403 + t405 + t291 + t292) * t126) * t126 +
           (t79 + t84 + t357 + (t403 + t380 + t239 + t240) * t126 + (t412 + t392 + t196 + t88 + t89) * t142) * t142) *
              t142) *
             t142 +
         (t16 + t27 + (t206 + t214 + (t282 + t287 + (t421 + t300 + t301) * t65) * t65) * t65 +
          (t70 + t152 + (t215 + t275 + t429) * t65 +
           (t98 + t262 + (t289 + t277 + t225) * t65 + (t434 + t222 + t264 + t108) * t126) * t126) *
              t126 +
          (t70 + t78 + (t215 + t220 + t429) * t65 +
           (t189 + t194 + (t405 + t382 + t383) * t65 + (t445 + t389 + t277 + t225) * t126) * t126 +
           (t98 + t103 + (t289 + t224 + t225) * t65 + (t288 * t126 + t224 + t225 + t389) * t126 +
            (t455 + t445 + t222 + t107 + t108) * t142) *
               t142) *
              t142 +
          (t28 + t36 + (t230 + t235 + (t316 * t65 + t300 + t301) * t65) * t65 +
           (t79 + t157 + (t298 + t313 + t240) * t65 + (t115 * t126 + t108 + t237 + t264) * t126) * t126 +
           (t79 + t84 + (t298 + t239 + t240) * t65 + (t476 + t380 + t198 + t199) * t126 +
            (t115 * t142 + t107 + t108 + t237 + t476) * t142) *
               t142 +
           (t37 + t42 + (t421 + t248 + t249) * t65 + (t434 + t246 + t159 + t89) * t126 +
            (t195 * t126 + t246 + t455 + t88 + t89) * t142 + (t43 * t576 + t337 + t358 + t412 + t46 + t47) * t576) *
               t576) *
              t576) *
             t576) *
            t576 +
        ((t2 + t512 + (t17 + t520 + (t29 + t525 + (t45 * t65 + t40 + t528) * t65) * t65) * t65) * t65 +
         (t2 + t512 + (t134 + t544 + (t145 + t549 + (t550 + t552 + t155) * t65) * t65) * t65 +
          (t17 + t520 + (t145 + t549 + (t559 + t561 + t260) * t65) * t65 +
           (t29 + t525 + (t559 + t552 + t155) * t65 + (t45 * t126 + t40 + t528 + t550) * t126) * t126) *
              t126) *
             t126 +
         (t2 + t512 + (t59 + t583 + (t71 + t587 + (t588 + t552 + t82) * t65) * t65) * t65 +
          (t169 + t602 + (t181 + t607 + (t608 + t610 + t192) * t65) * t65 +
           (t207 + t619 + (t620 + t622 + t273) * t65 + (t625 + t626 + t628 + t233) * t126) * t126) *
              t126 +
          (t17 + t520 + (t71 + t587 + (t635 + t561 + t101) * t65) * t65 +
           (t207 + t619 + (t640 + t622 + t218) * t65 + (t643 + t644 + t646 + t285) * t126) * t126 +
           (t29 + t525 + (t635 + t552 + t82) * t65 + (t643 + t653 + t628 + t233) * t126 +
            (t45 * t142 + t40 + t528 + t588 + t625) * t142) *
               t142) *
              t142) *
             t142 +
         (t2 + t512 + (t169 + t602 + (t207 + t619 + (t665 + t628 + t233) * t65) * t65) * t65 +
          (t59 + t583 + (t181 + t607 + (t626 + t622 + t273) * t65) * t65 +
           (t71 + t587 + (t620 + t610 + t192) * t65 + (t678 + t608 + t552 + t82) * t126) * t126) *
              t126 +
          (t134 + t544 + (t181 + t607 + (t653 + t622 + t218) * t65) * t65 +
           (t181 + t607 + (a[88] * t6 + t376 + t689) * t65 + (t238 * t126 + t218 + t622 + t689) * t126) * t126 +
           (t145 + t549 + (t640 + t610 + t192) * t65 + (t290 * t126 + t273 + t622 + t689) * t126 +
            (t312 * t126 + t155 + t552 + t608 + t704) * t142) *
               t142) *
              t142 +
          (t17 + t520 + (t207 + t619 + (t712 + t646 + t285) * t65) * t65 +
           (t71 + t587 + (t644 + t622 + t218) * t65 + (t719 + t640 + t561 + t101) * t126) * t126 +
           (t145 + t549 + (t644 + t622 + t273) * t65 + (t223 * t126 + t192 + t610 + t689) * t126 +
            (t276 * t126 + t260 + t561 + t620 + t729) * t142) *
               t142 +
           (t29 + t525 + (t712 + t628 + t233) * t65 + (t719 + t653 + t552 + t82) * t126 +
            (t197 * t126 + t155 + t552 + t626 + t729) * t142 + (t45 * t576 + t40 + t528 + t665 + t678 + t704) * t576) *
               t576) *
              t576) *
             t576 +
         ((t3 + t757 + (t18 + t761 + (t38 * t65 + t32 + t522) * t65) * t65) * t65 +
          (t3 + t757 + (t135 + t772 + (t773 + t585 + t148) * t65) * t65 +
           (t18 + t761 + (t258 * t65 + t148 + t585) * t65 + (t38 * t126 + t32 + t522 + t773) * t126) * t126) *
              t126 +
          (t3 + t757 + (t60 + t789 + (t790 + t546 + t74) * t65) * t65 +
           (t170 + t798 + (t799 + t604 + t184) * t65 + (t802 + t803 + t616 + t210) * t126) * t126 +
           (t18 + t761 + (t99 * t65 + t546 + t74) * t65 + (t283 * t126 + t210 + t616 + t812) * t126 +
            (t38 * t142 + t32 + t522 + t790 + t802) * t142) *
               t142) *
              t142 +
          (t3 + t757 + (t170 + t798 + (t822 + t616 + t210) * t65) * t65 +
           (t60 + t789 + (t803 + t604 + t184) * t65 + (t829 + t799 + t546 + t74) * t126) * t126 +
           (t135 + t772 + (t812 + t604 + t184) * t65 + (t216 * t126 + t374 * t65 + t184 + t604) * t126 +
            (t271 * t126 + t148 + t585 + t799 + t840) * t142) *
               t142 +
           (t18 + t761 + (t283 * t65 + t210 + t616) * t65 + (t99 * t126 + t546 + t74 + t812) * t126 +
            (t190 * t126 + t258 * t142 + t148 + t585 + t803) * t142 +
            (t38 * t576 + t32 + t522 + t822 + t829 + t840) * t576) *
               t576) *
              t576 +
          ((t4 + t864 + (t30 * t65 + t21 + t515) * t65) * t65 +
           (t4 + t864 + (t870 + t578 + t138) * t65 + (t30 * t126 + t21 + t515 + t870) * t126) * t126 +
           (t4 + t864 + (t878 + t539 + t63) * t65 + (t881 + t882 + t597 + t173) * t126 +
            (t30 * t142 + t21 + t515 + t878 + t881) * t142) *
               t142 +
           (t4 + t864 + (t890 + t597 + t173) * t65 + (t893 + t882 + t539 + t63) * t126 +
            (t182 * t126 + t138 + t578 + t882 + t896) * t142 + (t30 * t576 + t21 + t515 + t890 + t893 + t896) * t576) *
               t576 +
           ((t19 * t65 + t505 + t7) * t65 + (t19 * t126 + t136 * t65 + t505 + t7) * t126 +
            (t171 * t126 + t19 * t142 + t61 * t65 + t505 + t7) * t142 +
            (t61 * t126 + t136 * t142 + t171 * t65 + t19 * t576 + t505 + t7) * t576 +
            t5 * (t576 + t65 + t126 + t142) * t930) *
               t930) *
              t930) *
             t930) *
            t930);
}

}  // namespace mbnrg_A2_A2_deg6
