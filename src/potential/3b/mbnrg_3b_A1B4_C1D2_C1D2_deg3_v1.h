#ifndef MBNRG_3B_A1B4_C1D2_C1D2_DEG3_V1_H
#define MBNRG_3B_A1B4_C1D2_C1D2_DEG3_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_3b_A1B4_C1D2_C1D2_deg3_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B4_C1D2_C1D2_deg3 {

//----------------------------------------------------------------------------//

struct mbnrg_A1B4_C1D2_C1D2_deg3_v1 {
    mbnrg_A1B4_C1D2_C1D2_deg3_v1(){};
    mbnrg_A1B4_C1D2_C1D2_deg3_v1(const std::string mon1, const std::string mon2, const std::string mon3);

    ~mbnrg_A1B4_C1D2_C1D2_deg3_v1(){};

    typedef poly_A1B4_C1D2_C1D2_deg3_v1 polynomial;

    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3,
                const size_t n, std::vector<double> *virial = 0);

   private:
    double m_k_x_intra_A_B_1;
    double m_k_x_inter_A_C_0;
    double m_k_x_inter_A_D_0;
    double m_k_x_intra_B_B_1;
    double m_k_x_inter_B_C_0;
    double m_k_x_inter_B_D_0;
    double m_k_x_inter_C_C_0;
    double m_k_x_intra_C_D_1;
    double m_k_x_inter_C_D_0;
    double m_k_x_intra_D_D_1;
    double m_k_x_inter_D_D_0;

    double m_ri = 7.0;
    double m_ro = 8.0;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1B4_C1D2_C1D2_deg3

////////////////////////////////////////////////////////////////////////////////

#endif
