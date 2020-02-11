
#ifndef POLY_1B_MBNRG_A1B3_DEG6_H
#define POLY_1B_MBNRG_A1B3_DEG6_H

namespace mbnrg_A1B3_deg6 {

struct poly_A1B3_deg6_v1 {
    static const unsigned degree = 6;
    static const unsigned n_vars = 6;

    static const unsigned size = 195;

    double eval(const double x[6], 
              const double a[195]);
    double eval_direct(const double x[6], 
                     const double a[195]);
    double eval(const double x[6], 
              const double a[195],
                    double g[6]);
    double eval_direct(const double x[6], 
                     const double a[195],
                           double g[6]);
};

} // namespace mbnrg_A1B3_deg6

#endif // POLY_1B_MBNRG_A1B3_DEG6_H

