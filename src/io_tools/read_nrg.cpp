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

#include "read_nrg.h"

/** @file read_nrg.cpp Contains the implementation of the functions
 * defined in the header file
 */

namespace tools {

void ReadNrg(const char* filename, std::vector<bblock::System>& systems) {
    // Check that filename is not empty
    assert(filename);
    std::ifstream ifs(filename);

    // Check that filename exists
    if (!ifs) {
        std::string text =
            std::string("Could not open NRG file ") + std::string(filename) + std::string(" for reading.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    // Declare variables to store the system number and line number
    size_t lineno(0);
    size_t sysno(0);

    // Read systems until EOF
    while (true) {
        // Read system from file
        bblock::System sys;
        ReadSystem(lineno, ifs, sys);

        // Store system in system vector
        systems.push_back(sys);
        sysno++;

        // Check if next line ends file
        std::streampos oldpos = ifs.tellg();
        std::string line;
        std::getline(ifs, line);
        if (ifs.eof() or line.empty()) {
            break;
        } else {
            ifs.seekg(oldpos);
        }
    }
    return;
}

void ReadSystem(size_t& lineno, std::istream& ifs, bblock::System& sys) {
    // Check we are not at the end of the file
    if (ifs.eof()) return;

    std::string line;
    std::getline(ifs, line);
    lineno++;

    size_t mon_count = 0;

    // Obtain first line. Should be SYSTEM, stating that we are about to
    // start reading a system.
    std::string word;
    std::istringstream iss(line);
    iss >> word;

    // Check that is not a blank line
    if (iss.fail()) {
        std::ostringstream oss;
        oss << "Unexpected text at line " << lineno << " of the NRG file:" << std::endl << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // Check that keyword is system
    if (word != "system") {
        std::ostringstream oss;
        oss << "Expected SYSTEM keyword in line  " << lineno << " of the NRG file." << std::endl
            << "Found " << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    // Read molecules while we do not finish the system
    while (word != "endsys") {
        ReadMolecule(lineno, ifs, sys, mon_count);

        iss.clear();

        std::streampos oldpos = ifs.tellg();

        std::getline(ifs, line);
        lineno++;
        iss.str(line);
        iss >> word;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (word == "molecule") {
            ifs.seekg(oldpos);
            lineno--;
        }
    }

    sys.Initialize();
    return;
}

void ReadMolecule(size_t& lineno, std::istream& ifs, bblock::System& sys, size_t& mon_count) {
    // Check that we are not at the end of the file
    if (ifs.eof()) return;

    // Read line. There should be a word called molecule
    std::string line;
    std::getline(ifs, line);
    lineno++;

    std::string word;
    std::istringstream iss(line);
    iss >> word;

    // Check that tehre is a word to read
    if (iss.fail()) {
        std::ostringstream oss;
        oss << "Unexpected text at line " << lineno << " of the NRG file:" << std::endl << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // Check that the word is molecule
    if (word != "molecule") {
        std::ostringstream oss;
        oss << "Expected MOLECULE keyword in line  " << lineno << " of the NRG file." << std::endl
            << "Found " << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    // Read all monomers of that molecule
    ReadMonomers(lineno, ifs, sys, mon_count);

    return;
}

void ReadMonomers(size_t& lineno, std::istream& ifs, bblock::System& sys, size_t& mon_count) {
    // Check that we are not at the end of the file
    if (ifs.eof()) return;

    // Read all monomers of the molecule
    // Keyword should be monomer
    std::vector<size_t> molec;
    std::string line;
    std::getline(ifs, line);
    lineno++;

    std::string word;
    std::istringstream iss(line);
    iss >> word;

    // Check if there is a keyword
    if (iss.fail()) {
        std::ostringstream oss;
        oss << "Unexpected text at line " << lineno << " of the NRG file:" << std::endl << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // Check that the keyword is monomer
    if (word != "monomer") {
        std::ostringstream oss;
        oss << "Expected MONOMER keyword in line  " << lineno << " of the NRG file." << std::endl
            << "Found " << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    // Read this monomer
    std::string mon_name;
    iss.clear();
    iss.str(line);
    iss >> word >> mon_name;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    while (word != "endmol") {
        // Check that we start with monomer, and that the monomer id is specified
        if (word != "monomer" || mon_name.empty()) {
            std::ostringstream oss;
            oss << "Expected MONOMER keyword in line  " << lineno << " of the NRG file, along with the monomer id."
                << std::endl
                << "Found " << iss.str() << std::endl;
            throw CUException(__func__, __FILE__, __LINE__, oss.str());
        }

        std::transform(mon_name.begin(), mon_name.end(), mon_name.begin(), ::tolower);

        std::vector<std::string> names;
        std::vector<double> xyz;

        std::getline(ifs, line);
        lineno++;
        iss.clear();
        iss.str(line);

        // Read all atoms of that monomer
        while (word != "endmon") {
            std::string name, x1, y1, z1;
            double x, y, z;

            iss >> name;

            // Ensure that atoms and coordinates are specified
            if (iss.eof()) {
                std::ostringstream oss;
                oss << "Expected coordinates or ENDMON in line  " << lineno << " of the NRG file." << std::endl
                    << "Found: " << iss.str() << std::endl;
                throw CUException(__func__, __FILE__, __LINE__, oss.str());
            }

            // Ensure that coordinates are numerical
            try {
                iss >> x1 >> y1 >> z1;
                x = std::stod(x1);
                y = std::stod(y1);
                z = std::stod(z1);
            } catch (std::invalid_argument s) {
                std::ostringstream oss;
                oss << "Expected numerical coordinates in line  " << lineno << " of the NRG file." << std::endl
                    << "Found: " << iss.str() << std::endl;
                throw CUException(__func__, __FILE__, __LINE__, oss.str());
            }

            // Add information to vectors
            names.push_back(name);
            xyz.push_back(x);
            xyz.push_back(y);
            xyz.push_back(z);

            std::getline(ifs, line);
            lineno++;
            iss.clear();
            iss.str(line);

            iss >> word;
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            iss.clear();
            iss.str(line);
        }

        // Add monomer to system
        sys.AddMonomer(xyz, names, mon_name);
        molec.push_back(mon_count);
        mon_count++;

        iss.clear();
        std::getline(ifs, line);
        lineno++;
        iss.str(line);
        iss >> word;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    }

    // Add molecule to system
    sys.AddMolecule(molec);

    return;
}

}  // namespace tools
