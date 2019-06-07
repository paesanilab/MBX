#ifndef X1B_A1B4_DEG5_V1X_H
#define X1B_A1B4_DEG5_V1X_H
 
#include "poly_1b_A1B4_deg5_v1x.h" 

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////

namespace x1b_A1B4_deg5 {

//----------------------------------------------------------------------------//

struct x1b_A1B4_v1x { 

    x1b_A1B4_v1x() {};
    x1b_A1B4_v1x(const std::string mon);

    ~x1b_A1B4_v1x() {};

    typedef poly_1b_A1B4_deg5_v1x polynomial;

    // returns 1b contribution only
    // xyz will be the coordinates of the nmon monomers, one after the other one

    std::vector<double> eval(const double* xyz, const size_t nmon) const;
    std::vector<double> eval(const double* xyz, double* grad, const size_t nmon) const;
    
private:

    double m_k_BB;
    double m_k_AB;


    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

} // namespace x1b_A1B4_deg5

////////////////////////////////////////////////////////////////////////////////

#endif 
