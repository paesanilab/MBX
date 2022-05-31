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
const double max_dist = 5;  // Angstrom
double emin = 0.0;
const double max_be = 70.0;
double min_poly_e = -5.0;

static std::vector<tools::xyz_frame> set1;
static std::vector<tools::xyz_frame> set2;
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

void get_configs(double R, std::vector<tools::xyz_frame>& mon1, std::vector<tools::xyz_frame>& mon2, int ns,
                 kit::mt19937& prg, std::vector<std::vector<double> >& configs) {
    size_t number_of_configs1 = mon1.size();
    size_t number_of_configs2 = mon2.size();

    configs = std::vector<std::vector<double> >(ns);
    for (size_t i = 0; i < ns; i++) {
        size_t idx1 = rand() % number_of_configs1;
        size_t idx2 = rand() % number_of_configs2;
        size_t nat = mon1[idx1].natm;
        size_t nat2 = mon2[idx2].natm;
        double m1[3 * nat], m2[3 * nat2], dim[3 * (nat2 + nat)];

        std::copy(mon1[idx1].xyz, mon1[idx1].xyz + 3 * nat, m1);
        std::copy(mon2[idx2].xyz, mon2[idx2].xyz + 3 * nat2, m2);

        com_to_origin(m1, mon1[idx1].at_name);
        com_to_origin(m2, mon2[idx2].at_name);

        std::copy(m1, m1 + 3 * nat, dim);
        std::copy(m2, m2 + 3 * nat2, dim + 3 * nat);

        rotate_monomer(prg, dim, nat);
        rotate_monomer(prg, dim + 3 * nat, nat2);

        // Translate COMs away along X axis
        for (size_t j = 0; j < nat; j++) {
            dim[3 * j] += R;
        }

        std::vector<double> dimv(dim, dim + 3 * (nat + nat2));
        configs[i] = dimv;
    }
}

