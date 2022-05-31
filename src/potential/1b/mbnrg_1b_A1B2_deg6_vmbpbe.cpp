#include "mbnrg_1b_A1B2_deg6_vmbpbe.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B2_deg6 {

mbnrg_A1B2_deg6_vmbpbe::mbnrg_A1B2_deg6_vmbpbe(const std::string mon1) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "mbpbe") {
        coefficients = std::vector<double>{1.399413526032413e+03,   // 0
                                           -8.188012303406360e+02,  // 1
                                           -1.998469438916813e+03,  // 2
                                           -6.968243736752291e+02,  // 3
                                           1.360304188351023e+03,   // 4
                                           1.113588764914643e+03,   // 5
                                           5.323945919621528e+02,   // 6
                                           -3.388495646142625e+02,  // 7
                                           1.101382039248987e+03,   // 8
                                           1.241259694683234e+03,   // 9
                                           -9.620804288750737e+02,  // 10
                                           -3.048982767761102e+03,  // 11
                                           -1.992307488119520e+03,  // 12
                                           -2.130406067037374e+03,  // 13
                                           2.206288539905275e+03,   // 14
                                           -8.726872681296642e+02,  // 15
                                           7.589893870077108e+02,   // 16
                                           -4.280425999325106e+02,  // 17
                                           3.332674328672110e+02,   // 18
                                           -1.533292705356804e+02,  // 19
                                           1.226097439136981e+03,   // 20
                                           3.204302934302096e+02,   // 21
                                           -8.440321066585686e+02,  // 22
                                           -2.711682706500657e+02,  // 23
                                           2.139212297528176e+03,   // 24
                                           1.165136841125014e+03,   // 25
                                           1.096483059331003e+03,   // 26
                                           -2.059465075973439e+03,  // 27
                                           2.554198614292255e+03,   // 28
                                           -1.373946478118471e+03,  // 29
                                           -1.365043326840889e+03,  // 30
                                           -5.820802024048180e+02,  // 31
                                           5.682544875130778e+02,   // 32
                                           1.936236882886696e+03,   // 33
                                           7.428380757015999e+02,   // 34
                                           -3.732972680368495e+03,  // 35
                                           8.593848431182618e+02,   // 36
                                           -9.341891883526431e+02,  // 37
                                           2.392046802883674e+02,   // 38
                                           1.026082283038101e+03,   // 39
                                           -3.550704608454960e+02,  // 40
                                           1.585861640177254e+03,   // 41
                                           3.158772533759962e+03,   // 42
                                           -1.149451358060151e+02,  // 43
                                           -1.001025645481171e+03,  // 44
                                           -6.582481648846356e+02,  // 45
                                           1.065253860175611e+03,   // 46
                                           2.949421638499998e+02,   // 47
                                           6.675215389235271e+02};  // 48
        m_k_x_intra_A_B_1 = 7.278471535225877e-01;                  // A^(-1))
        m_k_x_intra_B_B_1 = 2.924745286903587e-01;                  // A^(-1))
        m_ri = 7.000000000000000e+00;                               // A
        m_ro = 8.000000000000000e+00;                               // A

    }  // end if mon1 == "mbpbe"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A1B2_deg6_vmbpbe::f_switch(const double r, double& g) {
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

std::vector<double> mbnrg_A1B2_deg6_vmbpbe::eval(const double* xyz1, const size_t n) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(9);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 9 * j;

        if (false) {
            continue;
        }

        std::copy(mon1, mon1 + 9, xyz.begin() + 0);

        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_B_1_a = xyz.data() + 3;

        const double* coords_B_2_a = xyz.data() + 6;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[3];

        double xs[3];

        xs[0] = vs[0].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_1_a);
        xs[1] = vs[1].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_2_a);
        xs[2] = vs[2].v_exp(m_k_x_intra_B_B_1, coords_B_1_a, coords_B_2_a);

        sw = 1.0;

        energies[j] = my_poly.eval(xs, coefficients.data());
        energies_sw[j] = energies[j] * sw;
    }
    return energies_sw;
}

//----------------------------------------------------------------------------//

std::vector<double> mbnrg_A1B2_deg6_vmbpbe::eval(const double* xyz1, double* grad1, const size_t n,
                                                 std::vector<double>* virial) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(9);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 9 * j;

        if (false) {
            continue;
        }

        std::vector<double> gradients(9, 0.0);

        std::copy(mon1, mon1 + 9, xyz.begin() + 0);
        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_B_1_a = xyz.data() + 3;

        const double* coords_B_2_a = xyz.data() + 6;

        double* coords_A_1_a_g = gradients.data() + 0;

        double* coords_B_1_a_g = gradients.data() + 3;

        double* coords_B_2_a_g = gradients.data() + 6;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[3];

        double xs[3];

        double gxs[3];

        xs[0] = vs[0].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_1_a);
        xs[1] = vs[1].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_2_a);
        xs[2] = vs[2].v_exp(m_k_x_intra_B_B_1, coords_B_1_a, coords_B_2_a);

        sw = 1.0;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 3; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_B_1_a_g, coords_A_1_a, coords_B_1_a);
        vs[1].grads(gxs[1], coords_A_1_a_g, coords_B_2_a_g, coords_A_1_a, coords_B_2_a);
        vs[2].grads(gxs[2], coords_B_1_a_g, coords_B_2_a_g, coords_B_1_a, coords_B_2_a);

        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0;
        }

        for (size_t i = 0; i < 9; i++) {
            grad1[i + j * 9] += gradients[0 + i];
        }

        if (virial != 0) {
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_B_1_a[0] * coords_B_1_a_g[0] -
                            coords_B_2_a[0] * coords_B_2_a_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_B_1_a[0] * coords_B_1_a_g[1] -
                            coords_B_2_a[0] * coords_B_2_a_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_B_1_a[0] * coords_B_1_a_g[2] -
                            coords_B_2_a[0] * coords_B_2_a_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_B_1_a[1] * coords_B_1_a_g[1] -
                            coords_B_2_a[1] * coords_B_2_a_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_B_1_a[1] * coords_B_1_a_g[2] -
                            coords_B_2_a[1] * coords_B_2_a_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_B_1_a[2] * coords_B_1_a_g[2] -
                            coords_B_2_a[2] * coords_B_2_a_g[2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }
    }
    return energies_sw;
}

//----------------------------------------------------------------------------//
}  // namespace mbnrg_A1B2_deg6
