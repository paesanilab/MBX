
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>

#include <netcdf.h>

#include "x2b_A1B2_A1B2_v1x_deg4.h" 
 

////////////////////////////////////////////////////////////////////////////////

namespace {

//----------------------------------------------------------------------------//

void error(int kode) {

    std::cerr << " ** Fatal Error in x2b_A1B2_A1B2_v1x::load_netcdf() ** :" 
              << nc_strerror(kode) << std::endl;
    std::exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------//

struct variable {
    double v_exp(const double& r0, const double& k,
                 const double * p1, const double * p2 );

    double v_coul(const double& r0, const double& k,
                  const double * p1, const double * p2 );
                  
    void grads(const double& gg, double * grd1, double * grd2,
               const double * p1, const double * p2);

    double g[3]; // diff(value, p1 - p2)
};

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

double variable::v_exp(const double& r0, const double& k,
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

double variable::v_coul(const double& r0, const double& k,
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

struct vsites {
    //void TwoParticleAverageSite() {}
    //void ThreeParticleAverageSite() {}
    void OutOfPlaneSite(const double& w12, const double& w13,
                        const double& wcross, const double x1[3],
                        const double y1[3], const double y2[3],
                        double vs[3]);
    //void LocalCoordinatesSite{}
};

void vsites::OutOfPlaneSite(const double& w12,
                            const double& w13,
                            const double& wcross,
                            const double x1[3],
                            const double y1[3],
                            const double y2[3],
                            double vs[3]) {
    double r12[3], r13[3];

    for (int i = 0; i < 3; ++i) {
        r12[i] = y1[i] - x1[i];
        r13[i] = y2[i] - x1[i];
    }
                            
    double rc[3];
    rc[0] = r12[1]*r13[2] - r12[2]*r13[1];
    rc[1] = r12[2]*r13[0] - r12[0]*r13[2];
    rc[2] = r12[0]*r13[1] - r12[1]*r13[0];
    
    vs[0] = x1[0] + w12 * r12[0] + w13 * r13[0] + wcross * rc[0];
    vs[1] = x1[1] + w12 * r12[1] + w13 * r13[1] + wcross * rc[1];
    vs[2] = x1[2] + w12 * r12[2] + w13 * r13[2] + wcross * rc[2];
}

} // namespace

////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B2_A1B2 {

//----------------------------------------------------------------------------//


std::string x2b_A1B2_A1B2_v1x::name() {
    return "x2b_A1B2_A1B2_v1x";
}


//----------------------------------------------------------------------------//

void x2b_A1B2_A1B2_v1x::load_netcdf(const char* fn)
{
    assert(fn);

    int rc, ncid;
    if ((rc = nc_open(fn, NC_NOWRITE, &ncid)))
        error(rc);

#   define RETRIEVE(name) \
    if ((rc = nc_get_att_double(ncid, NC_GLOBAL, #name , &m_##name))) \
        error(rc);
    RETRIEVE(d_intra_AB)
    RETRIEVE(k_intra_AB)
    RETRIEVE(d_intra_BB)
    RETRIEVE(k_intra_BB)
    RETRIEVE(d_AA)
    RETRIEVE(k_AA)
    RETRIEVE(d_AB)
    RETRIEVE(k_AB)
    RETRIEVE(d_BB)
    RETRIEVE(k_BB)


    RETRIEVE(r2i)
    RETRIEVE(r2f)

#   undef RETRIEVE

    int varid;

    if ((rc = nc_inq_varid(ncid, "poly", &varid)))
        error(rc);

    for (size_t n = 0; n < poly_type::size; ++n) {
        if ((rc = nc_get_var1_double(ncid, varid, &n, m_poly + n)))
            error(rc);
    }

    if ((rc = nc_close(ncid)))
        error(rc);
}

//----------------------------------------------------------------------------//

double x2b_A1B2_A1B2_v1x::f_switch(const double& r, double& g) const
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

double x2b_A1B2_A1B2_v1x::eval(const double* mon1, const double* mon2 ) const
{
    // the switch

    // ##DEFINE HERE## right now it assumes 1st atom of each monomer
    const double d12[3] = {mon1[0] -  mon2[0],
                           mon1[1] -  mon2[1],
                           mon1[2] -  mon2[2]};

    const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
    const double r12 = std::sqrt(r12sq);

    if (r12 > m_r2f)
        return 0.0;

    double xcrd[18]; // coordinates of real sites ONLY

    std::copy(mon1, mon1 + 9, xcrd);
    std::copy(mon2, mon2 + 9, xcrd + 9);
    
    double v[15];
    
    double sw = 0.0;
    double gsw = 0.0;
    
    const double* A_1_a= xcrd + 0;
    const double* B_1_a= xcrd + 3;
    const double* B_2_a= xcrd + 6;

    const double* A_1_b= xcrd + 9;
    const double* B_1_b= xcrd + 12;
    const double* B_2_b= xcrd + 15;


    
    variable vr[15];
    
    v[0]  = vr[0].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_a, B_1_a);
    v[1]  = vr[1].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_a, B_2_a);
    v[2]  = vr[2].v_exp(m_d_intra_BB, m_k_intra_BB, B_1_a, B_2_a);

    v[3]  = vr[3].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_b, B_1_b);
    v[4]  = vr[4].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_b, B_2_b);
    v[5]  = vr[5].v_exp(m_d_intra_BB, m_k_intra_BB, B_1_b, B_2_b);

    v[6]  = vr[6].v_exp(m_d_AA, m_k_AA, A_1_a, A_1_b);
    v[7]  = vr[7].v_exp(m_d_AB, m_k_AB, A_1_a, B_1_b);
    v[8]  = vr[8].v_exp(m_d_AB, m_k_AB, A_1_a, B_2_b);

    v[9]  = vr[9].v_exp(m_d_AB, m_k_AB, B_1_a, A_1_b);
    v[10]  = vr[10].v_exp(m_d_BB, m_k_BB, B_1_a, B_1_b);
    v[11]  = vr[11].v_exp(m_d_BB, m_k_BB, B_1_a, B_2_b);

    v[12]  = vr[12].v_exp(m_d_AB, m_k_AB, B_2_a, A_1_b);
    v[13]  = vr[13].v_exp(m_d_BB, m_k_BB, B_2_a, B_1_b);
    v[14]  = vr[14].v_exp(m_d_BB, m_k_BB, B_2_a, B_2_b);

     
    
    sw = f_switch(r12, gsw);
    
    const double E_poly = mb_system::poly_model::eval(m_poly, v);
    
    return sw*E_poly;
}

double x2b_A1B2_A1B2_v1x::eval(const double* mon1, const double* mon2, 
                double * grd1, double * grd2) const
{

    // ##DEFINE HERE## right now it assumes 1st atom of each monomer
    const double d12[3] = {mon1[0] -  mon2[0],
                           mon1[1] -  mon2[1],
                           mon1[2] -  mon2[2]};

    const double r12sq = d12[0]*d12[0] + d12[1]*d12[1] + d12[2]*d12[2];
    const double r12 = std::sqrt(r12sq);

    if (r12 > m_r2f)
        return 0.0;

    double xcrd[18]; // coordinates of real sites ONLY

    std::copy(mon1, mon1 + 9, xcrd);
    std::copy(mon2, mon2 + 9, xcrd + 9);
    
    double v[15];
    
    double sw = 0.0;
    double gsw = 0.0;
    
    const double* A_1_a= xcrd + 0;
    const double* B_1_a= xcrd + 3;
    const double* B_2_a= xcrd + 6;

    const double* A_1_b= xcrd + 9;
    const double* B_1_b= xcrd + 12;
    const double* B_2_b= xcrd + 15;


    
    variable vr[15];
    
    v[0]  = vr[0].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_a, B_1_a);
    v[1]  = vr[1].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_a, B_2_a);
    v[2]  = vr[2].v_exp(m_d_intra_BB, m_k_intra_BB, B_1_a, B_2_a);

