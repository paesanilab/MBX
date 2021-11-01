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
#include "potential/3b/energy3b.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("energy3b::get_3b_energy") {
    SECTION("cl-h2o-h2o") {
        std::vector<double> xyz1 = {-4.4259727000e-03, 7.2497235000e-03, 1.5058480830e+00,
                                    -4.4259727000e-03, 7.2497235000e-03, 1.5058480830e+00,
                                    -4.4259727000e-03, 7.2497235000e-03, 1.5058480830e+00};
        std::vector<double> xyz2 = {1.5227529162e+00,  -8.0192355620e-01, -1.1604165083e+00, 1.3240025487e+00,
                                    -6.0027547870e-01, -2.2274381720e-01, 6.5831177100e-01,  -1.1375239818e+00,
                                    -1.4420001631e+00, 1.5227529162e+00,  -8.0192355620e-01, -1.1604165083e+00,
                                    1.3240025487e+00,  -6.0027547870e-01, -2.2274381720e-01, 6.5831177100e-01,
                                    -1.1375239818e+00, -1.4420001631e+00, -6.6145178200e-02, 1.7367956497e+00,
                                    -1.1588983873e+00, -1.4179538960e-01, 1.4630148967e+00,  -2.2150062790e-01,
                                    6.5683290630e-01,  1.1562191534e+00,  -1.4406948567e+00};
        std::vector<double> xyz3 = {-6.6145178200e-02, 1.7367956497e+00,  -1.1588983873e+00, -1.4179538960e-01,
                                    1.4630148967e+00,  -2.2150062790e-01, 6.5683290630e-01,  1.1562191534e+00,
                                    -1.4406948567e+00, -1.4702921679e+00, -9.0858682050e-01, -1.1603278036e+00,
                                    -1.1957504615e+00, -8.3804995530e-01, -2.2275387400e-01, -1.3288712563e+00,
                                    8.0703453000e-03,  -1.4412466488e+00, -1.4702921679e+00, -9.0858682050e-01,
                                    -1.1603278036e+00, -1.1957504615e+00, -8.3804995530e-01, -2.2275387400e-01,
                                    -1.3288712563e+00, 8.0703453000e-03,  -1.4412466488e+00};
        size_t nm = 3;
        std::string mon1 = "cl";
        std::string mon2 = "h2o";
        std::string mon3 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 2.1391947668e-01;

        std::vector<double> grad1_expected = {-2.3447528989e-01, -1.4983227301e-01, 8.5147585842e-01,
                                              -1.2027431580e-02, 2.7728207015e-01,  8.5168681961e-01,
                                              2.4663737023e-01,  -1.2891026336e-01, 8.5145452972e-01};
        std::vector<double> grad2_expected = {
            9.2696016209e-01,  -5.9346019729e-01, 4.1203661196e-01,  -5.2857452397e-01, 5.6069250586e-01,
            -9.8779818803e-01, -3.6547195652e-01, -1.0561926017e-01, 2.1283412874e-01,  1.6722795726e+00,
            -2.6913842239e-02, -1.4448755970e+00, -1.8065164042e+00, -1.8715753089e-01, 2.3061762044e-01,
            2.8277174200e-01,  -1.0419369140e-01, 7.2538709831e-01,  5.0298632443e-02,  1.0991450046e+00,
            4.1301721708e-01,  -2.2086732239e-01, -7.3723361470e-01, -9.8853212086e-01, 2.7411442384e-01,
            -2.6388924930e-01, 2.1271607230e-01};
        std::vector<double> grad3_expected = {
            -8.1223175252e-01, 1.4629922754e+00,  -1.4439319887e+00, 1.0652557373e+00,  -1.4711182895e+00,
            2.2974095269e-01,  -5.1462376541e-02, 2.9634523873e-01,  7.2564262490e-01,  -9.7719813201e-01,
            -5.0637835253e-01, 4.1217035142e-01,  7.4929952563e-01,  1.7821966020e-01,  -9.8806871065e-01,
            9.1391127495e-02,  3.6914168672e-01,  2.1308241783e-01,  -8.6026782969e-01, -1.4336106839e+00,
            -1.4455596448e+00, 7.4129817245e-01,  1.6578853464e+00,  2.3153549133e-01,  -2.3121344688e-01,
            -1.9338653971e-01, 7.2536845520e-01};
        std::vector<double> virial_expected = {-1.9938382222e+00, -1.9576964049e-05, -1.2457461202e-04,
                                               -1.9576964049e-05, -1.9938278881e+00, 1.1066670256e-03,
                                               -1.2457461202e-04, 1.1066670256e-03,  -3.8870924575e+00};

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

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
        size_t nm = 1;
        std::string mon1 = "h2o";
        std::string mon2 = "h2o";
        std::string mon3 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 1.4662741082e-01;

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

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("li-h2o-h2o") {
        std::vector<double> xyz1 = {
            -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
            -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
            1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00,
            -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
            -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
            1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00,
            -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,
            -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01,
            1.5460063700e-01,  -1.0620904050e+00, -1.7782237730e-01, 1.5460063700e-01,  -1.0620904050e+00};
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
        size_t nm = 15;
        std::string mon1 = "li";
        std::string mon2 = "h2o";
        std::string mon3 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 6.6537845821e-01;

        std::vector<double> grad1_expected = {
            -3.8764486752e-01, 2.5932076282e-01,  -1.0179753554e-01, -5.1792690727e-01, 9.7128747562e-01,
            5.9649142039e-01,  1.0328249145e+00,  -3.3964055475e-01, -7.5137276698e-01, -1.8937053992e-01,
            4.3068595877e-02,  2.9348322539e-01,  -7.1262971881e-02, 5.5774171497e-02,  -6.4263007232e-02,
            6.0986902000e-01,  5.3815306771e-02,  9.5759297985e-01,  -4.3696035600e-02, 8.8216635454e-02,
            -2.0133082986e-01, -6.9679660618e-03, -6.2155604604e-01, 8.6718453794e-03,  -1.1857896753e+00,
            -3.0263452126e-01, 5.5847976963e-01,  6.3436496922e-02,  1.4886864303e-02,  -3.7617026181e-03,
            7.1724828520e-02,  -8.2962126724e-02, 1.3659968426e-01,  1.1362172008e-02,  -3.7883089574e-02,
            -3.3905654994e-02, 1.5857518108e-01,  1.0481729099e-01,  2.3473517633e-01,  -3.0244156111e-01,
            -1.1576087437e+00, -6.4225676721e-01, 2.0785466942e-01,  -1.0546703009e-01, 1.8258975341e-04};
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
        std::vector<double> virial_expected = {3.1513023300e-01,  1.8255055693e+00,  -4.3344338955e-01,
                                               1.8255055693e+00,  -4.2971634514e+00, 4.7780827979e-01,
                                               -4.3344338955e-01, 4.7780827979e-01,  -4.6681227881e+00};

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("cs-h2o-h2o") {
        std::vector<double> xyz1 = {3.2822936850e+00, 2.4725476270e+00, -3.3685766700e-01,
                                    3.2822936850e+00, 2.4725476270e+00, -3.3685766700e-01,
                                    3.2822936850e+00, 2.4725476270e+00, -3.3685766700e-01};
        std::vector<double> xyz2 = {4.6423862440e+00,  4.8846581920e+00,  -7.9319686810e-01, 5.5888140450e+00,
                                    5.0435305540e+00,  -8.1230703670e-01, 4.2565170720e+00,  5.7495827880e+00,
                                    -9.4900547950e-01, 4.6423862440e+00,  4.8846581920e+00,  -7.9319686810e-01,
                                    5.5888140450e+00,  5.0435305540e+00,  -8.1230703670e-01, 4.2565170720e+00,
                                    5.7495827880e+00,  -9.4900547950e-01, 1.9577742960e+00,  1.6498238250e+00,
                                    -2.6803938630e+00, 2.2798079560e+00,  1.0538141290e+00,  -3.3606054190e+00,
                                    1.0744078230e+00,  1.8837916330e+00,  -2.9741712370e+00};
        std::vector<double> xyz3 = {1.9577742960e+00,  1.6498238250e+00,  -2.6803938630e+00, 2.2798079560e+00,
                                    1.0538141290e+00,  -3.3606054190e+00, 1.0744078230e+00,  1.8837916330e+00,
                                    -2.9741712370e+00, 2.4393531210e+00,  2.5097433290e+00,  4.9496556790e+00,
                                    2.9546761940e+00,  2.0407189980e+00,  5.6082326770e+00,  1.7445021530e+00,
                                    2.9323398370e+00,  5.4575274610e+00,  2.4393531210e+00,  2.5097433290e+00,
                                    4.9496556790e+00,  2.9546761940e+00,  2.0407189980e+00,  5.6082326770e+00,
                                    1.7445021530e+00,  2.9323398370e+00,  5.4575274610e+00};
        size_t nm = 3;
        std::string mon1 = "cs";
        std::string mon2 = "h2o";
        std::string mon3 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 7.3272385776e-02;

        std::vector<double> grad1_expected = {-1.4599009063e-02, 2.6487559922e-01, -4.5991578026e-01,
                                              0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> grad2_expected = {9.3949334810e-02, -1.2564575754e-01, 1.8681934406e-01,  -7.2295764517e-02,
                                              1.4944555455e-02, 1.5768804636e-02,  -2.3486089594e-02, -5.4763691067e-02,
                                              1.1704659904e-02, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00,
                                              0.0000000000e+00, 0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> grad3_expected = {-1.6339185807e-02, -1.0387934630e-01, 1.6179388979e-01, -1.5427240398e-02,
                                              2.7195332108e-02,  5.0789917644e-02,  4.8197954569e-02, -2.2726691873e-02,
                                              3.3039164227e-02,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00, 0.0000000000e+00,
                                              0.0000000000e+00,  0.0000000000e+00,  0.0000000000e+00};
        std::vector<double> virial_expected = {1.3116098508e-01, 2.9266789438e-02,  3.6296393155e-02,
                                               2.9266789438e-02, 3.8385024531e-01,  -3.0490577546e-01,
                                               3.6296393155e-02, -3.0490577546e-01, 7.1979561287e-01};

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

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
        size_t nm = 1;
        std::string mon1 = "ch4";
        std::string mon2 = "h2o";
        std::string mon3 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -8.6542153165e-01;

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

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("co2-co2-co2") {
        std::vector<double> xyz1 = {
            6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01,
            -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,  6.6630444410e-01,
            -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01,
            -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,  6.6630444410e-01,  -3.8357176030e-01,
            1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01, -4.4811475090e-01,
            -5.7997649630e-01, 4.1069507510e-01,  6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,
            1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01,
            4.1069507510e-01,  6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,
            -1.9222069500e-01, -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,
            6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01,
            -1.7587628680e-01, -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01};
        std::vector<double> xyz2 = {
            2.4803292099e+00,  7.5103875900e-01,  -2.9043390394e+00, 2.2674715176e+00,  1.8651909097e+00,
            -2.6082983571e+00, 2.7020706245e+00,  -3.5351972400e-01, -3.2106052081e+00, 2.4803292099e+00,
            7.5103875900e-01,  -2.9043390394e+00, 2.2674715176e+00,  1.8651909097e+00,  -2.6082983571e+00,
            2.7020706245e+00,  -3.5351972400e-01, -3.2106052081e+00, 2.4803292099e+00,  7.5103875900e-01,
            -2.9043390394e+00, 2.2674715176e+00,  1.8651909097e+00,  -2.6082983571e+00, 2.7020706245e+00,
            -3.5351972400e-01, -3.2106052081e+00, -1.4243133388e+00, 7.7214187430e-01,  -2.3329941557e+00,
            -2.4475497469e+00, 1.2692755042e+00,  -2.0709757555e+00, -4.0159255460e-01, 2.6460559090e-01,
            -2.5998516399e+00, -1.4243133388e+00, 7.7214187430e-01,  -2.3329941557e+00, -2.4475497469e+00,
            1.2692755042e+00,  -2.0709757555e+00, -4.0159255460e-01, 2.6460559090e-01,  -2.5998516399e+00,
            6.4986549630e-01,  3.4030903031e+00,  -6.8029207450e-01, 7.9296435600e-02,  2.4412841935e+00,
            -3.3312155960e-01, 1.2145816282e+00,  4.3686301334e+00,  -1.0197806380e+00};
        std::vector<double> xyz3 = {
            -1.4243133388e+00, 7.7214187430e-01,  -2.3329941557e+00, -2.4475497469e+00, 1.2692755042e+00,
            -2.0709757555e+00, -4.0159255460e-01, 2.6460559090e-01,  -2.5998516399e+00, 6.4986549630e-01,
            3.4030903031e+00,  -6.8029207450e-01, 7.9296435600e-02,  2.4412841935e+00,  -3.3312155960e-01,
            1.2145816282e+00,  4.3686301334e+00,  -1.0197806380e+00, 5.1568113210e-01,  3.8303951459e+00,
            -4.0222969886e+00, -3.7522694700e-01, 3.2992897240e+00,  -3.4756612500e+00, 1.3973748074e+00,
            4.3616623515e+00,  -4.5737147405e+00, 6.4986549630e-01,  3.4030903031e+00,  -6.8029207450e-01,
            7.9296435600e-02,  2.4412841935e+00,  -3.3312155960e-01, 1.2145816282e+00,  4.3686301334e+00,
            -1.0197806380e+00, 5.1568113210e-01,  3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01,
            3.2992897240e+00,  -3.4756612500e+00, 1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00,
            5.1568113210e-01,  3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01, 3.2992897240e+00,
            -3.4756612500e+00, 1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00};
        size_t nm = 6;
        std::string mon1 = "co2";
        std::string mon2 = "co2";
        std::string mon3 = "co2";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -2.9409183708e-02;

        std::vector<double> grad1_expected = {
            -2.2393574060e-01, -6.1256986419e-02, 2.3264725800e-01,  1.1782780887e-01,  3.5096359374e-02,
            -8.5666668051e-02, 8.2523962249e-02,  2.9186736265e-02,  -1.2584736468e-01, 2.9003484020e-01,
            -6.3664935562e-02, -2.1645665940e-02, -1.3504246528e-01, -3.6704494309e-03, 5.6990889249e-02,
            -1.5280091268e-01, -2.5157456601e-03, 1.0895156267e-02,  1.1086451311e-01,  -1.1060185672e-02,
            -2.1053898568e-03, -6.4779717627e-02, 2.1125226000e-02,  7.8878596870e-03,  -5.3839809774e-02,
            -3.5085379829e-03, 4.5351806207e-03,  1.0781199201e-01,  -1.0738924759e-01, 4.8559442552e-02,
            3.9716989725e-02,  2.9980758536e-02,  -1.0851416322e-04, -7.0270452913e-02, 5.4260004751e-02,
            5.2040718688e-03,  6.8664674332e-02,  1.1298032953e-02,  -3.8531508839e-02, -3.0316100895e-02,
            -3.5150734101e-03, 1.1749941515e-02,  -4.8391761972e-02, -4.2689970671e-03, 1.6299181318e-02,
            -8.6038852670e-02, 8.7223331146e-02,  6.0024136708e-03,  4.4992453429e-02,  -2.0684666565e-02,
            -2.5320703813e-03, 3.3737025337e-02,  -3.0574374341e-02, -6.3863764664e-03};
        std::vector<double> grad2_expected = {
            8.3666542758e-02,  -6.4410895359e-02, 6.7864262774e-02,  -3.6524197356e-02, 6.1007962961e-02,
            -4.1608970365e-02, -3.1314653910e-02, 1.3589023180e-02,  -6.1810888301e-02, 8.6484744583e-03,
            3.3204507350e-01,  -3.5506982568e-02, 1.1304379696e-02,  -1.2634033179e-01, -9.1451320699e-03,
            1.9248834223e-02,  -2.3351091458e-01, -3.2331209420e-02, 2.5426241560e-02,  5.7505794531e-02,
            -2.8402623836e-02, -1.3713610729e-02, -2.6915605301e-02, 1.8551597573e-02,  -9.4428862264e-03,
            -3.3097350335e-02, -3.3954868109e-03, -2.7229991113e-01, 4.1030254718e-02,  -2.8761088950e-02,
            1.2974682252e-01,  -7.1019916330e-03, -2.4071589584e-02, 9.0006314682e-02,  -5.0707822640e-04,
            -2.1525316140e-02, 2.0851545795e-02,  -4.5834397736e-02, 2.4973404504e-02,  -1.4324442344e-02,
            1.3921727546e-02,  4.0258491718e-03,  -6.1573406645e-03, 2.6142622096e-02,  -7.9606278966e-03,
            -4.7570041984e-03, -3.1225622137e-02, 3.5815757671e-02,  -7.5163824755e-03, -3.8448518199e-02,
            -1.6051640315e-02, 2.5450189923e-02,  3.6621477092e-02,  -2.9072792622e-02};
        std::vector<double> grad3_expected = {
            -2.6815824329e-01, 1.7360939052e-01,  1.3248504136e-01,  2.0335552506e-01,  -1.1714310498e-01,
            -6.4389172658e-02, 7.2558996211e-02,  -6.9678485533e-02, -5.3673498075e-02, -1.8797859640e-01,
            -1.0569968911e-01, 3.4683045571e-02,  6.8893443281e-02,  1.2738190482e-01,  2.0975217430e-04,
            7.7692002495e-02,  7.5975087818e-02,  -4.1498532633e-03, 6.2976687049e-03,  -3.9310493918e-02,
            -1.0165321414e-03, -1.5513681990e-02, 1.0148718755e-02,  1.2650433758e-02,  1.4701282973e-02,
            2.5112433923e-02,  -8.7050389947e-03, 1.5315113520e-01,  2.1444151625e-01,  -1.2009779232e-01,
            -6.0552032771e-02, -4.2021292832e-02, 7.6248489161e-02,  -1.1731085732e-01, -1.8269292397e-01,
            6.4552297575e-02,  2.5742197565e-02,  3.5597899761e-02,  1.0587414919e-02,  -8.3902475848e-03,
            -2.5740855628e-02, -1.6621541009e-02, -7.6785242321e-03, -7.6009585147e-03, -4.5221136829e-03,
            9.3762975436e-03,  -1.4270569121e-03, 2.6441988082e-02,  2.6004064081e-03,  2.9069938333e-03,
            -1.5725153837e-02, -1.7844133297e-02, -4.3915639172e-03, 1.5078741983e-03};
        std::vector<double> virial_expected = {-2.3665599707e-01, 6.1254058810e-02,  7.8075547317e-02,
                                               6.1254058810e-02,  -2.2767072231e-01, -5.6217795636e-02,
                                               7.8075547317e-02,  -5.6217795636e-02, -4.2417660822e-01};

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("Unknow Trimer") {
        std::vector<double> xyz1 = {0.0, 0.0, 0.0};
        std::vector<double> xyz2 = {0.0, 0.0, 0.0};
        std::vector<double> xyz3 = {0.0, 0.0, 0.0};
        size_t nm = 1;
        std::string mon1 = "notAmon";
        std::string mon2 = "neitherAmon";
        std::string mon3 = "AlsoNotMon";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 0.0;

        std::vector<double> grad1_expected = {0.0, 0.0, 0.0};
        std::vector<double> grad2_expected = {0.0, 0.0, 0.0};
        std::vector<double> grad3_expected = {0.0, 0.0, 0.0};
        std::vector<double> virial_expected(9, 0.0);

        SECTION("No gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3);
            double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3);
            double e3 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2);
            double e4 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e3b::get_3b_energy(mon1, mon2, mon3, nm, xyz1, xyz2, xyz3, grad1, grad2, grad3, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon2, mon1, mon3, nm, xyz2, xyz1, xyz3, grad2, grad1, grad3, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon3, mon2, mon1, nm, xyz3, xyz2, xyz1, grad3, grad2, grad1, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e3b::get_3b_energy(mon1, mon3, mon2, nm, xyz1, xyz3, xyz2, grad1, grad3, grad2, &virial);
                REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }
}
