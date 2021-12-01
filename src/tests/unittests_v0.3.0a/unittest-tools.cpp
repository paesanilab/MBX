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

#include "bblock/sys_tools.h"
#include "tools/math_tools.h"
#include "tools/properties.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"

#include "setup_monomer_mix.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("mathtools::InvertUnitCell") {
    SECTION("Matrices with inverse") {
        std::vector<double> mat1 = {1, 2, 3, 3, 2, 1, 2, 1, 3};
        std::vector<double> mat1_inv = {-5.0 / 12.0, 1.0 / 4.0,  1.0 / 3.0,  7.0 / 12.0, 1.0 / 4.0,
                                        -2.0 / 3.0,  1.0 / 12.0, -1.0 / 4.0, 1.0 / 3.0};

        std::vector<double> mat2 = {-5, 2, 3, -3, 2, 1, 2, 2, 3};
        std::vector<double> mat2_inv = {-1.0 / 7.0, 0.0,        1.0 / 7.0,  -11.0 / 28.0, 3.0 / 4.0,
                                        1.0 / 7.0,  5.0 / 14.0, -1.0 / 2.0, 1.0 / 7.0};

        REQUIRE(VectorsAreEqual(InvertUnitCell(mat1), mat1_inv, TOL));
        REQUIRE(VectorsAreEqual(InvertUnitCell(mat2), mat2_inv, TOL));
    }

    SECTION("Matrix too short") {
        std::vector<double> mat1 = {1, 2, 3, 3, 2, 3};
        std::vector<double> mat1_inv = {0.0, 0.0};

        mat1_inv = InvertUnitCell(mat1);
        REQUIRE(mat1_inv.size() == 0);
    }

    SECTION("Matrix has no inverse") {
        std::vector<double> mat1 = {1, 2, 3, 1, 2, 3, 1, 2, 3};
        std::vector<double> mat1_inv;
        bool inversionFailed = false;
        try {
            mat1_inv = InvertUnitCell(mat1);
        } catch (...) {
            inversionFailed = true;
        }
        REQUIRE(inversionFailed);
    }
}

TEST_CASE("mathtools::switch_function") {
    double rout = 10.0;
    double rin = 9.0;
    SECTION("Below cutoff") {
        double r1 = 4.0;
        double g1 = 1000.0;
        double r2 = rin;
        double g2 = 1000.0;

        double sw1 = switch_function(r1, rin, rout, g1);
        double sw2 = switch_function(r2, rin, rout, g2);

        REQUIRE(sw1 == Approx(1.0).margin(TOL));
        REQUIRE(sw2 == Approx(1.0).margin(TOL));
        REQUIRE(g1 == Approx(0.0).margin(TOL));
        REQUIRE(g2 == Approx(0.0).margin(TOL));
    }

    SECTION("Between rin and rout") {
        double r1 = 9.3283756238;
        double g1 = 1000.0;
        double r2 = 9.5;
        double g2 = 1000.0;
        double r3 = 9.898765432123;
        double g3 = 1000.0;

        double sw1 = switch_function(r1, rin, rout, g1);
        double sw2 = switch_function(r2, rin, rout, g2);
        double sw3 = switch_function(r3, rin, rout, g3);

        REQUIRE(sw1 == Approx(0.7567136227605).margin(TOL));
        REQUIRE(sw2 == Approx(0.5).margin(TOL));
        REQUIRE(sw3 == Approx(0.0250745796465674).margin(TOL));
        REQUIRE(g1 == Approx(-1.3479523637029).margin(TOL));
        REQUIRE(g2 == Approx(-1.570796326794896).margin(TOL));
        REQUIRE(g3 == Approx(-0.491193261769934).margin(TOL));
    }

    SECTION("Beyond cutoff") {
        double r1 = rout;
        double g1 = 1000.0;
        double r2 = 14.0;
        double g2 = 1000.0;

        double sw1 = switch_function(r1, rin, rout, g1);
        double sw2 = switch_function(r2, rin, rout, g2);

        REQUIRE(sw1 == Approx(0.0).margin(TOL));
        REQUIRE(sw2 == Approx(0.0).margin(TOL));
        REQUIRE(g1 == Approx(0.0).margin(TOL));
        REQUIRE(g2 == Approx(0.0).margin(TOL));
    }
}

