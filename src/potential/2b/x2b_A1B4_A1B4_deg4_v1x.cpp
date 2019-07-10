

#include "x2b_A1B4_A1B4_deg4_v1x.h" 
 

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

namespace x2b_A1B4_A1B4_deg4 {

//----------------------------------------------------------------------------//

x2b_A1B4_A1B4_v1x::x2b_A1B4_A1B4_v1x(std::string mon1, std::string mon2) {

    // =====>> SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==
    if (mon1 == "nh4+" && mon2  == "nh4+") {
        coefficients = std::vector<double> { 
            -1.258162706686234e+01, // 0
             5.872791163130529e+02, // 1
             4.355011508165553e+02, // 2
             1.326958842325910e+02, // 3
            -3.913565474073841e+01, // 4
             2.664686352108820e+01, // 5
             2.153953274934437e+02, // 6
             1.033846358886397e+02, // 7
            -1.060049878436181e+03, // 8
            -1.079611681812854e+02, // 9
             2.257469910633830e+01, // 10
             7.568862047908951e+01, // 11
            -2.211741282828226e+02, // 12
             2.002888764566030e+02, // 13
             1.405006218174538e+03, // 14
            -1.203702287233747e+02, // 15
            -1.103962378885418e+03, // 16
            -1.056414441460958e+03, // 17
             6.385385017430773e+02, // 18
            -1.243679106195327e+02, // 19
             1.334615185809697e+03, // 20
            -2.745474615189796e+01, // 21
             5.505068167374426e+02, // 22
            -4.352541880806919e+02, // 23
            -2.283493413046065e+02, // 24
            -3.721904739921831e+02, // 25
            -2.318896155234081e+03, // 26
             1.220402690410962e+03, // 27
            -2.521442186403552e+03, // 28
             1.963064509944475e+03, // 29
             7.766178907692272e+02, // 30
            -1.830857021083576e+03, // 31
             3.871304194076276e+02, // 32
            -1.352676619771494e+03, // 33
             1.405927322474099e+03, // 34
            -6.483081756642003e+02, // 35
             8.280113611946665e+01, // 36
             1.856699882628584e+02, // 37
            -9.052729151026368e+02, // 38
            -1.609336910754211e+02, // 39
            -7.352074746158067e+01, // 40
            -4.906505735643082e+00, // 41
             3.975822883928418e+02, // 42
             3.430462192009072e+01, // 43
            -3.603933745252344e+02, // 44
             9.356425193715997e+01, // 45
            -7.617845782998901e+01, // 46
            -1.398745095288941e+02, // 47
             6.734531130421767e+02, // 48
             2.203151061887170e+01, // 49
             5.814006423795980e+01, // 50
             8.910436406372412e+01, // 51
            -1.092702657083815e+03, // 52
             8.530919362085332e+01, // 53
             2.274914375709717e+03, // 54
             6.533991311204629e+02, // 55
             1.523494771599648e+02, // 56
            -1.134603434549202e+02, // 57
             4.527253238897414e+02, // 58
            -1.155445918998397e+03, // 59
            -7.929448287656339e+01, // 60
             4.621816844025914e+02, // 61
            -3.509178509227352e+02, // 62
            -5.871567210618606e+02, // 63
             2.158066721235833e+03, // 64
            -3.096375624373851e+02, // 65
             4.258823232451285e+02, // 66
             1.686060599756725e+02, // 67
            -5.412564694876887e+02, // 68
            -1.247582253705335e+02, // 69
            -7.292207970504651e+02, // 70
             6.084615026343356e+02, // 71
            -4.730807209409639e+02, // 72
            -3.131008083892204e+01, // 73
            -7.454959928991680e+01, // 74
             3.997578845899482e+02, // 75
            -2.811880713732343e+03, // 76
             9.937109111424229e+02, // 77
             2.683741658137011e+02, // 78
            -1.588760939590923e+03, // 79
             2.167534469106663e+03, // 80
            -4.006585148755351e+02, // 81
            -7.861478979496442e+02, // 82
             1.839027453204668e+03, // 83
             5.930144644571815e+02, // 84
             2.077141326791833e+01, // 85
            -1.848560465983351e+03, // 86
             5.200085768438981e+02, // 87
             2.468654101362472e+03, // 88
             2.441257742446621e+03, // 89
            -1.479084425699391e+03, // 90
            -1.949226116242809e+03, // 91
             2.439874581730366e+03, // 92
            -1.564263010289254e+03, // 93
             5.023722308104497e+02, // 94
            -2.235908957604985e+03, // 95
             1.363688431736008e+03, // 96
            -8.364693155627317e+02, // 97
             1.536073346969295e+03, // 98
            -1.212901635211863e+03, // 99
            -4.520405407536056e+02, // 100
             2.784653138526961e+03, // 101
            -8.810034574737326e+02, // 102
            -4.377705765408173e+02, // 103
             6.358922379638133e+02, // 104
            -1.342303583392261e+03, // 105
            -1.746227932399032e+03, // 106
             8.188757098648585e+02, // 107
            -1.578689925914740e+03, // 108
            -2.599306834109864e+02, // 109
            -1.784780138662787e+03, // 110
            -2.982882928907381e+02, // 111
            -1.399399380653713e+02, // 112
             7.621218266746772e+02, // 113
            -1.860712827196653e+03, // 114
             2.263186590244506e+03, // 115
            -9.717792714722317e+02, // 116
             8.627372351535680e+02, // 117
            -2.034447701227052e+03, // 118
            -3.784235214500626e+01, // 119
             5.427920907213114e+02, // 120
             1.138215011146067e+03, // 121
             1.038441409326312e+03, // 122
            -2.001309843786350e+03, // 123
             2.056471039443793e+03, // 124
            -1.277212448953462e+03, // 125
             9.404764658318435e+02, // 126
             3.052337608016521e+02, // 127
             3.521911374113714e+02, // 128
             1.441510886632776e+03, // 129
            -8.573184076428892e+02, // 130
             8.103419882020909e+02, // 131
            -2.099862985692797e+00, // 132
             1.057595124132473e+03, // 133
             1.353612768517345e+02, // 134
            -1.548491030631787e+03, // 135
             1.738361866853766e+03, // 136
            -3.569786873305891e+02, // 137
            -1.107422176058264e+03, // 138
             4.116642418375870e+02, // 139
             3.808727036313027e+02, // 140
            -8.380849393345868e+02, // 141
            -5.743393684457288e+02, // 142
             5.334891835229075e+02, // 143
            -1.460342464346908e+02, // 144
            -1.621989686518839e+03, // 145
            -4.498292427508766e+02, // 146
             4.948469426569381e+02, // 147
             9.130182464991474e+01, // 148
            -4.415860175762240e+02, // 149
            -7.343027478539711e+02, // 150
             8.815927027826213e+01, // 151
            -3.661086669580205e+01, // 152
            -5.098462275780918e+01, // 153
            -4.069107189632419e+02, // 154
             2.162530390303678e+02, // 155
             4.458580333054375e+01, // 156
            -1.643364413400928e+02, // 157
            -3.809976400562311e+02, // 158
             1.493733036453080e+01, // 159
             4.650047831458461e+02, // 160
            -3.462495782684849e+02, // 161
             4.218059149494838e+02, // 162
             3.242610042663231e+02, // 163
            -6.388072484236462e+02, // 164
            -3.967281100434363e+01, // 165
             5.053398402253749e+02, // 166
             1.529886043169803e+03, // 167
            -1.200906874135650e+02, // 168
            -6.692247004762240e+02, // 169
            -3.771231465972653e+02, // 170
            -9.437483821147379e+01, // 171
            -5.106475542174218e+02, // 172
            -8.107245511978754e+02, // 173
             8.000615313319253e+02, // 174
             9.684989290484356e+01, // 175
             4.868417237054368e+02, // 176
            -1.261043072505948e+03, // 177
            -3.380948208500090e+02, // 178
            -3.081650923597978e+02, // 179
            -1.396880993749170e+03, // 180
            -1.789170459881757e+02, // 181
             3.569254667012259e+02, // 182
             6.329774704851189e+02, // 183
            -2.191540032217164e+02, // 184
             1.134673221505401e+02, // 185
             4.738361314445332e+02, // 186
            -1.180659782514488e+02, // 187
             6.739393066531169e+02, // 188
             7.046278101719746e+02, // 189
            -1.596471667671004e+01}; // 190

    m_k_AA =  3.031458274164928e-01; // A^(-1))
    m_k_AB =  7.608152205269185e-01; // A^(-1))
    m_k_BB =  2.498151160751839e-01; // A^(-1))
    m_k_intra_BB =  1.918062182195191e+00; // A^(-1))
    m_k_intra_AB =  1.716588273927643e-01; // A^(-1))
    m_r2i =  7.000000000000000e+00; // A
    m_r2f =  8.000000000000000e+00; // A

    } // if mon1 == "nh4+" && mon2  == "nh4+"

}

//----------------------------------------------------------------------------//

double x2b_A1B4_A1B4_v1x::f_switch(const double& r, double& g) const
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

double x2b_A1B4_A1B4_v1x::eval(const double* xyz1, const double* xyz2, const size_t ndim) const
{

    std::vector<double> energies(ndim,0.0);

    for (size_t j = 0; j < ndim; j++) {
        double mon1[15];
        double mon2[15];

        std::copy(xyz1 + j * 15, xyz1 + (j+1) * 15, mon1);
        std::copy(xyz2 + j * 15, xyz2 + (j+1) * 15, mon2);

        // Right now it assumes 1st atom of each monomer
        const double d12[3] = {mon1[0] -  mon2[0],
                               mon1[1] -  mon2[1],
                               mon1[2] -  mon2[2]};
    
        const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double r12 = std::sqrt(r12sq);
    
        if (r12 > m_r2f)
            continue;
    
        double xcrd[30]; // coordinates of real sites ONLY
    
        std::copy(mon1, mon1 + 15, xcrd);
        std::copy(mon2, mon2 + 15, xcrd + 15);
        
        double v[45];
        
        double sw = 0.0;
        double gsw = 0.0;
    
        const double* A_1_a= xcrd + 0;
        const double* B_1_a= xcrd + 3;
        const double* B_2_a= xcrd + 6;
        const double* B_3_a= xcrd + 9;
        const double* B_4_a= xcrd + 12;


        const double* A_1_b= xcrd + 15;
        const double* B_1_b= xcrd + 18;
        const double* B_2_b= xcrd + 21;
        const double* B_3_b= xcrd + 24;
        const double* B_4_b= xcrd + 27;


    
        variable vr[45];
    
        v[0]  = vr[0].v_exp(m_k_intra_BB, B_1_a, B_2_a);
        v[1]  = vr[1].v_exp(m_k_intra_BB, B_1_a, B_3_a);
        v[2]  = vr[2].v_exp(m_k_intra_BB, B_1_a, B_4_a);
        v[3]  = vr[3].v_exp(m_k_intra_BB, B_2_a, B_3_a);
        v[4]  = vr[4].v_exp(m_k_intra_BB, B_2_a, B_4_a);
        v[5]  = vr[5].v_exp(m_k_intra_BB, B_3_a, B_4_a);
        v[6]  = vr[6].v_exp(m_k_intra_AB, A_1_a, B_1_a);
        v[7]  = vr[7].v_exp(m_k_intra_AB, A_1_a, B_2_a);
        v[8]  = vr[8].v_exp(m_k_intra_AB, A_1_a, B_3_a);
        v[9]  = vr[9].v_exp(m_k_intra_AB, A_1_a, B_4_a);
        v[10]  = vr[10].v_exp(m_k_intra_BB, B_1_b, B_2_b);
        v[11]  = vr[11].v_exp(m_k_intra_BB, B_1_b, B_3_b);
        v[12]  = vr[12].v_exp(m_k_intra_BB, B_1_b, B_4_b);
        v[13]  = vr[13].v_exp(m_k_intra_BB, B_2_b, B_3_b);
        v[14]  = vr[14].v_exp(m_k_intra_BB, B_2_b, B_4_b);
        v[15]  = vr[15].v_exp(m_k_intra_BB, B_3_b, B_4_b);
        v[16]  = vr[16].v_exp(m_k_intra_AB, A_1_b, B_1_b);
        v[17]  = vr[17].v_exp(m_k_intra_AB, A_1_b, B_2_b);
        v[18]  = vr[18].v_exp(m_k_intra_AB, A_1_b, B_3_b);
        v[19]  = vr[19].v_exp(m_k_intra_AB, A_1_b, B_4_b);
        v[20]  = vr[20].v_exp(m_k_AA, A_1_a, A_1_b);
        v[21]  = vr[21].v_exp(m_k_AB, A_1_a, B_1_b);
        v[22]  = vr[22].v_exp(m_k_AB, A_1_a, B_2_b);
        v[23]  = vr[23].v_exp(m_k_AB, A_1_a, B_3_b);
        v[24]  = vr[24].v_exp(m_k_AB, A_1_a, B_4_b);
        v[25]  = vr[25].v_exp(m_k_AB, B_1_a, A_1_b);
        v[26]  = vr[26].v_exp(m_k_AB, B_2_a, A_1_b);
        v[27]  = vr[27].v_exp(m_k_AB, B_3_a, A_1_b);
        v[28]  = vr[28].v_exp(m_k_AB, B_4_a, A_1_b);
        v[29]  = vr[29].v_exp(m_k_BB, B_1_a, B_1_b);
        v[30]  = vr[30].v_exp(m_k_BB, B_1_a, B_2_b);
        v[31]  = vr[31].v_exp(m_k_BB, B_1_a, B_3_b);
        v[32]  = vr[32].v_exp(m_k_BB, B_1_a, B_4_b);
        v[33]  = vr[33].v_exp(m_k_BB, B_2_a, B_1_b);
        v[34]  = vr[34].v_exp(m_k_BB, B_2_a, B_2_b);
        v[35]  = vr[35].v_exp(m_k_BB, B_2_a, B_3_b);
        v[36]  = vr[36].v_exp(m_k_BB, B_2_a, B_4_b);
        v[37]  = vr[37].v_exp(m_k_BB, B_3_a, B_1_b);
        v[38]  = vr[38].v_exp(m_k_BB, B_3_a, B_2_b);
        v[39]  = vr[39].v_exp(m_k_BB, B_3_a, B_3_b);
        v[40]  = vr[40].v_exp(m_k_BB, B_3_a, B_4_b);
        v[41]  = vr[41].v_exp(m_k_BB, B_4_a, B_1_b);
        v[42]  = vr[42].v_exp(m_k_BB, B_4_a, B_2_b);
        v[43]  = vr[43].v_exp(m_k_BB, B_4_a, B_3_b);
        v[44]  = vr[44].v_exp(m_k_BB, B_4_a, B_4_b);
     
    
        sw = f_switch(r12, gsw);
        
        energies[j] = sw*polynomial::eval(coefficients.data(), v);
    }

    double energy = 0.0;
    for (size_t i = 0; i < ndim; i++) {
      energy += energies[i];
    }

    return energy;
    
}

double x2b_A1B4_A1B4_v1x::eval(const double* xyz1, const double* xyz2, 
                double * grad1, double * grad2, const size_t ndim) const
{

    std::vector<double> energies(ndim,0.0);

    for (size_t j = 0; j < ndim; j++) {
        double mon1[15];
        double mon2[15];

        std::copy(xyz1 + j * 15, xyz1 + (j+1) * 15, mon1);
        std::copy(xyz2 + j * 15, xyz2 + (j+1) * 15, mon2);

        // Right now it assumes 1st atom of each monomer
        const double d12[3] = {mon1[0] -  mon2[0],
                               mon1[1] -  mon2[1],
                               mon1[2] -  mon2[2]};
    
        const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double r12 = std::sqrt(r12sq);
    
        if (r12 > m_r2f)
            continue;
    
        double xcrd[30]; // coordinates of real sites ONLY
    
        std::copy(mon1, mon1 + 15, xcrd);
        std::copy(mon2, mon2 + 15, xcrd + 15);
        
        double v[45];
        
        double sw = 0.0;
        double gsw = 0.0;
    
        const double* A_1_a= xcrd + 0;
        const double* B_1_a= xcrd + 3;
        const double* B_2_a= xcrd + 6;
        const double* B_3_a= xcrd + 9;
        const double* B_4_a= xcrd + 12;


        const double* A_1_b= xcrd + 15;
        const double* B_1_b= xcrd + 18;
        const double* B_2_b= xcrd + 21;
        const double* B_3_b= xcrd + 24;
        const double* B_4_b= xcrd + 27;


    
        variable vr[45];
    
        v[0]  = vr[0].v_exp(m_k_intra_BB, B_1_a, B_2_a);
        v[1]  = vr[1].v_exp(m_k_intra_BB, B_1_a, B_3_a);
        v[2]  = vr[2].v_exp(m_k_intra_BB, B_1_a, B_4_a);
        v[3]  = vr[3].v_exp(m_k_intra_BB, B_2_a, B_3_a);
        v[4]  = vr[4].v_exp(m_k_intra_BB, B_2_a, B_4_a);
        v[5]  = vr[5].v_exp(m_k_intra_BB, B_3_a, B_4_a);
        v[6]  = vr[6].v_exp(m_k_intra_AB, A_1_a, B_1_a);
        v[7]  = vr[7].v_exp(m_k_intra_AB, A_1_a, B_2_a);
        v[8]  = vr[8].v_exp(m_k_intra_AB, A_1_a, B_3_a);
        v[9]  = vr[9].v_exp(m_k_intra_AB, A_1_a, B_4_a);
        v[10]  = vr[10].v_exp(m_k_intra_BB, B_1_b, B_2_b);
        v[11]  = vr[11].v_exp(m_k_intra_BB, B_1_b, B_3_b);
        v[12]  = vr[12].v_exp(m_k_intra_BB, B_1_b, B_4_b);
        v[13]  = vr[13].v_exp(m_k_intra_BB, B_2_b, B_3_b);
        v[14]  = vr[14].v_exp(m_k_intra_BB, B_2_b, B_4_b);
        v[15]  = vr[15].v_exp(m_k_intra_BB, B_3_b, B_4_b);
        v[16]  = vr[16].v_exp(m_k_intra_AB, A_1_b, B_1_b);
        v[17]  = vr[17].v_exp(m_k_intra_AB, A_1_b, B_2_b);
        v[18]  = vr[18].v_exp(m_k_intra_AB, A_1_b, B_3_b);
        v[19]  = vr[19].v_exp(m_k_intra_AB, A_1_b, B_4_b);
        v[20]  = vr[20].v_exp(m_k_AA, A_1_a, A_1_b);
        v[21]  = vr[21].v_exp(m_k_AB, A_1_a, B_1_b);
        v[22]  = vr[22].v_exp(m_k_AB, A_1_a, B_2_b);
        v[23]  = vr[23].v_exp(m_k_AB, A_1_a, B_3_b);
        v[24]  = vr[24].v_exp(m_k_AB, A_1_a, B_4_b);
        v[25]  = vr[25].v_exp(m_k_AB, B_1_a, A_1_b);
        v[26]  = vr[26].v_exp(m_k_AB, B_2_a, A_1_b);
        v[27]  = vr[27].v_exp(m_k_AB, B_3_a, A_1_b);
        v[28]  = vr[28].v_exp(m_k_AB, B_4_a, A_1_b);
        v[29]  = vr[29].v_exp(m_k_BB, B_1_a, B_1_b);
        v[30]  = vr[30].v_exp(m_k_BB, B_1_a, B_2_b);
        v[31]  = vr[31].v_exp(m_k_BB, B_1_a, B_3_b);
        v[32]  = vr[32].v_exp(m_k_BB, B_1_a, B_4_b);
        v[33]  = vr[33].v_exp(m_k_BB, B_2_a, B_1_b);
        v[34]  = vr[34].v_exp(m_k_BB, B_2_a, B_2_b);
        v[35]  = vr[35].v_exp(m_k_BB, B_2_a, B_3_b);
        v[36]  = vr[36].v_exp(m_k_BB, B_2_a, B_4_b);
        v[37]  = vr[37].v_exp(m_k_BB, B_3_a, B_1_b);
        v[38]  = vr[38].v_exp(m_k_BB, B_3_a, B_2_b);
        v[39]  = vr[39].v_exp(m_k_BB, B_3_a, B_3_b);
        v[40]  = vr[40].v_exp(m_k_BB, B_3_a, B_4_b);
        v[41]  = vr[41].v_exp(m_k_BB, B_4_a, B_1_b);
        v[42]  = vr[42].v_exp(m_k_BB, B_4_a, B_2_b);
        v[43]  = vr[43].v_exp(m_k_BB, B_4_a, B_3_b);
        v[44]  = vr[44].v_exp(m_k_BB, B_4_a, B_4_b);
     
    
        double g[45];

        // the switch
        sw = f_switch(r12, gsw);
        
        energies[j] = sw*polynomial::eval(coefficients.data(), v, g);
        
        double xgrd[30];
        std::fill(xgrd, xgrd + 30, 0.0);

        double* A_1_a_g= xgrd + 0;
        double* B_1_a_g= xgrd + 3;
        double* B_2_a_g= xgrd + 6;
        double* B_3_a_g= xgrd + 9;
        double* B_4_a_g= xgrd + 12;

        double* A_1_b_g= xgrd + 15;
        double* B_1_b_g= xgrd + 18;
        double* B_2_b_g= xgrd + 21;
        double* B_3_b_g= xgrd + 24;
        double* B_4_b_g= xgrd + 27;



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
        vr[10].grads(g[10], B_1_b_g, B_2_b_g, B_1_b, B_2_b);
        vr[11].grads(g[11], B_1_b_g, B_3_b_g, B_1_b, B_3_b);
        vr[12].grads(g[12], B_1_b_g, B_4_b_g, B_1_b, B_4_b);
        vr[13].grads(g[13], B_2_b_g, B_3_b_g, B_2_b, B_3_b);
        vr[14].grads(g[14], B_2_b_g, B_4_b_g, B_2_b, B_4_b);
        vr[15].grads(g[15], B_3_b_g, B_4_b_g, B_3_b, B_4_b);
        vr[16].grads(g[16], A_1_b_g, B_1_b_g, A_1_b, B_1_b);
        vr[17].grads(g[17], A_1_b_g, B_2_b_g, A_1_b, B_2_b);
        vr[18].grads(g[18], A_1_b_g, B_3_b_g, A_1_b, B_3_b);
        vr[19].grads(g[19], A_1_b_g, B_4_b_g, A_1_b, B_4_b);
        vr[20].grads(g[20], A_1_a_g, A_1_b_g, A_1_a, A_1_b);
        vr[21].grads(g[21], A_1_a_g, B_1_b_g, A_1_a, B_1_b);
        vr[22].grads(g[22], A_1_a_g, B_2_b_g, A_1_a, B_2_b);
        vr[23].grads(g[23], A_1_a_g, B_3_b_g, A_1_a, B_3_b);
        vr[24].grads(g[24], A_1_a_g, B_4_b_g, A_1_a, B_4_b);
        vr[25].grads(g[25], B_1_a_g, A_1_b_g, B_1_a, A_1_b);
        vr[26].grads(g[26], B_2_a_g, A_1_b_g, B_2_a, A_1_b);
        vr[27].grads(g[27], B_3_a_g, A_1_b_g, B_3_a, A_1_b);
        vr[28].grads(g[28], B_4_a_g, A_1_b_g, B_4_a, A_1_b);
        vr[29].grads(g[29], B_1_a_g, B_1_b_g, B_1_a, B_1_b);
        vr[30].grads(g[30], B_1_a_g, B_2_b_g, B_1_a, B_2_b);
        vr[31].grads(g[31], B_1_a_g, B_3_b_g, B_1_a, B_3_b);
        vr[32].grads(g[32], B_1_a_g, B_4_b_g, B_1_a, B_4_b);
        vr[33].grads(g[33], B_2_a_g, B_1_b_g, B_2_a, B_1_b);
        vr[34].grads(g[34], B_2_a_g, B_2_b_g, B_2_a, B_2_b);
        vr[35].grads(g[35], B_2_a_g, B_3_b_g, B_2_a, B_3_b);
        vr[36].grads(g[36], B_2_a_g, B_4_b_g, B_2_a, B_4_b);
        vr[37].grads(g[37], B_3_a_g, B_1_b_g, B_3_a, B_1_b);
        vr[38].grads(g[38], B_3_a_g, B_2_b_g, B_3_a, B_2_b);
        vr[39].grads(g[39], B_3_a_g, B_3_b_g, B_3_a, B_3_b);
        vr[40].grads(g[40], B_3_a_g, B_4_b_g, B_3_a, B_4_b);
        vr[41].grads(g[41], B_4_a_g, B_1_b_g, B_4_a, B_1_b);
        vr[42].grads(g[42], B_4_a_g, B_2_b_g, B_4_a, B_2_b);
        vr[43].grads(g[43], B_4_a_g, B_3_b_g, B_4_a, B_3_b);
        vr[44].grads(g[44], B_4_a_g, B_4_b_g, B_4_a, B_4_b);


    // ##DEFINE HERE## the redistribution of the gradients
    

    
        for (int i = 0; i < 15; ++i) {
            grad1[i + j*15] += sw*xgrd[i];
        }

        for (int i = 0; i < 15; ++i) {
            grad2[i + j*15] += sw*xgrd[i + 15];
        }

        // gradient of the switch

        gsw *= energies[j]/r12;
        for (int i = 0; i < 3; ++i) {
            const double d = gsw*d12[i];
            grad1[i + j*15] += d;
            grad2[i + j*15] -= d;
        }

    }

    double energy = 0.0;
    for (size_t i = 0; i < ndim; i++) {
      energy += energies[i];
    }

    return energy;
}

} // namespace x2b_A1B4_A1B4_deg4

////////////////////////////////////////////////////////////////////////////////
