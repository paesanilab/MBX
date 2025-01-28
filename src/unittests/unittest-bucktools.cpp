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
#include "potential/buckingham/bucktools.h"
#include "setup_dimer_custom_json.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("bucktools::Repulsion") {
    // The reference point. Always at origin.
    std::vector<double> p1 = {0.0, 0.0, 0.0};
    std::vector<double> g1 = {0.0, 0.0, 0.0};
    // The set of "monomers of type 2"
    // 10 monomers; first 4 of 4 atoms each, second 6 of 2 atoms each.
    // Need to put xxxyyyzzz for first type and then same for second
    std::vector<double> xyz2;
    size_t nmon2a = 4;
    size_t nmon2b = 6;
    size_t nat2a = 3;
    size_t nat2b = 2;

    size_t first_index_2a = 0;
    size_t first_index_2b = nmon2a * nat2a * 3;

    double cutoff = 9.0;

    std::vector<double> box = {100.0, 0.0, 0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 100.0};
    std::vector<double> box_inv = {1 / 100.0, 0.0, 0.0, 0.0, 1 / 100.0, 0.0, 0.0, 0.0, 1 / 100.0};

    std::vector<double> virial(9, 0.0);

    for (size_t i = 0; i < nat2a; i++) {
        // x coordinate for atom i is i+1
        for (size_t k = 0; k < nmon2a; k++) {
            xyz2.push_back(double(i) + 1.0);
        }
        // y and z coordinates are 0
        for (size_t k = 0; k < nmon2a * 2; k++) {
            xyz2.push_back(0.0);
        }
    }

    for (size_t i = 0; i < nat2b; i++) {
        // x coordinate for atom i 0.0
        for (size_t k = 0; k < nmon2b; k++) {
            xyz2.push_back(0.0);
        }
        // y coordinate for atom i is i + 1
        for (size_t k = 0; k < nmon2b; k++) {
            xyz2.push_back(double(i) + 1.0);
        }
        // z coordinate is 0.0
        for (size_t k = 0; k < nmon2b; k++) {
            xyz2.push_back(0.0);
        }
    }

    std::vector<double> grad2(xyz2.size(), 0.0);
    std::vector<double> numgrads2(grad2.size(), 0.0);
    std::vector<size_t> islocal(xyz2.size() / 3, 1);

    // Buckingham is going to be Aexp(-br) = A [exp(-b)] ^r
    // For b = 0.693147180559945309417; exp(-b) = 0.5
    double a = 10.0;
    double b = 0.693147180559945309417;
    double expb = std::exp(-b);

    REQUIRE(expb == Approx(0.5).margin(TOL * TOL));

    double erep = 0.0;
    double erep_g = 0.0;

    SECTION("Repulsion") {
        size_t start2 = 0;
        size_t end2 = nmon2a;
        size_t i = 0;
        for (size_t j = 0; j < nat2a; j++) {
            double erep_expected = a * std::pow(expb, j + 1.0) * nmon2a;
            erep = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2a, g1.data(),
                                   grad2.data() + first_index_2a, 1, nmon2a, start2, end2, i, j, false, cutoff, box,
                                   box_inv, false, islocal, 0, 0, &virial);
            erep_g = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2a, g1.data(),
                                     grad2.data() + first_index_2a, 1, nmon2a, start2, end2, i, j, true, cutoff, box,
                                     box_inv, false, islocal, 0, 0, &virial);
            REQUIRE(erep == Approx(erep_expected).margin(TOL));
            REQUIRE(erep_g == Approx(erep_expected).margin(TOL));
        }

        // Calculate numerical gradients for mon 2a
        double step = 0.001;
        for (size_t j = 0; j < nat2a; j++) {
            for (size_t k = 0; k < 3; k++) {
                size_t shift = first_index_2a + j * nmon2a * 3 + k * nmon2a;
                for (size_t i = 0; i < nmon2a; i++) {
                    xyz2[shift + i] += step;
                    double erep_p = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2a, g1.data(),
                                                    grad2.data() + first_index_2a, 1, nmon2a, start2, end2, 0, j, false,
                                                    cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] += step;
                    double erep_pp = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2a, g1.data(),
                                                     grad2.data() + first_index_2a, 1, nmon2a, start2, end2, 0, j,
                                                     false, cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] -= 4 * step;
                    double erep_mm = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2a, g1.data(),
                                                     grad2.data() + first_index_2a, 1, nmon2a, start2, end2, 0, j,
                                                     false, cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] += step;
                    double erep_m = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2a, g1.data(),
                                                    grad2.data() + first_index_2a, 1, nmon2a, start2, end2, 0, j, false,
                                                    cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] += step;
                    double numdiff = (-erep_pp + 8 * erep_p - 8 * erep_m + erep_mm) / 12 / step;
                    numgrads2[shift + i] += numdiff;
                }
            }
        }
        REQUIRE(VectorsAreEqual(numgrads2, grad2, TOL));

        end2 = nmon2b;
        i = 0;
        for (size_t j = 0; j < nat2b; j++) {
            double erep_expected = a * std::pow(expb, j + 1.0) * nmon2b;
            erep = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2b, g1.data(),
                                   grad2.data() + first_index_2b, 1, nmon2b, start2, end2, i, j, false, cutoff, box,
                                   box_inv, false, islocal, 0, 0, &virial);
            erep_g = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2b, g1.data(),
                                     grad2.data() + first_index_2b, 1, nmon2b, start2, end2, i, j, true, cutoff, box,
                                     box_inv, false, islocal, 0, 0, &virial);
            REQUIRE(erep == Approx(erep_expected).margin(TOL));
            REQUIRE(erep_g == Approx(erep_expected).margin(TOL));
        }

        // Calculate numerical gradients for mon 2a
        for (size_t j = 0; j < nat2b; j++) {
            for (size_t k = 0; k < 3; k++) {
                size_t shift = first_index_2b + j * nmon2b * 3 + k * nmon2b;
                for (size_t i = 0; i < nmon2b; i++) {
                    xyz2[shift + i] += step;
                    double erep_p = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2b, g1.data(),
                                                    grad2.data() + first_index_2b, 1, nmon2b, start2, end2, 0, j, false,
                                                    cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] += step;
                    double erep_pp = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2b, g1.data(),
                                                     grad2.data() + first_index_2b, 1, nmon2b, start2, end2, 0, j,
                                                     false, cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] -= 4 * step;
                    double erep_mm = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2b, g1.data(),
                                                     grad2.data() + first_index_2b, 1, nmon2b, start2, end2, 0, j,
                                                     false, cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] += step;
                    double erep_m = buck::Repulsion(a, b, p1.data(), xyz2.data() + first_index_2b, g1.data(),
                                                    grad2.data() + first_index_2b, 1, nmon2b, start2, end2, 0, j, false,
                                                    cutoff, box, box_inv, false, islocal, 0, 0, &virial);
                    xyz2[shift + i] += step;
                    double numdiff = (-erep_pp + 8 * erep_p - 8 * erep_m + erep_mm) / 12 / step;
                    numgrads2[shift + i] += numdiff;
                }
            }
        }

        REQUIRE(VectorsAreEqual(numgrads2, grad2, TOL));

        std::vector<double> virial_manual(9, 0.0);
        for (size_t i = 0; i < nmon2a; i++) {
            for (size_t j = 0; j < nat2a; j++) {
                for (size_t k = 0; k < 3; k++) {
                    size_t shift_c = first_index_2a + j * 3 * nmon2a + k * nmon2a + i;
                    for (size_t k2 = 0; k2 < 3; k2++) {
                        size_t shift_f = first_index_2a + j * 3 * nmon2a + k2 * nmon2a + i;
                        virial_manual[3 * k + k2] -= xyz2[shift_c] * numgrads2[shift_f];
                    }
                }
            }
        }

        for (size_t i = 0; i < nmon2b; i++) {
            for (size_t j = 0; j < nat2b; j++) {
                for (size_t k = 0; k < 3; k++) {
                    size_t shift_c = first_index_2b + j * 3 * nmon2b + k * nmon2b + i;
                    for (size_t k2 = 0; k2 < 3; k2++) {
                        size_t shift_f = first_index_2b + j * 3 * nmon2b + k2 * nmon2b + i;
                        virial_manual[3 * k + k2] -= xyz2[shift_c] * numgrads2[shift_f];
                    }
                }
            }
        }

        REQUIRE(VectorsAreEqual(virial, virial_manual, TOL));
    }
}