TEST_CASE("mathtools::IsZero") {
    REQUIRE(IsZero(0.000000000000002));
    REQUIRE(IsZero(-0.000000000000002));
    REQUIRE(!IsZero(0.002));
    REQUIRE(!IsZero(-0.002));
}

TEST_CASE("mathtools::BoxVecToBoxABCabc") {
    SECTION("Expected behavior") {
        SECTION("Cubic box") {
            std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
            std::vector<double> box_abc_expected = {10.0, 10.0, 10.0, 90.0, 90.0, 90.0};
            std::vector<double> box_abc = BoxVecToBoxABCabc(box);

            REQUIRE(VectorsAreEqual(box_abc, box_abc_expected, TOL));
        }

        SECTION("Orthonormal box") {
            std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 30.0};
            std::vector<double> box_abc_expected = {10.0, 20.0, 30.0, 90.0, 90.0, 90.0};
            std::vector<double> box_abc = BoxVecToBoxABCabc(box);

            REQUIRE(VectorsAreEqual(box_abc, box_abc_expected, TOL));
        }

        SECTION("Hexagonal box") {
            std::vector<double> box = {10.0, 0.0, 0.0, 5.0, 8.660254037844386, 0.0, 0.0, 0.0, 20.0};
            std::vector<double> box_abc_expected = {10.0, 10.0, 20.0, 90.0, 90.0, 60.0};
            std::vector<double> box_abc = BoxVecToBoxABCabc(box);

            REQUIRE(VectorsAreEqual(box_abc, box_abc_expected, TOL));
        }

        SECTION("Triclinic box") {
            std::vector<double> box = {10.0,
                                       0.0,
                                       0.0,
                                       21.6506350946109661,
                                       12.5,
                                       0.0,
                                       5.2094453300079104,
                                       20.9769760091738822,
                                       20.80452250988236834};
            std::vector<double> box_abc_expected = {10.0, 25.0, 30.0, 60.0, 80.0, 30.0};
            std::vector<double> box_abc = BoxVecToBoxABCabc(box);

            REQUIRE(VectorsAreEqual(box_abc, box_abc_expected, TOL));
        }
    }

    SECTION("Exceptions") {
        std::vector<double> box1 = {10.0, 1.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        bool v1_is_not_x_oriented = false;
        try {
            std::vector<double> abc = BoxVecToBoxABCabc(box1);
        } catch (...) {
            v1_is_not_x_oriented = true;
        }

        REQUIRE(v1_is_not_x_oriented);

        std::vector<double> box2 = {0.0, 10.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        bool v1_x_is_zero = false;
        try {
            std::vector<double> abc = BoxVecToBoxABCabc(box2);
        } catch (...) {
            v1_x_is_zero = true;
        }

        REQUIRE(v1_x_is_zero);

        std::vector<double> box3 = {10.0, 0.0, 10.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
        v1_is_not_x_oriented = false;
        try {
            std::vector<double> abc = BoxVecToBoxABCabc(box3);
        } catch (...) {
            v1_is_not_x_oriented = true;
        }

        REQUIRE(v1_is_not_x_oriented);

        std::vector<double> box4 = {10.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0};
        bool v2_y_component_is_zero = false;
        try {
            std::vector<double> abc = BoxVecToBoxABCabc(box4);
        } catch (...) {
            v2_y_component_is_zero = true;
        }

        REQUIRE(v2_y_component_is_zero);

        std::vector<double> box5 = {10.0, 0.0, 0.0, 10.0, 10.0, 10.0, 0.0, 0.0, 10.0};
        bool v2_is_not_in_XY_plane = false;
        try {
            std::vector<double> abc = BoxVecToBoxABCabc(box5);
        } catch (...) {
            v2_is_not_in_XY_plane = true;
        }

        REQUIRE(v2_is_not_in_XY_plane);
    }
}

TEST_CASE("mathtools::BoxABCabcToBoxVec") {
    SECTION("Expected behavior") {
        SECTION("Cubic box") {
            std::vector<double> box_expected = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
            std::vector<double> box_abc = {10.0, 10.0, 10.0, 90.0, 90.0, 90.0};
            std::vector<double> box = BoxABCabcToBoxVec(box_abc);

            REQUIRE(VectorsAreEqual(box, box_expected, TOL));
        }

        SECTION("Orthonormal box") {
            std::vector<double> box_expected = {10.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 30.0};
            std::vector<double> box_abc = {10.0, 20.0, 30.0, 90.0, 90.0, 90.0};
            std::vector<double> box = BoxABCabcToBoxVec(box_abc);

            REQUIRE(VectorsAreEqual(box, box_expected, TOL));
        }

        SECTION("Hexagonal box") {
            std::vector<double> box_expected = {10.0, 0.0, 0.0, 5.0, 8.660254037844386, 0.0, 0.0, 0.0, 20.0};
            std::vector<double> box_abc = {10.0, 10.0, 20.0, 90.0, 90.0, 60.0};
            std::vector<double> box = BoxABCabcToBoxVec(box_abc);

            REQUIRE(VectorsAreEqual(box, box_expected, TOL));
        }

        SECTION("Triclinic box") {
            std::vector<double> box_expected = {10.0,
                                                0.0,
                                                0.0,
                                                21.6506350946109661,
                                                12.5,
                                                0.0,
                                                5.2094453300079104,
                                                20.9769760091738822,
                                                20.80452250988236834};
            std::vector<double> box_abc = {10.0, 25.0, 30.0, 60.0, 80.0, 30.0};
            std::vector<double> box = BoxABCabcToBoxVec(box_abc);

            REQUIRE(VectorsAreEqual(box, box_expected, TOL));
        }
    }
}

TEST_CASE("mathtools::MatrixTimesVector") {
    SECTION("Expected behavior - double") {
        std::vector<double> A = {1.1, 2.0, 3.0, 4.0, 5.0, 6.0};
        std::vector<double> b = {1.0, 0.0, 1.0};
        std::vector<double> c_expected = {4.1, 10.0};

        std::vector<double> c(2);
        MatrixTimesVector(A, b, c);

        REQUIRE(VectorsAreEqual(c, c_expected, TOL));
    }

    SECTION("Expected behavior - int") {
        std::vector<int> A = {1, 2, 3, -4, 5, 6};
        std::vector<int> b = {1, 0, 1};
        std::vector<int> c_expected = {4, 2};

        std::vector<int> c(2);
        MatrixTimesVector(A, b, c);

        REQUIRE(VectorsAreEqual(c, c_expected));
    }

    SECTION("Exceptions") {
        std::vector<int> A = {2, 3, -4, 5, 6};
        std::vector<int> b = {1, 0, 1};

        std::vector<int> c(10);
        bool size_does_not_match = false;
        try {
            MatrixTimesVector(A, b, c);
        } catch (...) {
            size_does_not_match = true;
        }
        REQUIRE(size_does_not_match);
    }
}

TEST_CASE("mathtools::DotProduct") {
    SECTION("Expected behavior - double") {
        std::vector<double> a = {1.0, 2.1, 3.0, -4.0};
        std::vector<double> b = {1.0, 1.0, 2.0, -1.0};
        double dot_expected = 13.1;

        double dot = DotProduct(a, b);
        REQUIRE(dot == Approx(dot_expected).margin(TOL));
    }

    SECTION("Expected behavior - int") {
        std::vector<int> a = {1, 2, 3, -4};
        std::vector<int> b = {1, 1, 2, 1};
        int dot_expected = 5;

        int dot = DotProduct(a, b);
        REQUIRE(dot == dot_expected);
    }

    SECTION("Exceptions") {
        std::vector<double> a = {1.0, 2.1, 3.0, -4.0};
        std::vector<double> b = {1.0, 1.0, 2.0, -1.0, 1.0};

        bool size_does_not_match = false;
        try {
            double dot = DotProduct(a, b);
        } catch (...) {
            size_does_not_match = true;
        }

        REQUIRE(size_does_not_match);
    }
}

TEST_CASE("properties::GetMass") {
    SETUP_MONMIX
    // Add atoms not in monomer mix
    atom_names.push_back("D");
    atom_masses.push_back(2.01410178);
    atom_names.push_back("N");
    atom_masses.push_back(14.0030740044);
    for (size_t i = 0; i < atom_masses.size(); i++) {
        std::string at = atom_names[i];
        SECTION(at) {
            double expected_mass = atom_masses[i];
            REQUIRE(tools::GetMass(at) == Approx(expected_mass).margin(TOL));
        }
    }

    SECTION("Exceptions") {
        std::string atom_name = "does_not_exist";
        bool atom_does_not_exist = false;
        try {
            double mass = tools::GetMass(atom_name);
        } catch (...) {
            atom_does_not_exist = true;
        }
        REQUIRE(atom_does_not_exist);
    }
}

TEST_CASE("properties::Distance") {
    SECTION("Gas phase") {
        std::vector<double> p1 = {0.0, 0.0, 0.0};
        std::vector<double> p2 = {5.0, 5.0, 5.0};
        double d = sqrt(75.0);
        REQUIRE(tools::Distance(p1.data(), p2.data()) == Approx(d).margin(TOL));
    }

    SECTION("PBC") {
        SECTION("Cubic box") {
            std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0};
            std::vector<double> box_inv = InvertUnitCell(box);
            std::vector<double> p1(3, 0.0);
            std::vector<double> p2(3, 0.0);
            double d_expected = 0.0;
            double separation = 0.95;
            double diff = separation / 2.0;
            double dist = 1.0 - separation;
            std::vector<double> vec_dist(3, 0.0);

            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    p1[j] -= diff * box[3 * i + j];
                    p2[j] += diff * box[3 * i + j];
                    vec_dist[j] += dist * box[3 * i + j];
                }
            }
            for (size_t i = 0; i < 3; i++) {
                d_expected += vec_dist[i] * vec_dist[i];
            }
            d_expected = sqrt(d_expected);

            REQUIRE(tools::Distance(p1.data(), p2.data(), box, box_inv) == Approx(d_expected).margin(TOL));
        }

        SECTION("Orthonormal box") {
            std::vector<double> box = {10.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 30.0};
            std::vector<double> box_inv = InvertUnitCell(box);
            std::vector<double> p1(3, 0.0);
            std::vector<double> p2(3, 0.0);
            double d_expected = 0.0;
            double separation = 0.95;
            double diff = separation / 2.0;
            double dist = 1.0 - separation;
            std::vector<double> vec_dist(3, 0.0);

            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    p1[j] -= diff * box[3 * i + j];
                    p2[j] += diff * box[3 * i + j];
                    vec_dist[j] += dist * box[3 * i + j];
                }
            }
            for (size_t i = 0; i < 3; i++) {
                d_expected += vec_dist[i] * vec_dist[i];
            }
            d_expected = sqrt(d_expected);

            REQUIRE(tools::Distance(p1.data(), p2.data(), box, box_inv) == Approx(d_expected).margin(TOL));
        }

        SECTION("Hexagonal box") {
            std::vector<double> box = {10.0, 0.0, 0.0, 5.0, 8.660254037844386, 0.0, 0.0, 0.0, 20.0};
            std::vector<double> box_inv = InvertUnitCell(box);
            std::vector<double> p1(3, 0.0);
            std::vector<double> p2(3, 0.0);
            double d_expected = 0.0;
            double separation = 0.95;
            double diff = separation / 2.0;
            double dist = 1.0 - separation;
            std::vector<double> vec_dist(3, 0.0);

            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    p1[j] -= diff * box[3 * i + j];
                    p2[j] += diff * box[3 * i + j];
                    vec_dist[j] += dist * box[3 * i + j];
                }
            }
            for (size_t i = 0; i < 3; i++) {
                d_expected += vec_dist[i] * vec_dist[i];
            }
            d_expected = sqrt(d_expected);

            REQUIRE(tools::Distance(p1.data(), p2.data(), box, box_inv) == Approx(d_expected).margin(TOL));
        }

        SECTION("Triclinic box") {
            std::vector<double> box = {10.0,
                                       0.0,
                                       0.0,
                                       21.6506350946109661,
                                       12.5,
                                       0.0,
                                       5.2094453300079104,
                                       20.9769760091738822,
                                       20.80452250988236834};
            std::vector<double> box_inv = InvertUnitCell(box);
            std::vector<double> p1(3, 0.0);
            std::vector<double> p2(3, 0.0);
            double d_expected = 0.0;
            double separation = 0.95;
            double diff = separation / 2.0;
            double dist = 1.0 - separation;
            std::vector<double> vec_dist(3, 0.0);

            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    p1[j] -= diff * box[3 * i + j];
                    p2[j] += diff * box[3 * i + j];
                    vec_dist[j] += dist * box[3 * i + j];
                }
            }
            for (size_t i = 0; i < 3; i++) {
                d_expected += vec_dist[i] * vec_dist[i];
            }
            d_expected = sqrt(d_expected);

            REQUIRE(tools::Distance(p1.data(), p2.data(), box, box_inv) == Approx(d_expected).margin(TOL));
        }
    }
}

