#include "mbnrg_2b_A2_A2_deg6_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A2_A2_deg6 {

mbnrg_A2_A2_deg6_v1::mbnrg_A2_A2_deg6_v1(const std::string mon1, const std::string mon2) {
    // =====>> BEGIN SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==

    if (mon1 == "h2" and mon2 == "h2") {
        coefficients = std::vector<double>{-4.156459529864483e+01,   // 0
                                           -2.835436835020936e+01,   // 1
                                           -8.525481182927030e+01,   // 2
                                           -3.559038181280243e+02,   // 3
                                           2.123388873945466e+02,    // 4
                                           1.320653320212141e+03,    // 5
                                           -6.567219145816430e+02,   // 6
                                           -2.319730430032536e+03,   // 7
                                           1.670508725923554e+03,    // 8
                                           -6.277011537274019e+02,   // 9
                                           -1.851039091616445e+03,   // 10
                                           -3.902216478831190e+02,   // 11
                                           4.980030887154543e+02,    // 12
                                           1.231251380549588e+03,    // 13
                                           -2.662658032636350e+02,   // 14
                                           1.195301005493694e+03,    // 15
                                           3.651310228856802e+03,    // 16
                                           -1.284522593003416e+03,   // 17
                                           -1.921025658956961e+03,   // 18
                                           -1.203559035014359e+03,   // 19
                                           1.768616186036649e+02,    // 20
                                           -9.457634111097997e+02,   // 21
                                           -1.185461971671837e+03,   // 22
                                           7.062408896593149e+01,    // 23
                                           6.452029889751063e+02,    // 24
                                           -9.531897840309721e+02,   // 25
                                           -2.608308555227185e+03,   // 26
                                           -4.628345478382462e+02,   // 27
                                           8.932632463436973e+02,    // 28
                                           -1.069205457782082e+03,   // 29
                                           6.765284738094070e+02,    // 30
                                           5.253422698026567e+02,    // 31
                                           1.466710488272021e+03,    // 32
                                           3.267063235650753e+02,    // 33
                                           7.159524782514087e+02,    // 34
                                           6.457200197472913e+02,    // 35
                                           -1.538763536581777e+03,   // 36
                                           -8.057718571688730e+02,   // 37
                                           2.197853870894555e+03,    // 38
                                           -5.772082784166466e+02,   // 39
                                           5.854180462648002e+02,    // 40
                                           1.593826828590237e+03,    // 41
                                           -1.919965344305499e+03,   // 42
                                           -9.289165206726533e+02,   // 43
                                           6.017915288895434e+02,    // 44
                                           1.138365877891996e+03,    // 45
                                           -1.006692331898371e+03,   // 46
                                           -5.643165835420226e+02,   // 47
                                           -1.907861997777964e+03,   // 48
                                           -2.368326087016495e+02,   // 49
                                           -1.440717533078094e+03,   // 50
                                           -1.060808234597151e+03,   // 51
                                           1.765850486524559e+02,    // 52
                                           -1.111924079223900e+03,   // 53
                                           -4.334040492700767e+01,   // 54
                                           5.322232086661679e+02,    // 55
                                           3.978416828489361e+02,    // 56
                                           -1.395487917864473e+03,   // 57
                                           6.436802124998712e+02,    // 58
                                           1.471324094970397e+03,    // 59
                                           -6.947635806225748e+02,   // 60
                                           -2.785288776794649e+02,   // 61
                                           1.386376578768814e+03,    // 62
                                           2.093127968324840e+03,    // 63
                                           -4.866581903430819e+02,   // 64
                                           1.359869047303764e+02,    // 65
                                           -1.970237624166946e+03,   // 66
                                           1.892340991198703e+03,    // 67
                                           -1.276311090861617e+03,   // 68
                                           1.755514649630413e+03,    // 69
                                           -3.899587215623895e+02,   // 70
                                           -1.900841041763113e+02,   // 71
                                           -6.518755608360860e+02,   // 72
                                           1.246533620329623e+01,    // 73
                                           1.837849901187047e+02,    // 74
                                           1.213357960219840e+03,    // 75
                                           4.910599063462682e+02,    // 76
                                           3.344130737876234e+03,    // 77
                                           1.236346292810488e+03,    // 78
                                           2.791573084665750e+01,    // 79
                                           -1.717870400223247e+02,   // 80
                                           9.375385179385179e+02,    // 81
                                           -1.447425067650028e+02,   // 82
                                           8.043533372771574e+02,    // 83
                                           3.941814339531420e+03,    // 84
                                           -2.088805248045854e+03,   // 85
                                           -1.438442216009643e+03,   // 86
                                           5.490390145822834e+02,    // 87
                                           5.284143180013700e+02,    // 88
                                           -2.434980401787571e+03,   // 89
                                           -2.085436973767424e+02,   // 90
                                           8.491799215522191e+02,    // 91
                                           -1.937586850925428e+03,   // 92
                                           4.577699521275841e+02,    // 93
                                           1.348980033205314e+03,    // 94
                                           -4.038264348780821e+03,   // 95
                                           1.467145509232588e+03,    // 96
                                           -7.955959564756080e+01,   // 97
                                           -1.776455669515114e+03,   // 98
                                           -4.713603605024207e+02,   // 99
                                           7.979199566646957e+02,    // 100
                                           -5.191141586483815e+02,   // 101
                                           1.466756787208067e+03,    // 102
                                           1.509917707619219e+03,    // 103
                                           -7.654883606335990e+02,   // 104
                                           -9.767986333397355e+02,   // 105
                                           1.663159052900195e+03,    // 106
                                           -4.897918430111698e+03,   // 107
                                           2.150222746635470e+03,    // 108
                                           -4.385961349878015e+02,   // 109
                                           8.754535998577277e+02,    // 110
                                           -5.144078903739917e+02,   // 111
                                           1.193925561369537e+03,    // 112
                                           1.209196485596413e+03,    // 113
                                           -1.146616014523275e+03,   // 114
                                           4.653027636663771e+02,    // 115
                                           3.387632846417737e+02,    // 116
                                           -2.130121205205614e+03,   // 117
                                           2.139658878902953e+02,    // 118
                                           9.252623473944233e+02,    // 119
                                           1.380010763640808e+03,    // 120
                                           -3.205388989030179e+03,   // 121
                                           -7.607311937375178e+01,   // 122
                                           3.596308471624228e+02,    // 123
                                           1.666554302010015e+03,    // 124
                                           -4.850732942692589e+02,   // 125
                                           1.745895823934500e+02,    // 126
                                           1.565872204317440e+03,    // 127
                                           -2.735612272236040e+03,   // 128
                                           9.974970709827535e+02,    // 129
                                           5.771233202629509e+03,    // 130
                                           1.658578869947904e+03,    // 131
                                           4.503924859472941e+02,    // 132
                                           3.802938974323217e+02,    // 133
                                           1.154641231511648e+03,    // 134
                                           4.817856356254378e+02,    // 135
                                           -3.097908210933826e+02,   // 136
                                           -1.482103545999801e+02,   // 137
                                           1.701676943350038e+03,    // 138
                                           5.840877530805307e+02,    // 139
                                           1.517237026619083e+03,    // 140
                                           1.335963118618656e+03,    // 141
                                           -1.085928489742948e+03,   // 142
                                           -7.205585986116276e+02,   // 143
                                           -3.948181789421680e+03,   // 144
                                           1.468095346884188e+03,    // 145
                                           -5.333216103927845e+01,   // 146
                                           1.376436914046871e+03,    // 147
                                           1.774321882321108e+03,    // 148
                                           -8.882273002321723e+02,   // 149
                                           -1.871293516204084e+02};  // 150
        m_k_x_intra_A_A_1 = 8.362418876695092e-01;                   // A^(-1))
        m_k_x_inter_A_A_0 = 9.800118559194722e-01;                   // A^(-1))
        m_ri = 6.000000000000000e+00;                                // A
        m_ro = 8.000000000000000e+00;                                // A

    }  // end if mon1 == "h2" and mon2 == "h2"
    // =====>> END SECTION CONSTRUCTOR <<=====
}

