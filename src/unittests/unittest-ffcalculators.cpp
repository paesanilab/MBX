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

#include "potential/force_field/calculators.h"
#include "potential/force_field/dihedral.h"
#include "potential/force_field/inversion.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

constexpr double TOL = 1E-6;

TEST_CASE("calculators::CalculateDistance(p1,p2)") {
    SECTION("Normal behavior") {
        std::vector<double> p1 = {1.1, 3.2, -5.8};
        std::vector<double> p2 = {2.1, -9.2, 2.1};

        double expected_d = sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) + (p2[1] - p1[1]) * (p2[1] - p1[1]) +
                                 (p2[2] - p1[2]) * (p2[2] - p1[2]));

        double d = eff::CalculateDistance(p1, p2);

        REQUIRE(d == Approx(expected_d).margin(TOL));
    }

    SECTION("Exceptions") {
        std::vector<double> p1 = {1.1, 3.2, -5.8};
        std::vector<double> p2 = {2.1, -9.2};

        bool sizes_are_not_3 = false;
        try {
            double d = eff::CalculateDistance(p1, p2);
        } catch (...) {
            sizes_are_not_3 = true;
        }

        REQUIRE(sizes_are_not_3);
    }
}

TEST_CASE("calculators::CalculateDistance(p1)") {
    SECTION("Normal behavior") {
        std::vector<double> p = {2.1, -5.1, 8.2};

        double expected_d = sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);

        double d = eff::CalculateDistance(p);

        REQUIRE(d == Approx(expected_d).margin(TOL));
    }

    SECTION("Exceptions") {
        std::vector<double> p = {2.1, -5.1};
        bool sizes_are_not_3 = false;
        try {
            double d = eff::CalculateDistance(p);
        } catch (...) {
            sizes_are_not_3 = true;
        }

        REQUIRE(sizes_are_not_3);
    }
}

TEST_CASE("calculators::CalculateAngle") {
    SECTION("Normal behavior") {
        std::vector<double> p1 = {1.0, 0.0, 0.0};
        std::vector<double> p2 = {0.0, 0.0, 0.0};
        std::vector<double> p3 = {1.0, 1.0, 0.0};
        double expected_angle = pi / 4;

        double angle = eff::CalculateAngle(p1, p2, p3);

        REQUIRE(angle == Approx(expected_angle).margin(TOL));

        std::vector<double> p3b = {-1.0, -1.0, 0.0};
        expected_angle = 3 * pi / 4;

        angle = eff::CalculateAngle(p1, p2, p3b);

        REQUIRE(angle == Approx(expected_angle).margin(TOL));
    }

    SECTION("Extreme cases") {
        SECTION("180deg") {
            std::vector<double> p1 = {1.0, 0.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {-1.0, 0.0, 0.0};
            double expected_angle = pi;

            double angle = eff::CalculateAngle(p1, p2, p3);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }

        SECTION("90deg") {
            std::vector<double> p1 = {0.0, 1.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 0.0, 1.0};
            double expected_angle = pi / 2;

            double angle = eff::CalculateAngle(p1, p2, p3);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }

        SECTION("0deg") {
            std::vector<double> p1 = {0.0, 0.0, 1.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 0.0, 1.0};
            double expected_angle = 0.0;

            double angle = eff::CalculateAngle(p1, p2, p3);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }
    }

    SECTION("Assertions") {
        SECTION("Point duplicated") {
            std::vector<double> p1 = {0.0, 0.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {-1.0, 0.0, 0.0};

            bool isnan = false;
            try {
                double angle = eff::CalculateAngle(p1, p2, p3);
            } catch (...) {
                isnan = true;
            }

            REQUIRE(isnan);
        }
    }
}

TEST_CASE("calculators::CalculateDihedralAngle") {
    SECTION("Normal behavior") {
        SECTION("Negative dihedral") {
            std::vector<double> p1 = {1.0, 4.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 0.0, 1.0};
            std::vector<double> p4 = {0.0, -1.0, 1.0};

            double expected_angle = -2.89661399046293;

            double angle = eff::CalculateDihedralAngle(p1, p2, p3, p4);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));

            angle = eff::CalculateDihedralAngle(p4, p3, p2, p1);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }

        SECTION("Positive dihedral") {
            std::vector<double> p1 = {1.0, 0.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 1.0, 0.0};
            std::vector<double> p4 = {-1.0, 1.0, -0.1};

            double expected_angle = 3.0419240011;

            double angle = eff::CalculateDihedralAngle(p1, p2, p3, p4);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }
    }

    SECTION("Extreme cases") {
        SECTION("180Deg") {
            std::vector<double> p1 = {1.0, 0.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 1.0, 0.0};
            std::vector<double> p4 = {-1.0, 1.0, 0.0};

            double expected_angle = pi;

            double angle = eff::CalculateDihedralAngle(p1, p2, p3, p4);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }

        SECTION("180+Deg") {
            std::vector<double> p1 = {1.0, 0.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 1.0, 0.0};
            std::vector<double> p4 = {-1.0, 1.0, 1E-52};

            double expected_angle = -pi;

            double angle = eff::CalculateDihedralAngle(p1, p2, p3, p4);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }

        SECTION("180-Deg") {
            std::vector<double> p1 = {1.0, 0.0, 0.0};
            std::vector<double> p2 = {0.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 1.0, 0.0};
            std::vector<double> p4 = {-1.0, 1.0, -1E-52};

            double expected_angle = pi;

            double angle = eff::CalculateDihedralAngle(p1, p2, p3, p4);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }

        SECTION("Three points aligned") {
            std::vector<double> p1 = {2.0, 0.0, 0.0};
            std::vector<double> p2 = {1.0, 0.0, 0.0};
            std::vector<double> p3 = {0.0, 0.0, 0.0};
            std::vector<double> p4 = {0.0, 1.0, 0.0};

            double expected_angle = 0.0;

            double angle = eff::CalculateDihedralAngle(p1, p2, p3, p4);

            REQUIRE(angle == Approx(expected_angle).margin(TOL));
        }
    }
}

