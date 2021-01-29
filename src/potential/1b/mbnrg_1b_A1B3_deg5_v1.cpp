#include "mbnrg_1b_A1B3_deg5_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B3_deg5 {

mbnrg_A1B3_deg5_v1::mbnrg_A1B3_deg5_v1(const std::string mon1) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "nh3") {
        coefficients = std::vector<double>{3.054726712989003e+03,   // 0
                                           -4.595552473413987e+02,  // 1
                                           4.519518504662589e+02,   // 2
                                           8.246782812851519e+02,   // 3
                                           2.585526757441972e+02,   // 4
                                           -1.640374757684719e+03,  // 5
                                           1.457376762324383e+03,   // 6
                                           -3.084856340802114e+03,  // 7
                                           -3.124213665758284e+03,  // 8
                                           4.605929570040107e+02,   // 9
                                           4.434590987911374e+01,   // 10
                                           1.423259930103530e+03,   // 11
                                           6.285911819909426e+02,   // 12
                                           5.911352166490782e+02,   // 13
                                           1.910919681932397e+02,   // 14
                                           -1.854185820891932e+03,  // 15
                                           8.183902122690940e+02,   // 16
                                           -1.478482042580928e+03,  // 17
                                           1.216356821394938e+03,   // 18
                                           -1.068598121035599e+03,  // 19
                                           5.288840275365075e+02,   // 20
                                           -6.589852080714010e+02,  // 21
                                           -4.140819715998549e+02,  // 22
                                           -2.783879020098983e+02,  // 23
                                           -8.965657703286215e+02,  // 24
                                           1.490429251989675e+03,   // 25
                                           1.530036348107699e+03,   // 26
                                           1.942605220135145e+02,   // 27
                                           -1.916161999167075e+02,  // 28
                                           4.884011835521700e+02,   // 29
                                           -1.034592753616248e+03,  // 30
                                           -2.466955137968876e+03,  // 31
                                           3.243220455012882e+02,   // 32
                                           -1.587766655692527e+03,  // 33
                                           -6.279160517400541e+00,  // 34
                                           -9.790726293710862e+02,  // 35
                                           -1.097526631801952e+03,  // 36
                                           -1.646614966327297e+03,  // 37
                                           1.759293093312335e+03,   // 38
                                           -1.644965808419162e+02,  // 39
                                           3.856248852045777e+02,   // 40
                                           2.737297349894860e+03,   // 41
                                           -2.875977201931354e+01,  // 42
                                           -8.378004265535463e+02,  // 43
                                           -6.021480472155871e+02,  // 44
                                           1.550134286817139e+03,   // 45
                                           5.275427342535129e+02,   // 46
                                           -2.785642031249102e+01,  // 47
                                           1.419739047552798e+03,   // 48
                                           9.727207336708664e+02,   // 49
                                           -8.733140054371408e+02,  // 50
                                           7.140068223447952e+02,   // 51
                                           -3.708785189606441e+02,  // 52
                                           1.229232651142115e+03,   // 53
                                           2.508971006215615e+02,   // 54
                                           1.941165506975422e+02,   // 55
                                           1.959476497633635e+03,   // 56
                                           -2.478698099880633e+03,  // 57
                                           1.110741577356795e+03,   // 58
                                           1.553604373614194e+03,   // 59
                                           -1.396377453882045e+03,  // 60
                                           3.929230518060398e+01,   // 61
                                           -9.533582762990765e+02,  // 62
                                           -5.557679736524487e+02,  // 63
                                           -9.464038203727563e+02,  // 64
                                           2.601069676922818e+01,   // 65
                                           7.316779088819994e+02,   // 66
                                           -7.212844186646375e+02,  // 67
                                           5.920666769023502e+02,   // 68
                                           4.214580212337903e+00,   // 69
                                           -1.639910113793271e+03,  // 70
                                           6.722837990617019e+02,   // 71
                                           -2.365499672506599e+02,  // 72
                                           -1.548692824912584e+03,  // 73
                                           6.893523452617483e+02,   // 74
                                           1.885690669775384e+03,   // 75
                                           -8.806238826832797e+02,  // 76
                                           -1.484384349590396e+03,  // 77
                                           1.343904427410822e+03,   // 78
                                           1.551913550119937e+03,   // 79
                                           -9.156298368224397e+01,  // 80
                                           -5.603921710045746e+02,  // 81
                                           2.842169415249708e+02,   // 82
                                           6.067812832028964e+02,   // 83
                                           -2.464099785136369e+03,  // 84
                                           -9.498815359411756e+02,  // 85
                                           2.174692916704421e+02,   // 86
                                           -1.066244435898038e+03,  // 87
                                           1.355817472351655e+03,   // 88
                                           2.180851728164583e+03,   // 89
                                           2.022765768198343e+03,   // 90
                                           -2.002896602806206e+02,  // 91
                                           -3.882525875336788e+02,  // 92
                                           -1.209207335017696e+03,  // 93
                                           -7.844708947672906e+02,  // 94
                                           -1.522201931124819e+02,  // 95
                                           6.945660720670562e+02,   // 96
                                           1.042284611587187e+03,   // 97
                                           -5.997620817364519e+02,  // 98
                                           -6.361802285148146e+02,  // 99
                                           -3.922074221007017e+03,  // 100
                                           2.598375288452326e+03};  // 101
        m_k_x_intra_A_B_1 = 7.659868609151662e-01;                  // A^(-1))
        m_k_x_intra_B_B_1 = 2.640646475112278e-01;                  // A^(-1))
        m_ri = 7.000000000000000e+00;                               // A
        m_ro = 8.000000000000000e+00;                               // A

    }  // end if mon1 == "nh3"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A1B3_deg5_v1::f_switch(const double r, double& g) {
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

