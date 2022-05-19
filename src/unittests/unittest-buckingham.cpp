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

#include "potential/buckingham/buckingham.h"
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

std::vector<double> sys_xyz = {
    -1.5897242500e+00, 1.0433792200e+00, -8.7808400000e-02, -6.3591971000e-01, 9.7898520000e-01, 0.0000000000e+00,
    -1.9006628000e+00, 1.7450105000e+00, -6.6454990000e-01, 1.6492450700e+00,  1.0859465600e+00, 0.0000000000e+00,
    2.6087802600e+00,  1.0958770400e+00, -2.8171150000e-02, 1.3383065300e+00,  1.7875778400e+00, 5.7674150000e-01,
    -6.1315209000e-01, 2.4697633600e+00, 2.0700508600e+00,  3.4684791000e-01,  2.4697633600e+00, 2.0700508600e+00,
    -9.3360667000e-01, 3.3746991900e+00, 2.0700508600e+00,  -1.3248347200e+00, 5.3532195400e+00, 2.0794472700e+00};
std::vector<double> box = {2.1000000000e+01, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 2.1000000000e+01,
                           0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 2.0000000000e+01};
std::vector<size_t> num_atoms = {1, 3, 3, 3};
std::vector<size_t> islocal = {1, 1, 1, 1};
std::vector<std::pair<std::string, size_t> > mon_type_count = {{"f-", 1}, {"h2o", 3}};
std::vector<std::string> mon_id = {"f-", "h2o", "h2o", "h2o"};
std::vector<size_t> force_ttm_for_idx = {};
bool do_grads = true;

TEST_CASE("buckingham::Initialize") {
    std::vector<double> internal_xyz_expected = {
        -1.5897242500e+00, 1.0433792200e+00, -8.7808400000e-02, -6.3591971000e-01, 2.6087802600e+00,  3.4684791000e-01,
        9.7898520000e-01,  1.0958770400e+00, 2.4697633600e+00,  0.0000000000e+00,  -2.8171150000e-02, 2.0700508600e+00,
        -1.9006628000e+00, 1.3383065300e+00, -9.3360667000e-01, 1.7450105000e+00,  1.7875778400e+00,  3.3746991900e+00,
        -6.6454990000e-01, 5.7674150000e-01, 2.0700508600e+00,  1.6492450700e+00,  -6.1315209000e-01, -1.3248347200e+00,
        1.0859465600e+00,  2.4697633600e+00, 5.3532195400e+00,  0.0000000000e+00,  2.0700508600e+00,  2.0794472700e+00};

    // Declare and initialize dispersion
    buck::Buckingham d;
    d.Initialize(sys_xyz, mon_id, num_atoms, mon_type_count, force_ttm_for_idx, islocal, do_grads, box);

    std::vector<double> box_inverse = box.size() ? InvertUnitCell(box) : std::vector<double>{};

    // Get everything and ensure that is correct
    std::vector<size_t> getislocal = d.GetIsLocal();
    std::vector<std::string> getmonids = d.GetMonIds();
    std::vector<size_t> getnumatsvec = d.GetNumAtomsVector();
    std::vector<size_t> getenforcettm = d.GetEnforceTTMForIndex();
    std::vector<std::pair<std::string, size_t> > getmontypecount = d.GetMonTypeCount();
    bool getdograds = d.GetDoGrads();
    std::vector<double> getbox = d.GetBox();
    std::vector<double> getboxinverse = d.GetBoxInverse();
    std::vector<double> getsysxyz = d.GetSystemXyz();
    std::vector<double> getinternalxyz = d.GetInternalXyz();

    // Require that everything matches what is expected
    REQUIRE(getdograds == do_grads);
    REQUIRE(VectorsAreEqual(getislocal, islocal));
    REQUIRE(VectorsAreEqual(getmonids, mon_id));
    REQUIRE(VectorsAreEqual(getnumatsvec, num_atoms));
    REQUIRE(VectorsAreEqual(getenforcettm, force_ttm_for_idx));
    REQUIRE(VectorsAreEqual(getmontypecount, mon_type_count));
    REQUIRE(VectorsAreEqual(getbox, box, TOL));
    REQUIRE(VectorsAreEqual(getboxinverse, box_inverse, TOL));
    REQUIRE(VectorsAreEqual(getsysxyz, sys_xyz, TOL));
    REQUIRE(VectorsAreEqual(getinternalxyz, internal_xyz_expected, TOL));
}

