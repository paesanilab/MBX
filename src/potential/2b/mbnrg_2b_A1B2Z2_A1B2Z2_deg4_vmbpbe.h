#ifndef MBNRG_2B_A1B2Z2_A1B2Z2_DEG4_VMBPBE_H
#define MBNRG_2B_A1B2Z2_A1B2Z2_DEG4_VMBPBE_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_2b_A1B2Z2_A1B2Z2_deg4_vmbpbe.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1B2Z2_A1B2Z2_deg4 {

//----------------------------------------------------------------------------//

struct mbnrg_A1B2Z2_A1B2Z2_deg4_vmbpbe {
    mbnrg_A1B2Z2_A1B2Z2_deg4_vmbpbe(){};
    mbnrg_A1B2Z2_A1B2Z2_deg4_vmbpbe(const std::string mon1, const std::string mon2);

    ~mbnrg_A1B2Z2_A1B2Z2_deg4_vmbpbe(){};

    typedef poly_A1B2Z2_A1B2Z2_deg4_vmbpbe polynomial;

    double eval(const double *xyz1, const double *xyz2, const size_t n);
    double eval(const double *xyz1, const double *xyz2, double *grad1, double *grad2, const size_t n,
                std::vector<double> *virial = 0);

   private:
    double m_k_x_inter_A_A_0;
    double m_k_x_intra_A_B_1;
    double m_k_x_inter_A_B_0;
    double m_k_x_inter_A_Z_0;
    double m_k_x_intra_B_B_1;
    double m_k_x_inter_B_B_0;
    double m_k_x_inter_B_Z_0;
    double m_k_x_inter_Z_Z_0;

    double m_ri = 7.0;
    double m_ro = 8.0;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1B2Z2_A1B2Z2_deg4

////////////////////////////////////////////////////////////////////////////////

#endif
