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

#include "x3b-h2o-ion-v1x-deg4-filtered.h"

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

double v_main(const double& k, const double& r0, const double* a1, const double* a2) {
    const double dx[3] = {a1[0] - a2[0], a1[1] - a2[1], a1[2] - a2[2]};
    const double dsq = dx[0] * dx[0] + dx[1] * dx[1] + dx[2] * dx[2];
    const double d = std::sqrt(dsq);

    //    return std::exp(-k*(d - r0))*d;
    return std::exp(-k * (d - r0));
}

//----------------------------------------------------------------------------//

void g_main(const double& g, const double& k, const double& r0, const double* a1, const double* a2, double* g1,
            double* g2) {
    const double dx[3] = {a1[0] - a2[0], a1[1] - a2[1], a1[2] - a2[2]};
    const double dsq = dx[0] * dx[0] + dx[1] * dx[1] + dx[2] * dx[2];
    const double d = std::sqrt(dsq);

    //    const double gg = g*(1.0/d - k)*std::exp(-k*(d - r0));
    const double gg = -k * g * std::exp(-k * (d - r0)) / d;

    for (int i = 0; i < 3; ++i) {
        g1[i] += gg * dx[i];
        g2[i] -= gg * dx[i];
    }
}

}  // namespace
//----------------------------------------------------------------------------//

double x3b_h2o_ion_v1x_deg4_filtered::f_switch(const double& r, double& g) const {
    if (r > m_r3f) {
        g = 0.0;
        return 0.0;
    } else if (r > m_r3i) {
        const double t1 = M_PI / (m_r3f - m_r3i);
        const double x = (r - m_r3i) * t1;
        g = -std::sin(x) * t1 / 2.0;
        return (1.0 + std::cos(x)) / 2.0;
    } else {
        g = 0.0;
        return 1.0;
    }
}

//----------------------------------------------------------------------------//

double x3b_h2o_ion_v1x_deg4_filtered::operator()(const double* xyz1, const double* xyz2, const double* xyz3,
                                                 size_t nt) const {
    double energy = 0.0;
    for (size_t i = 0; i < nt; i++) {
        const double* w1 = xyz1 + 9 * i;
        const double* w2 = xyz2 + 9 * i;
        const double* x = xyz3 + 3 * i;

        const double* Oa = w1;
        const double* Ha1 = w1 + 3;
        const double* Ha2 = w1 + 6;

        const double* Ob = w2;
        const double* Hb1 = w2 + 3;
        const double* Hb2 = w2 + 6;

        const double* X = x;

        double rab[3], rac[3], rbc[3];
        double drab(0), drac(0), drbc(0);

        for (int n = 0; n < 3; ++n) {
            rab[n] = Oa[n] - Ob[n];
            drab += rab[n] * rab[n];

            rac[n] = Oa[n] - X[n];
            drac += rac[n] * rac[n];

            rbc[n] = Ob[n] - X[n];
            drbc += rbc[n] * rbc[n];
        }

        drab = std::sqrt(drab);
        drac = std::sqrt(drac);
        drbc = std::sqrt(drbc);

        if ((drab > m_r3f and drac > m_r3f) or (drab > m_r3f and drbc > m_r3f) or (drac > m_r3f and drbc > m_r3f))
            continue;

        double gab, gac, gbc;

        const double sab = f_switch(drab, gab);
        const double sac = f_switch(drac, gac);
        const double sbc = f_switch(drbc, gbc);

        const double s = sab * sac + sab * sbc + sac * sbc;

        double v[21];

        v[0] = v_intra(m_kOH_intra, m_dOH_intra, Oa, Ha1);
        v[1] = v_intra(m_kOH_intra, m_dOH_intra, Oa, Ha2);
        v[2] = v_intra(m_kHH_intra, m_dHH_intra, Ha1, Ha2);

        v[3] = v_intra(m_kOH_intra, m_dOH_intra, Ob, Hb1);
        v[4] = v_intra(m_kOH_intra, m_dOH_intra, Ob, Hb2);
        v[5] = v_intra(m_kHH_intra, m_dHH_intra, Hb1, Hb2);

        v[6] = v_main(m_kHH, m_dHH, Ha1, Hb1);
        v[7] = v_main(m_kHH, m_dHH, Ha1, Hb2);
        v[8] = v_main(m_kHH, m_dHH, Ha2, Hb1);
        v[9] = v_main(m_kHH, m_dHH, Ha2, Hb2);

        v[10] = v_main(m_kOH, m_dOH, Oa, Hb1);
        v[11] = v_main(m_kOH, m_dOH, Oa, Hb2);

        v[12] = v_main(m_kOH, m_dOH, Ob, Ha1);
        v[13] = v_main(m_kOH, m_dOH, Ob, Ha2);

        v[14] = v_main(m_kOO, m_dOO, Oa, Ob);

        v[15] = v_main(m_kXH, m_dXH, Ha1, X);
        v[16] = v_main(m_kXH, m_dXH, Ha2, X);

        v[17] = v_main(m_kXH, m_dXH, Hb1, X);
        v[18] = v_main(m_kXH, m_dXH, Hb2, X);

        v[19] = v_main(m_kXO, m_dXO, Oa, X);

        v[20] = v_main(m_kXO, m_dXO, Ob, X);

        double g[21];
        double retval = h2o_ion::poly_3b_h2o_ion_v1x_deg4_filtered::eval(m_coeffs, v, g);

        retval *= s;

        energy += retval;
    }

    return energy;
}

