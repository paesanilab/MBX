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

#include "tools/testutils.h"
#include "potential/1b/ps.h"
#include "potential/1b/x1b_A1B2_deg4_v1x.h"
#include "potential/1b/x1b_A1B4_deg5_exp0_v1x.cpp"
#include "potential/1b/mbnrg_1b_A1B3_deg5_v1.h"
#include "potential/1b/x1b_A1B2C4_deg5_v1x.h"
#include "potential/1b/mbnrg_1b_A2_deg8_v1.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("x1b_A1B2_deg4_v1x::struct") {
    std::vector<double> xyz = {
        9.9261400000e+00, -3.6517300000e+00, -6.8323800000e+00, 1.0814400000e+01, -4.4223500000e+00, -6.7105400000e+00,
        8.9523200000e+00, -3.0785500000e+00, -6.9238500000e+00, 1.1317500000e+01, -2.7030800000e+00, -1.0013150000e+01,
        1.1157300000e+01, -2.4897700000e+00, -8.8912500000e+00, 1.1420900000e+01, -2.9554800000e+00, -1.1151280000e+01,
        7.4149400000e+00, -4.6995700000e+00, -4.6090700000e+00, 6.3672900000e+00, -4.3735300000e+00, -4.9668500000e+00,
        8.4735700000e+00, -5.1000200000e+00, -4.4835000000e+00};
    std::vector<double> grad(27, 0.0);
    size_t nmon = 3;
    std::vector<double> virial(9, 0.0);

    std::vector<double> energies_expected = {3.2476808973e+00, 3.5463262393e-01, 3.4147851820e+00};
    std::vector<double> grad_expected = {-6.5872245969e+01, 8.8282177814e+01,  -1.3860966834e+01, 1.5969700179e+01,
                                         -3.7417500620e+01, 5.8425250951e+00,  4.9902545790e+01,  -5.0864677193e+01,
                                         8.0184417392e+00,  5.7423629654e+00,  1.3302864451e+01,  3.5386830965e+01,
                                         -2.4611751598e+00, -6.3683410966e+00, -1.7572929390e+01, -3.2811878056e+00,
                                         -6.9345233546e+00, -1.7813901575e+01, 2.9745954554e+01,  -7.5778237614e-02,
                                         4.0675037659e+01,  2.7521314998e+01,  -1.5109817554e+01, -1.2354572041e+01,
                                         -5.7267269552e+01, 1.5185595792e+01,  -2.8320465617e+01};
    std::vector<double> virial_expected = {1.2381340104e+02,  -4.8505477006e+01, 1.8683238846e+01,
                                           -4.8505477006e+01, 1.0935575241e+01,  -8.1542367248e+00,
                                           1.8683238846e+01,  -8.1542367248e+00, -1.4018906261e+00};

    x1b_A1B2_deg4::x1b_A1B2_v1x ph("co2");
    std::vector<double> e_nograd = ph.eval(xyz.data(), nmon);
    std::vector<double> e = ph.eval(xyz.data(), grad.data(), nmon, &virial);

    REQUIRE(VectorsAreEqual(e_nograd, energies_expected, TOL));
    REQUIRE(VectorsAreEqual(e, energies_expected, TOL));
    REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
    REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
}

