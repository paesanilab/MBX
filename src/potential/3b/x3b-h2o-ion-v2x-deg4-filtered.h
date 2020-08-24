#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include <iostream>
#include "poly-3b-h2o-ion-v1x_deg4_filtered.h"

////////////////////////////////////////////////////////////////////////////////

struct x3b_h2o_ion_v1x_deg4_filtered {
    x3b_h2o_ion_v1x_deg4_filtered(std::string m3);

    static const unsigned ncoeffs = h2o_ion::poly_3b_h2o_ion_v1x_deg4_filtered::size;

    double operator()(const double* xyz1, const double* xyz2, const double* xyz3, size_t nt) const;
    double operator()(const double* xyz1, const double* xyz2, const double* xyz3, double* grad1, double* grad2,
                      double* grad3, size_t nt, std::vector<double>* virial = 0) const;

   public:
    double m_r3i;
    double m_r3f;

   private:
    double m_kOH_intra;
    double m_kHH_intra;

    double m_kOO;
    double m_kOH;
    double m_kHH;

    double m_kXO;
    double m_kXH;

    double m_dOH_intra;
    double m_dHH_intra;

    double m_dOO;
    double m_dOH;
    double m_dHH;

    double m_dXO;
    double m_dXH;

   private:
    double m_coeffs[ncoeffs];

   private:
    double f_switch(const double& r, double& g) const;
};

//----------------------------------------------------------------------------//
