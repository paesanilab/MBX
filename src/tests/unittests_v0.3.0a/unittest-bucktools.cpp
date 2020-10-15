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

    std::vector<double> box;
    std::vector<double> box_inv;
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
                    for (size_t k2 = k; k2 < 3; k2++) {
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
                    for (size_t k2 = k; k2 < 3; k2++) {
                        size_t shift_f = first_index_2b + j * 3 * nmon2b + k2 * nmon2b + i;
                        virial_manual[3 * k + k2] -= xyz2[shift_c] * numgrads2[shift_f];
                    }
                }
            }
        }

        virial_manual[3] = virial_manual[1];
        virial_manual[6] = virial_manual[2];
        virial_manual[7] = virial_manual[5];
        REQUIRE(VectorsAreEqual(virial, virial_manual, TOL));
    }
}