TEST_CASE("x1b_A1B4_deg5_exp0::struct") {
    std::vector<double> xyz = {
        -3.3099600000e+00, 1.4404010000e+01, 1.3046900000e+01, -2.5090200000e+00, 1.4669480000e+01, 1.2329400000e+01,
        -4.1929500000e+00, 1.4316780000e+01, 1.2445600000e+01, -3.1944300000e+00, 1.3405090000e+01, 1.3412900000e+01,
        -3.2439700000e+00, 1.5197490000e+01, 1.3763500000e+01, -6.9251900000e+00, 1.2729200000e+01, 1.2999000000e+01,
        -6.3721200000e+00, 1.3024600000e+01, 1.3829800000e+01, -7.4085400000e+00, 1.3486100000e+01, 1.2437900000e+01,
        -6.3225200000e+00, 1.2150900000e+01, 1.2339100000e+01, -7.6702900000e+00, 1.1987100000e+01, 1.3413300000e+01};
    std::vector<double> grad(30, 0.0);
    size_t nmon = 2;
    std::vector<double> virial(9, 0.0);

    std::vector<double> energies_expected = {3.8778480235e+00, 3.3542528943e+00};
    std::vector<double> grad_expected = {
        -2.4886358992e+01, -2.4448141116e+00, 3.6940570962e+01,  3.9510099656e+00,  1.3520648775e+01,
        -6.3317212783e+00, 1.9153279467e+01,  -1.6970289687e+01, 1.6918908457e-01,  -1.4384972782e+01,
        7.5291927462e+00,  -1.5735773153e+01, 1.6167042341e+01,  -1.6347377222e+00, -1.5042265615e+01,
        3.9514785430e+01,  5.7658420300e+01,  -2.7209954552e+00, -2.3433808763e+01, -2.3639400647e+01,
        -2.6223674579e+01, 9.9553409402e+00,  -2.0444464938e+01, 3.4318773501e+00,  -1.6114360295e+01,
        7.6963280066e+00,  1.2346584927e+01,  -9.9219573119e+00, -2.1270882722e+01, 1.3166207757e+01};
    std::vector<double> virial_expected = {3.4433906878e+01,  -4.3870665345e+01, 2.6562864769e+01,
                                           -4.3870665345e+01, 1.4871706787e+01,  1.9972157158e+01,
                                           2.6562864769e+01,  1.9972157158e+01,  3.8502310634e+01};

    x1b_A1B4_deg5_exp0::x1b_A1B4_v1x ph("ch4");
    std::vector<double> e_nograd = ph.eval(xyz.data(), nmon);
    std::vector<double> e = ph.eval(xyz.data(), grad.data(), nmon, &virial);

    REQUIRE(VectorsAreEqual(e_nograd, energies_expected, TOL));
    REQUIRE(VectorsAreEqual(e, energies_expected, TOL));
    REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
    REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
}

