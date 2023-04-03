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
#include "io_tools/read_nrg.h"
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

    // TODO giving segfault for unknown reasons
    //    SECTION("Energy without initialization") {
    //        bool failed = false;
    //        bblock::System s2;
    //        try {
    //            s2.Energy(true);
    //        } catch (...) {
    //            failed = true;
    //        }
    //
    //        REQUIRE(failed);
    //    }
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

TEST_CASE("External fields") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetUpFromJson();
    }

    // Calculate energies
    std::vector<double> en, eperm, eind;
    std::vector<size_t> natoms;
    std::cout << std::scientific << std::setprecision(8);
    for (size_t i = 0; i < systems.size(); i++) {
        en.push_back(systems[i].Energy(true));
        eperm.push_back(systems[i].GetPermanentElectrostaticEnergy());
        eind.push_back(systems[i].GetInducedElectrostaticEnergy());
        natoms.push_back(systems[i].GetNumSites());
    }

    std::vector<double> expected_energies = {3.72619635e+01, 9.83496483e+00, 3.12130764e+01};
    std::vector<double> expected_eperm = {3.80896378e+01, 3.14377321e-01, 3.12130764e+01};
    std::vector<double> expected_eind = {-1.03882612e+01, -3.99993938e-02, 0.00000000e+00};

    SECTION("Energy") {
        REQUIRE(VectorsAreEqual(en, expected_energies, TOL));
        REQUIRE(VectorsAreEqual(eperm, expected_eperm, TOL));
        REQUIRE(VectorsAreEqual(eind, expected_eind, TOL));
    }

    std::vector<double> expected_phi_all = {
        1.92086063e-01, 1.75606626e-01, 2.10970122e-01, 1.87491804e-01, 1.76912623e-01, 1.96175918e-01, 2.21515637e-01,
        2.28253375e-01, 2.12165661e-01, 3.15086220e-01, 3.07350459e-01, 3.29849643e-01, 3.17072544e-01, 3.11208324e-01,
        3.00865053e-01, 3.65992698e-01, 3.20003981e-01, 1.91539479e-01, 1.89139201e-01, 2.08100390e-01, 1.94503009e-01,
        2.43833904e-01, 2.31936933e-01, 2.25484218e-01, 2.67383327e-01, 2.51713404e-01, 4.71830526e-01, 3.91464675e-01,
        5.89193241e-01, 3.81201378e-01, 5.13269557e-01, 1.13216009e-01, 3.25631165e-02};
    std::vector<double> expected_efq_all = {
        -1.37133803e-02, 6.88385775e-03,  7.43760901e-03,  -1.18995250e-02, 4.82128043e-03,  6.14982966e-03,
        -1.56467233e-02, 8.78958861e-03,  8.85647303e-03,  -1.15993670e-02, -3.74873372e-04, 1.26834600e-02,
        -1.07303897e-02, 2.02605799e-03,  1.07783330e-02,  -1.28167703e-02, -2.75410796e-03, 1.26045497e-02,
        -1.89320655e-02, 2.15592912e-03,  6.29388292e-03,  -2.13259236e-02, 3.91393606e-04,  5.48813630e-03,
        -1.74067585e-02, 4.84219318e-03,  8.59698228e-03,  -1.43202795e-02, 1.89643167e-02,  -1.91363122e-02,
        -1.75321981e-02, 1.61950998e-02,  -2.60796831e-02, -1.95370756e-02, 1.52976404e-02,  -1.36070239e-02,
        -1.60971309e-02, 1.73408747e-02,  -1.96421412e-02, -1.96503378e-02, 3.25016544e-02,  -3.30839394e-02,
        -2.29991267e-02, 3.46863313e-02,  -2.47454877e-02, -2.78561860e-02, 3.99882307e-02,  -5.07196928e-02,
        -2.21676714e-02, 3.49760457e-02,  -3.41572140e-02, -1.20146568e-02, 8.19186772e-03,  -6.43107762e-03,
        -1.24593682e-02, 9.41123028e-03,  -5.14349318e-03, -1.32754844e-02, 9.13115979e-03,  -8.18355743e-03,
        -1.23534647e-02, 8.63406307e-03,  -6.66171822e-03, -1.12010772e-02, 1.06527985e-03,  2.25487061e-02,
        -7.48836137e-03, -6.51170995e-04, 2.20220307e-02,  -1.04367358e-02, 3.35481786e-03,  1.86824478e-02,
        -1.55226838e-02, 1.16691104e-03,  2.79223678e-02,  -1.15668308e-02, 4.21360266e-04,  2.21157025e-02,
        -1.20245115e-01, 8.92371445e-02,  -5.23689952e-03, -9.51021339e-02, 3.20174056e-02,  -1.37041201e-02,
        -1.61358656e-01, 1.75802981e-01,  -9.65272664e-02, -5.49166141e-02, 6.08924669e-02,  -6.32854479e-03,
        -1.39190049e-01, 8.82713484e-02,  7.04642129e-02,  1.16586399e-02,  4.35696076e-03,  8.41427905e-03,
        -8.88900214e-03, -5.78917961e-03, -8.28587391e-04};
    std::vector<double> expected_efd_all = {
        -1.53844645e-02, 7.73773415e-03,  8.32853972e-03,  -8.61850577e-03, 3.64804520e-03,  4.50638473e-03,
        -1.11416109e-02, 6.34869139e-03,  6.33429553e-03,  -1.29302345e-02, -4.28711448e-04, 1.44017104e-02,
        -7.88625535e-03, 1.24309972e-03,  7.94597587e-03,  -9.30892204e-03, -1.96218294e-03, 9.19008641e-03,
        -2.11573359e-02, 2.44893356e-03,  7.09400795e-03,  -1.53241447e-02, 2.46695383e-04,  3.84113276e-03,
        -1.28589991e-02, 3.32936462e-03,  6.09816040e-03,  -1.65239032e-02, 2.50541157e-02,  -2.29059648e-02,
        -1.00296982e-02, 9.14364245e-03,  -1.20392156e-02, -9.09721687e-03, 1.05102214e-02,  -5.84061761e-03,
        -0.00000000e+00, 0.00000000e+00,  -0.00000000e+00, -2.39140523e-02, 4.12419402e-02,  -4.08018996e-02,
        -1.14542099e-02, 1.86884471e-02,  -1.72144460e-02, -8.27177983e-03, 1.56185311e-02,  -1.90517098e-02,
        -0.00000000e+00, 0.00000000e+00,  -0.00000000e+00, -1.52100652e-02, 1.02849806e-02,  -8.09628799e-03,
        -7.08929559e-03, 4.99697259e-03,  -3.63569864e-03, -5.45983246e-03, 2.63121885e-03,  -3.51221709e-03,
        -0.00000000e+00, 0.00000000e+00,  -0.00000000e+00, -1.26619305e-02, 1.32723149e-03,  2.55137371e-02,
        -4.17313645e-03, -4.45824069e-05, 1.09215606e-02,  -5.12064419e-03, 1.34171968e-03,  9.63029434e-03,
        -7.08959831e-03, 7.55287668e-04,  1.31869946e-02,  -5.75447738e-03, 2.53129521e-04,  1.10163475e-02,
        -1.40984917e-01, 1.02983177e-01,  -4.15101337e-03, -5.03083352e-02, 2.27931938e-02,  -5.18188477e-03,
        -7.59044658e-02, 7.81124382e-02,  -3.87172227e-02, -3.53438012e-02, 3.23124508e-02,  -2.71624595e-03,
        -6.80999255e-02, 4.39931577e-02,  2.81304325e-02,  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
        0.00000000e+00,  -0.00000000e+00, 0.00000000e+00};

    // Get electrostati fields from the full system
    std::vector<double> phi, efq, efd;
    systems[0].GetElectrostaticFields(phi, efq, efd);

    SECTION("Electrostatic potential and fields") {
        REQUIRE(VectorsAreEqual(phi, expected_phi_all, TOL));
        REQUIRE(VectorsAreEqual(efq, expected_efq_all, TOL));
        REQUIRE(VectorsAreEqual(efd, expected_efd_all, TOL));
    }

    // Calculate fields from point charges to the atomic sites
    std::vector<double> xyz_real = systems[1].GetXyz();

    std::vector<double> efx, phix, efxd, phixd;
    systems[2].Hack3GetPotentialAtPoints(xyz_real);
    systems[2].GetPhiXAndEfX(phix, efx, phixd, efxd);

    std::vector<double> expected_efx = {
        -1.16523750e-02, 6.89012917e-03,  6.44783932e-03,  -1.01147479e-02, 5.83058942e-03, 5.50964766e-03,
        -1.37666426e-02, 8.10291219e-03,  7.62140855e-03,  -1.09172357e-02, 1.14476903e-03, 9.46947806e-03,
        -9.63826449e-03, 1.59736013e-03,  8.33331160e-03,  -1.24718571e-02, 4.06441469e-04, 1.08406620e-02,
        -1.63590585e-02, 6.21880068e-03,  5.51564866e-03,  -1.93538070e-02, 6.30181111e-03, 5.65323901e-03,
        -1.40876562e-02, 5.89413151e-03,  5.25550854e-03,  -1.51868011e-02, 1.61468420e-02, -1.54520412e-02,
        -1.69213986e-02, 1.08857209e-02,  -2.09103593e-02, -2.00200864e-02, 1.38371556e-02, -1.02175014e-02,
        -1.65804168e-02, 1.44414327e-02,  -1.56653911e-02, -2.07131342e-02, 2.65654730e-02, -3.20971110e-02,
        -2.29439911e-02, 3.03595601e-02,  -2.44734756e-02, -3.12112238e-02, 3.36388402e-02, -5.08632065e-02,
        -2.33284408e-02, 2.91385841e-02,  -3.35821747e-02, -1.35636121e-02, 8.00201658e-03, -3.05546415e-03,
        -1.30559395e-02, 7.51512229e-03,  -1.57033230e-03, -1.58607359e-02, 9.55784020e-03, -3.17555509e-03,
        -1.39101186e-02, 8.18602818e-03,  -2.73454834e-03, -1.13149582e-02, 8.27086958e-04, 2.12646586e-02,
        -7.77209041e-03, -5.31542593e-04, 2.08317157e-02,  -1.06542530e-02, 2.81932672e-03, 1.78681755e-02,
        -1.52232388e-02, 8.83382580e-04,  2.62335176e-02,  -1.19577635e-02, 3.16838561e-04, 2.05994351e-02,
        -1.22023468e-01, 8.88455856e-02,  -2.07246311e-03, -9.58875482e-02, 3.23429593e-02, -1.19608574e-02,
        -1.63169362e-01, 1.73108927e-01,  -9.37474750e-02, -5.94940863e-02, 6.10888146e-02, -1.01353013e-03,
        -1.39642772e-01, 8.89007611e-02,  7.33389213e-02};
    std::vector<double> expected_phix = {
        1.84244108e-01, 1.69756393e-01, 2.01152222e-01, 1.75760807e-01, 1.66531020e-01, 1.85501513e-01, 1.98732438e-01,
        2.08902553e-01, 1.88969875e-01, 3.17578039e-01, 3.14490582e-01, 3.29764289e-01, 3.19890507e-01, 2.98352282e-01,
        2.90746900e-01, 3.50951833e-01, 3.07161176e-01, 1.87850726e-01, 1.83025944e-01, 2.03464596e-01, 1.89982628e-01,
        2.36085772e-01, 2.25097755e-01, 2.17900983e-01, 2.59092305e-01, 2.43210846e-01, 4.79557101e-01, 3.98317486e-01,
        5.99783020e-01, 3.89142929e-01, 5.18481471e-01};
    std::vector<double> expected_efxd(expected_efx.size(), 0.0);
    std::vector<double> expected_phixd(expected_phix.size(), 0.0);

    SECTION("Potential and fields at arbitrary points") {
        REQUIRE(VectorsAreEqual(efx, expected_efx, TOL));
        REQUIRE(VectorsAreEqual(phix, expected_phix, TOL));
        REQUIRE(VectorsAreEqual(efxd, expected_efxd, TOL));
        REQUIRE(VectorsAreEqual(phixd, expected_phixd, TOL));
    }

    // In the system of real atoms, set the fields coming from the point charges
    // Derivatives can be set to 0. Not implemented properly.
    std::vector<double> defx(9 * phix.size(), 0.0);
    std::vector<double> dmui(9 * phix.size(), 0.0);
    systems[1].SetExternalElectrostaticPotentialAndFieldInSites(phix, efx, defx, dmui);

    // Calculate energies
    double enn = systems[1].Energy(true);
    double ep = systems[1].GetPermanentElectrostaticEnergy();
    double ei = systems[1].GetInducedElectrostaticEnergy();
    double epext = systems[1].GetPermanentElectrostaticEnergyExternalFieldContribution();

    // Get the fields
    std::vector<double> phi_1, efq_1, efd_1;
    systems[1].GetElectrostaticFields(phi_1, efq_1, efd_1);

    // Get potential to positions of point charges from the atoms
    std::vector<double> xyz_fake = systems[2].GetXyz();
    std::vector<double> efx2, phix2, efx2d, phix2d, defx2;
    systems[1].Hack3GetPotentialAtPoints(xyz_fake);
    systems[1].GetPhiXAndEfX(phix2, efx2, phix2d, efx2d);

    // Set fields in system 2
    systems[2].SetExternalElectrostaticPotentialAndFieldInSites(phix2, efx2, defx2);

    // Get energies
    double enn2 = systems[2].Energy(true);
    double ep2 = systems[2].GetPermanentElectrostaticEnergy();
    double ei2 = systems[2].GetInducedElectrostaticEnergy();

    // Assert energies
    SECTION("Energy of the system combination") {
        REQUIRE(enn + enn2 == Approx(en[0]).margin(TOL));
        REQUIRE(ep + ep2 == Approx(eperm[0]).margin(TOL));
        REQUIRE(ei + ei2 == Approx(eind[0]).margin(TOL));
    }

    SECTION("Assertions") {
        bool failed = false;
        try {
            std::vector<double> phi_bad;
            systems[1].SetExternalElectrostaticPotentialAndFieldInSites(phi_bad, efx2, defx2);
        } catch (...) {
            failed = true;
        }
        REQUIRE(failed);
    }
}

