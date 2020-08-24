#ifndef X2B_A1B2_A1B2_V1X_H
#define X2B_A1B2_A1B2_V1X_H

#include "poly_2b_A1B2_A1B2_v1x.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace x2b_A1B2_A1B2_deg5 {

//----------------------------------------------------------------------------//

struct x2b_A1B2_A1B2_v1x {
    x2b_A1B2_A1B2_v1x(){};
    x2b_A1B2_A1B2_v1x(const std::string mon1, const std::string mon2);
    ~x2b_A1B2_A1B2_v1x(){};

    typedef poly_2b_A1B2_A1B2_deg5_v1x polynomial;

    double eval(const double* xyz1, const double* xyz2, const size_t ndim) const;
    double eval(const double* xyz1, const double* xyz2, double* grad1, double* grad2, const size_t ndim,
                std::vector<double>* virial = 0) const;

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
    double m_r2i = 8.0;
    double m_r2f = 9.0;

    double f_switch(const double&, double&) const;  // At1_a -- At1_b separation

   private:
    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace x2b_A1B2_A1B2_deg5

////////////////////////////////////////////////////////////////////////////////

#endif