TEST_CASE("x1b_A1B3_deg5_v1::struct") {
    SECTION("nh3") {
        std::vector<double> xyz = {
            -1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00, 1.6636378008e+00,
            -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,  6.8368652710e-01,  -1.8778475277e+00,
            4.5835317270e-01,  7.1032361800e-02,  1.3253591612e+00,  1.5992161603e+00,  -3.5649305290e-01,
            1.7112172925e+00,  2.2347398082e+00,  3.3141779120e-01,  3.0734064400e-01,  1.6708149436e+00,
            -2.9199566840e-01, 1.5915360076e+00,  1.9683083217e+00,  -1.2614354220e+00, -1.6441806609e+00,
            -1.6474447284e+00, 3.9846272690e-01,  -6.2596824480e-01, -1.7186182488e+00, 3.3660579490e-01,
            -2.0279253230e+00, -2.2803120707e+00, -2.9307077260e-01, -1.9129140343e+00, -2.0201554870e+00,
            1.3011627956e+00,  1.4996742515e+00,  -1.5117612237e+00, 1.4924089150e-01,  2.0687387995e+00,
            -1.7073176192e+00, 9.6405578870e-01,  1.5595189444e+00,  -5.0555055410e-01, -2.2826688700e-02,
            1.9547066131e+00,  -1.9730283747e+00, -6.2933263920e-01};
        std::vector<double> grad(48, 0.0);
        size_t nmon = 4;
        std::vector<double> virial(9, 0.0);

        std::vector<double> energies_expected = {-4.0175518037e-02, -4.0180394258e-02, -4.0177881849e-02,
                                                 -4.0179844582e-02};
        std::vector<double> grad_expected = {
            -3.1149881976e+00, 6.4747045666e+00,  -8.2576839819e-01, 1.2018885987e+00,  1.1710953689e+00,
            1.9064744842e+00,  8.7996110145e-01,  4.7577793373e-01,  -2.3326637013e+00, 1.0331384975e+00,
            -8.1215778692e+00, 1.2519576153e+00,  6.5047623359e+00,  3.0031997178e+00,  -9.8790293587e-01,
            6.8681389845e-01,  -1.3945239911e+00, -2.0087638680e+00, -8.1942670750e+00, -9.2365456054e-01,
            7.6776864762e-01,  1.0026908406e+00,  -6.8502116621e-01, 2.2288981563e+00,  -6.5067831682e+00,
            -3.0072202392e+00, 9.6296334260e-01,  8.1961974942e+00,  9.2736489941e-01,  -7.4485710128e-01,
            -6.9052691317e-01, 1.3857085407e+00,  2.0115812487e+00,  -9.9888741284e-01, 6.9414679908e-01,
            -2.2296874900e+00, 3.1168915871e+00,  -6.4702757681e+00, 8.5392346318e-01,  -1.1924392361e+00,
            -1.1762057818e+00, -1.9073202626e+00, -1.0348406920e+00, 8.1171296361e+00,  -1.2782081027e+00,
            -8.8961165891e-01, -4.7064808615e-01, 2.3316049021e+00};
        std::vector<double> virial_expected = {-1.5460397497e+01, 3.4913832175e-01,  2.1084920443e+00,
                                               3.4913832175e-01,  -1.4834688657e+01, 4.7296070461e+00,
                                               2.1084920443e+00,  4.7296070461e+00,  1.3016391273e+01};

        mbnrg_A1B3_deg5::mbnrg_A1B3_deg5_v1 ph("nh3");
        std::vector<double> e_nograd = ph.eval(xyz.data(), nmon);
        std::vector<double> e = ph.eval(xyz.data(), grad.data(), nmon, &virial);

        REQUIRE(VectorsAreEqual(e_nograd, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(e, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("nh3pbe0d3bj") {
        std::vector<double> xyz = {
            -1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00, 1.6636378008e+00,
            -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,  6.8368652710e-01,  -1.8778475277e+00,
            4.5835317270e-01,  7.1032361800e-02,  1.3253591612e+00,  1.5992161603e+00,  -3.5649305290e-01,
            1.7112172925e+00,  2.2347398082e+00,  3.3141779120e-01,  3.0734064400e-01,  1.6708149436e+00,
            -2.9199566840e-01, 1.5915360076e+00,  1.9683083217e+00,  -1.2614354220e+00, -1.6441806609e+00,
            -1.6474447284e+00, 3.9846272690e-01,  -6.2596824480e-01, -1.7186182488e+00, 3.3660579490e-01,
            -2.0279253230e+00, -2.2803120707e+00, -2.9307077260e-01, -1.9129140343e+00, -2.0201554870e+00,
            1.3011627956e+00,  1.4996742515e+00,  -1.5117612237e+00, 1.4924089150e-01,  2.0687387995e+00,
            -1.7073176192e+00, 9.6405578870e-01,  1.5595189444e+00,  -5.0555055410e-01, -2.2826688700e-02,
            1.9547066131e+00,  -1.9730283747e+00, -6.2933263920e-01};
        std::vector<double> grad(48, 0.0);
        size_t nmon = 4;
        std::vector<double> virial(9, 0.0);

        std::vector<double> energies_expected = {1.4018912593e-02, 1.4014668005e-02, 1.4017733811e-02,
                                                 1.4016580042e-02};
        std::vector<double> grad_expected = {
            6.2636403844e-01,  7.7767915892e+00,  -1.3221766469e+00, -3.7315031239e-01, 1.2374137348e+00,
            6.6119876069e-01,  -4.9638483262e-01, 9.7103596770e-01,  -9.5559219200e-01, 2.4317110657e-01,
            -9.9852412916e+00, 1.6165700782e+00,  7.8670260372e+00,  -7.1353116349e-01, -4.6858434616e-01,
            1.0506338354e+00,  2.8102591546e-01,  -9.6185479401e-01, -1.0087333464e+01, -1.1962368476e-01,
            7.7555309836e-01,  1.1696735913e+00,  5.5212893280e-01,  6.5488604181e-01,  -7.8682208834e+00,
            7.1129565855e-01,  4.5435903174e-01,  1.0089431941e+01,  1.2370593542e-01,  -7.5026159055e-01,
            -1.0510875915e+00, -2.8575030391e-01, 9.5699300463e-01,  -1.1701234662e+00, -5.4925129006e-01,
            -6.6109044582e-01, -6.2529688486e-01, -7.7741626692e+00, 1.3391750226e+00,  3.7757302028e-01,
            -1.2372945711e+00, -6.5541969071e-01, -2.4504902986e-01, 9.9803516720e+00,  -1.6454564451e+00,
            4.9277289444e-01,  -9.6889443180e-01, 9.6170111327e-01};
        std::vector<double> virial_expected = {-2.2824245667e+01, 3.3417691335e-01,  2.0177069593e+00,
                                               3.3417691335e-01,  -2.2225470104e+01, 4.5259112292e+00,
                                               2.0177069593e+00,  4.5259112292e+00,  4.4265282154e+00};

        mbnrg_A1B3_deg5::mbnrg_A1B3_deg5_v1 ph("nh3pbe0d3bj");
        std::vector<double> e_nograd = ph.eval(xyz.data(), nmon);
        std::vector<double> e = ph.eval(xyz.data(), grad.data(), nmon, &virial);

        REQUIRE(VectorsAreEqual(e_nograd, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(e, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x1b_A1B2C4_v1x::struct") {
    SECTION("n2o5") {
        std::vector<double> xyz = {
            8.2680440000e+00, 1.0783073000e+01, 1.0117501000e+01, 9.3865410000e+00, 1.1620399000e+01, 1.0436688000e+01,
            8.3598230000e+00, 9.4073400000e+00, 9.8844810000e+00, 1.0525770000e+01, 1.1253619000e+01, 1.0065799000e+01,
            9.0863690000e+00, 1.2796432000e+01, 1.0771404000e+01, 9.3304000000e+00, 8.9002400000e+00, 1.0425331000e+01,
            7.2746500000e+00, 8.8236580000e+00, 9.7258010000e+00};
        std::vector<double> grad(21, 0.0);
        size_t nmon = 1;
        std::vector<double> virial(9, 0.0);

        std::vector<double> energies_expected = {3.0520836360e+00};
        std::vector<double> grad_expected = {6.4721152873e+01,  -5.6750540373e+01, 1.3809257371e+01,  -1.0879045174e+02,
                                             -7.7943593170e+01, 3.8767109843e+01,  4.3847440598e+00,  1.0526926787e+02,
                                             -1.0500917482e+02, 8.0355635182e+01,  6.5964729876e+00,  -3.2521396713e+01,
                                             9.9285069042e+00,  9.9556285185e+01,  8.7403410272e+00,  -8.7982126475e+00,
                                             -1.7472027514e+01, 4.3057689850e+01,  -4.1801374631e+01, -5.9255864990e+01,
                                             3.3156173436e+01};
        std::vector<double> virial_expected = {-1.0044478649e+02, -5.9147166137e+01, 1.9743252937e+01,
                                               -5.9147166137e+01, -1.4246362465e+02, -3.3182784319e+01,
                                               1.9743252937e+01,  -3.3182784319e+01, -4.4507127408e+01};

        x1b_A1B2C4_deg5::x1b_A1B2C4_v1x ph("n2o5");
        std::vector<double> e_nograd = ph.eval(xyz.data(), nmon);
        std::vector<double> e = ph.eval(xyz.data(), grad.data(), nmon, &virial);

        REQUIRE(VectorsAreEqual(e_nograd, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(e, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_1b_A2_deg8::struct") {
    SECTION("h2") {
        std::vector<double> xyz = {3.5927079900e+00,  -8.4084266700e-01, 8.0129372700e-01,  3.8147658100e+00,
                                   2.4972866800e-02,  -4.2299288300e-01, -1.1221514300e+00, -1.6953293900e+00,
                                   2.4726422100e+00,  -1.0730754800e+00, -3.0449198500e+00, 1.7376520600e+00,
                                   1.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  2.0831958000e+00,
                                   -4.4599113500e-01, 3.0238165600e+00,  -1.7998662700e+00, 7.4937353100e-01,
                                   5.4245516000e+00,  -3.9361521900e-01, 4.1803592100e-01,  3.8895249900e+00};
        std::vector<double> grad(24, 0.0);
        size_t nmon = 4;
        std::vector<double> virial(9, 0.0);

        std::vector<double> energies_expected = {6.6310985904e+01, 6.7805257190e+01, 8.7706940302e-03,
                                                 8.1853508237e+01};
        std::vector<double> grad_expected = {-1.0332327796e+01, -4.0286308791e+01, 5.6965931533e+01,  1.0332327796e+01,
                                             4.0286308791e+01,  -5.6965931533e+01, -2.1488350647e+00, 5.9093044627e+01,
                                             3.2182211583e+01,  2.1488350647e+00,  -5.9093044627e+01, -3.2182211583e+01,
                                             -2.3356157652e+00, -2.9400616546e+00, -3.8891170838e-01, 2.3356157652e+00,
                                             2.9400616546e+00,  3.8891170838e-01,  8.8857047824e+00,  -2.0936291451e+00,
                                             -9.6994013124e+00, -8.8857047824e+00, 2.0936291451e+00,  9.6994013124e+00};
        std::vector<double> virial_expected = {9.0176304020e+00,  -1.0347081242e+01, 4.0979644503e-01,
                                               -1.0347081242e+01, -1.1564649678e+02, 8.8769884942e+00,
                                               4.0979644503e-01,  8.8769884942e+00,  -7.8537288029e+01};

        mbnrg_A2_deg8::mbnrg_A2_deg8_v1 ph("h2");
        std::vector<double> e_nograd = ph.eval(xyz.data(), nmon);
        std::vector<double> e = ph.eval(xyz.data(), grad.data(), nmon, &virial);

        REQUIRE(VectorsAreEqual(e_nograd, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(e, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}
