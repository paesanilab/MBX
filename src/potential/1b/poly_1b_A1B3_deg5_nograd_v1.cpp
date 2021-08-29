#include "poly_1b_A1B3_deg5_v1.h"

namespace mbnrg_A1B3_deg5 {

double poly_A1B3_deg5_v1::eval(const double x[6], const double a[102]) {
    const double t1 = a[1];
    const double t2 = a[3];
    const double t3 = a[18];
    const double t4 = a[100];
    const double t6 = a[43];
    const double t15 = a[5];
    const double t16 = a[11];
    const double t17 = a[84];
    const double t5 = x[5];
    const double t18 = t5 * t17;
    const double t19 = a[25];
    const double t25 = (t15 + (t16 + (t18 + t19) * t5) * t5) * t5;
    const double t26 = a[53];
    const double t27 = t5 * t26;
    const double t28 = a[26];
    const double t32 = (t16 + (t27 + t28) * t5) * t5;
    const double t34 = (t27 + t19) * t5;
    const double t46 = t5 * a[73];
    const double t47 = a[35];
    const double t53 = t5 * a[77];
    const double t55 = (t53 + t47) * t5;
    const double t24 = x[4];
    const double t56 = t24 * t17;
    const double t63 = t24 * t26;
    const double t79 = a[0];
    const double t80 = a[7];
    const double t81 = a[9];
    const double t82 = a[56];
    const double t84 = a[41];
    const double t90 = (t80 + (t81 + (t82 * t5 + t84) * t5) * t5) * t5;
    const double t91 = a[15];
    const double t92 = a[88];
    const double t93 = t5 * t92;
    const double t94 = a[47];
    const double t98 = (t91 + (t93 + t94) * t5) * t5;
    const double t99 = a[74];
    const double t102 = (t99 * t5 + t94) * t5;
    const double t103 = t24 * t82;
    const double t110 = a[2];
    const double t111 = a[19];
    const double t112 = a[89];
    const double t113 = t5 * t112;
    const double t114 = a[45];
    const double t118 = (t111 + (t113 + t114) * t5) * t5;
    const double t120 = t5 * a[94];
    const double t121 = a[30];
    const double t123 = (t120 + t121) * t5;
    const double t124 = t24 * t112;
    const double t129 = a[12];
    const double t130 = a[59];
    const double t131 = t5 * t130;
    const double t132 = a[46];
    const double t134 = (t131 + t132) * t5;
    const double t135 = t24 * t130;
    const double t137 = t5 * a[67];
    const double t140 = a[57];
    const double t142 = a[62];
    const double t143 = t24 * t142;
    const double t144 = t5 * t142;
    const double t145 = a[22];
    const double t152 = a[4];
    const double t153 = a[8];
    const double t154 = a[90];
    const double t156 = a[39];
    const double t160 = (t153 + (t154 * t5 + t156) * t5) * t5;
    const double t161 = a[98];
    const double t162 = t5 * t161;
    const double t163 = a[33];
    const double t165 = (t162 + t163) * t5;
    const double t166 = t24 * t154;
    const double t171 = a[10];
    const double t172 = a[68];
    const double t173 = t5 * t172;
    const double t174 = a[36];
    const double t176 = (t173 + t174) * t5;
    const double t177 = t24 * t172;
    const double t179 = t5 * a[50];
    const double t182 = a[85];
    const double t184 = a[81];
    const double t185 = t24 * t184;
    const double t186 = t5 * t184;
    const double t187 = a[42];
    const double t192 = a[13];
    const double t193 = a[51];
    const double t195 = a[24];
    const double t197 = (t193 * t5 + t195) * t5;
    const double t198 = t24 * t193;
    const double t199 = a[99];
    const double t200 = t5 * t199;
    const double t203 = a[65];
    const double t205 = a[72];
    const double t206 = t24 * t205;
    const double t207 = t5 * t205;
    const double t208 = a[40];
    const double t211 = a[101];
    const double t213 = a[97];
    const double t215 = a[79];
    const double t216 = t24 * t215;
    const double t217 = t5 * t215;
    const double t218 = a[38];
    const double t64 = x[3];
    const double t240 = t64 * t82;
    const double t247 = a[6];
    const double t248 = a[17];
    const double t249 = a[86];
    const double t251 = a[44];
    const double t256 = a[20];
    const double t257 = a[92];
    const double t258 = t5 * t257;
    const double t259 = a[37];
    const double t261 = (t258 + t259) * t5;
    const double t262 = a[58];
    const double t263 = t24 * t262;
    const double t264 = a[80];
    const double t265 = t5 * t264;
    const double t266 = a[48];
    const double t271 = a[78];
    const double t272 = t24 * t271;
    const double t274 = t5 * a[63];
    const double t275 = a[32];
    const double t278 = t64 * t262;
    const double t283 = a[16];
    const double t284 = a[60];
    const double t286 = a[31];
    const double t288 = (t284 * t5 + t286) * t5;
    const double t289 = a[66];
    const double t290 = t24 * t289;
    const double t291 = a[70];
    const double t292 = t5 * t291;
    const double t293 = a[28];
    const double t296 = a[71];
    const double t297 = t64 * t296;
    const double t298 = a[82];
    const double t299 = t24 * t298;
    const double t300 = a[93];
    const double t301 = t5 * t300;
    const double t302 = a[29];
    const double t305 = a[75];
    const double t69 = x[2];
    const double t306 = t305 * t69;
    const double t307 = a[96];
    const double t308 = t64 * t307;
    const double t309 = a[54];
    const double t310 = t24 * t309;
    const double t311 = a[87];
    const double t312 = t5 * t311;
    const double t313 = a[49];
    const double t327 = t64 * t154;
    const double t332 = t24 * t296;
    const double t335 = t64 * t289;
    const double t338 = a[83];
    const double t339 = t69 * t338;
    const double t340 = a[55];
    const double t341 = t64 * t340;
    const double t342 = t24 * t340;
    const double t343 = a[76];
    const double t345 = a[27];
    const double t353 = t64 * t193;
    const double t356 = t64 * t309;
    const double t357 = t24 * t307;
    const double t361 = t64 * t215;
    const double t381 = (t111 + (t144 + t132) * t5) * t5;
    const double t383 = (t131 + t114) * t5;
    const double t392 = t24 * t92;
    const double t410 = (t256 + (t262 * t5 + t266) * t5) * t5;
    const double t412 = (t265 + t259) * t5;
    const double t418 = t5 * t271;
    const double t420 = (t418 + t275) * t5;
    const double t421 = t24 * t257;
    const double t424 = t24 * t264;
    const double t431 = (t289 * t5 + t293) * t5;
    const double t432 = t24 * t284;
    const double t435 = t24 * t300;
    const double t436 = t5 * t298;
    const double t439 = t24 * t311;
    const double t440 = t5 * t309;
    const double t459 = a[69];
    const double t461 = a[23];
    const double t465 = a[52];
    const double t466 = t5 * t465;
    const double t474 = t69 * a[61];
    const double t475 = a[95];
    const double t477 = a[64];
    const double t478 = t24 * t477;
    const double t479 = t5 * t477;
    const double t480 = a[34];
    const double t487 = t64 * t284;
    const double t491 = a[91] * t69;
    const double t492 = t64 * t477;
    const double t89 = x[1];
    const double t496 = t89 * t305;
    const double t497 = t64 * t311;
    const double t510 = (t186 + t174) * t5;
    const double t515 = t24 * t161;
    const double t524 = (t296 * t5 + t302) * t5;
    const double t527 = t24 * t291;
    const double t531 = t5 * t340;
    const double t543 = t89 * t338;
    const double t557 = t5 * t307;
    const double t567 = x[0];
    return ((t1 + (t2 + (t3 + (t4 * t5 + t6) * t5) * t5) * t5) * t5 +
            (t1 + t25 + (t2 + t32 + (t3 + t34 + (t4 * t24 + t18 + t6) * t24) * t24) * t24) * t24 +
            (t1 + t25 + (t15 + (a[21] + (t46 + t47) * t5) * t5 + (t16 + t55 + (t56 + t46 + t19) * t24) * t24) * t24 +
             (t2 + t32 + (t16 + t55 + (t63 + t53 + t28) * t24) * t24 +
              (t3 + t34 + (t63 + t46 + t19) * t24 + (t4 * t64 + t18 + t56 + t6) * t64) * t64) *
                 t64) *
                t64 +
            (t79 + t90 + (t80 + t98 + (t81 + t102 + (t103 + t93 + t84) * t24) * t24) * t24 +
             (t110 + t118 + (t111 + t123 + (t124 + t120 + t114) * t24) * t24 +
              (t129 + t134 + (t135 + t137 + t132) * t24 + (t140 * t64 + t143 + t144 + t145) * t64) * t64) *
                 t64 +
             (t152 + t160 + (t153 + t165 + (t166 + t162 + t156) * t24) * t24 +
              (t171 + t176 + (t177 + t179 + t174) * t24 + (t182 * t64 + t185 + t186 + t187) * t64) * t64 +
              (t192 + t197 + (t198 + t200 + t195) * t24 + (t203 * t64 + t206 + t207 + t208) * t64 +
               (t211 * t69 + t213 * t64 + t216 + t217 + t218) * t69) *
                  t69) *
                 t69) *
                t69 +
            (t79 + t90 + (t110 + t118 + (t129 + t134 + (t140 * t24 + t144 + t145) * t24) * t24) * t24 +
             (t80 + t98 + (t111 + t123 + (t143 + t137 + t132) * t24) * t24 +
              (t81 + t102 + (t135 + t120 + t114) * t24 + (t240 + t124 + t93 + t84) * t64) * t64) *
                 t64 +
             (t247 + (t248 + (t249 * t5 + t251) * t5) * t5 + (t256 + t261 + (t263 + t265 + t266) * t24) * t24 +
              (t256 + t261 + (t272 + t274 + t275) * t24 + (t278 + t272 + t265 + t266) * t64) * t64 +
              (t283 + t288 + (t290 + t292 + t293) * t24 + (t297 + t299 + t301 + t302) * t64 +
               (t306 + t308 + t310 + t312 + t313) * t69) *
                  t69) *
                 t69 +
             (t152 + t160 + (t171 + t176 + (t182 * t24 + t186 + t187) * t24) * t24 +
              (t153 + t165 + (t185 + t179 + t174) * t24 + (t327 + t177 + t162 + t156) * t64) * t64 +
              (t283 + t288 + (t332 + t301 + t302) * t24 + (t335 + t299 + t292 + t293) * t64 +
               (t343 * t5 + t339 + t341 + t342 + t345) * t69) *
                  t69 +
              (t192 + t197 + (t203 * t24 + t207 + t208) * t24 + (t353 + t206 + t200 + t195) * t64 +
               (t339 + t356 + t357 + t312 + t313) * t69 + (t211 * t89 + t213 * t24 + t217 + t218 + t306 + t361) * t89) *
                  t89) *
                 t89) *
                t89 +
            (t79 + (t110 + (t129 + (t140 * t5 + t145) * t5) * t5) * t5 +
             (t80 + t381 + (t81 + t383 + (t103 + t113 + t84) * t24) * t24) * t24 +
             (t80 + t381 + (t91 + (t137 + t121) * t5 + (t392 + t120 + t94) * t24) * t24 +
              (t81 + t383 + (t99 * t24 + t120 + t94) * t24 + (t240 + t392 + t113 + t84) * t64) * t64) *
                 t64 +
             (t247 + t410 + (t248 + t412 + (t249 * t24 + t251 + t258) * t24) * t24 +
              (t256 + t420 + (t421 + t274 + t259) * t24 + (t278 + t424 + t418 + t266) * t64) * t64 +
              (t283 + t431 + (t432 + t292 + t286) * t24 + (t297 + t435 + t436 + t302) * t64 +
               (t306 + t308 + t439 + t440 + t313) * t69) *
                  t69) *
                 t69 +
             (t247 + t410 + (t256 + t420 + (t263 + t418 + t266) * t24) * t24 +
              (t248 + t412 + (t424 + t274 + t259) * t24 + (t249 * t64 + t251 + t258 + t421) * t64) * t64 +
              (a[14] + (t459 * t5 + t461) * t5 + (t459 * t24 + t461 + t466) * t24 +
               (t465 * t24 + t459 * t64 + t461 + t466) * t64 + (t475 * t64 + t474 + t478 + t479 + t480) * t69) *
                  t69 +
              (t283 + t431 + (t332 + t436 + t302) * t24 + (t487 + t435 + t292 + t286) * t64 +
               (t475 * t24 + t479 + t480 + t491 + t492) * t69 + (t496 + t474 + t497 + t357 + t440 + t313) * t89) *
                  t89) *
                 t89 +
             (t152 + (t171 + (t182 * t5 + t187) * t5) * t5 + (t153 + t510 + (t166 + t173 + t156) * t24) * t24 +
              (t153 + t510 + (t515 + t179 + t163) * t24 + (t327 + t515 + t173 + t156) * t64) * t64 +
              (t283 + t524 + (t432 + t301 + t286) * t24 + (t335 + t527 + t436 + t293) * t64 +
               (t343 * t24 + t339 + t341 + t345 + t531) * t69) *
                  t69 +
              (t283 + t524 + (t290 + t436 + t293) * t24 + (t487 + t527 + t301 + t286) * t64 +
               (t475 * t5 + t478 + t480 + t491 + t492) * t69 + (t343 * t64 + t342 + t345 + t491 + t531 + t543) * t89) *
                  t89 +
              (t192 + (t203 * t5 + t208) * t5 + (t198 + t207 + t195) * t24 + (t199 * t24 + t195 + t207 + t353) * t64 +
               (t339 + t356 + t439 + t557 + t313) * t69 + (t543 + t474 + t497 + t310 + t557 + t313) * t89 +
               (t211 * t567 + t213 * t5 + t216 + t218 + t306 + t361 + t496) * t567) *
                  t567) *
                 t567) *
                t567);
}

}  // namespace mbnrg_A1B3_deg5
