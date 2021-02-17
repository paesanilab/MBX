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

#include "potential/lj/lj.h"
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

TEST_CASE("lj::No Long Range") {
    std::vector<double> ljlr(2, 0.0);
    std::vector<double> sys_xyz = {4.5278490461619709E+00, 6.1779461427431679E+00, 3.9068784243576893E+00,
                                   5.1292456840248581E+00, 7.3641793429207905E+00, 3.8284728828715551E+00};
    std::vector<double> box = {};
    std::vector<size_t> num_atoms = {1, 1};
    std::vector<std::pair<std::string, size_t> > mon_type_count = {{"nc", 1}, {"cd", 1}};
    std::vector<size_t> islocal = {1, 1};
    std::vector<std::string> mon_id = {"nc", "cd"};
    bool do_grads = true;
    std::vector<std::pair<std::string, std::string> > lj_pairs = {{"nc", "nc"}, {"cd", "nc"}, {"cd", "cd"}};

    std::vector<double> internal_ljlr_expected(2, 0.0);
    std::vector<double> internal_xyz_expected = sys_xyz;
    std::vector<size_t> internal_is_local_atom_expected = {1, 1};

    std::vector<double> box_ABCabc = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
    std::vector<double> box_inverse = box.size() ? InvertUnitCell(box) : std::vector<double>{};

    // Declare and initialize dispersion
    lj::LennardJones d;
    d.Initialize(ljlr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);

    SECTION("Initialize") {
        // Get everything and ensure that is correct
        std::vector<size_t> getislocal = d.GetIsLocal();
        std::vector<std::string> getmonids = d.GetMonIds();
        std::vector<size_t> getnumatsvec = d.GetNumAtomsVector();
        std::vector<std::pair<std::string, size_t> > getmontypecount = d.GetMonTypeCount();
        bool getdograds = d.GetDoGrads();
        std::vector<double> getbox = d.GetBox();
        std::vector<double> getboxabc = d.GetBoxAbc();
        std::vector<double> getboxinverse = d.GetBoxInverse();
        std::vector<double> getsysxyz = d.GetSystemXyz();
        std::vector<double> getinternalxyz = d.GetInternalXyz();
        std::vector<double> getljlrsys = d.GetSystemLjLongRange();
        std::vector<double> getinternalljlr = d.GetInternalLjLongRange();
        std::vector<size_t> getislocalatom = d.GetIsLocalAtom();

        // Require that everything matches what is expected
        REQUIRE(getdograds == do_grads);
        REQUIRE(VectorsAreEqual(getislocal, islocal));
        REQUIRE(VectorsAreEqual(getmonids, mon_id));
        REQUIRE(VectorsAreEqual(getnumatsvec, num_atoms));
        REQUIRE(VectorsAreEqual(getmontypecount, mon_type_count));
        REQUIRE(VectorsAreEqual(getbox, box, TOL));
        REQUIRE(VectorsAreEqual(getboxabc, box_ABCabc, TOL));
        REQUIRE(VectorsAreEqual(getboxinverse, box_inverse, TOL));
        REQUIRE(VectorsAreEqual(getsysxyz, sys_xyz, TOL));
        REQUIRE(VectorsAreEqual(getinternalxyz, internal_xyz_expected, TOL));
        REQUIRE(VectorsAreEqual(getljlrsys, ljlr, TOL));
        REQUIRE(VectorsAreEqual(getinternalljlr, internal_ljlr_expected, TOL));
        REQUIRE(VectorsAreEqual(getislocalatom, internal_is_local_atom_expected));
    }

    double cutoff = 100.0;
    std::vector<double> virial(9, 0.0);
    std::vector<double> grad(sys_xyz.size(), 0.0);
    bool use_ghost = false;

    d.SetNewParameters(sys_xyz, lj_pairs, do_grads, cutoff, box);
    nlohmann::json jsonMon = {
        {"nc",
         {{"sites", 1},
          {"nat", 1},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.0})},
          {"pol", nlohmann::json::array({0.0})},
          {"polfac", nlohmann::json::array({0.0})},
          {"c6lr", nlohmann::json::array({0.0})}}},
        {"cd",
         {{"sites", 1},
          {"nat", 1},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.0})},
          {"pol", nlohmann::json::array({0.0})},
          {"polfac", nlohmann::json::array({0.0})},
          {"c6lr", nlohmann::json::array({0.0})}}},
    };

    nlohmann::json jsonDisp = {
        {"pairs", nlohmann::json::array({nlohmann::json::array({"cd", "nc"})})},
        {"types1", nlohmann::json::array({{"cd"}})},
        {"types2", nlohmann::json::array({{"nc"}})},
        {"c6", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 10.0}})})},
        {"d6", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 1.0}})})},
        {"a", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 100.0}})})},
        {"sigma", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 3.325000}})})},
        {"epsilon", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 0.120984}})})}};

    d.SetJsonLennardJones(jsonDisp);
    d.SetJsonMonomers(jsonMon);
    d.SetEwaldAlpha(0.0);
    d.SetEwaldGridDensity(1.0);
    d.SetEwaldSplineOrder(6);
    SECTION("Energy") {
        double expected_energy = 2.8140638986e+04;
        std::vector<double> expected_grads = {1.1465301877e+05,  2.2614894863e+05,  -1.4947592742e+04,
                                              -1.1465301877e+05, -2.2614894863e+05, 1.4947592742e+04};
        double energy = d.GetLennardJones(grad, &virial, use_ghost);
        REQUIRE(energy == Approx(expected_energy).margin(TOL));
        REQUIRE(VectorsAreEqual(grad, expected_grads, TOL));

        double s = 0.001;
        for (size_t j = 0; j < sys_xyz.size(); j++) {
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double ep = d.GetLennardJones(grad);
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double epp = d.GetLennardJones(grad);
            sys_xyz[j] -= 4 * s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double emm = d.GetLennardJones(grad);
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double em = d.GetLennardJones(grad);
            sys_xyz[j] += s;

            double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
            REQUIRE(numgrad == Approx(expected_grads[j]).margin(TOL));
        }
    }
}

