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

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("ps::energy") {
    SECTION("Two water molecules") {
        std::vector<double> xyz = {-4.4590985000e-03, -5.1342579600e-02, 1.5813800000e-05,  9.8613021140e-01,
                                   -7.4573098400e-02, 5.4324000000e-06,  -1.5974709230e-01, 8.9671808950e-01,
                                   -1.6493200000e-05, -4.4590985000e-03, -5.1342579600e-02, 3.0000158138e+00,
                                   9.8613021140e-01,  -7.4573098400e-02, 3.0000054324e+00,  -1.5974709230e-01,
                                   8.9671808950e-01,  3.0000164932e+00};
        std::vector<double> grad = {
            0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
            0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
            0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
        std::vector<double> virial = {1.7472305348e+01, 1.7010689646e+01, 1.4982676725e+01,
                                      1.7010689646e+01, 2.0715840288e+01, 2.2655787267e+01,
                                      1.4982676725e+01, 2.2655787267e+01, 2.1016158132e+01};
        size_t nw = 2;

        std::vector<double> virial_expected = {-4.6972025339e+01, 7.8013643720e+00, 1.4983550088e+01,
                                               7.8013643720e+00,  1.8210088420e+01, 2.2655934045e+01,
                                               1.4983550088e+01,  2.2655934045e+01, 2.1016158119e+01};
        std::vector<double> grad_expected = {-3.9093808962e+01, -6.3152460748e+00, 6.6956246860e-04,  3.3418032085e+01,
                                             4.8742954215e+00,  -5.5352199777e-04, 5.6757768777e+00,  1.4409506533e+00,
                                             -1.1604047083e-04, -3.9093809063e+01, -6.3152454252e+00, 4.1696370030e-04,
                                             3.3418032111e+01,  4.8742953949e+00,  -3.5590136124e-04, 5.6757769526e+00,
                                             1.4409500303e+00,  -6.1062339060e-05};
        std::vector<double> energies_expected = {7.6144931941e-01, 7.6144931649e-01};

        std::vector<double> energies = ps::pot_nasa(xyz.data(), grad.data(), nw, &virial);

        REQUIRE(VectorsAreEqual(energies, energies_expected, TOL));
        REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
        REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
    }
}

TEST_CASE("ps::dms") {
    std::vector<double> xyz = {-4.4590985000e-03, -5.1342579600e-02, 1.5813800000e-05,
                               9.8613021140e-01,  -7.4573098400e-02, 5.4324000000e-06,
                               -1.5974709230e-01, 8.9671808950e-01,  -1.6493200000e-05};
    std::vector<double> chg = {0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
    std::vector<double> chg_der(27, 0.0);
    std::vector<double> virial(6, 0.0);

    std::vector<double> chg_expected = {-6.4456911666e-01, 3.2006450285e-01, 3.2450461382e-01};
    std::vector<double> chg_der_expected = {-1.9774807929e-01, -5.8226673322e-02, 4.3312083589e-06,  -5.2438371945e-02,
                                            -5.3746127141e-02, 2.5249227302e-06,  2.5018645124e-01,  1.1197280046e-01,
                                            -6.8561310891e-06, -5.5012500373e-02, -6.5151995707e-02, 2.9638990631e-06,
                                            -2.2842827899e-02, -2.1123412103e-01, 7.8486074784e-06,  7.7855328272e-02,
                                            2.7638611674e-01,  -1.0812506542e-05, 2.5276057967e-01,  1.2337866903e-01,
                                            -7.2951074220e-06, 7.5281199844e-02,  2.6498024817e-01,  -1.0373530209e-05,
                                            -3.2804177951e-01, -3.8835891720e-01, 1.7668637631e-05};
    std::vector<double> virial_expected = {-1.9632861438e-01, -5.5402918562e-02, -5.1163893112e-02,
                                           -2.0476395121e-01, -6.3363074138e-02, -5.5412228377e-02};

    ps::dms_nasa(0, 0, 0, xyz.data(), chg.data(), chg_der.data(), &virial);

    REQUIRE(VectorsAreEqual(chg, chg_expected, TOL));
    REQUIRE(VectorsAreEqual(chg_der, chg_der_expected, TOL));
    REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
}
