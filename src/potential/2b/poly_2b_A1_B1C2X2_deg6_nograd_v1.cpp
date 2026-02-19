
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
 * @file poly_2b_A1_B1C2X2_deg6_nograd_v1.cpp
 * @brief Contains the implementation of the polynomials without gradients for symmetry A1_B1C2X2
 */

/**
 * @namespace mbnrg_A1_B1C2X2_deg6
 * @brief Encloses the structure of the polynomial for symmetry A1_B1C2X2
 */

namespace mbnrg_A1_B1C2X2_deg6 {

double poly_A1_B1C2X2_deg6_v1::eval(const double x[8],
            const double a[906]) {
    const double t1 = a[2];
    const double t2 = a[11];
    const double t3 = a[26];
    const double t4 = a[97];
    const double t5 = a[692];
    const double t7 = a[305];
    const double t19 = a[24];
    const double t20 = a[87];
    const double t6 = x[4];
    const double t22 = t6*a[703];
    const double t23 = a[187];
    const double t34 = t6*a[431];
    const double t35 = a[198];
    const double t61 = a[1];
    const double t14 = x[7];
    const double t75 = (a[13]+(a[30]+(a[139]+(a[899]*t14+a[284])*t14)*t14)*t14)*t14;
    const double t76 = a[3];
    const double t87 = (a[15]+(a[101]+(t14*a[522]+a[310])*t14)*t14)*t14;
    const double t88 = a[38];
    const double t96 = (a[58]+(t14*a[762]+a[383])*t14)*t14;
    const double t97 = a[57];
    const double t102 = (t14*a[738]+a[161])*t14;
    const double t103 = a[455];
    const double t106 = t14*a[393];
    const double t107 = a[352];
    const double t126 = (a[20]+(a[65]+(a[728]*t14+a[172])*t14)*t14)*t14;
    const double t127 = a[16];
    const double t135 = (a[134]+(a[878]*t14+a[212])*t14)*t14;
    const double t136 = a[67];
    const double t141 = (a[726]*t14+a[379])*t14;
    const double t74 = x[6];
    const double t143 = t74*a[590];
    const double t145 = t14*a[583];
    const double t146 = a[331];
    const double t160 = (a[83]+(t14*a[580]+a[182])*t14)*t14;
    const double t161 = a[95];
    const double t166 = (a[718]*t14+a[207])*t14;
    const double t168 = t74*a[494];
    const double t170 = t14*a[763];
    const double t171 = a[233];
    const double t180 = (t14*a[584]+a[340])*t14;
    const double t182 = t74*a[453];
    const double t184 = a[416]*t14;
    const double t185 = a[147];
    const double t189 = t74*a[555];
    const double t191 = a[725]*t14;
    const double t200 = a[5];
    const double t211 = (a[42]+(a[112]+(a[716]*t14+a[163])*t14)*t14)*t14;
    const double t212 = a[35];
    const double t220 = (a[128]+(t14*a[752]+a[165])*t14)*t14;
    const double t221 = a[107];
    const double t226 = (t14*a[553]+a[152])*t14;
    const double t227 = a[454];
    const double t230 = t14*a[837];
    const double t231 = a[258];
    const double t237 = (t212+t220+(t221+t226+(t227*t74+t230+t231)*t74)*t74)*t74;
    const double t245 = (a[133]+(a[462]*t14+a[157])*t14)*t14;
    const double t246 = a[59];
    const double t251 = (t14*a[489]+a[312])*t14;
    const double t253 = t74*a[776];
    const double t255 = t14*a[766];
    const double t256 = a[339];
    const double t265 = (t14*a[417]+a[270])*t14;
    const double t267 = t74*a[693];
    const double t269 = t14*a[595];
    const double t270 = a[255];
    const double t274 = a[760]*t74;
    const double t276 = a[851]*t14;
    const double t178 = x[5];
    const double t282 = (t245+(t246+t251+(t253+t255+t256)*t74)*t74+(t265+(t267+t269+t270)*
t74+(t274+t276)*t178)*t178)*t178;
    const double t283 = a[43];
    const double t291 = (a[89]+(t14*a[868]+a[285])*t14)*t14;
    const double t292 = a[126];
    const double t297 = (t14*a[811]+a[329])*t14;
    const double t298 = a[613];
    const double t301 = t14*a[735];
    const double t302 = a[293];
    const double t306 = (t292+t297+(t298*t74+t301+t302)*t74)*t74;
    const double t311 = (t14*a[654]+a[227])*t14;
    const double t313 = t74*a[567];
    const double t315 = t14*a[526];
    const double t316 = a[249];
    const double t320 = t74*a[753];
    const double t322 = t14*a[512];
    const double t326 = (t311+(t313+t315+t316)*t74+(t320+t322)*t178)*t178;
    const double t327 = a[137];
    const double t332 = (t14*a[831]+a[196])*t14;
    const double t333 = a[832];
    const double t336 = a[696]*t14;
    const double t337 = a[216];
    const double t339 = (t333*t74+t336+t337)*t74;
    const double t341 = t74*a[642];
    const double t342 = a[392];
    const double t345 = (t14*t342+t341)*t178;
    const double t346 = a[708];
    const double t347 = t6*t346;
    const double t348 = a[765];
    const double t349 = t74*t348;
    const double t351 = t14*a[558];
    const double t352 = a[357];
    const double t361 = a[21];
    const double t369 = (a[79]+(a[507]*t14+a[185])*t14)*t14;
    const double t370 = a[96];
    const double t375 = (t14*a[443]+a[320])*t14;
    const double t376 = a[497];
    const double t379 = t14*a[745];
    const double t380 = a[186];
    const double t389 = (a[624]*t14+a[307])*t14;
    const double t391 = t74*a[614];
    const double t393 = t14*a[427];
    const double t394 = a[297];
    const double t398 = t74*a[668];
    const double t400 = a[476]*t14;
    const double t405 = a[130];
    const double t410 = (a[575]*t14+a[277])*t14;
    const double t411 = a[744];
    const double t414 = t14*a[706];
    const double t415 = a[279];
    const double t417 = (t411*t74+t414+t415)*t74;
    const double t419 = t74*a[795];
    const double t420 = a[424];
    const double t423 = (t14*t420+t419)*t178;
    const double t425 = t6*a[609];
    const double t426 = a[749];
    const double t427 = t74*t426;
    const double t429 = t14*a[561];
    const double t430 = a[251];
    const double t438 = t6*a[429];
    const double t439 = a[786];
    const double t442 = t14*a[649];
    const double t443 = a[238];
    const double t287 = x[3];
    const double t450 = t287*t346;
    const double t459 = a[8];
    const double t470 = (a[19]+(a[138]+(t14*a[840]+a[156])*t14)*t14)*t14;
    const double t471 = a[37];
    const double t479 = (a[119]+(t14*a[781]+a[374])*t14)*t14;
    const double t480 = a[121];
    const double t485 = (a[773]*t14+a[241])*t14;
    const double t486 = a[737];
    const double t489 = a[483]*t14;
    const double t490 = a[375];
    const double t504 = (a[110]+(t14*a[858]+a[276])*t14)*t14;
    const double t505 = a[76];
    const double t510 = (t14*a[751]+a[364])*t14;
    const double t512 = a[559]*t74;
    const double t514 = a[623]*t14;
    const double t515 = a[300];
    const double t524 = (t14*a[554]+a[160])*t14;
    const double t526 = a[533]*t74;
    const double t528 = t14*a[625];
    const double t529 = a[291];
    const double t533 = a[394]*t74;
    const double t535 = t14*a[638];
    const double t542 = a[44];
    const double t550 = (a[63]+(t14*a[465]+a[230])*t14)*t14;
    const double t551 = a[93];
    const double t556 = (t14*a[541]+a[222])*t14;
    const double t557 = a[872];
    const double t560 = t14*a[525];
    const double t561 = a[153];
    const double t565 = (t551+t556+(t557*t74+t560+t561)*t74)*t74;
    const double t570 = (t14*a[783]+a[267])*t14;
    const double t572 = a[731]*t74;
    const double t574 = a[548]*t14;
    const double t575 = a[318];
    const double t579 = a[505]*t74;
    const double t581 = a[422]*t14;
    const double t585 = (t570+(t572+t574+t575)*t74+(t579+t581)*t178)*t178;
    const double t586 = a[125];
    const double t591 = (a[450]*t14+a[373])*t14;
    const double t592 = a[810];
    const double t595 = t14*a[588];
    const double t596 = a[326];
    const double t598 = (t592*t74+t595+t596)*t74;
    const double t600 = a[519]*t74;
    const double t602 = t14*a[471];
    const double t604 = (t600+t602)*t178;
    const double t605 = a[806];
    const double t606 = t6*t605;
    const double t607 = a[388];
    const double t608 = t74*t607;
    const double t610 = t14*a[612];
    const double t611 = a[313];
    const double t618 = a[77];
    const double t623 = (t14*a[576]+a[346])*t14;
    const double t624 = a[720];
    const double t627 = t14*a[436];
    const double t628 = a[159];
    const double t632 = a[843]*t74;
    const double t633 = a[782];
    const double t638 = a[664]*t6;
    const double t639 = a[774];
    const double t640 = t74*t639;
    const double t642 = a[800]*t14;
    const double t643 = a[148];
    const double t649 = a[739]*t6;
    const double t652 = t287*t605;
    const double t659 = a[22];
    const double t667 = (a[129]+(t14*a[648]+a[348])*t14)*t14;
    const double t668 = a[105];
    const double t673 = (t14*a[655]+a[166])*t14;
    const double t674 = a[472];
    const double t677 = a[501]*t14;
    const double t678 = a[206];
    const double t687 = (t14*a[711]+a[334])*t14;
    const double t689 = a[408]*t74;
    const double t691 = t14*a[403];
    const double t692 = a[332];
    const double t696 = a[802]*t74;
    const double t698 = t14*a[538];
    const double t703 = a[72];
    const double t708 = (t14*a[437]+a[359])*t14;
    const double t709 = a[557];
    const double t712 = t14*a[605];
    const double t713 = a[264];
    const double t715 = (t709*t74+t712+t713)*t74;
    const double t717 = a[440]*t74;
    const double t718 = a[895];
    const double t721 = (t14*t718+t717)*t178;
    const double t722 = a[448];
    const double t723 = t6*t722;
    const double t724 = a[874];
    const double t725 = t74*t724;
    const double t727 = a[856]*t14;
    const double t728 = a[316];
    const double t734 = a[793]*t6;
    const double t735 = a[456];
    const double t738 = t14*a[524];
    const double t739 = a[376];
    const double t742 = t287*t722;
    const double t747 = a[123];
    const double t752 = (t14*a[757]+a[354])*t14;
    const double t753 = a[866];
    const double t756 = a[695]*t14;
    const double t757 = a[236];
    const double t761 = a[707]*t74;
    const double t762 = a[663];
    const double t766 = a[517];
    const double t767 = t6*t766;
    const double t768 = a[864];
    const double t769 = t74*t768;
    const double t771 = a[730]*t14;
    const double t772 = a[202];
    const double t775 = t287*t766;
    const double t777 = a[570]*t6;
    const double t780 = a[585];
    const double t782 = a[821];
    const double t783 = t287*t782;
    const double t784 = t6*t782;
    const double t785 = a[615];
    const double t788 = t14*a[446];
    const double t789 = a[174];
    const double t832 = (t245+(t276*t74+t265)*t74)*t74;
    const double t845 = (t212+t220+(t246+t251+(t274+t269+t270)*t74)*t74+(t221+t226+(t267+
t255+t256)*t74+(t178*t227+t230+t231+t253)*t178)*t178)*t178;
    const double t848 = (t322*t74+t311)*t74;
    const double t855 = (t292+t297+(t320+t315+t316)*t74+(t178*t298+t301+t302+t313)*t178)*
t178;
    const double t857 = t342*t74*t14;
    const double t860 = (t178*t333+t336+t337+t341)*t178;
    const double t861 = t178*t348;
    const double t881 = t420*t74*t14;
    const double t884 = (t178*t411+t414+t415+t419)*t178;
    const double t885 = t178*t426;
    const double t919 = a[48];
    const double t927 = (a[53]+(t14*a[606]+a[240])*t14)*t14;
    const double t928 = a[132];
    const double t933 = (t14*a[515]+a[304])*t14;
    const double t934 = a[389];
    const double t937 = t14*a[511];
    const double t938 = a[221];
    const double t952 = t74*a[826];
    const double t954 = t14*a[477];
    const double t955 = a[272];
    const double t971 = a[32];
    const double t979 = (a[62]+(t14*a[402]+a[311])*t14)*t14;
    const double t980 = a[146];
    const double t985 = (a[882]*t14+a[167])*t14;
    const double t986 = a[627];
    const double t989 = t14*a[637];
    const double t990 = a[319];
    const double t994 = (t980+t985+(t74*t986+t989+t990)*t74)*t74;
    const double t996 = a[884]*t74;
    const double t1006 = (t980+t985+(t14*a[833]+t996+a[266])*t74+(t178*t986+t989+t990+t996)*
t178)*t178;
    const double t1007 = a[92];
    const double t1012 = (t14*a[867]+a[363])*t14;
    const double t1013 = a[412];
    const double t1016 = a[537]*t14;
    const double t1017 = a[169];
    const double t1019 = (t1013*t74+t1016+t1017)*t74;
    const double t1024 = (t1013*t178+t74*a[686]+t1016+t1017)*t178;
    const double t1025 = a[503];
    const double t1027 = a[528];
    const double t1028 = t178*t1027;
    const double t1029 = t74*t1027;
    const double t1031 = a[563]*t14;
    const double t1032 = a[265];
    const double t1045 = a[530];
    const double t1048 = t14*a[628];
    const double t1049 = a[325];
    const double t1058 = a[617]*t6;
    const double t1059 = a[604];
    const double t1060 = t178*t1059;
    const double t1061 = t74*t1059;
    const double t1063 = a[873]*t14;
    const double t1064 = a[358];
    const double t1080 = a[17];
    const double t1088 = (a[82]+(t14*a[478]+a[263])*t14)*t14;
    const double t1089 = a[64];
    const double t1094 = (t14*a[721]+a[323])*t14;
    const double t1095 = a[713];
    const double t1098 = t14*a[540];
    const double t1099 = a[351];
    const double t1104 = a[113];
    const double t1109 = (a[791]*t14+a[254])*t14;
    const double t1111 = a[748]*t74;
    const double t1113 = a[447]*t14;
    const double t1114 = a[188];
    const double t1117 = a[887];
    const double t1120 = t74*a[643];
    const double t1122 = t14*a[890];
    const double t1123 = a[315];
    const double t1128 = a[140];
    const double t1133 = (t14*a[420]+a[347])*t14;
    const double t1134 = a[838];
    const double t1137 = a[474]*t14;
    const double t1138 = a[287];
    const double t1140 = (t1134*t74+t1137+t1138)*t74;
    const double t1141 = a[709];
    const double t1144 = a[661]*t74;
    const double t1146 = t14*a[656];
    const double t1147 = a[150];
    const double t1149 = (t1141*t178+t1144+t1146+t1147)*t178;
    const double t1150 = a[828];
    const double t1151 = t6*t1150;
    const double t1152 = a[545];
    const double t1153 = t178*t1152;
    const double t1154 = a[592];
    const double t1155 = t74*t1154;
    const double t1157 = a[502]*t14;
    const double t1158 = a[294];
    const double t1164 = a[897]*t6;
    const double t1165 = a[798];
    const double t1167 = a[819];
    const double t1170 = a[484]*t14;
    const double t1171 = a[256];
    const double t1174 = t287*t1150;
    const double t1179 = a[52];
    const double t1184 = (t14*a[697]+a[239])*t14;
    const double t1185 = a[799];
    const double t1188 = a[673]*t14;
    const double t1189 = a[380];
    const double t1192 = a[387];
    const double t1195 = a[594]*t74;
    const double t1197 = a[902]*t14;
    const double t1198 = a[353];
    const double t1201 = a[409];
    const double t1202 = t6*t1201;
    const double t1203 = a[808];
    const double t1204 = t178*t1203;
    const double t1205 = a[634];
    const double t1206 = t74*t1205;
    const double t1208 = a[904]*t14;
    const double t1209 = a[278];
    const double t1212 = t287*t1201;
    const double t1214 = a[458]*t6;
    const double t684 = x[2];
    const double t1218 = a[796]*t684;
    const double t1219 = a[466];
    const double t1220 = t287*t1219;
    const double t1221 = t6*t1219;
    const double t1222 = a[539];
    const double t1224 = a[572];
    const double t1227 = a[754]*t14;
    const double t1228 = a[372];
    const double t1257 = (t581*t74+t570)*t74;
    const double t1264 = (t551+t556+(t579+t574+t575)*t74+(t178*t557+t560+t561+t572)*t178)*
t178;
    const double t1265 = t602*t74;
    const double t1268 = (t178*t592+t595+t596+t600)*t178;
    const double t1269 = t178*t607;
    const double t1281 = t178*t639;
    const double t1308 = (t1141*t74+t1146+t1147)*t74;
    const double t1311 = (t1134*t178+t1137+t1138+t1144)*t178;
    const double t1312 = t178*t1154;
    const double t1313 = t74*t1152;
    const double t1332 = a[761];
    const double t1335 = a[676]*t14;
    const double t1336 = a[164];
    const double t1344 = a[683];
    const double t1346 = a[516];
    const double t1347 = t178*t1346;
    const double t1348 = t74*t1346;
    const double t1350 = a[741]*t14;
    const double t1351 = a[195];
    const double t1360 = a[769]*t684;
    const double t1361 = a[593];
    const double t1362 = t287*t1361;
    const double t1363 = t6*t1361;
    const double t1364 = a[680];
    const double t1366 = a[652];
    const double t1369 = a[425]*t14;
    const double t1370 = a[253];
    const double t1388 = t718*t74*t14;
    const double t1391 = (t178*t709+t712+t713+t717)*t178;
    const double t1392 = t178*t724;
    const double t1410 = t178*t1205;
    const double t1411 = t74*t1203;
    const double t1429 = t178*t768;
    const double t1451 = a[4];
    const double t1462 = (a[45]+(a[71]+(t14*a[770]+a[292])*t14)*t14)*t14;
    const double t1463 = a[27];
    const double t1471 = (a[108]+(a[857]*t14+a[244])*t14)*t14;
    const double t1472 = a[141];
    const double t1477 = (t14*a[839]+a[274])*t14;
    const double t1478 = a[441];
    const double t1481 = a[675]*t14;
    const double t1482 = a[344];
    const double t1500 = a[114];
    const double t1505 = (a[688]*t14+a[183])*t14;
    const double t1507 = a[807]*t74;
    const double t1509 = a[824]*t14;
    const double t1510 = a[299];
    const double t1518 = a[546]*t74;
    const double t1537 = a[7];
    const double t1538 = a[23];
    const double t1546 = (a[136]+(t14*a[788]+a[321])*t14)*t14;
    const double t1547 = a[144];
    const double t1552 = (t14*a[690]+a[203])*t14;
    const double t1553 = a[803];
    const double t1556 = t14*a[727];
    const double t1557 = a[205];
    const double t1563 = (t1538+t1546+(t1547+t1552+(t1553*t74+t1556+t1557)*t74)*t74)*t74;
    const double t1571 = a[581]*t74;
    const double t1573 = a[473]*t14;
    const double t1574 = a[345];
    const double t1589 = (t1538+t1546+(a[55]+(t14*a[645]+a[259])*t14+(t1571+t1573+t1574)*t74
)*t74+(t1547+t1552+(t74*a[401]+t1573+t1574)*t74+(t1553*t178+t1556+t1557+t1571)*
t178)*t178)*t178;
    const double t1590 = a[29];
    const double t1591 = a[106];
    const double t1596 = (t14*a[682]+a[234])*t14;
    const double t1597 = a[747];
    const double t1600 = a[704]*t14;
    const double t1601 = a[360];
    const double t1605 = (t1591+t1596+(t1597*t74+t1600+t1601)*t74)*t74;
    const double t1607 = a[871]*t74;
    const double t1617 = (t1591+t1596+(t14*a[439]+t1607+a[257])*t74+(t1597*t178+t1600+t1601+
t1607)*t178)*t178;
    const double t1618 = a[99];
    const double t1619 = a[700];
    const double t1622 = t14*a[900];
    const double t1623 = a[327];
    const double t1625 = (t1619*t74+t1622+t1623)*t74;
    const double t1630 = (t1619*t178+t74*a[804]+t1622+t1623)*t178;
    const double t1631 = a[480];
    const double t1633 = a[825];
    const double t1634 = t178*t1633;
    const double t1635 = t74*t1633;
    const double t1636 = a[154];
    const double t1646 = a[70];
    const double t1651 = (t14*a[701]+a[336])*t14;
    const double t1652 = a[566];
    const double t1655 = a[500]*t14;
    const double t1656 = a[303];
    const double t1662 = a[853]*t74;
    const double t1673 = a[85];
    const double t1674 = a[658];
    const double t1677 = a[506]*t14;
    const double t1678 = a[308];
    const double t1680 = (t1674*t74+t1677+t1678)*t74;
    const double t1685 = (t1674*t178+t74*a[848]+t1677+t1678)*t178;
    const double t1687 = a[552]*t6;
    const double t1688 = a[755];
    const double t1689 = t178*t1688;
    const double t1690 = t74*t1688;
    const double t1691 = a[309];
    const double t1699 = a[633]*t6;
    const double t1700 = a[599];
    const double t1719 = a[12];
    const double t1730 = (a[33]+(a[54]+(t14*a[445]+a[268])*t14)*t14)*t14;
    const double t1731 = a[41];
    const double t1739 = (a[111]+(t14*a[413]+a[377])*t14)*t14;
    const double t1740 = a[74];
    const double t1745 = (t14*a[678]+a[199])*t14;
    const double t1746 = a[646];
    const double t1749 = a[449]*t14;
    const double t1750 = a[175];
    const double t1757 = a[47];
    const double t1765 = (a[115]+(t14*a[406]+a[184])*t14)*t14;
    const double t1766 = a[103];
    const double t1771 = (t14*a[812]+a[246])*t14;
    const double t1773 = a[444]*t74;
    const double t1775 = a[860]*t14;
    const double t1776 = a[181];
    const double t1781 = a[100];
    const double t1786 = (t14*a[529]+a[306])*t14;
    const double t1788 = a[635]*t74;
    const double t1790 = t14*a[532];
    const double t1791 = a[180];
    const double t1794 = a[817];
    const double t1797 = a[780]*t74;
    const double t1799 = a[520]*t14;
    const double t1800 = a[208];
    const double t1807 = a[28];
    const double t1815 = (a[56]+(t14*a[569]+a[248])*t14)*t14;
    const double t1816 = a[75];
    const double t1821 = (t14*a[586]+a[252])*t14;
    const double t1822 = a[875];
    const double t1825 = a[415]*t14;
    const double t1826 = a[367];
    const double t1830 = (t1816+t1821+(t1822*t74+t1825+t1826)*t74)*t74;
    const double t1831 = a[94];
    const double t1836 = (t14*a[896]+a[314])*t14;
    const double t1838 = a[487]*t74;
    const double t1840 = a[535]*t14;
    const double t1841 = a[210];
    const double t1844 = a[677];
    const double t1847 = a[883]*t74;
    const double t1849 = a[852]*t14;
    const double t1850 = a[342];
    const double t1854 = (t1831+t1836+(t1838+t1840+t1841)*t74+(t178*t1844+t1847+t1849+t1850)
*t178)*t178;
    const double t1855 = a[73];
    const double t1860 = (t14*a[880]+a[226])*t14;
    const double t1861 = a[679];
    const double t1864 = t14*a[421];
    const double t1865 = a[333];
    const double t1867 = (t1861*t74+t1864+t1865)*t74;
    const double t1868 = a[734];
    const double t1871 = t74*a[805];
    const double t1873 = t14*a[404];
    const double t1874 = a[155];
    const double t1876 = (t178*t1868+t1871+t1873+t1874)*t178;
    const double t1877 = a[621];
    const double t1878 = t6*t1877;
    const double t1879 = a[498];
    const double t1880 = t178*t1879;
    const double t1881 = a[888];
    const double t1882 = t74*t1881;
    const double t1884 = a[639]*t14;
    const double t1885 = a[365];
    const double t1892 = a[109];
    const double t1897 = (t14*a[777]+a[225])*t14;
    const double t1898 = a[423];
    const double t1901 = a[901]*t14;
    const double t1902 = a[201];
    const double t1905 = a[689];
    const double t1908 = a[407]*t74;
    const double t1910 = a[536]*t14;
    const double t1911 = a[218];
    const double t1915 = a[630]*t6;
    const double t1916 = a[814];
    const double t1917 = t178*t1916;
    const double t1918 = a[659];
    const double t1919 = t74*t1918;
    const double t1921 = a[861]*t14;
    const double t1922 = a[355];
    const double t1928 = a[597]*t6;
    const double t1931 = t287*t1877;
    const double t1938 = a[31];
    const double t1946 = (a[142]+(t14*a[434]+a[189])*t14)*t14;
    const double t1947 = a[131];
    const double t1952 = (t14*a[565]+a[204])*t14;
    const double t1953 = a[513];
    const double t1956 = a[504]*t14;
    const double t1957 = a[370];
    const double t1962 = a[68];
    const double t1967 = (t14*a[771]+a[151])*t14;
    const double t1969 = a[534]*t74;
    const double t1971 = t14*a[719];
    const double t1972 = a[298];
    const double t1975 = a[830];
    const double t1978 = t74*a[885];
    const double t1980 = a[687]*t14;
    const double t1981 = a[301];
    const double t1986 = a[118];
    const double t1991 = (t14*a[493]+a[275])*t14;
    const double t1992 = a[523];
    const double t1995 = a[877]*t14;
    const double t1996 = a[337];
    const double t1998 = (t1992*t74+t1995+t1996)*t74;
    const double t1999 = a[729];
    const double t2002 = a[418]*t74;
    const double t2004 = a[845]*t14;
    const double t2005 = a[170];
    const double t2007 = (t178*t1999+t2002+t2004+t2005)*t178;
    const double t2008 = a[870];
    const double t2009 = t6*t2008;
    const double t2010 = a[775];
    const double t2011 = t178*t2010;
    const double t2012 = a[496];
    const double t2013 = t74*t2012;
    const double t2015 = a[809]*t14;
    const double t2016 = a[368];
    const double t2022 = a[705]*t6;
    const double t2023 = a[631];
    const double t2025 = a[564];
    const double t2028 = a[385]*t14;
    const double t2029 = a[350];
    const double t2032 = t287*t2008;
    const double t2037 = a[88];
    const double t2042 = (t14*a[495]+a[245])*t14;
    const double t2043 = a[823];
    const double t2046 = t14*a[542];
    const double t2047 = a[178];
    const double t2050 = a[767];
    const double t2053 = a[598]*t74;
    const double t2055 = a[574]*t14;
    const double t2056 = a[382];
    const double t2059 = a[854];
    const double t2060 = t6*t2059;
    const double t2061 = a[438];
    const double t2062 = t178*t2061;
    const double t2063 = a[400];
    const double t2064 = t74*t2063;
    const double t2066 = t14*a[451];
    const double t2067 = a[362];
    const double t2070 = t287*t2059;
    const double t2072 = a[452]*t6;
    const double t2075 = a[822];
    const double t2077 = a[636];
    const double t2078 = t287*t2077;
    const double t2079 = t6*t2077;
    const double t2080 = a[836];
    const double t2082 = a[681];
    const double t2085 = a[641]*t14;
    const double t2086 = a[335];
    const double t2119 = (t1831+t1836+(t1844*t74+t1849+t1850)*t74)*t74;
    const double t2126 = (t1816+t1821+(t1847+t1840+t1841)*t74+(t178*t1822+t1825+t1826+t1838)
*t178)*t178;
    const double t2129 = (t1868*t74+t1873+t1874)*t74;
    const double t2132 = (t178*t1861+t1864+t1865+t1871)*t178;
    const double t2133 = t178*t1881;
    const double t2134 = t74*t1879;
    const double t2147 = t178*t1918;
    const double t2148 = t74*t1916;
    const double t2170 = a[90];
    const double t2175 = (t14*a[710]+a[288])*t14;
    const double t2176 = a[685];
    const double t2179 = a[863]*t14;
    const double t2180 = a[211];
    const double t2186 = a[482]*t74;
    const double t2197 = a[50];
    const double t2202 = (t14*a[531]+a[378])*t14;
    const double t2203 = a[391];
    const double t2206 = a[660]*t14;
    const double t2207 = a[371];
    const double t2209 = (t2203*t74+t2206+t2207)*t74;
    const double t2214 = (t178*t2203+t74*a[905]+t2206+t2207)*t178;
    const double t2215 = a[411];
    const double t2217 = a[820];
    const double t2218 = t178*t2217;
    const double t2219 = t74*t2217;
    const double t2221 = t14*a[881];
    const double t2222 = a[328];
    const double t2228 = a[486]*t6;
    const double t2229 = a[669];
    const double t2242 = a[127];
    const double t2247 = (t14*a[491]+a[269])*t14;
    const double t2248 = a[591];
    const double t2251 = a[461]*t14;
    const double t2252 = a[295];
    const double t2255 = a[893];
    const double t2258 = a[891]*t74;
    const double t2260 = t14*a[626];
    const double t2261 = a[191];
    const double t2264 = a[571];
    const double t2265 = t6*t2264;
    const double t2266 = a[433];
    const double t2267 = t178*t2266;
    const double t2268 = a[651];
    const double t2269 = t74*t2268;
    const double t2271 = a[715]*t14;
    const double t2272 = a[361];
    const double t2275 = t287*t2264;
    const double t2277 = a[397]*t6;
    const double t2281 = a[792]*t684;
    const double t2282 = a[865];
    const double t2283 = t287*t2282;
    const double t2284 = t6*t2282;
    const double t2285 = a[582];
    const double t2287 = a[849];
    const double t2290 = a[386]*t14;
    const double t2291 = a[322];
    const double t2312 = (t1999*t74+t2004+t2005)*t74;
    const double t2315 = (t178*t1992+t1995+t1996+t2002)*t178;
    const double t2316 = t178*t2012;
    const double t2317 = t74*t2010;
    const double t2336 = t178*t2268;
    const double t2337 = t74*t2266;
    const double t2343 = a[657]*t684;
    const double t2344 = a[732];
    const double t2347 = a[666];
    const double t2363 = t178*t2063;
    const double t2364 = t74*t2061;
    const double t2385 = a[14];
    const double t2393 = (a[143]+(t14*a[618]+a[149])*t14)*t14;
    const double t2394 = a[120];
    const double t2399 = (t14*a[647]+a[215])*t14;
    const double t2400 = a[464];
    const double t2403 = a[603]*t14;
    const double t2404 = a[286];
    const double t2418 = a[797]*t74;
    const double t2420 = t14*a[414];
    const double t2421 = a[317];
    const double t2437 = a[36];
    const double t2438 = a[78];
    const double t2443 = (t14*a[653]+a[176])*t14;
    const double t2444 = a[468];
    const double t2447 = a[459]*t14;
    const double t2448 = a[343];
    const double t2452 = (t2438+t2443+(t2444*t74+t2447+t2448)*t74)*t74;
    const double t2454 = a[470]*t74;
    const double t2464 = (t2438+t2443+(t14*a[632]+t2454+a[194])*t74+(t178*t2444+t2447+t2448+
t2454)*t178)*t178;
    const double t2465 = a[81];
    const double t2466 = a[481];
    const double t2469 = t14*a[717];
    const double t2470 = a[280];
    const double t2472 = (t2466*t74+t2469+t2470)*t74;
    const double t2477 = (t178*t2466+t74*a[724]+t2469+t2470)*t178;
    const double t2478 = a[547];
    const double t2480 = a[602];
    const double t2481 = t178*t2480;
    const double t2482 = t74*t2480;
    const double t2483 = a[232];
    const double t2491 = a[691];
    const double t2494 = t14*a[746];
    const double t2495 = a[271];
    const double t2504 = a[815]*t6;
    const double t2505 = a[432];
    const double t2506 = t178*t2505;
    const double t2507 = t74*t2505;
    const double t2508 = a[381];
    const double t2524 = a[46];
    const double t2532 = (a[69]+(t14*a[889]+a[190])*t14)*t14;
    const double t2533 = a[98];
    const double t2538 = (t14*a[390]+a[296])*t14;
    const double t2539 = a[457];
    const double t2542 = t14*a[463];
    const double t2543 = a[273];
    const double t2548 = a[49];
    const double t2553 = (t14*a[568]+a[283])*t14;
    const double t2555 = a[759]*t74;
    const double t2557 = a[509]*t14;
    const double t2558 = a[338];
    const double t2561 = a[834];
    const double t2564 = a[508]*t74;
    const double t2566 = a[694]*t14;
    const double t2567 = a[231];
    const double t2572 = a[122];
    const double t2577 = (a[399]*t14+a[171])*t14;
    const double t2578 = a[573];
    const double t2581 = t14*a[475];
    const double t2582 = a[214];
    const double t2584 = (t2578*t74+t2581+t2582)*t74;
    const double t2585 = a[714];
    const double t2588 = a[750]*t74;
    const double t2590 = a[398]*t14;
    const double t2591 = a[158];
    const double t2593 = (t178*t2585+t2588+t2590+t2591)*t178;
    const double t2594 = a[650];
    const double t2595 = t6*t2594;
    const double t2596 = a[859];
    const double t2597 = t178*t2596;
    const double t2598 = a[620];
    const double t2599 = t74*t2598;
    const double t2601 = t14*a[527];
    const double t2602 = a[366];
    const double t2608 = a[801]*t6;
    const double t2609 = a[596];
    const double t2611 = a[428];
    const double t2614 = a[785]*t14;
    const double t2615 = a[223];
    const double t2618 = t287*t2594;
    const double t2623 = a[117];
    const double t2628 = (t14*a[869]+a[173])*t14;
    const double t2629 = a[587];
    const double t2632 = a[672]*t14;
    const double t2633 = a[179];
    const double t2636 = a[743];
    const double t2639 = a[430]*t74;
    const double t2641 = a[898]*t14;
    const double t2642 = a[237];
    const double t2645 = a[816];
    const double t2646 = t6*t2645;
    const double t2647 = a[405];
    const double t2648 = t178*t2647;
    const double t2649 = a[756];
    const double t2650 = t74*t2649;
    const double t2652 = a[740]*t14;
    const double t2653 = a[235];
    const double t2656 = t287*t2645;
    const double t2658 = a[665]*t6;
    const double t2661 = a[490];
    const double t2663 = a[544];
    const double t2664 = t287*t2663;
    const double t2665 = t6*t2663;
    const double t2666 = a[787];
    const double t2668 = a[467];
    const double t2671 = t14*a[790];
    const double t2672 = a[192];
    const double t2693 = (t2585*t74+t2590+t2591)*t74;
    const double t2696 = (t178*t2578+t2581+t2582+t2588)*t178;
    const double t2697 = t178*t2598;
    const double t2698 = t74*t2596;
    const double t2717 = a[674];
    const double t2720 = t14*a[772];
    const double t2721 = a[341];
    const double t2729 = a[758];
    const double t2731 = a[435];
    const double t2732 = t178*t2731;
    const double t2733 = t74*t2731;
    const double t2735 = t14*a[562];
    const double t2736 = a[220];
    const double t2745 = a[629]*t684;
    const double t2746 = a[813];
    const double t2747 = t287*t2746;
    const double t2748 = t6*t2746;
    const double t2749 = a[842];
    const double t2751 = a[549];
    const double t2754 = t14*a[589];
    const double t2755 = a[197];
    const double t2766 = t178*t2649;
    const double t2767 = t74*t2647;
    const double t2788 = a[86];
    const double t2793 = (t14*a[577]+a[224])*t14;
    const double t2794 = a[862];
    const double t2797 = t14*a[607];
    const double t2798 = a[177];
    const double t2804 = a[850]*t74;
    const double t2815 = a[61];
    const double t2816 = a[733];
    const double t2819 = a[894]*t14;
    const double t2820 = a[289];
    const double t2822 = (t2816*t74+t2819+t2820)*t74;
    const double t2827 = (t178*t2816+a[395]*t74+t2819+t2820)*t178;
    const double t2828 = a[794];
    const double t2830 = a[469];
    const double t2831 = t178*t2830;
    const double t2832 = t74*t2830;
    const double t2833 = a[162];
    const double t2839 = a[600]*t6;
    const double t2840 = a[460];
    const double t2851 = a[66];
    const double t2856 = (t14*a[670]+a[369])*t14;
    const double t2857 = a[662];
    const double t2860 = a[847]*t14;
    const double t2861 = a[262];
    const double t2864 = a[556];
    const double t2867 = a[699]*t74;
    const double t2869 = t14*a[712];
    const double t2870 = a[349];
    const double t2873 = a[608];
    const double t2874 = t6*t2873;
    const double t2875 = a[844];
    const double t2876 = t178*t2875;
    const double t2877 = a[886];
    const double t2878 = t74*t2877;
    const double t2880 = a[667]*t14;
    const double t2881 = a[213];
    const double t2884 = t287*t2873;
    const double t2886 = t6*a[723];
    const double t2889 = a[426];
    const double t2891 = a[855];
    const double t2892 = t287*t2891;
    const double t2893 = t6*t2891;
    const double t2894 = a[892];
    const double t2896 = a[876];
    const double t2899 = a[601]*t14;
    const double t2900 = a[282];
    const double t2911 = t178*t2877;
    const double t2912 = t74*t2875;
    const double t2918 = a[479]*t684;
    const double t2919 = a[640];
    const double t2922 = a[903];
    const double t2938 = a[698];
    const double t2941 = t14*a[578];
    const double t2942 = a[290];
    const double t2950 = a[518];
    const double t2952 = a[702];
    const double t2953 = t178*t2952;
    const double t2954 = t74*t2952;
    const double t2955 = a[261];
    const double t2963 = a[827];
    const double t2965 = a[742];
    const double t2966 = t287*t2965;
    const double t2967 = t6*t2965;
    const double t2968 = a[492];
    const double t2970 = a[671];
    const double t2973 = t14*a[779];
    const double t2974 = a[219];
    const double t2986 = a[768];
    const double t2989 = a[684];
    const double t2992 = a[846];
    const double t2068 = x[1];
    const double t3001 = x[0];
    return((t1+(t2+(t3+(t4+(t5*t6+t7)*t6)*t6)*t6)*t6)*t6+(t1+(a[9]+(t19+(t20+(t22+t23)*t6)*t6)*t6)*t6+(t2+(t19+(a[91]+(t34+t35)*t6)*t6)*t6+(t3+(t20+(t6*a
[521]+t35)*t6)*t6+(t4+(t34+t23)*t6+(t287*t5+t22+t7)*t287)*t287)*t287)*t287)*
t287+(t61+t75+(t76+t87+(t88+t96+(t97+t102+(t103*t74+t106+t107)*t74)*t74)*t74)*
t74+(t126+(t127+t135+(t136+t141+(t143+t145+t146)*t74)*t74)*t74+(t160+(t161+t166
+(t168+t170+t171)*t74)*t74+(t180+(t182+t184+t185)*t74+(t189+t191)*t178)*t178)*
t178)*t178+(t200+t211+t237+t282+(t283+t291+t306+t326+(t327+t332+t339+t345+(t347
+t349+t351+t352)*t6)*t6)*t6)*t6+(t200+t211+t237+t282+(t361+t369+(t370+t375+(
t376*t74+t379+t380)*t74)*t74+(t389+(t391+t393+t394)*t74+(t398+t400)*t178)*t178+
(t405+t410+t417+t423+(t425+t427+t429+t430)*t6)*t6)*t6+(t283+t291+t306+t326+(
t405+t410+t417+t423+(t439*t74+t438+t442+t443)*t6)*t6+(t327+t332+t339+t345+(t438
+t427+t429+t430)*t6+(t450+t425+t349+t351+t352)*t287)*t287)*t287)*t287+(t459+
t470+(t471+t479+(t480+t485+(t486*t74+t489+t490)*t74)*t74)*t74+(t504+(t505+t510+
(t512+t514+t515)*t74)*t74+(t524+(t526+t528+t529)*t74+(t533+t535)*t178)*t178)*
t178+(t542+t550+t565+t585+(t586+t591+t598+t604+(t606+t608+t610+t611)*t6)*t6)*t6
+(t542+t550+t565+t585+(t618+t623+(t624*t74+t627+t628)*t74+(t14*t633+t632)*t178+
(t638+t640+t642+t643)*t6)*t6+(t586+t591+t598+t604+(t649+t640+t642+t643)*t6+(
t652+t638+t608+t610+t611)*t287)*t287)*t287+(t659+t667+(t668+t673+(t674*t74+t677
+t678)*t74)*t74+(t687+(t689+t691+t692)*t74+(t696+t698)*t178)*t178+(t703+t708+
t715+t721+(t723+t725+t727+t728)*t6)*t6+(t703+t708+t715+t721+(t735*t74+t734+t738
+t739)*t6+(t742+t734+t725+t727+t728)*t287)*t287+(t747+t752+(t74*t753+t756+t757)
*t74+(t14*t762+t761)*t178+(t767+t769+t771+t772)*t6+(t775+t777+t769+t771+t772)*
t287+(t684*t780+t74*t785+t783+t784+t788+t789)*t684)*t684)*t684)*t684)*t684+(t61
+t75+(t126+(t160+(t191*t74+t180)*t74)*t74)*t74+(t76+t87+(t127+t135+(t161+t166+(
t189+t184+t185)*t74)*t74)*t74+(t88+t96+(t136+t141+(t182+t170+t171)*t74)*t74+(
t97+t102+(t168+t145+t146)*t74+(t103*t178+t106+t107+t143)*t178)*t178)*t178)*t178
+(t200+t211+t832+t845+(t283+t291+t848+t855+(t327+t332+t857+t860+(t347+t861+t351
+t352)*t6)*t6)*t6)*t6+(t200+t211+t832+t845+(t361+t369+(t400*t74+t389)*t74+(t370
+t375+(t398+t393+t394)*t74+(t178*t376+t379+t380+t391)*t178)*t178+(t405+t410+
t881+t884+(t425+t885+t429+t430)*t6)*t6)*t6+(t283+t291+t848+t855+(t405+t410+t881
+t884+(t178*t439+t438+t442+t443)*t6)*t6+(t327+t332+t857+t860+(t438+t885+t429+
t430)*t6+(t450+t425+t861+t351+t352)*t287)*t287)*t287)*t287+(a[6]+(a[39]+(a[116]
+(t14*a[396]+a[281])*t14)*t14)*t14+(t919+t927+(t928+t933+(t74*t934+t937+t938)*
t74)*t74)*t74+(t919+t927+(a[145]+(t14*a[818]+a[260])*t14+(t952+t954+t955)*t74)*
t74+(t928+t933+(t74*a[841]+t954+t955)*t74+(t178*t934+t937+t938+t952)*t178)*t178
)*t178+(t971+t979+t994+t1006+(t1007+t1012+t1019+t1024+(t1025*t6+t1028+t1029+
t1031+t1032)*t6)*t6)*t6+(t971+t979+t994+t1006+(a[124]+(t14*a[644]+a[330])*t14+(
t1045*t74+t1048+t1049)*t74+(t1045*t178+t74*a[510]+t1048+t1049)*t178+(t1058+
t1060+t1061+t1063+t1064)*t6)*t6+(t1007+t1012+t1019+t1024+(t6*a[514]+t1060+t1061
+t1063+t1064)*t6+(t1025*t287+t1028+t1029+t1031+t1032+t1058)*t287)*t287)*t287+(
t1080+t1088+(t1089+t1094+(t1095*t74+t1098+t1099)*t74)*t74+(t1104+t1109+(t1111+
t1113+t1114)*t74+(t1117*t178+t1120+t1122+t1123)*t178)*t178+(t1128+t1133+t1140+
t1149+(t1151+t1153+t1155+t1157+t1158)*t6)*t6+(t1128+t1133+t1140+t1149+(t1165*
t178+t1167*t74+t1164+t1170+t1171)*t6+(t1174+t1164+t1153+t1155+t1157+t1158)*t287
)*t287+(t1179+t1184+(t1185*t74+t1188+t1189)*t74+(t1192*t178+t1195+t1197+t1198)*
t178+(t1202+t1204+t1206+t1208+t1209)*t6+(t1212+t1214+t1204+t1206+t1208+t1209)*
t287+(t1222*t178+t1224*t74+t1218+t1220+t1221+t1227+t1228)*t684)*t684)*t684)*
t684+(t459+t470+(t504+(t535*t74+t524)*t74)*t74+(t471+t479+(t505+t510+(t533+t528
+t529)*t74)*t74+(t480+t485+(t526+t514+t515)*t74+(t178*t486+t489+t490+t512)*t178
)*t178)*t178+(t542+t550+t1257+t1264+(t586+t591+t1265+t1268+(t606+t1269+t610+
t611)*t6)*t6)*t6+(t542+t550+t1257+t1264+(t618+t623+t633*t74*t14+(t178*t624+t627
+t628+t632)*t178+(t638+t1281+t642+t643)*t6)*t6+(t586+t591+t1265+t1268+(t649+
t1281+t642+t643)*t6+(t652+t638+t1269+t610+t611)*t287)*t287)*t287+(t1080+t1088+(
t1104+t1109+(t1117*t74+t1122+t1123)*t74)*t74+(t1089+t1094+(t1120+t1113+t1114)*
t74+(t1095*t178+t1098+t1099+t1111)*t178)*t178+(t1128+t1133+t1308+t1311+(t1151+
t1312+t1313+t1157+t1158)*t6)*t6+(t1128+t1133+t1308+t1311+(t1165*t74+t1167*t178+
t1164+t1170+t1171)*t6+(t1174+t1164+t1312+t1313+t1157+t1158)*t287)*t287+(a[135]+
(t14*a[736]+a[250])*t14+(t1332*t74+t1335+t1336)*t74+(t1332*t178+t74*a[764]+
t1335+t1336)*t178+(t1344*t6+t1347+t1348+t1350+t1351)*t6+(t1344*t287+t6*a[419]+
t1347+t1348+t1350+t1351)*t287+(t1364*t178+t1366*t74+t1360+t1362+t1363+t1369+
t1370)*t684)*t684)*t684+(t659+t667+(t698*t74+t687)*t74+(t668+t673+(t696+t691+
t692)*t74+(t178*t674+t677+t678+t689)*t178)*t178+(t703+t708+t1388+t1391+(t723+
t1392+t727+t728)*t6)*t6+(t703+t708+t1388+t1391+(t178*t735+t734+t738+t739)*t6+(
t742+t734+t1392+t727+t728)*t287)*t287+(t1179+t1184+(t1192*t74+t1197+t1198)*t74+
(t1185*t178+t1188+t1189+t1195)*t178+(t1202+t1410+t1411+t1208+t1209)*t6+(t1212+
t1214+t1410+t1411+t1208+t1209)*t287+(t1364*t74+t1366*t178+t684*a[619]+t1362+
t1363+t1369+t1370)*t684)*t684+(t747+t752+t762*t74*t14+(t178*t753+t756+t757+t761
)*t178+(t767+t1429+t771+t772)*t6+(t775+t777+t1429+t771+t772)*t287+(t1222*t74+
t1224*t178+t1220+t1221+t1227+t1228+t1360)*t684+(t178*t785+t2068*t780+t1218+t783
+t784+t788+t789)*t2068)*t2068)*t2068)*t2068)*t2068+(a[0]+(t1451+t1462+(t1463+
t1471+(t1472+t1477+(t1478*t74+t1481+t1482)*t74)*t74)*t74)*t74+(t1451+t1462+(a
[25]+(a[60]+(t14*a[560]+a[209])*t14)*t14+(t1500+t1505+(t1507+t1509+t1510)*t74)*
t74)*t74+(t1463+t1471+(t1500+t1505+(t14*a[410]+t1518+a[229])*t74)*t74+(t1472+
t1477+(t1518+t1509+t1510)*t74+(t1478*t178+t1481+t1482+t1507)*t178)*t178)*t178)*
t178+(t1537+t1563+t1589+(t1590+t1605+t1617+(t1618+t1625+t1630+(t1631*t6+t1634+
t1635+t1636)*t6)*t6)*t6)*t6+(t1537+t1563+t1589+(a[34]+(t1646+t1651+(t1652*t74+
t1655+t1656)*t74)*t74+(t1646+t1651+(t14*a[835]+t1662+a[243])*t74+(t1652*t178+
t1655+t1656+t1662)*t178)*t178+(t1673+t1680+t1685+(t1687+t1689+t1690+t1691)*t6)*
t6)*t6+(t1590+t1605+t1617+(t1673+t1680+t1685+(t1700*t178+t1700*t74+t1699+a[193]
)*t6)*t6+(t1618+t1625+t1630+(t1699+t1689+t1690+t1691)*t6+(t1631*t287+t1634+
t1635+t1636+t1687)*t287)*t287)*t287)*t287+(t1719+t1730+(t1731+t1739+(t1740+
t1745+(t1746*t74+t1749+t1750)*t74)*t74)*t74+(t1757+t1765+(t1766+t1771+(t1773+
t1775+t1776)*t74)*t74+(t1781+t1786+(t1788+t1790+t1791)*t74+(t178*t1794+t1797+
t1799+t1800)*t178)*t178)*t178+(t1807+t1815+t1830+t1854+(t1855+t1860+t1867+t1876
+(t1878+t1880+t1882+t1884+t1885)*t6)*t6)*t6+(t1807+t1815+t1830+t1854+(t1892+
t1897+(t1898*t74+t1901+t1902)*t74+(t178*t1905+t1908+t1910+t1911)*t178+(t1915+
t1917+t1919+t1921+t1922)*t6)*t6+(t1855+t1860+t1867+t1876+(t1928+t1917+t1919+
t1921+t1922)*t6+(t1931+t1915+t1880+t1882+t1884+t1885)*t287)*t287)*t287+(t1938+
t1946+(t1947+t1952+(t1953*t74+t1956+t1957)*t74)*t74+(t1962+t1967+(t1969+t1971+
t1972)*t74+(t178*t1975+t1978+t1980+t1981)*t178)*t178+(t1986+t1991+t1998+t2007+(
t2009+t2011+t2013+t2015+t2016)*t6)*t6+(t1986+t1991+t1998+t2007+(t178*t2023+
t2025*t74+t2022+t2028+t2029)*t6+(t2032+t2022+t2011+t2013+t2015+t2016)*t287)*
t287+(t2037+t2042+(t2043*t74+t2046+t2047)*t74+(t178*t2050+t2053+t2055+t2056)*
t178+(t2060+t2062+t2064+t2066+t2067)*t6+(t2070+t2072+t2062+t2064+t2066+t2067)*
t287+(t178*t2080+t2075*t684+t2082*t74+t2078+t2079+t2085+t2086)*t684)*t684)*t684
)*t684+(t1719+t1730+(t1757+t1765+(t1781+t1786+(t1794*t74+t1799+t1800)*t74)*t74)
*t74+(t1731+t1739+(t1766+t1771+(t1797+t1790+t1791)*t74)*t74+(t1740+t1745+(t1788
+t1775+t1776)*t74+(t1746*t178+t1749+t1750+t1773)*t178)*t178)*t178+(t1807+t1815+
t2119+t2126+(t1855+t1860+t2129+t2132+(t1878+t2133+t2134+t1884+t1885)*t6)*t6)*t6
+(t1807+t1815+t2119+t2126+(t1892+t1897+(t1905*t74+t1910+t1911)*t74+(t178*t1898+
t1901+t1902+t1908)*t178+(t1915+t2147+t2148+t1921+t1922)*t6)*t6+(t1855+t1860+
t2129+t2132+(t1928+t2147+t2148+t1921+t1922)*t6+(t1931+t1915+t2133+t2134+t1884+
t1885)*t287)*t287)*t287+(a[40]+(a[51]+(t14*a[778]+a[228])*t14)*t14+(t2170+t2175
+(t2176*t74+t2179+t2180)*t74)*t74+(t2170+t2175+(t14*a[829]+t2186+a[324])*t74+(
t178*t2176+t2179+t2180+t2186)*t178)*t178+(t2197+t2202+t2209+t2214+(t2215*t6+
t2218+t2219+t2221+t2222)*t6)*t6+(t2197+t2202+t2209+t2214+(t14*a[442]+t178*t2229
+t2229*t74+t2228+a[217])*t6+(t2215*t287+t2218+t2219+t2221+t2222+t2228)*t287)*
t287+(t2242+t2247+(t2248*t74+t2251+t2252)*t74+(t178*t2255+t2258+t2260+t2261)*
t178+(t2265+t2267+t2269+t2271+t2272)*t6+(t2275+t2277+t2267+t2269+t2271+t2272)*
t287+(t178*t2285+t2287*t74+t2281+t2283+t2284+t2290+t2291)*t684)*t684)*t684+(
t1938+t1946+(t1962+t1967+(t1975*t74+t1980+t1981)*t74)*t74+(t1947+t1952+(t1978+
t1971+t1972)*t74+(t178*t1953+t1956+t1957+t1969)*t178)*t178+(t1986+t1991+t2312+
t2315+(t2009+t2316+t2317+t2015+t2016)*t6)*t6+(t1986+t1991+t2312+t2315+(t178*
t2025+t2023*t74+t2022+t2028+t2029)*t6+(t2032+t2022+t2316+t2317+t2015+t2016)*
t287)*t287+(t2242+t2247+(t2255*t74+t2260+t2261)*t74+(t178*t2248+t2251+t2252+
t2258)*t178+(t2265+t2336+t2337+t2271+t2272)*t6+(t2275+t2277+t2336+t2337+t2271+
t2272)*t287+(t14*a[789]+t178*t2347+t2344*t287+t2344*t6+t2347*t74+t2343+a[384])*
t684)*t684+(t2037+t2042+(t2050*t74+t2055+t2056)*t74+(t178*t2043+t2046+t2047+
t2053)*t178+(t2060+t2363+t2364+t2066+t2067)*t6+(t2070+t2072+t2363+t2364+t2066+
t2067)*t287+(t178*t2287+t2285*t74+t2283+t2284+t2290+t2291+t2343)*t684+(t178*
t2082+t2068*t2075+t2080*t74+t2078+t2079+t2085+t2086+t2281)*t2068)*t2068)*t2068)
*t2068+(a[10]+(t2385+t2393+(t2394+t2399+(t2400*t74+t2403+t2404)*t74)*t74)*t74+(
t2385+t2393+(a[80]+(t14*a[879]+a[247])*t14+(t2418+t2420+t2421)*t74)*t74+(t2394+
t2399+(t74*a[550]+t2420+t2421)*t74+(t178*t2400+t2403+t2404+t2418)*t178)*t178)*
t178+(t2437+t2452+t2464+(t2465+t2472+t2477+(t2478*t6+t2481+t2482+t2483)*t6)*t6)
*t6+(t2437+t2452+t2464+(a[104]+(t2491*t74+t2494+t2495)*t74+(t178*t2491+t74*a
[543]+t2494+t2495)*t178+(t2504+t2506+t2507+t2508)*t6)*t6+(t2465+t2472+t2477+(t6
*a[722]+t2506+t2507+t2508)*t6+(t2478*t287+t2481+t2482+t2483+t2504)*t287)*t287)*
t287+(t2524+t2532+(t2533+t2538+(t2539*t74+t2542+t2543)*t74)*t74+(t2548+t2553+(
t2555+t2557+t2558)*t74+(t178*t2561+t2564+t2566+t2567)*t178)*t178+(t2572+t2577+
t2584+t2593+(t2595+t2597+t2599+t2601+t2602)*t6)*t6+(t2572+t2577+t2584+t2593+(
t178*t2609+t2611*t74+t2608+t2614+t2615)*t6+(t2618+t2608+t2597+t2599+t2601+t2602
)*t287)*t287+(t2623+t2628+(t2629*t74+t2632+t2633)*t74+(t178*t2636+t2639+t2641+
t2642)*t178+(t2646+t2648+t2650+t2652+t2653)*t6+(t2656+t2658+t2648+t2650+t2652+
t2653)*t287+(t178*t2666+t2661*t684+t2668*t74+t2664+t2665+t2671+t2672)*t684)*
t684)*t684+(t2524+t2532+(t2548+t2553+(t2561*t74+t2566+t2567)*t74)*t74+(t2533+
t2538+(t2564+t2557+t2558)*t74+(t178*t2539+t2542+t2543+t2555)*t178)*t178+(t2572+
t2577+t2693+t2696+(t2595+t2697+t2698+t2601+t2602)*t6)*t6+(t2572+t2577+t2693+
t2696+(t178*t2611+t2609*t74+t2608+t2614+t2615)*t6+(t2618+t2608+t2697+t2698+
t2601+t2602)*t287)*t287+(a[84]+(t14*a[610]+a[302])*t14+(t2717*t74+t2720+t2721)*
t74+(t178*t2717+t74*a[485]+t2720+t2721)*t178+(t2729*t6+t2732+t2733+t2735+t2736)
*t6+(t2729*t287+t6*a[622]+t2732+t2733+t2735+t2736)*t287+(t178*t2749+t2751*t74+
t2745+t2747+t2748+t2754+t2755)*t684)*t684+(t2623+t2628+(t2636*t74+t2641+t2642)*
t74+(t178*t2629+t2632+t2633+t2639)*t178+(t2646+t2766+t2767+t2652+t2653)*t6+(
t2656+t2658+t2766+t2767+t2652+t2653)*t287+(t178*t2751+t2749*t74+t684*a[784]+
t2747+t2748+t2754+t2755)*t684+(t178*t2668+t2068*t2661+t2666*t74+t2664+t2665+
t2671+t2672+t2745)*t2068)*t2068)*t2068+(a[18]+(t2788+t2793+(t2794*t74+t2797+
t2798)*t74)*t74+(t2788+t2793+(t14*a[551]+t2804+a[356])*t74+(t178*t2794+t2797+
t2798+t2804)*t178)*t178+(t2815+t2822+t2827+(t2828*t6+t2831+t2832+t2833)*t6)*t6+
(t2815+t2822+t2827+(t178*t2840+t2840*t74+t2839+a[200])*t6+(t2828*t287+t2831+
t2832+t2833+t2839)*t287)*t287+(t2851+t2856+(t2857*t74+t2860+t2861)*t74+(t178*
t2864+t2867+t2869+t2870)*t178+(t2874+t2876+t2878+t2880+t2881)*t6+(t2884+t2886+
t2876+t2878+t2880+t2881)*t287+(t178*t2894+t2889*t684+t2896*t74+t2892+t2893+
t2899+t2900)*t684)*t684+(t2851+t2856+(t2864*t74+t2869+t2870)*t74+(t178*t2857+
t2860+t2861+t2867)*t178+(t2874+t2911+t2912+t2880+t2881)*t6+(t2884+t2886+t2911+
t2912+t2880+t2881)*t287+(t14*a[611]+t178*t2922+t287*t2919+t2919*t6+t2922*t74+
t2918+a[242])*t684+(t178*t2896+t2068*t2889+t2894*t74+t2892+t2893+t2899+t2900+
t2918)*t2068)*t2068+(a[102]+(t2938*t74+t2941+t2942)*t74+(t178*t2938+a[488]*t74+
t2941+t2942)*t178+(t2950*t6+t2953+t2954+t2955)*t6+(t287*t2950+t6*a[579]+t2953+
t2954+t2955)*t287+(t178*t2968+t2963*t684+t2970*t74+t2966+t2967+t2973+t2974)*
t684+(t178*t2970+t2068*t2963+t2968*t74+t684*a[616]+t2966+t2967+t2973+t2974)*
t2068+(t178*t2992+t2068*t2986+t287*t2989+t2986*t684+t2989*t6+t2992*t74+t3001*a
[499]+a[168])*t3001)*t3001)*t3001)*t3001)*t3001);
}

} // namespace mbnrg_A1_B1C2X2_deg6

