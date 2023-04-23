#include "mbnrg_2b_A1_A1_deg9_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1_A1_deg9 {

mbnrg_A1_A1_deg9_v1::mbnrg_A1_A1_deg9_v1(const std::string mon1, const std::string mon2) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "na+" and mon2 == "na+") {
        coefficients = std::vector<double>{5.168979440098036e-03,   // 0
                                           -1.057172129456625e-02,  // 1
                                           9.840375789382686e-03,   // 2
                                           -4.814498332310714e-03,  // 3
                                           1.440222969269185e-03,   // 4
                                           -2.595906223646280e-04,  // 5
                                           2.792990738765786e-05,   // 6
                                           -1.641074627874924e-06,  // 7
                                           3.985370934016254e-08};  // 8
        m_k_x_inter_A_A_0 = 4.380148617370266e-01;                  // A^(-1))
        m_d_x_inter_A_A_0 = 7.568203895545039e+00;                  // A^(-1))
        m_ri = 7.000000000000000e+00;                               // A
        m_ro = 9.000000000000000e+00;                               // A

    }  // end if mon1 == "na+" and mon2 == "na+"
    if (mon1 == "cl-" and mon2 == "cl-") {
        coefficients = std::vector<double>{9.425632806990795e-01,   // 0
                                           -1.210233366348845e+00,  // 1
                                           6.587041999446325e-01,   // 2
                                           -1.955522739983074e-01,  // 3
                                           3.464872380127313e-02,   // 4
                                           -3.804138871440972e-03,  // 5
                                           2.526544676101109e-04,   // 6
                                           -9.198214156188702e-06,  // 7
                                           1.394590200717518e-07};  // 8
        m_k_x_inter_A_A_0 = 3.397551018612796e-01;                  // A^(-1))
        m_d_x_inter_A_A_0 = 1.015131833802185e+01;                  // A^(-1))
        m_ri = 7.000000000000000e+00;                               // A
        m_ro = 9.000000000000000e+00;                               // A

    }  // end if mon1 == "cl-" and mon2 == "cl-"
    if (mon1 == "ar" and mon2 == "ar") {
        coefficients = std::vector<double>{-1.038017069401949e-02,   // 0
                                           7.837643711507530e-03,    // 1
                                           -1.751940803887289e-02,   // 2
                                           3.457675184309017e-03,    // 3
                                           -5.884364847391292e-04,   // 4
                                           5.005200675827483e-04,    // 5
                                           -4.983623603401647e-05,   // 6
                                           2.371551601409795e-06,    // 7
                                           -5.544062914063120e-08};  // 8
        m_k_x_inter_A_A_0 = 6.511296783204924e-01;                   // A^(-1))
        m_d_x_inter_A_A_0 = 5.457946966470896e+00;                   // A^(-1))
        m_ri = 7.000000000000000e+00;                                // A
        m_ro = 9.000000000000000e+00;                                // A

    }  // end if mon1 == "ar" and mon2 == "ar"

    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A1_A1_deg9_v1::f_switch(const double r, double &g) {
    if (r > m_ro) {
        g = 0.0;
        return 0.0;
    } else if (r > m_ri) {
        const double t1 = M_PI / (m_ro - m_ri);
        const double x = (r - m_ri) * t1;
        g = -std::sin(x) * t1 / 2.0;
        return (1.0 + std::cos(x)) / 2.0;
    } else {
        g = 0.0;
        return 1.0;
    }
}

//----------------------------------------------------------------------------//

double mbnrg_A1_A1_deg9_v1::eval(const double *xyz1, const double *xyz2, const size_t n) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(6);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double *mon1 = xyz1 + 3 * j;
        const double *mon2 = xyz2 + 3 * j;

        const double d12[3] = {mon1[0] - mon2[0], mon1[1] - mon2[1], mon1[2] - mon2[2]};

        const double d12rsq = d12[0] * d12[0] + d12[1] * d12[1] + d12[2] * d12[2];
        const double d12r = std::sqrt(d12rsq);

        if (true && d12r > m_ro) {
            continue;
        }

        std::copy(mon1, mon1 + 3, xyz.begin() + 0);

        std::copy(mon2, mon2 + 3, xyz.begin() + 3);

        const double *coords_A_1_a = xyz.data() + 0;

        const double *coords_A_2_b = xyz.data() + 3;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[1];

        double xs[1];

        xs[0] = vs[0].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_2_b);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);

        sw = sw12;

        energies[j] = my_poly.eval(xs, coefficients.data());
        energies_sw[j] = energies[j] * sw;
    }

    double energy = 0.0;
    for (size_t i = 0; i < n; i++) energy += energies_sw[i];

    return energy;
}

//----------------------------------------------------------------------------//

double mbnrg_A1_A1_deg9_v1::eval(const double *xyz1, const double *xyz2, double *grad1, double *grad2, const size_t n,
                                 std::vector<double> *virial) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(6);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double *mon1 = xyz1 + 3 * j;
        const double *mon2 = xyz2 + 3 * j;

        const double d12[3] = {mon1[0] - mon2[0], mon1[1] - mon2[1], mon1[2] - mon2[2]};

        const double d12rsq = d12[0] * d12[0] + d12[1] * d12[1] + d12[2] * d12[2];
        const double d12r = std::sqrt(d12rsq);

        if (true && d12r > m_ro) {
            continue;
        }

        std::vector<double> gradients(6, 0.0);

        std::copy(mon1, mon1 + 3, xyz.begin() + 0);

        std::copy(mon2, mon2 + 3, xyz.begin() + 3);
        const double *coords_A_1_a = xyz.data() + 0;

        const double *coords_A_2_b = xyz.data() + 3;

        double *coords_A_1_a_g = gradients.data() + 0;

        double *coords_A_2_b_g = gradients.data() + 3;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[1];

        double xs[1];

        double gxs[1];

        xs[0] = vs[0].v_exp0(m_d_x_inter_A_A_0, m_k_x_inter_A_A_0, coords_A_1_a, coords_A_2_b);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);

        sw = sw12;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 1; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_A_2_b_g, coords_A_1_a, coords_A_2_b);
        gsw12 *= (1.0) * energies[j] / d12r;

        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0 + (gsw12 * d12[i]);
            gradients[3 + i] += 0.0 - (gsw12 * d12[i]);
        }

        for (size_t i = 0; i < 3; i++) {
            grad1[i + j * 3] += gradients[0 + i];
        }

        for (size_t i = 0; i < 3; i++) {
            grad2[i + j * 3] += gradients[3 + i];
        }

        if (virial != 0) {
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_A_2_b[0] * coords_A_2_b_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_A_2_b[0] * coords_A_2_b_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_A_2_b[0] * coords_A_2_b_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_A_2_b[1] * coords_A_2_b_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_A_2_b[1] * coords_A_2_b_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_A_2_b[2] * coords_A_2_b_g[2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }
    }

    double energy = 0.0;
    for (size_t i = 0; i < n; i++) energy += energies_sw[i];

    return energy;
}

//----------------------------------------------------------------------------//
}  // namespace mbnrg_A1_A1_deg9