std::vector<double> mbnrg_A1B3_deg5_v1::eval(const double* xyz1, const size_t n) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(12);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 12 * j;

        if (false) {
            continue;
        }

        std::copy(mon1, mon1 + 12, xyz.begin() + 0);

        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_B_1_a = xyz.data() + 3;

        const double* coords_B_2_a = xyz.data() + 6;

        const double* coords_B_3_a = xyz.data() + 9;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[6];

        double xs[6];

        xs[0] = vs[0].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_1_a);
        xs[1] = vs[1].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_2_a);
        xs[2] = vs[2].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_3_a);
        xs[3] = vs[3].v_exp(m_k_x_intra_B_B_1, coords_B_1_a, coords_B_2_a);
        xs[4] = vs[4].v_exp(m_k_x_intra_B_B_1, coords_B_1_a, coords_B_3_a);
        xs[5] = vs[5].v_exp(m_k_x_intra_B_B_1, coords_B_2_a, coords_B_3_a);

        sw = 1.0;

        energies[j] = my_poly.eval(xs, coefficients.data());
        energies_sw[j] = energies[j] * sw;
    }
    return energies_sw;
}

//----------------------------------------------------------------------------//

std::vector<double> mbnrg_A1B3_deg5_v1::eval(const double* xyz1, double* grad1, const size_t n,
                                             std::vector<double>* virial) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(12);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 12 * j;

        if (false) {
            continue;
        }

        std::vector<double> gradients(12, 0.0);

        std::copy(mon1, mon1 + 12, xyz.begin() + 0);
        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_B_1_a = xyz.data() + 3;

        const double* coords_B_2_a = xyz.data() + 6;

        const double* coords_B_3_a = xyz.data() + 9;

        double* coords_A_1_a_g = gradients.data() + 0;

        double* coords_B_1_a_g = gradients.data() + 3;

        double* coords_B_2_a_g = gradients.data() + 6;

        double* coords_B_3_a_g = gradients.data() + 9;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[6];

        double xs[6];

        double gxs[6];

        xs[0] = vs[0].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_1_a);
        xs[1] = vs[1].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_2_a);
        xs[2] = vs[2].v_exp(m_k_x_intra_A_B_1, coords_A_1_a, coords_B_3_a);
        xs[3] = vs[3].v_exp(m_k_x_intra_B_B_1, coords_B_1_a, coords_B_2_a);
        xs[4] = vs[4].v_exp(m_k_x_intra_B_B_1, coords_B_1_a, coords_B_3_a);
        xs[5] = vs[5].v_exp(m_k_x_intra_B_B_1, coords_B_2_a, coords_B_3_a);

        sw = 1.0;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 6; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_B_1_a_g, coords_A_1_a, coords_B_1_a);
        vs[1].grads(gxs[1], coords_A_1_a_g, coords_B_2_a_g, coords_A_1_a, coords_B_2_a);
        vs[2].grads(gxs[2], coords_A_1_a_g, coords_B_3_a_g, coords_A_1_a, coords_B_3_a);
        vs[3].grads(gxs[3], coords_B_1_a_g, coords_B_2_a_g, coords_B_1_a, coords_B_2_a);
        vs[4].grads(gxs[4], coords_B_1_a_g, coords_B_3_a_g, coords_B_1_a, coords_B_3_a);
        vs[5].grads(gxs[5], coords_B_2_a_g, coords_B_3_a_g, coords_B_2_a, coords_B_3_a);

        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0;
        }

        for (size_t i = 0; i < 12; i++) {
            grad1[i + j * 12] += gradients[0 + i];
        }

        if (virial != 0) {
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_B_1_a[0] * coords_B_1_a_g[0] -
                            coords_B_2_a[0] * coords_B_2_a_g[0] - coords_B_3_a[0] * coords_B_3_a_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_B_1_a[0] * coords_B_1_a_g[1] -
                            coords_B_2_a[0] * coords_B_2_a_g[1] - coords_B_3_a[0] * coords_B_3_a_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_B_1_a[0] * coords_B_1_a_g[2] -
                            coords_B_2_a[0] * coords_B_2_a_g[2] - coords_B_3_a[0] * coords_B_3_a_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_B_1_a[1] * coords_B_1_a_g[1] -
                            coords_B_2_a[1] * coords_B_2_a_g[1] - coords_B_3_a[1] * coords_B_3_a_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_B_1_a[1] * coords_B_1_a_g[2] -
                            coords_B_2_a[1] * coords_B_2_a_g[2] - coords_B_3_a[1] * coords_B_3_a_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_B_1_a[2] * coords_B_1_a_g[2] -
                            coords_B_2_a[2] * coords_B_2_a_g[2] - coords_B_3_a[2] * coords_B_3_a_g[2];

            (*virial)[3] = (*virial)[1];
            (*virial)[6] = (*virial)[2];
            (*virial)[7] = (*virial)[5];
        }
    }
    return energies_sw;
}

//----------------------------------------------------------------------------//
}  // namespace mbnrg_A1B3_deg5
