#ifndef X1b_A1B4_DEG5_EXP0_V1X_H
#define X1b_A1B4_DEG5_EXP0_V1X_H

#include "poly_1b_A1B4_deg5_exp0_v1x.h"

#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

namespace x1b_A1B4_deg5_exp0 {

//----------------------------------------------------------------------------//

struct x1b_A1B4_v1x {
    x1b_A1B4_v1x(){};
    x1b_A1B4_v1x(const std::string mon);

    ~x1b_A1B4_v1x(){};

    typedef poly_1b_A1B4_deg5_exp0_v1x polynomial;

    // returns 1b contribution only
    // XYZ is for the real sites
    std::vector<double> eval(const double* xyz, const size_t nmon) const;
    std::vector<double> eval(const double* xyz, double* grad, const size_t nmon, std::vector<double>* virial = 0) const;

   private:
    double m_k_AB;
    double m_d_AB;
    double m_k_BB;
    double m_d_BB;

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace x1b_A1B4_deg5_exp0

////////////////////////////////////////////////////////////////////////////////

#endif
