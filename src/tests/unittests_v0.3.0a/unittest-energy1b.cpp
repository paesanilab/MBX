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
#include "potential/1b/energy1b.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

// Input data
std::vector<std::vector<double> > xyz = {
    {-3.3099600000e+00, 1.4404010000e+01,  1.3046900000e+01,  -2.5090200000e+00, 1.4669480000e+01,
     1.2329400000e+01,  -4.1929500000e+00, 1.4316780000e+01,  1.2445600000e+01,  -3.1944300000e+00,
     1.3405090000e+01,  1.3412900000e+01,  -3.2439700000e+00, 1.5197490000e+01,  1.3763500000e+01,
     -6.9251900000e+00, 1.2729200000e+01,  1.2999000000e+01,  -6.3721200000e+00, 1.3024600000e+01,
     1.3829800000e+01,  -7.4085400000e+00, 1.3486100000e+01,  1.2437900000e+01,  -6.3225200000e+00,
     1.2150900000e+01,  1.2339100000e+01,  -7.6702900000e+00, 1.1987100000e+01,  1.3413300000e+01},  // ch4
    {9.9261400000e+00, -3.6517300000e+00, -6.8323800000e+00, 1.0814400000e+01, -4.4223500000e+00, -6.7105400000e+00,
     8.9523200000e+00, -3.0785500000e+00, -6.9238500000e+00, 1.1317500000e+01, -2.7030800000e+00, -1.0013150000e+01,
     1.1157300000e+01, -2.4897700000e+00, -8.8912500000e+00, 1.1420900000e+01, -2.9554800000e+00, -1.1151280000e+01,
     7.4149400000e+00, -4.6995700000e+00, -4.6090700000e+00, 6.3672900000e+00, -4.3735300000e+00, -4.9668500000e+00,
     8.4735700000e+00, -5.1000200000e+00, -4.4835000000e+00},  // co2
    {7.4372664830e-01,  1.3745066180e+00,  8.2228117080e-01,  1.5966080550e+00,  9.4041883440e-01,
     1.0251293750e+00,  8.4611620640e-01,  2.3077049320e+00,  1.0097486520e+00,  2.8905925930e+00,
     -1.7061104380e-01, 9.3095149180e-01,  2.7240924310e+00,  -7.8786885510e-01, 2.0726128570e-01,
     3.3243787470e+00,  -6.6848224410e-01, 1.6262376630e+00,  1.7474594210e+00,  -1.4737233740e+00,
     -1.3296783390e+00, 2.2444822770e+00,  -1.7974450600e+00, -2.0835207810e+00, 1.3684055850e+00,
     -1.9338516330e+00, -1.1304174950e+00, -7.5858982760e-01, -2.2804310900e+00, -5.8461710420e-01,
     -1.3281652060e+00, -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00, -1.9057671840e+00,
     2.3302304610e-01,  -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01,
     1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00,
     -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,
     1.4232346440e+00,  -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+0}  // h2o
};

std::vector<std::vector<double> > grad = {
    {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00},  // ch4
    {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00},  // co2
    {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00}  // h2o
};

std::vector<std::vector<double> > virial = {
    {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00},  // ch4
    {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
     0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00},  // co2
    {3.4410851260e+01, -1.6296570844e+01, 2.6188375935e+00, -1.6296570844e+01, 3.1994134571e-01, -9.3643747304e-02,
     2.6188375935e+00, -9.3643747304e-02, 2.1593608300e-02}  // h2o
};

std::vector<size_t> nmon = {2, 3, 6};
std::vector<std::string> monid = {"ch4", "co2", "h2o"};

// Expected data
std::vector<std::vector<double> > grad_expected = {
    {-2.4886358992e+01, -2.4448141116e+00, 3.6940570962e+01,  3.9510099656e+00,  1.3520648775e+01,
     -6.3317212783e+00, 1.9153279467e+01,  -1.6970289687e+01, 1.6918908457e-01,  -1.4384972782e+01,
     7.5291927462e+00,  -1.5735773153e+01, 1.6167042341e+01,  -1.6347377222e+00, -1.5042265615e+01,
     3.9514785430e+01,  5.7658420300e+01,  -2.7209954552e+00, -2.3433808763e+01, -2.3639400647e+01,
     -2.6223674579e+01, 9.9553409402e+00,  -2.0444464938e+01, 3.4318773501e+00,  -1.6114360295e+01,
     7.6963280066e+00,  1.2346584927e+01,  -9.9219573119e+00, -2.1270882722e+01, 1.3166207757e+01},  // ch4
    {-6.5872245969e+01, 8.8282177814e+01,  -1.3860966834e+01, 1.5969700179e+01,  -3.7417500620e+01, 5.8425250951e+00,
     4.9902545790e+01,  -5.0864677193e+01, 8.0184417392e+00,  5.7423629654e+00,  1.3302864451e+01,  3.5386830965e+01,
     -2.4611751598e+00, -6.3683410966e+00, -1.7572929390e+01, -3.2811878056e+00, -6.9345233546e+00, -1.7813901575e+01,
     2.9745954554e+01,  -7.5778237614e-02, 4.0675037659e+01,  2.7521314998e+01,  -1.5109817554e+01, -1.2354572041e+01,
     -5.7267269552e+01, 1.5185595792e+01,  -2.8320465617e+01},  // co2
    {-1.4069369812e+01, 1.4427586026e+01,  -2.1432516469e+00, 1.9554589232e+01,  -1.6162696076e+01,
     3.6227794573e+00,  -5.4852194204e+00, 1.7351100502e+00,  -1.4795278104e+00, 2.3523870198e+00,
     1.1705903857e+00,  5.9577826609e+00,  -3.7272240395e+00, -3.5382320742e+00, -1.0391148626e+01,
     1.3748370197e+00,  2.3676416884e+00,  4.4333659650e+00,  -8.8748646241e+02, -1.0584243217e+03,
     4.7656670788e+02,  3.7852081036e+00,  -7.5809279718e+00, -8.4591127046e+00, 8.8370125431e+02,
     1.0660052497e+03,  -4.6810759518e+02, -3.8330255328e-01, -5.9270840607e+00, -6.5302574680e+00,
     1.5306352013e+00,  -2.6879277759e+00, -4.7704494613e+00, -1.1473326480e+00, 8.6150118367e+00,
     1.1300706929e+01,  -3.1648620706e-02, -1.1860487700e-01, 1.3060626486e-01,  1.4204503084e+00,
     7.1303347262e-01,  -2.2106585069e-01, -1.3888016877e+00, -5.9442859561e-01, 9.0459585827e-02,
     -9.0926565422e+00, -4.0362031614e+00, 1.7635967973e-01,  1.3772047859e+01,  7.5691179493e+00,
     1.1003124465e+00,  -4.6793913168e+00, -3.5329147879e+00, -1.2766721262e+00}  // h2o
};

