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
#include "potential/2b/x2b_A1B2_A1B2_v1x.h"
#include "potential/2b/x2b_A1B2Z2_C1D2_v1x.h"
#include "potential/2b/x2b_A1B2Z2_C1D4_deg3_exp0_v1x.h"
#include "potential/2b/x2b-A1B2Z2_C-v1x.h"
#include "potential/2b/x2b_A1B4_A1B4_deg4_exp0_v1x.h"
#include "potential/2b/x2b-v9x.h"
#include "potential/2b/mbnrg_2b_A1B4_C1D2_deg4_v1.h"
#include "potential/2b/mbnrg_2b_A1_B1C2X2_deg5_v1.h"
#include "potential/2b/mbnrg_2b_A2_A2_deg6_v1.h"
#include "potential/2b/mbnrg_2b_A1B2Z2_C2_deg5_v1.h"
#include "potential/2b/mbnrg_2b_A1B3_A1B3_deg4_v1.h"
#include "potential/2b/mbnrg_2b_A1_A1_deg9_v1.h"
#include "potential/2b/mbnrg_2b_A1_B1_deg9_v1.h"
#include "potential/2b/mbnrg_2b_A1_B2_deg7_v1.h"
#include "potential/2b/x2b_A1B2C4_D1E2_deg3_v1x.h"
#include "potential/2b/mbnrg_2b_A1B2Z2_A1B2Z2_deg4_vmbpbe.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("mbnrg_A1_B1C2X2_deg5_v1::struct") {
    SECTION("f--h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                    0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00,  -1.9611387200e-03, -7.5739101100e-01,
                                    2.0000000000e+00,  -5.8831383900e-01, 7.5739101100e-01,  2.0000000000e+00,
                                    -5.8831383900e-01, 0.0000000000e+00,  4.0000000000e+00,  3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00,  2.5883138390e+00,  7.5739101100e-01,
                                    4.0000000000e+00,  2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 3.2526133935e+01;
        std::vector<double> grad1_expected = {2.1316282073e-14,  1.1176544442e+02,  -1.1554661995e+00,
                                              -1.1518563880e-15, -8.5312690233e-02, -6.3127695748e-02};
        std::vector<double> grad2_expected = {
            -1.4210854715e-14, -1.0819806107e+02, 3.5405481483e+00,  3.3114770161e+00,  -1.7836916734e+00,
            -1.1925409744e+00, -3.3114770161e+00, -1.7836916734e+00, -1.1925409744e+00, -4.7739590059e-15,
            7.6987689223e-02,  -1.2985548301e-01, 3.0776795131e-01,  4.1625005051e-03,  9.6491589379e-02,
            -3.0776795131e-01, 4.1625005051e-03,  9.6491589379e-02};
        std::vector<double> virial_expected = {5.4823672099e+00,  3.5787345309e-14,  -5.0237591864e-15,
                                               3.5787345309e-14,  2.2318963808e+02,  -2.5634431820e+00,
                                               -5.0237591864e-15, -2.5634431820e+00, -1.5061677948e+00};

        mbnrg_A1_B1C2X2_deg5::mbnrg_A1_B1C2X2_deg5_v1 ph("f-", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("cl--h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                    0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00,  -1.9611387200e-03, -7.5739101100e-01,
                                    2.0000000000e+00,  -5.8831383900e-01, 7.5739101100e-01,  2.0000000000e+00,
                                    -5.8831383900e-01, 0.0000000000e+00,  4.0000000000e+00,  3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00,  2.5883138390e+00,  7.5739101100e-01,
                                    4.0000000000e+00,  2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 7.7808932932e+01;
        std::vector<double> grad1_expected = {2.8421709430e-14, 2.1317812824e+02, -1.1183023395e+01,
                                              2.5326962749e-16, 5.8386269734e-02, 4.0040100139e-02};
        std::vector<double> grad2_expected = {
            -3.5527136788e-14, -1.7574677524e+02, 1.0719877278e+00,  1.0862404821e+01,  -1.8715676501e+01,
            5.0555178334e+00,  -1.0862404821e+01, -1.8715676501e+01, 5.0555178334e+00,  0.0000000000e+00,
            -2.1954613618e-02, -6.3496134117e-01, 5.4404811368e-01,  -1.8215828058e-02, 2.9746062052e-01,
            -5.4404811368e-01, -1.8215828058e-02, 2.9746062052e-01};
        std::vector<double> virial_expected = {1.7278289840e+01, 5.9629384763e-14,  5.9952043330e-15,
                                               5.9629384763e-14, 4.2658980157e+02,  -2.2205886389e+01,
                                               5.9952043330e-15, -2.2205886389e+01, 6.3156056683e+00};

        mbnrg_A1_B1C2X2_deg5::mbnrg_A1_B1C2X2_deg5_v1 ph("cl-", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("br--h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                    0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00,  -1.9611387200e-03, -7.5739101100e-01,
                                    2.0000000000e+00,  -5.8831383900e-01, 7.5739101100e-01,  2.0000000000e+00,
                                    -5.8831383900e-01, 0.0000000000e+00,  4.0000000000e+00,  3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00,  2.5883138390e+00,  7.5739101100e-01,
                                    4.0000000000e+00,  2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 9.3748952002e+01;
        std::vector<double> grad1_expected = {-6.3948846218e-14, 2.3778538268e+02,  -1.4547567486e+01,
                                              1.1102230246e-15,  -1.9680060498e-02, 3.5289986718e-04};
        std::vector<double> grad2_expected = {
            0.0000000000e+00,  -1.8896015371e+02, -4.0737030763e+00, 1.5997161014e+01, -2.4412614487e+01,
            9.3106352810e+00,  -1.5997161014e+01, -2.4412614487e+01, 9.3106352810e+00, 7.2164496601e-15,
            -1.2715941746e-01, -1.4942128513e-01, 3.0966884316e-01,  7.3419738977e-02, 7.4534192634e-02,
            -3.0966884316e-01, 7.3419738977e-02,  7.4534192634e-02};
        std::vector<double> virial_expected = {2.4701292704e+01,  -1.2352618928e-13, 3.8191672047e-14,
                                               -1.2352618928e-13, 4.7549204512e+02,  -2.9093723372e+01,
                                               3.8191672047e-14,  -2.9093723372e+01, 1.1009590165e+01};

        mbnrg_A1_B1C2X2_deg5::mbnrg_A1_B1C2X2_deg5_v1 ph("br-", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("i--h2o") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                    0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00,  2.0000000000e+00,  -1.9611387200e-03, -7.5739101100e-01,
                                    2.0000000000e+00,  -5.8831383900e-01, 7.5739101100e-01,  2.0000000000e+00,
                                    -5.8831383900e-01, 0.0000000000e+00,  4.0000000000e+00,  3.0000000000e+00,
                                    -7.5739101100e-01, 4.0000000000e+00,  2.5883138390e+00,  7.5739101100e-01,
                                    4.0000000000e+00,  2.5883138390e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.1310213932e+02;
        std::vector<double> grad1_expected = {-3.1974423109e-14, 2.4259612514e+02, -2.2049230482e+01,
                                              1.1657341759e-15,  4.8105557535e-02, 5.2604295914e-02};
        std::vector<double> grad2_expected = {
            -4.9737991503e-14, -1.6819944156e+02, -1.5694546352e+01, 3.1472105448e+01, -3.7198341790e+01,
            1.8871888417e+01,  -3.1472105448e+01, -3.7198341790e+01, 1.8871888417e+01, 3.7747582837e-15,
            -2.0866599729e-01, -2.8444611099e-01, 2.9563378241e-01,  8.0280219877e-02, 1.1592090754e-01,
            -2.9563378241e-01, 8.0280219876e-02,  1.1592090754e-01};
        std::vector<double> virial_expected = {4.8121200265e+01,  -5.9410809605e-14, 4.9307780081e-14,
                                               -5.9410809605e-14, 4.8538467252e+02,  -4.3888043780e+01,
                                               4.9307780081e-14,  -4.3888043780e+01, 2.2427666019e+01};

        mbnrg_A1_B1C2X2_deg5::mbnrg_A1_B1C2X2_deg5_v1 ph("i-", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("ar-h2o") {
        std::vector<double> xyz1 = {-4.4259727000e-03, 7.2497235000e-03, 1.5058480830e+00,
                                    -4.4259727000e-03, 7.2497235000e-03, 1.5058480830e+00,
                                    -4.4259727000e-03, 7.2497235000e-03, 1.5058480830e+00};
        std::vector<double> xyz2 = {1.5227529162e+00,  -8.0192355620e-01, -1.1604165083e+00, 1.3240025487e+00,
                                    -6.0027547870e-01, -2.2274381720e-01, 6.5831177100e-01,  -1.1375239818e+00,
                                    -1.4420001631e+00, -6.6145178200e-02, 1.7367956497e+00,  -1.1588983873e+00,
                                    -1.4179538960e-01, 1.4630148967e+00,  -2.2150062790e-01, 6.5683290630e-01,
                                    1.1562191534e+00,  -1.4406948567e+00, -1.4702921679e+00, -9.0858682050e-01,
                                    -1.1603278036e+00, -1.1957504615e+00, -8.3804995530e-01, -2.2275387400e-01,
                                    -1.3288712563e+00, 8.0703453000e-03,  -1.4412466488e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 7.4400095277e+00;
        std::vector<double> grad1_expected = {5.3858064805e+00,  -2.7389122807e+00, -8.3146132081e+00,
                                              -3.1749297379e-01, 6.0410819213e+00,  -8.3092677283e+00,
                                              -5.0695399406e+00, -3.2880601136e+00, -8.3143384000e+00};
        std::vector<double> grad2_expected = {
            -2.7518867566e+00, 2.2856820292e-01, 8.7637644786e-01,  -3.3214944088e+00, 2.1703215953e+00,
            7.1863949808e+00,  6.8757468490e-01, 3.4002248255e-01,  2.5184177945e-01,  1.1776483311e+00,
            -2.4982732385e+00, 8.7463100065e-01, -2.2179550926e-01, -3.9680299582e+00, 7.1826898578e+00,
            -6.3835984808e-01, 4.2522127545e-01, 2.5194686984e-01,  1.5743668478e+00,  2.2682244059e+00,
            8.7731112213e-01,  3.5443508211e+00, 1.7855092693e+00,  7.1857830504e+00,  -4.9177728315e-02,
            -7.6567356148e-01, 2.5124422751e-01};
        std::vector<double> virial_expected = {1.5088796148e+01,  -4.4459931065e-05, 1.5398900745e-03,
                                               -4.4459931065e-05, 1.5088798562e+01,  -1.7737650926e-02,
                                               1.5398900745e-03,  -1.7737650926e-02, 4.6482304145e+01};

        mbnrg_A1_B1C2X2_deg5::mbnrg_A1_B1C2X2_deg5_v1 ph("ar", "h2o");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_A1B2_A1B2_v1x::struct") {
    SECTION("co2_archive-co2_archive") {
        std::vector<double> xyz1 = {6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,
                                    -1.9222069500e-01, -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01,
                                    4.1069507510e-01,  6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,
                                    1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01, -4.4811475090e-01,
                                    -5.7997649630e-01, 4.1069507510e-01,  6.6630444410e-01,  -3.8357176030e-01,
                                    1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01,
                                    -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,  6.6630444410e-01,
                                    -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01,
                                    -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01};
        std::vector<double> xyz2 = {2.4803292099e+00,  7.5103875900e-01,  -2.9043390394e+00, 2.2674715176e+00,
                                    1.8651909097e+00,  -2.6082983571e+00, 2.7020706245e+00,  -3.5351972400e-01,
                                    -3.2106052081e+00, -1.4243133388e+00, 7.7214187430e-01,  -2.3329941557e+00,
                                    -2.4475497469e+00, 1.2692755042e+00,  -2.0709757555e+00, -4.0159255460e-01,
                                    2.6460559090e-01,  -2.5998516399e+00, 6.4986549630e-01,  3.4030903031e+00,
                                    -6.8029207450e-01, 7.9296435600e-02,  2.4412841935e+00,  -3.3312155960e-01,
                                    1.2145816282e+00,  4.3686301334e+00,  -1.0197806380e+00, 5.1568113210e-01,
                                    3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01, 3.2992897240e+00,
                                    -3.4756612500e+00, 1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 4;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 9.2886922733e-01;
        std::vector<double> grad1_expected = {
            4.2836652432e+00,  6.3074562760e-01,  -8.5918478829e-01, -1.5266795348e+00, 4.5930321037e-01,
            -1.9154748633e+00, -2.1610696419e+00, -3.4288771856e-01, 4.4410281584e-01,  -4.4194461871e+00,
            -5.8136963986e-01, 1.2479559076e-01,  1.9931236056e+00,  5.8288324765e-01,  -8.4626067850e-01,
            1.7192146623e+00,  7.5705855899e-01,  -1.9614165357e+00, 1.5431895736e+00,  2.1901660370e+00,
            -7.9903226894e-01, -7.5333474497e-01, 6.1897982187e-01,  4.1413180499e-02,  -1.6259099786e+00,
            6.1079412219e-02,  3.5759192385e-01,  -2.5785536040e-02, -5.9398518880e-02, 6.0449818120e-02,
            5.3257511735e-02,  3.6431383759e-02,  -4.0714415477e-02, -3.2887948722e-02, 1.7550184532e-02,
            -1.4244942326e-02};
        std::vector<double> grad2_expected = {
            -2.4855545333e-01, 1.1213986763e-02,  1.4720807684e+00,  -1.9224235032e-01, -9.5406181948e-02,
            4.2979134666e-01,  -1.5511826292e-01, -6.6296892423e-01, 4.2868472070e-01,  4.1976952831e+00,
            -2.1011829148e+00, -9.1608295762e-01, -2.0299920851e+00, 1.0624041832e+00,  6.1456456193e-01,
            -1.4605952787e+00, 2.8020656484e-01,  2.9844000191e+00,  -2.0191536606e+00, -2.9016781358e+00,
            1.1046587045e+00,  1.6226323047e+00,  -2.0099720079e+00, 2.1707338440e-02,  1.2325765059e+00,
            2.0414248726e+00,  -7.2633887831e-01, -1.7727677956e-01, 5.4378691702e-03,  -1.4902605674e-04,
            1.1731987313e-01,  4.7533210772e-03,  -7.8761791010e-03, 6.5372879458e-02,  -4.7742396585e-03,
            2.5347448405e-03};
        std::vector<double> virial_expected = {1.8068446294e-01,  -2.9887779240e+00, 1.1505203548e+00,
                                               -2.9887779240e+00, 7.4309743102e+00,  -5.0183109093e+00,
                                               1.1505203548e+00,  -5.0183109093e+00, 1.3835171657e+01};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2_archive", "co2_archive");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2-co2") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -7.2641834338e-02;
        std::vector<double> grad1_expected = {-3.7516100054e+00, 1.9798734990e+00,  -1.2827873187e-01,
                                              1.1066957331e+00,  -1.6950219415e+00, -5.0716153539e-01,
                                              1.7018093672e+00,  -1.7082371254e+00, -5.0689659081e-01};
        std::vector<double> grad2_expected = {4.4211615494e+00,  -1.6771311283e+00, 2.1278879082e+00,
                                              -1.3980282353e+00, 2.2202520769e+00,  9.1607907361e-02,
                                              -2.0800284090e+00, 8.8026461931e-01,  -1.0771589574e+00};
        std::vector<double> virial_expected = {1.0628460610e+00, 2.1746659686e+00, 1.6592472356e+00,
                                               2.1746659686e+00, 2.8704108541e+00, 2.3267414019e+00,
                                               1.6592472356e+00, 2.3267414019e+00, 1.7984685605e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2", "co2");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2cm5100-co2cm5100") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -7.2641834338e-02;
        std::vector<double> grad1_expected = {-3.7516100054e+00, 1.9798734990e+00,  -1.2827873187e-01,
                                              1.1066957331e+00,  -1.6950219415e+00, -5.0716153539e-01,
                                              1.7018093672e+00,  -1.7082371254e+00, -5.0689659081e-01};
        std::vector<double> grad2_expected = {4.4211615494e+00,  -1.6771311283e+00, 2.1278879082e+00,
                                              -1.3980282353e+00, 2.2202520769e+00,  9.1607907361e-02,
                                              -2.0800284090e+00, 8.8026461931e-01,  -1.0771589574e+00};
        std::vector<double> virial_expected = {1.0628460610e+00, 2.1746659686e+00, 1.6592472356e+00,
                                               2.1746659686e+00, 2.8704108541e+00, 2.3267414019e+00,
                                               1.6592472356e+00, 2.3267414019e+00, 1.7984685605e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2cm5100", "co2cm5100");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2cm595-co2cm595") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -2.3595929819e-02;
        std::vector<double> grad1_expected = {-3.6706658222e+00, 1.8254607415e+00,  -1.3385151999e-01,
                                              1.1319034217e+00,  -1.6335853387e+00, -4.9812540206e-01,
                                              1.6576014323e+00,  -1.5683156058e+00, -4.6024350903e-01};
        std::vector<double> grad2_expected = {4.2235862648e+00,  -1.6298519924e+00, 2.0541129961e+00,
                                              -1.3191748668e+00, 2.1463207175e+00,  9.6801102035e-02,
                                              -2.0232504298e+00, 8.5997147789e-01,  -1.0586936670e+00};
        std::vector<double> virial_expected = {9.5799606496e-01, 2.0339672302e+00, 1.5115295191e+00,
                                               2.0339672302e+00, 2.8196735688e+00, 2.2593128639e+00,
                                               1.5115295191e+00, 2.2593128639e+00, 1.7004644611e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2cm595", "co2cm595");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2cm590-co2cm590") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 2.7747742116e-02;
        std::vector<double> grad1_expected = {-3.6046589633e+00, 1.7048045509e+00,  -1.4237797317e-01,
                                              1.1066833942e+00,  -1.5515378570e+00, -4.8356811199e-01,
                                              1.6753080238e+00,  -1.4815327526e+00, -4.1691263841e-01};
        std::vector<double> grad2_expected = {4.0400868267e+00,  -1.5834360815e+00, 1.9949830794e+00,
                                              -1.2239458927e+00, 2.0637821020e+00,  1.0602398840e-01,
                                              -1.9934733888e+00, 8.4792003810e-01,  -1.0581483443e+00};
        std::vector<double> virial_expected = {7.0657315040e-01, 1.9806688658e+00, 1.3530133511e+00,
                                               1.9806688658e+00, 2.7170573342e+00, 2.1947294134e+00,
                                               1.3530133511e+00, 2.1947294134e+00, 1.5914655906e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2cm590", "co2cm590");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2cm5875-co2cm5875") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 5.1365130858e-02;
        std::vector<double> grad1_expected = {-3.5556838371e+00, 1.6445464046e+00,  -1.4161915398e-01,
                                              1.1055380806e+00,  -1.5209461142e+00, -4.7853088929e-01,
                                              1.6589035628e+00,  -1.4257447326e+00, -3.9613827709e-01};
        std::vector<double> grad2_expected = {3.9352064472e+00,  -1.5429902828e+00, 1.9520454572e+00,
                                              -1.2015815981e+00, 2.0217863413e+00,  9.9394569556e-02,
                                              -1.9423826555e+00, 8.2334838375e-01,  -1.0351517064e+00};
        std::vector<double> virial_expected = {6.7262826846e-01, 1.9087652962e+00, 1.3010927804e+00,
                                               1.9087652962e+00, 2.6816263933e+00, 2.1493252907e+00,
                                               1.3010927804e+00, 2.1493252907e+00, 1.5517466291e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2cm5875", "co2cm5875");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2cm585-co2cm585") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 7.7654078997e-02;
        std::vector<double> grad1_expected = {-3.4965150140e+00, 1.5547179837e+00,  -1.5098969765e-01,
                                              1.1299799741e+00,  -1.4945554296e+00, -4.7099339241e-01,
                                              1.6030916842e+00,  -1.3381019702e+00, -3.7005459518e-01};
        std::vector<double> grad2_expected = {3.8053678256e+00,  -1.4976600692e+00, 1.9025021241e+00,
                                              -1.1282058947e+00, 1.9672866619e+00,  1.1689205246e-01,
                                              -1.9137185751e+00, 8.0831282340e-01,  -1.0273564913e+00};
        std::vector<double> virial_expected = {6.4032181440e-01, 1.8271216705e+00, 1.2133427074e+00,
                                               1.8271216705e+00, 2.6642119935e+00, 2.1220128909e+00,
                                               1.2133427074e+00, 2.1220128909e+00, 1.4955479936e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2cm585", "co2cm585");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("co2cm580-co2cm580") {
        std::vector<double> xyz1 = {-8.4197913010e-01, 2.9813449180e-01,  7.1190763510e-01,
                                    -1.8419219106e+00, 9.0474595250e-01,  7.6206419480e-01,
                                    1.5998834930e-01,  -3.0310208490e-01, 6.5993554450e-01};
        std::vector<double> xyz2 = {-3.2948198847e+00, -1.4151144755e+00, -1.4314187751e+00,
                                    -2.3067160876e+00, -1.7333305570e+00, -8.8649447400e-01,
                                    -4.2723603122e+00, -1.1016996178e+00, -1.9876095026e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.3441466096e-01;
        std::vector<double> grad1_expected = {-3.1858290547e+00, 1.5554777664e+00,  -1.0433564789e-02,
                                              1.0926736509e+00,  -1.5240357082e+00, -5.3264085367e-01,
                                              1.3974135568e+00,  -1.2463802060e+00, -3.8560682257e-01};
        std::vector<double> grad2_expected = {3.3806743267e+00,  -1.4431563790e+00, 1.7019694349e+00,
                                              -1.0119202018e+00, 1.8984433390e+00,  1.2505428008e-01,
                                              -1.6730122779e+00, 7.5965118784e-01,  -8.9834247391e-01};
        std::vector<double> virial_expected = {7.6343720285e-01, 1.5716644081e+00, 1.1299296208e+00,
                                               1.5716644081e+00, 2.6226489171e+00, 2.0036754591e+00,
                                               1.1299296208e+00, 2.0036754591e+00, 1.4293468003e+00};

        x2b_A1B2_A1B2_deg5::x2b_A1B2_A1B2_v1x ph("co2cm580", "co2cm580");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_A1B2Z2_C1D2_v1x::struct") {
    SECTION("h2o-co2") {
        std::vector<double> xyz1 = {-1.1036011760e-02, 2.2989672850e-05,  -5.5211266869e-03,
                                    3.8335135909e-01,  1.0250177013e+00,  -1.8279944626e-01,
                                    4.3228412531e-01,  -2.7848809765e-01, 9.4394167282e-01};
        std::vector<double> xyz2 = {4.2086974547e+00, 2.8269706666e-02,  -1.5952691537e-02,
                                    4.6800226436e+00, 8.4958789099e-01,  6.4614677056e-01,
                                    3.8068555636e+00, -8.7081093285e-01, -6.3417053592e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.1102997556e-01;
        std::vector<double> grad1_expected = {-1.1039496285e-01, -3.1881170895e-02, -2.3800371921e-02,
                                              1.5209728626e-01,  -6.0045671672e-02, -3.9465324183e-02,
                                              1.3503224671e-01,  2.3135467667e-02,  9.6927945865e-03};
        std::vector<double> grad2_expected = {4.2288790803e-01,  2.0165353741e+00,  1.6411426718e+00,
                                              -5.0858600722e-01, -1.4683382311e+00, -1.1797441779e+00,
                                              -9.1036470940e-02, -4.7940576806e-01, -4.0782559239e-01};
        std::vector<double> virial_expected = {8.2905214555e-01, 2.2256304268e-01, 1.7736598918e-01,
                                               2.2256304268e-01, 8.4099529507e-01, 6.4391529249e-01,
                                               1.7736598918e-01, 6.4391529249e-01, 5.1334248195e-01};

        x2b_A1B2Z2_C1D2_deg4::x2b_A1B2Z2_C1D2_v1x ph("h2o", "co2");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-co2_archive") {
        std::vector<double> xyz1 = {
            -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,
            -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00, -9.1278996460e-01,
            1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00,
            -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00, -9.1278996460e-01, 1.3711133940e+00,
            -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00,
            1.3696631160e+00,  -3.3238997190e+00, -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00,
            -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,
            -3.3238997190e+00, -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01,
            1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+0};
        std::vector<double> xyz2 = {
            6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01,
            -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,  2.4803292099e+00,
            7.5103875900e-01,  -2.9043390394e+00, 2.2674715176e+00,  1.8651909097e+00,  -2.6082983571e+00,
            2.7020706245e+00,  -3.5351972400e-01, -3.2106052081e+00, -1.4243133388e+00, 7.7214187430e-01,
            -2.3329941557e+00, -2.4475497469e+00, 1.2692755042e+00,  -2.0709757555e+00, -4.0159255460e-01,
            2.6460559090e-01,  -2.5998516399e+00, 6.4986549630e-01,  3.4030903031e+00,  -6.8029207450e-01,
            7.9296435600e-02,  2.4412841935e+00,  -3.3312155960e-01, 1.2145816282e+00,  4.3686301334e+00,
            -1.0197806380e+00, 5.1568113210e-01,  3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01,
            3.2992897240e+00,  -3.4756612500e+00, 1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 5;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 7.5362344392e+02;
        std::vector<double> grad1_expected = {
            -1.0291903673e-01, -9.0563377706e-02, 6.0489667491e-02,  3.4085456364e-02,  7.8255832123e-02,
            -7.2119624943e-02, 8.0115902494e-03,  4.2751485126e-02,  -4.8650192514e-02, -1.0975316454e-01,
            -2.5429048649e-02, -4.7960082476e-01, 2.9456459063e-01,  -1.3104991880e-01, 1.3828350656e-01,
            8.6486820480e-01,  1.2415922836e-01,  4.7242533383e-01,  8.4346687062e+01,  6.9664515025e+01,
            -5.8024598318e+02, -6.7671641339e+02, -1.0400699076e+03, 1.0067069487e+03,  4.3220930219e+02,
            -2.3799501903e+02, 3.3527736129e+02,  1.1813845351e-01,  8.6669380723e-01,  1.4938778827e+00,
            3.2330664892e-02,  2.3390205602e-01,  3.2440903313e-01,  1.0695857650e+00,  1.2503370722e-01,
            8.4615123483e-01,  3.4134840407e+01,  7.1430317113e+01,  -7.6125274446e+00, 8.8282813842e+00,
            9.1823295248e+01,  -9.7450888549e+00, -1.3166815848e+01, 5.2366578014e+00,  -8.8952292687e+00};
        std::vector<double> grad2_expected = {
            9.9078217484e-01,  4.2037699473e-01,  -6.7931100111e-01, 3.1829427916e-01,  -1.0597311794e-01,
            1.9077687205e-01,  -1.2482544639e+00, -3.4484781634e-01, 5.4881427902e-01,  -7.9241574195e-01,
            2.6850813041e+00,  1.0197778181e+00,  -6.4052573636e-01, -8.0276537843e-01, -4.9065502743e-01,
            3.8326184743e-01,  -1.8499961865e+00, -6.6023080633e-01, 5.8512813490e+02,  4.6678621535e+02,
            -5.5256388297e+02, -1.4923396930e+02, 1.3064745232e+02,  4.1522870625e+01,  -2.7573374147e+02,
            6.1096674397e+02,  -2.5069731451e+02, 2.8352802372e+00,  5.3953005729e+00,  -3.5063628776e+00,
            -2.3535618188e+00, -3.3504918950e+00, -7.3879938108e-01, -1.7017733018e+00, -3.2704382483e+00,
            1.5807241080e+00,  -2.8713478885e+01, -2.6831679577e+01, 1.6845139039e+01,  -6.7927994574e+00,
            -1.4788895346e+02, 1.2499253342e+01,  5.7099723990e+00,  6.2303628743e+00,  -3.0915468136e+00};
        std::vector<double> virial_expected = {9.5345687924e+02,  5.1320557907e+02,  -3.3395885269e+02,
                                               5.1320557907e+02,  1.8897800093e+03,  -1.2748725485e+03,
                                               -3.3395885269e+02, -1.2748725485e+03, 1.0536993559e+03};

        x2b_A1B2Z2_C1D2_deg4::x2b_A1B2Z2_C1D2_v1x ph("h2o", "co2_archive");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_A1B2Z2_C1D4_v1x::struct") {
    SECTION("h2o-ch4") {
        std::vector<double> xyz1 = {-4.4590985000e-03, -5.1342579600e-02, 3.0000158138e+00,  9.8613021140e-01,
                                    -7.4573098400e-02, 3.0000054324e+00,  -1.5974709230e-01, 8.9671808950e-01,
                                    3.0000164932e+00,  -4.4590985000e-03, -5.1342579600e-02, 1.5813800000e-05,
                                    9.8613021140e-01,  -7.4573098400e-02, 5.4324000000e-06,  -1.5974709230e-01,
                                    8.9671808950e-01,  1.6493200000e-05};
        std::vector<double> xyz2 = {
            -5.9895400000e+00, -4.3787000000e-01, 2.5724300000e+00,  -5.2478300000e+00, 1.0363000000e-01,
            1.9881600000e+00,  -6.5511600000e+00, 2.3073000000e-01,  3.1955700000e+00,  -6.6391500000e+00,
            -8.8267000000e-01, 1.9054200000e+00,  -5.4527200000e+00, -1.1652700000e+00, 3.1896100000e+00,
            -5.9895400000e+00, -4.3787000000e-01, 2.5724300000e+00,  -5.2478300000e+00, 1.0363000000e-01,
            1.9881600000e+00,  -6.5511600000e+00, 2.3073000000e-01,  3.1955700000e+00,  -6.6391500000e+00,
            -8.8267000000e-01, 1.9054200000e+00,  -5.4527200000e+00, -1.1652700000e+00, 3.1896100000e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 9.3915973596e-03;
        std::vector<double> grad1_expected = {7.1983905305e-02,  2.8008512763e-01,  1.3564668044e-03, -1.2769127585e-01,
                                              -2.4872930945e-03, -6.7981912618e-04, 5.3617917016e-02, -2.7434424756e-01,
                                              -6.5677711267e-04, 1.1592813988e-01,  2.8175665333e-01, 4.0248971519e-03,
                                              -1.7606532089e-01, 8.6698614328e-03,  1.5949006613e-03, 5.6211408265e-02,
                                              -2.9012237319e-01, -2.8502477674e-03};
        std::vector<double> grad2_expected = {
            -3.9709553276e-01, 1.0302282035e-01,  -4.8458735126e-02, 8.4758131776e-02,  4.8669984918e-02,
            -6.2290488571e-02, 1.0462974755e-01,  -5.8123444661e-02, -3.6959458986e-02, 1.2123751266e-01,
            2.1811861739e-02,  5.4625751628e-02,  8.8559594292e-02,  -1.1863480933e-01, 9.3063060490e-02,
            -2.9655905708e-01, -3.7299313780e-02, 1.3047877673e-01,  1.0464636191e-01,  8.2723613327e-02,
            -9.1732665656e-02, 8.7505040843e-02,  -3.9476133032e-02, -6.1322958408e-02, 5.6076963139e-02,
            2.3221153210e-02,  -2.2773280558e-03, 5.2256463928e-02,  -2.9473461294e-02, 2.2084625345e-02};
        std::vector<double> virial_expected = {3.6097074212e-01,  -1.5858101032e-01, 1.3089644321e-02,
                                               -1.5858101032e-01, 4.4031923998e-01,  2.5842611038e-01,
                                               1.3089644321e-02,  2.5842611038e-01,  -5.7781384144e-02};

        x2b_A1B2Z2_C1D4_deg3_exp0::x2b_A1B2Z2_C1D4_v1x ph("h2o", "ch4");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_h2o_ion_v2x::struct") {
    SECTION("h2o-li") {
        std::vector<double> xyz1 = {1.7474594210e+00, -1.4737233740e+00, -1.3296783390e+00,
                                    2.2444822770e+00, -1.7974450600e+00, -2.0835207810e+00,
                                    1.0684055850e+00, -2.1338516330e+00, -1.1304174950e+00};
        std::vector<double> xyz2 = {-1.7782237730e-01, 1.5460063700e-01, -1.0620904050e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 8.0147096979e-01;
        std::vector<double> grad1_expected = {-3.9877621083e+00, 3.4502566355e+00,  -4.0490509777e+00,
                                              -1.1748994038e-01, -8.4942782557e-02, 3.6255623717e+00,
                                              -6.6489531430e-03, 2.4150584196e-01,  1.6119575870e-01};
        std::vector<double> grad2_expected = {4.1119010018e+00, -3.6068196949e+00, 2.6229284736e-01};
        std::vector<double> virial_expected = {7.9704483438e+00,  -6.7379303355e+00, -1.1875391214e+00,
                                               -6.7379303355e+00, 6.0049981227e+00,  8.5298529793e-01,
                                               -1.1875391214e+00, 8.5298529793e-01,  2.6307963878e+00};

        h2o_ion::x2b_h2o_ion_v2x ph("h2o", "li+");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-na") {
        std::vector<double> xyz1 = {2.7721678110e+00,  8.2864804120e-01,  1.3407652800e+00,  2.8242223750e+00,
                                    5.9951176200e-01,  2.2720625940e+00,  3.6577708060e+00,  6.7292239890e-01,
                                    1.0032623000e+00,  -1.0695654940e+00, -1.6239523270e+00, -2.5977628240e-01,
                                    -1.6372337220e+00, -1.4281547330e+00, -1.0129308230e+00, -1.3257667840e+00,
                                    -2.4893344620e+00, 5.9173029930e-02,  -1.7765826110e+00, 2.1211182870e-01,
                                    -2.2850759190e+00, -1.6076762700e+00, 3.9673134650e-02,  -3.2146924790e+00,
                                    -2.6124275400e+00, 6.8516610220e-01,  -2.2745541860e+00, 1.1557765290e+00,
                                    2.9477075680e+00,  4.0818352720e-01,  1.9410235610e+00,  2.6842002830e+00,
                                    8.9989145890e-01,  1.1206057440e+00,  3.9037648930e+00,  4.3935098830e-01};
        std::vector<double> xyz2 = {2.9835636860e-01,  5.8244279830e-01,  -2.3250860780e-01, 2.9835636860e-01,
                                    5.8244279830e-01,  -2.3250860780e-01, 2.9835636860e-01,  5.8244279830e-01,
                                    -2.3250860780e-01, 2.9835636860e-01,  5.8244279830e-01,  -2.3250860780e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 4;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 7.3392533177e+00;
        std::vector<double> grad1_expected = {
            -2.9293080668e+00, -1.4238878213e+00, -2.0431170732e+00, 6.6133448112e-01,  4.8243727809e-01,
            -1.7239437288e-02, 2.0624333577e-01,  4.8002299178e-01,  7.1454590284e-01,  5.9519124643e+00,
            7.0156420822e+00,  1.9469978675e+00,  -3.4862885911e-01, 3.5350423199e-01,  -6.2420257223e-01,
            -3.1811498423e-01, 7.2226695033e-01,  -9.0641847467e-01, 2.9047971253e+00,  1.4315123570e+00,
            2.0725764548e+00,  -8.3736224307e-01, -2.8361722149e-01, 4.3818552069e-02,  -2.4262632115e-01,
            -6.1273947988e-01, -4.8588966611e-01, -4.8269719070e+00, -7.3657306172e+00, -3.2987344885e+00,
            5.7802289992e-01,  -4.2489112496e-01, 3.4855888223e-01,  7.4813673700e-01,  -8.5608934192e-01,
            3.8969721730e-01};
        std::vector<double> grad2_expected = {2.0617302499e+00,  4.6142755143e-01,  1.3458106077e+00,
                                              -5.2851686210e+00, -8.0914132646e+00, -4.1637682060e-01,
                                              -1.8248085611e+00, -5.3515565568e-01, -1.6305053407e+00,
                                              3.5008122701e+00,  8.6467110841e+00,  2.5604783890e+00};
        std::vector<double> virial_expected = {1.8132640431e+01, 2.0508966300e+01, 7.5854568492e+00,
                                               2.0508966300e+01, 4.0304984914e+01, 7.3118984343e+00,
                                               7.5854568492e+00, 7.3118984343e+00, 7.0543914173e+00};

        h2o_ion::x2b_h2o_ion_v2x ph("h2o", "na+");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
    SECTION("h2o-k") {
        std::vector<double> xyz1 = {
            2.6597493150e+00,  3.7459420900e+00,  -2.7115058550e-01, 3.6168052410e+00,  3.6909856940e+00,
            -3.0898568140e-01, 2.4756964800e+00,  4.6869357310e+00,  -3.0447203980e-01, -2.4596215450e+00,
            -1.3307686320e+00, 1.6558717950e-01,  -2.8502305670e+00, -1.6982123550e+00, 9.6207666330e-01,
            -2.9142201740e+00, -1.7646650170e+00, -5.6040167150e-01, 2.3868934490e+00,  -3.1455907500e+00,
            4.1103255950e-02,  2.7089746660e+00,  -3.6401137150e+00, -7.1446525680e-01, 2.7729279050e+00,
            -3.5736975860e+00, 8.0718092060e-01,  -1.9864852310e+00, 1.5070138030e+00,  2.1838885590e-02,
            -2.7550924640e+00, 2.0773876700e+00,  2.9247198430e-02,  -2.3285334330e+00, 6.0539680420e-01,
            7.5568470150e-02,  4.6520682970e-01,  -1.0776665390e+00, 3.1610115760e-02,  -4.3418595880e-01,
            -1.4146126480e+00, 8.4118085910e-02,  1.0496754310e+00,  -1.8470977930e+00, 4.0629166460e-02};
        std::vector<double> xyz2 = {7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,
                                    1.6129075690e+00,  -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,
                                    -9.9484716940e-02, 7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02,
                                    7.9016737220e-01,  1.6129075690e+00,  -9.9484716940e-02};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 5;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.0512435167e+01;
        std::vector<double> grad1_expected = {
            -6.0919090874e+00, -6.9602707139e+00, 5.5979477294e-01,  -1.0562351807e-01, -7.9558690288e-02,
            7.9116174968e-03,  -6.9001028168e-02, -1.1164290020e-01, 7.7720122363e-03,  3.7160630257e-01,
            2.9805719439e-01,  -2.8628419070e-02, -1.5328484241e-01, -1.2369305478e-01, -2.0409886129e-01,
            -1.3519924052e-01, -1.0491130737e-01, 2.2620221074e-01,  7.4673922978e-02,  3.0325499370e-01,
            -1.6374817120e-02, -3.5521867817e-02, -1.2111476318e-01, 4.8173701014e-04,  -3.4994437459e-02,
            -1.2056702814e-01, 1.3030820182e-02,  1.0936102388e+01,  9.9126446086e-01,  -5.0290693925e-01,
            5.1134730810e-01,  -1.6384552647e-01, -1.4339851102e-02, 4.2798493059e-01,  -2.6285568571e-01,
            -6.5148358204e-03, 1.8461439631e+00,  1.4000568524e+01,  -6.8868254689e-01, -2.1430215062e-01,
            6.9270080544e-01,  -2.1227750365e-02, 2.6947583063e-01,  5.6634853685e-01,  -3.6045729678e-02};
        std::vector<double> grad2_expected = {
            6.2665336337e+00,  7.1514723044e+00,  -5.7547840267e-01, -8.3122219640e-02, -6.9452832242e-02,
            6.5250696247e-03,  -4.1576177025e-03, -6.1573202388e-02, 2.8622599281e-03,  -1.1875434627e+01,
            -5.6456324868e-01, 5.2376162617e-01,  -1.9013176431e+00, -1.5259617867e+01, 7.4595602694e-01};
        std::vector<double> virial_expected = {4.5752275578e+01,  2.0145317288e+01,  -2.7887423768e+00,
                                               2.0145317288e+01,  5.7292526073e+01,  -3.3333465732e+00,
                                               -2.7887423768e+00, -3.3333465732e+00, 5.8187619383e-01};

        h2o_ion::x2b_h2o_ion_v2x ph("h2o", "k+");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-rb") {
        std::vector<double> xyz1 = {
            -1.7428543420e+00, -4.0415235570e+00, -5.2281327930e-02, -1.2799702230e+00, -4.8814607780e+00,
            -5.5259141270e-02, -2.6687334400e+00, -4.2895947640e+00, -8.5281669350e-02, 4.3383131180e+00,
            3.2390181030e+00,  1.3325160940e-02,  5.2925835800e+00,  3.2442801940e+00,  1.1175183190e-01,
            4.1012507660e+00,  4.1572127150e+00,  -1.2194934820e-01, 4.6436621030e+00,  -1.4024067280e+00,
            2.4136366790e-04,  4.5256285580e+00,  -2.3519501630e+00, 5.3458050450e-02,  5.5915768500e+00,
            -1.2763749330e+00, -7.0366450030e-02, -1.8876071560e+00, 2.6224545870e+00,  2.5212264120e-02,
            -1.6560764410e+00, 3.5536300830e+00,  5.1521484640e-03,  -2.8474164440e+00, 2.6208630030e+00,
            2.6359847690e-02,  2.7843038080e+00,  7.9342504490e-01,  3.1934423250e-02,  3.4447677520e+00,
            8.8767431400e-02,  1.9511609960e-02,  3.2945296650e+00,  1.6135004810e+00,  2.3451158790e-02};
        std::vector<double> xyz2 = {-2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02,
                                    2.1755455960e-01,  7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,
                                    7.4740077520e-02,  -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02,
                                    -2.0182527570e-02, 2.1755455960e-01,  7.4740077520e-02};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 5;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 5.6933692562e+00;
        std::vector<double> grad1_expected = {
            1.6652514429e-01,  3.6507724768e-01,  8.8083142798e-03,  -2.4276317085e-01, -6.7442058937e-02,
            -7.1673419983e-03, 1.0877185748e-01,  -2.1985763448e-01, 3.2266416564e-04,  -3.6081944687e-02,
            -1.9227459472e-01, 3.8615380340e-02,  -1.2293912517e-01, 1.3100132426e-01,  -3.8639340805e-02,
            1.1031237371e-01,  2.3176002654e-02,  3.1409734776e-03,  -5.3757410020e-02, 2.9170443906e-01,
            -4.3547909514e-02, 1.1580163284e-01,  -5.3930699671e-02, 1.7435982910e-03,  -1.4621882048e-01,
            -1.9732362810e-01, 3.8834930470e-02,  3.9205422558e+00,  -5.0342773711e+00, 1.0394354652e-01,
            -9.5172023783e-02, -2.4561716131e-01, 5.2354928116e-03,  2.5349902981e-01,  2.9247558774e-02,
            -1.0094838621e-03, -1.1595521947e+01, -2.3318181985e+00, 1.7512012103e-01,  -5.6570510204e-01,
            -1.3710128986e-01, 9.5488161374e-03,  -6.3674619511e-01, -1.4448468354e-01, 1.0874788379e-02};
        std::vector<double> grad2_expected = {
            -3.2533830928e-02, -7.7777554259e-02, -1.9636364471e-03, 4.8708696142e-02, 3.8097267807e-02,
            -3.1170130130e-03, 8.4174597665e-02,  -4.0450111287e-02, 2.9693807528e-03, -4.0788692619e+00,
            5.2506469736e+00,  -1.0816955547e-01, 1.2797973244e+01,  2.6134041719e+00, -1.9554372554e-01};
        std::vector<double> virial_expected = {4.5642319513e+01,  -2.2265218880e+00, -3.5216462197e-01,
                                               -2.2265218880e+00, 1.4735301926e+01,  -3.9753562772e-01,
                                               -3.5216462197e-01, -3.9753562772e-01, 2.1131079863e-02};

        h2o_ion::x2b_h2o_ion_v2x ph("h2o", "rb+");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("h2o-cs") {
        std::vector<double> xyz1 = {
            -5.2718689600e+00, -5.1184387920e+00, -6.4447592170e-02, -5.0429736460e+00, -5.9701878600e+00,
            3.0875632580e-01,  -6.2136505200e+00, -5.1783815240e+00, -2.3191504100e-01, 8.7247460360e+00,
            -4.0707243320e+00, -4.9403338130e-01, 8.5842549660e+00,  -5.0180937930e+00, -5.2607205250e-01,
            9.6750071550e+00,  -3.9764821660e+00, -4.1521702300e-01, 4.6423862440e+00,  4.8846581920e+00,
            -7.9319686810e-01, 5.5888140450e+00,  5.0435305540e+00,  -8.1230703670e-01, 4.2565170720e+00,
            5.7495827880e+00,  -9.4900547950e-01, 1.9577742960e+00,  1.6498238250e+00,  -2.6803938630e+00,
            2.2798079560e+00,  1.0538141290e+00,  -3.3606054190e+00, 1.0744078230e+00,  1.8837916330e+00,
            -2.9741712370e+00, 2.4393531210e+00,  2.5097433290e+00,  4.9496556790e+00,  2.9546761940e+00,
            2.0407189980e+00,  5.6082326770e+00,  1.7445021530e+00,  2.9323398370e+00,  5.4575274610e+00};
        std::vector<double> xyz2 = {3.2822936850e+00,  2.4725476270e+00,  -3.3685766700e-01, 3.2822936850e+00,
                                    2.4725476270e+00,  -3.3685766700e-01, 3.2822936850e+00,  2.4725476270e+00,
                                    -3.3685766700e-01, 3.2822936850e+00,  2.4725476270e+00,  -3.3685766700e-01,
                                    3.2822936850e+00,  2.4725476270e+00,  -3.3685766700e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 5;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.3721453047e+01;
        std::vector<double> grad1_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  -1.0496828792e+01, -1.8535980131e+01,
            3.5075669649e+00,  -9.8790579312e-01, -1.5084415221e+00, 2.7992613975e-01,  -7.3288494900e-01,
            -1.6127621150e+00, 2.9990708938e-01,  9.8732161531e+00,  6.1259720188e+00,  1.7461736647e+01,
            7.1252846389e-01,  5.9190414677e-01,  1.4871313656e+00,  9.3464776519e-01,  4.4239299090e-01,
            1.4214812139e+00,  5.2910436184e-03,  -8.3975079034e-03, -1.9211127235e-01, -7.4874812829e-02,
            5.4833950946e-02,  9.0593594441e-02,  6.7614916946e-02,  -4.8449132263e-02, 1.0250922817e-01};
        std::vector<double> grad2_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  1.2217619534e+01,  2.1657183768e+01, -4.0874001940e+00, -1.1520392382e+01,
            -7.1602691565e+00, -2.0370349227e+01, 1.9688522649e-03, 2.0126892196e-03,  -9.9155025922e-04};
        std::vector<double> virial_expected = {3.3208346030e+01, 3.9881569005e+01, 2.2074636916e+01,
                                               3.9881569005e+01, 6.0060555575e+01, 7.1489379734e+00,
                                               2.2074636916e+01, 7.1489379734e+00, 5.0968165932e+01};

        h2o_ion::x2b_h2o_ion_v2x ph("h2o", "cs+");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_A1B4_A1B4_v1x::struct") {
    SECTION("ch4-ch4") {
        std::vector<double> xyz1 = {-3.3099600000e+00, 1.4404010000e+01,  1.3046900000e+01,  -2.5090200000e+00,
                                    1.4669480000e+01,  1.2329400000e+01,  -4.1929500000e+00, 1.4316780000e+01,
                                    1.2445600000e+01,  -3.1944300000e+00, 1.3405090000e+01,  1.3412900000e+01,
                                    -3.2439700000e+00, 1.5197490000e+01,  1.3763500000e+01};
        std::vector<double> xyz2 = {-6.9251900000e+00, 1.2729200000e+01,  1.2999000000e+01,  -6.3721200000e+00,
                                    1.3024600000e+01,  1.3829800000e+01,  -7.4085400000e+00, 1.3486100000e+01,
                                    1.2437900000e+01,  -6.3225200000e+00, 1.2150900000e+01,  1.2339100000e+01,
                                    -7.6702900000e+00, 1.1987100000e+01,  1.3413300000e+01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.6326245771e-01;
        std::vector<double> grad1_expected = {
            2.6705871951e-02,  -2.1520848300e-01, 3.5120828621e-01,  -2.5309794301e-01, -2.6530898155e-02,
            -1.0126295028e-02, -3.5542545215e-01, -1.1910007087e-01, -5.6796268527e-02, -1.8290606495e-02,
            9.3866814782e-02,  -1.1827884400e-02, -6.7376112408e-02, -1.0789961783e-01, -1.3436837235e-01};
        std::vector<double> grad2_expected = {2.6797785861e-01, 3.9077760758e-02,  -4.2918516823e-01, 1.6594589633e-01,
                                              8.1731288952e-02, -2.0622524478e-02, 9.5033086369e-02,  -6.0983933339e-03,
                                              6.4547066476e-02, -2.9496588277e-02, 1.7609714893e-01,  1.3186855341e-01,
                                              1.6802398908e-01, 8.4064449772e-02,  1.1530260691e-01};
        std::vector<double> virial_expected = {2.4056733951e+00,  1.1759679418e+00,  -4.8198844235e-01,
                                               1.1759679418e+00,  9.4856914781e-01,  -1.9674355416e-02,
                                               -4.8198844235e-01, -1.9674355416e-02, 1.4518642364e-01};

        x2b_A1B4_A1B4_deg4_exp0::x2b_A1B4_A1B4_v1x ph("ch4", "ch4");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_v9x::struct") {
    SECTION("h2o-h2o") {
        std::vector<double> xyz1 = {1.7474594210e+00,  -1.4737233740e+00, -1.3296783390e+00, 2.2444822770e+00,
                                    -1.7974450600e+00, -2.0835207810e+00, 1.0684055850e+00,  -2.1338516330e+00,
                                    -1.1304174950e+00, 1.7474594210e+00,  -1.4737233740e+00, -1.3296783390e+00,
                                    2.2444822770e+00,  -1.7974450600e+00, -2.0835207810e+00, 1.0684055850e+00,
                                    -2.1338516330e+00, -1.1304174950e+00};
        std::vector<double> xyz2 = {7.4372664830e-01,  1.3745066180e+00,  8.2228117080e-01,  1.5966080550e+00,
                                    9.4041883440e-01,  1.0251293750e+00,  8.4611620640e-01,  2.3077049320e+00,
                                    1.0097486520e+00,  2.8905925930e+00,  -1.7061104380e-01, 9.3095149180e-01,
                                    2.7240924310e+00,  -7.8786885510e-01, 2.0726128570e-01,  3.3243787470e+00,
                                    -6.6848224410e-01, 1.6262376630e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 4.3150257624e+00;
        std::vector<double> grad1_expected = {1.2563135491e-01,  -1.2797876775e-01, 2.7288516037e-01, -1.2759824572e-01,
                                              5.8244766442e-02,  -1.2645960580e-01, 7.1938034852e-02, 1.6052347464e-01,
                                              -7.5595691561e-02, 7.5632040650e+00,  4.1772548895e+00, 1.2659517070e+01,
                                              3.8492687061e-01,  1.1295431152e+00,  9.4293707984e-01, 5.0647177664e-01,
                                              1.2347152426e+00,  7.2663855762e-01};
        std::vector<double> grad2_expected = {
            2.6608429402e-01,  -9.5315088847e-03, 3.0301261059e-01,  -3.5929577276e-01, -4.0740584191e-02,
            -1.7305221458e-01, 2.3240334704e-02,  -4.0517380256e-02, -2.0079025902e-01, -1.0094769333e+00,
            -3.3531895202e+00, -2.8808009830e+00, -6.5090890399e+00, -3.5740779611e+00, -1.0209638571e+01,
            -9.3603673911e-01, 3.8575423399e-01,  -1.2386531532e+00};
        std::vector<double> virial_expected = {9.4857101932e+00, 7.0202529345e+00, 1.5350808086e+01,
                                               7.0202529345e+00, 8.0945192175e+00, 1.2761828774e+01,
                                               1.5350808086e+01, 1.2761828774e+01, 2.6576313383e+01};

        x2o::x2b_v9x ph;
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A2_A2_deg6_v1::struct") {
    SECTION("h2-h2") {
        std::vector<double> xyz1 = {1.6216168900e+00,  -1.0270244000e+00, 3.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 4.0238165600e+00,  1.6216168900e+00,  -1.0270244000e+00,
                                    3.9469574100e+00,  2.0831958000e+00,  -4.4599113500e-01, 4.0238165600e+00,
                                    1.6216168900e+00,  -1.0270244000e+00, 3.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 4.0238165600e+00};
        std::vector<double> xyz2 = {1.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 3.0238165600e+00,  1.6216168900e+00,  -2.0270244000e+00,
                                    2.9469574100e+00,  2.0831958000e+00,  -1.4459911350e+00, 3.0238165600e+00,
                                    3.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  4.0831958000e+00,
                                    -4.4599113500e-01, 3.0238165600e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.9131884042e+02;
        std::vector<double> grad1_expected = {
            1.1566673697e+01,  1.4560072044e+01,  -1.7549929466e+02, -1.4456889935e+01, -1.8198262049e+01,
            -1.8609415745e+02, -2.1664885000e+00, -5.7985224487e+01, -5.5618809128e+01, 8.2435952827e+00,
            -3.1715151290e+01, -4.0719478104e+01, 3.3894878258e+00,  -1.7135445904e+00, -2.6020406799e+00,
            6.8961347895e+00,  1.4173926142e+00,  -2.6975787622e+00};
        std::vector<double> grad2_expected = {
            1.4456889935e+01,  1.8198262049e+01,  1.8609415745e+02,  -1.1566673697e+01, -1.4560072044e+01,
            1.7549929466e+02,  -8.2435952827e+00, 3.1715151290e+01,  4.0719478104e+01,  2.1664885000e+00,
            5.7985224487e+01,  5.5618809128e+01,  -6.8961347895e+00, -1.4173926142e+00, 2.6975787622e+00,
            -3.3894878258e+00, 1.7135445904e+00,  2.6020406799e+00};
        std::vector<double> virial_expected = {2.6159503957e+01,  9.5736585762e-01, -1.2541992275e+01,
                                               9.5736585762e-01,  9.1651092523e+01, 9.3892758805e+01,
                                               -1.2541992275e+01, 9.3892758805e+01, 4.6290786399e+02};

        mbnrg_A2_A2_deg6::mbnrg_A2_A2_deg6_v1 ph("h2", "h2");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_2b_A1B2Z2_C2_deg5_v1::struct") {
    SECTION("h2-h2o") {
        std::vector<double> xyz1 = {1.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 3.0238165600e+00,  1.6216168900e+00,  -1.0270244000e+00,
                                    2.9469574100e+00,  2.0831958000e+00,  -4.4599113500e-01, 3.0238165600e+00,
                                    1.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 3.0238165600e+00};
        std::vector<double> xyz2 = {3.1323791000e+00,  -2.6311071100e-01, 1.8748945900e-01,  3.5927079900e+00,
                                    -8.4084266700e-01, 8.0129372700e-01,  3.8147658100e+00,  2.4972866800e-02,
                                    -4.2299288300e-01, -7.8557467500e-01, -2.1325879200e+00, 1.6833544100e+00,
                                    -1.1221514300e+00, -1.6953293900e+00, 2.4726422100e+00,  -1.0730754800e+00,
                                    -3.0449198500e+00, 1.7376520600e+00,  -9.8569402700e-01, 3.2822858700e-01,
                                    3.1375299800e+00,  -1.7998662700e+00, 7.4937353100e-01,  3.4245516000e+00,
                                    -3.9361521900e-01, 4.1803592100e-01,  3.8895249900e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 8.0018869775e-02;
        std::vector<double> grad1_expected = {
            1.9563051090e-01,  5.3600625777e-01,  -2.9712212276e-01, 3.4817708562e-02, -3.8367616321e-01,
            -1.3228330247e-01, 2.3535406972e-01,  3.0232899640e-01,  7.0798928942e-01, -6.6314083307e-01,
            -5.9856884365e-01, -6.3254599220e-01, 3.4748117153e-01,  2.7200712286e-01, 1.0159058328e-01,
            -8.8628823116e-01, -1.3593498655e-01, -1.1662706381e-01};
        std::vector<double> grad2_expected = {
            -1.3583223167e-01, -9.4623797467e-01, 8.4341226749e-01, -2.1338579945e-01, 5.9821214955e-01,
            -5.8520347814e-01, 1.1876981165e-01,  1.9569573056e-01, 1.7119663588e-01,  -4.5705378103e-01,
            8.8347957964e-01,  -1.0139155241e-02, 6.3378031315e-01, -2.1159406192e-01, -2.8393538660e-01,
            2.5106023123e-01,  -3.7564567047e-01, 2.1863124463e-01, 1.1509731779e+00,  -1.8189820361e-01,
            3.8776108724e-01,  1.2204921723e-01,  8.4231663748e-02, 1.2004492707e-01,  -7.3421533551e-01,
            -3.8405596441e-02, -4.9276953378e-01};
        std::vector<double> virial_expected = {4.3186262096e+00, 6.0777419434e-01, 1.2508049898e-01,
                                               6.0777419434e-01, 1.2850036373e+00, 1.0246940769e-02,
                                               1.2508049898e-01, 1.0246940769e-02, 2.1665358086e+00};

        mbnrg_A1B2Z2_C2_deg5::mbnrg_A1B2Z2_C2_deg5_v1 ph("h2o", "h2");
        double e_nograd = ph.eval(xyz2.data(), xyz1.data(), ndim);
        double e = ph.eval(xyz2.data(), xyz1.data(), grad2.data(), grad1.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1B3_A1B3_deg4_v1::struct") {
    SECTION("nh3-nh3") {
        std::vector<double> xyz1 = {-1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00,
                                    1.6636378008e+00,  -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,
                                    6.8368652710e-01,  -1.8778475277e+00, 4.5835317270e-01,  7.1032361800e-02,
                                    -1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00,
                                    1.6636378008e+00,  -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,
                                    6.8368652710e-01,  -1.8778475277e+00, 4.5835317270e-01,  7.1032361800e-02,
                                    -1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00,
                                    1.6636378008e+00,  -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,
                                    6.8368652710e-01,  -1.8778475277e+00, 4.5835317270e-01,  7.1032361800e-02};
        std::vector<double> xyz2 = {1.3253591612e+00,  1.5992161603e+00,  -3.5649305290e-01, 1.7112172925e+00,
                                    2.2347398082e+00,  3.3141779120e-01,  3.0734064400e-01,  1.6708149436e+00,
                                    -2.9199566840e-01, 1.5915360076e+00,  1.9683083217e+00,  -1.2614354220e+00,
                                    -1.6441806609e+00, -1.6474447284e+00, 3.9846272690e-01,  -6.2596824480e-01,
                                    -1.7186182488e+00, 3.3660579490e-01,  -2.0279253230e+00, -2.2803120707e+00,
                                    -2.9307077260e-01, -1.9129140343e+00, -2.0201554870e+00, 1.3011627956e+00,
                                    1.4996742515e+00,  -1.5117612237e+00, 1.4924089150e-01,  2.0687387995e+00,
                                    -1.7073176192e+00, 9.6405578870e-01,  1.5595189444e+00,  -5.0555055410e-01,
                                    -2.2826688700e-02, 1.9547066131e+00,  -1.9730283747e+00, -6.2933263920e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -4.6809460161e+00;
        std::vector<double> grad1_expected = {
            -1.5188331288e+00, 1.2150888820e+00,  -8.6211151428e-02, 1.6915197373e-01,  -6.0452387138e-01,
            7.1969539548e-01,  7.5484688730e-02,  -8.0306228030e-01, -5.0677514954e-01, -3.5986604167e-01,
            7.3071446958e-01,  -9.3337659496e-02, 1.7915096595e+00,  4.6879750479e+00,  -9.0078758624e-01,
            -1.8093263104e-01, -2.4217224955e+00, 7.0186490189e-01,  -2.2277890034e-01, -2.5137943521e+00,
            1.3474241589e-01,  -8.6313079442e-01, 1.8580633064e+00,  -2.3897556286e-01, -4.0831828309e-01,
            4.6761239152e-01,  -4.6548021749e-02, 1.2741013525e-01,  -3.1592132318e-01, 1.8425100882e-01,
            1.0806661570e-01,  -3.6150186574e-01, -9.1242476076e-02, -2.5731493874e-01, 5.8096035539e-01,
            -7.6099587311e-02};
        std::vector<double> grad2_expected = {
            4.7595951006e+00,  -1.8290826665e+00, -5.2092601467e-02, -2.5175503784e+00, 1.8239281777e-01,
            -1.3377660825e-01, 1.8669521930e+00,  8.3186708323e-01,  -2.8052908377e-01, -2.4749344073e+00,
            2.7660556560e-01,  4.3302685848e-01,  -1.2100270002e+00, -1.4884412678e+00, 3.3293634007e-01,
            -7.3436334498e-01, -3.4723318506e-01, 1.1065656834e-01,  7.5310599595e-01,  2.1236780750e-01,
            5.4315492785e-01,  6.6661701559e-01,  1.2785138540e-02,  -6.8359200493e-01, 4.0839746883e-01,
            -4.6740713675e-01, 4.7744724973e-02,  -1.2843028811e-01, 3.1575486852e-01,  -1.8565735663e-01,
            2.5742646584e-01,  -5.8066584235e-01, 7.7841880172e-02,  -1.0723717567e-01, 3.6116855259e-01,
            8.9709827797e-02};
        std::vector<double> virial_expected = {-1.6593587598e+00, 6.5202841563e-01, 2.6210791334e-01,
                                               6.5202841563e-01,  1.4809190471e-01, 4.8783754284e-01,
                                               2.6210791334e-01,  4.8783754284e-01, 3.3045624545e+00};

        mbnrg_A1B3_A1B3_deg4::mbnrg_A1B3_A1B3_deg4_v1 ph("nh3", "nh3");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("nh3pbe0d3bj-nh3pbe0d3bj") {
        std::vector<double> xyz1 = {-1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00,
                                    1.6636378008e+00,  -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,
                                    6.8368652710e-01,  -1.8778475277e+00, 4.5835317270e-01,  7.1032361800e-02,
                                    -1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00,
                                    1.6636378008e+00,  -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,
                                    6.8368652710e-01,  -1.8778475277e+00, 4.5835317270e-01,  7.1032361800e-02,
                                    -1.8178036807e+00, 1.4650383854e+00,  -9.8166285200e-02, -2.3900020496e+00,
                                    1.6636378008e+00,  -9.1004414870e-01, -2.2692501928e+00, 1.9242797140e+00,
                                    6.8368652710e-01,  -1.8778475277e+00, 4.5835317270e-01,  7.1032361800e-02};
        std::vector<double> xyz2 = {1.3253591612e+00,  1.5992161603e+00,  -3.5649305290e-01, 1.7112172925e+00,
                                    2.2347398082e+00,  3.3141779120e-01,  3.0734064400e-01,  1.6708149436e+00,
                                    -2.9199566840e-01, 1.5915360076e+00,  1.9683083217e+00,  -1.2614354220e+00,
                                    -1.6441806609e+00, -1.6474447284e+00, 3.9846272690e-01,  -6.2596824480e-01,
                                    -1.7186182488e+00, 3.3660579490e-01,  -2.0279253230e+00, -2.2803120707e+00,
                                    -2.9307077260e-01, -1.9129140343e+00, -2.0201554870e+00, 1.3011627956e+00,
                                    1.4996742515e+00,  -1.5117612237e+00, 1.4924089150e-01,  2.0687387995e+00,
                                    -1.7073176192e+00, 9.6405578870e-01,  1.5595189444e+00,  -5.0555055410e-01,
                                    -2.2826688700e-02, 1.9547066131e+00,  -1.9730283747e+00, -6.2933263920e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -5.6164776849e+00;
        std::vector<double> grad1_expected = {
            -1.5598375928e+00, 1.3103403979e+00,  -9.8812169850e-02, -1.3575578895e-01, -5.9629358266e-01,
            6.2370881600e-01,  -2.1185644179e-01, -7.5759030236e-01, -3.7477353030e-01, -3.9500269720e-01,
            5.2561695601e-01,  -5.7090421966e-02, 1.7748968157e+00,  3.0511164748e+00,  -6.3180904818e-01,
            -2.2916843415e-01, -2.2935053954e+00, 5.1497251612e-01,  -2.4613156302e-01, -2.3318612706e+00,
            2.7639450960e-01,  -8.3976220748e-01, 3.8469994070e+00,  -5.6620312979e-01, -3.2741065550e-01,
            4.1632595632e-01,  -4.4264814500e-02, 8.6503059871e-02,  -2.8262939442e-01, 1.6854738592e-01,
            6.8840035210e-02,  -3.2375286071e-01, -8.1034651703e-02, -3.2527988260e-01, 5.7447119856e-01,
            -7.0129627562e-02};
        std::vector<double> grad2_expected = {
            3.1052582442e+00,  -1.7928261274e+00, 6.6138864830e-02,  -2.3489589589e+00, 2.4340033643e-01,
            1.3033445681e-02,  3.8771509943e+00,  7.8467991418e-01,  -4.2365811282e-01, -2.3309977588e+00,
            2.8267240791e-01,  2.5145310843e-01,  -1.3058339834e+00, -1.5279125474e+00, 3.4630338119e-01,
            -5.2662042456e-01, -3.8441374066e-01, 1.0169421016e-01,  7.2137254391e-01,  -9.8906110434e-02,
            4.7863319099e-01,  6.5124725299e-01,  -2.6151681740e-01, -5.1998563009e-01, 3.2745653783e-01,
            -4.1619592362e-01, 4.5024292138e-02,  -8.7275110616e-02, 2.8234376468e-01,  -1.6962043309e-01,
            3.2537682411e-01,  -5.7420623056e-01, 7.1668257914e-02,  -6.8210808308e-02, 3.2364348975e-01,
            7.9809590886e-02};
        std::vector<double> virial_expected = {-2.7126357584e+00, 7.6529433128e-01,  2.5105447851e-01,
                                               7.6529433128e-01,  -6.9620223050e-01, 4.6001679959e-01,
                                               2.5105447851e-01,  4.6001679959e-01,  2.3515367431e+00};

        mbnrg_A1B3_A1B3_deg4::mbnrg_A1B3_A1B3_deg4_v1 ph("nh3pbe0d3bj", "nh3pbe0d3bj");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1_A1_deg9_v1::struct") {
    SECTION("ar-ar") {
        std::vector<double> xyz1 = {-6.1315209000e-01, 2.4697633600e+00, 2.0700508600e+00};
        std::vector<double> xyz2 = {-1.0000000000e+00, -2.0000000000e+00, -3.0000000000e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -4.2400811422e-03;
        std::vector<double> grad1_expected = {1.9345549728e-04, 2.2352461294e-03, 2.5354388249e-03};
        std::vector<double> grad2_expected = {-1.9345549728e-04, -2.2352461294e-03, -2.5354388249e-03};
        std::vector<double> virial_expected = {-7.4837854799e-05, -8.6470029351e-04, -9.8082921033e-04,
                                               -8.6470029351e-04, -9.9910212500e-03, -1.1332811561e-02,
                                               -9.8082921033e-04, -1.1332811561e-02, -1.2854803795e-02};

        mbnrg_A1_A1_deg9::mbnrg_A1_A1_deg9_v1 ph("ar", "ar");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("cl-cl") {
        std::vector<double> xyz1 = {1.6216168900e+00, -1.0270244000e+00, 3.9469574100e+00};
        std::vector<double> xyz2 = {4.6216168900e+00, -2.0270244000e+00, 2.9469574100e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -1.4227642188e+00;
        std::vector<double> grad1_expected = {1.4373762604e+00, -4.7912542013e-01, -4.7912542013e-01};
        std::vector<double> grad2_expected = {-1.4373762604e+00, 4.7912542013e-01, 4.7912542013e-01};
        std::vector<double> virial_expected = {4.3121287811e+00,  -1.4373762604e+00, -1.4373762604e+00,
                                               -1.4373762604e+00, 4.7912542013e-01,  4.7912542013e-01,
                                               -1.4373762604e+00, 4.7912542013e-01,  4.7912542013e-01};

        mbnrg_A1_A1_deg9::mbnrg_A1_A1_deg9_v1 ph("cl-", "cl-");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("na-na") {
        std::vector<double> xyz1 = {1.6216168900e+00, -1.0270244000e+00, 1.9469574100e+00,
                                    1.6216168900e+00, -1.0270244000e+00, 1.9469574100e+00};
        std::vector<double> xyz2 = {1.6216168900e+00, -3.0270244000e+00, 2.9469574100e+00,
                                    4.0831958000e+00, -1.4459911350e+00, 3.0238165600e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 3.3682991352e-01;
        std::vector<double> grad1_expected = {0.0000000000e+00, 9.0341454859e-01,  -4.5170727429e-01,
                                              1.4623001530e-01, -2.4888705302e-02, 6.3970782873e-02};
        std::vector<double> grad2_expected = {0.0000000000e+00,  -9.0341454859e-01, 4.5170727429e-01,
                                              -1.4623001530e-01, 2.4888705302e-02,  -6.3970782873e-02};
        std::vector<double> virial_expected = {3.5995672166e-01,  -6.1265512068e-02, 1.5746912998e-01,
                                               -6.1265512068e-02, -1.7964015576e+00, 8.7661291855e-01,
                                               1.5746912998e-01,  8.7661291855e-01,  -3.8281975143e-01};

        mbnrg_A1_A1_deg9::mbnrg_A1_A1_deg9_v1 ph("na+", "na+");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1_B1_deg9_v1::struct") {
    SECTION("na-cl") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                    0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {2.1000000000e+00, 3.2000000000e+00, 1.5000000000e+00,
                                    2.1500000000e+01, 3.1000000000e+00, 9.0000000000e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -1.2616368300e+00;
        std::vector<double> grad1_expected = {-2.7571163122e-01, -4.2013200948e-01, -1.9693687945e-01,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> grad2_expected = {2.7571163122e-01, 4.2013200948e-01, 1.9693687945e-01,
                                              0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> virial_expected = {-5.7899442557e-01, -8.8227721991e-01, -4.1356744683e-01,
                                               -8.8227721991e-01, -1.3444224303e+00, -6.3019801422e-01,
                                               -4.1356744683e-01, -6.3019801422e-01, -2.9540531917e-01};

        mbnrg_A1_B1_deg9::mbnrg_A1_B1_deg9_v1 ph("na+", "cl-");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }

    SECTION("ar-cs") {
        std::vector<double> xyz1 = {-1.0000000000e+00, -2.0000000000e+00, -3.0000000000e+00};
        std::vector<double> xyz2 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 5.9630264568e-02;
        std::vector<double> grad1_expected = {4.0515213797e-01, 8.1030427594e-01, 1.2154564139e+00};
        std::vector<double> grad2_expected = {-4.0515213797e-01, -8.1030427594e-01, -1.2154564139e+00};
        std::vector<double> virial_expected = {4.0515213797e-01, 8.1030427594e-01, 1.2154564139e+00,
                                               8.1030427594e-01, 1.6206085519e+00, 2.4309128278e+00,
                                               1.2154564139e+00, 2.4309128278e+00, 3.6463692417e+00};

        mbnrg_A1_B1_deg9::mbnrg_A1_B1_deg9_v1 ph("cs+", "ar");
        double e_nograd = ph.eval(xyz2.data(), xyz1.data(), ndim);
        double e = ph.eval(xyz2.data(), xyz1.data(), grad2.data(), grad1.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1_B2_deg7_v1::struct") {
    SECTION("cs-h2") {
        std::vector<double> xyz1 = {3.1323791000e+00,  -2.6311071100e-01, 1.8748945900e-01,
                                    -7.8557467500e-01, -2.1325879200e+00, 1.6833544100e+00};
        std::vector<double> xyz2 = {1.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 3.0238165600e+00,  1.6216168900e+00,  -1.0270244000e+00,
                                    2.9469574100e+00,  2.0831958000e+00,  -4.4599113500e-01, 3.0238165600e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 1.5613206146e+00;
        std::vector<double> grad1_expected = {-1.5564792945e+00, -3.6618387032e-01, 3.9566584773e+00,
                                              3.3805952416e+00,  1.3302459741e+00,  1.8742593196e+00};
        std::vector<double> grad2_expected = {1.1947045472e+00,  1.2882275657e+00,  -3.7176835252e-01,
                                              3.6177474729e-01,  -9.2204369540e-01, -3.5848901248e+00,
                                              -3.9298944622e+00, -8.6197335060e-01, -2.4856165564e+00,
                                              5.4929922062e-01,  -4.6827262349e-01, 6.1135723682e-01};
        std::vector<double> virial_expected = {1.0068677920e+01,  4.3971143324e+00, -9.3348811271e-02,
                                               4.3971143324e+00,  2.5582243230e+00, 7.7728867977e-01,
                                               -9.3348811271e-02, 7.7728867977e-01, 1.3515135170e+01};

        mbnrg_A1_B2_deg7::mbnrg_A1_B2_deg7_v1 ph("cs+", "h2");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("x2b_A1B2C4_D1E2_v1x::struct") {
    SECTION("h2o-n2o5") {
        std::vector<double> xyz1 = {
            9.7496420000e+00, 1.2183527000e+01, 7.3906700000e+00, 1.0555377000e+01, 1.2319729000e+01, 7.8897220000e+00,
            1.0049082000e+01, 1.1915591000e+01, 6.5216610000e+00, 1.0785064000e+01, 1.1798443000e+01, 4.9853560000e+00,
            1.1395789000e+01, 1.2325561000e+01, 4.4696820000e+00, 1.1168816000e+01, 1.0921351000e+01, 4.9884520000e+00};
        std::vector<double> xyz2 = {
            8.2680440000e+00, 1.0783073000e+01, 1.0117501000e+01, 9.3865410000e+00, 1.1620399000e+01, 1.0436688000e+01,
            8.3598230000e+00, 9.4073400000e+00, 9.8844810000e+00, 1.0525770000e+01, 1.1253619000e+01, 1.0065799000e+01,
            9.0863690000e+00, 1.2796432000e+01, 1.0771404000e+01, 9.3304000000e+00, 8.9002400000e+00, 1.0425331000e+01,
            7.2746500000e+00, 8.8236580000e+00, 9.7258010000e+00, 8.2680440000e+00, 1.0783073000e+01, 1.0117501000e+01,
            9.3865410000e+00, 1.1620399000e+01, 1.0436688000e+01, 8.3598230000e+00, 9.4073400000e+00, 9.8844810000e+00,
            1.0525770000e+01, 1.1253619000e+01, 1.0065799000e+01, 9.0863690000e+00, 1.2796432000e+01, 1.0771404000e+01,
            9.3304000000e+00, 8.9002400000e+00, 1.0425331000e+01, 7.2746500000e+00, 8.8236580000e+00, 9.7258010000e+00};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 2;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -9.7971418759e-01;
        std::vector<double> grad1_expected = {
            1.6963256694e+00,  -3.4319382890e-01, -1.7178336323e-01, -8.5287264060e-01, 7.2949502248e-01,
            -1.5977322405e+00, -2.3531175402e-01, -5.5377536795e-02, 1.7436672262e-01,  4.9580515878e-03,
            3.3471496344e-01,  -1.0148595880e-01, -1.2152103243e-01, 2.1063237900e-02,  4.0591199072e-02,
            1.0315940658e-01,  -3.6050949993e-01, 9.7311061218e-02};
        std::vector<double> grad2_expected = {
            -2.0928816996e+00, -1.8971027549e+00, 4.1188942604e-01,  3.0475019059e+00,  2.6407900107e-01,
            -7.5074405536e-01, -1.1267797500e+00, 4.0254201703e-01,  -2.0890173315e+00, -4.3459290533e-01,
            -7.9230067820e-01, 8.6617872530e-01,  -7.4929038585e-01, 2.1108771609e+00,  1.7779561170e+00,
            9.0072445686e-01,  2.0555770148e-02,  8.8272377747e-01,  -1.5282289673e-01, -4.3957417286e-01,
            4.9616222220e-01,  1.5202267696e-01,  1.7443309701e-01,  7.7287163945e-02,  -1.2646199680e+00,
            -3.4958717676e-01, -6.7892837984e-02, -9.6850686091e-02, -3.7273878505e-02, -1.3991728438e-01,
            8.1689212567e-01,  1.4499275966e-02,  -6.7059656934e-02, 3.0073041501e-01,  3.1142388032e-01,
            8.4574521875e-02,  1.6998955863e-01,  -7.8274233251e-02, 3.7000577398e-02,  -6.4760547918e-02,
            -3.0489666190e-02, 3.9591214595e-02};
        std::vector<double> virial_expected = {-3.8065335588e+00, -1.4937934807e+00, 7.9171645329e-01,
                                               -1.4937934807e+00, -5.8667234441e+00, -1.2002229081e+00,
                                               7.9171645329e-01,  -1.2002229081e+00, -4.7012499422e+00};

        x2b_A1B2C4_D1E2_deg3::x2b_A1B2C4_D1E2_v1x ph("h2o", "n2o5");
        double e_nograd = ph.eval(xyz2.data(), xyz1.data(), ndim);
        double e = ph.eval(xyz2.data(), xyz1.data(), grad2.data(), grad1.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1B2Z2_A1B2Z2_deg4_vmbpbe::struct") {
    SECTION("mbpbe-mbpbe") {
        std::vector<double> xyz1 = {1.7446413520e+01,  -2.3400777000e+00, -1.3656342560e-01, 1.7336517080e+01,
                                    -1.3991926120e+00, 6.3450725850e-02,  1.6804980910e+01,  -2.8010115110e+00,
                                    4.0279153980e-01,  1.7446413520e+01,  -2.3400777000e+00, -1.3656342560e-01,
                                    1.7336517080e+01,  -1.3991926120e+00, 6.3450725850e-02,  1.6804980910e+01,
                                    -2.8010115110e+00, 4.0279153980e-01,  1.8130286090e+01,  3.4634481370e-01,
                                    2.3908143380e-01,  1.9018808010e+01,  -3.7362024750e-02, 2.1367451900e-01,
                                    1.8150356650e+01,  1.0128089980e+00,  9.2480894580e-01};
        std::vector<double> xyz2 = {1.8130286090e+01,  3.4634481370e-01,  2.3908143380e-01,  1.9018808010e+01,
                                    -3.7362024750e-02, 2.1367451900e-01,  1.8150356650e+01,  1.0128089980e+00,
                                    9.2480894580e-01,  2.0136387580e+01,  -1.5728360700e+00, -1.0436900630e-01,
                                    1.9362877520e+01,  -2.1550800780e+00, -1.0903138800e-01, 2.0809672630e+01,
                                    -2.0327938160e+00, 3.9555665570e-01,  2.0136387580e+01,  -1.5728360700e+00,
                                    -1.0436900630e-01, 1.9362877520e+01,  -2.1550800780e+00, -1.0903138800e-01,
                                    2.0809672630e+01,  -2.0327938160e+00, 3.9555665570e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 3;
        std::vector<double> virial(9, 0.0);

        double energy_expected = 9.3820452886e+00;
        std::vector<double> grad1_expected = {
            3.9991966717e+00,  3.8504833353e+00,  -7.9398801654e-01, 3.1201698348e+00,  8.6384359323e+00,
            9.1944809270e-01,  -8.2288828838e-01, 1.6360921378e+00,  1.0337164244e+00,  1.5369776330e+01,
            -1.7235522998e+00, -9.3911951619e-01, 9.0664390530e-01,  2.5375348884e+00,  8.3092081917e-01,
            -8.5299942602e-01, 7.2731318063e-01,  6.0592666054e-01,  1.1427641309e+00,  -5.1019364729e+00,
            -2.0284319613e+00, 5.9422954971e+00,  -6.9111495488e+00, -1.4589592300e+00, 1.9770148961e+00,
            -2.5276587204e-01, 6.7084497369e-01};
        std::vector<double> grad2_expected = {
            -9.2893315360e+00, -1.2276667099e+01, -1.7533917501e+00, 1.8460834481e+00,  -2.1284726556e+00,
            1.1443842215e-01,  1.1467698699e+00,  2.8012834901e-01,  4.7977682742e-01,  -5.2515874052e+00,
            1.7886357267e+00,  -8.1964706196e-01, -9.0854884512e+00, -1.6144681643e+00, -2.2539522418e-01,
            -1.0863449527e+00, -1.7154633316e+00, 5.4731432262e-01,  -6.1000720851e+00, 1.4096392130e+01,
            2.0404632737e+00,  -2.7099953999e+00, -6.1845260288e-01, 4.7429503998e-01,  -2.5200703904e-01,
            -1.2120876335e+00, 3.0178790401e-01};
        std::vector<double> virial_expected = {4.8135920746e+01,  2.1861729575e-01, -1.4822121229e+00,
                                               2.1861729575e-01,  4.7109297084e+01, 7.2629695736e+00,
                                               -1.4822121229e+00, 7.2629695736e+00, -1.0621420750e+00};

        mbnrg_A1B2Z2_A1B2Z2_deg4::mbnrg_A1B2Z2_A1B2Z2_deg4_vmbpbe ph("mbpbe", "mbpbe");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("mbnrg_A1B4_C1D2_deg4_v1::struct") {
    SECTION("ch4-co2") {
        std::vector<double> xyz1 = {-1.1036011760e-02, 2.2989672850e-05,  -5.5211266869e-03, 3.8335135909e-01,
                                    1.0250177013e+00,  -1.8279944626e-01, 4.3228412531e-01,  -2.7848809765e-01,
                                    9.4394167282e-01,  -1.0651930778e+00, -4.8427581542e-03, 7.5801374646e-02,
                                    3.8105911850e-01,  -7.4196078293e-01, -7.7115565216e-01};
        std::vector<double> xyz2 = {4.2086974547e+00, 2.8269706666e-02,  -1.5952691537e-02,
                                    4.6800226436e+00, 8.4958789099e-01,  6.4614677056e-01,
                                    3.8068555636e+00, -8.7081093285e-01, -6.3417053592e-01};
        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        size_t ndim = 1;
        std::vector<double> virial(9, 0.0);

        double energy_expected = -9.9052610663e-02;
        std::vector<double> grad1_expected = {
            7.7247396698e-01,  2.4578036350e-01,  2.1155073392e-01,  -6.6581213900e-03, -6.1538666068e-01,
            6.6817548758e-02,  -5.3234228956e-02, 1.8680265806e-01,  -4.3526827458e-01, -3.5643675413e-01,
            -2.1151809692e-01, -1.2686223621e-01, -3.1811402216e-01, 3.4224092105e-01,  2.3196057000e-01};
        std::vector<double> grad2_expected = {6.2619945508e-01,  5.4096475320e-01,  3.6726488343e-01,
                                              -3.5661973077e-01, -4.4107527237e-01, -3.2498839545e-01,
                                              -3.0761056465e-01, -4.7808665833e-02, 9.5251701286e-03};
        std::vector<double> virial_expected = {-1.9830618641e-02, -2.2836574446e-01, -1.1965849996e-01,
                                               -2.2836574446e-01, 1.2535108842e+00,  2.5579857278e-01,
                                               -1.1965849996e-01, 2.5579857278e-01,  8.3463375691e-01};

        mbnrg_A1B4_C1D2_deg4::mbnrg_A1B4_C1D2_deg4_v1 ph("ch4", "co2");
        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);

        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
        REQUIRE(e == Approx(energy_expected).margin(TOL));
        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

// TEST_CASE("mbnrg_A2_A2_deg6_v1::struct") {
//    SECTION("h2-h2") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        size_t ndim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> virial_expected = {};
//
//        mbnrg_A2_A2_deg6::mbnrg_A2_A2_deg6_v1 ph(",");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
//        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("mbnrg_A2_A2_deg6_v1::struct") {
//    SECTION("h2-h2") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        size_t ndim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> virial_expected = {};
//
//        mbnrg_A2_A2_deg6::mbnrg_A2_A2_deg6_v1 ph(",");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
//        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}

// TEST_CASE("mbnrg_A2_A2_deg6_v1::struct") {
//    SECTION("h2-h2") {
//        std::vector<double> xyz1 = {};
//        std::vector<double> xyz2 = {};
//        std::vector<double> grad1(xyz1.size(), 0.0);
//        std::vector<double> grad2(xyz2.size(), 0.0);
//        size_t ndim = ;
//        std::vector<double> virial(9, 0.0);
//
//        double energy_expected = ;
//        std::vector<double> grad1_expected = {};
//        std::vector<double> grad2_expected = {};
//        std::vector<double> virial_expected = {};
//
//        mbnrg_A2_A2_deg6::mbnrg_A2_A2_deg6_v1 ph(",");
//        double e_nograd = ph.eval(xyz1.data(), xyz2.data(), ndim);
//        double e = ph.eval(xyz1.data(), xyz2.data(), grad1.data(), grad2.data(), ndim, &virial);
//
//        REQUIRE(e_nograd == Approx(energy_expected).margin(TOL));
//        REQUIRE(e == Approx(energy_expected).margin(TOL));
//        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
//        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
//        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//    }
//}