TEST_CASE("variable::variable") {
    std::vector<double> p1 = {2.0, 0.5, 1.5};
    std::vector<double> p2 = {0.1, 0.4, 0.3};
    double r =
        sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) + (p2[1] - p1[1]) * (p2[1] - p1[1]) + (p2[2] - p1[2]) * (p2[2] - p1[2]));
    double r0 = 0.9;
    double k = 1.02;

    double step = 1E-04;

    SECTION("exp") {
        double expected_value = std::exp(-k * r);

        variable v;
        double value = v.v_exp(k, p1.data(), p2.data());
        REQUIRE(value == Approx(expected_value).margin(TOL));

        std::vector<double> grad1(3, 0.0);
        std::vector<double> grad2(3, 0.0);

        double grad = 4.0 * value;  // Function is 2*xi^2; dfdxi = 4*xi
        v.grads(grad, grad1.data(), grad2.data(), p1.data(), p2.data());

        std::vector<double> grad1_expected(3, 0.0);
        std::vector<double> grad2_expected(3, 0.0);

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p1[i] += step;
            double vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] -= 4 * step;
            vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;

            grad1_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p2[i] += step;
            double vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] -= 4 * step;
            vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_exp(k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;

            grad2_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    }

    SECTION("exp0") {
        double expected_value = std::exp(-k * (r - r0));

        variable v;
        double value = v.v_exp0(r0, k, p1.data(), p2.data());
        REQUIRE(value == Approx(expected_value).margin(TOL));

        std::vector<double> grad1(3, 0.0);
        std::vector<double> grad2(3, 0.0);

        double grad = 4.0 * value;  // Function is 2*xi^2; dfdxi = 4*xi
        v.grads(grad, grad1.data(), grad2.data(), p1.data(), p2.data());

        std::vector<double> grad1_expected(3, 0.0);
        std::vector<double> grad2_expected(3, 0.0);

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p1[i] += step;
            double vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] -= 4 * step;
            vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;

            grad1_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p2[i] += step;
            double vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] -= 4 * step;
            vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_exp0(r0, k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;

            grad2_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    }

    SECTION("coul") {
        double expected_value = std::exp(-k * r) / r;

        variable v;
        double value = v.v_coul(k, p1.data(), p2.data());
        REQUIRE(value == Approx(expected_value).margin(TOL));

        std::vector<double> grad1(3, 0.0);
        std::vector<double> grad2(3, 0.0);

        double grad = 4.0 * value;  // Function is 2*xi^2; dfdxi = 4*xi
        v.grads(grad, grad1.data(), grad2.data(), p1.data(), p2.data());

        std::vector<double> grad1_expected(3, 0.0);
        std::vector<double> grad2_expected(3, 0.0);

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p1[i] += step;
            double vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] -= 4 * step;
            vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;

            grad1_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p2[i] += step;
            double vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] -= 4 * step;
            vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_coul(k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;

            grad2_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    }

    SECTION("coul0") {
        double expected_value = std::exp(-k * (r - r0)) / r;

        variable v;
        double value = v.v_coul0(r0, k, p1.data(), p2.data());
        REQUIRE(value == Approx(expected_value).margin(TOL));

        std::vector<double> grad1(3, 0.0);
        std::vector<double> grad2(3, 0.0);

        double grad = 4.0 * value;  // Function is 2*xi^2; dfdxi = 4*xi
        v.grads(grad, grad1.data(), grad2.data(), p1.data(), p2.data());

        std::vector<double> grad1_expected(3, 0.0);
        std::vector<double> grad2_expected(3, 0.0);

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p1[i] += step;
            double vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] -= 4 * step;
            vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;

            grad1_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p2[i] += step;
            double vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] -= 4 * step;
            vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_coul0(r0, k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;

            grad2_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    }

    SECTION("gau") {
        double expected_value = std::exp(-k * r * r);

        variable v;
        double value = v.v_gau(k, p1.data(), p2.data());
        REQUIRE(value == Approx(expected_value).margin(TOL));

        std::vector<double> grad1(3, 0.0);
        std::vector<double> grad2(3, 0.0);

        double grad = 4.0 * value;  // Function is 2*xi^2; dfdxi = 4*xi
        v.grads(grad, grad1.data(), grad2.data(), p1.data(), p2.data());

        std::vector<double> grad1_expected(3, 0.0);
        std::vector<double> grad2_expected(3, 0.0);

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p1[i] += step;
            double vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] -= 4 * step;
            vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;

            grad1_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p2[i] += step;
            double vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] -= 4 * step;
            vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_gau(k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;

            grad2_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    }

    SECTION("gau0") {
        double expected_value = std::exp(-k * (r - r0) * (r - r0));

        variable v;
        double value = v.v_gau0(r0, k, p1.data(), p2.data());
        REQUIRE(value == Approx(expected_value).margin(TOL));

        std::vector<double> grad1(3, 0.0);
        std::vector<double> grad2(3, 0.0);

        double grad = 4.0 * value;  // Function is 2*xi^2; dfdxi = 4*xi
        v.grads(grad, grad1.data(), grad2.data(), p1.data(), p2.data());

        std::vector<double> grad1_expected(3, 0.0);
        std::vector<double> grad2_expected(3, 0.0);

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p1[i] += step;
            double vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] -= 4 * step;
            vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;
            vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p1[i] += step;

            grad1_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        for (size_t i = 0; i < 3; i++) {
            variable vv;
            p2[i] += step;
            double vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fpp = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] -= 4 * step;
            vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fmm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;
            vv_val = vv.v_gau0(r0, k, p1.data(), p2.data());
            double fm = 2.0 * vv_val * vv_val;  // Our function is 2*xi^2
            p2[i] += step;

            grad2_expected[i] = (-fpp + 8 * fp - 8 * fm + fmm) / (12 * step);
        }

        REQUIRE(VectorsAreEqual(grad1, grad1_expected, TOL));
        REQUIRE(VectorsAreEqual(grad2, grad2_expected, TOL));
    }
}

