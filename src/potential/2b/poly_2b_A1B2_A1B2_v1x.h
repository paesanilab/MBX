#ifndef POLY_2B_A1B2_A1B2_DEG5_H
#define POLY_2B_A1B2_A1B2_DEG5_H

namespace x2b_A1B2_A1B2_deg5 {

struct poly_2b_A1B2_A1B2_deg5_v1x {
    static const unsigned n_vars = 15;
    static const unsigned size = 2269;

    static double eval(const double a[2269], const double x[15]);

    static double eval(const double a[2269], const double x[15], double g[15]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x2b_A1B2_A1B2_deg5

#endif  // POLY_MODEL_H
