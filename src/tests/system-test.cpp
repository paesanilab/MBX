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

#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

#define REL_TOL 1E-06
namespace {

static std::vector<bblock::System> systems;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

void CompareVector(std::vector<double> vec_ref, std::vector<double> vec, std::string test, int &exitcode) {
    if (vec_ref.size() != vec.size()) {
        std::cerr << " ** Error ** : "
                  << "Size vectors in test " << test << " do not match: \nRef(" << vec_ref.size() << ") vs. Test("
                  << vec.size() << ")\n";
        exitcode = 1;
        return;
    }

    for (size_t i = 0; i < vec.size(); i++) {
        double diff = std::abs(vec_ref[i] - vec[i]);
        if (diff > REL_TOL) {
            std::cerr << " ** Error ** : "
                      << " Vector values in test: \" " << test << " \" do not match\n";
            std::cerr << "vec_ref[" << i << "] = " << vec_ref[i] << " vs. "
                      << "vec[" << i << "] = " << vec[i] << std::endl;
            exitcode = 1;
        }
    }
}

void PrintError(std::string test, int &exitcode) {
    std::cerr << " ** Error ** : "
              << "Test: \" " << test << " has failed." << std::endl;
    exitcode = 1;
}

template <typename T>
void PrintDifference(T val1, std::string header1, T val2, std::string header2) {
    std::cerr << header1 << "(" << val1 << ") != " << header2 << "(" << val2 << ")" << std::endl;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: energy input.nrg" << std::endl;
        return 0;
    }