TEST_CASE("External charges") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetUpFromJson();
    }

    // Calculate reference energy and gradients
    double expected_energy = systems[0].Energy(true);
    std::vector<double> expected_grads = systems[0].GetRealGrads();

    // Calculate the energy contribution and the gradients from the 2 point charges
    double energy_point_charges = systems[2].Energy(true);
    std::vector<double> gradients_point_charges = systems[2].GetRealGrads();

    // Get Charge information from system[2] (the point charges)
    std::vector<double> external_chg_xyz = systems[2].GetXyz();
    std::vector<double> external_chg_charge = systems[2].GetCharges();

    // Set external charges in system 1 (the real atoms)
    systems[1].SetExternalChargesAndPositions(external_chg_charge, external_chg_xyz);

    // Calculate energy and gradients of system 1 and the external charges
    double en1 = systems[1].Energy(true);
    std::vector<double> grads = systems[1].GetRealGrads();
    std::vector<double> grads_external_charges = systems[1].GetExternalChargesGradients();

    // Check that it adds up together
    SECTION("Energy") { REQUIRE(en1 == Approx(expected_energy).margin(TOL)); }

    SECTION("Gradients") {
        size_t n1 = systems[1].GetNumRealSites();
        size_t n2 = systems[2].GetNumRealSites();

        for (size_t i = 0; i < 3 * n1; i++) {
            REQUIRE(grads[i] == Approx(expected_grads[i]).margin(TOL));
        }

        for (size_t i = 0; i < 3 * n2; i++) {
            REQUIRE(grads_external_charges[i] == Approx(expected_grads[3 * n1 + i]).margin(TOL));
        }
    }

    // Check the getters
    SECTION("Getters") {
        std::vector<double> chg_get = systems[1].GetExternalCharges();
        std::vector<double> xyz_get = systems[1].GetExternalChargesPositions();

        REQUIRE(VectorsAreEqual(chg_get, external_chg_charge, TOL));
        REQUIRE(VectorsAreEqual(xyz_get, external_chg_xyz, TOL));
    }

    // Make sure setter works for the local+tag version
    std::vector<size_t> islocal(systems[1].GetNumMon(), 1);
    std::vector<int> tag(systems[1].GetNumRealSites());

    for (size_t i = 0; i < tag.size(); i++) tag[i] = i;

    systems[1].SetExternalChargesAndPositions(external_chg_charge, external_chg_xyz, islocal, tag);
    double en1b = systems[1].Energy(true);
    REQUIRE(en1b == Approx(expected_energy).margin(TOL));
}

