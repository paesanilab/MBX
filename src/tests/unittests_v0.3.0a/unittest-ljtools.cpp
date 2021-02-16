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

#include "potential/lj/ljtools.h"
#include "tools/math_tools.h"
#include "json/json.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("ljtools::lj") {
    SECTION("Normal Behavior") {
        SECTION("Gas Phase") {
            double eps = 0.120984;
            double sigma = 3.325000;
            double ljchgi = 0.0;
            double ljchgj = 0.0;
            std::vector<double> p1 = {4.5278490461619709E+00, 6.1779461427431679E+00, 3.9068784243576893E+00};
            std::vector<double> xyz2 = {5.1292456840248581E+00, 7.3641793429207905E+00, 3.8284728828715551E+00};

            std::vector<double> grad1(3, 0.0);

            std::vector<double> grad2(3, 0.0);
            double phi1 = 0.0;
            std::vector<double> phi2 = {0.0};
            size_t nmon1 = 1;
            size_t nmon2 = 1;
            size_t start2 = 0;
            size_t end2 = 1;
            size_t atom_index1 = 0;
            size_t atom_index2 = 0;
            double lj_scale_factor = 1.0000000000e+00;
            bool do_grads = true;
            double cutoff = 5.0000000000e+01;
            double ewald_alpha = 0.0000000000e+00;
            bool use_ghost = false;
            std::vector<double> box = {};
            std::vector<double> box_inverse = {};
            std::vector<size_t> islocal = {1, 1};
            std::vector<double> virial(9, 0.0);
            size_t isl1_offset = 0;
            size_t isl2_offset = 1;

            std::vector<double> expected_grad1 = {1.1465301877e+05, 2.2614894863e+05, -1.4947592742e+04};
            std::vector<double> expected_grad2 = {-1.1465301877e+05, -2.2614894863e+05, 1.4947592742e+04};
            std::vector<double> expected_phi2 = {0.0};
            double expected_phi1 = 0.0;
            std::vector<double> expected_virial = {6.8951940007e+04,  1.3600521736e+05,  -8.9894320194e+03,
                                                   1.3600521736e+05,  2.6826539105e+05,  -1.7731330774e+04,
                                                   -8.9894320194e+03, -1.7731330774e+04, 1.1719741029e+03};
            double expected_energy = 2.8140638986e+04;

            std::vector<double> expected_grad1_ndiff(grad1.size());
            for (size_t i = 0; i < grad1.size(); i++) expected_grad1_ndiff[i] = expected_grad1[i] - grad1[i];

            std::vector<double> expected_grad2_ndiff(grad2.size());
            for (size_t i = 0; i < grad2.size(); i++) expected_grad2_ndiff[i] = expected_grad2[i] - grad2[i];

            double energy = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);

            REQUIRE(energy == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, expected_grad1, TOL));
            REQUIRE(VectorsAreEqual(grad2, expected_grad2, TOL));
            REQUIRE(phi1 == Approx(expected_phi1).margin(TOL));
            REQUIRE(VectorsAreEqual(phi2, expected_phi2, TOL));
            REQUIRE(VectorsAreEqual(virial, expected_virial, TOL));

            //// Check numerical gradients
            do_grads = false;
            double s = 0.0001;
            for (size_t i = 0; i < p1.size(); i++) {
                p1[i] += s;
                double ep = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double epp =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] -= 4 * s;
                double emm =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double em = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad1_ndiff[i]).margin(TOL));
            }

            for (size_t i = 0; i < xyz2.size(); i++) {
                xyz2[i] += s;
                double ep = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double epp =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] -= 4 * s;
                double emm =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double em = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad2_ndiff[i]).margin(TOL));
            }
        }
        SECTION("PBC") {
            double eps = 0.120984;
            double sigma = 3.325000;
            double ljchgi = 2 * std::sqrt(0.170100) * 3.250000 * 3.250000 * 3.250000;
            double ljchgj = 2 * std::sqrt(0.086050) * 3.400000 * 3.400000 * 3.400000;
            std::vector<double> p1 = {4.5278490461619709E+00, 6.1779461427431679E+00, 3.9068784243576893E+00};
            std::vector<double> xyz2 = {5.1292456840248581E+00, 7.3641793429207905E+00, 3.8284728828715551E+00};

            std::vector<double> grad1(3, 0.0);

            std::vector<double> grad2(3, 0.0);
            double phi1 = 0.0;
            std::vector<double> phi2 = {0.0};
            size_t nmon1 = 1;
            size_t nmon2 = 1;
            size_t start2 = 0;
            size_t end2 = 1;
            size_t atom_index1 = 0;
            size_t atom_index2 = 0;
            double lj_scale_factor = 1.0000000000e+00;
            bool do_grads = true;
            double cutoff = 9.0;
            double ewald_alpha = 0.6;
            bool use_ghost = false;
            std::vector<double> box = {20.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 20.0};
            std::vector<double> box_inverse = {0.05, 0.0, 0.0, 0.0, 0.05, 0.0, 0.0, 0.0, 0.05};
            std::vector<size_t> islocal = {1, 1};
            std::vector<double> virial(9, 0.0);
            size_t isl1_offset = 0;
            size_t isl2_offset = 1;

            std::vector<double> expected_grad1 = {1.1465401332e+05, 2.2615091035e+05, -1.4947722405e+04};
            std::vector<double> expected_grad2 = {-1.1465401332e+05, -2.2615091035e+05, 1.4947722405e+04};
            std::vector<double> expected_phi2 = {-5.0635586424e+00};
            double expected_phi1 = -4.1234980425e+00;
            std::vector<double> expected_virial = {6.8952538129e+04,  1.3600639714e+05,  -8.9895099981e+03,
                                                   1.3600639714e+05,  2.6826771811e+05,  -1.7731484584e+04,
                                                   -8.9895099981e+03, -1.7731484584e+04, 1.1719842691e+03};
            double expected_energy = 2.8143808123e+04;

            std::vector<double> expected_grad1_ndiff(grad1.size());
            for (size_t i = 0; i < grad1.size(); i++) expected_grad1_ndiff[i] = expected_grad1[i] - grad1[i];

            std::vector<double> expected_grad2_ndiff(grad2.size());
            for (size_t i = 0; i < grad2.size(); i++) expected_grad2_ndiff[i] = expected_grad2[i] - grad2[i];

            double energy = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);

            REQUIRE(energy == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, expected_grad1, TOL));
            REQUIRE(VectorsAreEqual(grad2, expected_grad2, TOL));
            REQUIRE(phi1 == Approx(expected_phi1).margin(TOL));
            REQUIRE(VectorsAreEqual(phi2, expected_phi2, TOL));
            REQUIRE(VectorsAreEqual(virial, expected_virial, TOL));

            //// Check numerical gradients
            do_grads = false;
            double s = 0.0001;
            for (size_t i = 0; i < p1.size(); i++) {
                p1[i] += s;
                double ep = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double epp =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] -= 4 * s;
                double emm =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double em = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad1_ndiff[i]).margin(TOL));
            }

            for (size_t i = 0; i < xyz2.size(); i++) {
                xyz2[i] += s;
                double ep = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double epp =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] -= 4 * s;
                double emm =
                    lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                           atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box, box_inverse,
                           use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double em = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                   end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                   box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad2_ndiff[i]).margin(TOL));
            }
        }
    }

    SECTION("Scan") {
        double eps = 0.120984;
        double sigma = 3.325000;
        double ljchgi = 2 * std::sqrt(0.170100) * 3.250000 * 3.250000 * 3.250000;
        double ljchgj = 2 * std::sqrt(0.086050) * 3.400000 * 3.400000 * 3.400000;
        std::vector<double> p1 = {0.001, 0.001, 0.001};
        std::vector<double> xyz2(60, 0.0);

        size_t nmon1 = 1;
        size_t nmon2 = xyz2.size() / 3;
        ;

        // fill in xyz2
        for (size_t i = 0; i < xyz2.size() / 3; i++) {
            size_t idx = (i + 1) % 3;
            xyz2[i + nmon2 * idx] = double(i) / 2.0 + 2.3;
        }

        std::vector<double> grad1(3, 0.0);

        std::vector<double> grad2(xyz2.size(), 0.0);
        double phi1 = 0.0;
        std::vector<double> phi2(xyz2.size() / 3, 0.0);
        size_t start2 = 0;
        size_t end2 = nmon2;
        size_t atom_index1 = 0;
        size_t atom_index2 = 0;
        double lj_scale_factor = 1.0000000000e+00;
        bool do_grads = true;
        double cutoff = 8.0;
        double ewald_alpha = 0.6;
        bool use_ghost = false;
        std::vector<double> box = {20.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 20.0};
        std::vector<double> box_inverse = {0.05, 0.0, 0.0, 0.0, 0.05, 0.0, 0.0, 0.0, 0.05};
        std::vector<size_t> islocal(nmon1 + nmon2, 1);
        std::vector<double> virial(9, 0.0);
        size_t isl1_offset = 0;
        size_t isl2_offset = 1;

        double energy = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                               end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                               box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);

        do_grads = false;
        double s = 0.0001;
        for (size_t i = 0; i < p1.size(); i++) {
            p1[i] += s;
            double ep = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                               end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                               box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            p1[i] += s;
            double epp = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            p1[i] -= 4 * s;
            double emm = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            p1[i] += s;
            double em = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                               end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                               box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            p1[i] += s;
            double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
            REQUIRE(numgrad == Approx(grad1[i]).margin(TOL));
        }

        for (size_t i = 0; i < xyz2.size(); i++) {
            xyz2[i] += s;
            double ep = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                               end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                               box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            xyz2[i] += s;
            double epp = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            xyz2[i] -= 4 * s;
            double emm = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                                end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            xyz2[i] += s;
            double em = lj::lj(eps, sigma, ljchgi, ljchgj, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2,
                               end2, atom_index1, atom_index2, lj_scale_factor, do_grads, cutoff, ewald_alpha, box,
                               box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
            xyz2[i] += s;
            double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
            REQUIRE(numgrad == Approx(grad2[i]).margin(TOL));
        }
    }
}

