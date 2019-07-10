#ifndef POLY_2B_A1B6_A1B6_DEG3_H
#define POLY_2B_A1B6_A1B6_DEG3_H

namespace x2b_A1B6_A1B6_deg3 {

struct poly_2b_A1B6_A1B6_deg3_v1x {
    static const unsigned n_vars = 91;
    static const unsigned size = 248;

    static double eval(const double a[248],
                       const double x[91]);

    static double eval(const double a[248],
                       const double x[91],
                             double g[91]);


public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace x2b_A1B6_A1B6_deg3

#endif // POLY_2B_A1B6_A1B6_DEG3_H