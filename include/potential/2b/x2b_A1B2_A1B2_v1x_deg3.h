#ifndef X2B_A1B2_A1B2_V1X_DEG3_H
#define X2B_A1B2_A1B2_V1X_DEG3_H
 
#include "poly_2b_A1B2_A1B2_v1x_deg3.h" 
 

#include <iostream>
////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B2_A1B2 {

//----------------------------------------------------------------------------//

struct x2b_A1B2_A1B2_v1x { 

    typedef mb_system::poly_model poly_type;

    static std::string name();
    void load_netcdf(const char*);

    // returns 2B contribution only
    // XYZ is for the real sites
//    double operator()(const double xyz[18], double grd[18]) const; 
    double operator()(const double xyz[18]) const; 

    double eval(const double* mon1, const double* mon2) const;
    double eval(const double* mon1, const double* mon2, double* g1, double* g2) const;
    void cart_to_vars(const double xyz[18], double* vars, double& s, double& gs) const;
    
private:

    double m_d_intra_AB;
    double m_k_intra_AB;
    double m_d_intra_BB;
    double m_k_intra_BB;
    double m_d_AA;
    double m_k_AA;
    double m_d_AB;
    double m_k_AB;
    double m_d_BB;
    double m_k_BB;

protected:
    double m_r2i = 6.0;
    double m_r2f = 7.0;

    double f_switch(const double&, double&) const; // At1_a -- At1_b separation

private:
    double m_poly[poly_type::size];
};

} // namespace x2b_A1B2_A1B2

////////////////////////////////////////////////////////////////////////////////

#endif 
