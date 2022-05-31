/* Original code from Greg Medders. Adapted by Marc Riera in 20171005*/

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

#include "read-xyz.h"
#include "mt19937.h"
#include "random-rotation.h"

#include "bblock/system.h"
#include "io_tools/read_xyz.h"
#include "io_tools/read_nrg.h"
#include "io_tools/write_xyz.h"

namespace {
const double step_size = 0.05;  // Angstrom
const double rcut_check = 1.2;
const double min_dist = 1.0;
const double max_dist = 4.5;  // Angstrom
double emin = 0.0;
const double max_be = 80.0;
double max_abs_poly_e = 10.0;

static std::vector<tools::xyz_frame> set1;
static std::vector<tools::xyz_frame> set2;
static std::vector<tools::xyz_frame> set3;
}  // namespace

////////////////////////////////////////////////////////////////////////////////

inline double dist(double* r1, double* r2) {
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

void get_configs(double R1, double R2, std::vector<tools::xyz_frame>& mon1, std::vector<tools::xyz_frame>& mon2,
                 std::vector<tools::xyz_frame>& mon3, int ns, kit::mt19937& prg,
                 std::vector<std::vector<double> >& configs) {
    size_t number_of_configs1 = mon1.size();
    size_t number_of_configs2 = mon2.size();
    size_t number_of_configs3 = mon3.size();

    configs = std::vector<std::vector<double> >(ns);
    for (size_t i = 0; i < ns; i++) {
        size_t idx1 = rand() % number_of_configs1;
        size_t idx2 = rand() % number_of_configs2;
        size_t idx3 = rand() % number_of_configs3;
        size_t nat1 = mon1[idx1].natm;
        size_t nat2 = mon2[idx2].natm;
        size_t nat3 = mon3[idx3].natm;
        double m1[3 * nat1], m2[3 * nat2], m3[3 * nat3], trim[3 * (nat1 + nat2 + nat3)];

        std::copy(mon1[idx1].xyz, mon1[idx1].xyz + 3 * nat1, m1);
        std::copy(mon2[idx2].xyz, mon2[idx2].xyz + 3 * nat2, m2);
        std::copy(mon3[idx3].xyz, mon3[idx3].xyz + 3 * nat2, m3);

        com_to_origin(m1, mon1[idx1].at_name);
        com_to_origin(m2, mon2[idx2].at_name);
        com_to_origin(m3, mon3[idx3].at_name);

        std::copy(m1, m1 + 3 * nat1, trim);
        std::copy(m2, m2 + 3 * nat2, trim + 3 * nat1);
        std::copy(m3, m3 + 3 * nat3, trim + 3 * nat1 + 3 * nat2);

        rotate_monomer(prg, trim, nat1);
        rotate_monomer(prg, trim + 3 * nat1, nat2);
        rotate_monomer(prg, trim + 3 * nat1 + 3 * nat2, nat3);

        // Translate COMs away along X axis
        for (size_t j = 0; j < nat2; j++) {
            trim[3 * j + 3 * nat1] += R1;
        }

        for (size_t j = 0; j < nat3; j++) {
            trim[3 * j + 3 * nat1 + 3 * nat2] += R2;
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

        std::vector<double> trimv(trim, trim + 3 * (nat1 + nat2 + nat3));
        configs[i] = trimv;
    }
}

void check_configs(const double rcheck, size_t nat1, size_t nat2, size_t nat3,
                   std::vector<std::vector<double> > confs_to_check, std::vector<std::vector<double> >& confs_checked) {
    size_t ns = confs_to_check.size();
    confs_checked.clear();

    for (size_t i = 0; i < ns; i++) {
        std::vector<double> m1(confs_to_check[i].data(), confs_to_check[i].data() + 3 * nat1);
        std::vector<double> m2(confs_to_check[i].data() + 3 * nat1, confs_to_check[i].data() + 3 * (nat1 + nat2));
        std::vector<double> m3(confs_to_check[i].data() + 3 * (nat1 + nat2),
                               confs_to_check[i].data() + 3 * (nat1 + nat2 + nat3));

        bool config_ok = true;
        // Check dimer 12
        for (size_t j = 0; j < nat1; j++) {
            for (size_t k = 0; k < nat2; k++) {
                double d = dist(m1.data() + 3 * j, m2.data() + 3 * k);
                if (d < rcheck) {
                    config_ok = false;
                    break;
                }
            }
            if (!config_ok) break;
        }

        if (!config_ok) continue;

        // Check dimer 13
        for (size_t j = 0; j < nat1; j++) {
            for (size_t k = 0; k < nat3; k++) {
                double d = dist(m1.data() + 3 * j, m3.data() + 3 * k);
                if (d < rcheck) {
                    config_ok = false;
                    break;
                }
            }
            if (!config_ok) break;
        }

        if (!config_ok) continue;

        // Check dimer 23
        for (size_t j = 0; j < nat2; j++) {
            for (size_t k = 0; k < nat3; k++) {
                double d = dist(m2.data() + 3 * j, m3.data() + 3 * k);
                if (d < rcheck) {
                    config_ok = false;
                    break;
                }
            }
            if (!config_ok) break;
        }

        if (config_ok) confs_checked.push_back(confs_to_check[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc < 14) {
        std::cout << "Usage: " << argv[0]
                  << " <mon1_set.xyz> <mon2_set.xyz> <mon3_set.xyz> <n_samples_x_point> <mon1.nrg> <mon2.nrg> "
                     "<mon3.nrg> <dim12.nrg> <dim13.nrg> <dim23.nrg> <trim.nrg> <mbx_mb.json> <mbx_no3b.json>"
                  << std::endl;
        return EXIT_FAILURE;
    }

    const double dR = step_size;

    kit::mt19937 prg(19273);
    int ns = 0;

    // Initialize random seed
    srand(time(NULL));

    std::vector<bblock::System> sys_m1;
    std::vector<bblock::System> sys_m2;
    std::vector<bblock::System> sys_m3;
    std::vector<bblock::System> sys_d12;
    std::vector<bblock::System> sys_d13;
    std::vector<bblock::System> sys_d23;
    std::vector<bblock::System> sys_t;
    std::vector<bblock::System> sys_no3b_t;

    try {
        // Checking each one of the input files in the command line
        tools::read_xyz(argv[1], set1);
        tools::read_xyz(argv[2], set2);
        tools::read_xyz(argv[3], set3);
        ns = atoi(argv[4]);
        tools::ReadNrg(argv[5], sys_m1);
        tools::ReadNrg(argv[6], sys_m2);
        tools::ReadNrg(argv[7], sys_m3);
        tools::ReadNrg(argv[8], sys_d12);
        tools::ReadNrg(argv[9], sys_d13);
        tools::ReadNrg(argv[10], sys_d23);
        tools::ReadNrg(argv[11], sys_t);
        tools::ReadNrg(argv[11], sys_no3b_t);

        sys_m1[0].SetUpFromJson(argv[12]);
        sys_m2[0].SetUpFromJson(argv[12]);
        sys_m3[0].SetUpFromJson(argv[12]);
        sys_d12[0].SetUpFromJson(argv[12]);
        sys_d13[0].SetUpFromJson(argv[12]);
        sys_d23[0].SetUpFromJson(argv[12]);
        sys_t[0].SetUpFromJson(argv[12]);
        sys_no3b_t[0].SetUpFromJson(argv[13]);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    size_t nat1 = set1[0].natm;
    size_t nat2 = set2[0].natm;
    size_t nat3 = set3[0].natm;
    size_t nat_tot = nat1 + nat2 + nat3;

    std::vector<std::string> atom_names(nat_tot);
    std::copy(set1[0].at_name.begin(), set1[0].at_name.end(), atom_names.begin());
    std::copy(set2[0].at_name.begin(), set2[0].at_name.end(), atom_names.begin() + nat1);
    std::copy(set3[0].at_name.begin(), set3[0].at_name.end(), atom_names.begin() + nat1 + nat2);

    std::vector<double> box;

    // std::string index = std::to_string(count) + ".xyz";
    // std::ofstream ofs(index.c_str());
    std::ofstream ofs("holes.xyz");
    std::ofstream ofsh("holes_high_energy.xyz");
    std::ofstream ofse("high_3b_discrepancy.xyz");
    size_t nholes = 0;
    size_t nholes_highe = 0;
    size_t nholes_big_classical_discrepancy = 0;
    long long unsigned int ntested = 0;
    long long unsigned int ntotal = 0;

    for (double R1 = min_dist; R1 < max_dist + dR / 2.0; R1 += dR) {
        for (double R2 = min_dist; R2 < max_dist + dR / 2.0; R2 += dR) {
            // Generate config pool
            std::cout << "Working on R1 = " << R1 << " and R2 = " << R2 << std::endl;
            std::vector<std::vector<double> > confs;
            get_configs(R1, R2, set1, set2, set3, ns, prg, confs);

            ntotal += ns;

            // Filter config pool according to rmin_check. Configs in which at least
            // one interatomic distance is lower than rcut_check, will be excluded from analysis.
            std::vector<std::vector<double> > confs_filtered;
            check_configs(rcut_check, nat1, nat2, nat3, confs, confs_filtered);

            ntested += confs_filtered.size();

            // Get the energies with MB-nrg and NO3B-nrg
            std::cout << confs_filtered.size() << " filtered configurations" << std::endl;
            for (size_t j = 0; j < confs_filtered.size(); j++) {
                std::vector<double> xyz1(confs_filtered[j].begin(), confs_filtered[j].begin() + 3 * nat1);
                std::vector<double> xyz2(confs_filtered[j].begin() + 3 * nat1,
                                         confs_filtered[j].begin() + 3 * nat1 + 3 * nat2);
                std::vector<double> xyz3(confs_filtered[j].begin() + 3 * nat1 + 3 * nat2, confs_filtered[j].end());

                std::vector<double> xyz12(3 * (nat1 + nat2), 0.0);
                std::vector<double> xyz13(3 * (nat1 + nat3), 0.0);
                std::vector<double> xyz23(3 * (nat2 + nat3), 0.0);

                for (size_t k = 0; k < 3 * nat1; k++) {
                    xyz12[k] = xyz1[k];
                    xyz13[k] = xyz1[k];
                }

                for (size_t k = 0; k < 3 * nat2; k++) {
                    xyz12[k + 3 * nat1] = xyz2[k];
                    xyz23[k] = xyz2[k];
                }

                for (size_t k = 0; k < 3 * nat3; k++) {
                    xyz13[k + 3 * nat1] = xyz3[k];
                    xyz23[k + 3 * nat2] = xyz3[k];
                }

                // Set xyz arrays to MBX systems
                sys_m1[0].SetRealXyz(xyz1);
                sys_m2[0].SetRealXyz(xyz2);
                sys_m3[0].SetRealXyz(xyz3);
                sys_d12[0].SetRealXyz(xyz12);
                sys_d13[0].SetRealXyz(xyz13);
                sys_d23[0].SetRealXyz(xyz23);
                sys_t[0].SetRealXyz(confs_filtered[j]);
                sys_no3b_t[0].SetRealXyz(confs_filtered[j]);

                // Get the energies. We will look at
                double e1 = sys_m1[0].Energy(false);
                double e2 = sys_m2[0].Energy(false);
                double e3 = sys_m3[0].Energy(false);
                double e12 = sys_d12[0].Energy(false);
                double e13 = sys_d13[0].Energy(false);
                double e23 = sys_d23[0].Energy(false);
                double et = sys_t[0].Energy(false);
                double et_no3b = sys_no3b_t[0].Energy(false);

                double e3b_wpoly = et - e12 - e13 - e23 + e1 + e2 + e3;
                double e3b_nopoly = et_no3b - e12 - e13 - e23 + e1 + e2 + e3;

                bool ishole = false;

                // A dimer will be considered a hole if the 2b polynomials behave weird
                // It is considered weird if total repulsion < -20.0 kcal/mol

                // MB-nrg
                double poly_e = sys_t[0].ThreeBodyEnergy(false);

                // Config is considered a hole if total repulsion for MB-nrg is lower than -20 kcal/mol
                if (std::fabs(poly_e) > max_abs_poly_e) {
                    ishole = true;
                }

                // If it is a hole, write it to the hole file
                if (ishole) {
                    if (et_no3b < max_be) {
                        nholes += 1;
                        std::stringstream ss;
                        ss << "Hole " << std::setw(7) << nholes << "  3B(wPOLY) = " << std::scientific
                           << std::setprecision(3) << std::setw(10) << e3b_wpoly << "  3B(TTM) = " << std::scientific
                           << std::setprecision(3) << std::setw(10) << e3b_nopoly << "  BE = " << std::scientific
                           << std::setprecision(3) << std::setw(10) << et_no3b;
                        std::string comment = ss.str();
                        tools::WriteXYZ(ofs, atom_names, confs_filtered[j], box, comment, false);
                    } else {
                        nholes_highe += 1;
                        std::stringstream ss;
                        ss << "High energy hole " << std::setw(7) << nholes_highe << "  3B(wPOLY) = " << std::scientific
                           << std::setprecision(3) << std::setw(10) << e3b_wpoly << "  3B(TTM) = " << std::scientific
                           << std::setprecision(3) << std::setw(10) << e3b_nopoly << "  BE = " << std::scientific
                           << std::setprecision(3) << std::setw(10) << et_no3b;
                        std::string comment = ss.str();
                        tools::WriteXYZ(ofsh, atom_names, confs_filtered[j], box, comment, false);
                    }
                } else {
                    double err = 1.0;
                    double max_error_allowed = std::max(1.01, std::fabs(e3b_nopoly / 2.0));
                    try {
                        err = std::fabs(e3b_wpoly - e3b_nopoly);
                    } catch (...) {
                        err = 1.1;
                    }

                    if (err > max_error_allowed && et_no3b < max_be) {
                        nholes_big_classical_discrepancy += 1;
                        std::stringstream ss;
                        ss << "High discrepancy hole " << std::setw(7) << nholes_big_classical_discrepancy
                           << "  3B(wPOLY) = " << std::scientific << std::setprecision(3) << std::setw(10) << e3b_wpoly
                           << "  3B(TTM) = " << std::scientific << std::setprecision(3) << std::setw(10) << e3b_nopoly
                           << "  BE = " << std::scientific << std::setprecision(3) << std::setw(10) << et_no3b;
                        std::string comment = ss.str();
                        tools::WriteXYZ(ofse, atom_names, confs_filtered[j], box, comment, false);
                    }
                }

                if (j % 100 == 0) std::cout << double(j) / confs_filtered.size() * 100.0 << " %" << std::endl;
            }
        }
    }

    ofs.close();
    ofsh.close();
    ofse.close();

    // Report
    std::cout << "From " << ntotal << " configurations, due to the distance criteria set, only " << ntested
              << " were tested" << std::endl
              << " and Chemphys found you:" << std::endl
              << nholes << " holes that were saved in holes.xyz" << std::endl
              << nholes_highe << " holes with high BE that were saved in holes_high_energy.xyz" << std::endl
              << nholes_big_classical_discrepancy
              << " probable holes that have a big discrepancy between classical TTM and polynomial correction"
              << std::endl;
    return 0;
}
