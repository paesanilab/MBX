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

std::vector<double> poly_2b_h2o_ion_v1x::eval(const size_t nd, const double a[429], const double* x) {
    std::vector<double> energy(nd, 0.0);
#pragma omp simd
    for (size_t nv = 0; nv < nd; nv++) {
        const double t1 = a[2];
        const double t2 = a[5];
        const double t3 = a[64];
        const double t4 = a[412];
        const double t6 = a[151];
        const double t16 = a[31];
        const double t5 = x[nv + nd * 7];
        const double t18 = t5 * a[404];
        const double t19 = a[174];
        const double t27 = a[311] * t5;
        const double t45 = a[10];
        const double t46 = a[50];
        const double t47 = a[285];
        const double t49 = a[125];
        const double t58 = t5 * a[293];
        const double t59 = a[160];
        const double t76 = a[56];
        const double t77 = a[314];
        const double t79 = a[145];
        const double t85 = t5 * a[271];
        const double t95 = a[422];
        const double t97 = a[122];
        const double t107 = a[269];
        const double t119 = a[0];
        const double t120 = a[3];
        const double t121 = a[22];
        const double t122 = a[323];
        const double t124 = a[126];
        const double t130 = (t120 + (t121 + (t122 * t5 + t124) * t5) * t5) * t5;
        const double t133 = t5 * a[359];
        const double t134 = a[155];
        const double t31 = x[nv + nd * 6];
        const double t149 = (t120 + (a[46] + (t133 + t134) * t5) * t5 +
                             (t121 + (t5 * a[215] + t134) * t5 + (t122 * t31 + t124 + t133) * t31) * t31) *
                            t31;
        const double t151 = a[51];
        const double t152 = a[196];
        const double t154 = a[110];
        const double t160 = t5 * a[189];
        const double t170 = a[258];
        const double t172 = a[153];
        const double t182 = a[330];
        const double t71 = x[nv + nd * 5];
        const double t191 = (a[13] + (t151 + (t152 * t5 + t154) * t5) * t5 +
                             (t151 + (t160 + a[176]) * t5 + (t152 * t31 + t154 + t160) * t31) * t31 +
                             (a[23] + (t170 * t5 + t172) * t5 + (t170 * t31 + t5 * a[275] + t172) * t31 +
                              (t182 * t31 + t182 * t5 + t71 * a[277] + a[158]) * t71) *
                                 t71) *
                            t71;
        const double t192 = a[7];
        const double t193 = a[32];
        const double t194 = a[409];
        const double t196 = a[71];
        const double t200 = (t193 + (t194 * t5 + t196) * t5) * t5;
        const double t202 = t5 * a[329];
        const double t210 = (t193 + (t202 + a[177]) * t5 + (t194 * t31 + t196 + t202) * t31) * t31;
        const double t212 = a[249];
        const double t214 = a[140];
        const double t224 = a[245];
        const double t231 = (a[36] + (t212 * t5 + t214) * t5 + (t212 * t31 + t5 * a[242] + t214) * t31 +
                             (t224 * t31 + t224 * t5 + t71 * a[210] + a[83]) * t71) *
                            t71;
        const double t232 = a[62];
        const double t233 = a[233];
        const double t235 = a[69];
        const double t237 = (t233 * t5 + t235) * t5;
        const double t242 = (t233 * t31 + t5 * a[373] + t235) * t31;
        const double t245 = a[357];
        const double t250 = (t245 * t31 + t245 * t5 + a[335] * t71 + a[113]) * t71;
        const double t251 = a[227];
        const double t254 = a[307] * t71;
        const double t255 = a[402];
        const double t256 = t31 * t255;
        const double t257 = t5 * t255;
        const double t258 = a[85];
        const double t268 = a[49];
        const double t269 = a[349];
        const double t271 = a[136];
        const double t277 = t5 * a[235];
        const double t287 = a[395];
        const double t289 = a[183];
        const double t299 = a[366];
        const double t307 = a[43];
        const double t308 = a[186];
        const double t310 = a[84];
        const double t312 = (t308 * t5 + t310) * t5;
        const double t317 = (t308 * t31 + t5 * a[250] + t310) * t31;
        const double t320 = a[226];
        const double t325 = (t320 * t31 + t320 * t5 + a[370] * t71 + a[101]) * t71;
        const double t153 = x[nv + nd * 4];
        const double t327 = a[309] * t153;
        const double t329 = a[244] * t71;
        const double t330 = a[354];
        const double t331 = t31 * t330;
        const double t332 = t5 * t330;
        const double t333 = a[161];
        const double t341 = a[302] * t153;
        const double t344 = a[383];
        const double t363 = a[9];
        const double t364 = a[40];
        const double t365 = a[251];
        const double t367 = a[132];
        const double t373 = (t363 + (t364 + (t365 * t5 + t367) * t5) * t5) * t5;
        const double t376 = t5 * a[203];
        const double t377 = a[103];
        const double t392 = (t363 + (a[59] + (t376 + t377) * t5) * t5 +
                             (t364 + (t5 * a[333] + t377) * t5 + (t365 * t31 + t367 + t376) * t31) * t31) *
                            t31;
        const double t394 = a[26];
        const double t395 = a[274];
        const double t397 = a[128];
        const double t403 = t5 * a[312];
        const double t413 = a[350];
        const double t415 = a[70];
        const double t425 = a[268];
        const double t434 = (a[6] + (t394 + (t395 * t5 + t397) * t5) * t5 +
                             (t394 + (t403 + a[91]) * t5 + (t395 * t31 + t397 + t403) * t31) * t31 +
                             (a[24] + (t413 * t5 + t415) * t5 + (t413 * t31 + t5 * a[288] + t415) * t31 +
                              (t425 * t31 + t425 * t5 + a[361] * t71 + a[180]) * t71) *
                                 t71) *
                            t71;
        const double t435 = a[8];
        const double t436 = a[37];
        const double t437 = a[263];
        const double t439 = a[141];
        const double t443 = (t436 + (t437 * t5 + t439) * t5) * t5;
        const double t445 = t5 * a[264];
        const double t453 = (t436 + (t445 + a[150]) * t5 + (t437 * t31 + t439 + t445) * t31) * t31;
        const double t455 = a[241];
        const double t457 = a[134];
        const double t467 = a[192];
        const double t474 = (a[25] + (t455 * t5 + t457) * t5 + (t455 * t31 + t5 * a[400] + t457) * t31 +
                             (t467 * t31 + t467 * t5 + a[372] * t71 + a[119]) * t71) *
                            t71;
        const double t475 = a[47];
        const double t476 = a[403];
        const double t478 = a[147];
        const double t480 = (t476 * t5 + t478) * t5;
        const double t485 = (t476 * t31 + a[389] * t5 + t478) * t31;
        const double t488 = a[344];
        const double t493 = (t488 * t31 + t488 * t5 + a[201] * t71 + a[165]) * t71;
        const double t494 = a[382];
        const double t497 = a[188] * t71;
        const double t498 = a[207];
        const double t499 = t31 * t498;
        const double t500 = t5 * t498;
        const double t501 = a[114];
        const double t508 = a[14];
        const double t509 = a[45];
        const double t510 = a[255];
        const double t512 = a[76];
        const double t516 = (t509 + (t510 * t5 + t512) * t5) * t5;
        const double t518 = t5 * a[193];
        const double t526 = (t509 + (t518 + a[146]) * t5 + (t510 * t31 + t512 + t518) * t31) * t31;
        const double t528 = a[342];
        const double t530 = a[72];
        const double t540 = a[221];
        const double t547 = (a[18] + (t528 * t5 + t530) * t5 + (t528 * t31 + t5 * a[351] + t530) * t31 +
                             (t540 * t31 + t540 * t5 + t71 * a[213] + a[112]) * t71) *
                            t71;
        const double t548 = a[41];
        const double t549 = a[326];
        const double t551 = a[105];
        const double t553 = (t549 * t5 + t551) * t5;
        const double t558 = (t549 * t31 + t5 * a[286] + t551) * t31;
        const double t561 = a[202];
        const double t566 = (t561 * t31 + t561 * t5 + t71 * a[287] + a[173]) * t71;
        const double t568 = a[316] * t153;
        const double t570 = a[220] * t71;
        const double t571 = a[347];
        const double t572 = t31 * t571;
        const double t573 = t5 * t571;
        const double t574 = a[137];
        const double t579 = a[30];
        const double t580 = a[336];
        const double t582 = a[102];
        const double t584 = (t580 * t5 + t582) * t5;
        const double t589 = (t580 * t31 + t5 * a[228] + t582) * t31;
        const double t592 = a[219];
        const double t597 = (t592 * t31 + t592 * t5 + t71 * a[304] + a[168]) * t71;
        const double t599 = a[423] * t153;
        const double t601 = a[416] * t71;
        const double t602 = a[322];
        const double t603 = t31 * t602;
        const double t604 = t5 * t602;
        const double t605 = a[159];
        const double t608 = a[337];
        const double t611 = a[289] * t153;
        const double t613 = a[247] * t71;
        const double t614 = a[290];
        const double t615 = t31 * t614;
        const double t616 = t5 * t614;
        const double t617 = a[82];
        const double t624 = a[35];
        const double t625 = a[246];
        const double t627 = a[123];
        const double t631 = (t624 + (t625 * t5 + t627) * t5) * t5;
        const double t633 = t5 * a[345];
        const double t641 = (t624 + (t633 + a[98]) * t5 + (t625 * t31 + t627 + t633) * t31) * t31;
        const double t643 = a[338];
        const double t645 = a[121];
        const double t655 = a[320];
        const double t662 = (a[39] + (t643 * t5 + t645) * t5 + (t643 * t31 + t5 * a[310] + t645) * t31 +
                             (t655 * t31 + t655 * t5 + t71 * a[406] + a[170]) * t71) *
                            t71;
        const double t663 = a[58];
        const double t664 = a[325];
        const double t666 = a[144];
        const double t668 = (t664 * t5 + t666) * t5;
        const double t673 = (t664 * t31 + t5 * a[331] + t666) * t31;
        const double t676 = a[214];
        const double t681 = (t676 * t31 + t676 * t5 + t71 * a[223] + a[99]) * t71;
        const double t682 = a[256];
        const double t685 = a[376] * t71;
        const double t686 = a[232];
        const double t687 = t31 * t686;
        const double t688 = t5 * t686;
        const double t689 = a[133];
        const double t694 = a[52];
        const double t695 = a[300];
        const double t697 = a[67];
        const double t699 = (t695 * t5 + t697) * t5;
        const double t704 = (t695 * t31 + t5 * a[388] + t697) * t31;
        const double t707 = a[399];
        const double t712 = (t707 * t31 + t707 * t5 + t71 * a[261] + a[178]) * t71;
        const double t714 = a[391] * t153;
        const double t716 = a[278] * t71;
        const double t717 = a[356];
        const double t718 = t717 * t31;
        const double t719 = t717 * t5;
        const double t720 = a[74];
        const double t723 = a[362];
        const double t726 = a[428] * t153;
        const double t728 = a[392] * t71;
        const double t729 = a[204];
        const double t730 = t729 * t31;
        const double t731 = t729 * t5;
        const double t732 = a[118];
        const double t761 = a[27];
        const double t762 = a[321];
        const double t764 = a[68];
        const double t770 = a[317] * t5;
        const double t780 = a[292];
        const double t782 = a[115];
        const double t792 = a[236];
        const double t800 = a[20];
        const double t801 = a[216];
        const double t803 = a[117];
        const double t805 = (t801 * t5 + t803) * t5;
        const double t810 = (t801 * t31 + t5 * a[339] + t803) * t31;
        const double t813 = a[303];
        const double t818 = (t813 * t31 + t813 * t5 + t71 * a[360] + a[138]) * t71;
        const double t819 = a[248];
        const double t822 = a[296] * t71;
        const double t823 = a[230];
        const double t824 = t823 * t31;
        const double t825 = t823 * t5;
        const double t826 = a[109];
        const double t832 = a[421] * t153;
        const double t835 = a[358];
        const double t864 = a[15];
        const double t865 = a[57];
        const double t866 = a[405];
        const double t868 = a[120];
        const double t877 = a[237] * t5;
        const double t878 = a[152];
        const double t895 = a[29];
        const double t896 = a[393];
        const double t898 = a[116];
        const double t904 = a[291] * t5;
        const double t914 = a[340];
        const double t916 = a[149];
        const double t926 = a[299];
        const double t936 = a[12];
        const double t937 = a[21];
        const double t938 = a[315];
        const double t940 = a[111];
        const double t944 = (t937 + (t938 * t5 + t940) * t5) * t5;
        const double t946 = a[398] * t5;
        const double t954 = (t937 + (t946 + a[96]) * t5 + (t938 * t31 + t940 + t946) * t31) * t31;
        const double t956 = a[199];
        const double t958 = a[90];
        const double t968 = a[224];
        const double t975 = (a[48] + (t956 * t5 + t958) * t5 + (t956 * t31 + a[415] * t5 + t958) * t31 +
                             (t968 * t31 + t968 * t5 + a[298] * t71 + a[135]) * t71) *
                            t71;
        const double t976 = a[63];
        const double t977 = a[229];
        const double t979 = a[89];
        const double t981 = (t977 * t5 + t979) * t5;
        const double t986 = (t977 * t31 + t5 * a[420] + t979) * t31;
        const double t989 = a[284];
        const double t994 = (t989 * t31 + t989 * t5 + t71 * a[380] + a[106]) * t71;
        const double t995 = a[318];
        const double t998 = a[266] * t71;
        const double t999 = a[341];
        const double t1000 = t999 * t31;
        const double t1001 = t999 * t5;
        const double t1002 = a[171];
        const double t1010 = a[252];
        const double t1012 = a[87];
        const double t1022 = a[368];
        const double t1029 = a[280] * t153;
        const double t1031 = a[407] * t71;
        const double t1032 = a[319];
        const double t1033 = t1032 * t31;
        const double t1034 = t1032 * t5;
        const double t1035 = a[129];
        const double t1051 = a[38];
        const double t1052 = a[365];
        const double t1054 = a[97];
        const double t1058 = (t1051 + (t1052 * t5 + t1054) * t5) * t5;
        const double t1060 = a[363] * t5;
        const double t1068 = (t1051 + (t1060 + a[88]) * t5 + (t1052 * t31 + t1054 + t1060) * t31) * t31;
        const double t1070 = a[379];
        const double t1072 = a[81];
        const double t1082 = a[222];
        const double t1089 = (a[53] + (t1070 * t5 + t1072) * t5 + (t1070 * t31 + t5 * a[414] + t1072) * t31 +
                              (t1082 * t31 + t1082 * t5 + t71 * a[364] + a[142]) * t71) *
                             t71;
        const double t1090 = a[61];
        const double t1091 = a[283];
        const double t1093 = a[164];
        const double t1095 = (t1091 * t5 + t1093) * t5;
        const double t1100 = (t1091 * t31 + t5 * a[205] + t1093) * t31;
        const double t1103 = a[343];
        const double t1108 = (t1103 * t31 + t1103 * t5 + t71 * a[396] + a[172]) * t71;
        const double t1109 = a[377];
        const double t1112 = a[334] * t71;
        const double t1113 = a[267];
        const double t1114 = t1113 * t31;
        const double t1115 = t1113 * t5;
        const double t1116 = a[169];
        const double t1121 = a[54];
        const double t1122 = a[218];
        const double t1124 = a[94];
        const double t1126 = (t1122 * t5 + t1124) * t5;
        const double t1131 = (t1122 * t31 + t5 * a[257] + t1124) * t31;
        const double t1134 = a[381];
        const double t1139 = (t1134 * t31 + t1134 * t5 + t71 * a[369] + a[104]) * t71;
        const double t1141 = a[197] * t153;
        const double t1143 = a[253] * t71;
        const double t1144 = a[374];
        const double t1145 = t1144 * t31;
        const double t1146 = t1144 * t5;
        const double t1147 = a[163];
        const double t1150 = a[385];
        const double t1153 = a[367] * t153;
        const double t1155 = a[270] * t71;
        const double t1156 = a[332];
        const double t1157 = t1156 * t31;
        const double t1158 = t1156 * t5;
        const double t1159 = a[162];
        const double t1164 = a[239];
        const double t1166 = a[107];
        const double t1168 = (t1164 * t5 + t1166) * t5;
        const double t1173 = (t1164 * t31 + t5 * a[348] + t1166) * t31;
        const double t1176 = a[313];
        const double t1181 = (t1176 * t31 + t1176 * t5 + t71 * a[231] + a[148]) * t71;
        const double t1182 = a[308];
        const double t1185 = a[297] * t71;
        const double t1186 = a[209];
        const double t1187 = t1186 * t31;
        const double t1188 = t1186 * t5;
        const double t1189 = a[95];
        const double t1192 = a[375];
        const double t1195 = a[194] * t153;
        const double t1197 = a[328] * t71;
        const double t1198 = a[190];
        const double t1199 = t1198 * t31;
        const double t1200 = t1198 * t5;
        const double t1201 = a[166];
        const double t1220 = a[208];
        const double t1222 = a[86];
        const double t1232 = a[355];
        const double t1238 = a[305];
        const double t1241 = a[187] * t71;
        const double t1242 = a[294];
        const double t1243 = t1242 * t31;
        const double t1244 = t1242 * t5;
        const double t1245 = a[127];
        const double t1265 = a[28];
        const double t1266 = a[410];
        const double t1268 = a[131];
        const double t1274 = a[378] * t5;
        const double t1284 = a[401];
        const double t1286 = a[139];
        const double t1296 = a[346];
        const double t1304 = a[19];
        const double t1305 = a[352];
        const double t1307 = a[108];
        const double t1309 = (t1305 * t5 + t1307) * t5;
        const double t1314 = (t1305 * t31 + t5 * a[240] + t1307) * t31;
        const double t1317 = a[243];
        const double t1322 = (t1317 * t31 + t1317 * t5 + a[417] * t71 + a[79]) * t71;
        const double t1323 = a[195];
        const double t1326 = a[265] * t71;
        const double t1327 = a[238];
        const double t1328 = t1327 * t31;
        const double t1329 = t1327 * t5;
        const double t1330 = a[157];
        const double t1336 = a[279] * t153;
        const double t1339 = a[282];
        const double t1350 = a[272];
        const double t1352 = a[124];
        const double t1354 = (t1350 * t5 + t1352) * t5;
        const double t1359 = (t1350 * t31 + t5 * a[259] + t1352) * t31;
        const double t1362 = a[324];
        const double t1367 = (t1362 * t31 + t1362 * t5 + t71 * a[234] + a[143]) * t71;
        const double t1368 = a[276];
        const double t1371 = a[206] * t71;
        const double t1372 = a[254];
        const double t1373 = t1372 * t31;
        const double t1374 = t1372 * t5;
        const double t1375 = a[93];
        const double t1378 = a[200];
        const double t1381 = a[301] * t153;
        const double t1383 = a[281] * t71;
        const double t1384 = a[306];
        const double t1385 = t1384 * t31;
        const double t1386 = t1384 * t5;
        const double t1387 = a[181];
        const double t1390 = a[225];
        const double t1393 = a[387] * t71;
        const double t1394 = a[198];
        const double t1397 = t5 + t31;
        const double t1398 = a[260] * t1397;
        const double t1411 = a[413];
        const double t897 = x[nv + nd * 3];
        const double t951 = x[nv + nd * 2];
        const double t1073 = x[nv + nd * 1];
        const double t1432 = x[nv + nd * 0];
        energy[nv] =
            ((t1 + (t2 + (t3 + (t4 * t5 + t6) * t5) * t5) * t5) * t5 +
             (t1 + (a[17] + (t16 + (t18 + t19) * t5) * t5) * t5 +
              (t2 + (t16 + (t27 + a[182]) * t5) * t5 + (t3 + (t27 + t19) * t5 + (t4 * t31 + t18 + t6) * t31) * t31) *
                  t31) *
                 t31 +
             (a[1] + (t45 + (t46 + (t47 * t5 + t49) * t5) * t5) * t5 +
              (t45 + (a[66] + (t58 + t59) * t5) * t5 +
               (t46 + (t5 * a[426] + t59) * t5 + (t47 * t31 + t49 + t58) * t31) * t31) *
                  t31 +
              (a[11] + (t76 + (t77 * t5 + t79) * t5) * t5 +
               (t76 + (t85 + a[167]) * t5 + (t77 * t31 + t79 + t85) * t31) * t31 +
               (a[42] + (t95 * t5 + t97) * t5 + (t95 * t31 + t5 * a[211] + t97) * t31 +
                (t107 * t31 + t107 * t5 + a[425] * t71 + a[154]) * t71) *
                   t71) *
                  t71) *
                 t71 +
             (t119 + t130 + t149 + t191 +
              (t192 + t200 + t210 + t231 +
               (t232 + t237 + t242 + t250 + (t251 * t153 + t254 + t256 + t257 + t258) * t153) * t153) *
                  t153) *
                 t153 +
             (t119 + t130 + t149 + t191 +
              (a[4] + (t268 + (t269 * t5 + t271) * t5) * t5 +
               (t268 + (t277 + a[179]) * t5 + (t269 * t31 + t271 + t277) * t31) * t31 +
               (a[60] + (t287 * t5 + t289) * t5 + (t287 * t31 + t5 * a[397] + t289) * t31 +
                (t299 * t31 + t299 * t5 + t71 * a[386] + a[80]) * t71) *
                   t71 +
               (t307 + t312 + t317 + t325 + (t327 + t329 + t331 + t332 + t333) * t153) * t153) *
                  t153 +
              (t192 + t200 + t210 + t231 +
               (t307 + t312 + t317 + t325 + (t344 * t31 + t344 * t5 + t71 * a[394] + t341 + a[185]) * t153) * t153 +
               (t232 + t237 + t242 + t250 + (t341 + t329 + t331 + t332 + t333) * t153 +
                (t251 * t897 + t254 + t256 + t257 + t258 + t327) * t897) *
                   t897) *
                  t897) *
                 t897 +
             (t373 + t392 + t434 +
              (t435 + t443 + t453 + t474 +
               (t475 + t480 + t485 + t493 + (t494 * t153 + t497 + t499 + t500 + t501) * t153) * t153) *
                  t153 +
              (t508 + t516 + t526 + t547 +
               (t548 + t553 + t558 + t566 + (t568 + t570 + t572 + t573 + t574) * t153) * t153 +
               (t579 + t584 + t589 + t597 + (t599 + t601 + t603 + t604 + t605) * t153 +
                (t608 * t897 + t611 + t613 + t615 + t616 + t617) * t897) *
                   t897) *
                  t897 +
              (t631 + t641 + t662 +
               (t663 + t668 + t673 + t681 + (t682 * t153 + t685 + t687 + t688 + t689) * t153) * t153 +
               (t694 + t699 + t704 + t712 + (t714 + t716 + t718 + t719 + t720) * t153 +
                (t723 * t897 + t726 + t728 + t730 + t731 + t732) * t897) *
                   t897) *
                  t951) *
                 t951 +
             (t373 + t392 + t434 +
              (t508 + t516 + t526 + t547 +
               (t579 + t584 + t589 + t597 + (t608 * t153 + t613 + t615 + t616 + t617) * t153) * t153) *
                  t153 +
              (t435 + t443 + t453 + t474 +
               (t548 + t553 + t558 + t566 + (t611 + t601 + t603 + t604 + t605) * t153) * t153 +
               (t475 + t480 + t485 + t493 + (t599 + t570 + t572 + t573 + t574) * t153 +
                (t494 * t897 + t497 + t499 + t500 + t501 + t568) * t897) *
                   t897) *
                  t897 +
              ((t761 + (t762 * t5 + t764) * t5) * t5 +
               (t761 + (t770 + a[184]) * t5 + (t762 * t31 + t764 + t770) * t31) * t31 +
               (a[34] + (t780 * t5 + t782) * t5 + (t780 * t31 + t5 * a[411] + t782) * t31 +
                (t792 * t31 + t792 * t5 + a[408] * t71 + a[130]) * t71) *
                   t71 +
               (t800 + t805 + t810 + t818 + (t819 * t153 + t822 + t824 + t825 + t826) * t153) * t153 +
               (t800 + t805 + t810 + t818 + (t835 * t31 + t835 * t5 + t71 * a[217] + t832 + a[78]) * t153 +
                (t819 * t897 + t822 + t824 + t825 + t826 + t832) * t897) *
                   t897) *
                  t951 +
              (t631 + t641 + t662 +
               (t694 + t699 + t704 + t712 + (t723 * t153 + t728 + t730 + t731 + t732) * t153) * t153 +
               (t663 + t668 + t673 + t681 + (t726 + t716 + t718 + t719 + t720) * t153 +
                (t682 * t897 + t685 + t687 + t688 + t689 + t714) * t897) *
                   t897) *
                  t1073) *
                 t1073 +
             ((t864 + (t865 + (t866 * t5 + t868) * t5) * t5) * t5 +
              (t864 + (a[44] + (t877 + t878) * t5) * t5 +
               (t865 + (t5 * a[273] + t878) * t5 + (t866 * t31 + t868 + t877) * t31) * t31) *
                  t31 +
              (a[16] + (t895 + (t896 * t5 + t898) * t5) * t5 +
               (t895 + (t904 + a[175]) * t5 + (t896 * t31 + t898 + t904) * t31) * t31 +
               (a[33] + (t914 * t5 + t916) * t5 + (t914 * t31 + t5 * a[262] + t916) * t31 +
                (t926 * t31 + t926 * t5 + t71 * a[424] + a[92]) * t71) *
                   t71) *
                  t71 +
              (t936 + t944 + t954 + t975 +
               (t976 + t981 + t986 + t994 + (t995 * t153 + t1000 + t1001 + t1002 + t998) * t153) * t153) *
                  t153 +
              (t936 + t944 + t954 + t975 +
               (a[65] + (t1010 * t5 + t1012) * t5 + (t1010 * t31 + a[427] * t5 + t1012) * t31 +
                (t1022 * t31 + t1022 * t5 + t71 * a[419] + a[100]) * t71 +
                (t1029 + t1031 + t1033 + t1034 + t1035) * t153) *
                   t153 +
               (t976 + t981 + t986 + t994 + (t153 * a[212] + t1031 + t1033 + t1034 + t1035) * t153 +
                (t995 * t897 + t1000 + t1001 + t1002 + t1029 + t998) * t897) *
                   t897) *
                  t897 +
              (t1058 + t1068 + t1089 +
               (t1090 + t1095 + t1100 + t1108 + (t1109 * t153 + t1112 + t1114 + t1115 + t1116) * t153) * t153 +
               (t1121 + t1126 + t1131 + t1139 + (t1141 + t1143 + t1145 + t1146 + t1147) * t153 +
                (t1150 * t897 + t1153 + t1155 + t1157 + t1158 + t1159) * t897) *
                   t897 +
               (t1168 + t1173 + t1181 + (t1182 * t153 + t1185 + t1187 + t1188 + t1189) * t153 +
                (t1192 * t897 + t1195 + t1197 + t1199 + t1200 + t1201) * t897) *
                   t951) *
                  t951 +
              (t1058 + t1068 + t1089 +
               (t1121 + t1126 + t1131 + t1139 + (t1150 * t153 + t1155 + t1157 + t1158 + t1159) * t153) * t153 +
               (t1090 + t1095 + t1100 + t1108 + (t1153 + t1143 + t1145 + t1146 + t1147) * t153 +
                (t1109 * t897 + t1112 + t1114 + t1115 + t1116 + t1141) * t897) *
                   t897 +
               ((t1220 * t5 + t1222) * t5 + (t1220 * t31 + t5 * a[384] + t1222) * t31 +
                (t1232 * t31 + t1232 * t5 + t71 * a[390] + a[156]) * t71 +
                (t1238 * t153 + t1241 + t1243 + t1244 + t1245) * t153 +
                (t1238 * t897 + t153 * a[418] + t1241 + t1243 + t1244 + t1245) * t897) *
                   t951 +
               (t1168 + t1173 + t1181 + (t1192 * t153 + t1197 + t1199 + t1200 + t1201) * t153 +
                (t1182 * t897 + t1185 + t1187 + t1188 + t1189 + t1195) * t897) *
                   t1073) *
                  t1073 +
              ((t1265 + (t1266 * t5 + t1268) * t5) * t5 +
               (t1265 + (t1274 + a[73]) * t5 + (t1266 * t31 + t1268 + t1274) * t31) * t31 +
               (a[55] + (t1284 * t5 + t1286) * t5 + (t1284 * t31 + t5 * a[327] + t1286) * t31 +
                (t1296 * t31 + t1296 * t5 + t71 * a[353] + a[75]) * t71) *
                   t71 +
               (t1304 + t1309 + t1314 + t1322 + (t1323 * t153 + t1326 + t1328 + t1329 + t1330) * t153) * t153 +
               (t1304 + t1309 + t1314 + t1322 + (t1339 * t31 + t1339 * t5 + t71 * a[295] + t1336 + a[77]) * t153 +
                (t1323 * t897 + t1326 + t1328 + t1329 + t1330 + t1336) * t897) *
                   t897 +
               (t1354 + t1359 + t1367 + (t1368 * t153 + t1371 + t1373 + t1374 + t1375) * t153 +
                (t1378 * t897 + t1381 + t1383 + t1385 + t1386 + t1387) * t897 +
                (t1390 * t153 + t1394 * t897 + t1393 + t1398) * t951) *
                   t951 +
               (t1354 + t1359 + t1367 + (t1378 * t153 + t1383 + t1385 + t1386 + t1387) * t153 +
                (t1368 * t897 + t1371 + t1373 + t1374 + t1375 + t1381) * t897 +
                (t1397 * a[191] + t1411 * t153 + t1411 * t897 + a[371] * t71) * t951 +
                (t1390 * t897 + t1394 * t153 + t1393 + t1398) * t1073) *
                   t1073) *
                  t1432) *
                 t1432);
    }
    return energy;
}

}  // namespace h2o_ion
