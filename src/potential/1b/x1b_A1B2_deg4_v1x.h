#ifndef X1B_A1B2_DEG4_H
#define X1B_A1B2_DEG4_H

#include "poly_1b_A1B2_deg4_v1x.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////

namespace x1b_A1B2_deg4 {

//----------------------------------------------------------------------------//

struct x1b_A1B2_v1x {
    x1b_A1B2_v1x(){};
    x1b_A1B2_v1x(const std::string mon);
    ~x1b_A1B2_v1x(){};

    typedef poly_1b_A1B2_deg4_v1x polynomial;

    std::vector<double> eval(const double* xyz, const size_t nmon) const;
    std::vector<double> eval(const double* xyz, double* grad, const size_t nmon, std::vector<double>* virial = 0) const;

   private:
    double d_intra_AB;
    double d_intra_BB;
    double k_intra_AB;
    double k_intra_BB;

   private:
    std::vector<double> coefficients;

    // private:
    //    double f_switch(const double& r, double& g) const;
};

//----------------------------------------------------------------------------//

}  // namespace x1b_A1B2_deg4

////////////////////////////////////////////////////////////////////////////////

#endif  // X1B_A1B2_DEG4_H
