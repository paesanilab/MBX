

#include "x2b_A1_A1_deg23_v1x.h" 
 

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

namespace x2b_A1_A1_deg23 {

//----------------------------------------------------------------------------//

x2b_A1_A1_v1x::x2b_A1_A1_v1x(std::string mon1, std::string mon2) {

    // =====>> SECTION CONSTRUCTOR <<=====
    // =>> PASTE RIGHT BELOW THIS LINE <==
    if ( mon1 == "he" && mon2 == "he" ) {
        coefficients = std::vector<double> {
            8.703802082384445e-03, // 0
            5.453079827958427e-02, // 1
           -3.960243176219747e-01, // 2
            6.665557935173138e-01, // 3
           -5.526701429189695e-02, // 4
           -7.205735596690755e-01, // 5
            1.314656844473333e-01, // 6
            8.326698998233450e-01, // 7
           -2.453141802377055e-01, // 8
           -9.300883298206759e-01, // 9
            6.738746386382013e-01, // 10
            6.652662938612909e-01, // 11
           -1.466349542304558e+00, // 12
            1.263791270410142e+00, // 13
           -6.817310841926912e-01, // 14
            2.563340388728538e-01, // 15
           -6.985693841730024e-02, // 16
            1.395291995696554e-02, // 17
           -2.028016386460166e-03, // 18
            2.090614682567161e-04, // 19
           -1.449626756881751e-05, // 20
            6.065256966899295e-07, // 21
           -1.157127150246831e-08}; // 22


        m_k_AA =  3.224372466704150e-01; // A^(-1))
        m_d_AA =  5.731623745108086e+00; // A^(-1))
        m_r2i =  7.000000000000000e+00; // A
        m_r2f =  8.000000000000000e+00; // A

    }

}

//----------------------------------------------------------------------------//

double x2b_A1_A1_v1x::f_switch(const double& r, double& g) const
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

double x2b_A1_A1_v1x::eval(const double* xyz1, const double* xyz2, const size_t ndim) const
{

    std::vector<double> energies(ndim,0.0);

    for (size_t j = 0; j < ndim; j++) {
        double mon1[3];
        double mon2[3];

        std::copy(xyz1 + j * 3, xyz1 + (j+1) * 3, mon1);
        std::copy(xyz2 + j * 3, xyz2 + (j+1) * 3, mon2);

        // Right now it assumes 1st atom of each monomer
        const double d12[3] = {mon1[0] -  mon2[0],
                               mon1[1] -  mon2[1],
                               mon1[2] -  mon2[2]};
    
        const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double r12 = std::sqrt(r12sq);
    
        if (r12 > m_r2f)
            continue;
    
        double xcrd[6]; // coordinates of real sites ONLY
    
        std::copy(mon1, mon1 + 3, xcrd);
        std::copy(mon2, mon2 + 3, xcrd + 3);
        
        double v[1];
        
        double sw = 0.0;
        double gsw = 0.0;
    
        const double* A_1_a= xcrd + 0;


        const double* A_1_b= xcrd + 3;


    
        variable vr[1];
    
        v[0]  = vr[0].v_exp0(m_d_AA, m_k_AA, A_1_a, A_1_b);
     
    
        sw = f_switch(r12, gsw);
        
        energies[j] = sw*polynomial::eval(coefficients.data(), v);
    }

    double energy = 0.0;
    for (size_t i = 0; i < ndim; i++) {
      energy += energies[i];
    }

    return energy;
    
}

double x2b_A1_A1_v1x::eval(const double* xyz1, const double* xyz2, 
                double * grad1, double * grad2, const size_t ndim, std::vector<double> *virial) const
{

    std::vector<double> energies(ndim,0.0);

    for (size_t j = 0; j < ndim; j++) {
        double mon1[3];
        double mon2[3];

        std::copy(xyz1 + j * 3, xyz1 + (j+1) * 3, mon1);
        std::copy(xyz2 + j * 3, xyz2 + (j+1) * 3, mon2);

        // Right now it assumes 1st atom of each monomer
        const double d12[3] = {mon1[0] -  mon2[0],
                               mon1[1] -  mon2[1],
                               mon1[2] -  mon2[2]};
    
        const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
        const double r12 = std::sqrt(r12sq);
    
        if (r12 > m_r2f)
            continue;
    
        double xcrd[6]; // coordinates of real sites ONLY
    
        std::copy(mon1, mon1 + 3, xcrd);
        std::copy(mon2, mon2 + 3, xcrd + 3);
        
        double v[1];
        
        double sw = 0.0;
        double gsw = 0.0;
    
        const double* A_1_a= xcrd + 0;


        const double* A_1_b= xcrd + 3;


    
        variable vr[1];
    
        v[0]  = vr[0].v_exp0(m_d_AA, m_k_AA, A_1_a, A_1_b);
     
    
        double g[1];

        // the switch
        sw = f_switch(r12, gsw);
        
        //FIXED:
        energies[j] = polynomial::eval(coefficients.data(), v, g);
        
        double xgrd[6];
        std::fill(xgrd, xgrd + 6, 0.0);

        double* A_1_a_g= xgrd + 0;

        double* A_1_b_g= xgrd + 3;



        vr[0].grads(g[0], A_1_a_g, A_1_b_g, A_1_a, A_1_b);


    // ##DEFINE HERE## the redistribution of the gradients
    

    
        for (int i = 0; i < 3; ++i) {
            grad1[i + j*3] += sw*xgrd[i];
        }

        for (int i = 0; i < 3; ++i) {
            grad2[i + j*3] += sw*xgrd[i + 3];
        }

        // gradient of the switch

        gsw *= energies[j]/r12;
        for (int i = 0; i < 3; ++i) {
            const double d = gsw*d12[i];
            grad1[i + j*3] += d;
            grad2[i + j*3] -= d;
        }

        //ADD:
        energies[j] *= sw;

        if (virial != 0) {

            (*virial)[0] += -xcrd[0]* grad1[0 + j*3]
                            -xcrd[3]* grad2[0 + j*3];

            (*virial)[1] += -xcrd[0]* grad1[1 + j*3]
                            -xcrd[3]* grad2[1 + j*3];

            (*virial)[2] += -xcrd[0]* grad1[2 + j*3]
                            -xcrd[3]* grad2[2 + j*3];

            (*virial)[4] += -xcrd[1]* grad1[1 + j*3]
                            -xcrd[4]* grad2[1 + j*3];

            (*virial)[5] += -xcrd[1]* grad1[2 + j*3]
                            -xcrd[4]* grad2[2 + j*3];

            (*virial)[8] += -xcrd[2]* grad1[2 + j*3]
                            -xcrd[5]* grad2[2 + j*3];

            (*virial)[3]=(*virial)[1];
            (*virial)[6]=(*virial)[2];
            (*virial)[7]=(*virial)[5];

        }

    }

    double energy = 0.0;
    for (size_t i = 0; i < ndim; i++) {
      energy += energies[i];
    }

    return energy;
}

} // namespace x2b_A1_A1_deg23

////////////////////////////////////////////////////////////////////////////////
