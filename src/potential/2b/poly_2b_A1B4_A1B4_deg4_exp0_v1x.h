#ifndef POLY_2B_A1B4_A1B4_DEG4_EXP0_H
#define POLY_2B_A1B4_A1B4_DEG4_EXP0_H

namespace x2b_A1B4_A1B4_deg4_exp0 {

struct poly_2b_A1B4_A1B4_deg4_exp0_v1x {
    static const unsigned n_vars = 45;
    static const unsigned size = 1345;

    static double eval(const double a[1345], const double x[45]);

    static double eval(const double a[1345], const double x[45], double g[45]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x2b_A1B4_A1B4_deg4_exp0

#endif  // POLY_MODEL_H