TEST_CASE("lj::Gas Phase") {
    std::vector<std::pair<std::string, std::string> > lj_pairs = {{"nc", "nc"}, {"cd", "nc"}, {"cd", "cd"}};
    double ljlr_nc = 2 * std::sqrt(0.170100) * 3.250000 * 3.250000 * 3.250000;
    double ljlr_cd = 2 * std::sqrt(0.086050) * 3.400000 * 3.400000 * 3.400000;
    size_t ntot_ats = 21;
    std::vector<double> sys_xyz(ntot_ats * 3, 0.0);

    for (size_t i = 1; i < ntot_ats; i++) {
        size_t idx = (i) % 3;
        sys_xyz[3 * i + idx] = double(i) / 2.0 + 2.3;
    }

    std::vector<double> ljlr(ntot_ats, ljlr_cd);
    ljlr[0] = ljlr_nc;

    std::vector<double> box = {};
    // std::vector<double> box = {20.0,0.0,0.0,0.0,20.0,0.0,0.0,0.0,20.0};
    std::vector<size_t> num_atoms(ntot_ats, 1);
    std::vector<std::pair<std::string, size_t> > mon_type_count = {{"nc", 1}, {"cd", ntot_ats - 1}};
    std::vector<size_t> islocal(ntot_ats, 1);
    std::vector<std::string> mon_id(ntot_ats, "cd");
    mon_id[0] = "nc";
    bool do_grads = true;

    // Declare and initialize dispersion
    lj::LennardJones d;
    d.Initialize(ljlr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);

    double cutoff = 8.0;
    std::vector<double> virial(9, 0.0);
    std::vector<double> grad(sys_xyz.size(), 0.0);
    bool use_ghost = false;

    d.SetNewParameters(sys_xyz, lj_pairs, do_grads, cutoff, box);
    nlohmann::json jsonMon = {
        {"nc",
         {{"sites", 1},
          {"nat", 1},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.0})},
          {"pol", nlohmann::json::array({0.0})},
          {"polfac", nlohmann::json::array({0.0})},
          {"c6lr", nlohmann::json::array({0.0})}}},
        {"cd",
         {{"sites", 1},
          {"nat", 1},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.0})},
          {"pol", nlohmann::json::array({0.0})},
          {"polfac", nlohmann::json::array({0.0})},
          {"c6lr", nlohmann::json::array({0.0})}}},
    };

    nlohmann::json jsonDisp = {
        {"pairs", nlohmann::json::array({nlohmann::json::array({"cd", "nc"}), nlohmann::json::array({"nc", "nc"}),
                                         nlohmann::json::array({"cd", "cd"})})},
        {"types1", nlohmann::json::array({{"cd"}, {"nc"}, {"cd"}})},
        {"types2", nlohmann::json::array({{"nc"}, {"nc"}, {"cd"}})},
        {"c6", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 10.0}}),
                                      nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 15.0}}),
                                      nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 5.0}})

               })},
        {"d6", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 1.0}}),
                                      nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 1.50}}),
                                      nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 0.50}})

               })},
        {"a", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 100.0}}),
                                     nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 15.0}}),
                                     nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 5.0}})})},
        {"sigma", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 3.325000}}),
                                         nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 3.250000}}),
                                         nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 3.400000}})

                  })},
        {"epsilon", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 0.120984}}),
                                           nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 0.170100}}),
                                           nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 0.086050}})})}};

    d.SetJsonLennardJones(jsonDisp);
    d.SetJsonMonomers(jsonMon);
    d.SetEwaldAlpha(0.0);
    d.SetEwaldGridDensity(2.5);
    d.SetEwaldSplineOrder(6);
    SECTION("Energy") {
        double expected_energy = 2.1380674525e+00;
        double energy = d.GetLennardJones(grad, &virial, use_ghost);

        double s = 0.001;
        for (size_t j = 0; j < sys_xyz.size(); j++) {
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double ep = d.GetLennardJones(grad);
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double epp = d.GetLennardJones(grad);
            sys_xyz[j] -= 4 * s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double emm = d.GetLennardJones(grad);
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double em = d.GetLennardJones(grad);
            sys_xyz[j] += s;

            double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
            REQUIRE(numgrad == Approx(grad[j]).margin(TOL));
        }
    }
}

