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

#include "potential/dispersion/disptools.h"
#include "tools/math_tools.h"
#include "setup_monomer_mix.h"
#include "json/json.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

TEST_CASE("disptools::factorial") {
    int fac5 = disp::factorial<5>();
    REQUIRE(fac5 == 120);
    int fac0 = disp::factorial<0>();
    REQUIRE(fac0 == 1);
}

TEST_CASE("disptools::tang_toennies") {
    REQUIRE(disp::tang_toennies(6, 1.5739453024e+01) == Approx(9.9525216483e-01).margin(TOL));
    REQUIRE(disp::tang_toennies(6, 7.7861960823e+00) == Approx(6.5982125868e-01).margin(TOL));
    REQUIRE(disp::tang_toennies(6, 2.0237085301e+01) == Approx(9.9978489638e-01).margin(TOL));
    REQUIRE(disp::tang_toennies(6, 2.4523130297e+01) == Approx(9.9999118823e-01).margin(TOL));
    REQUIRE(disp::tang_toennies(6, 1.0) == Approx(8.3241149288e-05).margin(TOL));
}

TEST_CASE("disptools::disp6") {
    SECTION("Normal Behavior") {
        SECTION("Gas Phase") {
            double C6 = 9.4198730000e+01;
            double d6 = 3.8950300000e+00;
            double c6i = 4.4825869765e+00;
            double c6j = 1.3042057313e+01;
            std::vector<double> p1 = {-7.5808708930e-01, 3.9350777550e-01, 1.4232346440e+00};
            std::vector<double> xyz2 = {
                6.6630444410e-01,  2.4803292099e+00,  -1.4243133388e+00, 6.4986549630e-01,  5.1568113210e-01,
                -3.8357176030e-01, 7.5103875900e-01,  7.7214187430e-01,  3.4030903031e+00,  3.8303951459e+00,
                1.1519802350e-01,  -2.9043390394e+00, -2.3329941557e+00, -6.8029207450e-01, -4.0222969886e+00,
                1.7838183644e+00,  2.2674715176e+00,  -2.4475497469e+00, 7.9296435600e-02,  -3.7522694700e-01,
                -1.9222069500e-01, 1.8651909097e+00,  1.2692755042e+00,  2.4412841935e+00,  3.2992897240e+00,
                -1.7587628680e-01, -2.6082983571e+00, -2.0709757555e+00, -3.3312155960e-01, -3.4756612500e+00,
                -4.4811475090e-01, 2.7020706245e+00,  -4.0159255460e-01, 1.2145816282e+00,  1.3973748074e+00,
                -5.7997649630e-01, -3.5351972400e-01, 2.6460559090e-01,  4.3686301334e+00,  4.3616623515e+00,
                4.1069507510e-01,  -3.2106052081e+00, -2.5998516399e+00, -1.0197806380e+00, -4.5737147405e+00};

            std::vector<double> grad1 = {-1.5602707392e+00, 7.9713906149e-01, 1.5174706448e+00};

            std::vector<double> grad2 = {
                2.7559036584e+00,  4.0500646739e-01,  -2.2881180029e+00, 2.0055166427e-01, 1.1268061198e+00,
                -9.8783554687e-01, -1.1740351586e-01, -1.5545731735e+01, 2.7497676281e-01, 2.1296689355e+00,
                -1.8696933143e+00, 3.0651863863e-02,  2.0141931283e+01,  2.5929214333e-01, -7.0132836925e-01,
                2.2502932951e-01,  6.5330230633e-01,  -9.8854654656e+00, 3.1190774309e-01, 1.5664101976e+00,
                -2.8418905843e-02, 3.4980154704e-02,  -1.0676288141e+00, 3.8390030254e-01, 1.4464816861e+01,
                -1.8052983728e-02, 1.3415728545e-01,  1.0658989420e+01,  5.3630444635e-01, -1.1179808492e+00,
                1.3315723238e+01,  1.0903716096e-01,  8.8664032643e+00,  6.0681187659e-02, 1.3292312484e-01,
                -4.6995368081e+00, -5.8794209620e-02, -1.8821714845e+01, 8.5457516147e-02, 1.7068057499e-01,
                -9.5593203280e+00, -5.6454564178e-03, 6.4835160270e+00,  6.2181255501e-02, -8.4041713371e-02};
            double phi1 = -2.3003598299e-01;
            std::vector<double> phi2 = {-1.5665182427e-01, -1.4744919246e-02, -1.8290249299e+01, -1.7241198426e-02,
                                        -6.9553853241e-02, -1.2684931892e-02, -2.4074160517e-02, -1.0852190438e+00,
                                        -3.8229270623e-02, -1.1074675116e+00, -3.8435693859e+00, -5.6511445188e-03,
                                        -4.0028883587e+00, -5.4220941515e-03, -8.0340013668e-03};
            size_t nmon1 = 2;
            size_t nmon2 = 5;
            size_t start2 = 0;
            size_t end2 = 5;
            size_t atom_index1 = 1;
            size_t atom_index2 = 1;
            double disp_scale_factor = 1.0000000000e+00;
            bool do_grads = true;
            double cutoff = 5.0000000000e+01;
            double ewald_alpha = 0.0000000000e+00;
            bool use_ghost = true;
            std::vector<double> box = {};
            std::vector<double> box_inverse = {};
            std::vector<size_t> islocal = {1, 1, 1, 1, 1, 1, 1};
            std::vector<double> virial = {-4.9775752344e+01, 6.2098964154e+00,  1.9536988789e+01,
                                          6.2098964154e+00,  -6.3298110336e+01, 2.1719295495e+01,
                                          1.9536988789e+01,  2.1719295495e+01,  -4.9199075510e+01};
            size_t isl1_offset = 1;
            size_t isl2_offset = 2;

            std::vector<double> expected_grad1 = {-1.8154521306e+00, 5.8364918629e-01, 1.8678816186e+00};
            std::vector<double> expected_grad2 = {
                2.7559036584e+00,  4.0500646739e-01,  -2.2881180029e+00, 2.0055166427e-01, 1.1268061198e+00,
                -9.8783554687e-01, -1.1740351586e-01, -1.5545731735e+01, 2.7497676281e-01, 2.1296689355e+00,
                -1.8696933143e+00, 3.0651863863e-02,  2.0141931283e+01,  2.5929214333e-01, -7.0132836925e-01,
                3.9324731510e-01,  6.5625882960e-01,  -9.9004438089e+00, 4.1070116677e-01, 1.5666019997e+00,
                -6.7181190753e-02, 3.6418257872e-02,  -1.0598644817e+00, 6.2549431204e-01, 1.4466272576e+01,
                -1.2387880000e-01, 1.3021774151e-01,  1.0628010645e+01,  3.2909181629e-01, -1.1204350583e+00,
                1.3315723238e+01,  1.0903716096e-01,  8.8664032643e+00,  6.0681187659e-02, 1.3292312484e-01,
                -4.6995368081e+00, -5.8794209620e-02, -1.8821714845e+01, 8.5457516147e-02, 1.7068057499e-01,
                -9.5593203280e+00, -5.6454564178e-03, 6.4835160270e+00,  6.2181255501e-02, -8.4041713371e-02};
            std::vector<double> expected_phi2 = {
                -1.5665182427e-01, -1.4744919246e-02, -1.8290249299e+01, -1.7241198426e-02, -6.9553853241e-02,
                -1.8148672107e-02, -2.4287991806e-02, -1.0863489008e+00, -4.7052350553e-02, -1.1075970214e+00,
                -3.8435693859e+00, -5.6511445188e-03, -4.0028883587e+00, -5.4220941515e-03, -8.0340013668e-03};
            double expected_phi1 = -2.7588965204e-01;
            std::vector<double> expected_virial = {-5.0320398464e+01, 6.1143287845e+00,  1.9940025908e+01,
                                                   6.1143287845e+00,  -6.3828691199e+01, 2.2121694807e+01,
                                                   1.9940025908e+01,  2.2121694807e+01,  -4.9868393590e+01};
            double expected_energy = -3.1099016679e-01;

            std::vector<double> expected_grad1_ndiff(grad1.size());
            for (size_t i = 0; i < grad1.size(); i++) expected_grad1_ndiff[i] = expected_grad1[i] - grad1[i];

            std::vector<double> expected_grad2_ndiff(grad2.size());
            for (size_t i = 0; i < grad2.size(); i++) expected_grad2_ndiff[i] = expected_grad2[i] - grad2[i];

            double energy =
                disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                            atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                            box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);

            REQUIRE(energy == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, expected_grad1, TOL));
            REQUIRE(VectorsAreEqual(grad2, expected_grad2, TOL));
            REQUIRE(phi1 == Approx(expected_phi1).margin(TOL));
            REQUIRE(VectorsAreEqual(phi2, expected_phi2, TOL));
            REQUIRE(VectorsAreEqual(virial, expected_virial, TOL));

            // Check numerical gradients
            do_grads = false;
            double s = 0.0001;
            for (size_t i = 0; i < p1.size(); i++) {
                p1[i] += s;
                double ep =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double epp =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] -= 4 * s;
                double emm =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double em =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad1_ndiff[i]).margin(TOL));
            }

            for (size_t i = 0; i < xyz2.size(); i++) {
                xyz2[i] += s;
                double ep =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double epp =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] -= 4 * s;
                double emm =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double em =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad2_ndiff[i]).margin(TOL));
            }
        }

        SECTION("PBC") {
            double C6 = 2.0807540000e+02;
            double d6 = 3.5304500000e+00;
            double c6i = 1.5405233572e+01;
            double c6j = 1.3042057313e+01;
            std::vector<double> p1 = {-1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00};
            std::vector<double> xyz2 = {
                6.6630444410e-01,  2.4803292099e+00,  -1.4243133388e+00, 6.4986549630e-01,  5.1568113210e-01,
                -3.8357176030e-01, 7.5103875900e-01,  7.7214187430e-01,  3.4030903031e+00,  3.8303951459e+00,
                1.1519802350e-01,  -2.9043390394e+00, -2.3329941557e+00, -6.8029207450e-01, -4.0222969886e+00,
                1.7838183644e+00,  2.2674715176e+00,  -2.4475497469e+00, 7.9296435600e-02,  -3.7522694700e-01,
                -1.9222069500e-01, 1.8651909097e+00,  1.2692755042e+00,  2.4412841935e+00,  3.2992897240e+00,
                -1.7587628680e-01, -2.6082983571e+00, -2.0709757555e+00, -3.3312155960e-01, -3.4756612500e+00,
                -4.4811475090e-01, 2.7020706245e+00,  -4.0159255460e-01, 1.2145816282e+00,  1.3973748074e+00,
                -5.7997649630e-01, -3.5351972400e-01, 2.6460559090e-01,  4.3686301334e+00,  4.3616623515e+00,
                4.1069507510e-01,  -3.2106052081e+00, -2.5998516399e+00, -1.0197806380e+00, -4.5737147405e+00};

            std::vector<double> grad1 = {-6.9504717770e-01, -2.6106503883e-02, 3.7523345337e-01};

            std::vector<double> grad2 = {
                6.1796676140e-01,  1.4193819477e-01,  -1.7892794346e+00, 4.6597436928e-02, 8.6794595614e-01,
                -4.7129515798e-02, -5.7345907839e-02, -1.4792107621e+01, 5.5945168783e-02, 1.6817079890e+00,
                -2.8918823331e-01, 2.6316479806e-02,  1.9471764045e+01,  1.1281504960e-01, -5.2900860376e-01,
                6.9501515951e-02,  3.9121764766e-01,  -9.1456263427e+00, 1.5264008403e-01, 1.3549064605e+00,
                -6.1278446053e-03, 8.6371981029e-03,  -1.0280762319e+00, 1.7797976446e-01, 1.3680911768e+01,
                -1.2267006450e-02, 9.4937621795e-02,  1.0212475834e+01,  3.2064416812e-01, -9.6382210761e-01,
                4.2876998612e-03,  3.0789351321e-02,  8.4477818840e+00,  1.8217356321e-02, 6.8530893972e-02,
                -1.4946965844e-02, -1.8838688266e-02, -1.7993554985e+01, 2.5443466627e-02, 8.8611292824e-02,
                2.5643638360e-02,  4.5382222314e-04,  6.2558637351e+00,  2.3052252965e-02, -4.2282742880e-02};
            double phi1 = -1.4461317433e-01;
            std::vector<double> phi2 = {-1.0195675674e-01, -1.4567495229e-02, -1.8288837122e+01, -1.6029203848e-02,
                                        -6.9497820128e-02, -1.2684931892e-02, -2.4074160517e-02, -1.0852190438e+00,
                                        -3.8229270623e-02, -1.1074675116e+00, -5.4774151250e-03, -5.3173844183e-03,
                                        -3.9984787023e+00, -4.9856391653e-03, -7.9681090694e-03};
            size_t nmon1 = 2;
            size_t nmon2 = 5;
            size_t start2 = 0;
            size_t end2 = 5;
            size_t atom_index1 = 0;
            size_t atom_index2 = 2;
            double disp_scale_factor = 1.0000000000e+00;
            bool do_grads = true;
            double cutoff = 9.0000000000e+00;
            double ewald_alpha = 6.0000000000e-01;
            bool use_ghost = false;
            std::vector<double> box = {2.0000000000e+01, 0.0000000000e+00, 0.0000000000e+00,
                                       0.0000000000e+00, 2.0000000000e+01, 0.0000000000e+00,
                                       0.0000000000e+00, 0.0000000000e+00, 2.0000000000e+01};
            std::vector<double> box_inverse = {5.0000000000e-02, 0.0000000000e+00, 0.0000000000e+00,
                                               0.0000000000e+00, 5.0000000000e-02, 0.0000000000e+00,
                                               0.0000000000e+00, 0.0000000000e+00, 5.0000000000e-02};
            std::vector<size_t> islocal = {1, 1, 1, 1, 1, 1, 1};
            std::vector<double> virial = {-2.7844471139e+01, 1.8782116284e+00,  7.0011653969e+00,
                                          1.8782116284e+00,  -5.5240271784e+01, 2.3946874976e+01,
                                          7.0011653969e+00,  2.3946874976e+01,  -3.4861057129e+01};
            size_t isl1_offset = 1;
            size_t isl2_offset = 2;

            std::vector<double> expected_grad1 = {-1.3473705722e+01, 4.4286417589e+00, 9.6698713949e+00};
            std::vector<double> expected_grad2 = {
                6.1796676140e-01,  1.4193819477e-01,  -1.7892794346e+00, 4.6597436928e-02, 8.6794595614e-01,
                -4.7129515798e-02, -5.7345907839e-02, -1.4792107621e+01, 5.5945168783e-02, 1.6817079890e+00,
                -2.8918823331e-01, 2.6316479806e-02,  1.9471764045e+01,  1.1281504960e-01, -5.2900860376e-01,
                6.9501515951e-02,  3.9121764766e-01,  -9.1456263427e+00, 1.5264008403e-01, 1.3549064605e+00,
                -6.1278446053e-03, 8.6371981029e-03,  -1.0280762319e+00, 1.7797976446e-01, 1.3680911768e+01,
                -1.2267006450e-02, 9.4937621795e-02,  1.0212475834e+01,  3.2064416812e-01, -9.6382210761e-01,
                1.2778031768e+01,  3.0898692633e-02,  8.4524752972e+00,  1.8320646205e-02, 6.8539325261e-02,
                -4.4721776159e+00, -1.8841759876e-02, -1.7991263160e+01, 2.5623384300e-02, 8.8625008697e-02,
                -9.2517815336e+00, 3.3912392567e-04,  6.2388663791e+00,  2.2968528868e-02, -4.2299734040e-02};
            std::vector<double> expected_phi2 = {
                -1.0195675674e-01, -1.4567495229e-02, -1.8288837122e+01, -1.6029203848e-02, -6.9497820128e-02,
                -1.2684931892e-02, -2.4074160517e-02, -1.0852190438e+00, -3.8229270623e-02, -1.1074675116e+00,
                -3.8435693859e+00, -5.6511445188e-03, -4.0028883587e+00, -5.4220941515e-03, -8.0340013668e-03};
            double expected_phi1 = -3.3983795501e+00;
            std::vector<double> expected_virial = {-4.0389158936e+01, 6.2507128192e+00,  1.6126400294e+01,
                                                   6.2507128192e+00,  -5.6769038896e+01, 2.0778085240e+01,
                                                   1.6126400294e+01,  2.0778085240e+01,  -4.1539442076e+01};
            double expected_energy = -7.4162415106e+00;

            std::vector<double> expected_grad1_ndiff(grad1.size());
            for (size_t i = 0; i < grad1.size(); i++) expected_grad1_ndiff[i] = expected_grad1[i] - grad1[i];

            std::vector<double> expected_grad2_ndiff(grad2.size());
            for (size_t i = 0; i < grad2.size(); i++) expected_grad2_ndiff[i] = expected_grad2[i] - grad2[i];

            double energy =
                disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                            atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                            box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);

            REQUIRE(energy == Approx(expected_energy).margin(TOL));
            REQUIRE(VectorsAreEqual(grad1, expected_grad1, TOL));
            REQUIRE(VectorsAreEqual(grad2, expected_grad2, TOL));
            REQUIRE(phi1 == Approx(expected_phi1).margin(TOL));
            REQUIRE(VectorsAreEqual(phi2, expected_phi2, TOL));
            REQUIRE(VectorsAreEqual(virial, expected_virial, TOL));

            // Check numerical gradients
            do_grads = false;
            double s = 0.0001;
            for (size_t i = 0; i < p1.size(); i++) {
                p1[i] += s;
                double ep =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double epp =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] -= 4 * s;
                double emm =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double em =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                p1[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad1_ndiff[i]).margin(TOL));
            }

            for (size_t i = 0; i < xyz2.size(); i++) {
                xyz2[i] += s;
                double ep =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double epp =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] -= 4 * s;
                double emm =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double em =
                    disp::disp6(C6, d6, c6i, c6j, p1, xyz2, grad1, grad2, phi1, phi2, nmon1, nmon2, start2, end2,
                                atom_index1, atom_index2, disp_scale_factor, do_grads, cutoff, ewald_alpha, box,
                                box_inverse, use_ghost, islocal, isl1_offset, isl2_offset, &virial);
                xyz2[i] += s;
                double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
                REQUIRE(numgrad == Approx(expected_grad2_ndiff[i]).margin(TOL));
            }
        }
    }
}

