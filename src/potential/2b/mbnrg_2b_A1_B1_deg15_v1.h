#ifndef MBNRG_2B_A1_B1_DEG15_V1_H
#define MBNRG_2B_A1_B1_DEG15_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_2b_A1_B1_deg15_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1_B1_deg15 {

//----------------------------------------------------------------------------//

struct mbnrg_A1_B1_deg15_v1 {
    mbnrg_A1_B1_deg15_v1(){};
    mbnrg_A1_B1_deg15_v1(const std::string mon1, const std::string mon2);

    ~mbnrg_A1_B1_deg15_v1(){};

    typedef poly_A1_B1_deg15_v1 polynomial;

    double eval(const double *xyz1, const double *xyz2, const size_t n);
    double eval(const double *xyz1, const double *xyz2, double *grad1, double *grad2, const size_t n,
                std::vector<double> *virial = 0);

   private:
    double m_k_x_inter_A_B_0;
    double m_d_x_inter_A_B_0;

    double m_ri = 7.0;
    double m_ro = 8.0;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1_B1_deg15

////////////////////////////////////////////////////////////////////////////////

#endif