TEST_CASE("lj::PBC") {
    std::vector<std::pair<std::string, std::string> > lj_pairs = {{"nc", "nc"}, {"cd", "nc"}, {"cd", "cd"}};
    double ljlr_nc = 2 * std::sqrt(0.170100) * 3.250000 * 3.250000 * 3.250000;
    double ljlr_cd = 2 * std::sqrt(0.086050) * 3.400000 * 3.400000 * 3.400000;
    size_t ntot_ats = 21;
    std::vector<double> sys_xyz(ntot_ats * 3, 0.0);

    for (size_t i = 1; i < ntot_ats; i++) {
        size_t idx = (i) % 3;
        sys_xyz[3 * i + idx] = double(i) / 2.0 + 2.3;
    }

    std::vector<double> ljlr(ntot_ats, ljlr_cd);
    ljlr[0] = ljlr_nc;

    std::vector<double> box = {20.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 20.0};
    std::vector<size_t> num_atoms(ntot_ats, 1);
    std::vector<std::pair<std::string, size_t> > mon_type_count = {{"nc", 1}, {"cd", ntot_ats - 1}};
    std::vector<size_t> islocal(ntot_ats, 1);
    std::vector<std::string> mon_id(ntot_ats, "cd");
    mon_id[0] = "nc";
    bool do_grads = true;

    lj::LennardJones d;
    d.Initialize(ljlr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);

    double cutoff = 8.0;
    std::vector<double> virial(9, 0.0);
    std::vector<double> grad(sys_xyz.size(), 0.0);
    bool use_ghost = false;

    d.SetNewParameters(sys_xyz, lj_pairs, do_grads, cutoff, box);
    nlohmann::json jsonMon = {
        {"nc",
         {{"sites", 1},
          {"nat", 1},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.0})},
          {"pol", nlohmann::json::array({0.0})},
          {"polfac", nlohmann::json::array({0.0})},
          {"c6lr", nlohmann::json::array({0.0})}}},
        {"cd",
         {{"sites", 1},
          {"nat", 1},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.0})},
          {"pol", nlohmann::json::array({0.0})},
          {"polfac", nlohmann::json::array({0.0})},
          {"c6lr", nlohmann::json::array({0.0})}}},
    };

    nlohmann::json jsonDisp = {
        {"pairs", nlohmann::json::array({nlohmann::json::array({"cd", "nc"}), nlohmann::json::array({"nc", "nc"}),
                                         nlohmann::json::array({"cd", "cd"})})},
        {"types1", nlohmann::json::array({{"cd"}, {"nc"}, {"cd"}})},
        {"types2", nlohmann::json::array({{"nc"}, {"nc"}, {"cd"}})},
        {"c6", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 10.0}}),
                                      nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 15.0}}),
                                      nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 5.0}})

               })},
        {"d6", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 1.0}}),
                                      nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 1.50}}),
                                      nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 0.50}})

               })},
        {"a", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 100.0}}),
                                     nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 15.0}}),
                                     nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 5.0}})})},
        {"sigma", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 3.325000}}),
                                         nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 3.250000}}),
                                         nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 3.400000}})

                  })},
        {"epsilon", nlohmann::json::array({nlohmann::json::array({{nlohmann::json::array({"nc", "cd"}), 0.120984}}),
                                           nlohmann::json::array({{nlohmann::json::array({"nc", "nc"}), 0.170100}}),
                                           nlohmann::json::array({{nlohmann::json::array({"cd", "cd"}), 0.086050}})})}};
    d.SetJsonLennardJones(jsonDisp);
    d.SetJsonMonomers(jsonMon);
    d.SetEwaldAlpha(0.6);
    d.SetEwaldGridDensity(2.5);
    d.SetEwaldSplineOrder(6);
    SECTION("Energy") {
        double expected_energy = 1.0684359220e+05;
        double energy = d.GetLennardJones(grad, &virial, use_ghost);
        std::vector<double> expected_grad = {
            -7.0581607315e-02, 1.3298917810e+01,  9.4469655912e-01,  -4.7626968186e-02, 5.0451502512e+04,
            -6.4042987472e-02, -3.2970954395e-02, -6.0109397479e-02, 5.0463902514e+04,  5.0464936258e+04,
            -4.0896502641e-02, -3.0188833449e-02, -1.4243616520e-02, 4.7774772788e+00,  -1.8959670335e-02,
            -9.2243067120e-03, -1.5414877757e-02, 4.7423908063e+00,  4.7147243826e+00,  -9.4254956037e-03,
            -7.3998794902e-03, -3.9367526853e-03, -3.4631031572e-02, -4.6898320627e-03, -2.6241515778e-03,
            -3.7171616811e-03, -4.3960014399e-02, -3.9491383522e-02, -2.4166256918e-03, -1.9866242737e-03,
            -1.2207938743e-03, 8.1648798839e-03,  -1.3368199480e-03, -8.5735821773e-04, -1.0963238802e-03,
            5.8197017357e-03,  5.7720682860e-02,  -7.7526138310e-04, -6.5186566707e-04, -4.7533258828e-04,
            6.6481094801e-02,  -4.8824232224e-04, -3.8976523914e-04, -4.6615122149e-04, 6.5514910212e-02,
            -4.6591216156e+00, -4.1721647636e-04, -3.5358969123e-04, -3.5616378584e-04, -4.6577325711e+00,
            -3.5803686235e-04, -4.0272753906e-04, -4.8304906483e-04, -4.6587282219e+00, -5.0464823583e+04,
            -6.1001451278e-04, -5.1460367755e-04, -6.6170876822e-04, -5.0464824172e+04, -6.9541276653e-04,
            -9.1800041091e-04, -1.1807816966e-03, -5.0464826572e+04};
        std::vector<double> expected_virial = {3.7850605137e+05, 2.2291029674e-01, 1.6836523149e-01,
                                               2.2291029674e-01, 4.5424751553e+05, 2.9423070555e-01,
                                               1.6836523149e-01, 2.9423070555e-01, 4.5421340125e+05};

        REQUIRE(energy == Approx(expected_energy).margin(TOL));
        REQUIRE(VectorsAreEqual(grad, expected_grad, TOL));
        REQUIRE(VectorsAreEqual(virial, expected_virial, TOL));

        double s = 0.001;
        for (size_t j = 0; j < sys_xyz.size(); j++) {
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double ep = d.GetLennardJones(grad);
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double epp = d.GetLennardJones(grad);
            sys_xyz[j] -= 4 * s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double emm = d.GetLennardJones(grad);
            sys_xyz[j] += s;
            d.SetNewParameters(sys_xyz, lj_pairs, false, cutoff, box);
            double em = d.GetLennardJones(grad);
            sys_xyz[j] += s;

            double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
            REQUIRE(numgrad == Approx(expected_grad[j]).margin(TOL));
        }
    }
}