//----------------------------------------------------------------------------//

double mbnrg_A2_A2_deg6_v1::f_switch(const double r, double& g) {
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

double mbnrg_A2_A2_deg6_v1::eval(const double* xyz1, const double* xyz2, const size_t n) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(12);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 6 * j;
        const double* mon2 = xyz2 + 6 * j;

        const double d12[3] = {mon1[0] - mon2[0], mon1[1] - mon2[1], mon1[2] - mon2[2]};

        const double d12rsq = d12[0] * d12[0] + d12[1] * d12[1] + d12[2] * d12[2];
        const double d12r = std::sqrt(d12rsq);

        if (true && d12r > m_ro) {
            continue;
        }

        std::copy(mon1, mon1 + 6, xyz.begin() + 0);

        std::copy(mon2, mon2 + 6, xyz.begin() + 6);

        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_A_2_a = xyz.data() + 3;

        const double* coords_A_3_b = xyz.data() + 6;

        const double* coords_A_4_b = xyz.data() + 9;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[6];

        double xs[6];

        xs[0] = vs[0].v_exp(m_k_x_intra_A_A_1, coords_A_1_a, coords_A_2_a);
        xs[1] = vs[1].v_exp(m_k_x_inter_A_A_0, coords_A_1_a, coords_A_3_b);
        xs[2] = vs[2].v_exp(m_k_x_inter_A_A_0, coords_A_1_a, coords_A_4_b);
        xs[3] = vs[3].v_exp(m_k_x_inter_A_A_0, coords_A_2_a, coords_A_3_b);
        xs[4] = vs[4].v_exp(m_k_x_inter_A_A_0, coords_A_2_a, coords_A_4_b);
        xs[5] = vs[5].v_exp(m_k_x_intra_A_A_1, coords_A_3_b, coords_A_4_b);

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

double mbnrg_A2_A2_deg6_v1::eval(const double* xyz1, const double* xyz2, double* grad1, double* grad2, const size_t n,
                                 std::vector<double>* virial) {
    std::vector<double> energies(n, 0.0);
    std::vector<double> energies_sw(n, 0.0);

    std::vector<double> xyz(12);
    double sw = 0.0;
    polynomial my_poly;

    for (size_t j = 0; j < n; j++) {
        const double* mon1 = xyz1 + 6 * j;
        const double* mon2 = xyz2 + 6 * j;

        const double d12[3] = {mon1[0] - mon2[0], mon1[1] - mon2[1], mon1[2] - mon2[2]};

        const double d12rsq = d12[0] * d12[0] + d12[1] * d12[1] + d12[2] * d12[2];
        const double d12r = std::sqrt(d12rsq);

        if (true && d12r > m_ro) {
            continue;
        }

        std::vector<double> gradients(12, 0.0);

        std::copy(mon1, mon1 + 6, xyz.begin() + 0);

        std::copy(mon2, mon2 + 6, xyz.begin() + 6);
        const double* coords_A_1_a = xyz.data() + 0;

        const double* coords_A_2_a = xyz.data() + 3;

        const double* coords_A_3_b = xyz.data() + 6;

        const double* coords_A_4_b = xyz.data() + 9;

        double* coords_A_1_a_g = gradients.data() + 0;

        double* coords_A_2_a_g = gradients.data() + 3;

        double* coords_A_3_b_g = gradients.data() + 6;

        double* coords_A_4_b_g = gradients.data() + 9;

        double w12 = -9.721486914088159e-02;  // from MBpol
        double w13 = -9.721486914088159e-02;
        double wcross = 9.859272078406150e-02;

        variable vs[6];

        double xs[6];

        double gxs[6];

        xs[0] = vs[0].v_exp(m_k_x_intra_A_A_1, coords_A_1_a, coords_A_2_a);
        xs[1] = vs[1].v_exp(m_k_x_inter_A_A_0, coords_A_1_a, coords_A_3_b);
        xs[2] = vs[2].v_exp(m_k_x_inter_A_A_0, coords_A_1_a, coords_A_4_b);
        xs[3] = vs[3].v_exp(m_k_x_inter_A_A_0, coords_A_2_a, coords_A_3_b);
        xs[4] = vs[4].v_exp(m_k_x_inter_A_A_0, coords_A_2_a, coords_A_4_b);
        xs[5] = vs[5].v_exp(m_k_x_intra_A_A_1, coords_A_3_b, coords_A_4_b);

        double gsw12 = 0.0;
        double sw12 = f_switch(d12r, gsw12);

        sw = sw12;

        energies[j] = my_poly.eval(xs, coefficients.data(), gxs);
        energies_sw[j] = energies[j] * sw;

        for (size_t i = 0; i < 6; i++) {
            gxs[i] *= sw;
        }

        vs[0].grads(gxs[0], coords_A_1_a_g, coords_A_2_a_g, coords_A_1_a, coords_A_2_a);
        vs[1].grads(gxs[1], coords_A_1_a_g, coords_A_3_b_g, coords_A_1_a, coords_A_3_b);
        vs[2].grads(gxs[2], coords_A_1_a_g, coords_A_4_b_g, coords_A_1_a, coords_A_4_b);
        vs[3].grads(gxs[3], coords_A_2_a_g, coords_A_3_b_g, coords_A_2_a, coords_A_3_b);
        vs[4].grads(gxs[4], coords_A_2_a_g, coords_A_4_b_g, coords_A_2_a, coords_A_4_b);
        vs[5].grads(gxs[5], coords_A_3_b_g, coords_A_4_b_g, coords_A_3_b, coords_A_4_b);
        gsw12 *= (1.0) * energies[j] / d12r;

        for (size_t i = 0; i < 3; i++) {
            gradients[0 + i] += 0.0 + (gsw12 * d12[i]);
            gradients[6 + i] += 0.0 - (gsw12 * d12[i]);
        }

        for (size_t i = 0; i < 6; i++) {
            grad1[i + j * 6] += gradients[0 + i];
        }

        for (size_t i = 0; i < 6; i++) {
            grad2[i + j * 6] += gradients[6 + i];
        }

        if (virial != 0) {
            (*virial)[0] += -coords_A_1_a[0] * coords_A_1_a_g[0] - coords_A_2_a[0] * coords_A_2_a_g[0] -
                            coords_A_3_b[0] * coords_A_3_b_g[0] - coords_A_4_b[0] * coords_A_4_b_g[0];

            (*virial)[1] += -coords_A_1_a[0] * coords_A_1_a_g[1] - coords_A_2_a[0] * coords_A_2_a_g[1] -
                            coords_A_3_b[0] * coords_A_3_b_g[1] - coords_A_4_b[0] * coords_A_4_b_g[1];

            (*virial)[2] += -coords_A_1_a[0] * coords_A_1_a_g[2] - coords_A_2_a[0] * coords_A_2_a_g[2] -
                            coords_A_3_b[0] * coords_A_3_b_g[2] - coords_A_4_b[0] * coords_A_4_b_g[2];

            (*virial)[4] += -coords_A_1_a[1] * coords_A_1_a_g[1] - coords_A_2_a[1] * coords_A_2_a_g[1] -
                            coords_A_3_b[1] * coords_A_3_b_g[1] - coords_A_4_b[1] * coords_A_4_b_g[1];

            (*virial)[5] += -coords_A_1_a[1] * coords_A_1_a_g[2] - coords_A_2_a[1] * coords_A_2_a_g[2] -
                            coords_A_3_b[1] * coords_A_3_b_g[2] - coords_A_4_b[1] * coords_A_4_b_g[2];

            (*virial)[8] += -coords_A_1_a[2] * coords_A_1_a_g[2] - coords_A_2_a[2] * coords_A_2_a_g[2] -
                            coords_A_3_b[2] * coords_A_3_b_g[2] - coords_A_4_b[2] * coords_A_4_b_g[2];

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
}  // namespace mbnrg_A2_A2_deg6
