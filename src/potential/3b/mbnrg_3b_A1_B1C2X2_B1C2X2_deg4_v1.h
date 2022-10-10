#ifndef MBNRG_3B_A1_B1C2X2_B1C2X2_DEG4_V1_H
#define MBNRG_3B_A1_B1C2X2_B1C2X2_DEG4_V1_H

#include <cmath>
#include <string>
#include <vector>

#include "tools/constants.h"
#include "tools/variable.h"
#include "tools/water_monomer_lp.h"
#include "poly_3b_A1_B1C2X2_B1C2X2_deg4_v1.h"

////////////////////////////////////////////////////////////////////////////////

namespace mbnrg_A1_B1C2X2_B1C2X2_deg4 {

//----------------------------------------------------------------------------//

struct mbnrg_A1_B1C2X2_B1C2X2_deg4_v1 {
    mbnrg_A1_B1C2X2_B1C2X2_deg4_v1(){};
    mbnrg_A1_B1C2X2_B1C2X2_deg4_v1(const std::string mon1, const std::string mon2, const std::string mon3);

    ~mbnrg_A1_B1C2X2_B1C2X2_deg4_v1(){};

    typedef poly_A1_B1C2X2_B1C2X2_deg4_v1 polynomial;

    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3,
                const size_t n, std::vector<double> *virial = 0);

   private:
    double m_k_x_inter_A_B_0;
    double m_k_x_inter_A_C_0;
    double m_k_x_inter_A_X_0;
    double m_k_x_inter_B_B_0;
    double m_k_x_intra_B_C_1;
    double m_k_x_inter_B_C_0;
    double m_k_x_inter_B_X_0;
    double m_k_x_intra_C_C_1;
    double m_k_x_inter_C_C_0;
    double m_k_x_inter_C_X_0;
    double m_k_x_inter_X_X_0;

    double m_ri = 2.5;
    double m_ro = 4.5;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1_B1C2X2_B1C2X2_deg4

namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_oldswitch {

//----------------------------------------------------------------------------//

struct mbnrg_A1_B1C2X2_B1C2X2_deg4_v1 {
    mbnrg_A1_B1C2X2_B1C2X2_deg4_v1(){};
    mbnrg_A1_B1C2X2_B1C2X2_deg4_v1(const std::string mon1, const std::string mon2, const std::string mon3);

    ~mbnrg_A1_B1C2X2_B1C2X2_deg4_v1(){};

    typedef poly_A1_B1C2X2_B1C2X2_deg4_v1 polynomial;

    double eval(const double *xyz1, const double *xyz2, const double *xyz3, const size_t n);
    double eval(const double *xyz1, const double *xyz2, const double *xyz3, double *grad1, double *grad2, double *grad3,
                const size_t n, std::vector<double> *virial = 0);

   private:
    double m_k_x_inter_A_B_0;
    double m_k_x_inter_A_C_0;
    double m_k_x_inter_A_X_0;
    double m_k_x_inter_B_B_0;
    double m_k_x_intra_B_C_1;
    double m_k_x_inter_B_C_0;
    double m_k_x_inter_B_X_0;
    double m_k_x_intra_C_C_1;
    double m_k_x_inter_C_C_0;
    double m_k_x_inter_C_X_0;
    double m_k_x_inter_X_X_0;

    double m_ri = 2.5;
    double m_ro = 4.5;

    double f_switch(const double, double &);

    std::vector<double> coefficients;
};

//----------------------------------------------------------------------------//

}  // namespace mbnrg_A1_B1C2X2_B1C2X2_deg4_oldswitch

////////////////////////////////////////////////////////////////////////////////

#endif
