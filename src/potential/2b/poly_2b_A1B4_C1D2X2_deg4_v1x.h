#ifndef POLY_2B_A1B4_C1D2X2_DEG4_H
#define POLY_2B_A1B4_C1D2X2_DEG4_H

namespace x2b_A1B4_C1D2X2_deg4 {

struct poly_2b_A1B4_C1D2X2_deg4_v1x {
    static const unsigned n_vars = 45;
    static const unsigned size = 979;

    static double eval(const double a[979],
                       const double x[45]);

    static double eval(const double a[979],
                       const double x[45],
                             double g[45]);


public:
    unsigned report_nvars(){ return n_vars; };
    unsigned report_size(){ return size; };
};

} // namespace x2b_A1B4_C1D2X2_deg4

#endif // POLY_2B_A1B4_C1D2X2_DEG4_H