
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

#include "poly_2b_A1B2Z2_C2_deg5_v1.h"

/**
 * @file poly_2b_A1B2Z2_C2_deg5_nograd_v1.cpp
 * @brief Contains the implementation of the polynomials without gradients for symmetry A1B2Z2_C2
 */

/**
 * @namespace mbnrg_A1B2Z2_C2_deg5
 * @brief Encloses the structure of the polynomial for symmetry A1B2Z2_C2
 */

namespace mbnrg_A1B2Z2_C2_deg5 {

double poly_A1B2Z2_C2_deg5_v1::eval(const double x[14], const double a[933]) {
    const double t1 = a[0];
    const double t2 = a[30];
    const double t4 = a[23];
    const double t9 = a[77];
    const double t3 = x[13];
    const double t10 = t3 * t9;
    const double t11 = a[21];
    const double t19 = a[64];
    const double t20 = t3 * t19;
    const double t21 = a[13];
    const double t24 = a[101];
    const double t5 = x[12];
    const double t25 = t5 * t24;
    const double t26 = a[62];
    const double t27 = t3 * t26;
    const double t28 = a[11];
    const double t36 = t3 * t24;
    const double t39 = t5 * t19;
    const double t6 = x[11];
    const double t42 = t6 * t9;
    const double t51 = a[125];
    const double t53 = a[26];
    const double t57 = a[65];
    const double t62 = a[97];
    const double t64 = a[103];
    const double t74 = a[90];
    const double t75 = a[527];
    const double t77 = a[277];
    const double t82 = a[837];
    const double t83 = t3 * t82;
    const double t84 = a[212];
    const double t92 = a[446];
    const double t93 = t3 * t92;
    const double t94 = a[268];
    const double t97 = a[383];
    const double t98 = t5 * t97;
    const double t99 = a[317];
    const double t100 = t3 * t99;
    const double t101 = a[230];
    const double t109 = t3 * t97;
    const double t112 = t5 * t92;
    const double t115 = t6 * t82;
    const double t124 = a[785];
    const double t125 = t3 * t3;
    const double t128 = a[368];
    const double t133 = a[425];
    const double t135 = a[678];
    const double t151 = a[2];
    const double t152 = a[139];
    const double t154 = a[18];
    const double t156 = (t152 * t3 + t154) * t3;
    const double t158 = a[135];
    const double t161 = (t152 * t5 + t158 * t3 + t154) * t5;
    const double t162 = a[54];
    const double t164 = a[59];
    const double t165 = t5 * t164;
    const double t166 = a[84];
    const double t167 = t3 * t166;
    const double t168 = a[5];
    const double t170 = (t162 * t6 + t165 + t167 + t168) * t6;
    const double t172 = a[47];
    const double t174 = t5 * t166;
    const double t175 = t3 * t164;
    const double t17 = x[10];
    const double t177 = (t162 * t17 + t172 * t6 + t168 + t174 + t175) * t17;
    const double t178 = a[81];
    const double t181 = a[71];
    const double t184 = a[6];
    const double t185 = a[93];
    const double t186 = a[445];
    const double t188 = a[171];
    const double t192 = a[362];
    const double t196 = a[646];
    const double t198 = a[575];
    const double t199 = t5 * t198;
    const double t200 = a[802];
    const double t201 = t3 * t200;
    const double t202 = a[188];
    const double t206 = a[314];
    const double t208 = t5 * t200;
    const double t209 = t3 * t198;
    const double t212 = a[673];
    const double t214 = a[863];
    const double t215 = t5 + t3;
    const double t56 = x[9];
    const double t223 =
        (t178 * t17 + t178 * t6 + t181 * t5 + t181 * t3 + t184 +
         (t185 + (t186 * t3 + t188) * t3 + (t186 * t5 + t192 * t3 + t188) * t5 + (t196 * t6 + t199 + t201 + t202) * t6 +
          (t196 * t17 + t206 * t6 + t202 + t208 + t209) * t17 + (t212 * t17 + t212 * t6 + t214 * t215) * t56) *
             t56) *
        t56;
    const double t224 = a[112];
    const double t225 = t224 * t17;
    const double t226 = t224 * t6;
    const double t227 = a[86];
    const double t228 = t227 * t5;
    const double t229 = t227 * t3;
    const double t230 = a[20];
    const double t231 = a[100];
    const double t233 = t56 * a[875];
    const double t234 = a[503];
    const double t237 = a[583];
    const double t240 = a[158];
    const double t244 = (t231 + (t234 * t17 + t234 * t6 + t237 * t3 + t237 * t5 + t233 + t240) * t56) * t56;
    const double t245 = t56 * t56;
    const double t249 = t245 * a[325] + a[98];
    const double t257 = (t162 * t3 + t168) * t3;
    const double t261 = (t162 * t5 + t172 * t3 + t168) * t5;
    const double t264 = (t152 * t6 + t154 + t165 + t167) * t6;
    const double t268 = (t152 * t17 + t158 * t6 + t154 + t174 + t175) * t17;
    const double t295 =
        (t181 * t17 + t181 * t6 + t178 * t5 + t178 * t3 + t184 +
         (t185 + (t196 * t3 + t202) * t3 + (t196 * t5 + t206 * t3 + t202) * t5 + (t186 * t6 + t188 + t199 + t201) * t6 +
          (t186 * t17 + t192 * t6 + t188 + t208 + t209) * t17 + (t214 * t17 + t212 * t215 + t214 * t6) * t56) *
             t56) *
        t56;
    const double t296 = a[123];
    const double t297 = t296 * t17;
    const double t298 = t296 * t6;
    const double t299 = t296 * t5;
    const double t300 = t296 * t3;
    const double t301 = a[16];
    const double t305 = a[286];
    const double t314 = (a[44] + (t305 * t17 + t305 * t3 + t305 * t5 + t305 * t6 + a[707] * t56 + a[280]) * t56) * t56;
    const double t318 = t245 * a[350] + a[119];
    const double t147 = x[8];
    const double t319 = t318 * t147;
    const double t322 = t227 * t17;
    const double t323 = t227 * t6;
    const double t324 = t224 * t5;
    const double t325 = t224 * t3;
    const double t333 = (t231 + (t237 * t17 + t234 * t3 + t234 * t5 + t237 * t6 + t233 + t240) * t56) * t56;
    const double t339 = a[96];
    const double t342 = a[122];
    const double t345 = a[7];
    const double t346 = a[51];
    const double t348 = t56 * a[477];
    const double t349 = a[669];
    const double t352 = a[803];
    const double t355 = a[191];
    const double t363 = t245 * a[576] + a[52];
    const double t364 = t363 * t147;
    const double t367 = a[66];
    const double t368 = t367 * t17;
    const double t369 = t367 * t6;
    const double t370 = t367 * t5;
    const double t371 = t367 * t3;
    const double t372 = a[19];
    const double t376 = a[798];
    const double t385 = (a[148] + (t376 * t17 + t376 * t3 + t376 * t5 + t376 * t6 + a[828] * t56 + a[276]) * t56) * t56;
    const double t389 = t245 * a[476] + a[117];
    const double t390 = t389 * t147;
    const double t394 = t245 * a[324] + a[88];
    const double t207 = x[7];
    const double t395 = t394 * t207;
    const double t403 = t394 * t147;
    const double t418 = t363 * t207;
    const double t210 = x[6];
    const double t422 = t318 * t210;
    const double t430 = a[48];
    const double t432 = a[9];
    const double t436 = a[138];
    const double t441 = a[151];
    const double t443 = a[132];
    const double t453 = a[137];
    const double t454 = a[739];
    const double t456 = a[156];
    const double t461 = a[593];
    const double t462 = t3 * t461;
    const double t463 = a[271];
    const double t471 = a[858];
    const double t472 = t3 * t471;
    const double t473 = a[209];
    const double t476 = a[489];
    const double t477 = t5 * t476;
    const double t478 = a[833];
    const double t479 = t3 * t478;
    const double t480 = a[274];
    const double t488 = t3 * t476;
    const double t491 = t5 * t471;
    const double t494 = t6 * t461;
    const double t503 = a[361];
    const double t506 = a[872];
    const double t511 = a[553];
    const double t513 = a[546];
    const double t527 = a[106];
    const double t528 = t527 * t17;
    const double t529 = t527 * t6;
    const double t530 = a[40];
    const double t531 = t530 * t5;
    const double t532 = t530 * t3;
    const double t533 = a[25];
    const double t534 = a[114];
    const double t535 = a[529];
    const double t537 = a[189];
    const double t541 = a[754];
    const double t545 = a[520];
    const double t547 = a[461];
    const double t548 = t5 * t547;
    const double t549 = a[544];
    const double t550 = t3 * t549;
    const double t551 = a[282];
    const double t555 = a[579];
    const double t557 = t5 * t549;
    const double t558 = t3 * t547;
    const double t561 = a[795];
    const double t563 = a[801];
    const double t569 =
        (t534 + (t535 * t3 + t537) * t3 + (t541 * t3 + t535 * t5 + t537) * t5 + (t545 * t6 + t548 + t550 + t551) * t6 +
         (t545 * t17 + t555 * t6 + t551 + t557 + t558) * t17 + (t563 * t17 + t215 * t561 + t563 * t6) * t56) *
        t56;
    const double t570 = a[128];
    const double t572 = t56 * a[816];
    const double t573 = a[727];
    const double t576 = a[909];
    const double t579 = a[173];
    const double t581 = (t573 * t17 + t576 * t3 + t576 * t5 + t573 * t6 + t572 + t579) * t56;
    const double t583 = a[550] * t56;
    const double t589 = t530 * t17;
    const double t590 = t530 * t6;
    const double t591 = t527 * t5;
    const double t592 = t527 * t3;
    const double t613 =
        (t534 + (t545 * t3 + t551) * t3 + (t555 * t3 + t545 * t5 + t551) * t5 + (t535 * t6 + t537 + t548 + t550) * t6 +
         (t535 * t17 + t541 * t6 + t537 + t557 + t558) * t17 + (t561 * t17 + t215 * t563 + t561 * t6) * t56) *
        t56;
    const double t614 = a[115];
    const double t617 = a[460];
    const double t624 = (t617 * t17 + t617 * t3 + t617 * t5 + a[411] * t56 + t617 * t6 + a[184]) * t56;
    const double t625 = a[555];
    const double t627 = t625 * t147 * t56;
    const double t635 = (t576 * t17 + t573 * t3 + t573 * t5 + t576 * t6 + t572 + t579) * t56;
    const double t641 = a[73];
    const double t643 = t56 * a[608];
    const double t644 = a[488];
    const double t647 = a[825];
    const double t650 = a[220];
    const double t653 = a[426];
    const double t655 = t653 * t147 * t56;
    const double t658 = a[87];
    const double t661 = a[879];
    const double t668 = (t661 * t17 + t661 * t3 + t661 * t5 + a[871] * t56 + t661 * t6 + a[241]) * t56;
    const double t669 = a[340];
    const double t671 = t669 * t147 * t56;
    const double t672 = a[845];
    const double t674 = t672 * t207 * t56;
    const double t683 = t672 * t147 * t56;
    const double t693 = t653 * t207 * t56;
    const double t699 = t625 * t210 * t56;
    const double t707 = a[152];
    const double t708 = a[628];
    const double t710 = a[269];
    const double t715 = a[796];
    const double t716 = t3 * t715;
    const double t717 = a[237];
    const double t725 = a[815];
    const double t726 = t3 * t725;
    const double t727 = a[279];
    const double t730 = a[826];
    const double t731 = t5 * t730;
    const double t732 = a[572];
    const double t733 = t3 * t732;
    const double t734 = a[183];
    const double t742 = t3 * t730;
    const double t745 = t5 * t725;
    const double t748 = t6 * t715;
    const double t757 = a[308];
    const double t760 = a[590];
    const double t765 = a[517];
    const double t767 = a[779];
    const double t779 = a[35];
    const double t780 = a[774];
    const double t782 = a[213];
    const double t784 = (t780 * t3 + t782) * t3;
    const double t786 = a[805];
    const double t789 = (t786 * t3 + t780 * t5 + t782) * t5;
    const double t790 = a[733];
    const double t792 = a[604];
    const double t793 = t5 * t792;
    const double t794 = a[614];
    const double t795 = t3 * t794;
    const double t796 = a[239];
    const double t798 = (t790 * t6 + t793 + t795 + t796) * t6;
    const double t800 = a[738];
    const double t802 = t5 * t794;
    const double t803 = t3 * t792;
    const double t805 = (t790 * t17 + t800 * t6 + t796 + t802 + t803) * t17;
    const double t806 = a[291];
    const double t808 = a[680];
    const double t812 = (t808 * t17 + t215 * t806 + t808 * t6) * t56;
    const double t813 = a[588];
    const double t816 = t56 * a[885];
    const double t817 = a[339];
    const double t818 = t17 * t817;
    const double t819 = t6 * t817;
    const double t820 = a[829];
    const double t821 = t5 * t820;
    const double t822 = t3 * t820;
    const double t823 = a[216];
    const double t830 = (t790 * t3 + t796) * t3;
    const double t834 = (t800 * t3 + t790 * t5 + t796) * t5;
    const double t837 = (t780 * t6 + t782 + t793 + t795) * t6;
    const double t841 = (t780 * t17 + t786 * t6 + t782 + t802 + t803) * t17;
    const double t846 = (t806 * t17 + t215 * t808 + t806 * t6) * t56;
    const double t847 = a[654];
    const double t848 = t147 * t847;
    const double t850 = t56 * a[810];
    const double t851 = a[501];
    const double t852 = t17 * t851;
    const double t853 = t6 * t851;
    const double t854 = t5 * t851;
    const double t855 = t3 * t851;
    const double t856 = a[205];
    const double t860 = t17 * t820;
    const double t861 = t6 * t820;
    const double t862 = t5 * t817;
    const double t863 = t3 * t817;
    const double t868 = a[467];
    const double t869 = t147 * t868;
    const double t871 = t56 * a[714];
    const double t872 = a[642];
    const double t875 = a[624];
    const double t878 = a[164];
    const double t881 = a[751];
    const double t882 = t207 * t881;
    const double t883 = a[491];
    const double t884 = t147 * t883;
    const double t886 = t56 * a[822];
    const double t887 = a[309];
    const double t888 = t17 * t887;
    const double t889 = t6 * t887;
    const double t890 = t5 * t887;
    const double t891 = t3 * t887;
    const double t892 = a[163];
    const double t900 = t147 * t881;
    const double t903 = t207 * t868;
    const double t910 = t210 * t847;
    const double t919 = a[919];
    const double t922 = a[906];
    const double t927 = a[424];
    const double t929 = a[653];
    const double t939 = a[357];
    const double t940 = t939 * t6;
    const double t941 = a[509];
    const double t942 = t941 * t215;
    const double t943 = t939 * t17;
    const double t944 = a[475];
    const double t948 = t939 * t215;
    const double t949 = t941 * t6;
    const double t950 = t941 * t17;
    const double t951 = a[849];
    const double t956 = a[569];
    const double t958 = a[621];
    const double t975 = a[1];
    const double t976 = a[126];
    const double t978 = a[22];
    const double t982 = a[107];
    const double t986 = a[108];
    const double t988 = a[43];
    const double t989 = t5 * t988;
    const double t990 = a[131];
    const double t991 = t3 * t990;
    const double t992 = a[3];
    const double t996 = a[31];
    const double t998 = t5 * t990;
    const double t999 = t3 * t988;
    const double t1002 = a[89];
    const double t1005 = a[149];
    const double t1008 = a[10];
    const double t1009 = a[74];
    const double t1010 = a[295];
    const double t1012 = a[210];
    const double t1016 = a[659];
    const double t1020 = a[652];
    const double t1022 = a[397];
    const double t1023 = t5 * t1022;
    const double t1024 = a[713];
    const double t1025 = t3 * t1024;
    const double t1026 = a[206];
    const double t1030 = a[570];
    const double t1032 = t5 * t1024;
    const double t1033 = t3 * t1022;
    const double t1036 = a[857];
    const double t1038 = a[870];
    const double t1047 = a[127];
    const double t1048 = t1047 * t17;
    const double t1049 = t1047 * t6;
    const double t1050 = a[121];
    const double t1051 = t1050 * t5;
    const double t1052 = t1050 * t3;
    const double t1053 = a[17];
    const double t1054 = a[56];
    const double t1056 = a[551] * t56;
    const double t1057 = a[417];
    const double t1060 = a[712];
    const double t1063 = a[187];
    const double t1067 = (t1054 + (t1057 * t17 + t1057 * t6 + t1060 * t3 + t1060 * t5 + t1056 + t1063) * t56) * t56;
    const double t1071 = t245 * a[932] + a[58];
    const double t1075 = a[67];
    const double t1076 = t1075 * t17;
    const double t1077 = t1075 * t6;
    const double t1078 = a[130];
    const double t1079 = t1078 * t5;
    const double t1080 = t1078 * t3;
    const double t1081 = a[4];
    const double t1082 = a[109];
    const double t1084 = t56 * a[913];
    const double t1085 = a[479];
    const double t1088 = a[305];
    const double t1091 = a[256];
    const double t1095 = (t1082 + (t1085 * t17 + t1085 * t6 + t1088 * t3 + t1088 * t5 + t1084 + t1091) * t56) * t56;
    const double t1099 = t245 * a[422] + a[78];
    const double t1100 = t1099 * t147;
    const double t1104 = t245 * a[657] + a[145];
    const double t1111 = t245 * a[355] + a[72];
    const double t1116 = t245 * a[302] + a[82];
    const double t1117 = t1116 * t207;
    const double t1121 = t1116 * t147;
    const double t1125 = t245 * a[586] + a[141];
    const double t1127 = t1099 * t210;
    const double t1131 = a[61];
    const double t1134 = a[94];
    const double t1137 = a[28];
    const double t1138 = a[133];
    const double t1139 = a[631];
    const double t1141 = a[181];
    const double t1145 = a[873];
    const double t1149 = a[370];
    const double t1151 = a[447];
    const double t1152 = t5 * t1151;
    const double t1153 = a[390];
    const double t1154 = t3 * t1153;
    const double t1155 = a[207];
    const double t1159 = a[842];
    const double t1161 = t5 * t1153;
    const double t1162 = t3 * t1151;
    const double t1165 = a[352];
    const double t1167 = a[454];
    const double t1174 = a[41];
    const double t1176 = t56 * a[303];
    const double t1177 = a[866];
    const double t1180 = a[781];
    const double t1183 = a[229];
    const double t1185 = (t1177 * t17 + t1177 * t6 + t1180 * t3 + t1180 * t5 + t1176 + t1183) * t56;
    const double t1187 = a[458] * t56;
    const double t1191 = a[42];
    const double t1193 = t56 * a[377];
    const double t1194 = a[574];
    const double t1197 = a[470];
    const double t1200 = a[259];
    const double t1202 = (t1194 * t17 + t1194 * t6 + t1197 * t3 + t1197 * t5 + t1193 + t1200) * t56;
    const double t1203 = a[345];
    const double t1205 = t1203 * t147 * t56;
    const double t1207 = t56 * a[595];
    const double t1211 = a[784];
    const double t1214 = a[886];
    const double t1216 = t1214 * t207 * t56;
    const double t1221 = t1214 * t147 * t56;
    const double t1222 = a[756];
    const double t1226 = t1203 * t210 * t56;
    const double t1230 = a[57];
    const double t1231 = a[360];
    const double t1233 = a[179];
    const double t1237 = a[519];
    const double t1241 = a[876];
    const double t1243 = a[552];
    const double t1244 = t5 * t1243;
    const double t1245 = a[432];
    const double t1246 = t3 * t1245;
    const double t1247 = a[185];
    const double t1251 = a[331];
    const double t1253 = t5 * t1245;
    const double t1254 = t3 * t1243;
    const double t1257 = a[800];
    const double t1259 = a[297];
    const double t1264 = a[335];
    const double t1267 = t56 * a[890];
    const double t1268 = a[844];
    const double t1269 = t17 * t1268;
    const double t1270 = t6 * t1268;
    const double t1271 = a[473];
    const double t1272 = t5 * t1271;
    const double t1273 = t3 * t1271;
    const double t1274 = a[261];
    const double t1277 = a[566];
    const double t1279 = a[718];
    const double t1280 = t147 * t1279;
    const double t1282 = t56 * a[315];
    const double t1283 = a[452];
    const double t1284 = t17 * t1283;
    const double t1285 = t6 * t1283;
    const double t1286 = a[299];
    const double t1287 = t5 * t1286;
    const double t1288 = t3 * t1286;
    const double t1289 = a[186];
    const double t1293 = a[807];
    const double t1294 = t207 * t1293;
    const double t1295 = a[639];
    const double t1300 = t210 * t1279;
    const double t1301 = a[670];
    const double t1303 = t147 * t1293;
    const double t1306 = a[717];
    const double t1308 = a[923];
    const double t1311 = a[353];
    const double t1313 = a[878];
    const double t485 = x[5];
    const double t501 = x[4];
    const double t1319 =
        t1230 + (t1231 * t3 + t1233) * t3 + (t1231 * t5 + t1237 * t3 + t1233) * t5 +
        (t1241 * t6 + t1244 + t1246 + t1247) * t6 + (t1241 * t17 + t1251 * t6 + t1247 + t1253 + t1254) * t17 +
        (t1257 * t17 + t1257 * t6 + t1259 * t215) * t56 +
        (t1264 * t147 + t1267 + t1269 + t1270 + t1272 + t1273 + t1274) * t147 +
        (t1277 * t207 + t1280 + t1282 + t1284 + t1285 + t1287 + t1288 + t1289) * t207 +
        (t1264 * t210 + t1295 * t147 + t1267 + t1269 + t1270 + t1272 + t1273 + t1274 + t1294) * t210 +
        (t1277 * t485 + t1301 * t207 + t1282 + t1284 + t1285 + t1287 + t1288 + t1289 + t1300 + t1303) * t485 +
        (t1306 * t17 + t1306 * t6 + t1308 * t215 + t1311 * t147 + t1311 * t210 + t1313 * t207 + t1313 * t485) * t501;
    const double t1321 =
        t1131 * t17 + t1131 * t6 + t1134 * t5 + t1134 * t3 + t1137 +
        (t1138 + (t1139 * t3 + t1141) * t3 + (t1139 * t5 + t1145 * t3 + t1141) * t5 +
         (t1149 * t6 + t1152 + t1154 + t1155) * t6 + (t1149 * t17 + t1159 * t6 + t1155 + t1161 + t1162) * t17 +
         (t1165 * t215 + t1167 * t17 + t1167 * t6) * t56) *
            t56 +
        (t1187 * t147 + t1174 + t1185) * t147 + (t1207 * t207 + t1191 + t1202 + t1205) * t207 +
        (t1211 * t147 * t56 + t1187 * t210 + t1174 + t1185 + t1216) * t210 +
        (t1222 * t207 * t56 + t1207 * t485 + t1191 + t1202 + t1221 + t1226) * t485 + t1319 * t501;
    const double t1323 = a[46];
    const double t1326 = a[118];
    const double t1329 = a[15];
    const double t1330 = a[124];
    const double t1332 = t56 * a[721];
    const double t1333 = a[682];
    const double t1336 = a[660];
    const double t1339 = a[233];
    const double t1347 = t245 * a[285] + a[92];
    const double t1352 = t245 * a[322] + a[37];
    const double t1356 = a[95];
    const double t1358 = t56 * a[753];
    const double t1359 = a[637];
    const double t1362 = a[778];
    const double t1365 = a[215];
    const double t1368 = a[384];
    const double t1371 = a[671];
    const double t1374 = t1368 * t56;
    const double t1376 = t1371 * t56;
    const double t1379 = t501 * a[316];
    const double t1380 = a[907];
    const double t1382 = a[640];
    const double t1387 = t56 * a[312];
    const double t1388 = a[915];
    const double t1391 = a[440];
    const double t1394 = a[253];
    const double t1395 = t1380 * t207 + t1380 * t485 + t1382 * t147 + t1382 * t210 + t1388 * t17 + t1388 * t6 +
                         t1391 * t3 + t1391 * t5 + t1379 + t1387 + t1394;
    const double t1408 = a[559] * t245 + a[80] + (a[867] * t501 + t56 * a[722]) * t501;
    const double t654 = x[3];
    const double t1410 = t1323 * t17 + t1323 * t6 + t1326 * t5 + t1326 * t3 + t1329 +
                         (t1330 + (t1333 * t17 + t1333 * t6 + t1336 * t3 + t1336 * t5 + t1332 + t1339) * t56) * t56 +
                         t1347 * t147 + t1352 * t207 + t1347 * t210 + t1352 * t485 +
                         (t1356 + (t1359 * t17 + t1359 * t6 + t1362 * t3 + t1362 * t5 + t1358 + t1365) * t56 +
                          t1368 * t147 * t56 + t1371 * t207 * t56 + t1374 * t210 + t1376 * t485 + t1395 * t501) *
                             t501 +
                         t1408 * t654;
    const double t1412 =
        t975 + (t976 * t3 + t978) * t3 + (t982 * t3 + t976 * t5 + t978) * t5 + (t986 * t6 + t989 + t991 + t992) * t6 +
        (t986 * t17 + t996 * t6 + t992 + t998 + t999) * t17 +
        (t1002 * t17 + t1002 * t6 + t1005 * t5 + t1005 * t3 + t1008 +
         (t1009 + (t1010 * t3 + t1012) * t3 + (t1010 * t5 + t1016 * t3 + t1012) * t5 +
          (t1020 * t6 + t1023 + t1025 + t1026) * t6 + (t1020 * t17 + t1030 * t6 + t1026 + t1032 + t1033) * t17 +
          (t1036 * t215 + t1038 * t17 + t1038 * t6) * t56) *
             t56) *
            t56 +
        (t1071 * t147 + t1048 + t1049 + t1051 + t1052 + t1053 + t1067) * t147 +
        (t1104 * t207 + t1076 + t1077 + t1079 + t1080 + t1081 + t1095 + t1100) * t207 +
        (t1071 * t210 + t1111 * t147 + t1048 + t1049 + t1051 + t1052 + t1053 + t1067 + t1117) * t210 +
        (t1104 * t485 + t1125 * t207 + t1076 + t1077 + t1079 + t1080 + t1081 + t1095 + t1121 + t1127) * t485 +
        t1321 * t501 + t1410 * t654;
    const double t1455 = t1078 * t17;
    const double t1456 = t1078 * t6;
    const double t1457 = t1075 * t5;
    const double t1458 = t1075 * t3;
    const double t1466 = (t1082 + (t1085 * t3 + t1085 * t5 + t1088 * t17 + t1088 * t6 + t1084 + t1091) * t56) * t56;
    const double t1470 = t1050 * t17;
    const double t1471 = t1050 * t6;
    const double t1472 = t1047 * t5;
    const double t1473 = t1047 * t3;
    const double t1481 = (t1054 + (t1057 * t3 + t1057 * t5 + t1060 * t17 + t1060 * t6 + t1056 + t1063) * t56) * t56;
    const double t1523 = (t1194 * t3 + t1194 * t5 + t1197 * t17 + t1197 * t6 + t1193 + t1200) * t56;
    const double t1532 = (t1177 * t3 + t1177 * t5 + t1180 * t17 + t1180 * t6 + t1176 + t1183) * t56;
    const double t1566 = t17 * t1286;
    const double t1567 = t6 * t1286;
    const double t1568 = t5 * t1283;
    const double t1569 = t3 * t1283;
    const double t1573 = t17 * t1271;
    const double t1574 = t6 * t1271;
    const double t1575 = t5 * t1268;
    const double t1576 = t3 * t1268;
    const double t1596 =
        t1230 + (t1241 * t3 + t1247) * t3 + (t1241 * t5 + t1251 * t3 + t1247) * t5 +
        (t1231 * t6 + t1233 + t1244 + t1246) * t6 + (t1231 * t17 + t1237 * t6 + t1233 + t1253 + t1254) * t17 +
        (t1257 * t215 + t1259 * t17 + t1259 * t6) * t56 +
        (t1277 * t147 + t1282 + t1289 + t1566 + t1567 + t1568 + t1569) * t147 +
        (t1264 * t207 + t1267 + t1274 + t1280 + t1573 + t1574 + t1575 + t1576) * t207 +
        (t1277 * t210 + t1301 * t147 + t1282 + t1289 + t1294 + t1566 + t1567 + t1568 + t1569) * t210 +
        (t1264 * t485 + t1295 * t207 + t1267 + t1274 + t1300 + t1303 + t1573 + t1574 + t1575 + t1576) * t485 +
        (t1306 * t215 + t1308 * t17 + t1308 * t6 + t1311 * t207 + t1311 * t485 + t1313 * t147 + t1313 * t210) * t501;
    const double t1598 =
        t1134 * t17 + t1134 * t6 + t1131 * t5 + t1131 * t3 + t1137 +
        (t1138 + (t1149 * t3 + t1155) * t3 + (t1149 * t5 + t1159 * t3 + t1155) * t5 +
         (t1139 * t6 + t1141 + t1152 + t1154) * t6 + (t1139 * t17 + t1145 * t6 + t1141 + t1161 + t1162) * t17 +
         (t1165 * t17 + t1165 * t6 + t1167 * t215) * t56) *
            t56 +
        (t1207 * t147 + t1191 + t1523) * t147 + (t1187 * t207 + t1174 + t1205 + t1532) * t207 +
        (t1222 * t147 * t56 + t1207 * t210 + t1191 + t1216 + t1523) * t210 +
        (t1211 * t207 * t56 + t1187 * t485 + t1174 + t1221 + t1226 + t1532) * t485 + t1596 * t501;
    const double t1600 = a[116];
    const double t1609 = a[869];
    const double t1622 = t245 * a[775] + a[136];
    const double t1630 = a[592];
    const double t1638 = a[666];
    const double t1641 = t1638 * t56;
    const double t1647 = a[531];
    const double t1654 = a[414];
    const double t1660 = t1647 * t147 + t1647 * t207 + t1647 * t210 + t1647 * t485 + t1654 * t17 + t1654 * t3 +
                         t1654 * t5 + t1654 * t6 + a[831] * t501 + a[545] * t56 + a[178];
    const double t1674 = (a[745] * t245 + a[99] + (a[427] * t501 + a[498] * t56) * t501) * t654;
    const double t1675 =
        t1600 * t17 + t1600 * t6 + t1600 * t5 + t1600 * t3 + a[14] +
        (a[134] + (t1609 * t17 + t1609 * t3 + t1609 * t5 + t1609 * t6 + a[820] * t56 + a[242]) * t56) * t56 +
        t1622 * t147 + t1622 * t207 + t1622 * t210 + t1622 * t485 +
        (a[105] + (t1630 * t17 + t1630 * t3 + t1630 * t5 + t1630 * t6 + a[578] * t56 + a[217]) * t56 +
         t1638 * t147 * t56 + t1641 * t207 + t1641 * t210 + t1641 * t485 + t1660 * t501) *
            t501 +
        t1674;
    const double t1713 = t1380 * t147 + t1380 * t210 + t1382 * t207 + t1382 * t485 + t1388 * t3 + t1388 * t5 +
                         t1391 * t17 + t1391 * t6 + t1379 + t1387 + t1394;
    const double t1098 = x[2];
    const double t1718 = t1326 * t17 + t1326 * t6 + t1323 * t5 + t1323 * t3 + t1329 +
                         (t1330 + (t1333 * t3 + t1333 * t5 + t1336 * t17 + t1336 * t6 + t1332 + t1339) * t56) * t56 +
                         t1352 * t147 + t1347 * t207 + t1352 * t210 + t1347 * t485 +
                         (t1356 + (t1359 * t3 + t1359 * t5 + t1362 * t17 + t1362 * t6 + t1358 + t1365) * t56 +
                          t1371 * t147 * t56 + t1368 * t207 * t56 + t1376 * t210 + t1374 * t485 + t1713 * t501) *
                             t501 +
                         t1674 + t1408 * t1098;
    const double t1720 =
        t975 + (t986 * t3 + t992) * t3 + (t996 * t3 + t986 * t5 + t992) * t5 + (t976 * t6 + t978 + t989 + t991) * t6 +
        (t976 * t17 + t982 * t6 + t978 + t998 + t999) * t17 +
        (t1005 * t17 + t1005 * t6 + t1002 * t5 + t1002 * t3 + t1008 +
         (t1009 + (t1020 * t3 + t1026) * t3 + (t1020 * t5 + t1030 * t3 + t1026) * t5 +
          (t1010 * t6 + t1012 + t1023 + t1025) * t6 + (t1010 * t17 + t1016 * t6 + t1012 + t1032 + t1033) * t17 +
          (t1036 * t17 + t1036 * t6 + t1038 * t215) * t56) *
             t56) *
            t56 +
        (t1104 * t147 + t1081 + t1455 + t1456 + t1457 + t1458 + t1466) * t147 +
        (t1071 * t207 + t1053 + t1100 + t1470 + t1471 + t1472 + t1473 + t1481) * t207 +
        (t1104 * t210 + t1125 * t147 + t1081 + t1117 + t1455 + t1456 + t1457 + t1458 + t1466) * t210 +
        (t1071 * t485 + t1111 * t207 + t1053 + t1121 + t1127 + t1470 + t1471 + t1472 + t1473 + t1481) * t485 +
        t1598 * t501 + t1675 * t654 + t1718 * t1098;
    const double t1722 = a[85];
    const double t1724 = a[8];
    const double t1726 = (t1722 * t3 + t1724) * t3;
    const double t1728 = a[120];
    const double t1731 = (t1722 * t5 + t1728 * t3 + t1724) * t5;
    const double t1733 = a[49];
    const double t1735 = a[38];
    const double t1738 = (t1722 * t6 + t1733 * t5 + t1735 * t3 + t1724) * t6;
    const double t1744 = (t1722 * t17 + t1728 * t6 + t1733 * t3 + t1735 * t5 + t1724) * t17;
    const double t1745 = a[111];
    const double t1746 = a[681];
    const double t1748 = a[166];
    const double t1753 = a[663];
    const double t1754 = t3 * t1753;
    const double t1755 = a[224];
    const double t1763 = a[764];
    const double t1764 = t3 * t1763;
    const double t1765 = a[169];
    const double t1768 = a[752];
    const double t1769 = t5 * t1768;
    const double t1770 = a[562];
    const double t1771 = t3 * t1770;
    const double t1772 = a[168];
    const double t1780 = t3 * t1768;
    const double t1783 = t5 * t1763;
    const double t1786 = t6 * t1753;
    const double t1795 = a[441];
    const double t1798 = a[563];
    const double t1803 = a[823];
    const double t1805 = a[917];
    const double t1818 =
        ((t1745 + (t1746 * t3 + t1748) * t3) * t3 +
         (t1745 + (t1754 + t1755) * t3 + (t1746 * t5 + t1748 + t1754) * t5) * t5 +
         (t1745 + (t1764 + t1765) * t3 + (t1769 + t1771 + t1772) * t5 + (t1746 * t6 + t1748 + t1764 + t1769) * t6) *
             t6 +
         (t1745 + (t1780 + t1772) * t3 + (t1783 + t1771 + t1765) * t5 + (t1770 * t5 + t1755 + t1771 + t1786) * t6 +
          (t1746 * t17 + t1748 + t1780 + t1783 + t1786) * t17) *
             t17 +
         (t1795 * t125 + (t1795 * t5 + t1798 * t3) * t5 + (t1795 * t6 + t1803 * t5 + t1805 * t3) * t6 +
          (t1795 * t17 + t1798 * t6 + t1803 * t3 + t1805 * t5) * t17) *
             t56) *
        t56;
    const double t1819 = a[143];
    const double t1820 = t1819 * t17;
    const double t1821 = t1819 * t6;
    const double t1822 = a[140];
    const double t1823 = t1822 * t5;
    const double t1824 = t1822 * t3;
    const double t1825 = a[24];
    const double t1826 = a[110];
    const double t1827 = a[379];
    const double t1829 = a[176];
    const double t1833 = a[506];
    const double t1837 = a[725];
    const double t1839 = a[607];
    const double t1840 = t5 * t1839;
    const double t1841 = a[433];
    const double t1842 = t3 * t1841;
    const double t1843 = a[177];
    const double t1847 = a[901];
    const double t1849 = t5 * t1841;
    const double t1850 = t3 * t1839;
    const double t1853 = a[448];
    const double t1855 = a[656];
    const double t1861 =
        (t1826 + (t1827 * t3 + t1829) * t3 + (t1827 * t5 + t1833 * t3 + t1829) * t5 +
         (t1837 * t6 + t1840 + t1842 + t1843) * t6 + (t1837 * t17 + t1847 * t6 + t1843 + t1849 + t1850) * t17 +
         (t1855 * t17 + t1853 * t215 + t1855 * t6) * t56) *
        t56;
    const double t1862 = a[29];
    const double t1864 = t56 * a[651];
    const double t1865 = a[758];
    const double t1868 = a[530];
    const double t1871 = a[195];
    const double t1873 = (t1865 * t17 + t1865 * t6 + t1868 * t3 + t1868 * t5 + t1864 + t1871) * t56;
    const double t1875 = a[743] * t56;
    const double t1881 = t1822 * t17;
    const double t1882 = t1822 * t6;
    const double t1883 = t1819 * t5;
    const double t1884 = t1819 * t3;
    const double t1905 =
        (t1826 + (t1837 * t3 + t1843) * t3 + (t1837 * t5 + t1847 * t3 + t1843) * t5 +
         (t1827 * t6 + t1829 + t1840 + t1842) * t6 + (t1827 * t17 + t1833 * t6 + t1829 + t1849 + t1850) * t17 +
         (t1853 * t17 + t1853 * t6 + t1855 * t215) * t56) *
        t56;
    const double t1906 = a[102];
    const double t1909 = a[928];
    const double t1916 = (t1909 * t17 + t1909 * t3 + t1909 * t5 + t1909 * t6 + a[496] * t56 + a[174]) * t56;
    const double t1917 = a[740];
    const double t1919 = t1917 * t147 * t56;
    const double t1927 = (t1868 * t17 + t1865 * t3 + t1865 * t5 + t1868 * t6 + t1864 + t1871) * t56;
    const double t1933 = a[129];
    const double t1934 = t1933 * t17;
    const double t1935 = t1933 * t6;
    const double t1936 = a[70];
    const double t1937 = t1936 * t5;
    const double t1938 = t1936 * t3;
    const double t1939 = a[27];
    const double t1940 = a[60];
    const double t1941 = a[703];
    const double t1943 = a[198];
    const double t1947 = a[418];
    const double t1951 = a[635];
    const double t1953 = a[329];
    const double t1954 = t5 * t1953;
    const double t1955 = a[298];
    const double t1956 = t3 * t1955;
    const double t1957 = a[273];
    const double t1961 = a[581];
    const double t1963 = t5 * t1955;
    const double t1964 = t3 * t1953;
    const double t1967 = a[453];
    const double t1969 = a[868];
    const double t1975 =
        (t1940 + (t1941 * t3 + t1943) * t3 + (t1941 * t5 + t1947 * t3 + t1943) * t5 +
         (t1951 * t6 + t1954 + t1956 + t1957) * t6 + (t1951 * t17 + t1961 * t6 + t1957 + t1963 + t1964) * t17 +
         (t1969 * t17 + t1967 * t215 + t1969 * t6) * t56) *
        t56;
    const double t1976 = a[147];
    const double t1978 = t56 * a[859];
    const double t1979 = a[925];
    const double t1982 = a[679];
    const double t1985 = a[281];
    const double t1987 = (t1979 * t17 + t1979 * t6 + t1982 * t3 + t1982 * t5 + t1978 + t1985) * t56;
    const double t1989 = a[770] * t56;
    const double t1990 = t1989 * t147;
    const double t1993 = a[79];
    const double t1995 = t56 * a[746];
    const double t1996 = a[404];
    const double t1999 = a[821];
    const double t2002 = a[167];
    const double t2004 = (t1996 * t17 + t1996 * t6 + t1999 * t3 + t1999 * t5 + t1995 + t2002) * t56;
    const double t2005 = a[407];
    const double t2007 = t2005 * t147 * t56;
    const double t2009 = a[692] * t56;
    const double t2010 = t2009 * t207;
    const double t2013 = a[32];
    const double t2015 = t56 * a[736];
    const double t2016 = a[755];
    const double t2019 = a[648];
    const double t2022 = a[247];
    const double t2024 = (t2016 * t17 + t2016 * t6 + t2019 * t3 + t2019 * t5 + t2015 + t2022) * t56;
    const double t2025 = a[689];
    const double t2027 = t2025 * t147 * t56;
    const double t2028 = a[505];
    const double t2030 = t2028 * t207 * t56;
    const double t2032 = a[916] * t56;
    const double t2038 = t1936 * t17;
    const double t2039 = t1936 * t6;
    const double t2040 = t1933 * t5;
    const double t2041 = t1933 * t3;
    const double t2062 =
        (t1940 + (t1951 * t3 + t1957) * t3 + (t1951 * t5 + t1961 * t3 + t1957) * t5 +
         (t1941 * t6 + t1943 + t1954 + t1956) * t6 + (t1941 * t17 + t1947 * t6 + t1943 + t1963 + t1964) * t17 +
         (t1967 * t17 + t1967 * t6 + t1969 * t215) * t56) *
        t56;
    const double t2068 = (t1999 * t17 + t1996 * t3 + t1996 * t5 + t1999 * t6 + t1995 + t2002) * t56;
    const double t2069 = t2009 * t147;
    const double t2077 = (t1982 * t17 + t1979 * t3 + t1979 * t5 + t1982 * t6 + t1978 + t1985) * t56;
    const double t2078 = t1989 * t207;
    const double t2081 = a[91];
    const double t2084 = a[702];
    const double t2091 = (t2084 * t17 + t2084 * t3 + t2084 * t5 + t2084 * t6 + a[759] * t56 + a[244]) * t56;
    const double t2092 = a[914];
    const double t2094 = t2092 * t147 * t56;
    const double t2097 = a[394];
    const double t2099 = t2097 * t210 * t56;
    const double t2107 = (t2019 * t17 + t2016 * t3 + t2016 * t5 + t2019 * t6 + t2015 + t2022) * t56;
    const double t2109 = t2028 * t147 * t56;
    const double t2111 = t2025 * t207 * t56;
    const double t2117 = a[144];
    const double t2118 = a[644];
    const double t2120 = a[262];
    const double t2124 = (t2117 + (t2118 * t3 + t2120) * t3) * t3;
    const double t2125 = a[600];
    const double t2126 = t3 * t2125;
    const double t2127 = a[270];
    const double t2134 = (t2117 + (t2126 + t2127) * t3 + (t2118 * t5 + t2120 + t2126) * t5) * t5;
    const double t2135 = a[847];
    const double t2136 = t3 * t2135;
    const double t2137 = a[197];
    const double t2140 = a[662];
    const double t2141 = t5 * t2140;
    const double t2142 = a[437];
    const double t2143 = t3 * t2142;
    const double t2144 = a[204];
    const double t2151 =
        (t2117 + (t2136 + t2137) * t3 + (t2141 + t2143 + t2144) * t5 + (t2118 * t6 + t2120 + t2136 + t2141) * t6) * t6;
    const double t2152 = t3 * t2140;
    const double t2155 = t5 * t2135;
    const double t2158 = t6 * t2125;
    const double t2166 =
        (t2117 + (t2152 + t2144) * t3 + (t2155 + t2143 + t2137) * t5 + (t2142 * t5 + t2127 + t2143 + t2158) * t6 +
         (t2118 * t17 + t2120 + t2152 + t2155 + t2158) * t17) *
        t17;
    const double t2167 = a[861];
    const double t2170 = a[288];
    const double t2175 = a[896];
    const double t2177 = a[366];
    const double t2188 = (t2167 * t125 + (t2167 * t5 + t2170 * t3) * t5 + (t2167 * t6 + t2175 * t5 + t2177 * t3) * t6 +
                          (t2167 * t17 + t2170 * t6 + t2175 * t3 + t2177 * t5) * t17) *
                         t56;
    const double t2189 = a[104];
    const double t2190 = a[818];
    const double t2192 = a[221];
    const double t2194 = (t2190 * t3 + t2192) * t3;
    const double t2196 = a[782];
    const double t2199 = (t2190 * t5 + t2196 * t3 + t2192) * t5;
    const double t2200 = a[763];
    const double t2202 = a[742];
    const double t2203 = t5 * t2202;
    const double t2204 = a[402];
    const double t2205 = t3 * t2204;
    const double t2206 = a[214];
    const double t2208 = (t2200 * t6 + t2203 + t2205 + t2206) * t6;
    const double t2210 = a[793];
    const double t2212 = t5 * t2204;
    const double t2213 = t3 * t2202;
    const double t2215 = (t2200 * t17 + t2210 * t6 + t2206 + t2212 + t2213) * t17;
    const double t2216 = a[392];
    const double t2218 = a[638];
    const double t2222 = (t2216 * t17 + t215 * t2218 + t2216 * t6) * t56;
    const double t2223 = a[772];
    const double t2226 = t56 * a[836];
    const double t2227 = a[510];
    const double t2228 = t17 * t2227;
    const double t2229 = t6 * t2227;
    const double t2230 = a[564];
    const double t2231 = t5 * t2230;
    const double t2232 = t3 * t2230;
    const double t2233 = a[226];
    const double t2240 = (t2200 * t3 + t2206) * t3;
    const double t2244 = (t2200 * t5 + t2210 * t3 + t2206) * t5;
    const double t2247 = (t2190 * t6 + t2192 + t2203 + t2205) * t6;
    const double t2251 = (t2190 * t17 + t2196 * t6 + t2192 + t2212 + t2213) * t17;
    const double t2256 = (t2218 * t17 + t215 * t2216 + t2218 * t6) * t56;
    const double t2257 = a[391];
    const double t2258 = t147 * t2257;
    const double t2260 = t56 * a[323];
    const double t2261 = a[629];
    const double t2262 = t17 * t2261;
    const double t2263 = t6 * t2261;
    const double t2264 = t5 * t2261;
    const double t2265 = t3 * t2261;
    const double t2266 = a[275];
    const double t2270 = t17 * t2230;
    const double t2271 = t6 * t2230;
    const double t2272 = t5 * t2227;
    const double t2273 = t3 * t2227;
    const double t2278 = a[50];
    const double t2279 = a[405];
    const double t2281 = a[182];
    const double t2283 = (t2279 * t3 + t2281) * t3;
    const double t2285 = a[601];
    const double t2288 = (t2279 * t5 + t2285 * t3 + t2281) * t5;
    const double t2289 = a[372];
    const double t2291 = a[518];
    const double t2292 = t2291 * t5;
    const double t2293 = a[881];
    const double t2294 = t2293 * t3;
    const double t2295 = a[272];
    const double t2297 = (t2289 * t6 + t2292 + t2294 + t2295) * t6;
    const double t2299 = a[617];
    const double t2301 = t5 * t2293;
    const double t2302 = t3 * t2291;
    const double t2304 = (t2289 * t17 + t2299 * t6 + t2295 + t2301 + t2302) * t17;
    const double t2305 = a[655];
    const double t2307 = a[734];
    const double t2311 = (t2307 * t17 + t215 * t2305 + t2307 * t6) * t56;
    const double t2312 = a[899];
    const double t2313 = t147 * t2312;
    const double t2315 = t56 * a[466];
    const double t2316 = a[697];
    const double t2317 = t17 * t2316;
    const double t2318 = t6 * t2316;
    const double t2319 = a[760];
    const double t2320 = t5 * t2319;
    const double t2321 = t3 * t2319;
    const double t2322 = a[161];
    const double t2325 = a[880];
    const double t2326 = t207 * t2325;
    const double t2327 = a[931];
    const double t2328 = t147 * t2327;
    const double t2330 = t56 * a[395];
    const double t2331 = a[412];
    const double t2332 = t17 * t2331;
    const double t2333 = t6 * t2331;
    const double t2334 = a[320];
    const double t2335 = t5 * t2334;
    const double t2336 = t3 * t2334;
    const double t2337 = a[180];
    const double t2340 = a[421];
    const double t2342 = a[463];
    const double t2343 = t207 * t2342;
    const double t2344 = a[690];
    const double t2345 = t147 * t2344;
    const double t2347 = a[515] * t56;
    const double t2348 = a[877];
    const double t2349 = t17 * t2348;
    const double t2350 = t6 * t2348;
    const double t2351 = a[806];
    const double t2352 = t5 * t2351;
    const double t2353 = t3 * t2351;
    const double t2354 = a[251];
    const double t2361 = (t2289 * t3 + t2295) * t3;
    const double t2365 = (t2289 * t5 + t2299 * t3 + t2295) * t5;
    const double t2368 = (t2279 * t6 + t2281 + t2292 + t2294) * t6;
    const double t2372 = (t2279 * t17 + t2285 * t6 + t2281 + t2301 + t2302) * t17;
    const double t2377 = (t2305 * t17 + t215 * t2307 + t2305 * t6) * t56;
    const double t2378 = t147 * t2325;
    const double t2379 = t17 * t2334;
    const double t2380 = t6 * t2334;
    const double t2381 = t5 * t2331;
    const double t2382 = t3 * t2331;
    const double t2385 = t207 * t2312;
    const double t2386 = t17 * t2319;
    const double t2387 = t6 * t2319;
    const double t2388 = t5 * t2316;
    const double t2389 = t3 * t2316;
    const double t2392 = a[611];
    const double t2393 = t210 * t2392;
    const double t2394 = a[542];
    const double t2396 = t2394 * t147;
    const double t2398 = t56 * a[435];
    const double t2399 = a[428];
    const double t2400 = t17 * t2399;
    const double t2401 = t6 * t2399;
    const double t2402 = t5 * t2399;
    const double t2403 = t3 * t2399;
    const double t2404 = a[165];
    const double t2408 = t207 * t2344;
    const double t2409 = t147 * t2342;
    const double t2410 = t17 * t2351;
    const double t2411 = t6 * t2351;
    const double t2412 = t5 * t2348;
    const double t2413 = t3 * t2348;
    const double t2418 = a[854];
    const double t2419 = t2418 * t125;
    const double t2421 = a[388];
    const double t2424 = (t2418 * t5 + t2421 * t3) * t5;
    const double t2426 = a[684];
    const double t2428 = a[494];
    const double t2431 = (t2418 * t6 + t2426 * t5 + t2428 * t3) * t6;
    const double t2437 = (t2418 * t17 + t2421 * t6 + t2426 * t3 + t2428 * t5) * t17;
    const double t2438 = a[549];
    const double t2439 = t2438 * t6;
    const double t2440 = a[817];
    const double t2441 = t2440 * t215;
    const double t2442 = t2438 * t17;
    const double t2443 = a[696];
    const double t2447 = t2438 * t215;
    const double t2448 = t2440 * t6;
    const double t2449 = t2440 * t17;
    const double t2450 = a[882];
    const double t2455 = a[547];
    const double t2456 = t2455 * t215;
    const double t2457 = a[363];
    const double t2458 = t2457 * t6;
    const double t2459 = t2457 * t17;
    const double t2460 = a[887];
    const double t2461 = t2460 * t147;
    const double t2462 = a[661];
    const double t2463 = t2462 * t207;
    const double t2464 = a[726];
    const double t2468 = t2455 * t6;
    const double t2469 = t2457 * t215;
    const double t2470 = t2455 * t17;
    const double t2471 = t2462 * t147;
    const double t2472 = t2460 * t207;
    const double t2473 = a[332];
    const double t2482 = a[69];
    const double t2483 = t2482 * t17;
    const double t2484 = t2482 * t6;
    const double t2485 = a[146];
    const double t2486 = t2485 * t5;
    const double t2487 = t2485 * t3;
    const double t2488 = a[12];
    const double t2489 = a[83];
    const double t2490 = a[582];
    const double t2492 = a[155];
    const double t2496 = a[457];
    const double t2500 = a[464];
    const double t2502 = a[499];
    const double t2503 = t5 * t2502;
    const double t2504 = a[371];
    const double t2505 = t3 * t2504;
    const double t2506 = a[208];
    const double t2510 = a[597];
    const double t2512 = t5 * t2504;
    const double t2513 = t3 * t2502;
    const double t2516 = a[599];
    const double t2518 = a[438];
    const double t2524 =
        (t2489 + (t2490 * t3 + t2492) * t3 + (t2490 * t5 + t2496 * t3 + t2492) * t5 +
         (t2500 * t6 + t2503 + t2505 + t2506) * t6 + (t2500 * t17 + t2510 * t6 + t2506 + t2512 + t2513) * t17 +
         (t2518 * t17 + t215 * t2516 + t2518 * t6) * t56) *
        t56;
    const double t2525 = a[55];
    const double t2527 = t56 * a[373];
    const double t2528 = a[289];
    const double t2531 = a[449];
    const double t2534 = a[238];
    const double t2536 = (t2528 * t17 + t2528 * t6 + t2531 * t3 + t2531 * t5 + t2527 + t2534) * t56;
    const double t2538 = a[630] * t56;
    const double t2542 = a[76];
    const double t2544 = t56 * a[918];
    const double t2545 = a[708];
    const double t2548 = a[791];
    const double t2551 = a[264];
    const double t2553 = (t2545 * t17 + t2545 * t6 + t2548 * t3 + t2548 * t5 + t2544 + t2551) * t56;
    const double t2554 = a[521];
    const double t2556 = t2554 * t147 * t56;
    const double t2558 = a[850] * t56;
    const double t2562 = a[39];
    const double t2564 = t56 * a[737];
    const double t2565 = a[471];
    const double t2568 = a[839];
    const double t2571 = a[258];
    const double t2573 = (t2565 * t17 + t2565 * t6 + t2568 * t3 + t2568 * t5 + t2564 + t2571) * t56;
    const double t2574 = a[474];
    const double t2576 = t2574 * t147 * t56;
    const double t2577 = a[612];
    const double t2579 = t2577 * t207 * t56;
    const double t2581 = t56 * a[811];
    const double t2585 = a[33];
    const double t2587 = t56 * a[512];
    const double t2588 = a[808];
    const double t2591 = a[685];
    const double t2594 = a[157];
    const double t2596 = (t2588 * t17 + t2588 * t6 + t2591 * t3 + t2591 * t5 + t2587 + t2594) * t56;
    const double t2597 = a[591];
    const double t2599 = t2597 * t147 * t56;
    const double t2600 = a[904];
    const double t2602 = t2600 * t207 * t56;
    const double t2603 = a[431];
    const double t2605 = t2603 * t210 * t56;
    const double t2607 = a[686] * t56;
    const double t2611 = a[75];
    const double t2612 = a[930];
    const double t2614 = a[250];
    const double t2616 = (t2612 * t3 + t2614) * t3;
    const double t2618 = a[538];
    const double t2621 = (t2612 * t5 + t2618 * t3 + t2614) * t5;
    const double t2622 = a[622];
    const double t2624 = a[892];
    const double t2625 = t5 * t2624;
    const double t2626 = a[560];
    const double t2627 = t3 * t2626;
    const double t2628 = a[255];
    const double t2630 = (t2622 * t6 + t2625 + t2627 + t2628) * t6;
    const double t2632 = a[382];
    const double t2634 = t5 * t2626;
    const double t2635 = t3 * t2624;
    const double t2637 = (t2622 * t17 + t2632 * t6 + t2628 + t2634 + t2635) * t17;
    const double t2638 = a[706];
    const double t2640 = a[469];
    const double t2644 = (t2638 * t17 + t215 * t2640 + t2638 * t6) * t56;
    const double t2645 = a[897];
    const double t2648 = a[851] * t56;
    const double t2649 = a[711];
    const double t2650 = t17 * t2649;
    const double t2651 = t6 * t2649;
    const double t2652 = a[557];
    const double t2653 = t5 * t2652;
    const double t2654 = t3 * t2652;
    const double t2655 = a[193];
    const double t2658 = a[675];
    const double t2660 = a[485];
    const double t2661 = t147 * t2660;
    const double t2663 = t56 * a[641];
    const double t2664 = a[765];
    const double t2665 = t17 * t2664;
    const double t2666 = t6 * t2664;
    const double t2667 = a[768];
    const double t2668 = t5 * t2667;
    const double t2669 = t3 * t2667;
    const double t2670 = a[225];
    const double t2673 = a[814];
    const double t2675 = a[762];
    const double t2676 = t207 * t2675;
    const double t2677 = a[528];
    const double t2678 = t147 * t2677;
    const double t2680 = t56 * a[625];
    const double t2681 = a[766];
    const double t2682 = t17 * t2681;
    const double t2683 = t6 * t2681;
    const double t2684 = a[674];
    const double t2685 = t5 * t2684;
    const double t2686 = t3 * t2684;
    const double t2687 = a[231];
    const double t2690 = a[920];
    const double t2692 = a[478];
    const double t2693 = t210 * t2692;
    const double t2694 = a[926];
    const double t2695 = t207 * t2694;
    const double t2696 = a[462];
    const double t2697 = t147 * t2696;
    const double t2699 = t56 * a[598];
    const double t2700 = a[459];
    const double t2701 = t17 * t2700;
    const double t2702 = t6 * t2700;
    const double t2703 = a[602];
    const double t2704 = t5 * t2703;
    const double t2705 = t3 * t2703;
    const double t2706 = a[218];
    const double t2709 = a[490];
    const double t2710 = t2709 * t6;
    const double t2711 = a[497];
    const double t2712 = t2711 * t215;
    const double t2713 = t2709 * t17;
    const double t2714 = a[615];
    const double t2716 = a[632];
    const double t2718 = a[500];
    const double t2720 = a[526];
    const double t2724 = t2611 + t2616 + t2621 + t2630 + t2637 + t2644 +
                         (t2645 * t147 + t2648 + t2650 + t2651 + t2653 + t2654 + t2655) * t147 +
                         (t2658 * t207 + t2661 + t2663 + t2665 + t2666 + t2668 + t2669 + t2670) * t207 +
                         (t2673 * t210 + t2676 + t2678 + t2680 + t2682 + t2683 + t2685 + t2686 + t2687) * t210 +
                         (t2690 * t485 + t2693 + t2695 + t2697 + t2699 + t2701 + t2702 + t2704 + t2705 + t2706) * t485 +
                         (t2714 * t147 + t2716 * t207 + t2718 * t210 + t2720 * t485 + t2710 + t2712 + t2713) * t501;
    const double t2726 = a[34];
    const double t2728 = t56 * a[571];
    const double t2729 = a[420];
    const double t2732 = a[645];
    const double t2735 = a[236];
    const double t2737 = (t2729 * t17 + t2729 * t6 + t2732 * t3 + t2732 * t5 + t2728 + t2735) * t56;
    const double t2738 = a[627];
    const double t2741 = a[294];
    const double t2744 = a[577];
    const double t2747 = a[891];
    const double t2751 = t501 * a[855];
    const double t2752 = a[311];
    const double t2754 = a[348];
    const double t2756 = a[853];
    const double t2758 = a[540];
    const double t2761 = t56 * a[898];
    const double t2762 = a[539];
    const double t2763 = t17 * t2762;
    const double t2764 = t6 * t2762;
    const double t2765 = a[554];
    const double t2766 = t5 * t2765;
    const double t2767 = t3 * t2765;
    const double t2768 = a[202];
    const double t2769 = t2758 * t147 + t2756 * t207 + t2754 * t210 + t2752 * t485 + t2751 + t2761 + t2763 + t2764 +
                         t2766 + t2767 + t2768;
    const double t2775 = t501 * a[636] + t56 * a[456];
    const double t2776 = t2775 * t654;
    const double t2779 = t2483 + t2484 + t2486 + t2487 + t2488 + t2524 + (t2538 * t147 + t2525 + t2536) * t147 +
                         (t2558 * t207 + t2542 + t2553 + t2556) * t207 +
                         (t2581 * t210 + t2562 + t2573 + t2576 + t2579) * t210 +
                         (t2607 * t485 + t2585 + t2596 + t2599 + t2602 + t2605) * t485 + t2724 * t501 +
                         (t2738 * t147 * t56 + t2741 * t207 * t56 + t2744 * t210 * t56 + t2747 * t485 * t56 +
                          t2769 * t501 + t2726 + t2737 + t2776) *
                             t654;
    const double t2781 = t2485 * t17;
    const double t2782 = t2485 * t6;
    const double t2783 = t2482 * t5;
    const double t2784 = t2482 * t3;
    const double t2805 =
        (t2489 + (t2500 * t3 + t2506) * t3 + (t2500 * t5 + t2510 * t3 + t2506) * t5 +
         (t2490 * t6 + t2492 + t2503 + t2505) * t6 + (t2490 * t17 + t2496 * t6 + t2492 + t2512 + t2513) * t17 +
         (t2516 * t17 + t215 * t2518 + t2516 * t6) * t56) *
        t56;
    const double t2811 = (t2548 * t17 + t2545 * t3 + t2545 * t5 + t2548 * t6 + t2544 + t2551) * t56;
    const double t2820 = (t2531 * t17 + t2528 * t3 + t2528 * t5 + t2531 * t6 + t2527 + t2534) * t56;
    const double t2829 = (t2591 * t17 + t2588 * t3 + t2588 * t5 + t2591 * t6 + t2587 + t2594) * t56;
    const double t2831 = t2600 * t147 * t56;
    const double t2833 = t2597 * t207 * t56;
    const double t2842 = (t2568 * t17 + t2565 * t3 + t2565 * t5 + t2568 * t6 + t2564 + t2571) * t56;
    const double t2844 = t2577 * t147 * t56;
    const double t2846 = t2574 * t207 * t56;
    const double t2852 = (t2622 * t3 + t2628) * t3;
    const double t2856 = (t2622 * t5 + t2632 * t3 + t2628) * t5;
    const double t2859 = (t2612 * t6 + t2614 + t2625 + t2627) * t6;
    const double t2863 = (t2612 * t17 + t2618 * t6 + t2614 + t2634 + t2635) * t17;
    const double t2868 = (t2640 * t17 + t215 * t2638 + t2640 * t6) * t56;
    const double t2870 = t17 * t2667;
    const double t2871 = t6 * t2667;
    const double t2872 = t5 * t2664;
    const double t2873 = t3 * t2664;
    const double t2877 = t17 * t2652;
    const double t2878 = t6 * t2652;
    const double t2879 = t5 * t2649;
    const double t2880 = t3 * t2649;
    const double t2884 = t207 * t2696;
    const double t2885 = t147 * t2694;
    const double t2886 = t17 * t2703;
    const double t2887 = t6 * t2703;
    const double t2888 = t5 * t2700;
    const double t2889 = t3 * t2700;
    const double t2893 = t207 * t2677;
    const double t2894 = t147 * t2675;
    const double t2895 = t17 * t2684;
    const double t2896 = t6 * t2684;
    const double t2897 = t5 * t2681;
    const double t2898 = t3 * t2681;
    const double t2901 = t2709 * t215;
    const double t2902 = t2711 * t6;
    const double t2903 = t2711 * t17;
    const double t2910 = t2611 + t2852 + t2856 + t2859 + t2863 + t2868 +
                         (t2658 * t147 + t2663 + t2670 + t2870 + t2871 + t2872 + t2873) * t147 +
                         (t2645 * t207 + t2648 + t2655 + t2661 + t2877 + t2878 + t2879 + t2880) * t207 +
                         (t2690 * t210 + t2699 + t2706 + t2884 + t2885 + t2886 + t2887 + t2888 + t2889) * t210 +
                         (t2673 * t485 + t2680 + t2687 + t2693 + t2893 + t2894 + t2895 + t2896 + t2897 + t2898) * t485 +
                         (t2716 * t147 + t2714 * t207 + t2720 * t210 + t2718 * t485 + t2901 + t2902 + t2903) * t501;
    const double t2912 = a[113];
    const double t2915 = a[296];
    const double t2922 = (t2915 * t17 + t2915 * t3 + t2915 * t5 + t2915 * t6 + t56 * a[327] + a[245]) * t56;
    const double t2923 = a[434];
    const double t2926 = t2923 * t56;
    const double t2928 = a[729];
    const double t2931 = t2928 * t56;
    const double t2934 = t501 * a[748];
    const double t2935 = a[483];
    const double t2938 = a[481];
    const double t2942 = t56 * a[749];
    const double t2943 = a[921];
    const double t2944 = t17 * t2943;
    const double t2945 = t6 * t2943;
    const double t2946 = t5 * t2943;
    const double t2947 = t3 * t2943;
    const double t2948 = a[252];
    const double t2949 = t2938 * t147 + t2938 * t207 + t2935 * t210 + t2935 * t485 + t2934 + t2942 + t2944 + t2945 +
                         t2946 + t2947 + t2948;
    const double t2956 = (t501 * a[889] + t56 * a[442]) * t654;
    const double t2964 = (t2732 * t17 + t2729 * t3 + t2729 * t5 + t2732 * t6 + t2728 + t2735) * t56;
    const double t2977 = t17 * t2765;
    const double t2978 = t6 * t2765;
    const double t2979 = t5 * t2762;
    const double t2980 = t3 * t2762;
    const double t2981 = t2756 * t147 + t2758 * t207 + t2752 * t210 + t2754 * t485 + t2751 + t2761 + t2768 + t2977 +
                         t2978 + t2979 + t2980;
    const double t2983 = t2775 * t1098;
    const double t2986 =
        t2781 + t2782 + t2783 + t2784 + t2488 + t2805 + (t2558 * t147 + t2542 + t2811) * t147 +
        (t2538 * t207 + t2525 + t2556 + t2820) * t207 + (t2607 * t210 + t2585 + t2829 + t2831 + t2833) * t210 +
        (t2581 * t485 + t2562 + t2605 + t2842 + t2844 + t2846) * t485 + t2910 * t501 +
        (t2923 * t147 * t56 + t2928 * t210 * t56 + t2926 * t207 + t2931 * t485 + t2949 * t501 + t2912 + t2922 + t2956) *
            t654 +
        (t2741 * t147 * t56 + t2738 * t207 * t56 + t2747 * t210 * t56 + t2744 * t485 * t56 + t2981 * t501 + t2726 +
         t2956 + t2964 + t2983) *
            t1098;
    const double t2988 = a[63];
    const double t2989 = a[436];
    const double t2991 = a[249];
    const double t2995 = (t2988 + (t2989 * t3 + t2991) * t3) * t3;
    const double t2996 = a[895];
    const double t2997 = t3 * t2996;
    const double t2998 = a[263];
    const double t3005 = (t2988 + (t2997 + t2998) * t3 + (t2989 * t5 + t2991 + t2997) * t5) * t5;
    const double t3006 = a[398];
    const double t3007 = t3 * t3006;
    const double t3008 = a[159];
    const double t3011 = a[393];
    const double t3012 = t5 * t3011;
    const double t3013 = a[830];
    const double t3014 = t3 * t3013;
    const double t3015 = a[203];
    const double t3022 =
        (t2988 + (t3007 + t3008) * t3 + (t3012 + t3014 + t3015) * t5 + (t2989 * t6 + t2991 + t3007 + t3012) * t6) * t6;
    const double t3023 = t3 * t3011;
    const double t3026 = t5 * t3006;
    const double t3029 = t6 * t2996;
    const double t3037 =
        (t2988 + (t3023 + t3015) * t3 + (t3026 + t3014 + t3008) * t5 + (t3013 * t5 + t2998 + t3014 + t3029) * t6 +
         (t2989 * t17 + t2991 + t3023 + t3026 + t3029) * t17) *
        t17;
    const double t3038 = a[523];
    const double t3041 = a[573];
    const double t3046 = a[658];
    const double t3048 = a[883];
    const double t3059 = (t3038 * t125 + (t3041 * t3 + t3038 * t5) * t5 + (t3048 * t3 + t3038 * t6 + t3046 * t5) * t6 +
                          (t3038 * t17 + t3046 * t3 + t3041 * t6 + t3048 * t5) * t17) *
                         t56;
    const double t3060 = a[53];
    const double t3061 = a[927];
    const double t3063 = a[232];
    const double t3065 = (t3061 * t3 + t3063) * t3;
    const double t3067 = a[450];
    const double t3070 = (t3067 * t3 + t3061 * t5 + t3063) * t5;
    const double t3071 = a[649];
    const double t3073 = a[283];
    const double t3074 = t5 * t3073;
    const double t3075 = a[619];
    const double t3076 = t3 * t3075;
    const double t3077 = a[266];
    const double t3079 = (t3071 * t6 + t3074 + t3076 + t3077) * t6;
    const double t3081 = a[343];
    const double t3083 = t5 * t3075;
    const double t3084 = t3 * t3073;
    const double t3086 = (t3071 * t17 + t3081 * t6 + t3077 + t3083 + t3084) * t17;
    const double t3087 = a[827];
    const double t3089 = a[534];
    const double t3093 = (t3087 * t17 + t215 * t3089 + t3087 * t6) * t56;
    const double t3094 = a[788];
    const double t3097 = a[482] * t56;
    const double t3098 = a[786];
    const double t3099 = t17 * t3098;
    const double t3100 = t6 * t3098;
    const double t3101 = a[838];
    const double t3102 = t5 * t3101;
    const double t3103 = t3 * t3101;
    const double t3104 = a[219];
    const double t3111 = (t3071 * t3 + t3077) * t3;
    const double t3115 = (t3081 * t3 + t3071 * t5 + t3077) * t5;
    const double t3118 = (t3061 * t6 + t3063 + t3074 + t3076) * t6;
    const double t3122 = (t3061 * t17 + t3067 * t6 + t3063 + t3083 + t3084) * t17;
    const double t3127 = (t3089 * t17 + t215 * t3087 + t3089 * t6) * t56;
    const double t3128 = a[605];
    const double t3129 = t147 * t3128;
    const double t3131 = t56 * a[728];
    const double t3132 = a[541];
    const double t3133 = t17 * t3132;
    const double t3134 = t6 * t3132;
    const double t3135 = t5 * t3132;
    const double t3136 = t3 * t3132;
    const double t3137 = a[200];
    const double t3141 = t17 * t3101;
    const double t3142 = t6 * t3101;
    const double t3143 = t5 * t3098;
    const double t3144 = t3 * t3098;
    const double t3149 = a[142];
    const double t3150 = a[856];
    const double t3152 = a[196];
    const double t3154 = (t3150 * t3 + t3152) * t3;
    const double t3156 = a[720];
    const double t3159 = (t3156 * t3 + t3150 * t5 + t3152) * t5;
    const double t3160 = a[430];
    const double t3162 = a[683];
    const double t3163 = t5 * t3162;
    const double t3164 = a[665];
    const double t3165 = t3 * t3164;
    const double t3166 = a[235];
    const double t3168 = (t3160 * t6 + t3163 + t3165 + t3166) * t6;
    const double t3170 = a[924];
    const double t3172 = t5 * t3164;
    const double t3173 = t3 * t3162;
    const double t3175 = (t3160 * t17 + t3170 * t6 + t3166 + t3172 + t3173) * t17;
    const double t3176 = a[603];
    const double t3178 = a[609];
    const double t3182 = (t3176 * t17 + t215 * t3178 + t3176 * t6) * t56;
    const double t3183 = a[504];
    const double t3184 = t147 * t3183;
    const double t3186 = a[468] * t56;
    const double t3187 = a[351];
    const double t3188 = t17 * t3187;
    const double t3189 = t6 * t3187;
    const double t3190 = a[338];
    const double t3191 = t5 * t3190;
    const double t3192 = t3 * t3190;
    const double t3193 = a[194];
    const double t3196 = a[613];
    const double t3197 = t207 * t3196;
    const double t3198 = a[290];
    const double t3199 = t147 * t3198;
    const double t3201 = a[709] * t56;
    const double t3202 = a[716];
    const double t3203 = t17 * t3202;
    const double t3204 = t6 * t3202;
    const double t3205 = a[750];
    const double t3206 = t5 * t3205;
    const double t3207 = t3 * t3205;
    const double t3208 = a[175];
    const double t3211 = a[365];
    const double t3213 = a[676];
    const double t3214 = t207 * t3213;
    const double t3215 = a[643];
    const double t3216 = t147 * t3215;
    const double t3218 = t56 * a[594];
    const double t3219 = a[633];
    const double t3220 = t17 * t3219;
    const double t3221 = t6 * t3219;
    const double t3222 = a[841];
    const double t3223 = t5 * t3222;
    const double t3224 = t3 * t3222;
    const double t3225 = a[243];
    const double t3232 = (t3160 * t3 + t3166) * t3;
    const double t3236 = (t3170 * t3 + t3160 * t5 + t3166) * t5;
    const double t3239 = (t3150 * t6 + t3152 + t3163 + t3165) * t6;
    const double t3243 = (t3150 * t17 + t3156 * t6 + t3152 + t3172 + t3173) * t17;
    const double t3248 = (t3178 * t17 + t215 * t3176 + t3178 * t6) * t56;
    const double t3249 = t147 * t3196;
    const double t3250 = t17 * t3205;
    const double t3251 = t6 * t3205;
    const double t3252 = t5 * t3202;
    const double t3253 = t3 * t3202;
    const double t3256 = t207 * t3183;
    const double t3257 = t17 * t3190;
    const double t3258 = t6 * t3190;
    const double t3259 = t5 * t3187;
    const double t3260 = t3 * t3187;
    const double t3263 = a[585];
    const double t3264 = t210 * t3263;
    const double t3265 = a[307];
    const double t3267 = t147 * t3265;
    const double t3269 = t56 * a[374];
    const double t3270 = a[301];
    const double t3271 = t17 * t3270;
    const double t3272 = t6 * t3270;
    const double t3273 = t5 * t3270;
    const double t3274 = t3 * t3270;
    const double t3275 = a[172];
    const double t3279 = t207 * t3215;
    const double t3280 = t147 * t3213;
    const double t3281 = t17 * t3222;
    const double t3282 = t6 * t3222;
    const double t3283 = t5 * t3219;
    const double t3284 = t3 * t3219;
    const double t3289 = a[548];
    const double t3290 = t3289 * t125;
    const double t3292 = a[846];
    const double t3295 = (t3292 * t3 + t3289 * t5) * t5;
    const double t3297 = a[865];
    const double t3299 = a[313];
    const double t3302 = (t3299 * t3 + t3289 * t6 + t3297 * t5) * t6;
    const double t3308 = (t3289 * t17 + t3297 * t3 + t3292 * t6 + t3299 * t5) * t17;
    const double t3309 = a[902];
    const double t3310 = t3309 * t6;
    const double t3311 = a[905];
    const double t3312 = t3311 * t215;
    const double t3313 = t3309 * t17;
    const double t3314 = a[522];
    const double t3318 = t3309 * t215;
    const double t3319 = t3311 * t6;
    const double t3320 = t3311 * t17;
    const double t3321 = a[567];
    const double t3326 = a[824];
    const double t3327 = t3326 * t6;
    const double t3328 = a[416];
    const double t3329 = t3328 * t215;
    const double t3330 = t3326 * t17;
    const double t3331 = a[731];
    const double t3332 = t3331 * t147;
    const double t3333 = a[874];
    const double t3334 = t3333 * t207;
    const double t3335 = a[911];
    const double t3339 = t3326 * t215;
    const double t3340 = t3328 * t6;
    const double t3341 = t3328 * t17;
    const double t3342 = t3333 * t147;
    const double t3343 = t3331 * t207;
    const double t3344 = a[378];
    const double t3351 = a[45];
    const double t3352 = a[794];
    const double t3354 = a[228];
    const double t3356 = (t3352 * t3 + t3354) * t3;
    const double t3358 = a[300];
    const double t3361 = (t3358 * t3 + t3352 * t5 + t3354) * t5;
    const double t3362 = a[912];
    const double t3364 = a[812];
    const double t3365 = t5 * t3364;
    const double t3366 = a[396];
    const double t3367 = t3 * t3366;
    const double t3368 = a[201];
    const double t3370 = (t3362 * t6 + t3365 + t3367 + t3368) * t6;
    const double t3372 = a[723];
    const double t3374 = t5 * t3366;
    const double t3375 = t3 * t3364;
    const double t3377 = (t3362 * t17 + t3372 * t6 + t3368 + t3374 + t3375) * t17;
    const double t3378 = a[610];
    const double t3380 = a[387];
    const double t3384 = (t3378 * t17 + t215 * t3380 + t3378 * t6) * t56;
    const double t3385 = a[423];
    const double t3388 = a[757] * t56;
    const double t3389 = a[862];
    const double t3390 = t17 * t3389;
    const double t3391 = t6 * t3389;
    const double t3392 = a[735];
    const double t3393 = t5 * t3392;
    const double t3394 = t3 * t3392;
    const double t3395 = a[240];
    const double t3398 = a[400];
    const double t3400 = a[584];
    const double t3401 = t147 * t3400;
    const double t3403 = t56 * a[513];
    const double t3404 = a[747];
    const double t3405 = t17 * t3404;
    const double t3406 = t6 * t3404;
    const double t3407 = a[537];
    const double t3408 = t5 * t3407;
    const double t3409 = t3 * t3407;
    const double t3410 = a[222];
    const double t3413 = a[700];
    const double t3415 = a[347];
    const double t3416 = t207 * t3415;
    const double t3417 = a[486];
    const double t3418 = t147 * t3417;
    const double t3420 = a[514] * t56;
    const double t3421 = a[769];
    const double t3422 = t17 * t3421;
    const double t3423 = t6 * t3421;
    const double t3424 = a[804];
    const double t3425 = t5 * t3424;
    const double t3426 = t3 * t3424;
    const double t3427 = a[199];
    const double t3430 = a[349];
    const double t3432 = a[626];
    const double t3433 = t210 * t3432;
    const double t3434 = a[507];
    const double t3435 = t207 * t3434;
    const double t3436 = a[484];
    const double t3437 = t147 * t3436;
    const double t3439 = a[668] * t56;
    const double t3440 = a[776];
    const double t3441 = t17 * t3440;
    const double t3442 = t6 * t3440;
    const double t3443 = a[346];
    const double t3444 = t5 * t3443;
    const double t3445 = t3 * t3443;
    const double t3446 = a[265];
    const double t3449 = a[620];
    const double t3450 = t3449 * t215;
    const double t3451 = a[444];
    const double t3452 = t3451 * t6;
    const double t3453 = t3451 * t17;
    const double t3454 = a[356];
    const double t3456 = a[596];
    const double t3458 = a[333];
    const double t3460 = a[634];
    const double t3464 = a[364];
    const double t3465 = t654 * t3464;
    const double t3467 = a[672] * t501;
    const double t3468 = a[719];
    const double t3470 = a[819];
    const double t3472 = a[832];
    const double t3474 = a[558];
    const double t3477 = a[536] * t56;
    const double t3478 = a[587];
    const double t3479 = t17 * t3478;
    const double t3480 = t6 * t3478;
    const double t3481 = a[330];
    const double t3482 = t5 * t3481;
    const double t3483 = t3 * t3481;
    const double t3484 = a[160];
    const double t3485 = t3474 * t147 + t3472 * t207 + t3470 * t210 + t3468 * t485 + t3465 + t3467 + t3477 + t3479 +
                         t3480 + t3482 + t3483 + t3484;
    const double t3487 = t3351 + t3356 + t3361 + t3370 + t3377 + t3384 +
                         (t3385 * t147 + t3388 + t3390 + t3391 + t3393 + t3394 + t3395) * t147 +
                         (t3398 * t207 + t3401 + t3403 + t3405 + t3406 + t3408 + t3409 + t3410) * t207 +
                         (t3413 * t210 + t3416 + t3418 + t3420 + t3422 + t3423 + t3425 + t3426 + t3427) * t210 +
                         (t3430 * t485 + t3433 + t3435 + t3437 + t3439 + t3441 + t3442 + t3444 + t3445 + t3446) * t485 +
                         (t3454 * t147 + t3456 * t207 + t3458 * t210 + t3460 * t485 + t3450 + t3452 + t3453) * t501 +
                         t3485 * t654;
    const double t3491 = (t3362 * t3 + t3368) * t3;
    const double t3495 = (t3372 * t3 + t3362 * t5 + t3368) * t5;
    const double t3498 = (t3352 * t6 + t3354 + t3365 + t3367) * t6;
    const double t3502 = (t3352 * t17 + t3358 * t6 + t3354 + t3374 + t3375) * t17;
    const double t3507 = (t3380 * t17 + t215 * t3378 + t3380 * t6) * t56;
    const double t3509 = t17 * t3407;
    const double t3510 = t6 * t3407;
    const double t3511 = t5 * t3404;
    const double t3512 = t3 * t3404;
    const double t3516 = t17 * t3392;
    const double t3517 = t6 * t3392;
    const double t3518 = t5 * t3389;
    const double t3519 = t3 * t3389;
    const double t3523 = t207 * t3436;
    const double t3524 = t147 * t3434;
    const double t3525 = t17 * t3443;
    const double t3526 = t6 * t3443;
    const double t3527 = t5 * t3440;
    const double t3528 = t3 * t3440;
    const double t3532 = t207 * t3417;
    const double t3533 = t147 * t3415;
    const double t3534 = t17 * t3424;
    const double t3535 = t6 * t3424;
    const double t3536 = t5 * t3421;
    const double t3537 = t3 * t3421;
    const double t3540 = t3449 * t6;
    const double t3541 = t3451 * t215;
    const double t3542 = t3449 * t17;
    const double t3550 = a[732] * t654;
    const double t3552 = a[403] * t501;
    const double t3553 = a[568];
    const double t3556 = a[401];
    const double t3560 = a[337] * t56;
    const double t3561 = a[556];
    const double t3562 = t17 * t3561;
    const double t3563 = t6 * t3561;
    const double t3564 = t5 * t3561;
    const double t3565 = t3 * t3561;
    const double t3566 = a[153];
    const double t3567 = t3556 * t147 + t3556 * t207 + t3553 * t210 + t3553 * t485 + t3550 + t3552 + t3560 + t3562 +
                         t3563 + t3564 + t3565 + t3566;
    const double t3569 = t1098 * t3464;
    const double t3574 = t17 * t3481;
    const double t3575 = t6 * t3481;
    const double t3576 = t5 * t3478;
    const double t3577 = t3 * t3478;
    const double t3578 = t3472 * t147 + t3474 * t207 + t3468 * t210 + t3470 * t485 + t3467 + t3477 + t3484 + t3550 +
                         t3569 + t3574 + t3575 + t3576 + t3577;
    const double t3580 = t3351 + t3491 + t3495 + t3498 + t3502 + t3507 +
                         (t3398 * t147 + t3403 + t3410 + t3509 + t3510 + t3511 + t3512) * t147 +
                         (t3385 * t207 + t3388 + t3395 + t3401 + t3516 + t3517 + t3518 + t3519) * t207 +
                         (t3430 * t210 + t3439 + t3446 + t3523 + t3524 + t3525 + t3526 + t3527 + t3528) * t210 +
                         (t3413 * t485 + t3420 + t3427 + t3433 + t3532 + t3533 + t3534 + t3535 + t3536 + t3537) * t485 +
                         (t3456 * t147 + t3454 * t207 + t3460 * t210 + t3458 * t485 + t3540 + t3541 + t3542) * t501 +
                         t3567 * t654 + t3578 * t1098;
    const double t3582 = a[543];
    const double t3583 = t3582 * t125;
    const double t3585 = a[376];
    const double t3588 = (t3585 * t3 + t3582 * t5) * t5;
    const double t3590 = a[730];
    const double t3592 = a[908];
    const double t3595 = (t3592 * t3 + t3582 * t6 + t3590 * t5) * t6;
    const double t3601 = (t3582 * t17 + t3590 * t3 + t3585 * t6 + t3592 * t5) * t17;
    const double t3602 = a[589];
    const double t3603 = t3602 * t215;
    const double t3604 = a[695];
    const double t3605 = t3604 * t6;
    const double t3606 = t3604 * t17;
    const double t3607 = a[516];
    const double t3611 = t3602 * t6;
    const double t3612 = t3604 * t215;
    const double t3613 = t3602 * t17;
    const double t3614 = a[508];
    const double t3619 = a[929];
    const double t3620 = t3619 * t215;
    const double t3621 = a[369];
    const double t3622 = t3621 * t6;
    const double t3623 = t3621 * t17;
    const double t3624 = a[647];
    const double t3625 = t3624 * t147;
    const double t3626 = a[715];
    const double t3627 = t3626 * t207;
    const double t3628 = a[408];
    const double t3632 = t3619 * t6;
    const double t3633 = t3621 * t215;
    const double t3634 = t3619 * t17;
    const double t3635 = t3626 * t147;
    const double t3636 = t3624 * t207;
    const double t3637 = a[344];
    const double t3642 = a[410];
    const double t3643 = t3642 * t215;
    const double t3644 = a[773];
    const double t3645 = t3644 * t6;
    const double t3646 = t3644 * t17;
    const double t3647 = a[487];
    const double t3649 = a[693];
    const double t3651 = a[429];
    const double t3653 = a[835];
    const double t3655 = a[310];
    const double t3656 = t3655 * t654;
    const double t3659 = t3642 * t6;
    const double t3660 = t3644 * t215;
    const double t3661 = t3642 * t17;
    const double t3667 = a[336] * t654;
    const double t3668 = t3655 * t1098;
    const double t2643 = x[1];
    const double t3673 =
        t2995 + t3005 + t3022 + t3037 + t3059 +
        (t3060 + t3065 + t3070 + t3079 + t3086 + t3093 +
         (t3094 * t147 + t3097 + t3099 + t3100 + t3102 + t3103 + t3104) * t147) *
            t147 +
        (t3060 + t3111 + t3115 + t3118 + t3122 + t3127 +
         (t3129 + t3131 + t3133 + t3134 + t3135 + t3136 + t3137) * t147 +
         (t3094 * t207 + t3097 + t3104 + t3129 + t3141 + t3142 + t3143 + t3144) * t207) *
            t207 +
        (t3149 + t3154 + t3159 + t3168 + t3175 + t3182 +
         (t3184 + t3186 + t3188 + t3189 + t3191 + t3192 + t3193) * t147 +
         (t3197 + t3199 + t3201 + t3203 + t3204 + t3206 + t3207 + t3208) * t207 +
         (t3211 * t210 + t3214 + t3216 + t3218 + t3220 + t3221 + t3223 + t3224 + t3225) * t210) *
            t210 +
        (t3149 + t3232 + t3236 + t3239 + t3243 + t3248 +
         (t3249 + t3201 + t3250 + t3251 + t3252 + t3253 + t3208) * t147 +
         (t3256 + t3199 + t3186 + t3257 + t3258 + t3259 + t3260 + t3193) * t207 +
         (t3265 * t207 + t3264 + t3267 + t3269 + t3271 + t3272 + t3273 + t3274 + t3275) * t210 +
         (t3211 * t485 + t3218 + t3225 + t3264 + t3279 + t3280 + t3281 + t3282 + t3283 + t3284) * t485) *
            t485 +
        (t3290 + t3295 + t3302 + t3308 + (t3314 * t147 + t3310 + t3312 + t3313) * t147 +
         (t3321 * t147 + t3314 * t207 + t3318 + t3319 + t3320) * t207 +
         (t3335 * t210 + t3327 + t3329 + t3330 + t3332 + t3334) * t210 +
         (t3344 * t210 + t3335 * t485 + t3339 + t3340 + t3341 + t3342 + t3343) * t485) *
            t501 +
        t3487 * t654 + t3580 * t1098 +
        (t3583 + t3588 + t3595 + t3601 + (t3607 * t147 + t3603 + t3605 + t3606) * t147 +
         (t3614 * t147 + t3607 * t207 + t3611 + t3612 + t3613) * t207 +
         (t3628 * t210 + t3620 + t3622 + t3623 + t3625 + t3627) * t210 +
         (t3637 * t210 + t3628 * t485 + t3632 + t3633 + t3634 + t3635 + t3636) * t485 +
         (t3647 * t147 + t3649 * t207 + t3651 * t210 + t3653 * t485 + t3643 + t3645 + t3646 + t3656) * t654 +
         (t3649 * t147 + t3647 * t207 + t3653 * t210 + t3651 * t485 + t3659 + t3660 + t3661 + t3667 + t3668) * t1098) *
            t2643;
    const double t3675 =
        t1726 + t1731 + t1738 + t1744 + t1818 +
        (t1820 + t1821 + t1823 + t1824 + t1825 + t1861 + (t1875 * t147 + t1862 + t1873) * t147) * t147 +
        (t1881 + t1882 + t1883 + t1884 + t1825 + t1905 + (t1906 + t1916 + t1919) * t147 +
         (t1875 * t207 + t1862 + t1919 + t1927) * t207) *
            t207 +
        (t1934 + t1935 + t1937 + t1938 + t1939 + t1975 + (t1976 + t1987 + t1990) * t147 +
         (t1993 + t2004 + t2007 + t2010) * t207 + (t2032 * t210 + t2013 + t2024 + t2027 + t2030) * t210) *
            t210 +
        (t2038 + t2039 + t2040 + t2041 + t1939 + t2062 + (t1993 + t2068 + t2069) * t147 +
         (t1976 + t2077 + t2007 + t2078) * t207 + (t2092 * t207 * t56 + t2081 + t2091 + t2094 + t2099) * t210 +
         (t2032 * t485 + t2013 + t2099 + t2107 + t2109 + t2111) * t485) *
            t485 +
        (t2124 + t2134 + t2151 + t2166 + t2188 +
         (t2189 + t2194 + t2199 + t2208 + t2215 + t2222 +
          (t2223 * t147 + t2226 + t2228 + t2229 + t2231 + t2232 + t2233) * t147) *
             t147 +
         (t2189 + t2240 + t2244 + t2247 + t2251 + t2256 +
          (t2258 + t2260 + t2262 + t2263 + t2264 + t2265 + t2266) * t147 +
          (t2223 * t207 + t2226 + t2233 + t2258 + t2270 + t2271 + t2272 + t2273) * t207) *
             t207 +
         (t2278 + t2283 + t2288 + t2297 + t2304 + t2311 +
          (t2313 + t2315 + t2317 + t2318 + t2320 + t2321 + t2322) * t147 +
          (t2326 + t2328 + t2330 + t2332 + t2333 + t2335 + t2336 + t2337) * t207 +
          (t2340 * t210 + t2343 + t2345 + t2347 + t2349 + t2350 + t2352 + t2353 + t2354) * t210) *
             t210 +
         (t2278 + t2361 + t2365 + t2368 + t2372 + t2377 +
          (t2378 + t2330 + t2379 + t2380 + t2381 + t2382 + t2337) * t147 +
          (t2385 + t2328 + t2315 + t2386 + t2387 + t2388 + t2389 + t2322) * t207 +
          (t2394 * t207 + t2393 + t2396 + t2398 + t2400 + t2401 + t2402 + t2403 + t2404) * t210 +
          (t2340 * t485 + t2347 + t2354 + t2393 + t2408 + t2409 + t2410 + t2411 + t2412 + t2413) * t485) *
             t485 +
         (t2419 + t2424 + t2431 + t2437 + (t2443 * t147 + t2439 + t2441 + t2442) * t147 +
          (t2450 * t147 + t2443 * t207 + t2447 + t2448 + t2449) * t207 +
          (t2464 * t210 + t2456 + t2458 + t2459 + t2461 + t2463) * t210 +
          (t2473 * t210 + t2464 * t485 + t2468 + t2469 + t2470 + t2471 + t2472) * t485) *
             t501) *
            t501 +
        t2779 * t654 + t2986 * t1098 + t3673 * t2643;
    const double t3683 = t2097 * t147 * t56;
    const double t3707 = t1917 * t210 * t56;
    const double t3720 = t147 * t2392;
    const double t3741 = t210 * t2257;
    const double t3772 = t2603 * t147 * t56;
    const double t3780 = t2554 * t210 * t56;
    const double t3788 = t147 * t2692;
    const double t3795 = t210 * t2660;
    const double t3804 = t2611 + t2616 + t2621 + t2630 + t2637 + t2644 +
                         (t2673 * t147 + t2680 + t2682 + t2683 + t2685 + t2686 + t2687) * t147 +
                         (t2690 * t207 + t2699 + t2701 + t2702 + t2704 + t2705 + t2706 + t3788) * t207 +
                         (t2645 * t210 + t2648 + t2650 + t2651 + t2653 + t2654 + t2655 + t2678 + t2884) * t210 +
                         (t2658 * t485 + t2663 + t2665 + t2666 + t2668 + t2669 + t2670 + t2695 + t2894 + t3795) * t485 +
                         (t2718 * t147 + t2720 * t207 + t2714 * t210 + t2716 * t485 + t2710 + t2712 + t2713) * t501;
    const double t3818 = t2754 * t147 + t2752 * t207 + t2758 * t210 + t2756 * t485 + t2751 + t2761 + t2763 + t2764 +
                         t2766 + t2767 + t2768;
    const double t3822 = t2483 + t2484 + t2486 + t2487 + t2488 + t2524 + (t2581 * t147 + t2562 + t2573) * t147 +
                         (t2607 * t207 + t2585 + t2596 + t3772) * t207 +
                         (t2538 * t210 + t2525 + t2536 + t2576 + t2833) * t210 +
                         (t2558 * t485 + t2542 + t2553 + t2602 + t2844 + t3780) * t485 + t3804 * t501 +
                         (t2744 * t147 * t56 + t2747 * t207 * t56 + t2738 * t210 * t56 + t2741 * t485 * t56 +
                          t3818 * t501 + t2726 + t2737 + t2776) *
                             t654;
    const double t3854 = t2611 + t2852 + t2856 + t2859 + t2863 + t2868 +
                         (t2690 * t147 + t2699 + t2706 + t2886 + t2887 + t2888 + t2889) * t147 +
                         (t2673 * t207 + t2680 + t2687 + t2895 + t2896 + t2897 + t2898 + t3788) * t207 +
                         (t2658 * t210 + t2663 + t2670 + t2676 + t2870 + t2871 + t2872 + t2873 + t2885) * t210 +
                         (t2645 * t485 + t2648 + t2655 + t2697 + t2877 + t2878 + t2879 + t2880 + t2893 + t3795) * t485 +
                         (t2720 * t147 + t2718 * t207 + t2716 * t210 + t2714 * t485 + t2901 + t2902 + t2903) * t501;
    const double t3866 = t2935 * t147 + t2935 * t207 + t2938 * t210 + t2938 * t485 + t2934 + t2942 + t2944 + t2945 +
                         t2946 + t2947 + t2948;
    const double t3882 = t2752 * t147 + t2754 * t207 + t2756 * t210 + t2758 * t485 + t2751 + t2761 + t2768 + t2977 +
                         t2978 + t2979 + t2980;
    const double t3886 =
        t2781 + t2782 + t2783 + t2784 + t2488 + t2805 + (t2607 * t147 + t2585 + t2829) * t147 +
        (t2581 * t207 + t2562 + t2842 + t3772) * t207 + (t2558 * t210 + t2542 + t2579 + t2811 + t2831) * t210 +
        (t2538 * t485 + t2525 + t2599 + t2820 + t2846 + t3780) * t485 + t3854 * t501 +
        (t2928 * t147 * t56 + t2923 * t210 * t56 + t2931 * t207 + t2926 * t485 + t3866 * t501 + t2912 + t2922 + t2956) *
            t654 +
        (t2747 * t147 * t56 + t2744 * t207 * t56 + t2741 * t210 * t56 + t2738 * t485 * t56 + t3882 * t501 + t2726 +
         t2956 + t2964 + t2983) *
            t1098;
    const double t3888 = a[150];
    const double t3889 = a[767];
    const double t3891 = a[234];
    const double t3896 = a[318];
    const double t3897 = t3 * t3896;
    const double t3898 = a[227];
    const double t3906 = a[701];
    const double t3907 = t3 * t3906;
    const double t3908 = a[162];
    const double t3911 = a[704];
    const double t3912 = t5 * t3911;
    const double t3913 = a[580];
    const double t3914 = t3 * t3913;
    const double t3915 = a[190];
    const double t3923 = t3 * t3911;
    const double t3926 = t5 * t3906;
    const double t3929 = t6 * t3896;
    const double t3938 = a[413];
    const double t3941 = a[419];
    const double t3946 = a[328];
    const double t3948 = a[894];
    const double t3960 = a[36];
    const double t3961 = a[525];
    const double t3963 = a[192];
    const double t3965 = (t3961 * t3 + t3963) * t3;
    const double t3967 = a[511];
    const double t3970 = (t3967 * t3 + t3961 * t5 + t3963) * t5;
    const double t3971 = a[532];
    const double t3973 = a[493];
    const double t3974 = t5 * t3973;
    const double t3975 = a[306];
    const double t3976 = t3 * t3975;
    const double t3977 = a[170];
    const double t3979 = (t3971 * t6 + t3974 + t3976 + t3977) * t6;
    const double t3981 = a[694];
    const double t3983 = t5 * t3975;
    const double t3984 = t3 * t3973;
    const double t3986 = (t3971 * t17 + t3981 * t6 + t3977 + t3983 + t3984) * t17;
    const double t3987 = a[451];
    const double t3989 = a[888];
    const double t3993 = (t3987 * t17 + t215 * t3989 + t3987 * t6) * t56;
    const double t3994 = a[864];
    const double t3997 = t56 * a[834];
    const double t3998 = a[443];
    const double t3999 = t17 * t3998;
    const double t4000 = t6 * t3998;
    const double t4001 = a[922];
    const double t4002 = t5 * t4001;
    const double t4003 = t3 * t4001;
    const double t4004 = a[154];
    const double t4011 = (t3971 * t3 + t3977) * t3;
    const double t4015 = (t3981 * t3 + t3971 * t5 + t3977) * t5;
    const double t4018 = (t3961 * t6 + t3963 + t3974 + t3976) * t6;
    const double t4022 = (t3961 * t17 + t3967 * t6 + t3963 + t3983 + t3984) * t17;
    const double t4027 = (t3989 * t17 + t215 * t3987 + t3989 * t6) * t56;
    const double t4028 = a[409];
    const double t4029 = t147 * t4028;
    const double t4031 = t56 * a[789];
    const double t4032 = a[292];
    const double t4033 = t17 * t4032;
    const double t4034 = t6 * t4032;
    const double t4035 = t5 * t4032;
    const double t4036 = t3 * t4032;
    const double t4037 = a[278];
    const double t4041 = t17 * t4001;
    const double t4042 = t6 * t4001;
    const double t4043 = t5 * t3998;
    const double t4044 = t3 * t3998;
    const double t4049 = a[389];
    const double t4050 = t147 * t4049;
    const double t4052 = t56 * a[415];
    const double t4053 = a[565];
    const double t4056 = a[797];
    const double t4059 = a[223];
    const double t4062 = a[688];
    const double t4063 = t207 * t4062;
    const double t4064 = a[687];
    const double t4065 = t147 * t4064;
    const double t4067 = t56 * a[502];
    const double t4068 = a[623];
    const double t4069 = t17 * t4068;
    const double t4070 = t6 * t4068;
    const double t4071 = t5 * t4068;
    const double t4072 = t3 * t4068;
    const double t4073 = a[267];
    const double t4081 = t147 * t4062;
    const double t4084 = t207 * t4049;
    const double t4091 = t210 * t4028;
    const double t4100 = a[761];
    const double t4103 = a[843];
    const double t4108 = a[900];
    const double t4110 = a[650];
    const double t4120 = a[780];
    const double t4121 = t4120 * t215;
    const double t4122 = a[561];
    const double t4123 = t4122 * t6;
    const double t4124 = t4122 * t17;
    const double t4125 = a[910];
    const double t4129 = t4120 * t6;
    const double t4130 = t4122 * t215;
    const double t4131 = t4120 * t17;
    const double t4132 = a[893];
    const double t4137 = a[524];
    const double t4139 = a[358];
    const double t4152 = a[68];
    const double t4153 = a[293];
    const double t4155 = a[248];
    const double t4159 = a[406];
    const double t4163 = a[667];
    const double t4165 = a[884];
    const double t4166 = t5 * t4165;
    const double t4167 = a[699];
    const double t4168 = t3 * t4167;
    const double t4169 = a[211];
    const double t4173 = a[304];
    const double t4175 = t5 * t4167;
    const double t4176 = t3 * t4165;
    const double t4179 = a[616];
    const double t4181 = a[848];
    const double t4186 = a[860];
    const double t4189 = a[677] * t56;
    const double t4190 = a[618];
    const double t4191 = t17 * t4190;
    const double t4192 = t6 * t4190;
    const double t4193 = a[399];
    const double t4194 = t5 * t4193;
    const double t4195 = t3 * t4193;
    const double t4196 = a[257];
    const double t4199 = a[799];
    const double t4201 = a[367];
    const double t4202 = t147 * t4201;
    const double t4204 = t56 * a[341];
    const double t4205 = a[691];
    const double t4206 = t17 * t4205;
    const double t4207 = t6 * t4205;
    const double t4208 = a[455];
    const double t4209 = t5 * t4208;
    const double t4210 = t3 * t4208;
    const double t4211 = a[254];
    const double t4215 = a[342];
    const double t4216 = t207 * t4215;
    const double t4217 = a[381];
    const double t4222 = t210 * t4201;
    const double t4223 = a[741];
    const double t4225 = t147 * t4215;
    const double t4228 = a[790];
    const double t4230 = a[705];
    const double t4233 = a[472];
    const double t4235 = a[439];
    const double t4241 = a[385];
    const double t4244 = t501 * a[813];
    const double t4245 = a[386];
    const double t4247 = a[771];
    const double t4252 = t56 * a[375];
    const double t4253 = a[334];
    const double t4256 = a[465];
    const double t4259 = a[260];
    const double t4260 = t4247 * t147 + t4253 * t17 + t4245 * t207 + t4247 * t210 + t4256 * t3 + t4241 * t654 +
                         t4245 * t485 + t4253 * t6 + t4256 * t5 + t4244 + t4252 + t4259;
    const double t4262 =
        t4152 + (t4153 * t3 + t4155) * t3 + (t4159 * t3 + t4153 * t5 + t4155) * t5 +
        (t4163 * t6 + t4166 + t4168 + t4169) * t6 + (t4163 * t17 + t4173 * t6 + t4169 + t4175 + t4176) * t17 +
        (t4179 * t17 + t215 * t4181 + t4179 * t6) * t56 +
        (t4186 * t147 + t4189 + t4191 + t4192 + t4194 + t4195 + t4196) * t147 +
        (t4199 * t207 + t4202 + t4204 + t4206 + t4207 + t4209 + t4210 + t4211) * t207 +
        (t4217 * t147 + t4186 * t210 + t4189 + t4191 + t4192 + t4194 + t4195 + t4196 + t4216) * t210 +
        (t4223 * t207 + t4199 * t485 + t4204 + t4206 + t4207 + t4209 + t4210 + t4211 + t4222 + t4225) * t485 +
        (t4233 * t147 + t4230 * t17 + t4235 * t207 + t4233 * t210 + t215 * t4228 + t4230 * t6 + t4235 * t485) * t501 +
        t4260 * t654;
    const double t4284 = t17 * t4208;
    const double t4285 = t6 * t4208;
    const double t4286 = t5 * t4205;
    const double t4287 = t3 * t4205;
    const double t4291 = t17 * t4193;
    const double t4292 = t6 * t4193;
    const double t4293 = t5 * t4190;
    const double t4294 = t3 * t4190;
    const double t4315 = t654 * a[321];
    const double t4318 = a[710];
    const double t4325 = a[787];
    const double t4331 = t4318 * t147 + t4325 * t17 + t4318 * t207 + t4318 * t210 + t4325 * t3 + t4318 * t485 +
                         t4325 * t5 + t4325 * t6 + t501 * a[535] + t56 * a[724] + t4315 + a[246];
    const double t4342 = t4241 * t1098 + t4245 * t147 + t4256 * t17 + t4247 * t207 + t4245 * t210 + t4253 * t3 +
                         t4247 * t485 + t4253 * t5 + t4256 * t6 + t4244 + t4252 + t4259 + t4315;
    const double t4344 =
        t4152 + (t4163 * t3 + t4169) * t3 + (t4173 * t3 + t4163 * t5 + t4169) * t5 +
        (t4153 * t6 + t4155 + t4166 + t4168) * t6 + (t4153 * t17 + t4159 * t6 + t4155 + t4175 + t4176) * t17 +
        (t4181 * t17 + t215 * t4179 + t4181 * t6) * t56 +
        (t4199 * t147 + t4204 + t4211 + t4284 + t4285 + t4286 + t4287) * t147 +
        (t4186 * t207 + t4189 + t4196 + t4202 + t4291 + t4292 + t4293 + t4294) * t207 +
        (t4223 * t147 + t4199 * t210 + t4204 + t4211 + t4216 + t4284 + t4285 + t4286 + t4287) * t210 +
        (t4217 * t207 + t4186 * t485 + t4189 + t4196 + t4222 + t4225 + t4291 + t4292 + t4293 + t4294) * t485 +
        (t4235 * t147 + t4228 * t17 + t4233 * t207 + t4235 * t210 + t215 * t4230 + t4228 * t6 + t4233 * t485) * t501 +
        t4331 * t654 + t4342 * t1098;
    const double t4346 = a[492];
    const double t4347 = t4346 * t125;
    const double t4349 = a[284];
    const double t4352 = (t4349 * t3 + t4346 * t5) * t5;
    const double t4354 = a[792];
    const double t4356 = a[809];
    const double t4359 = (t4356 * t3 + t4346 * t6 + t4354 * t5) * t6;
    const double t4365 = (t4346 * t17 + t4354 * t3 + t4349 * t6 + t4356 * t5) * t17;
    const double t4366 = a[533];
    const double t4367 = t4366 * t6;
    const double t4368 = a[852];
    const double t4369 = t4368 * t215;
    const double t4370 = t4366 * t17;
    const double t4371 = a[777];
    const double t4375 = t4366 * t215;
    const double t4376 = t4368 * t6;
    const double t4377 = t4368 * t17;
    const double t4378 = a[380];
    const double t4383 = a[319];
    const double t4384 = t4383 * t6;
    const double t4385 = a[664];
    const double t4386 = t4385 * t215;
    const double t4387 = t4383 * t17;
    const double t4388 = a[354];
    const double t4389 = t4388 * t147;
    const double t4390 = a[359];
    const double t4391 = t4390 * t207;
    const double t4392 = a[744];
    const double t4396 = t4383 * t215;
    const double t4397 = t4385 * t6;
    const double t4398 = t4385 * t17;
    const double t4399 = t4390 * t147;
    const double t4400 = t4388 * t207;
    const double t4401 = a[698];
    const double t4406 = a[606];
    const double t4407 = t4406 * t6;
    const double t4408 = a[840];
    const double t4409 = t4408 * t215;
    const double t4410 = t4406 * t17;
    const double t4411 = a[495];
    const double t4413 = a[326];
    const double t4415 = a[783];
    const double t4417 = a[287];
    const double t4419 = a[903];
    const double t4420 = t4419 * t654;
    const double t4423 = t4406 * t215;
    const double t4424 = t4408 * t6;
    const double t4425 = t4408 * t17;
    const double t4431 = a[480] * t654;
    const double t4432 = t4419 * t1098;
    const double t4437 =
        (t3888 + (t3889 * t3 + t3891) * t3) * t3 +
        (t3888 + (t3897 + t3898) * t3 + (t3889 * t5 + t3891 + t3897) * t5) * t5 +
        (t3888 + (t3907 + t3908) * t3 + (t3912 + t3914 + t3915) * t5 + (t3889 * t6 + t3891 + t3907 + t3912) * t6) * t6 +
        (t3888 + (t3923 + t3915) * t3 + (t3926 + t3914 + t3908) * t5 + (t3913 * t5 + t3898 + t3914 + t3929) * t6 +
         (t3889 * t17 + t3891 + t3923 + t3926 + t3929) * t17) *
            t17 +
        (t3938 * t125 + (t3941 * t3 + t3938 * t5) * t5 + (t3948 * t3 + t3938 * t6 + t3946 * t5) * t6 +
         (t3938 * t17 + t3946 * t3 + t3941 * t6 + t3948 * t5) * t17) *
            t56 +
        (t3960 + t3965 + t3970 + t3979 + t3986 + t3993 +
         (t3994 * t147 + t3997 + t3999 + t4000 + t4002 + t4003 + t4004) * t147) *
            t147 +
        (t3960 + t4011 + t4015 + t4018 + t4022 + t4027 +
         (t4029 + t4031 + t4033 + t4034 + t4035 + t4036 + t4037) * t147 +
         (t3994 * t207 + t3997 + t4004 + t4029 + t4041 + t4042 + t4043 + t4044) * t207) *
            t207 +
        (t3960 + t3965 + t3970 + t3979 + t3986 + t3993 +
         (t4053 * t17 + t4056 * t3 + t4053 * t6 + t4056 * t5 + t4050 + t4052 + t4059) * t147 +
         (t4063 + t4065 + t4067 + t4069 + t4070 + t4071 + t4072 + t4073) * t207 +
         (t3994 * t210 + t3997 + t3999 + t4000 + t4002 + t4003 + t4004 + t4050 + t4063) * t210) *
            t210 +
        (t3960 + t4011 + t4015 + t4018 + t4022 + t4027 +
         (t4081 + t4067 + t4069 + t4070 + t4071 + t4072 + t4073) * t147 +
         (t4056 * t17 + t4053 * t3 + t4053 * t5 + t4056 * t6 + t4052 + t4059 + t4065 + t4084) * t207 +
         (t4064 * t207 + t4031 + t4033 + t4034 + t4035 + t4036 + t4037 + t4065 + t4091) * t210 +
         (t3994 * t485 + t3997 + t4004 + t4041 + t4042 + t4043 + t4044 + t4081 + t4084 + t4091) * t485) *
            t485 +
        (t4100 * t125 + (t4103 * t3 + t4100 * t5) * t5 + (t4110 * t3 + t4100 * t6 + t4108 * t5) * t6 +
         (t4100 * t17 + t4108 * t3 + t4103 * t6 + t4110 * t5) * t17 + (t4125 * t147 + t4121 + t4123 + t4124) * t147 +
         (t4132 * t147 + t4125 * t207 + t4129 + t4130 + t4131) * t207 +
         (t4137 * t147 + t4139 * t207 + t4125 * t210 + t4121 + t4123 + t4124) * t210 +
         (t4139 * t147 + t4137 * t207 + t4132 * t210 + t4125 * t485 + t4129 + t4130 + t4131) * t485) *
            t501 +
        t4262 * t654 + t4344 * t1098 +
        (t4347 + t4352 + t4359 + t4365 + (t4371 * t147 + t4367 + t4369 + t4370) * t147 +
         (t4378 * t147 + t4371 * t207 + t4375 + t4376 + t4377) * t207 +
         (t4392 * t210 + t4384 + t4386 + t4387 + t4389 + t4391) * t210 +
         (t4401 * t210 + t4392 * t485 + t4396 + t4397 + t4398 + t4399 + t4400) * t485 +
         (t4411 * t147 + t4413 * t207 + t4415 * t210 + t4417 * t485 + t4407 + t4409 + t4410 + t4420) * t654 +
         (t4413 * t147 + t4411 * t207 + t4417 * t210 + t4415 * t485 + t4423 + t4424 + t4425 + t4431 + t4432) * t1098) *
            t2643;
    const double t4444 = t147 * t3263;
    const double t4465 = t210 * t3128;
    const double t4494 = t147 * t3432;
    const double t4501 = t210 * t3400;
    const double t4514 = t3470 * t147 + t3468 * t207 + t3474 * t210 + t3472 * t485 + t3465 + t3467 + t3477 + t3479 +
                         t3480 + t3482 + t3483 + t3484;
    const double t4516 = t3351 + t3356 + t3361 + t3370 + t3377 + t3384 +
                         (t3413 * t147 + t3420 + t3422 + t3423 + t3425 + t3426 + t3427) * t147 +
                         (t3430 * t207 + t3439 + t3441 + t3442 + t3444 + t3445 + t3446 + t4494) * t207 +
                         (t3385 * t210 + t3388 + t3390 + t3391 + t3393 + t3394 + t3395 + t3418 + t3523) * t210 +
                         (t3398 * t485 + t3403 + t3405 + t3406 + t3408 + t3409 + t3410 + t3435 + t3533 + t4501) * t485 +
                         (t3458 * t147 + t3460 * t207 + t3454 * t210 + t3456 * t485 + t3450 + t3452 + t3453) * t501 +
                         t4514 * t654;
    const double t4540 = t3553 * t147 + t3553 * t207 + t3556 * t210 + t3556 * t485 + t3550 + t3552 + t3560 + t3562 +
                         t3563 + t3564 + t3565 + t3566;
    const double t4546 = t3468 * t147 + t3470 * t207 + t3472 * t210 + t3474 * t485 + t3467 + t3477 + t3484 + t3550 +
                         t3569 + t3574 + t3575 + t3576 + t3577;
    const double t4548 = t3351 + t3491 + t3495 + t3498 + t3502 + t3507 +
                         (t3430 * t147 + t3439 + t3446 + t3525 + t3526 + t3527 + t3528) * t147 +
                         (t3413 * t207 + t3420 + t3427 + t3534 + t3535 + t3536 + t3537 + t4494) * t207 +
                         (t3398 * t210 + t3403 + t3410 + t3416 + t3509 + t3510 + t3511 + t3512 + t3524) * t210 +
                         (t3385 * t485 + t3388 + t3395 + t3437 + t3516 + t3517 + t3518 + t3519 + t3532 + t4501) * t485 +
                         (t3460 * t147 + t3458 * t207 + t3456 * t210 + t3454 * t485 + t3540 + t3541 + t3542) * t501 +
                         t4540 * t654 + t4546 * t1098;
    const double t3880 = x[0];
    const double t4606 =
        t2995 + t3005 + t3022 + t3037 + t3059 +
        (t3149 + t3154 + t3159 + t3168 + t3175 + t3182 +
         (t3211 * t147 + t3218 + t3220 + t3221 + t3223 + t3224 + t3225) * t147) *
            t147 +
        (t3149 + t3232 + t3236 + t3239 + t3243 + t3248 +
         (t4444 + t3269 + t3271 + t3272 + t3273 + t3274 + t3275) * t147 +
         (t3211 * t207 + t3218 + t3225 + t3281 + t3282 + t3283 + t3284 + t4444) * t207) *
            t207 +
        (t3060 + t3065 + t3070 + t3079 + t3086 + t3093 +
         (t3216 + t3186 + t3188 + t3189 + t3191 + t3192 + t3193) * t147 +
         (t3214 + t3267 + t3201 + t3250 + t3251 + t3252 + t3253 + t3208) * t207 +
         (t3094 * t210 + t3097 + t3099 + t3100 + t3102 + t3103 + t3104 + t3184 + t3197) * t210) *
            t210 +
        (t3060 + t3111 + t3115 + t3118 + t3122 + t3127 +
         (t3280 + t3201 + t3203 + t3204 + t3206 + t3207 + t3208) * t147 +
         (t3279 + t3267 + t3186 + t3257 + t3258 + t3259 + t3260 + t3193) * t207 +
         (t3198 * t207 + t3131 + t3133 + t3134 + t3135 + t3136 + t3137 + t3199 + t4465) * t210 +
         (t3094 * t485 + t3097 + t3104 + t3141 + t3142 + t3143 + t3144 + t3249 + t3256 + t4465) * t485) *
            t485 +
        (t3290 + t3295 + t3302 + t3308 + (t3335 * t147 + t3327 + t3329 + t3330) * t147 +
         (t3344 * t147 + t3335 * t207 + t3339 + t3340 + t3341) * t207 +
         (t3314 * t210 + t3310 + t3312 + t3313 + t3332 + t3334) * t210 +
         (t3321 * t210 + t3314 * t485 + t3318 + t3319 + t3320 + t3342 + t3343) * t485) *
            t501 +
        t4516 * t654 + t4548 * t1098 +
        (t4347 + t4352 + t4359 + t4365 + (t4392 * t147 + t4384 + t4386 + t4387) * t147 +
         (t4401 * t147 + t4392 * t207 + t4396 + t4397 + t4398) * t207 +
         (t4371 * t210 + t4367 + t4369 + t4370 + t4389 + t4391) * t210 +
         (t4378 * t210 + t4371 * t485 + t4375 + t4376 + t4377 + t4399 + t4400) * t485 +
         (t4415 * t147 + t4417 * t207 + t4411 * t210 + t4413 * t485 + t4407 + t4409 + t4410 + t4420) * t654 +
         (t4417 * t147 + t4415 * t207 + t4413 * t210 + t4411 * t485 + t4423 + t4424 + t4425 + t4431 + t4432) * t1098) *
            t2643 +
        (t3583 + t3588 + t3595 + t3601 + (t3628 * t147 + t3620 + t3622 + t3623) * t147 +
         (t3637 * t147 + t3628 * t207 + t3632 + t3633 + t3634) * t207 +
         (t3607 * t210 + t3603 + t3605 + t3606 + t3625 + t3627) * t210 +
         (t3614 * t210 + t3607 * t485 + t3611 + t3612 + t3613 + t3635 + t3636) * t485 +
         (t3651 * t147 + t3653 * t207 + t3647 * t210 + t3649 * t485 + t3643 + t3645 + t3646 + t3656) * t654 +
         (t3653 * t147 + t3651 * t207 + t3649 * t210 + t3647 * t485 + t3659 + t3660 + t3661 + t3667 + t3668) * t1098) *
            t3880;
    const double t4608 =
        t1726 + t1731 + t1738 + t1744 + t1818 +
        (t1934 + t1935 + t1937 + t1938 + t1939 + t1975 + (t2032 * t147 + t2013 + t2024) * t147) * t147 +
        (t2038 + t2039 + t2040 + t2041 + t1939 + t2062 + (t2081 + t2091 + t3683) * t147 +
         (t2032 * t207 + t2013 + t2107 + t3683) * t207) *
            t207 +
        (t1820 + t1821 + t1823 + t1824 + t1825 + t1861 + (t1976 + t1987 + t2027) * t147 +
         (t1993 + t2068 + t2094 + t2030) * t207 + (t1875 * t210 + t1862 + t1873 + t1990 + t2010) * t210) *
            t210 +
        (t1881 + t1882 + t1883 + t1884 + t1825 + t1905 + (t1993 + t2004 + t2109) * t147 +
         (t1976 + t2077 + t2094 + t2111) * t207 + (t2005 * t207 * t56 + t1906 + t1916 + t2007 + t3707) * t210 +
         (t1875 * t485 + t1862 + t1927 + t2069 + t2078 + t3707) * t485) *
            t485 +
        (t2124 + t2134 + t2151 + t2166 + t2188 +
         (t2278 + t2283 + t2288 + t2297 + t2304 + t2311 +
          (t2340 * t147 + t2347 + t2349 + t2350 + t2352 + t2353 + t2354) * t147) *
             t147 +
         (t2278 + t2361 + t2365 + t2368 + t2372 + t2377 +
          (t3720 + t2398 + t2400 + t2401 + t2402 + t2403 + t2404) * t147 +
          (t2340 * t207 + t2347 + t2354 + t2410 + t2411 + t2412 + t2413 + t3720) * t207) *
             t207 +
         (t2189 + t2194 + t2199 + t2208 + t2215 + t2222 +
          (t2345 + t2315 + t2317 + t2318 + t2320 + t2321 + t2322) * t147 +
          (t2343 + t2396 + t2330 + t2379 + t2380 + t2381 + t2382 + t2337) * t207 +
          (t2223 * t210 + t2226 + t2228 + t2229 + t2231 + t2232 + t2233 + t2313 + t2326) * t210) *
             t210 +
         (t2189 + t2240 + t2244 + t2247 + t2251 + t2256 +
          (t2409 + t2330 + t2332 + t2333 + t2335 + t2336 + t2337) * t147 +
          (t2408 + t2396 + t2315 + t2386 + t2387 + t2388 + t2389 + t2322) * t207 +
          (t2327 * t207 + t2260 + t2262 + t2263 + t2264 + t2265 + t2266 + t2328 + t3741) * t210 +
          (t2223 * t485 + t2226 + t2233 + t2270 + t2271 + t2272 + t2273 + t2378 + t2385 + t3741) * t485) *
             t485 +
         (t2419 + t2424 + t2431 + t2437 + (t2464 * t147 + t2456 + t2458 + t2459) * t147 +
          (t2473 * t147 + t2464 * t207 + t2468 + t2469 + t2470) * t207 +
          (t2443 * t210 + t2439 + t2441 + t2442 + t2461 + t2463) * t210 +
          (t2450 * t210 + t2443 * t485 + t2447 + t2448 + t2449 + t2471 + t2472) * t485) *
             t501) *
            t501 +
        t3822 * t654 + t3886 * t1098 + t4437 * t2643 + t4606 * t3880;
    return (
        (t1 + (t2 * t3 + t4) * t3) * t3 + (t1 + (t10 + t11) * t3 + (t2 * t5 + t10 + t4) * t5) * t5 +
        (t1 + (t20 + t21) * t3 + (t25 + t27 + t28) * t5 + (t2 * t6 + t20 + t25 + t4) * t6) * t6 +
        (t1 + (t36 + t28) * t3 + (t39 + t27 + t21) * t5 + (t26 * t5 + t11 + t27 + t42) * t6 +
         (t2 * t17 + t36 + t39 + t4 + t42) * t17) *
            t17 +
        ((t51 * t3 + t53) * t3 + (t57 * t3 + t51 * t5 + t53) * t5 + (t64 * t3 + t62 * t5 + t51 * t6 + t53) * t6 +
         (t51 * t17 + t62 * t3 + t64 * t5 + t57 * t6 + t53) * t17 +
         ((t74 + (t75 * t3 + t77) * t3) * t3 + (t74 + (t83 + t84) * t3 + (t75 * t5 + t77 + t83) * t5) * t5 +
          (t74 + (t93 + t94) * t3 + (t98 + t100 + t101) * t5 + (t75 * t6 + t77 + t93 + t98) * t6) * t6 +
          (t74 + (t109 + t101) * t3 + (t112 + t100 + t94) * t5 + (t99 * t5 + t100 + t115 + t84) * t6 +
           (t75 * t17 + t109 + t112 + t115 + t77) * t17) *
              t17 +
          (t124 * t125 + (t124 * t5 + t128 * t3) * t5 + (t124 * t6 + t133 * t5 + t135 * t3) * t6 +
           (t124 * t17 + t128 * t6 + t133 * t3 + t135 * t5) * t17) *
              t56) *
             t56) *
            t56 +
        (t151 + t156 + t161 + t170 + t177 + t223 + (t249 * t147 + t225 + t226 + t228 + t229 + t230 + t244) * t147) *
            t147 +
        (t151 + t257 + t261 + t264 + t268 + t295 + (t297 + t298 + t299 + t300 + t301 + t314 + t319) * t147 +
         (t249 * t207 + t230 + t319 + t322 + t323 + t324 + t325 + t333) * t207) *
            t207 +
        (t151 + t156 + t161 + t170 + t177 + t223 +
         (t339 * t17 + t339 * t6 + t342 * t5 + t342 * t3 + t345 +
          (t346 + (t349 * t17 + t352 * t3 + t349 * t6 + t352 * t5 + t348 + t355) * t56) * t56 + t364) *
             t147 +
         (t368 + t369 + t370 + t371 + t372 + t385 + t390 + t395) * t207 +
         (t249 * t210 + t225 + t226 + t228 + t229 + t230 + t244 + t364 + t395) * t210) *
            t210 +
        (t151 + t257 + t261 + t264 + t268 + t295 + (t368 + t369 + t370 + t371 + t372 + t385 + t403) * t147 +
         (t342 * t17 + t342 * t6 + t339 * t5 + t339 * t3 + t345 +
          (t346 + (t352 * t17 + t349 * t3 + t349 * t5 + t352 * t6 + t348 + t355) * t56) * t56 + t390 + t418) *
             t207 +
         (t389 * t207 + t297 + t298 + t299 + t300 + t301 + t314 + t390 + t422) * t210 +
         (t249 * t485 + t230 + t322 + t323 + t324 + t325 + t333 + t403 + t418 + t422) * t485) *
            t485 +
        ((t430 * t3 + t432) * t3 + (t436 * t3 + t430 * t5 + t432) * t5 +
         (t443 * t3 + t430 * t6 + t441 * t5 + t432) * t6 +
         (t430 * t17 + t441 * t3 + t436 * t6 + t443 * t5 + t432) * t17 +
         ((t453 + (t454 * t3 + t456) * t3) * t3 + (t453 + (t462 + t463) * t3 + (t454 * t5 + t456 + t462) * t5) * t5 +
          (t453 + (t472 + t473) * t3 + (t477 + t479 + t480) * t5 + (t454 * t6 + t456 + t472 + t477) * t6) * t6 +
          (t453 + (t488 + t480) * t3 + (t491 + t479 + t473) * t5 + (t478 * t5 + t463 + t479 + t494) * t6 +
           (t454 * t17 + t456 + t488 + t491 + t494) * t17) *
              t17 +
          (t503 * t125 + (t506 * t3 + t503 * t5) * t5 + (t513 * t3 + t511 * t5 + t503 * t6) * t6 +
           (t503 * t17 + t511 * t3 + t513 * t5 + t506 * t6) * t17) *
              t56) *
             t56 +
         (t528 + t529 + t531 + t532 + t533 + t569 + (t583 * t147 + t570 + t581) * t147) * t147 +
         (t589 + t590 + t591 + t592 + t533 + t613 + (t614 + t624 + t627) * t147 +
          (t583 * t207 + t570 + t627 + t635) * t207) *
             t207 +
         (t528 + t529 + t531 + t532 + t533 + t569 +
          (t641 + (t644 * t17 + t647 * t3 + t647 * t5 + t644 * t6 + t643 + t650) * t56 + t655) * t147 +
          (t658 + t668 + t671 + t674) * t207 + (t583 * t210 + t570 + t581 + t655 + t674) * t210) *
             t210 +
         (t589 + t590 + t591 + t592 + t533 + t613 + (t658 + t668 + t683) * t147 +
          (t641 + (t647 * t17 + t644 * t3 + t644 * t5 + t647 * t6 + t643 + t650) * t56 + t671 + t693) * t207 +
          (t669 * t207 * t56 + t614 + t624 + t671 + t699) * t210 +
          (t583 * t485 + t570 + t635 + t683 + t693 + t699) * t485) *
             t485 +
         ((t707 + (t708 * t3 + t710) * t3) * t3 + (t707 + (t716 + t717) * t3 + (t708 * t5 + t710 + t716) * t5) * t5 +
          (t707 + (t726 + t727) * t3 + (t731 + t733 + t734) * t5 + (t708 * t6 + t710 + t726 + t731) * t6) * t6 +
          (t707 + (t742 + t734) * t3 + (t745 + t733 + t727) * t5 + (t732 * t5 + t717 + t733 + t748) * t6 +
           (t708 * t17 + t710 + t742 + t745 + t748) * t17) *
              t17 +
          (t757 * t125 + (t760 * t3 + t757 * t5) * t5 + (t767 * t3 + t765 * t5 + t757 * t6) * t6 +
           (t757 * t17 + t765 * t3 + t767 * t5 + t760 * t6) * t17) *
              t56 +
          (t779 + t784 + t789 + t798 + t805 + t812 + (t813 * t147 + t816 + t818 + t819 + t821 + t822 + t823) * t147) *
              t147 +
          (t779 + t830 + t834 + t837 + t841 + t846 + (t848 + t850 + t852 + t853 + t854 + t855 + t856) * t147 +
           (t813 * t207 + t816 + t823 + t848 + t860 + t861 + t862 + t863) * t207) *
              t207 +
          (t779 + t784 + t789 + t798 + t805 + t812 +
           (t872 * t17 + t875 * t3 + t875 * t5 + t872 * t6 + t869 + t871 + t878) * t147 +
           (t882 + t884 + t886 + t888 + t889 + t890 + t891 + t892) * t207 +
           (t813 * t210 + t816 + t818 + t819 + t821 + t822 + t823 + t869 + t882) * t210) *
              t210 +
          (t779 + t830 + t834 + t837 + t841 + t846 + (t900 + t886 + t888 + t889 + t890 + t891 + t892) * t147 +
           (t875 * t17 + t872 * t3 + t872 * t5 + t875 * t6 + t871 + t878 + t884 + t903) * t207 +
           (t883 * t207 + t850 + t852 + t853 + t854 + t855 + t856 + t884 + t910) * t210 +
           (t813 * t485 + t816 + t823 + t860 + t861 + t862 + t863 + t900 + t903 + t910) * t485) *
              t485 +
          (t919 * t125 + (t922 * t3 + t919 * t5) * t5 + (t929 * t3 + t927 * t5 + t919 * t6) * t6 +
           (t919 * t17 + t927 * t3 + t929 * t5 + t922 * t6) * t17 + (t944 * t147 + t940 + t942 + t943) * t147 +
           (t951 * t147 + t944 * t207 + t948 + t949 + t950) * t207 +
           (t956 * t147 + t958 * t207 + t944 * t210 + t940 + t942 + t943) * t210 +
           (t958 * t147 + t956 * t207 + t951 * t210 + t944 * t485 + t948 + t949 + t950) * t485) *
              t501) *
             t501) *
            t501 +
        t1412 * t654 + t1720 * t1098 + t3675 * t2643 + t4608 * t3880);
}

}  // namespace mbnrg_A1B2Z2_C2_deg5