TEST_CASE("calculators::CalculateGradB") {
    SECTION("Normal Behavior") {
        std::vector<double> p1 = {1.0, 4.0, 0.1};
        std::vector<double> p2 = {0.2, 0.3, 0.4};
        std::vector<double> p3 = {0.11, 0.12, 1.12};
        std::vector<double> p4 = {0.35, -1.02, 1.1};

        // Expected outputs
        std::vector<std::vector<double> > expected_grads = {
            {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00},
            {1.1028771706e+00, -2.3198450830e-01, 7.9863519251e-02, -1.9527694561e+00, 8.4215747481e-01,
             -3.3556813311e-02, -2.5611007547e+00, -1.3176933569e+00, -6.4956093357e-01, 3.4109930402e+00,
             7.0752039043e-01, 6.0325422763e-01},
            {6.0893111905e-01, -1.2808551125e-01, 4.4095012069e-02, -1.0781817974e+00, 4.6498006053e-01,
             -1.8527709545e-02, -1.4140595074e+00, -7.2753749174e-01, -3.5864181136e-01, 1.8833101857e+00,
             3.9064294246e-01, 3.3307450883e-01},
            {-2.3026106166e-01, 4.8434223316e-02, -1.6674076879e-02, 4.0770339629e-01, -1.7582744426e-01,
             7.0060634720e-03, 5.3471210985e-01, 2.7511084589e-01, 1.3561672520e-01, -7.1215444448e-01,
             -1.4771762495e-01, -1.2594871180e-01},
            {8.0003762592e-01, -1.6828377649e-01, 5.7933759118e-02, -1.4165576016e+00, 6.1090907016e-01,
             -2.4342432657e-02, -1.8578469318e+00, -9.5586733779e-01, -4.7119770092e-01, 2.4743669074e+00,
             5.1324204412e-01, 4.3760637446e-01}};
        std::vector<std::vector<double> > expected_curr_force = {
            {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00},
            {-1.1028771706e+00, 2.3198450830e-01, -7.9863519251e-02, -1.0326355742e+00, -2.1419296844e-01,
             -1.8262768888e-01, -1.8825278597e+00, 3.9597999807e-01, -1.3632098294e-01, -3.4109930402e+00,
             -7.0752039043e-01, -6.0325422763e-01},
            {-6.0893111905e-01, 1.2808551125e-01, -4.4095012069e-02, -5.7014865527e-01, -1.1826227566e-01,
             -1.0083415082e-01, -1.0393993336e+00, 2.1863227363e-01, -7.5266848298e-02, -1.8833101857e+00,
             -3.9064294246e-01, -3.3307450883e-01},
            {2.3026106166e-01, -4.8434223316e-02, 1.6674076879e-02, 2.1559587047e-01, 4.4719667456e-02,
             3.8129400673e-02, 3.9303820509e-01, -8.2673553485e-02, 2.8461387265e-02, 7.1215444448e-01,
             1.4771762495e-01, 1.2594871180e-01},
            {-8.0003762592e-01, 1.6828377649e-01, -5.7933759118e-02, -7.4908370144e-01, -1.5537762366e-01,
             -1.3247986860e-01, -1.3656036771e+00, 2.8724767001e-01, -9.8888542134e-02, -2.4743669074e+00,
             -5.1324204412e-01, -4.3760637446e-01}};

        // Make a dihedral
        std::string topo = "diHedrAl";
        std::vector<size_t> idxs = {1, 2, 3, 4};
        std::vector<std::string> funcform = {"NONE", "cos", "haRm", "HCoS", "coS3"};
        std::vector<std::vector<double> > linear_params = {{}, {2.0}, {2.1}, {1.6}, {10.0, 1.0, 0.1}};
        std::vector<std::vector<double> > nonlinear_params = {{}, {3.0, pi / 6.0}, {2.5}, {5.1}, {}};

        std::vector<std::vector<double> > points = {p1, p2, p3, p4};

        for (size_t i = 0; i < funcform.size(); i++) {
            double phi = eff::CalculateDihedralAngle(p1, p2, p3, p4);
            double s = 0.0001;
            SECTION(funcform[i]) {
                eff::Dihedral dihedral(topo, idxs, funcform[i]);
                dihedral.SetParameters(linear_params[i], nonlinear_params[i]);

                double energy = dihedral.GetEnergy(phi);
                double cummu_grad = -1.0 / sin(phi) * dihedral.GetTopologyGradient(phi);
                std::vector<double> grads(12, 0.0);
                size_t mon_num = 0;
                size_t nat = 4;
                std::vector<double> curr_force(12, 0.0);
                eff::CalculateGradB(p1, p2, p3, p4, idxs, cummu_grad, phi, grads, mon_num, nat, curr_force);

                REQUIRE(VectorsAreEqual(grads, expected_grads[i], TOL));
                REQUIRE(VectorsAreEqual(curr_force, expected_curr_force[i], TOL));

                for (size_t j = 0; j < points.size(); j++) {
                    for (size_t k = 0; k < 3; k++) {
                        points[j][k] += s;
                        double phi_p = eff::CalculateDihedralAngle(points[0], points[1], points[2], points[3]);
                        double ep = dihedral.GetEnergy(phi_p);
                        points[j][k] += s;
                        double phi_pp = eff::CalculateDihedralAngle(points[0], points[1], points[2], points[3]);
                        double epp = dihedral.GetEnergy(phi_pp);
                        points[j][k] -= 4 * s;
                        double phi_mm = eff::CalculateDihedralAngle(points[0], points[1], points[2], points[3]);
                        double emm = dihedral.GetEnergy(phi_mm);
                        points[j][k] += s;
                        double phi_m = eff::CalculateDihedralAngle(points[0], points[1], points[2], points[3]);
                        double em = dihedral.GetEnergy(phi_m);
                        points[j][k] += s;

                        double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                        REQUIRE(numgrad == Approx(grads[3 * j + k]).margin(TOL));
                    }
                }
            }
        }
    }
}

