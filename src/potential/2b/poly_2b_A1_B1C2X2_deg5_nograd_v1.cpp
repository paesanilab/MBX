
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
 * @file poly_2b_A1_B1C2X2_deg5_nograd_v1.cpp
 * @brief Contains the implementation of the polynomials without gradients for symmetry A1_B1C2X2
 */

/**
 * @namespace mbnrg_A1_B1C2X2_deg5
 * @brief Encloses the structure of the polynomial for symmetry A1_B1C2X2
 */

namespace mbnrg_A1_B1C2X2_deg5 {

double poly_A1_B1C2X2_deg5_v1::eval(const double x[8], const double a[496]) {
    const double t1 = a[2];
    const double t5 = x[7];
    const double t12 = (a[14] + (a[19] + (t5 * a[359] + a[84]) * t5) * t5) * t5;
    const double t13 = a[12];
    const double t21 = (a[34] + (t5 * a[350] + a[182]) * t5) * t5;
    const double t22 = a[62];
    const double t27 = (a[232] * t5 + a[127]) * t5;
    const double t28 = a[407];
    const double t31 = t5 * a[375];
    const double t32 = a[128];
    const double t33 = x[6];
    const double t38 = (t13 + t21 + (t22 + t27 + (t28 * t33 + t31 + t32) * t33) * t33) * t33;
    const double t46 = t33 * a[273];
    const double t48 = t5 * a[419];
    const double t49 = a[176];
    const double t60 = x[5];
    const double t64 = (t13 + t21 + (a[54] + (t5 * a[246] + a[150]) * t5 + (t46 + t48 + t49) * t33) * t33 +
                        (t22 + t27 + (t33 * a[249] + t48 + t49) * t33 + (t28 * t60 + t31 + t32 + t46) * t60) * t60) *
                       t60;
    const double t65 = a[15];
    const double t73 = (a[23] + (t5 * a[292] + a[173]) * t5) * t5;
    const double t74 = a[56];
    const double t79 = (t5 * a[423] + a[186]) * t5;
    const double t80 = a[314];
    const double t83 = t5 * a[204];
    const double t84 = a[121];
    const double t88 = (t74 + t79 + (t33 * t80 + t83 + t84) * t33) * t33;
    const double t90 = t33 * a[201];
    const double t100 = (t74 + t79 + (t5 * a[368] + t90 + a[171]) * t33 + (t60 * t80 + t83 + t84 + t90) * t60) * t60;
    const double t101 = a[33];
    const double t106 = (t5 * a[348] + a[130]) * t5;
    const double t107 = a[217];
    const double t110 = t5 * a[304];
    const double t111 = a[183];
    const double t113 = (t107 * t33 + t110 + t111) * t33;
    const double t118 = (t107 * t60 + t33 * a[250] + t110 + t111) * t60;
    const double t119 = a[390];
    const double t121 = a[416];
    const double t122 = t60 * t121;
    const double t123 = t33 * t121;
    const double t125 = t5 * a[483];
    const double t126 = a[125];
    const double t144 = a[65];
    const double t149 = (t5 * a[289] + a[139]) * t5;
    const double t150 = a[261];
    const double t153 = t5 * a[208];
    const double t154 = a[168];
    const double t160 = t33 * a[282];
    const double t171 = a[31];
    const double t176 = (t5 * a[220] + a[102]) * t5;
    const double t177 = a[357];
    const double t180 = t5 * a[227];
    const double t181 = a[158];
    const double t183 = (t177 * t33 + t180 + t181) * t33;
    const double t188 = (t177 * t60 + t33 * a[281] + t180 + t181) * t60;
    const double t147 = x[4];
    const double t190 = t147 * a[248];
    const double t191 = a[456];
    const double t192 = t60 * t191;
    const double t193 = t33 * t191;
    const double t195 = t5 * a[276];
    const double t196 = a[113];
    const double t204 = t147 * a[262];
    const double t205 = a[448];
    const double t226 = a[1];
    const double t237 = (a[17] + (a[39] + (t5 * a[365] + a[187]) * t5) * t5) * t5;
    const double t238 = a[3];
    const double t246 = (a[20] + (t5 * a[396] + a[133]) * t5) * t5;
    const double t247 = a[50];
    const double t252 = (t5 * a[494] + a[79]) * t5;
    const double t253 = a[453];
    const double t256 = t5 * a[216];
    const double t257 = a[78];
    const double t264 = a[13];
    const double t272 = (a[27] + (t5 * a[230] + a[87]) * t5) * t5;
    const double t273 = a[22];
    const double t278 = (t5 * a[280] + a[179]) * t5;
    const double t280 = t33 * a[428];
    const double t282 = a[490] * t5;
    const double t283 = a[89];
    const double t288 = a[38];
    const double t293 = (t5 * a[242] + a[109]) * t5;
    const double t295 = t33 * a[306];
    const double t297 = t5 * a[272];
    const double t298 = a[123];
    const double t301 = a[450];
    const double t304 = t33 * a[197];
    const double t306 = a[438] * t5;
    const double t307 = a[165];
    const double t314 = a[5];
    const double t322 = (a[55] + (t5 * a[219] + a[145]) * t5) * t5;
    const double t323 = a[46];
    const double t328 = (t5 * a[222] + a[169]) * t5;
    const double t329 = a[334];
    const double t332 = t5 * a[203];
    const double t333 = a[140];
    const double t337 = (t323 + t328 + (t329 * t33 + t332 + t333) * t33) * t33;
    const double t338 = a[45];
    const double t343 = (t5 * a[210] + a[177]) * t5;
    const double t345 = t33 * a[436];
    const double t347 = t5 * a[400];
    const double t348 = a[80];
    const double t351 = a[410];
    const double t354 = t33 * a[331];
    const double t356 = a[347] * t5;
    const double t357 = a[196];
    const double t361 = (t338 + t343 + (t345 + t347 + t348) * t33 + (t351 * t60 + t354 + t356 + t357) * t60) * t60;
    const double t362 = a[57];
    const double t367 = (t5 * a[366] + a[115]) * t5;
    const double t368 = a[377];
    const double t371 = t5 * a[425];
    const double t372 = a[166];
    const double t374 = (t33 * t368 + t371 + t372) * t33;
    const double t375 = a[279];
    const double t378 = t33 * a[325];
    const double t380 = t5 * a[299];
    const double t381 = a[190];
    const double t383 = (t375 * t60 + t378 + t380 + t381) * t60;
    const double t384 = a[462];
    const double t385 = t147 * t384;
    const double t386 = a[266];
    const double t387 = t60 * t386;
    const double t388 = a[286];
    const double t389 = t33 * t388;
    const double t391 = t5 * a[259];
    const double t392 = a[184];
    const double t399 = a[28];
    const double t404 = (t5 * a[245] + a[104]) * t5;
    const double t405 = a[247];
    const double t408 = t5 * a[412];
    const double t409 = a[124];
    const double t412 = a[443];
    const double t415 = t33 * a[381];
    const double t417 = t5 * a[393];
    const double t418 = a[98];
    const double t422 = t147 * a[327];
    const double t423 = a[488];
    const double t424 = t60 * t423;
    const double t425 = a[360];
    const double t426 = t33 * t425;
    const double t428 = t5 * a[356];
    const double t429 = a[172];
    const double t435 = t147 * a[312];
    const double t279 = x[3];
    const double t438 = t279 * t384;
    const double t445 = a[8];
    const double t453 = (a[26] + (t5 * a[209] + a[185]) * t5) * t5;
    const double t454 = a[49];
    const double t459 = (t5 * a[484] + a[154]) * t5;
    const double t460 = a[485];
    const double t463 = t5 * a[316];
    const double t464 = a[157];
    const double t469 = a[59];
    const double t474 = (t5 * a[355] + a[143]) * t5;
    const double t476 = t33 * a[385];
    const double t478 = a[472] * t5;
    const double t479 = a[100];
    const double t482 = a[376];
    const double t485 = t33 * a[373];
    const double t487 = t5 * a[215];
    const double t488 = a[67];
    const double t493 = a[60];
    const double t498 = (t5 * a[302] + a[85]) * t5;
    const double t499 = a[205];
    const double t502 = t5 * a[294];
    const double t503 = a[119];
    const double t505 = (t33 * t499 + t502 + t503) * t33;
    const double t506 = a[402];
    const double t509 = t33 * a[409];
    const double t511 = t5 * a[344];
    const double t512 = a[93];
    const double t514 = (t506 * t60 + t509 + t511 + t512) * t60;
    const double t515 = a[401];
    const double t516 = t147 * t515;
    const double t517 = a[237];
    const double t518 = t60 * t517;
    const double t519 = a[421];
    const double t520 = t33 * t519;
    const double t522 = t5 * a[482];
    const double t523 = a[164];
    const double t529 = t147 * a[198];
    const double t530 = a[337];
    const double t532 = a[214];
    const double t535 = t5 * a[445];
    const double t536 = a[101];
    const double t539 = t279 * t515;
    const double t544 = a[29];
    const double t549 = (t5 * a[447] + a[170]) * t5;
    const double t550 = a[271];
    const double t553 = t5 * a[223];
    const double t554 = a[137];
    const double t557 = a[211];
    const double t560 = t33 * a[429];
    const double t562 = t5 * a[431];
    const double t563 = a[120];
    const double t566 = a[406];
    const double t567 = t147 * t566;
    const double t568 = a[463];
    const double t569 = t60 * t568;
    const double t570 = a[341];
    const double t571 = t33 * t570;
    const double t573 = t5 * a[466];
    const double t574 = a[95];
    const double t577 = t279 * t566;
    const double t579 = t147 * a[486];
    const double t582 = a[233];
    const double t584 = a[267];
    const double t585 = t279 * t584;
    const double t586 = t147 * t584;
    const double t587 = a[455];
    const double t589 = a[310];
    const double t592 = a[458] * t5;
    const double t593 = a[161];
    const double t626 = (t338 + t343 + (t33 * t351 + t356 + t357) * t33) * t33;
    const double t633 = (t323 + t328 + (t354 + t347 + t348) * t33 + (t329 * t60 + t332 + t333 + t345) * t60) * t60;
    const double t636 = (t33 * t375 + t380 + t381) * t33;
    const double t639 = (t368 * t60 + t371 + t372 + t378) * t60;
    const double t640 = t60 * t388;
    const double t641 = t33 * t386;
    const double t654 = t60 * t425;
    const double t655 = t33 * t423;
    const double t677 = a[66];
    const double t682 = (t5 * a[315] + a[73]) * t5;
    const double t683 = a[293];
    const double t686 = t5 * a[389];
    const double t687 = a[175];
    const double t693 = t33 * a[351];
    const double t704 = a[41];
    const double t709 = (t5 * a[397] + a[83]) * t5;
    const double t710 = a[411];
    const double t713 = t5 * a[224];
    const double t714 = a[195];
    const double t716 = (t33 * t710 + t713 + t714) * t33;
    const double t721 = (t33 * a[343] + t60 * t710 + t713 + t714) * t60;
    const double t722 = a[342];
    const double t724 = a[226];
    const double t725 = t60 * t724;
    const double t726 = t33 * t724;
    const double t728 = a[471] * t5;
    const double t729 = a[118];
    const double t735 = t147 * a[465];
    const double t736 = a[420];
    const double t749 = a[24];
    const double t754 = (t5 * a[340] + a[108]) * t5;
    const double t755 = a[452];
    const double t758 = t5 * a[417];
    const double t759 = a[86];
    const double t762 = a[405];
    const double t765 = t33 * a[251];
    const double t767 = t5 * a[330];
    const double t768 = a[146];
    const double t771 = a[378];
    const double t772 = t147 * t771;
    const double t773 = a[200];
    const double t774 = t60 * t773;
    const double t775 = a[369];
    const double t776 = t33 * t775;
    const double t778 = t5 * a[446];
    const double t779 = a[188];
    const double t782 = t279 * t771;
    const double t784 = t147 * a[332];
    const double t432 = x[2];
    const double t788 = a[481] * t432;
    const double t789 = a[358];
    const double t790 = t279 * t789;
    const double t791 = t147 * t789;
    const double t792 = a[457];
    const double t794 = a[491];
    const double t797 = t5 * a[313];
    const double t798 = a[72];
    const double t819 = (t33 * t506 + t511 + t512) * t33;
    const double t822 = (t499 * t60 + t502 + t503 + t509) * t60;
    const double t823 = t60 * t519;
    const double t824 = t33 * t517;
    const double t843 = t60 * t775;
    const double t844 = t33 * t773;
    const double t850 = t432 * a[329];
    const double t851 = a[258];
    const double t854 = a[221];
    const double t870 = t60 * t570;
    const double t871 = t33 * t568;
    const double t903 = a[4];
    const double t911 = (a[61] + (t5 * a[374] + a[94]) * t5) * t5;
    const double t912 = a[35];
    const double t917 = (t5 * a[319] + a[141]) * t5;
    const double t918 = a[442];
    const double t921 = t5 * a[353];
    const double t922 = a[189];
    const double t936 = t33 * a[384];
    const double t938 = t5 * a[243];
    const double t939 = a[147];
    const double t955 = a[7];
    const double t963 = (a[21] + (t5 * a[404] + a[76]) * t5) * t5;
    const double t964 = a[30];
    const double t969 = (t5 * a[413] + a[181]) * t5;
    const double t970 = a[270];
    const double t973 = t5 * a[268];
    const double t974 = a[193];
    const double t978 = (t964 + t969 + (t33 * t970 + t973 + t974) * t33) * t33;
    const double t980 = t33 * a[444];
    const double t990 =
        (t964 + t969 + (t5 * a[335] + t980 + a[75]) * t33 + (t60 * t970 + t973 + t974 + t980) * t60) * t60;
    const double t991 = a[37];
    const double t996 = (a[399] * t5 + a[163]) * t5;
    const double t997 = a[467];
    const double t1000 = t5 * a[307];
    const double t1001 = a[138];
    const double t1003 = (t33 * t997 + t1000 + t1001) * t33;
    const double t1008 = (t33 * a[333] + t60 * t997 + t1000 + t1001) * t60;
    const double t1009 = a[206];
    const double t1011 = a[422];
    const double t1012 = t60 * t1011;
    const double t1013 = t33 * t1011;
    const double t1015 = a[454] * t5;
    const double t1016 = a[131];
    const double t1029 = a[388];
    const double t1032 = t5 * a[433];
    const double t1033 = a[92];
    const double t1042 = a[395] * t147;
    const double t1043 = a[394];
    const double t1044 = t60 * t1043;
    const double t1045 = t33 * t1043;
    const double t1047 = a[469] * t5;
    const double t1048 = a[111];
    const double t1064 = a[16];
    const double t1072 = (a[43] + (t5 * a[345] + a[74]) * t5) * t5;
    const double t1073 = a[53];
    const double t1078 = (t5 * a[487] + a[144]) * t5;
    const double t1079 = a[234];
    const double t1082 = t5 * a[263];
    const double t1083 = a[97];
    const double t1088 = a[64];
    const double t1093 = (t5 * a[244] + a[149]) * t5;
    const double t1095 = t33 * a[241];
    const double t1097 = a[321] * t5;
    const double t1098 = a[135];
    const double t1101 = a[274];
    const double t1104 = t33 * a[240];
    const double t1106 = t5 * a[391];
    const double t1107 = a[132];
    const double t1112 = a[36];
    const double t1117 = (t5 * a[324] + a[77]) * t5;
    const double t1118 = a[476];
    const double t1121 = t5 * a[328];
    const double t1122 = a[99];
    const double t1124 = (t1118 * t33 + t1121 + t1122) * t33;
    const double t1125 = a[440];
    const double t1128 = t33 * a[277];
    const double t1130 = t5 * a[403];
    const double t1131 = a[122];
    const double t1133 = (t1125 * t60 + t1128 + t1130 + t1131) * t60;
    const double t1134 = a[474];
    const double t1135 = t147 * t1134;
    const double t1136 = a[207];
    const double t1137 = t60 * t1136;
    const double t1138 = a[430];
    const double t1139 = t33 * t1138;
    const double t1141 = t5 * a[298];
    const double t1142 = a[96];
    const double t1148 = t147 * a[460];
    const double t1149 = a[288];
    const double t1151 = a[265];
    const double t1154 = t5 * a[297];
    const double t1155 = a[142];
    const double t1158 = t279 * t1134;
    const double t1163 = a[40];
    const double t1168 = (t5 * a[252] + a[191]) * t5;
    const double t1169 = a[479];
    const double t1172 = t5 * a[269];
    const double t1173 = a[174];
    const double t1176 = a[386];
    const double t1179 = t33 * a[382];
    const double t1181 = t5 * a[202];
    const double t1182 = a[90];
    const double t1185 = a[477];
    const double t1186 = t147 * t1185;
    const double t1187 = a[228];
    const double t1188 = t60 * t1187;
    const double t1189 = a[434];
    const double t1190 = t33 * t1189;
    const double t1192 = t5 * a[354];
    const double t1193 = a[153];
    const double t1196 = t279 * t1185;
    const double t1198 = t147 * a[451];
    const double t1201 = a[432];
    const double t1203 = a[470];
    const double t1204 = t279 * t1203;
    const double t1205 = t147 * t1203;
    const double t1206 = a[493];
    const double t1208 = a[238];
    const double t1211 = t5 * a[320];
    const double t1212 = a[114];
    const double t1233 = (t1125 * t33 + t1130 + t1131) * t33;
    const double t1236 = (t1118 * t60 + t1121 + t1122 + t1128) * t60;
    const double t1237 = t60 * t1138;
    const double t1238 = t33 * t1136;
    const double t1257 = a[278];
    const double t1260 = t5 * a[370];
    const double t1261 = a[116];
    const double t1269 = a[424];
    const double t1271 = a[480];
    const double t1272 = t60 * t1271;
    const double t1273 = t33 * t1271;
    const double t1275 = a[489] * t5;
    const double t1276 = a[70];
    const double t1285 = a[414] * t432;
    const double t1286 = a[468];
    const double t1287 = t279 * t1286;
    const double t1288 = t147 * t1286;
    const double t1289 = a[254];
    const double t1291 = a[379];
    const double t1294 = t5 * a[346];
    const double t1295 = a[167];
    const double t1306 = t60 * t1189;
    const double t1307 = t33 * t1187;
    const double t1336 = a[18];
    const double t1341 = (t5 * a[199] + a[192]) * t5;
    const double t1342 = a[367];
    const double t1345 = t5 * a[285];
    const double t1346 = a[155];
    const double t1352 = t33 * a[408];
    const double t1363 = a[47];
    const double t1368 = (t5 * a[473] + a[126]) * t5;
    const double t1369 = a[441];
    const double t1372 = t5 * a[235];
    const double t1373 = a[103];
    const double t1375 = (t1369 * t33 + t1372 + t1373) * t33;
    const double t1380 = (t1369 * t60 + t33 * a[257] + t1372 + t1373) * t60;
    const double t1381 = a[305];
    const double t1383 = a[361];
    const double t1384 = t60 * t1383;
    const double t1385 = t33 * t1383;
    const double t1387 = t5 * a[398];
    const double t1388 = a[107];
    const double t1394 = a[492] * t147;
    const double t1395 = a[349];
    const double t1408 = a[63];
    const double t1413 = (t5 * a[253] + a[91]) * t5;
    const double t1414 = a[352];
    const double t1417 = t5 * a[380];
    const double t1418 = a[129];
    const double t1421 = a[303];
    const double t1424 = t33 * a[435];
    const double t1426 = t5 * a[364];
    const double t1427 = a[69];
    const double t1430 = a[475];
    const double t1431 = t147 * t1430;
    const double t1432 = a[212];
    const double t1433 = t60 * t1432;
    const double t1434 = a[284];
    const double t1435 = t33 * t1434;
    const double t1437 = t5 * a[229];
    const double t1438 = a[159];
    const double t1441 = t279 * t1430;
    const double t1443 = t147 * a[295];
    const double t1446 = a[255];
    const double t1448 = a[308];
    const double t1449 = t279 * t1448;
    const double t1450 = t147 * t1448;
    const double t1451 = a[311];
    const double t1453 = a[239];
    const double t1456 = t5 * a[231];
    const double t1457 = a[152];
    const double t1468 = t60 * t1434;
    const double t1469 = t33 * t1432;
    const double t1475 = a[260] * t432;
    const double t1476 = a[291];
    const double t1479 = a[439];
    const double t1500 = a[236];
    const double t1503 = t5 * a[296];
    const double t1504 = a[112];
    const double t1512 = a[218];
    const double t1514 = a[371];
    const double t1515 = t60 * t1514;
    const double t1516 = t33 * t1514;
    const double t1518 = a[437] * t5;
    const double t1519 = a[82];
    const double t1527 = a[363];
    const double t1529 = a[283];
    const double t1530 = t279 * t1529;
    const double t1531 = t147 * t1529;
    const double t1532 = a[449];
    const double t1534 = a[339];
    const double t1537 = a[478] * t5;
    const double t1538 = a[88];
    const double t1550 = a[290];
    const double t1553 = a[338];
    const double t1556 = a[372];
    const double t1069 = x[1];
    const double t1565 = x[0];
    return (
        (t1 + t12 + t38 + t64 +
         (t65 + t73 + t88 + t100 +
          (t101 + t106 + t113 + t118 + (t119 * t147 + t122 + t123 + t125 + t126) * t147) * t147) *
             t147) *
            t147 +
        (t1 + t12 + t38 + t64 +
         (a[9] + (a[42] + (t5 * a[323] + a[156]) * t5) * t5 + (t144 + t149 + (t150 * t33 + t153 + t154) * t33) * t33 +
          (t144 + t149 + (t5 * a[392] + t160 + a[178]) * t33 + (t150 * t60 + t153 + t154 + t160) * t60) * t60 +
          (t171 + t176 + t183 + t188 + (t190 + t192 + t193 + t195 + t196) * t147) * t147) *
             t147 +
         (t65 + t73 + t88 + t100 +
          (t171 + t176 + t183 + t188 + (t205 * t33 + t205 * t60 + t5 * a[415] + t204 + a[117]) * t147) * t147 +
          (t101 + t106 + t113 + t118 + (t204 + t192 + t193 + t195 + t196) * t147 +
           (t119 * t279 + t122 + t123 + t125 + t126 + t190) * t279) *
              t279) *
             t279) *
            t279 +
        (t226 + t237 + (t238 + t246 + (t247 + t252 + (t253 * t33 + t256 + t257) * t33) * t33) * t33 +
         (t264 + t272 + (t273 + t278 + (t280 + t282 + t283) * t33) * t33 +
          (t288 + t293 + (t295 + t297 + t298) * t33 + (t301 * t60 + t304 + t306 + t307) * t60) * t60) *
             t60 +
         (t314 + t322 + t337 + t361 + (t362 + t367 + t374 + t383 + (t385 + t387 + t389 + t391 + t392) * t147) * t147) *
             t147 +
         (t314 + t322 + t337 + t361 +
          (t399 + t404 + (t33 * t405 + t408 + t409) * t33 + (t412 * t60 + t415 + t417 + t418) * t60 +
           (t422 + t424 + t426 + t428 + t429) * t147) *
              t147 +
          (t362 + t367 + t374 + t383 + (t435 + t424 + t426 + t428 + t429) * t147 +
           (t438 + t422 + t387 + t389 + t391 + t392) * t279) *
              t279) *
             t279 +
         (t445 + t453 + (t454 + t459 + (t33 * t460 + t463 + t464) * t33) * t33 +
          (t469 + t474 + (t476 + t478 + t479) * t33 + (t482 * t60 + t485 + t487 + t488) * t60) * t60 +
          (t493 + t498 + t505 + t514 + (t516 + t518 + t520 + t522 + t523) * t147) * t147 +
          (t493 + t498 + t505 + t514 + (t33 * t532 + t530 * t60 + t529 + t535 + t536) * t147 +
           (t539 + t529 + t518 + t520 + t522 + t523) * t279) *
              t279 +
          (t544 + t549 + (t33 * t550 + t553 + t554) * t33 + (t557 * t60 + t560 + t562 + t563) * t60 +
           (t567 + t569 + t571 + t573 + t574) * t147 + (t577 + t579 + t569 + t571 + t573 + t574) * t279 +
           (t33 * t589 + t432 * t582 + t587 * t60 + t585 + t586 + t592 + t593) * t432) *
              t432) *
             t432) *
            t432 +
        (t226 + t237 + (t264 + t272 + (t288 + t293 + (t301 * t33 + t306 + t307) * t33) * t33) * t33 +
         (t238 + t246 + (t273 + t278 + (t304 + t297 + t298) * t33) * t33 +
          (t247 + t252 + (t295 + t282 + t283) * t33 + (t253 * t60 + t256 + t257 + t280) * t60) * t60) *
             t60 +
         (t314 + t322 + t626 + t633 + (t362 + t367 + t636 + t639 + (t385 + t640 + t641 + t391 + t392) * t147) * t147) *
             t147 +
         (t314 + t322 + t626 + t633 +
          (t399 + t404 + (t33 * t412 + t417 + t418) * t33 + (t405 * t60 + t408 + t409 + t415) * t60 +
           (t422 + t654 + t655 + t428 + t429) * t147) *
              t147 +
          (t362 + t367 + t636 + t639 + (t435 + t654 + t655 + t428 + t429) * t147 +
           (t438 + t422 + t640 + t641 + t391 + t392) * t279) *
              t279) *
             t279 +
         (a[6] + (a[51] + (t5 * a[362] + a[151]) * t5) * t5 + (t677 + t682 + (t33 * t683 + t686 + t687) * t33) * t33 +
          (t677 + t682 + (t5 * a[336] + t693 + a[194]) * t33 + (t60 * t683 + t686 + t687 + t693) * t60) * t60 +
          (t704 + t709 + t716 + t721 + (t147 * t722 + t725 + t726 + t728 + t729) * t147) * t147 +
          (t704 + t709 + t716 + t721 + (t33 * t736 + t5 * a[426] + t60 * t736 + t735 + a[162]) * t147 +
           (t279 * t722 + t725 + t726 + t728 + t729 + t735) * t279) *
              t279 +
          (t749 + t754 + (t33 * t755 + t758 + t759) * t33 + (t60 * t762 + t765 + t767 + t768) * t60 +
           (t772 + t774 + t776 + t778 + t779) * t147 + (t782 + t784 + t774 + t776 + t778 + t779) * t279 +
           (t33 * t794 + t60 * t792 + t788 + t790 + t791 + t797 + t798) * t432) *
              t432) *
             t432 +
         (t445 + t453 + (t469 + t474 + (t33 * t482 + t487 + t488) * t33) * t33 +
          (t454 + t459 + (t485 + t478 + t479) * t33 + (t460 * t60 + t463 + t464 + t476) * t60) * t60 +
          (t493 + t498 + t819 + t822 + (t516 + t823 + t824 + t522 + t523) * t147) * t147 +
          (t493 + t498 + t819 + t822 + (t33 * t530 + t532 * t60 + t529 + t535 + t536) * t147 +
           (t539 + t529 + t823 + t824 + t522 + t523) * t279) *
              t279 +
          (t749 + t754 + (t33 * t762 + t767 + t768) * t33 + (t60 * t755 + t758 + t759 + t765) * t60 +
           (t772 + t843 + t844 + t778 + t779) * t147 + (t782 + t784 + t843 + t844 + t778 + t779) * t279 +
           (t147 * t851 + t279 * t851 + t33 * t854 + t5 * a[326] + t60 * t854 + t850 + a[180]) * t432) *
              t432 +
          (t544 + t549 + (t33 * t557 + t562 + t563) * t33 + (t550 * t60 + t553 + t554 + t560) * t60 +
           (t567 + t870 + t871 + t573 + t574) * t147 + (t577 + t579 + t870 + t871 + t573 + t574) * t279 +
           (t33 * t792 + t60 * t794 + t790 + t791 + t797 + t798 + t850) * t432 +
           (t1069 * t582 + t33 * t587 + t589 * t60 + t585 + t586 + t592 + t593 + t788) * t1069) *
              t1069) *
             t1069) *
            t1069 +
        (a[0] + (a[10] + (a[48] + (t5 * a[213] + a[68]) * t5) * t5) * t5 +
         (t903 + t911 + (t912 + t917 + (t33 * t918 + t921 + t922) * t33) * t33) * t33 +
         (t903 + t911 + (a[32] + (t5 * a[275] + a[81]) * t5 + (t936 + t938 + t939) * t33) * t33 +
          (t912 + t917 + (t33 * a[301] + t938 + t939) * t33 + (t60 * t918 + t921 + t922 + t936) * t60) * t60) *
             t60 +
         (t955 + t963 + t978 + t990 +
          (t991 + t996 + t1003 + t1008 + (t1009 * t147 + t1012 + t1013 + t1015 + t1016) * t147) * t147) *
             t147 +
         (t955 + t963 + t978 + t990 +
          (a[44] + (a[459] * t5 + a[105]) * t5 + (t1029 * t33 + t1032 + t1033) * t33 +
           (t1029 * t60 + t33 * a[318] + t1032 + t1033) * t60 + (t1042 + t1044 + t1045 + t1047 + t1048) * t147) *
              t147 +
          (t991 + t996 + t1003 + t1008 + (t147 * a[256] + t1044 + t1045 + t1047 + t1048) * t147 +
           (t1009 * t279 + t1012 + t1013 + t1015 + t1016 + t1042) * t279) *
              t279) *
             t279 +
         (t1064 + t1072 + (t1073 + t1078 + (t1079 * t33 + t1082 + t1083) * t33) * t33 +
          (t1088 + t1093 + (t1095 + t1097 + t1098) * t33 + (t1101 * t60 + t1104 + t1106 + t1107) * t60) * t60 +
          (t1112 + t1117 + t1124 + t1133 + (t1135 + t1137 + t1139 + t1141 + t1142) * t147) * t147 +
          (t1112 + t1117 + t1124 + t1133 + (t1149 * t60 + t1151 * t33 + t1148 + t1154 + t1155) * t147 +
           (t1158 + t1148 + t1137 + t1139 + t1141 + t1142) * t279) *
              t279 +
          (t1163 + t1168 + (t1169 * t33 + t1172 + t1173) * t33 + (t1176 * t60 + t1179 + t1181 + t1182) * t60 +
           (t1186 + t1188 + t1190 + t1192 + t1193) * t147 + (t1196 + t1198 + t1188 + t1190 + t1192 + t1193) * t279 +
           (t1201 * t432 + t1206 * t60 + t1208 * t33 + t1204 + t1205 + t1211 + t1212) * t432) *
              t432) *
             t432 +
         (t1064 + t1072 + (t1088 + t1093 + (t1101 * t33 + t1106 + t1107) * t33) * t33 +
          (t1073 + t1078 + (t1104 + t1097 + t1098) * t33 + (t1079 * t60 + t1082 + t1083 + t1095) * t60) * t60 +
          (t1112 + t1117 + t1233 + t1236 + (t1135 + t1237 + t1238 + t1141 + t1142) * t147) * t147 +
          (t1112 + t1117 + t1233 + t1236 + (t1149 * t33 + t1151 * t60 + t1148 + t1154 + t1155) * t147 +
           (t1158 + t1148 + t1237 + t1238 + t1141 + t1142) * t279) *
              t279 +
          (a[52] + (t5 * a[300] + a[71]) * t5 + (t1257 * t33 + t1260 + t1261) * t33 +
           (t1257 * t60 + t33 * a[418] + t1260 + t1261) * t60 + (t1269 * t147 + t1272 + t1273 + t1275 + t1276) * t147 +
           (t1269 * t279 + t147 * a[287] + t1272 + t1273 + t1275 + t1276) * t279 +
           (t1289 * t60 + t1291 * t33 + t1285 + t1287 + t1288 + t1294 + t1295) * t432) *
              t432 +
          (t1163 + t1168 + (t1176 * t33 + t1181 + t1182) * t33 + (t1169 * t60 + t1172 + t1173 + t1179) * t60 +
           (t1186 + t1306 + t1307 + t1192 + t1193) * t147 + (t1196 + t1198 + t1306 + t1307 + t1192 + t1193) * t279 +
           (t1289 * t33 + t1291 * t60 + t432 * a[495] + t1287 + t1288 + t1294 + t1295) * t432 +
           (t1069 * t1201 + t1206 * t33 + t1208 * t60 + t1204 + t1205 + t1211 + t1212 + t1285) * t1069) *
              t1069) *
             t1069 +
         (a[11] + (a[58] + (t5 * a[464] + a[160]) * t5) * t5 +
          (t1336 + t1341 + (t1342 * t33 + t1345 + t1346) * t33) * t33 +
          (t1336 + t1341 + (t5 * a[322] + t1352 + a[106]) * t33 + (t1342 * t60 + t1345 + t1346 + t1352) * t60) * t60 +
          (t1363 + t1368 + t1375 + t1380 + (t1381 * t147 + t1384 + t1385 + t1387 + t1388) * t147) * t147 +
          (t1363 + t1368 + t1375 + t1380 + (t1395 * t33 + t1395 * t60 + t5 * a[383] + t1394 + a[136]) * t147 +
           (t1381 * t279 + t1384 + t1385 + t1387 + t1388 + t1394) * t279) *
              t279 +
          (t1408 + t1413 + (t1414 * t33 + t1417 + t1418) * t33 + (t1421 * t60 + t1424 + t1426 + t1427) * t60 +
           (t1431 + t1433 + t1435 + t1437 + t1438) * t147 + (t1441 + t1443 + t1433 + t1435 + t1437 + t1438) * t279 +
           (t1446 * t432 + t1451 * t60 + t1453 * t33 + t1449 + t1450 + t1456 + t1457) * t432) *
              t432 +
          (t1408 + t1413 + (t1421 * t33 + t1426 + t1427) * t33 + (t1414 * t60 + t1417 + t1418 + t1424) * t60 +
           (t1431 + t1468 + t1469 + t1437 + t1438) * t147 + (t1441 + t1443 + t1468 + t1469 + t1437 + t1438) * t279 +
           (t147 * t1476 + t1476 * t279 + t1479 * t33 + t1479 * t60 + t5 * a[387] + t1475 + a[110]) * t432 +
           (t1069 * t1446 + t1451 * t33 + t1453 * t60 + t1449 + t1450 + t1456 + t1457 + t1475) * t1069) *
              t1069 +
          (a[25] + (t5 * a[309] + a[148]) * t5 + (t1500 * t33 + t1503 + t1504) * t33 +
           (t1500 * t60 + t33 * a[461] + t1503 + t1504) * t60 + (t147 * t1512 + t1515 + t1516 + t1518 + t1519) * t147 +
           (t147 * a[264] + t1512 * t279 + t1515 + t1516 + t1518 + t1519) * t279 +
           (t1527 * t432 + t1532 * t60 + t1534 * t33 + t1530 + t1531 + t1537 + t1538) * t432 +
           (t1069 * t1527 + t1532 * t33 + t1534 * t60 + t432 * a[317] + t1530 + t1531 + t1537 + t1538) * t1069 +
           (t1069 * t1550 + t147 * t1553 + t1550 * t432 + t1553 * t279 + t1556 * t33 + t1556 * t60 + t1565 * a[225] +
            t5 * a[427] + a[134]) *
               t1565) *
              t1565) *
             t1565) *
            t1565);
}

}  // namespace mbnrg_A1_B1C2X2_deg5