TEST_CASE("buckingham::SetNewParameters") {
    // Declare and initialize dispersion
    buck::Buckingham d;
    d.Initialize(sys_xyz, mon_id, num_atoms, mon_type_count, force_ttm_for_idx, islocal, do_grads, box);

    box[3] = 10.0;
    do_grads = false;
    double cutoff = 10.0;
    for (size_t i = 0; i < sys_xyz.size(); i++) sys_xyz[i] += 0.002 * double(i);

    std::vector<double> box_inverse = box.size() ? InvertUnitCell(box) : std::vector<double>{};

    std::vector<double> internal_xyz_expected = {
        -1.5897242500e+00, 1.0453792200e+00, -8.3808400000e-02, -6.2991971000e-01, 2.6327802600e+00,  3.8884791000e-01,
        9.8698520000e-01,  1.1218770400e+00, 2.5137633600e+00,  1.0000000000e-02,  -1.7115000000e-04, 2.1160508600e+00,
        -1.8886628000e+00, 1.3683065300e+00, -8.8560667000e-01, 1.7590105000e+00,  1.8195778400e+00,  3.4246991900e+00,
        -6.4854990000e-01, 6.1074150000e-01, 2.1220508600e+00,  1.6672450700e+00,  -5.7715209000e-01, -1.2708347200e+00,
        1.1059465600e+00,  2.5077633600e+00, 5.4092195400e+00,  2.2000000000e-02,  2.1100508600e+00,  2.1374472700e+00};

    std::vector<std::pair<std::string, std::string> > buck_pairs = {{"f-", "h2o"}};

    d.SetNewParameters(sys_xyz, buck_pairs, force_ttm_for_idx, do_grads, cutoff, box);

    std::vector<double> getbox = d.GetBox();
    std::vector<double> getsysxyz = d.GetSystemXyz();
    std::vector<double> getinternalxyz = d.GetInternalXyz();
    std::vector<double> getboxinverse = d.GetBoxInverse();
    bool getdograds = d.GetDoGrads();
    double getcutoff = d.GetCutoff();
    std::vector<std::pair<std::string, std::string> > getbuckpairs = d.GetBuckPairs();

    REQUIRE(VectorsAreEqual(getbox, box, TOL));
    REQUIRE(VectorsAreEqual(getboxinverse, box_inverse, TOL));
    REQUIRE(VectorsAreEqual(getsysxyz, sys_xyz, TOL));
    REQUIRE(VectorsAreEqual(getinternalxyz, internal_xyz_expected, TOL));
    REQUIRE(getdograds == do_grads);
    REQUIRE(getcutoff == Approx(cutoff).margin(TOL));
    REQUIRE(getbuckpairs == buck_pairs);
    box[3] = 0.0;
}

TEST_CASE("buckingham::SetJsonDispersionRepulsion") {
    // Declare and initialize dispersion
    buck::Buckingham d;
    d.Initialize(sys_xyz, mon_id, num_atoms, mon_type_count, force_ttm_for_idx, islocal, do_grads, box);

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

    d.SetJsonDispersionRepulsion(jsonDisp);
    nlohmann::json getjsondisp = d.GetJsonDispersionRepulsion();

    REQUIRE(jsonDisp == getjsondisp);
}

TEST_CASE("buckingham::SetJsonMonomers") {
    // Declare and initialize dispersion
    buck::Buckingham d;
    d.Initialize(sys_xyz, mon_id, num_atoms, mon_type_count, force_ttm_for_idx, islocal, do_grads, box);

    nlohmann::json jsonMon = {
        {"co2",
         {{"sites", 3},
          {"nat", 3},
          {"exc12", nlohmann::json::array({nlohmann::json::array({0, 1}), nlohmann::json::array({0, 2})})},
          {"exc12", nlohmann::json::array({nlohmann::json::array({1, 2})})},
          {"exc12", nlohmann::json::array()},
          {"charges", nlohmann::json::array({0.706027, -0.3530135, -0.3530135})},
          {"pol", nlohmann::json::array({1.471677, 0.769790, 0.769790})},
          {"polfac", nlohmann::json::array({1.471677, 0.769790, 0.769790})},
          {"c6lr",
           nlohmann::json::array({17.91673320223304547491, 13.04205731316957524126, 13.04205731316957524126})}}}};

    d.SetJsonMonomers(jsonMon);
    nlohmann::json getjsonmon = d.GetJsonMonomers();

    REQUIRE(jsonMon == getjsonmon);
}