TEST_CASE("calculators::CalculateCrossProduct") {
    std::vector<double> a = {1, 2, 3};
    std::vector<double> b = {1, 5, 7};
    std::vector<double> c = {-2, 4, -9};

    std::vector<double> axb_expected = {-1, -4, 3};
    std::vector<double> axc_expected = {-30, 3, 8};
    std::vector<double> bxc_expected = {-73, -5, 14};

    std::vector<double> axb = eff::CalculateCrossProduct(a, b);
    std::vector<double> axc = eff::CalculateCrossProduct(a, c);
    std::vector<double> bxc = eff::CalculateCrossProduct(b, c);

    REQUIRE(VectorsAreEqual(axb, axb_expected, TOL));
    REQUIRE(VectorsAreEqual(axc, axc_expected, TOL));
    REQUIRE(VectorsAreEqual(bxc, bxc_expected, TOL));
}

TEST_CASE("calculators::CalculateDotProduct") {
    std::vector<double> a = {1, 2, 3};
    std::vector<double> b = {1, 5, 7};
    std::vector<double> c = {-2, 4, -9};

    double axb_expected = 32;
    double axc_expected = -21;
    double bxc_expected = -45;

    double axb = eff::CalculateDotProduct(a, b);
    double axc = eff::CalculateDotProduct(a, c);
    double bxc = eff::CalculateDotProduct(b, c);

    REQUIRE(axb == Approx(axb_expected).margin(TOL));
    REQUIRE(axc == Approx(axc_expected).margin(TOL));
    REQUIRE(bxc == Approx(bxc_expected).margin(TOL));
}

