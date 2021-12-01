/******************************************************************************
Copyright 2019 The Regents of the University of California.
All Rights Reserved.

Permission to copy, modify and distribute any part of this Software for
educational, research and non-profit purposes, without fee, and without
a written agreement is hereby granted, provided that the above copyright
notice, this paragraph and the following three paragraphs appear in all
copies.

Those desiring to incorporate this Software into commercial products or
use for commercial purposes should contact the:
Office of Innovation & Commercialization
University of California, San Diego
9500 Gilman Drive, Mail Code 0910
La Jolla, CA 92093-0910
Ph: (858) 534-5815
FAX: (858) 534-7345
E-MAIL: invent@ucsd.edu

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE, EVEN IF THE UNIVERSITY
OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS. THE UNIVERSITY OF CALIFORNIA MAKES NO
REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER IMPLIED OR
EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE
SOFTWARE WILL NOT INFRINGE ANY PATENT, TRADEMARK OR OTHER RIGHTS.
******************************************************************************/

#include "x1b_A1B2_deg4_v1x.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

double v_intra(const double& k, const double& r0, const double* a1, const double* a2) {
    const double dx[3] = {a1[0] - a2[0], a1[1] - a2[1], a1[2] - a2[2]};
    const double dsq = dx[0] * dx[0] + dx[1] * dx[1] + dx[2] * dx[2];
    const double d = std::sqrt(dsq);

    return std::exp(-k * (d - r0));
}

//----------------------------------------------------------------------------//

void g_intra(const double& g, const double& k, const double& r0, const double* a1, const double* a2, double* g1,
             double* g2) {
    const double dx[3] = {a1[0] - a2[0], a1[1] - a2[1], a1[2] - a2[2]};
    const double dsq = dx[0] * dx[0] + dx[1] * dx[1] + dx[2] * dx[2];
    const double d = std::sqrt(dsq);

    const double gg = -k * g * std::exp(-k * (d - r0)) / d;

    for (int i = 0; i < 3; ++i) {
        g1[i] += gg * dx[i];
        g2[i] -= gg * dx[i];
    }
}

//----------------------------------------------------------------------------//

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace x1b_A1B2_deg4 {

//----------------------------------------------------------------------------//

x1b_A1B2_v1x::x1b_A1B2_v1x(const std::string mon) {
    coefficients = std::vector<double>{-1.645734441711526e+01, -2.811519309501759e+01, 2.812464786974462e+00,
                                       -1.037329541815288e+00, 7.711862844029446e-01,  7.913896321274219e-01,
                                       3.005541855414814e-03,  -1.222605763830645e-02, 1.239401252785214e-02,
                                       -3.582498850688396e-02, 1.057324226557168e-02,  -5.356166050348692e-03,
                                       -6.550258588146693e-05, 8.832821683619509e-06,  -3.433335550453666e-05,
                                       1.765975011972418e-05,  1.118169151448979e-04,  -1.924761970754873e-05,
                                       4.119019009196602e-05,  1.144904322884920e-05,  1.134739977303551e-05};

    d_intra_AB = 4.735980599672855e+00;  // A^(-1)
    d_intra_BB = 4.280371549403782e+01;  // A^(-1)
    k_intra_AB = 1.205203215552125e+00;  // A^(-1)
    k_intra_BB = 1.361445498514881e-01;  // A^(-1)
}

//----------------------------------------------------------------------------//

std::vector<double> x1b_A1B2_v1x::eval(const double* xyz, const size_t nmon) const {
    std::vector<double> energies(nmon, 0.0);

    for (size_t j = 0; j < nmon; j++) {
        double xcrd[9];
        std::copy(xyz + j * 9, xyz + (j + 1) * 9, xcrd);
        const double* A0 = xcrd + 0;
        const double* B0 = xcrd + 3;
        const double* B1 = xcrd + 6;
        double x[3], g[3];
        x[0] = v_intra(k_intra_AB, d_intra_AB, A0, B0);
        x[1] = v_intra(k_intra_AB, d_intra_AB, A0, B1);
        x[2] = v_intra(k_intra_BB, d_intra_BB, B0, B1);
        energies[j] = polynomial::eval(coefficients.data(), x, g);
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << xyz[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Output energies for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        std::cerr << energies[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    return energies;
}

//----------------------------------------------------------------------------//
std::vector<double> x1b_A1B2_v1x::eval(const double* xyz, double* grad, const size_t nmon,
                                       std::vector<double>* virial) const {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << xyz[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input gradients for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << grad[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input virial:\n";
    for (size_t i = 0; i < 9; i++) {
        std::cerr << (*virial)[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    std::vector<double> energies(nmon, 0.0);

    for (size_t j = 0; j < nmon; j++) {
        double xcrd[9];
        std::copy(xyz + j * 9, xyz + (j + 1) * 9, xcrd);
        const double* A0 = xcrd + 0;
        const double* B0 = xcrd + 3;
        const double* B1 = xcrd + 6;
        double x[3], g[3];
        x[0] = v_intra(k_intra_AB, d_intra_AB, A0, B0);
        x[1] = v_intra(k_intra_AB, d_intra_AB, A0, B1);
        x[2] = v_intra(k_intra_BB, d_intra_BB, B0, B1);
        energies[j] = polynomial::eval(coefficients.data(), x, g);
        double xgrd[9];
        std::fill(xgrd, xgrd + 9, 0.0);
        double* gA0 = xgrd + 0;
        double* gB0 = xgrd + 3;
        double* gB1 = xgrd + 6;
        g_intra(g[0], k_intra_AB, d_intra_AB, A0, B0, gA0, gB0);
        g_intra(g[1], k_intra_AB, d_intra_AB, A0, B1, gA0, gB1);
        g_intra(g[2], k_intra_BB, d_intra_BB, B0, B1, gB0, gB1);
        for (size_t i = 0; i < 9; i++) grad[i + j * 9] = xgrd[i];

        if (virial != 0) {
            (*virial)[0] += -A0[0] * gA0[0] - B0[0] * gB0[0] - B1[0] * gB1[0];

            (*virial)[1] += -A0[0] * gA0[1] - B0[0] * gB0[1] - B1[0] * gB1[1];

            (*virial)[2] += -A0[0] * gA0[2] - B0[0] * gB0[2] - B1[0] * gB1[2];

            (*virial)[4] += -A0[1] * gA0[1] - B0[1] * gB0[1] - B1[1] * gB1[1];

            (*virial)[5] += -A0[1] * gA0[2] - B0[1] * gB0[2] - B1[1] * gB1[2];

            (*virial)[8] += -A0[2] * gA0[2] - B0[2] * gB0[2] - B1[2] * gB1[2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Output energies for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        std::cerr << energies[i] << " , ";
    }
    std::cerr << std::endl;
    std::cerr << "Output gradients for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << grad[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Output virial:\n";
    for (size_t i = 0; i < 9; i++) {
        std::cerr << (*virial)[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    return energies;
}

//----------------------------------------------------------------------------//

}  // namespace x1b_A1B2_deg4