TEST_CASE("buckingham::GetRepulsion") {
    std::vector<double> tmp = {
        -1.3248347200e+00, 5.3532195400e+00, 2.0794472700e+00, -1.5897242500e+00, 1.0433792200e+00, -8.7808400000e-02,
        -6.3591971000e-01, 9.7898520000e-01, 0.0000000000e+00, -1.9006628000e+00, 1.7450105000e+00, -6.6454990000e-01,
        1.6492450700e+00,  1.0859465600e+00, 0.0000000000e+00, 2.6087802600e+00,  1.0958770400e+00, -2.8171150000e-02,
        1.3383065300e+00,  1.7875778400e+00, 5.7674150000e-01, -6.1315209000e-01, 2.4697633600e+00, 2.0700508600e+00,
        3.4684791000e-01,  2.4697633600e+00, 2.0700508600e+00, -9.3360667000e-01, 3.3746991900e+00, 2.0700508600e+00};

    sys_xyz = tmp;

    // Declare and initialize dispersion
    buck::Buckingham d;
    d.Initialize(sys_xyz, mon_id, num_atoms, mon_type_count, force_ttm_for_idx, islocal, do_grads, box);

    std::vector<double> grad = {
        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00,
        0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00, 0.0000000000e+00};

    std::vector<double> virial = {-1.8810228414e+01, 3.4747301698e+01,  -1.2968087978e+00,
                                  3.4747301698e+01,  -3.5309658403e+00, 9.8307910416e+00,
                                  -1.2968087978e+00, 9.8307910416e+00,  1.0191690656e+01};

    bool use_ghost = false;

    double cutoff = 9.0;

    std::vector<double> grad_expected = {
        2.6853299690e+00,  -1.2399801037e+01, -6.5512162650e-02, 2.1102428767e-04,  3.4334350003e-03, 1.7265445862e-03,
        -5.6432453320e-04, 3.5831528072e-03,  1.7033786359e-03,  1.2057121768e-03,  7.5551396796e-03, 5.7455878166e-03,
        -1.2900108813e-04, 1.8509350678e-04,  9.0196289099e-05,  -8.1858990327e-05, 8.8595798088e-05, 4.3859787176e-05,
        -3.8430806123e-03, 5.1454456228e-03,  2.1684991026e-03,  -7.3086886067e-01, 2.9611911887e+00, 9.6497275353e-03,
        -1.3483096394e-01, 2.3256757548e-01,  7.5787532582e-04,  -1.8164286156e+00, 9.1860514101e+00, 4.3626493572e-02};

    std::vector<double> virial_expected = {-1.7341970055e+01, 2.8645442630e+01, -1.3256346429e+00,
                                           2.8645442630e+01,  2.3930132729e+01, 9.9910439136e+00,
                                           -1.3256346429e+00, 9.9910439136e+00, 1.0218786823e+01};

    double energy_expected = 4.4300261242e+00;

    std::vector<std::pair<std::string, std::string> > buck_pairs = {{"f-", "h2o"}};

    d.SetNewParameters(sys_xyz, buck_pairs, force_ttm_for_idx, true, cutoff, box);
    double energy = d.GetRepulsion(grad, &virial, use_ghost);

    REQUIRE(energy == Approx(energy_expected).margin(TOL));
    REQUIRE(VectorsAreEqual(grad, grad_expected, TOL));
    REQUIRE(VectorsAreEqual(virial, virial_expected, TOL));

    double s = 0.001;
    for (size_t j = 0; j < sys_xyz.size(); j++) {
        sys_xyz[j] += s;
        d.SetNewParameters(sys_xyz, buck_pairs, force_ttm_for_idx, false, cutoff, box);
        double ep = d.GetRepulsion(grad);
        sys_xyz[j] += s;
        d.SetNewParameters(sys_xyz, buck_pairs, force_ttm_for_idx, false, cutoff, box);
        double epp = d.GetRepulsion(grad);
        sys_xyz[j] -= 4 * s;
        d.SetNewParameters(sys_xyz, buck_pairs, force_ttm_for_idx, false, cutoff, box);
        double emm = d.GetRepulsion(grad);
        sys_xyz[j] += s;
        d.SetNewParameters(sys_xyz, buck_pairs, force_ttm_for_idx, false, cutoff, box);
        double em = d.GetRepulsion(grad);
        sys_xyz[j] += s;

        double numgrad = (emm - 8 * em + 8 * ep - epp) / 12.0 / s;
        REQUIRE(numgrad == Approx(grad_expected[j]).margin(TOL));
    }
}
