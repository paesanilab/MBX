/* Original code from Greg Medders. Adapted by Marc Riera in 20171005*/

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "read-xyz.h"
#include "mt19937.h"
#include "random-rotation.h"

namespace {
const double step_size = 0.1;  // Angstrom
const double r_cut = 2.2;
const double rcut_check = 2.2;
const double kB = 0.0019872041;  // kcal/mol/K from wiki
const double PITIMES2 = 2 * M_PI;
const double max_dist = 40;  // Angstrom
double emin = 0.0;

static std::vector<tools::xyz_frame> set1;
static std::vector<tools::xyz_frame> set2;
static std::vector<tools::xyz_frame> set3;
}  // namespace

////////////////////////////////////////////////////////////////////////////////

inline double dist(const double* r1, const double* r2) {
    const double dx = r1[0] - r2[0];
    const double dy = r1[1] - r2[1];
    const double dz = r1[2] - r2[2];

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

////////////////////////////////////////////////////////////////////////////////

inline double get_mass(std::string at) {
    if (at == "C")
        return 12.0;
    else if (at == "O")
        return 16.0;
    else if (at == "H")
        return 1.0;
    else
        return 1.0;
}

////////////////////////////////////////////////////////////////////////////////

void com_to_origin(double* crd, std::vector<std::string> at_names) {
    // Center the molecule at the center of mass
    // Finc COM coordinate:
    std::vector<double> COM(3, 0.0);
    double tot_mass = 0.0;
    for (size_t i = 0; i < at_names.size(); i++) {
        double mass = get_mass(at_names[i]);
        for (size_t j = 0; j < 3; j++) {
            COM[j] += crd[3 * i + j] * mass;
        }
        tot_mass += mass;
    }

    for (size_t j = 0; j < 3; j++) {
        COM[j] = COM[j] / tot_mass;
    }

    // Center molecule
    for (size_t i = 0; i < at_names.size(); i++)
        for (size_t j = 0; j < 3; j++) crd[3 * i + j] = crd[3 * i + j] - COM[j];
}

////////////////////////////////////////////////////////////////////////////////

void rotate_atom(const double* rot, double* xyz) {
    double old[3] = {xyz[0], xyz[1], xyz[2]};

    for (size_t i = 0; i < 3; ++i) {
        xyz[i] = 0.0;
        for (size_t j = 0; j < 3; ++j) xyz[i] += rot[i * 3 + j] * old[j];
    }
}

////////////////////////////////////////////////////////////////////////////////

void rotate_monomer(kit::mt19937& prg, double* xyz, size_t n) {
    double x[3];
    for (size_t k = 0; k < 3; ++k) x[k] = prg.random_double();

    double rot[9];
    random_rotation(x, rot);

    for (size_t k = 0; k < n; ++k) rotate_atom(rot, xyz + 3 * k);
}

////////////////////////////////////////////////////////////////////////////////

void get_configs(const double& R1, const double& R2, std::vector<tools::xyz_frame>& mon1,
                 std::vector<tools::xyz_frame>& mon2, std::vector<tools::xyz_frame>& mon3, int ns, kit::mt19937& prg,
                 std::ofstream& ofs, std::ofstream& ofa) {
    size_t nat1 = mon1[0].natm;
    size_t nat2 = mon2[0].natm;
    size_t nat3 = mon3[0].natm;
    double m1[3 * nat1], m2[3 * nat2], m3[3 * nat3], trim[3 * (nat1 + nat2 + nat3)];

    std::vector<std::string> atoms(nat1 + nat2 + nat3);

    std::copy(mon1[0].at_name.begin(), mon1[0].at_name.end(), atoms.begin());
    std::copy(mon2[0].at_name.begin(), mon2[0].at_name.end(), atoms.begin() + nat1);
    std::copy(mon3[0].at_name.begin(), mon3[0].at_name.end(), atoms.begin() + nat1 + nat2);

    for (size_t i = 0; i < ns; i++) {
        std::copy(mon1[0].xyz, mon1[0].xyz + 3 * nat1, m1);
        std::copy(mon2[0].xyz, mon2[0].xyz + 3 * nat2, m2);
        std::copy(mon3[0].xyz, mon3[0].xyz + 3 * nat3, m3);

        com_to_origin(m1, mon1[0].at_name);
        com_to_origin(m2, mon2[0].at_name);
        com_to_origin(m3, mon3[0].at_name);

        std::copy(m1, m1 + 3 * nat1, trim);
        std::copy(m2, m2 + 3 * nat2, trim + 3 * nat1);
        std::copy(m3, m3 + 3 * nat3, trim + 3 * nat1 + 3 * nat2);

        rotate_monomer(prg, trim, nat1);
        rotate_monomer(prg, trim + 3 * nat1, nat2);
        rotate_monomer(prg, trim + 3 * nat1 + 3 * nat2, nat3);

        // Translate COMs away along X axis: m2 R1 distance, and m3 R2 distance
        for (size_t j = 0; j < nat2; j++) {
            trim[3 * j + 3 * nat1] += R1;
        }

        for (size_t j = 0; j < nat3; j++) {
            trim[3 * j + 3 * (nat1 + nat2)] += R2;
        }

        // Get a random angle and move m3 so the angle 213 is actually what it is supposed to be
        double ang = prg.random_double() * M_PI;
        double sang = sin(ang);
        double cang = cos(ang);

        double dx = R2 * (cang - 1);
        double dy = R2 * sang;

        // Move monomer 3
        for (size_t j = 0; j < nat3; j++) {
            trim[3 * j + 3 * (nat1 + nat2)] += dx;
            trim[3 * j + 3 * (nat1 + nat2) + 1] += dy;
        }

        tools::print_xyz(ofs, atoms, trim, std::to_string(ang));
        ofa << std::scientific << std::setw(20) << std::setprecision(8) << ang << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc < 7) {
        std::cout << "Usage: " << argv[0]
                  << " <eq_geom1.xyz> <eq_geom2.xyz> <eq_geom3.xyz> <n_samples_x_point> <r12_start> <r12_end> "
                  << std::endl;
        return EXIT_FAILURE;
    }

    // 1. Load Configurations
    const double dR = step_size;

    kit::mt19937 prg(27606);
    int ns = 0;
    double r12 = 0;
    double r13 = 0;

    std::cout << "\nrcut = " << r_cut << std::endl;
    try {
        // Checking each one of the input files in the command line
        tools::read_xyz(argv[1], set1);
        tools::read_xyz(argv[2], set2);
        tools::read_xyz(argv[3], set3);
        ns = atoi(argv[4]);
        r12 = atof(argv[5]);
        r13 = atof(argv[6]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // 2. Do trapezoidal rule for numerical integration of radial coord
    //    Don't start at R = 0 because first (and final) points are zero
    //    We do a double loop looping from distance 1-2 and 1-3

    size_t count = 0;
    size_t confsxr12 = int((max_dist + dR / 2.0) / dR);
    double R1 = r12;
    count = confsxr12 * (int(R1 / dR) - 1) + 1;
    double R2 = r13;
    count += int(R2 / dR) - 1;
    std::cout << "Working on d12 = " << R1 << " and d13 = " << R2 << " ; index " << count << std::endl;
    std::string index = std::to_string(count) + ".xyz";
    std::string angles = std::to_string(count) + ".angles";
    std::ofstream ofs(index.c_str());
    std::ofstream ofa(angles.c_str());
    get_configs(R1, R2, set1, set2, set3, ns, prg, ofs, ofa);
    ofs.close();
    ofa.close();

    return 0;
}
