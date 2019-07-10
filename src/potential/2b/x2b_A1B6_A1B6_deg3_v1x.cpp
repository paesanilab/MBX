

#include "x2b_A1B6_A1B6_deg3_v1x.h" 
 

////////////////////////////////////////////////////////////////////////////////

namespace {

struct variable {
    double v_exp0(const double& r0, const double& k,
                 const double * p1, const double * p2 );
                 
    double v_exp(const double& k,
                 const double * p1, const double * p2 );

    double v_coul0(const double& r0, const double& k,
                  const double * p1, const double * p2 );
                  
    double v_coul(const double& k,
                  const double * p1, const double * p2 );

    double v_gau0(const double& r0, const double& k,
                 const double * p1, const double * p2 );
                  
    void grads(const double& gg, double * grd1, double * grd2,
               const double * p1, const double * p2);

    double g[3]; // diff(value, p1 - p2)
};

//----------------------------------------------------------------------------//

double variable::v_gau0(const double& r0, const double& k,
                       const double * p1, const double * p2)
{
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);

    const double exp1 = std::exp(-k*(r0 - r)*(r0 - r));
    const double gg = 2*k*(r0 - r)*exp1/r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_exp0(const double& r0, const double& k,
                       const double * p1, const double * p2)
{
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);

    const double exp1 = std::exp(k*(r0 - r));
    const double gg = - k*exp1/r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_exp(const double& k,
                       const double * p1, const double * p2)
{
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double r = std::sqrt(g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);

    const double exp1 = std::exp(k*(- r));
    const double gg = - k*exp1/r;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return exp1;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //


double variable::v_coul(const double& k,
                        const double * p1, const double * p2)
{
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double rsq = g[0]*g[0] + g[1]*g[1] + g[2]*g[2];
    const double r = std::sqrt(rsq);

    const double exp1 = std::exp(k*(-r));
    const double rinv = 1.0/r;
    const double val = exp1*rinv;

    const double gg = - (k + rinv)*val*rinv;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return val;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

double variable::v_coul0(const double& r0, const double& k,
                        const double * p1, const double * p2)
{
    g[0] = p1[0] - p2[0];
    g[1] = p1[1] - p2[1];
    g[2] = p1[2] - p2[2];

    const double rsq = g[0]*g[0] + g[1]*g[1] + g[2]*g[2];
    const double r = std::sqrt(rsq);

    const double exp1 = std::exp(k*(r0 - r));
    const double rinv = 1.0/r;
    const double val = exp1*rinv;

    const double gg = - (k + rinv)*val*rinv;

    g[0] *= gg;
    g[1] *= gg;
    g[2] *= gg;

    return val;
}

//----------------------------------------------------------------------------//

void variable::grads(const double& gg, double * grd1, double * grd2, 
                     const double * p1, const double * p2) {
    for (size_t i = 0; i < 3 ; i++) {
        double d = gg*g[i];
        grd1[i] += d;
        grd2[i] -= d;
    }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

struct monomer {
    double oh1[3];
    double oh2[3];

    void setup(const double* ohh,
               const double& in_plane_g, const double& out_of_plane_g,
               double x1[3], double x2[3]);

    void grads(const double* g1, const double* g2,
               const double& in_plane_g, const double& out_of_plane_g,
               double* grd) const;
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

void monomer::setup(const double* ohh,
                    const double& in_plane_g, const double& out_of_plane_g,
                    double* x1, double* x2)
{
    for (int i = 0; i < 3; ++i) {
        oh1[i] = ohh[i + 3] - ohh[i];
        oh2[i] = ohh[i + 6] - ohh[i];
    }

    const double v[3] = {
        oh1[1]*oh2[2] - oh1[2]*oh2[1],
        oh1[2]*oh2[0] - oh1[0]*oh2[2],
        oh1[0]*oh2[1] - oh1[1]*oh2[0]
    };

    for (int i = 0; i < 3; ++i) {
        const double in_plane = ohh[i] + 0.5*in_plane_g*(oh1[i] + oh2[i]);
        const double out_of_plane = out_of_plane_g*v[i];

        x1[i] = in_plane + out_of_plane;
        x2[i] = in_plane - out_of_plane;
    }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

void monomer::grads(const double* g1, const double* g2,
                    const double& in_plane_g, const double& out_of_plane_g,
                    double* grd) const
{
    const double gm[3] = {
        g1[0] - g2[0],
        g1[1] - g2[1],
        g1[2] - g2[2]
    };

    const double t1[3] = {
        oh2[1]*gm[2] - oh2[2]*gm[1],
        oh2[2]*gm[0] - oh2[0]*gm[2],
        oh2[0]*gm[1] - oh2[1]*gm[0]
    };

    const double t2[3] = {
        oh1[1]*gm[2] - oh1[2]*gm[1],
        oh1[2]*gm[0] - oh1[0]*gm[2],
        oh1[0]*gm[1] - oh1[1]*gm[0]
    };

    for (int i = 0; i < 3; ++i) {
        const double gsum = g1[i] + g2[i];
        const double in_plane = 0.5*in_plane_g*gsum;

        const double gh1 = in_plane + out_of_plane_g*t1[i];
        const double gh2 = in_plane - out_of_plane_g*t2[i];

        grd[i + 0] += gsum - (gh1 + gh2); // O
        grd[i + 3] += gh1; // H1
        grd[i + 6] += gh2; // H2
    }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

//struct vsites {
//    //void TwoParticleAverageSite() {}
//    //void ThreeParticleAverageSite() {}
//    void OutOfPlaneSite(const double& w12, const double& w13,
//                        const double& wcross, const double x1[3],
//                        const double y1[3], const double y2[3],
//                        double vs[3]);
//    //void LocalCoordinatesSite{}
//};
//
//void vsites::OutOfPlaneSite(const double& w12,
//                            const double& w13,
//                            const double& wcross,
//                            const double x1[3],
//                            const double y1[3],
//                            const double y2[3],
//                            double vs[3]) {
//    double r12[3], r13[3];
//
//    for (int i = 0; i < 3; ++i) {
//        r12[i] = y1[i] - x1[i];
//        r13[i] = y2[i] - x1[i];
//    }
//                            
//    double rc[3];
//    rc[0] = r12[1]*r13[2] - r12[2]*r13[1];
//    rc[1] = r12[2]*r13[0] - r12[0]*r13[2];
//    rc[2] = r12[0]*r13[1] - r12[1]*r13[0];
//    
//    vs[0] = x1[0] + w12 * r12[0] + w13 * r13[0] + wcross * rc[0];
//    vs[1] = x1[1] + w12 * r12[1] + w13 * r13[1] + wcross * rc[1];
//    vs[2] = x1[2] + w12 * r12[2] + w13 * r13[2] + wcross * rc[2];
//}

} // namespace

////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B6_A1B6_deg3 {

//----------------------------------------------------------------------------//

x2b_A1B6_A1B6_v1x::x2b_A1B6_A1B6_v1x(std::string mon1, std::string mon2) {

    // =====>> SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==
    if (mon1 == "pf6-" && mon2  == "pf6-") {
        coefficients = std::vector<double> { 
            -2.639823366916608e+02, // 0
            -4.126194680391475e+02, // 1
            -1.835967963050870e+02, // 2
            -4.867578299689653e+02, // 3
            -8.291219273657514e+02, // 4
            -2.802077753342287e+03, // 5
             5.127035586417202e+02, // 6
            -1.895828508906955e+03, // 7
            -6.705906702961948e+02, // 8
            -1.909673463553796e+02, // 9
            -1.643356864198834e+03, // 10
             1.953531090884596e+02, // 11
             6.223343449717348e+02, // 12
             1.526167274140012e+02, // 13
            -2.025189199569201e+02, // 14
             3.612417796023308e+02, // 15
            -8.924188462648601e+01, // 16
            -6.262092484964875e+02, // 17
             5.684032567484436e+02, // 18
             7.439306701472450e+00, // 19
            -8.769354123217155e+00, // 20
            -2.158852632261257e+02, // 21
            -4.323943709887124e+02, // 22
             5.369387700690741e+02, // 23
             5.369016512653777e+02, // 24
            -6.306172130925506e+02, // 25
             9.435976028174273e+02, // 26
             5.724139062195930e+01, // 27
             3.807977599893858e+02, // 28
            -7.018328973660134e+01, // 29
             3.015502056693022e+02, // 30
            -7.586736832810179e+01, // 31
             3.003931591457412e+02, // 32
            -2.185679668998267e+01, // 33
             2.403068985196989e+02, // 34
            -2.694146316813902e+02, // 35
            -1.508331266970718e+02, // 36
            -2.639711043608363e+02, // 37
             1.734685627019485e+04, // 38
            -3.713440253055300e+02, // 39
             2.682536974174201e+03, // 40
            -2.587674978242645e+03, // 41
             3.906053063862180e+02, // 42
            -7.677611307183604e+03, // 43
             3.498168040586726e+03, // 44
             8.427196495030994e+03, // 45
            -3.959578667632607e+03, // 46
            -6.466308038615440e+03, // 47
             7.798321875998884e+01, // 48
            -2.381178814750513e+03, // 49
            -3.203345909095113e+02, // 50
             1.272953019014189e+03, // 51
             2.148378760206172e+04, // 52
             2.996004838463188e+03, // 53
             4.657164207017697e+02, // 54
             4.979476197179684e+03, // 55
             3.709785380405004e+03, // 56
             6.556764858194103e+02, // 57
            -4.389889525841439e+03, // 58
             8.968010811618380e+03, // 59
            -2.662776642077781e+03, // 60
             1.960335666784098e+03, // 61
             3.516802698458418e+03, // 62
            -9.420281566201639e+02, // 63
             9.228763725035296e+02, // 64
             1.104051049088814e+03, // 65
            -1.178424436304193e+03, // 66
             1.213607765427731e+03, // 67
             1.679227961021969e+03, // 68
             1.143782557580700e+03, // 69
            -4.528657800858442e+03, // 70
            -3.578311327257288e+03, // 71
             1.230866912357562e+03, // 72
            -9.181396333904202e+03, // 73
            -1.704558258524601e+03, // 74
             1.385855148691781e+03, // 75
            -5.327873195624935e+01, // 76
             2.748174431201458e+02, // 77
            -2.224683467925978e+01, // 78
            -2.102509311564571e+02, // 79
             9.971514533272750e+02, // 80
            -2.150018816680448e+01, // 81
            -3.391977814872938e+01, // 82
            -4.390956460956373e+03, // 83
            -4.679853218615592e+02, // 84
            -1.932699042837087e+02, // 85
             2.889449784652558e+01, // 86
             7.180563117356857e+02, // 87
            -8.494071363906177e+01, // 88
            -6.596078313432856e+02, // 89
            -5.629268548571018e+02, // 90
            -3.174910385183285e+03, // 91
             1.890173912823389e+02, // 92
             2.321946185417087e+03, // 93
             4.072230459579568e+02, // 94
            -4.596159967054160e+02, // 95
             3.718170717634785e+03, // 96
            -1.444262218595984e+02, // 97
            -1.751388197779895e+03, // 98
             1.055506467833912e+03, // 99
             2.137133940273618e+02, // 100
             1.319705343160751e+02, // 101
             3.077933997537495e+03, // 102
            -6.066624617960767e+02, // 103
            -1.420248747648108e+03, // 104
            -7.263634833242810e+02, // 105
             1.265316939726799e+03, // 106
             1.539358714346245e+02, // 107
             4.927450412896600e+02, // 108
             1.393891116488910e+03, // 109
            -1.925581155820556e+03, // 110
            -1.613548957518509e+02, // 111
             4.651404890517838e+02, // 112
             7.662279933710037e+01, // 113
            -1.408102645884889e+02, // 114
            -1.294838567212986e+03, // 115
             1.719344377012427e+03, // 116
            -5.199506858344751e+02, // 117
             5.068120971630200e+02, // 118
            -5.443094729892964e+02, // 119
            -1.953263263988100e+02, // 120
             3.139113989049873e+01, // 121
             1.220481242277335e+02, // 122
             4.219133104157503e+02, // 123
             4.858865332268402e+02, // 124
            -4.306074399122278e+02, // 125
            -1.498814375687498e+02, // 126
            -1.820565780794576e+02, // 127
            -1.183713522718589e+02, // 128
            -4.311670651169460e+01, // 129
            -1.148078766725496e+02, // 130
            -2.565818936563704e+02, // 131
             2.717934518785387e+02, // 132
             1.898791609790700e+02, // 133
            -1.523560027150246e+02, // 134
             1.072187140098686e+02, // 135
            -1.105468972089269e+02, // 136
             4.289181215876739e+02, // 137
            -1.729373668088454e+02, // 138
             9.048831631184318e+01, // 139
             3.038429437122694e+03, // 140
             3.108887819990341e+02, // 141
             3.583733633561681e+02, // 142
             2.445866567200776e+02, // 143
             2.790706769760337e+03, // 144
             3.489581862730940e+02, // 145
             1.395061603182488e+03, // 146
            -1.374775154475995e+03, // 147
            -5.183809852178326e+03, // 148
             1.253787770593169e+03, // 149
             5.090406381185971e+02, // 150
            -2.565863176358436e+02, // 151
            -7.236562858884522e+02, // 152
            -1.065495252543026e+03, // 153
             4.124122732129574e+02, // 154
             9.464511586054072e+02, // 155
            -6.902454656399210e+02, // 156
             2.389795596043811e+03, // 157
            -1.972310069943561e+03, // 158
             5.971469707877029e+02, // 159
            -1.925885571976482e+03, // 160
             1.277653564047395e+03, // 161
            -1.401241667880901e-01, // 162
            -2.826053015840193e+03, // 163
             3.140895252530939e+02, // 164
             2.202551381185935e+02, // 165
             8.625768011599523e+02, // 166
            -2.793381263587510e+02, // 167
            -5.706890652623377e+02, // 168
            -1.949198554498454e+02, // 169
            -3.067065143698961e+02, // 170
            -1.542622201300123e+02, // 171
            -1.074833401344904e+03, // 172
             6.151353450089534e+02, // 173
            -2.536012523616441e+02, // 174
            -8.182060744771371e+01, // 175
            -1.288206928297965e+01, // 176
            -1.376534997882277e+03, // 177
             7.991676387013311e+01, // 178
            -3.389829702243613e+02, // 179
            -2.304141606829190e+02, // 180
             4.214097366085095e+01, // 181
            -9.677895835663767e+01, // 182
            -7.206863556544855e+02, // 183
             2.648757045490819e+02, // 184
            -2.192234554057906e+02, // 185
            -1.498456018538646e+02, // 186
             2.202873902924404e+01, // 187
             5.998366070617978e+00, // 188
             6.214403534589054e+01, // 189
             1.085345745903045e+02, // 190
             3.007112420615877e+02, // 191
            -4.498011411341641e+02, // 192
             1.035769797699967e+02, // 193
             8.661615247786747e+02, // 194
            -3.909550195142799e+02, // 195
             3.030183799899040e+02, // 196
            -3.949176478569722e+02, // 197
             8.074079408809760e+02, // 198
            -6.445992926466719e+02, // 199
            -5.955374803411222e+01, // 200
             3.090343058410401e+02, // 201
            -2.740934170993458e+01, // 202
             6.470230642068019e+01, // 203
             8.227960616996384e+00, // 204
             1.830373258487871e+02, // 205
             1.802709546043868e+02, // 206
             1.395045063341840e+03, // 207
            -2.337330306715484e+03, // 208
            -1.575984745739300e+02, // 209
            -7.131180394603850e+02, // 210
             1.124560938686993e+03, // 211
            -1.048790692354950e+02, // 212
             1.115223316931502e+02, // 213
             6.741324193954458e+01, // 214
             4.064898953031391e+01, // 215
            -8.465391096423639e+01, // 216
            -6.994106415819915e+01, // 217
            -5.559653724107126e+01, // 218
            -2.855918076990304e+02, // 219
            -1.913699524313908e+02, // 220
             3.021504028553254e+01, // 221
             1.507727513177251e+02, // 222
            -1.408079469017156e+02, // 223
             2.686226203327452e+02, // 224
             4.272312123730045e+01, // 225
            -3.146549928531763e+01, // 226
            -8.991997195235595e+01, // 227
             3.489265454078840e+01, // 228
             4.977789268409948e+01, // 229
            -1.639466168156369e+01, // 230
            -5.718073830758752e+02, // 231
            -4.366261739662532e+02, // 232
             2.512330353370854e+02, // 233
             2.199882455354369e+02, // 234
             3.023387360469646e+02, // 235
            -3.683393567338018e+02, // 236
             1.508481817691048e+02, // 237
            -2.426048486275612e+02, // 238
             1.088073034704104e+02, // 239
            -4.603171772033856e+01, // 240
            -2.387948482515278e+01, // 241
            -5.342480327486315e+01, // 242
             1.101242566431623e+02, // 243
             1.781537481935047e+01, // 244
            -2.920389300106008e+01, // 245
            -6.329303001222989e+01, // 246
             9.020737789275771e+01}; // 247

    m_k_AA =  1.442666558135639e-01; // A^(-1))
    m_k_AB =  4.731583599584525e-01; // A^(-1))
    m_k_BB =  1.181008411096585e+00; // A^(-1))
    m_k_intra_BB =  5.759365675989001e-01; // A^(-1))
    m_k_intra_AB =  2.626589484798270e-01; // A^(-1))
    m_r2i =  7.000000000000000e+00; // A
    m_r2f =  8.000000000000000e+00; // A

    } // if mon1 == "pf6-" && mon2  == "pf6-"

}

//----------------------------------------------------------------------------//

double x2b_A1B6_A1B6_v1x::f_switch(const double& r, double& g) const
{
    if (r > m_r2f) {
        g = 0.0;
        return 0.0;
    } else if (r > m_r2i) {
        const double t1 = M_PI/(m_r2f - m_r2i);
        const double x = (r - m_r2i)*t1;
        g = - std::sin(x)*t1/2.0;
        return (1.0 + std::cos(x))/2.0;
    } else {
        g = 0.0;
        return 1.0;
    }
}

//----------------------------------------------------------------------------//

double x2b_A1B6_A1B6_v1x::eval(const double* xyz1, const double* xyz2, const size_t ndim) const
{

    std::vector<double> energies(ndim,0.0);

    for (size_t j = 0; j < ndim; j++) {
        double mon1[21];
        double mon2[21];

        std::copy(xyz1 + j * 21, xyz1 + (j+1) * 21, mon1);
        std::copy(xyz2 + j * 21, xyz2 + (j+1) * 21, mon2);

        // Right now it assumes 1st atom of each monomer
        const double d12[3] = {mon1[0] -  mon2[0],
                               mon1[1] -  mon2[1],
                               mon1[2] -  mon2[2]};
    
        const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double r12 = std::sqrt(r12sq);
    
        if (r12 > m_r2f)
            continue;
    
        double xcrd[42]; // coordinates of real sites ONLY
    
        std::copy(mon1, mon1 + 21, xcrd);
        std::copy(mon2, mon2 + 21, xcrd + 21);
        
        double v[91];
        
        double sw = 0.0;
        double gsw = 0.0;
    
        const double* A_1_a= xcrd + 0;
        const double* B_1_a= xcrd + 3;
        const double* B_2_a= xcrd + 6;
        const double* B_3_a= xcrd + 9;
        const double* B_4_a= xcrd + 12;
        const double* B_5_a= xcrd + 15;
        const double* B_6_a= xcrd + 18;


        const double* A_1_b= xcrd + 21;
        const double* B_1_b= xcrd + 24;
        const double* B_2_b= xcrd + 27;
        const double* B_3_b= xcrd + 30;
        const double* B_4_b= xcrd + 33;
        const double* B_5_b= xcrd + 36;
        const double* B_6_b= xcrd + 39;


    
        variable vr[91];
    
        v[0]  = vr[0].v_exp(m_k_intra_BB, B_1_a, B_2_a);
        v[1]  = vr[1].v_exp(m_k_intra_BB, B_1_a, B_3_a);
        v[2]  = vr[2].v_exp(m_k_intra_BB, B_1_a, B_4_a);
        v[3]  = vr[3].v_exp(m_k_intra_BB, B_1_a, B_5_a);
        v[4]  = vr[4].v_exp(m_k_intra_BB, B_1_a, B_6_a);
        v[5]  = vr[5].v_exp(m_k_intra_BB, B_2_a, B_3_a);
        v[6]  = vr[6].v_exp(m_k_intra_BB, B_2_a, B_4_a);
        v[7]  = vr[7].v_exp(m_k_intra_BB, B_2_a, B_5_a);
        v[8]  = vr[8].v_exp(m_k_intra_BB, B_2_a, B_6_a);
        v[9]  = vr[9].v_exp(m_k_intra_BB, B_3_a, B_4_a);
        v[10]  = vr[10].v_exp(m_k_intra_BB, B_3_a, B_5_a);
        v[11]  = vr[11].v_exp(m_k_intra_BB, B_3_a, B_6_a);
        v[12]  = vr[12].v_exp(m_k_intra_BB, B_4_a, B_5_a);
        v[13]  = vr[13].v_exp(m_k_intra_BB, B_4_a, B_6_a);
        v[14]  = vr[14].v_exp(m_k_intra_BB, B_5_a, B_6_a);
        v[15]  = vr[15].v_exp(m_k_intra_AB, A_1_a, B_1_a);
        v[16]  = vr[16].v_exp(m_k_intra_AB, A_1_a, B_2_a);
        v[17]  = vr[17].v_exp(m_k_intra_AB, A_1_a, B_3_a);
        v[18]  = vr[18].v_exp(m_k_intra_AB, A_1_a, B_4_a);
        v[19]  = vr[19].v_exp(m_k_intra_AB, A_1_a, B_5_a);
        v[20]  = vr[20].v_exp(m_k_intra_AB, A_1_a, B_6_a);
        v[21]  = vr[21].v_exp(m_k_intra_BB, B_1_b, B_2_b);
        v[22]  = vr[22].v_exp(m_k_intra_BB, B_1_b, B_3_b);
        v[23]  = vr[23].v_exp(m_k_intra_BB, B_1_b, B_4_b);
        v[24]  = vr[24].v_exp(m_k_intra_BB, B_1_b, B_5_b);
        v[25]  = vr[25].v_exp(m_k_intra_BB, B_1_b, B_6_b);
        v[26]  = vr[26].v_exp(m_k_intra_BB, B_2_b, B_3_b);
        v[27]  = vr[27].v_exp(m_k_intra_BB, B_2_b, B_4_b);
        v[28]  = vr[28].v_exp(m_k_intra_BB, B_2_b, B_5_b);
        v[29]  = vr[29].v_exp(m_k_intra_BB, B_2_b, B_6_b);
        v[30]  = vr[30].v_exp(m_k_intra_BB, B_3_b, B_4_b);
        v[31]  = vr[31].v_exp(m_k_intra_BB, B_3_b, B_5_b);
        v[32]  = vr[32].v_exp(m_k_intra_BB, B_3_b, B_6_b);
        v[33]  = vr[33].v_exp(m_k_intra_BB, B_4_b, B_5_b);
        v[34]  = vr[34].v_exp(m_k_intra_BB, B_4_b, B_6_b);
        v[35]  = vr[35].v_exp(m_k_intra_BB, B_5_b, B_6_b);
        v[36]  = vr[36].v_exp(m_k_intra_AB, A_1_b, B_1_b);
        v[37]  = vr[37].v_exp(m_k_intra_AB, A_1_b, B_2_b);
        v[38]  = vr[38].v_exp(m_k_intra_AB, A_1_b, B_3_b);
        v[39]  = vr[39].v_exp(m_k_intra_AB, A_1_b, B_4_b);
        v[40]  = vr[40].v_exp(m_k_intra_AB, A_1_b, B_5_b);
        v[41]  = vr[41].v_exp(m_k_intra_AB, A_1_b, B_6_b);
        v[42]  = vr[42].v_exp(m_k_AA, A_1_a, A_1_b);
        v[43]  = vr[43].v_exp(m_k_AB, A_1_a, B_1_b);
        v[44]  = vr[44].v_exp(m_k_AB, A_1_a, B_2_b);
        v[45]  = vr[45].v_exp(m_k_AB, A_1_a, B_3_b);
        v[46]  = vr[46].v_exp(m_k_AB, A_1_a, B_4_b);
        v[47]  = vr[47].v_exp(m_k_AB, A_1_a, B_5_b);
        v[48]  = vr[48].v_exp(m_k_AB, A_1_a, B_6_b);
        v[49]  = vr[49].v_exp(m_k_AB, B_1_a, A_1_b);
        v[50]  = vr[50].v_exp(m_k_AB, B_2_a, A_1_b);
        v[51]  = vr[51].v_exp(m_k_AB, B_3_a, A_1_b);
        v[52]  = vr[52].v_exp(m_k_AB, B_4_a, A_1_b);
        v[53]  = vr[53].v_exp(m_k_AB, B_5_a, A_1_b);
        v[54]  = vr[54].v_exp(m_k_AB, B_6_a, A_1_b);
        v[55]  = vr[55].v_exp(m_k_BB, B_1_a, B_1_b);
        v[56]  = vr[56].v_exp(m_k_BB, B_1_a, B_2_b);
        v[57]  = vr[57].v_exp(m_k_BB, B_1_a, B_3_b);
        v[58]  = vr[58].v_exp(m_k_BB, B_1_a, B_4_b);
        v[59]  = vr[59].v_exp(m_k_BB, B_1_a, B_5_b);
        v[60]  = vr[60].v_exp(m_k_BB, B_1_a, B_6_b);
        v[61]  = vr[61].v_exp(m_k_BB, B_2_a, B_1_b);
        v[62]  = vr[62].v_exp(m_k_BB, B_2_a, B_2_b);
        v[63]  = vr[63].v_exp(m_k_BB, B_2_a, B_3_b);
        v[64]  = vr[64].v_exp(m_k_BB, B_2_a, B_4_b);
        v[65]  = vr[65].v_exp(m_k_BB, B_2_a, B_5_b);
        v[66]  = vr[66].v_exp(m_k_BB, B_2_a, B_6_b);
        v[67]  = vr[67].v_exp(m_k_BB, B_3_a, B_1_b);
        v[68]  = vr[68].v_exp(m_k_BB, B_3_a, B_2_b);
        v[69]  = vr[69].v_exp(m_k_BB, B_3_a, B_3_b);
        v[70]  = vr[70].v_exp(m_k_BB, B_3_a, B_4_b);
        v[71]  = vr[71].v_exp(m_k_BB, B_3_a, B_5_b);
        v[72]  = vr[72].v_exp(m_k_BB, B_3_a, B_6_b);
        v[73]  = vr[73].v_exp(m_k_BB, B_4_a, B_1_b);
        v[74]  = vr[74].v_exp(m_k_BB, B_4_a, B_2_b);
        v[75]  = vr[75].v_exp(m_k_BB, B_4_a, B_3_b);
        v[76]  = vr[76].v_exp(m_k_BB, B_4_a, B_4_b);
        v[77]  = vr[77].v_exp(m_k_BB, B_4_a, B_5_b);
        v[78]  = vr[78].v_exp(m_k_BB, B_4_a, B_6_b);
        v[79]  = vr[79].v_exp(m_k_BB, B_5_a, B_1_b);
        v[80]  = vr[80].v_exp(m_k_BB, B_5_a, B_2_b);
        v[81]  = vr[81].v_exp(m_k_BB, B_5_a, B_3_b);
        v[82]  = vr[82].v_exp(m_k_BB, B_5_a, B_4_b);
        v[83]  = vr[83].v_exp(m_k_BB, B_5_a, B_5_b);
        v[84]  = vr[84].v_exp(m_k_BB, B_5_a, B_6_b);
        v[85]  = vr[85].v_exp(m_k_BB, B_6_a, B_1_b);
        v[86]  = vr[86].v_exp(m_k_BB, B_6_a, B_2_b);
        v[87]  = vr[87].v_exp(m_k_BB, B_6_a, B_3_b);
        v[88]  = vr[88].v_exp(m_k_BB, B_6_a, B_4_b);
        v[89]  = vr[89].v_exp(m_k_BB, B_6_a, B_5_b);
        v[90]  = vr[90].v_exp(m_k_BB, B_6_a, B_6_b);
     
    
        sw = f_switch(r12, gsw);
        
        energies[j] = sw*polynomial::eval(coefficients.data(), v);
    }

    double energy = 0.0;
    for (size_t i = 0; i < ndim; i++) {
      energy += energies[i];
    }

    return energy;
    
}

double x2b_A1B6_A1B6_v1x::eval(const double* xyz1, const double* xyz2, 
                double * grad1, double * grad2, const size_t ndim) const
{

    std::vector<double> energies(ndim,0.0);

    for (size_t j = 0; j < ndim; j++) {
        double mon1[21];
        double mon2[21];

        std::copy(xyz1 + j * 21, xyz1 + (j+1) * 21, mon1);
        std::copy(xyz2 + j * 21, xyz2 + (j+1) * 21, mon2);

        // Right now it assumes 1st atom of each monomer
        const double d12[3] = {mon1[0] -  mon2[0],
                               mon1[1] -  mon2[1],
                               mon1[2] -  mon2[2]};
    
        const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double r12 = std::sqrt(r12sq);
    
        if (r12 > m_r2f)
            continue;
    
        double xcrd[42]; // coordinates of real sites ONLY
    
        std::copy(mon1, mon1 + 21, xcrd);
        std::copy(mon2, mon2 + 21, xcrd + 21);
        
        double v[91];
        
        double sw = 0.0;
        double gsw = 0.0;
    
        const double* A_1_a= xcrd + 0;
        const double* B_1_a= xcrd + 3;
        const double* B_2_a= xcrd + 6;
        const double* B_3_a= xcrd + 9;
        const double* B_4_a= xcrd + 12;
        const double* B_5_a= xcrd + 15;
        const double* B_6_a= xcrd + 18;


        const double* A_1_b= xcrd + 21;
        const double* B_1_b= xcrd + 24;
        const double* B_2_b= xcrd + 27;
        const double* B_3_b= xcrd + 30;
        const double* B_4_b= xcrd + 33;
        const double* B_5_b= xcrd + 36;
        const double* B_6_b= xcrd + 39;


    
        variable vr[91];
    
        v[0]  = vr[0].v_exp(m_k_intra_BB, B_1_a, B_2_a);
        v[1]  = vr[1].v_exp(m_k_intra_BB, B_1_a, B_3_a);
        v[2]  = vr[2].v_exp(m_k_intra_BB, B_1_a, B_4_a);
        v[3]  = vr[3].v_exp(m_k_intra_BB, B_1_a, B_5_a);
        v[4]  = vr[4].v_exp(m_k_intra_BB, B_1_a, B_6_a);
        v[5]  = vr[5].v_exp(m_k_intra_BB, B_2_a, B_3_a);
        v[6]  = vr[6].v_exp(m_k_intra_BB, B_2_a, B_4_a);
        v[7]  = vr[7].v_exp(m_k_intra_BB, B_2_a, B_5_a);
        v[8]  = vr[8].v_exp(m_k_intra_BB, B_2_a, B_6_a);
        v[9]  = vr[9].v_exp(m_k_intra_BB, B_3_a, B_4_a);
        v[10]  = vr[10].v_exp(m_k_intra_BB, B_3_a, B_5_a);
        v[11]  = vr[11].v_exp(m_k_intra_BB, B_3_a, B_6_a);
        v[12]  = vr[12].v_exp(m_k_intra_BB, B_4_a, B_5_a);
        v[13]  = vr[13].v_exp(m_k_intra_BB, B_4_a, B_6_a);
        v[14]  = vr[14].v_exp(m_k_intra_BB, B_5_a, B_6_a);
        v[15]  = vr[15].v_exp(m_k_intra_AB, A_1_a, B_1_a);
        v[16]  = vr[16].v_exp(m_k_intra_AB, A_1_a, B_2_a);
        v[17]  = vr[17].v_exp(m_k_intra_AB, A_1_a, B_3_a);
        v[18]  = vr[18].v_exp(m_k_intra_AB, A_1_a, B_4_a);
        v[19]  = vr[19].v_exp(m_k_intra_AB, A_1_a, B_5_a);
        v[20]  = vr[20].v_exp(m_k_intra_AB, A_1_a, B_6_a);
        v[21]  = vr[21].v_exp(m_k_intra_BB, B_1_b, B_2_b);
        v[22]  = vr[22].v_exp(m_k_intra_BB, B_1_b, B_3_b);
        v[23]  = vr[23].v_exp(m_k_intra_BB, B_1_b, B_4_b);
        v[24]  = vr[24].v_exp(m_k_intra_BB, B_1_b, B_5_b);
        v[25]  = vr[25].v_exp(m_k_intra_BB, B_1_b, B_6_b);
        v[26]  = vr[26].v_exp(m_k_intra_BB, B_2_b, B_3_b);
        v[27]  = vr[27].v_exp(m_k_intra_BB, B_2_b, B_4_b);
        v[28]  = vr[28].v_exp(m_k_intra_BB, B_2_b, B_5_b);
        v[29]  = vr[29].v_exp(m_k_intra_BB, B_2_b, B_6_b);
        v[30]  = vr[30].v_exp(m_k_intra_BB, B_3_b, B_4_b);
        v[31]  = vr[31].v_exp(m_k_intra_BB, B_3_b, B_5_b);
        v[32]  = vr[32].v_exp(m_k_intra_BB, B_3_b, B_6_b);
        v[33]  = vr[33].v_exp(m_k_intra_BB, B_4_b, B_5_b);
        v[34]  = vr[34].v_exp(m_k_intra_BB, B_4_b, B_6_b);
        v[35]  = vr[35].v_exp(m_k_intra_BB, B_5_b, B_6_b);
        v[36]  = vr[36].v_exp(m_k_intra_AB, A_1_b, B_1_b);
        v[37]  = vr[37].v_exp(m_k_intra_AB, A_1_b, B_2_b);
        v[38]  = vr[38].v_exp(m_k_intra_AB, A_1_b, B_3_b);
        v[39]  = vr[39].v_exp(m_k_intra_AB, A_1_b, B_4_b);
        v[40]  = vr[40].v_exp(m_k_intra_AB, A_1_b, B_5_b);
        v[41]  = vr[41].v_exp(m_k_intra_AB, A_1_b, B_6_b);
        v[42]  = vr[42].v_exp(m_k_AA, A_1_a, A_1_b);
        v[43]  = vr[43].v_exp(m_k_AB, A_1_a, B_1_b);
        v[44]  = vr[44].v_exp(m_k_AB, A_1_a, B_2_b);
        v[45]  = vr[45].v_exp(m_k_AB, A_1_a, B_3_b);
        v[46]  = vr[46].v_exp(m_k_AB, A_1_a, B_4_b);
        v[47]  = vr[47].v_exp(m_k_AB, A_1_a, B_5_b);
        v[48]  = vr[48].v_exp(m_k_AB, A_1_a, B_6_b);
        v[49]  = vr[49].v_exp(m_k_AB, B_1_a, A_1_b);
        v[50]  = vr[50].v_exp(m_k_AB, B_2_a, A_1_b);
        v[51]  = vr[51].v_exp(m_k_AB, B_3_a, A_1_b);
        v[52]  = vr[52].v_exp(m_k_AB, B_4_a, A_1_b);
        v[53]  = vr[53].v_exp(m_k_AB, B_5_a, A_1_b);
        v[54]  = vr[54].v_exp(m_k_AB, B_6_a, A_1_b);
        v[55]  = vr[55].v_exp(m_k_BB, B_1_a, B_1_b);
        v[56]  = vr[56].v_exp(m_k_BB, B_1_a, B_2_b);
        v[57]  = vr[57].v_exp(m_k_BB, B_1_a, B_3_b);
        v[58]  = vr[58].v_exp(m_k_BB, B_1_a, B_4_b);
        v[59]  = vr[59].v_exp(m_k_BB, B_1_a, B_5_b);
        v[60]  = vr[60].v_exp(m_k_BB, B_1_a, B_6_b);
        v[61]  = vr[61].v_exp(m_k_BB, B_2_a, B_1_b);
        v[62]  = vr[62].v_exp(m_k_BB, B_2_a, B_2_b);
        v[63]  = vr[63].v_exp(m_k_BB, B_2_a, B_3_b);
        v[64]  = vr[64].v_exp(m_k_BB, B_2_a, B_4_b);
        v[65]  = vr[65].v_exp(m_k_BB, B_2_a, B_5_b);
        v[66]  = vr[66].v_exp(m_k_BB, B_2_a, B_6_b);
        v[67]  = vr[67].v_exp(m_k_BB, B_3_a, B_1_b);
        v[68]  = vr[68].v_exp(m_k_BB, B_3_a, B_2_b);
        v[69]  = vr[69].v_exp(m_k_BB, B_3_a, B_3_b);
        v[70]  = vr[70].v_exp(m_k_BB, B_3_a, B_4_b);
        v[71]  = vr[71].v_exp(m_k_BB, B_3_a, B_5_b);
        v[72]  = vr[72].v_exp(m_k_BB, B_3_a, B_6_b);
        v[73]  = vr[73].v_exp(m_k_BB, B_4_a, B_1_b);
        v[74]  = vr[74].v_exp(m_k_BB, B_4_a, B_2_b);
        v[75]  = vr[75].v_exp(m_k_BB, B_4_a, B_3_b);
        v[76]  = vr[76].v_exp(m_k_BB, B_4_a, B_4_b);
        v[77]  = vr[77].v_exp(m_k_BB, B_4_a, B_5_b);
        v[78]  = vr[78].v_exp(m_k_BB, B_4_a, B_6_b);
        v[79]  = vr[79].v_exp(m_k_BB, B_5_a, B_1_b);
        v[80]  = vr[80].v_exp(m_k_BB, B_5_a, B_2_b);
        v[81]  = vr[81].v_exp(m_k_BB, B_5_a, B_3_b);
        v[82]  = vr[82].v_exp(m_k_BB, B_5_a, B_4_b);
        v[83]  = vr[83].v_exp(m_k_BB, B_5_a, B_5_b);
        v[84]  = vr[84].v_exp(m_k_BB, B_5_a, B_6_b);
        v[85]  = vr[85].v_exp(m_k_BB, B_6_a, B_1_b);
        v[86]  = vr[86].v_exp(m_k_BB, B_6_a, B_2_b);
        v[87]  = vr[87].v_exp(m_k_BB, B_6_a, B_3_b);
        v[88]  = vr[88].v_exp(m_k_BB, B_6_a, B_4_b);
        v[89]  = vr[89].v_exp(m_k_BB, B_6_a, B_5_b);
        v[90]  = vr[90].v_exp(m_k_BB, B_6_a, B_6_b);
     
    
        double g[91];

        // the switch
        sw = f_switch(r12, gsw);
        
        energies[j] = sw*polynomial::eval(coefficients.data(), v, g);
        
        double xgrd[42];
        std::fill(xgrd, xgrd + 42, 0.0);

        double* A_1_a_g= xgrd + 0;
        double* B_1_a_g= xgrd + 3;
        double* B_2_a_g= xgrd + 6;
        double* B_3_a_g= xgrd + 9;
        double* B_4_a_g= xgrd + 12;
        double* B_5_a_g= xgrd + 15;
        double* B_6_a_g= xgrd + 18;

        double* A_1_b_g= xgrd + 21;
        double* B_1_b_g= xgrd + 24;
        double* B_2_b_g= xgrd + 27;
        double* B_3_b_g= xgrd + 30;
        double* B_4_b_g= xgrd + 33;
        double* B_5_b_g= xgrd + 36;
        double* B_6_b_g= xgrd + 39;



        vr[0].grads(g[0], B_1_a_g, B_2_a_g, B_1_a, B_2_a);
        vr[1].grads(g[1], B_1_a_g, B_3_a_g, B_1_a, B_3_a);
        vr[2].grads(g[2], B_1_a_g, B_4_a_g, B_1_a, B_4_a);
        vr[3].grads(g[3], B_1_a_g, B_5_a_g, B_1_a, B_5_a);
        vr[4].grads(g[4], B_1_a_g, B_6_a_g, B_1_a, B_6_a);
        vr[5].grads(g[5], B_2_a_g, B_3_a_g, B_2_a, B_3_a);
        vr[6].grads(g[6], B_2_a_g, B_4_a_g, B_2_a, B_4_a);
        vr[7].grads(g[7], B_2_a_g, B_5_a_g, B_2_a, B_5_a);
        vr[8].grads(g[8], B_2_a_g, B_6_a_g, B_2_a, B_6_a);
        vr[9].grads(g[9], B_3_a_g, B_4_a_g, B_3_a, B_4_a);
        vr[10].grads(g[10], B_3_a_g, B_5_a_g, B_3_a, B_5_a);
        vr[11].grads(g[11], B_3_a_g, B_6_a_g, B_3_a, B_6_a);
        vr[12].grads(g[12], B_4_a_g, B_5_a_g, B_4_a, B_5_a);
        vr[13].grads(g[13], B_4_a_g, B_6_a_g, B_4_a, B_6_a);
        vr[14].grads(g[14], B_5_a_g, B_6_a_g, B_5_a, B_6_a);
        vr[15].grads(g[15], A_1_a_g, B_1_a_g, A_1_a, B_1_a);
        vr[16].grads(g[16], A_1_a_g, B_2_a_g, A_1_a, B_2_a);
        vr[17].grads(g[17], A_1_a_g, B_3_a_g, A_1_a, B_3_a);
        vr[18].grads(g[18], A_1_a_g, B_4_a_g, A_1_a, B_4_a);
        vr[19].grads(g[19], A_1_a_g, B_5_a_g, A_1_a, B_5_a);
        vr[20].grads(g[20], A_1_a_g, B_6_a_g, A_1_a, B_6_a);
        vr[21].grads(g[21], B_1_b_g, B_2_b_g, B_1_b, B_2_b);
        vr[22].grads(g[22], B_1_b_g, B_3_b_g, B_1_b, B_3_b);
        vr[23].grads(g[23], B_1_b_g, B_4_b_g, B_1_b, B_4_b);
        vr[24].grads(g[24], B_1_b_g, B_5_b_g, B_1_b, B_5_b);
        vr[25].grads(g[25], B_1_b_g, B_6_b_g, B_1_b, B_6_b);
        vr[26].grads(g[26], B_2_b_g, B_3_b_g, B_2_b, B_3_b);
        vr[27].grads(g[27], B_2_b_g, B_4_b_g, B_2_b, B_4_b);
        vr[28].grads(g[28], B_2_b_g, B_5_b_g, B_2_b, B_5_b);
        vr[29].grads(g[29], B_2_b_g, B_6_b_g, B_2_b, B_6_b);
        vr[30].grads(g[30], B_3_b_g, B_4_b_g, B_3_b, B_4_b);
        vr[31].grads(g[31], B_3_b_g, B_5_b_g, B_3_b, B_5_b);
        vr[32].grads(g[32], B_3_b_g, B_6_b_g, B_3_b, B_6_b);
        vr[33].grads(g[33], B_4_b_g, B_5_b_g, B_4_b, B_5_b);
        vr[34].grads(g[34], B_4_b_g, B_6_b_g, B_4_b, B_6_b);
        vr[35].grads(g[35], B_5_b_g, B_6_b_g, B_5_b, B_6_b);
        vr[36].grads(g[36], A_1_b_g, B_1_b_g, A_1_b, B_1_b);
        vr[37].grads(g[37], A_1_b_g, B_2_b_g, A_1_b, B_2_b);
        vr[38].grads(g[38], A_1_b_g, B_3_b_g, A_1_b, B_3_b);
        vr[39].grads(g[39], A_1_b_g, B_4_b_g, A_1_b, B_4_b);
        vr[40].grads(g[40], A_1_b_g, B_5_b_g, A_1_b, B_5_b);
        vr[41].grads(g[41], A_1_b_g, B_6_b_g, A_1_b, B_6_b);
        vr[42].grads(g[42], A_1_a_g, A_1_b_g, A_1_a, A_1_b);
        vr[43].grads(g[43], A_1_a_g, B_1_b_g, A_1_a, B_1_b);
        vr[44].grads(g[44], A_1_a_g, B_2_b_g, A_1_a, B_2_b);
        vr[45].grads(g[45], A_1_a_g, B_3_b_g, A_1_a, B_3_b);
        vr[46].grads(g[46], A_1_a_g, B_4_b_g, A_1_a, B_4_b);
        vr[47].grads(g[47], A_1_a_g, B_5_b_g, A_1_a, B_5_b);
        vr[48].grads(g[48], A_1_a_g, B_6_b_g, A_1_a, B_6_b);
        vr[49].grads(g[49], B_1_a_g, A_1_b_g, B_1_a, A_1_b);
        vr[50].grads(g[50], B_2_a_g, A_1_b_g, B_2_a, A_1_b);
        vr[51].grads(g[51], B_3_a_g, A_1_b_g, B_3_a, A_1_b);
        vr[52].grads(g[52], B_4_a_g, A_1_b_g, B_4_a, A_1_b);
        vr[53].grads(g[53], B_5_a_g, A_1_b_g, B_5_a, A_1_b);
        vr[54].grads(g[54], B_6_a_g, A_1_b_g, B_6_a, A_1_b);
        vr[55].grads(g[55], B_1_a_g, B_1_b_g, B_1_a, B_1_b);
        vr[56].grads(g[56], B_1_a_g, B_2_b_g, B_1_a, B_2_b);
        vr[57].grads(g[57], B_1_a_g, B_3_b_g, B_1_a, B_3_b);
        vr[58].grads(g[58], B_1_a_g, B_4_b_g, B_1_a, B_4_b);
        vr[59].grads(g[59], B_1_a_g, B_5_b_g, B_1_a, B_5_b);
        vr[60].grads(g[60], B_1_a_g, B_6_b_g, B_1_a, B_6_b);
        vr[61].grads(g[61], B_2_a_g, B_1_b_g, B_2_a, B_1_b);
        vr[62].grads(g[62], B_2_a_g, B_2_b_g, B_2_a, B_2_b);
        vr[63].grads(g[63], B_2_a_g, B_3_b_g, B_2_a, B_3_b);
        vr[64].grads(g[64], B_2_a_g, B_4_b_g, B_2_a, B_4_b);
        vr[65].grads(g[65], B_2_a_g, B_5_b_g, B_2_a, B_5_b);
        vr[66].grads(g[66], B_2_a_g, B_6_b_g, B_2_a, B_6_b);
        vr[67].grads(g[67], B_3_a_g, B_1_b_g, B_3_a, B_1_b);
        vr[68].grads(g[68], B_3_a_g, B_2_b_g, B_3_a, B_2_b);
        vr[69].grads(g[69], B_3_a_g, B_3_b_g, B_3_a, B_3_b);
        vr[70].grads(g[70], B_3_a_g, B_4_b_g, B_3_a, B_4_b);
        vr[71].grads(g[71], B_3_a_g, B_5_b_g, B_3_a, B_5_b);
        vr[72].grads(g[72], B_3_a_g, B_6_b_g, B_3_a, B_6_b);
        vr[73].grads(g[73], B_4_a_g, B_1_b_g, B_4_a, B_1_b);
        vr[74].grads(g[74], B_4_a_g, B_2_b_g, B_4_a, B_2_b);
        vr[75].grads(g[75], B_4_a_g, B_3_b_g, B_4_a, B_3_b);
        vr[76].grads(g[76], B_4_a_g, B_4_b_g, B_4_a, B_4_b);
        vr[77].grads(g[77], B_4_a_g, B_5_b_g, B_4_a, B_5_b);
        vr[78].grads(g[78], B_4_a_g, B_6_b_g, B_4_a, B_6_b);
        vr[79].grads(g[79], B_5_a_g, B_1_b_g, B_5_a, B_1_b);
        vr[80].grads(g[80], B_5_a_g, B_2_b_g, B_5_a, B_2_b);
        vr[81].grads(g[81], B_5_a_g, B_3_b_g, B_5_a, B_3_b);
        vr[82].grads(g[82], B_5_a_g, B_4_b_g, B_5_a, B_4_b);
        vr[83].grads(g[83], B_5_a_g, B_5_b_g, B_5_a, B_5_b);
        vr[84].grads(g[84], B_5_a_g, B_6_b_g, B_5_a, B_6_b);
        vr[85].grads(g[85], B_6_a_g, B_1_b_g, B_6_a, B_1_b);
        vr[86].grads(g[86], B_6_a_g, B_2_b_g, B_6_a, B_2_b);
        vr[87].grads(g[87], B_6_a_g, B_3_b_g, B_6_a, B_3_b);
        vr[88].grads(g[88], B_6_a_g, B_4_b_g, B_6_a, B_4_b);
        vr[89].grads(g[89], B_6_a_g, B_5_b_g, B_6_a, B_5_b);
        vr[90].grads(g[90], B_6_a_g, B_6_b_g, B_6_a, B_6_b);


    // ##DEFINE HERE## the redistribution of the gradients
    

    
        for (int i = 0; i < 21; ++i) {
            grad1[i + j*21] += sw*xgrd[i];
        }

        for (int i = 0; i < 21; ++i) {
            grad2[i + j*21] += sw*xgrd[i + 21];
        }

        // gradient of the switch

        gsw *= energies[j]/r12;
        for (int i = 0; i < 3; ++i) {
            const double d = gsw*d12[i];
            grad1[i + j*21] += d;
            grad2[i + j*21] -= d;
        }

    }

    double energy = 0.0;
    for (size_t i = 0; i < ndim; i++) {
      energy += energies[i];
    }

    return energy;
}

} // namespace x2b_A1B6_A1B6_deg3

////////////////////////////////////////////////////////////////////////////////
