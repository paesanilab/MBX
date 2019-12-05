#include "poly_1b_A1B3_deg6_v1.h"

namespace mbnrg_A1B3_deg6 {

double poly_A1B3_deg6_v1::eval(const double x[6], 
            const double a[195]) {
    const double t1 = a[1];
    const double t2 = a[3];
    const double t3 = a[18];
    const double t4 = a[43];
    const double t5 = a[137];
    const double t7 = a[100];
    const double t18 = a[5];
    const double t19 = a[11];
    const double t20 = a[25];
    const double t21 = a[172];
    const double t6 = x[5];
    const double t22 = t6*t21;
    const double t23 = a[84];
    const double t31 = (t18+(t19+(t20+(t22+t23)*t6)*t6)*t6)*t6;
    const double t32 = a[26];
    const double t33 = a[175];
    const double t34 = t6*t33;
    const double t35 = a[53];
    const double t41 = (t19+(t32+(t34+t35)*t6)*t6)*t6;
    const double t42 = a[163];
    const double t47 = (t20+(t42*t6+t35)*t6)*t6;
    const double t49 = (t34+t23)*t6;
    const double t62 = a[35];
    const double t64 = t6*a[151];
    const double t65 = a[73];
    const double t73 = t6*a[180];
    const double t74 = a[77];
    const double t78 = (t62+(t73+t74)*t6)*t6;
    const double t80 = (t73+t65)*t6;
    const double t44 = x[4];
    const double t81 = t44*t21;
    const double t94 = t44*t33;
    const double t119 = a[0];
    const double t120 = a[7];
    const double t121 = a[9];
    const double t122 = a[41];
    const double t123 = a[164];
    const double t125 = a[56];
    const double t133 = (t120+(t121+(t122+(t123*t6+t125)*t6)*t6)*t6)*t6;
    const double t134 = a[15];
    const double t135 = a[47];
    const double t136 = a[161];
    const double t137 = t6*t136;
    const double t138 = a[88];
    const double t144 = (t134+(t135+(t137+t138)*t6)*t6)*t6;
    const double t145 = a[156];
    const double t146 = t6*t145;
    const double t147 = a[74];
    const double t151 = (t135+(t146+t147)*t6)*t6;
    const double t153 = (t146+t138)*t6;
    const double t154 = t44*t123;
    const double t163 = a[2];
    const double t164 = a[19];
    const double t165 = a[45];
    const double t166 = a[141];
    const double t167 = t6*t166;
    const double t168 = a[89];
    const double t174 = (t164+(t165+(t167+t168)*t6)*t6)*t6;
    const double t175 = a[30];
    const double t177 = t6*a[112];
    const double t178 = a[94];
    const double t182 = (t175+(t177+t178)*t6)*t6;
    const double t184 = t6*a[106];
    const double t186 = (t184+t178)*t6;
    const double t187 = t44*t166;
    const double t194 = a[12];
    const double t195 = a[46];
    const double t196 = a[140];
    const double t197 = t6*t196;
    const double t198 = a[59];
    const double t202 = (t195+(t197+t198)*t6)*t6;
    const double t204 = t6*a[126];
    const double t205 = a[67];
    const double t207 = (t204+t205)*t6;
    const double t208 = t44*t196;
    const double t213 = a[22];
    const double t214 = a[162];
    const double t215 = t6*t214;
    const double t216 = a[62];
    const double t218 = (t215+t216)*t6;
    const double t219 = t44*t214;
    const double t221 = t6*a[165];
    const double t224 = a[104];
    const double t226 = a[155];
    const double t227 = t44*t226;
    const double t228 = t6*t226;
    const double t229 = a[57];
    const double t238 = a[4];
    const double t239 = a[8];
    const double t240 = a[39];
    const double t241 = a[192];
    const double t243 = a[90];
    const double t249 = (t239+(t240+(t241*t6+t243)*t6)*t6)*t6;
    const double t250 = a[33];
    const double t251 = a[105];
    const double t252 = t6*t251;
    const double t253 = a[98];
    const double t257 = (t250+(t252+t253)*t6)*t6;
    const double t258 = a[110];
    const double t261 = (t258*t6+t253)*t6;
    const double t262 = t241*t44;
    const double t269 = a[10];
    const double t270 = a[36];
    const double t271 = a[166];
    const double t272 = t271*t6;
    const double t273 = a[68];
    const double t277 = (t270+(t272+t273)*t6)*t6;
    const double t279 = a[121]*t6;
    const double t280 = a[50];
    const double t282 = (t279+t280)*t6;
    const double t283 = t44*t271;
    const double t288 = a[42];
    const double t289 = a[118];
    const double t290 = t289*t6;
    const double t291 = a[81];
    const double t293 = (t290+t291)*t6;
    const double t294 = t44*t289;
    const double t296 = a[139]*t6;
    const double t299 = a[147];
    const double t301 = a[150];
    const double t302 = t44*t301;
    const double t303 = t301*t6;
    const double t304 = a[85];
    const double t311 = a[13];
    const double t312 = a[24];
    const double t313 = a[129];
    const double t315 = a[51];
    const double t319 = (t312+(t313*t6+t315)*t6)*t6;
    const double t320 = a[124];
    const double t321 = t6*t320;
    const double t322 = a[99];
    const double t324 = (t321+t322)*t6;
    const double t325 = t313*t44;
    const double t330 = a[40];
    const double t331 = a[130];
    const double t332 = t331*t6;
    const double t333 = a[72];
    const double t335 = (t332+t333)*t6;
    const double t336 = t44*t331;
    const double t338 = a[133]*t6;
    const double t341 = a[149];
    const double t343 = a[191];
    const double t344 = t44*t343;
    const double t345 = t343*t6;
    const double t346 = a[65];
    const double t351 = a[38];
    const double t352 = a[132];
    const double t354 = a[79];
    const double t356 = (t352*t6+t354)*t6;
    const double t357 = t352*t44;
    const double t358 = a[136];
    const double t359 = t6*t358;
    const double t362 = a[185];
    const double t364 = a[117];
    const double t365 = t44*t364;
    const double t366 = t364*t6;
    const double t367 = a[97];
    const double t370 = a[184];
    const double t372 = a[122];
    const double t374 = a[168];
    const double t375 = t374*t44;
    const double t376 = t6*t374;
    const double t377 = a[101];
    const double t118 = x[3];
    const double t409 = t118*t123;
    const double t418 = a[6];
    const double t419 = a[17];
    const double t420 = a[44];
    const double t421 = a[157];
    const double t423 = a[86];
    const double t430 = a[20];
    const double t431 = a[37];
    const double t432 = a[189];
    const double t433 = t6*t432;
    const double t434 = a[92];
    const double t438 = (t431+(t433+t434)*t6)*t6;
    const double t439 = a[48];
    const double t440 = a[176];
    const double t441 = t6*t440;
    const double t442 = a[80];
    const double t444 = (t441+t442)*t6;
    const double t445 = a[115];
    const double t446 = t44*t445;
    const double t447 = a[181];
    const double t448 = t6*t447;
    const double t449 = a[58];
    const double t456 = a[32];
    const double t458 = t6*a[193];
    const double t459 = a[63];
    const double t462 = a[135];
    const double t463 = t44*t462;
    const double t465 = a[108]*t6;
    const double t466 = a[78];
    const double t471 = a[109];
    const double t475 = t118*t445;
    const double t482 = a[16];
    const double t483 = a[31];
    const double t484 = a[160];
    const double t486 = a[60];
    const double t490 = (t483+(t484*t6+t486)*t6)*t6;
    const double t491 = a[28];
    const double t492 = a[103];
    const double t493 = t6*t492;
    const double t494 = a[70];
    const double t496 = (t493+t494)*t6;
    const double t497 = a[125];
    const double t498 = t497*t44;
    const double t499 = a[138];
    const double t500 = t6*t499;
    const double t501 = a[66];
    const double t506 = a[29];
    const double t507 = a[159];
    const double t508 = t507*t6;
    const double t509 = a[93];
    const double t511 = (t508+t509)*t6;
    const double t512 = a[154];
    const double t513 = t44*t512;
    const double t515 = t6*a[152];
    const double t516 = a[82];
    const double t519 = a[119];
    const double t520 = t118*t519;
    const double t521 = a[153];
    const double t522 = t44*t521;
    const double t523 = a[145];
    const double t524 = t523*t6;
    const double t525 = a[71];
    const double t530 = a[49];
    const double t531 = a[186];
    const double t533 = a[87];
    const double t535 = (t531*t6+t533)*t6;
    const double t536 = a[173];
    const double t537 = t536*t44;
    const double t538 = a[194];
    const double t539 = t6*t538;
    const double t540 = a[54];
    const double t543 = a[158];
    const double t544 = t118*t543;
    const double t545 = a[111];
    const double t546 = t44*t545;
    const double t547 = a[178];
    const double t548 = t547*t6;
    const double t549 = a[96];
    const double t552 = a[182];
    const double t150 = x[2];
    const double t553 = t552*t150;
    const double t554 = a[120];
    const double t555 = t118*t554;
    const double t556 = a[170];
    const double t557 = t556*t44;
    const double t558 = a[116];
    const double t559 = t6*t558;
    const double t560 = a[75];
    const double t582 = t241*t118;
    const double t589 = t519*t44;
    const double t596 = t497*t118;
    const double t601 = a[27];
    const double t602 = a[123];
    const double t604 = a[76];
    const double t607 = a[128];
    const double t608 = t607*t44;
    const double t609 = a[144];
    const double t610 = t609*t6;
    const double t611 = a[55];
    const double t614 = t607*t118;
    const double t615 = a[102];
    const double t619 = a[142];
    const double t620 = t619*t150;
    const double t621 = a[187];
    const double t622 = t621*t118;
    const double t623 = a[188];
    const double t624 = t623*t44;
    const double t625 = a[146];
    const double t626 = t6*t625;
    const double t627 = a[83];
    const double t641 = t313*t118;
    const double t646 = t543*t44;
    const double t649 = t536*t118;
    const double t652 = a[148];
    const double t653 = t652*t150;
    const double t654 = t623*t118;
    const double t655 = t621*t44;
    const double t663 = t352*t118;
    const double t666 = t556*t118;
    const double t667 = t554*t44;
    const double t671 = t374*t118;
    const double t697 = (t164+(t195+(t228+t216)*t6)*t6)*t6;
    const double t701 = (t165+(t215+t198)*t6)*t6;
    const double t703 = (t197+t168)*t6;
    const double t717 = (t204+t178)*t6;
    const double t718 = t44*t136;
    const double t725 = t44*t145;
    const double t746 = (t430+(t439+(t445*t6+t449)*t6)*t6)*t6;
    const double t750 = (t431+(t448+t442)*t6)*t6;
    const double t752 = (t441+t434)*t6;
    const double t760 = t6*t462;
    const double t764 = (t456+(t760+t466)*t6)*t6;
    const double t766 = (t465+t459)*t6;
    const double t767 = t44*t432;
    const double t774 = (t471*t6+t466)*t6;
    const double t775 = t44*t440;
    const double t778 = t44*t447;
    const double t789 = (t491+(t497*t6+t501)*t6)*t6;
    const double t791 = (t500+t494)*t6;
    const double t792 = t484*t44;
    const double t797 = t512*t6;
    const double t799 = (t797+t516)*t6;
    const double t800 = t507*t44;
    const double t803 = t523*t44;
    const double t804 = t521*t6;
    const double t811 = (t536*t6+t540)*t6;
    const double t812 = t531*t44;
    const double t815 = t547*t44;
    const double t816 = t545*t6;
    const double t819 = t558*t44;
    const double t820 = t556*t6;
    const double t849 = a[23];
    const double t850 = a[113];
    const double t852 = a[69];
    const double t857 = a[143];
    const double t858 = t6*t857;
    const double t859 = a[52];
    const double t861 = (t858+t859)*t6;
    const double t867 = t44*t857;
    const double t877 = a[34];
    const double t878 = a[190];
    const double t880 = a[64];
    const double t882 = (t878*t6+t880)*t6;
    const double t883 = t878*t44;
    const double t884 = a[171];
    const double t885 = t884*t6;
    const double t888 = a[114];
    const double t890 = a[183];
    const double t891 = t890*t44;
    const double t892 = t890*t6;
    const double t893 = a[95];
    const double t897 = a[169]*t150;
    const double t898 = a[127];
    const double t900 = a[167];
    const double t901 = t900*t44;
    const double t902 = t900*t6;
    const double t903 = a[61];
    const double t916 = t484*t118;
    const double t924 = t878*t118;
    const double t928 = a[179]*t150;
    const double t929 = a[177];
    const double t930 = t929*t118;
    const double t931 = t929*t44;
    const double t932 = a[174];
    const double t934 = a[91];
    const double t941 = t531*t118;
    const double t944 = t900*t118;
    const double t225 = x[1];
    const double t948 = t552*t225;
    const double t949 = t558*t118;
    const double t968 = (t270+(t303+t291)*t6)*t6;
    const double t970 = (t290+t273)*t6;
    const double t979 = t251*t44;
    const double t997 = (t506+(t519*t6+t525)*t6)*t6;
    const double t999 = (t524+t509)*t6;
    const double t1005 = (t804+t516)*t6;
    const double t1006 = t492*t44;
    const double t1009 = t499*t44;
    const double t1016 = (t607*t6+t611)*t6;
    const double t1020 = t609*t44;
    const double t1021 = t615*t6;
    const double t1024 = t625*t44;
    const double t1025 = t623*t6;
    const double t1051 = t929*t6;
    const double t1066 = t619*t225;
    const double t1067 = t625*t118;
    const double t1080 = (t345+t333)*t6;
    const double t1085 = t320*t44;
    const double t1094 = (t543*t6+t549)*t6;
    const double t1097 = t538*t44;
    const double t1100 = t621*t6;
    const double t1125 = t554*t6;
    const double t1135 = x[0];
    return((t1+(t2+(t3+(t4+(t5*t6+t7)*t6)*t6)*t6)*t6)*t6+(t1+t31+(t2+t41+(t3+t47+(t4+t49+(t5*t44+t22+t7)*t44)*t44)*t44)*t44)*t44+(t1+t31+(t18+(a[21]+(t62+(
t64+t65)*t6)*t6)*t6+(t19+t78+(t20+t80+(t81+t64+t23)*t44)*t44)*t44)*t44+(t2+t41+
(t19+t78+(t32+(t6*a[131]+t74)*t6+(t94+t73+t35)*t44)*t44)*t44+(t3+t47+(t20+t80+(
t42*t44+t35+t73)*t44)*t44+(t4+t49+(t94+t64+t23)*t44+(t5*t118+t22+t7+t81)*t118)*
t118)*t118)*t118)*t118+(t119+t133+(t120+t144+(t121+t151+(t122+t153+(t154+t137+
t125)*t44)*t44)*t44)*t44+(t163+t174+(t164+t182+(t165+t186+(t187+t177+t168)*t44)
*t44)*t44+(t194+t202+(t195+t207+(t208+t204+t198)*t44)*t44+(t213+t218+(t219+t221
+t216)*t44+(t224*t118+t227+t228+t229)*t118)*t118)*t118)*t118+(t238+t249+(t239+
t257+(t240+t261+(t262+t252+t243)*t44)*t44)*t44+(t269+t277+(t270+t282+(t283+t279
+t273)*t44)*t44+(t288+t293+(t294+t296+t291)*t44+(t299*t118+t302+t303+t304)*t118
)*t118)*t118+(t311+t319+(t312+t324+(t325+t321+t315)*t44)*t44+(t330+t335+(t336+
t338+t333)*t44+(t341*t118+t344+t345+t346)*t118)*t118+(t351+t356+(t357+t359+t354
)*t44+(t362*t118+t365+t366+t367)*t118+(t372*t118+t370*t150+t375+t376+t377)*t150
)*t150)*t150)*t150)*t150+(t119+t133+(t163+t174+(t194+t202+(t213+t218+(t224*t44+
t228+t229)*t44)*t44)*t44)*t44+(t120+t144+(t164+t182+(t195+t207+(t227+t221+t216)
*t44)*t44)*t44+(t121+t151+(t165+t186+(t219+t204+t198)*t44)*t44+(t122+t153+(t208
+t177+t168)*t44+(t409+t187+t137+t125)*t118)*t118)*t118)*t118+(t418+(t419+(t420+
(t421*t6+t423)*t6)*t6)*t6+(t430+t438+(t439+t444+(t446+t448+t449)*t44)*t44)*t44+
(t430+t438+(t456+(t458+t459)*t6+(t463+t465+t466)*t44)*t44+(t439+t444+(t471*t44+
t465+t466)*t44+(t475+t463+t448+t449)*t118)*t118)*t118+(t482+t490+(t491+t496+(
t498+t500+t501)*t44)*t44+(t506+t511+(t513+t515+t516)*t44+(t520+t522+t524+t525)*
t118)*t118+(t530+t535+(t537+t539+t540)*t44+(t544+t546+t548+t549)*t118+(t553+
t555+t557+t559+t560)*t150)*t150)*t150)*t150+(t238+t249+(t269+t277+(t288+t293+(
t299*t44+t303+t304)*t44)*t44)*t44+(t239+t257+(t270+t282+(t302+t296+t291)*t44)*
t44+(t240+t261+(t294+t279+t273)*t44+(t582+t283+t252+t243)*t118)*t118)*t118+(
t482+t490+(t506+t511+(t589+t524+t525)*t44)*t44+(t491+t496+(t522+t515+t516)*t44+
(t596+t513+t500+t501)*t118)*t118+(t601+(t602*t6+t604)*t6+(t608+t610+t611)*t44+(
t615*t44+t610+t611+t614)*t118+(t620+t622+t624+t626+t627)*t150)*t150)*t150+(t311
+t319+(t330+t335+(t341*t44+t345+t346)*t44)*t44+(t312+t324+(t344+t338+t333)*t44+
(t641+t336+t321+t315)*t118)*t118+(t530+t535+(t646+t548+t549)*t44+(t649+t546+
t539+t540)*t118+(t653+t654+t655+t626+t627)*t150)*t150+(t351+t356+(t362*t44+t366
+t367)*t44+(t663+t365+t359+t354)*t118+(t620+t666+t667+t559+t560)*t150+(t370*
t225+t372*t44+t376+t377+t553+t671)*t225)*t225)*t225)*t225)*t225+(t119+(t163+(
t194+(t213+(t224*t6+t229)*t6)*t6)*t6)*t6+(t120+t697+(t121+t701+(t122+t703+(t154
+t167+t125)*t44)*t44)*t44)*t44+(t120+t697+(t134+(t175+(t221+t205)*t6)*t6+(t135+
t717+(t718+t177+t138)*t44)*t44)*t44+(t121+t701+(t135+t717+(t725+t184+t147)*t44)
*t44+(t122+t703+(t725+t177+t138)*t44+(t409+t718+t167+t125)*t118)*t118)*t118)*
t118+(t418+t746+(t419+t750+(t420+t752+(t421*t44+t423+t433)*t44)*t44)*t44+(t430+
t764+(t431+t766+(t767+t458+t434)*t44)*t44+(t439+t774+(t775+t465+t442)*t44+(t475
+t778+t760+t449)*t118)*t118)*t118+(t482+t789+(t483+t791+(t792+t493+t486)*t44)*
t44+(t506+t799+(t800+t515+t509)*t44+(t520+t803+t804+t525)*t118)*t118+(t530+t811
+(t812+t539+t533)*t44+(t544+t815+t816+t549)*t118+(t553+t555+t819+t820+t560)*
t150)*t150)*t150)*t150+(t418+t746+(t430+t764+(t439+t774+(t446+t760+t449)*t44)*
t44)*t44+(t419+t750+(t431+t766+(t778+t465+t442)*t44)*t44+(t420+t752+(t775+t458+
t434)*t44+(t421*t118+t423+t433+t767)*t118)*t118)*t118+(a[14]+(t849+(t850*t6+
t852)*t6)*t6+(t849+t861+(t850*t44+t852+t858)*t44)*t44+(t849+t861+(t6*a[107]+
t859+t867)*t44+(t850*t118+t852+t858+t867)*t118)*t118+(t877+t882+(t883+t885+t880
)*t44+(t888*t118+t891+t892+t893)*t118+(t898*t118+t897+t901+t902+t903)*t150)*
t150)*t150+(t482+t789+(t506+t799+(t589+t804+t525)*t44)*t44+(t483+t791+(t803+
t515+t509)*t44+(t916+t800+t493+t486)*t118)*t118+(t877+t882+(t888*t44+t892+t893)
*t44+(t924+t891+t885+t880)*t118+(t932*t6+t928+t930+t931+t934)*t150)*t150+(t530+
t811+(t646+t816+t549)*t44+(t941+t815+t539+t533)*t118+(t898*t44+t902+t903+t928+
t944)*t150+(t948+t897+t949+t667+t820+t560)*t225)*t225)*t225)*t225+(t238+(t269+(
t288+(t299*t6+t304)*t6)*t6)*t6+(t239+t968+(t240+t970+(t262+t272+t243)*t44)*t44)
*t44+(t239+t968+(t250+(t296+t280)*t6+(t979+t279+t253)*t44)*t44+(t240+t970+(t258
*t44+t253+t279)*t44+(t582+t979+t272+t243)*t118)*t118)*t118+(t482+t997+(t483+
t999+(t792+t508+t486)*t44)*t44+(t491+t1005+(t1006+t515+t494)*t44+(t596+t1009+
t797+t501)*t118)*t118+(t601+t1016+(t602*t44+t604+t610)*t44+(t614+t1020+t1021+
t611)*t118+(t620+t622+t1024+t1025+t627)*t150)*t150)*t150+(t482+t997+(t491+t1005
+(t498+t797+t501)*t44)*t44+(t483+t999+(t1009+t515+t494)*t44+(t916+t1006+t508+
t486)*t118)*t118+(t877+(t888*t6+t893)*t6+(t883+t892+t880)*t44+(t884*t44+t880+
t892+t924)*t118+(t932*t44+t1051+t928+t930+t934)*t150)*t150+(t601+t1016+(t608+
t1021+t611)*t44+(t602*t118+t1020+t604+t610)*t118+(t932*t118+t150*a[134]+t1051+
t931+t934)*t150+(t1066+t928+t1067+t655+t1025+t627)*t225)*t225)*t225+(t311+(t330
+(t341*t6+t346)*t6)*t6+(t312+t1080+(t325+t332+t315)*t44)*t44+(t312+t1080+(t1085
+t338+t322)*t44+(t641+t1085+t332+t315)*t118)*t118+(t530+t1094+(t812+t548+t533)*
t44+(t649+t1097+t816+t540)*t118+(t653+t654+t1024+t1100+t627)*t150)*t150+(t530+
t1094+(t537+t816+t540)*t44+(t941+t1097+t548+t533)*t118+(t898*t6+t901+t903+t928+
t944)*t150+(t652*t225+t1067+t1100+t624+t627+t928)*t225)*t225+(t351+(t362*t6+
t367)*t6+(t357+t366+t354)*t44+(t358*t44+t354+t366+t663)*t118+(t620+t666+t819+
t1125+t560)*t150+(t1066+t897+t949+t557+t1125+t560)*t225+(t370*t1135+t372*t6+
t375+t377+t553+t671+t948)*t1135)*t1135)*t1135)*t1135)*t1135);
}

} // namespace mbnrg_A1B3_deg6