std::vector<std::vector<double> > virial_expected = {
    {3.4433906878e+01, -4.3870665345e+01, 2.6562864769e+01, -4.3870665345e+01, 1.4871706787e+01, 1.9972157158e+01,
     2.6562864769e+01, 1.9972157158e+01, 3.8502310634e+01},  // ch4
    {1.2381340104e+02, -4.8505477006e+01, 1.8683238846e+01, -4.8505477006e+01, 1.0935575241e+01, -8.1542367248e+00,
     1.8683238846e+01, -8.1542367248e+00, -1.4018906261e+00},  // co2
    {3.3571765492e+02, 3.9630381602e+02, -1.7745019977e+02, 3.9630381602e+02, 4.6891626099e+02, -2.2753000092e+02,
     -1.7745019977e+02, -2.2753000092e+02, 6.5668105794e+01}  // h2o
};

std::vector<std::vector<size_t> > bad_idxs_expected = {
    {},  // ch4
    {},  // co2
    {2}  // h2o
};

std::vector<double> energies_expected = {7.2321009178e+00, 7.0170987032e+00, 1.6000521553e+02};

TEST_CASE("energy1b::get_1b_energy_nograd") {
    for (size_t i = 0; i < nmon.size(); i++) {
        SECTION(monid[i]) {
            std::vector<size_t> bad_idxs;
            double e = e1b::get_1b_energy(monid[i], nmon[i], xyz[i], bad_idxs);
            REQUIRE(e == Approx(energies_expected[i]).margin(TOL));
            REQUIRE(VectorsAreEqual(bad_idxs, bad_idxs_expected[i]));
        }
    }

    SECTION("Unknown monomer") {
        std::vector<size_t> bad_idxs;
        std::vector<size_t> bad_idxs_expected;
        std::vector<double> xyz_nomon = {1.0, 0.0, 0.0};
        size_t nmon_nomon = 1;
        double e = e1b::get_1b_energy("not_a_monomer", nmon_nomon, xyz_nomon, bad_idxs);

        REQUIRE(VectorsAreEqual(bad_idxs, bad_idxs_expected));
        REQUIRE(e == Approx(0.0).margin(TOL));
    }
}

TEST_CASE("energy1b::get_1b_energy_grad") {
    for (size_t i = 0; i < nmon.size(); i++) {
        SECTION(monid[i]) {
            std::vector<size_t> bad_idxs;
            double e = e1b::get_1b_energy(monid[i], nmon[i], xyz[i], grad[i], bad_idxs, &virial[i]);
            REQUIRE(e == Approx(energies_expected[i]).margin(TOL));
            REQUIRE(VectorsAreEqual(bad_idxs, bad_idxs_expected[i]));
            REQUIRE(VectorsAreEqual(grad[i], grad_expected[i], TOL));
            REQUIRE(VectorsAreEqual(virial[i], virial_expected[i], TOL));
        }
    }

    SECTION("Unknown monomer") {
        std::vector<size_t> bad_idxs;
        std::vector<size_t> bad_idxs_expected;
        std::vector<double> grad_nomon = {2.0, 0.0, 0.0};
        std::vector<double> grad_nomon_expected = {2.0, 0.0, 0.0};
        std::vector<double> xyz_nomon = {1.0, 0.0, 0.0};
        size_t nmon_nomon = 1;
        std::vector<double> virial_nomon(9, 1.0);
        std::vector<double> virial_nomon_expected(9, 1.0);
        std::string mon_nomon = "not_a_monomer";
        double e = e1b::get_1b_energy(mon_nomon, nmon_nomon, xyz_nomon, grad_nomon, bad_idxs, &virial_nomon);

        REQUIRE(VectorsAreEqual(bad_idxs, bad_idxs_expected));
        REQUIRE(VectorsAreEqual(grad_nomon, grad_nomon_expected, TOL));
        REQUIRE(VectorsAreEqual(virial_nomon, virial_nomon_expected, TOL));
        REQUIRE(e == Approx(0.0).margin(TOL));
    }
}