TEST_CASE("disptools::GetC6") {
    std::vector<std::pair<std::string, std::string> > ignore_disp;

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
                   nlohmann::json::array({{{"a", "a"}, 150.0}, {{"a", "b"}, 250.0}, {{"b", "b"}, 500.0}})})}};

    std::vector<std::string> mon1 = {"mon_t", "mon_t", "h2o", "f",  "cl",          "br",  "i",    "li",
                                     "na",    "k",     "rb",  "cs", "he",          "he",  "ar",   "co2",
                                     "ch4",   "co2",   "ch4", "ar", "nh3pbe0d3bj", "nh3", "ar",   "h2",
                                     "h2",    "ar",    "cs",  "na", "cl",          "cl",  "n2o5", "h2o"};
    std::vector<std::string> mon2 = {"test_mon", "mon_t", "h2o", "h2o", "h2o",         "h2o", "h2o",  "h2o",
                                     "h2o",      "h2o",   "h2o", "h2o", "he",          "h2o", "h2o",  "co2",
                                     "ch4",      "h2o",   "h2o", "cs",  "nh3pbe0d3bj", "nh3", "ar",   "h2",
                                     "h2o",      "h2o",   "h2",  "na",  "cl",          "na",  "n2o5", "n2o5"};
    std::vector<size_t> index1 = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                                  4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};
    std::vector<size_t> index2 = {0, 0, 1, 0, 1, 0, 1, 2, 1, 2, 1, 0, 0, 2, 1, 1,
                                  0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 1};
    std::vector<double> expected_out_c6 = {20.0,
                                           25.0,
                                           8.349556669872743e+01,
                                           3.488640000000000e+02,
                                           3.068900000000000e+02,
                                           9.426500000000000e+02,
                                           5.681559999999999e+02,
                                           1.871220000000000e+01,
                                           8.578690000000000e+01,
                                           2.181920000000000e+02,
                                           2.861310000000000e+02,
                                           7.045400000000000e+02,
                                           24.348011,
                                           26.208221,
                                           170.8771,
                                           170.09525896,
                                           104.10825,
                                           130.8452,
                                           105.979,
                                           1500.0914,
                                           39.9906,
                                           98.6092,
                                           774.5257,
                                           45.4166,
                                           40.6489,
                                           382.031,
                                           372.8086,
                                           251.48,
                                           3066.2919,
                                           784.5853,
                                           165.351572694970,
                                           79.65812014580179};
    std::vector<double> expected_out_d6 = {2.0,
                                           2.5,
                                           9.775202425217957e+00,
                                           3.586190000000000e+00,
                                           2.782260000000000e+00,
                                           3.058250000000000e+00,
                                           2.799110000000000e+00,
                                           4.006630000000000e+00,
                                           3.822550000000000e+00,
                                           3.321390000000000e+00,
                                           3.313640000000000e+00,
                                           3.028640000000000e+00,
                                           4.02693,
                                           3.9707,
                                           3.45707,
                                           3.52744,
                                           3.25885,
                                           3.7359,
                                           3.68542,
                                           3.28039,
                                           3.83901,
                                           3.4174,
                                           3.41808,
                                           3.11276,
                                           3.62823,
                                           3.48054,
                                           3.24781,
                                           4.42822,
                                           1.82786,
                                           2.85113,
                                           4.19428,
                                           6.4749};

    for (size_t i = 0; i < mon1.size(); i++) {
        SECTION(mon1[i] + " -- " + mon2[i]) {
            double c6 = 0.0;
            double d6 = 0.0;
            disp::GetC6(mon1[i], mon2[i], index1[i], index2[i], c6, d6, ignore_disp, jsonDisp);

            REQUIRE(c6 == Approx(expected_out_c6[i]).margin(TOL));
            REQUIRE(d6 == Approx(expected_out_d6[i]).margin(TOL));

            c6 = 0.0;
            d6 = 0.0;
            disp::GetC6(mon2[i], mon1[i], index2[i], index1[i], c6, d6, ignore_disp, jsonDisp);

            REQUIRE(c6 == Approx(expected_out_c6[i]).margin(TOL));
            REQUIRE(d6 == Approx(expected_out_d6[i]).margin(TOL));
        }
    }

    SECTION("Unkonwn pair") {
        double c6 = 100.0;
        double d6 = 1.0;
        disp::GetC6("notAmon", "NeitherAMon", 0, 2, c6, d6, ignore_disp, jsonDisp);

        REQUIRE(c6 == Approx(0.0).margin(TOL));
        REQUIRE(d6 == Approx(0.0).margin(TOL));
    }

    SECTION("Ignore dispersion") {
        double c6 = 100.0;
        double d6 = 1.0;

        std::vector<std::pair<std::string, std::string> > ignore_disp_filled = {{"h2o", "h2o"}, {"mon_t", "mon_t"}};
        for (size_t i = 0; i < ignore_disp_filled.size(); i++) {
            disp::GetC6(ignore_disp_filled[i].first, ignore_disp_filled[i].second, 0, 1, c6, d6, ignore_disp_filled,
                        jsonDisp);
            REQUIRE(c6 == Approx(0.0).margin(TOL));
            REQUIRE(d6 == Approx(0.0).margin(TOL));

            disp::GetC6(ignore_disp_filled[i].second, ignore_disp_filled[i].first, 0, 1, c6, d6, ignore_disp_filled,
                        jsonDisp);
            REQUIRE(c6 == Approx(0.0).margin(TOL));
            REQUIRE(d6 == Approx(0.0).margin(TOL));
        }
    }
}