TEST_CASE("water_lone_pairs") {
    std::vector<double> ohh = {-4.45909850e-03, -5.13425796e-02, 1.58138000e-05, 9.86130211e-01, -7.45730984e-02,
                               5.43240000e-06,  -1.59747092e-01, 8.96718089e-01, -1.64932000e-05};
    double wpar = -9.72148691e-02;
    double wper = 9.85927208e-02;
    std::vector<double> lp(6, 0.0);
    std::vector<double> lp_expected = {-4.50599082e-02, -9.62928864e-02, 9.22544693e-02,
                                       -4.50619969e-02, -9.62995148e-02, -9.22186917e-02};

    monomer m;
    m.setup(ohh.data(), wpar, wper, lp.data(), lp.data() + 3);

    REQUIRE(VectorsAreEqual(lp, lp_expected, TOL));

    std::vector<double> lp_grads = {-3.20986004e-01, -8.83967986e-03, 1.32951414e-01,
                                    -2.40589346e-01, -4.20190011e-03, 1.08928886e-01};
    std::vector<double> wat_grads = {7.34398467e-01, 2.98780376e-01, -2.53156873e-01, -2.05608860e-01, 7.66832320e-03,
                                     5.76607508e-03, 2.88598962e-02, -2.93103039e-01, 8.28008339e-03};
    std::vector<double> water_grads_expected = {1.15929202e-01,  2.81756838e-01,  4.01487144e-03,
                                                -1.76066707e-01, 8.67028831e-03,  1.59472667e-03,
                                                5.62116582e-02,  -2.90123046e-01, -2.84001193e-03};

    m.grads(lp_grads.data(), lp_grads.data() + 3, wpar, wper, wat_grads.data());

    REQUIRE(VectorsAreEqual(wat_grads, water_grads_expected, TOL));
}
