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

// For testing
//#define PRIVATE PUBLIC

namespace {

static std::vector<bblock::System> systems;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
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
    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    // NO PBC TEST (SYSTEM 0)

    // Retrieve dimers
    std::cout << "SYSTEM 0\n";

    std::vector<size_t> dimers = systems[0].GetPairList(2, 15.0, 0, systems[0].GetNumMon());
    size_t ndimers = dimers.size() / 2;
    // Print all dimers
    for (size_t j = 0; j < ndimers; j++) {
        std::cout << "Dimer[" << std::setw(10) << std::right << j << "] -> " << std::setw(8) << std::right
                  << dimers[2 * j] << std::setw(8) << std::right << dimers[2 * j + 1] << std::endl;
    }

    // retrieve Trimers
    std::vector<size_t> trimers = systems[0].GetPairList(3, 4.5, 0, systems[0].GetNumMon());
    size_t ntrimers = trimers.size() / 3;
    // Print all trimers
    for (size_t j = 0; j < ntrimers; j++) {
        std::cout << "Trimer[" << std::setw(10) << std::right << j << "] -> " << std::setw(8) << std::right
                  << trimers[3 * j] << std::setw(8) << std::right << trimers[3 * j + 1] << std::setw(8) << std::right
                  << trimers[3 * j + 2] << std::endl;
    }

    if (systems.size() > 1) {
        std::cout << "SYSTEM 1 (" << systems[1].GetNumMon() << " mons)\n";
        std::vector<double> box(9, 0.0);
        box[0] = 100.0;
        box[4] = 100.0;
        box[8] = 100.0;
        systems[1].SetPBC(box);
        std::vector<size_t> dimers = systems[1].GetPairList(2, 15.0, 0, systems[1].GetNumMon());
        ;
        size_t ndimers = dimers.size() / 2;
        // Print all dimers
        for (size_t j = 0; j < ndimers; j++) {
            std::cout << "Dimer[" << std::setw(10) << std::right << j << "] -> " << std::setw(8) << std::right
                      << dimers[2 * j] << std::setw(8) << std::right << dimers[2 * j + 1] << std::endl;
        }

        // retrieve Trimers
        std::vector<size_t> trimers = systems[1].GetPairList(3, 4.5, 0, systems[1].GetNumMon());
        ;
        size_t ntrimers = trimers.size() / 3;
        // Print all trimers
        for (size_t j = 0; j < ntrimers; j++) {
            std::cout << "Trimer[" << std::setw(10) << std::right << j << "] -> " << std::setw(8) << std::right
                      << trimers[3 * j] << std::setw(8) << std::right << trimers[3 * j + 1] << std::setw(8)
                      << std::right << trimers[3 * j + 2] << std::endl;
        }
    }

    return 0;
}