// Helper function for TEST_CASE("Pair List"), sorts dimers, trimers
void sortEdge(std::vector<size_t> &nmers, size_t n) {
    // convert iterators to pointers
    size_t *begin = &(*(nmers.begin()));
    size_t *end = &(*(nmers.end()));

    // Sort within nmers
    for (size_t *i = begin; i < end; i += n) std::sort(i, i + n);

    // Sort across nmers
    if (n == 2) {
        typedef std::array<size_t, 2> Edge;
        Edge *edge_begin = reinterpret_cast<Edge *>(begin);
        Edge *edge_end = reinterpret_cast<Edge *>(end);
        std::sort(edge_begin, edge_end);
    } else if (n == 3) {
        typedef std::array<size_t, 3> Edge;
        Edge *edge_begin = reinterpret_cast<Edge *>(begin);
        Edge *edge_end = reinterpret_cast<Edge *>(end);
        std::sort(edge_begin, edge_end);
    }
}

TEST_CASE("Pair List") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    for (size_t i = 0; i < systems.size(); i++) {
        systems[i].SetUpFromJson();
    }

    std::vector<size_t> dimers = systems[0].GetPairList(2, 10.0, 0, systems[0].GetNumMon());
    std::vector<size_t> trimers = systems[0].GetPairList(3, 10.0, 0, systems[0].GetNumMon());

    std::vector<size_t> expected_d = {6, 7, 6, 8, 6, 0, 6, 1, 6, 2, 6, 3, 6, 4, 7, 9, 7, 0, 7, 1, 7,
                                      2, 7, 3, 7, 5, 8, 3, 8, 4, 9, 1, 9, 2, 9, 3, 9, 5, 0, 1, 0, 2,
                                      0, 3, 0, 4, 0, 5, 1, 2, 1, 5, 2, 3, 2, 5, 3, 4, 3, 5, 4, 5};
    std::vector<size_t> expected_t = {
        6, 7, 8, 6, 7, 0, 6, 7, 1, 6, 7, 2, 6, 7, 3, 6, 7, 4, 6, 7, 9, 6, 7, 5, 6, 8, 0, 6, 8, 1, 6, 8, 2,
        6, 8, 3, 6, 8, 4, 6, 0, 1, 6, 0, 2, 6, 0, 3, 6, 0, 4, 6, 0, 5, 6, 1, 2, 6, 1, 3, 6, 1, 4, 6, 9, 1,
        6, 1, 5, 6, 2, 3, 6, 2, 4, 6, 9, 2, 6, 2, 5, 6, 3, 4, 6, 9, 3, 6, 3, 5, 6, 4, 5, 7, 9, 0, 7, 9, 1,
        7, 9, 2, 7, 9, 3, 7, 9, 5, 7, 0, 1, 7, 0, 2, 7, 0, 3, 7, 0, 5, 7, 0, 4, 7, 1, 2, 7, 1, 3, 7, 1, 5,
        7, 2, 3, 7, 2, 5, 7, 3, 5, 7, 8, 3, 7, 3, 4, 7, 4, 5, 8, 3, 4, 8, 9, 3, 8, 0, 3, 8, 2, 3, 8, 3, 5,
        8, 0, 4, 8, 4, 5, 9, 1, 2, 9, 1, 3, 9, 1, 5, 9, 0, 1, 9, 2, 3, 9, 2, 5, 9, 0, 2, 9, 3, 5, 9, 0, 3,
        9, 3, 4, 9, 0, 5, 9, 4, 5, 0, 1, 2, 0, 1, 3, 0, 1, 4, 0, 1, 5, 0, 2, 3, 0, 2, 4, 0, 2, 5, 0, 3, 4,
        0, 3, 5, 0, 4, 5, 1, 2, 5, 1, 2, 3, 1, 3, 5, 1, 4, 5, 2, 3, 5, 2, 3, 4, 2, 4, 5, 3, 4, 5};

    std::vector<size_t> dimers2 = systems[0].GetPairList(2, 5.0, 0, systems[0].GetNumMon());
    std::vector<size_t> trimers2 = systems[0].GetPairList(3, 5.0, 0, systems[0].GetNumMon());

    std::vector<size_t> expected_d2 = {7, 9, 8, 4, 1, 2, 2, 5};
    std::vector<size_t> expected_t2 = {1, 2, 5};

    // sort dimers and trimers before comparing
    REQUIRE(dimers.size() == expected_d.size());
    sortEdge(dimers, 2);
    sortEdge(expected_d, 2);

    REQUIRE(trimers.size() == expected_t.size());
    sortEdge(trimers, 3);
    sortEdge(expected_t, 3);

    REQUIRE(dimers2.size() == expected_d2.size());
    sortEdge(dimers2, 2);
    sortEdge(expected_d2, 2);

    REQUIRE(trimers2.size() == expected_t2.size());
    sortEdge(trimers2, 3);
    sortEdge(expected_t2, 3);

    SECTION("Expected behavior") {
        REQUIRE(VectorsAreEqual(dimers, expected_d));
        REQUIRE(VectorsAreEqual(trimers, expected_t));
        REQUIRE(VectorsAreEqual(dimers2, expected_d2));
        REQUIRE(VectorsAreEqual(trimers2, expected_t2));
    }

    SECTION("Assertions") {
        bool failed = false;
        try {
            std::vector<size_t> x = systems[0].GetPairList(4, 10.0, 0, systems[0].GetNumMon());
        } catch (...) {
            failed = true;
        }
        REQUIRE(failed);
    }
}

