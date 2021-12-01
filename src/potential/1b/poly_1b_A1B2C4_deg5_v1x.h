#ifndef POLY_1B_A1B2C4_DEG5_H
#define POLY_1B_A1B2C4_DEG5_H

namespace x1b_A1B2C4_deg5 {

struct poly_1b_A1B2C4_deg5_v1x {
    static const unsigned n_vars = 21;
    static const unsigned size = 2624;

    static double eval(const double a[2624], const double x[21]);

    static double eval(const double a[2624], const double x[21], double g[21]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x1b_A1B2C4_deg5

#endif  // POLY_1B_A1B2C4_DEG5_H