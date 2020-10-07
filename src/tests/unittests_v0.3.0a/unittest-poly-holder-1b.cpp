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
