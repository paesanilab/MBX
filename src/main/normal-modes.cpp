#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H

#include <cmath>
#include <cassert>
#include <cstddef>

#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include <string>

#include "io_tools/read_nrg.h"
#include "io_tools/write_nrg.h"
#include "io_tools/read_xyz.h"
#include "io_tools/write_xyz.h"

#include "bblock/system.h"

#include "tools/properties.h"
#include "tools/constants.h"

#define VERBOSE yes

namespace {

//--------------------------------------------------------------------------------//

void dump_nm_displacements(const int, std::vector<double>, std::vector<std::vector<double> >, std::vector<double>,
                           std::vector<std::string>);

//--------------------------------------------------------------------------------//

void hessian_fd(const int natm, std::vector<double> xyz, const std::vector<std::string> at_name, double* H,
                bblock::System& ss) {
    // Increment of x when calculationg the hessian.
    const double eps = 1.0e-5;

    // I have no clue what the hell is that.
    const size_t ndofs = 3 * natm;

    // Filling teh Hessian with 0s
    std::fill(H, H + ndofs * ndofs, 0.0);

    // Calculating the Hessian for each one of the points
    for (size_t i = 0; i < ndofs; ++i) {
        // Fixing x0, the original value
        const double x0 = xyz[i];

        // Gradients for x0+eps
        xyz[i] = x0 + eps;
        ss.SetRealXyz(xyz);
        double ep = ss.Energy(true);
        std::vector<double> gp = ss.GetRealGrads();

        // Gradients for x0-eps
        xyz[i] = x0 - eps;
        ss.SetRealXyz(xyz);
        double em = ss.Energy(true);
        std::vector<double> gm = ss.GetRealGrads();

        // Reseting the position at the initial value
        xyz[i] = x0;

        // Updating the Hessian
        for (size_t j = 0; j < ndofs; ++j) {
            H[i * ndofs + j] += 0.25 * (gp[j] - gm[j]) / eps;
            H[j * ndofs + i] += 0.25 * (gp[j] - gm[j]) / eps;
        }
    }

    //
    // kcal/mol -> J/10 (Na x atomic-mass-unit = 0.001 kg; so that
    //                   lengths are in A and time is in ps)

    for (size_t i = 0; i < ndofs; ++i)
        for (size_t j = 0; j < ndofs; ++j) H[j * ndofs + i] *= constants::kcal_J / 10.0;
}

}  // namespace

