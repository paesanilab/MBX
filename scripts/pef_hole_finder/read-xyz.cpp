#include <cassert>

#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "read-xyz.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

////////////////////////////////////////////////////////////////////////////////

void read_frame(size_t& lineno, std::istream& is, tools::xyz_frame& frame, bool do_bind) {
    assert(is && !is.eof());
    assert(frame.natm == 0);  // empty

    std::string line;
    std::getline(is, line);

    if (is.eof()) return;

    // number of atoms
    size_t natm;
    std::istringstream iss(line);
    iss >> natm;

    frame.natm = natm;
    delete frame.xyz;
    frame.xyz = new double[3 * frame.natm];

    std::getline(is, line);  // Extract energy, if provided
    ++lineno;

    if (line.length() != 0) {
        double tmpA;

        iss.clear();
        iss.str(line);
        iss >> tmpA;
        if (iss.fail()) {
            std::ostringstream oss;
            oss << "unexpected text at line " << lineno << " of the XYZ file:" << std::endl << iss.str() << std::endl;
            throw std::runtime_error(oss.str());
        }

        frame.E = tmpA;
        if (do_bind) {
            double tmpB, tmpC, tmpD;
            iss >> tmpB >> tmpC >> tmpD;
            if (iss.fail()) {
                std::ostringstream oss;
                oss << "unexpected text at line " << lineno << " of the XYZ file:" << std::endl
                    << iss.str() << std::endl;
                throw std::runtime_error(oss.str());
            }
            frame.Ebind = tmpA;
            frame.E = tmpB;
            frame.e1b[0] = tmpC;
            frame.e1b[1] = tmpD;
        } else {
            frame.Ebind = 10000;
            frame.e1b[0] = 0.0;
            frame.e1b[1] = 0.0;
        }
    } else {
        frame.E = 10000;
        frame.Ebind = 10000;
        frame.e1b[0] = 0.0;
        frame.e1b[1] = 0.0;
    }

    ++lineno;

    for (size_t n = 0; n < natm; ++n) {
        if (is.eof()) {
            std::ostringstream oss;
            oss << "unexpected EOF at line " << lineno << " of the XYZ file";
            throw std::runtime_error(oss.str());
        }

        std::getline(is, line);
        ++lineno;

        std::string name;
        double x, y, z;

        iss.clear();
        iss.str(line);
        iss >> name >> x >> y >> z;
        if (iss.fail()) {
            std::ostringstream oss;
            oss << "unexpected text at line " << lineno << " of the XYZ file";
            throw std::runtime_error(oss.str());
        }
        frame.at_name.push_back(name);

        frame.xyz[3 * n + 0] = x;
        frame.xyz[3 * n + 1] = y;
        frame.xyz[3 * n + 2] = z;
    }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace tools {

////////////////////////////////////////////////////////////////////////////////

size_t read_xyz(const char* filename, std::vector<xyz_frame>& frames, bool do_bind) {
    assert(filename);

    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open XYZ file for reading");

    size_t lineno(0);
    size_t frameno(0);

    while (!ifs.eof()) {
        xyz_frame frame;

        read_frame(lineno, ifs, frame, do_bind);

        if (frame.natm > 0) {
            frames.push_back(frame);
            ++frameno;
        }
    }

    return frameno;
}

size_t read_connectivity(const char* filename, std::vector<int*>& bonds, std::vector<int*>& angles,
                         std::vector<int*>& improper_dh, std::vector<int*>& dihedrals, std::vector<int*>& dh_params) {
    assert(filename);
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open connectivity file for reading");

    bonds.clear();
    angles.clear();
    dihedrals.clear();
    dh_params.clear();
    improper_dh.clear();

    //  size_t lineno(0);
    while (!ifs.eof()) {
        assert(ifs && !ifs.eof());

        std::string line;
        std::getline(ifs, line);

        if (ifs.eof()) return 0;

        std::string kindacon;
        std::istringstream iss(line);
        iss >> kindacon;  // D (dihedral), B (bond) or A (angle)

        if (kindacon == "B") {
            int* b = new int[3];
            iss >> b[0] >> b[1] >> b[2];
            bonds.push_back(b);
        } else if (kindacon == "A") {
            int* a = new int[4];
            iss >> a[0] >> a[1] >> a[2] >> a[3];
            angles.push_back(a);
        } else if (kindacon == "I") {
            int* a = new int[5];
            iss >> a[0] >> a[1] >> a[2] >> a[3] >> a[4];
            improper_dh.push_back(a);
        } else if (kindacon == "D") {
            int* d = new int[5];
            int* p = new int[2];
            iss >> d[0] >> d[1] >> d[2] >> d[3] >> d[4] >> p[0] >> p[1];
            dihedrals.push_back(d);
            dh_params.push_back(p);
        }
    }
    return 1;
}

size_t read_connectivity(const char* filename, std::vector<int*>& bonds, std::vector<int*>& angles,
                         std::vector<int*>& improper_dh, std::vector<int*>& dihedrals, std::vector<int*>& dh_params,
                         const int use_typefunc) {
    assert(filename);
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open connectivity file for reading");

    bonds.clear();
    angles.clear();
    dihedrals.clear();
    dh_params.clear();
    improper_dh.clear();

    //  size_t lineno(0);
    while (!ifs.eof()) {
        assert(ifs && !ifs.eof());

        std::string line;
        std::getline(ifs, line);

        if (ifs.eof()) return 0;

        std::string kindacon;
        std::istringstream iss(line);
        iss >> kindacon;  // D (dihedral), B (bond) or A (angle)

        if (kindacon == "B") {
            int* b = new int[4];
            iss >> b[0] >> b[1] >> b[2] >> b[3];
            bonds.push_back(b);
        } else if (kindacon == "A") {
            int* a = new int[5];
            iss >> a[0] >> a[1] >> a[2] >> a[3] >> a[4];
            angles.push_back(a);
        } else if (kindacon == "I") {
            int* a = new int[6];
            iss >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5];
            improper_dh.push_back(a);
        } else if (kindacon == "D") {
            int* d = new int[6];
            int* p = new int[2];
            iss >> d[0] >> d[1] >> d[2] >> d[3] >> d[4] >> p[0] >> p[1] >> d[5];
            dihedrals.push_back(d);
            dh_params.push_back(p);
        }
    }
    return 1;
}

size_t read_intra_lj(const char* filename, std::vector<int*>& pairs, std::vector<double*>& lj) {
    assert(filename);
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open intramolecular file for reading");

    pairs.clear();
    lj.clear();

    while (!ifs.eof()) {
        assert(ifs && !ifs.eof());

        std::string line;
        std::getline(ifs, line);

        if (ifs.eof()) return 0;

        std::istringstream iss(line);

        int* b = new int[2];
        double* c = new double[3];

        iss >> b[0] >> b[1] >> c[0] >> c[1] >> c[2];
        pairs.push_back(b);
        lj.push_back(c);
    }
    return 1;
}

size_t read_c6(const char* filename, std::vector<int*>& pairs, std::vector<double>& c6) {
    assert(filename);
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open c6 file for reading");

    pairs.clear();
    c6.clear();

    while (!ifs.eof()) {
        assert(ifs && !ifs.eof());

        std::string line;
        std::getline(ifs, line);

        if (ifs.eof()) return 0;

        std::istringstream iss(line);

        int* b = new int[2];
        double c;

        iss >> b[0] >> b[1] >> c;
        pairs.push_back(b);
        c6.push_back(c);
    }
    return 1;
}

size_t read_molinfo(const char* filename, std::vector<double>& chg, std::vector<double>& pol,
                    std::vector<int>& atomtype) {
    assert(filename);
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open connectivity file for reading");

    chg.clear();
    pol.clear();
    atomtype.clear();

    while (!ifs.eof()) {
        assert(ifs && !ifs.eof());

        std::string line;
        std::getline(ifs, line);

        if (ifs.eof()) return 0;

        std::istringstream iss(line);
        double c, p;
        int a;

        iss >> c >> p >> a;

        chg.push_back(c);
        pol.push_back(p);
        atomtype.push_back(a);
    }

    return 1;
}

size_t read_molinfo(const char* filename, std::vector<double>& chg, std::vector<double>& pol,
                    std::vector<double>& polfac, std::vector<int>& atomtype) {
    assert(filename);
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("could not open connectivity file for reading");

    chg.clear();
    pol.clear();
    polfac.clear();
    atomtype.clear();

    while (!ifs.eof()) {
        assert(ifs && !ifs.eof());

        std::string line;
        std::getline(ifs, line);

        if (ifs.eof()) return 0;

        std::istringstream iss(line);
        double c, p, pf;
        int a;

        iss >> c >> p >> pf >> a;

        chg.push_back(c);
        pol.push_back(p);
        polfac.push_back(pf);
        atomtype.push_back(a);
    }
    return 1;
}

void print_xyz(std::ostream& os, std::vector<std::string> elements, const double* x, std::string comment) {
    std::ios::fmtflags saved_flags = os.flags();

    const size_t natm = elements.size();

    if (!comment.empty())
        os << natm << std::endl << comment << std::endl;
    else
        os << natm << std::endl << std::endl;
    for (size_t n = 0; n < natm; ++n) {
        const size_t n3 = n * 3;

        std::string name = elements[n];

        os << std::setw(5) << std::left << name << std::setw(25) << std::right << x[n3 + 0] << std::setw(25)
           << std::right << x[n3 + 1] << std::setw(25) << std::right << x[n3 + 2] << std::endl;
    }

    os.flags(saved_flags);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace tools

////////////////////////////////////////////////////////////////////////////////

namespace kit {
namespace io {
void print_xyz(std::ostream& os, const char* atom, const double* x) {
    std::ios::fmtflags saved_flags = os.flags();

    os << std::setw(5) << std::left << atom << std::setprecision(9) << std::scientific << std::setw(18) << std::right
       << x[0] << std::setw(18) << std::right << x[1] << std::setw(18) << std::right << x[2] << std::endl;

    os.flags(saved_flags);
}
}  // namespace io
}  // namespace kit
