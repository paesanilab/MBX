#ifndef X1B_A1B4_DEG3_H
#define X1B_A1B4_DEG3_H

#include "poly_1b_A1B4_deg3.h"

////////////////////////////////////////////////////////////////////////////////

namespace A1B4 {

//----------------------------------------------------------------------------//

struct x1b_v1x {

    static const unsigned ncoeffs = poly_1b_v1x::size;

    double operator()(const double* w1) const;

    double operator()(const double* w1, double* g1) const;

    void load_poly_dat(const char*);

private:

    double d_intra_AB;
    double d_intra_BB;
    double k_intra_AB;
    double k_intra_BB;
private:
    double m_coeffs[ncoeffs];

//private:
//    double f_switch(const double& r, double& g) const;
};

//----------------------------------------------------------------------------//

} // namespace A1B4

////////////////////////////////////////////////////////////////////////////////

#endif // X3B_V2X_H