TEST_CASE("calculators::CalculateInversionAngle") {
    SECTION("Regular thetrahedron") {
        std::vector<double> p1 = {0.0, 0.0, 1.0};
        std::vector<double> p2 = {sqrt(8.0 / 9.0), 0, -1.0 / 3.0};
        std::vector<double> p3 = {-sqrt(2.0 / 9.0), sqrt(2.0 / 3.0), -1.0 / 3.0};
        std::vector<double> p4 = {-sqrt(2.0 / 9.0), -sqrt(2.0 / 3.0), -1.0 / 3.0};

        std::vector<double> inv = eff::CalculateInversionAngle(p1, p2, p3, p4);
        double inv_expected = acos(1 / sqrt(3.0));

        for (size_t i = 0; i < 3; i++) {
            REQUIRE(inv[i] == Approx(inv_expected).margin(TOL));
        }
    }

    SECTION("Extreme cases: Coplanar") {
        std::vector<double> p1 = {0.0, 0.0, -1.0 / 3.0};
        std::vector<double> p2 = {sqrt(8.0 / 9.0), 0, -1.0 / 3.0};
        std::vector<double> p3 = {-sqrt(2.0 / 9.0), sqrt(2.0 / 3.0), -1.0 / 3.0};
        std::vector<double> p4 = {-sqrt(2.0 / 9.0), -sqrt(2.0 / 3.0), -1.0 / 3.0};

        std::vector<double> inv = eff::CalculateInversionAngle(p1, p2, p3, p4);
        double inv_expected = 0.0;

        for (size_t i = 0; i < 3; i++) {
            REQUIRE(inv[i] == Approx(inv_expected).margin(TOL));
        }
    }

    SECTION("Extreme cases: Far away") {
        std::vector<double> p1 = {0.0, 0.0, 1E+50};
        std::vector<double> p2 = {sqrt(8.0 / 9.0), 0, -1.0 / 3.0};
        std::vector<double> p3 = {-sqrt(2.0 / 9.0), sqrt(2.0 / 3.0), -1.0 / 3.0};
        std::vector<double> p4 = {-sqrt(2.0 / 9.0), -sqrt(2.0 / 3.0), -1.0 / 3.0};

        std::vector<double> inv = eff::CalculateInversionAngle(p1, p2, p3, p4);
        double inv_expected = 0.0;

        for (size_t i = 0; i < 3; i++) {
            REQUIRE(inv[i] == Approx(inv_expected).margin(TOL));
        }
    }
}

