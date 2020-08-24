#ifndef POLY_1B_A1B2_H
#define POLY_1B_A1B2_H

namespace x1b_A1B2_deg4 {

struct poly_1b_A1B2_deg4_v1x {
    static const unsigned n_vars = 3;
    static const unsigned size = 21;

    static double eval(const double a[21], const double x[3], double g[3]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x1b_A1B2_deg4

#endif  // POLY_1B_V1X_H
