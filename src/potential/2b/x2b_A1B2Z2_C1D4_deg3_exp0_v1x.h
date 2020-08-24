#ifndef X2B_A1B2Z2_C1D4_DEG3_EXP0_V1X_H
#define X2B_A1B2Z2_C1D4_DEG3_EXP0_V1X_H

#include "poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B2Z2_C1D4_deg3_exp0 {

//----------------------------------------------------------------------------//

struct x2b_A1B2Z2_C1D4_v1x {
    typedef poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x polynomial;

    x2b_A1B2Z2_C1D4_v1x(){};
    x2b_A1B2Z2_C1D4_v1x(const std::string mon1, const std::string mon2);

    ~x2b_A1B2Z2_C1D4_v1x(){};

    double eval(const double* xyz1, const double* xyz2, const size_t ndim) const;
    double eval(const double* xyz1, const double* xyz2, double* grad1, double* grad2, const size_t ndim,
                std::vector<double>* virial = 0) const;

   private:
    double m_k_intra_AB;
    double m_d_intra_AB;
    double m_k_intra_BB;
    double m_d_intra_BB;
    double m_k_intra_CD;
    double m_d_intra_CD;
    double m_k_intra_DD;
    double m_d_intra_DD;
    double m_k_AC;
    double m_d_AC;
    double m_k_AD;
    double m_d_AD;
    double m_k_BC;
    double m_d_BC;
    double m_k_BD;
    double m_d_BD;
    double m_k_CZ;
    double m_d_CZ;
    double m_k_DZ;
    double m_d_DZ;

   protected:
    double m_r2i = 8.0;
    double m_r2f = 9.0;

    double f_switch(const double&, double&) const;  // At1_a -- At1_b separation

   private:
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace x2b_A1B2Z2_C1D4_deg3_exp0

////////////////////////////////////////////////////////////////////////////////

#endif
