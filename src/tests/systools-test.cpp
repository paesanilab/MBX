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

#include "tools/math_tools.h"

#include "bblock/system.h"

#define REL_TOL 1E-04

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

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " input.nrg" << std::endl;
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

    // Setting Vector with expected positions of the entire system
    std::vector<double> xyz_expected = {
        1.29053e+00,  4.40150e+00,  3.83867e-02,  1.40062e+00,  5.02504e+00,  7.56240e-01,  3.38249e-01,  4.22779e+00,
        -1.13894e-02, 1.11085e+00,  4.49747e+00,  1.80923e-01,  1.27803e+00,  1.60669e+00,  2.03625e-01,  1.60463e+00,
        2.51914e+00,  1.82890e-01,  1.85519e+00,  1.11079e+00,  -3.76182e-01, 1.47085e+00,  1.69556e+00,  7.54974e-02,
        -1.13520e+00, 3.01117e+00,  -3.01867e-02, -1.75524e+00, 2.83899e+00,  -7.38120e-01, -5.23152e-01, 2.26042e+00,
        -2.52638e-02, -1.13690e+00, 2.81426e+00,  -1.80176e-01};

    // Setting expected charges
    std::vector<double> chg_expected = {0.0000000000e+00, 5.7769308798e-01, 5.7615998121e-01, -1.1538530692e+00,
                                        0.0000000000e+00, 5.7663564934e-01, 5.7823701694e-01, -1.1548726663e+00,
                                        0.0000000000e+00, 5.7859034043e-01, 5.7709162608e-01, -1.1556819665e+00};

    // Setting expected polarizabilities
    std::vector<double> pol_expected = {1.31000e+00, 2.94000e-01, 2.94000e-01, 0.00000e+00, 1.31000e+00, 2.94000e-01,
                                        2.94000e-01, 0.00000e+00, 1.31000e+00, 2.94000e-01, 2.94000e-01, 0.00000e+00};

    // Setting expected polarizability factors
    std::vector<double> polfac_expected = {1.31000e+00, 2.94000e-01, 2.94000e-01, 1.31000e+00,
                                           1.31000e+00, 2.94000e-01, 2.94000e-01, 1.31000e+00,
                                           1.31000e+00, 2.94000e-01, 2.94000e-01, 1.31000e+00};

    std::vector<double> box = {100.0, 0.0, 0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 100.0};
    std::vector<double> box_inv = InvertUnitCell(box);
    int exitcode = 0;

    //////////////////////////////////////////////////////////////////////////////

    // Test M-Sites
    std::string test = "M-sites";

    // Check that M-sites are the same as expected

    std::vector<double> xyz = systems[0].GetXyz();
    CompareVector(xyz_expected, xyz, test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test charges
    test = "charges";

    std::vector<double> chg = systems[0].GetCharges();
    CompareVector(chg_expected, chg, test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test polarizabilities
    test = "polarizabilities";

    std::vector<double> pol = systems[0].GetPolarizabilities();
    CompareVector(pol_expected, pol, test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test polarizability factors
    test = "polarizability factors";

    std::vector<double> polfac = systems[0].GetPolarizabilityFactors();
    CompareVector(polfac_expected, polfac, test, exitcode);

    //////////////////////////////////////////////////////////////////////////////

    // Test Systools functions
    // Prepare a fake system to order and play with

    std::vector<std::string> mon_input = {"i", "cl", "br", "cl"};
    std::vector<size_t> islocal_input = {1, 1, 1, 1};
    std::vector<double> xyz_input = {0.0, 0.0, 0.0,   // i
                                     0.0, 0.0, 3.0,   // cl
                                     0.0, 0.0, 6.0,   // br
                                     0.0, 0.0, 9.0};  // cl
    std::vector<size_t> sites_input = {1, 1, 1, 1};
    std::vector<size_t> nats_input = {1, 1, 1, 1};
    std::vector<size_t> first_index_input = {0, 1, 2, 3};
    size_t nsites_input = 4;
    size_t natoms_input = 4;

    // Declare also outputs

    std::vector<std::string> mon = mon_input;
    std::vector<size_t> islocal = islocal_input;
    xyz = xyz_input;
    std::vector<size_t> sites = sites_input;
    std::vector<size_t> nats = nats_input;
    std::vector<size_t> first_index = {0, 1, 2, 3};

    std::vector<size_t> original2current;
    std::vector<std::pair<size_t, size_t>> original_order;
    std::vector<std::pair<size_t, size_t>> original_order_realSites;

    // And expected outputs
    std::vector<size_t> original2current_expected = {0, 2, 1, 3};
    std::vector<std::pair<size_t, size_t>> original_order_expected;
    std::vector<std::pair<size_t, size_t>> original_order_realSites_expected;
    std::vector<std::pair<std::string, size_t>> mon_type_num_expected;

    // Fill in expected outputs
    // original_order
    original_order_expected.push_back(std::make_pair(0, 0));
    original_order_expected.push_back(std::make_pair(2, 2));
    original_order_expected.push_back(std::make_pair(1, 1));
    original_order_expected.push_back(std::make_pair(3, 3));

    // original_order_realsites
    original_order_realSites_expected.push_back(std::make_pair(0, 0));
    original_order_realSites_expected.push_back(std::make_pair(2, 2));
    original_order_realSites_expected.push_back(std::make_pair(1, 1));
    original_order_realSites_expected.push_back(std::make_pair(3, 3));

    // mon_type num
    mon_type_num_expected.push_back(std::make_pair("i", 1));
    mon_type_num_expected.push_back(std::make_pair("br", 1));
    mon_type_num_expected.push_back(std::make_pair("cl", 2));

    //////////////////////////////////////////////////////////////////////////////

    test = "Order Monomers";

    std::vector<std::pair<std::string, size_t>> mon_type_num =
        systools::OrderMonomers(mon, islocal, sites, nats, original2current, original_order, original_order_realSites);

    if (original2current != original2current_expected) {
        std::cerr << test << ":: original2current vector does not match" << std::endl;
        exitcode = 1;
    }

    if (original_order != original_order_expected) {
        std::cerr << test << ":: original_order vector does not match" << std::endl;
        exitcode = 1;
    }

    if (original_order_realSites != original_order_realSites_expected) {
        std::cerr << test << ":: original_order_realSites vector does not match" << std::endl;
        exitcode = 1;
    }

    if (mon_type_num != mon_type_num_expected) {
        std::cerr << test << ":: mon_type_num vector does not match" << std::endl;
        exitcode = 1;
    }

    // Test assertions for this function
    std::vector<std::string> mon_empty;
    std::vector<size_t> islocal_empty;
    std::vector<size_t> nats_bad = {1, 4};
    std::vector<size_t> sites_empty;

    // Try empty monomer vector
    try {
        exitcode = 1;
        std::vector<std::pair<std::string, size_t>> mon_type_num_cp =
            systools::OrderMonomers(mon_empty, islocal_empty, sites, nats, original2current, original_order, original_order_realSites);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Try nats not matching mon size
    try {
        exitcode = 1;
        std::vector<std::pair<std::string, size_t>> mon_type_num_cp =
	    systools::OrderMonomers(mon, islocal, sites, nats_bad, original2current, original_order, original_order_realSites);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Try sites not matching mon size
    try {
        exitcode = 1;
        std::vector<std::pair<std::string, size_t>> mon_type_num_cp =
	    systools::OrderMonomers(mon, islocal, sites_empty, nats, original2current, original_order, original_order_realSites);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    test = "SetUpMonomers";

    // Function call
    size_t nsites = systools::SetUpMonomers(mon_input, sites, nats, first_index);

    if (nsites != nsites_input) {
        std::cerr << test << ":: nsites do not match: "
                  << "Expected = " << nsites_input << ", found " << nsites << std::endl;
        exitcode = 1;
    }

    if (sites != sites_input) {
        std::cerr << test << ":: sites vector does not match" << std::endl;
        exitcode = 1;
    }

    if (nats != nats_input) {
        std::cerr << test << ":: nats vector does not match" << std::endl;
        exitcode = 1;
    }

    if (first_index != first_index_input) {
        std::cerr << test << ":: first_index vector does not match" << std::endl;
        exitcode = 1;
    }

    // Test assertions
    std::vector<std::string> mon_unknown = {"cl", "monx"};

    // Try unknown monomer monx
    try {
        exitcode = 1;
        systools::SetUpMonomers(mon_unknown, sites, nats, first_index);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Try empty monomer vector
    try {
        exitcode = 1;
        systools::SetUpMonomers(mon_empty, sites, nats, first_index);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    test = "FixMonomerCoordinates";

    // Test assertions

    // Try box size different from 9
    try {
        std::vector<double> box_bad(6, 10.0);
        std::vector<double> box_bad_inv(6, 10.0);
        exitcode = 1;
        systools::FixMonomerCoordinates(xyz, box_bad, box_bad_inv, nats, first_index);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Try nats and first index with different size
    try {
        exitcode = 1;
        systools::FixMonomerCoordinates(xyz, box, box_inv, nats_bad, first_index);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Try xyz with too few coordinates
    std::vector<double> xyz_short(11, 0.0);
    try {
        exitcode = 1;
        systools::FixMonomerCoordinates(xyz_short, box, box_inv, nats, first_index);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    // Try with actual coordinates. Should go through
    xyz_short = xyz;
    try {
        exitcode = 0;
        systools::FixMonomerCoordinates(xyz_short, box, box_inv, nats, first_index);
    } catch (CUException &e) {
        std::cerr << "!!!!!Error message NOT expected:" << std::endl;
        exitcode = 1;
        std::cerr << e.what() << std::endl;
    }

    //////////////////////////////////////////////////////////////////////////////

    if (exitcode == 0) {
        std::cout << "All tests passed!\n";
    }

    return exitcode;
}
