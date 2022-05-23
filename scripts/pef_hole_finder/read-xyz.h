#ifndef READ_XYZ_H
#define READ_XYZ_H

#include <vector>
#include <iostream>
#include <string>

namespace tools {

struct xyz_frame {
    inline xyz_frame();
    inline xyz_frame(const xyz_frame&);
    inline ~xyz_frame();
    inline xyz_frame& operator=(const xyz_frame&);
    inline xyz_frame(const double* coords, const std::vector<std::string> names, const int n_atom);

    size_t natm;
    double* xyz;  // 3*natm
    double E;
    double Ebind;
    double e1b[2];
    std::vector<std::string> at_name;
};

// returns number of frames loaded
size_t read_xyz(const char*, std::vector<xyz_frame>&, bool do_bind = false);

size_t read_connectivity(const char*, std::vector<int*>&, std::vector<int*>&, std::vector<int*>&, std::vector<int*>&,
                         std::vector<int*>&);

size_t read_connectivity(const char*, std::vector<int*>&, std::vector<int*>&, std::vector<int*>&, std::vector<int*>&,
                         std::vector<int*>&, const int);

size_t read_intra_lj(const char*, std::vector<int*>&, std::vector<double*>&);

size_t read_c6(const char*, std::vector<int*>&, std::vector<double>&);

size_t read_molinfo(const char*, std::vector<double>&, std::vector<double>&, std::vector<int>&);

size_t read_molinfo(const char*, std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<int>&);

size_t read_dispersion(const char*, std::vector<int>&, std::vector<int>&, std::vector<double>&);

void print_xyz(std::ostream&, std::vector<std::string>, const double*, std::string comment = "");

inline xyz_frame::xyz_frame()
    : natm(0),
      xyz(0),
      E(0),
      Ebind(0),
      at_name(0)  //, e1b(0)
{}

inline xyz_frame::xyz_frame(const xyz_frame& o) {
    natm = o.natm;
    E = o.E;
    Ebind = o.Ebind;
    at_name = o.at_name;
    std::copy(o.e1b, o.e1b + 2, e1b);

    if (natm > 0) {
        xyz = new double[3 * natm];
        std::copy(o.xyz, o.xyz + 3 * natm, xyz);
    }
}

inline xyz_frame::xyz_frame(const double* coords, const std::vector<std::string> names, const int n_atom) {
    natm = n_atom;
    E = 0.0;
    Ebind = 0.0;
    at_name = names;
    e1b[0] = e1b[1] = 0.0;
    if (natm > 0) {
        xyz = new double[3 * natm];
        std::copy(coords, coords + 3 * natm, xyz);
    }
}

inline xyz_frame::~xyz_frame() {
    if (natm > 0) delete[] xyz;
}

inline xyz_frame& xyz_frame::operator=(const xyz_frame& o) {
    if (this != &o) {
        if (natm > 0) delete[] xyz;

        natm = o.natm;
        E = o.E;
        Ebind = o.Ebind;
        at_name = o.at_name;
        std::copy(o.e1b, o.e1b + 2, e1b);
        if (natm > 0) {
            xyz = new double[3 * natm];
            std::copy(o.xyz, o.xyz + 3 * natm, xyz);
        }
    }

    return *this;
}

}  // namespace tools

namespace kit {
namespace io {
void print_xyz(std::ostream& os, const char* atom, const double* x);
}
}  // namespace kit

#endif  // READ_XYZ_H
