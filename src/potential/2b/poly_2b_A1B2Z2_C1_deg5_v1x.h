#ifndef POLY_2B_A1B2Z2_C1_DEG5_V1X_H
#define POLY_2B_A1B2Z2_C1_DEG5_V1X_H

namespace x2b_A1B2Z2_C1_deg5 {

struct poly_2b_A1B2Z2_C1_deg5_v1x {
    static const unsigned n_vars = 8;
    static const unsigned size = 496;

    static double eval(const double a[496],
                       const double x[8]);

    static double eval(const double a[496],
                       const double x[8],
                             double g[8]);

   // static double eval_direct(const double a[496],
   //                           const double x[8]);

public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace x2b_A1B2Z2_C1_deg5

#endif // POLY_2B_A1B2Z2_C1_DEG5_V1X_H
