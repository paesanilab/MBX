#ifndef POLY_2B_A1B2C4_D1E2_DEG3_H
#define POLY_2B_A1B2C4_D1E2_DEG3_H

namespace x2b_A1B2C4_D1E2_deg3 {

struct poly_2b_A1B2C4_D1E2_deg3_v1x {
    static const unsigned n_vars = 45;
    static const unsigned size = 983;

    static double eval(const double a[983], const double x[45]);

    static double eval(const double a[983], const double x[45], double g[45]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x2b_A1B2C4_D1E2_deg3

#endif  // POLY_2B_A1B2C4_D1E2_DEG3_H