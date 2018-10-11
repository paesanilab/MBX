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
