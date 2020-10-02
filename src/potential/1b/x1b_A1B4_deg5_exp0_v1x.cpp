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

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>

#include "x1b_A1B4_deg5_exp0_v1x.h"

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

struct variable {
    double v_exp0(const double& r0, const double& k, const double* p1, const double* p2);
    void grads(const double& gg, double* grd1, double* grd2, const double* p1, const double* p2);

    double g[3];  // diff(value, p1 - p2)
};

//----------------------------------------------------------------------------//

void variable::grads(const double& gg, double* grd1, double* grd2, const double* p1, const double* p2) {
    for (size_t i = 0; i < 3; i++) {
        double d = gg * g[i];
        grd1[i] += d;
        grd2[i] -= d;
    }
}

//----------------------------------------------------------------------------//

double variable::v_exp0(const double& r0, const double& k, const double* p1, const double* p2) {
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

    const double exp1 = std::exp(k * (r0 - r));
    const double gg = -k * exp1 / r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace x1b_A1B4_deg5_exp0 {

//----------------------------------------------------------------------------//

x1b_A1B4_v1x::x1b_A1B4_v1x(std::string mon) {
    if (mon == "ch4") {
        coefficients = std::vector<double>{6.732796543406264e+00,   // 0
                                           -3.199534889958855e+01,  // 1
                                           4.816946897414885e+01,   // 2
                                           -1.735134745779326e+01,  // 3
                                           5.540853355085503e+00,   // 4
                                           3.674562567702663e+01,   // 5
                                           -4.815224675927961e+00,  // 6
                                           2.627262702443186e+01,   // 7
                                           5.732794197325295e+00,   // 8
                                           3.852236276775862e-01,   // 9
                                           -7.071051750517825e-01,  // 10
                                           1.735094439321447e+02,   // 11
                                           4.240318189029261e-01,   // 12
                                           3.346439571675577e+01,   // 13
                                           1.014267713093113e+01,   // 14
                                           3.361377434288113e-01,   // 15
                                           2.821471257124354e-01,   // 16
                                           -1.456480521469099e+01,  // 17
                                           1.321788002140712e+01,   // 18
                                           -7.621495711019917e-02,  // 19
                                           -7.802379984267166e+01,  // 20
                                           -1.616553932458161e+01,  // 21
                                           1.344904372417372e+00,   // 22
                                           -1.256503070565662e+00,  // 23
                                           -9.410448598074393e-02,  // 24
                                           3.798034672029592e-01,   // 25
                                           4.330967209297894e+01,   // 26
                                           2.183866694543516e+01,   // 27
                                           -8.126039941137592e-02,  // 28
                                           9.078430267679375e+00,   // 29
                                           -4.059541126214171e+01,  // 30
                                           5.854790581899270e+00,   // 31
                                           -2.029130842284113e+00,  // 32
                                           3.787281154090795e+00,   // 33
                                           1.576893857500565e+01,   // 34
                                           -5.704561559081884e-01,  // 35
                                           -2.178900972070562e+00,  // 36
                                           1.559200715851029e-02,   // 37
                                           3.156086095004381e-02,   // 38
                                           6.326904081498777e-02,   // 39
                                           -2.286316796789887e-01,  // 40
                                           2.403710917102203e-01,   // 41
                                           -1.757672389705301e-01,  // 42
                                           3.040382505360328e-01,   // 43
                                           -3.145294194046978e-01,  // 44
                                           -7.242377416869957e+00,  // 45
                                           3.084742368768087e-03,   // 46
                                           -5.438448291316456e+00,  // 47
                                           3.320891803884619e+00,   // 48
                                           -1.515800929810612e-02,  // 49
                                           -3.012704926864189e-01,  // 50
                                           2.227608734267589e+00,   // 51
                                           -1.747492289346341e-02,  // 52
                                           2.079196435294671e+00,   // 53
                                           1.011539877539035e+00,   // 54
                                           2.079812238294741e-02,   // 55
                                           6.881262140569232e-01,   // 56
                                           1.413493503814983e+01,   // 57
                                           -7.406499875168482e-02,  // 58
                                           -4.075579272665745e-03,  // 59
                                           -2.935633981449055e-02,  // 60
                                           -2.039123267706673e+00,  // 61
                                           -1.015193569221590e-01,  // 62
                                           3.569243764533696e-02,   // 63
                                           -2.709081485107040e-02,  // 64
                                           5.236293401779534e-01,   // 65
                                           -1.957464284805002e+01,  // 66
                                           -4.217848905359178e-03,  // 67
                                           1.241700525065736e-01,   // 68
                                           3.870391959162621e-02,   // 69
                                           2.123509623093697e-01,   // 70
                                           7.010642202316745e+00,   // 71
                                           -1.959670660572543e-03,  // 72
                                           -1.785821535472558e-03,  // 73
                                           8.884096483098933e-03,   // 74
                                           -1.698786911436892e-02,  // 75
                                           -1.108024805007986e-01,  // 76
                                           3.310177769026760e+00,   // 77
                                           -4.327777972641277e-01,  // 78
                                           -2.149470388422807e+01,  // 79
                                           1.120266695274850e-02,   // 80
                                           -4.394838533109841e+00,  // 81
                                           -2.907797423734520e-03,  // 82
                                           -7.198531548679854e-03,  // 83
                                           -9.127625984446805e-04,  // 84
                                           -5.267109779560364e-01,  // 85
                                           -3.436032585206882e-02,  // 86
                                           -1.451211995342315e+00,  // 87
                                           -6.898969301984253e-05,  // 88
                                           1.520655351518767e-01,   // 89
                                           3.198256075972031e-03,   // 90
                                           8.415852340507003e-04,   // 91
                                           7.809737907458543e-03,   // 92
                                           7.642339924466895e-02,   // 93
                                           7.799916517551859e-03,   // 94
                                           -9.658001210522895e-04,  // 95
                                           2.424040749934430e-01,   // 96
                                           -1.316518526625256e-02,  // 97
                                           -5.595533337099026e-03,  // 98
                                           2.000678516101739e+00,   // 99
                                           1.224932041937965e-02,   // 100
                                           1.005651698728719e-02,   // 101
                                           -5.463617520018973e-01,  // 102
                                           -3.104861817581814e-01,  // 103
                                           4.348571007184168e-02,   // 104
                                           -8.342204333674573e-03,  // 105
                                           -1.983407629786179e-03,  // 106
                                           -1.146866462312938e-03,  // 107
                                           -1.738431407977597e-03,  // 108
                                           9.967587104885034e-02,   // 109
                                           -4.476196653825214e-03,  // 110
                                           3.227095065409571e-04,   // 111
                                           -1.193787838748514e-01,  // 112
                                           -9.011258291571671e-02,  // 113
                                           -7.153031617352228e-01,  // 114
                                           5.688314627842994e-02,   // 115
                                           -4.777209144477785e-01,  // 116
                                           1.400827695916126e-03,   // 117
                                           -4.523365845751130e-02,  // 118
                                           3.510131344272691e-01,   // 119
                                           6.251183092430750e-02,   // 120
                                           -3.022986430649115e-01,  // 121
                                           7.745906428374079e-02,   // 122
                                           4.735514317373797e-04,   // 123
                                           7.148767975430169e-04,   // 124
                                           1.988317808359492e-02,   // 125
                                           -8.371944069805408e-01,  // 126
                                           -1.666554459295129e-03,  // 127
                                           3.431820650943669e-01,   // 128
                                           -4.911690815575549e-02,  // 129
                                           4.594264013070892e-02,   // 130
                                           -1.855140770725280e+00,  // 131
                                           -7.840573397754142e-05,  // 132
                                           1.736668873461356e-02,   // 133
                                           4.827597391365735e-04,   // 134
                                           -1.139726252964246e-02,  // 135
                                           -2.265745336730261e-01,  // 136
                                           2.730229829610295e-02,   // 137
                                           7.986704286446672e-02,   // 138
                                           1.869339290420380e-04,   // 139
                                           4.405807486274421e-02,   // 140
                                           -2.931813027998460e-03,  // 141
                                           -1.901480364315465e-03,  // 142
                                           4.164982301854039e-04,   // 143
                                           3.524891107602107e-02,   // 144
                                           -6.120729720899952e-01,  // 145
                                           -2.789468701441157e-03,  // 146
                                           7.118454145512657e-02,   // 147
                                           3.829928880375801e-01,   // 148
                                           1.587357620765208e-02,   // 149
                                           -2.550406267956443e-03,  // 150
                                           -1.218302548869911e-02,  // 151
                                           -4.695328423632971e-05,  // 152
                                           1.688784021124334e-02,   // 153
                                           -1.215749187439004e-02,  // 154
                                           -1.708314447762619e-03,  // 155
                                           1.264181292398335e-03,   // 156
                                           1.241548119723182e-05,   // 157
                                           4.399642610305619e-02,   // 158
                                           3.748915277085309e-02,   // 159
                                           -8.332802943754600e-05,  // 160
                                           1.316492753465578e-04,   // 161
                                           -2.767951476964873e-01,  // 162
                                           -4.981691086816255e-02,  // 163
                                           -3.327540677453101e-03,  // 164
                                           7.028967454465957e-01,   // 165
                                           2.592583368365799e-02,   // 166
                                           4.428125791330868e-02,   // 167
                                           -5.007155155327733e-03,  // 168
                                           -1.946442771915921e-01,  // 169
                                           3.971408188547710e-03,   // 170
                                           5.839885456074572e-04,   // 171
                                           -2.442837205442596e-01,  // 172
                                           4.717055508260014e-02,   // 173
                                           2.340094921455495e-04,   // 174
                                           -3.692867464669407e-03,  // 175
                                           5.424278543680913e-01,   // 176
                                           -4.136058107461059e-03,  // 177
                                           2.212202778768447e-03,   // 178
                                           3.186795711180684e-02,   // 179
                                           -3.530079910621631e-03,  // 180
                                           -1.567972548078817e-03,  // 181
                                           -3.362195133567158e-03,  // 182
                                           2.243624573043021e-04,   // 183
                                           -6.133047593499611e-01,  // 184
                                           1.067259489941310e-01,   // 185
                                           5.274680434240750e-04,   // 186
                                           4.952406309297214e-02,   // 187
                                           6.263551575996416e-02,   // 188
                                           -5.253464580654005e-03,  // 189
                                           -1.012583975155237e-05,  // 190
                                           -2.014131892992360e+00,  // 191
                                           -1.987314713644384e-03,  // 192
                                           2.385771690786483e-02,   // 193
                                           1.084469570162463e-02,   // 194
                                           -2.100047975158793e-03,  // 195
                                           -7.176667579143046e-05,  // 196
                                           -1.497134495348207e-04,  // 197
                                           -6.052851489544695e-02,  // 198
                                           -4.903704766288376e-02,  // 199
                                           2.316904142828253e+00,   // 200
                                           4.783160243588478e-04,   // 201
                                           4.010593839431822e-01,   // 202
                                           1.323626451610253e-01,   // 203
                                           2.227271423508487e-03,   // 204
                                           7.729245079904340e-04,   // 205
                                           6.155006083669020e+00};  // 206

        m_k_AB = 9.118575345478506e-01;  // A^(-1))
        m_d_AB = 4.848036021821232e+00;  // A^(-1))
        m_k_BB = 3.397786901029656e-01;  // A^(-1))
        m_d_BB = 6.999341954887242e+00;  // A^(-1))
    }                                    // end if (mon == "ch4")
}

std::vector<double> x1b_A1B4_v1x::eval(const double* xyz, const size_t nmon) const {
    std::vector<double> energies(nmon, 0.0);

    for (size_t j = 0; j < nmon; j++) {
        double xcrd[15];  // coordinates of real sites ONLY

        std::copy(xyz + j * 15, xyz + (j + 1) * 15, xcrd);

        double v[10];

        const double* A_1 = xcrd + 0;
        const double* B_1 = xcrd + 3;
        const double* B_2 = xcrd + 6;
        const double* B_3 = xcrd + 9;
        const double* B_4 = xcrd + 12;

        variable vr[10];

        v[0] = vr[0].v_exp0(m_d_AB, m_k_AB, A_1, B_1);
        v[1] = vr[1].v_exp0(m_d_AB, m_k_AB, A_1, B_2);
        v[2] = vr[2].v_exp0(m_d_AB, m_k_AB, A_1, B_3);
        v[3] = vr[3].v_exp0(m_d_AB, m_k_AB, A_1, B_4);
        v[4] = vr[4].v_exp0(m_d_BB, m_k_BB, B_1, B_2);
        v[5] = vr[5].v_exp0(m_d_BB, m_k_BB, B_1, B_3);
        v[6] = vr[6].v_exp0(m_d_BB, m_k_BB, B_1, B_4);
        v[7] = vr[7].v_exp0(m_d_BB, m_k_BB, B_2, B_3);
        v[8] = vr[8].v_exp0(m_d_BB, m_k_BB, B_2, B_4);
        v[9] = vr[9].v_exp0(m_d_BB, m_k_BB, B_3, B_4);

        energies[j] = polynomial::eval(coefficients.data(), v);
    }

#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nExiting " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 15; j++) {
            std::cerr << xyz[15 * i + j] << " , ";
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

std::vector<double> x1b_A1B4_v1x::eval(const double* xyz, double* grad, const size_t nmon,
                                       std::vector<double>* virial) const {
#ifdef DEBUG
    std::cerr << std::scientific << std::setprecision(10);
    std::cerr << "\nEntering " << __func__ << " in " << __FILE__ << std::endl;
    std::cerr << "Input coordinates for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 15; j++) {
            std::cerr << xyz[15 * i + j] << " , ";
        }
        std::cerr << std::endl;
    }
    std::cerr << "Input gradients for " << nmon << " monomers:\n";
    for (size_t i = 0; i < nmon; i++) {
        for (size_t j = 0; j < 15; j++) {
            std::cerr << grad[15 * i + j] << " , ";
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
        double xcrd[15];  // coordinates of real sites ONLY

        std::copy(xyz + j * 15, xyz + (j + 1) * 15, xcrd);

        double v[10];

        const double* A_1 = xcrd + 0;
        const double* B_1 = xcrd + 3;
        const double* B_2 = xcrd + 6;
        const double* B_3 = xcrd + 9;
        const double* B_4 = xcrd + 12;

        variable vr[10];

        v[0] = vr[0].v_exp0(m_d_AB, m_k_AB, A_1, B_1);
        v[1] = vr[1].v_exp0(m_d_AB, m_k_AB, A_1, B_2);
        v[2] = vr[2].v_exp0(m_d_AB, m_k_AB, A_1, B_3);
        v[3] = vr[3].v_exp0(m_d_AB, m_k_AB, A_1, B_4);
        v[4] = vr[4].v_exp0(m_d_BB, m_k_BB, B_1, B_2);
        v[5] = vr[5].v_exp0(m_d_BB, m_k_BB, B_1, B_3);
        v[6] = vr[6].v_exp0(m_d_BB, m_k_BB, B_1, B_4);
        v[7] = vr[7].v_exp0(m_d_BB, m_k_BB, B_2, B_3);
        v[8] = vr[8].v_exp0(m_d_BB, m_k_BB, B_2, B_4);
        v[9] = vr[9].v_exp0(m_d_BB, m_k_BB, B_3, B_4);

        double g[10];

        energies[j] = polynomial::eval(coefficients.data(), v, g);

        double xgrd[15];
        std::fill(xgrd, xgrd + 15, 0.0);

        double* A_1_g = xgrd + 0;
        double* B_1_g = xgrd + 3;
        double* B_2_g = xgrd + 6;
        double* B_3_g = xgrd + 9;
        double* B_4_g = xgrd + 12;

        vr[0].grads(g[0], A_1_g, B_1_g, A_1, B_1);
        vr[1].grads(g[1], A_1_g, B_2_g, A_1, B_2);
        vr[2].grads(g[2], A_1_g, B_3_g, A_1, B_3);
        vr[3].grads(g[3], A_1_g, B_4_g, A_1, B_4);
        vr[4].grads(g[4], B_1_g, B_2_g, B_1, B_2);
        vr[5].grads(g[5], B_1_g, B_3_g, B_1, B_3);
        vr[6].grads(g[6], B_1_g, B_4_g, B_1, B_4);
        vr[7].grads(g[7], B_2_g, B_3_g, B_2, B_3);
        vr[8].grads(g[8], B_2_g, B_4_g, B_2, B_4);
        vr[9].grads(g[9], B_3_g, B_4_g, B_3, B_4);

        for (size_t i = 0; i < 15; i++) grad[i + j * 15] = xgrd[i];

        if (virial != 0) {
            (*virial)[0] +=
                -A_1[0] * A_1_g[0] - B_1[0] * B_1_g[0] - B_2[0] * B_2_g[0] - B_3[0] * B_3_g[0] - B_4[0] * B_4_g[0];

            (*virial)[1] +=
                -A_1[0] * A_1_g[1] - B_1[0] * B_1_g[1] - B_2[0] * B_2_g[1] - B_3[0] * B_3_g[1] - B_4[0] * B_4_g[1];

            (*virial)[2] +=
                -A_1[0] * A_1_g[2] - B_1[0] * B_1_g[2] - B_2[0] * B_2_g[2] - B_3[0] * B_3_g[2] - B_4[0] * B_4_g[2];

            (*virial)[4] +=
                -A_1[1] * A_1_g[1] - B_1[1] * B_1_g[1] - B_2[1] * B_2_g[1] - B_3[1] * B_3_g[1] - B_4[1] * B_4_g[1];

            (*virial)[5] +=
                -A_1[1] * A_1_g[2] - B_1[1] * B_1_g[2] - B_2[1] * B_2_g[2] - B_3[1] * B_3_g[2] - B_4[1] * B_4_g[2];

            (*virial)[8] +=
                -A_1[2] * A_1_g[2] - B_1[2] * B_1_g[2] - B_2[2] * B_2_g[2] - B_3[2] * B_3_g[2] - B_4[2] * B_4_g[2];

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
        for (size_t j = 0; j < 15; j++) {
            std::cerr << grad[15 * i + j] << " , ";
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

}  // namespace x1b_A1B4_deg5_exp0

////////////////////////////////////////////////////////////////////////////////
