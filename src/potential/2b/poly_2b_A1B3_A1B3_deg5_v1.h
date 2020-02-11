
#ifndef POLY_2B_MBNRG_A1B3_A1B3_DEG5_H
#define POLY_2B_MBNRG_A1B3_A1B3_DEG5_H

namespace mbnrg_A1B3_A1B3_deg5 {

struct poly_A1B3_A1B3_deg5_v1 {
    static const unsigned degree = 5;
    static const unsigned n_vars = 28;

    static const unsigned size = 1967;

    double eval(const double x[28],
              const double a[1967]);
    double eval_direct(const double x[28],
                     const double a[1967]);
    double eval(const double x[28],
              const double a[1967],
                    double g[28]);
    double eval_direct(const double x[28],
                     const double a[1967],
                           double g[28]);
};

} // namespace mbnrg_A1B3_A1B3_deg5

#endif // POLY_2B_MBNRG_A1B3_A1B3_DEG5_H