void check_configs(const double rcheck, size_t nat1, size_t nat2, std::vector<std::vector<double> > confs_to_check,
                   std::vector<std::vector<double> >& confs_checked) {
    size_t ns = confs_to_check.size();
    confs_checked.clear();

    for (size_t i = 0; i < ns; i++) {
        std::vector<double> m1(confs_to_check[i].data(), confs_to_check[i].data() + 3 * nat1);
        std::vector<double> m2(confs_to_check[i].data() + 3 * nat1, confs_to_check[i].data() + 3 * (nat1 + nat2));

        bool config_ok = true;
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

        if (config_ok) confs_checked.push_back(confs_to_check[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc < 9) {
        std::cout << "Usage: " << argv[0]
                  << " <eq_geom1.xyz> <eq_geom2.xyz>  <n_samples_x_point> <mon1.nrg> <mon2.nrg> <dim.nrg> "
                     "<mbx_mb.json> <mbx_ttm.json>"
                  << std::endl;
        return EXIT_FAILURE;
    }

    const double dR = step_size;

    kit::mt19937 prg(27606);
    int ns = 0;

    // Initialize random seed
    srand(time(NULL));

    std::vector<bblock::System> sys_m1;
    std::vector<bblock::System> sys_m2;
    std::vector<bblock::System> sys_d;
    std::vector<bblock::System> sys_ttm_m1;
    std::vector<bblock::System> sys_ttm_m2;
    std::vector<bblock::System> sys_ttm_d;

    try {
        // Checking each one of the input files in the command line
        tools::read_xyz(argv[1], set1);
        tools::read_xyz(argv[2], set2);
        ns = atoi(argv[3]);
        tools::ReadNrg(argv[4], sys_m1);
        tools::ReadNrg(argv[5], sys_m2);
        tools::ReadNrg(argv[6], sys_d);
        tools::ReadNrg(argv[4], sys_ttm_m1);
        tools::ReadNrg(argv[5], sys_ttm_m2);
        tools::ReadNrg(argv[6], sys_ttm_d);

        sys_m1[0].SetUpFromJson(argv[7]);
        sys_m2[0].SetUpFromJson(argv[7]);
        sys_d[0].SetUpFromJson(argv[7]);
        sys_ttm_m1[0].SetUpFromJson(argv[8]);
        sys_ttm_m2[0].SetUpFromJson(argv[8]);
        sys_ttm_d[0].SetUpFromJson(argv[8]);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    size_t nat1 = set1[0].natm;
    size_t nat2 = set2[0].natm;
    size_t nat_tot = nat1 + nat2;

    std::vector<std::string> atom_names(nat_tot);
    std::copy(set1[0].at_name.begin(), set1[0].at_name.end(), atom_names.begin());
    std::copy(set2[0].at_name.begin(), set2[0].at_name.end(), atom_names.begin() + nat1);

    std::vector<double> box;

    // std::string index = std::to_string(count) + ".xyz";
    // std::ofstream ofs(index.c_str());
    std::ofstream ofs("holes.xyz");
    std::ofstream ofsh("holes_high_energy.xyz");
    size_t nholes = 0;
    size_t nholes_highe = 0;
    size_t ntested = 0;
    size_t ntotal = 0;

    for (double R = min_dist; R < max_dist + dR / 2.0; R += dR) {
        // Generate config pool
        std::cout << "Working on d = " << R << std::endl;
        std::vector<std::vector<double> > confs;
        get_configs(R, set1, set2, ns, prg, confs);

        ntotal += ns;

        // Filter config pool according to rmin_check. Configs in which at least
        // one interatomic distance is lower than rcut_check, will be excluded from analysis.
        std::vector<std::vector<double> > confs_filtered;
        check_configs(rcut_check, nat1, nat2, confs, confs_filtered);

        ntested += confs_filtered.size();

        // Get the energies with MB-nrg and TTM-nrg
        for (size_t j = 0; j < confs_filtered.size(); j++) {
            std::vector<double> xyz1(confs_filtered[j].begin(), confs_filtered[j].begin() + 3 * nat1);
            std::vector<double> xyz2(confs_filtered[j].begin() + 3 * nat1, confs_filtered[j].end());

            // Set xyz arrays to MBX systems
            sys_m1[0].SetRealXyz(xyz1);
            sys_m2[0].SetRealXyz(xyz2);
            sys_d[0].SetRealXyz(confs_filtered[j]);
            sys_ttm_m1[0].SetRealXyz(xyz1);
            sys_ttm_m2[0].SetRealXyz(xyz2);
            sys_ttm_d[0].SetRealXyz(confs_filtered[j]);

            // Get the energies. We will look at
            // double em1_mb = sys_m1[0].Energy(false);
            // double em2_mb = sys_m2[0].Energy(false);
            // double edim_md = sys_d[0].Energy(false);
            // double em1_ttm = sys_ttm_m1[0].Energy(false);
            // double em2_ttm = sys_ttm_m2[0].Energy(false);
            double edim_ttm = sys_ttm_d[0].Energy(false);

            bool ishole = false;

            // A dimer will be considered a hole if the 2b polynomials behave weird
            // It is considered weird if total repulsion < -20.0 kcal/mol

            // MB-nrg
            double poly_e = sys_d[0].TwoBodyEnergy(false);
            double buck_e = sys_d[0].Buckingham(false);

            // TTM-nrg
            double ttm_e = sys_ttm_d[0].Buckingham(false);

            // Config is considered a hole if total repulsion for MB-nrg is lower than -20 kcal/mol
            if ((poly_e + buck_e) < min_poly_e) {
                ishole = true;
            }

            // If it is a hole, write it to the hole file
            if (ishole) {
                if (edim_ttm < max_be) {
                    nholes += 1;
                    std::stringstream ss;
                    ss << "Hole " << std::setw(7) << nholes << "  POLY(MB)  = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << poly_e << "  BUCK(MB)  = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << buck_e << "  BUCK(TTM) = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << ttm_e << "  BE(TTM) = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << edim_ttm;
                    std::string comment = ss.str();
                    tools::WriteXYZ(ofs, atom_names, confs_filtered[j], box, comment, false);
                } else {
                    nholes_highe += 1;
                    std::stringstream ss;
                    ss << "High energy hole " << std::setw(7) << nholes_highe << "  POLY(MB)  = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << poly_e << "  BUCK(MB)  = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << buck_e << "  BUCK(TTM) = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << ttm_e << "  BE(TTM) = " << std::scientific
                       << std::setprecision(3) << std::setw(10) << edim_ttm;
                    std::string comment = ss.str();
                    tools::WriteXYZ(ofsh, atom_names, confs_filtered[j], box, comment, false);
                }
            }
        }
    }

    ofs.close();
    ofsh.close();

    // Report
    std::cout << "From " << ntotal << " configurations, due to the distance criteria set, only " << ntested
              << " were tested" << std::endl
              << " and Chemphys found you " << nholes << " holes that were saved in holes.xyz" << std::endl
              << "However, there are also " << nholes_highe << " that were saved in holes_high_energy.xyz" << std::endl;
    return 0;
}
