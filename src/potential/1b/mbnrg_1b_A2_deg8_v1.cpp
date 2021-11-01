#include "mbnrg_1b_A2_deg8_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A2_deg8 {

mbnrg_A2_deg8_v1::mbnrg_A2_deg8_v1(const std::string mon1) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "h2") {
        coefficients = std::vector<double>{1.613784847330184e+01,   // 0
                                           2.160385423783417e+01,   // 1
                                           -1.058229566240631e+01,  // 2
                                           2.055471348773575e+00,   // 3
                                           -2.214743499455893e-01,  // 4
                                           1.400597998041749e-02,   // 5
                                           -4.891007467601349e-04,  // 6
                                           7.404581503618356e-06};  // 7
        m_k_x_intra_A_A_1 = 8.417304002388686e-01;                  // A^(-1))
        m_d_x_intra_A_A_1 = 3.555622179537664e+00;                  // A^(-1))
        m_ri = 6.000000000000000e+00;                               // A
        m_ro = 8.000000000000000e+00;                               // A

    }  // end if mon1 == "h2"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

std::vector<double> mbnrg_A2_deg8_v1::eval(const double* xyz1, const size_t n) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(6);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 6 * j;

        if (false) {
            continue;
        }

        std::copy(mon1, mon1 + 6, xyz.begin() + 0);

        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_A_2_a = xyz.data() + 3;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[1];

        double xs[1];

        xs[0] = vs[0].v_exp0(m_d_x_intra_A_A_1, m_k_x_intra_A_A_1, coords_A_1_a, coords_A_2_a);

        sw = 1.0;

        energies[j] = my_poly.eval(xs, coefficients.data());
        energies_sw[j] = energies[j] * sw;
    }
    return energies_sw;
}

//----------------------------------------------------------------------------//

std::vector<double> mbnrg_A2_deg8_v1::eval(const double* xyz1, double* grad1, const size_t n,
                                           std::vector<double>* virial) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(6);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 6 * j;

        if (false) {
            continue;
        }

        std::vector<double> gradients(6, 0.0);

        std::copy(mon1, mon1 + 6, xyz.begin() + 0);
        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_A_2_a = xyz.data() + 3;

        double* coords_A_1_a_g = gradients.data() + 0;

        double* coords_A_2_a_g = gradients.data() + 3;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[1];

        double xs[1];

        double gxs[1];

        xs[0] = vs[0].v_exp0(m_d_x_intra_A_A_1, m_k_x_intra_A_A_1, coords_A_1_a, coords_A_2_a);

        sw = 1.0;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 1; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_A_2_a_g, coords_A_1_a, coords_A_2_a);

        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0;
        }

        for (size_t i = 0; i < 6; i++) {
            grad1[i + j * 6] += gradients[0 + i];
        }

        if (virial != 0) {
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_A_2_a[0] * coords_A_2_a_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_A_2_a[0] * coords_A_2_a_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_A_2_a[0] * coords_A_2_a_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_A_2_a[1] * coords_A_2_a_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_A_2_a[1] * coords_A_2_a_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_A_2_a[2] * coords_A_2_a_g[2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }
    }
    return energies_sw;
}

//----------------------------------------------------------------------------//
}  // namespace mbnrg_A2_deg8
