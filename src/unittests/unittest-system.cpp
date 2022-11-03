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
#include "setup_co2_2_h2o_2.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <utility>

constexpr double TOL = 1E-6;

void CreateSystem(bblock::System &s, std::vector<std::string> mon_ids, std::vector<size_t> nats,
                  std::vector<size_t> first_index, std::vector<double> coords, std::vector<std::string> atom_names,
                  nlohmann::json j) {
    for (size_t i = 0; i < mon_ids.size(); i++) {
        size_t numat = nats[i];
        size_t fi = first_index[i];
        std::vector<double> xyz(coords.begin() + 3 * fi, coords.begin() + 3 * (fi + numat));
        std::vector<std::string> atoms(atom_names.begin() + fi, atom_names.begin() + fi + numat);
        s.AddMonomer(xyz, atoms, mon_ids[i]);
    }

    s.Initialize();
    s.SetUpFromJson(j);
}

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

    // Quick test to check one of the getters
    size_t nats_ch4 = s.GetMonNumAt(2);
    REQUIRE(nats_ch4 == 5);
}

TEST_CASE("system::getters") {
    SECTION("System Information") {
        // Set up system
        SETUP_CO2_2_H2O_2

        // Create system object
        bblock::System s;
        nlohmann::json j;
        CreateSystem(s, monomer_names, n_atoms_vector, first_index_realSites, real_coords, atom_names, j);

        // Start "getting"

        // Number of molecules
        size_t s_nummol = s.GetNumMol();
        size_t nummol = 0;
        REQUIRE(s_nummol == nummol);  // For now, no molecules

        // Number of Monomers
        size_t s_nummon = s.GetNumMon();
        REQUIRE(s_nummon == n_monomers);

        // Number of Sites
        size_t s_numsites = s.GetNumSites();
        REQUIRE(s_numsites == n_sites);

        // Number of atoms
        size_t s_numats = s.GetNumRealSites();
        REQUIRE(s_numats == n_atoms);

        // Vector of number of atoms
        std::vector<size_t> s_nats = s.GetMonNumAt();
        REQUIRE(VectorsAreEqual(s_nats, n_atoms_vector));

        // First index vector
        std::vector<size_t> s_fi_sites = s.GetFirstInd();
        REQUIRE(VectorsAreEqual(s_fi_sites, first_index));

        // First index of a given monomer
        for (size_t i = 0; i < s_fi_sites.size(); i++) {
            size_t s_fi_ind = s.GetFirstInd(i);
            REQUIRE(s_fi_ind == s_fi_sites[i]);
        }

        // First index real sites
        std::vector<size_t> s_fi_realsites = s.GetFirstIndRealSites();
        REQUIRE(VectorsAreEqual(s_fi_realsites, first_index_realSites));

        // Sites vector
        std::vector<size_t> s_sites = s.GetSitesVector();
        REQUIRE(VectorsAreEqual(s_sites, n_sites_vector));

        // Atoms vector
        std::vector<size_t> s_atoms = s.GetAtomsVector();
        REQUIRE(VectorsAreEqual(s_atoms, n_atoms_vector));

        // Charges
        std::vector<double> s_charges = s.GetCharges();
        REQUIRE(VectorsAreEqual(s_charges, charges, TOL));

        std::vector<double> s_realcharges = s.GetRealCharges();
        REQUIRE(VectorsAreEqual(s_realcharges, real_charges, TOL));

        // Polarizability
        std::vector<double> s_pols = s.GetPolarizabilities();
        REQUIRE(VectorsAreEqual(s_pols, pol, TOL));

        std::vector<double> s_realpols = s.GetRealPolarizabilities();
        REQUIRE(VectorsAreEqual(s_realpols, real_pol, TOL));

        // Polarizability Factor
        std::vector<double> s_polfac = s.GetPolarizabilityFactors();
        REQUIRE(VectorsAreEqual(s_polfac, polfac, TOL));

        std::vector<double> s_realpolfac = s.GetRealPolarizabilityFactors();
        REQUIRE(VectorsAreEqual(s_realpolfac, real_polfac, TOL));

        // C6 for long range
        std::vector<double> s_c6lr = s.GetRealC6lr();
        REQUIRE(VectorsAreEqual(s_c6lr, C6_long_range, TOL));

        // Get mon id of index
        for (size_t i = 0; i < monomer_names.size(); i++) {
            std::string s_id = s.GetMonId(i);
            REQUIRE(s_id == monomer_names[i]);
        }
    }
}
