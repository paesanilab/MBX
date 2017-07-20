#ifndef POLY_1B_A1B4_DEG3_H
#define POLY_1B_A1B4_DEG3_H

namespace A1B4 {

struct poly_1b_v1x {
    static const unsigned n_vars = 10;
    static const unsigned size = 29;

    static double eval(const double a[29],
                       const double x[10]);

    static double eval(const double a[29],
                       const double x[10],
                             double g[10]);

    static double eval_direct(const double a[29],
                              const double x[10]);

public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace A1B4

#endif // POLY_1B_A1B4_DEG3_H
