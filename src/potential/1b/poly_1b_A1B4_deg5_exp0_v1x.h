#ifndef POLY_1B_A1B4_DEG5_EXP0_H
#define POLY_1B_A1B4_DEG5_EXP0_H

namespace x1b_A1B4_deg5_exp0 {

struct poly_1b_A1B4_deg5_exp0_v1x {
    static const unsigned n_vars = 10;
    static const unsigned size = 207;

    static double eval(const double a[207], const double x[10]);

    static double eval(const double a[207], const double x[10], double g[10]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x1b_A1B4_deg5_exp0

#endif  // POLY_1B_A1B4_DEG5_EXP0_H