TEST_CASE("JSON: mbx.json") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    char jj1[] = "unittests_inputs/crazy.json";
    systems[0].SetUpFromJson(jj1);
    char jj2[] = "unittests_inputs/mbx.json";
    systems[1].SetUpFromJson(jj2);
    char jj3[] = "unittests_inputs/mbx_pbc.json";
    systems[2].SetUpFromJson(jj3);

    std::vector<double> boxAbc = systems[0].GetBoxABCabc();
    std::vector<double> box = systems[0].GetBox();
    std::vector<double> box_reconverted = BoxVecToBoxABCabc(box);

    std::vector<double> expected_boxAbc = {20.0, 33.912, 25.213, 90.0, 120.0, 92.5};

    REQUIRE(VectorsAreEqual(boxAbc, expected_boxAbc, TOL));
    REQUIRE(VectorsAreEqual(box_reconverted, expected_boxAbc, TOL));

    size_t maxeval1b = systems[0].GetMaxEval1b();
    size_t maxeval2b = systems[0].GetMaxEval2b();
    size_t maxeval3b = systems[0].GetMaxEval3b();

    size_t expected_maxeval1b = 600;
    size_t expected_maxeval2b = 700;
    size_t expected_maxeval3b = 800;

    REQUIRE(maxeval1b == expected_maxeval1b);
    REQUIRE(maxeval2b == expected_maxeval2b);
    REQUIRE(maxeval3b == expected_maxeval3b);

    double diptol = systems[0].GetDipoleTolerance();
    std::string dipmethod = systems[0].GetDipoleMethod();
    size_t maxiter = systems[0].GetMaxIterationsDipoles();

    double expected_diptol = 10.0;
    std::string expected_dipmethod = "iter";
    size_t expected_maxiter = 102;

    REQUIRE(diptol == Approx(expected_diptol).margin(TOL));
    REQUIRE(dipmethod == expected_dipmethod);
    REQUIRE(maxiter == expected_maxiter);

    double alpha_e, grid_dens_e;
    size_t spline_e;
    systems[0].GetEwaldParamsElectrostatics(alpha_e, grid_dens_e, spline_e);

    double expected_alpha_e = 0.61;
    double expected_grid_dens_e = 2.6;
    size_t expected_spline_e = 5;

    REQUIRE(alpha_e == Approx(expected_alpha_e).margin(TOL));
    REQUIRE(grid_dens_e == Approx(expected_grid_dens_e).margin(TOL));
    REQUIRE(spline_e == expected_spline_e);

    double alpha_d, grid_dens_d;
    size_t spline_d;
    systems[0].GetEwaldParamsDispersion(alpha_d, grid_dens_d, spline_d);

    double expected_alpha_d = 0.59;
    double expected_grid_dens_d = 2.4;
    size_t expected_spline_d = 7;

    REQUIRE(alpha_d == Approx(expected_alpha_d).margin(TOL));
    REQUIRE(grid_dens_d == Approx(expected_grid_dens_d).margin(TOL));
    REQUIRE(spline_d == expected_spline_d);

    double alpha_lj, grid_dens_lj;
    size_t spline_lj;
    systems[0].GetEwaldParamsLennardJones(alpha_lj, grid_dens_lj, spline_lj);

    double expected_alpha_lj = 0.56;
    double expected_grid_dens_lj = 2.9;
    size_t expected_spline_lj = 8;

    REQUIRE(alpha_lj == Approx(expected_alpha_lj).margin(TOL));
    REQUIRE(grid_dens_lj == Approx(expected_grid_dens_lj).margin(TOL));
    REQUIRE(spline_lj == expected_spline_lj);

    std::vector<int> grid_e = systems[0].GetFFTDimensionElectrostatics(0);
    std::vector<int> grid_d = systems[0].GetFFTDimensionDispersion(0);
    std::vector<int> grid_lj = systems[0].GetFFTDimensionLennardJones(0);

    // std::vector<int> expected_grid_e = {300,500,700};
    // std::vector<int> expected_grid_d = {304,504,704};
    // std::vector<int> expected_grid_lj = {302,502,702};

    // REQUIRE(VectorsAreEqual(grid_e,expected_grid_e));
    // REQUIRE(VectorsAreEqual(grid_d,expected_grid_d));
    // REQUIRE(VectorsAreEqual(grid_lj,expected_grid_lj));

    SECTION("Assertions") {
        std::vector<int> grid_bad = {101, 101, 101};
        bool failed = false;
        try {
            systems[0].SetFFTDimensionElectrostatics(grid_bad);
        } catch (...) {
            failed = true;
        }
        REQUIRE(failed);
    }
}