TEST_CASE("ljtools::GetLjParams") {
    std::vector<std::pair<std::string, std::string> > lj_pairs = {{"mon_t", "test_mon"}, {"mon_t", "mon_t"}};

    nlohmann::json jsonDisp = {
        {"pairs", nlohmann::json::array(
                      {nlohmann::json::array({"mon_t", "test_mon"}), nlohmann::json::array({"mon_t", "mon_t"})})},
        {"types1", nlohmann::json::array({{"a", "b"}, {"a", "b"}})},
        {"types2", nlohmann::json::array({{"c"}, {"a", "b"}})},
        {"c6",
         nlohmann::json::array({nlohmann::json::array({{{"a", "c"}, 10.0}, {{"b", "c"}, 20.0}}),
                                nlohmann::json::array({{{"a", "a"}, 15.0}, {{"a", "b"}, 25.0}, {{"b", "b"}, 50.0}})})},
        {"d6",
         nlohmann::json::array({nlohmann::json::array({{{"a", "c"}, 1.0}, {{"b", "c"}, 2.0}}),
                                nlohmann::json::array({{{"a", "a"}, 1.5}, {{"a", "b"}, 2.5}, {{"b", "b"}, 5.0}})})},
        {"a", nlohmann::json::array(
                  {nlohmann::json::array({{{"a", "c"}, 100.0}, {{"b", "c"}, 200.0}}),
                   nlohmann::json::array({{{"a", "a"}, 150.0}, {{"a", "b"}, 250.0}, {{"b", "b"}, 500.0}})})},
        {"sigma",
         nlohmann::json::array({nlohmann::json::array({{{"a", "c"}, 0.2}, {{"b", "c"}, 0.3}}),
                                nlohmann::json::array({{{"a", "a"}, 0.6}, {{"a", "b"}, 0.8}, {{"b", "b"}, 1.23}})})},
        {"epsilon",
         nlohmann::json::array({nlohmann::json::array({{{"a", "c"}, 12.0}, {{"b", "c"}, 22.0}}),
                                nlohmann::json::array({{{"a", "a"}, 12.5}, {{"a", "b"}, 22.5}, {{"b", "b"}, 52.0}})})}};

    std::vector<std::string> mon1 = {"mon_t", "mon_t"};
    std::vector<std::string> mon2 = {"test_mon", "mon_t"};
    std::vector<size_t> index1 = {1, 1};
    std::vector<size_t> index2 = {0, 0};
    std::vector<double> expected_out_epsilon = {22.0, 22.5};
    std::vector<double> expected_out_sigma = {0.3, 0.8};

    for (size_t i = 0; i < mon1.size(); i++) {
        SECTION(mon1[i] + " -- " + mon2[i]) {
            double eps = 0.0;
            double sigma = 0.0;
            lj::GetLjParams(mon1[i], mon2[i], index1[i], index2[i], eps, sigma, lj_pairs, jsonDisp);

            REQUIRE(eps == Approx(expected_out_epsilon[i]).margin(TOL));
            REQUIRE(sigma == Approx(expected_out_sigma[i]).margin(TOL));

            eps = 0.0;
            sigma = 0.0;
            lj::GetLjParams(mon2[i], mon1[i], index2[i], index1[i], eps, sigma, lj_pairs, jsonDisp);

            REQUIRE(eps == Approx(expected_out_epsilon[i]).margin(TOL));
            REQUIRE(sigma == Approx(expected_out_sigma[i]).margin(TOL));
        }
    }

    SECTION("Unkonwn pair") {
        double c6 = 100.0;
        double d6 = 1.0;
        lj::GetLjParams("notAmon", "NeitherAMon", 0, 2, c6, d6, lj_pairs, jsonDisp);

        REQUIRE(c6 == Approx(0.0).margin(TOL));
        REQUIRE(d6 == Approx(0.0).margin(TOL));
    }
}