// std::vector<double> c6lr = {1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00, 1.5405233572e+01, 4.4825869765e+00,
//                            4.4825869765e+00, 1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01, 1.7916733202e+01,
//                            1.3042057313e+01, 1.3042057313e+01, 1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01,
//                            1.7916733202e+01, 1.3042057313e+01, 1.3042057313e+01, 1.7916733202e+01, 1.3042057313e+01,
//                            1.3042057313e+01};
// std::vector<double> sys_xyz = {
//    6.6630444410e-01,  -3.8357176030e-01, 1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01,
//    -4.4811475090e-01, -5.7997649630e-01, 4.1069507510e-01,  2.4803292099e+00,  7.5103875900e-01,  -2.9043390394e+00,
//    2.2674715176e+00,  1.8651909097e+00,  -2.6082983571e+00, 2.7020706245e+00,  -3.5351972400e-01, -3.2106052081e+00,
//    -1.4243133388e+00, 7.7214187430e-01,  -2.3329941557e+00, -2.4475497469e+00, 1.2692755042e+00,  -2.0709757555e+00,
//    -4.0159255460e-01, 2.6460559090e-01,  -2.5998516399e+00, 6.4986549630e-01,  3.4030903031e+00,  -6.8029207450e-01,
//    7.9296435600e-02,  2.4412841935e+00,  -3.3312155960e-01, 1.2145816282e+00,  4.3686301334e+00,  -1.0197806380e+00,
//    5.1568113210e-01,  3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01, 3.2992897240e+00,  -3.4756612500e+00,
//    1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00, -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00,
//    -4.0252047300e-01, 1.9907698680e+00,  -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00,
//    -1.4297465600e+00, -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,
//    -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00};
// std::vector<double> box = {2.0000000000e+01, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 2.0000000000e+01,
//                           0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 2.0000000000e+01};
// std::vector<size_t> num_atoms = {3, 3, 3, 3, 3, 3, 3};
// std::vector<size_t> islocal = {1, 1, 1, 1, 1, 1, 1};
// std::vector<std::pair<std::string, size_t> > mon_type_count = {{"h2o", 2}, {"co2_archive", 5}};
// std::vector<std::string> mon_id = {"h2o", "h2o", "co2_archive", "co2_archive", "co2_archive", "co2_archive",
// "co2_archive"}; bool do_grads = true;
//
// TEST_CASE("dispersion::Initialize") {
//    std::vector<double> internal_c6lr_expected = {
//        1.5405233572e+01, 1.5405233572e+01, 4.4825869765e+00, 4.4825869765e+00, 4.4825869765e+00, 4.4825869765e+00,
//        1.7916733202e+01, 1.7916733202e+01, 1.7916733202e+01, 1.7916733202e+01, 1.7916733202e+01, 1.3042057313e+01,
//        1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01,
//        1.3042057313e+01, 1.3042057313e+01, 1.3042057313e+01};
//    std::vector<double> internal_xyz_expected = {
//        6.6630444410e-01,  2.4803292099e+00,  -3.8357176030e-01, 7.5103875900e-01,  1.1519802350e-01,
//        -2.9043390394e+00, 1.7838183644e+00,  2.2674715176e+00,  -1.9222069500e-01, 1.8651909097e+00,
//        -1.7587628680e-01, -2.6082983571e+00, -4.4811475090e-01, 2.7020706245e+00,  -5.7997649630e-01,
//        -3.5351972400e-01, 4.1069507510e-01,  -3.2106052081e+00, -1.4243133388e+00, 6.4986549630e-01,
//        5.1568113210e-01,  -9.1278996460e-01, -1.4297465600e+00, 7.7214187430e-01,  3.4030903031e+00,
//        3.8303951459e+00,  1.3711133940e+00,  -2.3744893640e-01, -2.3329941557e+00, -6.8029207450e-01,
//        -4.0222969886e+00, -2.9257875690e+00, 1.1236431040e+00,  -2.4475497469e+00, 7.9296435600e-02,
//        -3.7522694700e-01, -4.0252047300e-01, -7.5808708930e-01, 1.2692755042e+00,  2.4412841935e+00,
//        3.2992897240e+00,  1.9907698680e+00,  3.9350777550e-01,  -2.0709757555e+00, -3.3312155960e-01,
//        -3.4756612500e+00, -3.4499780000e+00, 1.4232346440e+00,  -4.0159255460e-01, 1.2145816282e+00,
//        1.3973748074e+00,  -1.7848310380e+00, -2.2258007790e+00, 2.6460559090e-01,  4.3686301334e+00,
//        4.3616623515e+00,  1.3696631160e+00,  -6.7917366630e-02, -2.5998516399e+00, -1.0197806380e+00,
//        -4.5737147405e+00, -3.3238997190e+00, 1.6302588920e+00};
//    std::vector<size_t> internal_is_local_atom_expected = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//                                                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//
//    std::vector<double> box_ABCabc = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
//    std::vector<double> box_inverse = box.size() ? InvertUnitCell(box) : std::vector<double>{};
//
//    // Declare and initialize dispersion
//    disp::Dispersion d;
//    d.Initialize(c6lr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);
//
//    // Get everything and ensure that is correct
//    std::vector<size_t> getislocal = d.GetIsLocal();
//    std::vector<std::string> getmonids = d.GetMonIds();
//    std::vector<size_t> getnumatsvec = d.GetNumAtomsVector();
//    std::vector<std::pair<std::string, size_t> > getmontypecount = d.GetMonTypeCount();
//    bool getdograds = d.GetDoGrads();
//    std::vector<double> getbox = d.GetBox();
//    std::vector<double> getboxabc = d.GetBoxAbc();
//    std::vector<double> getboxinverse = d.GetBoxInverse();
//    std::vector<double> getsysxyz = d.GetSystemXyz();
//    std::vector<double> getinternalxyz = d.GetInternalXyz();
//    std::vector<double> getc6lrsys = d.GetSystemC6LongRange();
//    std::vector<double> getinternalc6lr = d.GetInternalC6LongRange();
//    std::vector<size_t> getislocalatom = d.GetIsLocalAtom();
//
//    // Require that everything matches what is expected
//    REQUIRE(getdograds == do_grads);
//    REQUIRE(VectorsAreEqual(getislocal, islocal));
//    REQUIRE(VectorsAreEqual(getmonids, mon_id));
//    REQUIRE(VectorsAreEqual(getnumatsvec, num_atoms));
//    REQUIRE(VectorsAreEqual(getmontypecount, mon_type_count));
//    REQUIRE(VectorsAreEqual(getbox, box, TOL));
//    REQUIRE(VectorsAreEqual(getboxabc, box_ABCabc, TOL));
//    REQUIRE(VectorsAreEqual(getboxinverse, box_inverse, TOL));
//    REQUIRE(VectorsAreEqual(getsysxyz, sys_xyz, TOL));
//    REQUIRE(VectorsAreEqual(getinternalxyz, internal_xyz_expected, TOL));
//    REQUIRE(VectorsAreEqual(getc6lrsys, c6lr, TOL));
//    REQUIRE(VectorsAreEqual(getinternalc6lr, internal_c6lr_expected, TOL));
//    REQUIRE(VectorsAreEqual(getislocalatom, internal_is_local_atom_expected));
//}
//
// TEST_CASE("dispersion::SetNewParameters") {
//    // Declare and initialize dispersion
//    disp::Dispersion d;
//    d.Initialize(c6lr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);
//
//    box[3] = 10.0;
//    do_grads = false;
//    double cutoff = 10.0;
//    for (size_t i = 0; i < sys_xyz.size(); i++) sys_xyz[i] += 0.002 * double(i);
//
//    std::vector<double> internal_xyz_expected = {
//        6.6630444410e-01,  2.4983292099e+00,  -3.8157176030e-01, 7.7103875900e-01,  1.1919802350e-01,
//        -2.8823390394e+00, 1.7898183644e+00,  2.2914715176e+00,  -1.8422069500e-01, 1.8911909097e+00,
//        -1.6587628680e-01, -2.5802983571e+00, -4.3611475090e-01, 2.7320706245e+00,  -5.6597649630e-01,
//        -3.2151972400e-01, 4.2669507510e-01,  -3.1766052081e+00, -1.3883133388e+00, 7.0386549630e-01,
//        5.8768113210e-01,  -8.2278996460e-01, -1.3217465600e+00, 8.1014187430e-01,  3.4590903031e+00,
//        3.9043951459e+00,  1.4631133940e+00,  -1.2744893640e-01, -2.2929941557e+00, -6.2229207450e-01,
//        -3.9462969886e+00, -2.8317875690e+00, 1.2356431040e+00,  -2.4055497469e+00, 1.3929643560e-01,
//        -2.9722694700e-01, -3.0652047300e-01, -6.4408708930e-01, 1.3132755042e+00,  2.5032841935e+00,
//        3.3792897240e+00,  2.0887698680e+00,  5.0950777550e-01,  -2.0249757555e+00, -2.6912155960e-01,
//        -3.3936612500e+00, -3.3499780000e+00, 1.5412346440e+00,  -3.5359255460e-01, 1.2805816282e+00,
//        1.4813748074e+00,  -1.6828310380e+00, -2.1058007790e+00, 3.1460559090e-01,  4.4366301334e+00,
//        4.4476623515e+00,  1.4736631160e+00,  5.4082633370e-02,  -2.5478516399e+00, -9.4978063800e-01,
//        -4.4857147405e+00, -3.2178997190e+00, 1.7542588920e+00};
//
//    d.SetNewParameters(sys_xyz, do_grads, cutoff, box);
//
//    std::vector<double> getbox = d.GetBox();
//    std::vector<double> getsysxyz = d.GetSystemXyz();
//    std::vector<double> getinternalxyz = d.GetInternalXyz();
//    bool getdograds = d.GetDoGrads();
//    double getcutoff = d.GetCutoff();
//    std::vector<double> getboxabc = d.GetBoxAbc();
//    std::vector<double> getboxinverse = d.GetBoxInverse();
//
//    std::vector<double> box_ABCabc = box.size() ? BoxVecToBoxABCabc(box) : std::vector<double>{};
//    std::vector<double> box_inverse = box.size() ? InvertUnitCell(box) : std::vector<double>{};
//
//    REQUIRE(VectorsAreEqual(getbox, box, TOL));
//    REQUIRE(VectorsAreEqual(getboxabc, box_ABCabc, TOL));
//    REQUIRE(VectorsAreEqual(getboxinverse, box_inverse, TOL));
//    REQUIRE(VectorsAreEqual(getsysxyz, sys_xyz, TOL));
//    REQUIRE(VectorsAreEqual(getinternalxyz, internal_xyz_expected, TOL));
//    REQUIRE(getdograds == do_grads);
//    REQUIRE(getcutoff == Approx(cutoff).margin(TOL));
//    box[3] = 0.0;
//}
//
// TEST_CASE("dispersion::SetJsonDispersionRepulsion") {
//    // Declare and initialize dispersion
//    disp::Dispersion d;
//    d.Initialize(c6lr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);
//
//    nlohmann::json jsonDisp = {
//        {"pairs", nlohmann::json::array(
//                      {nlohmann::json::array({"mon_t", "test_mon"}), nlohmann::json::array({"mon_t", "mon_t"})})},
//        {"types1", nlohmann::json::array({{"a", "b"}, {"a", "b"}})},
//        {"types2", nlohmann::json::array({{"c"}, {"a", "b"}})},
//        {"c6",
//         nlohmann::json::array({nlohmann::json::array({{{"a", "c"}, 10.0}, {{"b", "c"}, 20.0}}),
//                                nlohmann::json::array({{{"a", "a"}, 15.0}, {{"a", "b"}, 25.0}, {{"b",
//                                "b"}, 50.0}})})},
//        {"d6",
//         nlohmann::json::array({nlohmann::json::array({{{"a", "c"}, 1.0}, {{"b", "c"}, 2.0}}),
//                                nlohmann::json::array({{{"a", "a"}, 1.5}, {{"a", "b"}, 2.5}, {{"b", "b"}, 5.0}})})},
//        {"a", nlohmann::json::array(
//                  {nlohmann::json::array({{{"a", "c"}, 100.0}, {{"b", "c"}, 200.0}}),
//                   nlohmann::json::array({{{"a", "a"}, 150.0}, {{"a", "b"}, 250.0}, {{"b", "b"}, 500.0}})})}};
//
//    d.SetJsonDispersionRepulsion(jsonDisp);
//    nlohmann::json getjsondisp = d.GetJsonDispersionRepulsion();
//
//    REQUIRE(jsonDisp == getjsondisp);
//}
//
// TEST_CASE("dispersion::SetJsonMonomers") {
//    // Declare and initialize dispersion
//    disp::Dispersion d;
//    d.Initialize(c6lr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);
//
//    nlohmann::json jsonMon = {
//        {"co2_archive",
//         {{"sites", 3},
//          {"nat", 3},
//          {"exc12", nlohmann::json::array({nlohmann::json::array({0, 1}), nlohmann::json::array({0, 2})})},
//          {"exc12", nlohmann::json::array({nlohmann::json::array({1, 2})})},
//          {"exc12", nlohmann::json::array()},
//          {"charges", nlohmann::json::array({0.706027, -0.3530135, -0.3530135})},
//          {"pol", nlohmann::json::array({1.471677, 0.769790, 0.769790})},
//          {"polfac", nlohmann::json::array({1.471677, 0.769790, 0.769790})},
//          {"c6lr",
//           nlohmann::json::array({17.91673320223304547491, 13.04205731316957524126, 13.04205731316957524126})}}}};
//
//    d.SetJsonMonomers(jsonMon);
//    nlohmann::json getjsonmon = d.GetJsonMonomers();
//
//    REQUIRE(jsonMon == getjsonmon);
//}
//
// TEST_CASE("dispersion::GetDispersion") {
//    // Declare and initialize dispersion
//    disp::Dispersion d;
//    d.Initialize(c6lr, sys_xyz, mon_id, num_atoms, mon_type_count, islocal, do_grads, box);
//
//    std::vector<double> grad = {
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
//        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};
//
//    std::vector<double> virial = {9.9409691176e+02,  7.4483337297e+01,  -7.2492383094e+02,
//                                  7.4483337297e+01,  1.9612208230e+03,  -9.5731672894e+02,
//                                  -7.2492383094e+02, -9.5731672894e+02, 1.5317301661e+03};
//
//    bool use_ghost = false;
//
//    double cutoff = 9.0;
//
//    std::vector<double> sys_xyz_new = {
//        -9.1278996460e-01, 1.3711133940e+00,  -2.9257875690e+00, -4.0252047300e-01, 1.9907698680e+00,
//        -3.4499780000e+00, -1.7848310380e+00, 1.3696631160e+00,  -3.3238997190e+00, -1.4297465600e+00,
//        -2.3744893640e-01, 1.1236431040e+00,  -7.5808708930e-01, 3.9350777550e-01,  1.4232346440e+00,
//        -2.2258007790e+00, -6.7917366630e-02, 1.6302588920e+00,  6.6630444410e-01,  -3.8357176030e-01,
//        1.1519802350e-01,  1.7838183644e+00,  -1.9222069500e-01, -1.7587628680e-01, -4.4811475090e-01,
//        -5.7997649630e-01, 4.1069507510e-01,  2.4803292099e+00,  7.5103875900e-01,  -2.9043390394e+00,
//        2.2674715176e+00,  1.8651909097e+00,  -2.6082983571e+00, 2.7020706245e+00,  -3.5351972400e-01,
//        -3.2106052081e+00, -1.4243133388e+00, 7.7214187430e-01,  -2.3329941557e+00, -2.4475497469e+00,
//        1.2692755042e+00,  -2.0709757555e+00, -4.0159255460e-01, 2.6460559090e-01,  -2.5998516399e+00,
//        6.4986549630e-01,  3.4030903031e+00,  -6.8029207450e-01, 7.9296435600e-02,  2.4412841935e+00,
//        -3.3312155960e-01, 1.2145816282e+00,  4.3686301334e+00,  -1.0197806380e+00, 5.1568113210e-01,
//        3.8303951459e+00,  -4.0222969886e+00, -3.7522694700e-01, 3.2992897240e+00,  -3.4756612500e+00,
//        1.3973748074e+00,  4.3616623515e+00,  -4.5737147405e+00};
//    sys_xyz = sys_xyz_new;
//
//    std::vector<double> grad_expected = {
//        1.8278849559e+00,  1.7907152559e+01,  -1.3323659446e+01, 2.4517885096e-01,  -8.1754366602e+00,
//        -2.9685140168e+00, -3.3193947268e+00, 8.8734780262e+00,  -2.0997813394e+01, -1.4748141848e+01,
//        4.3866642360e+00,  1.0796871686e+01,  -3.5638022460e+00, 6.0470351178e+00,  7.6124183085e+00,
//        -1.2697321973e+00, 2.6389106353e-01,  9.3102897948e-01,  3.2637397383e+00,  -2.0935110407e+00,
//        -8.0865828981e-01, 5.8540735679e-01,  -8.1443557560e-01, 1.0590198455e+00,  1.6162704520e+01,
//        -1.1094862140e+01, -1.5200236569e+01, 1.6292598604e+00,  -1.9553003302e-01, -7.8216602000e-01,
//        2.3040221187e+00,  -7.5674428308e-01, -3.8117359876e-01, 5.7576569384e-01,  -2.0546865258e-01,
//        -4.2932332816e-01, -3.4894210290e+00, -1.6271517147e+01, 1.9451989104e+01,  -1.0488320180e+01,
//        -1.2758163883e+00, 1.0473233244e+01,  7.3860579033e+00,  -1.9449873122e+01, 5.4977716987e+00,
//        1.7545895208e-01,  1.1626915132e+00,  1.6753610071e+00,  4.3945924951e-01,  2.2781437560e+00,
//        1.3249978459e+00,  2.2276764545e-01,  6.3319165347e-01,  9.7960554891e-01,  7.4738854637e-01,
//        2.8089783625e+00,  -1.8667881604e+00, 1.2144696243e+00,  1.5518693138e+01,  -2.5725949822e+00,
//        9.9222977682e-02,  4.5328116592e-01,  -4.7136001585e-01};
//
//    std::vector<double> virial_expected = {9.2835460731e+02,  8.3377540689e+01,  -7.0285578672e+02,
//                                           8.3377540689e+01,  1.8766570886e+03,  -9.3781287810e+02,
//                                           -7.0285578672e+02, -9.3781287810e+02, 1.4554322098e+03};
//
//    double energy_expected = -6.6842876953e+01;
//
//    d.SetNewParameters(sys_xyz, true, cutoff, box);
//    d.SetEwaldAlpha(0.60);
//    d.SetEwaldGridDensity(2.5);
//    d.SetEwaldSplineOrder(6);
//    double energy = d.GetDispersion(grad, &virial, use_ghost);
//
//    REQUIRE(energy == Approx(energy_expected).margin(TOL));
//    REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
//    REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));
//
//    double s = 0.001;
//    for (size_t j = 0; j < sys_xyz.size(); j++) {
//        sys_xyz[j] += s;
//        d.SetNewParameters(sys_xyz, false, cutoff, box);
//        double ep = d.GetDispersion(grad);
//        sys_xyz[j] += s;
//        d.SetNewParameters(sys_xyz, false, cutoff, box);
//        double epp = d.GetDispersion(grad);
//        sys_xyz[j] -= 4 * s;
//        d.SetNewParameters(sys_xyz, false, cutoff, box);
//        double emm = d.GetDispersion(grad);
//        sys_xyz[j] += s;
//        d.SetNewParameters(sys_xyz, false, cutoff, box);
//        double em = d.GetDispersion(grad);
//        sys_xyz[j] += s;
//
//        double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
//        REQUIRE(numgrad == Approx(grad_expected[j]).margin(TOL));
//    }
//}