TEST_CASE("Dipoles") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    char jj4[] = "unittests_inputs/mbx.json";
    systems[1].SetUpFromJson(jj4);

    // Calculate energy to fill dipole vectors
    systems[1].Energy(true);

    std::vector<double> mu_p, mu_i;
    systems[1].GetDipoles(mu_p, mu_i);

    std::vector<double> expected_mu_p = {
        -1.3036793844e+00, 1.6686615298e+00,  8.0603217182e-01,  9.4390313242e-01,  -8.3815537466e-01,
        -4.7834317663e-01, 3.5977625198e-01,  -8.3050615513e-01, -3.2768874383e-01, -2.3501780822e+00,
        -1.5768899996e+00, 1.5449115865e+00,  1.2350483144e+00,  5.3511028956e-01,  -9.2482482763e-01,
        1.1151295165e+00,  1.0417794587e+00,  -6.2008675885e-01, -2.4771154098e+00, -6.7818588671e-01,
        -4.7127336725e-01, 1.2349414817e+00,  5.9952888436e-01,  3.8778702525e-01,  1.2421741795e+00,
        7.8656750981e-02,  8.3486090633e-02,  0.0000000000e+00,  -0.0000000000e+00, -0.0000000000e+00,
        1.5992322072e+00,  3.3125342511e-01,  -2.8882750544e+00, 1.2333688886e+00,  -1.4384872657e-02,
        -2.1730874255e+00, -2.6084619498e+00, -9.2041297924e-02, 5.2846796239e+00,  0.0000000000e+00,
        0.0000000000e+00,  -0.0000000000e+00, 2.3637103997e+00,  2.8925566082e+00,  -2.4565071588e+00,
        2.8798575174e+00,  2.1896743834e+00,  -2.5225570485e+00, -5.1790574682e+00, -5.0708091961e+00,
        5.3616398085e+00,  -0.0000000000e+00, 0.0000000000e+00,  -0.0000000000e+00, -2.9293657824e+00,
        7.2366467693e-01,  -2.4275496091e+00, -2.4286938594e+00, 8.9750797306e-02,  -2.7626534026e+00,
        5.4581795141e+00,  -9.2379680449e-01, 5.5485994973e+00,  -1.3248120255e+00, -2.3929660107e-01,
        -2.0458395550e+00, 4.1021853228e-01,  -2.4845718923e-03, 6.1727682678e-01,  2.2968872803e-01,
        1.4627211822e-01,  5.7099671032e-01,  4.1870550025e-01,  1.3936168805e-01,  4.2578517698e-01,
        2.6619953421e-01,  -4.3852633309e-02, 4.3178111019e-01,  4.3370368116e-01,  2.4582512912e+00,
        2.2071577871e+00,  -2.3422391662e-01, -6.7391383668e-01, -5.9623424110e-01, -2.7298918938e-03,
        -6.4754153259e-01, -6.4699366920e-01, -1.2319318802e-01, -4.6950855277e-01, -5.4377386482e-01,
        -7.3556415334e-02, -6.6728696520e-01, -4.2015614663e-01};
    std::vector<double> expected_mu_i = {
        -1.9469476438e-03, -2.8719499685e-05, 8.0366588203e-04,  -1.0251549859e-03, -7.5537023792e-04,
        2.3925223867e-04,  -1.1008860460e-03, 4.2764106243e-04,  6.7291407511e-04,  -4.8560114268e-04,
        -1.4090334552e-03, 3.3817961177e-03,  -5.6027388312e-04, 5.4442966426e-04,  8.8756074396e-04,
        -7.6996026387e-05, -1.7221369922e-03, 5.9307778158e-04,  -2.1104103430e-03, -3.8441286674e-03,
        2.7826438637e-04,  -7.4691364088e-04, -3.2369327533e-03, -2.5522094983e-04, -1.7174901995e-03,
        -3.8509444461e-04, 1.8695459575e-03,  2.0116406138e-03,  3.3448091425e-03,  -3.9143346972e-03,
        -8.3045048905e-05, 1.4041291151e-03,  -8.4197434799e-04, -2.3366898358e-04, -3.0518208655e-04,
        -6.9735663071e-04, 0.0000000000e+00,  0.0000000000e+00,  -0.0000000000e+00, 1.3828617183e-03,
        7.5500790767e-03,  -1.6143602243e-03, -5.2056764218e-04, -2.0272846892e-05, 5.9691949625e-04,
        3.4662014960e-04,  7.2748564482e-04,  1.1099312304e-05,  0.0000000000e+00,  0.0000000000e+00,
        -0.0000000000e+00, 1.9420655047e-03,  -1.9407514381e-04, -3.0910616730e-03, -6.3902036538e-05,
        3.5453931337e-04,  -7.1828851507e-04, 4.1304276296e-04,  -1.6922997550e-04, -6.7781404708e-04,
        0.0000000000e+00,  0.0000000000e+00,  -0.0000000000e+00, 3.2358416768e-05,  2.5715947089e-04,
        9.8805479063e-04,  1.4223996397e-04,  -1.2699392794e-04, 3.6744212326e-04,  1.2813755500e-05,
        1.9444003551e-04,  1.5593804173e-04,  -1.6833538389e-04, 3.5301018812e-05,  5.0040930312e-04,
        1.6087088715e-04,  5.0357451340e-05,  4.3831206102e-04,  1.4990466267e-03,  4.9041156273e-04,
        -2.6700837485e-03, 9.7360735844e-05,  -1.3090661884e-04, -2.0164785053e-04, 6.7498950243e-04,
        8.1182359087e-04,  -8.2435626499e-04, 1.5506822124e-03,  -1.0111158980e-04, -1.5760343965e-03,
        -1.4849640320e-04, -2.0797837283e-04, -8.5475489360e-04};

    SECTION("Atomic Dipoles") {
        REQUIRE(VectorsAreEqual(mu_p, expected_mu_p, TOL));
        REQUIRE(VectorsAreEqual(mu_i, expected_mu_i, TOL));
    }

    std::vector<double> m_mu_p, m_mu_i;
    systems[1].GetMolecularDipoles(m_mu_p, m_mu_i);

    std::vector<double> expected_m_mu_p = {
        -1.11022302e-16, 0.00000000e+00,  2.51371000e-07,  -2.51371000e-07, -2.51371000e-07, -1.11022302e-16,
        2.51371000e-07,  -2.51371000e-07, -2.51371000e-07, 2.24139146e-01,  2.24827255e-01,  2.23317144e-01,
        6.45104489e-02,  1.14217955e-02,  3.82575601e-01,  1.00119872e-01,  -1.10381330e-01, 3.58396486e-01,
        2.69286500e-07,  2.77555756e-17,  2.69286500e-07,  2.69286500e-07,  4.03929750e-07,  -1.34643250e-07};
    std::vector<double> expected_m_mu_i = {
        -4.07298868e-03, -3.56448675e-04, 1.71583220e-03,  -1.12287105e-03, -2.58674078e-03, 4.86243464e-03,
        -4.57481418e-03, -7.46615587e-03, 1.89258939e-03,  1.69492658e-03,  4.44375617e-03,  -5.45366568e-03,
        1.20891423e-03,  8.25729187e-03,  -1.00634142e-03, 2.29120623e-03,  -8.76580595e-06, -4.48716424e-03,
        1.79947640e-04,  4.10264049e-04,  2.45015632e-03,  3.67358267e-03,  8.62238572e-04,  -6.12687715e-03};

    SECTION("Molecular Dipoles") {
        REQUIRE(VectorsAreEqual(m_mu_p, expected_m_mu_p, TOL));
        REQUIRE(VectorsAreEqual(m_mu_i, expected_m_mu_i, TOL));
    }

    std::vector<double> t_mu_p, t_mu_i, t_mu;
    systems[1].GetTotalDipole(t_mu_p, t_mu_i, t_mu);

    std::vector<double> expected_t_mu_p = {3.88770006e-01, 1.25867621e-01, 9.64289365e-01};
    std::vector<double> expected_t_mu_i = {-7.22096559e-04, 3.55543954e-03, -6.15303593e-03};
    std::vector<double> expected_t_mu = {3.88047909e-01, 1.29423061e-01, 9.58136330e-01};

    SECTION("Molecular Dipoles") {
        REQUIRE(VectorsAreEqual(t_mu_p, expected_t_mu_p, TOL));
        REQUIRE(VectorsAreEqual(t_mu_i, expected_t_mu_i, TOL));
        REQUIRE(VectorsAreEqual(t_mu, expected_t_mu, TOL));
    }
}

