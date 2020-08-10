#ifndef POLY_2B_A1B2Z2_C1D4_DEG3_EXP0_H
#define POLY_2B_A1B2Z2_C1D4_DEG3_EXP0_H

namespace x2b_A1B2Z2_C1D4_deg3_exp0 {

struct poly_2b_A1B2Z2_C1D4_deg3_exp0_v1x {
    static const unsigned n_vars = 38;
    static const unsigned size = 603;

    static double eval(const double a[603], const double x[38]);

    static double eval(const double a[603], const double x[38], double g[38]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x2b_A1B2Z2_C1D4_deg3_exp0

#endif  // POLY_MODEL_H
