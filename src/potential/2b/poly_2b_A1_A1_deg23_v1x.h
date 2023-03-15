#ifndef POLY_2B_A1_A1_DEG23_V1X_H
#define POLY_2B_A1_A1_DEG23_V1X_H

namespace x2b_A1_A1_deg23 {

struct poly_2b_A1_A1_deg23_v1x {
    static const unsigned n_vars = 1;
    static const unsigned size = 23;

    static double eval(const double a[23],
                       const double x[1]);

    static double eval(const double a[23],
                       const double x[1],
                             double g[1]);

public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace x2b_A1_A1_deg23

#endif // POLY_2B_A1_A1_DEG23_V1X_H
