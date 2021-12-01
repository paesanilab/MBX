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

double poly_1b_A1B4_deg5_exp0_v1x::eval(const double a[207], const double x[10]) {
    const double t1 = a[0];
    const double t2 = a[7];
    const double t3 = a[26];
    const double t4 = a[203];
    const double t6 = a[45];
    const double t15 = a[2];
    const double t16 = a[20];
    const double t17 = a[128];
    const double t5 = x[9];
    const double t18 = t17 * t5;
    const double t19 = a[31];
    const double t25 = (t15 + (t16 + (t18 + t19) * t5) * t5) * t5;
    const double t26 = a[103];
    const double t27 = t26 * t5;
    const double t28 = a[29];
    const double t32 = (t16 + (t27 + t28) * t5) * t5;
    const double t34 = (t27 + t19) * t5;
    const double t44 = a[11];
    const double t45 = a[126];
    const double t46 = t45 * t5;
    const double t47 = a[47];
    const double t51 = (t44 + (t46 + t47) * t5) * t5;
    const double t52 = a[148];
    const double t53 = t52 * t5;
    const double t55 = (t53 + t47) * t5;
    const double t29 = x[8];
    const double t56 = t17 * t29;
    const double t63 = t26 * t29;
    const double t79 = a[21];
    const double t80 = a[131];
    const double t81 = t80 * t5;
    const double t82 = a[34];
    const double t86 = (t79 + (t81 + t82) * t5) * t5;
    const double t87 = a[85];
    const double t88 = t87 * t5;
    const double t90 = (t88 + t82) * t5;
    const double t97 = a[5];
    const double t98 = a[13];
    const double t99 = a[145];
    const double t100 = t99 * t5;
    const double t101 = a[51];
    const double t105 = (t98 + (t100 + t101) * t5) * t5;
    const double t106 = a[99];
    const double t107 = t106 * t5;
    const double t108 = a[30];
    const double t110 = (t107 + t108) * t5;
    const double t111 = t99 * t29;
    const double t116 = a[27];
    const double t117 = a[136];
    const double t118 = t117 * t5;
    const double t119 = a[57];
    const double t121 = (t118 + t119) * t5;
    const double t122 = t117 * t29;
    const double t123 = a[176];
    const double t124 = t123 * t5;
    const double t127 = a[165];
    const double t41 = x[7];
    const double t128 = t127 * t41;
    const double t129 = a[112];
    const double t130 = t129 * t29;
    const double t131 = t129 * t5;
    const double t132 = a[66];
    const double t143 = a[87];
    const double t144 = t143 * t5;
    const double t147 = a[200];
    const double t148 = t147 * t41;
    const double t149 = a[102];
    const double t150 = t149 * t29;
    const double t151 = t149 * t5;
    const double t152 = a[79];
    const double t170 = t127 * t29;
    const double t182 = (t122 + t107 + t101) * t29;
    const double t183 = t17 * t41;
    const double t194 = a[191];
    const double t195 = t194 * t29;
    const double t196 = a[206];
    const double t197 = t196 * t5;
    const double t198 = a[56];
    const double t201 = t129 * t41;
    const double t206 = t149 * t41;
    const double t43 = x[6];
    const double t209 = t17 * t43;
    const double t216 = t147 * t29;
    const double t223 = t26 * t41;
    const double t230 = t117 * t41;
    const double t233 = t26 * t43;
    const double t242 = t99 * t41;
    const double t254 = t127 * t5;
    const double t264 = (t98 + (t131 + t119) * t5) * t5;
    const double t266 = (t118 + t101) * t5;
    const double t274 = (t124 + t108) * t5;
    const double t275 = t80 * t29;
    const double t280 = t87 * t29;
    const double t290 = (t144 + t108) * t5;
    const double t291 = t45 * t29;
    const double t296 = t194 * t5;
    const double t298 = (t296 + t198) * t5;
    const double t299 = t106 * t29;
    const double t302 = t143 * t29;
    const double t307 = t52 * t29;
    const double t310 = t123 * t29;
    const double t325 = t45 * t41;
    const double t336 = t80 * t43;
    const double t344 = t52 * t41;
    const double t347 = t87 * t43;
    const double t348 = t106 * t41;
    const double t60 = x[5];
    const double t351 = t17 * t60;
    const double t358 = t147 * t5;
    const double t364 = (t151 + t119) * t5;
    const double t389 = t26 * t60;
    const double t413 = a[1];
    const double t414 = a[4];
    const double t415 = a[24];
    const double t416 = a[199];
    const double t418 = a[54];
    const double t424 = (t414 + (t415 + (t416 * t5 + t418) * t5) * t5) * t5;
    const double t425 = a[18];
    const double t426 = a[187];
    const double t427 = t426 * t5;
    const double t428 = a[36];
    const double t432 = (t425 + (t427 + t428) * t5) * t5;
    const double t433 = a[93];
    const double t436 = (t433 * t5 + t428) * t5;
    const double t437 = t416 * t29;
    const double t444 = a[3];
    const double t445 = a[12];
    const double t446 = a[129];
    const double t447 = t446 * t5;
    const double t448 = a[35];
    const double t452 = (t445 + (t447 + t448) * t5) * t5;
    const double t453 = a[163];
    const double t454 = t453 * t5;
    const double t455 = a[77];
    const double t457 = (t454 + t455) * t5;
    const double t458 = t446 * t29;
    const double t463 = a[14];
    const double t464 = a[104];
    const double t465 = t464 * t5;
    const double t466 = a[61];
    const double t468 = (t465 + t466) * t5;
    const double t469 = t464 * t29;
    const double t470 = a[175];
    const double t471 = t470 * t5;
    const double t474 = a[113];
    const double t475 = t474 * t41;
    const double t476 = a[120];
    const double t477 = t476 * t29;
    const double t478 = t476 * t5;
    const double t479 = a[65];
    const double t486 = a[202];
    const double t487 = t486 * t5;
    const double t488 = a[81];
    const double t490 = (t487 + t488) * t5;
    const double t491 = t426 * t29;
    const double t496 = a[17];
    const double t497 = a[137];
    const double t498 = t497 * t5;
    const double t499 = a[68];
    const double t501 = (t498 + t499) * t5;
    const double t502 = t497 * t29;
    const double t503 = a[121];
    const double t504 = t503 * t5;
    const double t507 = a[172];
    const double t508 = t507 * t41;
    const double t509 = a[159];
    const double t510 = t509 * t29;
    const double t511 = t509 * t5;
    const double t512 = a[53];
    const double t517 = t433 * t29;
    const double t520 = a[116];
    const double t521 = t520 * t41;
    const double t522 = a[109];
    const double t523 = t522 * t29;
    const double t524 = t522 * t5;
    const double t525 = a[48];
    const double t528 = t416 * t43;
    const double t529 = a[169];
    const double t530 = t529 * t41;
    const double t537 = t529 * t29;
    const double t542 = a[9];
    const double t543 = a[100];
    const double t544 = t543 * t5;
    const double t545 = a[33];
    const double t547 = (t544 + t545) * t5;
    const double t548 = a[122];
    const double t549 = t548 * t29;
    const double t550 = a[114];
    const double t551 = t550 * t5;
    const double t552 = a[62];
    const double t555 = a[89];
    const double t556 = t555 * t41;
    const double t557 = a[188];
    const double t558 = t557 * t29;
    const double t559 = a[158];
    const double t560 = t559 * t5;
    const double t561 = a[32];
    const double t568 = a[96];
    const double t569 = t568 * t41;
    const double t570 = a[119];
    const double t571 = t570 * t29;
    const double t574 = t446 * t43;
    const double t575 = t548 * t41;
    const double t580 = t520 * t29;
    const double t583 = a[173];
    const double t584 = t583 * t41;
    const double t585 = t568 * t29;
    const double t588 = t464 * t43;
    const double t589 = t557 * t41;
    const double t592 = t474 * t60;
    const double t593 = t476 * t43;
    const double t594 = t507 * t29;
    const double t601 = t529 * t5;
    const double t605 = (t496 + (t601 + t525) * t5) * t5;
    const double t607 = (t524 + t499) * t5;
    const double t612 = t548 * t5;
    const double t614 = (t612 + t552) * t5;
    const double t615 = t543 * t29;
    const double t618 = t559 * t29;
    const double t619 = t557 * t5;
    const double t624 = t453 * t29;
    const double t627 = t550 * t29;
    const double t628 = t570 * t5;
    const double t637 = a[162];
    const double t638 = t637 * t41;
    const double t639 = a[184];
    const double t640 = t639 * t29;
    const double t641 = t639 * t5;
    const double t642 = a[71];
    const double t645 = t543 * t43;
    const double t649 = t555 * t60;
    const double t650 = t559 * t43;
    const double t655 = t520 * t5;
    const double t657 = (t655 + t512) * t5;
    const double t660 = t568 * t5;
    const double t663 = t470 * t29;
    const double t96 = x[4];
    const double t669 = t474 * t96;
    const double t670 = t507 * t5;
    const double t677 = a[6];
    const double t678 = a[16];
    const double t679 = a[117];
    const double t681 = a[76];
    const double t685 = (t678 + (t5 * t679 + t681) * t5) * t5;
    const double t686 = a[97];
    const double t687 = t686 * t5;
    const double t688 = a[41];
    const double t690 = (t687 + t688) * t5;
    const double t691 = t679 * t29;
    const double t696 = a[22];
    const double t697 = a[125];
    const double t698 = t697 * t5;
    const double t699 = a[40];
    const double t701 = (t698 + t699) * t5;
    const double t702 = t697 * t29;
    const double t703 = a[133];
    const double t704 = t703 * t5;
    const double t707 = a[178];
    const double t708 = t707 * t41;
    const double t709 = a[170];
    const double t710 = t709 * t29;
    const double t711 = t709 * t5;
    const double t712 = a[46];
    const double t717 = t686 * t29;
    const double t718 = a[198];
    const double t719 = t718 * t5;
    const double t722 = a[193];
    const double t723 = t722 * t41;
    const double t724 = a[101];
    const double t725 = t724 * t29;
    const double t726 = t724 * t5;
    const double t727 = a[44];
    const double t730 = t679 * t43;
    const double t731 = a[149];
    const double t732 = t731 * t41;
    const double t737 = t731 * t29;
    const double t740 = a[164];
    const double t741 = t740 * t41;
    const double t742 = a[166];
    const double t743 = t742 * t29;
    const double t744 = a[130];
    const double t745 = t744 * t5;
    const double t746 = a[50];
    const double t749 = t697 * t43;
    const double t750 = t742 * t41;
    const double t753 = t707 * t60;
    const double t754 = t709 * t43;
    const double t755 = t722 * t29;
    const double t760 = t731 * t5;
    const double t762 = (t760 + t727) * t5;
    const double t765 = t744 * t29;
    const double t766 = t742 * t5;
    const double t769 = t703 * t29;
    const double t772 = t740 * t60;
    const double t773 = t744 * t43;
    const double t774 = a[147];
    const double t778 = t707 * t96;
    const double t779 = t722 * t5;
    const double t784 = a[28];
    const double t785 = a[91];
    const double t787 = a[74];
    const double t789 = (t5 * t785 + t787) * t5;
    const double t790 = t785 * t29;
    const double t791 = a[156];
    const double t792 = t791 * t5;
    const double t795 = a[84];
    const double t796 = t795 * t41;
    const double t797 = a[150];
    const double t798 = t797 * t29;
    const double t799 = t797 * t5;
    const double t800 = a[38];
    const double t803 = t785 * t43;
    const double t804 = a[108];
    const double t805 = t804 * t41;
    const double t806 = t791 * t29;
    const double t809 = t795 * t60;
    const double t810 = t797 * t43;
    const double t811 = a[182];
    const double t812 = t811 * t41;
    const double t813 = t804 * t29;
    const double t816 = t795 * t96;
    const double t818 = t804 * t5;
    const double t821 = a[157];
    const double t823 = a[161];
    const double t824 = t823 * t96;
    const double t825 = t823 * t60;
    const double t826 = a[88];
    const double t827 = t826 * t43;
    const double t828 = t823 * t41;
    const double t829 = t826 * t29;
    const double t830 = t826 * t5;
    const double t831 = a[73];
    const double t840 = t474 * t29;
    const double t853 = t416 * t41;
    const double t860 = t555 * t29;
    const double t871 = t583 * t29;
    const double t876 = t474 * t43;
    const double t889 = t426 * t41;
    const double t896 = t522 * t41;
    const double t899 = t509 * t41;
    const double t906 = t433 * t41;
    const double t909 = t497 * t41;
    const double t912 = t416 * t60;
    const double t925 = t446 * t41;
    const double t930 = t637 * t29;
    const double t935 = t555 * t43;
    const double t942 = t453 * t41;
    const double t945 = t550 * t41;
    const double t948 = t446 * t60;
    const double t955 = t464 * t41;
    const double t958 = t583 * t43;
    const double t961 = t464 * t60;
    const double t962 = t470 * t41;
    const double t965 = t476 * t60;
    const double t966 = t476 * t41;
    const double t973 = a[8];
    const double t974 = a[10];
    const double t975 = a[194];
    const double t977 = a[58];
    const double t982 = a[23];
    const double t983 = a[94];
    const double t984 = t983 * t5;
    const double t985 = a[39];
    const double t987 = (t984 + t985) * t5;
    const double t988 = a[153];
    const double t989 = t988 * t29;
    const double t990 = a[105];
    const double t991 = t990 * t5;
    const double t992 = a[49];
    const double t997 = a[154];
    const double t998 = t997 * t29;
    const double t999 = a[140];
    const double t1000 = t999 * t5;
    const double t1001 = a[70];
    const double t1004 = t988 * t41;
    const double t1009 = a[86];
    const double t1010 = t1009 * t29;
    const double t1011 = a[151];
    const double t1012 = t1011 * t5;
    const double t1013 = a[43];
    const double t1016 = a[167];
    const double t1017 = t1016 * t41;
    const double t1018 = a[106];
    const double t1019 = t1018 * t29;
    const double t1020 = a[179];
    const double t1021 = t1020 * t5;
    const double t1022 = a[42];
    const double t1025 = t988 * t43;
    const double t1030 = t1016 * t29;
    const double t1033 = t1009 * t41;
    const double t1036 = t997 * t43;
    const double t1037 = t1018 * t41;
    const double t1040 = t988 * t60;
    const double t1045 = a[15];
    const double t1046 = a[144];
    const double t1047 = t1046 * t5;
    const double t1048 = a[78];
    const double t1051 = a[135];
    const double t1052 = t1051 * t29;
    const double t1053 = a[92];
    const double t1054 = t1053 * t5;
    const double t1055 = a[64];
    const double t1058 = t1051 * t41;
    const double t1059 = a[138];
    const double t1060 = t1059 * t29;
    const double t1063 = t1051 * t43;
    const double t1064 = a[118];
    const double t1065 = t1064 * t41;
    const double t1066 = a[185];
    const double t1067 = t1066 * t29;
    const double t1070 = t1051 * t60;
    const double t1071 = t1059 * t43;
    const double t1072 = t1066 * t41;
    const double t1073 = t1064 * t29;
    const double t1076 = a[204];
    const double t1078 = a[83];
    const double t1079 = t1078 * t60;
    const double t1080 = t1078 * t43;
    const double t1081 = t1078 * t41;
    const double t1082 = t1078 * t29;
    const double t1083 = a[115];
    const double t1084 = t1083 * t5;
    const double t1085 = a[52];
    const double t1090 = a[19];
    const double t1091 = a[82];
    const double t1093 = a[63];
    const double t1095 = (t1091 * t5 + t1093) * t5;
    const double t1096 = a[142];
    const double t1097 = t1096 * t29;
    const double t1098 = a[95];
    const double t1099 = t1098 * t5;
    const double t1100 = a[37];
    const double t1103 = a[107];
    const double t1104 = t1103 * t41;
    const double t1105 = a[146];
    const double t1106 = t1105 * t29;
    const double t1107 = a[98];
    const double t1108 = t1107 * t5;
    const double t1109 = a[55];
    const double t1112 = t1096 * t43;
    const double t1113 = a[168];
    const double t1114 = t1113 * t41;
    const double t1115 = a[90];
    const double t1116 = t1115 * t29;
    const double t1119 = t1103 * t60;
    const double t1120 = t1105 * t43;
    const double t1121 = a[127];
    const double t1122 = t1121 * t41;
    const double t1123 = t1113 * t29;
    const double t1126 = a[195];
    const double t1127 = t1126 * t96;
    const double t1128 = a[180];
    const double t1129 = t1128 * t60;
    const double t1130 = a[110];
    const double t1131 = t1130 * t43;
    const double t1132 = t1128 * t41;
    const double t1133 = t1130 * t29;
    const double t1134 = a[189];
    const double t1135 = t1134 * t5;
    const double t1136 = a[69];
    const double t1139 = a[190];
    const double t134 = x[3];
    const double t1140 = t1139 * t134;
    const double t1141 = a[201];
    const double t1142 = t1141 * t96;
    const double t1143 = a[134];
    const double t1144 = t1143 * t60;
    const double t1145 = a[139];
    const double t1146 = t1145 * t43;
    const double t1147 = t1143 * t41;
    const double t1148 = t1145 * t29;
    const double t1149 = a[183];
    const double t1150 = t1149 * t5;
    const double t1151 = a[59];
    const double t1158 = t707 * t29;
    const double t1165 = t679 * t41;
    const double t1170 = t740 * t29;
    const double t1175 = t707 * t43;
    const double t1182 = t686 * t41;
    const double t1185 = t724 * t41;
    const double t1188 = t679 * t60;
    const double t1195 = t697 * t41;
    const double t1198 = t740 * t43;
    const double t1202 = t697 * t60;
    const double t1203 = t703 * t41;
    const double t1206 = t709 * t60;
    const double t1207 = t709 * t41;
    const double t1212 = t1103 * t29;
    const double t1215 = t1096 * t41;
    const double t1218 = t1103 * t43;
    const double t1219 = t1121 * t29;
    const double t1222 = t1096 * t60;
    const double t1223 = t1115 * t41;
    const double t1226 = t1130 * t60;
    const double t1227 = t1128 * t43;
    const double t1228 = t1130 * t41;
    const double t1229 = t1128 * t29;
    const double t1232 = a[152];
    const double t1233 = t1232 * t134;
    const double t1234 = a[186];
    const double t1236 = a[143];
    const double t1237 = t1236 * t60;
    const double t1238 = t1236 * t43;
    const double t1239 = t1236 * t41;
    const double t1240 = t1236 * t29;
    const double t1241 = a[205];
    const double t1243 = a[67];
    const double t1248 = t795 * t29;
    const double t1251 = t785 * t41;
    const double t1254 = t795 * t43;
    const double t1255 = t811 * t29;
    const double t1258 = t785 * t60;
    const double t1259 = t791 * t41;
    const double t1262 = t797 * t60;
    const double t1263 = t811 * t43;
    const double t1264 = t797 * t41;
    const double t1267 = t1145 * t60;
    const double t1268 = t1143 * t43;
    const double t1269 = t1145 * t41;
    const double t1270 = t1143 * t29;
    const double t1274 = t826 * t60;
    const double t1275 = t823 * t43;
    const double t1276 = t826 * t41;
    const double t1277 = t823 * t29;
    const double t1292 = (t444 + (t463 + (t474 * t5 + t479) * t5) * t5) * t5;
    const double t1296 = (t445 + (t478 + t466) * t5) * t5;
    const double t1298 = (t465 + t448) * t5;
    const double t1306 = (t471 + t455) * t5;
    const double t1319 = t555 * t5;
    const double t1323 = (t542 + (t1319 + t561) * t5) * t5;
    const double t1325 = (t560 + t545) * t5;
    const double t1331 = (t619 + t552) * t5;
    const double t1340 = (t5 * t583 + t561) * t5;
    const double t1361 = t637 * t5;
    const double t1363 = (t1361 + t642) * t5;
    const double t1387 = (t496 + (t670 + t512) * t5) * t5;
    const double t1389 = (t511 + t499) * t5;
    const double t1394 = t486 * t29;
    const double t1402 = (t660 + t552) * t5;
    const double t1405 = t503 * t29;
    const double t1423 = (t655 + t525) * t5;
    const double t1432 = t416 * t96;
    const double t1443 = (t982 + (t5 * t988 + t992) * t5) * t5;
    const double t1445 = (t991 + t985) * t5;
    const double t1451 = t997 * t5;
    const double t1453 = (t1451 + t1001) * t5;
    const double t1454 = t983 * t29;
    const double t1457 = t990 * t29;
    const double t1462 = t1009 * t5;
    const double t1464 = (t1462 + t1013) * t5;
    const double t1467 = t1020 * t29;
    const double t1468 = t1018 * t5;
    const double t1475 = t1051 * t5;
    const double t1477 = (t1475 + t1055) * t5;
    const double t1478 = t1046 * t29;
    const double t1481 = t1053 * t29;
    const double t1482 = t1059 * t5;
    const double t1485 = t1066 * t5;
    const double t1489 = t1083 * t29;
    const double t1490 = t1078 * t5;
    const double t1495 = t1016 * t5;
    const double t1497 = (t1495 + t1022) * t5;
    const double t1500 = t1011 * t29;
    const double t1503 = t999 * t29;
    const double t1506 = t1064 * t5;
    const double t1509 = t988 * t96;
    const double t1516 = (t1096 * t5 + t1100) * t5;
    const double t1517 = t1091 * t29;
    const double t1520 = t1107 * t29;
    const double t1521 = t1105 * t5;
    const double t1524 = t1098 * t29;
    const double t1525 = t1115 * t5;
    const double t1528 = t1126 * t60;
    const double t1529 = t1134 * t29;
    const double t1530 = t1130 * t5;
    const double t1533 = t1103 * t96;
    const double t1534 = t1113 * t5;
    const double t1537 = t1143 * t96;
    const double t1538 = t1141 * t60;
    const double t1539 = t1149 * t29;
    const double t1540 = t1145 * t5;
    const double t1560 = t1046 * t41;
    const double t1564 = t1083 * t41;
    const double t1570 = (t1030 + t1468 + t1022) * t29;
    const double t1571 = t983 * t41;
    const double t1574 = t1053 * t41;
    const double t1577 = t990 * t41;
    const double t1588 = t997 * t60;
    const double t1589 = t999 * t41;
    const double t1596 = a[25];
    const double t1597 = a[141];
    const double t1599 = a[75];
    const double t1601 = (t1597 * t5 + t1599) * t5;
    const double t1602 = t1597 * t29;
    const double t1603 = a[192];
    const double t1604 = t1603 * t5;
    const double t1607 = t1597 * t41;
    const double t1608 = t1603 * t29;
    const double t1611 = a[197];
    const double t1612 = t1611 * t43;
    const double t1613 = a[177];
    const double t1614 = t1613 * t41;
    const double t1615 = a[181];
    const double t1616 = t1615 * t29;
    const double t1617 = t1615 * t5;
    const double t1618 = a[60];
    const double t1621 = t1611 * t60;
    const double t1622 = a[155];
    const double t1623 = t1622 * t43;
    const double t1624 = t1615 * t41;
    const double t1625 = t1613 * t29;
    const double t1628 = t1611 * t96;
    const double t1630 = t1613 * t5;
    const double t1633 = a[196];
    const double t1634 = t1633 * t134;
    const double t1635 = a[171];
    const double t1636 = t1635 * t96;
    const double t1637 = t1635 * t60;
    const double t1638 = a[111];
    const double t1639 = t1638 * t43;
    const double t1640 = a[123];
    const double t1641 = t1640 * t41;
    const double t1642 = a[124];
    const double t1643 = t1642 * t29;
    const double t1644 = t1642 * t5;
    const double t1645 = a[72];
    const double t1652 = t1091 * t41;
    const double t1655 = t1126 * t43;
    const double t1656 = t1134 * t41;
    const double t1659 = t1098 * t41;
    const double t1662 = t1105 * t60;
    const double t1663 = t1107 * t41;
    const double t1666 = a[132];
    const double t1667 = t1666 * t134;
    const double t1668 = t1638 * t60;
    const double t1669 = t1635 * t43;
    const double t1670 = t1642 * t41;
    const double t1671 = t1640 * t29;
    const double t185 = x[2];
    const double t1674 = t1139 * t185;
    const double t1675 = t1141 * t43;
    const double t1676 = t1149 * t41;
    const double t1687 = (t696 + (t5 * t707 + t712) * t5) * t5;
    const double t1689 = (t711 + t699) * t5;
    const double t1700 = t740 * t5;
    const double t1702 = (t1700 + t746) * t5;
    const double t1715 = t774 * t5;
    const double t1723 = (t779 + t727) * t5;
    const double t1733 = t679 * t96;
    const double t1740 = (t1103 * t5 + t1109) * t5;
    const double t1745 = t1121 * t5;
    const double t1748 = t1128 * t5;
    const double t1751 = t1096 * t96;
    const double t1754 = t1236 * t96;
    const double t1757 = t1236 * t5;
    const double t1772 = t1638 * t96;
    const double t1773 = t1640 * t5;
    const double t1776 = t1232 * t185;
    const double t1785 = (t5 * t795 + t800) * t5;
    const double t1790 = t811 * t5;
    const double t1795 = t785 * t96;
    const double t1798 = t1145 * t96;
    const double t1799 = t1143 * t5;
    const double t1805 = t826 * t96;
    const double t1806 = t823 * t5;
    const double t1867 = t426 * t43;
    const double t1876 = t433 * t43;
    const double t1907 = t486 * t43;
    const double t1911 = t426 * t60;
    const double t1953 = t983 * t43;
    const double t1956 = t990 * t43;
    const double t1967 = t1009 * t60;
    const double t1968 = t1011 * t43;
    const double t1977 = t1126 * t41;
    const double t1980 = t1091 * t43;
    const double t1983 = t1107 * t43;
    const double t1989 = t1149 * t43;
    const double t1990 = t1141 * t41;
    const double t2020 = t1020 * t41;
    const double t2030 = t997 * t41;
    const double t2033 = t1009 * t43;
    const double t2036 = t983 * t60;
    const double t2039 = t990 * t60;
    const double t2044 = t1611 * t29;
    const double t2047 = t1611 * t41;
    const double t2048 = t1622 * t29;
    const double t2051 = t1597 * t43;
    const double t2054 = t1597 * t60;
    const double t2055 = t1603 * t43;
    const double t2058 = t1615 * t60;
    const double t2059 = t1615 * t43;
    const double t2060 = t1622 * t41;
    const double t2063 = t1640 * t60;
    const double t2064 = t1642 * t43;
    const double t2065 = t1635 * t41;
    const double t2066 = t1638 * t29;
    const double t2071 = t1126 * t29;
    const double t2078 = t1091 * t60;
    const double t2081 = t1107 * t60;
    const double t2082 = t1121 * t43;
    const double t2083 = t1105 * t41;
    const double t2086 = t1642 * t60;
    const double t2087 = t1640 * t43;
    const double t2088 = t1638 * t41;
    const double t2089 = t1635 * t29;
    const double t2092 = t1149 * t60;
    const double t2093 = t1141 * t29;
    const double t2106 = (t1490 + t1055) * t5;
    const double t2152 = (t1611 * t5 + t1618) * t5;
    const double t2155 = t1622 * t5;
    const double t2162 = t1597 * t96;
    const double t2165 = t1640 * t96;
    const double t2166 = t1638 * t5;
    const double t2177 = t1603 * t41;
    const double t2184 = a[160] * t134;
    const double t2185 = a[174];
    const double t2195 = t1633 * t185;
    const double t2202 = (t1126 * t5 + t1136) * t5;
    const double t2211 = t1091 * t96;
    const double t2214 = t1642 * t96;
    const double t2215 = t1635 * t5;
    const double t2218 = t1666 * t185;
    const double t211 = x[1];
    const double t2221 = t1139 * t211;
    const double t2222 = t1149 * t96;
    const double t2223 = t1141 * t5;
    const double t2252 = t686 * t43;
    const double t2265 = t686 * t60;
    const double t2279 = t1098 * t43;
    const double t2299 = t1098 * t60;
    const double t2300 = t1115 * t43;
    const double t2325 = t1232 * t211;
    const double t2338 = t791 * t43;
    const double t212 = x[0];
    const double t2351 = t212 * t821 + t1140 + t1274 + t1277 + t1674 + t1805 + t1806 + t2221 + t827 + t828 + t831;
    const double t2353 = t784 + t1785 + (t1248 + t1790 + t800) * t29 + (t796 + t1255 + t1790 + t800) * t41 +
                         (t803 + t805 + t798 + t799 + t787) * t43 + (t1258 + t2338 + t1264 + t813 + t799 + t787) * t60 +
                         (t60 * t791 + t1264 + t1795 + t2338 + t787 + t798 + t818) * t96 +
                         (t1233 + t1798 + t1267 + t1989 + t1990 + t1270 + t1799 + t1151) * t134 +
                         (t1776 + t1634 + t1798 + t2092 + t1146 + t1147 + t2093 + t1799 + t1151) * t185 +
                         (t2325 + t2195 + t1634 + t2222 + t1267 + t1146 + t1147 + t1270 + t2223 + t1151) * t211 +
                         t2351 * t212;
    const double t2355 =
        t677 + t1687 + (t696 + t1702 + (t1158 + t1700 + t712) * t29) * t29 +
        (t696 + t1702 + (t1170 + t1715 + t746) * t29 + (t708 + t1170 + t1700 + t712) * t41) * t41 +
        (t678 + t1689 + (t710 + t745 + t699) * t29 + (t723 + t743 + t766 + t727) * t41 +
         (t730 + t732 + t702 + t698 + t681) * t43) *
            t43 +
        (t678 + t1689 + (t755 + t766 + t727) * t29 + (t1207 + t743 + t745 + t699) * t41 +
         (t2252 + t1185 + t725 + t704 + t688) * t43 + (t1188 + t2252 + t1195 + t737 + t698 + t681) * t60) *
            t60 +
        (t678 + t1723 + (t710 + t766 + t699) * t29 + (t1207 + t765 + t766 + t699) * t41 +
         (t2252 + t1185 + t769 + t726 + t688) * t43 + (t43 * t718 + t1203 + t2265 + t688 + t725 + t726) * t60 +
         (t1733 + t2265 + t2252 + t1195 + t702 + t760 + t681) * t96) *
            t96 +
        (t1090 + t1740 + (t1212 + t1745 + t1109) * t29 + (t1977 + t1229 + t1748 + t1136) * t41 +
         (t1980 + t1656 + t1520 + t1108 + t1093) * t43 + (t1222 + t2279 + t1228 + t1123 + t1521 + t1100) * t60 +
         (t1115 * t60 + t1100 + t1106 + t1228 + t1534 + t1751 + t2279) * t96 +
         (t1234 * t41 + t1241 * t43 + t1233 + t1237 + t1240 + t1243 + t1754 + t1757) * t134) *
            t134 +
        (t1090 + t1740 + (t2071 + t1748 + t1136) * t29 + (t1104 + t1229 + t1745 + t1109) * t41 +
         (t1112 + t1114 + t1133 + t1521 + t1100) * t43 + (t2078 + t2279 + t1663 + t1529 + t1108 + t1093) * t60 +
         (t1751 + t2299 + t2300 + t2083 + t1133 + t1534 + t1100) * t96 +
         (t1667 + t1772 + t2086 + t2064 + t2065 + t2089 + t1773 + t1645) * t134 +
         (t1234 * t29 + t1241 * t60 + t1238 + t1239 + t1243 + t1667 + t1754 + t1757 + t1776) * t185) *
            t185 +
        (t1090 + t2202 + (t1212 + t1748 + t1109) * t29 + (t1104 + t1219 + t1748 + t1109) * t41 +
         (t1112 + t1114 + t1106 + t1530 + t1100) * t43 + (t1222 + t2300 + t2083 + t1123 + t1530 + t1100) * t60 +
         (t2211 + t2299 + t2279 + t1663 + t1520 + t1135 + t1093) * t96 +
         (t1667 + t2214 + t1668 + t2064 + t2065 + t1671 + t2215 + t1645) * t134 +
         (t2218 + t2184 + t2214 + t2086 + t1639 + t1641 + t2089 + t2215 + t1645) * t185 +
         (t1234 * t5 + t1241 * t96 + t1237 + t1238 + t1239 + t1240 + t1243 + t1667 + t2218 + t2325) * t211) *
            t211 +
        t2353 * t212;
    const double t2357 =
        t413 + t1292 + (t444 + t1323 + (t463 + t1340 + (t840 + t1319 + t479) * t29) * t29) * t29 +
        (t444 + t1323 + (t542 + t1363 + (t860 + t1361 + t561) * t29) * t29 +
         (t463 + t1340 + (t871 + t1361 + t561) * t29 + (t475 + t860 + t1319 + t479) * t41) * t41) *
            t41 +
        (t414 + t1296 + (t445 + t1325 + (t477 + t560 + t466) * t29) * t29 +
         (t496 + t1331 + (t558 + t641 + t552) * t29 + (t508 + t585 + t660 + t512) * t41) * t41 +
         (t415 + t1298 + (t469 + t544 + t448) * t29 + (t521 + t549 + t612 + t525) * t41 +
          (t528 + t530 + t458 + t447 + t418) * t43) *
             t43) *
            t43 +
        (t414 + t1296 + (t496 + t1331 + (t594 + t660 + t512) * t29) * t29 +
         (t445 + t1325 + (t585 + t641 + t552) * t29 + (t966 + t558 + t560 + t466) * t41) * t41 +
         (t425 + t1306 + (t510 + t551 + t499) * t29 + (t899 + t571 + t551 + t499) * t41 +
          (t1867 + t896 + t502 + t454 + t428) * t43) *
             t43 +
         (t415 + t1298 + (t580 + t612 + t525) * t29 + (t955 + t549 + t544 + t448) * t41 +
          (t1876 + t909 + t523 + t454 + t428) * t43 + (t912 + t1867 + t925 + t537 + t447 + t418) * t60) *
             t60) *
            t60 +
        (t414 + t1387 + (t445 + t1402 + (t477 + t619 + t466) * t29) * t29 +
         (t445 + t1402 + (t618 + t641 + t545) * t29 + (t966 + t618 + t619 + t466) * t41) * t41 +
         (t425 + t1389 + (t663 + t551 + t455) * t29 + (t899 + t627 + t628 + t499) * t41 +
          (t1867 + t896 + t624 + t498 + t428) * t43) *
             t43 +
         (t425 + t1389 + (t510 + t628 + t499) * t29 + (t962 + t627 + t551 + t455) * t41 +
          (t41 * t503 + t1405 + t1907 + t488 + t504) * t43 + (t1911 + t1907 + t942 + t523 + t498 + t428) * t60) *
             t60 +
         (t415 + t1423 + (t469 + t612 + t448) * t29 + (t955 + t615 + t612 + t448) * t41 +
          (t1876 + t909 + t624 + t524 + t428) * t43 + (t433 * t60 + t1907 + t428 + t502 + t524 + t942) * t60 +
          (t1432 + t1911 + t1867 + t925 + t458 + t601 + t418) * t96) *
             t96) *
            t96 +
        (t973 + t1443 + (t982 + t1464 + (t989 + t1462 + t992) * t29) * t29 +
         (t1045 + t1477 + (t1052 + t1485 + t1055) * t29 + (t1076 * t41 + t1082 + t1085 + t1490) * t41) * t41 +
         (t974 + t1445 + (t1457 + t1012 + t985) * t29 + (t1564 + t1481 + t1054 + t1048) * t41 +
          (t43 * t975 + t1454 + t1560 + t977 + t984) * t43) *
             t43 +
         (t982 + t1453 + t1570 + (t1081 + t1073 + t1482 + t1055) * t41 + (t1953 + t1574 + t1467 + t1000 + t985) * t43 +
          (t1040 + t1956 + t1058 + t1030 + t1451 + t992) * t60) *
             t60 +
         (t982 + t1497 + (t998 + t1468 + t1001) * t29 + (t1081 + t1060 + t1506 + t1055) * t41 +
          (t1953 + t1574 + t1503 + t1021 + t985) * t43 + (t1967 + t1968 + t1072 + t1019 + t1468 + t1013) * t60 +
          (t1509 + t1967 + t1956 + t1058 + t998 + t1495 + t992) * t96) *
             t96 +
         (t1090 + t1516 + (t1097 + t1525 + t1100) * t29 + (t1977 + t1133 + t1530 + t1136) * t41 +
          (t1980 + t1656 + t1524 + t1099 + t1093) * t43 + (t1119 + t1983 + t1132 + t1123 + t1521 + t1109) * t60 +
          (t1121 * t60 + t1106 + t1109 + t1132 + t1533 + t1534 + t1983) * t96 +
          (t1140 + t1537 + t1144 + t1989 + t1990 + t1148 + t1540 + t1151) * t134) *
             t134) *
            t134 +
        (t973 + t1443 + (t1045 + t1477 + (t1076 * t29 + t1085 + t1490) * t29) * t29 +
         (t982 + t1464 + (t1082 + t1485 + t1055) * t29 + (t1004 + t1052 + t1462 + t992) * t41) * t41 +
         (t982 + t1453 + (t1082 + t1482 + t1055) * t29 + (t1017 + t1073 + t1468 + t1022) * t41 +
          (t1025 + t1017 + t1052 + t1451 + t992) * t43) *
             t43 +
         (t974 + t1445 + (t1489 + t1054 + t1048) * t29 + (t1577 + t1481 + t1012 + t985) * t41 +
          (t1956 + t2020 + t1481 + t1000 + t985) * t43 + (t60 * t975 + t1478 + t1571 + t1953 + t977 + t984) * t60) *
             t60 +
         (t982 + t1497 + (t1082 + t1506 + t1055) * t29 + (t2030 + t1060 + t1468 + t1001) * t41 +
          (t2033 + t1037 + t1067 + t1468 + t1013) * t43 + (t2036 + t1968 + t1589 + t1481 + t1021 + t985) * t60 +
          (t1509 + t2039 + t2033 + t2030 + t1052 + t1495 + t992) * t96) *
             t96 +
         (t1596 + t1601 + (t2044 + t1617 + t1618) * t29 + (t2047 + t2048 + t1617 + t1618) * t41 +
          (t2051 + t1614 + t1616 + t1604 + t1599) * t43 + (t2054 + t2055 + t1624 + t1625 + t1604 + t1599) * t60 +
          (t1628 + t2058 + t2059 + t2060 + t2048 + t1630 + t1618) * t96 +
          (t1634 + t1636 + t2063 + t2064 + t2065 + t2066 + t1644 + t1645) * t134) *
             t134 +
         (t1090 + t1516 + (t2071 + t1530 + t1136) * t29 + (t1215 + t1133 + t1525 + t1100) * t41 +
          (t1218 + t1114 + t1229 + t1521 + t1109) * t43 + (t2078 + t1983 + t1659 + t1529 + t1099 + t1093) * t60 +
          (t1533 + t2081 + t2082 + t2083 + t1229 + t1534 + t1109) * t96 +
          (t1667 + t1636 + t2086 + t2087 + t2088 + t2089 + t1644 + t1645) * t134 +
          (t1674 + t1634 + t1537 + t2092 + t1268 + t1269 + t2093 + t1540 + t1151) * t185) *
             t185) *
            t185 +
        (t973 + (t1045 + (t1076 * t5 + t1085) * t5) * t5 + (t982 + t2106 + (t989 + t1475 + t992) * t29) * t29 +
         (t982 + t2106 + (t1010 + t1485 + t1013) * t29 + (t1004 + t1010 + t1475 + t992) * t41) * t41 +
         (t982 + t2106 + (t998 + t1482 + t1001) * t29 + (t1017 + t1019 + t1506 + t1022) * t41 +
          (t1025 + t1017 + t998 + t1475 + t992) * t43) *
             t43 +
         (t982 + t2106 + (t1030 + t1506 + t1022) * t29 + (t2030 + t1019 + t1482 + t1001) * t41 +
          (t2033 + t1037 + t1019 + t1485 + t1013) * t43 + (t1040 + t2033 + t2030 + t1030 + t1475 + t992) * t60) *
             t60 +
         (t974 + (t1084 + t1048) * t5 + (t1457 + t1054 + t985) * t29 + (t1577 + t1500 + t1054 + t985) * t41 +
          (t1956 + t2020 + t1503 + t1054 + t985) * t43 + (t2039 + t1968 + t1589 + t1467 + t1054 + t985) * t60 +
          (t96 * t975 + t1047 + t1454 + t1571 + t1953 + t2036 + t977) * t96) *
             t96 +
         (t1596 + t2152 + (t1602 + t1617 + t1599) * t29 + (t2047 + t1616 + t2155 + t1618) * t41 +
          (t2051 + t1614 + t1608 + t1617 + t1599) * t43 + (t1621 + t2059 + t2060 + t1625 + t2155 + t1618) * t60 +
          (t2162 + t2058 + t2055 + t1624 + t1608 + t1630 + t1599) * t96 +
          (t1634 + t2165 + t1637 + t2064 + t2065 + t1643 + t2166 + t1645) * t134) *
             t134 +
         (t1596 + t2152 + (t2044 + t2155 + t1618) * t29 + (t1607 + t1616 + t1617 + t1599) * t41 +
          (t1612 + t1614 + t2048 + t2155 + t1618) * t43 + (t2054 + t2059 + t2177 + t1625 + t1617 + t1599) * t60 +
          (t1603 * t60 + t1599 + t1616 + t1630 + t2059 + t2162 + t2177) * t96 +
          (t2185 * t29 + t2185 * t41 + t2185 * t43 + t2185 * t5 + t2185 * t60 + t2185 * t96 + t2184 + a[80]) * t134 +
          (t2195 + t2184 + t2165 + t2086 + t1669 + t1670 + t2089 + t2166 + t1645) * t185) *
             t185 +
         (t1090 + t2202 + (t1097 + t1530 + t1100) * t29 + (t1215 + t1116 + t1530 + t1100) * t41 +
          (t1218 + t1114 + t1106 + t1748 + t1109) * t43 + (t1119 + t2082 + t2083 + t1123 + t1748 + t1109) * t60 +
          (t2211 + t2081 + t1983 + t1659 + t1524 + t1135 + t1093) * t96 +
          (t1667 + t2214 + t1637 + t2087 + t2088 + t1643 + t2215 + t1645) * t134 +
          (t2218 + t2184 + t2214 + t2063 + t1669 + t1670 + t2066 + t2215 + t1645) * t185 +
          (t2221 + t2195 + t1634 + t2222 + t1144 + t1268 + t1269 + t1148 + t2223 + t1151) * t211) *
             t211) *
            t211 +
        t2355 * t212;
    double e =
        ((t1 + (t2 + (t3 + (t4 * t5 + t6) * t5) * t5) * t5) * t5 +
         (t1 + t25 + (t2 + t32 + (t3 + t34 + (t29 * t4 + t18 + t6) * t29) * t29) * t29) * t29 +
         (t1 + t25 + (t15 + t51 + (t16 + t55 + (t56 + t46 + t19) * t29) * t29) * t29 +
          (t2 + t32 + (t16 + t55 + (t63 + t53 + t28) * t29) * t29 +
           (t3 + t34 + (t63 + t46 + t19) * t29 + (t4 * t41 + t18 + t56 + t6) * t41) * t41) *
              t41) *
             t41 +
         (t1 + t25 + (t15 + t86 + (t16 + t90 + (t56 + t81 + t19) * t29) * t29) * t29 +
          (t97 + t105 + (t98 + t110 + (t111 + t107 + t101) * t29) * t29 +
           (t116 + t121 + (t122 + t124 + t119) * t29 + (t128 + t130 + t131 + t132) * t41) * t41) *
              t41 +
          (t2 + t32 + (t16 + t90 + (t63 + t88 + t28) * t29) * t29 +
           (t116 + t121 + (t122 + t144 + t119) * t29 + (t148 + t150 + t151 + t152) * t41) * t41 +
           (t3 + t34 + (t63 + t81 + t19) * t29 + (t148 + t130 + t131 + t132) * t41 +
            (t4 * t43 + t128 + t18 + t56 + t6) * t43) *
               t43) *
              t43) *
             t43 +
         (t1 + t25 + (t97 + t105 + (t116 + t121 + (t170 + t131 + t132) * t29) * t29) * t29 +
          (t15 + t86 + (t98 + t110 + (t130 + t124 + t119) * t29) * t29 +
           (t16 + t90 + t182 + (t183 + t111 + t81 + t19) * t41) * t41) *
              t41 +
          (t15 + t51 + (t98 + t110 + (t130 + t144 + t119) * t29) * t29 +
           (t98 + t110 + (t195 + t197 + t198) * t29 + (t201 + t195 + t144 + t119) * t41) * t41 +
           (t16 + t55 + t182 + (t206 + t195 + t124 + t119) * t41 + (t209 + t201 + t111 + t46 + t19) * t43) * t43) *
              t43 +
          (t2 + t32 + (t116 + t121 + (t216 + t151 + t152) * t29) * t29 +
           (t16 + t90 + (t150 + t144 + t119) * t29 + (t223 + t122 + t88 + t28) * t41) * t41 +
           (t16 + t55 + (t150 + t124 + t119) * t29 + (t230 + t195 + t107 + t101) * t41 +
            (t233 + t230 + t122 + t53 + t28) * t43) *
               t43 +
           (t3 + t34 + (t216 + t131 + t132) * t29 + (t223 + t130 + t81 + t19) * t41 +
            (t233 + t242 + t130 + t46 + t19) * t43 + (t4 * t60 + t170 + t18 + t183 + t209 + t6) * t60) *
               t60) *
              t60) *
             t60 +
         (t1 + (t97 + (t116 + (t254 + t132) * t5) * t5) * t5 +
          (t15 + t264 + (t16 + t266 + (t56 + t100 + t19) * t29) * t29) * t29 +
          (t15 + t264 + (t79 + t274 + (t275 + t107 + t82) * t29) * t29 +
           (t16 + t266 + (t280 + t107 + t82) * t29 + (t183 + t275 + t100 + t19) * t41) * t41) *
              t41 +
          (t15 + t264 + (t44 + t290 + (t291 + t107 + t47) * t29) * t29 +
           (t98 + t298 + (t299 + t197 + t108) * t29 + (t201 + t302 + t296 + t119) * t41) * t41 +
           (t16 + t266 + (t307 + t107 + t47) * t29 + (t206 + t310 + t296 + t119) * t41 +
            (t209 + t201 + t291 + t100 + t19) * t43) *
               t43) *
              t43 +
          (t15 + t264 + (t98 + t298 + (t130 + t296 + t119) * t29) * t29 +
           (t44 + t290 + (t302 + t197 + t108) * t29 + (t325 + t299 + t107 + t47) * t41) * t41 +
           (t79 + t274 + (t310 + t197 + t108) * t29 + (t123 * t41 + t196 * t29 + t108 + t197) * t41 +
            (t143 * t41 + t107 + t299 + t336 + t82) * t43) *
               t43 +
           (t16 + t266 + (t150 + t296 + t119) * t29 + (t344 + t310 + t107 + t47) * t41 +
            (t347 + t348 + t302 + t107 + t82) * t43 + (t351 + t336 + t325 + t130 + t100 + t19) * t60) *
               t60) *
              t60 +
          (t2 + (t116 + (t358 + t152) * t5) * t5 + (t16 + t364 + (t63 + t118 + t28) * t29) * t29 +
           (t16 + t364 + (t280 + t144 + t82) * t29 + (t223 + t280 + t118 + t28) * t41) * t41 +
           (t16 + t364 + (t307 + t124 + t47) * t29 + (t230 + t299 + t296 + t101) * t41 +
            (t233 + t230 + t307 + t118 + t28) * t43) *
               t43 +
           (t16 + t364 + (t122 + t296 + t101) * t29 + (t344 + t299 + t124 + t47) * t41 +
            (t347 + t348 + t299 + t144 + t82) * t43 + (t389 + t347 + t344 + t122 + t118 + t28) * t60) *
               t60 +
           (t3 + (t358 + t132) * t5 + (t63 + t131 + t19) * t29 + (t223 + t275 + t131 + t19) * t41 +
            (t233 + t242 + t291 + t131 + t19) * t43 + (t389 + t336 + t325 + t111 + t131 + t19) * t60 +
            (t4 * t96 + t183 + t209 + t254 + t351 + t56 + t6) * t96) *
               t96) *
              t96) *
             t96 +
         (t413 + t424 + (t414 + t432 + (t415 + t436 + (t437 + t427 + t418) * t29) * t29) * t29 +
          (t444 + t452 + (t445 + t457 + (t458 + t454 + t448) * t29) * t29 +
           (t463 + t468 + (t469 + t471 + t466) * t29 + (t475 + t477 + t478 + t479) * t41) * t41) *
              t41 +
          (t414 + t432 + (t425 + t490 + (t491 + t487 + t428) * t29) * t29 +
           (t496 + t501 + (t502 + t504 + t499) * t29 + (t508 + t510 + t511 + t512) * t41) * t41 +
           (t415 + t436 + (t517 + t487 + t428) * t29 + (t521 + t523 + t524 + t525) * t41 +
            (t528 + t530 + t491 + t427 + t418) * t43) *
               t43) *
              t43 +
          (t444 + t452 + (t496 + t501 + (t537 + t524 + t525) * t29) * t29 +
           (t542 + t547 + (t549 + t551 + t552) * t29 + (t556 + t558 + t560 + t561) * t41) * t41 +
           (t445 + t457 + (t523 + t504 + t499) * t29 + (t569 + t571 + t551 + t552) * t41 +
            (t574 + t575 + t502 + t454 + t448) * t43) *
               t43 +
           (t463 + t468 + (t580 + t511 + t512) * t29 + (t584 + t585 + t560 + t561) * t41 +
            (t588 + t589 + t510 + t471 + t466) * t43 + (t592 + t593 + t556 + t594 + t478 + t479) * t60) *
               t60) *
              t60 +
          (t444 + t605 + (t445 + t607 + (t458 + t498 + t448) * t29) * t29 +
           (t542 + t614 + (t615 + t551 + t545) * t29 + (t556 + t618 + t619 + t561) * t41) * t41 +
           (t445 + t607 + (t624 + t504 + t455) * t29 + (t569 + t627 + t628 + t552) * t41 +
            (t574 + t575 + t624 + t498 + t448) * t43) *
               t43 +
           (t542 + t614 + (t549 + t628 + t552) * t29 + (t638 + t640 + t641 + t642) * t41 +
            (t41 * t639 + t545 + t551 + t627 + t645) * t43 + (t649 + t650 + t638 + t585 + t619 + t561) * t60) *
               t60 +
           (t463 + t657 + (t469 + t511 + t466) * t29 + (t584 + t618 + t660 + t561) * t41 +
            (t588 + t589 + t663 + t511 + t466) * t43 + (t583 * t60 + t558 + t561 + t638 + t650 + t660) * t60 +
            (t669 + t649 + t593 + t556 + t477 + t670 + t479) * t96) *
               t96) *
              t96 +
          (t677 + t685 + (t678 + t690 + (t691 + t687 + t681) * t29) * t29 +
           (t696 + t701 + (t702 + t704 + t699) * t29 + (t708 + t710 + t711 + t712) * t41) * t41 +
           (t678 + t690 + (t717 + t719 + t688) * t29 + (t723 + t725 + t726 + t727) * t41 +
            (t730 + t732 + t717 + t687 + t681) * t43) *
               t43 +
           (t696 + t701 + (t737 + t726 + t727) * t29 + (t741 + t743 + t745 + t746) * t41 +
            (t749 + t750 + t725 + t704 + t699) * t43 + (t753 + t754 + t741 + t755 + t711 + t712) * t60) *
               t60 +
           (t696 + t762 + (t702 + t726 + t699) * t29 + (t741 + t765 + t766 + t746) * t41 +
            (t749 + t750 + t769 + t726 + t699) * t43 + (t41 * t774 + t743 + t746 + t766 + t772 + t773) * t60 +
            (t778 + t772 + t754 + t741 + t710 + t779 + t712) * t96) *
               t96 +
           (t784 + t789 + (t790 + t792 + t787) * t29 + (t796 + t798 + t799 + t800) * t41 +
            (t803 + t805 + t806 + t792 + t787) * t43 + (t809 + t810 + t812 + t813 + t799 + t800) * t60 +
            (t60 * t811 + t798 + t800 + t810 + t812 + t816 + t818) * t96 +
            (t134 * t821 + t824 + t825 + t827 + t828 + t829 + t830 + t831) * t134) *
               t134) *
              t134) *
             t134 +
         (t413 + t424 + (t444 + t452 + (t463 + t468 + (t840 + t478 + t479) * t29) * t29) * t29 +
          (t414 + t432 + (t445 + t457 + (t477 + t471 + t466) * t29) * t29 +
           (t415 + t436 + (t469 + t454 + t448) * t29 + (t853 + t458 + t427 + t418) * t41) * t41) *
              t41 +
          (t444 + t452 + (t542 + t547 + (t860 + t560 + t561) * t29) * t29 +
           (t496 + t501 + (t558 + t551 + t552) * t29 + (t530 + t549 + t524 + t525) * t41) * t41 +
           (t463 + t468 + (t871 + t560 + t561) * t29 + (t521 + t585 + t511 + t512) * t41 +
            (t876 + t508 + t860 + t478 + t479) * t43) *
               t43) *
              t43 +
          (t414 + t432 + (t496 + t501 + (t594 + t511 + t512) * t29) * t29 +
           (t425 + t490 + (t510 + t504 + t499) * t29 + (t889 + t502 + t487 + t428) * t41) * t41 +
           (t445 + t457 + (t585 + t551 + t552) * t29 + (t896 + t571 + t504 + t499) * t41 +
            (t593 + t899 + t558 + t471 + t466) * t43) *
               t43 +
           (t415 + t436 + (t580 + t524 + t525) * t29 + (t906 + t523 + t487 + t428) * t41 +
            (t588 + t909 + t549 + t454 + t448) * t43 + (t912 + t574 + t889 + t537 + t427 + t418) * t60) *
               t60) *
              t60 +
          (t444 + t605 + (t542 + t614 + (t860 + t619 + t561) * t29) * t29 +
           (t445 + t607 + (t618 + t551 + t545) * t29 + (t925 + t615 + t498 + t448) * t41) * t41 +
           (t542 + t614 + (t930 + t641 + t642) * t29 + (t575 + t640 + t628 + t552) * t41 +
            (t935 + t569 + t930 + t619 + t561) * t43) *
               t43 +
           (t445 + t607 + (t585 + t628 + t552) * t29 + (t942 + t627 + t504 + t455) * t41 +
            (t650 + t945 + t640 + t551 + t545) * t43 + (t948 + t645 + t942 + t549 + t498 + t448) * t60) *
               t60 +
           (t463 + t657 + (t871 + t660 + t561) * t29 + (t955 + t618 + t511 + t466) * t41 +
            (t958 + t589 + t930 + t660 + t561) * t43 + (t961 + t650 + t962 + t558 + t511 + t466) * t60 +
            (t669 + t965 + t935 + t966 + t860 + t670 + t479) * t96) *
               t96) *
              t96 +
          (t973 + (t974 + (t5 * t975 + t977) * t5) * t5 + (t982 + t987 + (t989 + t991 + t992) * t29) * t29 +
           (t982 + t987 + (t998 + t1000 + t1001) * t29 + (t1004 + t998 + t991 + t992) * t41) * t41 +
           (t982 + t987 + (t1010 + t1012 + t1013) * t29 + (t1017 + t1019 + t1021 + t1022) * t41 +
            (t1025 + t1017 + t1010 + t991 + t992) * t43) *
               t43 +
           (t982 + t987 + (t1030 + t1021 + t1022) * t29 + (t1033 + t1019 + t1012 + t1013) * t41 +
            (t1036 + t1037 + t1019 + t1000 + t1001) * t43 + (t1040 + t1036 + t1033 + t1030 + t991 + t992) * t60) *
               t60 +
           (t1045 + (t1047 + t1048) * t5 + (t1052 + t1054 + t1055) * t29 + (t1058 + t1060 + t1054 + t1055) * t41 +
            (t1063 + t1065 + t1067 + t1054 + t1055) * t43 + (t1070 + t1071 + t1072 + t1073 + t1054 + t1055) * t60 +
            (t1076 * t96 + t1079 + t1080 + t1081 + t1082 + t1084 + t1085) * t96) *
               t96 +
           (t1090 + t1095 + (t1097 + t1099 + t1100) * t29 + (t1104 + t1106 + t1108 + t1109) * t41 +
            (t1112 + t1114 + t1116 + t1099 + t1100) * t43 + (t1119 + t1120 + t1122 + t1123 + t1108 + t1109) * t60 +
            (t1127 + t1129 + t1131 + t1132 + t1133 + t1135 + t1136) * t96 +
            (t1140 + t1142 + t1144 + t1146 + t1147 + t1148 + t1150 + t1151) * t134) *
               t134) *
              t134 +
          (t677 + t685 + (t696 + t701 + (t1158 + t711 + t712) * t29) * t29 +
           (t678 + t690 + (t710 + t704 + t699) * t29 + (t1165 + t702 + t687 + t681) * t41) * t41 +
           (t696 + t701 + (t1170 + t745 + t746) * t29 + (t732 + t743 + t726 + t727) * t41 +
            (t1175 + t723 + t1170 + t711 + t712) * t43) *
               t43 +
           (t678 + t690 + (t755 + t726 + t727) * t29 + (t1182 + t725 + t719 + t688) * t41 +
            (t754 + t1185 + t743 + t704 + t699) * t43 + (t1188 + t749 + t1182 + t737 + t687 + t681) * t60) *
               t60 +
           (t696 + t762 + (t1170 + t766 + t746) * t29 + (t1195 + t765 + t726 + t699) * t41 +
            (t29 * t774 + t1198 + t746 + t750 + t766) * t43 + (t1202 + t773 + t1203 + t743 + t726 + t699) * t60 +
            (t778 + t1206 + t1198 + t1207 + t1170 + t779 + t712) * t96) *
               t96 +
           (t1090 + t1095 + (t1212 + t1108 + t1109) * t29 + (t1215 + t1106 + t1099 + t1100) * t41 +
            (t1218 + t1114 + t1219 + t1108 + t1109) * t43 + (t1222 + t1120 + t1223 + t1123 + t1099 + t1100) * t60 +
            (t1127 + t1226 + t1227 + t1228 + t1229 + t1135 + t1136) * t96 +
            (t1234 * t96 + t1241 * t5 + t1233 + t1237 + t1238 + t1239 + t1240 + t1243) * t134) *
               t134 +
           (t784 + t789 + (t1248 + t799 + t800) * t29 + (t1251 + t798 + t792 + t787) * t41 +
            (t1254 + t805 + t1255 + t799 + t800) * t43 + (t1258 + t810 + t1259 + t813 + t792 + t787) * t60 +
            (t816 + t1262 + t1263 + t1264 + t1255 + t818 + t800) * t96 +
            (t1233 + t1142 + t1267 + t1268 + t1269 + t1270 + t1150 + t1151) * t134 +
            (t185 * t821 + t1140 + t1274 + t1275 + t1276 + t1277 + t824 + t830 + t831) * t185) *
               t185) *
              t185) *
             t185 +
         (t413 + t1292 + (t414 + t1296 + (t415 + t1298 + (t437 + t447 + t418) * t29) * t29) * t29 +
          (t414 + t1296 + (t425 + t1306 + (t491 + t454 + t428) * t29) * t29 +
           (t415 + t1298 + (t517 + t454 + t428) * t29 + (t853 + t491 + t447 + t418) * t41) * t41) *
              t41 +
          (t444 + t1323 + (t445 + t1325 + (t458 + t544 + t448) * t29) * t29 +
           (t496 + t1331 + (t502 + t551 + t499) * t29 + (t530 + t523 + t612 + t525) * t41) * t41 +
           (t463 + t1340 + (t469 + t560 + t466) * t29 + (t521 + t510 + t660 + t512) * t41 +
            (t876 + t508 + t477 + t1319 + t479) * t43) *
               t43) *
              t43 +
          (t444 + t1323 + (t496 + t1331 + (t537 + t612 + t525) * t29) * t29 +
           (t445 + t1325 + (t523 + t551 + t499) * t29 + (t925 + t502 + t544 + t448) * t41) * t41 +
           (t542 + t1363 + (t549 + t641 + t552) * t29 + (t575 + t571 + t641 + t552) * t41 +
            (t935 + t569 + t558 + t1361 + t561) * t43) *
               t43 +
           (t463 + t1340 + (t580 + t660 + t512) * t29 + (t955 + t510 + t560 + t466) * t41 +
            (t958 + t589 + t585 + t1361 + t561) * t43 + (t592 + t935 + t966 + t594 + t1319 + t479) * t60) *
               t60) *
              t60 +
          (t414 + t1387 + (t425 + t1389 + (t491 + t498 + t428) * t29) * t29 +
           (t425 + t1389 + (t1394 + t504 + t488) * t29 + (t889 + t1394 + t498 + t428) * t41) * t41 +
           (t445 + t1402 + (t624 + t551 + t455) * t29 + (t896 + t1405 + t628 + t499) * t41 +
            (t593 + t899 + t663 + t619 + t466) * t43) *
               t43 +
           (t445 + t1402 + (t523 + t628 + t499) * t29 + (t942 + t1405 + t551 + t455) * t41 +
            (t650 + t945 + t627 + t641 + t545) * t43 + (t965 + t650 + t962 + t510 + t619 + t466) * t60) *
               t60 +
           (t415 + t1423 + (t517 + t524 + t428) * t29 + (t906 + t1394 + t524 + t428) * t41 +
            (t588 + t909 + t624 + t612 + t448) * t43 + (t961 + t645 + t942 + t502 + t612 + t448) * t60 +
            (t1432 + t948 + t574 + t889 + t491 + t601 + t418) * t96) *
               t96) *
              t96 +
          (t973 + t1443 + (t974 + t1445 + (t29 * t975 + t977 + t984) * t29) * t29 +
           (t982 + t1453 + (t1454 + t1000 + t985) * t29 + (t1004 + t1457 + t1451 + t992) * t41) * t41 +
           (t982 + t1464 + (t1454 + t1012 + t985) * t29 + (t1017 + t1467 + t1468 + t1022) * t41 +
            (t1025 + t1017 + t1457 + t1462 + t992) * t43) *
               t43 +
           (t1045 + t1477 + (t1478 + t1054 + t1048) * t29 + (t1058 + t1481 + t1482 + t1055) * t41 +
            (t1063 + t1065 + t1481 + t1485 + t1055) * t43 +
            (t1076 * t60 + t1080 + t1081 + t1085 + t1489 + t1490) * t60) *
               t60 +
           (t982 + t1497 + (t1454 + t1021 + t985) * t29 + (t1033 + t1500 + t1468 + t1013) * t41 +
            (t1036 + t1037 + t1503 + t1468 + t1001) * t43 + (t1079 + t1071 + t1072 + t1481 + t1506 + t1055) * t60 +
            (t1509 + t1070 + t1036 + t1033 + t1457 + t1495 + t992) * t96) *
               t96 +
           (t1090 + t1516 + (t1517 + t1099 + t1093) * t29 + (t1104 + t1520 + t1521 + t1109) * t41 +
            (t1112 + t1114 + t1524 + t1525 + t1100) * t43 + (t1528 + t1131 + t1132 + t1529 + t1530 + t1136) * t60 +
            (t1533 + t1129 + t1120 + t1122 + t1520 + t1534 + t1109) * t96 +
            (t1140 + t1537 + t1538 + t1146 + t1147 + t1539 + t1540 + t1151) * t134) *
               t134) *
              t134 +
          (t973 + t1443 + (t982 + t1453 + (t989 + t1451 + t992) * t29) * t29 +
           (t974 + t1445 + (t1457 + t1000 + t985) * t29 + (t41 * t975 + t1454 + t977 + t984) * t41) * t41 +
           (t1045 + t1477 + (t1052 + t1482 + t1055) * t29 + (t1560 + t1481 + t1054 + t1048) * t41 +
            (t1076 * t43 + t1082 + t1085 + t1490 + t1564) * t43) *
               t43 +
           (t982 + t1464 + t1570 + (t1571 + t1467 + t1012 + t985) * t41 +
            (t1080 + t1574 + t1073 + t1485 + t1055) * t43 + (t1040 + t1063 + t1577 + t1030 + t1462 + t992) * t60) *
               t60 +
           (t982 + t1497 + (t1010 + t1468 + t1013) * t29 + (t1571 + t1500 + t1021 + t985) * t41 +
            (t1080 + t1574 + t1067 + t1506 + t1055) * t43 + (t1588 + t1071 + t1589 + t1019 + t1468 + t1001) * t60 +
            (t1509 + t1588 + t1063 + t1577 + t1010 + t1495 + t992) * t96) *
               t96 +
           (t1596 + t1601 + (t1602 + t1604 + t1599) * t29 + (t1607 + t1608 + t1604 + t1599) * t41 +
            (t1612 + t1614 + t1616 + t1617 + t1618) * t43 + (t1621 + t1623 + t1624 + t1625 + t1617 + t1618) * t60 +
            (t1622 * t60 + t1616 + t1618 + t1623 + t1624 + t1628 + t1630) * t96 +
            (t1634 + t1636 + t1637 + t1639 + t1641 + t1643 + t1644 + t1645) * t134) *
               t134 +
           (t1090 + t1516 + (t1212 + t1521 + t1109) * t29 + (t1652 + t1520 + t1099 + t1093) * t41 +
            (t1655 + t1656 + t1229 + t1530 + t1136) * t43 + (t1222 + t1131 + t1659 + t1123 + t1525 + t1100) * t60 +
            (t1533 + t1662 + t1227 + t1663 + t1219 + t1534 + t1109) * t96 +
            (t1667 + t1636 + t1668 + t1669 + t1670 + t1671 + t1644 + t1645) * t134 +
            (t1674 + t1634 + t1537 + t1267 + t1675 + t1676 + t1270 + t1540 + t1151) * t185) *
               t185) *
              t185 +
          (t677 + t1687 + (t678 + t1689 + (t691 + t698 + t681) * t29) * t29 +
           (t678 + t1689 + (t717 + t704 + t688) * t29 + (t1165 + t717 + t698 + t681) * t41) * t41 +
           (t696 + t1702 + (t702 + t745 + t699) * t29 + (t732 + t725 + t766 + t727) * t41 +
            (t1175 + t723 + t710 + t1700 + t712) * t43) *
               t43 +
           (t696 + t1702 + (t737 + t766 + t727) * t29 + (t1195 + t725 + t745 + t699) * t41 +
            (t1198 + t750 + t743 + t1715 + t746) * t43 + (t753 + t1198 + t1207 + t755 + t1700 + t712) * t60) *
               t60 +
           (t678 + t1723 + (t717 + t726 + t688) * t29 + (t29 * t718 + t1182 + t688 + t726) * t41 +
            (t754 + t1185 + t769 + t766 + t699) * t43 + (t1206 + t773 + t1203 + t725 + t766 + t699) * t60 +
            (t1733 + t1202 + t749 + t1182 + t717 + t760 + t681) * t96) *
               t96 +
           (t1090 + t1740 + (t1517 + t1108 + t1093) * t29 + (t1215 + t1524 + t1521 + t1100) * t41 +
            (t1218 + t1114 + t1520 + t1745 + t1109) * t43 + (t1528 + t1227 + t1228 + t1529 + t1748 + t1136) * t60 +
            (t1751 + t1226 + t1120 + t1223 + t1524 + t1534 + t1100) * t96 +
            (t1234 * t60 + t1241 * t29 + t1233 + t1238 + t1239 + t1243 + t1754 + t1757) * t134) *
               t134 +
           (t1090 + t1740 + (t1097 + t1521 + t1100) * t29 + (t1652 + t1524 + t1108 + t1093) * t41 +
            (t1655 + t1656 + t1133 + t1748 + t1136) * t43 + (t1119 + t1227 + t1663 + t1123 + t1745 + t1109) * t60 +
            (t1751 + t1662 + t1131 + t1659 + t1116 + t1534 + t1100) * t96 +
            (t1667 + t1772 + t1637 + t1669 + t1670 + t1643 + t1773 + t1645) * t134 +
            (t1234 * t43 + t1241 * t41 + t1237 + t1240 + t1243 + t1667 + t1754 + t1757 + t1776) * t185) *
               t185 +
           (t784 + t1785 + (t790 + t799 + t787) * t29 + (t1251 + t806 + t799 + t787) * t41 +
            (t1254 + t805 + t798 + t1790 + t800) * t43 + (t809 + t1263 + t1264 + t813 + t1790 + t800) * t60 +
            (t1795 + t1262 + t810 + t1259 + t806 + t818 + t787) * t96 +
            (t1233 + t1798 + t1538 + t1268 + t1269 + t1539 + t1799 + t1151) * t134 +
            (t1776 + t1634 + t1798 + t1144 + t1675 + t1676 + t1148 + t1799 + t1151) * t185 +
            (t211 * t821 + t1140 + t1275 + t1276 + t1674 + t1805 + t1806 + t825 + t829 + t831) * t211) *
               t211) *
              t211) *
             t211 +
         t2357 * t212);

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
    std::cerr << "Output polynomial energy: " << e << std::endl;
#endif

    return e;
}

}  // namespace x1b_A1B4_deg5_exp0
