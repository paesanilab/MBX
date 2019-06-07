#include "x1b_A1B4_deg5_v1x.h" 
 

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

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

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

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


//----------------------------------------------------------------------------//

void variable::grads(const double& gg, double * grd1, double * grd2, 
                     const double * p1, const double * p2) {
    for (size_t i = 0; i < 3 ; i++) {
        double d = gg*g[i];
        grd1[i] += d;
        grd2[i] -= d;
    }
}

//----------------------------------------------------------------------------//

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

namespace x1b_A1B4_deg5 {

//----------------------------------------------------------------------------//



x1b_A1B4_v1x::x1b_A1B4_v1x(std::string mon) {


    // =====>> SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <== 
        if (mon == "nh4+") {
        coefficients = std::vector<double> { 
            -6.148598860488139e+02, // 0
             5.209135656892887e+02, // 1
             1.543809143387429e+03, // 2
            -1.563526990333822e+03, // 3
             2.593437100322274e+02, // 4
            -4.254024891732873e+02, // 5
            -6.460103369640569e+02, // 6
             6.175657756442957e+02, // 7
            -6.939823646754515e+02, // 8
             5.965254688964784e+02, // 9
             5.574345833291875e+02, // 10
            -2.533488879736459e+02, // 11
            -2.282327600141727e+01, // 12
            -3.686940402577628e+02, // 13
             3.257532359389988e+02, // 14
             1.192820159865518e+03, // 15
            -1.821143209299858e+03, // 16
             1.475735278478795e+02, // 17
             8.439757401496730e+02, // 18
            -1.065776727547311e+03, // 19
            -1.463047255822178e+01, // 20
            -2.149348512326401e+02, // 21
             4.055289209104459e+02, // 22
             1.028297508338643e+03, // 23
            -3.452557637819190e+02, // 24
            -4.790585804735468e+01, // 25
             6.704245632762860e+02, // 26
            -3.147703313101899e+02, // 27
            -5.345431842349586e+02, // 28
             1.969086451296410e+03, // 29
             2.909979251341341e+02, // 30
             7.610352152082650e+02, // 31
            -2.985433308796882e+02, // 32
             1.827428982335341e+02, // 33
             2.870335814061150e+02, // 34
            -2.041256638698777e+02, // 35
             2.589079576907351e+02, // 36
             8.113045043080905e+02, // 37
            -8.528197980537892e+02, // 38
            -5.606846971912478e+02, // 39
            -4.169183676219765e+01, // 40
             5.032513526479947e+02, // 41
             6.036733045815829e+01, // 42
             1.301644689730002e+03, // 43
            -8.117432126284427e+02, // 44
             3.189607330792629e+02, // 45
             6.546158648291076e+02, // 46
             6.305590442254299e+02, // 47
             5.077881450787604e+02, // 48
            -7.227441360399324e+02, // 49
            -7.057390150209612e+02, // 50
             8.132426250554907e+02, // 51
             4.398230844494389e+02, // 52
             8.879260839538655e+01, // 53
            -6.360268724304462e+02, // 54
             1.164152400082376e+02, // 55
             3.475800561842025e-01, // 56
            -3.562536448333919e+02, // 57
            -8.372399001445021e+02, // 58
             1.004510662822643e+03, // 59
             9.477775817424136e+02, // 60
            -4.864074297932142e+02, // 61
             8.060464289748988e+02, // 62
             1.787155915420646e+03, // 63
            -1.924576192906400e+02, // 64
            -3.366165518339370e+02, // 65
            -1.429563219335490e+03, // 66
             1.568641778684869e+03, // 67
            -9.274684431032103e+02, // 68
             1.748515322598838e+03, // 69
            -1.736748408601354e+03, // 70
            -1.458324601402352e+03, // 71
             1.055105305612514e+03, // 72
             1.941078964585229e+02, // 73
             4.723390519227437e+02, // 74
            -2.784393783332316e+02, // 75
             7.743954804199800e+02, // 76
            -4.923403343243850e+02, // 77
            -6.099679819073954e+02, // 78
            -3.411443762786890e+02, // 79
            -3.949885324720059e+01, // 80
            -6.900960008881575e+02, // 81
             4.554596906819153e+03, // 82
             7.428092752467415e+02, // 83
             5.651290564784970e+02, // 84
            -4.425989996189667e+02, // 85
             1.115624686960774e+02, // 86
             2.539823372749246e+02, // 87
             1.090530787623635e+03, // 88
             4.800377440600228e+02, // 89
             8.258365661075688e+01, // 90
             1.000473559330793e+03, // 91
            -6.223962537627652e+01, // 92
            -3.802156501003724e+02, // 93
            -1.409162919527501e+03, // 94
             1.224087466084152e+03, // 95
            -3.131517218610990e+02, // 96
             1.008049896593605e+03, // 97
             8.408526606970305e+02, // 98
            -4.737452287158817e+02, // 99
            -6.532132264852332e+02, // 100
            -8.274571234175196e+02, // 101
             1.962205781838729e+02, // 102
             9.770552717054536e+02, // 103
             1.648561155791810e+03, // 104
             2.141763203233980e+02, // 105
             1.602141109128281e+03, // 106
             2.351066338311291e+02, // 107
             7.263212578240750e+02, // 108
             6.367028390213568e+02, // 109
             7.128975798836690e+02, // 110
             3.792992849729737e+02, // 111
             6.178729864557399e+02, // 112
             4.179851854281039e+03, // 113
             5.533853325881191e+02, // 114
             8.751350374820001e+01, // 115
            -4.812740957468914e+02, // 116
            -8.862018474749721e+01, // 117
             2.095385532103545e+02, // 118
            -6.290398418415327e+02, // 119
             1.082416864981599e+02, // 120
             1.487855177343360e+02, // 121
             1.856008735037873e+01, // 122
             6.481351585041192e+02, // 123
             1.809267837606433e+03, // 124
             6.406572508706925e+02, // 125
            -4.628016403114155e+01, // 126
             1.156558926718013e+02, // 127
             7.290984995585945e+02, // 128
             5.134541956855008e+02, // 129
            -1.351019691551744e+03, // 130
             1.966684674072291e+02, // 131
            -2.841844193322596e+02, // 132
            -4.280622906836338e+02, // 133
             4.769547203298372e+02, // 134
             9.367779286103745e+01, // 135
            -1.708265023646712e+03, // 136
            -3.911685377478896e+02, // 137
             7.194925758755048e+02, // 138
            -7.041562870173744e+02, // 139
             3.421883191772284e+02, // 140
             1.348165596569828e+03, // 141
            -7.280179983931241e+02, // 142
            -2.584305590874799e+02, // 143
            -1.031812194630973e+03, // 144
             2.250420449263796e+02, // 145
            -6.460824213074732e+02, // 146
             1.745180310382134e+03, // 147
            -1.453757900999359e+03, // 148
             1.857720214031527e+03, // 149
             4.884949589734408e+02, // 150
            -1.824985836280659e+03, // 151
             1.868091264268011e+03, // 152
             8.911564666961772e+02, // 153
             1.234525710874513e+03, // 154
            -1.149676476735348e+03, // 155
             8.882110013504652e+01, // 156
             8.668647750863468e+01, // 157
            -1.469377091731724e+03, // 158
            -5.770794067780639e+02, // 159
             4.183322385099954e+02, // 160
             5.591395301372814e+01, // 161
             8.557638392800771e+02, // 162
            -3.471967702748294e+02, // 163
             6.196226953172165e+01, // 164
             3.388919652184848e+02, // 165
            -1.220426846892731e+03, // 166
             1.606734345601136e+02, // 167
            -1.320961402330686e+03, // 168
             3.137240865627384e+02, // 169
             6.890892716948880e+02, // 170
            -1.622406903147866e+03, // 171
             7.098114921256633e+02, // 172
            -1.589203049802014e+02, // 173
             2.174178507291227e+02, // 174
            -7.271412876966190e+02, // 175
            -2.686591506983584e+02, // 176
            -3.320914544580660e+02, // 177
            -5.230896676924147e+02, // 178
             2.008298767188983e+03, // 179
            -3.441906847522694e+02, // 180
            -8.939882928936823e+02, // 181
             1.167877358617883e+03, // 182
             6.689073436742524e+02, // 183
            -8.583197342913396e+02, // 184
             1.483672161944835e+03, // 185
            -4.122036938228844e+02, // 186
            -6.972381381018546e+02, // 187
            -8.897415547276243e+01, // 188
            -5.399514254535884e+02, // 189
             5.378153992787712e+02, // 190
            -3.933972441837219e+02, // 191
            -9.312591482872571e+02, // 192
            -2.066182943878594e+02, // 193
            -1.141326782813641e+03, // 194
            -8.556881656194047e+00, // 195
            -3.559092397901225e+01, // 196
            -1.395791791263981e+03, // 197
            -7.153715424601147e+02, // 198
             4.640277563037704e+01, // 199
            -4.217771845794684e+02, // 200
            -2.253056441182956e+02, // 201
            -1.426231815896882e+03, // 202
             6.947156509087034e+02, // 203
            -8.220569050996359e+02, // 204
            -2.416337669120024e+02, // 205
            -1.626626132344886e+02}; // 206

    m_k_BB =  4.203658580893818e-01; // A^(-1))
    m_k_AB =  1.446992671968345e+00; // A^(-1))

    } // end if mon == "nh4+"

}

//----------------------------------------------------------------------------//

std::vector<double> x1b_A1B4_v1x::eval(const double* xyz, const size_t nmon) const
{

    std::vector<double> energies(nmon,0.0);

    for (size_t j = 0; j < nmon; j++) {

        double xcrd[15]; // coordinates of real sites ONLY

        std::copy(xyz + j*15, xyz + (j+1)*15, xcrd);
    
        double v[10];
    
        const double* A_1_a= xcrd + 0;
        const double* B_1_a= xcrd + 3;
        const double* B_2_a= xcrd + 6;
        const double* B_3_a= xcrd + 9;
        const double* B_4_a= xcrd + 12;



        variable vr[10];

        v[0]  = vr[0].v_exp(m_k_BB, B_1_a, B_2_a);
        v[1]  = vr[1].v_exp(m_k_BB, B_1_a, B_3_a);
        v[2]  = vr[2].v_exp(m_k_BB, B_1_a, B_4_a);
        v[3]  = vr[3].v_exp(m_k_BB, B_2_a, B_3_a);
        v[4]  = vr[4].v_exp(m_k_BB, B_2_a, B_4_a);
        v[5]  = vr[5].v_exp(m_k_BB, B_3_a, B_4_a);
        v[6]  = vr[6].v_exp(m_k_AB, A_1_a, B_1_a);
        v[7]  = vr[7].v_exp(m_k_AB, A_1_a, B_2_a);
        v[8]  = vr[8].v_exp(m_k_AB, A_1_a, B_3_a);
        v[9]  = vr[9].v_exp(m_k_AB, A_1_a, B_4_a);

     
    
        energies[j] = polynomial::eval(coefficients.data(), v);

    }

    return energies;
}

std::vector<double> x1b_A1B4_v1x::eval(const double* xyz, 
                double * grad, const size_t nmon) const
{

    std::vector<double> energies(nmon,0.0);

    for (size_t j = 0; j < nmon; j++) {

        double xcrd[15]; // coordinates of real sites ONLY

        std::copy(xyz + j*15, xyz + (j+1)*15, xcrd);

        double v[10];
    
        const double* A_1_a= xcrd + 0;
        const double* B_1_a= xcrd + 3;
        const double* B_2_a= xcrd + 6;
        const double* B_3_a= xcrd + 9;
        const double* B_4_a= xcrd + 12;



        variable vr[10];

        v[0]  = vr[0].v_exp(m_k_BB, B_1_a, B_2_a);
        v[1]  = vr[1].v_exp(m_k_BB, B_1_a, B_3_a);
        v[2]  = vr[2].v_exp(m_k_BB, B_1_a, B_4_a);
        v[3]  = vr[3].v_exp(m_k_BB, B_2_a, B_3_a);
        v[4]  = vr[4].v_exp(m_k_BB, B_2_a, B_4_a);
        v[5]  = vr[5].v_exp(m_k_BB, B_3_a, B_4_a);
        v[6]  = vr[6].v_exp(m_k_AB, A_1_a, B_1_a);
        v[7]  = vr[7].v_exp(m_k_AB, A_1_a, B_2_a);
        v[8]  = vr[8].v_exp(m_k_AB, A_1_a, B_3_a);
        v[9]  = vr[9].v_exp(m_k_AB, A_1_a, B_4_a);

     
    
        double g[10];
        
        energies[j] = polynomial::eval(coefficients.data(), v, g);
        
        double xgrd[15];
        std::fill(xgrd, xgrd + 15, 0.0);

        double* A_1_a_g= xgrd + 0;
        double* B_1_a_g= xgrd + 3;
        double* B_2_a_g= xgrd + 6;
        double* B_3_a_g= xgrd + 9;
        double* B_4_a_g= xgrd + 12;


        vr[0].grads(g[0], B_1_a_g, B_2_a_g, B_1_a, B_2_a);
        vr[1].grads(g[1], B_1_a_g, B_3_a_g, B_1_a, B_3_a);
        vr[2].grads(g[2], B_1_a_g, B_4_a_g, B_1_a, B_4_a);
        vr[3].grads(g[3], B_2_a_g, B_3_a_g, B_2_a, B_3_a);
        vr[4].grads(g[4], B_2_a_g, B_4_a_g, B_2_a, B_4_a);
        vr[5].grads(g[5], B_3_a_g, B_4_a_g, B_3_a, B_4_a);
        vr[6].grads(g[6], A_1_a_g, B_1_a_g, A_1_a, B_1_a);
        vr[7].grads(g[7], A_1_a_g, B_2_a_g, A_1_a, B_2_a);
        vr[8].grads(g[8], A_1_a_g, B_3_a_g, A_1_a, B_3_a);
        vr[9].grads(g[9], A_1_a_g, B_4_a_g, A_1_a, B_4_a);


        for (size_t i = 0; i < 15; i++)
            grad[i + j*15] = xgrd[i];

    }

    return energies;
}

} // namespace x1b_A1B4_deg5

////////////////////////////////////////////////////////////////////////////////
