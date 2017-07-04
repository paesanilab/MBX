#ifndef POLY_2B_A1B2_A1B2_V1X_DEG3_H
#define POLY_2B_A1B2_A1B2_V1X_DEG3_H

namespace mb_system {

struct poly_model {
    static const unsigned n_vars = 15;
    static const unsigned size = 134;

    static double eval(const double a[134],
                       const double x[15]);

    static double eval(const double a[134],
                       const double x[15],
                             double g[15]);

    static double eval_direct(const double a[134],
                              const double x[15]);

public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace mb_system

#endif // POLY_2B_A1B2_A1B2_V1X_DEG3_H
