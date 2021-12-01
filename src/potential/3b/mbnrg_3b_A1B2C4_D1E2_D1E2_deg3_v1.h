#ifndef MBNRG_3B_A1B2C4_D1E2_D1E2_DEG3_V1_H
#define MBNRG_3B_A1B2C4_D1E2_D1E2_DEG3_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_3b_A1B2C4_D1E2_D1E2_deg3_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B2C4_D1E2_D1E2_deg3 {

//----------------------------------------------------------------------------//

struct mbnrg_A1B2C4_D1E2_D1E2_deg3_v1 {
    mbnrg_A1B2C4_D1E2_D1E2_deg3_v1(){};
    mbnrg_A1B2C4_D1E2_D1E2_deg3_v1(const std::string mon1, const std::string mon2, const std::string mon3);

    ~mbnrg_A1B2C4_D1E2_D1E2_deg3_v1(){};

    typedef poly_A1B2C4_D1E2_D1E2_deg3_v1 polynomial;

    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3,
                const size_t n, std::vector<double> *virial = 0);

   private:
    double m_k_x_intra_A_B_1;
    double m_k_x_intra_A_C_1;
    double m_k_x_inter_A_D_0;
    double m_k_x_inter_A_E_0;
    double m_k_x_intra_B_B_1;
    double m_k_x_intra_B_C_1;
    double m_k_x_inter_B_D_0;
    double m_k_x_inter_B_E_0;
    double m_k_x_intra_C_C_1;
    double m_k_x_inter_C_D_0;
    double m_k_x_inter_C_E_0;
    double m_k_x_inter_D_D_0;
    double m_k_x_intra_D_E_1;
    double m_k_x_inter_D_E_0;
    double m_k_x_intra_E_E_1;
    double m_k_x_inter_E_E_0;

    double m_ri = 4.0;
    double m_ro = 5.0;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1B2C4_D1E2_D1E2_deg3

////////////////////////////////////////////////////////////////////////////////

#endif