    v[3]  = vr[3].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_b, B_1_b);
    v[4]  = vr[4].v_exp(m_d_intra_AB, m_k_intra_AB, A_1_b, B_2_b);
    v[5]  = vr[5].v_exp(m_d_intra_BB, m_k_intra_BB, B_1_b, B_2_b);

    v[6]  = vr[6].v_exp(m_d_AA, m_k_AA, A_1_a, A_1_b);
    v[7]  = vr[7].v_exp(m_d_AB, m_k_AB, A_1_a, B_1_b);
    v[8]  = vr[8].v_exp(m_d_AB, m_k_AB, A_1_a, B_2_b);

    v[9]  = vr[9].v_exp(m_d_AB, m_k_AB, B_1_a, A_1_b);
    v[10]  = vr[10].v_exp(m_d_BB, m_k_BB, B_1_a, B_1_b);
    v[11]  = vr[11].v_exp(m_d_BB, m_k_BB, B_1_a, B_2_b);

    v[12]  = vr[12].v_exp(m_d_AB, m_k_AB, B_2_a, A_1_b);
    v[13]  = vr[13].v_exp(m_d_BB, m_k_BB, B_2_a, B_1_b);
    v[14]  = vr[14].v_exp(m_d_BB, m_k_BB, B_2_a, B_2_b);

     
    
    double g[15];
    
    const double E_poly = mb_system::poly_model::eval(m_poly, v, g);
    
    double xgrd[18];
    std::fill(xgrd, xgrd + 18, 0.0);

    double* A_1_a_g = xgrd + 0;
    double* B_1_a_g = xgrd + 3;
    double* B_2_a_g = xgrd + 6;

    double* A_1_b_g = xgrd + 9;
    double* B_1_b_g = xgrd + 12;
    double* B_2_b_g = xgrd + 15;


    vr[0].grads(g[0], A_1_a_g, B_1_a_g, A_1_a, B_1_a);
    vr[1].grads(g[1], A_1_a_g, B_2_a_g, A_1_a, B_2_a);
    vr[2].grads(g[2], B_1_a_g, B_2_a_g, B_1_a, B_2_a);

    vr[3].grads(g[3], A_1_b_g, B_1_b_g, A_1_b, B_1_b);
    vr[4].grads(g[4], A_1_b_g, B_2_b_g, A_1_b, B_2_b);
    vr[5].grads(g[5], B_1_b_g, B_2_b_g, B_1_b, B_2_b);

    vr[6].grads(g[6], A_1_a_g, A_1_b_g, A_1_a, A_1_b);
    vr[7].grads(g[7], A_1_a_g, B_1_b_g, A_1_a, B_1_b);
    vr[8].grads(g[8], A_1_a_g, B_2_b_g, A_1_a, B_2_b);

    vr[9].grads(g[9], B_1_a_g, A_1_b_g, B_1_a, A_1_b);
    vr[10].grads(g[10], B_1_a_g, B_1_b_g, B_1_a, B_1_b);
    vr[11].grads(g[11], B_1_a_g, B_2_b_g, B_1_a, B_2_b);

    vr[12].grads(g[12], B_2_a_g, A_1_b_g, B_2_a, A_1_b);
    vr[13].grads(g[13], B_2_a_g, B_1_b_g, B_2_a, B_1_b);
    vr[14].grads(g[14], B_2_a_g, B_2_b_g, B_2_a, B_2_b);



    // ##DEFINE HERE## the redistribution of the gradients
    
    // the switch
    
    sw = f_switch(r12, gsw);
    
    for (int i = 0; i < 9; ++i) {
        grd1[i] += sw*xgrd[i];
    }

    for (int i = 0; i < 9; ++i) {
        grd2[i] += sw*xgrd[i + 9];
    }

    // gradient of the switch

    gsw *= E_poly/r12;
    for (int i = 0; i < 3; ++i) {
        const double d = gsw*d12[i];
        grd1[i] += d;
        grd2[i] -= d;
    }
    return sw*E_poly;
}

double x2b_A1B2_A1B2_v1x::operator()(const double crd[18]) const
{
    const double E_poly = eval(crd, crd + 9);

    return E_poly;
}

double x2b_A1B2_A1B2_v1x::operator()(const double crd[18],
                            double grd[18]) const
{
    const double E_poly = eval(crd, crd + 9, grd, grd + 9);

    return E_poly;
}

} // namespace x2b_A1B2_A1B2

////////////////////////////////////////////////////////////////////////////////
