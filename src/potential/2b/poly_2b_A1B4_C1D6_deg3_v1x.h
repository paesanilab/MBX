#ifndef POLY_2B_A1B4_C1D6_DEG3_H
#define POLY_2B_A1B4_C1D6_DEG3_H

namespace x2b_A1B4_C1D6_deg3 {

struct poly_2b_A1B4_C1D6_deg3_v1x {
    static const unsigned n_vars = 66;
    static const unsigned size = 465;

    static double eval(const double a[465],
                       const double x[66]);

    static double eval(const double a[465],
                       const double x[66],
                             double g[66]);


public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace x2b_A1B4_C1D6_deg3

#endif // POLY_2B_A1B4_C1D6_DEG3_H