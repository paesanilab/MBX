#include "mbnrg_2b_A1_B1_deg15_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1_B1_deg15 {

mbnrg_A1_B1_deg15_v1::mbnrg_A1_B1_deg15_v1(const std::string mon1, const std::string mon2) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "ar" and mon2 == "cs") {
        coefficients = std::vector<double>{-4.965048705232530e-02,   // 0
                                           5.200899896394952e-01,    // 1
                                           -1.881214303302322e+00,   // 2
                                           3.290267856983993e+00,    // 3
                                           -3.389514915192743e+00,   // 4
                                           2.219267705449683e+00,    // 5
                                           -9.735824575424189e-01,   // 6
                                           2.962696153169506e-01,    // 7
                                           -6.346042409536531e-02,   // 8
                                           9.590460313553838e-03,    // 9
                                           -1.010190851288500e-03,   // 10
                                           7.183214292098551e-05,    // 11
                                           -3.234268515380597e-06,   // 12
                                           8.091463151535712e-08,    // 13
                                           -8.071183627352966e-10};  // 14
        m_k_x_inter_A_B_0 = 5.447610225428158e-01;                   // A^(-1))
        m_d_x_inter_A_B_0 = 5.569833150409103e+00;                   // A^(-1))
        m_ri = 7.000000000000000e+00;                                // A
        m_ro = 8.000000000000000e+00;                                // A

    }  // end if mon1 == "ar" and mon2 == "cs"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A1_B1_deg15_v1::f_switch(const double r, double &g) {
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

double mbnrg_A1_B1_deg15_v1::eval(const double *xyz1, const double *xyz2, const size_t n) {
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

        const double *coords_B_1_b = xyz.data() + 3;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[1];

        double xs[1];

        xs[0] = vs[0].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_1_b);

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

double mbnrg_A1_B1_deg15_v1::eval(const double *xyz1, const double *xyz2, double *grad1, double *grad2, const size_t n,
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

        const double *coords_B_1_b = xyz.data() + 3;

        double *coords_A_1_a_g = gradients.data() + 0;

        double *coords_B_1_b_g = gradients.data() + 3;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[1];

        double xs[1];

        double gxs[1];

        xs[0] = vs[0].v_exp0(m_d_x_inter_A_B_0, m_k_x_inter_A_B_0, coords_A_1_a, coords_B_1_b);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);

        sw = sw12;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 1; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_B_1_b_g, coords_A_1_a, coords_B_1_b);
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
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_B_1_b[0] * coords_B_1_b_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_B_1_b[0] * coords_B_1_b_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_B_1_b[0] * coords_B_1_b_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_B_1_b[1] * coords_B_1_b_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_B_1_b[1] * coords_B_1_b_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_B_1_b[2] * coords_B_1_b_g[2];

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
}  // namespace mbnrg_A1_B1_deg15
