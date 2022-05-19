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

#include "bblock/system.h"
#include "tools/math_tools.h"
#include "json/json.h"
#include "setup_h2o_2_ch4_1.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

void TestEnergy(bblock::System &s, TestEnergyData &testData) {
    SECTION("Total Energy") {
        double e_nograd = s.Energy(false);
        double e = s.Energy(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.total_energy).margin(TOL));
        REQUIRE(e == Approx(testData.total_energy).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial, TOL));
    }

    SECTION("One Body Polynomials") {
        double e_nograd = s.OneBodyEnergy(false);
        double e = s.OneBodyEnergy(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_1b).margin(TOL));
        REQUIRE(e == Approx(testData.energy_1b).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_1b, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_1b, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_1b, TOL));
    }

    SECTION("Two Body Polynomials") {
        double e_nograd = s.TwoBodyEnergy(false);
        double e = s.TwoBodyEnergy(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_2b).margin(TOL));
        REQUIRE(e == Approx(testData.energy_2b).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_2b, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_2b, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_2b, TOL));
    }

    SECTION("Three Body Polynomials") {
        double e_nograd = s.ThreeBodyEnergy(false);
        double e = s.ThreeBodyEnergy(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_3b).margin(TOL));
        REQUIRE(e == Approx(testData.energy_3b).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_3b, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_3b, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_3b, TOL));
    }

    SECTION("Dispersion") {
        double e_nograd = s.Dispersion(false);
        double e = s.Dispersion(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_disp).margin(TOL));
        REQUIRE(e == Approx(testData.energy_disp).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_disp, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_disp, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_disp, TOL));
    }

    SECTION("Buckingham") {
        double e_nograd = s.Buckingham(false);
        double e = s.Buckingham(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_buck).margin(TOL));
        REQUIRE(e == Approx(testData.energy_buck).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_buck, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_buck, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_buck, TOL));
    }

    SECTION("Lennard Jones") {
        double e_nograd = s.LennardJones(false);
        double e = s.LennardJones(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_lj).margin(TOL));
        REQUIRE(e == Approx(testData.energy_lj).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_lj, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_lj, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_lj, TOL));
    }

    SECTION("Classical One Body") {
        double e_nograd = s.ClassicPotential(false);
        double e = s.ClassicPotential(true);
        std::vector<double> g = s.GetGrads();
        std::vector<double> g_real = s.GetRealGrads();
        std::vector<double> v = s.GetVirial();

        REQUIRE(e_nograd == Approx(testData.energy_ff).margin(TOL));
        REQUIRE(e == Approx(testData.energy_ff).margin(TOL));
        REQUIRE(VectorsAreEqual(g, testData.grad_ff, TOL));
        REQUIRE(VectorsAreEqual(g_real, testData.real_grad_ff, TOL));
        REQUIRE(VectorsAreEqual(v, testData.virial_ff, TOL));
    }
}

TEST_CASE("system::setup") {
    SETUP_H2O_2_CH4_1

    // Set up system
    bblock::System s;
    for (size_t i = 0; i < testData.monomer_names.size(); i++) {
        size_t numat = testData.natoms[i];
        size_t fi = testData.real_first_index[i];
        std::vector<double> xyz(testData.real_coords.begin() + 3 * fi, testData.real_coords.begin() + 3 * (fi + numat));
        std::vector<std::string> atoms(testData.real_atom_names.begin() + fi,
                                       testData.real_atom_names.begin() + fi + numat);
        s.AddMonomer(xyz, atoms, testData.monomer_names[i]);
    }

    s.Initialize();
    nlohmann::json j(testData.json_mbx);
    s.SetUpFromJson(j);

    // Assert that coordinates, atoms, mon ids are properly set
    std::vector<double> xyz = s.GetXyz();
    std::vector<double> real_xyz = s.GetRealXyz();
    std::vector<std::string> atoms = s.GetAtomNames();
    std::vector<std::string> real_atoms = s.GetRealAtomNames();
    std::vector<std::string> mon_ids = s.GetMonId();

    // REQUIRE(VectorsAreEqual(xyz,coords,TOL));
    REQUIRE(VectorsAreEqual(real_xyz, testData.real_coords, TOL));
    REQUIRE(VectorsAreEqual(atoms, testData.atom_names));
    REQUIRE(VectorsAreEqual(real_atoms, testData.real_atom_names));
    REQUIRE(VectorsAreEqual(mon_ids, testData.monomer_names));
}

TEST_CASE("system::energy") {
    SETUP_H2O_2_CH4_1

    // Set up system
    bblock::System s;
    for (size_t i = 0; i < testData.monomer_names.size(); i++) {
        size_t numat = testData.natoms[i];
        size_t fi = testData.real_first_index[i];
        std::vector<double> xyz(testData.real_coords.begin() + 3 * fi, testData.real_coords.begin() + 3 * (fi + numat));
        std::vector<std::string> atoms(testData.real_atom_names.begin() + fi,
                                       testData.real_atom_names.begin() + fi + numat);
        s.AddMonomer(xyz, atoms, testData.monomer_names[i]);
    }

    s.Initialize();
    nlohmann::json j(testData.json_mbx);
    s.SetUpFromJson(j);

    TestEnergy(s, testData);
}
