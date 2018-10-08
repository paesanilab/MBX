#include "read_nrg.h"

/** @file read_nrg.cpp Contains the implementation of the functions
 * defined in the header file
 */

namespace tools {

void ReadNrg(char* filename, std::vector<bblock::System>& systems) {
    assert(filename);
    std::ifstream ifs(filename);

    if (!ifs) {
        std::string text =
            std::string("Could not open NRG file ") + std::string(filename) + std::string(" for reading.");
        throw CUException(__func__, __FILE__, __LINE__, text);
    }

    size_t lineno(0);
    size_t sysno(0);

    while (true) {
        bblock::System sys;
        ReadSystem(lineno, ifs, sys);

        systems.push_back(sys);
        sysno++;

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
    assert(ifs);

    if (ifs.eof()) return;

    std::string line;
    std::getline(ifs, line);
    lineno++;

    size_t mon_count = 0;

    std::string word;
    std::istringstream iss(line);
    iss >> word;

    if (iss.fail()) {
        std::ostringstream oss;
        oss << "Unexpected text at line " << lineno << " of the NRG file:" << std::endl << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    if (word != "system") {
        std::ostringstream oss;
        oss << "Expected SYSTEM keyword in line  " << lineno << " of the NRG file." << std::endl
            << "Found " << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

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
    assert(ifs);

    if (ifs.eof()) return;

    std::string line;
    std::getline(ifs, line);
    lineno++;

    std::string word;
    std::istringstream iss(line);
    iss >> word;

    if (iss.fail()) {
        std::ostringstream oss;
        oss << "Unexpected text at line " << lineno << " of the NRG file:" << std::endl << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (word != "molecule") {
        std::ostringstream oss;
        oss << "Expected MOLECULE keyword in line  " << lineno << " of the NRG file." << std::endl
            << "Found " << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    ReadMonomers(lineno, ifs, sys, mon_count);

    return;
}

void ReadMonomers(size_t& lineno, std::istream& ifs, bblock::System& sys, size_t& mon_count) {
    assert(ifs);

    if (ifs.eof()) return;

    std::vector<size_t> molec;
    std::string line;
    std::getline(ifs, line);
    lineno++;

    std::string word;
    std::istringstream iss(line);
    iss >> word;

    if (iss.fail()) {
        std::ostringstream oss;
        oss << "Unexpected text at line " << lineno << " of the NRG file:" << std::endl << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (word != "monomer") {
        std::ostringstream oss;
        oss << "Expected MONOMER keyword in line  " << lineno << " of the NRG file." << std::endl
            << "Found " << iss.str() << std::endl;
        throw CUException(__func__, __FILE__, __LINE__, oss.str());
    }

    std::string mon_name;
    iss.clear();
    iss.str(line);
    iss >> word >> mon_name;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    while (word != "endmol") {
        if (word != "monomer" || mon_name.empty()) {
            std::ostringstream oss;
            oss << "Expected MONOMER keyword in line  " << lineno << " of the NRG file." << std::endl
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

        while (word != "endmon") {
            std::string name;
            double x, y, z;

            iss >> name;

            if (iss.eof()) {
                std::ostringstream oss;
                oss << "Expected coordinates or ENDMON in line  " << lineno << " of the NRG file." << std::endl
                    << "Found: " << iss.str() << std::endl;
                throw CUException(__func__, __FILE__, __LINE__, oss.str());
            }

            try {
                iss >> x >> y >> z;
            } catch (std::string s) {
                std::ostringstream oss;
                oss << "Expected numerical coordinates in line  " << lineno << " of the NRG file." << std::endl
                    << "Found: " << iss.str() << std::endl;
                throw CUException(__func__, __FILE__, __LINE__, oss.str());
            }

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

    sys.AddMolecule(molec);

    return;
}

}  // namespace tools
