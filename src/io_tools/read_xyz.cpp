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

#include "io_tools/read_xyz.h"

/**
 * @file read_xyz.cpp
 * @brief Contains the function definitions that allow to read an XYZ file
 */

namespace tools {

void ReadXYZ(const char *filename, std::vector<std::vector<std::string> > &atom_names,
             std::vector<std::vector<double> > &coords, std::vector<std::vector<double> > &boxes, bool read_box,
             double to_angstrom) {
    // Check that filename is not empty
    assert(filename);
    std::ifstream ifs(filename);

    // Check that filename exists
    if (!ifs) {
        std::ostringstream oss;
        oss << "Could not open XYZ file " << filename << " for reading.";
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    // Declare variables to store the line number
    size_t lineno(0);

    // Clear and reset the output vectors
    atom_names.clear();
    coords.clear();
    boxes.clear();

    // If file is empty, return
    if (ifs.peek() == std::ifstream::traits_type::eof()) return;

    // Read trajectory til the end
    std::vector<double> crd;
    std::vector<double> box;
    std::vector<std::string> at_names;

    size_t frameno = 0;
    while (true) {
        // Read system from file

        ReadFrame(lineno, ifs, at_names, crd, box, read_box, to_angstrom);

        // Store data in vectors
        atom_names.push_back(at_names);
        coords.push_back(crd);
        boxes.push_back(box);

        // Check if next line ends file
        std::streampos oldpos = ifs.tellg();
        std::string line;
        std::getline(ifs, line);
        if (ifs.eof() or line.empty()) {
            break;
        } else {
            ifs.seekg(oldpos);
        }
        frameno++;
    }

    return;
}

void ReadFrame(size_t &lineno, std::istream &ifs, std::vector<std::string> &atom_names, std::vector<double> &coords,
               std::vector<double> &box, bool read_box, double to_angstrom) {
    // Check we are not at the end of the file
    if (ifs.eof()) return;

    // Clear input vectors
    atom_names.clear();
    coords.clear();

    std::string line;
    std::getline(ifs, line);
    lineno++;

    size_t mon_count = 0;

    // Obtain first line. Should be the number of atoms
    size_t nat;
    std::istringstream iss(line);
    try {
        iss >> nat;
    } catch (...) {
        std::ostringstream oss;
        oss << "Expected number of atoms. Found " << iss.str();
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    // Second line should contain the cell if read_box is true
    std::getline(ifs, line);
    lineno++;
    std::string tmp;
    try {
        // Will assume that the box line is and is a othrogonal box:
        // # CELL(abcABC):   19.72950    19.72950    19.72950    90.00000    90.00000    90.00000  Step:           0
        // Bead:       0 positions{angstrom}  cell{angstrom}
        if (read_box) {
            std::istringstream boxline(line);
            double x;
            double y;
            double z;
            boxline >> tmp >> tmp >> x >> y >> z;
            box = {x * to_angstrom, 0.0, 0.0, 0.0, y * to_angstrom, 0.0, 0.0, 0.0, z * to_angstrom};
        }
    } catch (...) {
        std::ostringstream oss;
        oss << "A problem has occured while reading the box at line " << lineno;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    // Now read each one of the atoms in the xyz
    for (size_t i = 0; i < nat; i++) {
        std::getline(ifs, line);
        lineno++;

        std::string at;
        double x;
        double y;
        double z;

        std::istringstream atline(line);
        try {
            atline >> at >> x >> y >> z;
            atom_names.push_back(at);
            coords.push_back(x * to_angstrom);
            coords.push_back(y * to_angstrom);
            coords.push_back(z * to_angstrom);
        } catch (...) {
            std::ostringstream oss;
            oss << "A problem has occured while reading atom " << i << " at line " << lineno;
            throw CUException(__func__, __FILE__, __LINE__, oss.str());
        }
    }

    if (atom_names.size() == 0 or coords.size() == 0) {
        std::ostringstream oss;
        oss << "A problem has occured while reading frame ending at line" << lineno;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }
}

}  // namespace tools
