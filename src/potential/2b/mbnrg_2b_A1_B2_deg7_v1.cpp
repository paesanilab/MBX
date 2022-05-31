#include "mbnrg_2b_A1_B2_deg7_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1_B2_deg7 {

mbnrg_A1_B2_deg7_v1::mbnrg_A1_B2_deg7_v1(const std::string mon1, const std::string mon2) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "cs+" and mon2 == "h2") {
        coefficients = std::vector<double>{2.525111079619635e+01,   // 0
                                           -1.541529715447551e+01,  // 1
                                           7.546371184796263e+01,   // 2
                                           1.960630037435289e+01,   // 3
                                           -3.129618192489096e+01,  // 4
                                           -8.828495549215652e+02,  // 5
                                           -6.647614473126559e+02,  // 6
                                           2.220614631828570e+01,   // 7
                                           1.614031403843475e+02,   // 8
                                           -4.469018753839221e+02,  // 9
                                           -2.727480486357374e+02,  // 10
                                           -2.891871985492263e+01,  // 11
                                           1.748708856611420e+02,   // 12
                                           1.348515597398527e+03,   // 13
                                           -6.755385943241849e+00,  // 14
                                           7.988391736485688e+02,   // 15
                                           1.474419942837531e+03,   // 16
                                           -6.009120016515063e+01,  // 17
                                           6.714499567393619e+02,   // 18
                                           1.201295793621010e+03,   // 19
                                           -1.360291673862884e+01,  // 20
                                           -1.139880825555895e+03,  // 21
                                           1.309023144985136e+02,   // 22
                                           -1.005777413433290e+03,  // 23
                                           -4.742530482854073e+01,  // 24
                                           1.106348254226378e+03,   // 25
                                           -1.795144846206065e+02,  // 26
                                           -7.175121318575367e+02,  // 27
                                           4.229097333712442e+01,   // 28
                                           -1.576448253522447e+02,  // 29
                                           -4.872701593006073e+01,  // 30
                                           7.849012423713503e+02,   // 31
                                           1.854924537217182e+01,   // 32
                                           1.035314501251553e+01,   // 33
                                           -1.453438986430064e+03,  // 34
                                           -4.724426152604204e+02,  // 35
                                           -8.298547035772714e+01,  // 36
                                           4.346996027282752e+01,   // 37
                                           -1.042309755897815e+03,  // 38
                                           5.042011671256398e+02,   // 39
                                           -1.374372849939266e+03,  // 40
                                           8.556841251334170e+02,   // 41
                                           -1.317258979875618e+02,  // 42
                                           -5.096280198106554e+02,  // 43
                                           3.903012192747581e+03,   // 44
                                           3.016043231674492e+03,   // 45
                                           -8.012440285504779e+01,  // 46
                                           -1.765856528988609e+03,  // 47
                                           -2.723296361886340e+01,  // 48
                                           -1.563260802019538e+02,  // 49
                                           4.589079489320322e+02,   // 50
                                           3.084279105076653e+02,   // 51
                                           -8.200921150225104e+02,  // 52
                                           2.941938051415485e+03,   // 53
                                           -1.401186594354967e+03,  // 54
                                           3.056450768427462e+03,   // 55
                                           -2.165504027080627e+02,  // 56
                                           3.951346025837410e+01,   // 57
                                           -5.015143743136828e+02,  // 58
                                           -1.799411831447079e+03,  // 59
                                           2.745843730517709e+01,   // 60
                                           4.066180859268553e+02};  // 61
        m_k_x_inter_A_B_0 = 2.795122249962683e-01;                  // A^(-1))
        m_k_x_intra_B_B_1 = 1.086897170421352e-01;                  // A^(-1))
        m_ri = 6.000000000000000e+00;                               // A
        m_ro = 8.000000000000000e+00;                               // A

    }  // end if mon1 == "cs+" and mon2 == "h2"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A1_B2_deg7_v1::f_switch(const double r, double &g) {
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