TEST_CASE("Setters") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    char jj5[] = "unittests_inputs/mbx.json";
    systems[1].SetUpFromJson(jj5);
    systems[2].SetUpFromJson(jj5);

    SECTION("Cutoffs") {
        double expected_twobcutoff = 12.0;
        double expected_threebcutoff = 7.2;

        systems[2].Set2bCutoff(expected_twobcutoff);
        systems[2].Set3bCutoff(expected_threebcutoff);

        double twobcutoff = systems[2].Get2bCutoff();
        double threebcutoff = systems[2].Get3bCutoff();

        REQUIRE(twobcutoff == Approx(expected_twobcutoff).margin(TOL));
        REQUIRE(threebcutoff == Approx(expected_threebcutoff).margin(TOL));
    }

    SECTION("Max Batch of poly evaluation") {
        size_t expected_nmax1b = 200;
        size_t expected_nmax2b = 300;
        size_t expected_nmax3b = 250;

        systems[2].SetNMaxEval1b(expected_nmax1b);
        systems[2].SetNMaxEval2b(expected_nmax2b);
        systems[2].SetNMaxEval3b(expected_nmax3b);

        size_t nmax1b = systems[2].GetMaxEval1b();
        size_t nmax2b = systems[2].GetMaxEval2b();
        size_t nmax3b = systems[2].GetMaxEval3b();

        REQUIRE(expected_nmax1b == nmax1b);
        REQUIRE(expected_nmax2b == nmax2b);
        REQUIRE(expected_nmax3b == nmax3b);
    }

    SECTION("Electrostatics Solver") {
        double expected_diptol = 1E-24;
        size_t expected_maxiter = 133;
        std::string expected_dipmethod = "iter";

        systems[2].SetDipoleTol(expected_diptol);
        systems[2].SetDipoleMaxIt(expected_maxiter);
        systems[2].SetDipoleMethod(expected_dipmethod);

        double diptol = systems[2].GetDipoleTolerance();
        size_t maxiter = systems[2].GetMaxIterationsDipoles();
        std::string dipmethod = systems[2].GetDipoleMethod();

        REQUIRE(diptol == Approx(expected_diptol).margin(TOL));
        REQUIRE(maxiter == expected_maxiter);
        REQUIRE(dipmethod == expected_dipmethod);
    }

    SECTION("Box") {
        std::vector<double> expected_box = {22.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 23.0};
        std::vector<double> expected_box_abc = {22.0, 20.0, 23.0, 90.0, 90.0, 90.0};

        systems[2].SetPBC(expected_box);

        std::vector<double> box = systems[2].GetBox();
        std::vector<double> box_abc = systems[2].GetBoxABCabc();

        REQUIRE(VectorsAreEqual(box, expected_box, TOL));
        REQUIRE(VectorsAreEqual(box_abc, expected_box_abc, TOL));

        systems[2].SetPBC(expected_box_abc);

        box = systems[2].GetBox();
        box_abc = systems[2].GetBoxABCabc();

        REQUIRE(VectorsAreEqual(box, expected_box, TOL));
        REQUIRE(VectorsAreEqual(box_abc, expected_box_abc, TOL));

        SECTION("Assertions") {
            std::vector<double> bad_box = {10.0, 2.0};
            bool failed = false;
            try {
                systems[2].SetPBC(bad_box);
            } catch (...) {
                failed = true;
            }
            REQUIRE(failed);
        }
    }

    SECTION("Xyz") {
        std::vector<double> original_xyz = systems[1].GetXyz();
        std::vector<double> original_real_xyz = systems[1].GetRealXyz();

        std::vector<double> expected_real_xyz;
        for (size_t i = 0; i < original_real_xyz.size(); i++) expected_real_xyz.push_back(original_real_xyz[i] + 0.1);

        std::vector<double> expected_xyz;
        for (size_t i = 0; i < original_xyz.size(); i++) expected_xyz.push_back(original_xyz[i] + 0.1);

        systems[1].SetRealXyz(expected_real_xyz);

        std::vector<double> real_xyz = systems[1].GetRealXyz();
        std::vector<double> xyz = systems[1].GetXyz();

        REQUIRE(VectorsAreEqual(real_xyz, expected_real_xyz, TOL));
        REQUIRE(VectorsAreEqual(xyz, expected_xyz, TOL));

        systems[1].SetXyz(expected_xyz);

        real_xyz = systems[1].GetRealXyz();
        xyz = systems[1].GetXyz();

        REQUIRE(VectorsAreEqual(real_xyz, expected_real_xyz, TOL));
        REQUIRE(VectorsAreEqual(xyz, expected_xyz, TOL));

        SECTION("Assertions") {
            bool setxyz_failed = false;
            try {
                std::vector<double> xyz_bad = {0.0, 1.0};
                systems[1].SetXyz(xyz_bad);
            } catch (...) {
                setxyz_failed = true;
            }
            REQUIRE(setxyz_failed);

            bool setrealxyz_failed = false;
            try {
                std::vector<double> xyz_bad = {0.0, 1.0};
                systems[1].SetRealXyz(xyz_bad);
            } catch (...) {
                setrealxyz_failed = true;
            }
            REQUIRE(setrealxyz_failed);
        }
    }

    SECTION("Pairs to use/ignore") {
        SECTION("TTM-nrg pairs") {
            std::vector<std::pair<std::string, std::string>> ttm_pairs = systems[1].GetTTMnrgPairs();

            REQUIRE(ttm_pairs.size() == 0);

            std::vector<std::pair<std::string, std::string>> expected_ttm_pairs = {{"ch4", "h2o"}, {"co2", "h2o"}};
            for (size_t i = 0; i < expected_ttm_pairs.size(); i++) {
                systems[1].AddTTMnrgPair(expected_ttm_pairs[i].second, expected_ttm_pairs[i].first);
            }

            ttm_pairs = systems[1].GetTTMnrgPairs();

            REQUIRE(VectorsAreEqual(ttm_pairs, expected_ttm_pairs));
        }

        SECTION("Lennard Jones") {
            std::vector<std::pair<std::string, std::string>> expected_use_lennard_jones = {{"co2", "h2o"},
                                                                                           {"ch4", "co2"}};
            systems[1].SetLennardJonesPairs(expected_use_lennard_jones);
            // TODO There is no getter in System, so far.
        }

        SECTION("Ignore dispersion") {
            std::vector<std::pair<std::string, std::string>> expected_ignore_dispersion = {{"ch4", "h2o"},
                                                                                           {"h2o", "co2"}};
            systems[1].SetIgnoreDispersionPairs(expected_ignore_dispersion);
            // TODO There is no getter in System, so far.
        }

        SECTION("FF monomers") {
            std::vector<std::string> ffmons = systems[1].GetFFMons();
            REQUIRE(ffmons.size() == 0);

            std::vector<std::string> expected_ffmons = {"ch4", "co2"};

            for (size_t i = 0; i < expected_ffmons.size(); i++) {
                systems[1].AddFFMon(expected_ffmons[i]);
            }

            ffmons = systems[1].GetFFMons();

            REQUIRE(VectorsAreEqual(ffmons, expected_ffmons));
        }

        SECTION("Polynomial pairs") {
            SECTION("One-body") {
                std::vector<std::string> ignore1b = systems[1].Get1bIgnorePoly();
                REQUIRE(ignore1b.size() == 0);

                std::vector<std::string> expected_ignore1b = {"co2", "h2o"};

                for (size_t i = 0; i < expected_ignore1b.size(); i++) {
                    systems[1].Add1bIgnorePoly(expected_ignore1b[i]);
                }

                ignore1b = systems[1].Get1bIgnorePoly();

                REQUIRE(VectorsAreEqual(ignore1b, expected_ignore1b));
            }

            SECTION("Two-body") {
                std::vector<std::vector<std::string>> ignore2b = systems[1].Get2bIgnorePoly();
                REQUIRE(ignore2b.size() == 0);

                std::vector<std::vector<std::string>> expected_ignore2b = {{"ch4", "co2"}, {"dp1", "h2o"}};
                for (size_t i = 0; i < expected_ignore2b.size(); i++) {
                    systems[1].Add2bIgnorePoly(expected_ignore2b[i][1], expected_ignore2b[i][0]);
                }

                ignore2b = systems[1].Get2bIgnorePoly();

                REQUIRE(ignore2b == expected_ignore2b);
            }

            SECTION("Three-body") {
                std::vector<std::vector<std::string>> ignore3b = systems[1].Get3bIgnorePoly();
                REQUIRE(ignore3b.size() == 0);

                std::vector<std::vector<std::string>> expected_ignore3b = {
                    {"h2o", "h2o", "h2o"}, {"ch4", "h2o", "h2o"}, {"ch4", "co2", "h2o"}};
                for (size_t i = 0; i < expected_ignore3b.size(); i++) {
                    systems[1].Add3bIgnorePoly(expected_ignore3b[i][2], expected_ignore3b[i][1],
                                               expected_ignore3b[i][0]);
                }

                ignore3b = systems[1].Get3bIgnorePoly();

                REQUIRE(ignore3b == expected_ignore3b);
            }
        }
    }
}

