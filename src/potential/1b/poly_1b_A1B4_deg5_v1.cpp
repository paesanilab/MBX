#include "poly_1b_A1B4_deg5_v1x.h"

namespace x1b_A1B4_deg5 {

double poly_1b_A1B4_deg5_v1x::eval(const double a[207], const double x[10])
{
    const double t1 = a[0];
    const double t2 = a[2];
    const double t3 = a[9];
    const double t4 = a[82];
    const double t6 = a[29];
    const double t15 = a[3];
    const double t16 = a[10];
    const double t17 = a[84];
    const double t5 = x[9];
    const double t18 = t17*t5;
    const double t19 = a[31];
    const double t25 = (t15+(t16+(t18+t19)*t5)*t5)*t5;
    const double t26 = a[83];
    const double t27 = t26*t5;
    const double t28 = a[30];
    const double t32 = (t16+(t27+t28)*t5)*t5;
    const double t34 = (t27+t19)*t5;
    const double t44 = a[11];
    const double t45 = a[86];
    const double t46 = t45*t5;
    const double t47 = a[32];
    const double t51 = (t44+(t46+t47)*t5)*t5;
    const double t52 = a[85];
    const double t53 = t52*t5;
    const double t55 = (t53+t47)*t5;
    const double t29 = x[8];
    const double t56 = t17*t29;
    const double t62 = (t15+t51+(t16+t55+(t56+t46+t19)*t29)*t29)*t29;
    const double t63 = t26*t29;
    const double t67 = (t16+t55+(t63+t53+t28)*t29)*t29;
    const double t69 = (t63+t46+t19)*t29;
    const double t80 = a[87]*t5;
    const double t84 = t45*t29;
    const double t89 = t52*t29;
    const double t91 = (t89+t80+t47)*t29;
    const double t43 = x[7];
    const double t92 = t17*t43;
    const double t99 = t26*t43;
    const double t115 = a[1];
    const double t116 = a[6];
    const double t117 = a[17];
    const double t118 = a[113];
    const double t120 = a[46];
    const double t126 = (t116+(t117+(t118*t5+t120)*t5)*t5)*t5;
    const double t127 = a[19];
    const double t128 = a[120];
    const double t129 = t128*t5;
    const double t130 = a[50];
    const double t134 = (t127+(t129+t130)*t5)*t5;
    const double t135 = a[117];
    const double t138 = (t135*t5+t130)*t5;
    const double t139 = t118*t29;
    const double t146 = a[5];
    const double t147 = a[18];
    const double t148 = a[114];
    const double t149 = t148*t5;
    const double t150 = a[47];
    const double t154 = (t147+(t149+t150)*t5)*t5;
    const double t155 = a[121];
    const double t156 = t155*t5;
    const double t157 = a[51];
    const double t159 = (t156+t157)*t5;
    const double t160 = t148*t29;
    const double t164 = (t147+t159+(t160+t156+t150)*t29)*t29;
    const double t165 = a[15];
    const double t166 = a[115];
    const double t167 = t166*t5;
    const double t168 = a[48];
    const double t170 = (t167+t168)*t5;
    const double t171 = t166*t29;
    const double t172 = a[122];
    const double t173 = t172*t5;
    const double t175 = (t171+t173+t168)*t29;
    const double t176 = a[110];
    const double t177 = t176*t43;
    const double t178 = a[118];
    const double t179 = t178*t29;
    const double t180 = t178*t5;
    const double t181 = a[44];
    const double t188 = a[16];
    const double t189 = a[116];
    const double t190 = t189*t5;
    const double t191 = a[49];
    const double t193 = (t190+t191)*t5;
    const double t194 = t189*t29;
    const double t196 = a[123]*t5;
    const double t199 = a[112];
    const double t200 = t199*t43;
    const double t201 = a[119];
    const double t202 = t201*t29;
    const double t203 = t201*t5;
    const double t204 = a[45];
    const double t209 = a[111];
    const double t79 = x[6];
    const double t213 = t176*t79;
    const double t220 = a[4];
    const double t221 = a[14];
    const double t222 = a[98];
    const double t224 = a[39];
    const double t228 = (t221+(t222*t5+t224)*t5)*t5;
    const double t229 = a[102];
    const double t230 = t229*t5;
    const double t231 = a[41];
    const double t233 = (t230+t231)*t5;
    const double t234 = t222*t29;
    const double t239 = a[13];
    const double t240 = a[99];
    const double t241 = t240*t5;
    const double t242 = a[40];
    const double t244 = (t241+t242)*t5;
    const double t245 = t240*t29;
    const double t246 = a[103];
    const double t247 = t246*t5;
    const double t249 = (t245+t247+t242)*t29;
    const double t250 = a[96];
    const double t251 = t250*t43;
    const double t252 = a[100];
    const double t253 = t252*t29;
    const double t254 = t252*t5;
    const double t255 = a[37];
    const double t260 = a[97];
    const double t261 = t260*t43;
    const double t262 = a[101];
    const double t263 = t262*t29;
    const double t264 = t262*t5;
    const double t265 = a[38];
    const double t268 = t250*t79;
    const double t273 = a[12];
    const double t274 = a[93];
    const double t276 = a[36];
    const double t278 = (t274*t5+t276)*t5;
    const double t279 = t274*t29;
    const double t280 = a[95];
    const double t281 = t280*t5;
    const double t284 = a[91];
    const double t285 = t284*t43;
    const double t286 = a[94];
    const double t287 = t286*t29;
    const double t288 = t286*t5;
    const double t289 = a[35];
    const double t292 = t284*t79;
    const double t293 = a[92];
    const double t297 = a[88];
    const double t299 = a[89];
    const double t300 = t299*t79;
    const double t301 = t299*t43;
    const double t302 = a[90];
    const double t303 = t302*t29;
    const double t304 = t302*t5;
    const double t305 = a[34];
    const double t314 = t176*t29;
    const double t320 = (t146+t154+(t165+t170+(t314+t180+t181)*t29)*t29)*t29;
    const double t324 = (t147+t159+(t179+t173+t168)*t29)*t29;
    const double t326 = (t171+t156+t150)*t29;
    const double t327 = t118*t43;
    const double t334 = t199*t29;
    const double t338 = (t188+t193+(t334+t203+t204)*t29)*t29;
    const double t340 = (t202+t196+t191)*t29;
    const double t341 = t148*t43;
    const double t348 = (t209*t29+t203+t204)*t29;
    const double t349 = t166*t43;
    const double t352 = t178*t43;
    const double t359 = a[8];
    const double t360 = a[25];
    const double t361 = a[164];
    const double t363 = a[68];
    const double t367 = (t360+(t361*t5+t363)*t5)*t5;
    const double t368 = a[24];
    const double t369 = a[166];
    const double t370 = t369*t5;
    const double t371 = a[70];
    const double t373 = (t370+t371)*t5;
    const double t374 = a[159];
    const double t375 = t374*t29;
    const double t376 = a[168];
    const double t377 = t376*t5;
    const double t378 = a[65];
    const double t382 = (t368+t373+(t375+t377+t378)*t29)*t29;
    const double t383 = a[162];
    const double t384 = t383*t29;
    const double t385 = a[170];
    const double t386 = t385*t5;
    const double t387 = a[67];
    const double t389 = (t384+t386+t387)*t29;
    const double t390 = t374*t43;
    const double t395 = a[23];
    const double t396 = a[165];
    const double t397 = t396*t5;
    const double t398 = a[69];
    const double t400 = (t397+t398)*t5;
    const double t401 = a[160];
    const double t402 = t401*t29;
    const double t403 = a[169];
    const double t404 = t403*t5;
    const double t405 = a[66];
    const double t407 = (t402+t404+t405)*t29;
    const double t408 = t401*t43;
    const double t409 = a[163];
    const double t410 = t409*t29;
    const double t413 = a[158];
    const double t414 = t413*t79;
    const double t415 = a[161];
    const double t416 = t415*t43;
    const double t417 = t415*t29;
    const double t418 = a[167];
    const double t419 = t418*t5;
    const double t420 = a[64];
    const double t425 = a[22];
    const double t426 = a[148];
    const double t428 = a[62];
    const double t430 = (t426*t5+t428)*t5;
    const double t431 = a[143];
    const double t432 = t431*t29;
    const double t433 = a[150];
    const double t434 = t433*t5;
    const double t435 = a[60];
    const double t437 = (t432+t434+t435)*t29;
    const double t438 = a[145];
    const double t439 = t438*t43;
    const double t440 = a[147];
    const double t441 = t440*t29;
    const double t442 = a[151];
    const double t443 = t442*t5;
    const double t444 = a[61];
    const double t447 = a[142];
    const double t448 = t447*t79;
    const double t449 = a[146];
    const double t450 = t449*t43;
    const double t451 = a[144];
    const double t452 = t451*t29;
    const double t453 = a[149];
    const double t454 = t453*t5;
    const double t455 = a[59];
    const double t458 = a[136];
    const double t136 = x[5];
    const double t459 = t458*t136;
    const double t460 = a[137];
    const double t461 = t460*t79;
    const double t462 = a[139];
    const double t463 = t462*t43;
    const double t464 = a[138];
    const double t465 = t464*t29;
    const double t466 = a[140];
    const double t467 = t466*t5;
    const double t468 = a[58];
    const double t475 = t250*t29;
    const double t479 = (t239+t244+(t475+t254+t255)*t29)*t29;
    const double t481 = (t253+t247+t242)*t29;
    const double t482 = t222*t43;
    const double t487 = t260*t29;
    const double t489 = (t487+t264+t265)*t29;
    const double t490 = t240*t43;
    const double t493 = t252*t43;
    const double t498 = t438*t29;
    const double t500 = (t498+t443+t444)*t29;
    const double t501 = t431*t43;
    const double t504 = t451*t43;
    const double t505 = t449*t29;
    const double t508 = a[106];
    const double t509 = t508*t136;
    const double t510 = a[107];
    const double t511 = t510*t79;
    const double t512 = a[108];
    const double t513 = t512*t43;
    const double t514 = t512*t29;
    const double t515 = a[109];
    const double t516 = t515*t5;
    const double t517 = a[43];
    const double t522 = t284*t29;
    const double t524 = (t522+t288+t289)*t29;
    const double t525 = t274*t43;
    const double t528 = t286*t43;
    const double t529 = t293*t29;
    const double t532 = t464*t43;
    const double t533 = t462*t29;
    const double t537 = t302*t43;
    const double t538 = t299*t29;
    const double t553 = (t146+(t165+(t176*t5+t181)*t5)*t5)*t5;
    const double t557 = (t147+(t180+t168)*t5)*t5;
    const double t559 = (t167+t150)*t5;
    const double t565 = (t116+t557+(t117+t559+(t139+t149+t120)*t29)*t29)*t29;
    const double t567 = (t173+t157)*t5;
    const double t568 = t128*t29;
    const double t572 = (t127+t567+(t568+t156+t130)*t29)*t29;
    const double t575 = (t135*t29+t130+t156)*t29;
    const double t582 = t199*t5;
    const double t586 = (t188+(t582+t204)*t5)*t5;
    const double t588 = (t203+t191)*t5;
    const double t592 = (t147+t588+(t160+t190+t150)*t29)*t29;
    const double t593 = t155*t29;
    const double t595 = (t593+t196+t157)*t29;
    const double t602 = (t209*t5+t204)*t5;
    const double t604 = (t171+t203+t168)*t29;
    const double t605 = t172*t29;
    const double t618 = (t368+(t374*t5+t378)*t5)*t5;
    const double t620 = (t377+t371)*t5;
    const double t621 = t361*t29;
    const double t625 = (t360+t620+(t621+t370+t363)*t29)*t29;
    const double t626 = t383*t5;
    const double t628 = (t626+t387)*t5;
    const double t629 = t369*t29;
    const double t631 = (t629+t386+t371)*t29;
    const double t632 = t376*t29;
    const double t637 = t401*t5;
    const double t639 = (t637+t405)*t5;
    const double t640 = t396*t29;
    const double t642 = (t640+t404+t398)*t29;
    const double t643 = t403*t29;
    const double t644 = t409*t5;
    const double t647 = t418*t29;
    const double t648 = t415*t5;
    const double t655 = (t431*t5+t435)*t5;
    const double t656 = t426*t29;
    const double t658 = (t656+t434+t428)*t29;
    const double t659 = t442*t29;
    const double t660 = t440*t5;
    const double t663 = t453*t29;
    const double t664 = t451*t5;
    const double t667 = t466*t29;
    const double t668 = t464*t5;
    const double t678 = (t368+t628+(t375+t626+t378)*t29)*t29;
    const double t680 = (t632+t386+t371)*t29;
    const double t681 = t361*t43;
    const double t687 = (t402+t644+t405)*t29;
    const double t688 = t396*t43;
    const double t691 = t418*t43;
    const double t696 = a[27];
    const double t697 = a[189];
    const double t699 = a[77];
    const double t701 = (t697*t5+t699)*t5;
    const double t702 = t697*t29;
    const double t703 = a[191];
    const double t704 = t703*t5;
    const double t706 = (t702+t704+t699)*t29;
    const double t707 = t697*t43;
    const double t708 = t703*t29;
    const double t711 = a[188];
    const double t713 = a[190];
    const double t714 = t713*t43;
    const double t715 = t713*t29;
    const double t716 = t713*t5;
    const double t717 = a[76];
    const double t720 = a[184];
    const double t721 = t720*t136;
    const double t722 = a[185];
    const double t723 = t722*t79;
    const double t724 = a[187];
    const double t725 = t724*t43;
    const double t726 = a[186];
    const double t727 = t726*t29;
    const double t728 = t726*t5;
    const double t729 = a[75];
    const double t735 = (t498+t660+t444)*t29;
    const double t736 = t426*t43;
    const double t739 = t453*t43;
    const double t742 = a[156];
    const double t743 = t742*t136;
    const double t744 = t726*t43;
    const double t745 = t724*t29;
    const double t283 = x[4];
    const double t748 = t458*t283;
    const double t749 = t466*t43;
    const double t760 = (t239+(t250*t5+t255)*t5)*t5;
    const double t762 = (t254+t242)*t5;
    const double t766 = (t221+t762+(t234+t241+t224)*t29)*t29;
    const double t767 = t229*t29;
    const double t769 = (t767+t247+t231)*t29;
    const double t774 = t260*t5;
    const double t776 = (t774+t265)*t5;
    const double t778 = (t245+t264+t242)*t29;
    const double t779 = t246*t29;
    const double t788 = (t438*t5+t444)*t5;
    const double t790 = (t656+t443+t428)*t29;
    const double t791 = t433*t29;
    const double t794 = t449*t5;
    const double t797 = t515*t29;
    const double t798 = t512*t5;
    const double t804 = (t432+t660+t435)*t29;
    const double t809 = t724*t5;
    const double t812 = t508*t283;
    const double t813 = t515*t43;
    const double t820 = (t284*t5+t289)*t5;
    const double t822 = (t279+t288+t276)*t29;
    const double t823 = t280*t29;
    const double t826 = t293*t5;
    const double t829 = t462*t5;
    const double t835 = t299*t5;
    const double t856 = t118*t79;
    const double t863 = t413*t43;
    const double t870 = t374*t79;
    const double t875 = t447*t43;
    const double t878 = t438*t79;
    const double t881 = t462*t79;
    const double t882 = t460*t43;
    const double t889 = t413*t29;
    const double t895 = (t417+t404+t405)*t29;
    const double t900 = t383*t43;
    const double t907 = a[28];
    const double t908 = a[200];
    const double t910 = a[80];
    const double t913 = a[198];
    const double t914 = t913*t29;
    const double t915 = a[201];
    const double t916 = t915*t5;
    const double t917 = a[79];
    const double t920 = t913*t43;
    const double t921 = a[199];
    const double t922 = t921*t29;
    const double t925 = t913*t79;
    const double t926 = t921*t43;
    const double t929 = a[192];
    const double t930 = t929*t136;
    const double t931 = a[194];
    const double t932 = t931*t79;
    const double t933 = t931*t43;
    const double t934 = a[193];
    const double t935 = t934*t29;
    const double t936 = a[195];
    const double t937 = t936*t5;
    const double t938 = a[78];
    const double t943 = t447*t29;
    const double t945 = (t943+t454+t455)*t29;
    const double t948 = t440*t43;
    const double t951 = a[157];
    const double t952 = t951*t136;
    const double t953 = t934*t43;
    const double t954 = t931*t29;
    const double t957 = t460*t29;
    const double t964 = a[7];
    const double t965 = a[21];
    const double t966 = a[132];
    const double t968 = a[55];
    const double t972 = (t965+(t5*t966+t968)*t5)*t5;
    const double t973 = a[133];
    const double t974 = t973*t5;
    const double t975 = a[56];
    const double t977 = (t974+t975)*t5;
    const double t978 = t966*t29;
    const double t982 = (t965+t977+(t978+t974+t968)*t29)*t29;
    const double t983 = a[134];
    const double t984 = t983*t29;
    const double t985 = a[135];
    const double t986 = t985*t5;
    const double t987 = a[57];
    const double t989 = (t984+t986+t987)*t29;
    const double t990 = t966*t43;
    const double t995 = t983*t5;
    const double t997 = (t995+t987)*t5;
    const double t998 = t973*t29;
    const double t1000 = (t998+t986+t975)*t29;
    const double t1001 = t973*t43;
    const double t1002 = t985*t29;
    const double t1004 = (t1001+t1002+t986+t975)*t43;
    const double t1005 = t966*t79;
    const double t1010 = a[26];
    const double t1011 = a[178];
    const double t1013 = a[73];
    const double t1015 = (t1011*t5+t1013)*t5;
    const double t1016 = t1011*t29;
    const double t1017 = a[181];
    const double t1018 = t1017*t5;
    const double t1020 = (t1016+t1018+t1013)*t29;
    const double t1021 = a[176];
    const double t1022 = t1021*t43;
    const double t1023 = a[180];
    const double t1024 = t1023*t29;
    const double t1025 = a[179];
    const double t1026 = t1025*t5;
    const double t1027 = a[72];
    const double t1030 = t1021*t79;
    const double t1031 = a[177];
    const double t1032 = t1031*t43;
    const double t1033 = t1025*t29;
    const double t1034 = t1023*t5;
    const double t1037 = a[141];
    const double t1038 = t1037*t136;
    const double t1039 = a[153];
    const double t1040 = t1039*t79;
    const double t1041 = t1039*t43;
    const double t1042 = a[154];
    const double t1043 = t1042*t29;
    const double t1044 = t1042*t5;
    const double t1045 = a[63];
    const double t1047 = (t1038+t1040+t1041+t1043+t1044+t1045)*t136;
    const double t1050 = t1021*t29;
    const double t1052 = (t1050+t1026+t1027)*t29;
    const double t1053 = t1011*t43;
    const double t1056 = t1025*t43;
    const double t1057 = t1031*t29;
    const double t1060 = a[196];
    const double t1061 = t1060*t136;
    const double t1062 = a[203];
    const double t1063 = t1062*t79;
    const double t1064 = a[204];
    const double t1065 = t1064*t43;
    const double t1066 = t1064*t29;
    const double t1067 = a[205];
    const double t1068 = t1067*t5;
    const double t1069 = a[81];
    const double t1072 = t1037*t283;
    const double t1073 = t1042*t43;
    const double t1074 = t1039*t29;
    const double t1079 = a[20];
    const double t1080 = a[129];
    const double t1082 = a[54];
    const double t1084 = (t1080*t5+t1082)*t5;
    const double t1085 = a[127];
    const double t1086 = t1085*t29;
    const double t1087 = a[130];
    const double t1088 = t1087*t5;
    const double t1089 = a[53];
    const double t1091 = (t1086+t1088+t1089)*t29;
    const double t1092 = t1085*t43;
    const double t1093 = a[128];
    const double t1094 = t1093*t29;
    const double t1097 = t1080*t79;
    const double t1098 = t1087*t43;
    const double t1099 = t1087*t29;
    const double t1100 = a[131];
    const double t1101 = t1100*t5;
    const double t1104 = a[152];
    const double t1105 = t1104*t136;
    const double t1106 = a[173];
    const double t1107 = t1106*t79;
    const double t1108 = a[172];
    const double t1109 = t1108*t43;
    const double t1110 = a[174];
    const double t1111 = t1110*t29;
    const double t1112 = a[175];
    const double t1113 = t1112*t5;
    const double t1114 = a[71];
    const double t1117 = t1104*t283;
    const double t1118 = a[202];
    const double t1119 = t1118*t136;
    const double t1120 = t1110*t43;
    const double t1121 = t1108*t29;
    const double t1124 = a[124];
    const double t351 = x[3];
    const double t1125 = t1124*t351;
    const double t1126 = a[171];
    const double t1127 = t1126*t283;
    const double t1128 = t1126*t136;
    const double t1129 = a[126];
    const double t1130 = t1129*t79;
    const double t1131 = a[125];
    const double t1132 = t1131*t43;
    const double t1133 = t1131*t29;
    const double t1134 = t1129*t5;
    const double t1135 = a[52];
    const double t1148 = t222*t79;
    const double t1155 = t431*t79;
    const double t1158 = t512*t79;
    const double t1159 = t510*t43;
    const double t1168 = t934*t79;
    const double t1171 = t510*t29;
    const double t1178 = (t1085*t5+t1089)*t5;
    const double t1179 = t1080*t29;
    const double t1181 = (t1179+t1088+t1082)*t29;
    const double t1182 = t1080*t43;
    const double t1183 = t1100*t29;
    const double t1186 = t1085*t79;
    const double t1187 = t1093*t5;
    const double t1190 = t1108*t79;
    const double t1191 = t1106*t43;
    const double t1192 = t1112*t29;
    const double t1193 = t1110*t5;
    const double t1196 = a[197];
    const double t1197 = t1196*t136;
    const double t1198 = t1112*t43;
    const double t1199 = t1106*t29;
    const double t1202 = a[104];
    const double t1203 = t1202*t351;
    const double t1204 = a[155];
    const double t1205 = t1204*t283;
    const double t1206 = t1204*t136;
    const double t1207 = a[105];
    const double t1208 = t1207*t79;
    const double t1209 = t1207*t43;
    const double t1210 = t1207*t29;
    const double t1211 = t1207*t5;
    const double t1212 = a[42];
    const double t1219 = t274*t79;
    const double t1222 = t464*t79;
    const double t1227 = t1131*t79;
    const double t1228 = t1129*t43;
    const double t1229 = t1129*t29;
    const double t1230 = t1131*t5;
    const double t1234 = t302*t79;
    const double t1301 = t1124*t283;
    const double t1312 = (t395+(t413*t5+t420)*t5)*t5;
    const double t1314 = (t419+t398)*t5;
    const double t1320 = (t648+t405)*t5;
    const double t1322 = (t629+t404+t371)*t29;
    const double t1327 = t385*t29;
    const double t1336 = (t5*t913+t917)*t5;
    const double t1340 = t915*t29;
    const double t1341 = t921*t5;
    const double t1346 = t936*t29;
    const double t1347 = t934*t5;
    const double t1354 = (t1021*t5+t1027)*t5;
    const double t1356 = (t1016+t1026+t1013)*t29;
    const double t1357 = t1017*t29;
    const double t1360 = t1031*t5;
    const double t1363 = t1067*t29;
    const double t1364 = t1064*t5;
    const double t1367 = t1108*t5;
    const double t1374 = (t447*t5+t455)*t5;
    const double t1376 = (t656+t454+t428)*t29;
    const double t1381 = t931*t5;
    const double t1384 = t1039*t5;
    const double t1387 = t458*t351;
    const double t1388 = t460*t5;
    const double t1401 = t361*t79;
    const double t1409 = t697*t79;
    const double t1412 = t724*t79;
    const double t1413 = t722*t43;
    const double t1420 = t1011*t79;
    const double t1423 = t1064*t79;
    const double t1424 = t1062*t43;
    const double t1427 = t1112*t79;
    const double t1438 = a[182];
    const double t1439 = t1438*t283;
    const double t1440 = a[206];
    const double t1441 = t1440*t136;
    const double t1442 = a[183];
    const double t1443 = t1442*t79;
    const double t1444 = t1442*t43;
    const double t1445 = t1442*t29;
    const double t1446 = t1442*t5;
    const double t1447 = a[74];
    const double t1450 = t1126*t351;
    const double t1451 = t1106*t5;
    const double t1458 = t426*t79;
    const double t1461 = t726*t79;
    const double t1464 = t1042*t79;
    const double t1467 = t1204*t351;
    const double t1468 = t1110*t79;
    const double t394 = x[2];
    const double t1471 = t458*t394;
    const double t1472 = t466*t79;
    const double t1503 = t1202*t283;
    const double t1516 = t508*t351;
    const double t1517 = t510*t5;
    const double t1530 = t1104*t351;
    const double t1533 = t508*t394;
    const double t1534 = t515*t79;
    const double t1549 = t1037*t351;
    const double t1570 = (t147+t588+(t179+t203+t168)*t29)*t29;
    const double t1572 = (t171+t190+t150)*t29;
    const double t1581 = t128*t43;
    const double t1603 = t983*t43;
    const double t1617 = t1124*t136;
    const double t1627 = (t395+t639+(t889+t648+t420)*t29)*t29;
    const double t1629 = (t647+t404+t398)*t29;
    const double t1635 = (t417+t644+t405)*t29;
    const double t1636 = t369*t43;
    const double t1639 = t376*t43;
    const double t1645 = (t1050+t1034+t1027)*t29;
    const double t1648 = t1023*t43;
    const double t1656 = (t943+t664+t455)*t29;
    const double t1659 = t442*t43;
    const double t1673 = (t368+t1320+(t375+t637+t378)*t29)*t29;
    const double t1675 = (t632+t404+t371)*t29;
    const double t1681 = (t384+t644+t387)*t29;
    const double t1689 = (t1016+t1034+t1013)*t29;
    const double t1699 = (t914+t1341+t917)*t29;
    const double t1703 = t915*t43;
    const double t1706 = t1067*t43;
    const double t1709 = t929*t283;
    const double t1710 = t936*t43;
    const double t1716 = (t432+t664+t435)*t29;
    const double t1723 = t951*t283;
    const double t1755 = t703*t43;
    const double t1758 = t1062*t29;
    const double t1761 = t720*t283;
    const double t1762 = t722*t29;
    const double t1768 = (t1050+t1360+t1027)*t29;
    const double t1771 = t1017*t43;
    const double t1774 = t1438*t136;
    const double t1776 = (t1774+t1443+t1444+t1445+t1446+t1447)*t136;
    const double t1777 = t1060*t283;
    const double t1780 = t1196*t283;
    const double t1791 = t742*t283;
    const double t1794 = t1118*t283;
    const double t1838 = t1062*t5;
    const double t1843 = t720*t351;
    const double t1844 = t722*t5;
    const double t1854 = t1067*t79;
    const double t1863 = t929*t394;
    const double t1865 = t936*t79;
    const double t1878 = t742*t351;
    const double t1881 = t951*t394;
    const double t1882 = t1060*t351;
    const double t476 = x[1];
    const double t1885 = t458*t476;
    const double t1897 = (t253+t264+t242)*t29;
    const double t1902 = t229*t43;
    const double t1916 = t1202*t136;
    const double t1922 = (t943+t794+t455)*t29;
    const double t1925 = t433*t43;
    const double t1935 = (t498+t794+t444)*t29;
    const double t1974 = t508*t476;
    const double t483 = x[0];
    const double t1997 = t297*t483+t1234+t1387+t1471+t1617+t1885+t305+t537+t538+t748+t835;
    const double t1999 = t273+t820+(t522+t826+t289)*t29+(t525+t287+t288+t276)*t43+(t280*t43+
t1219+t276+t287+t288)*t79+(t1916+t1227+t1132+t1229+t1134+t1135)*t136+(t812+
t1128+t1222+t749+t957+t829+t468)*t283+(t1516+t1709+t1128+t1222+t749+t533+t1388+
t468)*t351+(t1533+t1549+t1761+t1128+t1472+t532+t957+t829+t468)*t394+(t1974+
t1863+t1843+t1072+t1128+t1472+t532+t533+t1388+t468)*t476+t1997*t483;
    const double t2001 = t220+t760+(t239+t776+(t475+t774+t255)*t29)*t29+(t221+t762+t1897+(
t482+t245+t241+t224)*t43)*t43+(t221+t762+t1897+(t1902+t779+t247+t231)*t43+(
t1148+t1902+t245+t241+t224)*t79)*t79+(t1079+t1084+(t1179+t1101+t1082)*t29+(
t1092+t1099+t1088+t1089)*t43+(t1093*t43+t1088+t1089+t1099+t1186)*t79+(t1916+
t1208+t1209+t1210+t1211+t1212)*t136)*t136+(t425+t788+t1922+(t736+t663+t443+t428
)*t43+(t1155+t1925+t452+t660+t435)*t79+(t1206+t1190+t1120+t1199+t1113+t1114)*
t136+(t812+t1105+t1158+t813+t1171+t798+t517)*t283)*t283+(t425+t1374+t1935+(t736
+t659+t454+t428)*t43+(t1155+t1925+t441+t664+t435)*t79+(t1206+t1190+t1120+t1192+
t1451+t1114)*t136+(t1723+t1197+t1168+t1710+t954+t1381+t938)*t283+(t1516+t1723+
t1105+t1158+t813+t514+t1517+t517)*t351)*t351+(t425+t788+t1922+(t501+t452+t660+
t435)*t43+(t1458+t1925+t663+t443+t428)*t79+(t1206+t1468+t1109+t1199+t1113+t1114
)*t136+(t1791+t1119+t1461+t744+t1762+t809+t729)*t283+(t1530+t1777+t1774+t1464+
t1073+t1074+t1384+t1045)*t351+(t1533+t1530+t1791+t1105+t1534+t513+t1171+t798+
t517)*t394)*t394+(t425+t1374+t1935+(t501+t441+t664+t435)*t43+(t1458+t1925+t659+
t454+t428)*t79+(t1206+t1468+t1109+t1192+t1451+t1114)*t136+(t1117+t1774+t1464+
t1073+t1074+t1384+t1045)*t283+(t1878+t1777+t1119+t1461+t744+t745+t1844+t729)*
t351+(t1881+t1882+t1777+t1197+t1865+t953+t954+t1381+t938)*t394+(t1974+t1881+
t1878+t1117+t1105+t1534+t513+t514+t1517+t517)*t476)*t476+t1999*t483;
    const double t2003 = t115+t553+(t146+t586+(t165+t602+(t314+t582+t181)*t29)*t29)*t29+(
t116+t557+t1570+(t117+t559+t1572+(t327+t160+t149+t120)*t43)*t43)*t43+(t116+t557
+t1570+(t127+t567+(t605+t196+t157)*t29+(t1581+t593+t156+t130)*t43)*t43+(t117+
t559+t1572+(t135*t43+t130+t156+t593)*t43+(t856+t1581+t160+t149+t120)*t79)*t79)*
t79+(t964+t972+(t965+t997+(t978+t995+t968)*t29)*t29+(t965+t977+t1000+(t990+t998
+t974+t968)*t43)*t43+(t965+t977+t1000+(t1603+t1002+t986+t987)*t43+(t1005+t1603+
t998+t974+t968)*t79)*t79+(t1079+t1178+(t1086+t1187+t1089)*t29+(t1182+t1099+
t1088+t1082)*t43+(t1100*t43+t1082+t1088+t1097+t1099)*t79+(t1617+t1130+t1228+
t1133+t1230+t1135)*t136)*t136)*t136+(t359+t618+t1627+(t360+t620+t1629+(t681+
t640+t370+t363)*t43)*t43+(t368+t628+t1635+(t1636+t643+t386+t371)*t43+(t870+
t1639+t402+t626+t378)*t79)*t79+(t1010+t1015+t1645+(t1053+t1033+t1018+t1013)*t43
+(t1030+t1648+t1057+t1026+t1027)*t79+(t1128+t1107+t1198+t1121+t1193+t1114)*t136
)*t136+(t425+t655+t1656+(t736+t663+t434+t428)*t43+(t878+t1659+t505+t660+t444)*
t79+(t1105+t1040+t1073+t1074+t1044+t1045)*t136+(t748+t1038+t881+t749+t957+t668+
t468)*t283)*t283)*t283+(t359+t1312+t1673+(t360+t1314+t1675+(t681+t629+t397+t363
)*t43)*t43+(t368+t1320+t1681+(t1636+t1327+t404+t371)*t43+(t870+t1639+t384+t637+
t378)*t79)*t79+(t1010+t1354+t1689+(t1053+t1357+t1026+t1013)*t43+(t1030+t1648+
t1033+t1360+t1027)*t79+(t1128+t1107+t1198+t1111+t1367+t1114)*t136)*t136+(t907+
t1336+t1699+(t43*t908+t1340+t910+t916)*t43+(t925+t1703+t922+t1341+t917)*t79+(
t1119+t1063+t1706+t1066+t1364+t1069)*t136+(t1709+t1061+t932+t1710+t954+t1347+
t938)*t283)*t283+(t425+t1374+t1716+(t736+t791+t454+t428)*t43+(t878+t1659+t441+
t794+t444)*t79+(t1105+t1040+t1073+t1043+t1384+t1045)*t136+(t1723+t1061+t932+
t1710+t935+t1381+t938)*t283+(t1387+t1709+t1038+t881+t749+t465+t1388+t468)*t351)
*t351)*t351+(t359+t618+t1627+(t368+t628+t1635+(t390+t402+t626+t378)*t43)*t43+(
t360+t620+t1629+(t1639+t643+t386+t371)*t43+(t1401+t1636+t640+t370+t363)*t79)*
t79+(t1010+t1015+t1645+(t1022+t1057+t1026+t1027)*t43+(t1420+t1648+t1033+t1018+
t1013)*t79+(t1128+t1427+t1191+t1121+t1193+t1114)*t136)*t136+(t696+t701+(t29*
t711+t716+t717)*t29+(t707+t715+t704+t699)*t43+(t1409+t1755+t715+t704+t699)*t79+
(t1197+t1423+t1065+t1758+t1068+t1069)*t136+(t1761+t1061+t1412+t744+t1762+t728+
t729)*t283)*t283+(t1010+t1354+t1768+(t1053+t1024+t1026+t1013)*t43+(t1420+t1771+
t1033+t1034+t1013)*t79+t1776+(t1777+t1441+t1423+t1706+t1758+t1364+t1069)*t283+(
t1450+t1780+t1774+t1427+t1120+t1121+t1451+t1114)*t351)*t351+(t425+t655+t1656+(
t439+t505+t660+t444)*t43+(t1458+t1659+t663+t434+t428)*t79+(t1105+t1464+t1041+
t1074+t1044+t1045)*t136+(t1791+t1061+t1461+t725+t1762+t728+t729)*t283+(t1467+
t1794+t1774+t1468+t1198+t1199+t1367+t1114)*t351+(t1471+t1450+t1761+t1038+t1472+
t463+t957+t668+t468)*t394)*t394)*t394+(t359+t1312+t1673+(t368+t1320+t1681+(t390
+t384+t637+t378)*t43)*t43+(t360+t1314+t1675+(t1639+t1327+t404+t371)*t43+(t1401+
t1636+t629+t397+t363)*t79)*t79+(t1010+t1354+t1689+(t1022+t1033+t1360+t1027)*t43
+(t1420+t1648+t1357+t1026+t1013)*t79+(t1128+t1427+t1191+t1111+t1367+t1114)*t136
)*t136+(t1010+t1354+t1768+(t1053+t1033+t1034+t1013)*t43+(t1420+t1771+t1024+
t1026+t1013)*t79+t1776+(t1127+t1774+t1427+t1120+t1199+t1367+t1114)*t283)*t283+(
t696+(t5*t711+t717)*t5+(t702+t716+t699)*t29+(t707+t708+t716+t699)*t43+(t1409+
t1755+t708+t716+t699)*t79+(t1197+t1423+t1065+t1363+t1838+t1069)*t136+(t1780+
t1441+t1423+t1706+t1066+t1838+t1069)*t283+(t1843+t1777+t1061+t1412+t744+t727+
t1844+t729)*t351)*t351+(t907+t1336+t1699+(t920+t922+t1341+t917)*t43+(t79*t908+
t1340+t1703+t910+t916)*t79+(t1119+t1854+t1424+t1066+t1364+t1069)*t136+(t1794+
t1441+t1854+t1065+t1758+t1364+t1069)*t283+(t1118*t351+t1440*t283+t1065+t1066+
t1069+t1441+t1838+t1854)*t351+(t1196*t351+t1061+t1347+t1777+t1863+t1865+t933+
t938+t954)*t394)*t394+(t425+t1374+t1716+(t439+t441+t794+t444)*t43+(t1458+t1659+
t791+t454+t428)*t79+(t1105+t1464+t1041+t1043+t1384+t1045)*t136+(t1205+t1774+
t1468+t1198+t1121+t1451+t1114)*t283+(t1878+t1794+t1061+t1461+t725+t727+t1844+
t729)*t351+(t1881+t1882+t1780+t1061+t1865+t933+t935+t1381+t938)*t394+(t1885+
t1863+t1843+t1127+t1038+t1472+t463+t465+t1388+t468)*t476)*t476)*t476+t2001*t483
;
    return((t1+(t2+(t3+(t4*t5+t6)*t5)*t5)*t5)*t5+(t1+t25+(t2+t32+(t3+t34+(t29*t4+t18+t6)*t29)*t29)*t29)*t29+(t1+t25+t62+(t2+t32+t67+(t3+t34+t69+(t4*t43+t18+
t56+t6)*t43)*t43)*t43)*t43+(t1+t25+t62+(t15+t51+(t44+(t80+a[33])*t5+(t84+t80+
t47)*t29)*t29+(t16+t55+t91+(t92+t84+t46+t19)*t43)*t43)*t43+(t2+t32+t67+(t16+t55
+t91+(t99+t89+t53+t28)*t43)*t43+(t3+t34+t69+(t99+t84+t46+t19)*t43+(t4*t79+t18+
t56+t6+t92)*t79)*t79)*t79)*t79+(t115+t126+(t116+t134+(t117+t138+(t139+t129+t120
)*t29)*t29)*t29+(t146+t154+t164+(t165+t170+t175+(t177+t179+t180+t181)*t43)*t43)
*t43+(t146+t154+t164+(t188+t193+(t194+t196+t191)*t29+(t200+t202+t203+t204)*t43)
*t43+(t165+t170+t175+(t209*t43+t202+t203+t204)*t43+(t213+t200+t179+t180+t181)*
t79)*t79)*t79+(t220+t228+(t221+t233+(t234+t230+t224)*t29)*t29+(t239+t244+t249+(
t251+t253+t254+t255)*t43)*t43+(t239+t244+t249+(t261+t263+t264+t265)*t43+(t268+
t261+t253+t254+t255)*t79)*t79+(t273+t278+(t279+t281+t276)*t29+(t285+t287+t288+
t289)*t43+(t293*t43+t287+t288+t289+t292)*t79+(t136*t297+t300+t301+t303+t304+
t305)*t136)*t136)*t136)*t136+(t115+t126+t320+(t116+t134+t324+(t117+t138+t326+(
t327+t160+t129+t120)*t43)*t43)*t43+(t146+t154+t338+(t147+t159+t340+(t341+t194+
t156+t150)*t43)*t43+(t165+t170+t348+(t349+t202+t173+t168)*t43+(t213+t352+t334+
t180+t181)*t79)*t79)*t79+(t359+t367+t382+(t368+t373+t389+(t390+t384+t377+t378)*
t43)*t43+(t395+t400+t407+(t408+t410+t404+t405)*t43+(t414+t416+t417+t419+t420)*
t79)*t79+(t425+t430+t437+(t439+t441+t443+t444)*t43+(t448+t450+t452+t454+t455)*
t79+(t459+t461+t463+t465+t467+t468)*t136)*t136)*t136+(t220+t228+t479+(t221+t233
+t481+(t482+t245+t230+t224)*t43)*t43+(t239+t244+t489+(t490+t263+t247+t242)*t43+
(t268+t493+t487+t254+t255)*t79)*t79+(t425+t430+t500+(t501+t441+t434+t435)*t43+(
t448+t504+t505+t454+t455)*t79+(t509+t511+t513+t514+t516+t517)*t136)*t136+(t273+
t278+t524+(t525+t287+t281+t276)*t43+(t292+t528+t529+t288+t289)*t79+(t509+t461+
t532+t533+t467+t468)*t136+(t283*t297+t300+t304+t305+t459+t537+t538)*t283)*t283)
*t283)*t283+(t115+t553+t565+(t116+t557+t572+(t117+t559+t575+(t327+t568+t149+
t120)*t43)*t43)*t43+(t146+t586+t592+(t147+t588+t595+(t341+t593+t190+t150)*t43)*
t43+(t165+t602+t604+(t349+t605+t203+t168)*t43+(t213+t352+t179+t582+t181)*t79)*
t79)*t79+(t359+t618+t625+(t368+t628+t631+(t390+t632+t626+t378)*t43)*t43+(t395+
t639+t642+(t408+t643+t644+t405)*t43+(t414+t416+t647+t648+t420)*t79)*t79+(t425+
t655+t658+(t439+t659+t660+t444)*t43+(t448+t450+t663+t664+t455)*t79+(t459+t461+
t463+t667+t668+t468)*t136)*t136)*t136+(t359+t618+t678+(t360+t620+t680+(t681+
t629+t370+t363)*t43)*t43+(t395+t639+t687+(t688+t643+t404+t398)*t43+(t414+t691+
t417+t648+t420)*t79)*t79+(t696+t701+t706+(t707+t708+t704+t699)*t43+(t711*t79+
t714+t715+t716+t717)*t79+(t721+t723+t725+t727+t728+t729)*t136)*t136+(t425+t655+
t735+(t736+t659+t434+t428)*t43+(t448+t739+t505+t664+t455)*t79+(t743+t723+t744+
t745+t728+t729)*t136+(t748+t721+t461+t749+t533+t668+t468)*t283)*t283)*t283+(
t220+t760+t766+(t221+t762+t769+(t482+t767+t241+t224)*t43)*t43+(t239+t776+t778+(
t490+t779+t264+t242)*t43+(t268+t493+t253+t774+t255)*t79)*t79+(t425+t788+t790+(
t501+t791+t660+t435)*t43+(t448+t504+t663+t794+t455)*t79+(t509+t511+t513+t797+
t798+t517)*t136)*t136+(t425+t788+t804+(t736+t791+t443+t428)*t43+(t448+t739+t452
+t794+t455)*t79+(t743+t723+t744+t727+t809+t729)*t136+(t812+t743+t511+t813+t514+
t798+t517)*t283)*t283+(t273+t820+t822+(t525+t823+t288+t276)*t43+(t292+t528+t287
+t826+t289)*t79+(t509+t461+t532+t667+t829+t468)*t136+(t812+t721+t461+t749+t465+
t829+t468)*t283+(t297*t351+t300+t303+t305+t459+t537+t748+t835)*t351)*t351)*t351
)*t351+(t115+t126+t320+(t146+t154+t338+(t165+t170+t348+(t177+t334+t180+t181)*
t43)*t43)*t43+(t116+t134+t324+(t147+t159+t340+(t352+t202+t173+t168)*t43)*t43+(
t117+t138+t326+(t349+t194+t156+t150)*t43+(t856+t341+t160+t129+t120)*t79)*t79)*
t79+(t359+t367+t382+(t395+t400+t407+(t863+t417+t419+t420)*t43)*t43+(t368+t373+
t389+(t416+t410+t404+t405)*t43+(t870+t408+t384+t377+t378)*t79)*t79+(t425+t430+
t437+(t875+t452+t454+t455)*t43+(t878+t450+t441+t443+t444)*t79+(t459+t881+t882+
t465+t467+t468)*t136)*t136)*t136+(t359+t367+(t395+t400+(t889+t419+t420)*t29)*
t29+(t368+t373+t895+(t390+t402+t377+t378)*t43)*t43+(t368+t373+t895+(t900+t410+
t386+t387)*t43+(t870+t900+t402+t377+t378)*t79)*t79+(t907+(t5*t908+t910)*t5+(
t914+t916+t917)*t29+(t920+t922+t916+t917)*t43+(t925+t926+t922+t916+t917)*t79+(
t930+t932+t933+t935+t937+t938)*t136)*t136+(t425+t430+t945+(t501+t452+t434+t435)
*t43+(t878+t948+t505+t443+t444)*t79+(t952+t932+t953+t954+t937+t938)*t136+(t748+
t930+t881+t532+t957+t467+t468)*t283)*t283)*t283+(t964+t972+t982+(t965+t977+t989
+(t990+t984+t974+t968)*t43)*t43+(t965+t997+t1000+t1004+(t1005+t1001+t998+t995+
t968)*t79)*t79+(t1010+t1015+t1020+(t1022+t1024+t1026+t1027)*t43+(t1030+t1032+
t1033+t1034+t1027)*t79+t1047)*t136+(t1010+t1015+t1052+(t1053+t1024+t1018+t1013)
*t43+(t1030+t1056+t1057+t1034+t1027)*t79+(t1061+t1063+t1065+t1066+t1068+t1069)*
t136+(t1072+t1061+t1040+t1073+t1074+t1044+t1045)*t283)*t283+(t1079+t1084+t1091+
(t1092+t1094+t1088+t1089)*t43+(t1097+t1098+t1099+t1101+t1082)*t79+(t1105+t1107+
t1109+t1111+t1113+t1114)*t136+(t1117+t1119+t1107+t1120+t1121+t1113+t1114)*t283+
(t1125+t1127+t1128+t1130+t1132+t1133+t1134+t1135)*t351)*t351)*t351+(t220+t228+
t479+(t239+t244+t489+(t251+t487+t254+t255)*t43)*t43+(t221+t233+t481+(t493+t263+
t247+t242)*t43+(t1148+t490+t245+t230+t224)*t79)*t79+(t425+t430+t500+(t875+t505+
t454+t455)*t43+(t1155+t504+t441+t434+t435)*t79+(t509+t1158+t1159+t514+t516+t517
)*t136)*t136+(t425+t430+t945+(t439+t505+t443+t444)*t43+(t1155+t948+t452+t434+
t435)*t79+(t952+t1168+t933+t954+t937+t938)*t136+(t812+t952+t1158+t513+t1171+
t516+t517)*t283)*t283+(t1079+t1178+t1181+(t1182+t1183+t1088+t1082)*t43+(t1186+
t1098+t1099+t1187+t1089)*t79+(t1105+t1190+t1191+t1192+t1193+t1114)*t136+(t1117+
t1197+t1190+t1198+t1199+t1193+t1114)*t283+(t1203+t1205+t1206+t1208+t1209+t1210+
t1211+t1212)*t351)*t351+(t273+t278+t524+(t285+t529+t288+t289)*t43+(t1219+t528+
t287+t281+t276)*t79+(t509+t1222+t882+t533+t467+t468)*t136+(t812+t930+t1222+t463
+t957+t467+t468)*t283+(t1203+t1127+t1128+t1227+t1228+t1229+t1230+t1135)*t351+(
t297*t394+t1125+t1234+t301+t304+t305+t459+t538+t748)*t394)*t394)*t394)*t394+(
t115+t553+t565+(t146+t586+t592+(t165+t602+t604+(t177+t179+t582+t181)*t43)*t43)*
t43+(t116+t557+t572+(t147+t588+t595+(t352+t605+t203+t168)*t43)*t43+(t117+t559+
t575+(t349+t593+t190+t150)*t43+(t856+t341+t568+t149+t120)*t79)*t79)*t79+(t359+
t618+t625+(t395+t639+t642+(t863+t647+t648+t420)*t43)*t43+(t368+t628+t631+(t416+
t643+t644+t405)*t43+(t870+t408+t632+t626+t378)*t79)*t79+(t425+t655+t658+(t875+
t663+t664+t455)*t43+(t878+t450+t659+t660+t444)*t79+(t459+t881+t882+t667+t668+
t468)*t136)*t136)*t136+(t964+t972+t982+(t965+t997+t1000+(t990+t998+t995+t968)*
t43)*t43+(t965+t977+t989+t1004+(t1005+t1001+t984+t974+t968)*t79)*t79+(t1010+
t1015+t1020+(t1022+t1033+t1034+t1027)*t43+(t1030+t1032+t1024+t1026+t1027)*t79+
t1047)*t136+(t1079+t1178+t1181+(t1092+t1099+t1187+t1089)*t43+(t1097+t1098+t1183
+t1088+t1082)*t79+(t1105+t1107+t1109+t1192+t1193+t1114)*t136+(t1301+t1128+t1130
+t1132+t1229+t1230+t1135)*t283)*t283)*t283+(t359+t1312+(t360+t1314+(t621+t397+
t363)*t29)*t29+(t368+t1320+t1322+(t390+t632+t637+t378)*t43)*t43+(t368+t1320+
t1322+(t900+t1327+t644+t387)*t43+(t870+t900+t632+t637+t378)*t79)*t79+(t907+
t1336+(t29*t908+t910+t916)*t29+(t920+t1340+t1341+t917)*t43+(t925+t926+t1340+
t1341+t917)*t79+(t930+t932+t933+t1346+t1347+t938)*t136)*t136+(t1010+t1354+t1356
+(t1053+t1357+t1034+t1013)*t43+(t1030+t1056+t1024+t1360+t1027)*t79+(t1061+t1063
+t1065+t1363+t1364+t1069)*t136+(t1127+t1119+t1107+t1120+t1192+t1367+t1114)*t283
)*t283+(t425+t1374+t1376+(t501+t791+t664+t435)*t43+(t878+t948+t659+t794+t444)*
t79+(t952+t932+t953+t1346+t1381+t938)*t136+(t1117+t1061+t1040+t1073+t1043+t1384
+t1045)*t283+(t1387+t1072+t930+t881+t532+t667+t1388+t468)*t351)*t351)*t351+(
t359+t618+t678+(t395+t639+t687+(t863+t417+t648+t420)*t43)*t43+(t360+t620+t680+(
t691+t643+t404+t398)*t43+(t1401+t688+t629+t370+t363)*t79)*t79+(t696+t701+t706+(
t43*t711+t715+t716+t717)*t43+(t1409+t714+t708+t704+t699)*t79+(t721+t1412+t1413+
t727+t728+t729)*t136)*t136+(t1010+t1015+t1052+(t1022+t1057+t1034+t1027)*t43+(
t1420+t1056+t1024+t1018+t1013)*t79+(t1061+t1423+t1424+t1066+t1068+t1069)*t136+(
t1127+t1197+t1427+t1109+t1199+t1193+t1114)*t283)*t283+(t1010+t1354+t1356+(t1022
+t1024+t1360+t1027)*t43+(t1420+t1056+t1357+t1034+t1013)*t79+(t1061+t1423+t1424+
t1363+t1364+t1069)*t136+(t1439+t1441+t1443+t1444+t1445+t1446+t1447)*t283+(t1450
+t1439+t1197+t1427+t1109+t1111+t1451+t1114)*t351)*t351+(t425+t655+t735+(t875+
t505+t664+t455)*t43+(t1458+t739+t659+t434+t428)*t79+(t743+t1461+t1413+t745+t728
+t729)*t136+(t1117+t1061+t1464+t1041+t1074+t1044+t1045)*t283+(t1467+t1439+t1119
+t1468+t1191+t1192+t1367+t1114)*t351+(t1471+t1450+t1072+t721+t1472+t882+t533+
t668+t468)*t394)*t394)*t394+(t220+t760+t766+(t239+t776+t778+(t251+t253+t774+
t255)*t43)*t43+(t221+t762+t769+(t493+t779+t264+t242)*t43+(t1148+t490+t767+t241+
t224)*t79)*t79+(t425+t788+t790+(t875+t663+t794+t455)*t43+(t1155+t504+t791+t660+
t435)*t79+(t509+t1158+t1159+t797+t798+t517)*t136)*t136+(t1079+t1084+t1091+(
t1182+t1099+t1101+t1082)*t43+(t1186+t1098+t1094+t1088+t1089)*t79+(t1105+t1190+
t1191+t1111+t1113+t1114)*t136+(t1503+t1206+t1208+t1209+t1210+t1211+t1212)*t283)
*t283+(t425+t1374+t1376+(t439+t659+t794+t444)*t43+(t1155+t948+t791+t664+t435)*
t79+(t952+t1168+t933+t1346+t1381+t938)*t136+(t1205+t1197+t1190+t1198+t1111+
t1451+t1114)*t283+(t1516+t1117+t952+t1158+t513+t797+t1517+t517)*t351)*t351+(
t425+t788+t804+(t875+t452+t794+t455)*t43+(t1458+t739+t791+t443+t428)*t79+(t743+
t1461+t1413+t727+t809+t729)*t136+(t1205+t1119+t1468+t1191+t1121+t1113+t1114)*
t283+(t1530+t1439+t1061+t1464+t1041+t1043+t1384+t1045)*t351+(t1533+t1530+t1117+
t743+t1534+t1159+t514+t798+t517)*t394)*t394+(t273+t820+t822+(t285+t287+t826+
t289)*t43+(t1219+t528+t823+t288+t276)*t79+(t509+t1222+t882+t667+t829+t468)*t136
+(t1503+t1128+t1227+t1228+t1133+t1134+t1135)*t283+(t1516+t1127+t930+t1222+t463+
t667+t1388+t468)*t351+(t1533+t1549+t1127+t721+t1472+t882+t465+t829+t468)*t394+(
t297*t476+t1234+t1301+t1387+t1471+t301+t303+t305+t459+t835)*t476)*t476)*t476)*
t476+t2003*t483);

}

} // namespace x1b_A1B4_deg5