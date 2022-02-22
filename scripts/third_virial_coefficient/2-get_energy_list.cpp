#include "bblock/system.h"
#include "io_tools/read_xyz.h"
#include "io_tools/read_nrg.h"

////////////////////////////////////////////////////////////////////////////////
namespace {
const double r_cut_check = 1.2;
}

inline double dist(const double* r1, const double* r2) {
    const double dx = r1[0] - r2[0];
    const double dy = r1[1] - r2[1];
    const double dz = r1[2] - r2[2];

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool check_dimer(std::vector<double> xyz, size_t nat1, size_t nat2, double dmin) {
    bool ok = true;
    for (size_t i = 0; i < nat1; i++) {
        double* ati = xyz.data() + 3 * i;
        for (size_t j = 0; j < nat2; j++) {
            double* atj = xyz.data() + 3 * nat1 + 3 * j;
            if (dist(ati, atj) < dmin) {
                ok = false;
                break;
            }
            if (!ok) break;
        }
    }
    return ok;
}

int main(int argc, char** argv) {
    std::vector<bblock::System> sys_m1;
    std::vector<bblock::System> sys_m2;
    std::vector<bblock::System> sys_m3;
    std::vector<bblock::System> sys_d12;
    std::vector<bblock::System> sys_d13;
    std::vector<bblock::System> sys_d23;
    std::vector<bblock::System> sys_t;

    if (argc < 9) {
        std::cout << "Usage: " << argv[0]
                  << "<mon1.nrg> <mon2.nrg> <mon3.nrg> <dimer12.nrg> <dimer13.nrg> <dimer23.nrg> <trimer.nrg> "
                     "<configs.xyz> <mbx.json>"
                  << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<std::vector<std::string> > ats;
    std::vector<std::vector<double> > coords;
    std::vector<std::vector<double> > boxes;

    try {
        // Checking each one of the input files in the command line
        tools::ReadNrg(argv[1], sys_m1);
        tools::ReadNrg(argv[2], sys_m2);
        tools::ReadNrg(argv[3], sys_m3);
        tools::ReadNrg(argv[4], sys_d12);
        tools::ReadNrg(argv[5], sys_d13);
        tools::ReadNrg(argv[6], sys_d23);
        tools::ReadNrg(argv[7], sys_t);
        tools::ReadXYZ(argv[8], ats, coords, boxes);

        if (argc == 10) {
            sys_m1[0].SetUpFromJson(argv[9]);
            sys_m2[0].SetUpFromJson(argv[9]);
            sys_m3[0].SetUpFromJson(argv[9]);
            sys_d12[0].SetUpFromJson(argv[9]);
            sys_d13[0].SetUpFromJson(argv[9]);
            sys_d23[0].SetUpFromJson(argv[9]);
            sys_t[0].SetUpFromJson(argv[9]);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Energies in order: be 2b12 2b13 2b23 3b123
    std::vector<std::vector<double> > energies(ats.size(), std::vector<double>(5, 100000.0));

    // NOTE. Assuming rigid virial calculation. Monomer energies can be computed outside the loop.
    size_t nat1 = sys_m1[0].GetNumRealSites();
    size_t nat2 = sys_m2[0].GetNumRealSites();
    size_t nat3 = sys_m3[0].GetNumRealSites();

    std::vector<double> xyzm1(coords[0].begin(), coords[0].begin() + 3 * nat1);
    std::vector<double> xyzm2(coords[0].begin() + 3 * nat1, coords[0].begin() + 3 * nat1 + 3 * nat2);
    std::vector<double> xyzm3(coords[0].begin() + 3 * nat1 + 3 * nat2, coords[0].end());

    sys_m1[0].SetRealXyz(xyzm1);
    sys_m2[0].SetRealXyz(xyzm2);
    sys_m3[0].SetRealXyz(xyzm3);

    double em1 = sys_m1[0].Energy(false);
    double em2 = sys_m2[0].Energy(false);
    double em3 = sys_m3[0].Energy(false);

    // Declare vectors outside the loop to minimize time
    std::vector<double> xyzd12(3 * nat1 + 3 * nat2);
    std::vector<double> xyzd13(3 * nat1 + 3 * nat3);
    std::vector<double> xyzd23(3 * nat2 + 3 * nat3);

    std::string fname(argv[8]);
    fname.replace(fname.end() - 3, fname.end(), "dat");

    std::ofstream off(fname.c_str());
    off << std::scientific << std::setprecision(8);

    // Get the energies for each dimer in the trimer and the 3b
    for (size_t i = 0; i < ats.size(); i++) {
        for (size_t j = 0; j < 3 * nat1; j++) {
            xyzd12[j] = coords[i][j];
            xyzd13[j] = coords[i][j];
        }

        for (size_t j = 0; j < 3 * nat2; j++) {
            xyzd12[j + 3 * nat1] = coords[i][j + 3 * nat1];
            xyzd23[j] = coords[i][j + 3 * nat1];
        }

        for (size_t j = 0; j < 3 * nat3; j++) {
            xyzd13[j + 3 * nat1] = coords[i][j + 3 * nat1 + 3 * nat2];
            xyzd23[j + 3 * nat2] = coords[i][j + 3 * nat1 + 3 * nat2];
        }

        sys_d12[0].SetRealXyz(xyzd12);
        sys_d13[0].SetRealXyz(xyzd13);
        sys_d23[0].SetRealXyz(xyzd23);

        sys_t[0].SetRealXyz(coords[i]);

        // Get the energies. If one energy has a hole (Electrostatic diverges) it will be set to a high positive number.
        bool arewegood = true;
        double et = 100000;
        double ed12 = 100000;
        double ed13 = 100000;
        double ed23 = 100000;

        // TODO the distance checked rn is for the first atom, but should be the COM

        // See if we can calculate IE 12 (i.e., there is no hole)
        if (check_dimer(xyzd12, nat1, nat2, r_cut_check)) {
            ed12 = sys_d12[0].Energy(false);
            energies[i][1] = ed12 - em1 - em2;  // 2B 12
        } else {
            arewegood = false;
        }

        // See if we can calculate IE 13 (i.e., there is no hole)
        if (check_dimer(xyzd13, nat1, nat3, r_cut_check)) {
            ed13 = sys_d13[0].Energy(false);
            energies[i][2] = ed13 - em1 - em3;  // 2B 13
        } else {
            arewegood = false;
        }

        // See if we can calculate IE 23 (i.e., there is no hole)
        if (check_dimer(xyzd23, nat2, nat3, r_cut_check)) {
            ed23 = sys_d23[0].Energy(false);
            energies[i][3] = ed23 - em2 - em3;  // 2B 23
        } else {
            arewegood = false;
        }

        // If the three dimers are fine, see if we can get the trimer energy.
        if (arewegood) {
            try {
                et = sys_t[0].Energy(false);
                energies[i][0] = et;  // BE
            } catch (...) {
                arewegood = false;
            }
        }

        // If all the energies have been succesfully calculated, get the 3b energy. Otherwise is set to a high positive
        // number.
        if (arewegood) {
            energies[i][4] = et - ed12 - ed13 - ed23 + em1 + em2 + em3;  // 3B
        } else {
            energies[i][4] = 100000;
        }

        // Write them int he file
        for (size_t j = 0; j < energies[i].size(); j++) {
            off << std::scientific << std::setprecision(10) << std::setw(20) << energies[i][j];
        }
        off << std::endl;
    }

    off.close();

    return 0;
}
