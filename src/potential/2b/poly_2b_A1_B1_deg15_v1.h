
#ifndef POLY_2B_MBNRG_A1_B1_DEG15_H
#define POLY_2B_MBNRG_A1_B1_DEG15_H

namespace mbnrg_A1_B1_deg15 {

struct poly_A1_B1_deg15_v1 {
    static const unsigned degree = 15;
    static const unsigned n_vars = 1;

    static const unsigned size = 15;

    double eval(const double x[1], const double a[15]);
    double eval_direct(const double x[1], const double a[15]);
    double eval(const double x[1], const double a[15], double g[1]);
    double eval_direct(const double x[1], const double a[15], double g[1]);
};

}  // namespace mbnrg_A1_B1_deg15

#endif  // POLY_2B_MBNRG_A1_B1_DEG15_H
