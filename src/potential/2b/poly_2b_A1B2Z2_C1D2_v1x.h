#ifndef POLY_2B_A1B2Z2_C1D2_DEG4_H
#define POLY_2B_A1B2Z2_C1D2_DEG4_H

namespace x2b_A1B2Z2_C1D2_deg4 {

struct poly_2b_A1B2Z2_C1D2_deg4_v1x {
    static const unsigned n_vars = 21;
    static const unsigned size = 1653;

    static double eval(const double a[1653], const double x[21]);

    static double eval(const double a[1653], const double x[21], double g[21]);

   public:
    unsigned report_nvars() { return n_vars; };
    unsigned report_size() { return size; };
};

}  // namespace x2b_A1B2Z2_C1D2_deg4

#endif  // POLY_MODEL_H