double mbnrg_A1_B2_deg7_v1::eval(const double *xyz1, const double *xyz2, const size_t n) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(9);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double *mon1 = xyz1 + 3 * j;
        const double *mon2 = xyz2 + 6 * j;

        const double d12[3] = {mon1[0] - mon2[0], mon1[1] - mon2[1], mon1[2] - mon2[2]};

        const double d12rsq = d12[0] * d12[0] + d12[1] * d12[1] + d12[2] * d12[2];
        const double d12r = std::sqrt(d12rsq);

        if (true && d12r > m_ro) {
            continue;
        }

        std::copy(mon1, mon1 + 3, xyz.begin() + 0);

        std::copy(mon2, mon2 + 6, xyz.begin() + 3);

        const double *coords_A_1_a = xyz.data() + 0;

        const double *coords_B_1_b = xyz.data() + 3;

        const double *coords_B_2_b = xyz.data() + 6;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[3];

        double xs[3];

        xs[0] = vs[0].v_exp(m_k_x_inter_A_B_0, coords_A_1_a, coords_B_1_b);
        xs[1] = vs[1].v_exp(m_k_x_inter_A_B_0, coords_A_1_a, coords_B_2_b);
        xs[2] = vs[2].v_exp(m_k_x_intra_B_B_1, coords_B_1_b, coords_B_2_b);

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

double mbnrg_A1_B2_deg7_v1::eval(const double *xyz1, const double *xyz2, double *grad1, double *grad2, const size_t n,
                                 std::vector<double> *virial) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(9);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double *mon1 = xyz1 + 3 * j;
        const double *mon2 = xyz2 + 6 * j;

        const double d12[3] = {mon1[0] - mon2[0], mon1[1] - mon2[1], mon1[2] - mon2[2]};

        const double d12rsq = d12[0] * d12[0] + d12[1] * d12[1] + d12[2] * d12[2];
        const double d12r = std::sqrt(d12rsq);

        if (true && d12r > m_ro) {
            continue;
        }

        std::vector<double> gradients(9, 0.0);

        std::copy(mon1, mon1 + 3, xyz.begin() + 0);

        std::copy(mon2, mon2 + 6, xyz.begin() + 3);
        const double *coords_A_1_a = xyz.data() + 0;

        const double *coords_B_1_b = xyz.data() + 3;

        const double *coords_B_2_b = xyz.data() + 6;

        double *coords_A_1_a_g = gradients.data() + 0;

        double *coords_B_1_b_g = gradients.data() + 3;

        double *coords_B_2_b_g = gradients.data() + 6;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[3];

        double xs[3];

        double gxs[3];

        xs[0] = vs[0].v_exp(m_k_x_inter_A_B_0, coords_A_1_a, coords_B_1_b);
        xs[1] = vs[1].v_exp(m_k_x_inter_A_B_0, coords_A_1_a, coords_B_2_b);
        xs[2] = vs[2].v_exp(m_k_x_intra_B_B_1, coords_B_1_b, coords_B_2_b);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);

        sw = sw12;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 3; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_B_1_b_g, coords_A_1_a, coords_B_1_b);
        vs[1].grads(gxs[1], coords_A_1_a_g, coords_B_2_b_g, coords_A_1_a, coords_B_2_b);
        vs[2].grads(gxs[2], coords_B_1_b_g, coords_B_2_b_g, coords_B_1_b, coords_B_2_b);
        gsw12 *= (1.0) * energies[j] / d12r;

        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0 + (gsw12 * d12[i]);
            gradients[3 + i] += 0.0 - (gsw12 * d12[i]);
        }

        for (size_t i = 0; i < 3; i++) {
            grad1[i + j * 3] += gradients[0 + i];
        }

        for (size_t i = 0; i < 6; i++) {
            grad2[i + j * 6] += gradients[3 + i];
        }

        if (virial != 0) {
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_B_1_b[0] * coords_B_1_b_g[0] -
                            coords_B_2_b[0] * coords_B_2_b_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_B_1_b[0] * coords_B_1_b_g[1] -
                            coords_B_2_b[0] * coords_B_2_b_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_B_1_b[0] * coords_B_1_b_g[2] -
                            coords_B_2_b[0] * coords_B_2_b_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_B_1_b[1] * coords_B_1_b_g[1] -
                            coords_B_2_b[1] * coords_B_2_b_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_B_1_b[1] * coords_B_1_b_g[2] -
                            coords_B_2_b[1] * coords_B_2_b_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_B_1_b[2] * coords_B_1_b_g[2] -
                            coords_B_2_b[2] * coords_B_2_b_g[2];

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
}  // namespace mbnrg_A1_B2_deg7
