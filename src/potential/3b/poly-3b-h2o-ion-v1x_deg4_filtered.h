#ifndef POLY_3B_H2O_ION_V1X_DEG4_FILTERED_H
#define POLY_3B_H2O_ION_V1X_DEG4_FILTERED_H

namespace h2o_ion {

//
// this is the polynomial used by x3b_h2o_ion_v1x (including gradients)
//

struct poly_3b_h2o_ion_v1x_deg4_filtered {
    static const unsigned n_vars = 21;
    static const unsigned size = 1016;

    static double eval(const double a[1016], const double x[21]);

    static double eval(const double a[1016], const double x[21], double g[21]);
};

}  // namespace h2o_ion

#endif  // POLY_3B_H2O_ION_V1X_H