double x3b_h2o_ion_v1x_deg4_filtered::operator()(const double* xyz1, const double* xyz2, const double* xyz3,
                                                 double* grad1, double* grad2, double* grad3, size_t nt,
                                                 std::vector<double>* virial) const {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates (1) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << xyz1[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input coordinates (2) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << xyz2[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input coordinates (3) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cerr << xyz3[3 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input gradients (1) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << grad1[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input gradients (2) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << grad2[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input gradients (3) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cerr << grad3[3 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input virial:\n";
    for (size_t i = 0; i < 9; i++) {
        std::cerr << (*virial)[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    double energy = 0.0;
    for (size_t i = 0; i < nt; i++) {
        const double* w1 = xyz1 + 9 * i;
        const double* w2 = xyz2 + 9 * i;
        const double* x = xyz3 + 3 * i;

        const double* Oa = w1;
        const double* Ha1 = w1 + 3;
        const double* Ha2 = w1 + 6;

        const double* Ob = w2;
        const double* Hb1 = w2 + 3;
        const double* Hb2 = w2 + 6;

        const double* X = x;

        double rab[3], rac[3], rbc[3];
        double drab(0), drac(0), drbc(0);

        for (int n = 0; n < 3; ++n) {
            rab[n] = Oa[n] - Ob[n];
            drab += rab[n] * rab[n];

            rac[n] = Oa[n] - X[n];
            drac += rac[n] * rac[n];

            rbc[n] = Ob[n] - X[n];
            drbc += rbc[n] * rbc[n];
        }

        drab = std::sqrt(drab);
        drac = std::sqrt(drac);
        drbc = std::sqrt(drbc);

        if ((drab > m_r3f and drac > m_r3f) or (drab > m_r3f and drbc > m_r3f) or (drac > m_r3f and drbc > m_r3f))
            continue;

        double gab, gac, gbc;

        const double sab = f_switch(drab, gab);
        const double sac = f_switch(drac, gac);
        const double sbc = f_switch(drbc, gbc);

        const double s = sab * sac + sab * sbc + sac * sbc;

        double v[21];

        v[0] = v_intra(m_kOH_intra, m_dOH_intra, Oa, Ha1);
        v[1] = v_intra(m_kOH_intra, m_dOH_intra, Oa, Ha2);
        v[2] = v_intra(m_kHH_intra, m_dHH_intra, Ha1, Ha2);

        v[3] = v_intra(m_kOH_intra, m_dOH_intra, Ob, Hb1);
        v[4] = v_intra(m_kOH_intra, m_dOH_intra, Ob, Hb2);
        v[5] = v_intra(m_kHH_intra, m_dHH_intra, Hb1, Hb2);

        v[6] = v_main(m_kHH, m_dHH, Ha1, Hb1);
        v[7] = v_main(m_kHH, m_dHH, Ha1, Hb2);
        v[8] = v_main(m_kHH, m_dHH, Ha2, Hb1);
        v[9] = v_main(m_kHH, m_dHH, Ha2, Hb2);

        v[10] = v_main(m_kOH, m_dOH, Oa, Hb1);
        v[11] = v_main(m_kOH, m_dOH, Oa, Hb2);

        v[12] = v_main(m_kOH, m_dOH, Ob, Ha1);
        v[13] = v_main(m_kOH, m_dOH, Ob, Ha2);

        v[14] = v_main(m_kOO, m_dOO, Oa, Ob);

        v[15] = v_main(m_kXH, m_dXH, Ha1, X);
        v[16] = v_main(m_kXH, m_dXH, Ha2, X);

        v[17] = v_main(m_kXH, m_dXH, Hb1, X);
        v[18] = v_main(m_kXH, m_dXH, Hb2, X);

        v[19] = v_main(m_kXO, m_dXO, Oa, X);

        v[20] = v_main(m_kXO, m_dXO, Ob, X);

        double g[21];
        double retval = h2o_ion::poly_3b_h2o_ion_v1x_deg4_filtered::eval(m_coeffs, v, g);

        for (int n = 0; n < 21; ++n) g[n] *= s;

        double grad[21];
        std::fill(grad, grad + 21, 0.0);

        double* g1 = grad;
        double* g2 = grad + 9;
        double* g3 = grad + 18;

        double* gOa = g1;
        double* gHa1 = g1 + 3;
        double* gHa2 = g1 + 6;

        double* gOb = g2;
        double* gHb1 = g2 + 3;
        double* gHb2 = g2 + 6;

        double* gX = g3;

        g_intra(g[0], m_kOH_intra, m_dOH_intra, Oa, Ha1, gOa, gHa1);
        g_intra(g[1], m_kOH_intra, m_dOH_intra, Oa, Ha2, gOa, gHa2);
        g_intra(g[2], m_kHH_intra, m_dHH_intra, Ha1, Ha2, gHa1, gHa2);

        g_intra(g[3], m_kOH_intra, m_dOH_intra, Ob, Hb1, gOb, gHb1);
        g_intra(g[4], m_kOH_intra, m_dOH_intra, Ob, Hb2, gOb, gHb2);
        g_intra(g[5], m_kHH_intra, m_dHH_intra, Hb1, Hb2, gHb1, gHb2);

        g_main(g[6], m_kHH, m_dHH, Ha1, Hb1, gHa1, gHb1);
        g_main(g[7], m_kHH, m_dHH, Ha1, Hb2, gHa1, gHb2);
        g_main(g[8], m_kHH, m_dHH, Ha2, Hb1, gHa2, gHb1);
        g_main(g[9], m_kHH, m_dHH, Ha2, Hb2, gHa2, gHb2);

        g_main(g[10], m_kOH, m_dOH, Oa, Hb1, gOa, gHb1);
        g_main(g[11], m_kOH, m_dOH, Oa, Hb2, gOa, gHb2);

        g_main(g[12], m_kOH, m_dOH, Ob, Ha1, gOb, gHa1);
        g_main(g[13], m_kOH, m_dOH, Ob, Ha2, gOb, gHa2);

        g_main(g[14], m_kOO, m_dOO, Oa, Ob, gOa, gOb);

        g_main(g[15], m_kXH, m_dXH, Ha1, X, gHa1, gX);
        g_main(g[16], m_kXH, m_dXH, Ha2, X, gHa2, gX);

        g_main(g[17], m_kXH, m_dXH, Hb1, X, gHb1, gX);
        g_main(g[18], m_kXH, m_dXH, Hb2, X, gHb2, gX);

        g_main(g[19], m_kXO, m_dXO, Oa, X, gOa, gX);

        g_main(g[20], m_kXO, m_dXO, Ob, X, gOb, gX);

        // gradients of the switching function

        gab *= (sac + sbc) * retval / drab;
        gac *= (sab + sbc) * retval / drac;
        gbc *= (sab + sac) * retval / drbc;

        retval *= s;

        for (int n = 0; n < 3; ++n) {
            gOa[n] += gab * rab[n] + gac * rac[n];
            gOb[n] += gbc * rbc[n] - gab * rab[n];
            gX[n] -= gac * rac[n] + gbc * rbc[n];
        }

        energy += retval;
        for (size_t k = 0; k < 9; k++) {
            grad1[9 * i + k] += g1[k];
            grad2[9 * i + k] += g2[k];
        }

        for (size_t k = 0; k < 3; k++) {
            grad3[3 * i + k] += g3[k];
        }

        if (virial != 0) {
            (*virial)[0] += -w1[0 + 0] * g1[0 + 0] - w1[0 + 3] * g1[0 + 3] - w1[0 + 6] * g1[0 + 6] -
                            w2[0 + 0] * g2[0 + 0] - w2[0 + 3] * g2[0 + 3] - w2[0 + 6] * g2[0 + 6] -
                            x[0 + 0] * g3[0 + 0];

            (*virial)[1] += -w1[0 + 0] * g1[0 + 1] - w1[0 + 3] * g1[0 + 4] - w1[0 + 6] * g1[0 + 7] -
                            w2[0 + 0] * g2[0 + 1] - w2[0 + 3] * g2[0 + 4] - w2[0 + 6] * g2[0 + 7] -
                            x[0 + 0] * g3[0 + 1];

            (*virial)[2] += -w1[0 + 0] * g1[0 + 2] - w1[0 + 3] * g1[0 + 5] - w1[0 + 6] * g1[0 + 8] -
                            w2[0 + 0] * g2[0 + 2] - w2[0 + 3] * g2[0 + 5] - w2[0 + 6] * g2[0 + 8] -
                            x[0 + 0] * g3[0 + 2];

            (*virial)[4] += -w1[0 + 1] * g1[0 + 1] - w1[0 + 4] * g1[0 + 4] - w1[0 + 7] * g1[0 + 7] -
                            w2[0 + 1] * g2[0 + 1] - w2[0 + 4] * g2[0 + 4] - w2[0 + 7] * g2[0 + 7] -
                            x[0 + 1] * g3[0 + 1];

            (*virial)[5] += -w1[0 + 1] * g1[0 + 2] - w1[0 + 4] * g1[0 + 5] - w1[0 + 7] * g1[0 + 8] -
                            w2[0 + 1] * g2[0 + 2] - w2[0 + 4] * g2[0 + 5] - w2[0 + 7] * g2[0 + 8] -
                            x[0 + 1] * g3[0 + 2];

            (*virial)[8] += -w1[0 + 2] * g1[0 + 2] - w1[0 + 5] * g1[0 + 5] - w1[0 + 8] * g1[0 + 8] -
                            w2[0 + 2] * g2[0 + 2] - w2[0 + 5] * g2[0 + 5] - w2[0 + 8] * g2[0 + 8] -
                            x[0 + 2] * g3[0 + 2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Output energy: " << energy << std::endl;
    std::cerr << "Output gradients (1) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << grad1[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Output gradients (2) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 9; j++) {
            std::cerr << grad2[9 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Output gradients (3) for " << nt << " monomers:\n";
    for (size_t i = 0; i < nt; i++) {
        for (size_t j = 0; j < 3; j++) {
            std::cerr << grad3[3 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Output virial:\n";
    for (size_t i = 0; i < 9; i++) {
        std::cerr << (*virial)[i] << " , ";
    }
    std::cerr << std::endl;
#endif

    return energy;
}

//----------------------------------------------------------------------------//

x3b_h2o_ion_v1x_deg4_filtered::x3b_h2o_ion_v1x_deg4_filtered(std::string m3) {
    if (m3 == "li+") {
        const double the_poly[] = {-1.160089594691294e+03,   // 0
                                   -7.282152306292261e+02,   // 1
                                   -1.398348953811319e+02,   // 2
                                   2.058162241530466e+02,    // 3
                                   3.738295577441383e+02,    // 4
                                   -2.979452244457583e+02,   // 5
                                   -2.830808479077972e+02,   // 6
                                   4.980516398069948e+02,    // 7
                                   -2.766624526041574e+01,   // 8
                                   7.785544371592293e+02,    // 9
                                   3.467269266034430e+01,    // 10
                                   4.069967114245576e+01,    // 11
                                   -5.961924027655842e+02,   // 12
                                   -6.091749319832783e+00,   // 13
                                   -5.816622501302178e+01,   // 14
                                   5.884636343340163e+01,    // 15
                                   4.277584323227427e+02,    // 16
                                   1.250247155696037e+01,    // 17
                                   3.654093659926691e+02,    // 18
                                   2.221864167688328e+02,    // 19
                                   -9.576354674816864e+01,   // 20
                                   -1.502113482422743e+02,   // 21
                                   1.748390080305978e+00,    // 22
                                   -1.052563111692568e+01,   // 23
                                   -2.288963481084084e+02,   // 24
                                   1.276765477982957e+02,    // 25
                                   -5.916965786655304e+01,   // 26
                                   -4.669671879557435e+02,   // 27
                                   -1.721272341402769e+03,   // 28
                                   4.729830064840633e+02,    // 29
                                   4.098866819129684e+01,    // 30
                                   4.759138467881231e+01,    // 31
                                   3.303634328187500e+01,    // 32
                                   7.530064244976739e+02,    // 33
                                   -1.232608021973931e+01,   // 34
                                   -2.935428751727240e+01,   // 35
                                   2.269176322223048e+02,    // 36
                                   -2.709502392218913e-01,   // 37
                                   -2.191591749401488e+02,   // 38
                                   -3.321267863225777e+02,   // 39
                                   -8.074914125951769e+02,   // 40
                                   1.283202946331503e+03,    // 41
                                   3.099126550117131e+02,    // 42
                                   6.935494774378494e+02,    // 43
                                   -5.239000200373679e+02,   // 44
                                   -1.972817095864513e+00,   // 45
                                   7.141421032452271e+00,    // 46
                                   4.430180932464800e+01,    // 47
                                   3.948978418670464e+02,    // 48
                                   -8.892991581226697e+00,   // 49
                                   9.169486667299515e+01,    // 50
                                   1.017474596949020e+02,    // 51
                                   -4.455021636992354e+02,   // 52
                                   1.917571711157266e+00,    // 53
                                   1.365635320196874e+01,    // 54
                                   -2.870492757790510e+00,   // 55
                                   1.845012581991244e+01,    // 56
                                   3.249246389614913e-01,    // 57
                                   6.737253035202350e+00,    // 58
                                   -1.559277439251137e-01,   // 59
                                   2.702790126137339e+02,    // 60
                                   -6.346328121359817e+01,   // 61
                                   -3.078754064197885e+02,   // 62
                                   -3.918868167121261e+00,   // 63
                                   -6.222995559550577e+02,   // 64
                                   -4.548106393990732e+01,   // 65
                                   -2.834895815082495e-02,   // 66
                                   -1.468241207783972e+01,   // 67
                                   -5.367532919228614e-01,   // 68
                                   -1.669354663506938e+00,   // 69
                                   7.425383476862560e+02,    // 70
                                   3.243582872230440e+02,    // 71
                                   -6.696158051069011e+02,   // 72
                                   3.842991419990177e+01,    // 73
                                   7.783254783384120e+00,    // 74
                                   -3.281115744257939e+01,   // 75
                                   7.484893079798460e-02,    // 76
                                   6.243844580372467e+01,    // 77
                                   5.052648435805850e+02,    // 78
                                   1.106369486250907e+02,    // 79
                                   4.654085442207755e-01,    // 80
                                   -5.140412750405968e+00,   // 81
                                   -5.667159955141018e+02,   // 82
                                   -9.175699863240321e+02,   // 83
                                   -2.247717748843573e+03,   // 84
                                   3.408328830823434e+02,    // 85
                                   -1.355905474890617e+02,   // 86
                                   -8.857179978982169e+02,   // 87
                                   2.025022406857039e+01,    // 88
                                   1.319406198281762e+02,    // 89
                                   6.540588701138176e+02,    // 90
                                   7.054645810306276e+02,    // 91
                                   3.969559502013927e+02,    // 92
                                   1.939920255240662e+02,    // 93
                                   2.518299287936580e+00,    // 94
                                   -9.972003836018185e+02,   // 95
                                   -5.637470517840417e+01,   // 96
                                   -1.358214627660960e+02,   // 97
                                   -2.057058053071033e+01,   // 98
                                   -1.273627843825270e+03,   // 99
                                   -1.498124812301065e+01,   // 100
                                   -1.052224341700308e+02,   // 101
                                   1.712113918406359e+02,    // 102
                                   -1.026824485504058e+00,   // 103
                                   -3.360846758159138e+02,   // 104
                                   1.308989654491087e+02,    // 105
                                   4.676140769426560e+02,    // 106
                                   2.344483790713626e+02,    // 107
                                   1.001551593592013e+02,    // 108
                                   -5.220302334618196e+01,   // 109
                                   1.595689368562496e+02,    // 110
                                   -9.119683487607474e+02,   // 111
                                   1.455790748039578e+03,    // 112
                                   -1.182934431900497e+02,   // 113
                                   2.270108592406306e+03,    // 114
                                   1.462474361946717e+03,    // 115
                                   2.424675055438973e+00,    // 116
                                   -4.981599388999271e+02,   // 117
                                   -7.959516145720629e+02,   // 118
                                   -3.592728589861052e+01,   // 119
                                   -8.654127324866195e+00,   // 120
                                   5.168928461002143e+02,    // 121
                                   1.472255881480938e+02,    // 122
                                   -6.690372745477248e+02,   // 123
                                   -7.946190000093487e+02,   // 124
                                   8.541863175533881e+02,    // 125
                                   -9.920890449893132e-01,   // 126
                                   -1.055012354620331e+02,   // 127
                                   -1.935125690424499e+01,   // 128
                                   -2.492390171267211e+00,   // 129
                                   -5.394945693858228e+01,   // 130
                                   4.324455907252909e+02,    // 131
                                   -1.581584768971579e+02,   // 132
                                   7.412127910925140e-01,    // 133
                                   -1.680875763130518e+02,   // 134
                                   2.167803025421843e-01,    // 135
                                   6.151517486923258e+02,    // 136
                                   1.696001979782282e+01,    // 137
                                   -1.503977531373282e+03,   // 138
                                   1.448549479988138e-01,    // 139
                                   2.429524181491363e+02,    // 140
                                   -2.990540224645226e+02,   // 141
                                   2.385406088505783e-02,    // 142
                                   5.226016382811916e+02,    // 143
                                   -2.305191469248509e+02,   // 144
                                   1.848365577903234e+01,    // 145
                                   2.651739973822606e+01,    // 146
                                   -4.985295752669751e+00,   // 147
                                   -2.019614623181119e+01,   // 148
                                   -9.516055594550100e+00,   // 149
                                   2.139906700312260e+00,    // 150
                                   -3.157554376275061e+01,   // 151
                                   -1.062405785306481e+02,   // 152
                                   6.249845709807499e+00,    // 153
                                   8.259356664962610e+00,    // 154
                                   9.602952031320244e+01,    // 155
                                   4.608564047641036e-01,    // 156
                                   -2.340700262361787e+01,   // 157
                                   1.380829194881315e+03,    // 158
                                   -3.678323376127018e+00,   // 159
                                   5.618410604380686e+02,    // 160
                                   7.657905429700190e+00,    // 161
                                   9.471247162606927e+01,    // 162
                                   -1.029854277448277e+03,   // 163
                                   -5.083526077898079e-03,   // 164
                                   1.428468224775047e-01,    // 165
                                   -2.196281417177527e-03,   // 166
                                   -4.605281218062317e-04,   // 167
                                   1.576129818496548e-01,    // 168
                                   -1.133321453037324e+02,   // 169
                                   2.247676837533576e+01,    // 170
                                   -2.141891240736691e+01,   // 171
                                   -5.829226683058710e-01,   // 172
                                   6.685519114821992e+02,    // 173
                                   5.576761055135069e+02,    // 174
                                   2.122097878367376e-01,    // 175
                                   6.805584376756663e+01,    // 176
                                   1.077430777148087e+02,    // 177
                                   -3.392544417404588e-01,   // 178
                                   7.825084828866721e+01,    // 179
                                   4.183394286707680e+01,    // 180
                                   1.753092034770542e-03,    // 181
                                   -9.125205519228364e-01,   // 182
                                   -1.152309456175957e-04,   // 183
                                   1.901997268841291e-02,    // 184
                                   5.957197053680288e-01,    // 185
                                   4.717198927425887e+02,    // 186
                                   -4.456270837156283e+02,   // 187
                                   1.821248730777555e+02,    // 188
                                   -7.879127723944479e-01,   // 189
                                   -4.644585388933573e-02,   // 190
                                   -1.264339527185251e+00,   // 191
                                   3.101067871131722e-01,    // 192
                                   -3.268217960458590e+01,   // 193
                                   1.653750537575722e-02,    // 194
                                   7.612087757300790e+00,    // 195
                                   -5.823078129408310e+02,   // 196
                                   1.441289139769212e+03,    // 197
                                   1.122364141355138e+01,    // 198
                                   1.908128177858146e+02,    // 199
                                   -5.820440733810743e+00,   // 200
                                   3.378634621930846e+01,    // 201
                                   -9.963198099517229e-01,   // 202
                                   -6.393577895068708e+01,   // 203
                                   -2.395105915518213e+00,   // 204
                                   -2.934395370928570e+00,   // 205
                                   -4.932094542332279e+02,   // 206
                                   -1.059347257246619e+02,   // 207
                                   5.240638651308235e+02,    // 208
                                   -1.121871645254736e+00,   // 209
                                   -7.768416454106409e+02,   // 210
                                   1.320018657673698e+01,    // 211
                                   4.094139254919190e+02,    // 212
                                   1.676961509262793e+01,    // 213
                                   2.508350428163525e+01,    // 214
                                   -8.866289593274727e+02,   // 215
                                   -1.226851276745647e+01,   // 216
                                   7.364836262172963e+00,    // 217
                                   -5.833289199458096e-03,   // 218
                                   -1.415039831075302e+01,   // 219
                                   1.056615179701532e+02,    // 220
                                   1.576805048135084e+01,    // 221
                                   4.818535444512975e-02,    // 222
                                   1.685976760826727e+00,    // 223
                                   -2.035032709316355e-02,   // 224
                                   -2.712439198675729e+02,   // 225
                                   5.928679478015907e+02,    // 226
                                   -2.531322162315710e+02,   // 227
                                   4.850196925830434e+00,    // 228
                                   -7.332428581502083e+02,   // 229
                                   -1.742076286638800e+02,   // 230
                                   3.388836613107913e+00,    // 231
                                   -3.416443037574687e+01,   // 232
                                   2.673071941914152e-01,    // 233
                                   1.036063751600431e-02,    // 234
                                   8.453440367894195e-01,    // 235
                                   -4.191319410285784e+00,   // 236
                                   6.288811594995991e+01,    // 237
                                   2.182550611704494e+01,    // 238
                                   3.594945311752156e+00,    // 239
                                   -6.200019023508253e+02,   // 240
                                   7.671016369247708e+00,    // 241
                                   1.826970547719718e+01,    // 242
                                   -1.936070066284678e+02,   // 243
                                   7.955104724586363e+02,    // 244
                                   -6.602526711871193e+00,   // 245
                                   -6.872987699367863e+01,   // 246
                                   -8.597160061292666e+00,   // 247
                                   -7.447160529468137e+00,   // 248
                                   -1.403243761238889e+01,   // 249
                                   1.656932312595992e+00,    // 250
                                   2.484529410062372e+02,    // 251
                                   -1.369436500854079e+02,   // 252
                                   -2.023392228742960e+01,   // 253
                                   9.714718637458315e+00,    // 254
                                   -1.858860630684485e+01,   // 255
                                   -6.417214333232002e+00,   // 256
                                   7.814807673211592e+02,    // 257
                                   5.464501151201649e-01,    // 258
                                   8.265691206363296e+02,    // 259
                                   -1.799700697320124e+02,   // 260
                                   7.484493734568096e+01,    // 261
                                   -1.577626327943604e+01,   // 262
                                   -1.879429292181587e+00,   // 263
                                   1.303452635476304e+02,    // 264
                                   4.810195812508881e+00,    // 265
                                   -2.208222601732879e+02,   // 266
                                   -5.666609239631174e+02,   // 267
                                   1.840002087676153e+00,    // 268
                                   -9.303558275859493e+02,   // 269
                                   -3.707653511849871e+00,   // 270
                                   -4.293012712103768e+01,   // 271
                                   5.936977720474566e+02,    // 272
                                   4.540969598185392e+02,    // 273
                                   1.690781784077143e+02,    // 274
                                   -5.695436462302268e+01,   // 275
                                   -1.652510777006241e+01,   // 276
                                   -1.170443241955668e+01,   // 277
                                   -3.082502109394367e+02,   // 278
                                   -1.080974383479120e-01,   // 279
                                   -9.460012540253263e-01,   // 280
                                   2.340961339422577e+01,    // 281
                                   -3.398142595406977e+02,   // 282
                                   2.950136196481346e+00,    // 283
                                   -1.746614114215284e+00,   // 284
                                   -1.280958735091182e+00,   // 285
                                   -8.020738517859894e-01,   // 286
                                   9.763516845601725e-01,    // 287
                                   2.063809564644503e+02,    // 288
                                   1.191044714447816e+03,    // 289
                                   6.357291167474708e+01,    // 290
                                   1.221050395973346e+00,    // 291
                                   -7.563917581381252e-01,   // 292
                                   5.042255871919562e-01,    // 293
                                   -2.861397772864869e-01,   // 294
                                   1.982862897307848e+02,    // 295
                                   -2.978679406689520e-01,   // 296
                                   4.992319450235834e+02,    // 297
                                   -7.490022416693893e-03,   // 298
                                   -8.714176005104862e+00,   // 299
                                   -4.973612263193989e-02,   // 300
                                   -3.163086300146723e+01,   // 301
                                   3.530925979204252e+01,    // 302
                                   5.210832236860222e+02,    // 303
                                   1.332232993823329e+03,    // 304
                                   -4.664313807834730e-03,   // 305
                                   -1.752480034234787e+01,   // 306
                                   7.604451304490514e-01,    // 307
                                   7.170658094249881e-01,    // 308
                                   3.405241046319656e+00,    // 309
                                   6.320068705469798e+02,    // 310
                                   3.492460454599304e+00,    // 311
                                   7.240650698303756e-03,    // 312
                                   -1.336731056386004e+02,   // 313
                                   -1.800759039416632e+01,   // 314
                                   1.572625283351878e+01,    // 315
                                   -2.198485520131758e+02,   // 316
                                   -4.880224246006942e+00,   // 317
                                   -1.033856943357198e+00,   // 318
                                   -2.007841561989167e+02,   // 319
                                   -1.636833121816585e+00,   // 320
                                   3.577019272734742e-02,    // 321
                                   -1.753294973536515e+02,   // 322
                                   -9.974742296212813e+00,   // 323
                                   3.758774815566931e+02,    // 324
                                   6.317460241893266e+01,    // 325
                                   6.486311129697930e+02,    // 326
                                   -3.452330708699211e-02,   // 327
                                   8.830720841298221e-01,    // 328
                                   -7.602215573001380e+00,   // 329
                                   2.088466580646973e+02,    // 330
                                   8.640113643886900e+01,    // 331
                                   -1.737755437737209e+03,   // 332
                                   -1.869146196575651e+02,   // 333
                                   1.592261046014050e+01,    // 334
                                   -1.690333399295352e+03,   // 335
                                   -7.678375066072704e+00,   // 336
                                   4.085330780561197e+00,    // 337
                                   6.840820108626738e+02,    // 338
                                   -5.370209029216191e-01,   // 339
                                   2.177132492552174e+01,    // 340
                                   -2.548599534229133e+02,   // 341
                                   1.002099260387287e+02,    // 342
                                   -1.659925395047477e-02,   // 343
                                   -3.237110037961852e-01,   // 344
                                   1.341169132188188e+02,    // 345
                                   -7.423350584314323e+00,   // 346
                                   -3.347261349547232e+01,   // 347
                                   -1.757654774873890e+00,   // 348
                                   1.941928316179834e+02,    // 349
                                   2.631750879298596e+01,    // 350
                                   4.748938252780463e+00,    // 351
                                   -9.267312164278692e+02,   // 352
                                   9.434129510676772e+01,    // 353
                                   2.819034431686522e-01,    // 354
                                   -1.426967791518044e+01,   // 355
                                   -1.069762124840763e+01,   // 356
                                   2.725360442061487e+01,    // 357
                                   -4.522837914698122e-01,   // 358
                                   3.835394505763910e+01,    // 359
                                   2.934933902162292e+02,    // 360
                                   -5.460765771295538e+00,   // 361
                                   6.910974935212307e+02,    // 362
                                   -1.877069747186732e+01,   // 363
                                   4.493117761583611e+00,    // 364
                                   -1.398641270627689e+02,   // 365
                                   1.403555745467421e+02,    // 366
                                   -2.020666017229213e+02,   // 367
                                   4.695304909950695e+00,    // 368
                                   2.985555591685317e-01,    // 369
                                   -1.036742657607889e+03,   // 370
                                   -3.104529601221303e+00,   // 371
                                   2.997033482596228e-01,    // 372
                                   -1.793236621530915e+02,   // 373
                                   -1.765875971247043e+01,   // 374
                                   -3.119188309901505e+00,   // 375
                                   -5.433862163561730e+02,   // 376
                                   6.782807577808775e+01,    // 377
                                   -2.655982408319322e+00,   // 378
                                   -8.393558001363841e+00,   // 379
                                   1.058264976770955e+01,    // 380
                                   -1.195603861227412e+01,   // 381
                                   3.730327007439550e-01,    // 382
                                   1.066068745233516e-01,    // 383
                                   -2.841704109430130e+00,   // 384
                                   5.002145511595466e+00,    // 385
                                   1.044661941602436e+02,    // 386
                                   4.512579121835281e+02,    // 387
                                   2.462238348209195e+00,    // 388
                                   2.501914670943962e+00,    // 389
                                   1.316983850505951e+01,    // 390
                                   1.734616197254785e+00,    // 391
                                   4.797846790382947e+01,    // 392
                                   -2.911857753410728e-03,   // 393
                                   -1.281067215511080e-01,   // 394
                                   5.447354534629847e+02,    // 395
                                   1.279090839045556e+02,    // 396
                                   1.078028505078298e+00,    // 397
                                   2.334754134772486e-04,    // 398
                                   1.092565610630955e+01,    // 399
                                   -4.964839094834722e-02,   // 400
                                   -2.155794856448981e+00,   // 401
                                   -3.902355650223758e+01,   // 402
                                   -6.124092878993349e+02,   // 403
                                   -8.258863938820891e+00,   // 404
                                   -4.024230671707610e+00,   // 405
                                   1.612943001493035e+01,    // 406
                                   -7.587547707413393e+02,   // 407
                                   2.006784455029081e+02,    // 408
                                   1.067598745377464e+00,    // 409
                                   -4.066849150175401e+02,   // 410
                                   5.913896049087534e+02,    // 411
                                   -1.644506353958091e+02,   // 412
                                   7.191839642690876e+00,    // 413
                                   -1.101115750880275e+01,   // 414
                                   2.044865975733173e+02,    // 415
                                   3.796026328274604e-03,    // 416
                                   3.499005624366298e+00,    // 417
                                   -3.226076641007884e+02,   // 418
                                   2.207892269465812e+02,    // 419
                                   -6.178072954566260e+02,   // 420
                                   5.916371485737692e+02,    // 421
                                   1.803752869951149e+02,    // 422
                                   -1.763341082554623e-02,   // 423
                                   -3.500290901968650e+01,   // 424
                                   -1.826181001525873e-01,   // 425
                                   9.685946321045542e+00,    // 426
                                   3.930285842596630e+00,    // 427
                                   -5.477121747739563e-01,   // 428
                                   2.021797947518532e+01,    // 429
                                   -3.304841279951766e+01,   // 430
                                   1.596213324684848e+00,    // 431
                                   -3.567023232059318e+01,   // 432
                                   8.976264882480194e+01,    // 433
                                   -4.170277782075239e+02,   // 434
                                   -5.396425903961861e-01,   // 435
                                   2.281490520069988e+02,    // 436
                                   -1.291517162163219e+02,   // 437
                                   -2.450472040417963e+01,   // 438
                                   -1.530943226073707e-03,   // 439
                                   -1.043912454331196e+03,   // 440
                                   -2.521057224371013e+01,   // 441
                                   -2.614243483243932e-02,   // 442
                                   3.858454024930857e-04,    // 443
                                   -5.003798048397651e-01,   // 444
                                   6.745345199799808e+00,    // 445
                                   -1.203467407500763e+02,   // 446
                                   5.418930024842267e+02,    // 447
                                   3.669437602253322e+02,    // 448
                                   -5.826520968329620e-01,   // 449
                                   -6.600228847974134e+00,   // 450
                                   2.259062603723578e+03,    // 451
                                   -3.219809601929544e+00,   // 452
                                   1.531475186306428e+01,    // 453
                                   1.923745300113607e-01,    // 454
                                   -1.003531419103480e+01,   // 455
                                   -5.062817662949446e+01,   // 456
                                   -8.262264698292606e+01,   // 457
                                   -5.677020744395985e-01,   // 458
                                   5.740000444641572e+00,    // 459
                                   -2.608310227134022e+01,   // 460
                                   3.377107906518368e+00,    // 461
                                   -3.521675137087191e+00,   // 462
                                   1.235274398055809e+02,    // 463
                                   -6.810051263838955e-01,   // 464
                                   -1.165870161983652e+03,   // 465
                                   8.846544682393862e+02,    // 466
                                   1.851010685226658e+00,    // 467
                                   -1.842933624075750e+00,   // 468
                                   -3.261974414203391e+00,   // 469
                                   2.078887971808618e-02,    // 470
                                   1.999454767346064e-01,    // 471
                                   2.518238690224583e+00,    // 472
                                   8.854507576630213e-01,    // 473
                                   -7.776898806582833e-01,   // 474
                                   -3.604463836584941e+02,   // 475
                                   1.162934357243269e+00,    // 476
                                   8.623381980375333e-02,    // 477
                                   -1.567964914250435e+02,   // 478
                                   -8.615032227855622e+02,   // 479
                                   -1.635712674007188e-02,   // 480
                                   -1.553346421894111e+02,   // 481
                                   -6.751631546021819e+01,   // 482
                                   -4.286093466860190e-01,   // 483
                                   2.383832652822579e+02,    // 484
                                   -3.145017409561521e+02,   // 485
                                   3.394167439092160e+00,    // 486
                                   -1.190688510075284e+01,   // 487
                                   1.093475914802531e+01,    // 488
                                   -4.064352605513099e+02,   // 489
                                   5.282279126614363e+02,    // 490
                                   -3.990120381059102e-03,   // 491
                                   1.730181042784333e+01,    // 492
                                   -2.175961388561790e+01,   // 493
                                   1.111128435589631e+01,    // 494
                                   -4.713128911858558e+01,   // 495
                                   9.116879618955272e-02,    // 496
                                   -1.145719011886473e+01,   // 497
                                   -8.743980529103330e-02,   // 498
                                   1.561064343507064e-01,    // 499
                                   -5.399628396519111e+02,   // 500
                                   1.069905686728378e+01,    // 501
                                   8.191905003639308e+00,    // 502
                                   4.195848791732365e+01,    // 503
                                   2.731410400085826e+01,    // 504
                                   -3.870646258349815e+00,   // 505
                                   -2.189566810629133e+02,   // 506
                                   -2.109666915392744e-04,   // 507
                                   6.359434551286899e-01,    // 508
                                   -1.844947380062843e-03,   // 509
                                   -1.643915784443890e+01,   // 510
                                   -2.168501775426415e+02,   // 511
                                   2.451520966409329e-01,    // 512
                                   2.558900099413633e+02,    // 513
                                   -3.628921725704236e+02,   // 514
                                   1.271768472128280e+02,    // 515
                                   -5.716260486609968e+02,   // 516
                                   2.181426279794564e-01,    // 517
                                   -1.317765268155577e-02,   // 518
                                   -1.143210317082554e-01,   // 519
                                   -5.211358653794780e+02,   // 520
                                   -1.133722901915792e+00,   // 521
                                   -1.885457119756486e+02,   // 522
                                   -2.600789464902141e+00,   // 523
                                   -6.411285785158062e-01,   // 524
                                   -2.032150081126826e+00,   // 525
                                   -5.010323563676988e+02,   // 526
                                   3.996165911276853e+00,    // 527
                                   -1.028209442822323e+02,   // 528
                                   2.929645179943665e+02,    // 529
                                   -7.108857265317826e+00,   // 530
                                   -3.861873234104532e-02,   // 531
                                   -1.965824749705624e-01,   // 532
                                   -5.884813506575379e+00,   // 533
                                   1.377284650822421e+02,    // 534
                                   -8.356844775644872e+01,   // 535
                                   -3.235375851170571e+02,   // 536
                                   3.175179511723787e+02,    // 537
                                   4.689981894463293e+02,    // 538
                                   1.461864121591177e+02,    // 539
                                   4.031834457256348e+00,    // 540
                                   2.762390515840905e+01,    // 541
                                   -1.698906076342809e-01,   // 542
                                   -4.607175901689991e+02,   // 543
                                   6.070216124550982e+01,    // 544
                                   -7.966547116317233e+02,   // 545
                                   7.602416107370796e-01,    // 546
                                   -4.116545227780421e-02,   // 547
                                   1.234243207312170e+02,    // 548
                                   -9.239108074326701e+00,   // 549
                                   -9.250552915101885e-03,   // 550
                                   8.106588185631381e+00,    // 551
                                   -5.794765688036356e+00,   // 552
                                   -5.647664049863243e-02,   // 553
                                   -1.272249390894011e+01,   // 554
                                   -1.373172996868379e-01,   // 555
                                   7.275622573316674e+00,    // 556
                                   1.497168576800040e+02,    // 557
                                   -5.672064564904154e-02,   // 558
                                   1.855292945800248e+01,    // 559
                                   -1.449830007689197e-02,   // 560
                                   6.635509934120666e+02,    // 561
                                   -2.551962349314389e+02,   // 562
                                   -3.427835340247179e+02,   // 563
                                   2.529228336605058e-02,    // 564
                                   -2.226170381884076e+01,   // 565
                                   9.751953590627963e-02,    // 566
                                   -1.106469225271466e+02,   // 567
                                   2.527396867019107e+00,    // 568
                                   2.781451254798025e+02,    // 569
                                   -1.672529877863103e+02,   // 570
                                   1.394540037281164e-01,    // 571
                                   -1.226344584322825e+01,   // 572
                                   1.849453855453205e-02,    // 573
                                   -2.113614663019144e+01,   // 574
                                   -8.032576215282837e-01,   // 575
                                   -2.149991102748831e-01,   // 576
                                   -4.819989171285225e+01,   // 577
                                   8.370732480016264e-02,    // 578
                                   7.723036792333997e+01,    // 579
                                   -1.016766852329539e+00,   // 580
                                   -1.497454228653379e+01,   // 581
                                   -7.122149548886613e-01,   // 582
                                   5.082646871678753e+01,    // 583
                                   3.281516621113341e+02,    // 584
                                   3.968822594628414e-02,    // 585
                                   6.014668001559885e+00,    // 586
                                   -2.561266467111069e-01,   // 587
                                   6.014851274945698e+01,    // 588
                                   -2.073584272972416e-03,   // 589
                                   1.931876188491874e-02,    // 590
                                   -6.501459464484333e-02,   // 591
                                   7.862019131598880e-01,    // 592
                                   -2.224202816278909e-01,   // 593
                                   7.619394611399667e+01,    // 594
                                   7.072262414124050e+00,    // 595
                                   -2.636857902955694e+01,   // 596
                                   1.021516879324537e+01,    // 597
                                   -1.538235999543720e+00,   // 598
                                   -6.222811525792201e-01,   // 599
                                   -1.401301356740919e+02,   // 600
                                   -7.269795171439927e+00,   // 601
                                   -6.693036713954925e-03,   // 602
                                   2.348895693729061e+02,    // 603
                                   -3.507142562385928e-01,   // 604
                                   3.114042930022371e+01,    // 605
                                   -2.259293867252240e+02,   // 606
                                   3.544162135042576e+00,    // 607
                                   -1.872792740782205e-03,   // 608
                                   1.800974078165344e+00,    // 609
                                   1.910899040560768e+00,    // 610
                                   -3.241283435503007e+01,   // 611
                                   -9.105669312732931e+01,   // 612
                                   -9.123448794078403e+00,   // 613
                                   -5.768601256462628e-04,   // 614
                                   1.571030135847967e+01,    // 615
                                   3.306465326474918e+01,    // 616
                                   1.311112054605982e+00,    // 617
                                   1.003550703647174e+03,    // 618
                                   3.612777565464834e-01,    // 619
                                   1.430390059397255e+00,    // 620
                                   2.601737432113293e+01,    // 621
                                   6.051241302161142e+00,    // 622
                                   -2.053388948817521e-01,   // 623
                                   -1.098038481402388e+00,   // 624
                                   7.224978954003214e+01,    // 625
                                   -4.957105519172364e+02,   // 626
                                   -3.518370093202025e+01,   // 627
                                   -5.695465506364600e+00,   // 628
                                   1.488966925636384e+00,    // 629
                                   1.956952982611978e+02,    // 630
                                   -1.840702774175460e+02,   // 631
                                   1.741996010917730e-05,    // 632
                                   1.760032696710995e+03,    // 633
                                   -1.430882359968909e+03,   // 634
                                   1.379858917232936e+00,    // 635
                                   1.350148375335877e+03,    // 636
                                   2.963748428786013e+01,    // 637
                                   1.024662641829799e+03,    // 638
                                   3.782584840885687e+01,    // 639
                                   -3.850311970241930e+01,   // 640
                                   -3.186469764084309e-02,   // 641
                                   3.125855758489075e+01,    // 642
                                   -9.266297273376881e+02,   // 643
                                   6.985350151117042e+01,    // 644
                                   -2.635592741584805e+00,   // 645
                                   2.506999127869805e+02,    // 646
                                   -5.548767462123176e-01,   // 647
                                   -8.719170812540539e-01,   // 648
                                   9.512838977603457e-03,    // 649
                                   -2.737307009353593e-01,   // 650
                                   -4.729851398320050e+01,   // 651
                                   -4.063471938847734e+01,   // 652
                                   -1.239551916275341e+02,   // 653
                                   -1.027805953251955e-01,   // 654
                                   -6.692386161258365e+01,   // 655
                                   -5.531302900639193e+02,   // 656
                                   -3.257825644009474e+01,   // 657
                                   -2.628362410940620e+00,   // 658
                                   -1.366391387313908e+01,   // 659
                                   -2.551851208691980e+02,   // 660
                                   -1.077838560513742e+02,   // 661
                                   5.409726635082206e-03,    // 662
                                   -8.080777579723173e-01,   // 663
                                   6.587577726811790e+00,    // 664
                                   5.738713968285042e+00,    // 665
                                   1.921095098629275e+02,    // 666
                                   3.391214532756276e-03,    // 667
                                   1.063273096145469e+00,    // 668
                                   -2.700463574231448e+01,   // 669
                                   1.940331073787246e+00,    // 670
                                   -4.400045829545024e+01,   // 671
                                   -7.098565635516896e-02,   // 672
                                   1.163325992919931e+00,    // 673
                                   1.901839868629724e-02,    // 674
                                   3.268701036503089e+01,    // 675
                                   6.614180607918694e+01,    // 676
                                   1.797766809539887e+02,    // 677
                                   2.785369551495161e+00,    // 678
                                   -4.369511416799724e+01,   // 679
                                   1.819811319546945e+01,    // 680
                                   -6.238634741435732e+01,   // 681
                                   1.956519965363881e-01,    // 682
                                   9.384174955892590e+00,    // 683
                                   1.098315952278280e+01,    // 684
                                   -2.805099306433919e-02,   // 685
                                   -1.538176574698907e-01,   // 686
                                   -1.068509699394979e+01,   // 687
                                   -2.830727957090905e+00,   // 688
                                   -4.550249095669215e-03,   // 689
                                   6.834928551247971e+01,    // 690
                                   -7.099551120917953e+01,   // 691
                                   -3.273683476183711e+02,   // 692
                                   -3.033202283848635e+02,   // 693
                                   -1.552012389789424e+00,   // 694
                                   4.265394368612785e+02,    // 695
                                   4.536101621369359e-01,    // 696
                                   -2.015358025537223e+01,   // 697
                                   9.781043512805785e-01,    // 698
                                   1.702599664494335e-01,    // 699
                                   1.937353338617085e+02,    // 700
                                   -2.407847213592236e+01,   // 701
                                   -7.113132613452977e-01,   // 702
                                   5.591107787595506e-01,    // 703
                                   3.594418703813959e+02,    // 704
                                   3.352280474064462e+00,    // 705
                                   1.909441146425793e+02,    // 706
                                   7.798849602843079e+01,    // 707
                                   3.471703195759271e+02,    // 708
                                   1.453573601902169e-02,    // 709
                                   4.539619790604374e+01,    // 710
                                   7.685867315201435e+02,    // 711
                                   1.675525737814826e+02,    // 712
                                   9.188953489544138e+00,    // 713
                                   3.413877440106068e-02,    // 714
                                   -4.379427479183282e+00,   // 715
                                   9.228774467210292e+01,    // 716
                                   2.791735551455041e-02,    // 717
                                   1.128898190243619e+03,    // 718
                                   -5.172009025346732e+01,   // 719
                                   -8.480309159147779e+02,   // 720
                                   -4.323613773255203e+02,   // 721
                                   5.658836867028834e-01,    // 722
                                   -3.284376261435440e-02,   // 723
                                   -2.002584980158124e-03,   // 724
                                   -1.169197493346718e+01,   // 725
                                   -4.606359334353557e+01,   // 726
                                   1.642936132283531e+02,    // 727
                                   1.651557310752057e+00,    // 728
                                   6.395959758846219e-01,    // 729
                                   2.123233608191876e+00,    // 730
                                   -1.365289827253784e+03,   // 731
                                   -6.645126509901741e+01,   // 732
                                   -6.983313147582493e+02,   // 733
                                   -2.074696315449144e+00,   // 734
                                   -1.782471656101971e+01,   // 735
                                   -1.764430959367777e+02,   // 736
                                   -2.174847571566457e+02,   // 737
                                   -2.834209279262194e+02,   // 738
                                   -1.884339684227191e+02,   // 739
                                   -1.253776778066233e+00,   // 740
                                   1.309724168975190e+02,    // 741
                                   -3.524354842586881e-02,   // 742
                                   -4.290309606104963e-03,   // 743
                                   -3.510317094058759e+00,   // 744
                                   7.770849716379525e+00,    // 745
                                   -2.102354938419852e+01,   // 746
                                   -1.689845366011500e+00,   // 747
                                   4.809197006512886e+00,    // 748
                                   2.126496598768625e-05,    // 749
                                   1.132649927581433e-01,    // 750
                                   7.328747116706180e+01,    // 751
                                   5.569994709101089e+02,    // 752
                                   -9.929864351271592e+02,   // 753
                                   -9.782663911891174e+02,   // 754
                                   4.041658470197500e-02,    // 755
                                   6.617187831647520e+01,    // 756
                                   -7.636487506505719e+01,   // 757
                                   3.718409083547889e+00,    // 758
                                   -1.697686777510680e+00,   // 759
                                   -2.883486616809326e+01,   // 760
                                   -1.197249140310367e+01,   // 761
                                   3.998084442524499e-02,    // 762
                                   -3.059151204656455e+01,   // 763
                                   2.317453753814607e+01,    // 764
                                   8.628130438586849e-03,    // 765
                                   2.628934146052575e+01,    // 766
                                   -4.721077783354628e+00,   // 767
                                   9.171686369116577e-02,    // 768
                                   -4.925592368211820e-02,   // 769
                                   -9.101313882170996e+02,   // 770
                                   1.181202288669822e-01,    // 771
                                   -5.158519153324625e+00,   // 772
                                   3.066033971331592e-01,    // 773
                                   -7.945626145308211e+00,   // 774
                                   8.257763080739373e+01,    // 775
                                   -2.297692914934968e+02,   // 776
                                   -8.057894659009265e-01,   // 777
                                   1.562108607705224e-02,    // 778
                                   1.031322819056357e+00,    // 779
                                   -5.032570736777715e-02,   // 780
                                   1.307467492695309e+02,    // 781
                                   2.157766397965743e+02,    // 782
                                   3.539819644455326e+00,    // 783
                                   3.980188009924052e+02,    // 784
                                   2.640922467404446e+02,    // 785
                                   2.930138520086446e+02,    // 786
                                   8.144774768331159e-01,    // 787
                                   -1.007426446866223e+01,   // 788
                                   6.427106427952051e-01,    // 789
                                   5.303008814633662e+02,    // 790
                                   3.255449445387428e+02,    // 791
                                   -1.115800656971878e+00,   // 792
                                   5.858881598673471e-01,    // 793
                                   5.421271197321190e+01,    // 794
                                   3.348393878281521e-01,    // 795
                                   -3.276042125673982e+02,   // 796
                                   -1.496522158887531e+02,   // 797
                                   5.999502321568560e+01,    // 798
                                   8.082819594207142e+01,    // 799
                                   -4.481171900601730e+01,   // 800
                                   -5.144070069610003e+00,   // 801
                                   1.524893310767066e+03,    // 802
                                   1.113350581220089e-01,    // 803
                                   -6.123146303533848e+01,   // 804
                                   -1.972419837552394e+00,   // 805
                                   -3.351296097974711e+01,   // 806
                                   3.940064409760364e+00,    // 807
                                   3.679167086858285e+00,    // 808
                                   2.101824813584658e+00,    // 809
                                   1.844291767206714e+01,    // 810
                                   1.593238537769107e+01,    // 811
                                   1.408573068996878e+00,    // 812
                                   -3.591747337584450e+02,   // 813
                                   1.035245461426785e+00,    // 814
                                   1.079224544429541e+01,    // 815
                                   4.501529986305668e+01,    // 816
                                   -2.654222805043019e+02,   // 817
                                   1.782135066747636e+03,    // 818
                                   1.216824836654230e+01,    // 819
                                   -1.679293350584758e-02,   // 820
                                   -1.446361374274541e+00,   // 821
                                   1.640544124035257e+01,    // 822
                                   -1.338174137529750e+00,   // 823
                                   8.594239097994432e-01,    // 824
                                   -7.333448598163213e+01,   // 825
                                   -5.635596396012389e-03,   // 826
                                   2.746533848723915e+00,    // 827
                                   -1.669260266556128e+01,   // 828
                                   -1.167582735515787e+00,   // 829
                                   3.493767311128922e+02,    // 830
                                   -5.710788201732744e+01,   // 831
                                   -1.109913498821368e+02,   // 832
                                   -1.780227005593942e-02,   // 833
                                   3.755887697865403e+01,    // 834
                                   -2.782539539128995e+02,   // 835
                                   -1.308385738612782e+01,   // 836
                                   -5.163903069616171e+01,   // 837
                                   -2.057790455562444e+00,   // 838
                                   -1.590371837222229e+00,   // 839
                                   -6.680319184115502e+02,   // 840
                                   2.862663411066211e-01,    // 841
                                   2.670473891640125e-01,    // 842
                                   -2.321082343187298e-02,   // 843
                                   4.580308783623664e+02,    // 844
                                   2.769160770379452e+01,    // 845
                                   -8.501864208202064e-04,   // 846
                                   -2.490116541718397e-02,   // 847
                                   1.159841390394109e+01,    // 848
                                   -4.503541833963329e+02,   // 849
                                   1.794826088621178e-01,    // 850
                                   1.942678602696606e+02,    // 851
                                   5.847765353995777e+00,    // 852
                                   7.407838501660866e-01,    // 853
                                   -1.260730128759154e+02,   // 854
                                   -3.313155469922680e+02,   // 855
                                   1.276676079996855e+00,    // 856
                                   -1.781357007702047e-02,   // 857
                                   -4.843398111088491e-02,   // 858
                                   -2.667926983328178e+00,   // 859
                                   -4.552932911408010e+00,   // 860
                                   1.680215096996722e+01,    // 861
                                   1.827645476380000e+02,    // 862
                                   1.594569283301816e+02,    // 863
                                   9.028687700295752e+00,    // 864
                                   1.022274307295881e-01,    // 865
                                   5.324549132559110e+02,    // 866
                                   -2.860039177288157e+02,   // 867
                                   -1.547679334419966e-02,   // 868
                                   -6.737458565294821e-03,   // 869
                                   3.990839478621983e+01,    // 870
                                   -3.250412546641391e+02,   // 871
                                   -1.401635676572932e+00,   // 872
                                   1.238305452761350e+02,    // 873
                                   2.311008906601770e+00,    // 874
                                   -9.003258048394775e+01,   // 875
                                   1.045120024155341e+02,    // 876
                                   2.292732999200061e+01,    // 877
                                   -4.707035353044657e+01,   // 878
                                   5.841374456874248e-03,    // 879
                                   -1.213148342688719e+02,   // 880
                                   -4.880824534434462e-06,   // 881
                                   3.162278989784031e+02,    // 882
                                   4.415898499740300e-02,    // 883
                                   1.173563528049076e-03,    // 884
                                   -1.530094803286396e+00,   // 885
                                   -3.512085676764414e+02,   // 886
                                   4.599660233954449e-02,    // 887
                                   3.019177632106114e+01,    // 888
                                   -6.616352222743005e+01,   // 889
                                   -4.878680492730433e+01,   // 890
                                   3.477539673635310e-01,    // 891
                                   1.383984822772057e-03,    // 892
                                   -1.319536738768756e+01,   // 893
                                   -4.248075619098974e-01,   // 894
                                   -5.514242059341082e-02,   // 895
                                   1.523909343817494e-02,    // 896
                                   6.658221162644909e+00,    // 897
                                   -9.163643971709950e+00,   // 898
                                   -1.348885320086047e+01,   // 899
                                   1.546412604437129e-04,    // 900
                                   1.250234935657932e+02,    // 901
                                   2.714453438303947e+01,    // 902
                                   3.639429550574156e+01,    // 903
                                   7.817731818076250e+01,    // 904
                                   -3.628959349701238e+01,   // 905
                                   2.842040933692927e+01,    // 906
                                   3.662013084689043e-02,    // 907
                                   2.146166222863903e+02,    // 908
                                   -1.311716550073660e-01,   // 909
                                   1.316806023942544e+02,    // 910
                                   -6.122397175086001e+00,   // 911
                                   1.508098275792295e-01,    // 912
                                   6.520447187418277e-01,    // 913
                                   -1.298317628905327e+01,   // 914
                                   1.031046525746960e+03,    // 915
                                   1.170636679745099e+01,    // 916
                                   -1.287047067170029e+02,   // 917
                                   1.643925499884252e+02,    // 918
                                   -1.743806391195618e+01,   // 919
                                   7.802756363132045e-02,    // 920
                                   2.819224260983589e-02,    // 921
                                   -9.310255450938994e+00,   // 922
                                   -3.004908435261350e+01,   // 923
                                   -1.242036530672068e+02,   // 924
                                   -1.066718459908860e-02,   // 925
                                   3.059215576080677e-01,    // 926
                                   -1.253918186242146e+01,   // 927
                                   -7.643044638542078e-02,   // 928
                                   -1.666964219394695e-01,   // 929
                                   2.075791044151127e+00,    // 930
                                   4.684830734135346e-01,    // 931
                                   -1.604203592724905e+01,   // 932
                                   -2.430537636004664e+01,   // 933
                                   -5.913885558425585e+00,   // 934
                                   -1.473813905497540e+00,   // 935
                                   -8.279638769832822e+01,   // 936
                                   1.140259637241280e-01,    // 937
                                   -1.238696358994758e+02,   // 938
                                   -7.594729920482318e-03,   // 939
                                   -7.354173777539200e-02,   // 940
                                   -1.058518377477904e+02,   // 941
                                   2.888236170809852e+00,    // 942
                                   3.318298672216434e-02,    // 943
                                   -2.106077044702672e+00,   // 944
                                   1.049585093048172e-03,    // 945
                                   2.686206349911472e+00,    // 946
                                   -2.706911118724955e-02,   // 947
                                   -1.759226939116629e+00,   // 948
                                   -3.651910153918206e-01,   // 949
                                   1.578671047348887e+02,    // 950
                                   -7.590209390920445e+01,   // 951
                                   8.482571551994276e-04,    // 952
                                   7.892493784360156e+00,    // 953
                                   -6.239974321319031e-01,   // 954
                                   -2.622234400582005e+02,   // 955
                                   5.424613266438754e+01,    // 956
                                   4.286250353741090e-01,    // 957
                                   8.349594187963751e+01,    // 958
                                   2.145607332696663e-02,    // 959
                                   4.042288007423461e-01,    // 960
                                   -3.284888370113691e+02,   // 961
                                   2.214300026339901e-02,    // 962
                                   -2.255195906563033e+03,   // 963
                                   1.086930989349169e+02,    // 964
                                   -9.728874907205778e-01,   // 965
                                   1.521234575414232e+01,    // 966
                                   3.495020416619725e+01,    // 967
                                   -1.517674242557927e-04,   // 968
                                   5.629189645592601e+02,    // 969
                                   -2.559807635109503e-01,   // 970
                                   -9.340848466121123e-03,   // 971
                                   -6.332851863547868e+00,   // 972
                                   6.083121241698423e+01,    // 973
                                   -7.411321461930172e+02,   // 974
                                   8.019455351257913e+02,    // 975
                                   5.413198009207216e+00,    // 976
                                   -6.336167277378459e-01,   // 977
                                   2.500153788642813e+01,    // 978
                                   5.734467964494348e+01,    // 979
                                   4.237586524089218e-03,    // 980
                                   -2.067974400593746e+02,   // 981
                                   -9.131558192283984e+00,   // 982
                                   1.409903309143950e+02,    // 983
                                   1.208585962832092e-02,    // 984
                                   3.297244446660032e-02,    // 985
                                   -1.839093475114396e-02,   // 986
                                   1.592530453639652e+01,    // 987
                                   6.376275683440498e-03,    // 988
                                   1.934772566088825e+01,    // 989
                                   6.474056981635809e+00,    // 990
                                   3.224375229751662e+00,    // 991
                                   1.085459374311455e+03,    // 992
                                   -9.033587065338670e-01,   // 993
                                   1.034419942269812e-02,    // 994
                                   2.214329884607638e+01,    // 995
                                   -2.346886995259778e-01,   // 996
                                   7.584117619243411e+01,    // 997
                                   1.247642978921109e-01,    // 998
                                   -1.117768243957909e+01,   // 999
                                   -1.339589409241134e+03,   // 1000
                                   5.964043739271654e-01,    // 1001
                                   -3.706612487743379e+00,   // 1002
                                   2.593455662744230e-03,    // 1003
                                   7.268676640962310e-02,    // 1004
                                   -6.234708100804825e-03,   // 1005
                                   1.243860813023088e+00,    // 1006
                                   -3.260411898468693e+00,   // 1007
                                   1.980099439007518e+01,    // 1008
                                   4.048636278086030e+01,    // 1009
                                   -3.121789837097498e+02,   // 1010
                                   -4.801817134387622e+01,   // 1011
                                   -1.163554196627544e+00,   // 1012
                                   4.127993191924044e+00,    // 1013
                                   -1.775733667033793e-03,   // 1014
                                   -3.438292212734237e+02};  // 1015

        m_r3i = 0.000000000000000e+00;        // A
        m_r3f = 4.500000000000000e+00;        // A
        m_kHH_intra = 1.679876695401232e-01;  // A^(-1)
        m_kOH_intra = 3.699637401847997e-01;  // A^(-1)
        m_kHH = 1.684254969412778e-01;        // A^(-1)
        m_kOH = 9.935182415951276e-01;        // A^(-1)
        m_kOO = 8.505367453684953e-01;        // A^(-1)
        m_kXO = 1.919355248053607e-01;        // A^(-1)
        m_kXH = 4.576806684033723e-02;        // A^(-1)
        m_dHH_intra = 1.975171570009532e+00;  // A
        m_dOH_intra = 1.068868829761452e+01;  // A
        m_dHH = 1.129172172428989e+00;        // A
        m_dOH = 4.802676630871010e+00;        // A
        m_dOO = 9.078259767894229e+00;        // A
        m_dXO = 6.346405495344363e+00;        // A
        m_dXH = 3.739394864987378e+00;        // A

        std::copy(the_poly, the_poly + ncoeffs, m_coeffs);

    } else if (m3 == "na+") {
        const double the_poly[] = {
            4.308868375740903e+02,   // 0
            -3.191775434204529e+02,  // 1
            3.323544471308123e+02,   // 2
            -2.261047913240567e+02,  // 3
            3.060229396221886e+02,   // 4
            -1.947082356113767e+02,  // 5
            -5.366926322167978e+02,  // 6
            -6.290257771178406e+01,  // 7
            4.769706355881793e+02,   // 8
            7.625860026470913e+01,   // 9
            -1.792943060833107e+02,  // 10
            1.089305746736779e+02,   // 11
            -2.827814288576009e+02,  // 12
            -7.028641665020325e+00,  // 13
            2.342563840273334e+01,   // 14
            -6.138665769343263e+01,  // 15
            2.375403651631666e+01,   // 16
            8.037787426692333e+01,   // 17
            1.300723088919588e+02,   // 18
            -5.382665186192715e+01,  // 19
            -2.479732663951264e+00,  // 20
            5.778907154595623e+01,   // 21
            3.826685526051769e+00,   // 22
            -4.079808007844304e+00,  // 23
            -1.247957663008547e+02,  // 24
            -4.386946818024948e+00,  // 25
            1.059793267318720e-01,   // 26
            -1.391129847178048e+02,  // 27
            3.797920713426813e-01,   // 28
            -1.563004113439337e+02,  // 29
            2.268371723248337e+02,   // 30
            -9.079183883276285e+01,  // 31
            1.935707183888356e+02,   // 32
            -1.383591240365476e+00,  // 33
            -5.810533135464011e+02,  // 34
            -2.719322049932542e-01,  // 35
            1.503934077045871e+01,   // 36
            -3.544844913695959e+00,  // 37
            1.427633661760111e+01,   // 38
            -1.583215175746976e+02,  // 39
            3.909599835033119e+02,   // 40
            -5.341545921343892e+02,  // 41
            -1.556450880419642e+02,  // 42
            4.494993613413051e+00,   // 43
            1.074028159352463e+02,   // 44
            4.613500026411208e+00,   // 45
            5.325681875643182e+00,   // 46
            -6.700246500051909e+01,  // 47
            -1.611463389852549e-01,  // 48
            -3.233802929315544e+02,  // 49
            -3.547065129174800e-01,  // 50
            2.426141440983974e+02,   // 51
            2.580901129303710e+02,   // 52
            9.470310583602132e+00,   // 53
            -2.638461039603810e+00,  // 54
            -4.325108683456060e+00,  // 55
            -6.844139534663408e+02,  // 56
            -8.943079634345975e+01,  // 57
            9.515276468457384e+02,   // 58
            -7.411799938433138e+01,  // 59
            -1.521500631453794e+02,  // 60
            3.604840111581139e+01,   // 61
            -1.577891310543510e+00,  // 62
            1.759370105214146e+00,   // 63
            -6.423038036806470e+01,  // 64
            6.296885739732511e+01,   // 65
            -1.621524101892210e+02,  // 66
            -5.736531420530834e+00,  // 67
            6.592964697450135e+01,   // 68
            1.178135876782064e+02,   // 69
            1.115863363709518e+01,   // 70
            -2.172276882150380e+02,  // 71
            4.136788145155134e-03,   // 72
            1.417786201932155e+02,   // 73
            2.125064388462305e+01,   // 74
            -1.522609905777512e+02,  // 75
            -2.470709305218683e+01,  // 76
            1.396222125733745e+02,   // 77
            6.003452857017850e+02,   // 78
            -1.238736899895957e+01,  // 79
            -9.598885190391228e+01,  // 80
            4.382011019815703e+01,   // 81
            1.143868500399729e+02,   // 82
            -2.828378715787923e+02,  // 83
            4.688068626939238e+02,   // 84
            1.716141790506777e+01,   // 85
            -8.667359867782544e+01,  // 86
            -7.718872645052590e+00,  // 87
            -4.369893746294309e+02,  // 88
            1.272243252817301e+01,   // 89
            1.258783405197034e+02,   // 90
            -1.222058290282317e+02,  // 91
            1.892875835888287e+00,   // 92
            4.569859780274798e+01,   // 93
            -8.111206771552931e+01,  // 94
            -1.581057197596032e+01,  // 95
            -2.316890053623043e+00,  // 96
            -2.452104094031454e+01,  // 97
            -2.344686130894000e+02,  // 98
            -5.479782712336771e+01,  // 99
            -4.496187079382000e+01,  // 100
            -6.854477282146304e+01,  // 101
            2.180489180332559e+02,   // 102
            5.204026472573183e+00,   // 103
            -1.825290332391480e+02,  // 104
            -2.028198580977909e+00,  // 105
            1.610713315078542e+02,   // 106
            9.845852399848247e-01,   // 107
            -2.909444416449241e+01,  // 108
            5.908765548057303e+01,   // 109
            1.811734144883828e+00,   // 110
            3.717563096101236e+02,   // 111
            1.207331525049756e+02,   // 112
            -1.042253317024956e+02,  // 113
            1.698244307361877e+02,   // 114
            1.425746242576024e+02,   // 115
            -2.663950051765765e+02,  // 116
            -3.307427527738972e+02,  // 117
            1.945371264783335e+01,   // 118
            -2.995896643812962e+00,  // 119
            -6.493824954214411e+00,  // 120
            5.797228895216087e+01,   // 121
            -1.447835133085472e+02,  // 122
            4.341824666464431e+01,   // 123
            1.319255774583723e+02,   // 124
            3.847272147130030e-01,   // 125
            -6.478584258874285e+01,  // 126
            1.904073526403010e+02,   // 127
            -1.660369754892211e+01,  // 128
            1.512293322133572e+01,   // 129
            3.285642028876900e+02,   // 130
            -3.591888461161786e+01,  // 131
            -5.713395680194904e+01,  // 132
            2.092443225309753e+02,   // 133
            2.885202198351195e+01,   // 134
            4.198432209500077e+02,   // 135
            -1.198975404019249e+02,  // 136
            1.959674338628269e+02,   // 137
            2.952998425578848e+01,   // 138
            5.361754047186889e+01,   // 139
            -1.453575751258196e+01,  // 140
            4.990896928269618e+01,   // 141
            1.701515680955530e+02,   // 142
            -1.521910701825786e+01,  // 143
            8.824245270751608e+01,   // 144
            -4.647637922511060e+01,  // 145
            -6.918389812028076e+01,  // 146
            -9.113840249053823e+01,  // 147
            -2.108109073343193e+02,  // 148
            8.945437884302400e+01,   // 149
            6.163353973588095e+00,   // 150
            -4.797317426232240e+01,  // 151
            -2.356354477977438e+02,  // 152
            4.959041638776308e+01,   // 153
            7.303358410566961e+00,   // 154
            6.810914349613712e+01,   // 155
            -1.255753217308824e+01,  // 156
            2.474555774929246e+00,   // 157
            -6.283688011884907e+02,  // 158
            -8.933918551238349e+01,  // 159
            -3.543535330801210e+01,  // 160
            6.543283336311282e+01,   // 161
            -5.459731730939301e+01,  // 162
            -9.016570753063921e+01,  // 163
            -2.282152397972354e-02,  // 164
            9.583173881906050e-02,   // 165
            3.330544419007538e-02,   // 166
            -1.252458267100783e+00,  // 167
            -1.481379958556501e-01,  // 168
            8.174945150112239e-01,   // 169
            -1.014191571865374e+02,  // 170
            -8.469098142423878e-01,  // 171
            -2.234063325323301e+02,  // 172
            6.926866846106189e-02,   // 173
            -6.201514780869814e+00,  // 174
            8.995325599822680e+00,   // 175
            -2.959372283566146e-03,  // 176
            4.700768379084301e+00,   // 177
            -2.821172054577983e-01,  // 178
            7.815550115193544e+00,   // 179
            3.292899460983346e+00,   // 180
            -1.701122616605706e+00,  // 181
            5.992178835897454e-02,   // 182
            7.746260481981783e+00,   // 183
            3.506978631069581e-02,   // 184
            -1.000363058278459e+01,  // 185
            -1.768840654209593e+01,  // 186
            2.615667146655258e-01,   // 187
            6.640156569414057e+01,   // 188
            -2.136582560071041e-02,  // 189
            1.812481269192507e+00,   // 190
            4.974396288605311e-01,   // 191
            5.386406753445150e-02,   // 192
            1.027616465592173e-01,   // 193
            -9.937102353794740e-03,  // 194
            -7.460118016361815e-01,  // 195
            -5.728244066123271e+00,  // 196
            1.647041299916939e+02,   // 197
            -7.058863648807500e+00,  // 198
            -6.355126819685505e-02,  // 199
            -5.603592816197475e+00,  // 200
            1.709428791044020e-01,   // 201
            1.443370395693890e+00,   // 202
            4.975507462595835e-01,   // 203
            9.113842249518571e-02,   // 204
            2.122090533420475e+01,   // 205
            1.084409131701911e+01,   // 206
            -3.568972214030481e+00,  // 207
            7.213172094650420e+01,   // 208
            2.668586320141885e+00,   // 209
            -2.420457555546345e+00,  // 210
            9.225279115606878e-03,   // 211
            4.216330201230286e+01,   // 212
            -1.373823870115912e-01,  // 213
            5.646156660164672e+01,   // 214
            -3.371810726206370e+01,  // 215
            -1.867659764858097e-01,  // 216
            -3.339421153231716e+00,  // 217
            1.616579758457436e-01,   // 218
            -4.578221493187252e-01,  // 219
            -5.947520694533055e+00,  // 220
            -2.938039586437869e+00,  // 221
            -4.912290129968370e-01,  // 222
            5.604196019759690e+00,   // 223
            1.137798015590660e+01,   // 224
            -6.626426603032985e+01,  // 225
            -1.472205436929930e+00,  // 226
            1.775186413211230e+01,   // 227
            3.970641977779500e-01,   // 228
            -3.257283888987707e+01,  // 229
            -3.753663811144592e+01,  // 230
            4.776305211796330e+01,   // 231
            -9.632104705235968e-03,  // 232
            1.270393005369109e-01,   // 233
            -3.151179416360942e-01,  // 234
            7.565331028974476e+00,   // 235
            -3.664715393185850e+00,  // 236
            9.401903521573490e+01,   // 237
            1.973627041970117e+01,   // 238
            2.590837970341046e+01,   // 239
            -4.663463639895671e-03,  // 240
            9.453942853492020e+00,   // 241
            1.047872732131342e+00,   // 242
            -1.965447870484045e-01,  // 243
            8.353105068064117e+00,   // 244
            -1.674100274835186e+01,  // 245
            -3.157383651933568e+01,  // 246
            9.864751578631879e-02,   // 247
            1.866435687364690e-02,   // 248
            -3.722544821675289e+01,  // 249
            -3.404130952533936e+00,  // 250
            1.100735091560403e+01,   // 251
            4.639931362322630e+00,   // 252
            9.695972980856268e+01,   // 253
            -4.974131631835781e+00,  // 254
            1.913859505627560e+01,   // 255
            -6.671220604517422e+00,  // 256
            -2.280790767419677e-01,  // 257
            -5.106633151382441e+01,  // 258
            -4.656850230271811e+00,  // 259
            5.611739203335551e+00,   // 260
            1.530590579876992e+01,   // 261
            -3.198852098341513e+01,  // 262
            -4.628140650565085e+00,  // 263
            2.349679682662354e+02,   // 264
            -2.890589163028155e+00,  // 265
            3.113446994360044e+01,   // 266
            -8.187911240981313e+00,  // 267
            6.836748474003856e+00,   // 268
            -5.416545389267618e+00,  // 269
            -1.477235589939437e+01,  // 270
            1.846333912304479e+01,   // 271
            6.703823579777429e+00,   // 272
            -2.772993401950263e+00,  // 273
            2.815657604035781e+02,   // 274
            -1.890265018338972e-02,  // 275
            -3.999029271478884e-04,  // 276
            3.783934892815957e+01,   // 277
            2.792797688098129e+02,   // 278
            -1.767068109106511e+01,  // 279
            3.174556872667252e+00,   // 280
            5.277946869861209e+01,   // 281
            -1.048659775396252e+00,  // 282
            1.252404289269555e-01,   // 283
            6.396020327984866e-01,   // 284
            1.602048554222584e-01,   // 285
            -4.065010857592010e-01,  // 286
            -7.890103004438911e-01,  // 287
            1.746765128760582e+02,   // 288
            -1.857814134251792e+00,  // 289
            2.730216442062689e+02,   // 290
            9.351025605579703e+01,   // 291
            1.357921081155165e+01,   // 292
            -4.401628484513663e-01,  // 293
            -8.988914371968034e+00,  // 294
            -1.182175774884961e+02,  // 295
            -5.276722378437736e+01,  // 296
            -2.065426057178092e+00,  // 297
            -1.478599412188704e+00,  // 298
            -1.514345865413468e-01,  // 299
            -1.105145918925815e-01,  // 300
            1.101174162067620e+01,   // 301
            1.571841622076332e+00,   // 302
            1.081159942550252e+01,   // 303
            5.328473356189160e-02,   // 304
            4.732209966503159e-01,   // 305
            4.862629576926240e+01,   // 306
            5.889071306918275e-02,   // 307
            -1.332323484174059e+02,  // 308
            -2.508476226493578e-03,  // 309
            8.220656076596602e+01,   // 310
            1.649735138275996e-02,   // 311
            -2.079629298223267e-01,  // 312
            1.632133322380398e+02,   // 313
            1.498593874747163e+02,   // 314
            -2.681264065958223e-01,  // 315
            6.957185516443589e+00,   // 316
            5.569222473170343e+01,   // 317
            2.564006087227187e+00,   // 318
            -1.059276252753136e+01,  // 319
            -2.383812647729551e-01,  // 320
            -1.443411106464983e-01,  // 321
            -3.332259568628992e+02,  // 322
            6.695129124450276e+01,   // 323
            8.459021199870520e+00,   // 324
            -9.544601609381385e+00,  // 325
            8.294669913564094e+00,   // 326
            -2.411417028163076e-01,  // 327
            -1.481207650785931e+02,  // 328
            -2.719618255170074e+01,  // 329
            3.786936735402713e+01,   // 330
            -2.181965511607230e+01,  // 331
            -2.286022570858323e+01,  // 332
            -2.240512458500369e+01,  // 333
            -2.635161115222022e+01,  // 334
            -1.346193161249767e+02,  // 335
            -3.026752295598825e+00,  // 336
            1.472224720047238e+01,   // 337
            -5.716586840484019e-01,  // 338
            8.525101324884223e+01,   // 339
            -1.288404986383943e-02,  // 340
            2.612261455497211e+02,   // 341
            -2.941621779401893e+00,  // 342
            -5.799304467047529e-02,  // 343
            8.321006527080423e+00,   // 344
            -3.401615691876474e+01,  // 345
            2.877763525272834e+00,   // 346
            8.991509924983490e-02,   // 347
            3.751346281537010e+00,   // 348
            -7.648385588896321e-01,  // 349
            -3.276941778420833e+02,  // 350
            5.293880410793173e+02,   // 351
            6.182359075245999e+01,   // 352
            -3.992785154029511e+02,  // 353
            1.536108581458035e-02,   // 354
            2.510160083305639e+01,   // 355
            3.844300680792034e-01,   // 356
            1.702392546769529e+00,   // 357
            -2.374092378834144e-02,  // 358
            8.339125055082943e+01,   // 359
            5.616880166460431e+00,   // 360
            4.927184110460992e-02,   // 361
            -3.672811917825560e+01,  // 362
            -3.125148141494633e+01,  // 363
            1.319309202820123e-01,   // 364
            -2.385042172626929e+01,  // 365
            -6.677811072341756e-02,  // 366
            -1.156453608256604e+01,  // 367
            1.693025802855755e+01,   // 368
            5.674163448349637e+00,   // 369
            -2.329075083355664e+00,  // 370
            -1.086510815443294e+01,  // 371
            -1.132692446666487e+01,  // 372
            -4.377250780808584e+01,  // 373
            -1.991734004766107e+00,  // 374
            3.115720266118042e-02,   // 375
            -1.470467844296872e+01,  // 376
            6.577468458413414e+00,   // 377
            4.690075249352542e+00,   // 378
            7.839812952386001e-02,   // 379
            -1.034751282210174e-01,  // 380
            1.683156407644666e+01,   // 381
            -2.343229349792333e+00,  // 382
            -4.149146876784912e-01,  // 383
            9.687163179577643e-02,   // 384
            2.356644766144153e+01,   // 385
            -2.649470585391161e+00,  // 386
            7.666018652392147e+00,   // 387
            -5.613738258833013e+00,  // 388
            4.826500007554008e-01,   // 389
            5.076581894596880e-02,   // 390
            -2.961641996432233e-01,  // 391
            2.035552537442694e+01,   // 392
            -1.010312651085401e+00,  // 393
            6.287497189230645e-02,   // 394
            4.958748222219981e-02,   // 395
            -2.539379302606493e+00,  // 396
            -6.840607008645133e-01,  // 397
            -8.740044213007693e-02,  // 398
            1.515912035740408e+00,   // 399
            -5.151376318118955e+00,  // 400
            4.578555004182837e-02,   // 401
            -4.343847588266330e+00,  // 402
            -5.586976458104289e-03,  // 403
            7.320032931802893e-01,   // 404
            -6.077334926502157e+01,  // 405
            -1.601379214618354e+00,  // 406
            3.527299734495724e+01,   // 407
            -6.562363232607122e+00,  // 408
            -1.382162772735291e-01,  // 409
            -1.018874678236217e+00,  // 410
            1.750716672976920e+00,   // 411
            1.490250847118170e+01,   // 412
            -2.573167703244919e-01,  // 413
            -6.558474175240955e-02,  // 414
            -5.515111940327782e+00,  // 415
            3.383016111518476e-01,   // 416
            -8.576525815499725e-01,  // 417
            4.957909274361132e+00,   // 418
            -5.570877488297656e-01,  // 419
            -2.648232744434915e-01,  // 420
            2.115985898879007e+01,   // 421
            -8.927896155480579e+01,  // 422
            -4.253188341498750e-01,  // 423
            -3.801270891074970e+01,  // 424
            9.242723437819493e-02,   // 425
            -5.206979286583734e+00,  // 426
            -3.110467995783642e-01,  // 427
            -5.025982199472248e+00,  // 428
            3.984561605858207e-01,   // 429
            3.962592626847049e+02,   // 430
            2.448944511923235e+02,   // 431
            7.581102646857508e+01,   // 432
            -1.172833089879092e+01,  // 433
            4.298687788647760e+00,   // 434
            3.715176628230625e-01,   // 435
            -2.277187610658741e+02,  // 436
            7.703898456401478e-02,   // 437
            -1.055432529835715e+01,  // 438
            1.156720275605748e-02,   // 439
            5.482968732848468e+00,   // 440
            5.640236492500083e+00,   // 441
            -2.364114690682602e+00,  // 442
            -1.284227269292232e+00,  // 443
            4.167961562043270e-01,   // 444
            -5.036407823850903e-02,  // 445
            -3.787160791397896e+01,  // 446
            4.123794500925325e+01,   // 447
            -3.542776878973996e+00,  // 448
            2.570682884461881e+01,   // 449
            7.154203232724059e-01,   // 450
            8.724963537103895e+01,   // 451
            5.357409097459060e-02,   // 452
            4.249956782233524e+02,   // 453
            5.801615557841498e+01,   // 454
            -9.690374211549456e+00,  // 455
            4.087661534889219e-03,   // 456
            -2.832562365632769e+01,  // 457
            -4.131329656016575e+00,  // 458
            -7.216514788417093e-02,  // 459
            1.330976532524707e+01,   // 460
            -5.911137402494698e+00,  // 461
            -1.077306799931365e-01,  // 462
            -8.521504195256750e+01,  // 463
            1.689437473886757e+00,   // 464
            -4.352456823856328e-01,  // 465
            -2.397902792327403e-01,  // 466
            -2.481006010531495e+00,  // 467
            -3.099474394849929e+00,  // 468
            -9.615021271235045e-01,  // 469
            -8.966740696319643e+00,  // 470
            7.780886077290619e-01,   // 471
            -6.018702437795894e+00,  // 472
            3.452998893088180e-01,   // 473
            -1.410452458290417e+01,  // 474
            7.842717618597122e+01,   // 475
            -8.249948238129205e+01,  // 476
            9.510318713363991e-01,   // 477
            -1.609663117152165e+01,  // 478
            -9.640143248582437e-01,  // 479
            -4.516406794707196e-03,  // 480
            1.914247808210847e-01,   // 481
            9.585420287999473e-01,   // 482
            -1.698036151168079e-03,  // 483
            6.632655768176703e-03,   // 484
            -7.672028235676311e+01,  // 485
            4.925957891962472e-01,   // 486
            -2.587432184804501e+01,  // 487
            4.500654628332271e+01,   // 488
            -3.514403671747735e+02,  // 489
            -3.775175271190248e+01,  // 490
            3.945969975408025e+00,   // 491
            -1.614455257909422e+02,  // 492
            -3.058659304587554e+00,  // 493
            1.593681404817783e+01,   // 494
            1.904618494805323e+01,   // 495
            1.065214531954510e-01,   // 496
            -1.356242758237477e+01,  // 497
            9.107136741446972e-02,   // 498
            -7.348436985411919e-03,  // 499
            1.283899528485140e+01,   // 500
            -2.879736321831307e+01,  // 501
            1.844453592479331e+01,   // 502
            3.105095729146107e+00,   // 503
            2.803914926982933e-03,   // 504
            2.741904038245512e+01,   // 505
            3.762219356347399e+00,   // 506
            2.475426278132359e-01,   // 507
            6.341730732447524e-01,   // 508
            2.264258073302477e-01,   // 509
            -7.823773909413345e+01,  // 510
            -3.809584654061858e+00,  // 511
            -5.299211967641008e+01,  // 512
            -2.264025714618887e+01,  // 513
            -1.233828932856546e-01,  // 514
            -2.349564555071360e-01,  // 515
            -9.066101398520134e+00,  // 516
            2.603979046879194e-01,   // 517
            -4.449759345823171e-02,  // 518
            6.090587147012312e+01,   // 519
            4.650899794215683e+00,   // 520
            6.569270737112901e-01,   // 521
            -1.464842486581749e+01,  // 522
            -1.689493885419092e-01,  // 523
            1.486158780537414e+00,   // 524
            6.865380407823571e+01,   // 525
            -2.183968504484116e-02,  // 526
            -3.270802338385472e+01,  // 527
            2.136092410752835e+01,   // 528
            -2.150270682907748e+01,  // 529
            -1.426185333093545e+00,  // 530
            2.833699431465233e+02,   // 531
            2.660527212877863e-01,   // 532
            -3.446676769045771e-02,  // 533
            5.902207448997867e-01,   // 534
            -1.983133456103780e-02,  // 535
            6.777442834190618e-01,   // 536
            3.502752491507239e-02,   // 537
            -1.241904059992250e+01,  // 538
            -9.287627901697688e+01,  // 539
            -4.291487533769400e-02,  // 540
            -1.257441898194166e+01,  // 541
            3.165335832174426e-01,   // 542
            1.458645053873751e+02,   // 543
            5.999073875159584e-02,   // 544
            -1.335703767884406e-01,  // 545
            9.326568891232169e-02,   // 546
            -5.517467674053326e+01,  // 547
            8.645036109550910e+00,   // 548
            2.137905098553023e-02,   // 549
            4.188016838127741e+02,   // 550
            -1.165159009168699e-01,  // 551
            1.191317075382750e+02,   // 552
            1.296045352097348e+01,   // 553
            3.118744084822666e+01,   // 554
            -3.159704371349984e-01,  // 555
            9.759827139708274e-01,   // 556
            3.176995735030362e+00,   // 557
            2.822174656157988e-02,   // 558
            2.743006438834193e-01,   // 559
            1.516953882960639e-01,   // 560
            -4.463566975105971e+02,  // 561
            -1.524680214573227e+02,  // 562
            3.896062779389635e+00,   // 563
            2.087841743544733e-02,   // 564
            -1.896979769662878e+02,  // 565
            4.845680878813979e-02,   // 566
            4.777174165293824e+01,   // 567
            -9.526149190641492e-01,  // 568
            -2.612850036409342e+01,  // 569
            7.496354963035679e+00,   // 570
            -1.791758566981044e+00,  // 571
            -3.607139235791679e+01,  // 572
            4.102909908741732e-01,   // 573
            1.311016459729686e+00,   // 574
            -1.683831582696941e+00,  // 575
            -1.138040626929021e+01,  // 576
            -5.195451722537419e+01,  // 577
            -3.477306036104760e+02,  // 578
            -5.544079651731700e-02,  // 579
            -5.743222243766308e+00,  // 580
            -7.535599656853551e+02,  // 581
            2.060949157811606e+01,   // 582
            8.113991847213965e+00,   // 583
            1.382024056426329e+02,   // 584
            -5.877999403517061e+00,  // 585
            2.774862437239069e+01,   // 586
            5.327154973941836e+00,   // 587
            -6.978044099020903e+00,  // 588
            3.885417618227620e-02,   // 589
            1.402453851151028e+02,   // 590
            -9.163240849048775e+01,  // 591
            -3.191077720352815e-02,  // 592
            4.090929006484795e-02,   // 593
            1.043811920859455e+02,   // 594
            -6.987747989724505e-02,  // 595
            9.185696437995725e-01,   // 596
            1.266387374826209e-01,   // 597
            -1.692359391754455e-01,  // 598
            -1.878176049873428e-01,  // 599
            -1.026355143114750e+02,  // 600
            -1.116996598975338e+01,  // 601
            1.297391796052884e-01,   // 602
            1.148948166135061e-01,   // 603
            1.802938080657295e+01,   // 604
            1.987450819845102e+00,   // 605
            1.376975054574075e+01,   // 606
            1.336027003675786e+01,   // 607
            6.493984049270164e-02,   // 608
            8.381454753682537e+01,   // 609
            2.555577131899303e+00,   // 610
            7.834844743069063e+01,   // 611
            -3.465812132945364e+02,  // 612
            -1.045634841171781e+01,  // 613
            1.166321741013400e+00,   // 614
            -4.327834389515459e-01,  // 615
            8.336234784388370e-02,   // 616
            -2.622911043837406e+02,  // 617
            2.018644623205036e+01,   // 618
            -1.041549033654949e+00,  // 619
            4.866640428079847e+01,   // 620
            4.155277123311222e+01,   // 621
            -6.738672336281682e+00,  // 622
            -6.898168302812134e+01,  // 623
            2.468325426003204e+02,   // 624
            4.591466035249018e-03,   // 625
            1.212820857113946e+01,   // 626
            -7.120232352621005e+00,  // 627
            2.898574341970795e-01,   // 628
            4.648978287895578e+00,   // 629
            -9.733896812623218e-02,  // 630
            -2.099543282512444e+00,  // 631
            1.034518404515245e-01,   // 632
            4.451835362970556e-01,   // 633
            1.375868604554257e+02,   // 634
            4.650018419346437e+00,   // 635
            -2.073146527995776e+01,  // 636
            5.169630567439653e+01,   // 637
            -3.125760093786080e+01,  // 638
            6.747345927598040e+01,   // 639
            -1.815564345180284e+02,  // 640
            2.744772040953809e-01,   // 641
            -5.759526903846415e-04,  // 642
            -4.444360906452953e+00,  // 643
            -1.364847437021442e+02,  // 644
            6.150172236162050e-02,   // 645
            2.851454923938104e-01,   // 646
            3.323346552647737e+01,   // 647
            2.429191795366816e+00,   // 648
            -2.722628060751065e+00,  // 649
            -2.365854463835601e-01,  // 650
            3.850813161858526e-01,   // 651
            -2.844277494692236e+00,  // 652
            9.704975366856162e+00,   // 653
            8.264625226777359e-01,   // 654
            7.983206291381430e+00,   // 655
            3.853739734788913e+01,   // 656
            -6.761527924590045e+00,  // 657
            2.953530196086096e+00,   // 658
            9.765551522097647e+01,   // 659
            -1.248759881322319e+01,  // 660
            -1.801912952406703e+00,  // 661
            -4.646103608487452e-01,  // 662
            -4.938598723575494e+02,  // 663
            -1.394980442050567e+01,  // 664
            -4.907946172666904e+01,  // 665
            3.438172106088052e-01,   // 666
            -4.303118747637381e+00,  // 667
            -1.339204067148234e+02,  // 668
            -4.088626267307332e-02,  // 669
            -1.256623828629015e-01,  // 670
            -5.059521307007692e+01,  // 671
            -6.386913036281126e+00,  // 672
            -1.469238387935656e-03,  // 673
            -5.517614484548410e-02,  // 674
            1.670898061692657e-01,   // 675
            -9.288791939619374e+01,  // 676
            -5.531952766671342e-01,  // 677
            3.635656324878610e-03,   // 678
            -4.547114474920811e+01,  // 679
            -9.454529252396001e+01,  // 680
            -9.742630857278907e-02,  // 681
            -4.754943983443559e+01,  // 682
            -9.821522774415001e+00,  // 683
            -2.310231007580859e+00,  // 684
            -2.417051262653524e+02,  // 685
            8.970826205545354e+00,   // 686
            1.931127062667924e-03,   // 687
            -3.113400480471927e-01,  // 688
            1.162609364364484e-01,   // 689
            3.307084336554680e+00,   // 690
            1.823211836618450e+02,   // 691
            -4.871122796642104e-03,  // 692
            -1.295030001137155e-03,  // 693
            -2.439778098268443e+00,  // 694
            2.339080769925651e+00,   // 695
            -8.954859797929803e-01,  // 696
            3.331057856589618e-01,   // 697
            6.579896536997852e-03,   // 698
            4.103144598570309e+01,   // 699
            1.741199482705301e+02,   // 700
            7.546495175945216e+01,   // 701
            -2.257293515174889e+01,  // 702
            6.603946689661060e-02,   // 703
            5.512108024789276e+01,   // 704
            -1.324940561242675e-01,  // 705
            3.704541043440487e-03,   // 706
            3.324382918589718e+01,   // 707
            7.870725615851306e-03,   // 708
            6.933588400522997e-03,   // 709
            5.381056493956969e-03,   // 710
            2.625589215223881e+02,   // 711
            -4.191118269624194e-02,  // 712
            1.091966774154468e+02,   // 713
            -2.421819730348664e-01,  // 714
            1.599057620684542e-02,   // 715
            -1.616420041389228e+01,  // 716
            2.920804402108254e-01,   // 717
            3.253589959225157e+01,   // 718
            8.081748010328302e+01,   // 719
            -8.267211641963613e+01,  // 720
            2.969441005426730e-01,   // 721
            6.684514505948930e+01,   // 722
            6.277413282404051e-01,   // 723
            -2.656084211197803e-01,  // 724
            -5.490615275302135e+01,  // 725
            1.036324781115542e+01,   // 726
            -5.188316064828668e+00,  // 727
            -1.544320222174997e+01,  // 728
            3.272678695720810e+02,   // 729
            -8.046510539123494e+00,  // 730
            6.126304827438429e-01,   // 731
            2.365761283212136e+01,   // 732
            -2.655623440094040e+00,  // 733
            8.867236042737233e+01,   // 734
            2.086864816710948e+02,   // 735
            -6.510994296736856e+00,  // 736
            -5.929438875172063e-01,  // 737
            -2.252634418816344e+00,  // 738
            1.844892397971096e-02,   // 739
            6.486316513023483e+01,   // 740
            -3.412385176570575e+00,  // 741
            6.298511320204501e-01,   // 742
            1.196100224968305e-01,   // 743
            4.656641637229694e+00,   // 744
            -7.612218862956954e+01,  // 745
            3.031780819376010e+01,   // 746
            -1.681148692386709e+01,  // 747
            2.488425718569197e+01,   // 748
            7.890377697993006e+00,   // 749
            7.819419701583489e-01,   // 750
            6.489126222256446e-02,   // 751
            -2.578323438732792e+01,  // 752
            5.147786856810458e+02,   // 753
            -1.256400071883564e+02,  // 754
            6.493406011379619e+01,   // 755
            1.153025224402053e-02,   // 756
            3.802574703265933e+00,   // 757
            8.173112314785475e-01,   // 758
            -2.293694661498435e+00,  // 759
            -8.743263795332011e+01,  // 760
            -8.590974092823449e-01,  // 761
            -4.584926423604490e-01,  // 762
            -9.668445884619947e+00,  // 763
            5.775896551401287e+00,   // 764
            -1.514547437794095e-02,  // 765
            -5.391013177445548e-01,  // 766
            1.212324331689035e-01,   // 767
            -4.908777405431644e-01,  // 768
            -1.498402681454635e+01,  // 769
            -1.737670246135241e+02,  // 770
            -1.810726426987246e+01,  // 771
            -8.439012185036675e+01,  // 772
            1.160344023969639e+00,   // 773
            2.015936951606085e-01,   // 774
            2.340398730429046e+02,   // 775
            -5.765597157890059e-02,  // 776
            1.255288080936669e+02,   // 777
            7.748431200983562e+00,   // 778
            1.011508263305543e+02,   // 779
            -9.399363392209455e-02,  // 780
            1.672885374806309e+00,   // 781
            9.352296551617691e+00,   // 782
            -4.117857027828491e-02,  // 783
            -2.156163853646210e-02,  // 784
            1.405402109695899e+01,   // 785
            -3.118813605496797e+00,  // 786
            4.760201238498617e+01,   // 787
            -7.024161226279539e-02,  // 788
            2.942830312646184e+00,   // 789
            2.119553317823854e+01,   // 790
            -4.016428378499729e-03,  // 791
            -4.631653345306610e+01,  // 792
            -2.136285290757842e-02,  // 793
            5.010789339192454e+00,   // 794
            3.853947665980922e+00,   // 795
            -1.107195068408183e+01,  // 796
            -4.819289752132169e-02,  // 797
            -1.448095709055469e-03,  // 798
            1.344100365164688e+02,   // 799
            -1.900358360610561e+02,  // 800
            1.304069181830818e+01,   // 801
            1.198555134928449e+00,   // 802
            1.124975664157684e+01,   // 803
            3.747436091167336e+00,   // 804
            1.025027194517045e+01,   // 805
            -9.075886765663921e-02,  // 806
            -3.415397953490968e-02,  // 807
            -4.489468058662142e-02,  // 808
            4.421588905942485e+00,   // 809
            3.725750664463474e-02,   // 810
            6.387694993340565e+01,   // 811
            -2.789849903912247e-01,  // 812
            -8.279413128696882e-03,  // 813
            5.335107021512980e+00,   // 814
            -1.111805302073736e+02,  // 815
            -3.482590087252584e-02,  // 816
            -1.310683228797951e-01,  // 817
            -6.863186060740607e-02,  // 818
            2.970241465708818e-02,   // 819
            3.945179260523523e-01,   // 820
            5.848565744792171e-03,   // 821
            -1.437189222336494e+02,  // 822
            -3.935738023471662e+00,  // 823
            8.286370099259242e+00,   // 824
            -1.346300463290646e+01,  // 825
            -4.934234905239689e+00,  // 826
            -3.151021587408236e+01,  // 827
            -1.505553522776301e+01,  // 828
            1.023515601391669e+01,   // 829
            -9.936140785323440e+01,  // 830
            -1.364232693758817e-02,  // 831
            6.182767038206037e+00,   // 832
            -9.997550723868581e-01,  // 833
            -4.032568992183031e+01,  // 834
            -1.966861372233585e+00,  // 835
            -6.281456620768707e+01,  // 836
            -9.690684062294666e-03,  // 837
            -9.023002572684124e+00,  // 838
            1.329553259377719e+01,   // 839
            -3.995663923501200e+00,  // 840
            6.545951407369976e-01,   // 841
            2.031198382600502e-02,   // 842
            1.475184705913451e+01,   // 843
            -1.221102194164574e+02,  // 844
            4.103799219583890e+00,   // 845
            -2.252147380042979e+01,  // 846
            7.346133904678115e+00,   // 847
            -4.282671863401241e-01,  // 848
            6.247255403898998e+02,   // 849
            -2.421252422772039e-02,  // 850
            -5.507077739526677e+02,  // 851
            -4.648037839110036e+00,  // 852
            4.196093342257799e+01,   // 853
            -2.771592746992668e+01,  // 854
            1.092627415221911e+00,   // 855
            2.439157027108685e+01,   // 856
            -3.713199012036043e+02,  // 857
            4.686837084404202e+00,   // 858
            -2.620659194466954e+01,  // 859
            3.378501288973229e+01,   // 860
            7.488915965892752e+00,   // 861
            -1.683276857425267e+02,  // 862
            4.166392792083427e+02,   // 863
            8.282837282328130e-01,   // 864
            -3.109694031900648e+00,  // 865
            -1.798366182372667e+01,  // 866
            -7.611651761462153e+01,  // 867
            1.149769627255015e+00,   // 868
            8.410194882650682e+00,   // 869
            -8.181504951256896e+00,  // 870
            6.204716360920089e+00,   // 871
            1.292103024221499e+02,   // 872
            4.203621735190917e+00,   // 873
            5.621310252547561e-01,   // 874
            1.846377622542992e+02,   // 875
            -8.523941569901567e+01,  // 876
            -6.273522918435956e+00,  // 877
            -7.116614974777432e+00,  // 878
            -2.559804623578509e+00,  // 879
            -6.287734492810045e+00,  // 880
            2.703713437087456e+00,   // 881
            5.814329933285276e+01,   // 882
            4.624466295940183e-01,   // 883
            -2.128637601913698e-01,  // 884
            -2.688289076292930e+01,  // 885
            -5.533849959340465e+02,  // 886
            1.575006585787880e+02,   // 887
            1.821245268201873e+02,   // 888
            -1.944163143891149e-03,  // 889
            -1.031040305449351e-03,  // 890
            8.916396542908744e-01,   // 891
            2.334905063996718e+02,   // 892
            -2.192165850432025e+01,  // 893
            1.948034959732008e+02,   // 894
            7.512474041555069e-01,   // 895
            -9.571552535179490e-03,  // 896
            2.685499280223101e+02,   // 897
            4.770172172233674e+01,   // 898
            6.700803255257105e-02,   // 899
            -3.689046727923533e-01,  // 900
            -2.500179875466847e+02,  // 901
            -4.116206612373157e-03,  // 902
            -9.174952552945323e+00,  // 903
            1.004016871015804e+02,   // 904
            3.344930100674325e+00,   // 905
            -5.352910993918998e+01,  // 906
            -4.441622336728411e+02,  // 907
            1.467195638036084e+01,   // 908
            -1.783067118615649e+01,  // 909
            5.105167892642306e+00,   // 910
            4.971522964521591e+00,   // 911
            -9.548218442416487e-01,  // 912
            -1.715388476348505e+00,  // 913
            1.817603078449412e+02,   // 914
            3.871318293814502e+00,   // 915
            5.629046584925494e+01,   // 916
            -1.060059803582381e+01,  // 917
            9.181703912171544e+00,   // 918
            -4.469189243457157e-01,  // 919
            -2.173289979976049e-02,  // 920
            4.509435496553203e-03,   // 921
            -2.607481249146257e+01,  // 922
            -1.535661671950439e+02,  // 923
            9.864308967710755e+01,   // 924
            1.698672872915198e-01,   // 925
            -5.935138804892423e-01,  // 926
            -2.101453177011232e+00,  // 927
            -1.371291099773159e+02,  // 928
            -1.350275587691827e-02,  // 929
            6.550400394506064e+00,   // 930
            6.243160275023039e-01,   // 931
            1.931194124082088e-01,   // 932
            -3.487593384249980e-02,  // 933
            9.451821874381274e+01,   // 934
            -4.206636243284274e+02,  // 935
            -5.340599847536852e+00,  // 936
            -1.807859097350127e-01,  // 937
            -1.235162410808871e+01,  // 938
            3.636081852607164e-01,   // 939
            2.642691265584321e+00,   // 940
            -8.039825248546242e+01,  // 941
            -1.067975624991088e+01,  // 942
            -8.103510686327881e-04,  // 943
            -4.710876042110424e+00,  // 944
            -1.188524056860231e+01,  // 945
            -1.248522483693638e+01,  // 946
            -1.248714081349846e+01,  // 947
            -1.179156333321580e+01,  // 948
            1.923869318700290e-02,   // 949
            2.646460377757365e+00,   // 950
            3.691454037319284e+01,   // 951
            2.273562280247686e+01,   // 952
            3.409054760551766e+00,   // 953
            -2.138510029212441e+00,  // 954
            -4.365657600199980e+01,  // 955
            -5.577818513869231e+01,  // 956
            7.716801992080872e+01,   // 957
            3.114876600908042e+02,   // 958
            -1.914090349978842e-01,  // 959
            1.228960303819884e+02,   // 960
            -4.319175491648465e+01,  // 961
            5.989980211799182e+02,   // 962
            3.757393871453921e+02,   // 963
            -2.880649484796317e-01,  // 964
            6.727528069459882e+01,   // 965
            1.135930490834926e+02,   // 966
            1.644488197676377e+01,   // 967
            -3.409110478765073e-01,  // 968
            1.538477608210416e-02,   // 969
            2.845844244634847e+01,   // 970
            -9.046305069047783e-02,  // 971
            -1.953101040070955e+00,  // 972
            -2.742271972429160e+00,  // 973
            -1.039626622151037e+02,  // 974
            1.049271630462425e+02,   // 975
            -2.565011905492849e+01,  // 976
            1.208221806133212e-02,   // 977
            1.048885575064831e+03,   // 978
            -3.291578928211052e-01,  // 979
            1.125599703589185e+02,   // 980
            -1.038138809722565e+02,  // 981
            8.386110019440639e+01,   // 982
            1.776321257985173e+02,   // 983
            -4.116337192090493e+00,  // 984
            1.228884663645284e+00,   // 985
            1.640024435172677e+00,   // 986
            -2.117547547178694e+01,  // 987
            -1.529217020712121e+02,  // 988
            1.050505545409819e+01,   // 989
            -1.478885205153518e+02,  // 990
            -7.132367526178909e-02,  // 991
            -3.952697218953390e+02,  // 992
            -2.672065419021589e+01,  // 993
            2.809347873386032e-02,   // 994
            -8.177606210613256e+01,  // 995
            -1.077489698502534e+00,  // 996
            2.704848573447535e+01,   // 997
            -4.993017268984104e+01,  // 998
            3.370331596905021e+01,   // 999
            5.829816112743063e-01,   // 1000
            9.606998572559633e+00,   // 1001
            -4.952299095556430e+02,  // 1002
            2.513173061561610e-02,   // 1003
            -5.624119316408098e+01,  // 1004
            8.610890695736570e-02,   // 1005
            -1.236778997632362e-01,  // 1006
            -5.333592401979697e+02,  // 1007
            -5.529395299359083e+01,  // 1008
            1.801220954259834e+02,   // 1009
            -2.433647008900491e+02,  // 1010
            1.617867585296672e+02,   // 1011
            1.209759747666432e+00,   // 1012
            7.222455736755377e+01,   // 1013
            -8.577285273298976e+00,  // 1014
            2.806043807898766e+02    // 1015
        };                           // 1015

        m_r3i = 0.000000000000000e+00;         // A
        m_r3f = 4.500000000000000e+00;         // A
        m_kHH_intra = -2.009859067711048e-01;  // A^(-1)
        m_kOH_intra = 1.079285891981862e+00;   // A^(-1)
        m_kHH = 4.884318738475223e-01;         // A^(-1)
        m_kOH = 6.109404006641794e-01;         // A^(-1)
        m_kOO = 6.521263121174117e-01;         // A^(-1)
        m_kXO = 1.252130248577817e-01;         // A^(-1)
        m_kXH = 4.772422237444002e-02;         // A^(-1)
        m_dHH_intra = 4.331575675326020e+00;   // A
        m_dOH_intra = 2.493557848672918e+00;   // A
        m_dHH = 1.003637515624313e+01;         // A
        m_dOH = 7.677682294461052e+00;         // A
        m_dOO = 4.789256072989140e+00;         // A
        m_dXO = 9.920052988243551e+00;         // A
        m_dXH = 6.916712361001890e+00;         // A

        std::copy(the_poly, the_poly + ncoeffs, m_coeffs);

    } else if (m3 == "k+") {
        const double the_poly[] = {
            -1.183549680608313e+01,  // 0
            2.785191087592593e+02,   // 1
            -1.446287980884764e+02,  // 2
            -1.168034218137005e+02,  // 3
            7.828349444722679e+02,   // 4
            -4.106554543042988e+02,  // 5
            7.509004599845643e+00,   // 6
            3.662295857533952e+02,   // 7
            -2.422689238299388e+02,  // 8
            -3.626476988795075e+01,  // 9
            2.717713591917796e+02,   // 10
            1.074908287271521e+02,   // 11
            -1.154914422251097e+03,  // 12
            -3.912640411308707e+01,  // 13
            1.964122507141232e+00,   // 14
            -6.588815266980959e+01,  // 15
            4.067381096085466e+00,   // 16
            2.066515963555360e+02,   // 17
            1.016782603947138e+02,   // 18
            3.126733321373151e+00,   // 19
            -5.680585170072289e+00,  // 20
            -6.771556328434706e-06,  // 21
            9.511576849328797e+00,   // 22
            -1.821856678154478e+02,  // 23
            -6.172187972932904e+02,  // 24
            -1.081641195759021e+01,  // 25
            -1.519870510613360e+01,  // 26
            -6.767436882794026e-04,  // 27
            2.456430731310396e-02,   // 28
            -4.627768393312803e+01,  // 29
            8.299419200806832e+01,   // 30
            -2.414681287780703e+02,  // 31
            1.114430203245947e+02,   // 32
            -1.995576803895893e+00,  // 33
            -1.311262279425138e+02,  // 34
            8.743957878452610e+00,   // 35
            -2.376280332232938e+00,  // 36
            1.274048723316423e+01,   // 37
            1.332375582958716e+01,   // 38
            -9.103605415602212e+02,  // 39
            5.115579297950227e-06,   // 40
            -1.896036584236395e+02,  // 41
            8.229399771689913e+01,   // 42
            -1.145770515391877e-01,  // 43
            2.479859009116055e-04,   // 44
            9.988698536718007e+01,   // 45
            -5.598312684400470e+00,  // 46
            -2.829708801730861e+00,  // 47
            -8.766341396484096e+01,  // 48
            -1.992228928841461e-04,  // 49
            2.409409251336333e-02,   // 50
            -1.187155430582583e+02,  // 51
            -2.359270280865605e-04,  // 52
            1.347279398932880e+02,   // 53
            -1.552202881114269e+01,  // 54
            -2.138879162903949e+01,  // 55
            7.432057090427503e+02,   // 56
            -3.914084058408996e+02,  // 57
            6.215334537652361e+02,   // 58
            3.144861917825422e+02,   // 59
            3.150502963213054e-05,   // 60
            2.092499863538714e+01,   // 61
            1.934520221000670e+00,   // 62
            -6.917648191307348e-01,  // 63
            -3.220632869850284e+01,  // 64
            1.967822919211160e+02,   // 65
            -1.133919282110889e+02,  // 66
            8.344493445982587e+01,   // 67
            -3.897950138313366e+02,  // 68
            -2.025671256239860e+01,  // 69
            -3.835281836233883e-04,  // 70
            -1.988426944419143e+02,  // 71
            -2.669997078296922e-01,  // 72
            2.060100204217901e+02,   // 73
            2.479681656781636e+00,   // 74
            -1.854749651063388e+02,  // 75
            -6.052160003110448e+01,  // 76
            -5.029889986423861e+01,  // 77
            4.143319087801494e+02,   // 78
            1.103557215669695e-03,   // 79
            -4.331597290484170e+02,  // 80
            2.257484519663170e+02,   // 81
            -1.467933305353322e+02,  // 82
            1.953111959092028e+02,   // 83
            6.699617022173568e-03,   // 84
            -1.210012756440164e+01,  // 85
            -2.490579832598676e+02,  // 86
            -5.180024746556879e+02,  // 87
            2.842047793263917e+02,   // 88
            3.210337589280918e+00,   // 89
            4.554871139708881e+02,   // 90
            -2.447504354919172e+01,  // 91
            1.011962096954906e+00,   // 92
            -2.672622233445241e+02,  // 93
            2.046419576213774e-01,   // 94
            -5.699994774058906e+00,  // 95
            2.886377299302492e+00,   // 96
            -1.066496555252902e+01,  // 97
            -9.324137866528221e+01,  // 98
            1.769350905763059e+01,   // 99
            3.058703320636328e+01,   // 100
            -1.567243190381967e+02,  // 101
            -5.761603166086512e-05,  // 102
            6.425450963799753e-01,   // 103
            -1.378050961271001e+01,  // 104
            -8.141724225838045e-01,  // 105
            1.951196253767082e+02,   // 106
            1.385760296216416e+00,   // 107
            7.388346121758622e+00,   // 108
            -6.015883653179574e-04,  // 109
            5.575793165623739e-01,   // 110
            -1.270412149168711e+02,  // 111
            5.989032555239633e+01,   // 112
            -1.312133477788879e+02,  // 113
            -8.128105102982694e-03,  // 114
            3.958669703537074e+02,   // 115
            1.260833585309353e+01,   // 116
            -5.003968740618563e-04,  // 117
            3.177306631210424e-03,   // 118
            4.750090014283638e-01,   // 119
            3.981469355622153e+00,   // 120
            4.986420190907323e+02,   // 121
            7.159811715986606e-04,   // 122
            5.544949919719326e+00,   // 123
            1.603829439438528e+01,   // 124
            1.654924346461346e-01,   // 125
            -9.502315759065547e+01,  // 126
            1.575529377496522e+01,   // 127
            2.833149402974465e+00,   // 128
            -2.867026598398589e+01,  // 129
            7.264868350347483e+01,   // 130
            3.288513716589869e+01,   // 131
            5.943657925000945e+01,   // 132
            4.216482862511379e-04,   // 133
            -3.320337826233680e+00,  // 134
            -8.081134892947395e+01,  // 135
            1.147217477259195e+02,   // 136
            -1.632567130368501e-04,  // 137
            3.420649957862511e+01,   // 138
            2.756068665553998e+02,   // 139
            -1.847072754684436e+00,  // 140
            -4.677466243463434e+01,  // 141
            1.943439390649148e+02,   // 142
            4.128584075476258e-04,   // 143
            -5.897211735791219e+00,  // 144
            -8.381923710457477e+01,  // 145
            -3.345669173169316e-03,  // 146
            -8.053638145958240e+02,  // 147
            2.858488077098793e-03,   // 148
            1.496671124213765e-03,   // 149
            -2.607080695902384e+01,  // 150
            -1.121169147747129e+01,  // 151
            -3.867321922299369e+01,  // 152
            -8.586524421390938e+01,  // 153
            1.025325063120949e+02,   // 154
            1.190288736494028e+02,   // 155
            1.127916397287427e+01,   // 156
            -6.533538609281062e-04,  // 157
            -1.964460756252339e-03,  // 158
            -1.263965136794111e+02,  // 159
            8.054944091364518e+02,   // 160
            -9.647876942132510e+01,  // 161
            1.062879489354729e+02,   // 162
            -6.238564016216556e+02,  // 163
            1.911593332273748e+01,   // 164
            1.447565644790390e+00,   // 165
            -1.528305594548118e-01,  // 166
            5.874262659068990e+01,   // 167
            4.980523958800334e+00,   // 168
            1.865254140707839e+00,   // 169
            -3.276274076167391e+02,  // 170
            -3.057669318917934e-01,  // 171
            3.303056586565565e+02,   // 172
            8.458667640113981e-04,   // 173
            1.660882800010405e+01,   // 174
            1.440049827252539e-05,   // 175
            2.541871972954252e-02,   // 176
            6.653324791662896e-01,   // 177
            1.371111378689062e+00,   // 178
            5.139651712270515e+00,   // 179
            1.149729131628784e-06,   // 180
            -7.616373735158080e+00,  // 181
            1.848224270899690e-02,   // 182
            -1.486148678384943e+01,  // 183
            -8.257930609890364e-03,  // 184
            3.027550274442141e-05,   // 185
            -2.050606755616478e-05,  // 186
            1.134449370688675e-02,   // 187
            6.678231487244777e+02,   // 188
            3.222033616816789e-03,   // 189
            1.450254713286415e+00,   // 190
            -9.179725460323589e-01,  // 191
            1.147242417933878e-01,   // 192
            1.067249042839896e-02,   // 193
            -2.220600956465754e+00,  // 194
            -1.117160799621998e+00,  // 195
            -8.435864064091875e+00,  // 196
            -2.720542520976686e-04,  // 197
            -1.196379420098548e+02,  // 198
            2.511090861100307e-02,   // 199
            -6.181200143073790e+00,  // 200
            -6.325345592278400e-07,  // 201
            6.512551109951698e+00,   // 202
            1.517153545397339e-04,   // 203
            1.539817315960580e-01,   // 204
            1.048919578577065e+01,   // 205
            -4.172923100662866e+00,  // 206
            -2.494979247065879e+00,  // 207
            5.760658107905120e+00,   // 208
            -1.372759383428849e-05,  // 209
            8.158477529926984e-01,   // 210
            8.792230161494487e-04,   // 211
            3.594349114005577e-04,   // 212
            -7.143814074799361e-01,  // 213
            5.639869532406508e-05,   // 214
            1.163157872993312e-01,   // 215
            -2.129599073005833e+00,  // 216
            -1.284856907025513e-04,  // 217
            -1.672407380458107e-01,  // 218
            -4.958600511626711e-01,  // 219
            4.739406507481337e+01,   // 220
            2.454232771353123e-06,   // 221
            -4.312818357565276e-01,  // 222
            7.770385628535695e+00,   // 223
            -4.989175046522452e-05,  // 224
            -2.453135512061128e+00,  // 225
            1.566389609781693e-01,   // 226
            2.430751188617456e+01,   // 227
            -2.387177118968877e+00,  // 228
            -3.945041562041966e-05,  // 229
            -2.502781081355927e-06,  // 230
            1.352111014169116e-04,   // 231
            1.004017030865621e-03,   // 232
            3.710662961052117e-02,   // 233
            2.024330679311931e+00,   // 234
            1.396657227587546e+02,   // 235
            -9.870318501044814e-02,  // 236
            1.705383066090608e+01,   // 237
            -1.979560356043053e+00,  // 238
            3.574792428163742e-05,   // 239
            -2.725385107665493e-02,  // 240
            1.394932764973034e-06,   // 241
            7.963682247571731e-01,   // 242
            -2.375725678432321e-02,  // 243
            2.919677848684566e-06,   // 244
            -2.000110978935206e+01,  // 245
            2.236313952560970e-05,   // 246
            2.488914060541197e-01,   // 247
            -1.105461376755444e+00,  // 248
            9.614270314980951e+01,   // 249
            -1.127007558625936e-04,  // 250
            -3.594866188147571e-03,  // 251
            5.375088462981516e-06,   // 252
            -1.462479563254122e-03,  // 253
            2.060803435579022e-05,   // 254
            -9.484193654792676e-06,  // 255
            -1.827629781129657e-06,  // 256
            -1.751692512735948e-02,  // 257
            -1.848290922430358e-05,  // 258
            2.587435812193639e-01,   // 259
            -1.146976625581272e-07,  // 260
            -2.598816637803982e+00,  // 261
            1.905730355322207e+01,   // 262
            7.014852480398065e-01,   // 263
            -9.034522968354705e-04,  // 264
            -4.036588391353242e+00,  // 265
            -4.104203186643906e+01,  // 266
            -1.497669245740403e+01,  // 267
            -3.082870271514292e-01,  // 268
            -1.414730665594043e+00,  // 269
            9.298018475788444e+01,   // 270
            -1.230599331337568e+01,  // 271
            6.191118367045567e+00,   // 272
            3.380132858910763e-07,   // 273
            2.678088398128059e+02,   // 274
            3.992671304335919e-03,   // 275
            7.069465325230146e-04,   // 276
            5.123578728277243e+01,   // 277
            -6.517452810031224e-03,  // 278
            -6.012021340342310e+00,  // 279
            1.049108419964449e-05,   // 280
            1.084923394537213e+02,   // 281
            1.415766142637233e+01,   // 282
            5.545714546026092e-02,   // 283
            -8.399063120216751e-06,  // 284
            8.734107717518368e-02,   // 285
            2.410307386103226e-02,   // 286
            -5.330150498042584e-01,  // 287
            1.098802467433811e-03,   // 288
            -2.312803857233209e-05,  // 289
            1.162567102342022e-03,   // 290
            -1.972159292541996e-04,  // 291
            1.610055323045843e+02,   // 292
            1.727764841885146e+01,   // 293
            -7.624646477680430e+00,  // 294
            -4.560645390559326e-03,  // 295
            4.124464883539903e+01,   // 296
            4.912581522005306e-07,   // 297
            -1.278928644545925e+01,  // 298
            9.020649148860196e-02,   // 299
            -3.644319850278915e-01,  // 300
            -4.994441375836659e-07,  // 301
            3.055243451657102e-06,   // 302
            2.504992388759481e+00,   // 303
            -3.097596018186313e-02,  // 304
            4.471441671688451e-01,   // 305
            1.311182206379871e+02,   // 306
            3.039846181499553e-02,   // 307
            -6.866235911192324e-04,  // 308
            1.023596525847868e-01,   // 309
            -2.526660701367180e+00,  // 310
            -3.497610679143934e-01,  // 311
            -7.634950610972979e-01,  // 312
            3.289391574522698e-04,   // 313
            6.435113531618613e+01,   // 314
            2.176474498446537e+00,   // 315
            2.698052956556889e-07,   // 316
            9.017410466005015e-05,   // 317
            7.313391480171029e+00,   // 318
            6.676808344963386e+00,   // 319
            -7.019511795038619e-02,  // 320
            -3.452108924250841e+00,  // 321
            1.234335742322089e-04,   // 322
            -2.681812615063173e-04,  // 323
            5.769815883168296e-01,   // 324
            -2.282323982436013e-05,  // 325
            -5.256700563622532e+01,  // 326
            -2.391218614925749e-01,  // 327
            6.886955112679946e+00,   // 328
            -4.179446757303375e+01,  // 329
            1.162895618753361e+02,   // 330
            -5.050755317244834e-06,  // 331
            7.740341992260841e+00,   // 332
            -2.289019464803420e-05,  // 333
            -2.855494800587120e+01,  // 334
            -2.855516338542473e+01,  // 335
            -6.744056395641641e-07,  // 336
            1.464334104970131e+01,   // 337
            -2.231702461762684e-01,  // 338
            1.311631814903646e+02,   // 339
            -2.022535464808195e-04,  // 340
            -8.306781366936105e-05,  // 341
            -5.125699101463226e-01,  // 342
            -3.222112463220765e-01,  // 343
            -3.929270734938738e-06,  // 344
            -9.886788261797026e+00,  // 345
            1.138091753219703e+02,   // 346
            -8.698447767902281e-02,  // 347
            -8.663108000555871e-06,  // 348
            4.235138881762957e+00,   // 349
            8.678899390206123e-04,   // 350
            -5.977765771792840e+02,  // 351
            2.562148772848413e-01,   // 352
            2.291983324011585e-03,   // 353
            8.447915173916771e+00,   // 354
            -2.519934122179109e+02,  // 355
            -7.081513517661622e-01,  // 356
            -2.945972815562047e-06,  // 357
            1.220360987710653e-03,   // 358
            5.655329788747152e+01,   // 359
            -2.783417407241769e-05,  // 360
            -5.167544468126264e-02,  // 361
            4.137168362118318e-04,   // 362
            2.129866050495390e+01,   // 363
            -2.686675938892640e+01,  // 364
            8.003902029828586e+01,   // 365
            4.140974233902182e-04,   // 366
            1.547703321573102e-07,   // 367
            -5.518201310501980e+01,  // 368
            4.995525793226897e-01,   // 369
            7.333955809605079e-01,   // 370
            -6.034030919370162e-05,  // 371
            2.871983518200107e+00,   // 372
            -3.328636803150019e+01,  // 373
            -2.239154948832771e-01,  // 374
            2.498041733660009e-03,   // 375
            -3.289796406136995e-06,  // 376
            2.670154067826459e+01,   // 377
            -3.970137457833678e+01,  // 378
            8.065082848123351e-03,   // 379
            2.444046533585674e-01,   // 380
            -1.579071162586393e-06,  // 381
            -1.899124654986346e+01,  // 382
            -8.866367324320343e+00,  // 383
            5.758641644388165e-01,   // 384
            -5.634067618463408e+00,  // 385
            -1.538526988372521e+01,  // 386
            -1.956375709127955e+00,  // 387
            -1.554570241972536e+02,  // 388
            1.637673793761356e+00,   // 389
            1.110196822691917e-02,   // 390
            -1.389417719039352e-01,  // 391
            -4.379245418153211e-06,  // 392
            3.716707062051943e+00,   // 393
            4.203886662813878e+00,   // 394
            -1.613634370212517e-01,  // 395
            -8.670887286075975e-08,  // 396
            -1.166564657425054e+00,  // 397
            -1.758317456789166e+01,  // 398
            -1.347829938118249e-04,  // 399
            1.785892000309578e+01,   // 400
            1.136223483538035e-02,   // 401
            -8.195719588624667e-01,  // 402
            -2.638697940102324e-02,  // 403
            2.193717412441001e+00,   // 404
            -6.111078897468572e+01,  // 405
            -1.451612019312871e-07,  // 406
            1.562638763140381e-06,   // 407
            -5.114884809238562e-06,  // 408
            -1.512199838040957e-01,  // 409
            7.693441363697272e+00,   // 410
            2.347090898573660e-01,   // 411
            3.460515243484031e-01,   // 412
            3.095066247108497e-01,   // 413
            5.790491416879285e-01,   // 414
            -4.586369779105866e+01,  // 415
            2.730196410644564e+00,   // 416
            -1.554592159675068e+00,  // 417
            -8.103413098345203e-07,  // 418
            9.575846965966934e-06,   // 419
            -1.066420863120638e-02,  // 420
            -6.431555042223245e-05,  // 421
            -9.902641433347423e+00,  // 422
            -1.149041241660846e+00,  // 423
            3.782476974967657e+00,   // 424
            -1.315659265928492e+00,  // 425
            3.283424907083221e+01,   // 426
            -3.870666316883273e-01,  // 427
            -4.938901293979126e+00,  // 428
            6.192493424531414e-01,   // 429
            -2.963716253556972e-03,  // 430
            3.638428366432546e+01,   // 431
            3.674752604827189e-05,   // 432
            -2.390486904507997e-05,  // 433
            4.060565239166825e+00,   // 434
            1.652969179954805e+00,   // 435
            -8.846893296714601e-05,  // 436
            -4.736825350316258e-03,  // 437
            3.018992894898652e+01,   // 438
            6.657677032865812e-03,   // 439
            1.350059852461251e-01,   // 440
            -7.241171332483945e+01,  // 441
            -6.552499039788010e+01,  // 442
            4.738695079217798e+00,   // 443
            -1.040504298373096e+00,  // 444
            -2.039927317828038e-01,  // 445
            -2.012948356686295e-04,  // 446
            4.287563974515043e+01,   // 447
            -2.282260049823325e+00,  // 448
            1.337016855109879e+01,   // 449
            1.028210506514159e-01,   // 450
            -1.166682018032691e+01,  // 451
            2.151018494936441e-02,   // 452
            2.430786978917750e+02,   // 453
            1.499167440102826e-06,   // 454
            2.394115547149733e-04,   // 455
            -5.829670363169334e-04,  // 456
            3.190594303761213e+01,   // 457
            1.610035289346027e+00,   // 458
            -1.539968608206449e-02,  // 459
            3.403238831985399e+01,   // 460
            7.099616670751330e+00,   // 461
            -1.736991160510886e-01,  // 462
            9.985193631478240e-05,   // 463
            2.470602965808683e+00,   // 464
            -5.225343835626257e-02,  // 465
            1.687217798536554e-01,   // 466
            -1.348922600187190e-04,  // 467
            -7.327309815301530e+01,  // 468
            -1.837270182291685e+00,  // 469
            -4.555537315112111e+00,  // 470
            9.100830067999787e-01,   // 471
            -8.453733067368438e+00,  // 472
            -2.168574268971172e+00,  // 473
            2.735225724913899e+00,   // 474
            -3.103595389519938e+01,  // 475
            -6.079735205658359e-04,  // 476
            1.527572055690910e+01,   // 477
            -7.103530402592685e+00,  // 478
            -4.751697863504674e-01,  // 479
            -2.358145310598234e-01,  // 480
            1.936568142892655e-02,   // 481
            2.075861181320570e+00,   // 482
            3.162805201800809e-01,   // 483
            -4.337566122447426e-03,  // 484
            2.383202165124781e+01,   // 485
            -2.332553267565272e+00,  // 486
            -2.240538035402573e+01,  // 487
            -2.966778183954042e+01,  // 488
            -3.348319297680011e+02,  // 489
            1.929023441022190e+01,   // 490
            7.053042853957341e+01,   // 491
            1.208354857596420e+02,   // 492
            1.541212050610899e-07,   // 493
            2.193188025841454e-04,   // 494
            -5.703635468831591e-06,  // 495
            2.142813757785780e+00,   // 496
            -5.974857073765736e+00,  // 497
            1.077471082819928e+00,   // 498
            1.310427460415016e-02,   // 499
            -1.333943630743594e-07,  // 500
            5.091304835488221e-06,   // 501
            6.455100247010560e+00,   // 502
            5.959735777437748e-07,   // 503
            -6.763185368941298e-03,  // 504
            -1.524836036048039e-05,  // 505
            2.087616302748597e+00,   // 506
            1.816443579187104e+00,   // 507
            -3.230791153726352e-01,  // 508
            -1.933479995988554e+00,  // 509
            -1.762279311699633e+02,  // 510
            3.998890312012437e-05,   // 511
            2.079720504782573e-05,   // 512
            -3.254760843668323e+00,  // 513
            -3.191740791781767e-02,  // 514
            1.435335457716959e-05,   // 515
            -6.162135716052156e-08,  // 516
            7.497967530547023e-01,   // 517
            1.292995074018722e-01,   // 518
            -5.460761248532737e+01,  // 519
            1.401718987644301e-05,   // 520
            8.831732346469373e-01,   // 521
            5.045784725665903e-06,   // 522
            2.105201311848156e-01,   // 523
            -1.565219468543412e+01,  // 524
            -6.833555977399234e+01,  // 525
            -1.135644748247752e-02,  // 526
            -3.509920154666012e+01,  // 527
            6.743827419975136e-04,   // 528
            -2.594930407124788e-06,  // 529
            -4.619923058541652e-01,  // 530
            -2.244559201625300e+02,  // 531
            5.507960011988678e-01,   // 532
            1.908192269976286e-01,   // 533
            3.014921961209776e+00,   // 534
            -5.118384251834139e-03,  // 535
            -1.460109087243526e+00,  // 536
            3.253378694820810e-02,   // 537
            -7.225393263791128e-07,  // 538
            -2.697058132946806e-07,  // 539
            1.578012762953275e-01,   // 540
            4.925821494838625e-06,   // 541
            -1.261387294268327e+00,  // 542
            1.266696601200812e-03,   // 543
            -7.070334246324818e-03,  // 544
            -1.761607908967096e-02,  // 545
            8.689274047459039e-03,   // 546
            -5.984878668199681e-05,  // 547
            -3.205113085437566e+00,  // 548
            -2.913140091216869e-01,  // 549
            -9.948074375177160e-05,  // 550
            1.500637034350208e-06,   // 551
            -8.359178763455996e-04,  // 552
            -2.225008478425601e-01,  // 553
            -7.949271036824665e-05,  // 554
            1.471856092030808e+01,   // 555
            1.398568281639873e+02,   // 556
            -1.097719062199677e+00,  // 557
            7.743607589266777e-01,   // 558
            -7.427177458735219e-02,  // 559
            4.827605673720157e+00,   // 560
            2.023986079033419e-03,   // 561
            -3.756684029326901e-04,  // 562
            -5.052263640581677e+00,  // 563
            4.884496921565985e+00,   // 564
            2.967041999918278e+01,   // 565
            -2.094109663363226e+00,  // 566
            1.102628227482470e-05,   // 567
            1.901809030298951e-05,   // 568
            1.793048978040859e-05,   // 569
            1.918656084627952e+01,   // 570
            -3.594531514033336e+01,  // 571
            -1.377908279642677e-05,  // 572
            -2.477188826119334e+00,  // 573
            3.451692598008148e+00,   // 574
            -6.367533311913257e+00,  // 575
            1.427171114834184e-05,   // 576
            -4.673726805969870e+01,  // 577
            3.518337334739607e-05,   // 578
            -1.063407946295000e-03,  // 579
            8.665925817305356e-02,   // 580
            -3.826341770750251e+02,  // 581
            -3.392961310809415e-04,  // 582
            2.315261221661149e+00,   // 583
            4.305922163207516e+00,   // 584
            -9.365332776780224e-01,  // 585
            4.366335721228774e-05,   // 586
            8.805801859444125e+00,   // 587
            1.049732708265392e+01,   // 588
            2.861258017832147e+00,   // 589
            2.912951272245129e-04,   // 590
            1.840272596004554e-04,   // 591
            2.185450892993040e-03,   // 592
            3.082715903499043e-03,   // 593
            1.260803026947584e+01,   // 594
            7.889479618890959e-03,   // 595
            1.824605698168482e-06,   // 596
            1.475115355430159e-01,   // 597
            -1.786790116979374e-01,  // 598
            1.778688952296523e+00,   // 599
            -1.366820135141522e-03,  // 600
            6.619787934789118e-05,   // 601
            5.650456365101192e-01,   // 602
            1.322190835942146e-02,   // 603
            -1.725365168901230e+02,  // 604
            -1.650140372519958e-02,  // 605
            -3.948884857961100e-05,  // 606
            1.064027091637986e+02,   // 607
            -5.596838076526279e-01,  // 608
            -9.864927900093130e+01,  // 609
            1.487781528716870e-04,   // 610
            1.313997225892597e+02,   // 611
            -9.553928014273498e-04,  // 612
            -1.922914108928554e-04,  // 613
            5.014658775735044e+00,   // 614
            -2.908527318920081e-01,  // 615
            -2.364635680796634e-04,  // 616
            4.741787471476171e-04,   // 617
            4.525204005036215e-02,   // 618
            -7.208087913205999e-02,  // 619
            6.124666719646330e-05,   // 620
            3.985961835493162e-05,   // 621
            -1.040533135318464e+01,  // 622
            -7.222216019832093e-05,  // 623
            -6.961907751115410e-05,  // 624
            8.675359531731120e-04,   // 625
            1.639012673981899e-05,   // 626
            1.158118416360863e+02,   // 627
            -2.600913373220255e+00,  // 628
            1.319594217588925e-05,   // 629
            1.656511265726237e-01,   // 630
            -1.010523837491564e-01,  // 631
            2.765342692771448e-01,   // 632
            2.632962025382024e-01,   // 633
            2.083287273920183e-05,   // 634
            -3.163806432882824e-01,  // 635
            -1.585916169931874e+01,  // 636
            2.907634677560877e+01,   // 637
            3.783973415376040e-05,   // 638
            2.780636513639560e+02,   // 639
            -1.015390498612459e+02,  // 640
            1.082448727428622e+00,   // 641
            -1.554351281721324e-03,  // 642
            -1.437228960082757e-07,  // 643
            -2.712347668009004e-04,  // 644
            -1.142084259875838e-01,  // 645
            -3.524602773393998e-02,  // 646
            2.468014895105828e-04,   // 647
            3.135791628387349e-07,   // 648
            -2.126115311246196e+01,  // 649
            1.231659192151463e-01,   // 650
            -1.213107909693322e-06,  // 651
            2.952225565728891e-07,   // 652
            -8.542313090055987e-06,  // 653
            -1.259253420516254e+01,  // 654
            -8.816587083706592e+00,  // 655
            1.180012216641874e+01,   // 656
            -1.744105901337008e-05,  // 657
            7.437697718983277e+00,   // 658
            -3.176617874921851e-04,  // 659
            -1.530150414355854e+00,  // 660
            -9.779479965607067e-03,  // 661
            -2.166398087945417e+01,  // 662
            2.992770947004646e+01,   // 663
            -1.764138836754659e+02,  // 664
            7.227729009028187e-05,   // 665
            3.717706404889544e-02,   // 666
            -1.491341745511704e+01,  // 667
            -3.771088717539894e+02,  // 668
            -1.445772542619485e-03,  // 669
            -2.710695170039693e-01,  // 670
            -9.490305550266104e-01,  // 671
            -7.260528057332877e+00,  // 672
            4.094866661480152e-03,   // 673
            -2.437866598524336e-01,  // 674
            1.782336766208499e-02,   // 675
            1.469138459852783e-04,   // 676
            -2.100634229186358e-01,  // 677
            2.923107589086504e-02,   // 678
            4.702296073499660e+02,   // 679
            -2.899725982364106e-05,  // 680
            3.529578338301514e-01,   // 681
            7.011232063951473e-05,   // 682
            1.516343117604058e+02,   // 683
            -2.052336117039833e+02,  // 684
            -4.341810255737723e-04,  // 685
            -9.313147905769537e+01,  // 686
            5.987742184459672e-02,   // 687
            2.925963296996013e-01,   // 688
            1.620885290619957e+01,   // 689
            6.838977218357808e-02,   // 690
            1.975713778702614e-04,   // 691
            -5.536397364342740e-05,  // 692
            -1.794872755381803e-02,  // 693
            4.090639875211071e-05,   // 694
            -4.808197648209153e-08,  // 695
            5.447916701135978e+00,   // 696
            -6.568642432864631e-02,  // 697
            3.095559740097107e-02,   // 698
            4.993676524722588e+01,   // 699
            4.083072241820726e-04,   // 700
            -7.171482822274662e-05,  // 701
            -7.370001921367040e-05,  // 702
            -2.113309504815983e-02,  // 703
            -3.315969746383387e-05,  // 704
            -2.121617201424169e+00,  // 705
            -8.004665842117823e-03,  // 706
            7.138693729531486e-05,   // 707
            -1.688751303688881e-02,  // 708
            7.009164218867543e-02,   // 709
            -1.543508812355977e-03,  // 710
            6.626856462586446e-03,   // 711
            2.166395956209400e-02,   // 712
            -1.640810146752055e+01,  // 713
            -4.840454316628567e+00,  // 714
            2.150054016703844e+01,   // 715
            1.167181917013988e+00,   // 716
            2.534370145442855e-01,   // 717
            -9.159901186331252e+00,  // 718
            -3.530156526284193e+01,  // 719
            4.386436825590712e+00,   // 720
            2.353046487363437e-01,   // 721
            1.137384593063410e+02,   // 722
            3.556503824679121e+00,   // 723
            3.809871921921054e-01,   // 724
            -5.576704779080445e+01,  // 725
            1.066897200900313e+02,   // 726
            5.738308423327987e-11,   // 727
            1.573561739096481e-04,   // 728
            8.045883375561861e+01,   // 729
            -1.474907829522202e+01,  // 730
            6.563583593939963e-02,   // 731
            -1.409991413639262e-01,  // 732
            -1.304058423493579e+00,  // 733
            -4.956297016682513e+01,  // 734
            -1.334318792018127e-03,  // 735
            -6.958128945520658e+00,  // 736
            -1.673527126531647e-02,  // 737
            9.880736842791213e+00,   // 738
            5.846964406274902e-02,   // 739
            -1.242475783039120e+02,  // 740
            -8.963104368397344e+00,  // 741
            -9.270831188863642e-01,  // 742
            -2.077042165517459e+00,  // 743
            3.372428903375991e-05,   // 744
            1.079120021704475e-04,   // 745
            3.406431468360484e-05,   // 746
            -1.344945205731269e-05,  // 747
            -5.187969024154769e-05,  // 748
            -3.108305603193972e+00,  // 749
            -6.040457750613752e+00,  // 750
            -1.449628868480312e-02,  // 751
            -4.112673833757345e-04,  // 752
            9.973283644002639e-04,   // 753
            -3.217077744668609e-03,  // 754
            3.234495124478343e+00,   // 755
            1.481552853576476e-03,   // 756
            -1.244174450435760e+00,  // 757
            2.309015204351385e+00,   // 758
            7.944182094535948e+00,   // 759
            1.165681906690160e+02,   // 760
            -3.783389095953125e-02,  // 761
            -4.511518552588432e+00,  // 762
            4.948706326782505e-08,   // 763
            -1.101072514918706e+02,  // 764
            -1.239598022127714e-01,  // 765
            -7.658331174577205e-01,  // 766
            -1.452622540321399e-02,  // 767
            6.343110432109985e+00,   // 768
            -9.024656241580456e+01,  // 769
            -8.004959413047829e+02,  // 770
            -5.448985086427571e+00,  // 771
            -1.069015027264183e-04,  // 772
            3.083948516343684e-01,   // 773
            -2.049848239259532e+00,  // 774
            3.095518992046285e+02,   // 775
            -1.453871166432934e-02,  // 776
            -3.102931301872164e+02,  // 777
            1.451170622055230e+01,   // 778
            1.414364520842047e+02,   // 779
            -1.572652393830358e+00,  // 780
            2.067535382974077e-02,   // 781
            3.846591144551321e+01,   // 782
            -6.441327717908125e-03,  // 783
            1.067811666274135e-02,   // 784
            4.971092011449874e-06,   // 785
            -6.414676336680277e-01,  // 786
            2.132753042608578e+01,   // 787
            -5.045319188549845e-01,  // 788
            5.248517075194001e-02,   // 789
            3.306289386290605e-06,   // 790
            6.214194305935740e-03,   // 791
            -6.818489371980430e-04,  // 792
            4.493838548644373e-01,   // 793
            -2.038548329537781e-07,  // 794
            4.839550639858303e+00,   // 795
            -1.127125850338366e+00,  // 796
            -8.317593290503736e-03,  // 797
            5.595890245069520e-08,   // 798
            -1.951300427172766e-04,  // 799
            -3.123697949044156e+02,  // 800
            3.546355512331710e+02,   // 801
            -9.269712568353718e-02,  // 802
            1.154367283161959e+02,   // 803
            5.410094943339890e-02,   // 804
            1.857526271927197e+01,   // 805
            -5.187618841315060e-04,  // 806
            2.317574330256369e-05,   // 807
            -5.670464833544672e-01,  // 808
            1.159751142013588e-04,   // 809
            1.149196355672363e-02,   // 810
            -1.044795469414907e+02,  // 811
            -2.795797949020442e-01,  // 812
            -4.706203532558365e-04,  // 813
            6.897584750505611e-06,   // 814
            -1.788120820699903e-05,  // 815
            -1.407607613077531e-02,  // 816
            1.429406890097404e-01,   // 817
            -1.610183816466448e-01,  // 818
            3.400711528025767e-02,   // 819
            9.183966707292026e+00,   // 820
            -9.147098738365418e-03,  // 821
            6.830184423110179e-04,   // 822
            -8.916513360552155e-06,  // 823
            5.315135145668360e+00,   // 824
            6.145582505657673e+00,   // 825
            -4.318338433532239e+01,  // 826
            -5.489227990290378e+00,  // 827
            -2.545738668969092e-05,  // 828
            1.910001930136034e-04,   // 829
            2.894985024189839e-01,   // 830
            -9.273466504463797e-04,  // 831
            5.659840792076489e+00,   // 832
            5.977320938247209e+00,   // 833
            -1.801310382580218e-05,  // 834
            1.970963492702938e-08,   // 835
            -3.508783401306984e+02,  // 836
            -3.161509987981683e-03,  // 837
            7.034782791060853e-06,   // 838
            4.372595800385600e+01,   // 839
            -3.236532010283570e-01,  // 840
            3.556005078131821e+01,   // 841
            2.316756009534024e+00,   // 842
            2.709550990465312e+00,   // 843
            1.356350312883877e-02,   // 844
            1.163834039179506e-07,   // 845
            -1.590620406209524e+02,  // 846
            -1.371847037518882e+01,  // 847
            -4.839195773325988e-01,  // 848
            6.268574781849655e-03,   // 849
            3.771670154383153e-05,   // 850
            2.768882161754700e-04,   // 851
            -6.207332084212862e-01,  // 852
            2.053098961068898e+01,   // 853
            -3.069614513299328e+01,  // 854
            7.111608059465016e-09,   // 855
            4.058630074014717e-06,   // 856
            -2.051817308305395e+02,  // 857
            -2.804222323508182e-06,  // 858
            6.939356051118000e+01,   // 859
            -1.503894235537099e-04,  // 860
            8.512865973694867e+00,   // 861
            -3.486728712998320e+02,  // 862
            1.658139534898869e-04,   // 863
            1.880370393430095e-04,   // 864
            -2.559366123845920e+00,  // 865
            -1.520484311183351e-04,  // 866
            8.883632312321717e-04,   // 867
            1.446078799758367e-01,   // 868
            -3.437258477444026e+01,  // 869
            -1.838917310066632e-05,  // 870
            -1.571070757103176e-06,  // 871
            2.924664336774771e-04,   // 872
            6.008548798511252e+00,   // 873
            -4.511078856410974e-01,  // 874
            2.003408143609218e+02,   // 875
            -1.191570329584980e-05,  // 876
            2.790825600462343e+00,   // 877
            3.494034588092543e-06,   // 878
            1.876744774111546e+01,   // 879
            2.039213202678244e+01,   // 880
            -1.324508067736345e+01,  // 881
            -3.826458058979379e-05,  // 882
            2.579663072964323e+00,   // 883
            1.172519677349886e+00,   // 884
            1.966668544974114e+00,   // 885
            -4.179587028925214e-03,  // 886
            6.362809768017312e-04,   // 887
            2.203054350756185e+02,   // 888
            -8.640355490092416e-04,  // 889
            -9.352328687554243e-04,  // 890
            -3.148961307683857e-01,  // 891
            1.382641920851529e-04,   // 892
            1.347392326143503e+02,   // 893
            -1.668177904772233e-04,  // 894
            -1.002397320198042e+00,  // 895
            -1.877316944606812e+01,  // 896
            2.475222714442213e-07,   // 897
            -2.110690998010313e+02,  // 898
            1.637209278269282e+00,   // 899
            -6.555291509201297e+00,  // 900
            8.411226878675349e-04,   // 901
            4.445113951447341e-03,   // 902
            1.031011878744669e+01,   // 903
            7.009807337288278e+00,   // 904
            1.559743175301588e+00,   // 905
            -2.117896811229812e-04,  // 906
            3.835355729896199e-04,   // 907
            1.974077700959808e-06,   // 908
            4.374906579735478e+00,   // 909
            -5.885674113441403e-01,  // 910
            1.286055354191841e+00,   // 911
            -4.607376749324844e+01,  // 912
            2.995108654500460e+01,   // 913
            1.523584144096749e+02,   // 914
            -5.006867139234629e-08,  // 915
            5.595060882980145e-04,   // 916
            2.347609706973127e-06,   // 917
            -1.334361644635743e+02,  // 918
            -9.155053455513831e-02,  // 919
            2.202451131504725e-02,   // 920
            6.977602165935357e-01,   // 921
            -1.085959329313928e+02,  // 922
            4.095097403752791e+01,   // 923
            3.527702680511406e+01,   // 924
            -2.396692939801242e+00,  // 925
            -9.128445887985519e+00,  // 926
            -6.184253641708805e+01,  // 927
            1.931202968606943e+02,   // 928
            -3.255880849817720e-03,  // 929
            -4.872408022199896e-05,  // 930
            6.307426073439253e+00,   // 931
            -1.660281531573416e-01,  // 932
            -1.918280074726981e-04,  // 933
            -3.631986294631392e+01,  // 934
            3.670996847734021e+02,   // 935
            -7.025117869126425e+00,  // 936
            4.953400830912048e-02,   // 937
            -2.646100648814677e+00,  // 938
            -3.041937701053981e+00,  // 939
            -8.022021170757931e+00,  // 940
            -1.222309120726247e+01,  // 941
            1.172627777693117e-04,   // 942
            -2.205249095225853e+00,  // 943
            -1.200876164393618e-05,  // 944
            1.428174874767705e+02,   // 945
            6.698161920532169e-06,   // 946
            -6.394953619740066e+00,  // 947
            -5.123444635097838e-04,  // 948
            9.395030791041503e-02,   // 949
            5.609224037227120e-01,   // 950
            -4.357730323098819e-06,  // 951
            2.186324867104641e+02,   // 952
            -4.411070894195969e+01,  // 953
            -6.560796606378801e+00,  // 954
            2.850510485573290e-04,   // 955
            3.478810300308940e+02,   // 956
            -1.759765901273621e+02,  // 957
            1.646874615092180e-02,   // 958
            -3.722372777146979e-01,  // 959
            -8.486031091156785e+01,  // 960
            2.370865390439791e+00,   // 961
            4.009007576870725e+02,   // 962
            -1.804012892272040e-03,  // 963
            3.316274038254902e-03,   // 964
            1.344773918260797e-04,   // 965
            1.671761974252994e+02,   // 966
            3.598838971297050e+00,   // 967
            -5.769254778800604e-01,  // 968
            1.974789767585788e-02,   // 969
            5.226995326887480e+02,   // 970
            -3.464841370617857e-01,  // 971
            -1.436168696336782e-04,  // 972
            2.846733636023847e-01,   // 973
            -2.283680078844745e-05,  // 974
            5.656035322930717e+02,   // 975
            -5.506262193604592e+01,  // 976
            -1.417354734672618e-03,  // 977
            5.308405239688908e+02,   // 978
            2.143600362068669e-01,   // 979
            3.182111200087612e+01,   // 980
            1.879875483973083e+02,   // 981
            -2.595575367396423e-05,  // 982
            1.412903914254548e+01,   // 983
            7.397368430800095e+01,   // 984
            7.957213315713472e+00,   // 985
            9.786297170326139e-01,   // 986
            -3.378161258688242e-05,  // 987
            1.404305852774229e+02,   // 988
            2.809336553430406e-05,   // 989
            5.569165153521451e-04,   // 990
            1.459432191558482e-02,   // 991
            -6.479916207424106e-03,  // 992
            -2.812232563468796e+02,  // 993
            9.844902854357214e+00,   // 994
            -2.406014514481681e+02,  // 995
            -3.769973036550997e-05,  // 996
            2.545712627628300e+02,   // 997
            5.548053299542879e-05,   // 998
            6.564496960909656e+01,   // 999
            1.101615118229767e-02,   // 1000
            -6.397799604700922e-07,  // 1001
            -1.762656793550220e-04,  // 1002
            -5.435738669253994e-02,  // 1003
            -3.945311642196170e+02,  // 1004
            1.594884034466814e+00,   // 1005
            3.119680676278214e+00,   // 1006
            -2.417184518229284e+02,  // 1007
            -4.769768842079672e+02,  // 1008
            1.795208035910823e+01,   // 1009
            -8.656291712659959e+01,  // 1010
            -2.480288477794002e-02,  // 1011
            2.829397092270745e+00,   // 1012
            2.444043820781232e-03,   // 1013
            -1.514423161539158e+01,  // 1014
            4.361353189644865e+02    // 1015
        };                           // 1015

        //----------------------------------------------------------------------------//

        m_r3i = 0.000000000000000e+00;        // A
        m_r3f = 4.500000000000000e+00;        // A
        m_kHH_intra = 1.505534948596776e+00;  // A^(-1)
        m_kOH_intra = 5.465082839032473e-01;  // A^(-1)
        m_kHH = 5.669173309079441e-01;        // A^(-1)
        m_kOH = 7.149221098880705e-01;        // A^(-1)
        m_kOO = 5.877602499345129e-01;        // A^(-1)
        m_kXO = 4.772570648100866e-01;        // A^(-1)
        m_kXH = 4.399022736050681e-02;        // A^(-1)
        m_dHH_intra = 9.488236920987529e+00;  // A
        m_dOH_intra = 3.154303998006160e+00;  // A
        m_dHH = 1.023524681458323e+01;        // A
        m_dOH = 5.287618686287184e+00;        // A
        m_dOO = 4.432532991705378e+00;        // A
        m_dXO = 3.541346877424266e+00;        // A
        m_dXH = 1.561826519728719e+00;        // A

        std::copy(the_poly, the_poly + ncoeffs, m_coeffs);

    } else if (m3 == "rb+") {
        const double the_poly[] = {-6.714728321324774e+02,  // 0
                                   -4.363140175199011e+01,  // 1
                                   -1.386914275178937e+02,  // 2
                                   -3.889582732490770e+01,  // 3
                                   2.112291866799408e+01,   // 4
                                   4.818514212211005e+02,   // 5
                                   -3.954639580670868e+00,  // 6
                                   1.919565817436346e+02,   // 7
                                   3.841432754791289e+02,   // 8
                                   -3.064039384869683e+00,  // 9
                                   -1.869750599698350e+02,  // 10
                                   5.545501639664313e+01,   // 11
                                   -3.219419865853253e+02,  // 12
                                   1.977871393489533e+01,   // 13
                                   -1.291414282498333e+01,  // 14
                                   1.281690819272867e+02,   // 15
                                   -5.240315338802967e-03,  // 16
                                   -2.739462113741977e+01,  // 17
                                   6.462588461388090e+01,   // 18
                                   1.668373433684081e+02,   // 19
                                   -1.588190015006985e-02,  // 20
                                   -1.977720372623251e-03,  // 21
                                   -8.874711057352635e-03,  // 22
                                   8.033476933325554e+00,   // 23
                                   -6.482852371440761e+00,  // 24
                                   2.174767923143513e+00,   // 25
                                   1.562022859975160e+00,   // 26
                                   -8.213379925552703e+00,  // 27
                                   4.152935603783272e-03,   // 28
                                   9.031232724284004e+01,   // 29
                                   1.312183717124072e+02,   // 30
                                   1.988108150759720e+02,   // 31
                                   6.190191208013559e+01,   // 32
                                   -7.563873536608644e-01,  // 33
                                   2.919477435592267e+00,   // 34
                                   -1.823460983203908e+00,  // 35
                                   -7.450224217306664e-02,  // 36
                                   1.335749882995847e+01,   // 37
                                   3.199149941375068e+00,   // 38
                                   -1.598641569648173e+02,  // 39
                                   6.937899621784768e-01,   // 40
                                   -4.005185119063229e+02,  // 41
                                   -1.228333967729277e+02,  // 42
                                   3.567240828181590e-01,   // 43
                                   1.142402989320469e+00,   // 44
                                   7.392785256175363e+00,   // 45
                                   1.598745144670876e-01,   // 46
                                   1.227169939587393e+01,   // 47
                                   4.275141101291148e+01,   // 48
                                   -5.622038267128394e-02,  // 49
                                   -5.730508324344782e-04,  // 50
                                   3.367314578116563e+02,   // 51
                                   -1.391845415004800e+00,  // 52
                                   -1.332906947751964e+01,  // 53
                                   -3.562917669165878e+01,  // 54
                                   -1.303710186295000e+01,  // 55
                                   2.259586052404480e+02,   // 56
                                   -2.815460467966766e+02,  // 57
                                   5.243846533578567e+00,   // 58
                                   1.573617916505273e+02,   // 59
                                   3.356602880708963e-03,   // 60
                                   9.247946061326073e+02,   // 61
                                   -4.452560934846617e+00,  // 62
                                   -3.827104399828588e-02,  // 63
                                   -1.265207441964850e+02,  // 64
                                   3.482478936499621e+02,   // 65
                                   3.612587750281490e+01,   // 66
                                   -4.074501349717139e+00,  // 67
                                   8.264623865476753e+01,   // 68
                                   2.820542808936633e+02,   // 69
                                   -1.506899723817858e+01,  // 70
                                   -1.701859316430090e+02,  // 71
                                   -3.160429109519755e-03,  // 72
                                   -2.266017468844487e+02,  // 73
                                   7.871906430321682e-02,   // 74
                                   -1.353597448417186e+02,  // 75
                                   9.249310597542120e-01,   // 76
                                   2.357131997929651e+02,   // 77
                                   4.315916427905495e+01,   // 78
                                   -2.832644185068246e-01,  // 79
                                   -1.351283493340793e+02,  // 80
                                   -3.095835099837664e+02,  // 81
                                   -3.491038465915290e+02,  // 82
                                   3.644425070992228e+02,   // 83
                                   2.067151302520338e+01,   // 84
                                   5.321340375747631e+01,   // 85
                                   1.199047692453141e+02,   // 86
                                   -5.282228546934670e+01,  // 87
                                   -4.033341204996053e+02,  // 88
                                   1.915464889949110e+00,   // 89
                                   9.411703303691772e+01,   // 90
                                   -1.030832247312555e+02,  // 91
                                   1.537520500399295e-01,   // 92
                                   -4.097062755293418e+02,  // 93
                                   5.411604275525666e+00,   // 94
                                   1.761460286969922e-03,   // 95
                                   3.819365442310202e-03,   // 96
                                   1.202173922991567e+00,   // 97
                                   6.616057575410919e+01,   // 98
                                   -1.275382897814560e-01,  // 99
                                   2.689272737718355e+01,   // 100
                                   -6.584812791848249e+00,  // 101
                                   6.557632059515470e-02,   // 102
                                   -1.979176493602647e-01,  // 103
                                   -4.779019764498494e+00,  // 104
                                   -1.134645949907565e-01,  // 105
                                   -2.144526254774982e+01,  // 106
                                   -4.457078681689003e-01,  // 107
                                   1.779664491556644e+00,   // 108
                                   -2.331879351581114e-02,  // 109
                                   1.594152183424314e-03,   // 110
                                   2.904686739607633e+02,   // 111
                                   8.409835583012338e+00,   // 112
                                   -3.833702688636285e-01,  // 113
                                   -1.890467742673176e+01,  // 114
                                   1.829697765919892e+02,   // 115
                                   5.688384663056255e+01,   // 116
                                   -1.760654851952368e+00,  // 117
                                   1.880109674710376e+01,   // 118
                                   2.885466492393321e-02,   // 119
                                   -1.644863413212304e+00,  // 120
                                   4.210266210924460e+01,   // 121
                                   2.440860446788038e+00,   // 122
                                   1.610806881725733e+02,   // 123
                                   5.148380764476956e+01,   // 124
                                   5.301297705167560e-03,   // 125
                                   -1.325524726568575e+00,  // 126
                                   2.057366736037476e-01,   // 127
                                   -1.307148432109262e-02,  // 128
                                   -2.050152873522737e+00,  // 129
                                   9.219725764562587e+01,   // 130
                                   -5.087723817145313e+00,  // 131
                                   -2.445164723173625e+02,  // 132
                                   1.370836006559031e-01,   // 133
                                   -2.020208757062742e+00,  // 134
                                   3.808178351995332e+01,   // 135
                                   -3.799326862153674e+02,  // 136
                                   -7.882969204698008e-02,  // 137
                                   2.215660520565834e+00,   // 138
                                   -6.075838896258147e+01,  // 139
                                   -3.425065885189007e-01,  // 140
                                   -3.243935694349357e+00,  // 141
                                   -5.378368875551785e+00,  // 142
                                   6.532358713717749e-01,   // 143
                                   -1.070236752060738e+02,  // 144
                                   -2.085502311092046e+01,  // 145
                                   2.017653514503628e+01,   // 146
                                   -7.033931281500982e+02,  // 147
                                   -1.851244602275381e+01,  // 148
                                   -1.409561928445775e-01,  // 149
                                   3.971285119445995e+01,   // 150
                                   -2.308874939980401e+02,  // 151
                                   3.996859853736046e+01,   // 152
                                   7.098162997901938e+00,   // 153
                                   2.944055828882035e-01,   // 154
                                   -1.983915707462395e-02,  // 155
                                   5.946219414219450e-01,   // 156
                                   -2.850667492385959e-02,  // 157
                                   -2.497630740353562e+00,  // 158
                                   -2.196423622919967e-01,  // 159
                                   1.535190365209169e+02,   // 160
                                   -3.425901338580987e+01,  // 161
                                   1.276129158340175e+02,   // 162
                                   -4.466689777184263e+01,  // 163
                                   6.998234924543068e-02,   // 164
                                   4.263353986570794e-02,   // 165
                                   5.064800787484299e-05,   // 166
                                   6.953978977807164e-01,   // 167
                                   2.142037732086546e-02,   // 168
                                   5.837438944560586e-03,   // 169
                                   -2.352044298724064e+01,  // 170
                                   -3.249447019598929e-01,  // 171
                                   -1.391671084090271e+01,  // 172
                                   -4.116511325001765e-03,  // 173
                                   4.545856188380518e+00,   // 174
                                   1.124013756037121e-05,   // 175
                                   -7.824572340120986e-05,  // 176
                                   -6.142704759061706e-01,  // 177
                                   1.174394634177042e+00,   // 178
                                   -1.510268296415493e+00,  // 179
                                   1.129863412721686e-05,   // 180
                                   -6.580540352432963e-01,  // 181
                                   9.718789544739972e-06,   // 182
                                   1.750617820134259e+00,   // 183
                                   6.802163888166469e-05,   // 184
                                   2.304188853080998e-02,   // 185
                                   -5.349960352483601e-03,  // 186
                                   -1.895755701810564e-01,  // 187
                                   8.024165904259242e+01,   // 188
                                   -1.531001303767819e-05,  // 189
                                   6.143980333485007e-02,   // 190
                                   3.313015946087843e-02,   // 191
                                   1.888349737460998e-03,   // 192
                                   -1.250110931957183e-01,  // 193
                                   -4.352343911273654e-03,  // 194
                                   -2.257145886021001e+00,  // 195
                                   -1.885572841000867e+00,  // 196
                                   -1.307473763022218e+01,  // 197
                                   -4.228047815023847e+01,  // 198
                                   1.395016663829664e-02,   // 199
                                   -2.261448529716092e-02,  // 200
                                   -2.537819222550004e-03,  // 201
                                   2.730446876241906e-03,   // 202
                                   6.647274937803384e-06,   // 203
                                   2.379721127280206e-03,   // 204
                                   3.174453849559434e+01,   // 205
                                   1.619029058088688e+01,   // 206
                                   -2.840515817501993e-02,  // 207
                                   7.736488271681914e-03,   // 208
                                   7.754235433638342e-05,   // 209
                                   -1.005595698707876e-01,  // 210
                                   6.138961354699473e-07,   // 211
                                   2.645306287133108e-01,   // 212
                                   8.284573835345902e-01,   // 213
                                   -7.107435508018771e-05,  // 214
                                   1.542032619427511e-04,   // 215
                                   -7.976689705998748e-02,  // 216
                                   8.916665337729315e+00,   // 217
                                   7.303896049581246e-05,   // 218
                                   -2.629249595244717e+00,  // 219
                                   1.277955235859407e+01,   // 220
                                   -5.201786106533473e-04,  // 221
                                   5.704809087633139e-02,   // 222
                                   1.254569111557653e-02,   // 223
                                   -7.126910412170754e-03,  // 224
                                   4.620101361706360e+01,   // 225
                                   -6.575102384889643e-03,  // 226
                                   -2.822798035662841e+00,  // 227
                                   -3.129084218282826e-03,  // 228
                                   -1.081911207434448e-03,  // 229
                                   -1.441258039550781e-01,  // 230
                                   1.418847255404676e-03,   // 231
                                   1.369561149731812e-03,   // 232
                                   -1.224244858303015e-01,  // 233
                                   -1.472335444634327e-01,  // 234
                                   2.427729618990854e+01,   // 235
                                   -4.504177140780162e-03,  // 236
                                   6.391888452284322e-02,   // 237
                                   -2.583166263109491e-01,  // 238
                                   -2.128366003815633e-04,  // 239
                                   1.461428754021740e-04,   // 240
                                   -5.340424672948237e-04,  // 241
                                   -1.173639403592221e+00,  // 242
                                   -2.346317722736357e-01,  // 243
                                   3.009820648721948e-03,   // 244
                                   1.799744000222648e-01,   // 245
                                   1.914496050794362e-01,   // 246
                                   4.272677888665318e-03,   // 247
                                   -4.313202790686972e-01,  // 248
                                   -3.634702379630934e+02,  // 249
                                   -1.366200887689173e-02,  // 250
                                   4.166790179029420e-01,   // 251
                                   -2.158611522293272e-03,  // 252
                                   -2.655040863797631e-01,  // 253
                                   -2.471244578526874e-01,  // 254
                                   8.477646316065131e-05,   // 255
                                   -5.060881596376556e-06,  // 256
                                   1.343630101811485e-01,   // 257
                                   -5.590616893520454e+00,  // 258
                                   5.999393983390101e+01,   // 259
                                   -6.443668655624251e-05,  // 260
                                   5.228684409408196e-01,   // 261
                                   7.239175830972887e+00,   // 262
                                   -1.458589662313280e-01,  // 263
                                   -2.201503287543999e+02,  // 264
                                   -1.479416319937067e+01,  // 265
                                   8.890477481359865e+00,   // 266
                                   -1.397143396506230e-01,  // 267
                                   3.994514220401729e-02,   // 268
                                   -4.286757949021438e-01,  // 269
                                   1.759535638597641e+01,   // 270
                                   3.305351969269277e+01,   // 271
                                   1.585171684381966e+00,   // 272
                                   7.889979264383080e-05,   // 273
                                   -1.138717547892599e+01,  // 274
                                   6.095228776179837e-05,   // 275
                                   1.055053861679342e-07,   // 276
                                   1.082626589609543e+01,   // 277
                                   -2.302923055633884e+00,  // 278
                                   -6.092825252715995e-03,  // 279
                                   -1.837070706798194e-02,  // 280
                                   2.077129866378322e+02,   // 281
                                   -6.708955729871093e+01,  // 282
                                   2.840143255064062e-03,   // 283
                                   1.435080503390045e-03,   // 284
                                   -7.734638613258618e-03,  // 285
                                   7.849652046366200e-02,   // 286
                                   -9.012088183113459e-01,  // 287
                                   -8.434899286462443e+00,  // 288
                                   -8.193669312767271e-01,  // 289
                                   3.943680258981485e+00,   // 290
                                   2.082981584475528e-02,   // 291
                                   1.719602252707356e+01,   // 292
                                   3.778652720707747e+00,   // 293
                                   -5.068734209642421e+00,  // 294
                                   5.494775146397105e-02,   // 295
                                   2.326975076893539e+02,   // 296
                                   2.461215974041024e-04,   // 297
                                   -9.573760256391191e-01,  // 298
                                   1.793478907150953e-03,   // 299
                                   -5.589145953650008e-02,  // 300
                                   4.026245666883726e-04,   // 301
                                   -1.090406552567644e-03,  // 302
                                   8.412960081856442e-01,   // 303
                                   3.160962360346736e-04,   // 304
                                   9.572688409356123e-02,   // 305
                                   -2.492442201705800e+02,  // 306
                                   -2.359236459604068e-06,  // 307
                                   -7.124081012276656e-02,  // 308
                                   1.066194838922440e-02,   // 309
                                   1.047574141777836e+02,   // 310
                                   -4.882000419469172e-03,  // 311
                                   5.545141688883068e-02,   // 312
                                   -4.748725507354680e-04,  // 313
                                   8.023635038427794e+01,   // 314
                                   1.630817829935597e+00,   // 315
                                   2.139676275897646e-03,   // 316
                                   -5.080108826133469e+00,  // 317
                                   1.374646979443325e+01,   // 318
                                   1.073675409345803e+00,   // 319
                                   -7.048540427968307e-03,  // 320
                                   1.095703861208231e-04,   // 321
                                   1.103531576906050e+00,   // 322
                                   -2.601656659943412e+00,  // 323
                                   3.500462985445887e-02,   // 324
                                   -4.090201590379030e-01,  // 325
                                   -1.613809163209218e+02,  // 326
                                   -2.821519447964519e-04,  // 327
                                   -6.469036102586738e+00,  // 328
                                   1.727893402080156e+00,   // 329
                                   4.577325035304951e+00,   // 330
                                   -4.167995632534388e-01,  // 331
                                   -8.906143654928534e-01,  // 332
                                   -6.900713038661780e-01,  // 333
                                   -3.430304831823862e+02,  // 334
                                   -1.886045486792436e+00,  // 335
                                   2.294902132320348e-07,   // 336
                                   6.669798891274169e+00,   // 337
                                   -2.715976933925071e-01,  // 338
                                   1.880867527629078e+02,   // 339
                                   1.878176188854761e-07,   // 340
                                   -4.258695857846682e-02,  // 341
                                   1.524632368200086e-02,   // 342
                                   -1.014209335044207e-02,  // 343
                                   5.783263024941789e-05,   // 344
                                   6.335908265288091e+01,   // 345
                                   5.667605720979927e+00,   // 346
                                   7.662713528614247e-03,   // 347
                                   5.059588602026490e-03,   // 348
                                   7.760659046507834e-03,   // 349
                                   -9.736020511889194e+00,  // 350
                                   -4.054881064019412e+02,  // 351
                                   -1.080390119214216e+02,  // 352
                                   6.087705314298826e+00,   // 353
                                   1.361931932555247e-01,   // 354
                                   -3.734778867032370e+02,  // 355
                                   -2.124101836997608e-01,  // 356
                                   9.896136947063756e-04,   // 357
                                   -1.273230510798113e-05,  // 358
                                   3.456420107203808e+02,   // 359
                                   6.919542426992226e-02,   // 360
                                   -2.001566213803325e-03,  // 361
                                   -1.343093266322780e+00,  // 362
                                   -1.197342174833058e-02,  // 363
                                   -1.830473102020364e-01,  // 364
                                   -1.023013941554691e+01,  // 365
                                   6.036729410236805e-04,   // 366
                                   -3.070929515376500e-04,  // 367
                                   -6.911406042723833e+00,  // 368
                                   2.272508951774306e-03,   // 369
                                   5.594565188971701e-02,   // 370
                                   -1.615964582836366e-02,  // 371
                                   -1.061406697060247e-01,  // 372
                                   -1.512322368437507e+00,  // 373
                                   -3.564667122393388e-03,  // 374
                                   5.549121382821617e-04,   // 375
                                   -2.952884117462492e-03,  // 376
                                   1.052874509867670e+02,   // 377
                                   3.043776763412937e+00,   // 378
                                   1.219580456498271e-04,   // 379
                                   4.323666126430639e-03,   // 380
                                   -3.137675314055304e-04,  // 381
                                   -9.229407079448144e+00,  // 382
                                   -9.097772889621057e-01,  // 383
                                   1.419376612330737e-03,   // 384
                                   1.431340221633478e+01,   // 385
                                   -2.713540108247452e+00,  // 386
                                   -4.334210993339043e-01,  // 387
                                   -3.195471818270286e+01,  // 388
                                   7.034248131611891e-01,   // 389
                                   2.909549170751049e-05,   // 390
                                   -5.405032373088560e-03,  // 391
                                   -3.374913700399322e-01,  // 392
                                   4.898147840889963e-01,   // 393
                                   1.292077912092629e+01,   // 394
                                   9.226650645017929e-02,   // 395
                                   -7.679837739716217e-05,  // 396
                                   -3.074023898967242e-02,  // 397
                                   -1.611974536860523e+00,  // 398
                                   6.271332336118721e-01,   // 399
                                   9.344243849149267e+00,   // 400
                                   7.823684054448107e-07,   // 401
                                   2.666363558844225e-02,   // 402
                                   -2.861714125596181e-04,  // 403
                                   2.345638169066201e+00,   // 404
                                   3.696868652864919e+00,   // 405
                                   -7.879535078931824e-06,  // 406
                                   -3.582473257343335e-03,  // 407
                                   -2.420511624798125e-03,  // 408
                                   4.044101948883332e-03,   // 409
                                   -4.050087974934675e-01,  // 410
                                   3.876926456172533e-03,   // 411
                                   5.762067940411119e-04,   // 412
                                   -3.795538280137941e-02,  // 413
                                   -2.786618867928316e+00,  // 414
                                   -9.298924862647209e-01,  // 415
                                   -2.482763084437897e-02,  // 416
                                   -4.736234014911567e-02,  // 417
                                   -2.522000225884173e-04,  // 418
                                   -4.377338154785951e-04,  // 419
                                   -6.103697107056612e-02,  // 420
                                   -1.971956950181026e-01,  // 421
                                   1.468617700345811e+01,   // 422
                                   -1.081598198445244e+00,  // 423
                                   -9.513844301184006e-01,  // 424
                                   1.355662757421005e-02,   // 425
                                   4.996132930603447e+00,   // 426
                                   4.675732483565100e-04,   // 427
                                   1.845460123319071e-01,   // 428
                                   -1.673767748275112e-01,  // 429
                                   2.920873482499891e+00,   // 430
                                   2.924587717806759e+02,   // 431
                                   3.483003585354563e-03,   // 432
                                   -1.569649979338484e-03,  // 433
                                   2.557104263346093e-01,   // 434
                                   -3.664859547310657e-02,  // 435
                                   6.608473127225184e-01,   // 436
                                   -8.609861799962194e-04,  // 437
                                   -2.521418032236243e-01,  // 438
                                   -1.730252937203181e-05,  // 439
                                   9.848385910193064e-02,   // 440
                                   -1.128021324714780e+01,  // 441
                                   5.495586614377665e+01,   // 442
                                   -1.530097025287709e-01,  // 443
                                   -5.368717557323595e+00,  // 444
                                   -8.573535322750788e-03,  // 445
                                   1.908465019638232e-02,   // 446
                                   1.077671013440069e+02,   // 447
                                   -1.014032462686930e-02,  // 448
                                   3.346993688822633e+00,   // 449
                                   1.169865889925316e+00,   // 450
                                   -1.490831162942768e+00,  // 451
                                   2.584446788292878e-04,   // 452
                                   1.712328300947614e+02,   // 453
                                   2.169178138895748e-01,   // 454
                                   1.741835385644535e+00,   // 455
                                   -5.026111906317567e-07,  // 456
                                   -5.098083045934325e+01,  // 457
                                   1.901240582989834e+01,   // 458
                                   -9.081411343398166e-04,  // 459
                                   7.639241848366082e+00,   // 460
                                   -2.800378735320489e-01,  // 461
                                   -3.796447255208806e-03,  // 462
                                   -3.492719818960360e-02,  // 463
                                   5.691184367391620e-02,   // 464
                                   -5.208367445837780e-04,  // 465
                                   -6.106223550879830e-02,  // 466
                                   -2.042601414524116e-01,  // 467
                                   -1.667632248816534e+00,  // 468
                                   3.601400825872047e-03,   // 469
                                   -2.597241327615505e-02,  // 470
                                   -3.718219609626669e+00,  // 471
                                   -3.828810679144197e-01,  // 472
                                   -2.842887993861698e+00,  // 473
                                   -4.696443596536158e+00,  // 474
                                   -2.297106582416725e+02,  // 475
                                   3.524475131562512e+01,   // 476
                                   1.372629643385085e+01,   // 477
                                   1.035946731355643e-04,   // 478
                                   -1.405670454077029e-03,  // 479
                                   2.455462248702896e-04,   // 480
                                   -7.762143687786061e-04,  // 481
                                   6.213796004889123e-03,   // 482
                                   -2.205489001330749e-03,  // 483
                                   1.345141962558431e-04,   // 484
                                   1.071208566424666e+02,   // 485
                                   2.823385065832095e-01,   // 486
                                   4.950131853601810e+00,   // 487
                                   1.180860858398840e+00,   // 488
                                   -2.981057066169781e+01,  // 489
                                   2.529042095930609e+01,   // 490
                                   1.138342740193430e+02,   // 491
                                   -3.716929141078171e+02,  // 492
                                   -2.601832402887359e-05,  // 493
                                   -3.805943506022009e-03,  // 494
                                   -2.385073661538376e-02,  // 495
                                   -1.695679842659849e+00,  // 496
                                   -3.020337312108612e-02,  // 497
                                   -2.446441145540622e-02,  // 498
                                   1.122702652274983e-05,   // 499
                                   3.063467255613795e-04,   // 500
                                   1.855473654804592e-01,   // 501
                                   5.217718586316986e+00,   // 502
                                   2.220008139070772e-05,   // 503
                                   6.911065462006019e-04,   // 504
                                   -1.631718725951852e-02,  // 505
                                   4.389091372764971e-01,   // 506
                                   -7.868640098875857e-04,  // 507
                                   2.408301911240879e-04,   // 508
                                   -9.202841560549771e-03,  // 509
                                   -9.409820429844829e+01,  // 510
                                   8.020516014348564e-02,   // 511
                                   2.021341436646141e-01,   // 512
                                   -5.670173254742616e-01,  // 513
                                   2.187247629424189e-02,   // 514
                                   3.211498666558766e-03,   // 515
                                   -1.095799073932056e-04,  // 516
                                   6.464931165025266e-02,   // 517
                                   -2.819934710526721e-02,  // 518
                                   5.818349003414536e-01,   // 519
                                   2.781159699637037e-01,   // 520
                                   -1.646319300709893e+00,  // 521
                                   -1.047191463627939e-03,  // 522
                                   -6.305436589796542e-03,  // 523
                                   -5.820313213957482e+01,  // 524
                                   -1.575447241495409e+01,  // 525
                                   -2.331858272252978e-04,  // 526
                                   -2.358649948909725e+01,  // 527
                                   5.030165129490639e-02,   // 528
                                   2.815520068366783e-06,   // 529
                                   1.713217697882108e-03,   // 530
                                   -6.353410552576287e+02,  // 531
                                   3.609670020513385e-01,   // 532
                                   8.354507915831444e-03,   // 533
                                   5.445800430412956e-03,   // 534
                                   -2.904425912973863e-04,  // 535
                                   -8.516142486998243e-02,  // 536
                                   2.765446201379409e-04,   // 537
                                   -6.352297744633986e-05,  // 538
                                   4.404321355372776e-03,   // 539
                                   2.483371807015336e-05,   // 540
                                   -1.544269827253065e-02,  // 541
                                   -2.553505882320682e-03,  // 542
                                   8.809353693694387e+00,   // 543
                                   4.652749434439631e-04,   // 544
                                   1.525767415240541e-02,   // 545
                                   -3.629643587672258e-03,  // 546
                                   1.688283847486422e-03,   // 547
                                   -1.476399436988888e+00,  // 548
                                   -2.283710154390495e-03,  // 549
                                   3.599363301663862e-03,   // 550
                                   -2.584908146258140e-03,  // 551
                                   -2.608148743006108e+01,  // 552
                                   1.153605772676723e-01,   // 553
                                   -3.643831991622186e+00,  // 554
                                   1.230398700280801e+00,   // 555
                                   -1.298165464699638e+01,  // 556
                                   1.097727200266110e-02,   // 557
                                   -1.850228968790565e+00,  // 558
                                   1.056419323679739e-01,   // 559
                                   8.306303633207023e-02,   // 560
                                   3.581713217227442e-01,   // 561
                                   8.346778594828889e-02,   // 562
                                   -4.705330328139756e-01,  // 563
                                   2.676076580410770e-02,   // 564
                                   -3.250434656821758e+02,  // 565
                                   1.637731274654800e-02,   // 566
                                   2.825617217620298e-03,   // 567
                                   -9.112265212703407e-04,  // 568
                                   1.761356405193958e-02,   // 569
                                   1.961707808548987e+00,   // 570
                                   -4.026861663609356e+01,  // 571
                                   6.026599664854580e-03,   // 572
                                   1.813138215350659e-02,   // 573
                                   -3.044754131130756e-01,  // 574
                                   -6.270846150002669e-01,  // 575
                                   -1.454106806467606e-03,  // 576
                                   -1.053526279363955e+02,  // 577
                                   -3.001550082126932e-04,  // 578
                                   1.737914810012879e-03,   // 579
                                   -2.681453782021645e-02,  // 580
                                   -9.666807278401181e+01,  // 581
                                   5.836271486589657e-02,   // 582
                                   -9.612192988149648e-01,  // 583
                                   5.100928264294129e-02,   // 584
                                   -1.166180233681435e-02,  // 585
                                   4.787921875835382e+00,   // 586
                                   -1.310496210162438e-01,  // 587
                                   -3.771890677817936e-02,  // 588
                                   6.163646215721495e-03,   // 589
                                   -1.316369750998313e-01,  // 590
                                   -5.446205331272846e+00,  // 591
                                   -1.419360346134627e-06,  // 592
                                   1.155442715666278e-05,   // 593
                                   4.159726949186608e-01,   // 594
                                   -2.510447043429310e-04,  // 595
                                   -2.716080427478290e-06,  // 596
                                   1.572209329400426e-03,   // 597
                                   -1.716173179581255e-03,  // 598
                                   4.986120488242236e-02,   // 599
                                   -3.907279357233652e+00,  // 600
                                   1.511665920948429e-02,   // 601
                                   1.879214231725746e-02,   // 602
                                   -6.162982000198345e-04,  // 603
                                   -8.220099855000164e+01,  // 604
                                   6.684252311874931e-03,   // 605
                                   -2.363592081010149e-02,  // 606
                                   1.483899814314188e+01,   // 607
                                   2.315390584940328e-02,   // 608
                                   -4.818839979395302e+02,  // 609
                                   -1.017117332861978e-02,  // 610
                                   3.625629954313404e+02,   // 611
                                   1.164377306095822e+01,   // 612
                                   -1.894313782202413e-03,  // 613
                                   7.549650939562645e-03,   // 614
                                   -1.197351101268412e-04,  // 615
                                   -1.299319532406617e-03,  // 616
                                   1.267874378151721e+01,   // 617
                                   -1.072385508837469e+00,  // 618
                                   -1.857416658618896e-04,  // 619
                                   -7.417927495583020e-02,  // 620
                                   9.598853094854696e-01,   // 621
                                   -1.113461560016485e-01,  // 622
                                   9.144175443900849e-04,   // 623
                                   -2.864692653324163e+01,  // 624
                                   2.827693255979795e-06,   // 625
                                   2.600466204561204e-03,   // 626
                                   1.389764862950828e+01,   // 627
                                   1.104475102963750e+00,   // 628
                                   -1.482153166484174e-04,  // 629
                                   1.060424297493825e-02,   // 630
                                   -3.004470436775701e-03,  // 631
                                   4.538833187527216e-05,   // 632
                                   1.920967915087813e-03,   // 633
                                   -6.041909503492435e-03,  // 634
                                   6.407295967755060e-03,   // 635
                                   -8.042433071709556e+00,  // 636
                                   3.265445840061734e-01,   // 637
                                   8.694412043643969e-03,   // 638
                                   2.610928567298836e+02,   // 639
                                   -2.021503385105576e-01,  // 640
                                   5.908989890942105e-02,   // 641
                                   1.337391315847855e-06,   // 642
                                   -1.386525332486171e-06,  // 643
                                   2.922373457919378e-03,   // 644
                                   -7.768006133765976e-03,  // 645
                                   1.448499460224677e-01,   // 646
                                   5.943032344922410e+00,   // 647
                                   1.959756956051152e-05,   // 648
                                   -7.529959489889410e+01,  // 649
                                   -9.881880346087028e-03,  // 650
                                   1.338269471325908e-05,   // 651
                                   2.233731949826486e-04,   // 652
                                   -2.800517849703327e-03,  // 653
                                   -1.087789596282207e+01,  // 654
                                   3.377352753209394e-01,   // 655
                                   -1.487342228260711e+01,  // 656
                                   -2.506511092985055e-05,  // 657
                                   1.937420570625932e-01,   // 658
                                   -1.473806410525659e+00,  // 659
                                   -8.373563136722496e-01,  // 660
                                   1.600005903267100e-04,   // 661
                                   -6.947734625352213e-02,  // 662
                                   7.392768847693867e+01,   // 663
                                   2.763904652505512e+02,   // 664
                                   1.112331229001417e-02,   // 665
                                   5.787724148766250e-06,   // 666
                                   -1.137630625956819e+00,  // 667
                                   -3.723533269410438e+02,  // 668
                                   -1.034600291040787e-04,  // 669
                                   1.666737671579567e+00,   // 670
                                   -4.515905105274747e-03,  // 671
                                   -9.293546270110044e+00,  // 672
                                   4.283100869588802e-06,   // 673
                                   -2.107841139866440e-04,  // 674
                                   9.071109685978960e-06,   // 675
                                   -5.289450242202802e+00,  // 676
                                   4.230308575083142e-01,   // 677
                                   6.555357343836958e-03,   // 678
                                   5.124638937905798e+02,   // 679
                                   6.466721430496499e-03,   // 680
                                   4.716564622069164e-03,   // 681
                                   4.061373743213756e-05,   // 682
                                   7.213884775927490e+00,   // 683
                                   -2.985023122072370e+02,  // 684
                                   4.944960563411140e-01,   // 685
                                   -4.746590108698331e+01,  // 686
                                   1.632451273801808e+00,   // 687
                                   -6.513368398724351e-04,  // 688
                                   -2.265368296652629e-01,  // 689
                                   -1.202151208026940e-02,  // 690
                                   8.629068193664644e-02,   // 691
                                   1.063783344589512e-04,   // 692
                                   -2.228843559643640e-04,  // 693
                                   2.282104788109029e-02,   // 694
                                   -1.177492885575739e-06,  // 695
                                   -9.104068046455305e+00,  // 696
                                   -4.947362588015695e-03,  // 697
                                   -2.164061191623048e-03,  // 698
                                   -1.342101373377292e+00,  // 699
                                   3.873072633605477e-01,   // 700
                                   8.305231364508270e-02,   // 701
                                   -5.037661082474296e-03,  // 702
                                   -1.121534874639331e-02,  // 703
                                   2.344996632820546e-03,   // 704
                                   1.333927571831253e+00,   // 705
                                   1.380984376674686e-04,   // 706
                                   -5.840893093887870e-02,  // 707
                                   -2.549502662055716e-04,  // 708
                                   2.961102662607111e-04,   // 709
                                   -2.536161309443987e-07,  // 710
                                   2.595784358671770e+00,   // 711
                                   -1.072381377609360e-01,  // 712
                                   -2.275572971256042e+01,  // 713
                                   -1.307541950512220e-01,  // 714
                                   -5.493487283811080e-01,  // 715
                                   7.973117285699997e-01,   // 716
                                   1.505694489331968e-02,   // 717
                                   6.729676431145117e-01,   // 718
                                   -5.061611064000457e+00,  // 719
                                   3.607761738646342e+00,   // 720
                                   1.579712699958506e-01,   // 721
                                   -3.470303563868018e-02,  // 722
                                   9.217842486516882e-02,   // 723
                                   -1.538185778528344e-05,  // 724
                                   -4.910066166627518e+00,  // 725
                                   5.599002147979984e+01,   // 726
                                   -1.724149015722695e-06,  // 727
                                   1.672627034567852e+00,   // 728
                                   -1.289319963140037e+00,  // 729
                                   -6.278245297247932e-02,  // 730
                                   -2.917953122938174e-02,  // 731
                                   -4.269986234783765e-01,  // 732
                                   -1.290789482123250e-01,  // 733
                                   1.624548272120024e+01,   // 734
                                   -3.413115561113293e+00,  // 735
                                   -1.414757706967025e+00,  // 736
                                   -6.023546223013450e-06,  // 737
                                   4.076542759233928e-01,   // 738
                                   1.385068257817988e-02,   // 739
                                   2.855862529149161e+01,   // 740
                                   -1.234875728136895e+00,  // 741
                                   5.449438629335457e+00,   // 742
                                   9.749796357755171e-03,   // 743
                                   1.668820306792762e+00,   // 744
                                   -2.976335608153407e-02,  // 745
                                   3.949079034475093e-02,   // 746
                                   2.194880555327119e-02,   // 747
                                   2.703257602369022e-03,   // 748
                                   2.534160129373549e-01,   // 749
                                   -6.276173943283802e+00,  // 750
                                   2.700225863663154e-04,   // 751
                                   -5.244477470738375e-01,  // 752
                                   4.767653275049414e+00,   // 753
                                   5.149083691358964e+00,   // 754
                                   2.050791484474550e+00,   // 755
                                   -1.229009995894582e-06,  // 756
                                   -3.374060514569928e-03,  // 757
                                   2.263982616211468e+00,   // 758
                                   3.567769989590359e-03,   // 759
                                   9.063595853083375e+01,   // 760
                                   3.830100247447090e-05,   // 761
                                   -7.646700218711925e+01,  // 762
                                   1.106187399745372e-03,   // 763
                                   -1.399947107398346e+01,  // 764
                                   1.290514221772747e-04,   // 765
                                   -4.317167882515283e-02,  // 766
                                   -2.835478186951369e-05,  // 767
                                   5.143658454660318e+00,   // 768
                                   -2.568275901707407e+01,  // 769
                                   -9.136145676639440e+01,  // 770
                                   4.600750235759046e-01,   // 771
                                   5.515154375283246e-02,   // 772
                                   1.738137986981519e-03,   // 773
                                   -3.136425814405986e-01,  // 774
                                   -1.923675833227878e+00,  // 775
                                   -7.485066486808091e-06,  // 776
                                   -4.789261300211181e+02,  // 777
                                   -7.906946165620906e+01,  // 778
                                   4.347249097984742e+02,   // 779
                                   -2.981845654419600e-02,  // 780
                                   6.277788432188659e-04,   // 781
                                   4.469948321818008e+01,   // 782
                                   -2.135004130105280e-05,  // 783
                                   1.247935778828086e-05,   // 784
                                   3.495361792833036e-01,   // 785
                                   -2.694074217753040e-01,  // 786
                                   -3.661784009385955e+00,  // 787
                                   -1.778454035174456e-03,  // 788
                                   1.121636552879032e-04,   // 789
                                   -1.125820280029701e-04,  // 790
                                   2.175786511174196e-04,   // 791
                                   -2.046610371792805e+00,  // 792
                                   2.660702563517253e-02,   // 793
                                   -1.498873227882019e-05,  // 794
                                   1.504419862591747e-01,   // 795
                                   4.006514794086209e-01,   // 796
                                   4.702570346656390e-05,   // 797
                                   7.944859904220653e-07,   // 798
                                   -1.468307785357535e+00,  // 799
                                   2.212012092543674e+00,   // 800
                                   6.954683644630590e+00,   // 801
                                   -3.727127245299785e-03,  // 802
                                   -3.160298465851620e+01,  // 803
                                   1.856387142681324e-01,   // 804
                                   -2.434518182643133e-01,  // 805
                                   -6.426406297244821e-04,  // 806
                                   -1.744522150196061e-04,  // 807
                                   -1.270513959608203e-03,  // 808
                                   3.320074671933048e+00,   // 809
                                   -2.946810424136282e-04,  // 810
                                   -3.064857452284930e+02,  // 811
                                   5.704719732656163e-01,   // 812
                                   -2.405713429618641e-06,  // 813
                                   -2.204919988634121e-06,  // 814
                                   6.071744625948750e-01,   // 815
                                   -1.881460028879492e-04,  // 816
                                   -2.505920507995312e-02,  // 817
                                   -5.084630248031251e-04,  // 818
                                   1.733914321458205e-03,   // 819
                                   3.154089359820176e+00,   // 820
                                   4.467382199929345e-03,   // 821
                                   -5.750085166085790e+00,  // 822
                                   -3.499585909010529e-05,  // 823
                                   5.708451839244659e-02,   // 824
                                   -1.564330505207665e+00,  // 825
                                   -8.352103788685879e+01,  // 826
                                   -3.130258995225760e-01,  // 827
                                   -4.557154404479185e-02,  // 828
                                   7.946546624061373e-03,   // 829
                                   6.434153482344831e+01,   // 830
                                   -1.008634016863438e-07,  // 831
                                   3.399331630618103e+00,   // 832
                                   5.223006892156911e-01,   // 833
                                   -5.162855960903745e-01,  // 834
                                   1.913698001846591e-06,   // 835
                                   3.442549685510289e+02,   // 836
                                   1.831467800141669e-04,   // 837
                                   9.205682716579578e-03,   // 838
                                   2.938643659026946e-01,   // 839
                                   -1.011251822697732e-01,  // 840
                                   -1.727413085345257e+01,  // 841
                                   4.948730462803781e-01,   // 842
                                   1.714136043612386e+02,   // 843
                                   4.540285225487115e+00,   // 844
                                   -8.818529886448348e-08,  // 845
                                   1.976298451321798e+02,   // 846
                                   8.728826417371929e-03,   // 847
                                   -1.974382885067633e+00,  // 848
                                   3.658564084004012e+00,   // 849
                                   -5.059847349963795e-03,  // 850
                                   -5.603103982323297e-02,  // 851
                                   -2.464830586448114e-01,  // 852
                                   -9.821134066199927e-01,  // 853
                                   -1.041640352631587e+02,  // 854
                                   -4.652031922123942e-07,  // 855
                                   -8.592504168963743e-05,  // 856
                                   3.251247091666103e+00,   // 857
                                   -1.613418574127261e-03,  // 858
                                   7.102717475452442e+01,   // 859
                                   -2.512683721072080e+00,  // 860
                                   2.269657285787001e-01,   // 861
                                   -1.973065699448463e+01,  // 862
                                   4.044117862129224e-03,   // 863
                                   5.040200367811742e+00,   // 864
                                   1.323728008635128e-03,   // 865
                                   -3.878433992778161e-02,  // 866
                                   2.332479012164918e+02,   // 867
                                   -7.656922572043767e-05,  // 868
                                   -2.631681260427753e+01,  // 869
                                   3.464268750991740e-03,   // 870
                                   2.581419115665996e-03,   // 871
                                   -2.843097854386598e+01,  // 872
                                   3.875213999672183e+00,   // 873
                                   1.010174278149341e-01,   // 874
                                   2.368650062786955e+01,   // 875
                                   1.187679324144617e-03,   // 876
                                   1.340241142652984e+00,   // 877
                                   1.332989272800486e-04,   // 878
                                   -2.034300265398554e-01,  // 879
                                   1.630183290884945e+02,   // 880
                                   2.602398230966954e+00,   // 881
                                   1.811620294612432e-01,   // 882
                                   -8.694453997093832e-03,  // 883
                                   -1.473947102316658e-03,  // 884
                                   -4.894104683807970e-02,  // 885
                                   -5.710091324470823e-01,  // 886
                                   -5.707588581478942e-02,  // 887
                                   1.781024868191055e+02,   // 888
                                   -1.271384483518931e-06,  // 889
                                   -3.184593720204684e-07,  // 890
                                   2.583658553751645e-04,   // 891
                                   -5.612942930864236e-03,  // 892
                                   3.573665722874343e+02,   // 893
                                   1.113312035299232e-01,   // 894
                                   -1.473319091365888e-02,  // 895
                                   5.120836859308599e-02,   // 896
                                   -2.440574517549679e-02,  // 897
                                   -1.584819660021867e+02,  // 898
                                   -1.019771425755970e+00,  // 899
                                   -1.331914878959130e-02,  // 900
                                   2.490327943588066e+01,   // 901
                                   -1.099113542985273e-05,  // 902
                                   1.461142182966858e-01,   // 903
                                   7.143942044705365e+00,   // 904
                                   4.122202042767271e-02,   // 905
                                   -1.073942219672822e+00,  // 906
                                   -4.810833351238202e-01,  // 907
                                   -2.487020576438238e-04,  // 908
                                   -3.860326305385521e+00,  // 909
                                   8.020894286868213e-04,   // 910
                                   3.131631389088053e-03,   // 911
                                   -9.769288632390484e+00,  // 912
                                   1.384004314176717e+01,   // 913
                                   4.865800232031077e+02,   // 914
                                   2.123054443350169e-06,   // 915
                                   -1.424845873545981e-01,  // 916
                                   -2.545174828740804e-07,  // 917
                                   2.096592112301541e+01,   // 918
                                   1.263795374423910e-02,   // 919
                                   -1.516348148696526e-05,  // 920
                                   5.670496538900013e-02,   // 921
                                   4.550504582823915e+00,   // 922
                                   -4.973809004734024e+00,  // 923
                                   2.944841863111308e+01,   // 924
                                   1.396396820318551e-01,   // 925
                                   -7.908348922398345e+00,  // 926
                                   -5.503933962861118e-02,  // 927
                                   -1.005287446550388e+00,  // 928
                                   -8.982341438149839e-06,  // 929
                                   -1.601516334675275e-02,  // 930
                                   8.225940415385207e+00,   // 931
                                   -2.442661583970928e-05,  // 932
                                   -7.659312443616395e-07,  // 933
                                   -1.713317705409222e-01,  // 934
                                   4.641863927378143e+02,   // 935
                                   -1.037143741868699e+00,  // 936
                                   3.340649058761744e-04,   // 937
                                   6.122350822356268e-01,   // 938
                                   1.763270179215438e-02,   // 939
                                   -1.040410652120869e+01,  // 940
                                   -1.705569054833241e+00,  // 941
                                   -1.698474977724325e+00,  // 942
                                   1.314195942743012e-02,   // 943
                                   -3.931872382376753e-04,  // 944
                                   -3.605115775914639e+02,  // 945
                                   -6.684687338843589e-04,  // 946
                                   -1.711912996490854e-03,  // 947
                                   -2.425919956553731e+00,  // 948
                                   -4.397214150686802e-03,  // 949
                                   2.203451722137785e-01,   // 950
                                   1.921503559393261e-04,   // 951
                                   -7.377185393869496e+01,  // 952
                                   -3.318564602281291e+00,  // 953
                                   1.372952366982986e-01,   // 954
                                   -4.682490640974286e-02,  // 955
                                   -8.585405719649255e+00,  // 956
                                   -7.916926426028262e+01,  // 957
                                   6.842841837667214e+00,   // 958
                                   -1.555767785510787e-04,  // 959
                                   1.066674256500291e+00,   // 960
                                   -8.128989729946743e+01,  // 961
                                   8.666582025254606e+02,   // 962
                                   3.263955862119750e+00,   // 963
                                   1.116108359906232e-05,   // 964
                                   3.314756344846971e-01,   // 965
                                   9.871491699884346e-01,   // 966
                                   1.653700549308369e+00,   // 967
                                   1.625196132350036e-02,   // 968
                                   -1.368582180349552e-04,  // 969
                                   2.982241757342457e+02,   // 970
                                   -2.692732738800014e-02,  // 971
                                   1.212640657651240e-02,   // 972
                                   9.232355428893594e-04,   // 973
                                   -1.222315568701405e-01,  // 974
                                   1.964242677519647e+01,   // 975
                                   2.530011401730263e+01,   // 976
                                   -2.025058947658918e-05,  // 977
                                   3.151858016990782e+02,   // 978
                                   -2.899464308497044e-03,  // 979
                                   3.191707588835817e-02,   // 980
                                   -1.114116182131097e+01,  // 981
                                   1.000150656975933e-05,   // 982
                                   -1.769806992752926e+02,  // 983
                                   1.315329534944367e+02,   // 984
                                   3.737962862007718e+01,   // 985
                                   -1.758418532145222e-04,  // 986
                                   8.269216492158980e-05,   // 987
                                   -3.141422727600816e+00,  // 988
                                   4.588030802222731e-02,   // 989
                                   -1.257089376854685e-01,  // 990
                                   2.153312846574052e-05,   // 991
                                   -6.663038191753395e+00,  // 992
                                   -2.094872159681336e+00,  // 993
                                   9.497717734722357e-01,   // 994
                                   -6.025263004772613e+01,  // 995
                                   -2.400056343274432e-05,  // 996
                                   -1.617917695673061e+01,  // 997
                                   -1.505649404065308e-03,  // 998
                                   -8.793957345315305e+01,  // 999
                                   -2.404767425859607e-03,  // 1000
                                   3.611712104337255e-04,   // 1001
                                   7.085675188674982e-04,   // 1002
                                   9.517367956718083e-05,   // 1003
                                   -4.498170143798781e+02,  // 1004
                                   -3.871247109439457e-01,  // 1005
                                   6.003289911080240e-04,   // 1006
                                   -4.007512401857818e+00,  // 1007
                                   2.804552252507493e+01,   // 1008
                                   3.882691834786128e+00,   // 1009
                                   -4.280139487254557e+00,  // 1010
                                   2.215187147246688e+01,   // 1011
                                   2.007680370800015e-03,   // 1012
                                   -1.196367168096373e+01,  // 1013
                                   3.196881573355839e-02,   // 1014
                                   2.990206229343475e+01};  // 1015

        m_r3i = 0.000000000000000e+00;        // A
        m_r3f = 4.500000000000000e+00;        // A
        m_kHH_intra = 4.935811847841025e+00;  // A^(-1)
        m_kOH_intra = 2.571848425019958e-01;  // A^(-1)
        m_kHH = 6.641539139948633e-01;        // A^(-1)
        m_kOH = 8.262323078009421e-01;        // A^(-1)
        m_kOO = 4.657013878889721e-01;        // A^(-1)
        m_kXO = 3.504015211699528e-01;        // A^(-1)
        m_kXH = 1.973569678997551e-02;        // A^(-1)
        m_dHH_intra = 2.117837104002506e+00;  // A
        m_dOH_intra = 7.619101258901348e-01;  // A
        m_dHH = 1.127561151275100e+01;        // A
        m_dOH = 6.835848274082730e+00;        // A
        m_dOO = 7.037153931329832e+00;        // A
        m_dXO = 1.112494982897995e+01;        // A
        m_dXH = 6.299823660916497e-01;        // A

        std::copy(the_poly, the_poly + ncoeffs, m_coeffs);

    } else if (m3 == "cs+") {
        const double the_poly[] = {-1.907671721947504e+01,   // 0
                                   -6.148224358427421e+00,   // 1
                                   3.537515593740038e+00,    // 2
                                   1.614469125451684e+02,    // 3
                                   1.176615831222012e+01,    // 4
                                   -1.204591691392065e+02,   // 5
                                   3.772944492328747e+01,    // 6
                                   -5.177977740754637e+00,   // 7
                                   1.380804245757920e+02,    // 8
                                   -1.012570178335393e+02,   // 9
                                   3.555284450059387e+02,    // 10
                                   -1.234933001318550e+02,   // 11
                                   2.610690842640349e+02,    // 12
                                   -1.125104307194856e-04,   // 13
                                   2.140592325754977e+02,    // 14
                                   -7.029069234382160e-01,   // 15
                                   1.854880368565840e+02,    // 16
                                   1.038219016103246e+02,    // 17
                                   -2.450732100427049e+00,   // 18
                                   5.438172729826776e+01,    // 19
                                   1.848947872965876e+00,    // 20
                                   -2.498019364093455e+02,   // 21
                                   -4.056629708552587e-03,   // 22
                                   -2.416514918375451e-03,   // 23
                                   -2.173590623515206e+00,   // 24
                                   -7.171004074110704e-01,   // 25
                                   -3.337965778730341e-01,   // 26
                                   6.309422849889904e-01,    // 27
                                   3.928268707881441e+01,    // 28
                                   2.428235886835362e-01,    // 29
                                   2.072131524097046e+01,    // 30
                                   2.653333110116160e-01,    // 31
                                   -5.517225903093236e+00,   // 32
                                   3.364160234375480e-01,    // 33
                                   6.501363214138814e+01,    // 34
                                   -8.455824287437207e-02,   // 35
                                   6.660085159764610e+01,    // 36
                                   -2.163082203812413e-04,   // 37
                                   7.476216686181053e-02,    // 38
                                   -5.699544731997848e+00,   // 39
                                   -1.538361246961896e+02,   // 40
                                   1.933335531569068e-02,    // 41
                                   2.296276791349662e-02,    // 42
                                   -1.061110643730067e+01,   // 43
                                   9.557427034534541e+01,    // 44
                                   1.333150172666767e+01,    // 45
                                   -1.428841216093072e-02,   // 46
                                   -1.508385863979206e+02,   // 47
                                   1.196836291967457e+01,    // 48
                                   5.529292029797814e+01,    // 49
                                   -5.936083587515043e+02,   // 50
                                   3.008783944274519e-02,    // 51
                                   2.321551621212172e+02,    // 52
                                   1.867408627228749e-03,    // 53
                                   -6.051332533247454e-04,   // 54
                                   7.834400947443293e-04,    // 55
                                   -9.638248229126457e+01,   // 56
                                   -2.350837987532366e-01,   // 57
                                   2.375796367578129e+02,    // 58
                                   -4.569790382950433e+01,   // 59
                                   -2.921801927740461e+02,   // 60
                                   -5.156244213449034e-03,   // 61
                                   -2.629100506776363e-01,   // 62
                                   -2.324444848907560e+00,   // 63
                                   -1.164367543071128e+02,   // 64
                                   3.290417515797008e-01,    // 65
                                   -2.758900290108592e+02,   // 66
                                   9.269189395039959e-04,    // 67
                                   2.146283585310898e+01,    // 68
                                   2.884088327205222e+02,    // 69
                                   -1.166471134604464e+01,   // 70
                                   -7.370617524447785e+00,   // 71
                                   3.581275035744231e+02,    // 72
                                   1.109348737817888e-01,    // 73
                                   -1.669412824333235e+02,   // 74
                                   9.117325261122372e-01,    // 75
                                   -1.026594903134509e+02,   // 76
                                   1.875020508243441e+00,    // 77
                                   -2.354375202913431e+00,   // 78
                                   2.791115453644455e-01,    // 79
                                   8.234298823363858e+01,    // 80
                                   2.883618578079405e-01,    // 81
                                   -3.493115856179533e+00,   // 82
                                   -6.394252198056419e-02,   // 83
                                   -3.252136920471837e+00,   // 84
                                   8.736118130426300e+00,    // 85
                                   8.413914235649314e-02,    // 86
                                   -5.783413467320754e+00,   // 87
                                   1.159449356937010e+00,    // 88
                                   2.747224945779190e-01,    // 89
                                   -1.054375207318352e+02,   // 90
                                   -6.223202401747776e+01,   // 91
                                   8.239187801907972e+00,    // 92
                                   1.997208144637740e+01,    // 93
                                   -8.763846517155385e+01,   // 94
                                   -3.021250220782509e+00,   // 95
                                   6.839237886985156e+00,    // 96
                                   1.090805075642684e+02,    // 97
                                   -1.193552759327678e+00,   // 98
                                   -2.998962560246538e+02,   // 99
                                   -5.386222958155349e+00,   // 100
                                   1.113800000897888e+02,    // 101
                                   -2.742734088796666e+01,   // 102
                                   1.028070780453401e-02,    // 103
                                   -9.349998056932901e+01,   // 104
                                   -5.984230296731994e+01,   // 105
                                   -1.096056800311213e+01,   // 106
                                   8.441141860428033e+01,    // 107
                                   -1.585303689147542e+00,   // 108
                                   -5.180481947598926e+01,   // 109
                                   -3.216331216552098e+02,   // 110
                                   8.846090171072389e-02,    // 111
                                   1.151242910377034e+02,    // 112
                                   -5.305463262001847e+01,   // 113
                                   1.850540564975088e+00,    // 114
                                   5.098344855132107e+02,    // 115
                                   9.244539614694584e+01,    // 116
                                   -2.565520431805773e+01,   // 117
                                   -3.504153358841384e+00,   // 118
                                   -4.425597753852555e+00,   // 119
                                   3.054076789031873e-01,    // 120
                                   -2.460649628823790e+02,   // 121
                                   -1.958390226590032e-01,   // 122
                                   -3.848084806910381e+01,   // 123
                                   1.619175183520839e+01,    // 124
                                   -2.411227265196510e+02,   // 125
                                   -7.281960967197051e+01,   // 126
                                   -1.110555387917483e-02,   // 127
                                   -5.779653193224109e+02,   // 128
                                   -9.322753543259143e+00,   // 129
                                   -5.409788734689256e-01,   // 130
                                   -4.017997711031625e+01,   // 131
                                   -1.461022263635368e-02,   // 132
                                   -9.981761840866712e+01,   // 133
                                   4.187346231092307e+01,    // 134
                                   1.057766280492746e+02,    // 135
                                   1.962022574570613e+01,    // 136
                                   3.348226911860046e+01,    // 137
                                   2.438757503044221e+02,    // 138
                                   -4.644469411752706e+00,   // 139
                                   4.365430927394812e+01,    // 140
                                   -5.694242307053954e+02,   // 141
                                   -1.951650294749731e+02,   // 142
                                   -1.356974703192258e+02,   // 143
                                   9.477870204051004e+01,    // 144
                                   -3.182199857053374e+02,   // 145
                                   1.784074720775991e+02,    // 146
                                   4.096732408016401e+02,    // 147
                                   9.321870427292617e+01,    // 148
                                   -1.491186366906122e+01,   // 149
                                   4.126981214902051e+00,    // 150
                                   -1.381085266872713e+02,   // 151
                                   -3.439074189516039e-01,   // 152
                                   3.217939332465468e+02,    // 153
                                   3.543020881202159e-02,    // 154
                                   2.902894596999981e+02,    // 155
                                   -1.832569805208164e-03,   // 156
                                   2.936631634645177e+02,    // 157
                                   1.812641792881587e+00,    // 158
                                   -1.587562612978448e+02,   // 159
                                   -6.148992133660557e+01,   // 160
                                   4.344339349551747e+00,    // 161
                                   -5.102762665275478e+02,   // 162
                                   4.876160670415592e+01,    // 163
                                   -7.504530818778397e-04,   // 164
                                   2.617620298915465e-08,    // 165
                                   5.803597983999364e-07,    // 166
                                   -1.276914360907623e+01,   // 167
                                   5.519062360974356e+00,    // 168
                                   2.636964266633379e-02,    // 169
                                   3.579585532801543e-03,    // 170
                                   1.929535716301209e-02,    // 171
                                   -9.048384614566997e-02,   // 172
                                   -4.971788868504554e+00,   // 173
                                   2.573111364000239e-04,    // 174
                                   -7.298480754473334e-03,   // 175
                                   4.489260908609247e-03,    // 176
                                   1.600595019595895e+01,    // 177
                                   1.041236360555380e-03,    // 178
                                   -1.055014711096996e+02,   // 179
                                   -1.366372780557969e+00,   // 180
                                   1.219995762673546e+01,    // 181
                                   2.077811307608417e-03,    // 182
                                   -1.502370269481831e+02,   // 183
                                   -1.330672547370203e+01,   // 184
                                   5.492288932600313e-05,    // 185
                                   1.549776439476490e+02,    // 186
                                   1.949520921331858e+00,    // 187
                                   4.249662249261280e-03,    // 188
                                   7.206743913512152e-04,    // 189
                                   1.696128461562315e+02,    // 190
                                   6.957372024311534e-01,    // 191
                                   3.876631247627382e+00,    // 192
                                   -8.948783953262597e+00,   // 193
                                   6.972083100775109e-08,    // 194
                                   -7.126142328729923e-02,   // 195
                                   -2.544198521369594e-04,   // 196
                                   6.737844265693898e-03,    // 197
                                   -2.048269181998721e-06,   // 198
                                   -1.986551028232957e+00,   // 199
                                   -2.826218077816950e-04,   // 200
                                   6.823967238668895e+01,    // 201
                                   -4.323457209077511e-03,   // 202
                                   -1.490078238331892e+02,   // 203
                                   -6.481756847831025e-06,   // 204
                                   -3.033274570305339e-06,   // 205
                                   -8.591876598712703e-02,   // 206
                                   -1.072480459421354e-01,   // 207
                                   1.276912825346744e+02,    // 208
                                   -3.125030534179530e-03,   // 209
                                   -2.046111629333454e-01,   // 210
                                   -3.233661995924054e-01,   // 211
                                   1.049430296547894e+00,    // 212
                                   -6.790935204567634e-03,   // 213
                                   1.589190406760696e+01,    // 214
                                   5.733805571432646e+01,    // 215
                                   -7.470269241275449e-03,   // 216
                                   2.307170092555613e-01,    // 217
                                   8.711176067865420e+00,    // 218
                                   -3.807458894744284e-02,   // 219
                                   1.827881466682828e-02,    // 220
                                   3.611364593358104e+01,    // 221
                                   2.020180749215158e+02,    // 222
                                   7.997203086672243e+00,    // 223
                                   -8.651417218195954e+01,   // 224
                                   1.985313858543096e+00,    // 225
                                   3.883608642915156e-01,    // 226
                                   -1.049912022351133e-03,   // 227
                                   -1.374433517352276e-01,   // 228
                                   -1.702561886168923e+02,   // 229
                                   -3.826193314902029e+01,   // 230
                                   7.804496678572524e+01,    // 231
                                   -3.397861726556575e+01,   // 232
                                   -3.313269383969341e-01,   // 233
                                   -1.209253938113901e+02,   // 234
                                   -2.003193981315307e+02,   // 235
                                   1.572417516905931e+01,    // 236
                                   -3.551352946881728e-01,   // 237
                                   -2.067549412666249e+02,   // 238
                                   -3.490461539625503e+01,   // 239
                                   1.376544334841610e-02,    // 240
                                   -4.547541814732001e+01,   // 241
                                   -3.696417127362198e-02,   // 242
                                   -1.848509486834336e+01,   // 243
                                   8.252401479658840e-03,    // 244
                                   8.198245530151039e-01,    // 245
                                   4.950755089269748e+00,    // 246
                                   -1.028504605095593e-06,   // 247
                                   -7.682896411048240e-02,   // 248
                                   3.696532653715013e-03,    // 249
                                   -5.521130635918053e-05,   // 250
                                   5.488173070600153e-04,    // 251
                                   -1.747178102230434e-02,   // 252
                                   -6.732761489991552e+01,   // 253
                                   1.022193086652313e+01,    // 254
                                   -1.283624419027607e+01,   // 255
                                   1.175291910630542e+00,    // 256
                                   7.902830351944786e+00,    // 257
                                   1.572038470969280e-01,    // 258
                                   8.858783770582687e-01,    // 259
                                   4.734471505311221e+00,    // 260
                                   -5.259613468534416e+01,   // 261
                                   6.753722703461071e+00,    // 262
                                   -2.523194717892452e-05,   // 263
                                   -3.389412501285361e+00,   // 264
                                   -1.885502756404619e-06,   // 265
                                   -3.581253808267805e+00,   // 266
                                   3.077158332163658e-04,    // 267
                                   -2.938090514926904e-03,   // 268
                                   -9.206890750321558e-01,   // 269
                                   5.344838530710857e-06,    // 270
                                   -1.963451165907529e-01,   // 271
                                   -2.158351812141226e-03,   // 272
                                   -7.831135317070979e+00,   // 273
                                   1.404551310765176e-03,    // 274
                                   -2.918741355493519e+01,   // 275
                                   -3.807956415046643e-01,   // 276
                                   1.277494920859680e-06,    // 277
                                   -9.586046816667537e+00,   // 278
                                   1.339411315203911e+02,    // 279
                                   6.753288780811809e+00,    // 280
                                   -8.606041976553310e-03,   // 281
                                   1.188341423169924e+00,    // 282
                                   9.503833509820384e-01,    // 283
                                   3.840103302363148e+00,    // 284
                                   6.158980381178893e-01,    // 285
                                   -1.753348290385438e-02,   // 286
                                   8.347831420696303e-02,    // 287
                                   -5.447075441216431e+00,   // 288
                                   8.417800843327704e-01,    // 289
                                   -1.952245532410614e+01,   // 290
                                   4.500389621813509e-01,    // 291
                                   6.760950159690422e-06,    // 292
                                   5.949887850222978e-05,    // 293
                                   -2.505376696164261e-02,   // 294
                                   2.452997712326859e+01,    // 295
                                   -2.979315856499889e-02,   // 296
                                   -1.574202597419562e+01,   // 297
                                   1.910102179272422e-03,    // 298
                                   2.645759623575966e-05,    // 299
                                   2.864989502186573e-03,    // 300
                                   1.206988655128431e+01,    // 301
                                   -1.005459713876887e-01,   // 302
                                   -1.750399235318656e-01,   // 303
                                   -2.052911679713567e+01,   // 304
                                   5.079073885860931e-05,    // 305
                                   -5.896722793917458e-03,   // 306
                                   1.259067711485826e-03,    // 307
                                   -8.113997689718854e-01,   // 308
                                   1.480732743756102e+00,    // 309
                                   -1.034545515397449e-01,   // 310
                                   2.794513787024569e-05,    // 311
                                   -3.537052414161554e-03,   // 312
                                   -2.186940777233525e-01,   // 313
                                   -1.971843019050703e+01,   // 314
                                   -2.099818258565306e-03,   // 315
                                   7.323041220622445e-03,    // 316
                                   -1.638983399586726e-01,   // 317
                                   -1.054526144198674e+01,   // 318
                                   -3.441206422814293e-03,   // 319
                                   -2.352480192188235e+00,   // 320
                                   3.515993607860852e-08,    // 321
                                   -2.746285839466242e+00,   // 322
                                   -2.131646411114810e-01,   // 323
                                   -2.299495616522213e-01,   // 324
                                   -2.473973527712853e+01,   // 325
                                   1.462678964176602e+00,    // 326
                                   -3.491058423216502e+00,   // 327
                                   3.081185951013705e+00,    // 328
                                   2.621789171391050e-01,    // 329
                                   -3.384153360522108e+00,   // 330
                                   1.509559115438453e+01,    // 331
                                   1.606675518703843e-03,    // 332
                                   3.444591406785204e+01,    // 333
                                   5.125661608607205e-03,    // 334
                                   5.020427687415072e-01,    // 335
                                   1.317474558558763e-01,    // 336
                                   2.048798318197689e-01,    // 337
                                   9.412255669538118e-01,    // 338
                                   9.239520412408030e+00,    // 339
                                   -1.755662677132801e-01,   // 340
                                   1.076096187813396e+00,    // 341
                                   3.513235810938183e-02,    // 342
                                   6.236051222514594e-09,    // 343
                                   -5.456448165936779e+01,   // 344
                                   2.899268038956259e+00,    // 345
                                   3.898664284548310e-03,    // 346
                                   8.572292392860593e+00,    // 347
                                   -1.672256203961868e+00,   // 348
                                   -1.598707722143221e-02,   // 349
                                   6.201655244128477e-03,    // 350
                                   -4.258408661437396e+02,   // 351
                                   -1.236278378427483e+00,   // 352
                                   -3.769578476034204e-03,   // 353
                                   5.405897792102601e-08,    // 354
                                   1.377627139522883e-03,    // 355
                                   -4.722309812217437e-02,   // 356
                                   3.666544619128304e+01,    // 357
                                   3.459638263022515e-04,    // 358
                                   8.793357636501570e-03,    // 359
                                   -3.403009173878579e+01,   // 360
                                   -1.014208863463339e-05,   // 361
                                   -2.152503235756160e-03,   // 362
                                   -8.694836487087886e-02,   // 363
                                   -5.910677981232869e-04,   // 364
                                   3.483870583414925e-03,    // 365
                                   1.881381077511518e+01,    // 366
                                   3.029185395978836e+00,    // 367
                                   -1.837367947114708e-07,   // 368
                                   -1.026828114209893e+02,   // 369
                                   -5.599714783668756e-01,   // 370
                                   6.514955269201571e-05,    // 371
                                   -1.674215831138449e+01,   // 372
                                   -5.482858576918882e-01,   // 373
                                   1.320977079587404e+00,    // 374
                                   4.504916632896981e-04,    // 375
                                   2.498267148618295e-02,    // 376
                                   1.683891325246133e+00,    // 377
                                   -5.841971055890961e-01,   // 378
                                   5.613936958368907e+01,    // 379
                                   2.603776647424101e-06,    // 380
                                   -1.680517691242089e+02,   // 381
                                   -1.207506282857411e-04,   // 382
                                   6.418452904108318e-05,    // 383
                                   7.399132646873652e-06,    // 384
                                   7.058655170273000e-02,    // 385
                                   -1.033527086325965e-03,   // 386
                                   3.838746651121048e-01,    // 387
                                   -3.844553080072456e-06,   // 388
                                   -1.962229098604893e-02,   // 389
                                   3.929186970387613e-04,    // 390
                                   3.596792179221171e+00,    // 391
                                   9.010887627066852e+01,    // 392
                                   -2.010924341626757e+01,   // 393
                                   1.772011358616443e-04,    // 394
                                   -2.914273580066532e+00,   // 395
                                   -2.012676804554904e+00,   // 396
                                   -2.616570109888938e-01,   // 397
                                   1.924362196908723e+00,    // 398
                                   -3.484652669721972e-02,   // 399
                                   6.637086703209594e-01,    // 400
                                   4.624341135363622e+02,    // 401
                                   1.290215839026023e-01,    // 402
                                   -3.790054386937444e-03,   // 403
                                   -9.449242480077025e-03,   // 404
                                   -1.356965659441317e+00,   // 405
                                   -8.402292057024080e-01,   // 406
                                   -2.805289126523396e+01,   // 407
                                   6.286989292073057e-02,    // 408
                                   -1.540232894685083e+00,   // 409
                                   -1.255416366151507e-01,   // 410
                                   4.598978028965760e+01,    // 411
                                   6.970708231374356e+01,    // 412
                                   -1.714046992895291e-02,   // 413
                                   -1.473032941610136e-02,   // 414
                                   5.895914861890951e-05,    // 415
                                   1.175537074126443e+01,    // 416
                                   -2.098136758805247e-01,   // 417
                                   -1.691956787978761e+00,   // 418
                                   1.394174188835004e-02,    // 419
                                   1.469499625372615e+01,    // 420
                                   9.148383445112761e-01,    // 421
                                   8.227315096231294e-04,    // 422
                                   -6.715874841246756e-05,   // 423
                                   -1.310251923960042e+02,   // 424
                                   -4.153476271880241e-08,   // 425
                                   4.208722161535967e-01,    // 426
                                   -5.666244551682617e-01,   // 427
                                   2.357853692915797e-04,    // 428
                                   6.579807732876310e-02,    // 429
                                   -1.983616071565613e+00,   // 430
                                   8.101083279575022e+00,    // 431
                                   -1.890602521342979e+02,   // 432
                                   5.055113720537576e-02,    // 433
                                   -1.094285145752277e-01,   // 434
                                   -6.386891751189414e-02,   // 435
                                   1.930307332825591e+00,    // 436
                                   2.425137609814209e+01,    // 437
                                   -9.576017475689350e-01,   // 438
                                   -3.956296333721072e-08,   // 439
                                   1.602641754081891e-01,    // 440
                                   1.053106932808851e-05,    // 441
                                   -1.976316164176204e-01,   // 442
                                   -1.540725306048434e+01,   // 443
                                   2.882891491664891e-05,    // 444
                                   -1.351322229094751e-05,   // 445
                                   -1.201911373546031e-01,   // 446
                                   -4.801718180221861e-01,   // 447
                                   8.384885479552663e+00,    // 448
                                   1.339257010790005e+00,    // 449
                                   -3.352903173710219e-02,   // 450
                                   7.838346170020437e-01,    // 451
                                   5.866490532094107e-03,    // 452
                                   7.769435174905239e+01,    // 453
                                   -3.719718135658891e+01,   // 454
                                   -1.003041566883359e-01,   // 455
                                   2.084311281643946e-01,    // 456
                                   1.285106802331015e+01,    // 457
                                   6.947670744925684e+01,    // 458
                                   -5.911469767459914e-05,   // 459
                                   4.000109196190427e-03,    // 460
                                   -4.058933031461523e-05,   // 461
                                   -1.666073727709565e-05,   // 462
                                   1.380511192263990e-01,    // 463
                                   -1.986142933490850e+01,   // 464
                                   -2.492267240563427e+00,   // 465
                                   -1.967930338325234e+01,   // 466
                                   5.528561989056013e-02,    // 467
                                   -6.070209204514234e-01,   // 468
                                   1.015428301230429e-04,    // 469
                                   -2.730637282798486e+02,   // 470
                                   1.591102460373213e+01,    // 471
                                   4.635413682800129e+02,    // 472
                                   6.081416870504503e+01,    // 473
                                   -6.894103073316557e-02,   // 474
                                   -1.996303620523792e+00,   // 475
                                   1.655452131796424e+02,    // 476
                                   7.695247302364837e-05,    // 477
                                   6.130047596222447e-02,    // 478
                                   -8.405277341448745e+00,   // 479
                                   5.856046645279649e-06,    // 480
                                   6.336182885636158e+01,    // 481
                                   -4.619793929728298e-02,   // 482
                                   -1.269319832859733e-05,   // 483
                                   -2.004642468051637e-03,   // 484
                                   4.162972192950559e-01,    // 485
                                   4.811326940752109e-02,    // 486
                                   -3.317759891303597e-06,   // 487
                                   8.932940269768017e-01,    // 488
                                   -1.670835168233786e-02,   // 489
                                   1.127635409583808e+00,    // 490
                                   2.063035928213626e-01,    // 491
                                   -1.468531350904654e-03,   // 492
                                   2.238002315690701e+00,    // 493
                                   4.819161286132401e-05,    // 494
                                   8.176405426615165e-02,    // 495
                                   1.875827876098524e-04,    // 496
                                   -3.316383037630250e+02,   // 497
                                   -2.859961535115079e-08,   // 498
                                   4.588857606000934e-04,    // 499
                                   -4.230165760272270e+00,   // 500
                                   -1.170599951495352e+01,   // 501
                                   1.306206242827700e-06,    // 502
                                   -2.517824598273252e+00,   // 503
                                   -1.479248288445977e+02,   // 504
                                   -1.947726366089750e+01,   // 505
                                   9.590559002454860e-01,    // 506
                                   2.592542797728292e-03,    // 507
                                   1.052946535859001e-02,    // 508
                                   -5.174062319414635e-03,   // 509
                                   -2.062314499245399e-03,   // 510
                                   1.297125437645851e+01,    // 511
                                   -3.971267158146572e+01,   // 512
                                   3.562218882883596e-04,    // 513
                                   2.235733759488620e+00,    // 514
                                   3.217274502175058e-02,    // 515
                                   -2.898362675786895e+00,   // 516
                                   -3.019614562459450e-01,   // 517
                                   -4.280889766645850e-08,   // 518
                                   7.346667137704305e+00,    // 519
                                   -5.324840249974498e+01,   // 520
                                   1.085676236885781e-02,    // 521
                                   3.117414122327391e-04,    // 522
                                   -2.175798989778358e+00,   // 523
                                   -4.026025260125116e-01,   // 524
                                   -9.868641771680743e-03,   // 525
                                   6.206436189199610e-04,    // 526
                                   -4.282938002848373e+00,   // 527
                                   5.467228014525718e-01,    // 528
                                   2.263160528716506e+02,    // 529
                                   -4.999857199482323e+01,   // 530
                                   -8.332698201436050e+01,   // 531
                                   -2.421526665577786e-04,   // 532
                                   5.028679335241967e-05,    // 533
                                   -2.101642809952545e-02,   // 534
                                   8.214200019498804e+01,    // 535
                                   4.326996074660006e-01,    // 536
                                   2.904758412645259e-03,    // 537
                                   7.479181147237941e+00,    // 538
                                   -5.147786521012657e+01,   // 539
                                   5.198891935844653e+00,    // 540
                                   2.455561822523656e+02,    // 541
                                   -2.307860366275971e-08,   // 542
                                   -2.747610454528997e-04,   // 543
                                   -1.136158813398482e+02,   // 544
                                   5.621142137579267e+00,    // 545
                                   3.038806553428399e+00,    // 546
                                   4.051764240461147e+01,    // 547
                                   -6.789586361565281e+01,   // 548
                                   1.616615384781145e-05,    // 549
                                   -2.069170304511523e+02,   // 550
                                   7.429248688918501e-02,    // 551
                                   -9.385134830988652e+01,   // 552
                                   4.636595239432100e+01,    // 553
                                   7.191688545468064e-02,    // 554
                                   4.632777771469029e-04,    // 555
                                   4.921049298588623e-03,    // 556
                                   4.214565435899298e+00,    // 557
                                   3.111791277542640e-05,    // 558
                                   -1.673301105654852e-01,   // 559
                                   6.693040613246917e-03,    // 560
                                   4.591606385034447e+00,    // 561
                                   -1.609015976552826e-01,   // 562
                                   2.734804631915226e-04,    // 563
                                   1.804698450493112e-08,    // 564
                                   5.153696825727551e-03,    // 565
                                   4.135077251013717e-01,    // 566
                                   9.188360925497706e+00,    // 567
                                   -8.498675793758585e-05,   // 568
                                   1.376188769947955e+01,    // 569
                                   -9.026817017048064e-04,   // 570
                                   5.125257393993202e-05,    // 571
                                   5.031844683908155e+01,    // 572
                                   -1.949541108807865e-03,   // 573
                                   5.148804836276739e-02,    // 574
                                   2.694420268466646e-06,    // 575
                                   1.959709345931627e+00,    // 576
                                   -1.335182859533077e+01,   // 577
                                   8.495049073254239e+01,    // 578
                                   1.753582169041223e+02,    // 579
                                   -2.545301387899869e+01,   // 580
                                   -2.729952877273727e+01,   // 581
                                   -6.530964032993796e-01,   // 582
                                   1.836240406111189e+02,    // 583
                                   -3.839266479006284e+00,   // 584
                                   -8.938201982412783e-05,   // 585
                                   -1.221291922395395e-01,   // 586
                                   1.161026966424248e+01,    // 587
                                   -2.045216438242797e-01,   // 588
                                   3.625310970037739e-09,    // 589
                                   -3.927671322425143e-01,   // 590
                                   9.206810410816973e+01,    // 591
                                   1.124998580842385e+02,    // 592
                                   -2.999004233546943e-04,   // 593
                                   1.781393751467686e+00,    // 594
                                   -2.556342181366451e-06,   // 595
                                   2.010154063693762e+00,    // 596
                                   -3.527206249680870e-05,   // 597
                                   -2.461799113365731e+00,   // 598
                                   3.160788807926236e-01,    // 599
                                   -7.438063248722567e+01,   // 600
                                   4.714953416317373e-05,    // 601
                                   -6.949978019341265e-04,   // 602
                                   -3.376951049497345e-03,   // 603
                                   -1.868560496115562e+02,   // 604
                                   8.886137873746952e+01,    // 605
                                   -5.524905424435330e+01,   // 606
                                   2.973280384507566e-01,    // 607
                                   7.765541261389654e-04,    // 608
                                   -7.840843531140867e+00,   // 609
                                   4.156571641531328e-04,    // 610
                                   8.095549189353132e-04,    // 611
                                   7.857776928619777e+00,    // 612
                                   -1.871403959617863e-04,   // 613
                                   -2.474365661743386e-02,   // 614
                                   2.178491095627052e+00,    // 615
                                   4.091831296142630e+01,    // 616
                                   -3.944410702237484e+01,   // 617
                                   1.140456228281048e-03,    // 618
                                   -2.712621151159238e+00,   // 619
                                   -1.416388193634209e+00,   // 620
                                   -1.229767293132646e+00,   // 621
                                   -4.591458440589150e-05,   // 622
                                   -3.454801544549441e+01,   // 623
                                   2.343489333446730e+02,    // 624
                                   2.884899849693564e-01,    // 625
                                   -6.131569917296683e-02,   // 626
                                   1.291248461300794e+02,    // 627
                                   2.280854540643510e-02,    // 628
                                   3.306641217745311e-03,    // 629
                                   1.986138867826714e+01,    // 630
                                   1.502007791889914e+01,    // 631
                                   2.052804568778633e+02,    // 632
                                   2.848894197789877e+01,    // 633
                                   7.347600226540041e+00,    // 634
                                   9.908119116405016e+00,    // 635
                                   8.958607018831258e-04,    // 636
                                   -4.482751159321198e-01,   // 637
                                   1.652191197123104e+02,    // 638
                                   -4.290523203314943e-03,   // 639
                                   -1.457846841853472e+00,   // 640
                                   -1.706903429315903e-02,   // 641
                                   2.364722980591455e-01,    // 642
                                   -2.618093491105744e+02,   // 643
                                   -8.478607363530795e+00,   // 644
                                   -1.399788818804483e+00,   // 645
                                   -1.042192130165231e+00,   // 646
                                   1.540361871438693e-02,    // 647
                                   2.255985392148454e-03,    // 648
                                   4.273445215862406e-01,    // 649
                                   6.412100396777683e+00,    // 650
                                   -1.072355909883954e+00,   // 651
                                   3.830875007483773e+00,    // 652
                                   -9.065949405555357e-03,   // 653
                                   -6.981926107931704e-05,   // 654
                                   1.344318319913397e-04,    // 655
                                   2.937207016330314e-01,    // 656
                                   1.497145499701766e+01,    // 657
                                   -5.756529359259128e-05,   // 658
                                   -1.532833594885178e-01,   // 659
                                   -1.307244094728398e+02,   // 660
                                   1.238907725315787e+01,    // 661
                                   2.900857862578030e-04,    // 662
                                   2.503839717564639e+00,    // 663
                                   6.597414802049658e-03,    // 664
                                   2.199066755452108e+01,    // 665
                                   2.171859637475553e+02,    // 666
                                   1.080415326048360e+02,    // 667
                                   -1.995767912593149e-02,   // 668
                                   -5.428916795171822e-03,   // 669
                                   1.896845176459375e-02,    // 670
                                   -3.682437428733754e+02,   // 671
                                   -9.630126306374694e+01,   // 672
                                   -4.092388190822749e-04,   // 673
                                   -4.830356022733416e-06,   // 674
                                   2.974045196909518e+02,    // 675
                                   -2.750049484622245e-03,   // 676
                                   -1.162492150335128e+01,   // 677
                                   1.490587753860527e+00,    // 678
                                   -1.156584005445950e-02,   // 679
                                   -7.066346642270240e+01,   // 680
                                   1.239627235241452e-01,    // 681
                                   3.058205786381475e+01,    // 682
                                   -2.195846137400743e-06,   // 683
                                   -6.697340870354258e-03,   // 684
                                   -8.427874049434470e+00,   // 685
                                   -1.352729246671000e+02,   // 686
                                   1.522608910150472e-03,    // 687
                                   -1.200066356536119e+00,   // 688
                                   -1.254202766463450e-03,   // 689
                                   -2.323105746680869e-02,   // 690
                                   -3.729423659447490e-01,   // 691
                                   1.357189105442326e-03,    // 692
                                   8.715790741664696e-04,    // 693
                                   -1.140038821837723e+00,   // 694
                                   -1.244663422714770e+02,   // 695
                                   -1.604808367827893e+01,   // 696
                                   -8.511566538934467e+00,   // 697
                                   3.432521829929983e-06,    // 698
                                   2.877135398034605e+00,    // 699
                                   7.939324587674655e-01,    // 700
                                   -1.022740412583001e+02,   // 701
                                   1.174265776690439e-04,    // 702
                                   -4.196367463428412e+00,   // 703
                                   -1.687293946834607e+02,   // 704
                                   8.004687267065420e-02,    // 705
                                   -2.324835239099538e-03,   // 706
                                   5.236780794344545e-02,    // 707
                                   -2.948487959413314e-03,   // 708
                                   6.256539759433924e-06,    // 709
                                   1.709850225713589e-01,    // 710
                                   9.957498446321898e-03,    // 711
                                   1.826509210888349e+00,    // 712
                                   -8.224625020434783e-05,   // 713
                                   -3.871561201251433e-08,   // 714
                                   1.079545557169195e-01,    // 715
                                   2.135169418872713e+02,    // 716
                                   1.081444911035032e+02,    // 717
                                   -4.786445136377538e+00,   // 718
                                   3.614588512326126e+00,    // 719
                                   3.799591187579094e+00,    // 720
                                   1.402454510128756e+01,    // 721
                                   -8.281772566719473e+00,   // 722
                                   1.855728108999033e-03,    // 723
                                   3.083842112998219e-02,    // 724
                                   1.374941132983235e+00,    // 725
                                   6.106378216559627e-06,    // 726
                                   6.751532189260909e+00,    // 727
                                   -8.815626004462493e-02,   // 728
                                   2.594782806912986e+02,    // 729
                                   1.516026040080658e-04,    // 730
                                   -3.971548164601201e+00,   // 731
                                   2.438549477543519e+02,    // 732
                                   -2.304581787310041e-01,   // 733
                                   1.167512580073878e+00,    // 734
                                   9.410899602544553e+00,    // 735
                                   -1.345393863282549e+02,   // 736
                                   2.295761927628683e+02,    // 737
                                   6.597317048499733e-04,    // 738
                                   2.524320545573065e+00,    // 739
                                   6.701888147001198e-04,    // 740
                                   -1.107670931262439e-03,   // 741
                                   -6.580283226085397e-05,   // 742
                                   -3.154031213221609e-02,   // 743
                                   -6.268902432151265e-02,   // 744
                                   1.462336267953653e+02,    // 745
                                   1.666795018060275e+02,    // 746
                                   9.429952433923976e+00,    // 747
                                   -7.875816951330084e-05,   // 748
                                   -1.483554506000900e+02,   // 749
                                   5.182688674687425e-05,    // 750
                                   -5.551156754695507e+01,   // 751
                                   -1.386870928844409e+00,   // 752
                                   -3.743948632362535e-03,   // 753
                                   -5.114219938509682e-03,   // 754
                                   5.540770220466139e-01,    // 755
                                   2.964413604767082e-01,    // 756
                                   6.213482333103889e-02,    // 757
                                   6.275704576532834e-04,    // 758
                                   5.463031293960511e-04,    // 759
                                   -1.015608982040166e-02,   // 760
                                   1.950461441742509e+00,    // 761
                                   1.168658535905368e+00,    // 762
                                   -6.641917987763838e+01,   // 763
                                   1.324750357302311e+02,    // 764
                                   -1.486448311106633e-06,   // 765
                                   -2.445018678032971e-02,   // 766
                                   2.259645418727269e+02,    // 767
                                   7.406902200664276e-05,    // 768
                                   4.404615137020199e-01,    // 769
                                   -8.100801352539810e+01,   // 770
                                   -1.060239748927063e+01,   // 771
                                   6.864900503940592e-01,    // 772
                                   2.406344421834693e-02,    // 773
                                   -9.850746138144659e-02,   // 774
                                   -8.521317161662633e-03,   // 775
                                   -7.305933901105172e+02,   // 776
                                   2.260919118077689e+01,    // 777
                                   -4.052288505711089e-02,   // 778
                                   1.355238741030955e+01,    // 779
                                   -7.497881565874291e-03,   // 780
                                   1.742305574427175e+00,    // 781
                                   -2.011401631510419e+00,   // 782
                                   -2.859877576789469e+01,   // 783
                                   5.280799828424444e-03,    // 784
                                   1.507336246521563e+01,    // 785
                                   5.594232245318158e-01,    // 786
                                   -7.613645680820600e-01,   // 787
                                   -6.265773433570626e-06,   // 788
                                   -2.325732308141311e+02,   // 789
                                   2.498649019149249e-02,    // 790
                                   1.035352068203798e-02,    // 791
                                   5.897904820069245e+00,    // 792
                                   2.131190848966349e-01,    // 793
                                   2.607820673858107e+02,    // 794
                                   3.174349454385387e+02,    // 795
                                   -3.239954046861741e-01,   // 796
                                   -5.263099151347961e-03,   // 797
                                   2.790174538111641e+02,    // 798
                                   -1.192137823774688e+00,   // 799
                                   -7.405394663736399e+00,   // 800
                                   2.072312356929756e-06,    // 801
                                   -3.946364905407110e+01,   // 802
                                   1.073418822836832e+02,    // 803
                                   -4.514307826007258e-01,   // 804
                                   -7.300898877340621e-04,   // 805
                                   -2.506429730295388e+02,   // 806
                                   8.291301012115771e-03,    // 807
                                   -3.575394738245213e-05,   // 808
                                   -1.622428614164629e-01,   // 809
                                   7.520666742837899e+01,    // 810
                                   -9.910062364446566e-04,   // 811
                                   -6.916017502645152e-03,   // 812
                                   1.271995695048592e+02,    // 813
                                   -1.112005934416187e-03,   // 814
                                   -1.348562862285953e+00,   // 815
                                   -5.100101979545070e-04,   // 816
                                   -8.071613159497058e+00,   // 817
                                   -1.286232443000267e+01,   // 818
                                   -2.611556522300317e-06,   // 819
                                   5.938188921916321e-06,    // 820
                                   7.441750905022161e-06,    // 821
                                   -1.708997823947441e-01,   // 822
                                   7.122682208432117e-03,    // 823
                                   -3.349093258281552e+00,   // 824
                                   2.363038389109436e+02,    // 825
                                   4.394824103551511e-01,    // 826
                                   2.459520402570837e+02,    // 827
                                   2.620742902028190e-04,    // 828
                                   -1.131745411610095e-04,   // 829
                                   -3.815715860853421e+00,   // 830
                                   -1.597036351181215e-01,   // 831
                                   1.111272589038933e-03,    // 832
                                   -5.066859616443605e-05,   // 833
                                   6.901642825899563e+01,    // 834
                                   -5.580378062534297e-01,   // 835
                                   4.406291578333746e-03,    // 836
                                   -9.681611443092231e-03,   // 837
                                   -1.061007104129749e+01,   // 838
                                   1.173478203791137e+00,    // 839
                                   3.431936333959107e-01,    // 840
                                   -2.860846871468991e-04,   // 841
                                   -1.004949064473788e+02,   // 842
                                   -1.414687832297590e+00,   // 843
                                   3.874512631717842e+01,    // 844
                                   -8.449305008500696e+01,   // 845
                                   -3.208960634706466e+02,   // 846
                                   8.425257840101295e+01,    // 847
                                   -4.013420061533045e-02,   // 848
                                   3.903077454233989e-03,    // 849
                                   -4.455146230437832e+00,   // 850
                                   4.207035075979316e-01,    // 851
                                   -4.684345332106234e-05,   // 852
                                   1.081738363275967e+00,    // 853
                                   1.077394431376893e+00,    // 854
                                   -4.195789139167019e+01,   // 855
                                   -8.557751690387576e+01,   // 856
                                   2.515848975444547e+02,    // 857
                                   -1.613448832349069e+01,   // 858
                                   -1.141165623420166e+01,   // 859
                                   8.654874884860442e-02,    // 860
                                   3.396119804711680e-06,    // 861
                                   -4.902185130160372e+00,   // 862
                                   -9.671787643359889e+01,   // 863
                                   1.826919335877568e-01,    // 864
                                   2.758953697222857e-02,    // 865
                                   -1.078303695526940e+00,   // 866
                                   -8.159701539749421e-01,   // 867
                                   -4.142485653854448e+00,   // 868
                                   -4.066563836007518e-01,   // 869
                                   -2.697699646877766e-02,   // 870
                                   -6.074826958021606e-02,   // 871
                                   -7.785592728509292e+01,   // 872
                                   1.706243575728456e+01,    // 873
                                   9.811642884481426e-03,    // 874
                                   8.995625388406083e-03,    // 875
                                   4.323390982684300e+00,    // 876
                                   4.646298595344046e+01,    // 877
                                   -9.430839708014224e+01,   // 878
                                   2.736600114225196e+01,    // 879
                                   -9.572176328745977e+00,   // 880
                                   -1.390442492002451e+00,   // 881
                                   3.990968251149632e+01,    // 882
                                   2.263088898090765e-02,    // 883
                                   -2.471877091713595e-02,   // 884
                                   3.386253026432649e+01,    // 885
                                   5.855116884540395e-03,    // 886
                                   2.978642329495942e+00,    // 887
                                   -1.709588763179334e+00,   // 888
                                   -3.479168449089398e-01,   // 889
                                   2.881072658700846e-01,    // 890
                                   -2.015722245973828e-02,   // 891
                                   -2.772479567118223e+02,   // 892
                                   2.223479297573442e-03,    // 893
                                   -1.455726941900249e+02,   // 894
                                   4.339349796063649e+01,    // 895
                                   7.938543311426367e-04,    // 896
                                   4.333379314258138e+01,    // 897
                                   -7.149628917220126e-03,   // 898
                                   -1.675480340389306e-02,   // 899
                                   9.000733250737049e+01,    // 900
                                   -1.147262126959641e+02,   // 901
                                   2.389626084833716e-03,    // 902
                                   9.733142149005822e+00,    // 903
                                   1.136825849780356e+01,    // 904
                                   4.738072449303392e-03,    // 905
                                   2.757323138429351e-01,    // 906
                                   -2.736974195938085e+02,   // 907
                                   1.476290568604850e+01,    // 908
                                   -9.443540433855385e+01,   // 909
                                   -5.551806003844187e-02,   // 910
                                   -1.533992604991320e-02,   // 911
                                   -1.439352571920952e-04,   // 912
                                   2.194109733035186e-01,    // 913
                                   1.090734940498795e+02,    // 914
                                   3.036549031155346e+01,    // 915
                                   6.078818114356989e+01,    // 916
                                   1.987938183526285e+02,    // 917
                                   1.360769901538710e-01,    // 918
                                   -1.022526479666119e+01,   // 919
                                   -9.642747045168948e-04,   // 920
                                   2.014520185909213e-02,    // 921
                                   -1.568242763000648e+00,   // 922
                                   2.459928650639735e+02,    // 923
                                   -2.092862268890822e+01,   // 924
                                   -1.169441689025899e+02,   // 925
                                   5.071068518520852e-05,    // 926
                                   7.488151950906992e-04,    // 927
                                   2.371782316374146e-01,    // 928
                                   -2.306852359886856e-03,   // 929
                                   1.790148053871517e+00,    // 930
                                   -9.695446113941458e-05,   // 931
                                   -4.144835486588582e-01,   // 932
                                   -1.822086871914013e-01,   // 933
                                   2.072615049444985e+00,    // 934
                                   5.756027315553780e+00,    // 935
                                   2.144504888479450e-04,    // 936
                                   3.808551275795757e+00,    // 937
                                   4.077902736266947e+01,    // 938
                                   2.335705468806036e-03,    // 939
                                   6.723990863789884e+01,    // 940
                                   -1.930033058266027e+00,   // 941
                                   4.266112832644024e-02,    // 942
                                   3.115327925879412e-08,    // 943
                                   3.170795789287278e-02,    // 944
                                   -1.299748615433680e+02,   // 945
                                   -5.556576160467806e+00,   // 946
                                   6.683801431164629e+01,    // 947
                                   -3.517655882427408e-02,   // 948
                                   2.556263810259800e+00,    // 949
                                   2.345111762520073e-01,    // 950
                                   -1.505583138120275e+01,   // 951
                                   1.681810817839476e+02,    // 952
                                   -2.032740822188664e-06,   // 953
                                   1.785511469935204e+01,    // 954
                                   9.476354947378772e-01,    // 955
                                   3.172967174396958e-03,    // 956
                                   -1.282897487836865e-01,   // 957
                                   3.995853528901114e+00,    // 958
                                   -4.966366072627249e-06,   // 959
                                   4.450121096017260e+02,    // 960
                                   -5.236183488002943e-01,   // 961
                                   1.757060222056064e+01,    // 962
                                   3.290595307072728e-04,    // 963
                                   1.588473769638697e+02,    // 964
                                   1.818714800090898e+00,    // 965
                                   -7.338601894141442e-03,   // 966
                                   -1.213688955345913e+02,   // 967
                                   3.154924840873894e+01,    // 968
                                   1.003837492664771e-03,    // 969
                                   1.208458799373361e-02,    // 970
                                   -1.400763199475763e-03,   // 971
                                   -3.014060520383228e-05,   // 972
                                   -3.763495044731509e+01,   // 973
                                   1.839038390182609e+00,    // 974
                                   2.184606714662075e+01,    // 975
                                   5.958773230110559e-01,    // 976
                                   -4.892672763024993e-04,   // 977
                                   -4.727324802375997e+01,   // 978
                                   -8.813940904238248e+01,   // 979
                                   -6.431540473266443e+01,   // 980
                                   3.526908989597660e-03,    // 981
                                   -4.642022468797495e+00,   // 982
                                   -2.203164902788084e-01,   // 983
                                   -4.475023315191053e-01,   // 984
                                   9.299357296377859e-05,    // 985
                                   -1.093244005837161e-02,   // 986
                                   1.485698708312152e+01,    // 987
                                   -1.895886301022489e+02,   // 988
                                   -4.813082156625847e-04,   // 989
                                   1.221857352018430e+02,    // 990
                                   2.841509202401149e+02,    // 991
                                   1.666515794869262e-03,    // 992
                                   -1.222583898743892e-02,   // 993
                                   -1.706127476169567e-03,   // 994
                                   1.999066028482542e-02,    // 995
                                   -1.304772072826224e-02,   // 996
                                   -1.357559005667231e-05,   // 997
                                   -3.911729187705288e+01,   // 998
                                   -1.395962593420680e-03,   // 999
                                   1.926701612229835e+01,    // 1000
                                   -2.914438205317049e-02,   // 1001
                                   1.301741139126806e+01,    // 1002
                                   -4.962661106781357e-07,   // 1003
                                   4.135516327687544e+02,    // 1004
                                   -1.290588842266402e+02,   // 1005
                                   -1.281014709861155e-02,   // 1006
                                   -2.991586093406112e+02,   // 1007
                                   6.092964816831870e-06,    // 1008
                                   -1.509402440053693e+02,   // 1009
                                   9.952788078041869e-04,    // 1010
                                   -2.759317158563336e+01,   // 1011
                                   -1.734365757934021e-02,   // 1012
                                   -3.289363819667326e+01,   // 1013
                                   -1.088023731389976e+02,   // 1014
                                   -2.396066490047632e+01};  // 1015

        //----------------------------------------------------------------------------//
        m_r3i = 0.000000000000000e+00;        // A
        m_r3f = 4.500000000000000e+00;        // A
        m_kHH_intra = 2.343614900390014e-01;  // A^(-1)
        m_kOH_intra = 8.068929184027878e-01;  // A^(-1)
        m_kHH = 2.580358900220996e-01;        // A^(-1)
        m_kOH = 7.063990540733522e-01;        // A^(-1)
        m_kOO = 2.374997393974423e-02;        // A^(-1)
        m_kXO = 5.995683887693093e-02;        // A^(-1)
        m_kXH = 4.022139878389860e-01;        // A^(-1)
        m_dHH_intra = 5.101199357764126e-01;  // A
        m_dOH_intra = 2.035944886211303e-04;  // A
        m_dHH = 7.186210165587381e+00;        // A
        m_dOH = 1.142220902170943e+01;        // A
        m_dOO = 3.021128948742693e+00;        // A
        m_dXO = 2.398846003968965e+00;        // A
        m_dXH = 8.914038895157681e+00;        // A

        std::copy(the_poly, the_poly + ncoeffs, m_coeffs);

    } else {
        std::cerr << "Ion " << m3 << " not found in 3b database\n";
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////////////////
