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
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"

#include "bblock/system.h"

namespace {

static std::vector<bblock::System> systems;

}  // namespace

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    if (argc != 1) {
        std::cerr << "usage: " << argv[0] << std::endl;
        return 0;
    }

    // Exit code
    size_t exitcode = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Create bad inputs
    ////////////////////////////////////////////////////////////////////////////

    // Non existing input file
    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg((char *)"not_exiting_file.nrg", systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////

    // Input with empty line at the beggining
    char *fname((char *)"input.nrg");
    std::ofstream ofs(fname);

    ofs << std::endl << "SYSTEM" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with no SYSTEM keyword at the beggining
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "MOLECULE" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with SYSTEM keyword at the beggining and followed by
    // endline
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl << std::endl << "MOLECULE" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with no MOLECULE keyword after SYSTEM
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl << "MONOMER" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with blank line after MOLECULE
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl << "MOLECULE" << std::endl << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with no MONOMER keyword after MOLECULE
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl << "MOLECULE" << std::endl << "MOLECULE" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with MONOMER keyword but no monomer name
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl << "MOLECULE" << std::endl << "MONOMER " << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with MONOMER keyword and monomer name but not atom info
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl << "MOLECULE" << std::endl << "MONOMER cl" << std::endl << "ENDMON" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with non-numerical coordinates
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl
        << "MOLECULE" << std::endl
        << "MONOMER cl" << std::endl
        << " Cl aa bb cc" << std::endl
        << "ENDMON" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with numerical coordinates but not endmon
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl
        << "MOLECULE" << std::endl
        << "MONOMER cl" << std::endl
        << " Cl 0.0 0.0 0.0" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    // Input file with numerical coordinates and endmol but no endmon
    fname = (char *)"input.nrg";
    ofs.open(fname);

    ofs << "SYSTEM" << std::endl
        << "MOLECULE" << std::endl
        << "MONOMER cl" << std::endl
        << " Cl 0.0 0.0 0.0" << std::endl
        << "ENDMOL" << std::endl;
    ofs.close();

    try {
        exitcode = 1;
        systems.clear();
        tools::ReadNrg(fname, systems);
    } catch (CUException &e) {
        exitcode = 0;
        std::cerr << "Error message expected:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    std::remove(fname);

    ////////////////////////////////////////////////////////////////////////////

    if (exitcode == 0) {
        std::cout << "All tests passed!\n";
    }

    return exitcode;
}
