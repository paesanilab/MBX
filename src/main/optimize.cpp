#include <cmath>
#include <cassert>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <cstdlib>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"
#include "io_tools/read_xyz.h"
#include "io_tools/write_xyz.h"

#include "bblock/system.h"

#include "tools/xmin.h"

namespace {

// Declare the system to be read from nrg
std::vector<bblock::System> s;

size_t number_of_atoms;

double GradFunction(double *crd, double *grd, int *numat) {
    std::vector<double> xyz(crd, crd + 3 * number_of_atoms);
    std::vector<double> grad;

    s[0].SetRealXyz(xyz);

    double energy = s[0].Energy(true);

    grad = s[0].GetRealGrads();
    std::copy(grad.begin(), grad.end(), grd);

    return energy;
}

void run_opt(std::vector<double> xyz, std::vector<double> &xyz_opt, double &grms, double &grad2, double tol,
             size_t maxit, size_t numats, double &e0, double &e1) {
    xyz_opt = xyz;
    std::vector<double> grad(xyz_opt.size());

    // Put first atom at the origin
    for (size_t i = 0; i < numats; i++) {
        for (size_t j = 0; j < 3; j++) {
            xyz_opt[3 * i + j] -= xyz[j];
        }
    }

    // Initialize the minimizer
    tools::xmin_opt optmzr;
    tools::xmin_opt_init(&optmzr);

    optmzr.maxiter = maxit;
    optmzr.grms_tol = tol;
    optmzr.print_level = 1;

    optmzr.method = 3;  // 1 - PRCG, 2 -- LBGFS, 3 -- TNCG

    int nat = int(numats);
    e0 = GradFunction(xyz_opt.data(), grad.data(), &nat);
    double energy = e0;

    tools::xmin(GradFunction, &nat, xyz_opt.data(), grad.data(), &energy, &grms, &optmzr);

    grad2 = 0.0;  // to double-check
    for (size_t i = 0; i < numats; i++) {
        grad2 += grad[i] * grad[i];
    }

    e1 = GradFunction(xyz_opt.data(), grad.data(), &nat);
}

}  // namespace

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0]
                  << " <config.nrg> [<mbx.json>] [<max iterations>] [<tolerance>] [<configs.xyz>] [<step>]\n";
        return 0;
    }

    // Declare atoms, coords and boxes to be read from xyz
    std::vector<std::vector<std::string> > ats;
    std::vector<std::vector<double> > coords;
    std::vector<std::vector<double> > boxes;
    size_t maxit = 1000;
    double tol = 1E-06;
    double step = 1;

    try {
        // Read the NRG file
        tools::ReadNrg(argv[1], s);

        if (argc > 2) {
            s[0].SetUpFromJson(argv[2]);
        } else {
            s[0].SetUpFromJson();
        }

        // Get number of maximum iterations
        if (argc > 3) {
            maxit = atoi(argv[3]);
        }

        // Get the tolerance for optimization
        if (argc > 4) {
            tol = atof(argv[4]);
        }

        // Read trajectory of inputed
        if (argc > 5) {
            tools::ReadXYZ(argv[5], ats, coords, boxes);
        }

        // read the step
        if (argc > 6) {
            step = atoi(argv[6]);
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    number_of_atoms = s[0].GetNumRealSites();
    int numats = int(number_of_atoms);
    std::vector<std::string> atoms = s[0].GetRealAtomNames();

    // Declare vectors of the optimized coordinates
    std::vector<std::vector<double> > opt_coords;

    // And the non-converged optimizations
    std::vector<std::vector<double> > not_converged_coords;

    std::vector<double> xyz_opt;
    double grms;
    double grad2;
    double e0;
    double e1;
    std::vector<double> e1_all;

    // Check if we need to do the xyz or just the nrg file
    if (coords.size() < 1) {
        // Just do the NRG file
        std::vector<double> xyz_nrg = s[0].GetRealXyz();
        run_opt(xyz_nrg, xyz_opt, grms, grad2, tol, maxit, number_of_atoms, e0, e1);
        if (grms > tol) {
            std::cout << "**WARNING** Optimization did not converge. Try running with more iterations!\n";
            not_converged_coords.push_back(xyz_opt);
        } else {
            std::cout << "Optimization Converged!\n";
            opt_coords.push_back(xyz_opt);
            e1_all.push_back(e1);
        }

        std::cout << "E = " << e0 << " -> " << e1 << ", |g| = " << std::sqrt(grad2) << std::endl;

    } else {
        for (size_t i = 0; i < coords.size(); i += step) {
            run_opt(coords[i], xyz_opt, grms, grad2, tol, maxit, number_of_atoms, e0, e1);
            if (grms > tol) {
                std::cout << "**WARNING** Optimization did not converge. Try running with more iterations!\n";
                not_converged_coords.push_back(xyz_opt);
            } else {
                std::cout << "Optimization Converged!\n";
                opt_coords.push_back(xyz_opt);
                e1_all.push_back(e1);
            }

            std::cout << "E = " << e0 << " -> " << e1 << ", |g| = " << std::sqrt(grad2) << std::endl;
        }
    }

    // Report optimized and non converged structures
    std::ofstream off;
    std::vector<double> box;
    off.open("optimized.xyz");
    for (size_t i = 0; i < opt_coords.size(); i++) {
        std::stringstream ss;
        ss << std::setprecision(8) << std::scientific << e1_all[i];
        std::cout << ss.str() << "   ++++!!!\n";
        tools::WriteXYZ(off, atoms, opt_coords[i], box, ss.str());
    }
    off.close();

    off.open("unconverged.xyz");
    for (size_t i = 0; i < not_converged_coords.size(); i++) {
        tools::WriteXYZ(off, atoms, not_converged_coords[i], box, "unconverged");
    }
    off.close();

    return 0;
}