int main(int argc, char** argv) {
    std::vector<std::vector<double> > nm;
    std::vector<double> freq;
    std::vector<std::string> elements;
    std::vector<bblock::System> s;

    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " opt.nrg mbx.json [print_displacements 0 = no / 1 = yes]" << std::endl;
        return 0;
    }

    int print_displacements = 0;

    try {
        std::ifstream ifs(argv[1]);
        tools::ReadNrg(argv[1], s);
        s[0].SetUpFromJson(argv[2]);
        if (argc > 2) {
            print_displacements = atoi(argv[3]);
        }

    } catch (const std::exception& e) {
        std::cerr << " ** Error ** : " << e.what() << std::endl;
        return 1;
    }

    // Get the necessary info from system

    // Defining number of atoms
    const int natm = s[0].GetNumRealSites();

    // Getting the atom names
    elements = s[0].GetRealAtomNames();

    // Degrees of freedom
    const int ndofs = 3 * natm;

    // Defining the pointer to the coordinates
    std::vector<double> xyz = s[0].GetRealXyz();

    // Calculating the sum of the gradient squared
    {
        double energy = s[0].Energy(true);
        std::vector<double> grads = s[0].GetRealGrads();

        double gg = 0;
        for (int n = 0; n < ndofs; ++n) gg += grads[n] * grads[n];

        gg = std::sqrt(gg);

        std::cout << argv[1] << " : E = " << energy << " , |g| = " << gg << std::endl;
    }

    // Defining the pointer to the Hessian
    double* H = new double[ndofs * ndofs];

    // Filling the Hessian
    hessian_fd(natm, xyz, elements, H, s[0]);

    // Setting the masses for all the atoms
    double mass[natm];
    for (int i = 0; i < natm; i++) {
        mass[i] = tools::GetMass(elements[i]);
    }

    for (int i = 0; i < natm; ++i) {
        const double mi = mass[i];
        for (int j = 0; j < natm; ++j) {
            const double mj = mass[j];
            for (int k = 0; k < 3; ++k)
                for (int l = 0; l < 3; ++l) H[(3 * i + k) * ndofs + 3 * j + l] /= std::sqrt(mi * mj);
        }
    }

    std::ofstream hessian;
    hessian.open("hessian.dat");

    hessian << std::scientific << std::setprecision(4);
    for (int i = 0; i < ndofs; ++i) {
        for (int j = 0; j < ndofs; ++j) hessian << std::setw(12) << H[i * ndofs + j];
        hessian << std::endl;
    }
    hessian.close();

    gsl_matrix_view H_gsl = gsl_matrix_view_array(H, ndofs, ndofs);

    gsl_vector* eval = gsl_vector_alloc(ndofs);
    gsl_matrix* evec = gsl_matrix_alloc(ndofs, ndofs);

    gsl_eigen_symmv_workspace* ws = gsl_eigen_symmv_alloc(ndofs);
    int status = gsl_eigen_symmv(&H_gsl.matrix, eval, evec, ws);
    gsl_eigen_symmv_free(ws);

    std::cout << "GSL::symmv status = " << status << std::endl;

    gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_ASC);

    const double radps_to_cm1 = 1.0e10 / (2 * M_PI * constants::c0);

    // Print the vibrational frequencies

    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout << std::setprecision(2);
    for (int n = 0; n < ndofs; ++n) {
        double eval_n = gsl_vector_get(eval, n);

        std::cout << std::setw(2) << n;
        if (eval_n > 0.0) {
            const double omega_n = std::sqrt(eval_n);  // in rad/ps
            std::cout << "  omega =   " << std::setw(15) << omega_n * radps_to_cm1;
        } else {
            const double omega_n = std::sqrt(-eval_n);  // in rad/ps
            std::cout << "  omega = I*" << std::setw(15) << omega_n * radps_to_cm1;
        }

        std::cout << " cm-1" << std::endl;
    }

    // Calculate the cartesian displacements and save them in 'nm'

    for (int n = 0; n < ndofs; ++n) {
        double eval_n = gsl_vector_get(eval, n);
        gsl_vector_view evec_n = gsl_matrix_column(evec, n);

        const double this_freq = std::sqrt(std::fabs(eval_n)) * radps_to_cm1;

        std::vector<double> this_nm;

        if (this_freq < 1.0) {  // assume it is just noise
            freq.push_back(0.0);
            for (int i = 0; i < ndofs; ++i) this_nm.push_back(0.0);
        } else {
            freq.push_back(this_freq);
            for (int i = 0; i < natm; ++i)
                for (int j = 0; j < 3; ++j)
                    this_nm.push_back(gsl_vector_get(&evec_n.vector, 3 * i + j) / std::sqrt(mass[i]));

            double norm(0);
            for (size_t i = 0; i < this_nm.size(); ++i) norm += this_nm[i] * this_nm[i];

            for (size_t i = 0; i < this_nm.size(); ++i) this_nm[i] /= std::sqrt(norm);
        }

        nm.push_back(this_nm);
    }

    // print the displacement matrix

    std::ofstream displacements;
    displacements.open("displacements.dat");

    displacements << std::scientific << std::setprecision(4);
    for (size_t i = 0; i < nm.size(); ++i) {
        displacements << std::setw(12) << freq[i];
        for (size_t j = 0; j < nm[i].size(); ++j) displacements << std::setw(12) << nm[i][j];
        displacements << std::endl;
    }

    if (print_displacements == 1) {
        dump_nm_displacements(natm, xyz, nm, freq, elements);
    }

    gsl_vector_free(eval);
    gsl_matrix_free(evec);
}

////////////////////////////////////////////////////////////////////////////////////

namespace {

void dump_nm_displacements(const int natm, std::vector<double> xyz, std::vector<std::vector<double> > nm,
                           std::vector<double> freq, std::vector<std::string> elements) {
    const int ndofs = 3 * natm;
    std::vector<double> box;
    const size_t ndisplace = 16;
    double displace[ndisplace] = {0.2,    0.15,    0.125, 0.1,    0.075, 0.05,   0.0375, 0.025,
                                  -0.025, -0.0375, -0.05, -0.075, -0.1,  -0.125, -0.15,  -0.2};

    for (int i = 0; i < ndofs; ++i) {
        std::string filename = "nm_" + std::to_string(static_cast<long long>(i)) + ".xyz";

        std::ofstream animate;
        animate.open(filename.c_str());

        for (size_t n = 0; n < ndisplace; ++n) {
            std::vector<double> d_xyz = xyz;

            for (int j = 0; j < ndofs; ++j) {
                d_xyz[j] += nm[i][j] * displace[n];
            }
            tools::WriteXYZ(animate, elements, d_xyz, box, std::to_string(static_cast<long double>(freq[i])));
        }
    }
}

}  // namespace
