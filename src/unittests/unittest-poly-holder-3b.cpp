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
#include "potential/3b/mbnrg_3b_A1B2_A1B2_A1B2_deg4_v1.h"
#include "potential/3b/mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1.h"
#include "potential/3b/x3b-h2o-ion-v1x-deg4-filtered.h"
#include "potential/3b/x3b-v2x.h"
#include "potential/3b/mbnrg_3b_A1_B1C2X2_B1C2X2_deg4_v1.h"
#include "potential/3b/mbnrg_3b_A1B2C4_D1E2_D1E2_deg3_v1.h"
#include "potential/3b/mbnrg_3b_A1B2_A1B2_A1B2_deg4_vmbpbe.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

TEST_CASE("mbnrg_A1B2_A1B2_A1B2_deg4_vmbpbe::struct") {
    SECTION("mbpbe-mbpbe-mbpbe") {
        std::vector<double> xyz1 = {1.7446413520e+01, -2.3400777000e+00, -1.3656342560e-01,
                                    1.7336517080e+01, -1.3991926120e+00, 6.3450725850e-02,
                                    1.6804980910e+01, -2.8010115110e+00, 4.0279153980e-01};
        std::vector<double> xyz2 = {1.8130286090e+01, 3.4634481370e-01,  2.3908143380e-01,
                                    1.9018808010e+01, -3.7362024750e-02, 2.1367451900e-01,
                                    1.8150356650e+01, 1.0128089980e+00,  9.2480894580e-01};
        std::vector<double> xyz3 = {2.0136387580e+01, -1.5728360700e+00, -1.0436900630e-01,
                                    1.9362877520e+01, -2.1550800780e+00, -1.0903138800e-01,
                                    2.0809672630e+01, -2.0327938160e+00, 3.9555665570e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -2.0316534362e-01;
        std::vector<double> grad1_expected = {7.8415243415e-02,  9.1636143540e-01,  2.5339811127e-01,
                                              -2.3656708270e-01, -1.0606683685e+00, -2.8727812353e-01,
                                              -3.4255964639e-01, -1.1412688984e-01, 1.0834193124e-02};
        std::vector<double> grad2_expected = {7.8287430234e-01,  -5.4489238428e-01, 1.6668456702e-02,
                                              -8.3242509048e-01, 7.5536489907e-01,  -1.4897082446e-03,
                                              7.5678923916e-02,  3.4694810379e-01,  6.6250701940e-02};
        std::vector<double> grad3_expected = {-8.5064040032e-01, -4.2365004790e-01, 8.3719907151e-02,
                                              1.0568104931e+00,  3.6499404140e-01,  -1.1593314351e-01,
                                              2.6841325707e-01,  -2.4033078909e-01, -2.6170394900e-02};
        std::vector<double> virial_expected = {-1.6536808816e-01, -6.9035607774e-04, 4.6802803972e-03,
                                               -6.9035607774e-04, -1.6212629613e-01, -2.2933993205e-02,
                                               4.6802803972e-03,  -2.2933993205e-02, -1.0017763562e-02};

        mbnrg_A1B2_A1B2_A1B2_deg4::mbnrg_A1B2_A1B2_A1B2_deg4_vmbpbe ph("mbpbe", "mbpbe", "mbpbe");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1B2_A1B2_A1B2_deg4_v1::struct") {
    SECTION("co2-co2-co2") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> xyz3 = {-1.2074773230e+00, -2.9527196541e+00, 1.5846527406e+00,
                                    -5.7435628700e-01, -3.7550817218e+00, 1.0197558981e+00,
                                    -1.8424461038e+00, -2.1521645918e+00, 2.1591944626e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 4.9621879400e-02;
        std::vector<double> grad1_expected = {3.6357674014e-01,  9.6923976481e-01,  1.3785438769e-01,
                                              -2.9794723035e-03, -6.0388048170e-01, -7.7520112915e-02,
                                              -3.9790696220e-01, -4.0320213564e-01, -9.1980435140e-02};
        std::vector<double> grad2_expected = {-4.7237087510e-01, -9.6219657524e-02, -5.3093540871e-01,
                                              1.9000272655e-01,  9.6913659281e-02,  2.8849085584e-01,
                                              3.4441345653e-01,  1.0534547329e-02,  3.2669128895e-01};
        std::vector<double> grad3_expected = {2.2260135009e-01,  -8.8403359042e-01, 6.1601522016e-01,
                                              -2.3482794762e-01, 6.0533243698e-01,  -2.2457987659e-01,
                                              -1.2509016099e-02, 3.0531545690e-01,  -4.4403591928e-01};
        std::vector<double> virial_expected = {8.2852431287e-01,  -4.3742690281e-01, 9.6596469840e-02,
                                               -4.3742690281e-01, 4.9846922060e-01,  1.2975477568e-01,
                                               9.6596469840e-02,  1.2975477568e-01,  3.7833256226e-01};

        mbnrg_A1B2_A1B2_A1B2_deg4::mbnrg_A1B2_A1B2_A1B2_deg4_v1 ph("co2", "co2", "co2");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1_B1C2X2_B1C2X2_deg4_v1::struct") {
    SECTION("f--h2o-h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00, -1.9611387200e-03,
                                    -7.5739101100e-01, 2.0000000000e+00, -5.8831383900e-01,
                                    7.5739101100e-01,  2.0000000000e+00, -5.8831383900e-01};
        std::vector<double> xyz3 = {0.0000000000e+00,  4.0000000000e+00, 3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00, 2.5883138390e+00,
                                    7.5739101100e-01,  4.0000000000e+00, 2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.8544877838e-01;
        std::vector<double> grad1_expected = {2.1840168563e-15, 4.7216142159e-01, -2.4177284197e-01};
        std::vector<double> grad2_expected = {2.7894353494e-15,  6.2403794593e-01,  1.2186959105e-01,
                                              6.0950937760e-01,  -3.8069026350e-01, 2.3026742917e-01,
                                              -6.0950937760e-01, -3.8069026350e-01, 2.3026742917e-01};
        std::vector<double> grad3_expected = {-7.7780663854e-16, 5.3971168524e-01,  1.4475112850e+00,
                                              -6.5833793397e-01, -4.3726526287e-01, -8.9407144622e-01,
                                              6.5833793397e-01,  -4.3726526287e-01, -8.9407144622e-01};
        std::vector<double> virial_expected = {-7.3964619358e-02, 7.5495165675e-15, 1.5543122345e-15,
                                               7.5495165675e-15,  1.6139605242e+00, 1.9771753089e-01,
                                               1.5543122345e-15,  1.9771753089e-01, 5.5691917322e-01};

        mbnrg_A1_B1C2X2_B1C2X2_deg4::mbnrg_A1_B1C2X2_B1C2X2_deg4_v1 ph("f-", "h2o", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("br--h2o-h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00, -1.9611387200e-03,
                                    -7.5739101100e-01, 2.0000000000e+00, -5.8831383900e-01,
                                    7.5739101100e-01,  2.0000000000e+00, -5.8831383900e-01};
        std::vector<double> xyz3 = {0.0000000000e+00,  4.0000000000e+00, 3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00, 2.5883138390e+00,
                                    7.5739101100e-01,  4.0000000000e+00, 2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 5.6556864303e-01;
        std::vector<double> grad1_expected = {-6.3837823916e-16, 1.4900602173e+00, -1.3435026767e-01};
        std::vector<double> grad2_expected = {1.1102230246e-16,  -7.3046982268e-01, -7.8206553150e-01,
                                              6.6065758002e-01,  -1.8598217995e-01, 6.8772098074e-01,
                                              -6.6065758002e-01, -1.8598217995e-01, 6.8772098074e-01};
        std::vector<double> grad3_expected = {-1.0599160438e-15, 3.2471751579e-01,  1.2861203753e+00,
                                              -4.7655350630e-01, -3.5617177524e-01, -8.7257326880e-01,
                                              4.7655350630e-01,  -3.5617177524e-01, -8.7257326880e-01};
        std::vector<double> virial_expected = {2.7887754106e-01, 1.2212453271e-15, 4.7739590059e-15,
                                               1.2212453271e-15, 3.7553725040e+00, 6.4935178929e-01,
                                               4.7739590059e-15, 6.4935178929e-01, 1.4662836102e+00};

        mbnrg_A1_B1C2X2_B1C2X2_deg4::mbnrg_A1_B1C2X2_B1C2X2_deg4_v1 ph("br-", "h2o", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-h2o-i-") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00, -1.9611387200e-03,
                                    -7.5739101100e-01, 2.0000000000e+00, -5.8831383900e-01,
                                    7.5739101100e-01,  2.0000000000e+00, -5.8831383900e-01};
        std::vector<double> xyz3 = {0.0000000000e+00,  4.0000000000e+00, 3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00, 2.5883138390e+00,
                                    7.5739101100e-01,  4.0000000000e+00, 2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.0473957436e+00;
        std::vector<double> grad1_expected = {-3.0531133177e-16, 1.9252597162e+00, 3.5529709174e-02};
        std::vector<double> grad2_expected = {3.6290415117e-15,  -1.8160009303e+00, -7.9909203618e-01,
                                              9.6825226266e-01,  2.4619035187e-01,  7.3937220898e-01,
                                              -9.6825226266e-01, 2.4619035187e-01,  7.3937220898e-01};
        std::vector<double> grad3_expected = {-2.0903417886e-15, 8.3053840925e-01,  1.3260048978e+00,
                                              -4.1939528340e-01, -7.1608894946e-01, -1.0205934944e+00,
                                              4.1939528340e-01,  -7.1608894946e-01, -1.0205934944e+00};
        std::vector<double> virial_expected = {8.3139868485e-01, 3.0531133177e-15, 5.4400928207e-15,
                                               3.0531133177e-15, 5.0537984118e+00, 1.5014236003e+00,
                                               5.4400928207e-15, 1.5014236003e+00, 2.1736165127e+00};

        mbnrg_A1_B1C2X2_B1C2X2_deg4::mbnrg_A1_B1C2X2_B1C2X2_deg4_v1 ph("i-", "h2o", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1_B1C2X2_B1C2X2_deg4_oldswitch::struct") {
    SECTION("cl--h2o-h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00, -1.9611387200e-03,
                                    -7.5739101100e-01, 2.0000000000e+00, -5.8831383900e-01,
                                    7.5739101100e-01,  2.0000000000e+00, -5.8831383900e-01};
        std::vector<double> xyz3 = {0.0000000000e+00,  4.0000000000e+00, 3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00, 2.5883138390e+00,
                                    7.5739101100e-01,  4.0000000000e+00, 2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 3.3572030397e-01;
        std::vector<double> grad1_expected = {-5.8182625384e-15, 1.1662421657e+00, 1.8059660835e-02};
        std::vector<double> grad2_expected = {-3.0531133177e-16, -1.1590497998e+00, 4.9012091356e-01,
                                              6.4460058104e-02,  3.5584717178e-01,  3.1826009728e-01,
                                              -6.4460058104e-02, 3.5584717178e-01,  3.1826009728e-01};
        std::vector<double> grad3_expected = {8.3960616237e-16,  -1.1754066850e-01, -4.5488294130e-01,
                                              2.7905415091e-01,  -3.0067302052e-01, -3.4490891383e-01,
                                              -2.7905415091e-01, -3.0067302052e-01, -3.4490891383e-01};
        std::vector<double> virial_expected = {5.2034914811e-01,  -1.5709655798e-14, -2.8865798640e-15,
                                               -1.5709655798e-14, 3.7702577506e+00,  2.3255208596e+00,
                                               -2.8865798640e-15, 2.3255208596e+00,  3.5255486880e+00};

        mbnrg_A1_B1C2X2_B1C2X2_deg4_oldswitch::mbnrg_A1_B1C2X2_B1C2X2_deg4_v1 ph("cl-", "h2o", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_3b_A1B4_C1D2_C1D2_deg3_v1::class") {
    SECTION("ch4-h2o-h2o") {
        std::vector<double> xyz1 = {-1.9895400000e+00, -4.3787000000e-01, 2.5724300000e+00,  -1.2478300000e+00,
                                    1.0363000000e-01,  1.9881600000e+00,  -2.5511600000e+00, 2.3073000000e-01,
                                    3.1955700000e+00,  -2.6391500000e+00, -8.8267000000e-01, 1.9054200000e+00,
                                    -1.4527200000e+00, -1.1652700000e+00, 3.1896100000e+00};
        std::vector<double> xyz2 = {-4.4590985000e-03, -5.1342579600e-02, 3.0000158138e+00,
                                    9.8613021140e-01,  -7.4573098400e-02, 3.0000054324e+00,
                                    -1.5974709230e-01, 8.9671808950e-01,  3.0000164932e+00};
        std::vector<double> xyz3 = {-4.4590985000e-03, -5.1342579600e-02, 1.5813800000e-05,
                                    9.8613021140e-01,  -7.4573098400e-02, 5.4324000000e-06,
                                    -1.5974709230e-01, 8.9671808950e-01,  1.6493200000e-05};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -8.6542153165e-01;
        std::vector<double> grad1_expected = {
            -1.2986363504e-01, -2.1298329637e-01, -4.2909417537e-01, -1.6020735854e+00, -1.1598726850e-01,
            4.5610144555e-01,  -1.4925168076e-03, 1.1352788256e-01,  8.9686107490e-02,  9.0349385666e-02,
            8.0517733627e-03,  9.7436642450e-03,  6.7661968517e-02,  4.5362008195e-01,  -1.3364533671e-01};
        std::vector<double> grad2_expected = {1.0959257760e+00,  -1.4596018983e-01, 8.0172012944e-01,
                                              -4.4021147491e-01, 3.8969650502e-01,  9.7920941713e-02,
                                              4.4858244538e-01,  -4.0167462931e-01, 2.9436181065e-02};
        std::vector<double> grad3_expected = {5.7014671731e-01,  6.5481135440e-02,  -9.5374218002e-01,
                                              -3.0146292273e-01, 1.8628627926e-01,  -1.1760539406e-01,
                                              2.0243784202e-01,  -3.4005827356e-01, 1.4947861665e-01};
        std::vector<double> virial_expected = {-1.0817372419e+00, -2.8545314809e-01, -1.7687894038e-01,
                                               -2.8545314809e-01, 1.1322095382e+00,  -5.7268826677e-01,
                                               -1.7687894038e-01, -5.7268826677e-01, -2.4691076218e+00};

        mbnrg_A1B4_C1D2_C1D2_deg3::mbnrg_A1B4_C1D2_C1D2_deg3_v1 ph("ch4", "h2o", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x3b-3b-h2o-ion-v1x_deg4_filtered::class") {
    SECTION("h2o-h2o-li") {
        std::vector<double> xyz1 = {
            7.4372664830e-01,  1.3745066180e+00,  8.2228117080e-01,  1.5966080550e+00,  9.4041883440e-01,
            1.0251293750e+00,  8.4611620640e-01,  2.3077049320e+00,  1.0097486520e+00,  2.8905925930e+00,
            -1.7061104380e-01, 9.3095149180e-01,  2.7240924310e+00,  -7.8786885510e-01, 2.0726128570e-01,
            3.3243787470e+00,  -6.6848224410e-01, 1.6262376630e+00,  -7.5858982760e-01, -2.2804310900e+00,
            -5.8461710420e-01, -1.3281652060e+00, -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00,
            -1.9057671840e+00, 2.3302304610e-01,  -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00,
            -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,
            -3.3238997190e+00, -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01,
            3.9350777550e-01,  1.4232346440e+00,  -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00,
            2.8905925930e+00,  -1.7061104380e-01, 9.3095149180e-01,  2.7240924310e+00,  -7.8786885510e-01,
            2.0726128570e-01,  3.3243787470e+00,  -6.6848224410e-01, 1.6262376630e+00,  -7.5858982760e-01,
            -2.2804310900e+00, -5.8461710420e-01, -1.3281652060e+00, -3.0099821950e+00, -8.3398172910e-01,
            -1.1158172200e+00, -1.9057671840e+00, 2.3302304610e-01,  -9.1278996460e-01, 1.3711133940e+00,
            -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00,
            1.3696631160e+00,  -3.3238997190e+00, -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,
            -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,  -2.2258007790e+00, -6.7917366630e-02,
            1.6302588920e+00,  -7.5858982760e-01, -2.2804310900e+00, -5.8461710420e-01, -1.3281652060e+00,
            -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00, -1.9057671840e+00, 2.3302304610e-01,
            -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,
            -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00, -1.4297465600e+00,
            -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,
            -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00,  -9.1278996460e-01, 1.3711133940e+00,
            -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00,
            1.3696631160e+00,  -3.3238997190e+00, -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,
            -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,  -2.2258007790e+00, -6.7917366630e-02,
            1.6302588920e+00,  -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01,
            3.9350777550e-01,  1.4232346440e+00,  -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00};
        std::vector<double> xyz2 = {
            1.7474594210e+00,  -1.4737233740e+00, -1.3296783390e+00, 2.2444822770e+00,  -1.7974450600e+00,
            -2.0835207810e+00, 1.0684055850e+00,  -2.1338516330e+00, -1.1304174950e+00, 1.7474594210e+00,
            -1.4737233740e+00, -1.3296783390e+00, 2.2444822770e+00,  -1.7974450600e+00, -2.0835207810e+00,
            1.0684055850e+00,  -2.1338516330e+00, -1.1304174950e+00, 1.7474594210e+00,  -1.4737233740e+00,
            -1.3296783390e+00, 2.2444822770e+00,  -1.7974450600e+00, -2.0835207810e+00, 1.0684055850e+00,
            -2.1338516330e+00, -1.1304174950e+00, 1.7474594210e+00,  -1.4737233740e+00, -1.3296783390e+00,
            2.2444822770e+00,  -1.7974450600e+00, -2.0835207810e+00, 1.0684055850e+00,  -2.1338516330e+00,
            -1.1304174950e+00, 1.7474594210e+00,  -1.4737233740e+00, -1.3296783390e+00, 2.2444822770e+00,
            -1.7974450600e+00, -2.0835207810e+00, 1.0684055850e+00,  -2.1338516330e+00, -1.1304174950e+00,
            7.4372664830e-01,  1.3745066180e+00,  8.2228117080e-01,  1.5966080550e+00,  9.4041883440e-01,
            1.0251293750e+00,  8.4611620640e-01,  2.3077049320e+00,  1.0097486520e+00,  7.4372664830e-01,
            1.3745066180e+00,  8.2228117080e-01,  1.5966080550e+00,  9.4041883440e-01,  1.0251293750e+00,
            8.4611620640e-01,  2.3077049320e+00,  1.0097486520e+00,  7.4372664830e-01,  1.3745066180e+00,
            8.2228117080e-01,  1.5966080550e+00,  9.4041883440e-01,  1.0251293750e+00,  8.4611620640e-01,
            2.3077049320e+00,  1.0097486520e+00,  7.4372664830e-01,  1.3745066180e+00,  8.2228117080e-01,
            1.5966080550e+00,  9.4041883440e-01,  1.0251293750e+00,  8.4611620640e-01,  2.3077049320e+00,
            1.0097486520e+00,  2.8905925930e+00,  -1.7061104380e-01, 9.3095149180e-01,  2.7240924310e+00,
            -7.8786885510e-01, 2.0726128570e-01,  3.3243787470e+00,  -6.6848224410e-01, 1.6262376630e+00,
            2.8905925930e+00,  -1.7061104380e-01, 9.3095149180e-01,  2.7240924310e+00,  -7.8786885510e-01,
            2.0726128570e-01,  3.3243787470e+00,  -6.6848224410e-01, 1.6262376630e+00,  2.8905925930e+00,
            -1.7061104380e-01, 9.3095149180e-01,  2.7240924310e+00,  -7.8786885510e-01, 2.0726128570e-01,
            3.3243787470e+00,  -6.6848224410e-01, 1.6262376630e+00,  -7.5858982760e-01, -2.2804310900e+00,
            -5.8461710420e-01, -1.3281652060e+00, -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00,
            -1.9057671840e+00, 2.3302304610e-01,  -7.5858982760e-01, -2.2804310900e+00, -5.8461710420e-01,
            -1.3281652060e+00, -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00, -1.9057671840e+00,
            2.3302304610e-01,  -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01,
            1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00};
        std::vector<double> xyz3 = {
            -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
            -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
            1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00,
            -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
            -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
            1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00,
            -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
            -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
            1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 15;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 6.6537845821e-01;
        std::vector<double> grad1_expected = {
            -2.2622565551e-01, 1.6376343512e-01,  5.1008962761e-01,  4.8719345513e-02,  -1.7468791946e-01,
            -3.0180368854e-01, 9.8868728483e-02,  1.4914382925e-01,  -9.0343149446e-02, 1.1395177718e+00,
            1.4232954617e+00,  1.1016281264e+00,  -1.5340348785e+00, 4.6513286333e-01,  -6.3720897785e-01,
            -2.7297411914e-01, -7.5655061276e-01, -1.6736996123e-01, 1.8228807779e+00,  9.9106578527e-01,
            2.0626069576e+00,  -1.2873680555e+00, -1.8202124281e+00, -1.5842368838e+00, 1.4589433511e-01,
            -2.2137597885e-01, -2.0462158371e-01, -4.4719959689e-01, 2.0006056927e-01,  -2.5719164463e-01,
            2.0762472736e-01,  7.4368521245e-02,  -6.3145427442e-02, 2.5680818900e-02,  -6.1423796875e-02,
            -5.7993958970e-03, -3.2881569602e-01, 5.9532823558e-03,  2.7586725318e-01,  1.8728947180e-01,
            2.8229285622e-02,  -1.8208314126e-02, 6.1613478674e-02,  -5.9598235799e-02, -1.7419343926e-01,
            -1.5233094023e+00, -7.5415157703e-01, -4.3227620825e+00, 1.0043897778e+00,  1.5655387367e+00,
            1.9756213851e+00,  9.9643127589e-02,  -1.0691725895e+00, 1.5138423116e+00,  -3.6893458587e-02,
            -6.7822602479e-01, -1.2151320078e-01, -2.5611742160e-03, 1.7064086561e-01,  1.1554989825e-02,
            1.3669221223e-02,  2.2484379719e-01,  7.8184422414e-02,  -3.3587624986e-01, 3.0459648321e-01,
            -9.0941853705e-01, 1.4674256353e-01,  5.0458693098e-02,  4.9575374361e-02,  -7.1856292261e-02,
            -3.7469837103e-02, 1.2743043618e-01,  1.0911461522e+00,  -1.4945480706e-01, 8.0302990427e-01,
            1.8793403994e-01,  -6.6315539317e-01, -3.0532646306e+00, 1.0798803390e-01,  9.0537427008e-01,
            -5.3870055513e-02, -2.4683951135e-02, -6.1763544943e-02, -7.4534956396e-02, 5.4229446238e-02,
            1.1719914093e-02,  -1.3675570615e-02, -4.7958076003e-02, 1.1531190052e-02,  1.1771245501e-01,
            1.6461312748e-01,  -7.0552056500e-03, 5.3840890576e-02,  2.2439992205e-01,  1.5107685915e-01,
            -7.1678833449e-02, -4.2269928895e-01, -8.5984341700e-02, -8.7208038248e-02, -8.1000745778e-02,
            -5.7191409338e-02, -1.2843342763e-01, 8.0458695388e-02,  7.8892776409e-02,  1.3802567152e-01,
            1.3170392893e-02,  -5.7285356440e-03, 3.2197361190e-02,  -1.3561021177e-01, 4.1279483841e-01,
            -3.1242334792e-01, 1.2018574259e-01,  5.5384063751e-03,  -3.0720313700e-02, -1.3761436155e-01,
            -8.6820046338e-02, -1.3391448701e-02, 2.9623778364e+00,  -5.9805793621e-01, 2.3150180326e-01,
            -3.2794980195e-01, -3.5499392397e-01, 5.2995829488e-02,  -2.8996385875e+00, 9.9508232627e-02,
            7.3153701493e-01,  -1.9705922853e-01, -3.5960321220e-01, 6.9964059568e-01,  -1.0120517405e-02,
            7.5539140001e-02,  -1.6368403754e-01, 5.1668857945e-02,  1.3880449889e-01,  -1.8544576782e-01};
        std::vector<double> grad2_expected = {
            7.1488136708e-01,  -8.3793091999e-01, -2.4162871592e-01, -4.9462958644e-02, 3.6916045768e-01,
            8.0975213393e-02,  -1.9913595940e-01, 7.1230354593e-02,  1.4450824845e-01,  3.8827700452e-01,
            -1.4924966355e+00, 3.3682666607e-03,  1.1288045467e+00,  -1.7837427318e-01, -1.2566304074e+00,
            -3.3166341816e-01, -4.3229427921e-01, 3.5972153302e-01,  -8.3559262457e-01, -1.7076194494e+00,
            1.0105006956e+00,  -5.6062107594e-01, 1.8580733634e-01,  -4.3725183252e-01, -3.1801827148e-01,
            2.9119752894e+00,  -9.5624586204e-02, 7.5206948645e-01,  -4.3827215042e-01, 3.1508745184e-01,
            -1.4075535421e-01, 6.5004433908e-02,  -1.9630785935e-01, -2.0804954169e-01, 1.1719382699e-01,
            -8.6126349896e-02, 4.2397620098e-01,  5.9516417220e-02,  -2.5146467634e-04, -1.9508303966e-02,
            -1.1478286864e-02, -1.3724003304e-01, -2.5329217958e-01, -7.8396634032e-02, 1.1828900516e-01,
            2.0007385625e+00,  -4.3504468355e-01, -1.9824506208e-01, -2.0380220305e+00, 1.6494982213e+00,
            5.5451113423e-02,  -1.5330905508e-01, -1.0104834146e+00, 1.8499354546e-02,  5.9958611560e-02,
            3.3235898875e-01,  3.7294624712e-01,  1.1380529540e-01,  -1.9190583977e-01, -6.7253655025e-02,
            -1.0428245978e-01, 5.4071577548e-02,  -7.2587973696e-02, 3.3955394017e-01,  3.0257542603e-01,
            1.0810716694e+00,  -5.9497569591e-03, -8.4654882566e-02, -2.0431481514e-01, -6.5646238564e-02,
            8.6050163377e-02,  -1.5301597311e-01, -4.8042390450e-01, -1.0455578099e+00, 1.2008107801e+00,
            8.9842356529e-01,  -8.7946635571e-01, 3.8439151694e-01,  -6.1927821148e-01, 2.1348946170e+00,
            1.6042271516e-01,  1.6491560400e-01,  3.9640258800e-02,  2.0711542476e-01,  -8.8546217110e-02,
            -6.6504959983e-02, -9.9621426171e-02, -1.2139330291e-01, 5.0490277679e-02,  -1.3323422396e-01,
            1.2834992925e-01,  -1.5202582882e-01, 3.4078421540e-02,  -1.1598505207e-02, 1.1223836412e-01,
            8.8285290351e-02,  -1.5479001316e-01, 6.4712279624e-02,  -1.5391741503e-01, 1.2181683325e-01,
            -5.8357040572e-02, 1.0545245499e-01,  -2.9681414891e-02, 1.2684755399e-02,  -1.6494737078e-02,
            -1.1612593287e-01, 6.7582543319e-02,  -9.6841668004e-02, 1.0702229065e-01,  -7.4474877260e-01,
            2.8378251209e-01,  -8.2820362393e-02, 1.2646966494e-01,  -1.2021015974e-01, -2.9738278618e-02,
            1.8194861822e-01,  -4.1772418355e-02, -2.8775948154e-01, 1.5997366845e-01,  1.9699673615e+00,
            -3.7823558983e-01, 7.5466560504e-01,  3.9925962021e-02,  1.2336471855e+00,  1.0965130978e+00,
            -2.3836712040e+00, 1.4431569935e-01,  2.7222012031e-01,  -3.7739451187e-01, 4.2887452770e-02,
            5.0658618702e-02,  1.4715497252e-02,  -2.3954693355e-01, -7.2152135610e-02, 1.1985634543e-02};
        std::vector<double> grad3_expected = {
            -3.8764486752e-01, 2.5932076282e-01,  -1.0179753554e-01, -5.1792690727e-01, 9.7128747562e-01,
            5.9649142039e-01,  1.0328249145e+00,  -3.3964055475e-01, -7.5137276698e-01, -1.8937053992e-01,
            4.3068595877e-02,  2.9348322539e-01,  -7.1262971881e-02, 5.5774171497e-02,  -6.4263007232e-02,
            6.0986902000e-01,  5.3815306771e-02,  9.5759297985e-01,  -4.3696035600e-02, 8.8216635454e-02,
            -2.0133082986e-01, -6.9679660618e-03, -6.2155604604e-01, 8.6718453794e-03,  -1.1857896753e+00,
            -3.0263452126e-01, 5.5847976963e-01,  6.3436496922e-02,  1.4886864303e-02,  -3.7617026181e-03,
            7.1724828520e-02,  -8.2962126724e-02, 1.3659968426e-01,  1.1362172008e-02,  -3.7883089574e-02,
            -3.3905654994e-02, 1.5857518108e-01,  1.0481729099e-01,  2.3473517633e-01,  -3.0244156111e-01,
            -1.1576087437e+00, -6.4225676721e-01, 2.0785466942e-01,  -1.0546703009e-01, 1.8258975341e-04};
        std::vector<double> virial_expected = {3.1513023300e-01,  1.8255055693e+00,  -4.3344338955e-01,
                                               1.8255055693e+00,  -4.2971634514e+00, 4.7780827979e-01,
                                               -4.3344338955e-01, 4.7780827979e-01,  -4.6681227881e+00};

        x3b_h2o_ion_v1x_deg4_filtered ph("li+");
        double e_nograd = ph(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e = ph(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-h2o-na") {
        std::vector<double> xyz1 = {
            -1.0695654940e+00, -1.6239523270e+00, -2.5977628240e-01, -1.6372337220e+00, -1.4281547330e+00,
            -1.0129308230e+00, -1.3257667840e+00, -2.4893344620e+00, 5.9173029930e-02,  -1.7765826110e+00,
            2.1211182870e-01,  -2.2850759190e+00, -1.6076762700e+00, 3.9673134650e-02,  -3.2146924790e+00,
            -2.6124275400e+00, 6.8516610220e-01,  -2.2745541860e+00, 1.1557765290e+00,  2.9477075680e+00,
            4.0818352720e-01,  1.9410235610e+00,  2.6842002830e+00,  8.9989145890e-01,  1.1206057440e+00,
            3.9037648930e+00,  4.3935098830e-01,  -1.7765826110e+00, 2.1211182870e-01,  -2.2850759190e+00,
            -1.6076762700e+00, 3.9673134650e-02,  -3.2146924790e+00, -2.6124275400e+00, 6.8516610220e-01,
            -2.2745541860e+00, 1.1557765290e+00,  2.9477075680e+00,  4.0818352720e-01,  1.9410235610e+00,
            2.6842002830e+00,  8.9989145890e-01,  1.1206057440e+00,  3.9037648930e+00,  4.3935098830e-01,
            1.1557765290e+00,  2.9477075680e+00,  4.0818352720e-01,  1.9410235610e+00,  2.6842002830e+00,
            8.9989145890e-01,  1.1206057440e+00,  3.9037648930e+00,  4.3935098830e-01};
        std::vector<double> xyz2 = {
            2.7721678110e+00,  8.2864804120e-01,  1.3407652800e+00,  2.8242223750e+00,  5.9951176200e-01,
            2.2720625940e+00,  3.6577708060e+00,  6.7292239890e-01,  1.0032623000e+00,  2.7721678110e+00,
            8.2864804120e-01,  1.3407652800e+00,  2.8242223750e+00,  5.9951176200e-01,  2.2720625940e+00,
            3.6577708060e+00,  6.7292239890e-01,  1.0032623000e+00,  2.7721678110e+00,  8.2864804120e-01,
            1.3407652800e+00,  2.8242223750e+00,  5.9951176200e-01,  2.2720625940e+00,  3.6577708060e+00,
            6.7292239890e-01,  1.0032623000e+00,  -1.0695654940e+00, -1.6239523270e+00, -2.5977628240e-01,
            -1.6372337220e+00, -1.4281547330e+00, -1.0129308230e+00, -1.3257667840e+00, -2.4893344620e+00,
            5.9173029930e-02,  -1.0695654940e+00, -1.6239523270e+00, -2.5977628240e-01, -1.6372337220e+00,
            -1.4281547330e+00, -1.0129308230e+00, -1.3257667840e+00, -2.4893344620e+00, 5.9173029930e-02,
            -1.7765826110e+00, 2.1211182870e-01,  -2.2850759190e+00, -1.6076762700e+00, 3.9673134650e-02,
            -3.2146924790e+00, -2.6124275400e+00, 6.8516610220e-01,  -2.2745541860e+00};
        std::vector<double> xyz3 = {2.9835636860e-01,  5.8244279830e-01,  -2.3250860780e-01, 2.9835636860e-01,
                                    5.8244279830e-01,  -2.3250860780e-01, 2.9835636860e-01,  5.8244279830e-01,
                                    -2.3250860780e-01, 2.9835636860e-01,  5.8244279830e-01,  -2.3250860780e-01,
                                    2.9835636860e-01,  5.8244279830e-01,  -2.3250860780e-01, 2.9835636860e-01,
                                    5.8244279830e-01,  -2.3250860780e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 6;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 9.9376173287e-02;
        std::vector<double> grad1_expected = {
            -2.3332369888e-01, -4.4555727821e-01, 7.0707374025e-02,  -2.2394204236e-02, 7.6841460921e-02,
            -8.3740461331e-02, 4.6277923047e-02,  6.0756777575e-02,  7.1336547479e-04,  -5.4746284472e-02,
            -8.6160421189e-02, -4.9927928401e-03, 6.8823116084e-02,  -1.0066173311e-02, -7.8418426961e-02,
            -7.0521414625e-02, 7.7311342294e-02,  4.0520912373e-02,  5.1946626201e-01,  -3.4399800537e-01,
            3.2345063798e-01,  -7.4559814764e-01, -8.9441718563e-01, -5.3524666126e-01, 1.4043564482e-01,
            6.9324464110e-02,  9.5005731349e-02,  -6.7647488896e-01, -5.6414190888e-01, -3.2069433086e-01,
            6.2043268262e-01,  -3.8573174445e-01, -2.7046359831e-01, -3.4286218631e-01, 2.4992235145e-01,
            6.3937293529e-01,  1.5031963115e-01,  6.1548301664e-01,  8.8416830072e-02,  4.9355232595e-03,
            -8.9419151561e-02, 8.6924899587e-03,  -1.6749333987e-02, -1.1620975334e-01, -1.1815652453e-02,
            9.1924240202e-02,  4.8956155077e-01,  9.9200319060e-02,  6.5151051182e-02,  -9.0147503992e-02,
            3.2363129570e-02,  -2.5407485737e-02, -6.7251851571e-02, -2.5791689598e-02};
        std::vector<double> grad2_expected = {
            2.4648998435e-01,  1.6952130670e-01,  1.8079888029e-01,  -1.2711236765e-01, -5.2533854228e-02,
            4.3697888693e-02,  3.6080538193e-02,  -5.0393073741e-02, -1.4004961299e-01, 3.7480591944e-02,
            9.1533423207e-02,  2.5739285365e-02,  -4.2258028660e-02, -4.2214096120e-02, 9.4997267428e-02,
            6.1993158956e-02,  -3.0644046855e-02, -7.8773008438e-02, 5.8550772590e-01,  5.9245198582e-01,
            4.2999971252e-01,  -5.9302504475e-01, 4.1327145726e-02,  4.8520819204e-01,  2.1827443416e-01,
            1.2472416614e-01,  -7.3734430495e-01, -3.3223327287e-01, 2.8573146721e-01,  -5.4843250324e-01,
            8.8406976770e-01,  8.5132385363e-01,  3.6885803601e-01,  -1.3673522770e-01, -7.0476265384e-02,
            -9.9452119182e-02, -2.6639909277e-01, -5.7935898561e-01, 5.1065692599e-02,  1.2632523368e-02,
            8.3952126412e-02,  -2.9802613431e-02, 4.3208918603e-02,  1.0797551166e-01,  -1.9978428218e-02,
            -2.6500265902e-01, -1.6376005278e-01, -1.5855540899e-01, 1.3713660799e-01,  -3.5123814941e-03,
            -6.8091171235e-02, -1.4105744247e-03, 9.2530399864e-02,  1.1073862096e-01};
        std::vector<double> grad3_expected = {
            5.3981825174e-02,  2.4136466098e-01,  -7.2127434163e-02, -7.7113922743e-04, 2.3997197389e-04,
            9.2676307361e-04,  -1.2506087450e-01, 4.1058742920e-01,  -6.1073307679e-02, -1.6196874474e-02,
            -3.6662775357e-01, 2.3081158029e-01,  7.2051830378e-02,  -2.2422764202e-02, -8.6578318529e-02,
            -2.3911801948e-03, -2.5742016080e-01, 1.0136200228e-02};
        std::vector<double> virial_expected = {-2.1340707444e+00, -1.0386415637e+00, 6.0754488203e-01,
                                               -1.0386415637e+00, 7.6760651908e-01,  -1.7212173922e+00,
                                               6.0754488203e-01,  -1.7212173922e+00, -1.5895108310e+00};

        x3b_h2o_ion_v1x_deg4_filtered ph("na+");
        double e_nograd = ph(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e = ph(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-h2o-k") {
        std::vector<double> xyz1 = {
            -2.4596215450e+00, -1.3307686320e+00, 1.6558717950e-01,  -2.8502305670e+00, -1.6982123550e+00,
            9.6207666330e-01,  -2.9142201740e+00, -1.7646650170e+00, -5.6040167150e-01, 2.3868934490e+00,
            -3.1455907500e+00, 4.1103255950e-02,  2.7089746660e+00,  -3.6401137150e+00, -7.1446525680e-01,
            2.7729279050e+00,  -3.5736975860e+00, 8.0718092060e-01,  -1.9864852310e+00, 1.5070138030e+00,
            2.1838885590e-02,  -2.7550924640e+00, 2.0773876700e+00,  2.9247198430e-02,  -2.3285334330e+00,
            6.0539680420e-01,  7.5568470150e-02,  4.6520682970e-01,  -1.0776665390e+00, 3.1610115760e-02,
            -4.3418595880e-01, -1.4146126480e+00, 8.4118085910e-02,  1.0496754310e+00,  -1.8470977930e+00,
            4.0629166460e-02,  2.3868934490e+00,  -3.1455907500e+00, 4.1103255950e-02,  2.7089746660e+00,
            -3.6401137150e+00, -7.1446525680e-01, 2.7729279050e+00,  -3.5736975860e+00, 8.0718092060e-01,
            -1.9864852310e+00, 1.5070138030e+00,  2.1838885590e-02,  -2.7550924640e+00, 2.0773876700e+00,
            2.9247198430e-02,  -2.3285334330e+00, 6.0539680420e-01,  7.5568470150e-02,  4.6520682970e-01,
            -1.0776665390e+00, 3.1610115760e-02,  -4.3418595880e-01, -1.4146126480e+00, 8.4118085910e-02,
            1.0496754310e+00,  -1.8470977930e+00, 4.0629166460e-02,  -1.9864852310e+00, 1.5070138030e+00,
            2.1838885590e-02,  -2.7550924640e+00, 2.0773876700e+00,  2.9247198430e-02,  -2.3285334330e+00,
            6.0539680420e-01,  7.5568470150e-02,  4.6520682970e-01,  -1.0776665390e+00, 3.1610115760e-02,
            -4.3418595880e-01, -1.4146126480e+00, 8.4118085910e-02,  1.0496754310e+00,  -1.8470977930e+00,
            4.0629166460e-02,  4.6520682970e-01,  -1.0776665390e+00, 3.1610115760e-02,  -4.3418595880e-01,
            -1.4146126480e+00, 8.4118085910e-02,  1.0496754310e+00,  -1.8470977930e+00, 4.0629166460e-02};
        std::vector<double> xyz2 = {
            2.6597493150e+00,  3.7459420900e+00,  -2.7115058550e-01, 3.6168052410e+00,  3.6909856940e+00,
            -3.0898568140e-01, 2.4756964800e+00,  4.6869357310e+00,  -3.0447203980e-01, 2.6597493150e+00,
            3.7459420900e+00,  -2.7115058550e-01, 3.6168052410e+00,  3.6909856940e+00,  -3.0898568140e-01,
            2.4756964800e+00,  4.6869357310e+00,  -3.0447203980e-01, 2.6597493150e+00,  3.7459420900e+00,
            -2.7115058550e-01, 3.6168052410e+00,  3.6909856940e+00,  -3.0898568140e-01, 2.4756964800e+00,
            4.6869357310e+00,  -3.0447203980e-01, 2.6597493150e+00,  3.7459420900e+00,  -2.7115058550e-01,
            3.6168052410e+00,  3.6909856940e+00,  -3.0898568140e-01, 2.4756964800e+00,  4.6869357310e+00,
            -3.0447203980e-01, -2.4596215450e+00, -1.3307686320e+00, 1.6558717950e-01,  -2.8502305670e+00,
            -1.6982123550e+00, 9.6207666330e-01,  -2.9142201740e+00, -1.7646650170e+00, -5.6040167150e-01,
            -2.4596215450e+00, -1.3307686320e+00, 1.6558717950e-01,  -2.8502305670e+00, -1.6982123550e+00,
            9.6207666330e-01,  -2.9142201740e+00, -1.7646650170e+00, -5.6040167150e-01, -2.4596215450e+00,
            -1.3307686320e+00, 1.6558717950e-01,  -2.8502305670e+00, -1.6982123550e+00, 9.6207666330e-01,
            -2.9142201740e+00, -1.7646650170e+00, -5.6040167150e-01, 2.3868934490e+00,  -3.1455907500e+00,
            4.1103255950e-02,  2.7089746660e+00,  -3.6401137150e+00, -7.1446525680e-01, 2.7729279050e+00,
            -3.5736975860e+00, 8.0718092060e-01,  2.3868934490e+00,  -3.1455907500e+00, 4.1103255950e-02,
            2.7089746660e+00,  -3.6401137150e+00, -7.1446525680e-01, 2.7729279050e+00,  -3.5736975860e+00,
            8.0718092060e-01,  -1.9864852310e+00, 1.5070138030e+00,  2.1838885590e-02,  -2.7550924640e+00,
            2.0773876700e+00,  2.9247198430e-02,  -2.3285334330e+00, 6.0539680420e-01,  7.5568470150e-02};
        std::vector<double> xyz3 = {
            7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,
            -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,
            1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02,
            7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,
            -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,
            1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 10;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 2.5355343818e-02;
        std::vector<double> grad1_expected = {
            -1.6981627453e-02, -1.5419398039e-02, 1.3867510301e-03,  1.6498126986e-03,  1.5138870420e-03,
            -7.8964550635e-04, 1.7046068795e-03,  1.5707903073e-03,  5.1402214141e-04,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  -3.1981619995e-01, -3.1201198394e-02,
            1.4803490807e-02,  1.2901852169e-01,  -1.2243935551e-02, -4.8882861856e-03, 6.6138479731e-02,
            2.0174763269e-04,  -2.7885522802e-03, -6.0467473940e-02, -2.6544658683e-01, 1.4127284427e-02,
            2.8544252557e-02,  8.0110654128e-02,  -4.6963393320e-03, -1.2064246479e-02, 1.4309234766e-01,
            -5.7385993844e-03, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            -4.2500158413e-01, -3.5169444259e-01, 3.3212765483e-02,  5.1409378099e-01,  -1.5258775063e-01,
            -1.4946673536e-02, 1.7353057172e-01,  4.1014347505e-01,  -2.5195258628e-02, -2.1353130275e-02,
            -1.7784006219e-01, 8.6600483363e-03,  2.7071732302e-02,  5.0468743129e-02,  -3.3406284856e-03,
            -2.1544551618e-01, 4.7644550518e-01,  -1.1740834995e-02, 0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  -2.5919639356e-01, -1.5876530051e-02, 1.1587264027e-02,
            5.4814745241e-01,  4.3114813677e-01,  -4.1857185165e-02, -9.1408854382e-01, 4.8217448083e-01,
            1.7372697101e-02,  -2.8711118773e-01, -1.0309814788e+00, 5.7066842741e-02,  6.5696856378e-01,
            1.3579641872e-01,  -3.3539338148e-02, 5.4364359013e-02,  -1.5826137669e-01, 4.6228937785e-03};
        std::vector<double> grad2_expected = {
            -1.0878225310e-04, -1.5138516261e-04, 1.1178509130e-05,  1.6074186234e-03,  -1.8597688939e-04,
            -5.9458529340e-05, -4.0720668700e-04, 1.5743937699e-03,  -5.1578010298e-05, 0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  2.8537104158e-01,  2.2504505842e-01,
            -2.1815760815e-02, -7.5664657341e-02, -6.7062978712e-02, 6.1070375039e-03,  -8.7234691058e-02,
            -8.9067515103e-02, 7.5536350149e-03,  1.7970507149e-01,  2.8035170403e-01,  -1.9787765399e-02,
            -1.0036444675e-01, -1.0332877384e-01, 8.7276420649e-03,  -6.7077464306e-02, -1.0133718521e-01,
            7.2414788689e-03,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            -4.2801688396e-01, 1.3314942023e-01,  1.2178079713e-02,  2.6174790150e-01,  2.6093162796e-02,
            9.2769432550e-02,  2.5296136862e-01,  1.6968594823e-02,  -1.1628224571e-01, 1.3987272598e-01,
            -5.3546355371e-01, 1.7492888725e-02,  7.2763745550e-02,  2.7797254364e-01,  9.5825104577e-02,
            6.3465899017e-02,  2.6831677076e-01,  -1.2539505221e-01, 0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  1.2926169881e-02,  -2.4098349621e-01, 9.9748548662e-03,
            2.8335136616e-01,  -5.5596681872e-02, -3.0661289533e-01, 3.0823615351e-01,  -2.9753499348e-02,
            2.8547443122e-01,  -9.7521093906e-01, -3.8529355534e-02, 4.2669680992e-02,  -1.4273655404e-01,
            -2.2107500203e-03, 6.0954953078e-03,  1.8836711245e-01,  5.7713769600e-01,  -3.3107656879e-02};
        std::vector<double> grad3_expected = {
            1.2535778191e-02,  1.1097688971e-02,  -1.0112696346e-03, 0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  2.1875053486e-03,  -2.5671178295e-02, 1.0284359550e-03,  3.1724307425e-02,
            -3.3442159938e-02, 1.2629875513e-04,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            -3.4931515474e-01, -8.2072459687e-02, 1.8263900125e-02,  -6.6375456400e-02, -3.5989994681e-01,
            1.8498474056e-02,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  2.0623795406e-02,
            -5.7111241013e-01, 2.4060833280e-02,  5.0535864559e-01,  5.1704884633e-01,  -4.3807917792e-02};
        std::vector<double> virial_expected = {7.4239608992e-02,  2.6660612739e+00,  -1.5226653399e-01,
                                               2.6660612739e+00,  1.4097937260e+00,  -2.0762806803e-01,
                                               -1.5226653399e-01, -2.0762806803e-01, -7.6441171691e-01};

        x3b_h2o_ion_v1x_deg4_filtered ph("k+");
        double e_nograd = ph(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e = ph(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-h2o-rb") {
        std::vector<double> xyz1 = {
            4.3383131180e+00,  3.2390181030e+00,  1.3325160940e-02,  5.2925835800e+00,  3.2442801940e+00,
            1.1175183190e-01,  4.1012507660e+00,  4.1572127150e+00,  -1.2194934820e-01, 4.6436621030e+00,
            -1.4024067280e+00, 2.4136366790e-04,  4.5256285580e+00,  -2.3519501630e+00, 5.3458050450e-02,
            5.5915768500e+00,  -1.2763749330e+00, -7.0366450030e-02, -1.8876071560e+00, 2.6224545870e+00,
            2.5212264120e-02,  -1.6560764410e+00, 3.5536300830e+00,  5.1521484640e-03,  -2.8474164440e+00,
            2.6208630030e+00,  2.6359847690e-02,  2.7843038080e+00,  7.9342504490e-01,  3.1934423250e-02,
            3.4447677520e+00,  8.8767431400e-02,  1.9511609960e-02,  3.2945296650e+00,  1.6135004810e+00,
            2.3451158790e-02,  4.6436621030e+00,  -1.4024067280e+00, 2.4136366790e-04,  4.5256285580e+00,
            -2.3519501630e+00, 5.3458050450e-02,  5.5915768500e+00,  -1.2763749330e+00, -7.0366450030e-02,
            -1.8876071560e+00, 2.6224545870e+00,  2.5212264120e-02,  -1.6560764410e+00, 3.5536300830e+00,
            5.1521484640e-03,  -2.8474164440e+00, 2.6208630030e+00,  2.6359847690e-02,  2.7843038080e+00,
            7.9342504490e-01,  3.1934423250e-02,  3.4447677520e+00,  8.8767431400e-02,  1.9511609960e-02,
            3.2945296650e+00,  1.6135004810e+00,  2.3451158790e-02,  -1.8876071560e+00, 2.6224545870e+00,
            2.5212264120e-02,  -1.6560764410e+00, 3.5536300830e+00,  5.1521484640e-03,  -2.8474164440e+00,
            2.6208630030e+00,  2.6359847690e-02,  2.7843038080e+00,  7.9342504490e-01,  3.1934423250e-02,
            3.4447677520e+00,  8.8767431400e-02,  1.9511609960e-02,  3.2945296650e+00,  1.6135004810e+00,
            2.3451158790e-02,  2.7843038080e+00,  7.9342504490e-01,  3.1934423250e-02,  3.4447677520e+00,
            8.8767431400e-02,  1.9511609960e-02,  3.2945296650e+00,  1.6135004810e+00,  2.3451158790e-02};
        std::vector<double> xyz2 = {
            -1.7428543420e+00, -4.0415235570e+00, -5.2281327930e-02, -1.2799702230e+00, -4.8814607780e+00,
            -5.5259141270e-02, -2.6687334400e+00, -4.2895947640e+00, -8.5281669350e-02, -1.7428543420e+00,
            -4.0415235570e+00, -5.2281327930e-02, -1.2799702230e+00, -4.8814607780e+00, -5.5259141270e-02,
            -2.6687334400e+00, -4.2895947640e+00, -8.5281669350e-02, -1.7428543420e+00, -4.0415235570e+00,
            -5.2281327930e-02, -1.2799702230e+00, -4.8814607780e+00, -5.5259141270e-02, -2.6687334400e+00,
            -4.2895947640e+00, -8.5281669350e-02, -1.7428543420e+00, -4.0415235570e+00, -5.2281327930e-02,
            -1.2799702230e+00, -4.8814607780e+00, -5.5259141270e-02, -2.6687334400e+00, -4.2895947640e+00,
            -8.5281669350e-02, 4.3383131180e+00,  3.2390181030e+00,  1.3325160940e-02,  5.2925835800e+00,
            3.2442801940e+00,  1.1175183190e-01,  4.1012507660e+00,  4.1572127150e+00,  -1.2194934820e-01,
            4.3383131180e+00,  3.2390181030e+00,  1.3325160940e-02,  5.2925835800e+00,  3.2442801940e+00,
            1.1175183190e-01,  4.1012507660e+00,  4.1572127150e+00,  -1.2194934820e-01, 4.3383131180e+00,
            3.2390181030e+00,  1.3325160940e-02,  5.2925835800e+00,  3.2442801940e+00,  1.1175183190e-01,
            4.1012507660e+00,  4.1572127150e+00,  -1.2194934820e-01, 4.6436621030e+00,  -1.4024067280e+00,
            2.4136366790e-04,  4.5256285580e+00,  -2.3519501630e+00, 5.3458050450e-02,  5.5915768500e+00,
            -1.2763749330e+00, -7.0366450030e-02, 4.6436621030e+00,  -1.4024067280e+00, 2.4136366790e-04,
            4.5256285580e+00,  -2.3519501630e+00, 5.3458050450e-02,  5.5915768500e+00,  -1.2763749330e+00,
            -7.0366450030e-02, -1.8876071560e+00, 2.6224545870e+00,  2.5212264120e-02,  -1.6560764410e+00,
            3.5536300830e+00,  5.1521484640e-03,  -2.8474164440e+00, 2.6208630030e+00,  2.6359847690e-02};
        std::vector<double> xyz3 = {
            -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,
            7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02,
            2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,
            -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,
            7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02,
            2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 10;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 3.1110481333e-01;
        std::vector<double> grad1_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  4.7217818786e-01,
            -6.6211861101e-01, -3.7500122801e-02, -1.1078038767e+00, 6.7635648330e-01,  2.2115418854e-02,
            1.6812232853e-01,  -8.5422286328e-01, 5.5782464139e-02,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  7.2710965932e-01,  4.0901279839e-01,  -3.3587053573e-03,
            -7.2697840320e-04, 8.3300915153e-01,  -2.0248325581e-02, -8.3877176982e-01, -8.4888226684e-01,
            1.2574829255e-02,  1.8401061457e-01,  3.3678189071e-02,  -2.2811606712e-03, -5.2826693800e-02,
            -3.6682135287e-02, 7.7587150175e-04,  -8.5139130622e-02, -2.7802836679e-02, 1.8629368017e-03};
        std::vector<double> grad2_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  -7.2535651211e-01,
            5.1624086377e-01,  -1.5585001225e-01, 6.1149734388e-01,  7.1003198206e-02,  8.4336850287e-02,
            1.9035372531e-01,  1.8828548958e-01,  3.3224829184e-02,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  -8.6213410434e-01, -4.8127558951e-01, 9.5663035749e-02,
            9.1228348457e-03,  -2.0347665941e-01, -1.4785700648e-03, 7.2684092000e-01,  2.6466272658e-01,
            -7.6810043292e-02, -1.2300002212e-01, 1.7594799347e-01,  -3.4798163676e-03, 3.3468431627e-02,
            -1.3408016301e-01, 2.8952026191e-03,  3.8048987442e-02,  -4.7839274378e-02, 1.0251648849e-03};
        std::vector<double> grad3_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 3.9100880322e-01, 6.4455439424e-02,
            -2.1094274136e-03, 0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00, 2.3855943839e-01,
            2.6949839264e-02,  -6.3422207093e-03, 5.4378129074e-03, 3.6778226812e-02, -7.9819876866e-04};
        std::vector<double> virial_expected = {1.6111801806e+00,  -4.9488125217e-01, -3.3916786994e-02,
                                               -4.9488125217e-01, -5.5216998736e-01, 4.4564577774e-02,
                                               -3.3916786994e-02, 4.4564577774e-02,  -8.2287579237e-03};

        x3b_h2o_ion_v1x_deg4_filtered ph("rb+");
        double e_nograd = ph(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e = ph(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-h2o-cs") {
        std::vector<double> xyz1 = {4.6423862440e+00,  4.8846581920e+00,  -7.9319686810e-01, 5.5888140450e+00,
                                    5.0435305540e+00,  -8.1230703670e-01, 4.2565170720e+00,  5.7495827880e+00,
                                    -9.4900547950e-01, 4.6423862440e+00,  4.8846581920e+00,  -7.9319686810e-01,
                                    5.5888140450e+00,  5.0435305540e+00,  -8.1230703670e-01, 4.2565170720e+00,
                                    5.7495827880e+00,  -9.4900547950e-01, 1.9577742960e+00,  1.6498238250e+00,
                                    -2.6803938630e+00, 2.2798079560e+00,  1.0538141290e+00,  -3.3606054190e+00,
                                    1.0744078230e+00,  1.8837916330e+00,  -2.9741712370e+00};
        std::vector<double> xyz2 = {1.9577742960e+00,  1.6498238250e+00,  -2.6803938630e+00, 2.2798079560e+00,
                                    1.0538141290e+00,  -3.3606054190e+00, 1.0744078230e+00,  1.8837916330e+00,
                                    -2.9741712370e+00, 2.4393531210e+00,  2.5097433290e+00,  4.9496556790e+00,
                                    2.9546761940e+00,  2.0407189980e+00,  5.6082326770e+00,  1.7445021530e+00,
                                    2.9323398370e+00,  5.4575274610e+00,  2.4393531210e+00,  2.5097433290e+00,
                                    4.9496556790e+00,  2.9546761940e+00,  2.0407189980e+00,  5.6082326770e+00,
                                    1.7445021530e+00,  2.9323398370e+00,  5.4575274610e+00};
        std::vector<double> xyz3 = {3.2822936850e+00, 2.4725476270e+00, -3.3685766700e-01,
                                    3.2822936850e+00, 2.4725476270e+00, -3.3685766700e-01,
                                    3.2822936850e+00, 2.4725476270e+00, -3.3685766700e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 7.3272385776e-02;
        std::vector<double> grad1_expected = {9.3949334810e-02, -1.2564575754e-01, 1.8681934406e-01,  -7.2295764517e-02,
                                              1.4944555455e-02, 1.5768804636e-02,  -2.3486089594e-02, -5.4763691067e-02,
                                              1.1704659904e-02, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> grad2_expected = {-1.6339185807e-02, -1.0387934630e-01, 1.6179388979e-01, -1.5427240398e-02,
                                              2.7195332108e-02,  5.0789917644e-02,  4.8197954569e-02, -2.2726691873e-02,
                                              3.3039164227e-02,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> grad3_expected = {-1.4599009063e-02, 2.6487559922e-01, -4.5991578026e-01,
                                              0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> virial_expected = {1.3116098508e-01, 2.9266789438e-02,  3.6296393155e-02,
                                               2.9266789438e-02, 3.8385024531e-01,  -3.0490577546e-01,
                                               3.6296393155e-02, -3.0490577546e-01, 7.1979561287e-01};

        x3b_h2o_ion_v1x_deg4_filtered ph("cs+");
        double e_nograd = ph(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e = ph(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x3b-v2x::class") {
    SECTION("h2o-h2o-h2o") {
        std::vector<double> xyz1 = {1.7474594210e+00, -1.4737233740e+00, -1.3296783390e+00,
                                    2.2444822770e+00, -1.7974450600e+00, -2.0835207810e+00,
                                    1.0684055850e+00, -2.1338516330e+00, -1.1304174950e+00};
        std::vector<double> xyz2 = {7.4372664830e-01, 1.3745066180e+00, 8.2228117080e-01,
                                    1.5966080550e+00, 9.4041883440e-01, 1.0251293750e+00,
                                    8.4611620640e-01, 2.3077049320e+00, 1.0097486520e+00};
        std::vector<double> xyz3 = {2.8905925930e+00, -1.7061104380e-01, 9.3095149180e-01,
                                    2.7240924310e+00, -7.8786885510e-01, 2.0726128570e-01,
                                    3.3243787470e+00, -6.6848224410e-01, 1.6262376630e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.4662741082e-01;
        std::vector<double> grad1_expected = {2.1927978974e-01,  8.2950770558e-02, 1.3988974052e-01,
                                              -3.9453362980e-02, 9.6759816186e-02, -7.3546733882e-02,
                                              -1.5474118083e-01, 6.1389929264e-02, 1.2232496613e-01};
        std::vector<double> grad2_expected = {6.3898576531e-01,  -3.4730981743e-02, 1.2680998130e-01,
                                              -3.3706509023e-01, 2.9522121369e-01,  -9.9298643677e-02,
                                              -2.2628766785e-01, -1.2899565153e-01, -5.5647030787e-02};
        std::vector<double> grad3_expected = {-5.9237616224e-01, -5.6596810222e-01, -2.3829943419e-02,
                                              4.2936379576e-01,  -1.1201147521e-01, 3.2738708264e-02,
                                              6.2294113326e-02,  3.0538448101e-01,  -1.6944104445e-01};
        std::vector<double> virial_expected = {4.6069419532e-01, 1.6180409648e-01, 4.4422852291e-01,
                                               1.6180409648e-01, 5.1428756147e-01, 2.9094380669e-01,
                                               4.4422852291e-01, 2.9094380669e-01, 5.1571096490e-01};

        x2o::x3b_v2x ph;
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("::struct") {
    SECTION("n2o5-h2o-h2o") {
        std::vector<double> xyz1 = {
            8.2680440000e+00, 1.0783073000e+01, 1.0117501000e+01, 9.3865410000e+00, 1.1620399000e+01, 1.0436688000e+01,
            8.3598230000e+00, 9.4073400000e+00, 9.8844810000e+00, 1.0525770000e+01, 1.1253619000e+01, 1.0065799000e+01,
            9.0863690000e+00, 1.2796432000e+01, 1.0771404000e+01, 9.3304000000e+00, 8.9002400000e+00, 1.0425331000e+01,
            7.2746500000e+00, 8.8236580000e+00, 9.7258010000e+00};
        std::vector<double> xyz2 = {9.7496420000e+00, 1.2183527000e+01, 7.3906700000e+00,
                                    1.0555377000e+01, 1.2319729000e+01, 7.8897220000e+00,
                                    1.0049082000e+01, 1.1915591000e+01, 6.5216610000e+00};
        std::vector<double> xyz3 = {1.0785064000e+01, 1.1798443000e+01, 4.9853560000e+00,
                                    1.1395789000e+01, 1.2325561000e+01, 4.4696820000e+00,
                                    1.1168816000e+01, 1.0921351000e+01, 4.9884520000e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        size_t ntrim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.0921723601e-01;
        std::vector<double> grad1_expected = {
            3.9701702165e-01,  9.4431286256e-01,  2.0926085554e-01,  -1.1828330492e+00, -7.5752865090e-01,
            7.8722268986e-02,  1.5378708673e-01,  -7.6324429739e-01, -1.5487215470e-02, 7.6341424588e-01,
            -4.8991523254e-02, -5.3912408360e-01, -8.0314024289e-02, 4.8484244070e-01,  6.2249357440e-02,
            -2.4951303751e-01, 1.9111069701e-01,  -1.4695459655e-01, 1.8166461704e-01,  1.2171179892e-01,
            2.5973864427e-02};
        std::vector<double> grad2_expected = {4.7213081029e-01,  -1.2240936809e-01, 1.4206921778e-01,
                                              -3.2017368720e-01, -8.0366275159e-02, 2.3370971053e-01,
                                              -1.5245445036e-03, 6.5278184907e-03,  -1.8892500660e-01};
        std::vector<double> grad3_expected = {-1.2445776493e-01, 1.2407444297e-01,  1.6663883134e-01,
                                              -1.1136017363e-02, 6.2086985547e-02,  -6.6824398270e-02,
                                              1.9383434216e-03,  -1.6212693141e-01, 3.8691194443e-02};
        std::vector<double> virial_expected = {4.7458144898e-01, 8.6643700793e-01,  5.3030869973e-01,
                                               8.6643700793e-01, -6.8231777999e-01, -6.0027372468e-01,
                                               5.3030869973e-01, -6.0027372468e-01, 8.6305601307e-01};

        mbnrg_A1B2C4_D1E2_D1E2_deg3::mbnrg_A1B2C4_D1E2_D1E2_deg3_v1 ph("n2o5", "h2o", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
        double e =
            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("::struct") {
//    SECTION("- -") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> xyz3 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        std::vector<double> grad3(xyz3.size(), 0.0);
//        size_t ntrim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> grad3_expected = {};
//        std::vector<double> virial_expected = {};
//
//        XX ph("","","");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), ntrim);
//        double e =
//            ph.eval(xyz1.data(), xyz2.data(), xyz3.data(), grad1.data(), grad2.data(), grad3.data(), ntrim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}
