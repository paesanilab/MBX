#ifndef MBNRG_3B_A1B2_A1B2_A1B2_DEG4_V1_H
#define MBNRG_3B_A1B2_A1B2_A1B2_DEG4_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_3b_A1B2_A1B2_A1B2_deg4_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B2_A1B2_A1B2_deg4 {

//----------------------------------------------------------------------------//

class mbnrg_A1B2_A1B2_A1B2_deg4_v1 {
   public:
    mbnrg_A1B2_A1B2_A1B2_deg4_v1(){};
    mbnrg_A1B2_A1B2_A1B2_deg4_v1(const std::string mon1, const std::string mon2, const std::string mon3);

    ~mbnrg_A1B2_A1B2_A1B2_deg4_v1(){};

    typedef poly_A1B2_A1B2_A1B2_deg4_v1 polynomial;

    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3,
                const size_t n, std::vector<double> *virial = 0);

   private:
    double m_k_x_inter_A_A_0;
    double m_k_x_intra_A_B_1;
    double m_k_x_inter_A_B_0;
    double m_k_x_intra_B_B_1;
    double m_k_x_inter_B_B_0;

    double m_ri = 3.5;
    double m_ro = 4.5;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1B2_A1B2_A1B2_deg4

////////////////////////////////////////////////////////////////////////////////

#endif
