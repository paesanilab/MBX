#ifndef POLY_1B_A1B2_DEG4_H
#define POLY_1B_A1B2_DEG4_H

namespace A1B2 {

struct poly_1b_v1x {
    static const unsigned n_vars = 3;
    static const unsigned size = 21;

    static double eval(const double a[21],
                       const double x[3]);

    static double eval(const double a[21],
                       const double x[3],
                             double g[3]);

    static double eval_direct(const double a[21],
                              const double x[3]);

public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace A1B2

#endif // POLY_1B_A1B4_DEG3_H
