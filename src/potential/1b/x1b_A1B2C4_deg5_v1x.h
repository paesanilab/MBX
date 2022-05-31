#ifndef X1B_A1B2C4_DEG5_V1X_H
#define X1B_A1B2C4_DEG5_V1X_H

#include "poly_1b_A1B2C4_deg5_v1x.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////

namespace x1b_A1B2C4_deg5 {

//----------------------------------------------------------------------------//

struct x1b_A1B2C4_v1x {
    x1b_A1B2C4_v1x(){};
    x1b_A1B2C4_v1x(const std::string mon);

    ~x1b_A1B2C4_v1x(){};

    typedef poly_1b_A1B2C4_deg5_v1x polynomial;

    // returns 1b contribution only
    // xyz will be the coordinates of the nmon monomers, one after the other one

    std::vector<double> eval(const double* xyz, const size_t nmon) const;
    std::vector<double> eval(const double* xyz, double* grad, const size_t nmon, std::vector<double>* virial = 0) const;

   private:
    double m_k_BB;
    double m_k_CC;
    double m_k_AB;
    double m_k_AC;
    double m_k_BC;

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace x1b_A1B2C4_deg5

////////////////////////////////////////////////////////////////////////////////

#endif
