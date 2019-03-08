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

#include "write_nrg.h"

/**
 * @file write_nrg.cpp
 * @brief Contains the function implementations of teh header file
 */

namespace tools {

void WriteNrg(const char* filename, std::vector<bblock::System> systems) {
    // Ensure that filename is not empty
    assert(filename);

    // Ensure that output file can be opened
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("could not open NRG file for writting");

    // Save system information in energy file
    SaveNrg(ofs, systems);
    return;
}

void SaveNrg(std::ostream& os, std::vector<bblock::System> systems) {
    // Loop over all systems
    for (size_t i = 0; i < systems.size(); i++) {
        // Obtain system properties
        std::vector<double> xyz = systems[i].GetXyz();
        std::vector<std::string> an = systems[i].GetAtomNames();
        // Write system
        os << "SYSTEM " << i << std::endl;

        // Write all moelcules of the system
        for (size_t j = 0; j < systems[i].GetNumMol(); j++) {
            os << "MOLECULE " << i << "." << j << std::endl;
            std::vector<size_t> m = systems[i].GetMolecule(j);

            // Write all monomers of the molecule
            for (size_t k = 0; k < m.size(); k++) {
                std::string mon_name = systems[i].GetMonId(m[k]);
                os << "MONOMER " << mon_name << std::endl;
                size_t ns = systems[i].GetMonNumAt(m[k]);
                size_t first_ind = systems[i].GetFirstInd(m[k]);

                // Write all atoms of the monomer
                for (size_t n = 0; n < ns; n++) {
                    os << std::scientific;
                    os << std::setprecision(8);
                    os << std::setw(5) << std::left << an[n + first_ind] << std::setw(20) << std::right
                       << xyz[3 * (n + first_ind)] << std::setw(20) << std::right << xyz[3 * (n + first_ind) + 1]
                       << std::setw(20) << std::right << xyz[3 * (n + first_ind) + 2] << std::endl;
                }

                os << "ENDMON " << std::endl;
            }
            os << "ENDMOL " << std::endl;
        }
        os << "ENDSYS " << std::endl;
    }
}

}  // namespace tools
