#ifndef X2B_A1B4_C1D6_DEG3_V1X_H
#define X2B_A1B4_C1D6_DEG3_V1X_H
 
#include "poly_2b_A1B4_C1D6_deg3_v1x.h" 

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B4_C1D6_deg3 {

//----------------------------------------------------------------------------//

struct x2b_A1B4_C1D6_v1x { 

    x2b_A1B4_C1D6_v1x() {};
    x2b_A1B4_C1D6_v1x(const std::string mon1, const std::string mon2);

    ~x2b_A1B4_C1D6_v1x() {};

    typedef poly_2b_A1B4_C1D6_deg3_v1x polynomial;

    // returns 2B contribution only
    // XYZ is for the real sites

    double eval(const double* xyz1, const double* xyz2, const size_t ndim) const;
    double eval(const double* xyz1, const double* xyz2, double* grad1, double* grad2, const size_t ndim) const;
    
private:

    double m_k_AC;
    double m_k_AD;
    double m_k_BC;
    double m_k_BD;
    double m_k_intra_BB;
    double m_k_intra_AB;
    double m_k_intra_DD;
    double m_k_intra_CD;

protected:
    double m_r2i = 7.0;
    double m_r2f = 8.0;

    double f_switch(const double&, double&) const; // At1_a -- At1_b separation

private:
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

} // namespace x2b_A1B4_C1D6_deg3

////////////////////////////////////////////////////////////////////////////////

#endif 