    try {
        std::ifstream ifs(argv[1]);

        if (!ifs) {
            throw std::runtime_error("could not open the NRG file");
        }

        tools::ReadNrg(argv[1], systems);
    } catch (const std::exception &e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    // Generate all the data manually, then compare with the data generated
    // by the classes

    // Generate the coordinates internally
    // They must match the input:
    std::vector<double> xyz_ref = {1.321764984e+00,  -9.741042851e-02, -1.766603401e-01,  // O
                                   1.417497992e+00,  6.385845339e-01,  4.439868776e-01,   // H
                                   3.540663798e-01,  -1.588118731e-01, -2.456073451e-01,  // H
                                   -2.208131200e+00, 1.976693677e-01,  2.393048227e-01,   // Br
                                   2.944922158e-01,  -2.351535749e+00, 1.444824160e+00,   // O
                                   9.312334658e-01,  -1.788380344e+00, 9.860224962e-01,   // H
                                   -4.776761258e-01, -1.772172188e+00, 1.485185464e+00,   // H
                                   -1.122835060e+00, -3.335553877e+00, -9.798953936e-01,  // O
                                   -5.748067974e-01, -3.252236355e+00, -1.893369508e-01,  // H
                                   -1.538286573e+00, -2.467574899e+00, -1.019958033e+00,  // H
                                   1.275636119e-01,  2.311277753e+00,  -1.655546212e+00,  // O
                                   7.574431466e-01,  1.582562474e+00,  -1.634768274e+00,  // H
                                   -6.864683765e-01, 1.872099084e+00,  -1.368183421e+00,  // H
                                   5.998702944e-01,  2.360680451e+00,  1.273512544e+00,   // O
                                   -2.130729533e-01, 1.844059568e+00,  1.366935547e+00,   // H
                                   5.104601172e-01,  2.693542675e+00,  3.706992464e-01};  // H

    // Atom names and monomers
    std::vector<std::string> atom_names_ref = {"O", "H", "H", "Br", "O", "H", "H", "O",
                                               "H", "H", "O", "H",  "H", "O", "H", "H"};

    std::vector<std::string> monomers_ref = {"h2o", "br-", "h2o", "h2o", "h2o", "h2o"};

    // Number of sites and number of atoms
    size_t nat_ref = 16;     // 1 + 5*3
    size_t nsites_ref = 21;  // 1 + 5*4

    std::vector<size_t> nats_ref = {3, 1, 3, 3, 3, 3};
    std::vector<size_t> nsitesv_ref = {4, 1, 4, 4, 4, 4};
    std::vector<size_t> first_index_ref = {0, 4, 5, 9, 13, 17};

    // Now we create a system that will be the same as the one read
    bblock::System system_ref;

    // Add monomers to the system
    size_t count = 0;
    for (size_t i = 0; i < monomers_ref.size(); i++) {
        std::vector<double> xyz(xyz_ref.begin() + 3 * count, xyz_ref.begin() + 3 * count + 3 * nats_ref[i]);
        std::vector<std::string> ats(atom_names_ref.begin() + count, atom_names_ref.begin() + count + nats_ref[i]);
        std::string monid = monomers_ref[i];
        system_ref.AddMonomer(xyz, ats, monid);
        count += nats_ref[i];
    }

    // Initialize the system to fill in the information
    system_ref.Initialize();

    //////////////////////////////////////////////////////////////////////////////

    int exitcode = 0;
    std::string test;
    std::string header_manual = "Manually created System ";
    std::string header_ref = "Reference ";
    std::string header_read = "Read System ";

    // Test GetNumMon
    test = "GetNumMon()";

    size_t nm_ref = system_ref.GetNumMon();
    size_t nm = systems[0].GetNumMon();

    if (nm_ref != monomers_ref.size()) {
        PrintError(test, exitcode);
        PrintDifference(nm_ref, header_manual, monomers_ref.size(), header_ref);
    }

    if (nm != monomers_ref.size()) {
        PrintError(test, exitcode);
        PrintDifference(nm, header_read, monomers_ref.size(), header_ref);
    }

    //////////////////////////////////////////////////////////////////////////////

    //  // Test GetNumMol()
    //  test = "GetNumMol()";
    //
    //  nm     = systems[0].GetNumMol();
    //
    //  if (nm     != monomers_ref.size()) {
    //    PrintError(test, exitcode);
    //    PrintDifference(nm, header_read,
    //                    monomers_ref.size(), header_ref);
    //  }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetNumSites()
    test = "GetNumSites()";

    size_t ns_ref = system_ref.GetNumSites();
    size_t ns = systems[0].GetNumSites();

    if (ns_ref != nsites_ref) {
        PrintError(test, exitcode);
        PrintDifference(ns_ref, header_manual, nsites_ref, header_ref);
    }

    if (ns != nsites_ref) {
        PrintError(test, exitcode);
        PrintDifference(ns, header_read, nsites_ref, header_ref);
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetNumRealSites()
    test = "GetNumRealSites()";

    ns_ref = system_ref.GetNumRealSites();
    ns = systems[0].GetNumRealSites();

    if (ns_ref != nat_ref) {
        PrintError(test, exitcode);
        PrintDifference(ns_ref, header_manual, nat_ref, header_ref);
    }

    if (ns != nat_ref) {
        PrintError(test, exitcode);
        PrintDifference(ns, header_read, nat_ref, header_ref);
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetRealAtomNames()

    test = "GetRealAtomNames()";

    std::vector<std::string> atnames_ref = system_ref.GetRealAtomNames();
    std::vector<std::string> atnames_read = systems[0].GetRealAtomNames();

    if (atnames_ref != atnames_read) {
        PrintError(test + std::string(":: RealAtomNames don't match"), exitcode);
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetMonNumAt(n)
    test = "GetMonNumAt(n)";

    for (size_t i = 0; i < monomers_ref.size(); i++) {
        size_t nat_ref = system_ref.GetMonNumAt(i);
        size_t nat = systems[0].GetMonNumAt(i);

        if (nat_ref != nats_ref[i]) {
            PrintError(test, exitcode);
            PrintDifference(nat_ref, header_manual, nats_ref[i], header_ref);
        }

        if (nat != nats_ref[i]) {
            PrintError(test, exitcode);
            PrintDifference(nat, header_read, nats_ref[i], header_ref);
        }
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetFirstIndex(n)
    test = "GetFirstIndex(n)";

    for (size_t i = 0; i < monomers_ref.size(); i++) {
        size_t fi_ref = system_ref.GetFirstInd(i);
        size_t fi = systems[0].GetFirstInd(i);

        if (fi_ref != first_index_ref[i]) {
            PrintError(test, exitcode);
            PrintDifference(fi_ref, header_manual, first_index_ref[i], header_ref);
        }

        if (fi != first_index_ref[i]) {
            PrintError(test, exitcode);
            PrintDifference(fi, header_read, first_index_ref[i], header_ref);
        }
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetXyz()
    test = "GetXyz()";

    std::vector<double> xyz_manual(xyz_ref.size(), 0.0);
    std::vector<double> xyz_read(xyz_ref.size(), 0.0);

    std::vector<double> xyz_tmp = system_ref.GetXyz();
    std::vector<double> xyz_tmp2 = systems[0].GetXyz();

    size_t fi = 0;
    size_t fi_sites = 0;
    for (size_t i = 0; i < monomers_ref.size(); i++) {
        std::copy(xyz_tmp.begin() + fi_sites, xyz_tmp.begin() + fi_sites + 3 * nats_ref[i], xyz_manual.begin() + fi);
        std::copy(xyz_tmp2.begin() + fi_sites, xyz_tmp2.begin() + fi_sites + 3 * nats_ref[i], xyz_read.begin() + fi);
        fi += 3 * nats_ref[i];
        fi_sites += 3 * nsitesv_ref[i];
    }

    CompareVector(xyz_ref, xyz_manual, header_manual + "::" + test, exitcode);
    CompareVector(xyz_ref, xyz_read, header_read + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test GetRealXyz()
    test = "GetRealXyz()";

    xyz_manual = system_ref.GetRealXyz();
    xyz_read = systems[0].GetRealXyz();

    CompareVector(xyz_ref, xyz_manual, header_manual + "::" + test, exitcode);
    CompareVector(xyz_ref, xyz_read, header_read + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test GetGrads and GetRealGrads
    // Grad values are already asserted in energy test
    // Just testing that functions return what it is supposed to
    test = "GetGrads()";

    std::vector<double> grads_manual = system_ref.GetGrads();
    std::vector<double> grads_read = systems[0].GetGrads();

    CompareVector(grads_manual, grads_read, header_manual + "::" + test, exitcode);

    test = "GetRealGrads()";

    grads_manual = system_ref.GetRealGrads();
    grads_read = systems[0].GetRealGrads();

    CompareVector(grads_manual, grads_read, header_manual + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test GetCharges and GetRealCharges
    // Values tested in elec-tools
    // Only testing that functions return what it is supposed to

    test = "GetCharges()";

    std::vector<double> chg_manual = system_ref.GetCharges();
    std::vector<double> chg_read = systems[0].GetCharges();

    CompareVector(chg_manual, chg_read, header_manual + "::" + test, exitcode);

    test = "GetRealCharges()";

    chg_manual = system_ref.GetRealCharges();
    chg_read = systems[0].GetRealCharges();

    CompareVector(chg_manual, chg_read, header_manual + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test GetPolarizabilities and GetRealPolarizabilities
    // Values tested in elec-tools
    // Only testing that functions return what it is supposed to

    test = "GetPolarizabilities()";

    std::vector<double> pols_manual = system_ref.GetPolarizabilities();
    std::vector<double> pols_read = systems[0].GetPolarizabilities();

    CompareVector(pols_manual, pols_read, header_manual + "::" + test, exitcode);

    test = "GetRealPolarizabilities()";

    pols_manual = system_ref.GetRealPolarizabilities();
    pols_read = systems[0].GetRealPolarizabilities();

    CompareVector(pols_manual, pols_read, header_manual + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test GetPolarizabilityFactors and GetRealPolarizabilityFactors
    // Values tested in elec-tools
    // Only testing that functions return what it is supposed to

    test = "GetPolarizabilityFactors()";

    std::vector<double> polfacs_manual = system_ref.GetPolarizabilityFactors();
    std::vector<double> polfacs_read = systems[0].GetPolarizabilityFactors();

    CompareVector(polfacs_manual, polfacs_read, header_manual + "::" + test, exitcode);

    test = "GetRealPolarizabilityFactors()";

    polfacs_manual = system_ref.GetRealPolarizabilityFactors();
    polfacs_read = systems[0].GetRealPolarizabilityFactors();

    CompareVector(polfacs_manual, polfacs_read, header_manual + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test GetMonId(n)

    test = "GetMonId(n)";

    for (size_t i = 0; i < monomers_ref.size(); i++) {
        std::string mon_manual = system_ref.GetMonId(i);
        std::string mon_read = systems[0].GetMonId(i);

        if (mon_manual != monomers_ref[i]) {
            PrintError(test, exitcode);
            PrintDifference(mon_manual, header_manual, monomers_ref[i], header_ref);
        }

        if (mon_read != monomers_ref[i]) {
            PrintError(test, exitcode);
            PrintDifference(mon_read, header_read, monomers_ref[i], header_ref);
        }
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test SetXyz()

    test = "SetXyz()";

    // Manually created system
    // Get the current XYZ of the system
    std::vector<double> orig_xyz = system_ref.GetXyz();
    // Create new xyz with all 1.0
    std::vector<double> new_xyz(orig_xyz.size(), 1.0);

    // Set the new xyz to the system
    system_ref.SetXyz(new_xyz);
    // Retrieve it
    std::vector<double> new_get_xyz = system_ref.GetXyz();

    // Compare it
    CompareVector(new_get_xyz, new_xyz, header_manual + "::" + test, exitcode);

    // Set old Xyz again
    system_ref.SetXyz(orig_xyz);

    // Compare it
    CompareVector(orig_xyz, system_ref.GetXyz(), header_manual + "::" + test, exitcode);

    // Read system
    // Get the current XYZ of the system
    orig_xyz = systems[0].GetXyz();
    // Create new xyz with all 1.0
    new_xyz = std::vector<double>(orig_xyz.size(), 1.0);

    // Set the new xyz to the system
    systems[0].SetXyz(new_xyz);
    // Retrieve it
    new_get_xyz = systems[0].GetXyz();

    // Compare it
    CompareVector(new_get_xyz, new_xyz, header_read + "::" + test, exitcode);

    // Set old Xyz again
    systems[0].SetXyz(orig_xyz);

    // Compare it
    CompareVector(orig_xyz, systems[0].GetXyz(), header_read + "::" + test, exitcode);

    // Testing the exceptions
    std::vector<double> v(2, 1.0);
    try {
        exitcode = 1;
        system_ref.SetXyz(v);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Make sure the xyz has not been changed
    CompareVector(orig_xyz, system_ref.GetXyz(), header_manual + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test SetRealXyz()

    test = "SetRealXyz()";

    // Manually created system
    // Get the current XYZ of the system
    orig_xyz = system_ref.GetRealXyz();
    // Create new xyz with all 1.0
    new_xyz = std::vector<double>(orig_xyz.size(), 1.0);

    // Set the new xyz to the system
    system_ref.SetRealXyz(new_xyz);
    // Retrieve it
    new_get_xyz = system_ref.GetRealXyz();

    // Compare it
    CompareVector(new_get_xyz, new_xyz, header_manual + "::" + test, exitcode);

    // Set old Xyz again
    system_ref.SetRealXyz(orig_xyz);

    // Compare it
    CompareVector(orig_xyz, system_ref.GetRealXyz(), header_manual + "::" + test, exitcode);

    // Read system
    // Get the current XYZ of the system
    orig_xyz = systems[0].GetRealXyz();
    // Create new xyz with all 1.0
    new_xyz = std::vector<double>(orig_xyz.size(), 1.0);

    // Set the new xyz to the system
    systems[0].SetRealXyz(new_xyz);
    // Retrieve it
    new_get_xyz = systems[0].GetRealXyz();

    // Compare it
    CompareVector(new_get_xyz, new_xyz, header_read + "::" + test, exitcode);

    // Set old Xyz again
    systems[0].SetRealXyz(orig_xyz);

    // Compare it
    CompareVector(orig_xyz, systems[0].GetRealXyz(), header_read + "::" + test, exitcode);

    // Testing the exceptions
    v = std::vector<double>(3, 2.0);
    try {
        exitcode = 1;
        system_ref.SetRealXyz(v);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Make sure the xyz has not been changed
    CompareVector(orig_xyz, system_ref.GetRealXyz(), header_manual + "::" + test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test Initialize()

    test = "Initialize()";

    // Assessing that once the system is initialized, we cannot reinitialize it
    try {
        exitcode = 1;
        system_ref.Initialize();
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test AddMonomer()

    test = "AddMonomer()";

    // Testing that once initialized, we cannot add a new monomer
    try {
        exitcode = 1;
        v = std::vector<double>(3, 0.0);
        std::string id_v = "cl-";
        std::vector<std::string> at_v = {"Cl"};
        system_ref.AddMonomer(v, at_v, id_v);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    // Indirect test of AddMonomerInfo()

    test = "AddMonomerInfo()";

    // Creating a fake system
    v = std::vector<double>(2, 1.0);
    std::string id_v = "cl-";
    std::vector<std::string> at_v = {"Cl"};

    bblock::System s_wrong;
    s_wrong.AddMonomer(v, at_v, id_v);

    // Testing that with less than 3 coordinates you cannot have a system
    try {
        exitcode = 1;
        s_wrong.Initialize();
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Create another fake system, but now with more coordinates than needed
    v = std::vector<double>(4, 1.0);

    bblock::System s_also_wrong;
    s_also_wrong.AddMonomer(v, at_v, id_v);

    // Testing that if coordinate size and atom size don't match, it throws
    // and exception
    try {
        exitcode = 1;
        s_also_wrong.Initialize();
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test GetPairList

    test = "GetPairList()";

    // test assertions:
    try {
        exitcode = 1;
        system_ref.GetPairList(5, 3.0, 0, 1);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test SetPBC()

    test = "SetPBC(box)";

    // Checking that the code will throw and exception if the box
    // has incorrect size

    try {
        std::vector<double> box(5, 10.0);
        exitcode = 1;
        system_ref.SetPBC(box);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test Energy(do_grads), and individual energy functions

    test = "Energy(do_grads)";

    // Make sure that energy is not calculated if system is not initialized
    bblock::System s_empty;

    // Total Energy
    try {
        exitcode = 1;
        s_empty.Energy(true);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // One Body
    try {
        exitcode = 1;
        s_empty.OneBodyEnergy(true);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Two Body
    try {
        exitcode = 1;
        s_empty.TwoBodyEnergy(true);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Three Body
    try {
        exitcode = 1;
        s_empty.ThreeBodyEnergy(true);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Electrostatics
    try {
        exitcode = 1;
        s_empty.Electrostatics(true);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    // Test energy

    // Make sure that energy and the sum of the individual terms are the same
    double energy_ref = system_ref.Energy(true);
    double energy_read = systems[0].Energy(true);

    double onebody_ref = system_ref.OneBodyEnergy(true);
    double twobody_ref = system_ref.TwoBodyEnergy(true);
    double threebody_ref = system_ref.ThreeBodyEnergy(true);
    double dispersion_ref = system_ref.Dispersion(true);
    double electrostatic_ref = system_ref.Electrostatics(true);

    double onebody_read = systems[0].OneBodyEnergy(true);
    double twobody_read = systems[0].TwoBodyEnergy(true);
    double threebody_read = systems[0].ThreeBodyEnergy(true);
    double dispersion_read = systems[0].Dispersion(true);
    double electrostatic_read = systems[0].Electrostatics(true);

    // Compare one body
    std::string text = " energy does not match for read and reference system";
    if (std::abs(onebody_ref - onebody_read) > REL_TOL) {
        std::cerr << "One body " << text << std::endl;
        exitcode = 1;
    }

    if (std::abs(twobody_ref - twobody_read) > REL_TOL) {
        std::cerr << "Two body " << text << std::endl;
        exitcode = 1;
    }

    if (std::abs(threebody_ref - threebody_read) > REL_TOL) {
        std::cerr << "Three body " << text << std::endl;
        exitcode = 1;
    }

    if (std::abs(dispersion_ref - dispersion_read) > REL_TOL) {
        std::cerr << "Dispersion " << text << std::endl;
        exitcode = 1;
    }

    if (std::abs(electrostatic_ref - electrostatic_read) > REL_TOL) {
        std::cerr << "Electrostatic " << text << std::endl;
        exitcode = 1;
    }

    double sum_ref = onebody_ref + twobody_ref + threebody_ref + dispersion_ref + electrostatic_ref;
    double sum_read = onebody_read + twobody_read + threebody_read + dispersion_read + electrostatic_read;

    text = "Sum of individual terms does not match total energy in ";

    if (std::abs(energy_ref - sum_ref) > REL_TOL) {
        std::cerr << text << "reference system" << std::endl;
        exitcode = 1;
    }

    if (std::abs(energy_read - sum_read) > REL_TOL) {
        std::cerr << text << "read system" << std::endl;
        exitcode = 1;
    }

    //////////////////////////////////////////////////////////////////////////////

    if (exitcode == 0) {
        std::cout << "All tests passed!\n";
    }

    return exitcode;
}
