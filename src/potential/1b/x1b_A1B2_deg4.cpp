#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif // HAVE_CONFIG_H

#include <cmath>
#include <cassert>
#include <cstdlib>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "x1b_A1B2_deg4.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

void error(int errnum)
{
    std::cerr << " ** Fatal Error in x1b_v1x::load_poly_dat() ** : "
              << std::endl;
    std::exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------//

double v_intra(const double& k, const double& r0,
               const double* a1, const double* a2)
{
    const double dx[3] = {a1[0] - a2[0],
                          a1[1] - a2[1],
                          a1[2] - a2[2]};
    const double dsq = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
    const double d = std::sqrt(dsq);

    return std::exp(-k*(d - r0));
}

//----------------------------------------------------------------------------//

void g_intra(const double& g, const double& k, const double& r0,
             const double* a1, const double* a2,
             double* g1, double* g2)
{
    const double dx[3] = {a1[0] - a2[0],
                          a1[1] - a2[1],
                          a1[2] - a2[2]};
    const double dsq = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
    const double d = std::sqrt(dsq);

    const double gg = - k*g*std::exp(-k*(d - r0))/d;

    for (int i = 0; i < 3; ++i) {
        g1[i] += gg*dx[i];
        g2[i] -= gg*dx[i];
    }
}

//----------------------------------------------------------------------------//

double v_main(const double& k, const double& r0,
              const double* a1, const double* a2)
{
    const double dx[3] = {a1[0] - a2[0],
                          a1[1] - a2[1],
                          a1[2] - a2[2]};
    const double dsq = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
    const double d = std::sqrt(dsq);

//    return std::exp(-k*(d - r0))*d;
    return std::exp(-k*(d - r0));
}

//----------------------------------------------------------------------------//

void g_main(const double& g, const double& k, const double& r0,
            const double* a1, const double* a2,
            double* g1, double* g2)
{
    const double dx[3] = {a1[0] - a2[0],
                          a1[1] - a2[1],
                          a1[2] - a2[2]};
    const double dsq = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
    const double d = std::sqrt(dsq);

//    const double gg = g*(1.0/d - k)*std::exp(-k*(d - r0));
    const double gg = - k*g*std::exp(-k*(d - r0))/d;

    for (int i = 0; i < 3; ++i) {
        g1[i] += gg*dx[i];
        g2[i] -= gg*dx[i];
    }
}

//----------------------------------------------------------------------------//

} // namespace

////////////////////////////////////////////////////////////////////////////////

namespace A1B2 {

//----------------------------------------------------------------------------//

double x1b_v1x::operator()(const double* w1) const
{
    const double* A0 = w1 + 0;
    const double* B0 = w1 + 3;
    const double* B1 = w1 + 6;
    double x[3];
    x[0] = v_intra(k_intra_AB, d_intra_AB, A0, B0);
    x[1] = v_intra(k_intra_AB, d_intra_AB, A0, B1);
    x[2] = v_intra(k_intra_BB, d_intra_BB, B0, B1);
    double retval = poly_1b_v1x::eval(m_coeffs, x); 
    return retval; 
}
 
//----------------------------------------------------------------------------//
 
double x1b_v1x::operator()(const double* w1, double* g1) const
{
    const double* A0 = w1 + 0;
    const double* B0 = w1 + 3;
    const double* B1 = w1 + 6;
    double x[3];
    x[0] = v_intra(k_intra_AB, d_intra_AB, A0, B0);
    x[1] = v_intra(k_intra_AB, d_intra_AB, A0, B1);
    x[2] = v_intra(k_intra_BB, d_intra_BB, B0, B1);
    double g[3];
    double retval = poly_1b_v1x::eval(m_coeffs, x, g); 
    double* gA0 = g1 + 0;
    double* gB0 = g1 + 3;
    double* gB1 = g1 + 6;
    g_intra(g[0], k_intra_AB, d_intra_AB, A0, B0, gA0, gB0);
    g_intra(g[1], k_intra_AB, d_intra_AB, A0, B1, gA0, gB1);
    g_intra(g[2], k_intra_BB, d_intra_BB, B0, B1, gB0, gB1);
    return retval;
}
 
//----------------------------------------------------------------------------//
 
void x1b_v1x::load_poly_dat(const char* fn)
{
    assert(fn); 
    std::ifstream infile(fn);
    std::string line;
    bool polynomial_flag = false;
    while (std::getline(infile, line)) 
    {
        std::string identifier;
        std::istringstream iss(line); 
        iss >> identifier; 
        if (identifier == "d_intra_AB") iss >> d_intra_AB;
        if (identifier == "d_intra_BB") iss >> d_intra_BB;
        if (identifier == "k_intra_AB") iss >> k_intra_AB;
        if (identifier == "k_intra_BB") iss >> k_intra_BB;
        if (polynomial_flag == true){
            int poly_num; 
            iss >> poly_num; 
            std::istringstream identifier_to_poly(identifier); 
            identifier_to_poly >> m_coeffs[poly_num]; 
            if (poly_num == ncoeffs - 1) polynomial_flag = false;
        }
        if (polynomial_flag == false && identifier == "polynomials:") polynomial_flag = true; 
    }
}
 
//----------------------------------------------------------------------------//
 
}