TEST_CASE("GetBuckParams") {
    std::vector<std::pair<std::string, std::string> > pairs = {{"f-", "h2o"},   {"cl-", "h2o"},
                                                               {"br-", "h2o"},  {"h2o", "i-"},
                                                               {"h2o", "li+"},  {"h2o", "na+"},
                                                               {"h2o", "k+"},   {"h2o", "rb+"},
                                                               {"cs+", "h2o"},  {"co2_archive", "h2o"},
                                                               {"ch4", "h2o"},  {"co2_archive", "co2_archive"},
                                                               {"ch4", "ch4"},  {"nh3pbe0d3bj", "nh3pbe0d3bj"},
                                                               {"nh3", "nh3"},  {"n2o5", "n2o5"},
                                                               {"h2o", "n2o5"}, {"h2", "h2"},
                                                               {"h2", "h2o"},   {"ar", "h2o"},
                                                               {"ar", "ar"},    {"cs+", "h2"},
                                                               {"na+", "na+"},  {"cl-", "cl-"},
                                                               {"cl-", "na+"},  {"ar", "cs+"},
                                                               {"f-pol100","h2o"}, {"f-pol75","h2o"},
                                                               {"f-pol50","h2o"}, {"f-pol25","h2o"},
                                                               {"f-pol0","h2o"}, {"f-poleff","h2o"},
                                                               {"cl-pol100","h2o"}, {"cl-pol75","h2o"},
                                                               {"cl-pol50","h2o"}, {"cl-pol25","h2o"},
                                                               {"cl-pol0","h2o"}, {"cl-poleff","h2o"},
                                                               {"br-pol100","h2o"}, {"br-pol75","h2o"},
                                                               {"br-pol50","h2o"}, {"br-pol25","h2o"},
                                                               {"br-pol0","h2o"}, {"br-poleff","h2o"},
                                                               {"h2o","i-pol100"}, {"h2o","i-pol75"},
                                                               {"h2o","i-pol50"}, {"h2o","i-pol25"},
                                                               {"h2o","i-pol0"}, {"h2o","i-poleff"},
                                                               };

    std::vector<std::pair<std::string, std::string> > buck_pairs = {{"f-", "h2o"},   {"cl-", "h2o"},
                                                                    {"br-", "h2o"},  {"i-", "h2o"},
                                                                    {"li+", "h2o"},  {"na+", "h2o"},
                                                                    {"k+", "h2o"},   {"rb+", "h2o"},
                                                                    {"cs+", "h2o"},  {"co2_archive", "h2o"},
                                                                    {"ch4", "h2o"},  {"co2_archive", "co2_archive"},
                                                                    {"ch4", "ch4"},  {"nh3pbe0d3bj", "nh3pbe0d3bj"},
                                                                    {"nh3", "nh3"},  {"n2o5", "n2o5"},
                                                                    {"h2o", "n2o5"}, {"h2", "h2"},
                                                                    {"h2", "h2o"},   {"ar", "h2o"},
                                                                    {"ar", "ar"},    {"cs+", "h2"},
                                                                    {"na+", "na+"},  {"cl-", "cl-"},
                                                                    {"cl-", "na+"},  {"ar", "cs+"},
                                                                    {"f-pol100","h2o"}, {"f-pol75","h2o"},
                                                                    {"f-pol50","h2o"}, {"f-pol25","h2o"},
                                                                    {"f-pol0","h2o"}, {"f-poleff","h2o"},
                                                                    {"cl-pol100","h2o"}, {"cl-pol75","h2o"},
                                                                    {"cl-pol50","h2o"}, {"cl-pol25","h2o"},
                                                                    {"cl-pol0","h2o"}, {"cl-poleff","h2o"},
                                                                    {"br-pol100","h2o"}, {"br-pol75","h2o"},
                                                                    {"br-pol50","h2o"}, {"br-pol25","h2o"},
                                                                    {"br-pol0","h2o"}, {"br-poleff","h2o"},
                                                                    {"h2o","i-pol100"}, {"h2o","i-pol75"},
                                                                    {"h2o","i-pol50"}, {"h2o","i-pol25"},
                                                                    {"h2o","i-pol0"}, {"h2o","i-poleff"},
                                                                    };

    std::vector<std::vector<double> > a_expected = {
        {35729.5, 799.875},                    // {"f-","h2o"}
        {50260.9, 2614.24},                    // {"cl-","h2o"}
        {37785.6, 3871.75},                    // {"br-","h2o"}
        {22240.7, 6221.63},                    // {"i-","h2o"}
        {32318.0, 3245.78},                    // {"li+","h2o"}
        {47827.7, 4992.61},                    // {"na+","h2o"}
        {49986.5, 4951.5},                     // {"k+","h2o"}
        {48456.3, 6794.51},                    // {"rb+","h2o"}
        {42431.0, 9403.73},                    // {"cs+","h2o"}
        {4735.44, 4956.27, 30678.4, 4559.97},  // {"co2_archive","h2o"}
        {11447.5, 4887.62, 6182.32, 1480.08},  // {"ch4","h2o"}
        {9038.48, 12608.9, 12608.9, 24274.7},  // {"co2_archive","co2_archive"}
        {42713.9, 3258.86, 3258.86, 2594.4},   // {"ch4","ch4"}
        {19267.3, 3232.98, 3232.98, 980.337},  // {"nh3pbe0d3bj","nh3pbe0d3bj"}
        {20413.5, 3259.49, 3259.49, 965.289},  // {"nh3","nh3"}
        {52582.5, 7849.47, 34641.1, 7849.47, 2692.58, 49876.0, 34641.1, 49876.0, 31773.9},  // {"n2o5","n2o5"}
        {93032.9, 12989.0, 153274.0, 2460.89, 514436.0, 1251.73},                           // {"h2o","n2o5"}
        {822.645},                            // {"h2","h2"}
        {2651.63, 839.321},                   // {"h2","h2o"}
        {55341.7, 5733.11},                   // {"ar","h2o"}
        {102057.0},                           // {"ar","ar"}
        {18116.1},                            // {"cs+","h2"}
        {33569.6},                            // {"na+","na+"}
        {2943.76},                            // {"cl-","cl-"}
        {33676.8},                            // {"cl-","na+"}
        {157654.0},                           // {"ar","cs+"}
        {35729.5, 799.875},                   // {"f-pol100","h2o"}
        {36197.6, 969.805},                   // {"f-pol75","h2o"}
        {38259.9, 1228.77},                   // {"f-pol50","h2o"}
        {41505.6, 1755.5},                    // {"f-pol25","h2o"}
        {21889.0, 8309.33},                   // {"f-pol0","h2o"}
        {40637.5, 1535.02},                   // {"f-poleff","h2o"}
        {50260.9, 2614.24},                   // {"cl-pol100","h2o"}
        {49544.7, 2980.48},                   // {"cl-pol75","h2o"}
        {49718.1, 3450.58},                   // {"cl-pol50","h2o"}
        {49141.1, 4162.42},                   // {"cl-pol25","h2o"}
        {43019.6, 5723.07},                   // {"cl-pol00","h2o"}
        {49726.1, 3460.17},                   // {"cl-poleff","h2o"}
        {37785.6, 3871.75},                   // {"br-pol100","h2o"}
        {37353.2, 4272.53},                   // {"br-pol75","h2o"}
        {37603.8, 4774.11},                   // {"br-pol50","h2o"}
        {37688.3, 5531.86},                   // {"br-pol25","h2o"}
        {33398.0, 7209.34},                   // {"br-pol0","h2o"}
        {37554.3, 4710.39},                   // {"br-poleff","h2o"}
        {22240.7, 6221.63},                   // {"i-pol100","h2o"}
        {21551.7, 6710.99},                   // {"i-pol75","h2o"}
        {21406.6, 7261.62},                   // {"i-pol50","h2o"}
        {21614.6, 7998.31},                   // {"i-pol25","h2o"}
        {19851.4, 9601.84},                   // {"i-pol0","h2o"}
        {21398.0, 7054.43},                   // {"i-poleff","h2o"}
        };

    std::vector<std::vector<double> > b_expected = {
        {3.57999, 2.70536},                    // {"f-","h2o"}
        {3.27293, 2.79252},                    // {"cl-","h2o"}
        {3.05463, 2.81519},                    // {"br-","h2o"}
        {2.72259, 2.80139},                    // {"i-","h2o"}
        {4.02333, 4.00663},                    // {"li+","h2o"}
        {3.76953, 3.82255},                    // {"na+","h2o"}
        {3.40125, 3.32139},                    // {"k+","h2o"}
        {3.23653, 3.31364},                    // {"rb+","h2o"}
        {3.02864, 3.27153},                    // {"cs+","h2o"}
        {2.93819, 3.7359, 3.53045, 3.89503},   // {"co2_archive","h2o"}
        {2.87176, 3.68542, 3.79757, 4.01558},  // {"ch4","h2o"}
        {3.12663, 3.64236, 3.64236, 3.52744},  // {"co2_archive","co2_archive"}
        {3.37925, 3.25885, 3.25885, 4.05972},  // {"ch4","ch4"}
        {3.09382, 3.44698, 3.44698, 3.83901},  // {"nh3pbe0d3bj","nh3pbe0d3bj"}
        {3.11493, 3.4174, 3.4174, 3.78007},    // {"nh3","nh3"}
        {3.86891, 3.19945, 3.60907, 3.19945, 2.33813, 4.19428, 3.60907, 4.19428, 3.56601},  // {"n2o5","n2o5"}
        {4.1869, 3.22662, 4.455, 3.24055, 6.4749, 3.03227},                                 // {"h2o","n2o5"}
        {3.11276},                            // {"h2","h2"}
        {3.05339, 3.62823},                   // {"h2","h2o"}
        {3.48054, 3.46238},                   // {"ar","h2o"}
        {3.41808},                            // {"ar","ar"}
        {3.24781},                            // {"cs+","h2"}
        {4.42822},                            // {"na+","na+"}
        {1.82786},                            // {"cl-","cl-"}
        {2.85113},                            // {"cl-","na+"}
        {3.28039},                            // {"ar","cs+"}
        {3.57999, 2.70536},                   // {"f-pol100","h2o"}
        {3.58864, 2.79262},                   // {"f-pol75","h2o"}
        {3.62164, 2.89671},                   // {"f-pol50","h2o"}
        {3.67265, 3.06551},                   // {"f-pol25","h2o"}
        {3.28017, 4.19907},                   // {"f-pol0","h2o"}
        {3.65841, 2.99892},                   // {"f-poleff","h2o"}
        {3.27293, 2.79252},                   // {"cl-pol100","h2o"}
        {3.27071, 2.8562},                    // {"cl-pol75","h2o"}
        {3.27741, 2.92533},                   // {"cl-pol50","h2o"}
        {3.27832, 3.01688},                   // {"cl-pol25","h2o"}
        {3.2164, 3.20482},                    // {"cl-pol0","h2o"}
        {3.27758, 2.92664},                   // {"cl-poleff","h2o"}
        {3.05463, 2.81519},                   // {"br-pol100","h2o"}
        {3.05395, 2.86428},                   // {"br-pol75","h2o"}
        {3.06123, 2.91776},                   // {"br-pol50","h2o"}
        {3.06682, 2.98962},                   // {"br-pol25","h2o"}
        {3.01321, 3.14144},                   // {"br-pol0","h2o"}
        {3.06017, 2.91132},                   // {"br-poleff","h2o"}
        {2.72259, 2.80139},                   // {"i-pol100","h2o"}
        {2.71173, 2.84378},                   // {"i-pol75","h2o"}
        {2.71169, 2.88502},                   // {"i-pol500","h2o"}
        {2.71978, 2.9324},                    // {"i-pol25","h2o"}
        {2.68628, 3.0327},                    // {"i-pol0","h2o"}
        {2.71045, 2.87024},                   // {"i-poleff","h2o"}
        };

    std::vector<std::vector<size_t> > types1 = {{0},                    // {"f-","h2o"}
                                                {0},                    // {"cl-","h2o"}
                                                {0},                    // {"br-","h2o"}
                                                {0, 1, 1},              // {"h2o","i-"}
                                                {0, 1, 1},              // {"h2o","li+"}
                                                {0, 1, 1},              // {"h2o","na+"}
                                                {0, 1, 1},              // {"h2o","k+"}
                                                {0, 1, 1},              // {"h2o","rb+"}
                                                {0},                    // {"cs+","h2o"}
                                                {0, 1, 1},              // {"co2_archive","h2o"}
                                                {0, 1, 1, 1, 1},        // {"ch4","h2o"}
                                                {0, 1, 1},              // {"co2_archive","co2_archive"}
                                                {0, 1, 1, 1, 1},        // {"ch4","ch4"}
                                                {0, 1, 1, 1},           // {"nh3pbe0d3bj","nh3pbe0d3bj"}
                                                {0, 1, 1, 1},           // {"nh3","nh3"}
                                                {0, 1, 1, 2, 2, 2, 2},  // {"n2o5","n2o5"}
                                                {0, 1, 1},              // {"h2o","n2o5"}
                                                {0, 0},                 // {"h2","h2"}
                                                {0, 0},                 // {"h2","h2o"}
                                                {0},                    // {"ar","h2o"}
                                                {0},                    // {"ar","ar"}
                                                {0},                    // {"cs+","h2"}
                                                {0},                    // {"na+","na+"}
                                                {0},                    // {"cl-","cl-"}
                                                {0},                    // {"cl-","na+"}
                                                {0},                    // {"ar","cs+"}
                                                {0},                    // {"f-pol100","h2o"}
                                                {0},                    // {"f-pol75","h2o"}
                                                {0},                    // {"f-pol50","h2o"}
                                                {0},                    // {"f-pol25","h2o"}
                                                {0},                    // {"f-pol0","h2o"}
                                                {0},                    // {"f-poleff","h2o"}
                                                {0},                    // {"cl-pol100","h2o"}
                                                {0},                    // {"cl-pol75","h2o"}
                                                {0},                    // {"cl-pol50","h2o"}
                                                {0},                    // {"cl-pol25","h2o"}
                                                {0},                    // {"cl-pol0","h2o"}
                                                {0},                    // {"cl-poleff","h2o"}
                                                {0},                    // {"br-pol100","h2o"}
                                                {0},                    // {"br-pol75","h2o"}
                                                {0},                    // {"br-pol50","h2o"}
                                                {0},                    // {"br-pol25","h2o"}
                                                {0},                    // {"br-pol0","h2o"}
                                                {0},                    // {"br-poleff","h2o"}
                                                {0, 1, 1},            // {"h2o","i-pol100"}
                                                {0, 1, 1},               // {"h2o","i-pol75"}
                                                {0, 1, 1},               // {"h2o","i-pol50"}
                                                {0, 1, 1},               // {"h2o","i-pol25"}
                                                {0, 1, 1},               // {"h2o","i-pol0"}
                                                {0, 1, 1},               // {"h2o","i-poleff"}
                                                    };

    std::vector<std::vector<size_t> > types2 = {{0, 1, 1},              // {"f-","h2o"}
                                                {0, 1, 1},              // {"cl-","h2o"}
                                                {0, 1, 1},              // {"br-","h2o"}
                                                {0},                    // {"h2o","i-"}
                                                {0},                    // {"h2o","li+"}
                                                {0},                    // {"h2o","na+"}
                                                {0},                    // {"h2o","k+"}
                                                {0},                    // {"h2o","rb+"}
                                                {0, 1, 1},              // {"cs+","h2o"}
                                                {0, 1, 1},              // {"co2_archive","h2o"}
                                                {0, 1, 1},              // {"ch4","h2o"}
                                                {0, 1, 1},              // {"co2_archive","co2_archive"}
                                                {0, 1, 1, 1, 1},        // {"ch4","ch4"}
                                                {0, 1, 1, 1},           // {"nh3pbe0d3bj","nh3pbe0d3bj"}
                                                {0, 1, 1, 1},           // {"nh3","nh3"}
                                                {0, 1, 1, 2, 2, 2, 2},  // {"n2o5","n2o5"}
                                                {0, 1, 1, 2, 2, 2, 2},  // {"h2o","n2o5"}
                                                {0, 0},                 // {"h2","h2"}
                                                {0, 1, 1},              // {"h2","h2o"}
                                                {0, 1, 1},              // {"ar","h2o"}
                                                {0},                    // {"ar","ar"}
                                                {0, 0},                 // {"cs+","h2"}
                                                {0},                    // {"na+","na+"}
                                                {0},                    // {"cl-","cl-"}
                                                {0},                    // {"cl-","na+"}
                                                {0},                    // {"ar","cs+"}
                                                {0, 1, 1},              // {"f-pol100","h2o"}
                                                {0, 1, 1},              // {"f-pol75","h2o"}
                                                {0, 1, 1},              // {"f-pol50","h2o"}
                                                {0, 1, 1},              // {"f-pol25","h2o"}
                                                {0, 1, 1},              // {"f-pol0","h2o"}
                                                {0, 1, 1},              // {"f-poleff","h2o"}
                                                {0, 1, 1},              // {"cl-pol100","h2o"}
                                                {0, 1, 1},              // {"cl-pol75","h2o"}
                                                {0, 1, 1},              // {"cl-pol50","h2o"}
                                                {0, 1, 1},              // {"cl-pol25","h2o"}
                                                {0, 1, 1},              // {"cl-pol0","h2o"}
                                                {0, 1, 1},              // {"cl-poleff","h2o"}
                                                {0, 1, 1},              // {"br-pol100","h2o"}
                                                {0, 1, 1},              // {"br-pol75","h2o"}
                                                {0, 1, 1},              // {"br-pol50","h2o"}
                                                {0, 1, 1},              // {"br-pol25","h2o"}
                                                {0, 1, 1},              // {"br-pol0","h2o"}
                                                {0, 1, 1},              // {"br-poleff","h2o"}
                                                {0},                    // {"h2o","i-pol100"}
                                                {0},                    // {"h2o","i-pol75"}
                                                {0},                    // {"h2o","i-pol50"}
                                                {0},                    // {"h2o","i-pol25"}
                                                {0},                    // {"h2o","i-pol0"}
                                                {0},                    // {"h2o","i-poleff"}
    };

    // number of different types for each monomer in types2
    std::vector<size_t> ntypes2 = {2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1};

    for (size_t i = 0; i < pairs.size(); i++) {
        SECTION("Pair " + pairs[i].first + "," + pairs[i].second) {
            std::string m1 = pairs[i].first;
            std::string m2 = pairs[i].second;
            for (size_t at1 = 0; at1 < types1[i].size(); at1++) {
                for (size_t at2 = 0; at2 < types2[i].size(); at2++) {
                    double a = -1.0;
                    double b = -1.0;
                    double a_swapped = -1.0;
                    double b_swapped = -1.0;
                    bool found = buck::GetBuckParams(m1, m2, at1, at2, buck_pairs, a, b);
                    bool found_swapped = buck::GetBuckParams(m2, m1, at2, at1, buck_pairs, a_swapped, b_swapped);
                    size_t ii = types1[i][at1];
                    size_t jj = types2[i][at2];
                    size_t nt2 = ntypes2[i]; // number of different types in types2
                    double a_exp = a_expected[i][ii * nt2 + jj];
                    double b_exp = b_expected[i][ii * nt2 + jj];
                    REQUIRE(a == Approx(a_exp).margin(TOL));
                    REQUIRE(b == Approx(b_exp).margin(TOL));
                    REQUIRE(a_swapped == Approx(a_exp).margin(TOL));
                    REQUIRE(b_swapped == Approx(b_exp).margin(TOL));
                    REQUIRE(found);
                    REQUIRE(found_swapped);
                }
            }
        }
    }

    SECTION("User-defined json parameters") {
        SETUP_DIM_JSON
        std::vector<std::pair<std::string, std::string> > jpairs = {{"mymon1", "mymon2"}, {"mymon1", "mymon3"}};

        std::vector<std::pair<std::string, std::string> > jbuck_pairs = {{"mymon1", "mymon2"}, {"mymon1", "mymon3"}};

        std::vector<std::vector<double> > ja_expected = {{1000.0, 2000.0, 3000.0, 4000.0},
                                                         {5000.0, 6000.0, 7000.0, 8000.0}};
        std::vector<std::vector<double> > jb_expected = {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}};
        std::vector<size_t> jntypes2 = {2, 2};
        std::vector<std::vector<size_t> > jtypes1 = {{0, 1}, {0, 1}};
        std::vector<std::vector<size_t> > jtypes2 = {{0, 1}, {0, 1}};

        for (size_t i = 0; i < jpairs.size(); i++) {
            SECTION("Pair " + jpairs[i].first + "," + jpairs[i].second) {
                std::string m1 = jpairs[i].first;
                std::string m2 = jpairs[i].second;
                for (size_t at1 = 0; at1 < jtypes1[i].size(); at1++) {
                    for (size_t at2 = 0; at2 < jtypes2[i].size(); at2++) {
                        double a = -1.0;
                        double b = -1.0;
                        double a_swapped = -1.0;
                        double b_swapped = -1.0;
                        bool found = buck::GetBuckParams(m1, m2, at1, at2, jbuck_pairs, a, b, user_j);
                        bool found_swapped =
                            buck::GetBuckParams(m2, m1, at2, at1, jbuck_pairs, a_swapped, b_swapped, user_j);
                        size_t ii = jtypes1[i][at1];
                        size_t jj = jtypes2[i][at2];
                        size_t nt2 = jntypes2[i];
                        double a_exp = ja_expected[i][ii * nt2 + jj];
                        double b_exp = jb_expected[i][ii * nt2 + jj];
                        REQUIRE(a == Approx(a_exp).margin(TOL));
                        REQUIRE(b == Approx(b_exp).margin(TOL));
                        REQUIRE(a_swapped == Approx(a_exp).margin(TOL));
                        REQUIRE(b_swapped == Approx(b_exp).margin(TOL));
                        REQUIRE(found);
                        REQUIRE(found_swapped);
                    }
                }
            }
        }
    }

    SECTION("Pair not found") {
        std::string m1 = "h2o";
        std::string m2 = "f-";
        std::string m3 = "none";

        std::vector<std::pair<std::string, std::string> > buck_pairs_empty;

        double a = -1.0;
        double b = -1.0;

        bool found = buck::GetBuckParams(m1, m2, 0, 0, buck_pairs_empty, a, b);
        REQUIRE(a == Approx(0.0).margin(TOL));
        REQUIRE(b == Approx(0.0).margin(TOL));
        REQUIRE(found == false);

        found = buck::GetBuckParams(m1, m3, 0, 0, buck_pairs, a, b);
        REQUIRE(a == Approx(0.0).margin(TOL));
        REQUIRE(b == Approx(0.0).margin(TOL));
        REQUIRE(found == false);
    }
}
