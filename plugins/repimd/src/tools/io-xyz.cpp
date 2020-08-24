#include <cassert>

#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "io-xyz.h"

namespace kit { namespace io {

void save_xyz(std::ostream&                   os,
              const std::string&              comment,
              const std::vector<std::string>& elements,
              const std::vector<double>&      xyz)
{
    assert(3*elements.size() == xyz.size());

    std::ios::fmtflags saved_flags = os.flags();

    os << std::setw(5) << elements.size()
       << '\n' << comment << '\n'
       << std::setprecision(9) << std::scientific;

    for (size_t n = 0; n < elements.size(); ++n)
        os << std::setw(5) << std::left << elements[n]
           << std::setw(18) << std::right << xyz[3*n + 0]
           << std::setw(18) << std::right << xyz[3*n + 1]
           << std::setw(18) << std::right << xyz[3*n + 2]
           << '\n';

    os.flags(saved_flags);
}

void load_xyz(std::istream&             is,
              std::string&              comment,
              std::vector<std::string>& elements,
              std::vector<double>&      xyz)
{
    comment.clear();
    elements.clear();
    xyz.clear();

    std::string line;
    std::getline(is, line);

    if (is.eof())
        return;

    size_t natoms;
    std::istringstream iss(line);
    iss >> natoms;

    xyz.reserve(3*natoms);
    elements.reserve(natoms);

    std::getline(is, comment);

    size_t lineno(2), n(0);
    while (!is.eof()) {
        std::getline(is, line);
        ++lineno;

        if (line.length() == 0)
            continue;

        std::string element;
        double x, y, z;

        iss.clear();
        iss.str(line);
        iss >> element >> x >> y >> z;
        if (iss.fail()) {
            std::ostringstream oss;
            oss << "unexpected text at line " << lineno << " of the XYZ stream";
            throw std::runtime_error(oss.str());
        }

        elements.push_back(element);

        xyz.push_back(x);
        xyz.push_back(y);
        xyz.push_back(z);

        if (++n == natoms)
            break;
    }

    if (n != natoms) {
        std::ostringstream oss;
        oss << "wrong number of atoms ("
            << n << " instead of " << natoms
            << ") in the XYZ stream";
        throw std::runtime_error(oss.str());
    }
}

}} // namespace kit::io