TEST_CASE("calculators::CalculateInversionGrad") {
    SECTION("Perfect thretrahedron") {
        std::vector<double> p1 = {0.0, 0.0, 1.0};
        std::vector<double> p2 = {sqrt(8.0 / 9.0), 0, -1.0 / 3.0};
        std::vector<double> p3 = {-sqrt(2.0 / 9.0), sqrt(2.0 / 3.0), -1.0 / 3.0};
        std::vector<double> p4 = {-sqrt(2.0 / 9.0), -sqrt(2.0 / 3.0), -1.0 / 3.0};

        // Expected outputs
        std::vector<std::vector<double> > expected_grads = {
            {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00},
            {-6.9388939039e-18, 6.9388939039e-18, 1.0828271806e-01, -5.1044962818e-02, 0.0000000000e+00,
             -3.6094239354e-02, 2.5522481409e-02, -4.4206234535e-02, -3.6094239354e-02, 2.5522481409e-02,
             4.4206234535e-02, -3.6094239354e-02}};
        std::vector<std::vector<double> > expected_curr_force = {
            {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
             0.0000000000e+00},
            {6.9388939039e-18, -6.9388939039e-18, -1.0828271806e-01, 5.1044962818e-02, -0.0000000000e+00,
             3.6094239354e-02, -2.5522481409e-02, 4.4206234535e-02, 3.6094239354e-02, -2.5522481409e-02,
             -4.4206234535e-02, 3.6094239354e-02}};

        // Make a inv
        std::string topo = "iNVersioN";
        std::vector<size_t> idxs = {1, 2, 3, 4};
        std::vector<std::string> funcform = {"NONE", "haRm"};
        std::vector<std::vector<double> > linear_params = {{}, {2.0}};
        std::vector<std::vector<double> > nonlinear_params = {{}, {pi / 3}};

        std::vector<std::vector<double> > points = {p1, p2, p3, p4};

        for (size_t i = 0; i < funcform.size(); i++) {
            std::vector<double> phi = eff::CalculateInversionAngle(p1, p2, p3, p4);

            ////////////////////////////////////
            // FIXME Eventually this needs to be reordered in the phi calculation to return ijkn, iknj, injk in this
            // order
            std::vector<double> temp_shift_vector = phi;

            // Shift the phis vector because the inversion angles are assigned
            // in the incorrect order from dlpoly
            for (int ii = 0; ii < 3; ii++) {
                phi[ii] = temp_shift_vector[(ii + 1) % 3];
            }
            ////////////////////////////////////

            double s = 0.0001;
            SECTION(funcform[i]) {
                eff::Inversion inv(topo, idxs, funcform[i]);
                inv.SetParameters(linear_params[i], nonlinear_params[i]);

                double energy = inv.GetEnergy(phi[0]) + inv.GetEnergy(phi[1]) + inv.GetEnergy(phi[2]);
                energy /= 3.0;
                std::vector<double> cummu_grad = {-1.0 / sin(phi[0]) / 3.0 * inv.GetTopologyGradient(phi[0]),
                                                  -1.0 / sin(phi[1]) / 3.0 * inv.GetTopologyGradient(phi[1]),
                                                  -1.0 / sin(phi[2]) / 3.0 * inv.GetTopologyGradient(phi[2])};
                std::vector<double> grads(12, 0.0);
                size_t mon_num = 0;
                size_t nat = 4;
                std::vector<double> curr_force(12, 0.0);
                eff::CalculateInversionGrad(p1, p2, p3, p4, idxs, cummu_grad, phi, grads, mon_num, nat, curr_force);

                REQUIRE(VectorsAreEqual(grads, expected_grads[i], TOL));
                REQUIRE(VectorsAreEqual(curr_force, expected_curr_force[i], TOL));

                for (size_t j = 0; j < points.size(); j++) {
                    for (size_t k = 0; k < 3; k++) {
                        points[j][k] += s;
                        std::vector<double> phi_p =
                            eff::CalculateInversionAngle(points[0], points[1], points[2], points[3]);
                        double ep = (inv.GetEnergy(phi_p[0]) + inv.GetEnergy(phi_p[1]) + inv.GetEnergy(phi_p[2])) / 3.0;

                        points[j][k] += s;
                        std::vector<double> phi_pp =
                            eff::CalculateInversionAngle(points[0], points[1], points[2], points[3]);
                        double epp =
                            (inv.GetEnergy(phi_pp[0]) + inv.GetEnergy(phi_pp[1]) + inv.GetEnergy(phi_pp[2])) / 3.0;

                        points[j][k] -= 4.0 * s;
                        std::vector<double> phi_mm =
                            eff::CalculateInversionAngle(points[0], points[1], points[2], points[3]);
                        double emm =
                            (inv.GetEnergy(phi_mm[0]) + inv.GetEnergy(phi_mm[1]) + inv.GetEnergy(phi_mm[2])) / 3.0;

                        points[j][k] += s;
                        std::vector<double> phi_m =
                            eff::CalculateInversionAngle(points[0], points[1], points[2], points[3]);
                        double em = (inv.GetEnergy(phi_m[0]) + inv.GetEnergy(phi_m[1]) + inv.GetEnergy(phi_m[2])) / 3.0;
                        points[j][k] += s;

                        double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                        REQUIRE(numgrad == Approx(grads[3 * j + k]).margin(TOL));
                    }
                }
            }
        }
    }
}
