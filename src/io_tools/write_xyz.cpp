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

#include "io_tools/write_xyz.h"

/**
 * @file read_xyz.cpp
 * @brief Contains the function definitions that allow to read an XYZ file
 */

namespace tools {
void WriteXYZ(std::ofstream &off, std::vector<std::string> &atom_names, std::vector<double> &coords,
              std::vector<double> &boxes, std::string comment, bool write_box, double to_angstrom) {
    off << atom_names.size() << std::endl;
    if (write_box) {
        if (boxes.size() != 9) {
            off << "100.0 0.0 0.0 0.0 100.0 0.0 0.0 0.0 100.0\n";
        } else {
            for (size_t i = 0; i < 9; i++) {
                off << std::scientific << std::setprecision(12) << std::setw(20) << boxes[i];
            }
            off << std::endl;
        }
    } else {
        off << comment << "\n";
    }

    for (size_t i = 0; i < atom_names.size(); i++) {
        off << std::scientific << std::setprecision(8) << std::left << std::setw(5) << atom_names[i] << std::setw(20)
            << coords[3 * i + 0] << std::setw(20) << coords[3 * i + 1] << std::setw(20) << coords[3 * i + 2]
            << std::endl;
    }
}

}  // namespace tools
