#include "poly_1b_A1B3_deg6_v1.h"

namespace mbnrg_A1B3_deg6 {

double poly_A1B3_deg6_v1::eval(const double x[6], 
            const double a[195],
                  double g[6]) {
    const double t1 = a[1];
    const double t2 = a[3];
    const double t3 = a[18];
    const double t4 = a[43];
    const double t5 = a[137];
    const double t8 = x[5];
    const double t6 = t8*t5;
    const double t7 = a[100];
    const double t9 = (t6+t7)*t8;
    const double t11 = (t4+t9)*t8;
    const double t13 = (t3+t11)*t8;
    const double t15 = (t2+t13)*t8;
    const double t18 = a[5];
    const double t19 = a[11];
    const double t20 = a[25];
    const double t21 = a[172];
    const double t22 = t8*t21;
    const double t23 = a[84];
    const double t25 = (t22+t23)*t8;
    const double t27 = (t20+t25)*t8;
    const double t29 = (t19+t27)*t8;
    const double t31 = (t18+t29)*t8;
    const double t32 = a[26];
    const double t33 = a[175];
    const double t34 = t8*t33;
    const double t35 = a[53];
    const double t37 = (t34+t35)*t8;
    const double t39 = (t32+t37)*t8;
    const double t41 = (t19+t39)*t8;
    const double t42 = a[163];
    const double t43 = t8*t42;
    const double t45 = (t43+t35)*t8;
    const double t47 = (t20+t45)*t8;
    const double t49 = (t34+t23)*t8;
    const double t48 = x[4];
    const double t50 = t48*t5;
    const double t52 = (t50+t22+t7)*t48;
    const double t54 = (t4+t49+t52)*t48;
    const double t56 = (t3+t47+t54)*t48;
    const double t58 = (t2+t41+t56)*t48;
    const double t61 = a[21];
    const double t62 = a[35];
    const double t63 = a[151];
    const double t64 = t8*t63;
    const double t65 = a[73];
    const double t67 = (t64+t65)*t8;
    const double t69 = (t62+t67)*t8;
    const double t71 = (t61+t69)*t8;
    const double t72 = a[180];
    const double t73 = t8*t72;
    const double t74 = a[77];
    const double t76 = (t73+t74)*t8;
    const double t78 = (t62+t76)*t8;
    const double t80 = (t73+t65)*t8;
    const double t81 = t48*t21;
    const double t83 = (t81+t64+t23)*t48;
    const double t85 = (t20+t80+t83)*t48;
    const double t87 = (t19+t78+t85)*t48;
    const double t89 = (t18+t71+t87)*t48;
    const double t91 = t8*a[131];
    const double t93 = (t91+t74)*t8;
    const double t94 = t48*t33;
    const double t96 = (t94+t73+t35)*t48;
    const double t98 = (t32+t93+t96)*t48;
    const double t100 = (t19+t78+t98)*t48;
    const double t101 = t48*t42;
    const double t103 = (t101+t73+t35)*t48;
    const double t105 = (t20+t80+t103)*t48;
    const double t107 = (t94+t64+t23)*t48;
    const double t104 = x[3];
    const double t108 = t104*t5;
    const double t110 = (t108+t81+t22+t7)*t104;
    const double t112 = (t4+t49+t107+t110)*t104;
    const double t114 = (t3+t47+t105+t112)*t104;
    const double t116 = (t2+t41+t100+t114)*t104;
    const double t119 = a[0];
    const double t120 = a[7];
    const double t121 = a[9];
    const double t122 = a[41];
    const double t123 = a[164];
    const double t124 = t8*t123;
    const double t125 = a[56];
    const double t127 = (t124+t125)*t8;
    const double t129 = (t122+t127)*t8;
    const double t131 = (t121+t129)*t8;
    const double t133 = (t120+t131)*t8;
    const double t134 = a[15];
    const double t135 = a[47];
    const double t136 = a[161];
    const double t137 = t8*t136;
    const double t138 = a[88];
    const double t140 = (t137+t138)*t8;
    const double t142 = (t135+t140)*t8;
    const double t144 = (t134+t142)*t8;
    const double t145 = a[156];
    const double t146 = t8*t145;
    const double t147 = a[74];
    const double t149 = (t146+t147)*t8;
    const double t151 = (t135+t149)*t8;
    const double t153 = (t146+t138)*t8;
    const double t154 = t48*t123;
    const double t156 = (t154+t137+t125)*t48;
    const double t158 = (t122+t153+t156)*t48;
    const double t160 = (t121+t151+t158)*t48;
    const double t162 = (t120+t144+t160)*t48;
    const double t163 = a[2];
    const double t164 = a[19];
    const double t165 = a[45];
    const double t166 = a[141];
    const double t167 = t8*t166;
    const double t168 = a[89];
    const double t170 = (t167+t168)*t8;
    const double t172 = (t165+t170)*t8;
    const double t174 = (t164+t172)*t8;
    const double t175 = a[30];
    const double t176 = a[112];
    const double t177 = t8*t176;
    const double t178 = a[94];
    const double t180 = (t177+t178)*t8;
    const double t182 = (t175+t180)*t8;
    const double t183 = a[106];
    const double t184 = t8*t183;
    const double t186 = (t184+t178)*t8;
    const double t187 = t48*t166;
    const double t189 = (t187+t177+t168)*t48;
    const double t191 = (t165+t186+t189)*t48;
    const double t193 = (t164+t182+t191)*t48;
    const double t194 = a[12];
    const double t195 = a[46];
    const double t196 = a[140];
    const double t197 = t8*t196;
    const double t198 = a[59];
    const double t200 = (t197+t198)*t8;
    const double t202 = (t195+t200)*t8;
    const double t203 = a[126];
    const double t204 = t8*t203;
    const double t205 = a[67];
    const double t207 = (t204+t205)*t8;
    const double t208 = t48*t196;
    const double t210 = (t208+t204+t198)*t48;
    const double t212 = (t195+t207+t210)*t48;
    const double t213 = a[22];
    const double t214 = a[162];
    const double t215 = t8*t214;
    const double t216 = a[62];
    const double t218 = (t215+t216)*t8;
    const double t219 = t48*t214;
    const double t220 = a[165];
    const double t221 = t8*t220;
    const double t223 = (t219+t221+t216)*t48;
    const double t224 = a[104];
    const double t225 = t104*t224;
    const double t226 = a[155];
    const double t227 = t48*t226;
    const double t228 = t8*t226;
    const double t229 = a[57];
    const double t231 = (t225+t227+t228+t229)*t104;
    const double t233 = (t213+t218+t223+t231)*t104;
    const double t235 = (t194+t202+t212+t233)*t104;
    const double t237 = (t163+t174+t193+t235)*t104;
    const double t238 = a[4];
    const double t239 = a[8];
    const double t240 = a[39];
    const double t241 = a[192];
    const double t242 = t8*t241;
    const double t243 = a[90];
    const double t245 = (t242+t243)*t8;
    const double t247 = (t240+t245)*t8;
    const double t249 = (t239+t247)*t8;
    const double t250 = a[33];
    const double t251 = a[105];
    const double t252 = t8*t251;
    const double t253 = a[98];
    const double t255 = (t252+t253)*t8;
    const double t257 = (t250+t255)*t8;
    const double t258 = a[110];
    const double t259 = t8*t258;
    const double t261 = (t259+t253)*t8;
    const double t262 = t48*t241;
    const double t264 = (t262+t252+t243)*t48;
    const double t266 = (t240+t261+t264)*t48;
    const double t268 = (t239+t257+t266)*t48;
    const double t269 = a[10];
    const double t270 = a[36];
    const double t271 = a[166];
    const double t272 = t8*t271;
    const double t273 = a[68];
    const double t275 = (t272+t273)*t8;
    const double t277 = (t270+t275)*t8;
    const double t278 = a[121];
    const double t279 = t8*t278;
    const double t280 = a[50];
    const double t282 = (t279+t280)*t8;
    const double t283 = t48*t271;
    const double t285 = (t283+t279+t273)*t48;
    const double t287 = (t270+t282+t285)*t48;
    const double t288 = a[42];
    const double t289 = a[118];
    const double t290 = t8*t289;
    const double t291 = a[81];
    const double t293 = (t290+t291)*t8;
    const double t294 = t48*t289;
    const double t295 = a[139];
    const double t296 = t8*t295;
    const double t298 = (t294+t296+t291)*t48;
    const double t299 = a[147];
    const double t300 = t104*t299;
    const double t301 = a[150];
    const double t302 = t48*t301;
    const double t303 = t8*t301;
    const double t304 = a[85];
    const double t306 = (t300+t302+t303+t304)*t104;
    const double t308 = (t288+t293+t298+t306)*t104;
    const double t310 = (t269+t277+t287+t308)*t104;
    const double t311 = a[13];
    const double t312 = a[24];
    const double t313 = a[129];
    const double t314 = t8*t313;
    const double t315 = a[51];
    const double t317 = (t314+t315)*t8;
    const double t319 = (t312+t317)*t8;
    const double t320 = a[124];
    const double t321 = t8*t320;
    const double t322 = a[99];
    const double t324 = (t321+t322)*t8;
    const double t325 = t48*t313;
    const double t327 = (t325+t321+t315)*t48;
    const double t329 = (t312+t324+t327)*t48;
    const double t330 = a[40];
    const double t331 = a[130];
    const double t332 = t8*t331;
    const double t333 = a[72];
    const double t335 = (t332+t333)*t8;
    const double t336 = t48*t331;
    const double t337 = a[133];
    const double t338 = t8*t337;
    const double t340 = (t336+t338+t333)*t48;
    const double t341 = a[149];
    const double t342 = t104*t341;
    const double t343 = a[191];
    const double t344 = t48*t343;
    const double t345 = t8*t343;
    const double t346 = a[65];
    const double t348 = (t342+t344+t345+t346)*t104;
    const double t350 = (t330+t335+t340+t348)*t104;
    const double t351 = a[38];
    const double t352 = a[132];
    const double t353 = t8*t352;
    const double t354 = a[79];
    const double t356 = (t353+t354)*t8;
    const double t357 = t48*t352;
    const double t358 = a[136];
    const double t359 = t8*t358;
    const double t361 = (t357+t359+t354)*t48;
    const double t362 = a[185];
    const double t363 = t104*t362;
    const double t364 = a[117];
    const double t365 = t48*t364;
    const double t366 = t8*t364;
    const double t367 = a[97];
    const double t369 = (t363+t365+t366+t367)*t104;
    const double t370 = a[184];
    const double t355 = x[2];
    const double t371 = t355*t370;
    const double t372 = a[122];
    const double t373 = t104*t372;
    const double t374 = a[168];
    const double t375 = t48*t374;
    const double t376 = t8*t374;
    const double t377 = a[101];
    const double t379 = (t371+t373+t375+t376+t377)*t355;
    const double t381 = (t351+t356+t361+t369+t379)*t355;
    const double t383 = (t311+t319+t329+t350+t381)*t355;
    const double t385 = (t238+t249+t268+t310+t383)*t355;
    const double t388 = t48*t224;
    const double t390 = (t388+t228+t229)*t48;
    const double t392 = (t213+t218+t390)*t48;
    const double t394 = (t194+t202+t392)*t48;
    const double t396 = (t163+t174+t394)*t48;
    const double t398 = (t227+t221+t216)*t48;
    const double t400 = (t195+t207+t398)*t48;
    const double t402 = (t164+t182+t400)*t48;
    const double t404 = (t219+t204+t198)*t48;
    const double t406 = (t165+t186+t404)*t48;
    const double t408 = (t208+t177+t168)*t48;
    const double t409 = t104*t123;
    const double t411 = (t409+t187+t137+t125)*t104;
    const double t413 = (t122+t153+t408+t411)*t104;
    const double t415 = (t121+t151+t406+t413)*t104;
    const double t417 = (t120+t144+t402+t415)*t104;
    const double t418 = a[6];
    const double t419 = a[17];
    const double t420 = a[44];
    const double t421 = a[157];
    const double t422 = t8*t421;
    const double t423 = a[86];
    const double t425 = (t422+t423)*t8;
    const double t427 = (t420+t425)*t8;
    const double t429 = (t419+t427)*t8;
    const double t430 = a[20];
    const double t431 = a[37];
    const double t432 = a[189];
    const double t433 = t8*t432;
    const double t434 = a[92];
    const double t436 = (t433+t434)*t8;
    const double t438 = (t431+t436)*t8;
    const double t439 = a[48];
    const double t440 = a[176];
    const double t441 = t8*t440;
    const double t442 = a[80];
    const double t444 = (t441+t442)*t8;
    const double t445 = a[115];
    const double t446 = t48*t445;
    const double t447 = a[181];
    const double t448 = t8*t447;
    const double t449 = a[58];
    const double t451 = (t446+t448+t449)*t48;
    const double t453 = (t439+t444+t451)*t48;
    const double t455 = (t430+t438+t453)*t48;
    const double t456 = a[32];
    const double t457 = a[193];
    const double t458 = t8*t457;
    const double t459 = a[63];
    const double t461 = (t458+t459)*t8;
    const double t462 = a[135];
    const double t463 = t48*t462;
    const double t464 = a[108];
    const double t465 = t8*t464;
    const double t466 = a[78];
    const double t468 = (t463+t465+t466)*t48;
    const double t470 = (t456+t461+t468)*t48;
    const double t471 = a[109];
    const double t472 = t48*t471;
    const double t474 = (t472+t465+t466)*t48;
    const double t475 = t104*t445;
    const double t477 = (t475+t463+t448+t449)*t104;
    const double t479 = (t439+t444+t474+t477)*t104;
    const double t481 = (t430+t438+t470+t479)*t104;
    const double t482 = a[16];
    const double t483 = a[31];
    const double t484 = a[160];
    const double t485 = t8*t484;
    const double t486 = a[60];
    const double t488 = (t485+t486)*t8;
    const double t490 = (t483+t488)*t8;
    const double t491 = a[28];
    const double t492 = a[103];
    const double t493 = t8*t492;
    const double t494 = a[70];
    const double t496 = (t493+t494)*t8;
    const double t497 = a[125];
    const double t498 = t48*t497;
    const double t499 = a[138];
    const double t500 = t8*t499;
    const double t501 = a[66];
    const double t503 = (t498+t500+t501)*t48;
    const double t505 = (t491+t496+t503)*t48;
    const double t506 = a[29];
    const double t507 = a[159];
    const double t508 = t8*t507;
    const double t509 = a[93];
    const double t511 = (t508+t509)*t8;
    const double t512 = a[154];
    const double t513 = t48*t512;
    const double t514 = a[152];
    const double t515 = t8*t514;
    const double t516 = a[82];
    const double t518 = (t513+t515+t516)*t48;
    const double t519 = a[119];
    const double t520 = t104*t519;
    const double t521 = a[153];
    const double t522 = t48*t521;
    const double t523 = a[145];
    const double t524 = t8*t523;
    const double t525 = a[71];
    const double t527 = (t520+t522+t524+t525)*t104;
    const double t529 = (t506+t511+t518+t527)*t104;
    const double t530 = a[49];
    const double t531 = a[186];
    const double t532 = t8*t531;
    const double t533 = a[87];
    const double t535 = (t532+t533)*t8;
    const double t536 = a[173];
    const double t537 = t48*t536;
    const double t538 = a[194];
    const double t539 = t8*t538;
    const double t540 = a[54];
    const double t542 = (t537+t539+t540)*t48;
    const double t543 = a[158];
    const double t544 = t104*t543;
    const double t545 = a[111];
    const double t546 = t48*t545;
    const double t547 = a[178];
    const double t548 = t8*t547;
    const double t549 = a[96];
    const double t551 = (t544+t546+t548+t549)*t104;
    const double t552 = a[182];
    const double t553 = t355*t552;
    const double t554 = a[120];
    const double t555 = t104*t554;
    const double t556 = a[170];
    const double t557 = t48*t556;
    const double t558 = a[116];
    const double t559 = t8*t558;
    const double t560 = a[75];
    const double t562 = (t553+t555+t557+t559+t560)*t355;
    const double t564 = (t530+t535+t542+t551+t562)*t355;
    const double t566 = (t482+t490+t505+t529+t564)*t355;
    const double t568 = (t418+t429+t455+t481+t566)*t355;
    const double t569 = t48*t299;
    const double t571 = (t569+t303+t304)*t48;
    const double t573 = (t288+t293+t571)*t48;
    const double t575 = (t269+t277+t573)*t48;
    const double t577 = (t302+t296+t291)*t48;
    const double t579 = (t270+t282+t577)*t48;
    const double t581 = (t294+t279+t273)*t48;
    const double t582 = t104*t241;
    const double t584 = (t582+t283+t252+t243)*t104;
    const double t586 = (t240+t261+t581+t584)*t104;
    const double t588 = (t239+t257+t579+t586)*t104;
    const double t589 = t48*t519;
    const double t591 = (t589+t524+t525)*t48;
    const double t593 = (t506+t511+t591)*t48;
    const double t595 = (t522+t515+t516)*t48;
    const double t596 = t104*t497;
    const double t598 = (t596+t513+t500+t501)*t104;
    const double t600 = (t491+t496+t595+t598)*t104;
    const double t601 = a[27];
    const double t602 = a[123];
    const double t603 = t8*t602;
    const double t604 = a[76];
    const double t606 = (t603+t604)*t8;
    const double t607 = a[128];
    const double t608 = t48*t607;
    const double t609 = a[144];
    const double t610 = t8*t609;
    const double t611 = a[55];
    const double t613 = (t608+t610+t611)*t48;
    const double t614 = t104*t607;
    const double t615 = a[102];
    const double t616 = t48*t615;
    const double t618 = (t614+t616+t610+t611)*t104;
    const double t619 = a[142];
    const double t620 = t355*t619;
    const double t621 = a[187];
    const double t622 = t104*t621;
    const double t623 = a[188];
    const double t624 = t48*t623;
    const double t625 = a[146];
    const double t626 = t8*t625;
    const double t627 = a[83];
    const double t629 = (t620+t622+t624+t626+t627)*t355;
    const double t631 = (t601+t606+t613+t618+t629)*t355;
    const double t633 = (t482+t490+t593+t600+t631)*t355;
    const double t634 = t48*t341;
    const double t636 = (t634+t345+t346)*t48;
    const double t638 = (t330+t335+t636)*t48;
    const double t640 = (t344+t338+t333)*t48;
    const double t641 = t104*t313;
    const double t643 = (t641+t336+t321+t315)*t104;
    const double t645 = (t312+t324+t640+t643)*t104;
    const double t646 = t48*t543;
    const double t648 = (t646+t548+t549)*t48;
    const double t649 = t104*t536;
    const double t651 = (t649+t546+t539+t540)*t104;
    const double t652 = a[148];
    const double t653 = t355*t652;
    const double t654 = t104*t623;
    const double t655 = t48*t621;
    const double t657 = (t653+t654+t655+t626+t627)*t355;
    const double t659 = (t530+t535+t648+t651+t657)*t355;
    const double t660 = t48*t362;
    const double t662 = (t660+t366+t367)*t48;
    const double t663 = t104*t352;
    const double t665 = (t663+t365+t359+t354)*t104;
    const double t666 = t104*t556;
    const double t667 = t48*t554;
    const double t669 = (t620+t666+t667+t559+t560)*t355;
    const double t658 = x[1];
    const double t670 = t658*t370;
    const double t671 = t104*t374;
    const double t672 = t48*t372;
    const double t674 = (t670+t553+t671+t672+t376+t377)*t658;
    const double t676 = (t351+t356+t662+t665+t669+t674)*t658;
    const double t678 = (t311+t319+t638+t645+t659+t676)*t658;
    const double t680 = (t238+t249+t575+t588+t633+t678)*t658;
    const double t683 = t8*t224;
    const double t685 = (t683+t229)*t8;
    const double t687 = (t213+t685)*t8;
    const double t689 = (t194+t687)*t8;
    const double t691 = (t163+t689)*t8;
    const double t693 = (t228+t216)*t8;
    const double t695 = (t195+t693)*t8;
    const double t697 = (t164+t695)*t8;
    const double t699 = (t215+t198)*t8;
    const double t701 = (t165+t699)*t8;
    const double t703 = (t197+t168)*t8;
    const double t705 = (t154+t167+t125)*t48;
    const double t707 = (t122+t703+t705)*t48;
    const double t709 = (t121+t701+t707)*t48;
    const double t711 = (t120+t697+t709)*t48;
    const double t713 = (t221+t205)*t8;
    const double t715 = (t175+t713)*t8;
    const double t717 = (t204+t178)*t8;
    const double t718 = t48*t136;
    const double t720 = (t718+t177+t138)*t48;
    const double t722 = (t135+t717+t720)*t48;
    const double t724 = (t134+t715+t722)*t48;
    const double t725 = t48*t145;
    const double t727 = (t725+t184+t147)*t48;
    const double t729 = (t135+t717+t727)*t48;
    const double t731 = (t725+t177+t138)*t48;
    const double t733 = (t409+t718+t167+t125)*t104;
    const double t735 = (t122+t703+t731+t733)*t104;
    const double t737 = (t121+t701+t729+t735)*t104;
    const double t739 = (t120+t697+t724+t737)*t104;
    const double t740 = t8*t445;
    const double t742 = (t740+t449)*t8;
    const double t744 = (t439+t742)*t8;
    const double t746 = (t430+t744)*t8;
    const double t748 = (t448+t442)*t8;
    const double t750 = (t431+t748)*t8;
    const double t752 = (t441+t434)*t8;
    const double t753 = t48*t421;
    const double t755 = (t753+t433+t423)*t48;
    const double t757 = (t420+t752+t755)*t48;
    const double t759 = (t419+t750+t757)*t48;
    const double t760 = t8*t462;
    const double t762 = (t760+t466)*t8;
    const double t764 = (t456+t762)*t8;
    const double t766 = (t465+t459)*t8;
    const double t767 = t48*t432;
    const double t769 = (t767+t458+t434)*t48;
    const double t771 = (t431+t766+t769)*t48;
    const double t772 = t8*t471;
    const double t774 = (t772+t466)*t8;
    const double t775 = t48*t440;
    const double t777 = (t775+t465+t442)*t48;
    const double t778 = t48*t447;
    const double t780 = (t475+t778+t760+t449)*t104;
    const double t782 = (t439+t774+t777+t780)*t104;
    const double t784 = (t430+t764+t771+t782)*t104;
    const double t785 = t8*t497;
    const double t787 = (t785+t501)*t8;
    const double t789 = (t491+t787)*t8;
    const double t791 = (t500+t494)*t8;
    const double t792 = t48*t484;
    const double t794 = (t792+t493+t486)*t48;
    const double t796 = (t483+t791+t794)*t48;
    const double t797 = t8*t512;
    const double t799 = (t797+t516)*t8;
    const double t800 = t48*t507;
    const double t802 = (t800+t515+t509)*t48;
    const double t803 = t48*t523;
    const double t804 = t8*t521;
    const double t806 = (t520+t803+t804+t525)*t104;
    const double t808 = (t506+t799+t802+t806)*t104;
    const double t809 = t8*t536;
    const double t811 = (t809+t540)*t8;
    const double t812 = t48*t531;
    const double t814 = (t812+t539+t533)*t48;
    const double t815 = t48*t547;
    const double t816 = t8*t545;
    const double t818 = (t544+t815+t816+t549)*t104;
    const double t819 = t48*t558;
    const double t820 = t8*t556;
    const double t822 = (t553+t555+t819+t820+t560)*t355;
    const double t824 = (t530+t811+t814+t818+t822)*t355;
    const double t826 = (t482+t789+t796+t808+t824)*t355;
    const double t828 = (t418+t746+t759+t784+t826)*t355;
    const double t830 = (t446+t760+t449)*t48;
    const double t832 = (t439+t774+t830)*t48;
    const double t834 = (t430+t764+t832)*t48;
    const double t836 = (t778+t465+t442)*t48;
    const double t838 = (t431+t766+t836)*t48;
    const double t840 = (t775+t458+t434)*t48;
    const double t841 = t104*t421;
    const double t843 = (t841+t767+t433+t423)*t104;
    const double t845 = (t420+t752+t840+t843)*t104;
    const double t847 = (t419+t750+t838+t845)*t104;
    const double t848 = a[14];
    const double t849 = a[23];
    const double t850 = a[113];
    const double t851 = t8*t850;
    const double t852 = a[69];
    const double t854 = (t851+t852)*t8;
    const double t856 = (t849+t854)*t8;
    const double t857 = a[143];
    const double t858 = t8*t857;
    const double t859 = a[52];
    const double t861 = (t858+t859)*t8;
    const double t862 = t48*t850;
    const double t864 = (t862+t858+t852)*t48;
    const double t866 = (t849+t861+t864)*t48;
    const double t867 = t48*t857;
    const double t868 = a[107];
    const double t869 = t8*t868;
    const double t871 = (t867+t869+t859)*t48;
    const double t872 = t104*t850;
    const double t874 = (t872+t867+t858+t852)*t104;
    const double t876 = (t849+t861+t871+t874)*t104;
    const double t877 = a[34];
    const double t878 = a[190];
    const double t879 = t8*t878;
    const double t880 = a[64];
    const double t882 = (t879+t880)*t8;
    const double t883 = t48*t878;
    const double t884 = a[171];
    const double t885 = t8*t884;
    const double t887 = (t883+t885+t880)*t48;
    const double t888 = a[114];
    const double t889 = t104*t888;
    const double t890 = a[183];
    const double t891 = t48*t890;
    const double t892 = t8*t890;
    const double t893 = a[95];
    const double t895 = (t889+t891+t892+t893)*t104;
    const double t896 = a[169];
    const double t897 = t355*t896;
    const double t898 = a[127];
    const double t899 = t104*t898;
    const double t900 = a[167];
    const double t901 = t48*t900;
    const double t902 = t8*t900;
    const double t903 = a[61];
    const double t905 = (t897+t899+t901+t902+t903)*t355;
    const double t907 = (t877+t882+t887+t895+t905)*t355;
    const double t909 = (t848+t856+t866+t876+t907)*t355;
    const double t911 = (t589+t804+t525)*t48;
    const double t913 = (t506+t799+t911)*t48;
    const double t915 = (t803+t515+t509)*t48;
    const double t916 = t104*t484;
    const double t918 = (t916+t800+t493+t486)*t104;
    const double t920 = (t483+t791+t915+t918)*t104;
    const double t921 = t48*t888;
    const double t923 = (t921+t892+t893)*t48;
    const double t924 = t104*t878;
    const double t926 = (t924+t891+t885+t880)*t104;
    const double t927 = a[179];
    const double t928 = t355*t927;
    const double t929 = a[177];
    const double t930 = t104*t929;
    const double t931 = t48*t929;
    const double t932 = a[174];
    const double t933 = t8*t932;
    const double t934 = a[91];
    const double t936 = (t928+t930+t931+t933+t934)*t355;
    const double t938 = (t877+t882+t923+t926+t936)*t355;
    const double t940 = (t646+t816+t549)*t48;
    const double t941 = t104*t531;
    const double t943 = (t941+t815+t539+t533)*t104;
    const double t944 = t104*t900;
    const double t945 = t48*t898;
    const double t947 = (t928+t944+t945+t902+t903)*t355;
    const double t948 = t658*t552;
    const double t949 = t104*t558;
    const double t951 = (t948+t897+t949+t667+t820+t560)*t658;
    const double t953 = (t530+t811+t940+t943+t947+t951)*t658;
    const double t955 = (t482+t789+t913+t920+t938+t953)*t658;
    const double t957 = (t418+t746+t834+t847+t909+t955)*t658;
    const double t958 = t8*t299;
    const double t960 = (t958+t304)*t8;
    const double t962 = (t288+t960)*t8;
    const double t964 = (t269+t962)*t8;
    const double t966 = (t303+t291)*t8;
    const double t968 = (t270+t966)*t8;
    const double t970 = (t290+t273)*t8;
    const double t972 = (t262+t272+t243)*t48;
    const double t974 = (t240+t970+t972)*t48;
    const double t976 = (t239+t968+t974)*t48;
    const double t978 = (t296+t280)*t8;
    const double t979 = t48*t251;
    const double t981 = (t979+t279+t253)*t48;
    const double t983 = (t250+t978+t981)*t48;
    const double t984 = t48*t258;
    const double t986 = (t984+t279+t253)*t48;
    const double t988 = (t582+t979+t272+t243)*t104;
    const double t990 = (t240+t970+t986+t988)*t104;
    const double t992 = (t239+t968+t983+t990)*t104;
    const double t993 = t8*t519;
    const double t995 = (t993+t525)*t8;
    const double t997 = (t506+t995)*t8;
    const double t999 = (t524+t509)*t8;
    const double t1001 = (t792+t508+t486)*t48;
    const double t1003 = (t483+t999+t1001)*t48;
    const double t1005 = (t804+t516)*t8;
    const double t1006 = t48*t492;
    const double t1008 = (t1006+t515+t494)*t48;
    const double t1009 = t48*t499;
    const double t1011 = (t596+t1009+t797+t501)*t104;
    const double t1013 = (t491+t1005+t1008+t1011)*t104;
    const double t1014 = t8*t607;
    const double t1016 = (t1014+t611)*t8;
    const double t1017 = t48*t602;
    const double t1019 = (t1017+t610+t604)*t48;
    const double t1020 = t48*t609;
    const double t1021 = t8*t615;
    const double t1023 = (t614+t1020+t1021+t611)*t104;
    const double t1024 = t48*t625;
    const double t1025 = t8*t623;
    const double t1027 = (t620+t622+t1024+t1025+t627)*t355;
    const double t1029 = (t601+t1016+t1019+t1023+t1027)*t355;
    const double t1031 = (t482+t997+t1003+t1013+t1029)*t355;
    const double t1033 = (t498+t797+t501)*t48;
    const double t1035 = (t491+t1005+t1033)*t48;
    const double t1037 = (t1009+t515+t494)*t48;
    const double t1039 = (t916+t1006+t508+t486)*t104;
    const double t1041 = (t483+t999+t1037+t1039)*t104;
    const double t1042 = t8*t888;
    const double t1044 = (t1042+t893)*t8;
    const double t1046 = (t883+t892+t880)*t48;
    const double t1047 = t48*t884;
    const double t1049 = (t924+t1047+t892+t880)*t104;
    const double t1050 = t48*t932;
    const double t1051 = t8*t929;
    const double t1053 = (t928+t930+t1050+t1051+t934)*t355;
    const double t1055 = (t877+t1044+t1046+t1049+t1053)*t355;
    const double t1057 = (t608+t1021+t611)*t48;
    const double t1058 = t104*t602;
    const double t1060 = (t1058+t1020+t610+t604)*t104;
    const double t1062 = t355*a[134];
    const double t1063 = t104*t932;
    const double t1065 = (t1062+t1063+t931+t1051+t934)*t355;
    const double t1066 = t658*t619;
    const double t1067 = t104*t625;
    const double t1069 = (t1066+t928+t1067+t655+t1025+t627)*t658;
    const double t1071 = (t601+t1016+t1057+t1060+t1065+t1069)*t658;
    const double t1073 = (t482+t997+t1035+t1041+t1055+t1071)*t658;
    const double t1074 = t8*t341;
    const double t1076 = (t1074+t346)*t8;
    const double t1078 = (t330+t1076)*t8;
    const double t1080 = (t345+t333)*t8;
    const double t1082 = (t325+t332+t315)*t48;
    const double t1084 = (t312+t1080+t1082)*t48;
    const double t1085 = t48*t320;
    const double t1087 = (t1085+t338+t322)*t48;
    const double t1089 = (t641+t1085+t332+t315)*t104;
    const double t1091 = (t312+t1080+t1087+t1089)*t104;
    const double t1092 = t8*t543;
    const double t1094 = (t1092+t549)*t8;
    const double t1096 = (t812+t548+t533)*t48;
    const double t1097 = t48*t538;
    const double t1099 = (t649+t1097+t816+t540)*t104;
    const double t1100 = t8*t621;
    const double t1102 = (t653+t654+t1024+t1100+t627)*t355;
    const double t1104 = (t530+t1094+t1096+t1099+t1102)*t355;
    const double t1106 = (t537+t816+t540)*t48;
    const double t1108 = (t941+t1097+t548+t533)*t104;
    const double t1109 = t8*t898;
    const double t1111 = (t928+t944+t901+t1109+t903)*t355;
    const double t1112 = t658*t652;
    const double t1114 = (t1112+t928+t1067+t624+t1100+t627)*t658;
    const double t1116 = (t530+t1094+t1106+t1108+t1111+t1114)*t658;
    const double t1117 = t8*t362;
    const double t1119 = (t1117+t367)*t8;
    const double t1121 = (t357+t366+t354)*t48;
    const double t1122 = t48*t358;
    const double t1124 = (t663+t1122+t366+t354)*t104;
    const double t1125 = t8*t554;
    const double t1127 = (t620+t666+t819+t1125+t560)*t355;
    const double t1129 = (t1066+t897+t949+t557+t1125+t560)*t658;
    const double t1118 = x[0];
    const double t1130 = t1118*t370;
    const double t1131 = t8*t372;
    const double t1133 = (t1130+t948+t553+t671+t375+t1131+t377)*t1118;
    const double t1135 = (t351+t1119+t1121+t1124+t1127+t1129+t1133)*t1118;
    const double t1137 = (t311+t1078+t1084+t1091+t1104+t1116+t1135)*t1118;
    const double t1139 = (t238+t964+t976+t992+t1031+t1073+t1137)*t1118;
    const double t1169 = 2.0*t1066;
    const double t1177 = t1118*t552;
    const double t1196 = 2.0*t553;
    const double t1203 = 2.0*t620;
    const double t1208 = 2.0*t653;
    const double t1230 = 2.0*t928;
    const double t1233 = t658*t896;
    const double t1246 = t658*t927;
    const double t1299 = 2.0*t409;
    const double t1306 = 2.0*t475;
    const double t1311 = 2.0*t520;
    const double t1314 = t355*t554;
    const double t1315 = 2.0*t544;
    const double t1322 = 2.0*t582;
    const double t1327 = 2.0*t596;
    const double t1330 = t355*t621;
    const double t1331 = 2.0*t614;
    const double t1336 = 2.0*t641;
    const double t1339 = t355*t623;
    const double t1340 = 2.0*t649;
    const double t1343 = t658*t374;
    const double t1344 = t355*t556;
    const double t1345 = 2.0*t663;
    const double t1380 = t355*t898;
    const double t1386 = 2.0*t916;
    const double t1389 = t355*t929;
    const double t1390 = 2.0*t924;
    const double t1393 = t658*t558;
    const double t1394 = t355*t900;
    const double t1395 = 2.0*t941;
    const double t1416 = t658*t625;
    const double t1417 = t355*t932;
    const double t1429 = t1118*t374;
    const double t1455 = 2.0*t94;
    const double t1463 = t104*t21;
    const double t1472 = 2.0*t154;
    const double t1484 = 2.0*t208;
    const double t1487 = t104*t226;
    const double t1488 = 2.0*t219;
    const double t1495 = 2.0*t262;
    const double t1503 = t104*t301;
    const double t1504 = 2.0*t294;
    const double t1509 = 2.0*t325;
    const double t1512 = t104*t343;
    const double t1516 = t355*t374;
    const double t1517 = t104*t364;
    const double t1518 = 2.0*t357;
    const double t1541 = t104*t166;
    const double t1548 = 2.0*t446;
    const double t1556 = t104*t462;
    const double t1562 = 2.0*t498;
    const double t1565 = t104*t521;
    const double t1569 = t104*t545;
    const double t1570 = 2.0*t537;
    const double t1585 = t104*t271;
    const double t1590 = 2.0*t589;
    const double t1593 = t104*t512;
    const double t1597 = t104*t615;
    const double t1598 = 2.0*t608;
    const double t1606 = t104*t331;
    const double t1610 = 2.0*t646;
    const double t1634 = 2.0*t725;
    const double t1637 = t104*t136;
    const double t1652 = t104*t447;
    const double t1653 = 2.0*t775;
    const double t1658 = 2.0*t792;
    const double t1661 = t104*t523;
    const double t1665 = t355*t558;
    const double t1666 = t104*t547;
    const double t1667 = 2.0*t812;
    const double t1681 = t104*t432;
    const double t1689 = t104*t857;
    const double t1693 = t104*t890;
    const double t1694 = 2.0*t883;
    const double t1701 = t104*t507;
    const double t1708 = t658*t554;
    const double t1722 = t104*t251;
    const double t1730 = t104*t499;
    const double t1734 = t355*t625;
    const double t1735 = t104*t609;
    const double t1743 = t104*t492;
    const double t1747 = t104*t884;
    const double t1750 = t658*t621;
    const double t1757 = t104*t320;
    const double t1761 = t104*t538;
    const double t1764 = t658*t623;
    const double t1767 = t658*t556;
    const double t1768 = t104*t358;
    const double t1793 = (((2.0*t22+t23)*t8+t20+t25)*t8+t19+t27)*t8;
    const double t1794 = 2.0*t34;
    const double t1798 = ((t1794+t35)*t8+t32+t37)*t8;
    const double t1801 = (2.0*t43+t35)*t8;
    const double t1815 = 2.0*t73;
    const double t1817 = (t1815+t74)*t8;
    const double t1818 = t48*t63;
    const double t1825 = t48*t72;
    const double t1847 = (((2.0*t124+t125)*t8+t122+t127)*t8+t121+t129)*t8;
    const double t1852 = ((2.0*t137+t138)*t8+t135+t140)*t8;
    const double t1853 = 2.0*t146;
    const double t1855 = (t1853+t147)*t8;
    const double t1866 = ((2.0*t167+t168)*t8+t165+t170)*t8;
    const double t1869 = (2.0*t177+t178)*t8;
    const double t1870 = t48*t176;
    const double t1871 = 2.0*t184;
    const double t1876 = 2.0*t197;
    const double t1878 = (t1876+t198)*t8;
    const double t1879 = t48*t203;
    const double t1880 = 2.0*t204;
    const double t1883 = t48*t220;
    const double t1884 = 2.0*t215;
    const double t1895 = ((2.0*t242+t243)*t8+t240+t245)*t8;
    const double t1898 = (2.0*t252+t253)*t8;
    const double t1899 = 2.0*t259;
    const double t1906 = (2.0*t272+t273)*t8;
    const double t1907 = t48*t278;
    const double t1908 = 2.0*t279;
    const double t1911 = t48*t295;
    const double t1912 = 2.0*t290;
    const double t1919 = (2.0*t314+t315)*t8;
    const double t1920 = 2.0*t321;
    const double t1923 = t48*t337;
    const double t1924 = 2.0*t332;
    const double t1927 = 2.0*t353;
    const double t1961 = (2.0*t433+t434)*t8;
    const double t1962 = 2.0*t441;
    const double t1967 = t48*t464;
    const double t1977 = (2.0*t485+t486)*t8;
    const double t1978 = 2.0*t493;
    const double t1981 = t48*t514;
    const double t1982 = 2.0*t508;
    const double t1985 = 2.0*t532;
    const double t2036 = ((2.0*t228+t216)*t8+t195+t693)*t8;
    const double t2038 = (t1884+t198)*t8;
    const double t2065 = ((2.0*t740+t449)*t8+t439+t742)*t8;
    const double t2068 = (2.0*t448+t442)*t8;
    const double t2075 = (2.0*t760+t466)*t8;
    const double t2076 = t48*t457;
    const double t2077 = 2.0*t465;
    const double t2080 = 2.0*t772;
    const double t2087 = (2.0*t785+t501)*t8;
    const double t2088 = 2.0*t500;
    const double t2091 = 2.0*t797;
    const double t2094 = 2.0*t809;
    const double t2114 = 2.0*t858;
    const double t2120 = 2.0*t879;
    const double t2144 = (2.0*t303+t291)*t8;
    const double t2158 = (2.0*t993+t525)*t8;
    const double t2159 = 2.0*t524;
    const double t2162 = 2.0*t804;
    const double t2165 = 2.0*t1014;
    const double t2184 = 2.0*t345;
    const double t2189 = 2.0*t1092;
    g[0] = ((((2.0*t1130+t948+t553+t671+t375+t1131+t377)*t1118+t351+
t1119+t1121+t1124+t1127+t1129+t1133)*t1118+t311+t1078+t1084+t1091+t1104+t1116+
t1135)*t1118+t238+t964+t976+t992+t1031+t1073+t1137)*t1118+t119+t691+t711+t739+
t828+t957+t1139;
    g[1] = ((((2.0*t670+t553+t671+t672+t376+t377)*t658+t351+t356+t662+
t665+t669+t674)*t658+t311+t319+t638+t645+t659+t676)*t658+t238+t249+t575+t588+
t633+t678)*t658+t119+t133+t396+t417+t568+t680+((((2.0*t948+t897+t949+t667+t820+
t560)*t658+t530+t811+t940+t943+t947+t951)*t658+t482+t789+t913+t920+t938+t953)*
t658+t418+t746+t834+t847+t909+t955+(((t1169+t928+t1067+t655+t1025+t627)*t658+
t601+t1016+t1057+t1060+t1065+t1069)*t658+t482+t997+t1035+t1041+t1055+t1071+((
2.0*t1112+t928+t1067+t624+t1100+t627)*t658+t530+t1094+t1106+t1108+t1111+t1114+(
t1177+t1169+t897+t949+t557+t1125+t560)*t1118)*t1118)*t1118)*t1118;
    g[2] = ((((2.0*t371+t373+t375+t376+t377)*t355+t351+t356+t361+t369+
t379)*t355+t311+t319+t329+t350+t381)*t355+t238+t249+t268+t310+t383)*t355+t119+
t133+t162+t237+t385+((((t1196+t555+t557+t559+t560)*t355+t530+t535+t542+t551+
t562)*t355+t482+t490+t505+t529+t564)*t355+t418+t429+t455+t481+t566+(((t1203+
t622+t624+t626+t627)*t355+t601+t606+t613+t618+t629)*t355+t482+t490+t593+t600+
t631+((t1208+t654+t655+t626+t627)*t355+t530+t535+t648+t651+t657+(t948+t1203+
t666+t667+t559+t560)*t658)*t658)*t658)*t658+((((t1196+t555+t819+t820+t560)*t355
+t530+t811+t814+t818+t822)*t355+t482+t789+t796+t808+t824)*t355+t418+t746+t759+
t784+t826+(((2.0*t897+t899+t901+t902+t903)*t355+t877+t882+t887+t895+t905)*t355+
t848+t856+t866+t876+t907+((t1230+t930+t931+t933+t934)*t355+t877+t882+t923+t926+
t936+(t1233+t1230+t944+t945+t902+t903)*t658)*t658)*t658+(((t1203+t622+t1024+
t1025+t627)*t355+t601+t1016+t1019+t1023+t1027)*t355+t482+t997+t1003+t1013+t1029
+((t1230+t930+t1050+t1051+t934)*t355+t877+t1044+t1046+t1049+t1053+(t1246+2.0*
t1062+t1063+t931+t1051+t934)*t658)*t658+((t1208+t654+t1024+t1100+t627)*t355+
t530+t1094+t1096+t1099+t1102+(t1246+t1230+t944+t901+t1109+t903)*t658+(t1177+
t1233+t1203+t666+t819+t1125+t560)*t1118)*t1118)*t1118)*t1118;
    g[3] = ((((2.0*t108+t81+t22+t7)*t104+t4+t49+t107+t110)*t104+t3+t47+
t105+t112)*t104+t2+t41+t100+t114)*t104+t1+t31+t89+t116+((((2.0*t225+t227+t228+
t229)*t104+t213+t218+t223+t231)*t104+t194+t202+t212+t233)*t104+t163+t174+t193+
t235+(((2.0*t300+t302+t303+t304)*t104+t288+t293+t298+t306)*t104+t269+t277+t287+
t308+((2.0*t342+t344+t345+t346)*t104+t330+t335+t340+t348+(t355*t372+2.0*t363+
t365+t366+t367)*t355)*t355)*t355)*t355+((((t1299+t187+t137+t125)*t104+t122+t153
+t408+t411)*t104+t121+t151+t406+t413)*t104+t120+t144+t402+t415+(((t1306+t463+
t448+t449)*t104+t439+t444+t474+t477)*t104+t430+t438+t470+t479+((t1311+t522+t524
+t525)*t104+t506+t511+t518+t527+(t1314+t1315+t546+t548+t549)*t355)*t355)*t355+(
((t1322+t283+t252+t243)*t104+t240+t261+t581+t584)*t104+t239+t257+t579+t586+((
t1327+t513+t500+t501)*t104+t491+t496+t595+t598+(t1330+t1331+t616+t610+t611)*
t355)*t355+((t1336+t336+t321+t315)*t104+t312+t324+t640+t643+(t1339+t1340+t546+
t539+t540)*t355+(t1343+t1344+t1345+t365+t359+t354)*t658)*t658)*t658)*t658+((((
t1299+t718+t167+t125)*t104+t122+t703+t731+t733)*t104+t121+t701+t729+t735)*t104+
t120+t697+t724+t737+(((t1306+t778+t760+t449)*t104+t439+t774+t777+t780)*t104+
t430+t764+t771+t782+((t1311+t803+t804+t525)*t104+t506+t799+t802+t806+(t1314+
t1315+t815+t816+t549)*t355)*t355)*t355+(((2.0*t841+t767+t433+t423)*t104+t420+
t752+t840+t843)*t104+t419+t750+t838+t845+((2.0*t872+t867+t858+t852)*t104+t849+
t861+t871+t874+(t1380+2.0*t889+t891+t892+t893)*t355)*t355+((t1386+t800+t493+
t486)*t104+t483+t791+t915+t918+(t1389+t1390+t891+t885+t880)*t355+(t1393+t1394+
t1395+t815+t539+t533)*t658)*t658)*t658+(((t1322+t979+t272+t243)*t104+t240+t970+
t986+t988)*t104+t239+t968+t983+t990+((t1327+t1009+t797+t501)*t104+t491+t1005+
t1008+t1011+(t1330+t1331+t1020+t1021+t611)*t355)*t355+((t1386+t1006+t508+t486)*
t104+t483+t999+t1037+t1039+(t1389+t1390+t1047+t892+t880)*t355+(t1416+t1417+2.0*
t1058+t1020+t610+t604)*t658)*t658+((t1336+t1085+t332+t315)*t104+t312+t1080+
t1087+t1089+(t1339+t1340+t1097+t816+t540)*t355+(t1416+t1394+t1395+t1097+t548+
t533)*t658+(t1429+t1393+t1344+t1345+t1122+t366+t354)*t1118)*t1118)*t1118)*t1118
;
    g[4] = ((((2.0*t50+t22+t7)*t48+t4+t49+t52)*t48+t3+t47+t54)*t48+t2+
t41+t56)*t48+t1+t31+t58+((((2.0*t81+t64+t23)*t48+t20+t80+t83)*t48+t19+t78+t85)*
t48+t18+t71+t87+(((t1455+t73+t35)*t48+t32+t93+t96)*t48+t19+t78+t98+((2.0*t101+
t73+t35)*t48+t20+t80+t103+(t1463+t1455+t64+t23)*t104)*t104)*t104)*t104+((((
t1472+t137+t125)*t48+t122+t153+t156)*t48+t121+t151+t158)*t48+t120+t144+t160+(((
2.0*t187+t177+t168)*t48+t165+t186+t189)*t48+t164+t182+t191+((t1484+t204+t198)*
t48+t195+t207+t210+(t1487+t1488+t221+t216)*t104)*t104)*t104+(((t1495+t252+t243)
*t48+t240+t261+t264)*t48+t239+t257+t266+((2.0*t283+t279+t273)*t48+t270+t282+
t285+(t1503+t1504+t296+t291)*t104)*t104+((t1509+t321+t315)*t48+t312+t324+t327+(
t1512+2.0*t336+t338+t333)*t104+(t1516+t1517+t1518+t359+t354)*t355)*t355)*t355)*
t355+((((2.0*t388+t228+t229)*t48+t213+t218+t390)*t48+t194+t202+t392)*t48+t163+
t174+t394+(((2.0*t227+t221+t216)*t48+t195+t207+t398)*t48+t164+t182+t400+((t1488
+t204+t198)*t48+t165+t186+t404+(t1541+t1484+t177+t168)*t104)*t104)*t104+(((
t1548+t448+t449)*t48+t439+t444+t451)*t48+t430+t438+t453+((2.0*t463+t465+t466)*
t48+t456+t461+t468+(t1556+2.0*t472+t465+t466)*t104)*t104+((t1562+t500+t501)*t48
+t491+t496+t503+(t1565+2.0*t513+t515+t516)*t104+(t1344+t1569+t1570+t539+t540)*
t355)*t355)*t355+(((2.0*t569+t303+t304)*t48+t288+t293+t571)*t48+t269+t277+t573+
((2.0*t302+t296+t291)*t48+t270+t282+t577+(t1585+t1504+t279+t273)*t104)*t104+((
t1590+t524+t525)*t48+t506+t511+t591+(t1593+2.0*t522+t515+t516)*t104+(t1339+
t1597+t1598+t610+t611)*t355)*t355+((2.0*t634+t345+t346)*t48+t330+t335+t636+(
t1606+2.0*t344+t338+t333)*t104+(t1330+t1569+t1610+t548+t549)*t355+(t372*t658+
t1314+t1517+t366+t367+2.0*t660)*t658)*t658)*t658)*t658+((((t1472+t167+t125)*t48
+t122+t703+t705)*t48+t121+t701+t707)*t48+t120+t697+t709+(((2.0*t718+t177+t138)*
t48+t135+t717+t720)*t48+t134+t715+t722+((t1634+t184+t147)*t48+t135+t717+t727+(
t1637+t1634+t177+t138)*t104)*t104)*t104+(((2.0*t753+t433+t423)*t48+t420+t752+
t755)*t48+t419+t750+t757+((2.0*t767+t458+t434)*t48+t431+t766+t769+(t1652+t1653+
t465+t442)*t104)*t104+((t1658+t493+t486)*t48+t483+t791+t794+(t1661+2.0*t800+
t515+t509)*t104+(t1665+t1666+t1667+t539+t533)*t355)*t355)*t355+(((t1548+t760+
t449)*t48+t439+t774+t830)*t48+t430+t764+t832+((2.0*t778+t465+t442)*t48+t431+
t766+t836+(t1681+t1653+t458+t434)*t104)*t104+((2.0*t862+t858+t852)*t48+t849+
t861+t864+(t1689+2.0*t867+t869+t859)*t104+(t1394+t1693+t1694+t885+t880)*t355)*
t355+((t1590+t804+t525)*t48+t506+t799+t911+(t1701+2.0*t803+t515+t509)*t104+(
t1389+t1693+2.0*t921+t892+t893)*t355+(t1708+t1380+t1666+t1610+t816+t549)*t658)*
t658)*t658+(((t1495+t272+t243)*t48+t240+t970+t972)*t48+t239+t968+t974+((2.0*
t979+t279+t253)*t48+t250+t978+t981+(t1722+2.0*t984+t279+t253)*t104)*t104+((
t1658+t508+t486)*t48+t483+t999+t1001+(t1730+2.0*t1006+t515+t494)*t104+(t1734+
t1735+2.0*t1017+t610+t604)*t355)*t355+((t1562+t797+t501)*t48+t491+t1005+t1033+(
t1743+2.0*t1009+t515+t494)*t104+(t1417+t1747+t1694+t892+t880)*t355+(t1750+t1389
+t1735+t1598+t1021+t611)*t658)*t658+((t1509+t332+t315)*t48+t312+t1080+t1082+(
t1757+2.0*t1085+t338+t322)*t104+(t1734+t1761+t1667+t548+t533)*t355+(t1764+t1394
+t1761+t1570+t816+t540)*t658+(t1429+t1767+t1665+t1768+t1518+t366+t354)*t1118)*
t1118)*t1118)*t1118;
    g[5] = ((((2.0*t6+t7)*t8+t4+t9)*t8+t3+t11)*t8+t2+t13)*t8+t1+t15+(
t1793+t18+t29+(t1798+t19+t39+(t1801+t20+t45+(t81+t1794+t23)*t48)*t48)*t48)*t48+
(t1793+t18+t29+(((2.0*t64+t65)*t8+t62+t67)*t8+t61+t69+(t1817+t62+t76+(t1818+
t1815+t65)*t48)*t48)*t48+(t1798+t19+t39+(t1817+t62+t76+(t1825+2.0*t91+t74)*t48)
*t48+(t1801+t20+t45+(t1825+t1815+t65)*t48+(t1463+t1818+t1794+t23)*t104)*t104)*
t104)*t104+(t1847+t120+t131+(t1852+t134+t142+(t1855+t135+t149+(t718+t1853+t138)
*t48)*t48)*t48+(t1866+t164+t172+(t1869+t175+t180+(t1870+t1871+t178)*t48)*t48+(
t1878+t195+t200+(t1879+t1880+t205)*t48+(t1487+t1883+t1884+t216)*t104)*t104)*
t104+(t1895+t239+t247+(t1898+t250+t255+(t979+t1899+t253)*t48)*t48+(t1906+t270+
t275+(t1907+t1908+t280)*t48+(t1503+t1911+t1912+t291)*t104)*t104+(t1919+t312+
t317+(t1085+t1920+t322)*t48+(t1512+t1923+t1924+t333)*t104+(t1516+t1517+t1122+
t1927+t354)*t355)*t355)*t355)*t355+(t1847+t120+t131+(t1866+t164+t172+(t1878+
t195+t200+(t227+t1884+t216)*t48)*t48)*t48+(t1852+t134+t142+(t1869+t175+t180+(
t1883+t1880+t205)*t48)*t48+(t1855+t135+t149+(t1879+t1871+t178)*t48+(t1637+t1870
+t1853+t138)*t104)*t104)*t104+(((2.0*t422+t423)*t8+t420+t425)*t8+t419+t427+(
t1961+t431+t436+(t778+t1962+t442)*t48)*t48+(t1961+t431+t436+(t1967+2.0*t458+
t459)*t48+(t1652+t1967+t1962+t442)*t104)*t104+(t1977+t483+t488+(t1009+t1978+
t494)*t48+(t1661+t1981+t1982+t509)*t104+(t1665+t1666+t1097+t1985+t533)*t355)*
t355)*t355+(t1895+t239+t247+(t1906+t270+t275+(t302+t1912+t291)*t48)*t48+(t1898+
t250+t255+(t1911+t1908+t280)*t48+(t1722+t1907+t1899+t253)*t104)*t104+(t1977+
t483+t488+(t803+t1982+t509)*t48+(t1730+t1981+t1978+t494)*t104+(t1734+t1735+
t1020+2.0*t603+t604)*t355)*t355+(t1919+t312+t317+(t344+t1924+t333)*t48+(t1757+
t1923+t1920+t322)*t104+(t1734+t1761+t815+t1985+t533)*t355+(t1343+t1665+t1768+
t365+t1927+t354)*t658)*t658)*t658)*t658+((((2.0*t683+t229)*t8+t213+t685)*t8+
t194+t687)*t8+t163+t689+(t2036+t164+t695+(t2038+t165+t699+(t187+t1876+t168)*t48
)*t48)*t48+(t2036+t164+t695+((2.0*t221+t205)*t8+t175+t713+(t1870+t1880+t178)*
t48)*t48+(t2038+t165+t699+(t183*t48+t178+t1880)*t48+(t1541+t1870+t1876+t168)*
t104)*t104)*t104+(t2065+t430+t744+(t2068+t431+t748+(t767+t1962+t434)*t48)*t48+(
t2075+t456+t762+(t2076+t2077+t459)*t48+(t1556+t1967+t2080+t466)*t104)*t104+(
t2087+t491+t787+(t1006+t2088+t494)*t48+(t1565+t1981+t2091+t516)*t104+(t1344+
t1569+t1097+t2094+t540)*t355)*t355)*t355+(t2065+t430+t744+(t2075+t456+t762+(
t463+t2080+t466)*t48)*t48+(t2068+t431+t748+(t1967+t2077+t459)*t48+(t1681+t2076+
t1962+t434)*t104)*t104+((2.0*t851+t852)*t8+t849+t854+(t867+t2114+t859)*t48+(t48
*t868+t1689+t2114+t859)*t104+(t1394+t1693+t1047+t2120+t880)*t355)*t355+(t2087+
t491+t787+(t522+t2091+t516)*t48+(t1743+t1981+t2088+t494)*t104+(t1417+t1747+t891
+t2120+t880)*t355+(t1767+t1394+t1761+t546+t2094+t540)*t658)*t658)*t658+(((2.0*
t958+t304)*t8+t288+t960)*t8+t269+t962+(t2144+t270+t966+(t283+t1912+t273)*t48)*
t48+(t2144+t270+t966+(t1907+2.0*t296+t280)*t48+(t1585+t1907+t1912+t273)*t104)*
t104+(t2158+t506+t995+(t800+t2159+t509)*t48+(t1593+t1981+t2162+t516)*t104+(
t1339+t1597+t1020+t2165+t611)*t355)*t355+(t2158+t506+t995+(t513+t2162+t516)*t48
+(t1701+t1981+t2159+t509)*t104+(t1389+t1693+t891+2.0*t1042+t893)*t355+(t1764+
t1389+t1735+t616+t2165+t611)*t658)*t658+((2.0*t1074+t346)*t8+t330+t1076+(t336+
t2184+t333)*t48+(t1606+t1923+t2184+t333)*t104+(t1330+t1569+t815+t2189+t549)*
t355+(t1750+t1380+t1666+t546+t2189+t549)*t658+(t1118*t372+2.0*t1117+t1314+t1517
+t1708+t365+t367)*t1118)*t1118)*t1118)*t1118;
    return (t1+t15)*t8+(t1+t31+t58)*t48+(t1+t31+t89+t116)*t104+(t119+
t133+t162+t237+t385)*t355+(t119+t133+t396+t417+t568+t680)*t658+(t119+t691+t711+
t739+t828+t957+t1139)*t1118;
}

} // namespace mbnrg_A1B3_deg6

