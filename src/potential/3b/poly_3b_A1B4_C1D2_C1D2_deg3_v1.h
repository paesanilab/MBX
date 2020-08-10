
#ifndef POLY_3B_MBNRG_A1B4_C1D2_C1D2_DEG3_H
#define POLY_3B_MBNRG_A1B4_C1D2_C1D2_DEG3_H

namespace mbnrg_A1B4_C1D2_C1D2_deg3 {

struct poly_A1B4_C1D2_C1D2_deg3_v1 {
    static const unsigned degree = 3;
    static const unsigned n_vars = 55;

    static const unsigned size = 1254;

    double eval(const double x[55], const double a[1254]);
    double eval_direct(const double x[55], const double a[1254]);
    double eval(const double x[55], const double a[1254], double g[55]);
    double eval_direct(const double x[55], const double a[1254], double g[55]);
};

}  // namespace mbnrg_A1B4_C1D2_C1D2_deg3

#endif  // POLY_3B_MBNRG_A1B4_C1D2_C1D2_DEG3_H
