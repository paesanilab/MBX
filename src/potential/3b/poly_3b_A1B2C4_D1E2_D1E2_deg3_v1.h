
#ifndef POLY_3B_MBNRG_A1B2C4_D1E2_D1E2_DEG3_H
#define POLY_3B_MBNRG_A1B2C4_D1E2_D1E2_DEG3_H

namespace mbnrg_A1B2C4_D1E2_D1E2_deg3 {

struct poly_A1B2C4_D1E2_D1E2_deg3_v1 {
    static const unsigned degree = 3;
    static const unsigned n_vars = 78;

    static const unsigned size = 2038;

    double eval(const double x[78], const double a[2038]);
    double eval_direct(const double x[78], const double a[2038]);
    double eval(const double x[78], const double a[2038], double g[78]);
    double eval_direct(const double x[78], const double a[2038], double g[78]);
};

}  // namespace mbnrg_A1B2C4_D1E2_D1E2_deg3

#endif  // POLY_3B_MBNRG_A1B2C4_D1E2_D1E2_DEG3_H
