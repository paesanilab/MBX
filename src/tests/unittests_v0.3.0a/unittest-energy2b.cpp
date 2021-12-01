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
#include "potential/2b/energy2b.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("energy2b::get_2b_energy") {
    SECTION("cl-h2o") {
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

        size_t nm = 3;
        std::string mon1 = "cl";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -2.4173367357e+00;

        std::vector<double> grad1_expected = {2.2970263732e-01,  2.1402885602e-01,  1.6224848352e-01,
                                              -3.0027483594e-01, 9.1766915710e-02,  1.6221125271e-01,
                                              7.0597715540e-02,  -3.0607171347e-01, 1.6196782961e-01};
        std::vector<double> grad2_expected = {
            -6.4348061516e+00, -5.1723653280e-01, 3.4977028811e+00,  1.9383053217e+00,  -3.9995892732e-01,
            -2.5350309792e+00, 4.2667981926e+00,  7.0316660410e-01,  -1.1249203855e+00, 3.6638577018e+00,
            -5.3171463666e+00, 3.4944545566e+00,  -6.2171395939e-01, 1.8808268337e+00,  -2.5335654884e+00,
            -2.7418689064e+00, 3.3445526171e+00,  -1.1231003210e+00, 2.7714274801e+00,  5.8284649229e+00,
            3.5008303627e+00,  -1.3169540402e+00, -1.4765776086e+00, -2.5354530610e+00, -1.5250711555e+00,
            -4.0458156008e+00, -1.1273451313e+00};
        std::vector<double> virial_expected = {6.8519918490e+00,  2.6778589990e-06, -8.8122004439e-05,
                                               2.6778589990e-06,  6.8519561292e+00, 1.1337668899e-03,
                                               -8.8122004439e-05, 1.1337668899e-03, 4.8825412595e+00};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
        }
    }
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
        size_t nm = 2;
        std::string mon1 = "h2o";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 4.3150257624e+00;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
        }
    }

    SECTION("co2-co2") {
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
        size_t nm = 4;
        std::string mon1 = "co2";
        std::string mon2 = "co2";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 9.2886922733e-01;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
        }
    }

    SECTION("cs-h2o") {
        std::vector<double> xyz1 = {3.2822936850e+00,  2.4725476270e+00,  -3.3685766700e-01, 3.2822936850e+00,
                                    2.4725476270e+00,  -3.3685766700e-01, 3.2822936850e+00,  2.4725476270e+00,
                                    -3.3685766700e-01, 3.2822936850e+00,  2.4725476270e+00,  -3.3685766700e-01,
                                    3.2822936850e+00,  2.4725476270e+00,  -3.3685766700e-01};
        std::vector<double> xyz2 = {
            -5.2718689600e+00, -5.1184387920e+00, -6.4447592170e-02, -5.0429736460e+00, -5.9701878600e+00,
            3.0875632580e-01,  -6.2136505200e+00, -5.1783815240e+00, -2.3191504100e-01, 8.7247460360e+00,
            -4.0707243320e+00, -4.9403338130e-01, 8.5842549660e+00,  -5.0180937930e+00, -5.2607205250e-01,
            9.6750071550e+00,  -3.9764821660e+00, -4.1521702300e-01, 4.6423862440e+00,  4.8846581920e+00,
            -7.9319686810e-01, 5.5888140450e+00,  5.0435305540e+00,  -8.1230703670e-01, 4.2565170720e+00,
            5.7495827880e+00,  -9.4900547950e-01, 1.9577742960e+00,  1.6498238250e+00,  -2.6803938630e+00,
            2.2798079560e+00,  1.0538141290e+00,  -3.3606054190e+00, 1.0744078230e+00,  1.8837916330e+00,
            -2.9741712370e+00, 2.4393531210e+00,  2.5097433290e+00,  4.9496556790e+00,  2.9546761940e+00,
            2.0407189980e+00,  5.6082326770e+00,  1.7445021530e+00,  2.9323398370e+00,  5.4575274610e+00};
        size_t nm = 5;
        std::string mon1 = "cs";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 1.3721453047e+01;

        std::vector<double> grad1_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  1.2217619534e+01,  2.1657183768e+01, -4.0874001940e+00, -1.1520392382e+01,
            -7.1602691565e+00, -2.0370349227e+01, 1.9688522649e-03, 2.0126892196e-03,  -9.9155025922e-04};
        std::vector<double> grad2_expected = {
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  -1.0496828792e+01, -1.8535980131e+01,
            3.5075669649e+00,  -9.8790579312e-01, -1.5084415221e+00, 2.7992613975e-01,  -7.3288494900e-01,
            -1.6127621150e+00, 2.9990708938e-01,  9.8732161531e+00,  6.1259720188e+00,  1.7461736647e+01,
            7.1252846389e-01,  5.9190414677e-01,  1.4871313656e+00,  9.3464776519e-01,  4.4239299090e-01,
            1.4214812139e+00,  5.2910436184e-03,  -8.3975079034e-03, -1.9211127235e-01, -7.4874812829e-02,
            5.4833950946e-02,  9.0593594441e-02,  6.7614916946e-02,  -4.8449132263e-02, 1.0250922817e-01};
        std::vector<double> virial_expected = {3.3208346030e+01, 3.9881569005e+01, 2.2074636916e+01,
                                               3.9881569005e+01, 6.0060555575e+01, 7.1489379734e+00,
                                               2.2074636916e+01, 7.1489379734e+00, 5.0968165932e+01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("li-h2o") {
        std::vector<double> xyz1 = {-1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
                                    1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
                                    -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00,
                                    -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
                                    1.5460063700e-01,  -1.0620904050e+00};
        std::vector<double> xyz2 = {
            1.7474594210e+00,  -1.4737233740e+00, -1.3296783390e+00, 2.2444822770e+00,  -1.7974450600e+00,
            -2.0835207810e+00, 1.0684055850e+00,  -2.1338516330e+00, -1.1304174950e+00, 7.4372664830e-01,
            1.3745066180e+00,  8.2228117080e-01,  1.5966080550e+00,  9.4041883440e-01,  1.0251293750e+00,
            8.4611620640e-01,  2.3077049320e+00,  1.0097486520e+00,  2.8905925930e+00,  -1.7061104380e-01,
            9.3095149180e-01,  2.7240924310e+00,  -7.8786885510e-01, 2.0726128570e-01,  3.3243787470e+00,
            -6.6848224410e-01, 1.6262376630e+00,  -7.5858982760e-01, -2.2804310900e+00, -5.8461710420e-01,
            -1.3281652060e+00, -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00, -1.9057671840e+00,
            2.3302304610e-01,  -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01,
            1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00,
            -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,
            1.4232346440e+00,  -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00};
        size_t nm = 6;
        std::string mon1 = "li";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 7.6007164699e+00;

        std::vector<double> grad1_expected = {
            4.1119010018e+00,  -3.6068196949e+00, 2.6229284736e-01,  3.0309574526e+00,  3.6329851388e+00,
            6.9128204697e+00,  1.3495243208e-01,  9.5674683775e-02,  -2.8113821566e-02, -5.9689097350e-01,
            -5.0131952742e+00, 1.5742283138e+00,  -3.4686716454e+00, 5.7613319393e+00,  -8.8429057905e+00,
            -1.9349081275e+00, -1.2459139035e+00, 4.7900459324e+00};
        std::vector<double> grad2_expected = {
            -3.9877621083e+00, 3.4502566355e+00,  -4.0490509777e+00, -1.1748994038e-01, -8.4942782557e-02,
            3.6255623717e+00,  -6.6489531430e-03, 2.4150584196e-01,  1.6119575870e-01,  -2.7640301479e+00,
            -1.3850337559e+00, -8.9379926031e+00, -5.4641433447e-02, -3.9138079927e-01, 5.3407082783e-01,
            -2.1228587126e-01, -1.8565705837e+00, 1.4911013056e+00,  -4.8399233995e-02, -3.1871106150e-02,
            1.4347814836e+00,  -2.7145695869e-01, -7.3404225116e-01, -9.6616229221e-01, 1.8490376061e-01,
            6.7023867353e-01,  -4.4050536985e-01, -1.5994000787e+00, 4.2111123954e+00,  -4.2850206834e+00,
            2.4815614614e+00,  8.1342084227e-01,  2.4156290060e+00,  -2.8527040921e-01, -1.1337963524e-02,
            2.9516336365e-01,  3.9787516637e+00,  -6.7193396727e+00, 1.0337044326e+01,  -3.7966340936e-01,
            4.2366847885e-01,  -7.3595486407e-01, -1.3041660897e-01, 5.3433925455e-01,  -7.5818367101e-01,
            -1.3790203555e+00, 2.2247932052e+00,  -5.4925837169e+00, 1.3191228676e-02,  4.0189793681e-01,
            3.1875082210e-01,  3.3007372543e+00,  -1.3807772385e+00, 3.8378696248e-01};
        std::vector<double> virial_expected = {2.0449258572e+01,  -3.4086293744e+00, 5.3940743537e+00,
                                               -3.4086293744e+00, 3.1869952551e+01,  -7.3084701468e+00,
                                               5.3940743537e+00,  -7.3084701468e+00, 4.1905692682e+01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("ch4-ch4") {
        std::vector<double> xyz1 = {-3.3099600000e+00, 1.4404010000e+01,  1.3046900000e+01,  -2.5090200000e+00,
                                    1.4669480000e+01,  1.2329400000e+01,  -4.1929500000e+00, 1.4316780000e+01,
                                    1.2445600000e+01,  -3.1944300000e+00, 1.3405090000e+01,  1.3412900000e+01,
                                    -3.2439700000e+00, 1.5197490000e+01,  1.3763500000e+01};
        std::vector<double> xyz2 = {-6.9251900000e+00, 1.2729200000e+01,  1.2999000000e+01,  -6.3721200000e+00,
                                    1.3024600000e+01,  1.3829800000e+01,  -7.4085400000e+00, 1.3486100000e+01,
                                    1.2437900000e+01,  -6.3225200000e+00, 1.2150900000e+01,  1.2339100000e+01,
                                    -7.6702900000e+00, 1.1987100000e+01,  1.3413300000e+01};
        size_t nm = 1;
        std::string mon1 = "ch4";
        std::string mon2 = "ch4";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 1.6326245771e-01;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
        }
    }

    SECTION("ch4-h2o") {
        std::vector<double> xyz1 = {
            -5.9895400000e+00, -4.3787000000e-01, 2.5724300000e+00,  -5.2478300000e+00, 1.0363000000e-01,
            1.9881600000e+00,  -6.5511600000e+00, 2.3073000000e-01,  3.1955700000e+00,  -6.6391500000e+00,
            -8.8267000000e-01, 1.9054200000e+00,  -5.4527200000e+00, -1.1652700000e+00, 3.1896100000e+00,
            -5.9895400000e+00, -4.3787000000e-01, 2.5724300000e+00,  -5.2478300000e+00, 1.0363000000e-01,
            1.9881600000e+00,  -6.5511600000e+00, 2.3073000000e-01,  3.1955700000e+00,  -6.6391500000e+00,
            -8.8267000000e-01, 1.9054200000e+00,  -5.4527200000e+00, -1.1652700000e+00, 3.1896100000e+00};
        std::vector<double> xyz2 = {-4.4590985000e-03, -5.1342579600e-02, 3.0000158138e+00,  9.8613021140e-01,
                                    -7.4573098400e-02, 3.0000054324e+00,  -1.5974709230e-01, 8.9671808950e-01,
                                    3.0000164932e+00,  -4.4590985000e-03, -5.1342579600e-02, 1.5813800000e-05,
                                    9.8613021140e-01,  -7.4573098400e-02, 5.4324000000e-06,  -1.5974709230e-01,
                                    8.9671808950e-01,  1.6493200000e-05};
        size_t nm = 2;
        std::string mon1 = "ch4";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 9.3915973596e-03;

        std::vector<double> grad1_expected = {
            -3.9709553276e-01, 1.0302282035e-01,  -4.8458735126e-02, 8.4758131776e-02,  4.8669984918e-02,
            -6.2290488571e-02, 1.0462974755e-01,  -5.8123444661e-02, -3.6959458986e-02, 1.2123751266e-01,
            2.1811861739e-02,  5.4625751628e-02,  8.8559594292e-02,  -1.1863480933e-01, 9.3063060490e-02,
            -2.9655905708e-01, -3.7299313780e-02, 1.3047877673e-01,  1.0464636191e-01,  8.2723613327e-02,
            -9.1732665656e-02, 8.7505040843e-02,  -3.9476133032e-02, -6.1322958408e-02, 5.6076963139e-02,
            2.3221153210e-02,  -2.2773280558e-03, 5.2256463928e-02,  -2.9473461294e-02, 2.2084625345e-02};
        std::vector<double> grad2_expected = {7.1983905305e-02,  2.8008512763e-01,  1.3564668044e-03, -1.2769127585e-01,
                                              -2.4872930945e-03, -6.7981912618e-04, 5.3617917016e-02, -2.7434424756e-01,
                                              -6.5677711267e-04, 1.1592813988e-01,  2.8175665333e-01, 4.0248971519e-03,
                                              -1.7606532089e-01, 8.6698614328e-03,  1.5949006613e-03, 5.6211408265e-02,
                                              -2.9012237319e-01, -2.8502477674e-03};
        std::vector<double> virial_expected = {3.6097074212e-01,  -1.5858101032e-01, 1.3089644321e-02,
                                               -1.5858101032e-01, 4.4031923998e-01,  2.5842611038e-01,
                                               1.3089644321e-02,  2.5842611038e-01,  -5.7781384144e-02};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("co2-h2o") {
        std::vector<double> xyz1 = {4.9261400000e+00,  -3.6517300000e+00, -6.8323800000e+00, 5.8144000000e+00,
                                    -4.4223500000e+00, -6.7105400000e+00, 3.9523200000e+00,  -3.0785500000e+00,
                                    -6.9238500000e+00, 4.9261400000e+00,  -3.6517300000e+00, -6.8323800000e+00,
                                    5.8144000000e+00,  -4.4223500000e+00, -6.7105400000e+00, 3.9523200000e+00,
                                    -3.0785500000e+00, -6.9238500000e+00, 4.9261400000e+00,  -3.6517300000e+00,
                                    -6.8323800000e+00, 5.8144000000e+00,  -4.4223500000e+00, -6.7105400000e+00,
                                    3.9523200000e+00,  -3.0785500000e+00, -6.9238500000e+00};
        std::vector<double> xyz2 = {-7.5858982760e-01, -2.2804310900e+00, -5.8461710420e-01, -1.3281652060e+00,
                                    -3.0099821950e+00, -8.3398172910e-01, -1.1158172200e+00, -1.9057671840e+00,
                                    2.3302304610e-01,  -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00,
                                    -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00,
                                    1.3696631160e+00,  -3.3238997190e+00, -1.4297465600e+00, -2.3744893640e-01,
                                    1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,
                                    -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00};
        size_t nm = 3;
        std::string mon1 = "co2";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -2.8795175245e-03;

        std::vector<double> grad1_expected = {
            6.1871896424e-02,  -8.0960881054e-02, 1.2247221932e-02,  -1.1192440140e-01, 1.0525097592e-01,
            -1.7702602355e-02, 5.2027318537e-02,  -2.5409989572e-02, 2.8991500122e-03,  7.6059218790e-02,
            -7.8813160374e-02, 7.1717854811e-03,  -8.1572543209e-02, 7.5384489133e-02,  -1.2664476477e-02,
            1.3035651734e-02,  -2.3287870129e-03, 9.6690360221e-04,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> grad2_expected = {
            7.3406491446e-02,  -3.1340994678e-03, -8.1823784977e-02, -3.5132045080e-02, -6.3884629985e-02,
            -3.9272326640e-02, -4.0249259929e-02, 6.8138624161e-02,  1.2365234203e-01,  3.0797196680e-02,
            5.3810274541e-03,  2.2616323169e-02,  2.6880963564e-02,  1.0141043035e-02,  -4.2034482623e-03,
            -6.5200487559e-02, -9.7646122362e-03, -1.3887087513e-02, 0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
            0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> virial_expected = {7.5124068990e-02,  -1.7321607631e-01, 8.3534170765e-02,
                                               -1.7321607631e-01, 4.6211782646e-02,  -1.2424978514e-01,
                                               8.3534170765e-02,  -1.2424978514e-01, -1.4822584962e-01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("Unknown monomer") {
        std::vector<double> xyz1 = {0.0, 0.0, 0.0};
        std::vector<double> xyz2 = {1.0, 0.0, 0.0};
        size_t nm = 1;
        std::string mon1 = "not_known";
        std::string mon2 = "neither_known";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 0.0;

        std::vector<double> grad1_expected(xyz1.size(), 0.0);
        std::vector<double> grad2_expected(xyz2.size(), 0.0);
        std::vector<double> virial_expected(9, 0.0);

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

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
        size_t nm = 3;
        std::string mon1 = "h2";
        std::string mon2 = "h2";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 1.9131884042e+02;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

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
        size_t nm = 3;
        std::string mon1 = "h2";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 8.0018869775e-02;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("nh3-nh3") {
        std::vector<double> xyz1 = {1.3253591612e+00,  1.5992161603e+00, -3.5649305290e-01, 1.7112172925e+00,
                                    2.2347398082e+00,  3.3141779120e-01, 3.0734064400e-01,  1.6708149436e+00,
                                    -2.9199566840e-01, 1.5915360076e+00, 1.9683083217e+00,  -1.2614354220e+00,
                                    1.3253591612e+00,  1.5992161603e+00, -3.5649305290e-01, 1.7112172925e+00,
                                    2.2347398082e+00,  3.3141779120e-01, 3.0734064400e-01,  1.6708149436e+00,
                                    -2.9199566840e-01, 1.5915360076e+00, 1.9683083217e+00,  -1.2614354220e+00};
        std::vector<double> xyz2 = {-1.6441806609e+00, -1.6474447284e+00, 3.9846272690e-01,  -6.2596824480e-01,
                                    -1.7186182488e+00, 3.3660579490e-01,  -2.0279253230e+00, -2.2803120707e+00,
                                    -2.9307077260e-01, -1.9129140343e+00, -2.0201554870e+00, 1.3011627956e+00,
                                    1.4996742515e+00,  -1.5117612237e+00, 1.4924089150e-01,  2.0687387995e+00,
                                    -1.7073176192e+00, 9.6405578870e-01,  1.5595189444e+00,  -5.0555055410e-01,
                                    -2.2826688700e-02, 1.9547066131e+00,  -1.9730283747e+00, -6.2933263920e-01};
        size_t nm = 2;
        std::string mon1 = "nh3";
        std::string mon2 = "nh3";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -2.5325051670e+00;

        std::vector<double> grad1_expected = {
            4.6753976964e-01,  3.9833722316e-01,  -1.0079490802e-01, -3.5088906727e-01, -1.3470317291e-01,
            -9.3402056764e-02, 5.8400102598e-01,  2.4756999282e-01,  -8.4748899676e-02, -3.3068549238e-01,
            -9.0085806389e-02, 1.8214957182e-01,  1.2096467314e+00,  1.4878133089e+00,  -3.3707211462e-01,
            -7.5680917784e-01, -2.1511998079e-01, -5.4106226986e-01, 7.3422486869e-01,  3.4706582865e-01,
            -1.1217573743e-01, -6.6274482794e-01, -9.7097282348e-03, 6.8417928320e-01};
        std::vector<double> grad2_expected = {
            -4.6761500115e-01, -3.9851276441e-01, 9.9696914798e-02,  -5.8411820802e-01, -2.4782659379e-01,
            8.3107366810e-02,  3.5105310753e-01,  1.3578567167e-01,  9.4618662417e-02,  3.3071386565e-01,
            8.9435449861e-02,  -1.8062665139e-01, -1.7908543189e+00, -4.6864052608e+00, 9.1085294531e-01,
            1.8054962881e-01,  2.4189864108e+00,  -7.0954763673e-01, 8.6402690959e-01,  -1.8566119152e+00,
            2.4765453640e-01,  2.2196018618e-01,  2.5139813366e+00,  -1.4282900625e-01};
        std::vector<double> virial_expected = {1.5894820286e+00,  -2.2838882873e+00, 4.0491829069e-01,
                                               -2.2838882873e+00, -2.5570388621e+00, 9.0883537648e-01,
                                               4.0491829069e-01,  9.0883537648e-01,  1.7484208737e+00};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
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
        size_t nm = 3;
        std::string mon1 = "nh3pbe0d3bj";
        std::string mon2 = "nh3pbe0d3bj";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -5.6164776849e+00;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("ar-ar") {
        std::vector<double> xyz1 = {1.6216168900e+00, -1.0270244000e+00, 2.9469574100e+00,
                                    1.6216168900e+00, -1.0270244000e+00, 2.9469574100e+00};
        std::vector<double> xyz2 = {2.0831958000e+00, -1.4459911350e+00, 3.0238165600e+00,
                                    3.6216168900e+00, -1.0270244000e+00, 2.9469574100e+00};
        size_t nm = 2;
        std::string mon1 = "ar";
        std::string mon2 = "ar";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -2.6455183439e+04;

        std::vector<double> grad1_expected = {-1.2179058127e+05, 1.1054708325e+05, -2.0279783915e+04,
                                              -1.1681346080e+01, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> grad2_expected = {1.2179058127e+05, -1.1054708325e+05, 2.0279783915e+04,
                                              1.1681346080e+01, 0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> virial_expected = {-5.6239326443e+04, 5.1026202189e+04,  -9.3607205545e+03,
                                               5.1026202189e+04,  -4.6315550532e+04, 8.4965548533e+03,
                                               -9.3607205545e+03, 8.4965548533e+03,  -1.5586869539e+03};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("cl-cl") {
        std::vector<double> xyz1 = {1.6216168900e+00, -1.0270244000e+00, 3.9469574100e+00};
        std::vector<double> xyz2 = {4.6216168900e+00, -2.0270244000e+00, 2.9469574100e+00};
        size_t nm = 1;
        std::string mon1 = "cl";
        std::string mon2 = "cl";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -1.4227642188e+00;

        std::vector<double> grad1_expected = {1.4373762604e+00, -4.7912542013e-01, -4.7912542013e-01};
        std::vector<double> grad2_expected = {-1.4373762604e+00, 4.7912542013e-01, 4.7912542013e-01};
        std::vector<double> virial_expected = {4.3121287811e+00,  -1.4373762604e+00, -1.4373762604e+00,
                                               -1.4373762604e+00, 4.7912542013e-01,  4.7912542013e-01,
                                               -1.4373762604e+00, 4.7912542013e-01,  4.7912542013e-01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("na-na") {
        std::vector<double> xyz1 = {1.6216168900e+00, -1.0270244000e+00, 1.9469574100e+00,
                                    1.6216168900e+00, -1.0270244000e+00, 1.9469574100e+00};
        std::vector<double> xyz2 = {1.6216168900e+00, -3.0270244000e+00, 2.9469574100e+00,
                                    4.0831958000e+00, -1.4459911350e+00, 3.0238165600e+00};
        size_t nm = 2;
        std::string mon1 = "na";
        std::string mon2 = "na";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 3.3682991352e-01;

        std::vector<double> grad1_expected = {0.0000000000e+00, 9.0341454859e-01,  -4.5170727429e-01,
                                              1.4623001530e-01, -2.4888705302e-02, 6.3970782873e-02};
        std::vector<double> grad2_expected = {0.0000000000e+00,  -9.0341454859e-01, 4.5170727429e-01,
                                              -1.4623001530e-01, 2.4888705302e-02,  -6.3970782873e-02};
        std::vector<double> virial_expected = {3.5995672166e-01,  -6.1265512068e-02, 1.5746912998e-01,
                                               -6.1265512068e-02, -1.7964015576e+00, 8.7661291855e-01,
                                               1.5746912998e-01,  8.7661291855e-01,  -3.8281975143e-01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("ar-cs") {
        std::vector<double> xyz1 = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                    0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> xyz2 = {2.1000000000e+00, 3.2000000000e+00, 1.5000000000e+00,
                                    2.1500000000e+01, 3.1000000000e+00, 9.0000000000e-01};
        size_t nm = 2;
        std::string mon1 = "ar";
        std::string mon2 = "cs";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -3.9545704132e-01;

        std::vector<double> grad1_expected = {-3.1162849344e-01, -4.7486246619e-01, -2.2259178103e-01,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> grad2_expected = {3.1162849344e-01, 4.7486246619e-01, 2.2259178103e-01,
                                              0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> virial_expected = {-6.5441983622e-01, -9.9721117901e-01, -4.6744274016e-01,
                                               -9.9721117901e-01, -1.5195598918e+00, -7.1229369929e-01,
                                               -4.6744274016e-01, -7.1229369929e-01, -3.3388767154e-01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("na-cl") {
        std::vector<double> xyz1 = {1.4000000000e+01, 3.0000000000e+00, 1.0000000000e+00,
                                    1.4000000000e+01, 3.0000000000e+00, 1.0000000000e+00};
        std::vector<double> xyz2 = {2.1000000000e+00, 3.2000000000e+00, 1.5000000000e+00,
                                    2.1500000000e+01, 3.1000000000e+00, 9.0000000000e-01};
        size_t nm = 2;
        std::string mon1 = "na";
        std::string mon2 = "cl";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -3.4240753238e-02;

        std::vector<double> grad1_expected = {0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              -6.2572475364e-02, -8.3429967152e-04, 8.3429967152e-04};
        std::vector<double> grad2_expected = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
                                              6.2572475364e-02, 8.3429967152e-04, -8.3429967152e-04};
        std::vector<double> virial_expected = {-4.6929356523e-01, -6.2572475364e-03, 6.2572475364e-03,
                                               -6.2572475364e-03, -8.3429967152e-05, 8.3429967152e-05,
                                               6.2572475364e-03,  8.3429967152e-05,  -8.3429967152e-05};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
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
        size_t nm = 3;
        std::string mon1 = "ar";
        std::string mon2 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 7.4400095277e+00;

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

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("h2-cs") {
        std::vector<double> xyz1 = {1.6216168900e+00,  -1.0270244000e+00, 2.9469574100e+00,  2.0831958000e+00,
                                    -4.4599113500e-01, 3.0238165600e+00,  1.6216168900e+00,  -1.0270244000e+00,
                                    2.9469574100e+00,  2.0831958000e+00,  -4.4599113500e-01, 3.0238165600e+00};
        std::vector<double> xyz2 = {3.1323791000e+00,  -2.6311071100e-01, 1.8748945900e-01,
                                    -7.8557467500e-01, -2.1325879200e+00, 1.6833544100e+00};
        size_t nm = 2;
        std::string mon1 = "h2";
        std::string mon2 = "cs";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 1.5613206146e+00;

        std::vector<double> grad1_expected = {1.1947045472e+00,  1.2882275657e+00,  -3.7176835252e-01,
                                              3.6177474729e-01,  -9.2204369540e-01, -3.5848901248e+00,
                                              -3.9298944622e+00, -8.6197335060e-01, -2.4856165564e+00,
                                              5.4929922062e-01,  -4.6827262349e-01, 6.1135723682e-01};
        std::vector<double> grad2_expected = {-1.5564792945e+00, -3.6618387032e-01, 3.9566584773e+00,
                                              3.3805952416e+00,  1.3302459741e+00,  1.8742593196e+00};
        std::vector<double> virial_expected = {1.0068677920e+01,  4.3971143324e+00, -9.3348811271e-02,
                                               4.3971143324e+00,  2.5582243230e+00, 7.7728867977e-01,
                                               -9.3348811271e-02, 7.7728867977e-01, 1.3515135170e+01};

        SECTION("No gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2);
            double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e2b::get_2b_energy(mon1, mon2, nm, xyz1, xyz2, grad1, grad2, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e2b::get_2b_energy(mon2, mon1, nm, xyz2, xyz1, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }

    //    SECTION("h2o-h2o") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        size_t nm = ;
    //        std::string mon1 = ;
    //        std::string mon2 = ;
    //
    //        std::vector<double> grad1(xyz1.size(),0.0);
    //        std::vector<double> grad2(xyz2.size(),0.0);
    //        std::vector<double> virial(9,0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2);
    //            double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e2b::get_2b_energy(mon1,mon2,nm,xyz1,xyz2,grad1,grad2,&virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //            REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //            REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(),grad1.end(),0.0);
    //                std::fill(grad2.begin(),grad2.end(),0.0);
    //                std::fill(virial.begin(),virial.end(),0.0);
    //
    //                double e2 = e2b::get_2b_energy(mon2,mon1,nm,xyz2,xyz1,grad2,grad1,&virial);
    //                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1,grad1_expected,TOL));
    //                REQUIRE(VectorsAreEqual(grad2,grad2_expected,TOL));
    //                REQUIRE(VectorsAreEqual(virial,virial_expected,TOL));
    //            }
    //        }
    //    }
}