TEST_CASE("Add Monomer") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    // Set up json defaults
    char jj5[] = "unittests_inputs/mbx.json";
    systems[2].SetUpFromJson(jj5);

    SECTION("Assertions") {
        bool failed = false;
        try {
            std::vector<double> xyz = {1.0, 0.0, 0.0};
            std::vector<std::string> ats = {"X"};
            std::string id = "dp1";
            size_t islocal = 1;
            int tag = 0;
            systems[2].AddMonomer(xyz, ats, id, islocal, tag);
        } catch (...) {
            failed = true;
        }

        REQUIRE(failed);
    }
}

TEST_CASE("JSON files") {
    // Read systems from input
    // 3 systems read: complete, without point charges, and only point charges
    std::vector<bblock::System> systems;
    try {
        std::ifstream ifs("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg");
        if (!ifs) throw std::runtime_error("could not open the NRG file");
        tools::ReadNrg("unittests_inputs/input_h2o_co2_ch4_dp1_unittest_system.nrg", systems);
        ifs.close();
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        REQUIRE(1 == 2);
    }

    SECTION("Set up") {
        // Set up json defaults (json does not exist)
        char jj6[] = "unittests_inputs/i_should_not_be_here.json";
        systems[2].SetUpFromJson(jj6);

        // Json does exist
        char jj7[] = "unittests_inputs/mbx_wextrajson.json";
        systems[0].SetUpFromJson(jj7);
        char jj8[] = "unittests_inputs/mbx_wextrajson_rightpaths.json";
        systems[1].SetUpFromJson(jj8);

        // TODO add getters to ensure that results are as expected
    }

    SECTION("Retrieve and use json in string format") {
        std::string json_full = systems[1].GetJsonText();
        std::string json_disprep = systems[1].GetJsonDispersionRepulsionText();
        std::string json_mons = systems[1].GetJsonMonomersText();

        systems[1].SetUpFromJson(json_full);
    }

    SECTION("Human readable json config") { std::string json_human = systems[1].GetCurrentSystemConfig(); }
}

// TODO Something is worng with this test
// TEST_CASE("Initialize") {
//    SECTION("Empty system") {
//        bblock::System s;
//
//        bool first_initialization_passed = false;
//        try {
//            s.Initialize();
//            first_initialization_passed = true;
//        } catch (...) {
//            first_initialization_passed = false;
//        }
//
//        REQUIRE(first_initialization_passed);
//
//        bool is_setup_with_json = false;
//        try {
//            s.SetUpFromJson("unittests_inputs/mbx.json");
//            is_setup_with_json = true;
//        } catch (...) {
//            is_setup_with_json = false;
//        }
//
//        REQUIRE(is_setup_with_json);
//
//        bool second_initialization_failed = false;
//        try {
//            s.Initialize();
//            second_initialization_failed = true;
//        } catch (...) {
//            second_initialization_failed = false;
//        }
//
//        REQUIRE(second_initialization_failed);
//    }
//}
