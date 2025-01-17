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
#include "potential/4b/energy4b.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("energy4b::get_4b_energy") {
    SECTION("h2o-h2o-h2o-h2o") {
        std::vector<double> xyz1 = {-1.502169000e+00, -1.913590000e-01,  1.434927000e+00,
                                    -2.006698000e+00, -4.223270000e-01,  2.219847000e+00,
                                    -6.010540000e-01, -5.969720000e-01,  1.553718000e+00};
        std::vector<double> xyz2 = {-1.744575000e+00, -3.823480000e-01, -1.309144000e+00,
                                    -2.516835000e+00, -7.667650000e-01, -1.733766000e+00,
                                    -1.888941000e+00, -4.796530000e-01, -3.476240000e-01};
        std::vector<double> xyz3 = {-5.604090000e-01,  2.017830000e+00, -1.219840000e-01,
                                    -9.898310000e-01,  1.592736000e+00, -8.774190000e-01,
                                    -9.477200000e-01,  1.533567000e+00,  6.252280000e-01};
        std::vector<double> xyz4 = { 9.648030000e-01, -1.165765000e+00,  1.439987000e+00,
                                     1.542224000e+00, -3.936920000e-01,  1.344373000e+00,
                                     9.795570000e-01, -1.522041000e+00,  5.278330000e-01};
        size_t nm = 1;
        std::string mon1 = "h2o";
        std::string mon2 = "h2o";
        std::string mon3 = "h2o";
        std::string mon4 = "h2o";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> grad4(xyz4.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = -1.41412667e-02;

        std::vector<double> grad1_expected = { 2.90893268e-01,  7.66194028e-02, -1.77955149e-01,
                                              -1.40050653e-01, -4.71073833e-02,  1.90729097e-01,
                                              -1.39714605e-01, -8.79830573e-04,  3.19933510e-02};
        std::vector<double> grad2_expected = { 7.86526073e-02,  7.77764046e-02, -2.01664585e-02,
                                              -2.45400991e-02, -1.95496408e-02,  8.09018690e-03,
                                              -6.25930146e-02, -5.84377594e-02,  5.76763191e-03};
        std::vector<double> grad3_expected = {-4.78112809e-02, -8.49569872e-02,  8.26381604e-03,
                                               1.41152306e-02,  5.44546440e-02, -4.16471029e-02,
                                              -1.30768813e-02,  2.17870874e-02,  6.77102240e-03};
        std::vector<double> grad4_expected = { 3.73009762e-02, -1.66603552e-02, -4.18771842e-02,
                                               2.83780954e-03,  5.45207393e-03,  1.28248656e-02,
                                               3.98664175e-03, -8.49765630e-03,  1.72059237e-02};
        std::vector<double> virial_expected = {-4.03122325e-02, 3.90621186e-02,  1.04315995e-01,
                                                3.90621186e-02, 2.03520905e-03,  8.85472520e-02,
                                                1.04315995e-01, 8.85472520e-02, -2.33902746e-01};

        SECTION("No gradients") {
            double e = e4b::get_4b_energy(mon1, mon2, mon3, mon4, nm, xyz1, xyz2, xyz3, xyz4);
            double e2 = e4b::get_4b_energy(mon2, mon1, mon4, mon3, nm, xyz2, xyz1, xyz4, xyz3);
            double e3 = e4b::get_4b_energy(mon1, mon4, mon3, mon2, nm, xyz1, xyz4, xyz3, xyz2);
            double e4 = e4b::get_4b_energy(mon4, mon3, mon2, mon1, nm, xyz4, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e4b::get_4b_energy(mon1, mon2, mon3, mon4, nm,
            xyz1, xyz2, xyz3, xyz4, grad1, grad2, grad3, grad4, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon2, mon1, mon3, mon4, nm, xyz2, xyz1, xyz3, xyz4,
                grad2, grad1, grad3, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon3, mon2, mon1, mon4, nm, xyz3, xyz2, xyz1, xyz4,
                grad3, grad2, grad1, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon4) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon4, mon2, mon3, mon1, nm, xyz4, xyz2, xyz3, xyz1,
                grad4, grad2, grad3, grad1, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon1, mon3, mon2, mon4, nm, xyz1, xyz3, xyz2, xyz4,
                grad1, grad3, grad2, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon3 != mon4) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon1, mon2, mon4, mon3, nm, xyz1, xyz2, xyz4, xyz3,
                grad1, grad2, grad4, grad3, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }
    }

    SECTION("Unknow Tetramer") {
        std::vector<double> xyz1 = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> xyz2 = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> xyz3 = {0.0, 0.0, 0.0, 0.0};
        std::vector<double> xyz4 = {0.0, 0.0, 0.0, 0.0};
        size_t nm = 1;
        std::string mon1 = "notAmon";
        std::string mon2 = "neitherAmon";
        std::string mon3 = "AlsoNotMon";
        std::string mon4 = "StillNotMon";

        std::vector<double> grad1(xyz1.size(), 0.0);
        std::vector<double> grad2(xyz2.size(), 0.0);
        std::vector<double> grad3(xyz3.size(), 0.0);
        std::vector<double> grad4(xyz4.size(), 0.0);
        std::vector<double> virial(9, 0.0);

        double expected_energy = 0.0;

        std::vector<double> grad1_expected = {0.0, 0.0, 0.0};
        std::vector<double> grad2_expected = {0.0, 0.0, 0.0};
        std::vector<double> grad3_expected = {0.0, 0.0, 0.0};
        std::vector<double> grad4_expected = {0.0, 0.0, 0.0};
        std::vector<double> virial_expected(9, 0.0);

        SECTION("No gradients") {
            double e = e4b::get_4b_energy(mon1, mon2, mon3, mon4, nm, xyz1, xyz2, xyz3, xyz4);
            double e2 = e4b::get_4b_energy(mon2, mon1, mon4, mon3, nm, xyz2, xyz1, xyz4, xyz3);
            double e3 = e4b::get_4b_energy(mon1, mon4, mon3, mon2, nm, xyz1, xyz4, xyz3, xyz2);
            double e4 = e4b::get_4b_energy(mon4, mon3, mon2, mon1, nm, xyz4, xyz3, xyz2, xyz1);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
        }

        SECTION("With gradients") {
            double e = e4b::get_4b_energy(mon1, mon2, mon3, mon4, nm,
            xyz1, xyz2, xyz3, xyz4, grad1, grad2, grad3, grad4, &virial);
            REQUIRE(e == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
            REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

            if (mon1 != mon2) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon2, mon1, mon3, mon4, nm, xyz2, xyz1, xyz3, xyz4,
                grad2, grad1, grad3, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon3, mon2, mon1, mon4, nm, xyz3, xyz2, xyz1, xyz4,
                grad3, grad2, grad1, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon1 != mon4) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon4, mon2, mon3, mon1, nm, xyz4, xyz2, xyz3, xyz1,
                grad4, grad2, grad3, grad1, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon2 != mon3) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon1, mon3, mon2, mon4, nm, xyz1, xyz3, xyz2, xyz4,
                grad1, grad3, grad2, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }

            if (mon3 != mon4) {
                std::fill(grad1.begin(), grad1.end(), 0.0);
                std::fill(grad2.begin(), grad2.end(), 0.0);
                std::fill(grad3.begin(), grad3.end(), 0.0);
                std::fill(grad4.begin(), grad4.end(), 0.0);
                std::fill(virial.begin(), virial.end(), 0.0);

                double e2 = e4b::get_4b_energy(mon1, mon2, mon4, mon3, nm, xyz1, xyz2, xyz4, xyz3,
                grad1, grad2, grad4, grad3, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
            }
        }

    }
    //    SECTION("--") {
    //        std::vector<double> xyz1 = {};
    //        std::vector<double> xyz2 = {};
    //        std::vector<double> xyz3 = {};
    //        std::vector<double> xyz4 = {};
    //        size_t nm = ;
    //        std::string mon1 = "";
    //        std::string mon2 = "";
    //        std::string mon3 = "";
    //        std::string mon4 = "";
    //
    //        std::vector<double> grad1(xyz1.size(), 0.0);
    //        std::vector<double> grad2(xyz2.size(), 0.0);
    //        std::vector<double> grad3(xyz3.size(), 0.0);
    //        std::vector<double> grad4(xyz4.size(), 0.0);
    //        std::vector<double> virial(9, 0.0);
    //
    //        double expected_energy = ;
    //
    //        std::vector<double> grad1_expected = {};
    //        std::vector<double> grad2_expected = {};
    //        std::vector<double> grad3_expected = {};
    //        std::vector<double> grad4_expected = {};
    //        std::vector<double> virial_expected = {};
    //
    //        SECTION("No gradients") {
    //            double e = e4b::get_4b_energy(mon1, mon2, mon3, mon4, nm, xyz1, xyz2, xyz3, xyz4);
    //            double e2 = e4b::get_4b_energy(mon2, mon1, mon4, mon3, nm, xyz2, xyz1, xyz4, xyz3);
    //            double e3 = e4b::get_4b_energy(mon1, mon4, mon3, mon2, nm, xyz1, xyz4, xyz3, xyz2);
    //            double e4 = e4b::get_4b_energy(mon4, mon3, mon2, mon1, nm, xyz4, xyz3, xyz2, xyz1);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e3 == Approx(expected_energy).margin(TOL));
    //            REQUIRE(e4 == Approx(expected_energy).margin(TOL));
    //        }
    //
    //        SECTION("With gradients") {
    //            double e = e4b::get_4b_energy(mon1, mon2, mon3, mon4, nm,
    //            xyz1, xyz2, xyz3, xyz4, grad1, grad2, grad3, grad4, &virial);
    //            REQUIRE(e == Approx(expected_energy).margin(TOL));
    //            REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
    //            REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    //            REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
    //            REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
    //            REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    //
    //            if (mon1 != mon2) {
    //                std::fill(grad1.begin(), grad1.end(), 0.0);
    //                std::fill(grad2.begin(), grad2.end(), 0.0);
    //                std::fill(grad3.begin(), grad3.end(), 0.0);
    //                std::fill(grad4.begin(), grad4.end(), 0.0);
    //                std::fill(virial.begin(), virial.end(), 0.0);
    //
    //                double e2 = e4b::get_4b_energy(mon2, mon1, mon3, mon4, nm, xyz2, xyz1, xyz3, xyz4,
    //                grad2, grad1, grad3, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
    //                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    //            }
    //
    //            if (mon1 != mon3) {
    //                std::fill(grad1.begin(), grad1.end(), 0.0);
    //                std::fill(grad2.begin(), grad2.end(), 0.0);
    //                std::fill(grad3.begin(), grad3.end(), 0.0);
    //                std::fill(grad4.begin(), grad4.end(), 0.0);
    //                std::fill(virial.begin(), virial.end(), 0.0);
    //
    //                double e2 = e4b::get_4b_energy(mon3, mon2, mon1, mon4, nm, xyz3, xyz2, xyz1, xyz4,
    //                grad3, grad2, grad1, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
    //                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    //            }
    //
    //            if (mon1 != mon4) {
    //                std::fill(grad1.begin(), grad1.end(), 0.0);
    //                std::fill(grad2.begin(), grad2.end(), 0.0);
    //                std::fill(grad3.begin(), grad3.end(), 0.0);
    //                std::fill(grad4.begin(), grad4.end(), 0.0);
    //                std::fill(virial.begin(), virial.end(), 0.0);
    //
    //                double e2 = e4b::get_4b_energy(mon4, mon2, mon3, mon1, nm, xyz4, xyz2, xyz3, xyz1,
    //                grad4, grad2, grad3, grad1, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
    //                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    //            }
    //
    //            if (mon2 != mon3) {
    //                std::fill(grad1.begin(), grad1.end(), 0.0);
    //                std::fill(grad2.begin(), grad2.end(), 0.0);
    //                std::fill(grad3.begin(), grad3.end(), 0.0);
    //                std::fill(grad4.begin(), grad4.end(), 0.0);
    //                std::fill(virial.begin(), virial.end(), 0.0);
    //
    //                double e2 = e4b::get_4b_energy(mon1, mon3, mon2, mon4, nm, xyz1, xyz3, xyz2, xyz4,
    //                grad1, grad3, grad2, grad4, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
    //                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    //            }
    //
    //            if (mon3 != mon4) {
    //                std::fill(grad1.begin(), grad1.end(), 0.0);
    //                std::fill(grad2.begin(), grad2.end(), 0.0);
    //                std::fill(grad3.begin(), grad3.end(), 0.0);
    //                std::fill(grad4.begin(), grad4.end(), 0.0);
    //                std::fill(virial.begin(), virial.end(), 0.0);
    //
    //                double e2 = e4b::get_4b_energy(mon1, mon2, mon4, mon3, nm, xyz1, xyz2, xyz4, xyz3,
    //                grad1, grad2, grad4, grad3, &virial); REQUIRE(e2 == Approx(expected_energy).margin(TOL));
    //                REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad3, grad3_expected, TOL));
    //                REQUIRE(VectorsAreEqual(grad4, grad4_expected, TOL));
    //                REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    //            }
    //        }
    //    }
}
